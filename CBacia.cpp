#include "stdafx.h"
#include <set>
#include "CPonto.h"
#include <list>
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include <map>
#include "CAcidentesTopograficos.h"
#include "cdeltasuperficie.h"
#include "CSegmento.h"
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CLPontos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include <sstream>
#include "CEquipDrenagem.h"
#include "Superficie.h"
#include "Mat.h"
#include "Monologo.h"

#define LADO_DIREITO 1
#define LADO_ESQUERDO 0

extern Matem Mat;
extern monologo monolog;

CBacia::CBacia(const CAciTopografico& EspigaoSuperf) : Erro(BACIA_OK), TotalSaidas(0),pCalculada(false),ExtTotalCN(INFINITO)
{
  CentroGravidade.x = CentroGravidade.y = CentroGravidade.z = 0.0;

  ItTalveguePrincipal = Talvegues.end();

  if (EspigaoSuperf.PegaTipo() == CAciTopografico::ESPIGAO)
  {
    lstitsetPontos LstPontos(EspigaoSuperf.PegaLista());

    if (**LstPontos.begin() == **LstPontos.rbegin())
    {
      *this = EspigaoSuperf;

      return;
    }

    Erro = BACIA_ABERTA;

    return;
  }

  Erro = FALTA_ESPIGAO;
}

CBacia::CBacia(const LDeltaSup& LstDSEspigaoBacia) : Erro(0), TotalSaidas(0),pCalculada(false),EquidisCNHistog(5),ExtTotalCN(INFINITO)
{
  CentroGravidade.x = CentroGravidade.y = CentroGravidade.z = 0.0;

  ItTalveguePrincipal = Talvegues.end();

  if (LstDSEspigaoBacia.begin()->TipoObjeto == CAciTopografico::ESPIGAO)
  {
    if (*LstDSEspigaoBacia.begin() == *LstDSEspigaoBacia.rbegin())
    {
      for (ItCLDeltaSup It = LstDSEspigaoBacia.begin(); It != LstDSEspigaoBacia.end(); ++It)
      {
        EspigaoBacia.InserePonto(It->PegaPCentral());
      }

      //--- o Ultimo ponto não foi inserido porque é igual ao primeiro
      //--- soma 1e-5 em x para diferenciar e poder inserir

      Ponto PUltimo((*LstDSEspigaoBacia.rbegin()).PCentral);

      PUltimo.x += 1e-5;
      PUltimo.y += 1e-5;

      EspigaoBacia.InserePonto(PUltimo);
      EspigaoBacia.Fechada = true;
      EspigaoBacia.CalculaAreaPerimetro();

      return;
    }
    Erro = CAciTopografico::BACIA_ABERTA;

    return;
  }
  
  Erro = FALTA_ESPIGAO;
}
int CBacia::Calcular(void* pSuperf,CString& StrObservacoes,CDialogEx* Dialogo)
{
  StrObservacoes = "Preparando os cálculos";
  Dialogo->UpdateData(false);

  LimpaIntersEspigao();
  LimpaIntersCN(pSuperf);

  CSuperficie* pSuperficie((CSuperficie*)pSuperf);

  Erro = BACIA_OK;

  StrObservacoes = "Verificando a bacia...";
  Dialogo->UpdateData(false);

  if (MontaBacia(pSuperficie))
  {
    int TotalSaidas(VerificaSaidas(pSuperficie));

    if (TotalSaidas == 1)
    {
      if (DefineTalveguePrincipal())
      {
        double EquidisCN(((CSuperficie*)pSuperf)->PegaEquidisCN());

        if (EquidisCN != INFINITO)
        {
          if (VerificaCnsCortandoTalveguePrincipal(pSuperficie))
          {
            DadosBacia.Kc = 0.28 * EspigaoBacia.Perimetro / sqrt(fabs(EspigaoBacia.Area));
            CalculaComprimentoDiagonal();
            DadosBacia.Kf = fabs(EspigaoBacia.Area) / (DadosBacia.Diagonal * DadosBacia.Diagonal);  //--- Verificar kf e F
			//---  Projeto de caragua usa esta formula para kf  --- ??? 
            // DadosBacia.Kf = DadosBacia.Comprimento / (2.0 * sqrt((fabs(Espigao.Area / 3.1415926))));
            CalculaComprimentoTalvegues();
            DadosBacia.Dd = DadosBacia.Lt / fabs(EspigaoBacia.Area) * 1000.0;
            DadosBacia.l = fabs(EspigaoBacia.Area) / (4.0 * DadosBacia.Diagonal);
            DadosBacia.Area = fabs(EspigaoBacia.Area);

            StrObservacoes = "Verificando os talvegues...";
            Dialogo->UpdateData(false);

            PerfilTalvPrincipal(pSuperficie);
            if (CalciMedioTalvPrincipal() == false)
            {
              Erro = TALVEGUE_PRINC_ONDULADO;
              return false;
            }

            StrObservacoes = "Calculando a curva hipsométrica";
            Dialogo->UpdateData(false);

            if (CalculaCurvaHipsometrica(pSuperficie, DadosBacia.L))
            {
              Erro = ERRO_CN_ESPIGAO;
              return false;
            }

            DadosBacia.ic = 28.4495 * pow((DadosBacia.t + 40) - 0, 07564) + 17, 2878 * (pow((DadosBacia.t + 70.0), -0.8236)) * (-0.47 - 0.8637 * (log(log(DadosBacia.T / DadosBacia.T - 1.0))));
            DadosBacia.C1 = 4.0 / (2.0 + DadosBacia.Kf);
            DadosBacia.C = 2.0 / (1.0 + DadosBacia.Kf) * DadosBacia.C2 / DadosBacia.C1;
            DadosBacia.RH = DadosBacia.Area / EspigaoBacia.Perimetro;
            DadosBacia.I = DadosBacia.L * EquidisCN / fabs(EspigaoBacia.Area);

            pCalculada = true;

            StrObservacoes = "Os cálculos foram executados com êxito";

            return true;
          }
        }
        else
        {
          monolog.mensagem(-1,"Erro->Equidistancias das CNS não encontrada.");
        }
      }
    }
    else
    {
      if (TotalSaidas == 0)
      {
        monolog.mensagem(-1, "Esta bacia não tem saída (foz). Toda bacia deve ter umae somente uma foz.");
      }
      else
      {
        monolog.mensagem(-1, "Esta bacia possui mais de uma saída (foz). Toda bacia deve ter uma e somente uma foz.");
      }
    }
  }

  StrObservacoes = "Houve erro no cálculo da bacia, verifique o arquivo de log";

  return TotalSaidas;
}

int CBacia::MontaBacia(void* pSuperficie)
{
  CSuperficie* pSuperficieAtual((CSuperficie*) pSuperficie);

  //--- Verifica se o talvegue faz parte desta bacia.
  //--- Ele faz se possuir pelo menos um ponto dentro dela

  LLDeltaSup& ListaRestAtual(pSuperficieAtual->PegaListaRestricoes());

  pSuperficieAtual->ItTalvFozSIG = ListaRestAtual.end();

  Talvegues.clear();

  for (ItLLDeltaSup LDRestricaoAtual = ListaRestAtual.begin(); LDRestricaoAtual != ListaRestAtual.end(); ++LDRestricaoAtual)
  {
    unsigned int TipoRestricao(pSuperficieAtual->VerfAciTopografico(LDRestricaoAtual));
    int ContaTotalSaidas(0);

    if (TipoRestricao == CAciTopografico::TALVEGUE)
    {
      int Resp (VerfTalvegue(LDRestricaoAtual));

      if (Resp == CAciTopografico::TALVEGUE_DENTRO_BACIA || Resp == CAciTopografico::TALVEGUE_CORTA_BACIA ||
          Resp == CAciTopografico::PONTO_FOZ_NO_ESPIGAO)
      {
        Talvegues.push_back(*LDRestricaoAtual);

        if(Resp == CAciTopografico::TALVEGUE_CORTA_BACIA)
        {
          pSuperficieAtual->ItTalvFozSIG = LDRestricaoAtual;
        }
        else
        {
          if (Resp == CAciTopografico::PONTO_FOZ_NO_ESPIGAO)
          {
            pSuperficieAtual->ItTalvFozSIG = LDRestricaoAtual;
          }
        }
      }
    }
  }

  return 1;
}

int CBacia::VerificaSaidas(void* voidpSuperficieAtual)
{
  CSuperficie* pSuperficieAtual((CSuperficie*) voidpSuperficieAtual);

 //--- Verifica se a bacia tem apenas 1 saida de talvegue

  TotalSaidas = 0;
  Ponto FozAnterior;

  citlstitsetPontos ItEspigaoPostFoz(EspigaoBacia.PegaLista().end());   //--- Para salvar a foz no espigão

  for (ItLLDeltaSup ItTalvegueAtual = Talvegues.begin(); ItTalvegueAtual != Talvegues.end(); ++ItTalvegueAtual)
  {
    unsigned int TipoRestricao(pSuperficieAtual->VerfAciTopografico(ItTalvegueAtual));
    int ContaTotalSaidas(0);

    if (TipoRestricao == CAciTopografico::TALVEGUE)
    {
      //--- Verifica se o talvegue tem 1 saida da bacia

      ItLDeltaSup ItPAntTalvegue(ItTalvegueAtual->begin()), ItPAtualTalvegue(ItPAntTalvegue);

      for (++ItPAtualTalvegue; ItPAtualTalvegue != ItTalvegueAtual->end(); ++ItPAntTalvegue, ++ItPAtualTalvegue)
      {
        citlstitsetPontos ItEspigaoAnt(EspigaoBacia.PegaLista().begin()), ItEspigaoAtual(ItEspigaoAnt);
        int ContaSaidas(0);
        Ponto Intersecao;

        for (++ItEspigaoAtual; ItEspigaoAtual != EspigaoBacia.PegaLista().end(); ++ItEspigaoAtual, ++ItEspigaoAnt)
        {
          if (CLPontos::CalculaIntersecao(*(*ItEspigaoAnt), *(*ItEspigaoAtual), ItPAntTalvegue->PegaPCentral(), ItPAtualTalvegue->PegaPCentral(), Intersecao,true,1E-1) == TRUE)
          {
            //--- Na definição da bacia a foz é inserida no espigão e o talvegue da foz é quebrado em 2.
            //--- Caso se recalcule esta bacia por uma alteração nos talvegues por exemplo isso pode provocar
            //--- que interseções ligeiramente diferentes sejam calculadas o que causaria mais que 1 saida (foz)
            //--- na bacia. Porisso este if

            if (Mat.R_P(FozAnterior, Intersecao).y > 1.0)   
            {
              ++ContaSaidas;

              Foz = Intersecao;

              //--- Salva o xyz do ponto anterior no talvegue para posicionar a foz no perfil

              Foz.fi = ItPAntTalvegue->PegaPCentral().x;
              Foz.S = ItPAntTalvegue->PegaPCentral().y;

              double MenorCota((*ItEspigaoAnt)->z < (*ItEspigaoAtual)->z ? (*ItEspigaoAnt)->z : (*ItEspigaoAtual)->z);

              if (ItPAntTalvegue->PegaPCentral().z < MenorCota) MenorCota = ItPAntTalvegue->PegaPCentral().z;
              if (ItPAtualTalvegue->PegaPCentral().z < MenorCota) MenorCota = ItPAntTalvegue->PegaPCentral().z;

              Foz.i = Foz.z = MenorCota;;

              Foz.Descricao = "Foz";
              Foz.Baliza2 |= FOZ;
              ItEspigaoPostFoz = ItEspigaoAtual;

              ItTalveguePrincipal = ItTalvegueAtual;

              FozAnterior = Foz;
            }
          }
        }

        TotalSaidas += ContaSaidas;
      }
    }
  }

  if (TotalSaidas > 1)
  {
    Erro = MUITAS_SAIDAS;
  }

  if (TotalSaidas == 0)
  {
    Erro = SEM_SAIDA;
  }
  
  return TotalSaidas;
}

int CBacia::VerfTalvegue(ItLLDeltaSup AciTopog)
{
  int ContaCorta(0);
  if (!EspigaoBacia.Fechada) return BACIA_ABERTA;  //AQUI=====================

  bool PontoDentro(false), PontoFora(false),PontoIgual(false);
  double AreaBacia(EspigaoBacia.PegaArea());

  for (ItLDeltaSup ItTalvegueAtual = AciTopog->begin() ; ItTalvegueAtual != AciTopog->end() ; ++ItTalvegueAtual)
  {
    citlstitsetPontos itPontoAnt(EspigaoBacia.PegaLista().begin()), itPontoAtual(itPontoAnt);

    //--- É necessário somar estes 1.234E-3 porque parece que o QGIS define os pontos entre
    //--- faixas de x e de y (Talvez para evitar pontos iguais, que são um problema...)
    //--- Mas na suavização dos talvegues os pontos são definidos na média dos 2 pontos da reta
    //--- o que provoca pontos iguais no talude e no espigao. No VU&E quando se tem um ponto
    //--- no espigao igual a um ponto do talvegue causa muitos erros principalmente neste metodo
    //--- Porisso é preciso adicionar este pequeno valor, para evitar essas igualdades.

    const Ponto PontoAtual(ItTalvegueAtual->PegaPCentral().x+1.234E-3,ItTalvegueAtual->PegaPCentral().y+1.234E-3);
    int Contador(0);

    for (++itPontoAtual; itPontoAtual != EspigaoBacia.PegaLista().end(); ++itPontoAtual, ++itPontoAnt)
    {
      if(!(*itPontoAnt)->Compara2D(PontoAtual,1E-4))  //--- Ponto no espigao = ao do talvegue
      {
        if ((AreaBacia < 0.0 && (*itPontoAnt)->x > PontoAtual.x && (*itPontoAtual)->x > PontoAtual.x) ||
            (AreaBacia > 0.0 && (*itPontoAnt)->x < PontoAtual.x && (*itPontoAtual)->x < PontoAtual.x))
        {
          if ((PontoAtual.y > (*itPontoAnt)->y && PontoAtual.y < (*itPontoAtual)->y) || (PontoAtual.y < (*itPontoAnt)->y && PontoAtual.y >(*itPontoAtual)->y))
          {
            ++Contador;
          }
        }
        else
        {
          if (((*itPontoAnt)->x < PontoAtual.x && (*itPontoAtual)->x >PontoAtual.x) || ((*itPontoAnt)->x > PontoAtual.x && (*itPontoAtual)->x < PontoAtual.x))
          {
            if (((*itPontoAnt)->y > PontoAtual.y && (*itPontoAtual)->y < PontoAtual.y) || ((*itPontoAnt)->y < PontoAtual.y && (*itPontoAtual)->y >PontoAtual.y))
            {
              Ponto PontoIntersecao;

              if (CLPontos::CalculaIntersecao(**itPontoAnt, **itPontoAtual, PontoAtual, Ponto(PontoAtual.x + 10000.0, PontoAtual.y), PontoIntersecao, false) == TRUE)
              {
                if ((AreaBacia < 0.0 && PontoIntersecao.x >PontoAtual.x) || (AreaBacia > 0.0 && PontoIntersecao.x < PontoAtual.x))
                {
                  ++Contador;
                }
              }
            }
          }
        }
      }
      else
      {
        PontoIgual = true;

        return CAciTopografico::PONTO_FOZ_NO_ESPIGAO;    //--- Tudo se passa como se....
      }
    }

    if (Contador % 2 == 0) PontoFora = true;
    else PontoDentro = true;
  }

  if (PontoDentro == false)
  {
    if (PontoFora == false) return CAciTopografico::SEM_PONTOS;                    //--- Talvegue ou bacia sem pontos
    else  return CAciTopografico::TALVEGUE_FORA_BACIA;                             //--- Talvegue está completamente fora da bacia
  }
  else
  {
    if (PontoFora == false) 
      return CAciTopografico::TALVEGUE_DENTRO_BACIA; //---  Talvegue esta completamente dentro da bacia
    else 
      return CAciTopografico::TALVEGUE_CORTA_BACIA;                     //---  Talvegue cruza a bacia
  }

  return -1;
}

int CBacia::DefineEspigao(ItCLLDeltaSup LstDSTalvegueBacia)
{
  if (LstDSTalvegueBacia->begin()->TipoObjeto == CAciTopografico::ESPIGAO)
  {
    if (*LstDSTalvegueBacia->begin() == *LstDSTalvegueBacia->rbegin())
    {
      for (ItCLDeltaSup It = LstDSTalvegueBacia->begin(); It != LstDSTalvegueBacia->end(); ++It)
      {
        EspigaoBacia.InserePonto(It->PegaPCentral());
      }

      EspigaoBacia.MudaObjeto(CAciTopografico::TALVEGUE, true);

      TotalSaidas = 0;

      return Erro = BACIA_OK;
    }
    return Erro = BACIA_ABERTA;
  }

  return Erro = FALTA_ESPIGAO;
}

int CBacia::CalculaComprimentoDiagonal()
{
  citlstitsetPontos itMaislonge;
  double ComprimentoAtual(-INFINITO), MaiorComprimento(-INFINITO);

  for (citlstitsetPontos it = EspigaoBacia.PegaLista().begin(); it != EspigaoBacia.PegaLista().end(); ++it)
  {
    ComprimentoAtual = hypot(Foz.x - (*it)->x, Foz.y - (*it)->y);

    if (MaiorComprimento < ComprimentoAtual) MaiorComprimento = ComprimentoAtual;
  }

  DadosBacia.Diagonal = MaiorComprimento;

  return 1;
}

int CBacia::CalculaComprimentoTalvegues()
{
  DadosBacia.Lt = 0.0;

  for (ItLLDeltaSup itTalvegueAtual = Talvegues.begin(); itTalvegueAtual != Talvegues.end(); ++itTalvegueAtual)
  {
    ItLDeltaSup itPAtual(itTalvegueAtual->begin()), itPAnterior(itPAtual);

    for (++itPAtual; itPAtual != itTalvegueAtual->end(); ++itPAtual, ++itPAnterior)
    {
      DadosBacia.Lt += hypot(itPAtual->PCentral.x - itPAnterior->PCentral.x, itPAtual->PCentral.y - itPAnterior->PCentral.y);
    }
  }

  return 1;
}

CArchive& CBacia::operator>>(CArchive& ar)
{
  if (!DadosBacia.NomeBacia.empty() && EspigaoBacia.PegaLista().size() > 1)
  {
    DadosBacia >> ar;
    Foz >> ar;
    EspigaoBacia >> ar;
    CentroGravidade >> ar;

    ar << Erro << TotalSaidas << pCalculada;

    ar << LstPontosBacia.size();

    for (itlstPontos itPonto = LstPontosBacia.begin(); itPonto != LstPontosBacia.end(); ++itPonto)
    {
      *itPonto >> ar;
    }

    ar << Talvegues.size();

    for (ItLLDeltaSup itLTalvegues = Talvegues.begin(); itLTalvegues != Talvegues.end(); ++itLTalvegues)
    {
      ar << itLTalvegues->size();

      for (ItLDeltaSup itTalvegue = itLTalvegues->begin(); itTalvegue != itLTalvegues->end(); ++itTalvegue)
      {
        itTalvegue->PCentral >> ar;
      }
    }

    ar << PerfilTalveguePrincipal.size();

    for (itsetPontos itPerfTalvPrinc = PerfilTalveguePrincipal.begin(); itPerfTalvPrinc != PerfilTalveguePrincipal.end(); ++itPerfTalvPrinc)
    {
      *itPerfTalvPrinc >> ar;
    }

    ar << ParticoesTalveguePrincipal.size();

    for (itsetPontos itPartTalvPrinc = ParticoesTalveguePrincipal.begin(); itPartTalvPrinc != ParticoesTalveguePrincipal.end(); ++itPartTalvPrinc)
    {
      *itPartTalvPrinc >> ar;
    }

    ar << SetCotasAreas.size();

    for (ittysetCotasArea itSetCA = SetCotasAreas.begin(); itSetCA != SetCotasAreas.end(); ++itSetCA)
    {
      ar << itSetCA->Cota << itSetCA->AreaTotal << itSetCA->AreaCN << itSetCA->Extensao << itSetCA->Inclinacao;
    }
  }
  return ar;
}

CArchive& CBacia::operator<<(CArchive& ar)
{
  DadosBacia << ar;
  Foz << ar;
  EspigaoBacia << ar;
  CentroGravidade << ar;

  ar >> Erro >> TotalSaidas >> pCalculada;

  size_t Tam;

  LstPontosBacia.clear();

  Ponto PAtual;

  for (ar >> Tam; Tam > 0; --Tam)
  {
    PAtual >> ar;
    LstPontosBacia.emplace_back(PAtual);
  }

  ItTalveguePrincipal = Talvegues.end();

  Talvegues.clear();

  for (ar >> Tam; Tam > 0; --Tam)
  {
    Talvegues.push_front(LDeltaSup());
    ItLLDeltaSup itTalvegueAtual(Talvegues.begin());

    size_t QuanPontos;
    Ponto PAtual;

    for (ar >> QuanPontos; QuanPontos > 0; --QuanPontos)
    {
      PAtual << ar;
      itTalvegueAtual->emplace_back(PAtual);
    }

    if (itTalvegueAtual->begin()->PCentral.Baliza == true)
    {
      ItTalveguePrincipal = itTalvegueAtual;
    }
  }

  PerfilTalveguePrincipal.clear();

  for (ar >> Tam; Tam; --Tam)
  {
    PAtual << ar;
    PerfilTalveguePrincipal.emplace(PAtual);
  }

  for (ar >> Tam; Tam; --Tam)
  {
    PAtual << ar;
    ParticoesTalveguePrincipal.emplace(PAtual);
  }

  double Area(0.0),AreaCN(0), Cota(0.0),Extensao(0.0),Inclinacao(0.0);
  SetCotasAreas.clear();

  for (ar >> Tam; Tam; --Tam)
  {
    ar >> Cota >> Area >> AreaCN >>  Extensao >> Inclinacao;
    SetCotasAreas.emplace(CCotaArea(Cota, Area,AreaCN,Extensao,Inclinacao));
  }
  return ar;
}

bool CBacia::DefineTalveguePrincipal()
{
  double MaiorExtensao(-INFINITO);
 
  if (ItTalveguePrincipal != Talvegues.end())
  {
    //--- Insere a foz no talvegue

    Ponto PAntFoz(Foz.fi,Foz.S,Foz.i);        //--- Ponto anterior do talvegue onde a esta foz foi encontrada

    Foz.Baliza2 = FOZ;

    //---- Pega a foz (comparador aproximado 1E-5)

    auto itPTalvegueAtual(ItTalveguePrincipal->begin());

    while (itPTalvegueAtual != ItTalveguePrincipal->end() && !itPTalvegueAtual->PCentral.Compara2D(PAntFoz,1E-2))
    {
      itPTalvegueAtual++;
    }

    if (itPTalvegueAtual != ItTalveguePrincipal->end())             
    {
      ItLDeltaSup itFoz(ItTalveguePrincipal->insert(itPTalvegueAtual,Foz));             //--- Achou o ponto anterior da foz neste talvegue, insere a foz  neste talvegue
      ItTalveguePrincipal->begin()->PCentral.Baliza = true;                             //---Maior talvegue e com foz, Marca este talvegue como o principal
      ItTalveguePrincipal->begin()->PCentral.Descricao += " Principal";
      itFoz->TipoObjeto = FOZ;                                                
      Foz.z = itFoz->PCentral.z;

      if(!VerfDescidaTalveguePrincipal())
      {
     // monolog.mensagem(-1, "Atenção o perfil do talvegue principal está ondulado.");
      }

      DadosBacia.h = fabs(ItTalveguePrincipal->begin()->PCentral.z - ItTalveguePrincipal->rbegin()->PCentral.z);

      return true;
    }
    else  Erro = TALVEGUE_PRINCIPAL_SEM_FOZ;
  }
  else Erro = FALTA_TALVEGUE_PRINCIPAL;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
  std::string ErroMsg;

  switch (Erro)
  {
    case TALVEGUE_PRINCIPAL_SEM_FOZ: ErroMsg = "A foz não está no talvegue principal"; break;
    case FALTA_TALVEGUE_PRINCIPAL: ErroMsg = "O talvegue principal não foi encontrado"; break;
    case TALVEGUE_PRINC_ONDULADO:  ErroMsg = "O talvegue principal possui altos e baixos"; break;
  }

  monolog.mensagem(-1,ErroMsg.c_str());
  
  if(Erro != BACIA_OK)
  {
    ItTalveguePrincipal = Talvegues.end();
    return false;
  }
  
  return true;
}

void CBacia::PerfilTalvPrincipal(void* pSuperfAtual)
{
  PerfilTalveguePrincipal.clear();
  CSuperficie* pSuperficieAtual((CSuperficie*)pSuperfAtual);

  if(ItTalveguePrincipal != Talvegues.end())
  {
    ItLDeltaSup itPAnterior(ItTalveguePrincipal->begin()), itPAtual(itPAnterior);
    double xInicial(0.0);

    PerfilTalveguePrincipal.insert(Ponto(0.0, itPAnterior->PCentral.z, itPAnterior->PCentral.z));

    for (++itPAtual; itPAtual != ItTalveguePrincipal->end(); ++itPAtual, ++itPAnterior)
    {
      pSuperficieAtual->Seccionar(itPAnterior->PCentral, itPAtual->PCentral, itPAnterior->PCentral, PerfilTalveguePrincipal, -INFINITO, true, xInicial, false, true, true);

      xInicial += hypot(itPAtual->PCentral.x - itPAnterior->PCentral.x, itPAtual->PCentral.y - itPAnterior->PCentral.y);

      PerfilTalveguePrincipal.insert(Ponto(xInicial, itPAtual->PCentral.z, itPAtual->PCentral.z));
    }

    LimpaSecao(PerfilTalveguePrincipal, 5.0, 0.005);

    //--- Se o arquivo da superficie (.dtr) não foi encontrado (só se o usuário o removeu) os pontos do talvegue ficam com a cota
    //--- igual a INFINITO, remove-os aqui

    for (itsetPontos itSPAtual = PerfilTalveguePrincipal.begin() = PerfilTalveguePrincipal.begin(); itSPAtual != PerfilTalveguePrincipal.end(); )
    {
      if (itSPAtual->z == INFINITO) PerfilTalveguePrincipal.erase(itSPAtual++);
      else itSPAtual++;
    }

    if (PerfilTalveguePrincipal.size() < 2) return;          //--- Não foram encontrados pontos com cotas válidas (ERRO GRAVE)

    //--- Calcula os is das retas

    itsetPontos itSPAnterior(PerfilTalveguePrincipal.begin()), itSPAtual(itSPAnterior);

    for (++itSPAtual; itSPAtual != PerfilTalveguePrincipal.end(); ++itSPAtual, ++itSPAnterior)
    {
      const_cast<Ponto*>(&*itSPAnterior)->i = (itSPAtual->z - itSPAnterior->z) / (itSPAtual->x - itSPAnterior->x);
    }

    const_cast<Ponto*>(&*PerfilTalveguePrincipal.rbegin())->i = itSPAnterior->i;
  }
}

void CBacia::LimpaSecao(setPontos& SetXPontos, double DistMinima, double IncMinima)
{
  if (SetXPontos.size() < 3) return;

  std::list<itsetPontos> LstRemover;
  std::list<itsetPontos>::iterator ItLstRemover;

  itsetPontos ItAnterior(SetXPontos.begin()), ItAtual(ItAnterior);
  ++ItAtual;
  double RampaAnterior((ItAtual->y - ItAnterior->y) / (ItAtual->x - ItAnterior->x)), RampaAtual(INFINITO);
  register itsetPontos ItProximo(ItAtual);

  for (++ItProximo; ItProximo != SetXPontos.end(); ++ItProximo)
  {
    RampaAtual = (ItProximo->y - ItAtual->y) / (ItProximo->x - ItAtual->x);

    if (*ItProximo != *SetXPontos.rbegin() && ((ItAtual->x - ItAnterior->x) < DistMinima || ((ItAtual->x - ItAnterior->x) < DistMinima * 20.0 && fabs(RampaAtual - RampaAnterior) < IncMinima)))
    {
      LstRemover.push_back(ItAtual);
      ItAtual = ItProximo;
      RampaAnterior = (ItProximo->y - ItAnterior->y) / (ItProximo->x - ItAnterior->x);
    }
    else
    {
      ItAnterior = ItAtual;
      ItAtual = ItProximo;
      RampaAnterior = RampaAtual;
    }
  }
  for (register std::list<itsetPontos>::iterator it = LstRemover.begin(); it != LstRemover.end(); ++it)
  {
    SetXPontos.erase(*it);
  }
}

//--- A declividade média esta sendo calcula como a declividade entre o ponto inicial e final da partição, verificar

bool CBacia::CalciMedioTalvPrincipal()
{
  if (TemTalveguePrincipal())
  {
    CalculaCotasParticoes();

    if (PegaPerfTalvPrincipal().size() > 2)   //--- Perfil sem cotas válidas (====> ERRO GRAVE <====(sem pontos na superfície?))
    {
      double Denominador(0.0);

      //-- Quando não tem partiçoes a rampa e a rampa entre o inicio e o fim do talvegue
      //--- Inclui 1 partição composta por todo o talvegue, no final a rampa será a rampa entre  início e o fim do talvegue;

      if (ParticoesTalveguePrincipal.size() == 0)   
      {
        Ponto ParticaoUnica(PegaPerfTalvPrincipal().rbegin()->x - PegaPerfTalvPrincipal().begin()->x, PegaPerfTalvPrincipal().rbegin()->y);

        ParticaoUnica.S = ParticaoUnica.x;
        ParticaoUnica.i = (PegaPerfTalvPrincipal().rbegin()->y - PegaPerfTalvPrincipal().begin()->y) / ParticaoUnica.S;

        ParticoesTalveguePrincipal.emplace(ParticaoUnica);
      }

      if (ParticoesTalveguePrincipal.size() > 0)
      {
        Ponto* pAnterior(nullptr);

        itsetPontos itAnt(ParticoesTalveguePrincipal.begin()), itAtual(itAnt);

        //--- Calcula as inclinações das partições do talvegue principal

        for (++itAtual; itAtual != ParticoesTalveguePrincipal.end(); ++itAtual, ++itAnt)
        {
          //--- i e S, ok alterar, não corrompe o set

          pAnterior = const_cast<Ponto*>(&*itAnt);

          pAnterior->S = (itAtual->x - itAnt->x);               //--- O L da partição vai em S
          pAnterior->i = (itAtual->y - itAnt->y) / (itAnt->S);
          
        }

        if(ParticoesTalveguePrincipal.size() > 1)
        {
          const_cast<Ponto*>(&*itAnt)->i = 0.0;     //--- Para nao ser incluído na somatória final
        }

        //-- Calcula a inclinação media das partições do perfil longitudinal

        Ponto PAnterior(*PerfilTalveguePrincipal.begin());

        for (itAtual = ParticoesTalveguePrincipal.begin(); itAtual != ParticoesTalveguePrincipal.end(); ++itAtual)
        {
          if (fabs(itAtual->i) > 0.0)
          {
            Denominador += itAtual->S / (sqrt(fabs(itAtual->i)));
          }
        }
      }

      // DadosBacia.iMTP = pow(((PerfilTalveguePrincipal.rbegin()->x - PerfilTalveguePrincipal.begin()->x) / Denominador),2.0);
      //--- Está calculando 2 vezes aqui e em dados hidrologicos, VERIFICAR!!!!

      if(Denominador != 0)   //--- Talvegues muito prquenos podem ficar com deltah = 0.0
      {
        DadosBacia.S = pow(((PerfilTalveguePrincipal.rbegin()->x - PerfilTalveguePrincipal.begin()->x) / Denominador), 2.0);
      }

      return true;
    }
  }

  monolog.mensagem(-1,"Talvegue principal ausente ou inválido.");

  return false;
}

int CBacia::CalculaCotasParticoes()
{
  for (itsetPontos it = ParticoesTalveguePrincipal.begin(); it != ParticoesTalveguePrincipal.end(); ++it)
  {
    Ponto PAnterior,PAtual;

    itsetPontos itProx(PerfilTalveguePrincipal.upper_bound(it->x - 0.01));

    if(itProx != PerfilTalveguePrincipal.end())
    {
      itsetPontos itPAnt(itProx);
      --itPAnt;

      const_cast<Ponto*>(&*it)->y = itPAnt->i * (it->x - itPAnt->x) + itPAnt->y;     //--- pode alterar o y, ordenação apenas por x
    }

    else return 0; //--- erro, partição fora do perfil....?????
  }

  return 1;
}

int CBacia::CalculaCurvaHipsometrica(void* pSuperf,double& pExtTotalCN)
{
  CSuperficie* pSuperficie((CSuperficie*)pSuperf);
  pExtTotalCN = 0.0;
  CString NomeProjeto(pSuperficie->PegaNomeProjeto().c_str());
  CSuperficie Superficie(NomeProjeto);

  std::ofstream ArqLog(NomeProjeto + "_DefBacias.log", std::ofstream::out | std::ofstream::trunc);
  //if (ArqLog.good() == false) CstaTexto.SetWindowText("Erro ao abrir o arquivo de log. Programa abortado.");
  tylstIntersecoes InterErro;

  int Erro(0);

  ArqLog << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

  GravaLog("\n======== Vias Urbanas & Estradas Versão 5.30 - 2021=========\n", ArqLog, true);
  GravaLog("ARQUIVO DE LOG DA DEFINIÇÃO DAS BACIAS HIDROGRÁFICAS - INÍCIO ========================", ArqLog);

  CalculaInterCNEspigao(pSuperf);
  if(VerificaBacia(InterErro,ArqLog)) 
  {
    CalculaAreasCN(pSuperf,pExtTotalCN,ArqLog);
    CalculaDadosHipsometricos(pSuperf);
  }
  else Erro = 1;

  LimpaIntersEspigao();
  LimpaIntersCN(pSuperf);

  GravaLog("ARQUIVO DE LOG DA DEFINIÇÃO DAS BACIAS HIDROGRÁFICAS - FIM ========================", ArqLog);

  return Erro;
}

int CBacia::CalculaInterCNEspigao(void* pSuperf)
{
  if (EspigaoBacia.Area < 0.0) InverteDirecaoGiro();

  CSuperficie* pSuperficie((CSuperficie *) pSuperf);
  citlstitsetPontos ItEspigaoAnt(EspigaoBacia.PegaLista().begin()), ItEspigaoAtual(ItEspigaoAnt);
  Ponto Intersecao;

  LstIntersecoesEspigao.clear();
  LstIntersecoesCN.clear();

  //--- Calcula as interseções das CN com o espigão. É preciso inserir na CN e no espigão os pontos de interseção
  //--- para uso no cálculo das áreas. No espigão é preciso tb inserir o proximo ponto da lista, para iniciar 
  //--- o cálculo da área. O ponto de interseção na CN tem que estar marcado, pra saber que ele é uma interseção 
  //--- com um espigão. Estas informações ficam guardadas em CIntersCNEspigao         

  for (++ItEspigaoAtual; ItEspigaoAtual != EspigaoBacia.PegaLista().end(); ++ItEspigaoAtual, ++ItEspigaoAnt)
  {
    for (ItLLPontos itLLCN = pSuperficie->LPontosCN.begin(); itLLCN != pSuperficie->LPontosCN.end() ;  ++itLLCN)
    {
      if(!Mat.EMultiplo(itLLCN->begin()->z, EquidisCNHistog) || itLLCN->size() < 2) continue;
 
      ItLPontos itPAnterior(itLLCN->begin()),itPAtual(itPAnterior);
     
      for (++itPAtual; itPAtual !=itLLCN->end() ; ++itPAtual, ++itPAnterior)
      {
        if (CLPontos::CalculaIntersecao(*(*ItEspigaoAnt), *(*ItEspigaoAtual), *itPAnterior, *itPAtual, Intersecao,TRUE,1E-5) == TRUE)
        {
          //--- Às vezes ao pegar o ponto do talvegue pode ser que o ponto caia em um ponto de uma CN, devido a aproximações 
          //--- no mousemove  do despongeomview.cpp. Neste caso se as interseçoes das CN cairiam no ponto do talvegue, produzirão
          //--- um comportamento errático do programa. Evita isso aqui, somando 1 cm em cada coordenada.

        
          if (!Intersecao.Compara2D(*(*ItEspigaoAtual),1e-3))  // && Intersecao != *(*ItEspigaoAnt))
          {
            Intersecao.Baliza2 |= PONTO_INTERS_CN_ESPIGAO;                   //--- indica na CN que este ponto é interseção CN->Talvegue
            Intersecao.z = itPAnterior->z;
            LstIntersecoesCN.emplace_back(CIntersCNEspigao(Intersecao, itPAtual, itLLCN, ItEspigaoAnt));
          }
        }
      }

      //--- Insere a(s) interseção(ões) na CN que acabou de ser varrida

      if (LstIntersecoesCN.size())
      {
        for (ittylstIntersecoes ItLstInterCN = LstIntersecoesCN.begin(); ItLstInterCN != LstIntersecoesCN.end(); ++ItLstInterCN)
        {
          ItLstInterCN->ItCN->insert(ItLstInterCN->ItPontoAntCN, ItLstInterCN->PontoIntersecao);
        }

        //--- As interseções das CN no espigão tem q ser inseridas depois que varrer todo o espigão, guarda-as 

        LstIntersecoesEspigao.insert(LstIntersecoesEspigao.end(), LstIntersecoesCN.begin(), LstIntersecoesCN.end());

        LstIntersecoesCN.clear();
      }
    }
  }

  //--- Verreu todo o espigão, é preciso inserir as interseções das CNs no espigão, mas eles tem q ser inseridas na ordem crescente de distância
  //--- Primeiramente, calcula as distancias das interseções até os pontos anteriores a elas no espigão para poder ordenar por distância

  setPontos IntersecoesOrdenadas_x;

  ittylstIntersecoes itPAnteriorEspigao(LstIntersecoesEspigao.end()),it(LstIntersecoesEspigao.begin());

  //--- As distâncias são na mesma aresta do espigão, em relação ao ponto anterior do espigão

  while (it != LstIntersecoesEspigao.end())
  {
    itPAnteriorEspigao = it;

    IntersecoesOrdenadas_x.clear();

    //--- Enquanto estiver na mesma aresta do espigão, calcula as distâncias em relação ao ponto anterior do espigão

    while (it != LstIntersecoesEspigao.end() && *it->itPEspigAnterior == *itPAnteriorEspigao->itPEspigAnterior)
    {
      //--- Os pontos são ordenados por x, y, e z como agora é preciso dos pontos ordenados por distâncias,
      //-- coloca a distancia em x e o valor de x vai pra S, depois destroca;

      it->PontoIntersecao.S = it->PontoIntersecao.x;
      it->PontoIntersecao.x = hypot(it->PontoIntersecao.x - (*it->itPEspigAnterior)->x, it->PontoIntersecao.y - (*it->itPEspigAnterior)->y);

      IntersecoesOrdenadas_x.emplace(it->PontoIntersecao);

      ++it;
    }

    //--- Fez 1 aresta do espigão, insere as inserseções nesta aresta do espigão.

    citlstitsetPontos itEspigaoProx(itPAnteriorEspigao->itPEspigAnterior);
    ++itEspigaoProx;

    double STemp;

    for (itsetPontos itp = IntersecoesOrdenadas_x.begin(); itp != IntersecoesOrdenadas_x.end(); ++itp)
    {
      //--- Volta o x e o S para os lugares certos

      Ponto PTemp(*itp);

      STemp = PTemp.x;
      PTemp.x = PTemp.S;
      PTemp.S = STemp;

      EspigaoBacia.InserePonto(itEspigaoProx, PTemp);     //--- O ponto posterior de sequencia do espigão passa a ser o inserido
    }
  }

  for (ittylstIntersecoes it = LstIntersecoesEspigao.begin(); it != LstIntersecoesEspigao.end(); ++it)
  {
    Ponto STemp(it->PontoIntersecao);

    it->PontoIntersecao.x = it->PontoIntersecao.S;
    it->PontoIntersecao.S = STemp.S;
  }

  return 0;
}

int CBacia::CalculaAreasCN(void* psuperf, double& pExtTotalCN, std::ofstream& pArqLog)
{
  CSuperficie* pSuperf((CSuperficie*)psuperf);

  //--------------------------------------------------------------------------------------------
  //   PREMISSAS:
  //
  //   O espigão tem q girar para a direita 
  //   O espigão tem que ser regularizado: do ponto mais baixo para o ponto mais alto
  //   e em 2 ramos o da esquerda e o da direita
  //   Estes pontos mais alto e mais baixo tem que ser marcados no espigão
  //   As interseções das CNs com o espigão tem q ser marcadas na CN e no Espigão
  //   O cálculo da área da CN sempre começa do lado esquerdo do espigão para o lado direito 
  //   No primeiro ramo as CNs são buscadas no espigão do ponto mais baixo para o mais alto
  //   No segundo ramo as CNs são buscadas no espigão da mesma forma, do ponto mais baixo para o mais alto 
  //   AS CN válidas são as que giram para o lado direito, se ela não for pega no primeiro ramo
  //   do espigão será pega no segundo
  //
  //    CÁLCULO DAS ÁREAS DAS CNs:  
  //    
  //   PARA todas as CNs do ponto mais baixo do espigão até o ponto mais alto do lado esquerdo:
  //      Calcule a area pela CN (ate chegar no espigão)   
  //      Calcule a area pelo espigão (até chegar no ponto inicial)
  //   Fim PARA
  //   
  //   CALCULE a area pela CN:
  //     Pegue o proximo ponto daCN
  //     Calcule a area da reta
  //   Fim CALCULE
  //   
  //   CALCULE a área pelo espigão:
  //      Pegue o proximo ponto do espigão
  //      SE este ponto for um ponto de CN
  //        SE a cota desta CN for >= a da CN atual
  //          CALCULE a area pela reta
  //        SE não
  //          CALCULE a área pela CN
  //      SENAO
  //         Calcule a área pelo espigão  // RECURSIVO
  //      FIM SE
  //   FIM CALCULE 
  //       
  //       Se a cota desta CN é maior ou igual a CN  
  //       Enquanto a CN não chegar no espigão :
  //         Pegue o proximo ponto da CN
  //         calcule a area da reta
  //       Fim Enquanto a CN não chegar no espigão
  //       Se este ponto no espigão não for o ponto inicial da CN
  //         calcule a área pelo esoigão  
  //       fim se     
  //-------------------------------------------------------------------------------------------

  pArqLog << "Areas pela CN2:" << std::endl;

  SetCotasAreas.clear();

  citlstitsetPontos ItEspigaoAnterior(EspigaoBacia.PegaLista().begin()), ItEspigaoAtual(ItEspigaoAnterior);

  for (ItEspigaoAtual++ ; ItEspigaoAtual != EspigaoBacia.PegaLista().end(); ++ItEspigaoAtual, ++ItEspigaoAnterior)
  {
    double AreaAtual(0.0), ExtensaoAtual(0.0);

    if ((*ItEspigaoAtual)->Baliza2 & PONTO_INTERS_CN_ESPIGAO && !((*ItEspigaoAtual)->Baliza2 & PONTO_CALCULADO))
    {
      ittylstIntersecoes itInterEspigao(PegaIntersEspigao(**ItEspigaoAtual));            //--- Pega a interseção no Espigão pra poder pegar a CN
      ItLPontos itCNAnterior(PegaPontoCN(itInterEspigao->ItCN, **ItEspigaoAtual)),       //--- Pega o ponto na CN, pra iniciar o cálculo das áreas
                itInicioCN(itCNAnterior), itCNAtual(itCNAnterior);
      ItLLPontos CNAtual(itInterEspigao->ItCN);                                          //--- Iterador para a CN que está sendo calculada

      if (itInterEspigao->PontoIntersecao.Baliza2 & CN_GIRA_DIREITA)
      {
        ++itCNAtual;
        pArqLog << "Areas pela CN Frente:" << std::endl;
        CalculaAreaPelaCN(itCNAtual, itCNAnterior, AreaAtual, ExtensaoAtual, itInterEspigao, pArqLog, PARA_FRENTE);
        pArqLog << "Areas pelo Espigão:" << std::endl;
        CalculaAreaPeloEspigao(AreaAtual,ExtensaoAtual, itCNAtual, *itInicioCN, *itCNAtual, pArqLog, PARA_FRENTE);
      }
      else
      {
        --itCNAtual;
        pArqLog << "Areas pela CN Tras:" << std::endl;
       // CalculaAreaPelaCNVoltando(itCNAtual, itCNAnterior, AreaAtual,ExtensaoAtual,itInterEspigao, pArqLog);
        CalculaAreaPelaCN(itCNAtual, itCNAnterior, AreaAtual, ExtensaoAtual, itInterEspigao, pArqLog,PARA_TRAS);
        pArqLog << "Areas pelo Espigão:" << std::endl;
        CalculaAreaPeloEspigao(AreaAtual,ExtensaoAtual,itInterEspigao->ItCN->begin(), itInterEspigao->PontoIntersecao, *itCNAtual, pArqLog, PARA_FRENTE);
      }

      const_cast<Ponto*>(&*EspigaoBacia.BuscaPonto(*itCNAtual))->Baliza2 |= PONTO_CALCULADO;   //--- Marca o ponto do outro lado do espigao
      itInterEspigao = PegaIntersEspigao(*itCNAtual);
      itInterEspigao->PontoIntersecao.Baliza2 |= PONTO_CALCULADO;
    }
  }

  CalculaAreaFaixaCNs();

  pExtTotalCN = 0.0;

  pArqLog << "Areas finais:" << std::endl;

  for (ittysetCotasArea it = SetCotasAreas.begin(); it != SetCotasAreas.end(); it++)
  {
    pExtTotalCN += it->Extensao;
    pArqLog << it->Cota << " " << it->AreaTotal << " "  << it->AreaCN << " " << it->Extensao << std::endl;
  }

  return 1;
}

int CBacia::CalculaAreaPelaCN(ItLPontos& itCNAtual, ItLPontos itCNAnterior,double& pAreaCNAtual,double& pExtensaoAtual,ittylstIntersecoes itInterEspigao,std::ofstream& pArqLog,int Sentido)
{
  bool PrimeiraVez(true);
  int Direcao(-1);

  //--- Calcula área pela CN
  //=========================

  pArqLog << itCNAnterior->x << " " << itCNAnterior->y << " " << itCNAnterior->z << std::endl;

  do         //---  vai haver no mínimo mais 1 ponto na CN
  {
    pArqLog << itCNAtual->x << " " << itCNAtual->y << " " << itCNAtual->z << " " << std::endl;

    //--- Calcula a área e a extensão da reta

    pAreaCNAtual += (itCNAtual->y + itCNAnterior->y) / 2.0 * (itCNAtual->x - itCNAnterior->x);
    pExtensaoAtual += hypot(itCNAtual->x - itCNAnterior->x,itCNAtual->y - itCNAnterior->y);

    IncrementaItCN(itCNAtual, itCNAnterior, *itInterEspigao->ItCN,Sentido);

    //--- Se itCNAtual ficar end no meio é porque a CN é circular e chegou no fim da CN
    //--- Neste caso o proximo ponto é o 1o depois do inicio da CN e o ponto anterior é o 
    //--- início da CN (Lista  circular)

    if (itCNAtual == itInterEspigao->ItCN->end())
    {
      itCNAtual = itCNAnterior = itInterEspigao->ItCN->begin();
      ++itCNAtual;
    }
  } while (itCNAtual->Baliza2 != PONTO_INTERS_CN_ESPIGAO);       //--- Enquanto não for interseção da CN com o talvegue, vai haver no mínimo 1 interseção com o talvegue,a saída da CN

  pAreaCNAtual += (itCNAtual->y + itCNAnterior->y) / 2.0 * (itCNAtual->x - itCNAnterior->x);   //--- Calcula a última reta
  pExtensaoAtual += hypot(itCNAtual->x - itCNAnterior->x, itCNAtual->y - itCNAnterior->y);

  pArqLog << "Area: " << pAreaCNAtual  << std::endl;
   
  return 1;
}

int CBacia::CalculaAreaPelaCNVoltando(ItLPontos& itCNAtual, ItLPontos itCNAnterior, double& pAreaCNAtual, double& pExtensaoAtual, ittylstIntersecoes itInterEspigao,std::ofstream& pArqLog)
{
  bool PrimeiraVez(true);
  int Direcao(-1);

  //--- Calcula área pela CN no sentido contrario 
  //=============================================

  pArqLog << itCNAnterior->x << " " << itCNAnterior->y << " " << itCNAnterior->z << std::endl;

  do         //---  vai haver no mínimo mais 1 ponto na CN
  {
    //--- Calcula a área da reta

    pArqLog << itCNAtual->x << " " << itCNAtual->y << " " << itCNAtual->z << std::endl;

    pAreaCNAtual += (itCNAtual->y + itCNAnterior->y) / 2.0 * (itCNAtual->x - itCNAnterior->x);
    pExtensaoAtual += hypot(itCNAtual->x - itCNAnterior->x, itCNAtual->y - itCNAnterior->y);

     IncrementaItCN(itCNAtual, itCNAnterior, *itInterEspigao->ItCN,PARA_TRAS);

  } while (itCNAtual->Baliza2 == PONTO_CN);       //--- Enquanto não for interseção da CN com o talvegue, vai haver no mínimo 1 interseção com o talvegue,a saída da CN

  pAreaCNAtual += (itCNAtual->y + itCNAnterior->y) / 2.0 * (itCNAtual->x - itCNAnterior->x);   //--- Calcula a última reta
  pExtensaoAtual += hypot(itCNAtual->x - itCNAnterior->x, itCNAtual->y - itCNAnterior->y);

  pArqLog << "Area: " << pAreaCNAtual << std::endl;

  return 1;
}

int CBacia::CalculaAreaPeloEspigao(double& pAreaCNAtual, double& pExtensaoAtual, ItLPontos& ItCNAtual, const Ponto& PontoInicial, const Ponto& PontoFinal, std::ofstream& pArqLog, int Direcao)
{
  ittylstIntersecoes itInterEspigao(PegaIntersEspigao(PontoFinal));          //--- Pega a interseção no Espigão pra poder pegar a CN
  if (itInterEspigao != LstIntersecoesEspigao.end())    //===== TEM Q ACHAR---> VERIFICAR 18072023
  {
  ItLPontos itCNAnterior(PegaPontoCN(itInterEspigao->ItCN, PontoFinal)),     //--- Pega o ponto na CN, pra iniciar o cálculo das áreas
            itInicioCN(itCNAnterior);
  ItLLPontos CNAtual(itInterEspigao->ItCN);                                             //--- Iterador para a CN que está sendo calculada

  citlstitsetPontos ItEspigaoCalcAtual, ItEspigaoCalcAnterior;    //--- iterador circular o espigão no cálculo das áreas

  //--- itCN está numa interseção da CN com o espigão
  //--- Pega a interseção no Espigão

  itInterEspigao = PegaIntersEspigao(PontoFinal);         //--- Pega a interseção no espigão pra poder pegar o espigao
  ItEspigaoCalcAnterior = PegaPontoEspigao(PontoFinal);   //--- Pega o iterador no Espigao, pra cálcular a área pelo espigão
  ItEspigaoCalcAtual = ItEspigaoCalcAnterior;

  itInterEspigao->PontoIntersecao.Baliza2 |= PONTO_CALCULADO;

  if (Direcao == PARA_FRENTE)
  {
    ++ItEspigaoCalcAtual;
    if (ItEspigaoCalcAtual == EspigaoBacia.PegaLista().end())
    {
      ItEspigaoCalcAtual = EspigaoBacia.PegaLista().begin();
    }
  }
  else --ItEspigaoCalcAtual;

  //Direcao = VerfLadoaSeguir(itInterEspigao);            //--- Verificar qual lado seguir, que é o lado que vai para cima

  pArqLog << (*ItEspigaoCalcAnterior)->x << " " << (*ItEspigaoCalcAnterior)->y << " " << (*ItEspigaoCalcAnterior)->z << std::endl;


  //--- A direção no espigão so pode ser para trás, pois está sempre voltando no espigão

  //--- Calcula área pelo espigão, até fechar, ou seja até chegar no inicio da CN
  //--- Se interceptar uma CN de cota menor, vai pela CN de cota menor
  //===============================================================================
  do
  {
    do         //---  vai haver no mínimo mais 1 ponto no talvegue
    {
      //--- Calcula a área da reta

      pArqLog << (*ItEspigaoCalcAtual)->x << " " << (*ItEspigaoCalcAtual)->y << " " << (*ItEspigaoCalcAtual)->z << std::endl;

      pAreaCNAtual += ((*ItEspigaoCalcAtual)->y + (*ItEspigaoCalcAnterior)->y) / 2.0 * ((*ItEspigaoCalcAtual)->x - (*ItEspigaoCalcAnterior)->x);

      IncrementaItEspigao(ItEspigaoCalcAnterior, ItEspigaoCalcAtual, Direcao);

      //--- enquanto não chegar no início da CN 

    } while (**ItEspigaoCalcAtual != PontoInicial);

    pArqLog << (*ItEspigaoCalcAtual)->x << " " << (*ItEspigaoCalcAtual)->y << " " << (*ItEspigaoCalcAtual)->z << std::endl;

    pAreaCNAtual += ((*ItEspigaoCalcAtual)->y + (*ItEspigaoCalcAnterior)->y) / 2.0 * ((*ItEspigaoCalcAtual)->x - (*ItEspigaoCalcAnterior)->x);

    if ((*ItEspigaoCalcAtual)->z < itInicioCN->z)    //--- Se interceptou uma CN de cota menor, vai pela CN de cota menor
    {
      //--- Acerta os iteradores para ir pela CN
      //--- Esta CN vai cortar o espigão em um ponto na frente
      //--- continua no espigão a partir deste ponto

      itInterEspigao = PegaIntersEspigao(**ItEspigaoCalcAtual);                                   //--- Pega o ponto daCN na lista de interseções no espigão
      ItCNAtual = itCNAnterior = PegaPontoCN(itInterEspigao->ItCN, **ItEspigaoCalcAtual);   //--- Iterador para o ponto anterior na CN (este ponto anterior é a interseçao)

      ++ItCNAtual;

      CalculaAreaPelaCN(ItCNAtual, itCNAnterior, pAreaCNAtual, pExtensaoAtual, itInterEspigao, pArqLog, PARA_FRENTE);

      //--- Acerta os iterador e volta a calcular a area pelo espigão 

      itInterEspigao = PegaIntersEspigao(*ItCNAtual);         //--- Pega a interseção no espigão pra poder pegar o espigao
      ItEspigaoCalcAnterior = PegaPontoEspigao(*ItCNAtual);   //--- Pega o iterador no Espigao, pra cálcular a área pelo espigão
      ItEspigaoCalcAtual = ItEspigaoCalcAnterior;

      itInterEspigao->PontoIntersecao.Baliza2 |= PONTO_CALCULADO;

      ++ItEspigaoCalcAtual;
    }
  } while (**ItEspigaoCalcAtual != PontoInicial);     //--- até chegar no ponde de origem da área

  //--- A Maior área prevalece

  CCotaArea CotaArea(PontoInicial.z, pAreaCNAtual,0.0, pExtensaoAtual);
  ittysetCotasArea ItCotaArea(SetCotasAreas.find(CotaArea));

  if (ItCotaArea == SetCotasAreas.end() || pAreaCNAtual < ItCotaArea->AreaTotal)
  {
    if (ItCotaArea == SetCotasAreas.end()) SetCotasAreas.emplace(CotaArea);
    else
    {
      if (pAreaCNAtual > 0.0)
      {
        const_cast<CCotaArea*>(&(*ItCotaArea))->AreaTotal = pAreaCNAtual;
        const_cast<CCotaArea*>(&(*ItCotaArea))->Extensao = pExtensaoAtual;
        }
      }
    }
  }

  return 1;
}

 ittylstIntersecoes CBacia::PegaIntersEspigao(const Ponto& PontoInters)
{
  ittylstIntersecoes it(LstIntersecoesEspigao.begin());

  while(it != LstIntersecoesEspigao.end() && it->PontoIntersecao != PontoInters) ++it;

 return it;
}

ItLPontos CBacia::PegaPontoCN(ItLLPontos ItCN,const Ponto& PontoInters)  //--- Pega o ponto na CN
{
  ItLPontos it(ItCN->begin());

  while (it != ItCN->end() && *it != PontoInters) ++it;

  return it;
}

int CBacia::VerfLado(const Ponto& PontoEspigAnt, const Ponto& PEspigAtual, ItLPontos itCNAtual)
{
  double Area((PEspigAtual.y + PontoEspigAnt.y) / 2.0 * (PEspigAtual.x - PontoEspigAnt.x));

  Area += (itCNAtual->y + PEspigAtual.y) / 2.0 * (itCNAtual->x - PEspigAtual.x);
  Area += (PontoEspigAnt.y + itCNAtual->y) / 2.0 * (PontoEspigAnt.x - itCNAtual->x);

  return Area < 0.0 ? LADO_ESQUERDO : LADO_DIREITO;
}

ittylstIntersecoes CBacia::PegaIntersCN(const Ponto& PontoInters)
{
  ittylstIntersecoes it(LstIntersecoesCN.begin());

  while (it != LstIntersecoesCN.end() && it->PontoIntersecao != PontoInters) ++it;

  return it;
}

citlstitsetPontos CBacia::PegaPontoEspigao(const Ponto& PontoCN)
{
  citlstitsetPontos it(EspigaoBacia.PegaLista().begin());

  while(it !=EspigaoBacia.PegaLista().end() && **it != PontoCN)  ++it;

  return it;
}

#define ENTROU_CN 1
#define SAIU_CN 0

int CBacia::VerfLadoaSeguir(ittylstIntersecoes IntersEspigao) //--- ATENCAO => NÃO ESTA SENDO USADA
{
  //--- A CN chegou em uma aresta do espigão a primeira vez.
  //--- Esta primeira vez é preciso decidir para que lado seguir.
  //--- Ela pode terminar aqui ou se interceptar denovo com o espigão (n vezes)
  //--- Para saber para que lado ir (ir sempre para o ponto mais alto): 
  //--- Vai para o lado cuja próxima curva é mais alta (Tenta dos 2 lados)
  //--- Se não achou é porque as 2 CN vizinhas têm a mesma cota
  //--- 
  //--- Neste caso é preciso ir além das CN vizinhas para verificar qual lado seguir:
  //--- Verifica pro lado esquerdo(ou direito):
  //--- Verifica se existe algum ponto (de outra CN ou de Talvegue) dentro da CN vizinha.
  //--- Se este ponto for mais alto que a CN significa que o plano desta CN é mais alto que
  //--- O plano da CN que está sendo calculada, então o plano da CN que esta entre
  //--- as 2 CN é mais baixo. significando que não é este o lado, segue para  o outro lado.
  //--- Se não existir ponto  dentro da CN vizinha  segue até achar um ponto de talvegue
  //--- (ou de outra CN (que estará na mesma cota) repetindo o processo).
  //--- Para decidir se não houver pontos dentro da proxima CN, vai haver ao menos 1 ponto, 
  //--- o próximo ponto de espigão.
  //--- Se o ponto de espigao estiver fora de uma CN ele está no plano da CN
  //--- que esta sendo feita. Se ele tiver a cota mais baixa significa que este lado esta abaixo, 
  //--- segue para o outro lado.
  //---
  //--- Se houver CN dentro da CN vizinha, apenas o ponto dela deve ser levado em consideração, 
  //--- se maior ou menor, e este plano esta totamente dentro da CN vizinha.
  //--- 
  //--- Se para os dois lados estiver menor ou igual despreza esta curva (curva repetida no mesmo plano)

  //--- Pega o ponto de interseção no espigão

  citlstitsetPontos itEspigao(EspigaoBacia.PegaPontoSequencia(IntersEspigao->PontoIntersecao));

  if (itEspigao == EspigaoBacia.PegaLista().end()) ASSERT(0);   //--- tem que achar!

  ++itEspigao;                                                                               //--- Procura na próxima intersecao com uma CN ou ponto de espigão, lado direito
  if (itEspigao == EspigaoBacia.PegaLista().end()) itEspigao = EspigaoBacia.PegaLista().begin();       //--- Pode ter sido o último, dá a volta

  if ((*itEspigao)->z > IntersEspigao->PontoIntersecao.z) return LADO_DIREITO;
  if ((*itEspigao)->z < IntersEspigao->PontoIntersecao.z) return LADO_ESQUERDO;

  //--- Neste Ponto a CN vizinha direita está na mesma cota
  //--- Tenta na CN vizinha esquerda

  --itEspigao;
  --itEspigao;

  if ((*itEspigao)->z > IntersEspigao->PontoIntersecao.z) return LADO_ESQUERDO;
  if ((*itEspigao)->z < IntersEspigao->PontoIntersecao.z) return LADO_DIREITO;

  //--- Neste ponto as 2 CN  vizinhas estão na mesma cota
  //--- Neste caso é preciso ir além das CN vizinhas para verificar qual lado seguir :
  //--- Verifica pro lado....(Ver cometário acima)

  //--- Vamos verificar pro lado direito. (o iterador já está apontando para o lado direito)

  citlstitsetPontos itAtual(itEspigao);  //--- itEspigao guarda a 1a interseção com CN
  int DentroCN(true);

  while ((*++itAtual)->Baliza2 == true)  //--- enquanto houver interseçoews com CNs
  {
    //--- É uma interseção com uma CN que esta dentro da proxima CN da CN que está sendo calculada
    //--- ... Se este ponto for mais alto que a CN significa que o plano desta CN é mais alto que...(ver comentário acima)

    if (DentroCN)
    {
      if ((*itAtual)->z > (*itEspigao)->z) return LADO_ESQUERDO;  //--- o Lado a seguir é o lado esquerdo
      if ((*itAtual)->z < (*itEspigao)->z) return LADO_DIREITO;  //--- o Lado a seguir é o lado direito
    }

    //--- Se chegou aqui a curva que está dentro da CN tem a mesma cota, é uma CN circular dentro de outraCN circular, continua procurando

    //--- Verifica se é a mesma CN se for saiu da CN

    //--- É preciso pegar a CN na lista de interseções das CNs

    ittylstIntersecoes itIntersCN(PegaIntersCN(**itAtual));

    if (itIntersCN == LstIntersecoesCN.end()) ASSERT(0);

    //--- Verifica se esta CN é a CN Vizinha a que está sendo feita, se for chegou no outro lado dela

    if (itIntersCN->ItCN == IntersEspigao->ItCN)
    {
      //--- Esta interseção é o outro lado da CN que é vizinha a CN que está sendo feita
      //--- Então este ponto é o fim da CN

      DentroCN = false;
    }
  }

  //--- Se chegou aqui é porque todas as interseções com as CN são de cotas iguais a da CN que está sendo calculada e não pode 
  //--- resolver dentro do while, Chegou no próximo ponto do espigão sem poder resolver 
  //--- Nesta aresta do espigão não há nenhum ponto acima ou abaixo da CN que está sendo calculada.
  //--- ou:
  //--- não há CNs cortando esta aresta do espigão (não entrou no while)   
  //--- então:
  //--- É um ponto de espigão, está no plano da CN que esta sendo calculada, ele decide para que lado ir.
  //--- (Estamos no lado direito)

  return ((*itAtual)->z > IntersEspigao->PontoIntersecao.z) ? LADO_DIREITO : LADO_ESQUERDO;
}

void CBacia::VerfCircular(citlstitsetPontos& ItEspigCalcAtual, citlstitsetPontos& ItEspigCalcAnterior)
{
  //--- A lista é circular
  //--- se chegou no fim do espigão continua do inicio, como o espigão é um poligono fechado  o inicio = fim
  //--- O fim do cálculo é o ponto inicial da CN, não o fim do espigão

  if (ItEspigCalcAtual == EspigaoBacia.PegaLista().end())      //--- Esta rodando pra frente e chegou no fim, continua do primeiro  
  {
    ItEspigCalcAtual = EspigaoBacia.PegaLista().begin();
  }
  else
  {
    if (ItEspigCalcAtual == EspigaoBacia.PegaLista().begin())   //--- Esta rodando pra traz e chegou no fim, continua do último
    {
      ItEspigCalcAtual = EspigaoBacia.PegaLista().end();
      --ItEspigCalcAtual;
    }
  }

  //--- O mesmo com o iterador para o anterior

  if (ItEspigCalcAnterior == EspigaoBacia.PegaLista().end())      //--- Esta rodando pra frente e chegou no fim, continua do primeiro  
  {
    ItEspigCalcAnterior = EspigaoBacia.PegaLista().begin();
  }
  else
  {
    if (ItEspigCalcAnterior == EspigaoBacia.PegaLista().begin())   //--- Esta rodando pra traz e chegou no fim, continua do último
    {
      ItEspigCalcAnterior = EspigaoBacia.PegaLista().end();
      --ItEspigCalcAnterior;
    }
  }
}

int CBacia::CalculaDadosHipsometricos(void* pSuperf)
{
  //==== NÃO ESTÁ FUNCIONANDO EM TODOS OS CASOS AVERIGUAR MELHOR!!!

  //--- Pode haver várias CN com a mesma cota,  geralmente uma está envolvendo a outra
  //--- Pega a  de maior área, que é sempre a última no set

  if(SetCotasAreas.size() < 2) return 0;

  tysetCotasArea SetFinal;

  //--- o Set a está ordenado por cota e area, basta remover os da mesma cota que tem menor area

  ittysetCotasArea itAnterior(SetCotasAreas.begin());
  ittysetCotasArea it(SetCotasAreas.begin());

  for (++it ; it != SetCotasAreas.end() ; ++it)
  {
    if (it->Cota == itAnterior->Cota)
    {
      SetCotasAreas.erase(itAnterior);
    }
    itAnterior = it;
  }

  //--- Calcula as áreas finais

  double AreaHipsometrica(0.0), AreaCNAnterior(0.0);
  rittysetCotasArea rit(SetCotasAreas.rend());

  for (rit = SetCotasAreas.rbegin() ; rit != SetCotasAreas.rend(); ++rit)
  {
    AreaHipsometrica = rit->AreaTotal - AreaCNAnterior;
    AreaCNAnterior = rit->AreaTotal;

    const_cast<CCotaArea*>((CCotaArea*)&*rit)->AreaTotal = AreaHipsometrica;   //--- Sem pb, ordenado por CN e só tem uma CN para cada área
  }

  //--- Limpa os velores errados
  //--- Aprimorar o cálculo

  //--- 1) Remove valores absurdos

  it = SetCotasAreas.end();
  it--;

  while(it != SetCotasAreas.end())
  {
    if(it->AreaCN > 99.0 || it->AreaCN < 1E-5 || fabs(it->Inclinacao) < 3.0)
    {
      SetCotasAreas.erase(*it--);
    }
    else
    {
      it--;
    }
  }

  //--- 2) Remove faixa de valores errados

  double AreaAnterior(100.0);
  it = SetCotasAreas.begin();

  while (it != SetCotasAreas.end())
  {
    if (it->AreaCN > AreaAnterior)
    {
      AreaAnterior = it->AreaCN;

      it--;

      do
      {
        SetCotasAreas.erase(*it--);
       
      } while (it != SetCotasAreas.end() && it->AreaCN < AreaAnterior);

      if(it != SetCotasAreas.end()) AreaAnterior = it->AreaCN;
    }
    else
    {
      AreaAnterior = it->AreaCN;
      it++;
    }
  }

  return 0;
}

void CBacia::LimpaIntersEspigao()
{
  citlstitsetPontos ItProx;

  for (citlstitsetPontos It = EspigaoBacia.PegaLista().begin(); It != EspigaoBacia.PegaLista().end(); )
  {
    if ((*It)->Baliza2 & PONTO_INTERS_CN_ESPIGAO)
    {
      ItProx = It;
      ++ItProx;

      EspigaoBacia.RemovePonto(**It);

      It = ItProx;
    }
    else ++It;
  }

  LstIntersecoesEspigao.clear();
}

void CBacia::LimpaIntersCN(void* pSuperf)
{
  CSuperficie* pSuperficie((CSuperficie*)pSuperf);

  for (ItLLPontos itLLCN = pSuperficie->LPontosCN.begin(); itLLCN != pSuperficie->LPontosCN.end() ; ++itLLCN)
  {
    for (ItLPontos itPAtual = itLLCN->begin() ; itPAtual != itLLCN->end() ; )
    {
      if (itPAtual->Baliza2 & PONTO_INTERS_CN_ESPIGAO)
      {
        pSuperficie->RemoveItSetCN(itPAtual);
        itLLCN->erase(itPAtual++);
      }
      else ++itPAtual;
    }
  }

  LstIntersecoesCN.clear();
}

#include <deque>

bool CBacia::VerificaBacia(tylstIntersecoes& InterErro, std::ofstream& pArqLog)
{
  if(!VerfCNCortandoVariasVezesEspigao(InterErro, pArqLog)) return false;

  //--- Agora as curvas de nivel que interceptam o espigão so podem estar subindo, ou no sentido contrário 
  //--- descendo. Se forem encontradas CN que contrariam esta regra elas são simplesmente eliminadas,
  //--- pois são lugares onde a água empoçaria ou pequenos morros. Nas bacias hidrográficas estas poças 
  //--- (ou pequenos morros) são muito difíceis de aconter pois so ocorrem em remansos  ou em pequenas saliênciasque 
  //-- devem ser eliminadas da área da bacia. Estas curvas são eliminadas e sua area é incluída na área 
  //--- da CN Anterior.
  //--- Regras:
  //--- As CN Validas entram por um lado do espigão e saem pelo outro
  //--- As CNs que entram e saem pelo mesmo lado do espigão são eliminadas
  //--- TUDO ERRADO as curvas de nivel que voltam, podem ser encostas de morros!!!!

  citlstitsetPontos ItIniEspigaoAnterior(EspigaoBacia.PegaLista().begin()), ItIniEspigaoNormalizado(ItIniEspigaoAnterior);

  //--- Coloca a foz como o primeiro ponto do espigão (Espigão Normalizado)

  while (ItIniEspigaoNormalizado != EspigaoBacia.PegaLista().end() && !(*ItIniEspigaoNormalizado)->Compara2D(Foz,1E-3))
  {
    ItIniEspigaoNormalizado++;
  }

  //--- Apenas nos projetos SIG o espigão vem com a foz
  //--- Se esta sem, a foz sera inserida posteriormente

  auto ItFozNormalizado(ItIniEspigaoNormalizado);

  lstPontos EspigaoNormalizado;

  //--- Move para o espigão normalizado os pontos da foz até o fim do espigão atual

  for( ; ItIniEspigaoNormalizado != EspigaoBacia.PegaLista().end() ; ItIniEspigaoNormalizado++)
  {
    EspigaoNormalizado.push_back(**ItIniEspigaoNormalizado);
  }

  //--- Move os pontos restantes, do início do espigão atual até a foz para o espigão normalizado

  for ( ; ItIniEspigaoAnterior != ItFozNormalizado ; ItIniEspigaoAnterior++)
  {
    EspigaoNormalizado.push_back(**ItIniEspigaoAnterior);
  }
  //--- Fecha o espigão (lista circular)
  
  EspigaoNormalizado.push_back(*EspigaoNormalizado.begin());

  //--- "Oficializa" o espigão normalizado

  EspigaoBacia.LimpaPontos();

  for (auto It = EspigaoNormalizado.begin(); It != EspigaoNormalizado.end() ; It++)
  {
    EspigaoBacia.InserePonto(*It);
  }
 
 citlstitsetPontos ItAnterior(EspigaoBacia.PegaLista().begin()), ItAtual(EspigaoBacia.PegaLista().begin());

  tysetItLLPontos SetCNMarcadas;

  //--- Define o sentido de giro da CN

  CAciTopografico EspigaoOk;   

  EspigaoOk.InserePonto(**ItAnterior);

  ++ItAtual;

  //--- Marca o sentido de giro da CN
  //--- Primeiramente faz subindo o espigão

  do
  {
    if ((*ItAtual)->Baliza2 & PONTO_INTERS_CN_ESPIGAO)  
    {
      //--- Verifica se a CN esta girando para a esquerda ou para a direita

      ittylstIntersecoes ItInterCN(LstIntersecoesEspigao.begin());

      //--- Posiciona no iterados da CN no espigão 

      for (; ItInterCN != LstIntersecoesEspigao.end(); ++ItInterCN)
      {
        if(*(*ItAtual) == ItInterCN->PontoIntersecao) break;
      }

      if (ItInterCN != LstIntersecoesEspigao.end())
      {
        if(SetCNMarcadas.find(ItInterCN->ItCN) == SetCNMarcadas.end())    //--- Já foi feita?
        {
          if (VerfLado(**ItAnterior, **ItAtual, ItInterCN->ItPontoAntCN) == LADO_DIREITO)   //--- Esta verificando com o ponto anterior, então se estiver a esquerda gira para direita
            ItInterCN->PontoIntersecao.Baliza2 |= CN_GIRA_DIREITA;
          else
            ItInterCN->PontoIntersecao.Baliza2 &= ~CN_GIRA_DIREITA;

          SetCNMarcadas.insert(ItInterCN->ItCN);
        }

        EspigaoOk.InserePonto(ItInterCN->PontoIntersecao);                      //--- Insere o ponto marcado
      }
    }
    else EspigaoOk.InserePonto(**ItAtual);     //--- não é ponto de CN, insere no espigão ok

    ++ItAnterior;
    ++ItAtual;

    }while(ItAtual != EspigaoBacia.PegaLista().end());

  //--- Agora faz descendo mas no sentido contrário.

  //--- Nos EspigoesOK estão as CN com os pontos do poligono do espigão
  //--- Reconstroi o espigão original
  //--- Agora o espigão começa no ponto mais baixo

  EspigaoBacia.LimpaPontos();

  for (citlstitsetPontos It = EspigaoOk.PegaLista().begin() ; It != EspigaoOk.PegaLista().end() ; ++It)
  {
    EspigaoBacia.InserePonto(**It);
  }

  return true;
}
 
bool CBacia::VerfCNCortandoVariasVezesEspigao(tylstIntersecoes& pInterErro, std::ofstream& pArqLog)
{
  setPontos setCNNerificadas;

  for (ittylstIntersecoes ItInterVerificar(LstIntersecoesEspigao.begin()) ;
       ItInterVerificar!= LstIntersecoesEspigao.end() ;
       ++ItInterVerificar)
  {
    //--- Verifica se esta CN já foi verificada

    ittylstIntersecoes itInterTemp(pInterErro.begin());

    if(pInterErro.size() > 0)
    {
      while ((itInterTemp)->ItCN != ItInterVerificar->ItCN && ++itInterTemp != pInterErro.end()) ;
    }

    if (itInterTemp == pInterErro.end())
    {
      ittylstIntersecoes ItInterCNAtual(ItInterVerificar);

      int ContaIntersecoes(1);

      while (++ItInterCNAtual != LstIntersecoesEspigao.end())
      {
        if (ItInterCNAtual->ItCN == ItInterVerificar->ItCN) ++ContaIntersecoes;
      }

      //--- As CN so podem cortar o espigão em 2 pontos um do lado esquerdo e outro do lado direito
      //--- As CNs que entram e saem pelo mesmo lado serão filtradas no verifica CN 
      //--- ERRADO!!!!!

      if (ContaIntersecoes > 2)
      {
        pInterErro.push_back(*ItInterVerificar);
      }
    }
  }

  if (pInterErro.size() > 0)
  {
    int Contador(1);

    pArqLog << "Houve erros na verificação da bacia:" << std::endl;
    pArqLog << "Existem curvas de nível cortando o espigão várias vezes:" << std::endl;
    pArqLog << "Verifique as curvas de nível nos pontos:" << std::endl;

    pArqLog << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

    for (ittylstIntersecoes ItInter = pInterErro.begin() ; ItInter != pInterErro.end() ; ++ItInter)
    {
      pArqLog << Contador++ << " : ";
      ItInter->PontoIntersecao >> pArqLog;
      pArqLog << std::endl;
    }

    pArqLog << "Pode haver outras curvas de nível cortando o espigão em vários pontos, Verifique!!!" << std::endl;
    pArqLog << "Para corrigir: Acerte o espigão modo que não haja curvas de nivel cortando-o várias vezes" << std::endl;

    pArqLog.flush();
  }

  pInterErro.clear();

  return pInterErro.size() == 0;
}

void CBacia::GravaLog(const std::string& strMsg, std::ostream& Stream, bool ForcarHora)
{
  CTime Agora(CTime::GetCurrentTime());
  static CTime GravAnterior(0);
  std::stringstream strstreamTemp;

  strstreamTemp << std::right << std::fixed << std::setfill('0') << std::setprecision(2);

  if (ForcarHora || CTimeSpan((Agora - GravAnterior)).GetTotalSeconds() > 30L)
  {
    strstreamTemp << "====> " << Agora.GetDay() << '/' << Agora.GetMonth() << '/' << Agora.GetYear() << ' ' << Agora.GetHour() << ':' << Agora.GetMinute() << ':' << Agora.GetSecond() << ' ' << std::ends;
    Stream << strstreamTemp.str();
    GravAnterior = Agora;
  }

  Stream << strMsg.c_str() << std::endl;
}

bool CBacia::VerfDescidaTalveguePrincipal()
{
  ItLDeltaSup ItAnt(ItTalveguePrincipal->begin()), ItAtual(ItTalveguePrincipal->begin());

  bool Descendo(ItTalveguePrincipal->begin()->PCentral.z > ItTalveguePrincipal->rbegin()->PCentral.z),Erro(false);

  ++ItAtual;

  do
  {
    if((!Descendo && (ItAtual->PCentral.z - ItAnt->PCentral.z) < (-(double)EquidisCNHistog))) 
    {
      Erro = true;
    }

    ++ItAtual;
    ++ItAnt;

  }while(!Erro && ItAtual != ItTalveguePrincipal->end());

  return !Erro;
}

bool CBacia::CalculaCotaFoz(ItLDeltaSup itFoz,ItLLDeltaSup itIalvegue)
{
  ItLDeltaSup itAnt(itFoz),itPos(itFoz);

  itAnt--;
  itPos++;

  if (itAnt != itIalvegue->end() && itPos != itIalvegue->end())
  {
    double DistTotal(hypot(itAnt->PCentral.x - itPos->PCentral.x, itAnt->PCentral.y - itPos->PCentral.y)),
           DistFoz(hypot(itAnt->PCentral.x - itFoz->PCentral.x, itAnt->PCentral.y - itFoz->PCentral.y)),
           i((itPos->PCentral.z - itAnt->PCentral.z) / DistTotal);

    itFoz->PCentral.z = DistFoz * i + itAnt->PCentral.z;

    return true;
  }

  return false;
}

void CBacia::RemoveFoz()
{
  itlstitsetPontos itPontoEspigao(EspigaoBacia.LstItPontos.begin());
  bool Achou(false);

  while(!Achou && itPontoEspigao != EspigaoBacia.LstItPontos.end())
  {
    if (**itPontoEspigao == Foz)
    {
      EspigaoBacia.LstItPontos.remove(*itPontoEspigao);

      Achou = true;
    }
    else
      itPontoEspigao++;
  }

  /*
  auto ItFoz(EspigaoBacia.SetPontosAciTopog.find(Foz));

  if (ItFoz != EspigaoBacia.SetPontosAciTopog.end())
  {
    EspigaoBacia.SetPontosAciTopog.erase(ItFoz);
  }
  */

  Foz = Ponto();
}

int CBacia::InsereFozNoEspigao(CAciTopografico& pEspigaoOrdenado, const Ponto& pFoz)
{
  bool PassouPelaFoz(false);
  itlstitsetPontos ItPInicial(pEspigaoOrdenado.LstItPontos.begin()),ItPPosterior(ItPInicial);
  ritlstitsetPontos ItrPAnterior(pEspigaoOrdenado.LstItPontos.rbegin());
  Ponto PFoz;

  //--- Ajusta a foz como o ponto mais baixo do espigão (1o (e último) ponto da lista)

  if (pEspigaoOrdenado.LstItPontos.size() > 3)
  {
    if (((*ItPInicial)->Baliza2 & FOZ) == FOZ)   //--- O primeiro ponto já é a foz
    {
      return true;   //--- A foz já está no inicio do espigão
    }
    else
    {
      ItrPAnterior++;
     
      //--- Pula as interseçoes das CNs com o espigão, pula a foz tb

      while (ItrPAnterior != pEspigaoOrdenado.LstItPontos.rend() &&
             ((*ItrPAnterior)->Baliza2 & FOZ) != FOZ)
      {
        ItrPAnterior++;
      }

      if (ItrPAnterior != pEspigaoOrdenado.LstItPontos.rend() &&
          ((*ItrPAnterior)->Baliza2 & FOZ) == FOZ)
      {
        PFoz = **ItrPAnterior;
        PassouPelaFoz = true;

      }

      if (PassouPelaFoz)
      {
        //--- Ajusta a foz, colocando-a como o primeiro ponto do espigão:
        //--- Teoricamente a foz deve ser o ponto mais baixo do espigão, mas na prática ela está na aresta que contém o ponto mais 
        //--- baixo do espigão. Sendo o ponto mais baixo do espigão ela deve ser o primeiro ponto da lista de pontos do espigão.
        // 
        //--- Se a foz estiver antes do primeiro ponto do espigão:
        //--- Move os pontos entre a foz e o primeiro ponto para o início da lista (a foz ficará sendo o primeiro ponto da lista)
        //--- Ajusta o  último ponto (que deve ser igual ao primeiro - lista circular) fazendo-o apontar para a foz q agora é o primeiro ponto da lista.
        //---
        //--- Se a foz estiver depois do primeiro ponto do espigão:
        //--- Move os pontos entre o primeiro e a foz para o fim da lista (a foz ficará sendo o primeiro ponto da lista)
        //--- Ajusta o  último ponto (que deve ser igual ao primeiro - lista circular) fazendo-o apontar para a foz q agora é o primeiro ponto da lista.
        //

        ritlstitsetPontos Itr(pEspigaoOrdenado.LstItPontos.rbegin());

        Itr++;   //--- = ao primeiro ponto (lista circular)

        while (**Itr != PFoz)    //--- Pode haver CNs entre o primeiro ponto e a foz, porisso o while
        {
          pEspigaoOrdenado.InserePonto(pEspigaoOrdenado.LstItPontos.begin(), **Itr);
        }
      
        pEspigaoOrdenado.InserePonto(pEspigaoOrdenado.LstItPontos.begin(), **Itr);

        return true;
      }
      else
      {

        monolog.mensagem(-1,"A foz não foi encontrada.");

        return false;
      }
    }
  }

  monolog.mensagem(-1, "Espigão inválido.");

  return false;
}

bool CBacia::VerificaCnsCortandoTalveguePrincipal(void* pSuperficie)
{
  double MaiorCotaTalvegue(-INFINITO),MenorCotaTalvegue(INFINITO);

  for(auto& PontoTalvegue : *ItTalveguePrincipal)
  {
    if (PontoTalvegue.PCentral.z != INFINITO)
    {
      if(PontoTalvegue.PCentral.z < MenorCotaTalvegue)
      { 
        MenorCotaTalvegue = PontoTalvegue.PCentral.z;
      }

      if(PontoTalvegue.PCentral.z > MaiorCotaTalvegue)
      {
        MaiorCotaTalvegue = PontoTalvegue.PCentral.z;
      }
    }
  }

  if (MaiorCotaTalvegue != INFINITO && MenorCotaTalvegue != INFINITO)
  {
    Ponto PIntersecao;
    CSuperficie* ppSuperficie((CSuperficie*)pSuperficie);

    auto ItPontoTalvegueAnterior(ItTalveguePrincipal->begin()), ItPontoTalvegueAtual(ItPontoTalvegueAnterior);
    std::set<double> SetCotasCortadas;

    for (ItPontoTalvegueAtual++; ItPontoTalvegueAtual != ItTalveguePrincipal->end(); ItPontoTalvegueAnterior++, ItPontoTalvegueAtual++)
    {
      for (ItLLPontos ItCNAtual = ppSuperficie->LPontosCN.begin(); ItCNAtual != ppSuperficie->LPontosCN.end(); ItCNAtual++)
      {
        if (ItCNAtual->begin()->z >= MenorCotaTalvegue && ItCNAtual->begin()->z <= MaiorCotaTalvegue)
        {
          ItLPontos ItPontoCNAnterior(ItCNAtual->begin()), ItPontoCNAtual(ItPontoCNAnterior);

          for (ItPontoCNAtual++; ItPontoCNAtual != ItCNAtual->end(); ItPontoCNAnterior++, ItPontoCNAtual++)
          {
            if (CLPontos::CalculaIntersecao(ItPontoTalvegueAnterior->PCentral, ItPontoTalvegueAtual->PCentral, *ItPontoCNAnterior, *ItPontoCNAtual, PIntersecao))
            {
              //--- Verifica a possibilidade remota de um ponto do talvegue ser o mesmo ponto da CN
              //--- Neste caso desconsidera a interseção
              //--- PRECISA ESTUDAR MAIS ESTE CASO: MESMO PASSANDO POR UM PONTO DO TALVEGUE A CN PODE VOLTAR

              double dx(fabs(PIntersecao.x - ItPontoTalvegueAnterior->PCentral.x)), dy(fabs(PIntersecao.y - ItPontoTalvegueAnterior->PCentral.y));

              //if (dx > 1E-4 || dy > 1E-4)
              if(true)
              {
                dx = fabs(PIntersecao.x - ItPontoTalvegueAtual->PCentral.x), dy = fabs(PIntersecao.y - ItPontoTalvegueAtual->PCentral.y);

                //if (dx > 1E-4 || dy > 1E-4)
                if(true)
                {
                  if(0) if ((SetCotasCortadas.emplace(ItCNAtual->begin()->z)).second == false)
                  {
                    std::stringstream strstrMensg;

                    strstrMensg << "A Curva de nível na cota " << ItCNAtual->begin()->z << " está cortando o talvegue principal mais de uma vez.";
                    monolog.mensagem(-1, strstrMensg.str().c_str());

                    //AfxMessageBox(strstrMensg.str().c_str());

                    return false;
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  return true;
}

double CBacia::PegaEquidisCN(void* pSuperf)
{
  CFile ArqCN;
  CFileException e;

  std::string NomeArqCN(((CSuperficie*)pSuperf)->PegaNomeProjeto().c_str());
  NomeArqCN += ".cnd";

  if (ArqCN.Open(NomeArqCN.c_str(), CFile::modeRead, &e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound)
    {
      return ((CSuperficie*)pSuperf)->PegaEquidisCN();
    }
  }

  CArchive CArcCurvasNivel(&ArqCN, CArchive::load);

  if (e.m_cause != CFileException::fileNotFound)
  {
    double EquidisCN, AngRotacaoBordos;

    CArcCurvasNivel >> AngRotacaoBordos >> EquidisCN;

    return EquidisCN;
  }
  
  return INFINITO;  
}

