#include "stdafx.h"
#include <list>
#include <map>
#include "CPonto.h"
#include <tuple>
#include "CAlinhamento.h"
#include "CAlinhamentos.h"
#include <set>
#include "Monologo.h"
#include "CSegmento.h"
#include "CDeltaSuperficie.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CAcidentesTopograficos.h"
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CLPontos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "Estaca.h"
#include "CEquipDrenagem.h"
#include "Superficie.h"
#include "CalcSU\CalcSU.h"
#include "CalcSU\CCCalcSU.h"
#include "CArqHPavimento.h"
#include "CHPavimento.h"
#include "CCalcTerraplProjeto.h"
#include "CalcSU\CGeraSuperficieProjTerrapl.h"
#include <sstream>
#include <iomanip>
#include "mat.h"

#define GRAVAR 2
#define ESQUERDO 0
#define DIREITO 1 
#define ATERRO 0
#define CORTE 1
#define FRONTEIRA_TERRAPLANAGEM 1
#define FRONTEIRA_SUPERF 0

extern class monologo  monolog;
extern Matem Mat;

CCalcTerraplProjeto::CCalcTerraplProjeto(const std::string& pNomeProjeto, CProgressCtrl& pCtrlProgresso, CStatic& pTexto, std::string& NomeProjBasico,int TipoProj) : NomeProjeto(pNomeProjeto), CstaTexto(pTexto), CtrlProgresso(pCtrlProgresso), NomeProjetoBasico(NomeProjBasico),
Limpeza(CString(pNomeProjeto.c_str()), CString(".lim")),TipoProjeto(TipoProj), SetAlinhamentosVariaveis{ "BORTEE", "BORTED","BORPIE","BORPID","OFFSEE", "OFFSED","LIMPEE","LIMPED","CLIMPE","CLIMPD","TALCTD","TALCTE","TALATE","TALATD"}
{
  Erro = LePontosTerraplProjeto(pNomeProjeto);
  CSuperficie Superficie(CString(NomeProjBasico.c_str()), true);
  AnguloRotacao = Superficie.PegaAngRotacaoBordos();
  Superficie.PegaListaFronteira(LFronteiraSuperf, FRONTEIRA_SUPERF);
  Erro |= LePontosPassagem();
}

CCalcTerraplProjeto::~CCalcTerraplProjeto()
{
}

bool CCalcTerraplProjeto::LePontosTerraplProjeto(const std::string& NomeProjeto)
{
  CFileException e;
  CFile ArqPonTerPro;

  if (ArqPonTerPro.Open(std::string(NomeProjeto + ".ptp").c_str(), CFile::modeRead | CFile::shareDenyNone, &e) == 0)   //--- Pontos Terraplanagem Projeto
  {
    monolog.mensagem(13, std::string(NomeProjeto + ".ptp. A nota de serviço foi calculada?...").c_str());

    return false;
  }

  CArchive CArqPonTerPro(&ArqPonTerPro, CArchive::load);

  MapaPontosTerrlProjeto.clear();
  size_t QuanSecoes(0);

  for (CArqPonTerPro >> QuanSecoes; QuanSecoes; QuanSecoes--)
  {
    Ponto Estaca;

    Estaca << CArqPonTerPro;
    pairittymapEstlstPontosBool pairitPontosTerr(MapaPontosTerrlProjeto.insert(std::pair<Ponto, lstPontos>(Estaca, lstPontos())));

    size_t QuanPontos(0);
    Ponto PontoAtual;

    for (CArqPonTerPro >> QuanPontos; QuanPontos; QuanPontos--)
    {
      PontoAtual << CArqPonTerPro;
      pairitPontosTerr.first->second.push_back(PontoAtual);
    }
  }

  return true;
}

#include "CConfigProj.h"

int CCalcTerraplProjeto::CalculaTerraplProjeto(int ProjGeoReferenciado,int DensidadeSimbologia,int TipoProjeto)
{
  if (Erro == 0)
  {
    CstaTexto.SetWindowText("Houve erros na inicialização, os cálculos não foram executados.");
    return 1;
  }

  //--- Define os alinhamentos da terraplanagem

  CstaTexto.SetWindowText("Preparando a terraplanagem para cálculo...");

  for (ittymapEstlstPontos it = MapaPontosTerrlProjeto.begin(); it != MapaPontosTerrlProjeto.end(); it++)
  {
    CAlinhamento AlinAtual(it->first.x, it->first.y, std::string(""),it->first.z < 0.0);

    for (ittymapAlinhamentosAtivos italn = Alinhamentos.MapAlinhamentosAtivos.begin(); italn != Alinhamentos.MapAlinhamentosAtivos.end(); ++italn)
    {
      italn->second->second.Ativo = false;
    }

    for (itlstPontos itPontos = it->second.begin(); itPontos != it->second.end(); ++itPontos)
    {
      ittymapAlinhamentosAtivos itAlinhamentoAtual;

      //--- Bordo de canteiro central(BCCnnE/D) nn número do canteiro central, 01, 02, 03...
      //--- É variavel pois o valor de x muda nas superelevações (não pôde ser incluido no set de 
      //--- alinhamentos variaveis porcausa do número nn que pode variar de 01 a 99. Podem existir 99 canteiros centrais em um trecho
      //--- idem para os pontos de alargamentos

      if (SetAlinhamentosVariaveis.find(std::string(itPontos->Descricao.GetBuffer())) != SetAlinhamentosVariaveis.end() || itPontos->Descricao.Find("BOCC") != -1 || itPontos->Descricao.Find("ALARG") != -1 || itPontos->Descricao.Find("TAL") != -1)
      {
        itAlinhamentoAtual = PegaAlinhamentoPorDescricao(std::string(itPontos->Descricao.GetBuffer()));
      }
      else
      {
        itAlinhamentoAtual = Alinhamentos.MapAlinhamentosAtivos.find(itPontos->i);

        //--- Se mudou a seção tipo é possivel que fique alinhamentos diferentes no mesmo x, verifica, 
        //--- se for o caso finaliza o atual inicia outro alinhamento no novo x (Proximo if)

        if (itAlinhamentoAtual != Alinhamentos.MapAlinhamentosAtivos.end())       //--- Achou um alinhamento no mesmo x?
        {
          if (itAlinhamentoAtual->second->first.PegaDescricao().compare(itPontos->Descricao.GetBuffer()) != 0)   //--- São diferentes? se sim so pode estar mudando a seção-tipo
          {
            //--- finaliza o anterior e inicia o atual

            Alinhamentos.MapAlinhamentosAtivos.erase(itAlinhamentoAtual);
            itAlinhamentoAtual = Alinhamentos.MapAlinhamentosAtivos.end();   //-- inicial um novo alinhamento 
          }
        }
      }

      CAlinhamento AlinAtual(it->first.x, itPontos->i, std::string(itPontos->Descricao.GetBuffer()), it->first.z < 0.0);

      if (itAlinhamentoAtual == Alinhamentos.MapAlinhamentosAtivos.end())
      {
        typairtymapCAlinhamentosBool PairAlinhamentos(Alinhamentos.MapAlinhamentos.insert(typairchavealinCAlinhamentos(AlinAtual.PegaChave(), AlinAtual)));
        typairittymapAlinhamentosAtivosbool PairAlinhamentosAtivos(Alinhamentos.MapAlinhamentosAtivos.insert(typairdoubleittymapAlinhamentos(itPontos->i, PairAlinhamentos.first)));
        itAlinhamentoAtual = PairAlinhamentosAtivos.first;
      }

      //--- em z i e fi estão as cotas vermelhas
      //--- Passa-as para a baliza para na simbologia saber se o ponto pertence a um talude de 
      //--- Corte ou aterro 

      itPontos->Baliza = it->first.fi < 0.0;     //--- Marca Offset Dir de corte ou aterro
      itAlinhamentoAtual->second->second.InserePonto(*itPontos);
      itAlinhamentoAtual->second->second.Ativo = true;
    }

    for (ittymapAlinhamentosAtivos italn = Alinhamentos.MapAlinhamentosAtivos.begin(); italn != Alinhamentos.MapAlinhamentosAtivos.end(); )
    {
      if (italn->second->second.Ativo == false) Alinhamentos.MapAlinhamentosAtivos.erase(italn++);
      else ++italn;
    }
  }
  
  InserePontosPassagemBordos();

  CstaTexto.SetWindowText("Preparando as seções...");

  double AreaTerrap(0.0);

  if (DefineFronteiraTerr(AreaTerrap) == 1)
  {
    CstaTexto.SetWindowText("Houve erro, quantidade insuficiente de seções (< 2) ou existem seções sem off-sets...");
    return 3;
  }

  Gravar();

  CstaTexto.SetWindowText("Definindo as seções de terraplanagem...");

  std::stringstream Msg;

  Msg << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

  if (Alinhamentos.LstPontosBordo.size() < 4)
  {
    CstaTexto.SetWindowText("Não há ou seções suficientes com off-sets...");
    return 2;
  }

  CString NomeProjTerrap(NomeProjeto.c_str());
  CSuperficie Superficie(CString(NomeProjetoBasico.c_str()));

  std::ofstream ArqLog(NomeProjeto + "_ProjTerraplanagem.rel", std::ofstream::out | std::ofstream::trunc);

  if (ArqLog.good() == false) CstaTexto.SetWindowText("Erro ao abrir o arquivo de log. Programa abortado.");
  else
  {
    SSetPontos SetPontosProblema;

    if (VerfProjTerrap(Superficie, SetPontosProblema, ArqLog,AreaTerrap) == 1)  //--- 1 OK
    {
      tylstpontos ListaFronteira;

      MudaTipoListaFronteira(ListaFronteira);                //---- Passa a fronteira para uma lista comum de pontos, para poder ser usada no cálculo da superfície

      CConfigProj ConfigProj(NULL,NomeProjetoBasico.c_str());
      ConfigProj.LeArquivo();
      std::string FormatoArqPontos(ConfigProj.FormArqPontos);
      
      if (CCalcSU::CalculaSUProjTerrpl(NomeProjTerrap, &CstaTexto, &CtrlProgresso, 0, Alinhamentos.LstPontosBordo, SetPontosProblema,AreaTerrap, ListaFronteira, FormatoArqPontos,ProjGeoReferenciado, TipoProjeto) != 1)
      {
        Msg << "Houve erro na definição da terraplanagem, a Terraplanagem NÃO foi gerada com êxito. Verifique o arquivo de log.";
        CstaTexto.SetWindowText(Msg.str().c_str());
        Msg << "Verifique próximo aos pontos:" << std::endl;

        int C(0);
        auto itpb(SetPontosProblema.begin());

        while(C++ < 25 && itpb != SetPontosProblema.end())
        {
          Msg << itpb->x << " " << itpb->y << std::endl;
          itpb++;
        }

        Msg << std::ends;

        GravaLog(Msg.str(), ArqLog);
     
        return 1;
      }
      else
      {
        CtrlProgresso.SetPos(75);
       
        CSuperficie SuperficieTerrap(NomeProjTerrap);

        //--- Se o projeto é georeferenciado (DRONES) interpola as curvas de nível somente dentro da terraplanagem
        //--- Não se pode usar a triangulação das CN porque devido as cotas serem inteiras ocorre muita diferenca nas CN por
        //--- causa dos erros de precisão
        //--- Se o projeto não for georeferenciado insere a fronteira da superficie pois neste caso as CN da superficie são ligadas as da 
        //--- Terraplanagem
        
        if(TipoProjeto == -1 || TipoProjeto == 4 || ProjGeoReferenciado == 1 || ProjGeoReferenciado == 3)   //--- 1 = SUPERF_DRONES
        {
          LDeltaSup ListaDeltaSupFronteira;

          for (register ittylstpontos it = Alinhamentos.LstPontosBordo.begin() ; it != Alinhamentos.LstPontosBordo.end() ; it++)
          {
            ListaDeltaSupFronteira.push_back(CDeltaSuperficie(*it,CAciTopografico::FRONTEIRA_TERRAP));
          }

          ItLLDeltaSup itLLDS = SuperficieTerrap.PegaListaRestricoes().insert(SuperficieTerrap.PegaListaRestricoes().end(),ListaDeltaSupFronteira);

          SuperficieTerrap.InsereListaNoMapa(itLLDS, CAciTopografico::FRONTEIRA_TERRAP);
        }

        //--- Se for drones a fronteira é o offset da terraplanagem porque nos drones não dá para mixar as 2 
        //--- superficies porque a superficie de drones tem muitos ponto e as CN são muito complexas
        //--- não emenda direito.
        //--- ProjGeoreferenciado = Drones

        if (SuperficieTerrap.DefineFronteiraOuPoligono(CSuperficie::TIPOFRONTEIRA, ProjGeoReferenciado, ProjGeoReferenciado == 1 || ProjGeoReferenciado == 3,false) == TRUE)
        {
          Msg.str("");
          Msg << "Definindo as curvas de nível..." << std::ends;
          CstaTexto.SetWindowText(Msg.str().c_str());

          GravaLog(Msg.str(), ArqLog);

          double DouEquidisCN(1.0);

          SuperficieTerrap.AtribuiEquidisCN(DouEquidisCN);
          SuperficieTerrap.Serialize(NomeProjeto.c_str(), GRAVAR, false, false, false);

          if (SuperficieTerrap.InterpolarCotas(SuperficieTerrap.pSuperficieAtual->begin(), SuperficieTerrap.pSuperficieAtual->end(), CtrlProgresso, SetPontosProblema, DouEquidisCN, TipoProjeto) == true)
          {
            Msg.str("");
            Msg << "Definindo a simbologia da terraplanagem." << std::ends;
            GravaLog(Msg.str(), ArqLog);
            CstaTexto.SetWindowText(Msg.str().c_str());

            DefineSimbologiaTerrap(Alinhamentos,DensidadeSimbologia);

            FechaExtremosTerrap(SuperficieTerrap);

            CalculaCNSimbologia(SuperficieTerrap);

            SuperficieTerrap.Serialize(NomeProjeto.c_str(), GRAVAR, false, true);

            Msg.str("");
            Msg << "O Projeto de terraplanagem foi gerado com êxito." << std::ends;
            GravaLog(Msg.str(), ArqLog);

            CstaTexto.SetWindowText(Msg.str().c_str());

           // pParametros->CDial->GetDlgItem(IDOK)->EnableWindow(true);
           //  pParametros->CDial->GetDlgItem(IDC_BUTPAUSAR)->EnableWindow(false);
           //  pParametros->EventoFim->SetEvent();

            return 0;   //--- retorno OK
          }
          else
          {
            Msg << "Houve erro na definição das curvas de nível. Existem pontos próximos com uma diferença de cotas muito elevadas ( > 100.00m), Verifique o arquivo de log.";
            CstaTexto.SetWindowText(Msg.str().c_str());
            Msg << "Verifique próximo aos pontos:" << std::endl
                << SetPontosProblema.begin()->x << " " << SetPontosProblema.begin()->y << " " << SetPontosProblema.begin()->z << std::ends;

            GravaLog(Msg.str(), ArqLog);
          }
        }
        else
        {
          SuperficieTerrap.Serialize(NomeProjeto.c_str(), GRAVAR, false, true);
          Msg.str("");
          Msg << "Houve erro na interpolação das cotas, a superfície não foi gerada ou está inválida." << std::ends;
          CstaTexto.SetWindowText(Msg.str().c_str());
          GravaLog(Msg.str(), ArqLog);
        }
      }
    }
    else
    {
      CSuperficie SuperficieTerrap(NomeProjTerrap);
      SuperficieTerrap.Serialize(NomeProjeto.c_str(), GRAVAR, false, true);
      CstaTexto.SetWindowText("Houve erro no cálculo da terraplanagem, verifique o arquivo de log.");
    }
  }

  return 1;
}

ittymapAlinhamentosAtivos CCalcTerraplProjeto::PegaAlinhamentoPorDescricao(const std::string& pDescricao)
{
  for (ittymapAlinhamentosAtivos italn = Alinhamentos.MapAlinhamentosAtivos.begin(); italn != Alinhamentos.MapAlinhamentosAtivos.end(); ++italn)
  {
    if (italn->second->first.PegaDescricao().compare(pDescricao) == 0) return italn;
  }

  return Alinhamentos.MapAlinhamentosAtivos.end();
}

int CCalcTerraplProjeto::DefineFronteiraTerr(double& AreaTerrap)
{
  Alinhamentos.LstPontosBordo.clear();

  typlstPontos* pLstOffEsq(NULL),* pLstOffDir(NULL);

  for (ittymapCAlinhamentos itAlin = Alinhamentos.MapAlinhamentos.begin(); itAlin != Alinhamentos.MapAlinhamentos.end() && (pLstOffEsq == NULL || pLstOffDir == NULL) ; itAlin++)
  {
    if (itAlin->first.PegaDescricao() == "OFFSEE" && !pLstOffEsq) pLstOffEsq = &itAlin->second.LstPontos;
    else  if (itAlin->first.PegaDescricao() == "OFFSED" && !pLstOffDir) pLstOffDir = &itAlin->second.LstPontos;
  }

  if (pLstOffEsq && pLstOffDir)  //--- Tem que ter os 2 off sets, erro erro será consistido em detalhe na função VerfProjTerrap()
  {
    Alinhamentos.LstPontosBordo = *pLstOffEsq;     //--- Insere o bordo esquerdo
    Alinhamentos.LstPontosBordo.insert(Alinhamentos.LstPontosBordo.end(), pLstOffDir->rbegin(), pLstOffDir->rend());   //--- Insere o bordo direito
    Alinhamentos.LstPontosBordo.push_back(*(*Alinhamentos.MapAlinhamentos.begin()).second.LstPontos.begin());          //--- Fecha repetindo o primeiro ponto  
  }

  if (Alinhamentos.LstPontosBordo.size() < 4) return 1;

  AreaTerrap = 0.0;

  ittylstpontos itAlnAnterior(Alinhamentos.LstPontosBordo.begin()), itAlnAtual(itAlnAnterior);

  for (itAlnAtual++; itAlnAtual != Alinhamentos.LstPontosBordo.end(); itAlnAtual++)
  {
    AreaTerrap += (itAlnAtual->y + itAlnAnterior->y) * (itAlnAtual->x - itAlnAnterior->x) / 2.0;
    itAlnAnterior = itAlnAtual;

  }

  return 0;
}

int CCalcTerraplProjeto::Gravar()
{
  CFileException e;
  CFile ArqTerPro;

  if (ArqTerPro.Open(std::string(NomeProjeto + ".tpr").c_str(), CFile::modeCreate | CFile::modeWrite, &e) == 0)   //--- Arquivo Terraplanagem Projeto
  {
    monolog.mensagem(1, std::string(NomeProjeto + ".tpr").c_str());

    return 1;
  }

  CArchive CArqPonTerPro(&ArqTerPro, CArchive::store);

  Alinhamentos.Serialize(CArqPonTerPro);

  return 0;
}

int CCalcTerraplProjeto::VerfProjTerrap(CSuperficie& SuperfTerrpl, SSetPontos& PontosProblema, std::ofstream& ArqLog,double AreaTerrap)
{
  GravaLog(NomeProjeto, ArqLog);
  GravaLog("====> Verificando o projeto de terraplanagem...", ArqLog);

  std::stringstream Msg;
  Msg << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

  //--- Verifica se todas as seções têm off-sets.

  int Retorno(0);

  if ((Retorno = VerificaOffSets(PontosProblema)) == 1)    //--- 1 OK.
  {
    //--- Verifica se há pontos da fronteira dentro da terraplanagem  
    //--- Se houver não há pb!!! 30122021  em teste
    //--- Há sim !!! 01012022 ===>>> Verificar porque... não era pra ser.

    return 1;

    if ((Retorno = VerfFrontDentroTerraplanagem(LFronteiraSuperf, Alinhamentos.LstPontosBordo, PontosProblema,AreaTerrap)) == 1)   //--- 1 Ok.
    {
      if (VerfFrontCruzTerraplanagem(LFronteiraSuperf, Alinhamentos.LstPontosBordo, PontosProblema, SuperfTerrpl) == 1)  //--- 1  OK.
      {
        GravaLog("====> Verificações finalizadas.", ArqLog);

        return 1;   //--- 1 OK
      }
      else
      {
        //--- Se cruzar não há pb!!! 30122021  em teste
         //--- Há sim !!! 01012022 ===>>> Verificar porque... não era pra ser.

     //   return 1;

        Msg.str("");
        Msg << " Há segmentos da fronteira cruzando a terraplanagem, verifique nas seções próximas aos pontos : " << std::ends;
        GravaLog(Msg.str(), ArqLog);
        GravaLogPontosProblema(PontosProblema, ArqLog, SuperfTerrpl);
      }
    }
    else
    {
      Msg.str("");

      switch (Retorno)
      {
        case 2:
        {
          Msg << " Não há seções suficientes para executar os cálculos." << std::ends;
          return 0;
        }
        break;
        case 3:
        {
          Msg << " Há um problema nos off-sets." << std::ends;
          GravaLog(Msg.str(), ArqLog);

          return 0;
        }
        break;
        case 4:
        {
          Msg << " A fronteira da superfície não existe ou está incompleta." << std::ends;
          GravaLog(Msg.str(), ArqLog);

          return 0;
        }
        break;
      }
   
      //--- Verifica se há pontos da fronteira dentro da terraplanagem  
      //--- Se houver não há pb!!! 30122021  em teste
       //--- Há sim !!! 01012022 ===>>> Verificar porque... não era pra ser.


  //    return 1;@???????
  //--- afinal isto é ou não é um pb @Verificar!!!!!

  
      Msg.str("");
      Msg << " Há pontos da fronteira da superficie dentro da terraplanagem, verifique nas seções próximas aos pontos" << std::ends;
      GravaLog(Msg.str(), ArqLog);
      GravaLogPontosProblema(PontosProblema, ArqLog, SuperfTerrpl);
    }
  }
  else
  {
    Msg.str("");
    Msg << " Há seções sem off-sets, verifique nas estacas abaixo." << std::ends;
    GravaLog(Msg.str(), ArqLog);
    GravaLogPontosProblema(PontosProblema, ArqLog, SuperfTerrpl,false,true);
  }

  return 0;
}

int CCalcTerraplProjeto::VerfFrontDentroTerraplanagem(const LDeltaSup& FronteiraSuperficie, const tylstpontos& FronteiraTerraplanagem, SSetPontos& PontosProblema,double Area)
{
  //--- Remove do terreno os pontos do terreno que estão entro da terraplanagem
  //--- Retorno: 0 não ok, 1 ok, 2 3 4 5 6 => erro

  if (FronteiraTerraplanagem.size() < 4) return 2;
  if (*FronteiraTerraplanagem.begin() != *FronteiraTerraplanagem.rbegin()) return 3;
  if (FronteiraSuperficie.size() < 4) return 4;

  unsigned int Contador(0);
  PontosProblema.clear();

  for (ItCLDeltaSup itFrontSuperf = FronteiraSuperficie.begin(); itFrontSuperf != FronteiraSuperficie.end() && PontosProblema.size() < 50; itFrontSuperf++)
  {
    cittylstcponto itFrontAnterior(FronteiraTerraplanagem.begin()), itFrontAtual(itFrontAnterior);

    Contador = 0;

    for (++itFrontAtual; itFrontAtual != FronteiraTerraplanagem.end(); ++itFrontAtual)
    {
      if ((Area > 0.0 && itFrontAnterior->x < itFrontSuperf->PCentral.x && itFrontAtual->x < itFrontSuperf->PCentral.x) ||
          (Area < 0.0 && itFrontAnterior->x > itFrontSuperf->PCentral.x && itFrontAtual->x > itFrontSuperf->PCentral.x))          //--- Se girar para a esquerda...
      {
        if ((itFrontSuperf->PCentral.y > itFrontAnterior->y && itFrontSuperf->PCentral.y < itFrontAtual->y) ||
          (itFrontSuperf->PCentral.y < itFrontAnterior->y && itFrontSuperf->PCentral.y > itFrontAtual->y))
        {
          ++Contador;
        }
      }
      else
      {
        if ((itFrontAnterior->x < itFrontSuperf->PCentral.x && itFrontAtual->x > itFrontSuperf->PCentral.x) || (itFrontAnterior->x > itFrontSuperf->PCentral.x && itFrontAtual->x < itFrontSuperf->PCentral.x))
        {
          if ((itFrontAnterior->y > itFrontSuperf->PCentral.y && itFrontAtual->y < itFrontSuperf->PCentral.y) || (itFrontAnterior->y < itFrontSuperf->PCentral.y && itFrontAtual->y > itFrontSuperf->PCentral.y))
          {
            Ponto PontoIntersecao;

            if (CLPontos::CalculaIntersecao(*itFrontAnterior, *itFrontAtual, itFrontSuperf->PCentral, Ponto(itFrontSuperf->PCentral.x + 10000.0, itFrontSuperf->PCentral.y), PontoIntersecao, false) == TRUE)
            {
              if ((Area > 0.0 && PontoIntersecao.x < itFrontSuperf->PCentral.x) || (Area < 0.0 && PontoIntersecao.x > itFrontSuperf->PCentral.x))
              {
                ++Contador;
              }
            }
          }
        }
      }

      itFrontAnterior = itFrontAtual;
    }

    if (Contador % 2 != 0)
    {
      PontosProblema.insert(itFrontSuperf->PCentral);
    }
  }

  return (PontosProblema.size() > 0) ? 6 : 1;    //--- 1 = OK, 6 = Há problemas
}

int CCalcTerraplProjeto::VerfFrontCruzTerraplanagem(const LDeltaSup& FronteiraSuperficie, const tylstpontos& FronteiraTerraplanagem, SSetPontos& PontosProblema, CSuperficie& SuperficieTerrap)
{
  Ponto PontoIntersecao;
  cittylstcponto itFrontTerrAnterior(FronteiraTerraplanagem.begin()), itFrontTerrAtual(itFrontTerrAnterior);

  PontosProblema.clear();

  for (++itFrontTerrAtual; itFrontTerrAtual != FronteiraTerraplanagem.end() && PontosProblema.size() < 50; ++itFrontTerrAtual)
  {
    if (SuperficieTerrap.VerificaIntersecaoAcidentesTopog(*itFrontTerrAnterior, *itFrontTerrAtual, PontoIntersecao, CAciTopografico::FRONTEIRA, false, true, NULL) == true)
    {
      PontosProblema.insert(PontoIntersecao);
    }

    itFrontTerrAnterior = itFrontTerrAtual;
  }

  return PontosProblema.size() == 0;
}

int CCalcTerraplProjeto::VerificaOffSets(SSetPontos& PontosProblema)
{
  for (ittymapEstlstPontos it = MapaPontosTerrlProjeto.begin(); it != MapaPontosTerrlProjeto.end() && PontosProblema.size() < 50; it++)
  {
    if (it->second.begin()->Descricao.Compare("OFFSEE") == -1 || it->second.rbegin()->Descricao.Compare("OFFSED") == -1)
    {
      PontosProblema.insert(it->first);
    }
  }

  return PontosProblema.size() == 0;
}

void CCalcTerraplProjeto::GravaLogPontosProblema(SSetPontos& PonProblema, std::ostream& Stream, CSuperficie& Superf, bool ForcarHora,bool Estaca)
{
  Ponto PontoDesrotacionado;
  int C(1);
  std::stringstream Msg;

  for (ItSSetPontos itPontosProblema = PonProblema.begin(); itPontosProblema != PonProblema.end() && C < 201; itPontosProblema++, C++)
  {
    Msg.str("");

    Msg << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

    double Cota(itPontosProblema->z);

    //  if(Superf.EstaRotacionada()) 
    //  {
    //    PontoDesrotacionado = itPontosProblema->RotacionaPlano(sin(-Superf.PegaAnguloRotacao()),cos(-Superf.PegaAnguloRotacao()),-Superf.PegaAnguloRotacao(),true);
    //   Msg << C << ' ' << PontoDesrotacionado.Nome << ", " << PontoDesrotacionado.y << ", " << PontoDesrotacionado.x;
    //  }
    //  else
    Msg << C << ' ' << itPontosProblema->Nome << ", " << itPontosProblema->y << ", ";
    
    if(!Estaca) Msg << itPontosProblema->x;
    else
    {
      CEstaca Est(itPontosProblema->x);
      Msg << Est.GetBuffer();
    }

    if (Cota != INFINITO)   Msg << ", " << Cota;

    Msg << std::ends;

    GravaLog(Msg.str(), Stream);
  }

  if (PonProblema.size() == 50)
  {
    Msg.str("");
    Msg << " Foram encontrados muitos erros, finalizando as verificações." << std::ends;
    GravaLog(Msg.str(), Stream);
  }
}

void CCalcTerraplProjeto::GravaLog(const std::string& strMsg, std::ostream& Stream, bool ForcarHora)
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

int CCalcTerraplProjeto::LePontosPassagem()
{
  CFile ArqPonPassagem;
  CFileException e;

  std::string NomeCompArquivo(NomeProjeto + ".pas");

  if (ArqPonPassagem.Open(NomeCompArquivo.data(), CFile::modeRead, &e) == 0)
  {
    monolog.mensagem(13, CString(NomeCompArquivo.data()) + " (Lendo os dados)");

    return false;
  }
  else
  {
    CArchive ar(&ArqPonPassagem, CArchive::load);

    for (int LADO = ESQUERDO; LADO <= DIREITO; LADO++)
    {
      size_t Tam(0);

      for (ar >> Tam; Tam; Tam--)
      {
        Ponto PAtual;

        PAtual << ar;
        PontosPassagem[LADO].push_back(PAtual);
      }
    }
  }

  return true;
}

void CCalcTerraplProjeto::InserePontosPassagemBordos()
{
  //--- Insere o ponto de passagem no offset e no bordo de terraplanagem, se a secao tiver alargamento o alargamento prevalece no lugar do bordo

  std::string strAlinhamentos[2][2] = { {"BORTEE","OFFSEE"},{"BORTED","OFFSED"} };

  for (int LADO = ESQUERDO; LADO <= DIREITO; LADO++)
  {
    for (itlstPontos it = PontosPassagem[LADO].begin(); it != PontosPassagem[LADO].end(); it++)
    {
      itlstPontos itAnterior, itAtual;
      bool Achou(false);
      ittymapCAlinhamentos italn;

      for (italn = Alinhamentos.MapAlinhamentos.begin(); italn != Alinhamentos.MapAlinhamentos.end() && !Achou; italn++)
      {
        //--- o alargamento prevalece sobre o bordo, então procura primeiro se tem alargamento

        if (LADO == ESQUERDO && italn->second.PegaChave().PegaDescricao().find("ALARGE") != std::string::npos ||
            LADO == DIREITO && italn->second.PegaChave().PegaDescricao().find("ALARGD") != std::string::npos)
        {
          itAnterior = italn->second.LstPontos.begin();
          itAtual = itAnterior;

          if(itAnterior->S < it->x)       //--- O alinhamento tem que começar antes da seçao atual
          {
            for (itAtual++; itAtual != italn->second.LstPontos.end() && itAtual->S < it->x; itAtual++)
            {
              itAnterior = itAtual;
            }

            if (itAtual != italn->second.LstPontos.end())
            {
              Achou = true; //--- Achou alargamento nesta estaca
              break;
            }
          }
        }
      }

      if (!Achou)   //--- Se não achou nos alargamentos procura nos bordos
      {
        for (italn = Alinhamentos.MapAlinhamentos.begin(); italn != Alinhamentos.MapAlinhamentos.end() && !Achou; italn++)
        {
          if (italn->second.PegaChave().PegaDescricao().compare(strAlinhamentos[LADO][0]) == 0)
          {
            itAnterior = italn->second.LstPontos.begin();
            itAtual = itAnterior;

            if(itAnterior->S < it->x)       //--- O alinhamento tem que começar antes da seçao atual
            {
              for (itAtual++; itAtual != italn->second.LstPontos.end() && itAtual->S < it->x; itAtual++)
              {
                itAnterior = itAtual;
              }

              if (itAtual != italn->second.LstPontos.end())
              {
                Achou = true;
                break;
              }
            }
          }
        }
      }

      if (Achou)     //--- Achou um alargamento ou um bordo
      {
        Ponto PPolar(Mat.R_P(*itAnterior, *itAtual)), PontoPassagem;

        //--- Nas curvas a distancia no eixo é diferente da distancia nos bordos, tem que calcular proporcional

        double DistEixo(itAtual->S - itAnterior->S),
               DistBordo(PPolar.y),                               //--- Nas curvas é a corda
          //--- it->x == Estaca no eixo do ponto de passagem, dist proporcional é a distancia do pp no bordo  
               DistProporcional = (DistBordo / DistEixo) * (it->x - itAnterior->S);

        //--- Os pontos de passagem não podem ficar muito próximos dos pontos dos offsets senão da problema na CGAL
        //--- Talvez seja um caso de interpolação não previsto, verificar

        if (DistBordo < 1.0) DistProporcional = DistBordo / 2.0;
        else
        {
          if (DistProporcional < 0.5) DistProporcional = 0.5;
          else if (DistBordo - DistProporcional < 0.5) DistProporcional = DistBordo - 0.5;
        }

        Mat.IrradiaPonto(PPolar.x, itAnterior->x, itAnterior->y, 0.0, DistProporcional, PontoPassagem);
        PontoPassagem.z = ((itAtual->z - itAnterior->z) / PPolar.y) * DistProporcional + itAnterior->z;
        PontoPassagem.Descricao = strAlinhamentos[LADO][1].c_str();
        PontoPassagem.Descricao += " - Ponto de Passagem";
        PontoPassagem.S = itAnterior->S + (it->x - itAnterior->S);       //--- A estaca é no eixo, não na distancia proporcional (bordo)

        italn->second.LstPontos.insert(itAtual, PontoPassagem);

        //--- Insere o ponto de passagem nos offsets

        bool AchouAlinOffSets(false);

        LADO == ESQUERDO ? italn-- : italn++;

        while (italn != Alinhamentos.MapAlinhamentos.end() && italn->second.PegaChave().PegaDescricao().compare(strAlinhamentos[LADO][1]) != 0)
        {
          LADO == ESQUERDO ? italn-- : italn++;
        }

        if (italn != Alinhamentos.MapAlinhamentos.end())
        {
          itlstPontos itAnterior(italn->second.LstPontos.begin()), itAtual(itAnterior);

          for (itAtual++; itAtual != italn->second.LstPontos.end() && itAtual->S < it->x; itAtual++)
          {
            itAnterior = itAtual;
          }

          if (itAtual != italn->second.LstPontos.end())
          {
            italn->second.LstPontos.insert(itAtual, PontoPassagem);
          }
        }
      }
    }
  }

  //--- Para desenhar cada alinhamento de off set com cores diferentes seria  preciso quebrar aqui as 2 listas de off sets 
  //--- em várias outras quando passar de corte para aterro. fica para uma outra versão
  //--- Ver CAlinhamentos.cpp
}

void CCalcTerraplProjeto::MudaTipoListaFronteira(tylstpontos& LstFronteira)
{
  LstFronteira.clear();

  for (register ItLDeltaSup it = LFronteiraSuperf.begin(); it != LFronteiraSuperf.end(); ++it)
  {
    LstFronteira.push_back(it->PCentral);
  }
}

void CCalcTerraplProjeto::DefineSimbologiaTerrap(const CAlinhamentos& Alinhamentos,int Densidade)
{
  int Lado(ESQUERDO), TipoTerr(ATERRO);
  bool Fim(false);

  lstPontos SimbologiaTerrap;

  tysetcittymapCAlinhamentos SetAlinFeitos;

  for (int Lado = ESQUERDO; Lado <= DIREITO; ++Lado)
  {
    cittymapCAlinhamentos citalnAnterior(Lado == ESQUERDO ? Alinhamentos.MapAlinhamentos.begin() : Alinhamentos.MapAlinhamentos.begin()),
                          citalnAtual(Alinhamentos.MapAlinhamentos.end());

    if (Lado == DIREITO)
    {
      while ((++citalnAnterior)->second.PegaChave().PegaDescricao().find("OFFSED") == std::string::npos);   //--- Posiciona no offset direito
    }

    SetAlinFeitos.push_back(citalnAnterior);  //--- Marca o offset como feito

    //--- O Alinhamento anterior é sempre o offset, o atual vai mudando a medida que vão entrando e saindo bermas entre
    //--- o offset e o bordo

    bool FezTodos(false), Bermas(false), Reiniciar(false);

    while (!FezTodos)    //--- Caso haja bermas pe preciso passar de novo para fazê-las
    {
      cittyplstPontos itPonAnteriorL1(citalnAnterior->second.LstPontos.begin()), itPonAtualL1(itPonAnteriorL1);

      ++itPonAtualL1;

      while (itPonAtualL1 != citalnAnterior->second.LstPontos.end() && (citalnAtual = ProcuraProxAlinhamento(itPonAtualL1, citalnAnterior, Lado)) != Alinhamentos.MapAlinhamentos.end())
      {
        SetAlinFeitos.push_back(citalnAtual);

        cittyplstPontos itPonAnteriorL2(citalnAtual->second.LstPontos.begin()), itPonAtualL2(itPonAnteriorL2);

        //--- Se estiver voltando para um alinhamento anterior posiciona corretamente os iteradores;

        while (itPonAnteriorL2->S < itPonAnteriorL1->S)
        {
          ++itPonAnteriorL2;
          ++itPonAtualL2;
        }

        Reiniciar = true;   //--- Reinicia a sobra nas distancias entre dos traços

        for (++itPonAtualL2;
             itPonAtualL2 != citalnAtual->second.LstPontos.end() && itPonAtualL1 != citalnAnterior->second.LstPontos.end();
             ++itPonAnteriorL1, ++itPonAtualL1, ++itPonAnteriorL2, ++itPonAtualL2)
        {
          //--- Verifica se não esta entrando ou saindo de alguma berma nova entre os 2 alinhamentos que estão sendo feitos

          cittymapCAlinhamentos citRasc(VerfEntrouAlinhamento(itPonAnteriorL1, itPonAtualL1, citalnAtual, Bermas));

          if (citRasc != Alinhamentos.MapAlinhamentos.end() && citRasc != citalnAtual)
          {
            if ((Lado == ESQUERDO && citRasc->second.PegaChave().PegaAfastamento() < citalnAtual->second.PegaChave().PegaAfastamento()) ||
                (Lado == DIREITO  && citRasc->second.PegaChave().PegaAfastamento() > citalnAtual->second.PegaChave().PegaAfastamento()))
            {
              citalnAtual = citRasc;

              SetAlinFeitos.push_back(citalnAtual);

              itPonAnteriorL2 = citalnAtual->second.LstPontos.begin();

              while (itPonAnteriorL2->S < itPonAnteriorL1->S)    //--- em S está a estaca
              {
                ++itPonAnteriorL2;
              }

              itPonAtualL2 = itPonAnteriorL2;
              ++itPonAtualL2;
            }
          }

          if (itPonAtualL2->Descricao.Find("Passagem") != std::string::npos) //--- Chegou a um ponto de passagem
          {
            cittyplstPontos itPonAnteriorBerma(citalnAtual->second.LstPontos.begin()), itPonAtualBerma(itPonAnteriorBerma); //--- berma ou prox alinhamento

            //--- O próximo alinhamento só pode ser um alinhamento de talude ou de bordo

            if (citalnAtual->first.PegaDescricao().find("TAL") != std::string::npos || citalnAtual->first.PegaDescricao().find("BOR") != std::string::npos)
            {
              ++itPonAtualBerma;

              if (itPonAtualBerma == citalnAtual->second.LstPontos.begin()) break;

              while (itPonAtualBerma->S < itPonAtualL1->S) ++itPonAtualBerma;

              //--- O alinhamento atual agora é o que está após a berma

              itPonAtualL2 = itPonAnteriorL2 = itPonAtualBerma;
              --itPonAnteriorL2;

              if (itPonAtualL2->Descricao.Find("Passagem") != std::string::npos) TipoTerr = itPonAnteriorL2->Baliza;  //---Se é ponto de passagem atualiza o tipo. Vale até o proximo ponto de passagem
            }
            else   //--- chegou em uma mudanca de corte para aterro ou vice versa
            {
              //--- Continua o alinhamento de off-set ate achar o ponto de passagem para comecar de novo

              while (itPonAtualL1 != citalnAnterior->second.LstPontos.end() && itPonAtualL1->Descricao.Find("Passagem") == std::string::npos)
              {
                ++itPonAtualL1;
              }

              //--- Despreza o primeiro segmento depois do ponto de passagem

              itPonAnteriorL1 = ++itPonAtualL1;
              ++itPonAtualL1;

              //--- Depois de um ponto de passagem so pode ter um bordo de terraplanagem ou bordo de pista como anterior
              //--- O bordo de terraplanagem vai do inicio ao fim do trecho

              if (Lado == ESQUERDO) citalnAtual = Alinhamentos.MapAlinhamentos.begin();
              else
              {
                citalnAtual = Alinhamentos.MapAlinhamentos.end();
                --citalnAtual;
              }

              //--- Posiciona no proximo bordo de terraplanagem

              do
              {
                if (Lado == ESQUERDO) ++citalnAtual;
                else --citalnAtual;

              } while (citalnAtual->first.PegaDescricao().find("BORTEE") == std::string::npos);

              SetAlinFeitos.push_back(citalnAtual);

              //--- Posiciona na estaca correta

              itPonAnteriorL2 = citalnAtual->second.LstPontos.begin(), itPonAtualL2 = itPonAnteriorL2;
              ++itPonAtualL2;

              while (itPonAnteriorL2->S < itPonAnteriorL1->S)
              {
                ++itPonAnteriorL2;
                ++itPonAtualL2;
              }
            }
          }

          DefineTracosSimbologia(*itPonAnteriorL1, *itPonAtualL1, *itPonAnteriorL2, *itPonAtualL2, SimbologiaTerrap, itPonAtualL1->Baliza, Lado, Reiniciar,Densidade);

          Reiniciar = false;
        }
      }

      //-- Caso haja bermas ou outros alinhamentos é preciso passar de novo para fazê-los.

      Bermas = true;

      cittymapCAlinhamentos citalnAtualRasc(Alinhamentos.MapAlinhamentos.begin());

      bool AchouAlinSemFazer(false);

      while (citalnAtualRasc != Alinhamentos.MapAlinhamentos.end() && !AchouAlinSemFazer)
      {
        if (atoi(&*(citalnAtualRasc->first.PegaDescricao().rbegin())) % 2 == 1)
        {
          if ((Lado == ESQUERDO && citalnAtualRasc->first.PegaAfastamento() < 0.0) ||
              (Lado == DIREITO  && citalnAtualRasc->first.PegaAfastamento() > 0.0))
          {
            auto AlinSet = std::find(SetAlinFeitos.begin(), SetAlinFeitos.end(), citalnAtualRasc);

            if (AlinSet == SetAlinFeitos.end())
            {
              AchouAlinSemFazer = true;
            }
          }
        }

        if (!AchouAlinSemFazer) ++citalnAtualRasc;
      }

      FezTodos = !AchouAlinSemFazer;

      if (!FezTodos) SetAlinFeitos.push_back(citalnAtualRasc);

      citalnAnterior = citalnAtualRasc;
      citalnAtual = Alinhamentos.MapAlinhamentos.end();
    }
  }

  GravaArqSimbologia(SimbologiaTerrap);
}

void CCalcTerraplProjeto::DefineTracosSimbologia(const Ponto& PAnteL1, const Ponto& PProxL1, const Ponto& PAnteL2, const Ponto& PProxL2, lstPontos& SimbolTerrap, int Tipo, int Lado, bool Reiniciar,int Densidade)
{
  static double SobraAnterior(0.0);
  static bool TracoGrande(false);

  double Dist(SobraAnterior);

  if (Reiniciar)
  {
    SobraAnterior = 0.0;
    TracoGrande = false;
  }

  Ponto RPP1P2(Mat.R_P(PAnteL2, PProxL2));

  double Equidis(Densidade / 2.0 + 0.5);

  for (; Dist < RPP1P2.y; Dist += Equidis)
  {
    Ponto P1Traco, P2Traco;

    Mat.IrradiaPonto(0.0, PAnteL2.x, PAnteL2.y, RPP1P2.x, Dist, P1Traco);
    Mat.IrradiaPonto(Lado == ESQUERDO ? -Matem::M_PI_2 : Matem::M_PI_2, P1Traco.x, P1Traco.y, RPP1P2.x, 1000.0, P2Traco);

    if (CLPontos::CalculaIntersecao(P1Traco, P2Traco, PAnteL1, PProxL1, P2Traco))
    {
      double ComprTraco(hypot(P1Traco.x - P2Traco.x, P1Traco.y - P2Traco.y));

      ComprTraco *= TracoGrande ? 0.7 : 0.3;

      if (Tipo == ATERRO) Mat.IrradiaPonto(Lado == ESQUERDO ? -Matem::M_PI_2 : Matem::M_PI_2, P1Traco.x, P1Traco.y, RPP1P2.x, ComprTraco, P2Traco);
      else Mat.IrradiaPonto(Lado == ESQUERDO ? Matem::M_PI_2 : -Matem::M_PI_2, P2Traco.x, P2Traco.y, RPP1P2.x, ComprTraco, P1Traco);

      SimbolTerrap.emplace_back(P1Traco);
      SimbolTerrap.emplace_back(P2Traco);
      SimbolTerrap.emplace_back(Ponto());     //--- Separador;

      TracoGrande = !TracoGrande;
    }
  }
  SobraAnterior = Dist - RPP1P2.y;
}

bool CCalcTerraplProjeto::GravaArqSimbologia(lstPontos& SimbologiaTerrap)
{
  CFile ArqSimbologia;
  CFileException e;

  std::string NomeCompArquivo(NomeProjeto + ".sbt");

  if (ArqSimbologia.Open(NomeCompArquivo.data(),CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &e) == 0)
  {
    monolog.mensagem(13, CString(NomeCompArquivo.data()) + " (escrevendo dados)");

    return false;
  }
  else
  {
    CArchive ar(&ArqSimbologia, CArchive::store);

    ar << SimbologiaTerrap.size();

    for (itlstPontos it = SimbologiaTerrap.begin(); it != SimbologiaTerrap.end(); ++it)
    {
      *it >> ar;
    }
  } 
  
  return true;
}

cittymapCAlinhamentos CCalcTerraplProjeto::VerfEntrouAlinhamento(cittyplstPontos itEstacaAtual, cittyplstPontos citPAtualL1,cittymapCAlinhamentos AlinAtual,bool Bermas)
{
  cittymapCAlinhamentos citProxAlinhamento(Alinhamentos.MapAlinhamentos.begin());
  bool Achou(false), LadoAlinhamento(AlinAtual->second.PegaChave().PegaAfastamento() > 0.0), LadoAtual(false);   //--- lado false = esquerdo, true = direito
  Ponto PInicial, PFinal;

  //--- Verifica se esta entrando em um novo alinhamento
  //--- Se achar um alinhamento que  se inicia na estaca atual no mesmo lado sim

  ++citProxAlinhamento;

  while (citProxAlinhamento != Alinhamentos.MapAlinhamentos.end() && !Achou)
  {
    LadoAtual = citProxAlinhamento->second.PegaChave().PegaAfastamento() >= 0.0;

    if (LadoAtual == LadoAlinhamento)
    {
      std::tie(PInicial, PFinal) = citProxAlinhamento->second.PegaInicioFim();

      if (PInicial.S == itEstacaAtual->S)     //--- Entrando um novo alinhamento?
      {
        if(PInicial.Descricao.Find("TAL") != -1)
        {
          if((!Bermas && atoi(PInicial.Descricao.Right(1)) % 2 == 0) || (Bermas && atoi(PInicial.Descricao.Right(1)) % 2 == 1)) 
          {
            if(citProxAlinhamento != AlinAtual)
            {
              if((LadoAtual == ESQUERDO && citProxAlinhamento->first.PegaAfastamento() > citPAtualL1->i && citProxAlinhamento->first.PegaAfastamento() < AlinAtual->first.PegaAfastamento()) ||
                 (LadoAtual == DIREITO  && citProxAlinhamento->first.PegaAfastamento() < citPAtualL1->i && citProxAlinhamento->first.PegaAfastamento() > AlinAtual->first.PegaAfastamento()))
              {
                Achou = true;
              }
            }
          }
        }
      }
    }

    if(!Achou) ++citProxAlinhamento;
  }

  return citProxAlinhamento;
}

cittymapCAlinhamentos CCalcTerraplProjeto::ProcuraProxAlinhamento(cittyplstPontos itEstacaAtual, cittymapCAlinhamentos AlinAtual,int pLado)
{
  cittymapCAlinhamentos citProxAlinhamento(Alinhamentos.MapAlinhamentos.begin()),citAlinMaixProximo(Alinhamentos.MapAlinhamentos.end());
  bool LadoAlinhamento(AlinAtual->second.PegaChave().PegaAfastamento() > 0.0),LadoAtual(false);   //--- lado false = esquerdo, true = direito
  Ponto PInicial,PFinal;

  //--- Procura por uma alinhamento que esteja passando pelo trecho da estaca atual no mesmo lado
  //--- Vai haver pelo menos um: o bordo da pista

  ++citProxAlinhamento;

  while (citProxAlinhamento != Alinhamentos.MapAlinhamentos.end())
  {
    LadoAtual = citProxAlinhamento->second.PegaChave().PegaAfastamento() >= 0.0;
    
    if(LadoAtual == LadoAlinhamento)
    {
      std::tie(PInicial, PFinal) = citProxAlinhamento->second.PegaInicioFim();

      if(PInicial.S <= itEstacaAtual->S && PFinal.S >= itEstacaAtual->S)
      {
        if(citProxAlinhamento != AlinAtual)
        {
          //--- O próximo alinhamento tem que estar depois do alinhamento atual (no eixo x)

          if ((pLado == ESQUERDO && AlinAtual->second.PegaChave().PegaAfastamento() < citProxAlinhamento->second.PegaChave().PegaAfastamento()) || 
              (pLado == DIREITO  && AlinAtual->second.PegaChave().PegaAfastamento() > citProxAlinhamento->second.PegaChave().PegaAfastamento()))
          {
            //--- Podem ter entrado varios alinhamentos entre os 2 alinhamentos atuais, separa o mais próximo.

            if(citAlinMaixProximo == Alinhamentos.MapAlinhamentos.end()) citAlinMaixProximo = citProxAlinhamento;
            else
            if((pLado == ESQUERDO && citProxAlinhamento->second.PegaChave().PegaAfastamento() < citAlinMaixProximo->second.PegaChave().PegaAfastamento()) ||
               (pLado == DIREITO  && citProxAlinhamento->second.PegaChave().PegaAfastamento() > citAlinMaixProximo->second.PegaChave().PegaAfastamento()))
            { 
              if(citProxAlinhamento->second.PegaChave().PegaDescricao().find("OFF") == std::string::npos)   //--- Para o lado direito pode chegar no offset
              {
                citAlinMaixProximo = citProxAlinhamento;
              }
            }
          }
        }
      }
    }

    ++citProxAlinhamento;
  }

  return citAlinMaixProximo;
}

void CCalcTerraplProjeto::CalculaCNSimbologia(CSuperficie& pSuperficieTerrap)
{
  pSuperficieTerrap.LLPontosCNSimbTerrapl.clear();

  for (ItLLPontos itLLCN = pSuperficieTerrap.LPontosCN.begin(); itLLCN != pSuperficieTerrap.LPontosCN.end(); itLLCN++)
  {
    ItLPontos itLCN(itLLCN->begin());

    while(itLCN != itLLCN->end())
    {
      LPontos CNAtual;

      CNAtual.emplace_back(*itLCN++);

      ItCLLDeltaSup itEntrou(pSuperficieTerrap.PegaListaRestricoes().end());    //--- Ponto de entrada na terraplanagem

      while (itEntrou == pSuperficieTerrap.PegaListaRestricoes().end() && itLCN != itLLCN->end())
      {
        itEntrou = pSuperficieTerrap.VerificaNaTangenteRestricoes(pSuperficieTerrap.PegaListaRestricoes().begin(),
                                     *itLCN, 5E-5, NULL, NULL, NULL, CAciTopografico::OFF_SETS, true,true);

        CNAtual.emplace_back(*itLCN);  //--- Insere na CN os pontos que estão fora da superficie de terraplanagem

        ++itLCN;
      }

      if (itLCN != itLLCN->end())    //--- Se a curva entrou na terraplanagem
      {
        pSuperficieTerrap.LLPontosCNSimbTerrapl.emplace_back(CNAtual);         //--- Insere na superficie de terraplanagem a CN cortada no offset esquerdo

        ItCLLDeltaSup itSaiu(pSuperficieTerrap.PegaListaRestricoes().end());   //--- Ponto de saida da terraplanagem

        while (itSaiu == pSuperficieTerrap.PegaListaRestricoes().end() && itLCN != itLLCN->end())
        {
          itSaiu = pSuperficieTerrap.VerificaNaTangenteRestricoes(pSuperficieTerrap.PegaListaRestricoes().begin(),
                                     *itLCN, 5E-5, NULL, NULL, NULL, CAciTopografico::OFF_SETS, true,true);

          //--- Não insere na CN os pontos que estão dentro da terraplanagem

          if (itSaiu == pSuperficieTerrap.PegaListaRestricoes().end()) ++itLCN;
        }
      }
      else
      {
        pSuperficieTerrap.LLPontosCNSimbTerrapl.emplace_back(CNAtual);    //--- A curva não entrou na terraplanagem, passa toda
      }
    }
  }
}

void CCalcTerraplProjeto::FechaExtremosTerrap(CSuperficie& SuperficieTerrap)       //--- Inclui um alinhamento = a primeira e a ultima seção pra evitar que na simbologia CNs entrem por aí.
{
   LDeltaSup LInicio,LFim;

   //--- Fecha o inicio da Terraplanagem;

   LInicio.push_back(CDeltaSuperficie(*Alinhamentos.MapAlinhamentos.begin()->second.LstPontos.begin(), CAciTopografico::OFF_SETS));   //--- Offset esquerdo primeira estaca
   LFim.push_back(CDeltaSuperficie(*Alinhamentos.MapAlinhamentos.begin()->second.LstPontos.rbegin(), CAciTopografico::OFF_SETS));     //--- Offset esquerdo última estaca

   ittymapCAlinhamentos itAlin(Alinhamentos.MapAlinhamentos.begin());

   while (itAlin != Alinhamentos.MapAlinhamentos.end() && itAlin->first.PegaDescricao().find("OFFSED") == std::string::npos)
   {
     ++itAlin;
   }

   if (itAlin != Alinhamentos.MapAlinhamentos.end())
   {
     LInicio.push_back(CDeltaSuperficie(*itAlin->second.LstPontos.begin(), CAciTopografico::OFF_SETS));   //--- Offset direito primeira estaca
     LFim.push_back(CDeltaSuperficie(*itAlin->second.LstPontos.rbegin(), CAciTopografico::OFF_SETS));     //--- Offset direito última estaca

     //--- Estas Listas são apenas para o cálculo da CN da terraplanagem. Elas "Fecham" os expremos da superficie de terraplanagem com
     //--- alinhamentos para poderem ser encontrados na definição as CNs de terraplanagem com simbologia, caso contrário
     //--- algumas CNs entram por ali e fica errado.

     ItLLDeltaSup itLLDS(SuperficieTerrap.PegaListaRestricoes().insert(SuperficieTerrap.PegaListaRestricoes().end(), LInicio));
     SuperficieTerrap.InsereListaNoMapa(itLLDS, CAciTopografico::OFF_SETS);

     itLLDS = SuperficieTerrap.PegaListaRestricoes().insert(SuperficieTerrap.PegaListaRestricoes().end(), LFim);
     SuperficieTerrap.InsereListaNoMapa(itLLDS, CAciTopografico::OFF_SETS);
   }
}
