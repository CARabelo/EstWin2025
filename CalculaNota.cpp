#include "stdafx.h"
#include "afxtempl.h"
#include "supercstring.h"
#include "math.h"
#include "estaca.h"
#include "cponto.h"
#include "clpontos.h"
#include <set>
#include <string>
#include "carqgradientes.h"
#include "cgradientes.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include "carqcurverticais.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "ccurverticais.h"
#include "carqcurhorizontais.h"
#include "perfil.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include <map>
#include "ccurhorizontal.h"
#include "Mat.h"
#include "ccurhorizontais.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "cestacasacalcular.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include <list>
#include "csecao.h"
#include "clistasecoes.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
#include "carqpistasfixas.h"
#include "cpistasfixas.h"
#include "CTipoSolos.h"
#include "CArqTiposSolos.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include "CArqFurosSondagem.h"
#include <algorithm>
#include "CPerfilGeologico.h"
#include "calculanota.h"
#include "resource.h"
#include "monologo.h"
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFINITO   1.7976931348623158e+308
#define X          0
#define Y          1
#define SIM        TRUE
#define NAO        FALSE
#define ESQUERDO   0
#define DIREITO    1
#define NENHUM     -1
#define PROJETO    0
#define LER        0  
#define IGUAL      0

extern class monologo monolog;
extern class Matem Mat;

//---------------------------------------------------------------------------------------------------------------------

CalculaNota::CalculaNota(CString &NomeProjeto,CDialog* Diag,bool pbSoCotas,CEvent* Evento,CProjeto* CProj) : Gradientes(NomeProjeto),
Alargamentos(NomeProjeto), CurvasVerticais(NomeProjeto), SecoesTipo(NomeProjeto), 
CurvasHorizontais(NomeProjeto,SecoesTipo), EstacasACalcular(&CurvasHorizontais,&CurvasVerticais,NomeProjeto,CProj),
TaludesPavimento(NomeProjeto),TaludesCorte(NomeProjeto),TaludesAterro(NomeProjeto),
HPavimento(NomeProjeto),Canteiros(NomeProjeto),PistasFixas(NomeProjeto),CanteirosCentrais(NomeProjeto),NomeProj(NomeProjeto),Dialogo(Diag),IAnte(INFINITO),IPost(INFINITO),
AlterouTerreno(false),bSoCotas(pbSoCotas),Terreno(CString(NomeProjeto+".tca")),EventoFim(Evento),HLimpeza(NomeProjeto,CString(".lim")),
PerfilGeologico(NomeProjeto.GetBuffer()), EstacaAnterior(INFINITO), CotaAnterior(INFINITO)
{
  CVsAnteriores[0] = CVsAnteriores[1] = INFINITO;

  PerfilGeologico.Serialize(NomeProjeto.GetBuffer(),0);
  PerfilGeologico.Calculais();
  PerfilGeologico.LimpaDadosNotaServico();
  AbreArquivoCalcGeomDetalhado(NomeProjeto);
}

void CalculaNota::MontaSecoes()
{
  POSITION Atual(EstacasACalcular.GetHeadPosition());
  
  if(Alargamentos.PegaErro() > 0)
  {
    monolog.mensagem(17,-1,"\r\nhouve erro nos alargamentos, a nota n�o poder� ser calculada.");
    return;
  }

  if(CurvasVerticais.LCurvasVerticais.GetCount() < 2)
  {
    monolog.mensagem(17,-1,"\r\nFaltam curvas verticais.");
    return;
  }

  if(SecoesTipo.GetCount() == 0)
  {
    monolog.mensagem(17,-1,"\r\nFalta a se��o tipo.");
    return;
  }
  if(HPavimento.GetCount() > 0 && TaludesPavimento.GetCount() == 0)
  {
    monolog.mensagem(17,-1,"\r\nFaltam expessuras do pavimento.");
    return;
  }
  if(TaludesCorte.GetCount() == 0)
  {
    monolog.mensagem(17,-1,"\r\nFaltam taludes de corte.");
    return;
  }

  if(TaludesAterro.GetCount() == 0)
  {
    monolog.mensagem(17,-1,"\r\nFaltam taludes de aterro.");
    return;
  }

  int ErroTaludes(TaludesCorte.ConsisteDados());

  if(ErroTaludes > 0)
  {
    monolog.mensagem(17,-1,"\r\nErro nos taludes de corte, faltam taludes de corte ou talude de corte com insufici�ncia de pontos.");
    return;
  }

  ErroTaludes = TaludesAterro.ConsisteDados();

  if(ErroTaludes > 0)
  {
    monolog.mensagem(17,-1,"\r\nErro nos taludes de Aterro, faltam taludes de aterro ou talde de aterro com insufici�ncia de pontos.");
    return; 
  }

  if(TaludesPavimento.GetCount () == 0)
  {
    monolog.mensagem(17,-1,"\r\nFaltam taludes do pavimento.");
    return;
  }

  if(EstacasACalcular.GetCount() < 2)
  {
    monolog.mensagem(17,-1,"\r\nN�o h� estacas para serem calculadas.");
    return;
  }

  CProgressCtrl* Prog(NULL);
  if (Dialogo)
  {
    Prog = (CProgressCtrl *) Dialogo->GetDlgItem(IDC_PROGRESSO);
    Prog->SetRange(0,EstacasACalcular.GetCount());
    Prog->SetStep(1);
  }

  setCSecao SetSecoesCalculadas;
  std::string strMsgSaida;

  if(bSoCotas) Secoes.Constroi(NomeProj);

  RecriaPerfisGeologicos();

  try
  {
    while(Atual)  //--- Calcula a nota de servi�o para todas as estacas.
    {
      CEstaca EstacaAtual( EstacasACalcular.GetNext(Atual));

      if (Prog) Prog->StepIt();
      if (Dialogo) Dialogo->SetDlgItemText(1037,"Calculando a estaca: " + EstacaAtual);

      Perfil ProjetoAtual(EstacaAtual);  //--- Lista com a secao de projeto.

      if((ProjetoAtual.Estaca.Cota = CurvasVerticais.CalculaCota(ProjetoAtual.Estaca,E,IAnte,IPost)) == INFINITO) continue;  //--- Se Se��o sem greide, pula

      POSITION PosSecAtual(Secoes.Posiciona(EstacaAtual));

      Perfil PontosInseridos;

      if (bSoCotas) PegaPontosInseridos(PontosInseridos,Secoes.Posiciona(EstacaAtual));

      SecTipo SecaoTipo;     

      if(!SecoesTipo.BuscaSecTipo(ProjetoAtual.Estaca,SecaoTipo)) continue; //--- Insere a se�ao tipo na lista

      double XGradiente(Gradientes.BuscaGradiente(EstacaAtual));

      SecaoTipo.InsereGradiente(XGradiente);

      ProjetoAtual.InsereLista(SecaoTipo);          //--- Insere a se�ao tipo na lista

      AplicaSuperElevacao(ProjetoAtual,XGradiente); //--- Aplica a supereleva�ao a se��o tipo.
      AplicaSuperLargura(ProjetoAtual);             //--- Aplica a superlargura aos bordos          

      SecaoTipo.IgualPerfil(ProjetoAtual);   //--- Faz a se��o tipo superelevada, que � igual ao perfil atual 

      Perfil PontosAInserir;                 //-- lista Tempor�ria, Como os perf�s a serem buscados podem ou n�o existir, n�o pode ser chamada por refer�ncia
      Alargamentos.BuscaAlargamento(ProjetoAtual.Estaca,ESQUERDO,PontosAInserir); //--- Pega os pontos de alarg Esq.
      ProjetoAtual.InsereListaLado(PontosAInserir,ESQUERDO,NAO);                  //--- Insere-os na secao de projeto.
      PontosAInserir.RemoveTudo();                                                //--- Vai reusar a lista,limpa os pontos de alargamento esquerdo.
      Alargamentos.BuscaAlargamento(ProjetoAtual.Estaca,DIREITO,PontosAInserir);  //--- Pega os pontos de alarg. Dir.
      ProjetoAtual.InsereListaLado(PontosAInserir,DIREITO,NAO);                   //--- Insere-os na secao de projeto.

      if(bSoCotas) ProjetoAtual.InsereLista(PontosInseridos);

      Perfil TerrenoAtual;

      Terreno.BuscaPerfil(EstacaAtual,TerrenoAtual);                //--- busca o perfil de terreno.
      double ExessoLateral,Limpeza(HLimpeza.BuscaH(EstacaAtual,&ExessoLateral));

      CSecao SecaoAtual(TerrenoAtual,ProjetoAtual,SecaoTipo);
      TaludesPavim TalPavimento;

      TaludesPavimento.BuscaTaludesPavim(EstacaAtual,TalPavimento); //--- busca os taludes do pavimento
      SecaoAtual.LimpaTerreno(Limpeza);

      SecaoAtual.CotaSecao(XGradiente,HPavimento.BuscaH(EstacaAtual),SecaoTipo);                              //--- Cota a se��o de projeto.
      SecaoAtual.CalculaBordos(TalPavimento,-HPavimento.BuscaH(EstacaAtual),Canteiros,true);                  //-- Calcula os bordos de pavimenta�ao.
      if(bSoCotas && PosSecAtual) RemovePontosEditadosProjeto(SecaoAtual,Secoes.GetAt(PosSecAtual).Projeto);  //--- A se��o esta sendo remontada, os pontos movidos est�o no lugar de origem, s�o removidos aqui

      Perfil ProjetoOriginal(SecaoAtual.Projeto);                     //--- Perfil do projeto sem os taludes 
 
      SecaoAtual.CalculaOffSets(TaludesCorte,TaludesAterro,false);    //--- finalmente calcula os offsets.

      CalculaPontosTerraplProjeto(SecaoAtual);                        //--- Calcula os pontos para a terraplanagem de projeto                                 

      Terreno.BuscaPerfil(EstacaAtual,TerrenoAtual);                  //--- busca o perfil de terreno original, sem a limpeza.

      SecaoAtual.IncluiListaLimpeza(Limpeza,ExessoLateral,TerrenoAtual);

      if(bSoCotas) SetSecoesCalculadas.insert(SecaoAtual).second;
      else Secoes.InsereOrdenada(SecaoAtual);

      //--- Calcula dos dados do perfil geol�gico

      tysettypaiitPerfildouble itCotasGeologicas;
      PerfilGeologico.CalculaCotasNaEstaca(EstacaAtual,itCotasGeologicas);               //--- Calcula as cotas dos perfis geologicos na estaca
      ittysettypaiitPerfildouble ItCotaGeologicaAtual(itCotasGeologicas.begin());
      Perfil TerrenoPrimitivo(TerrenoAtual);
      double DeltahPerfil(0.0);
      Ponto pCVs,OffSetEsq,OffSetDir;
      tlstwstr ListaRotulosExcluir;

      ListaRotulosExcluir.push_back("OFF");

      while(ItCotaGeologicaAtual != itCotasGeologicas.end())
      {
        if(ItCotaGeologicaAtual->second != INFINITO)
        {
          TerrenoAtual = TerrenoPrimitivo;
          if(TerrenoAtual.Estaca.Cota != INFINITO)   
          {
            DeltahPerfil = ItCotaGeologicaAtual->second - TerrenoAtual.Estaca.Cota;                          

            if(DeltahPerfil > -0.05) CPerfilGeologico::AjustaLimpeza(SecaoAtual.Limpeza,TerrenoAtual);     //--- � perfil geol�gico, acerta as cotas do perfil
            else TerrenoAtual.AlteraFaixaY(Ponto(-INFINITO,INFINITO,DeltahPerfil));                        //---  sen�o esta na camada do TN, define o perfil da camada
                                             
            SecaoAtual.Terreno = TerrenoAtual;                                         
            SecaoAtual.Projeto = ProjetoOriginal;                                     //--- Projeto sem os pontos de taludes calculados anteriormente 

            SecaoAtual.CalculaOffSets(TaludesCorte,TaludesAterro,CSecao::VUE_PERFIL_GEOLOGICO,&pCVs,&OffSetEsq,&OffSetDir);     //--- finalmente, calcula os offsets.

            if(OffSetEsq.x != INFINITO) SecaoAtual.Projeto.AddHead(OffSetEsq);      //--- So guarda os offsets de corte
            if(OffSetDir.x != INFINITO) SecaoAtual.Projeto.AddTail(OffSetDir);

            //--- Para o terreno natural as �reas so s�o calculadas quando encontra os 2 off-sets. Mas para os perfils geol�gicos deve-se calcular as �reas 
            //--- Mesmo que n�o encontre os 2 offsets, pois um offset pode estar na pista de rolamento, por exemplo.
            //--- Neste caso o offset esquerdo � a primeira intersec�o do projeto com o terreno e o offset direito a �ltima.
    
            if(OffSetEsq.x == INFINITO || OffSetDir.x == INFINITO) 
            {
              Ponto PontoPassEsq(OffSetEsq),PontoPassDir(OffSetDir);                            //--- Pontos de Passagem

              SecaoAtual.CalcPrimUltIntersProjTerreno(&PontoPassEsq,&PontoPassEsq);

              //--- Marca os offsets como pontos de passagem (usado nos desenhos das se��es para n�o desenhar off sets de aterro
              //--- Nos perf�s geol�gicos

              if(OffSetEsq.x == INFINITO && PontoPassEsq.x != INFINITO) pCVs.x = 0.0;            //--- Sinaliza que � Ponto de passagem CV = 0.0       
              if(OffSetDir.x == INFINITO && PontoPassDir.x != INFINITO) pCVs.y = 0.0;       
            }
            
            CDadosPerfisGeologicos DadosPerfGeologicoAtual(std::string(ItCotaGeologicaAtual->first->Descricao),EstacaAtual.EstVirtual,
                                                           &pCVs,&OffSetEsq,&OffSetDir,&TerrenoAtual,DeltahPerfil);
            PerfilGeologico.InsereDadosNotaServico(DadosPerfGeologicoAtual);
          }
        }
        ++ItCotaGeologicaAtual;
      }
    }

    if(bSoCotas) AcertaLista(SetSecoesCalculadas);    //--- Remove as se��es que estavam calculada mas n�o foram calculadas neste rec�lculo  
    GravaSecao();
    RecriaPerfisGeologicos();                         //--- Se��es podem ter sido inseridas ou removidas do terreno, acerta os perf�s geol�gicos 
    PerfilGeologico.Serialize(std::string(NomeProj.GetBuffer()),1);
    GravaPontosPassagem();
    GravaPontosTerraplProjeto(NomeProj);
  }
  catch(...)
  {
    if (Dialogo)  
    {
      Dialogo->SetDlgItemText(1038,"houve erros, os c�lculos n�o foram executados ou est�o inv�lidos.");
      if (Dialogo->GetDlgItem(IDC_PAUSA)) Dialogo->GetDlgItem(IDC_PAUSA)->EnableWindow(false);
      if(Dialogo->GetDlgItem(IDC_OK)) Dialogo->GetDlgItem(IDC_OK)->EnableWindow(true);
    }

    if(EventoFim) SetEvent(EventoFim);

    return;
  }

  if (Dialogo)  
  {
    ((CMainFrame*)AfxGetMainWnd())->GreideAlterado = false;
    ((CMainFrame*)AfxGetMainWnd())->SecaoAlterada = false;

    Dialogo->SetDlgItemText(1038,"OK, O c�lculo da nota de servi�o foi executado com �xito.");

    //--- Pode ser o dialogo de volumes ou da nota de servi�o, dependendo de quem chamou,
    //--- Se for o dialogo dos volumes, os controles tem ourto nome mas mesmo assim n�o precisam ser ligados pois 
    //--- Neste caso o dialogo � n�o modal

    if (Dialogo->GetDlgItem(IDC_PAUSA)) Dialogo->GetDlgItem(IDC_PAUSA)->EnableWindow(false);
    if (Dialogo->GetDlgItem(IDC_OK)) Dialogo->GetDlgItem(IDC_OK)->EnableWindow(true);
  }

  if(EventoFim) SetEvent(EventoFim);
}

void CalculaNota::AplicaSuperElevacao(Perfil& SecaoAtual,double Gradiente)   //--- Est� igual ao calculanota, UNIFICAR
{
  int LadoCurva(NENHUM);
  Perfil SecaoRasc = SecaoAtual;
  if((SE = CurvasHorizontais.BuscaSuperElevacao(SecaoRasc.Estaca,LadoCurva)) == INFINITO) return;  //--- busca a super-eleva�ao, se na tangente n�o faz nada
  if( LadoCurva == DIREITO) SE *= -1.0;
  POSITION PGradiente(SecaoRasc.GetHeadPosition());  //--- Inicializa o atual

  //--- Marca o gradiente.

  while(PGradiente && SecaoRasc.GetAt(PGradiente).x < Gradiente)  SecaoRasc.GetNext(PGradiente);

  Ponto PonGradiente(SecaoRasc.GetAt(PGradiente));

  if (PonGradiente.y != 0.0)                    //---- O y do gradiente tem que ser 0.00
  {
    for (POSITION Atual = SecaoRasc.GetHeadPosition(); Atual != NULL; SecaoRasc.GetNext(Atual))
    {
      SecaoRasc.GetAt(Atual).y -= PonGradiente.y;    //--- Ajusta a se��o para aplica��o do gradiente
    }
	
    SecaoAtual = SecaoRasc;
  }

  //--- O c�lculo � feito por lado a partir do 0,0.

  POSITION PZero;

  for (PZero = SecaoRasc.GetHeadPosition(); PZero; SecaoRasc.GetNext(PZero))
  {
    if (SecaoRasc.GetAt(PZero).x >= 0.0) break;
  }
  
  if (!PZero || fabs(SecaoRasc.GetAt(PZero).x - 0.0) > 0.001) return;

  for (int LadoPista = ESQUERDO ; LadoPista <= DIREITO ; LadoPista++)
  {
    POSITION Anterior(PZero),Ultimo = LadoPista == ESQUERDO ? SecaoRasc.GetHeadPosition() : SecaoRasc.GetTailPosition();
    double DeltaYanterior(0.0);

    while(Anterior != Ultimo)
    {
      Ponto& PAnterior(LadoPista == ESQUERDO ? SecaoRasc.GetPrev(Anterior) : SecaoRasc.GetNext(Anterior));
      Ponto& PAtual(SecaoRasc.GetAt(Anterior)); 

      double DeltaX(PAtual.x - PAnterior .x),DeltaY(PAnterior.y - PAtual.y),i(LadoPista == ESQUERDO ? PAtual.i : PAnterior.i);

      //--- Verifica a necessidade de se aplicar a super eleva��o � pista.
      //--- Caso positivo aplica-a.
      //--- Se for true tem q aplicar a SE .... Exceto se for pista fixa ou canteiro
      if (LadoCurva != LadoPista || fabs(SE) >= fabs(i))           //--- Teve que incluir o = na compara��o do SE para entrar no if certo no caso de canteiro central
      {
        int Ini_Fim_Fixas(-1),Ini_Fim_Canteiros(-5); 

        //--- Verifica se � canteiro ou pista fixa

        bool EFixa(PistasFixas.VerfPistaFixas(SecaoAtual.Estaca,PAtual.x,Ini_Fim_Fixas)),
             ECant(CanteirosCentrais.ECanteiro(SecaoAtual.Estaca,PAtual.x,Ini_Fim_Fixas));

        if(!EFixa && !ECant)         //--- Se n�o for pista fixa ou canteiro (para efeito de SE os 2 s�o iguais) , aplica a SE      
        {
          Ponto& PontoOriginal = SecaoAtual.GetAt(SecaoAtual.PosicionaPontoX(PAtual.x));
          PontoOriginal.y  = DeltaYanterior = SE * DeltaX + DeltaYanterior;
          PontoOriginal.i = SE;
        }
        else
        {
          //--- � pista fixa:
          //--- Se for pista fixa e o lado for esquerdo e n�o for fim de pista fixa ou o lado for direito e n�o for inicio de pista fixa ou
          //--- Se for canteiro e o lado for esquerdo e n�o for in�cio de pista fixa ou o lado for direito e n�o for fim de pista fixa ou
          //--- aplica a SE

          Ponto& PontoOriginal(SecaoAtual.GetAt(SecaoAtual.PosicionaPontoX(PAtual.x)));

          if(!ECant || (EFixa && (LadoPista == ESQUERDO && Ini_Fim_Fixas == CPistasFixas::FIM) || (LadoPista == DIREITO && Ini_Fim_Fixas == CPistasFixas::INICIO))) 
          {
            PontoOriginal.y = DeltaYanterior = SE * DeltaX + DeltaYanterior;
            PontoOriginal.i = SE;
          }
          else      //--- Sen�o � ponto de pista fixa ou canteiro: N�o aplica a SE, Mantem o delta y atual
          {
            DeltaYanterior += PontoOriginal.y - DeltaYanterior;
          }
        }
      }
      else 
      {
        if (LadoCurva == ESQUERDO) DeltaY *= -1.0;
        DeltaYanterior += i > 0.0 ? DeltaY : -DeltaY;
        Ponto& PontoOriginal = SecaoAtual.GetAt(SecaoAtual.PosicionaPontoX(PAtual.x));
        PontoOriginal.y = DeltaYanterior;
      }
    }
  }
}

/*
void CalculaNota::AplicaSuperElevacao(Perfil& SecaoAtual, double Gradiente)   
{
  int LadoCurva(NENHUM);
  if ((SE = CurvasHorizontais.BuscaSuperElevacao(SecaoAtual.Estaca, LadoCurva)) == INFINITO) return;  //--- busca a super-eleva�ao, se na tangente n�o faz nada
  if (LadoCurva == DIREITO) SE *= -1.0;

  POSITION Anterior(SecaoAtual.GetHeadPosition()), Atual(Anterior);

  for (SecaoAtual.GetNext(Atual) ; Atual ; SecaoAtual.GetNext(Atual))
  {
    Ponto& PAnterior(SecaoAtual.GetAt(Anterior)), &PAtual(SecaoAtual.GetAt(Atual));

    if ((LadoCurva == DIREITO && SE < PAnterior.i) || (LadoCurva == ESQUERDO && SE > PAnterior.i))
    {
      int Ini_Fim_Fixas(-1), Ini_Fim_Canteiros(-5);

      //--- Verifica se � canteiro ou pista fixa

      bool EFixa(PistasFixas.VerfPistaFixas(SecaoAtual.Estaca, PAtual.x, Ini_Fim_Fixas)),
           ECant(CanteirosCentrais.ECanteiro(SecaoAtual.Estaca, PAtual.x, Ini_Fim_Fixas));

      //--- Se n�o for pista fixa ou se o ponto for inicio ou fim de pista fixa  aplica a supereleva��o

      if ((!EFixa && !ECant) || (Ini_Fim_Fixas != CPistasFixas::MEIO))       
      {
        if (LadoCurva == DIREITO)
        {
          if (PAnterior.i > 0.0)
          {
            PAnterior.y = -SE * (PAtual.x - PAnterior.x) + PAtual.y;
            PAnterior.i = -SE;
          }
          else
          {
            PAtual.y = SE * (PAtual.x - PAnterior.x) + PAnterior.y;
            PAnterior.i = SE;
          }

          if (PAtual == SecaoAtual.GetTail()) PAtual.i = PAnterior.i;
        }
        else
        {
          if (PAnterior.i < 0.0)
          {
            PAtual.y = SE * (PAtual.x - PAnterior.x) + PAnterior.y;
            PAnterior.i = -SE;
          }
          else
          {
            PAnterior.y = -SE * (PAtual.x - PAnterior.x) + PAtual.y;
            PAnterior.i = SE;
          }
        }
      }
    }
    Anterior = Atual;
  }
}
*/
void CalculaNota::AplicaSuperLargura(Perfil& ProjetoAtual)    
{
  if((SL = CurvasHorizontais.BuscaSuperLargura(ProjetoAtual.Estaca)) == 0.0) return;  //--- busca a superlargura, se na tangente n�o faz nada

  Ponto& BordoEsquerdo = ProjetoAtual.GetHead(),&BordoDireito = ProjetoAtual.GetTail();

  BordoEsquerdo.y -= SL * BordoEsquerdo.i;
  BordoEsquerdo.x -= SL;

  BordoDireito.y += SL * BordoDireito.i;
  BordoDireito.x += SL;
}

void CalculaNota::GravaSecao()
{
  CFile ArqNota;
  CFileException e;

  std::string NomeCompArquivo(NomeProj + ".not");

  if (ArqNota.Open(NomeCompArquivo.data(),CFile::modeCreate | CFile::modeWrite,&e) == 0)
  {
    monolog.mensagem(13,CString(NomeCompArquivo.data()) + " (Gravando os dados)");

    throw e.m_cause;
  }
  else
  {	
    CArchive ar(&ArqNota,CArchive::store);

    POSITION Atual = Secoes.GetHeadPosition();

    ar << (double )0.0 << (double )0.0 << (double )0.0 << (unsigned int) Secoes.GetCount();  //--- volumes e quantidades de secoes que ser�o gravadas.

    CSecao Secao;

    while(Atual)
    {
      Secao = Secoes.GetNext(Atual);

      Secao.E = E;
      Secao.SE = SE;
      Secao.IAnte = IAnte;
      Secao.IPost = IPost;

      Secao.Serialize(ar);
    }

    ar.Close();
    ArqNota.Close();
  }
}

void CalculaNota::AcertaLista(const setCSecao& SetSecoesCalculadas)
{
  //--- Remove a faixa de se��es recalculadas e insere as secoes novas calculadas
  //--- As se��es que devem ser removidas � igual ao conjunto das se��es existentes - as secoes calculadas
  //--- As existentes est�o em Secoes e as calculadas est�o no set SetSecoesCalculadas;

  std::list<POSITION> lstPOEstacasARemover;

  POSITION POSecaoAtual = Secoes.GetHeadPosition();

  itcsetCSecao itcsetSecCalcPrimeira = SetSecoesCalculadas.begin(),itcsetSecCalcUltima = --SetSecoesCalculadas.end();

  POSITION Atual = Secoes.GetHeadPosition();

  while(Atual && Secoes.GetAt(Atual) < *itcsetSecCalcPrimeira)  Secoes.GetNext(Atual);

  while(Atual && !(*itcsetSecCalcUltima < Secoes.GetAt(Atual))) 
  {
    lstPOEstacasARemover.push_back(Atual);
    Secoes.GetNext(Atual);
  }

  //--- Pega todas as posi��es a serem removidas e as guarda na lista lstPOEstacasARemover
  /*  while(POSecaoAtual)
  {
  CSecao SecaoAtual = Secoes.GetAt(POSecaoAtual);
  itcsetCSecao it = SetSecoesCalculadas.find(SecaoAtual);
  if (it != SetSecoesCalculadas.end())  
  {
  lstPOEstacasARemover.push_back(POSecaoAtual);
  }
  Secoes.GetNext(POSecaoAtual);
  }
  */
  //--- Remove as se��es que foram recalculadas. 

  for (std::list<POSITION>::iterator itlstPOSITION = lstPOEstacasARemover.begin()  ; itlstPOSITION != lstPOEstacasARemover.end() ; itlstPOSITION++)
    Secoes.RemoveAt(*itlstPOSITION);

  Secoes.LimpaAtual();

  //--- insere as secoes calculadas na lista definitiva de se��es

  for(itcsetCSecao itCSecaoAtual = SetSecoesCalculadas.begin() ; itCSecaoAtual != SetSecoesCalculadas.end() ; itCSecaoAtual++)
    Secoes.InsereOrdenada(const_cast<CSecao&>(*itCSecaoAtual));

} 
/*
void CalculaNota::InsereGradiTerreno(Perfil& TerrenoAtual,double XGradiente)              //--- Inssere no terreno o ponto onde x = gradiente
{
register POSITION TerrAtual = TerrenoAtual.GetHeadPosition();

while(TerrAtual && TerrenoAtual.GetAt(TerrAtual).x < XGradiente) TerrenoAtual.GetNext(TerrAtual);

if (TerrAtual && fabs(TerrenoAtual.GetAt(TerrAtual).x - XGradiente) > 1e-3)	
{
TerrenoAtual.Interpola(XGradiente);
AlterouTerreno = true;
}
}
*/

void CalculaNota::PegaPontosInseridos(Perfil& PontosInseridos,POSITION POSSecaoAtual)
{
  PontosInseridos.RemoveTudo();

  if (!POSSecaoAtual) return;

  CSecao& SecaoAtual(Secoes.GetAt(POSSecaoAtual));

  POSITION POSProjetoAtual = SecaoAtual.Projeto.GetHeadPosition();

  while(POSProjetoAtual)
  {
    Ponto& PAtual = SecaoAtual.Projeto.GetNext(POSProjetoAtual);

    //--- Acerta o delta y do ponto inserido, pois a cota ser� recalculada.

    if (PAtual.Descricao == "PONINS") 
    {
      PAtual.y = PAtual.y - SecaoAtual.Projeto.Estaca.Cota;
      PontosInseridos.InsereOrdenada(PAtual); 
    }
  }
}

void CalculaNota::RemovePontosEditadosProjeto(CSecao &SecAtual,const Perfil& ProjetoEditado)           //--- A se��o esta sendo remontada, os pontos movidos est�o no lugar de origem, s�o removidos aqui
{
  if(SecAtual.SecaoTipo.GetCount() > 2)
  {
    POSITION Atual(SecAtual.Projeto.GetHeadPosition());

    while(Atual != NULL)
    {
      Ponto PontoAtual(SecAtual.Projeto.GetAt(Atual));

      if(PontoAtual.Descricao.Find("BORTE") == -1 && (PontoAtual.Descricao.Find("PONINS") == -1 && 
        (SecAtual.SecaoTipo.GetHead().x > PontoAtual.x || SecAtual.SecaoTipo.GetTail().x < PontoAtual.x))) //-- N�o pode estar na secao tipo
      {
        if(ProjetoEditado.Find(PontoAtual) == NULL) 
        {
          POSITION PARemover(Atual);
          SecAtual.Projeto.GetNext(Atual);
          SecAtual.Projeto.RemoveAt(PARemover);
        }
        else  SecAtual.Projeto.GetNext(Atual);
      }
      else SecAtual.Projeto.GetNext(Atual);
    }
  }
}

void CalculaNota::RecriaPerfisGeologicos()
{
  //--- Cria set com as estacas atuais

  tysetEstaca SetEstacas;
 
  for(register POSITION Atual = EstacasACalcular.GetHeadPosition() ; Atual != NULL ; EstacasACalcular.GetNext(Atual))
  {
    SetEstacas.insert(EstacasACalcular.GetAt(Atual).EstVirtual);
  }

  //--- coloca a cota do terreno nas estacas, pra saber qual se a este ponto � do terreno

  for(register POSITION Atual = Terreno.GetHeadPosition() ; Atual != NULL ; Terreno.GetNext(Atual))
  {
    ittysetEstaca itEstAtual(SetEstacas.find(Terreno.GetAt(Atual).Estaca));

    if(itEstAtual != SetEstacas.end())
    {
      const_cast<CEstaca*>(&*itEstAtual)->Cota = Terreno.GetAt(Atual).Estaca.Cota;
    }
  }

  //--- Remove dos perfis geol�gicos as estacas que n�o existem mais 
  //--- N�o remove as que foram criadas a m�o livre. perf�s criado a m�o livre n�o s�o removidos

  std::list<ittyPerfilTipoSolo> LstItSetPerfilTipoSolo;                    //--- Lista para guardar os iteradores a remover
  std::list<ittyPerfilTipoSolo>::iterator ItLstItSetPerfilTipoSolo;

  for(ittysetCPerfilTipoSolo itPTS =  PerfilGeologico.PegaPerfisSolos().begin() ; itPTS != PerfilGeologico.PegaPerfisSolos().end() ; itPTS++)
  {
    LstItSetPerfilTipoSolo.clear();

    for(register ittyPerfilTipoSolo it= itPTS->PegaPerfil().begin() ; it != itPTS->PegaPerfil().end() ; it++)
    {
      if(it->Sinalizadores != CEstaca::MAO_LIVRE && SetEstacas.find(*it) == SetEstacas.end()) LstItSetPerfilTipoSolo.push_back(it);
    }

    for(ItLstItSetPerfilTipoSolo = LstItSetPerfilTipoSolo.begin() ; ItLstItSetPerfilTipoSolo != LstItSetPerfilTipoSolo.end() ; ItLstItSetPerfilTipoSolo++)
    {
       PerfilGeologico.RemovePontoPerfil(**ItLstItSetPerfilTipoSolo,itPTS);
    }
  }

  //--- Se algum perfil ficou com menos que 2 pontos, remove-o 

  for(ittysetCPerfilTipoSolo itPTS =  PerfilGeologico.PegaPerfisSolos().begin() ; itPTS != PerfilGeologico.PegaPerfisSolos().end() ;)
  {
    if(itPTS->PegaPerfil().size() < 2)  
    {
      PerfilGeologico.RemovePerfil(itPTS++);
    }
    else itPTS++;
  }

  //--- No Terreno natural insere as estacas criadas, nos outros perfis interpola-as (n�o interpola mais)
  //--- Para saber se a estaca do perfil � do terreno natual � preciso comparar as cotas

  const tysetCPerfilTipoSolo& SetPerfSolos(PerfilGeologico.PegaPerfisSolos()); 

  for(ittysetCPerfilTipoSolo itPTS =  PerfilGeologico.PegaPerfisSolos().begin() ; itPTS != PerfilGeologico.PegaPerfisSolos().end() ; itPTS++)
  {
    for(ittysetEstaca itSetEst = SetEstacas.begin() ; itSetEst != SetEstacas.end() ; itSetEst++)
    {
      ittyPerfilTipoSolo itPTSAtual(itPTS->PegaPerfil().lower_bound(*itSetEst));                  //--- Pega a estaca Igual ou maior  

      if(itPTSAtual != itPTS->PegaPerfil().end())          //--- Pegou alguma coisa???   
      {
        if(*itPTSAtual == *itSetEst)                       //--- Sim e � igual, j� existe no perfil
        {
                                                           //--- ent�o n�o faz nada deixa como est�
        }
        else   //--- Pegou alguma coisa mas a estaca n�o � igual (Ent�o pegou a pr�xima)
        {
           ittysetEstaca itEstAtual(SetEstacas.find(*itPTSAtual));   //--- Pega esta pr�xima estaca do perfil no terreno atual

          if(itEstAtual != SetEstacas.end())                         //--- Verifica se esta proxima estaca � do terreno natural
          {
            if(fabs(itPTSAtual->Cota - itSetEst->Cota) < 0.05)       //--- se sim � perfil do terreno natural 
            {
              const_cast<CPerfilTipoSolo*>(&*itPTS)->IncluiPonto(*itSetEst);     //--- insere a estaca no terreno natural
            }
            else                                 //--- N�o � terreno natural, Interpola a estaca no perfil existente
            {
            /* N�O vejo utilidade en interpolar a cota no perfil...j� que se a se��o n�o for encontrada ela ser� interpolada de qualquer jeito,
               quando sua cota for solicitada.
               No Terreno natural sim porque a cota foi redefinida a partir da triangula��o.

               CEstaca EST(itSetEst->EstVirtual,itPTS->CalculaCota(CEstaca(itSetEst->EstVirtual)));
               const_cast<CPerfilTipoSolo*>(&*itPTS)->IncluiPonto(EST);       
               */
            }
          }
        }
      }
    }
  }
}

/*
void CalculaNota::LimpaTerreno(Perfil& TerrenoAtual,double HLimp)
{
double cota;

for (POSITION Pos = TerrenoAtual.GetHeadPosition() ; Pos ;  TerrenoAtual.GetNext(Pos)) 
{
cota= TerrenoAtual.GetAt(Pos).y;
TerrenoAtual.GetAt(Pos).y -= HLimp;
cota=TerrenoAtual.GetAt(Pos).y;
}
}
*/
/*

void CalculaNota::GravaPerfilGeologico(lstCDadosPerfGeologico& LstPerfGeologicos)
{
CFile ArqPerfGeoCalc;
CFileException e;

std::string NomeCompArquivo(NomeProj + ".pgc");

if (ArqPerfGeoCalc.Open(NomeCompArquivo.data(),CFile::modeCreate | CFile::modeWrite,&e) == 0)
{
monolog.mensagem(13,CString(NomeCompArquivo.data()) + " (Gravando os dados)");

throw e.m_cause;
}
else
{	
CArchive ar(&ArqPerfGeoCalc,CArchive::store);

ar <<  LstPerfGeologicos.size();  //--- volumes e quantidades de secoes que ser�o gravadas.

for(itlstStDadosPerfGeol it = LstPerfGeologicos.begin() ; it != LstPerfGeologicos.end() ; ++it)
{
it->Serialize(ar);
}

ar.Close();
ArqPerfGeoCalc.Close();
}
}
*/

bool CalculaNota::AbreArquivoCalcGeomDetalhado(const CString& NomeProjeto)
{
   CFile ArqCalcSec;
   CFileException e;
   unsigned int QSecoes(0);
   Ponto PontoACalc;

   CFile ArqCalcGeo;

   if (ArqCalcGeo.Open(NomeProjeto + CString(".geo"), CFile::modeRead, &e) == 0)
   {
     monolog.mensagem(1, NomeProjeto + CString(".geo"));

     return false;
   }

   CArchive CArqCalcGeo(&ArqCalcGeo, CArchive::load);

   Ponto CoordEstaca;
   CEstaca EstacaAtual;

   //--- Cria o mapa com as secoes ordenadas para acesso randomico

   for (CArqCalcGeo >> QSecoes; QSecoes; QSecoes--)
   {
     EstacaAtual << CArqCalcGeo;
     PontoACalc << CArqCalcGeo;

     SecoesGeometricas.insert(MSecaoGeom::value_type(EstacaAtual, PontoACalc));
   }

   return true;
}

void CalculaNota::CalculaPontosTerraplProjeto(CSecao& SecAtual)
{
  itMSecaoGeom itSG(SecoesGeometricas.find(SecAtual.Projeto.Estaca));
  bool PrimeiroPonto(true);

  if (itSG != SecoesGeometricas.end())
  {
    pairtymapEstlstPontos pairitPontosTerr;
    POSITION Atual(SecAtual.Projeto.GetHeadPosition());
    double DistProp[2] = { INFINITO,INFINITO };

    const Ponto& PontoInicial(SecAtual.Projeto.GetHead());
    double ExessoLateral(0.0), Limpeza(HLimpeza.BuscaH(SecAtual.Projeto.Estaca, &ExessoLateral));
    Ponto PontoaCalcular;

    //--- Deixar a limpeza por enquanto:
    //--- Para se fazer corretamente a limpeza � preciso alem do talude no fim da limpeza, incluir os pontos de terreno
    //--- Que est�o nela descontando-se a altura da limpeza.
    ///--- Fica para uma vers�o futura

    if (false && ExessoLateral > 0.10)
    {
      if (PontoInicial.Descricao == "OFFSEE")
      {
        Mat.IrradiaPonto(itSG->second.fi, itSG->second.x, itSG->second.y, Matem::M_PI_2, PontoInicial.x - ExessoLateral - 0.2, PontoaCalcular);
        PontoaCalcular.i = PontoInicial.x - ExessoLateral - 0.2;
        PontoaCalcular.z = SecAtual.Terreno.Calculay(PontoaCalcular.i) - Limpeza;

        if (PontoaCalcular.z != INFINITO)
        {
          PontoaCalcular.Descricao = "CLIMPE";                            //--- Crista Limpeza Lado Esquerdo

          pairitPontosTerr = PontosTerrplProjeto.insert(std::pair<Ponto, lstPontos>(Ponto(SecAtual.Projeto.Estaca.EstVirtual, PontoaCalcular.i), lstPontos()));
          pairitPontosTerr.first->second.push_back(PontoaCalcular);

          PrimeiroPonto = false;
        }

        //--- P� da limpeza

        Mat.IrradiaPonto(itSG->second.fi, itSG->second.x, itSG->second.y, Matem::M_PI_2, PontoInicial.x - ExessoLateral, PontoaCalcular);
        PontoaCalcular.i = PontoInicial.x - ExessoLateral;
        PontoaCalcular.z = SecAtual.Terreno.Calculay(PontoaCalcular.i);

        if (PontoaCalcular.z != INFINITO)
        {
          PontoaCalcular.z -= Limpeza;

          PontoaCalcular.Descricao = "LIMPED";

          pairitPontosTerr = PontosTerrplProjeto.insert(std::pair<Ponto, lstPontos>(Ponto(SecAtual.Projeto.Estaca.EstVirtual, PontoaCalcular.i), lstPontos()));
          pairitPontosTerr.first->second.push_back(PontoaCalcular);
        }
      }
    }

    int ContaPontosAlargamentos[2] = { 1,1 };

    while (Atual != NULL)
    {
      const Ponto& PontoAtual(SecAtual.Projeto.GetAt(Atual));
      Mat.IrradiaPonto(itSG->second.fi, itSG->second.x, itSG->second.y, Matem::M_PI_2, PontoAtual.x, PontoaCalcular);

      if (PrimeiroPonto)
      {
        Ponto Alinhamento(SecAtual.Projeto.Estaca.EstVirtual, PontoAtual.x);
        Alinhamento.Descricao = PontoAtual.Descricao;
        Alinhamento.z = SecAtual.CVs[0];
        Alinhamento.i = SecAtual.CVs[1];
        Alinhamento.fi = SecAtual.CVs[2];
        pairitPontosTerr = PontosTerrplProjeto.insert(std::pair<Ponto, lstPontos>(Alinhamento, lstPontos()));
        PrimeiroPonto = false;
      }

      PontoaCalcular.i = PontoAtual.x;
      PontoaCalcular.z = PontoAtual.y;
      PontoaCalcular.S = SecAtual.Projeto.Estaca.EstVirtual;
      PontoaCalcular.AcidenteTopografico = PontoAtual.AcidenteTopografico;
      PontoaCalcular.Baliza = SecoesTipo.EstMudancaSecTipo(PontoaCalcular.S);

      //--- Caso haja, calcula o ponto de passagem no alinhamento dos bordos 

      if (PontoAtual.Descricao == "OFFSEE")
      {
        {
          if (CVsAnteriores[ESQUERDO] != INFINITO)
          {
            if (SecAtual.CVs[ESQUERDO] != INFINITO)
            {
              if ((CVsAnteriores[ESQUERDO] > 0.0 && SecAtual.CVs[ESQUERDO] < 0.0) || (CVsAnteriores[ESQUERDO] < 0.0 && SecAtual.CVs[ESQUERDO] > 0.0))
              {
                double DistTotalx(fabs(CVsAnteriores[ESQUERDO]) + fabs(SecAtual.CVs[ESQUERDO])),
                       DistTotaly(SecAtual.Estaca.EstVirtual - EstacaAnterior);

                DistProp[ESQUERDO] = (DistTotaly / DistTotalx)*fabs(CVsAnteriores[ESQUERDO]);  //--- Distancia no bordo proporcional aos off-sets

                Ponto PontoGeo;
                CEstaca EstacaPP(EstacaAnterior + DistProp[ESQUERDO]);
                PontosPassagem[ESQUERDO].push_back(Ponto(EstacaPP.EstVirtual));    //--- Salva os pontos de passagem parainserir nos bordos
              }
            }
          }
        }
      }
      else
      {
        if (PontoAtual.Descricao == "OFFSED")
        {
          {
            if (CVsAnteriores[DIREITO] != INFINITO)
            {
              if (SecAtual.CVs[DIREITO] != INFINITO)
              {
                if ((CVsAnteriores[DIREITO] > 0.0 && SecAtual.CVs[DIREITO] < 0.0) || (CVsAnteriores[DIREITO] < 0.0 && SecAtual.CVs[DIREITO] > 0.0))
                {
                  double DistTotalx(fabs(CVsAnteriores[DIREITO]) + fabs(SecAtual.CVs[DIREITO])),
                    DistTotaly(SecAtual.Estaca.EstVirtual - EstacaAnterior);

                  DistProp[DIREITO] = (DistTotaly / DistTotalx)*fabs(CVsAnteriores[DIREITO]);  //--- Distancia no bordo proporcional aos off-sets

                  Ponto PontoGeo;
                  CEstaca EstacaPP(EstacaAnterior + DistProp[DIREITO]);
                  PontosPassagem[DIREITO].push_back(Ponto(EstacaPP.EstVirtual));    //--- Salva os pontos de passagem parainserir nos bordos
                }
              }
            }
          }
        }
      }

      //--- Se for ponto de alargamento insere o contador pois na terraplanagem ser� preciso distinguir

      PontoaCalcular.Descricao = PontoAtual.Descricao;

      if (PontoaCalcular.Descricao == "ALARGE") PontoaCalcular.Descricao.Format("%s%02d", PontoaCalcular.Descricao, ContaPontosAlargamentos[ESQUERDO]++);
      if (PontoaCalcular.Descricao == "ALARGD") PontoaCalcular.Descricao.Format("%s%02d", PontoaCalcular.Descricao, ContaPontosAlargamentos[DIREITO]++);

      pairitPontosTerr.first->second.push_back(PontoaCalcular);

      SecAtual.Projeto.GetNext(Atual);
    }

    //---- Deixar a limpeza, por enquanto

    if (false && ExessoLateral > 0.10)
    {
      const Ponto PontoFinal(SecAtual.Projeto.GetTail());

      if (PontoFinal.Descricao == "OFFSED")
      {
        Mat.IrradiaPonto(itSG->second.fi, itSG->second.x, itSG->second.y, Matem::M_PI_2, PontoFinal.x + ExessoLateral, PontoaCalcular);
        PontoaCalcular.i = PontoFinal.x + ExessoLateral;
        PontoaCalcular.z = SecAtual.Terreno.Calculay(PontoaCalcular.i) - Limpeza;

        if (PontoaCalcular.z != INFINITO)
        {
          PontoaCalcular.Descricao = "LIMPED";
          pairitPontosTerr.first->second.push_back(PontoaCalcular);
        }

        Mat.IrradiaPonto(itSG->second.fi, itSG->second.x, itSG->second.y, Matem::M_PI_2, PontoFinal.x + ExessoLateral + 0.2, PontoaCalcular);
        PontoaCalcular.i = PontoFinal.x + ExessoLateral + 0.2;
        PontoaCalcular.z = SecAtual.Terreno.Calculay(PontoaCalcular.i);

        if (PontoaCalcular.z != INFINITO)
        {
          PontoaCalcular.Descricao = "CLIMPD";                            //--- Crista Limpeza dir
          pairitPontosTerr.first->second.push_back(PontoaCalcular);
        }
      }
    }
    CVsAnteriores[ESQUERDO] = SecAtual.CVs[ESQUERDO];
    CVsAnteriores[DIREITO] = SecAtual.CVs[DIREITO];
    EstacaAnterior = SecAtual.Estaca.EstVirtual;
  }
}

bool CalculaNota::GravaPontosTerraplProjeto(const CString& NomeProjeto)
{
  CFileException e;
  CFile ArqPonTerPro;

  if (ArqPonTerPro.Open(NomeProjeto + CString(".ptp"), CFile::modeCreate | CFile::modeWrite, &e) == 0)   //--- Pontos Terraplanagem Projeto
  {
    monolog.mensagem(1, NomeProjeto + CString(".ptp"));

    return false;
  }

  CArchive CArqPonTerPro(&ArqPonTerPro, CArchive::store);

  CArqPonTerPro << PontosTerrplProjeto.size();

  for(ittymapEstlstPontos it = PontosTerrplProjeto.begin() ; it != PontosTerrplProjeto.end() ; it++)
  {
    it->first >> CArqPonTerPro;

    CArqPonTerPro << it->second.size();

    for (register itlstPontos itp = it->second.begin() ; itp != it->second.end(); itp++)
    {
      *itp >> CArqPonTerPro;
    }
  }

  CArqPonTerPro.Flush();

  return true;
}

void CalculaNota::GravaPontosPassagem()
{
  CFile ArqPonPassagem;
  CFileException e;

  std::string NomeCompArquivo(NomeProj + ".pas");

  if (ArqPonPassagem.Open(NomeCompArquivo.data(), CFile::modeCreate | CFile::modeWrite, &e) == 0)
  {
    monolog.mensagem(13, CString(NomeCompArquivo.data()) + " (Gravando os dados)");

    throw e.m_cause;
  }
  else
  {
    CArchive ar(&ArqPonPassagem, CArchive::store);

    for (int LADO = ESQUERDO; LADO <= DIREITO; LADO++)
    {
      ar << PontosPassagem[LADO].size();

      for (itlstPontos it = PontosPassagem[LADO].begin(); it != PontosPassagem[LADO].end(); it++)
      {
        *it >> ar;
      }
    }
  }
}
 