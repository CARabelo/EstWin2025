#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "carqterreno.h"
#include "math.h"
#include "Estaca.h"
#include "cponto.h" 
#include "clpontos.h" 
#include "perfil.h"
#include "afxtempl.h"
#include "supercstring.h"
#include "clistaperfis.h"
#include "xygreide.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "xymouse.h"
#include <set>
#include <string>
#include "mytoolbar.h"
#include <string>
#include "meleditbox.h"
#include "cdpopuppontos.h"
#include <map>
#include "Mat.h"
#include "comboprocdessecao.h"
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "meleditbox.h"
#include "CArqOAC.h"
#include "CArqOAE.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include <algorithm>
#include <list>
#include "CPerfilGeologico.h"
#include "CMemDC.h"
#include "CTipoSolos.h"
#include "DesPerfilGeologicoView.h"
#include "dentraponto.h" 
#include "math.h"
#include "ddeslizantes.h"
#include "deslizantesgeom.h"
#include "palette.h"
#include "childfrm.h"
#include "DconfGeologico.h"
#include "destacacota.h"
#include <iomanip> 
#include <strstream>  
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "CArqTiposSolos.h"
#include "CDSelTipoSolo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ESQUERDO 0
#define DIREITO  1
#define ANTERIOR   0
#define POSTERIOR  1

#define EVENT_MOUSE_STOP 1000
#define NENHUM_TIMER 0

#include "dialogo.h"
extern class dialogo dialogar;
#include "monologo.h"
extern class monologo monolog; 
#include "Mat.h"
extern class Matem Mat; 

/////////////////////////////////////////////////////////////////////////////
// DesPerfilGeoView
IMPLEMENT_DYNCREATE(DesPerfilGeoView, CView)

  DesPerfilGeoView::DesPerfilGeoView() : Arrastando(FALSE),DeltaYMetro(0.0),PAtual(NULL),TerminarArrasto(FALSE) ,
  DPopUpPonto(NULL,RGB(255,255,166)),LimiteEsq(INFINITO),LimiteDir(-INFINITO),LimiteSup(-INFINITO),LimiteInf(INFINITO),
  OAC(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual())),OAE(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual())),CursorAtual(IDC_HAND),
  PermitirArrastar(true),PerfilGeologico(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()).GetBuffer()),EZoom(false),itPosAnterior(setEstacasTerreno.end()),
  itPosAtual(setEstacasTerreno.end()),Tipo(NENHUMTIPO),pPontoMouse(NULL),pPosAnterior(NULL),itPerfilAtual(PerfilGeologico.PegaPerfisSolos().end()),
  itPerfilAnterior(PerfilGeologico.PegaPerfisSolos().end()),InserindoPonto(false),PonMouseClicado(0,0),PonMousePixelsAtual(0,0),PonMousePixelsAnterior(0,0),
  pEstSendoMovida(NULL),ArrastandoPonto(false),PontoOriginal(INFINITO,INFINITO,0),itPerfilMarcado(PerfilGeologico.PegaPerfisSolos().end()),
  itInicioTrechoTerreno(setEstacasTerreno.end()),itFimTrechoTerreno(setEstacasTerreno.end()),TrechoTerrenoCopiado(false),CopiarApenasTrecho(false),MoverApenasEixoy(false),
  TrechoPerfilGeolCopiado(false),CircularPontos(false),AlterouPerfil(false),itPerfilSelecionado(PerfilGeologico.PegaPerfisSolos().end()),MarcandoTrecho(false),TrechoMarcado(false),
  PegouPrimeiroPonto(false),Trecho(((CMainFrame*) AfxGetMainWnd())->PegaRuaAtual())
{
  Resolucao[X] = 1024.0 ; Resolucao[Y] = 768.0;
  Escala[X] = Escala[Y] = 100.0;
  Deltas[X] = Deltas[Y] = 0.0;

  LeTerreno(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()).GetBuffer());
  CalculaCotasFurosSondagem();
  PerfilGeologico.Serialize(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual(),0);

  CalculaIsTerrenoPrimitivo();
}

DesPerfilGeoView::~DesPerfilGeoView()
{
  if(AlterouPerfil) GravarProjeto(1,true,true);

  CMainFrame* MainFrame((CMainFrame*) AfxGetMainWnd());
  std::string NomeArquivo(MainFrame->PegaProjetoAtual() + std::string(".ini"));

  //--- Grava os parametros atuais da janela

  CString CSDados,CSPerfSelecionado;   //--- (itPerfilSelecionado == PerfilGeologico.PegaPerfisSolos().end() ? "" : itPerfilSelecionado->PegaTipoSolo().c_str());   Pode ter 2 perfís com o mesmo nome

  CSDados.Format("%lf,%lf,%lf,%lf,%lf,%s",Escala[X],Escala[Y],RazaoHV,Deltas[X],Deltas[Y],CSPerfSelecionado.GetBuffer());

  ::WritePrivateProfileString(_T("DadosDesenhos"),_T("DadosPerfilGeologico"),CSDados,NomeArquivo.c_str());		

  //--- Grava as cores

  CSDados.Format("%lu,%lu,%lu,%lu,%lu,%lu",Cores[0],Cores[1],Cores[2],Cores[3],Cores[4],Cores[5]);
  ::WritePrivateProfileString(_T("DadosDesenhos"),_T("DadosDesGeolCores"),CSDados,NomeArquivo.c_str());

  //--- Salva o estado  da configuração do desenho  

  CSDados.Format("%i,%i,%i,%i,%i,%i,%i,%i,%i",DesenharMalha,Cotar,EquiMalhaX,EquiMalhaY,EquiCotas,MostrarDesc,PermitirArrastar,CircularPontos,MoverApenasEixoy);
  ::WritePrivateProfileString(_T("DadosDesenhos"),_T("DadosDesGeolConfig"),CSDados,NomeArquivo.c_str());

  CSDados.Format("%i,%i",/*MostrarToolCoordenadas*/1,MostrarPonto);
  ::WritePrivateProfileString(_T("SettingsChild"),_T("StatusVisualizarGeol"),CSDados,NomeArquivo.c_str());
}

void DesPerfilGeoView::GravarProjeto(int Tipo,bool Forcar,bool Perguntar)      //--- Chamado pelo menu Calcular volumes (Atualiza os arquivos antes de calcular)
{
#ifdef DEMOVER
  ::MessageBox(NULL,"Função indisponível na cópia de demonstração","VU&E",MB_OK);
#else
  int Resposta(IDOK);

  if(Perguntar) Resposta = dialogar.DoModal(4, CString(" Perfil Geológico - ") + CString(Trecho.c_str()));

  if (Resposta == IDOK) 
  {
    std::string NomeArq(((CMainFrame*) AfxGetMainWnd())->PegaProjetoBase());
    NomeArq = NomeArq.substr(0,NomeArq.find_last_of('\\')+1);
    NomeArq += Trecho + '\\' + Trecho;

    PerfilGeologico.Serialize(NomeArq.c_str(),1);
    AlterouPerfil = false;
  }
#endif
}

BEGIN_MESSAGE_MAP(DesPerfilGeoView, CView)
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_RBUTTONDBLCLK()
  ON_WM_SETFOCUS()
  ON_WM_RBUTTONUP()
  ON_WM_ERASEBKGND()
  ON_WM_CLOSE()
  ON_WM_RBUTTONDOWN()
  ON_WM_SIZE()
  ON_WM_CREATE()
  ON_WM_KEYUP()
  ON_WM_MOUSEWHEEL()
  ON_COMMAND(ID_BUTARRASTAR, &DesPerfilGeoView::OnButarrastar)
  ON_COMMAND(ID_GRAVAR, OnGravarperfil)
  ON_COMMAND(ID_BUTLOCK,OnTrancarGreide)
  ON_COMMAND(ID_ENQUADRAR, OnEnquadrar)
  ON_COMMAND(ID_POPUP_REMOVER_TODO_PERFIL, &DesPerfilGeoView::OnPopupRemoverTodoPerfil)
  ON_COMMAND(ID_POPUP_PERFIL_SAIR, &DesPerfilGeoView::OnPopupPerfilSair)
  ON_COMMAND(ID_POPUP_REMOVER_TRECHO_PERFIL, &DesPerfilGeoView::OnPopupRemoverTrechoPerfil)
  ON_COMMAND(ID_TERRENO_COPIARTODOOPERFIL, &DesPerfilGeoView::OnTerrenoCopiartodooperfil)
  ON_COMMAND(ID_TERRENO_COPIARAPENASESTETRECHO, &DesPerfilGeoView::OnTerrenoCopiarapenasestetrecho)
  ON_COMMAND(ID_TERRENO_SAIR, &DesPerfilGeoView::OnTerrenoSair)
  ON_COMMAND(ID_BUTMOVSOY, &DesPerfilGeoView::OnButmovesoy)
  ON_COMMAND(ID_POPUP_PERFIL_SELECIONAR, &DesPerfilGeoView::OnPopupPerfilSelecionar)
  ON_COMMAND(ID_PERFIL_COPIARTODOPERFIL, &DesPerfilGeoView::OnPerfilCopiartodoperfil)
  ON_COMMAND(ID_PERFIL_COPIARAPENASESTETRECHO, &DesPerfilGeoView::OnPerfilCopiarapenasestetrecho)
  ON_COMMAND(ID_POPUP_MUDAR_TIPO, &DesPerfilGeoView::OnPopupMudarTipo)
  ON_COMMAND(ID_QUALQUERTRECHO_REMOVER, &DesPerfilGeoView::OnQualquertrechoRemover)
  ON_COMMAND(ID_QUALQUERTRECHO_COPIAR, &DesPerfilGeoView::OnQualquertrechoCopiar)
  ON_COMMAND(ID_QUALQUERTRECHO_SAIR, &DesPerfilGeoView::OnQualquertrechoSair)
  ON_COMMAND(ID_RESTTODOTERRENO, &DesPerfilGeoView::OnRestTodoTerreno)
  ON_COMMAND(ID_RESTESTETRECHOTERR, &DesPerfilGeoView::OnRestEsteTrechoTerreno)
  ON_COMMAND(ID_BUTCORPERFATUAL, OnCorPerfAtual)
  ON_COMMAND(ID_POPUP_VISUALIZARPERFILSELECIONADO, &DesPerfilGeoView::OnPopupVisualizarperfilselecionado)
  ON_COMMAND(ID_CONFDESGREIDE, OnConfDesGeologico)
  ON_COMMAND(ID_IRPARAPONTO, OnIrparaponto)
  ON_COMMAND(ID_ALTERNARTITULOGREIDE, OnAlternartitulogreide)
  ON_COMMAND(ID_BUTFECHar, OnButFechar)
  ON_UPDATE_COMMAND_UI(ID_GRAVAR, OnUpdateGravarperfil)
  ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DesPerfilGeoView drawing

void DesPerfilGeoView::OnDraw(CDC* ppDC)
{
  if(setEstacasTerreno.size() == 0) return;

  if (!ppDC) return;      

  CMMemDC pDC (ppDC);

  CPen* pPenaAnterior,PenaZoom;                     

  PenaZoom.CreatePen(PS_DASH,2,Cores[CORMALHA]);
  pPenaAnterior = pDC->SelectObject(&PenaZoom); 

  if (EZoom)
  {
    CRect RectZoom(InicioZoom,FimZoom);  
    CBrush brushBack(Cores[CORFUNDO]);
    CBrush* pOldBrush = pDC->SelectObject(&brushBack);
    pDC->Rectangle(&RectZoom);

    pDC->SelectObject(pOldBrush);
  }

  bool PrimeiroMovimento(true);

  //--- Desenha a malha

  CPen PenaMalha;

  PenaMalha.CreatePen(PS_SOLID,1,Cores[CORMALHA]);
  pDC->SelectObject(&PenaMalha);		

  if(DesenharMalha)
  {
    //--- Deltas x e y neste desenho estão em pixels. 

    double DELTAXMTS((double)EquiMalhaX * Escala[X]),DELTAYMTS((double)EquiMalhaY * Escala[Y]),
           LARGVIDEOMTS(LarguraVideo * Escala[X]) , ALTVIDEOMTS(AlturaVideo * Escala[Y]),
           XInicial(fmod(Deltas[X] / Escala[X],(double) EquiMalhaX) * Escala[X]),
           YInicial(fmod((Deltas[Y] + ALTVIDEOMTS) / Escala[Y],(double) EquiMalhaY) * Escala[Y]); 				//--- O y é invertido..

    //--- Malha minima = 2 pixels

    if(DELTAXMTS < 2.0) DELTAXMTS = 2.0;
    if(DELTAYMTS < 2.0) DELTAYMTS = 2.0;

    double XAtual(XInicial); 
    for(; XAtual < LARGVIDEOMTS ; XAtual += DELTAXMTS)
    {
      pDC->MoveTo(XAtual,0);
      pDC->LineTo(XAtual,ALTVIDEOMTS); 
    }

    double YAtual = YInicial;
    for(; YAtual < ALTVIDEOMTS; YAtual += DELTAYMTS)
    {
      pDC->MoveTo(0,YAtual);														//--- ALTVIDEOMTS - YAtual porque y cresce para baixo
      pDC->LineTo( LARGVIDEOMTS,YAtual); 
    }

    //--- Texto de debug

    {
      /*
      CFont* pFonteAnterior(pDC->SelectObject(&CurrierNewHorizontal));
      pDC->SetTextAlign(TA_LEFT);
      pDC->SetTextColor(Cores[CORTEXTO]);
      pDC->SetBkColor(Cores[CORFUNDO]);

      //--- Texto de DEBUG

     
      CString Texto;
      Texto.Format("Anterior: %-ld %-ld",PonMousePixelsAnterior.x,PonMousePixelsAnterior.y);
      pDC->TextOut(100,100,Texto);
      Texto.Format("Atual: %-ld %-ld",PonMousePixelsAtual.x,PonMousePixelsAtual.y);
      pDC->TextOut(100,120,Texto);
      */
      
    }

    //--- cota a malha

    if(Cotar)
    {
      CFont* pFonteAnterior = pDC->SelectObject(&CurrierNewVertical);
      pDC->SetTextAlign(TA_BOTTOM | TA_LEFT);
      pDC->SetTextColor(Cores[CORTEXTO]);
      pDC->SetBkColor(Cores[CORFUNDO]);

      CString Coord;
      const unsigned long EquiCota(EquiMalhaX * EquiCotas);

      CEstaca EstacaAtual(floor(Deltas[X] / Escala[X] / (double)EquiMalhaX) * -EquiMalhaX);			//--- Valor da cota onde esta a coordenada + 1 pixel
      if(EstacaAtual.EstVirtual > 0.0) EstacaAtual = EstacaAtual.EstVirtual - EquiMalhaX; 												//--- Pega o múltiplo inferior
      unsigned int QuantX(floor((double )((double)ALTVIDEOMTS / (double)DELTAYMTS)) / (double)EquiCotas), 				//--- cota 3 linhas da malha x
                   QuantY(floor((double)(LARGVIDEOMTS / (double)DELTAXMTS)) / (double)EquiCotas);				          //--- cota 4 linhas da malha Y

      DELTAXMTS *= EquiCotas;

      for(XAtual = XInicial ; XAtual < LARGVIDEOMTS ; XAtual += DELTAXMTS)
      {
        double Resto(fabs(fmod(EstacaAtual.EstVirtual,20.0)));

        std::string EstacaMostrar(EstacaAtual.GetBuffer());

        if(Resto < 0.005) EstacaMostrar = EstacaMostrar.substr(0,EstacaMostrar.find('+'));
        else if((Resto - int(Resto)) < 0.005)EstacaMostrar = EstacaMostrar.substr(0,EstacaMostrar.find('.'));

        pDC->TextOut(XAtual-1,ALTVIDEOMTS-4,EstacaMostrar.c_str());
        EstacaAtual += EquiCota;
      }

      pDC->SelectObject(&CurrierNewHorizontal);

      DELTAYMTS *= EquiCotas;
      long CotaY(floor((Deltas[Y]+ALTVIDEOMTS-YInicial)/Escala[Y])),DeltaCota(EquiCotas * EquiMalhaY);      

      for(YAtual = YInicial; YAtual < ALTVIDEOMTS; YAtual += DELTAYMTS)
      {
        Coord.Format("%-ld",CotaY);
        pDC->TextOut(3,YAtual,Coord);
        CotaY -= DeltaCota;
      }
      pDC->SelectObject(pFonteAnterior);
    }
  }

  CPen Pena[TOTALPENAS]; 

  if (Pena[CORTERRENO].CreatePen(PS_SOLID,1,Cores[CORTERRENO]))
  {
    //--- Desenha o Terreno
    //*************************

    if(false)   //--- Não desenha o terreno  
    {
      pDC->SelectObject(&Pena[CORTERRENO]);    

      //--- Os valores dos pixels são calculados na chamada das funções <MoveTo> e <LineTo>
      //--- A coordenada x em pixels é calculada pela fórmula:
      //--- Sendo:
      //---   px           -> Valor da coordenada x em pixels.
      //---   p(x)         -> Valor do afastamento x do ponto.
      //---   Escala(x)    -> Valor da escala(Pixels/metro) no eixo x.
      //---   Resolução(x) -> Valor da resolução no eixo x (em pixels).
      //---   D(x)         -> Valor do Deslocamento (tecla seta para a direita ou esquerda)
      //---
      //--- px = D(x) + (p(x) * escala(x) + Resolução(x) / 2).
      //---
      //---  "Resolução(x) / 2"  porque considera-se o x = 0 no meio do eixo x.
      //---
      //--- A Coordenada y em pixels é calculada pela fórmula:
      //---
      //--- py = D(y) + (Resolução y - (p(y)  * escala(y)))
      //---
      //--- " Resolucao y -"  porque as coordenadas do video são espelhadas no eixo x
      //--- ou seja:
      //---
      //--- são assim:       O------> (x)
      //---                  |
      //--- 			      	   |
      //--- 				         |
      //---	  			         V (y)
      //---
      //--- E queremos o desenho assim:
      //---
      //---                  ^ (y)
      //---                  |
      //---                  |
      //---                  |
      //---                  O------> (x)
      //---

      //--- Existem funções gráficas que já fazem isso!!!! (Valeu como exercício)

      POINT P;

      ittysetEstaca it(setEstacasTerreno.begin());

      while(it!=setEstacasTerreno.end() && (it->Cota == INFINITO || it->Cota < 0.01 || it->EstVirtual == INFINITO)) ++it;

      if(it == setEstacasTerreno.end()) return;

      P.x = (int) (Deltas[X] + (it->EstVirtual*Escala[X]));
      P.y = (int) (Deltas[Y] + (Resolucao[Y] - (it->Cota) * Escala[Y]));
      pDC->MoveTo(P);                             
      if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0);   //--- circula o ponto

      ++it;

      while(it!=setEstacasTerreno.end()) 
      {
        if(it->Cota != INFINITO && it->Cota > 0.01)
        {
          P.x = (int) (Deltas[X] + (it->EstVirtual*Escala[X]));
          P.y = (int) (Deltas[Y] + (Resolucao[Y] - (it->Cota) * Escala[Y]));
          pDC->LineTo(P);                             
          if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0);   //--- circula o ponto
        }
        ++it;
      }
    }
  }

  DesenhaFuroSondagem(pDC);
  DesenhaPerfisGeologicos(pDC,&Pena[CORTERRENO]);

  //--- Desenha o perfil que está marcado

  //  if(itPerfilMarcado != PerfilGeologico.PegaPerfisSolos().end() && pitInicioTrecho == NULL)

  CPen PenaMarcada;
  PenaMarcada.CreatePen(PS_SOLID,4,RGB(190,190,190));
  pDC->SelectObject(&PenaMarcada);  

  if(itPerfilMarcado != PerfilGeologico.PegaPerfisSolos().end())
  {
    ittyPerfilTipoSolo itPontoAtual(itPerfilMarcado->PegaPerfil().begin()),itPontoAnterior(itPontoAtual); 

    pDC->MoveTo((int) (Deltas[X] + (itPontoAtual->EstVirtual*Escala[X])),
      (int) (Deltas[Y] + (Resolucao[Y] - (itPontoAtual->Cota) * Escala[Y])));

    while (++itPontoAtual != itPerfilMarcado->PegaPerfil().end())
    {
      pDC->LineTo((int) Deltas[X] + (itPontoAtual->EstVirtual*Escala[X]),
        (int) (Deltas[Y] + (Resolucao[Y] - (itPontoAtual->Cota) * Escala[Y])));

      itPontoAnterior = itPontoAtual; 
    }
  }

  if(TrechoMarcado)                   
  {
    ittyPerfilTipoSolo itPontoAtual(itInicioTrecho),itPontoAnterior(itPontoAtual); 

    pDC->MoveTo((int) (Deltas[X] + (itPontoAtual->EstVirtual*Escala[X])),
                (int) (Deltas[Y] + (Resolucao[Y] - (itPontoAtual->Cota) * Escala[Y])));

    while (itPontoAtual++ != itFimTrecho)
    {
      pDC->LineTo((int) Deltas[X] + (itPontoAtual->EstVirtual*Escala[X]),
        (int) (Deltas[Y] + (Resolucao[Y] - (itPontoAtual->Cota) * Escala[Y])));

      itPontoAnterior = itPontoAtual; 
    }
  }        

  if(itInicioTrechoTerreno != setEstacasTerreno.end())           //--- Marca o trecho de terreno que estiver marcado
  {
    CPen PenaMarcada;
    PenaMarcada.CreatePen(PS_SOLID,4,RGB(190,190,190));
    pDC->SelectObject(&PenaMarcada);    

    ittysetEstaca itIni(itInicioTrechoTerreno);

    pDC->MoveTo((int) (Deltas[X] + (itIni->EstVirtual*Escala[X])),(int) (Deltas[Y] + (Resolucao[Y] - (itIni->Cota) * Escala[Y])));

    for (++itIni ; itIni != itFimTrechoTerreno ; ++itIni)
    {
      pDC->LineTo((int) (Deltas[X] + (itIni->EstVirtual*Escala[X])),(int) (Deltas[Y] + (Resolucao[Y] - (itIni->Cota) * Escala[Y])));
    }

    if(itFimTrechoTerreno != setEstacasTerreno.end())  pDC->LineTo((int) (Deltas[X] + (itIni->EstVirtual*Escala[X])),(int) (Deltas[Y] + (Resolucao[Y] - (itIni->Cota) * Escala[Y])));
  }

  pDC->SelectObject(pPenaAnterior);   
}
/////////////////////////////////////////////////////////////////////////////
// DesPerfilGeoView diagnostics

#ifdef _DEBUG
void DesPerfilGeoView::AssertValid() const
{
  CView::AssertValid();
}

void DesPerfilGeoView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// DesPerfilGeoView message handlers

void DesPerfilGeoView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();

  CMainFrame* MainFrame((CMainFrame*) AfxGetMainWnd());
  std::string NomeArquivo(MainFrame->PegaProjetoAtual() + std::string(".ini"));

  //--- le o estado anterior da configuração do desenho e atualiza-os 

  char Buffer[512] = {0};

  //--- Salva o estado  da configuração do desenho  

  ::GetPrivateProfileString(_T("DadosDesenhos"),_T("DadosDesGeolConfig"),_T("1,1,20,2,3,1,1,1,0"),Buffer,511,NomeArquivo.c_str());
  _stscanf(Buffer,"%d,%d,%d,%d,%d,%d,%d,%d,%d",&DesenharMalha,&Cotar,&EquiMalhaX,&EquiMalhaY,&EquiCotas,&MostrarDesc,&PermitirArrastar,&CircularPontos,&MoverApenasEixoy);

  ((CChildFrame*) GetParentFrame())->m_wndToolBarPerfGeo.GetToolBarCtrl().SetState(ID_BUTARRASTAR,PermitirArrastar); 
  ((CChildFrame*) GetParentFrame())->m_wndToolBarPerfGeo.GetToolBarCtrl().SetState(ID_BUTMOVSOY,MoverApenasEixoy); 
  ((CChildFrame*) GetParentFrame())->MudaCorPerfGeoSel(RGB(0,0,0));

  //--- mostra ou não o título da janela

  if (!MostrarTitulo) ((CChildFrame*) GetParentFrame())->ModifyStyle(WS_CAPTION,0,SWP_FRAMECHANGED);  
  //--- Pega as cores

  ::GetPrivateProfileString(_T("DadosDesenhos"),_T("DadosDesGeolCores"),_T("0,4227072,12632256,255,33023,16711680"),Buffer,511,NomeArquivo.c_str());
  _stscanf(Buffer,"%lu,%lu,%lu,%lu,%lu,%lu",&Cores[0],&Cores[1],&Cores[2],&Cores[3],&Cores[4],&Cores[5]);

  MouseXYGre = (CXYGreide *) MainFrame->MouseXYGre;

  char chTipoPerfil[511] = {0};

  //--- Pega o status da janela e atualiza-a

  ::GetPrivateProfileString(_T("DadosDesenhos"),_T("DadosPerfilGeologico"),_T("-1.0,10.0,1.0,0.0,0.0,"),Buffer,511,NomeArquivo.c_str()); 
  _stscanf(Buffer,"%lf,%lf,%lf,%lf,%lf,%s",&Escala[X],&Escala[Y],&RazaoHV,&Deltas[X],&Deltas[Y],&chTipoPerfil);

  SelecionaPerfil(&chTipoPerfil[0]);

  if(Escala[X] == -1.0) OnEnquadrar();

  DefineParametros();

  //--- Inicializa os valores da escala, da razão e da tolerância.

  ((CChildFrame*) GetParentFrame())->DeslizantesGeol.MudarEscala(Escala[X]*100.0);
  ((CChildFrame*) GetParentFrame())->DeslizantesGeol.MudarRazao(RazaoHV);
  ::GetPrivateProfileString(_T("SettingsChild"),_T("StatusVisualizarGeol"),_T("1,1"),Buffer,511,NomeArquivo.c_str());	

  int Lixo(0);

  _stscanf(Buffer,"%i,%i",&Lixo/*MostrarToolCoordenadas*/,&MostrarPonto);

  // MainFrame->ShowControlBar(MainFrame->PegaBarra(6),MostrarToolCoordenadas, FALSE);	

  DPopUpPonto.Create();     //--- dialogo popup do ponto

  //--- Parametros do tamanho da janela

  RECT Coord; 
  GetClientRect(&Coord);

  Resolucao[X] = Coord.right; 
  Resolucao[Y] = Coord.bottom;

  CalculaParametros();

  pDC = GetDC();         //--- Pega o DC (proprietário)

  if(!pDC) throw -1;

  //--- Fontes para escrita horizontal e vertical

  CFont* LF(pDC->GetCurrentFont());  
  LOGFONT Log;

  LF->GetLogFont(&Log);
  Log.lfEscapement = 900 ;

  strcpy(Log.lfFaceName,"Courier New");
  CurrierNewVertical.CreateFontIndirect(&Log);

  Log.lfEscapement = 0;
  CurrierNewHorizontal.CreateFontIndirect(&Log);

  CArqTiposSolos ArqTiposSolos(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()));
  ArqTiposSolos.CriaSet(SetTiposSolos);
  ArqTiposSolos.ConverteGDI(SetTiposSolos);

  //((CChildFrame*) GetParentFrame())->MudaCorPerfGeoSel(RGB(0,0,255));    //--- Era pra funcionar....mas não funciona

  if(PerfilGeologico.PegaPerfisSolos().size() == 0)                        //--- Se 1a vez cria os trecho de perfis 
  {
    if(PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem().size() == 0) AfxMessageBox("Não foram encontrados furos de sondagem");

    if(setEstacasTerreno.size() > 1)
    {
      itInicioTrechoTerreno = setEstacasTerreno.begin();
      itFimTrechoTerreno = setEstacasTerreno.end();
      --itFimTrechoTerreno;

      PerfilGeologico.CriaPerfisNosTrechos(itInicioTrechoTerreno,itFimTrechoTerreno,Ponto(0.0,0.0));
      itInicioTrechoTerreno = itFimTrechoTerreno = setEstacasTerreno.end();

      AlterouPerfil = true;
    }
  }

  RedrawWindow();
}

void DesPerfilGeoView::CalculaParametros()
{
  //--- Calcula a Largura e a Altura do video, que depende da resolucao e da escala
  //--- em pixels por metro.
  //--- Dependendo da resolução do video a escala deve variar, os valores default são:
  //---
  //--- 1024 x 768 -> 20 pixels por metro.
  //--- 640 X 480  -> 12,5 pixels por metro (proporcionalmente), todas as escalas
  //--- tem seu velor default proporcionalmente ao de 1024 x 768, Esta escala varia
  //--- quando o usuário pressiona o botão + ou o botão - (Alargar ou encolher o desenho).

  AlturaVideo = Resolucao[Y] / Escala[Y];
  LarguraVideo = Resolucao[X] / Escala[X];

  //--- A escala mudou, acerta as tolerâncias (6 pixels em qualquer eixo);

  Toleranciax = 2.0 / Escala[X];
  Toleranciay = 2.0 / Escala[Y];

  Toleranciaxy_2 = pow(Toleranciax,2.0) + pow(Toleranciay/RazaoHV,2.0);
}

BOOL DesPerfilGeoView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
  // TODO: Add your specialized code here and/or call the base class

  dwStyle |= WS_MAXIMIZEBOX | WS_SYSMENU |WS_MINIMIZEBOX | WS_MAXIMIZE;

  return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void DesPerfilGeoView::OnMouseMove(UINT nFlags, CPoint point) 
{
  SetCursor(LoadCursor(NULL,CursorAtual));

  //--- Se não houve movimento do mouse retorna;

  if(PonMousePixelsAtual == point) return;

  PonMousePixelsAtual = point;

  //--- Se estiver executando um zoom, desenha o retangulo e sai

  if(EZoom)
  {
    FimZoom = point;
    RedrawWindow();

    return;
  }

  RedrawWindow();

  if(InserindoPonto)
  {  
    //  if(PonMousePixelsAtual.x < PonMouseClicado.x) return;     //--- O desenho do perfil não pode retroceder

    if(!Arrastando)
    {
      DesenharApagar(OBJETO_LINHA,PonMouseClicado,PonMousePixelsAnterior,NULL,OPERACAO_APAGAR);
      DesenharApagar(OBJETO_LINHA,PonMouseClicado,PonMousePixelsAtual,NULL,OPERACAO_DESENHAR);

      PonMousePixelsAnterior = PonMousePixelsAtual;
    }
  }

  if(TrechoTerrenoCopiado/*  || TrechoMarcado*/) 
  {
    DesenharApagar(OBJETO_PERFIL_TERRENO,PonMousePixelsAnterior,PonMouseClicado,NULL,OPERACAO_APAGAR);     //--- Neste caso é diferente pois não tem o ponto anterior
    DesenharApagar(OBJETO_PERFIL_TERRENO,PonMouseClicado,PonMousePixelsAtual,NULL,OPERACAO_DESENHAR);

    PonMousePixelsAnterior = PonMousePixelsAtual;
  }

  if(TrechoPerfilGeolCopiado) 
  {
    DesenharApagar(OBJETO_PERFIL_GEOLOGICO,PonMousePixelsAnterior,PonMouseClicado,NULL,OPERACAO_APAGAR);     //--- Neste caso é diferente pois não tem o ponto anterior
    DesenharApagar(OBJETO_PERFIL_GEOLOGICO,PonMouseClicado,PonMousePixelsAtual,NULL,OPERACAO_DESENHAR);

    PonMousePixelsAnterior = PonMousePixelsAtual;
  }

  if(MarcandoTrecho) 
  {
    if(Arrastando) PonMouseClicado = TransfCoordReaisParaVideo(*itInicioTrecho);

    DesenharApagar(OBJETO_LINHA,PonMouseClicado,PonMousePixelsAnterior,NULL,OPERACAO_APAGAR);
    DesenharApagar(OBJETO_LINHA,PonMouseClicado,PonMousePixelsAtual,NULL,OPERACAO_DESENHAR);

    PonMousePixelsAnterior = PonMousePixelsAtual;

    DPopUpPonto.MostraDescricao(point.x,point.y,std::string("Marcar Trecho: \r\nSelecione o segundo ponto."));
  }

  static POSITION PosAnterior(NULL);

  //--- Mostra as coordenadas de onde o mouse está, Estaca e cota.

  TransfPixelsXY(point,MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY);  //--- Pega as coordenadas (em metros)do ponto local o mouse está.
  MouseXYGre->UpdateData(FALSE);

  if (ArrastandoPonto)    //--- Esta Arrastando um ponto  de perfil?
  {
    if(MouseXYGre->m_DOCoorX > LimitesMoverEstaca[0] && MouseXYGre->m_DOCoorX < LimitesMoverEstaca[1])    //--- Verifica se esta depois do ponto anterior e antes do ponto posterior
    {
      pEstSendoMovida->AlteraValor(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY);

      CRect Rect;
      GetWindowRect(&Rect);

      if(MostrarDesc) DPopUpPonto.MostraDescricao(point.x+0+Rect.left,point.y+0+Rect.top,PreparaDescricao(pPontoMouse,Tipo));

      RedrawWindow();
    }
  }
  else
  {
    if (Arrastando)     //--- Está arrastando?
    {
      //    bool Ok(true);

      //--- Esta variavel (ArrastandoPonto) se faz necessária porque quando o mouse der pulos
      //--- Maiores que a tolerância de aproximação do ponto (tolerâncias x e y) o ponto não 
      //--- seria mais encontrado nas chamadas a PosicionaPonto (do perfil), fazendo-o parar 
      //--- de ser arrastado no meio da operação. Esta variável assegura o arrasto do ponto
      //--- Independentemente dos saltos do mouse, ela apenas retorna a FALSE , no final da 
      //--- operação, no método LPbuttonUP.
      {
        if(PermitirArrastar)   
        {
          Deltas[X] += point.x - InicioArrast.x;  
          Deltas[Y] += point.y - InicioArrast.y;

          InicioArrast = point;
          DeltaYMetro = Deltas[Y] / Escala[Y];    //--- Deslocamento do Arrasto.
        }
      }
      //   if(Ok)  RedrawWindow();
    }
    else   //--- Não está arrastando nada...
    {
      //--- Se o mouse estiver sobre algum ponto mostra as coordenadas do ponto.
      //--- Caso contrário, apaga o diaslogo popup

      Tipo = TODOSTIPOS;

      pPontoMouse = VerfPonto(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY,Tipo);

      if (pPontoMouse != NULL)
      {
        //  if (pPosAnterior != pPontoMouse)  //---PosAnterior != pos para evitar chamadas desnecessarias ao desenho; 
        {
          pPosAnterior = pPontoMouse;  //--- Atualiza a anterior

          CRect Rect;
          GetWindowRect(&Rect);
          if(MostrarDesc) DPopUpPonto.MostraDescricao(point.x+0+Rect.left,point.y+0+Rect.top,PreparaDescricao(pPontoMouse,Tipo));
        }
      }
      else 
      {
        PAtual = PosAnterior = NULL;
        pPosAnterior = NULL;
        if(!MarcandoTrecho) DPopUpPonto.ShowWindow(false);    //--- Se não estiver sobre um ponto apaga o pop up, exeto se estiver marcando um trecho
      }
    }
  }
}

void DesPerfilGeoView::TransfPixelsXY(CPoint& Ponto,double& Coorx,double& Coory)
{
  //--- Mapeia as cordenadas do ponto que estão em pixels para metros.
  //---  
  //--- O cálculo é feito pela fórmula.
  //--- Sendo.
  //---  px           -> Valor da coordenada x em pixels.
  //---  p(x)         -> Valor do afastamento (x) do ponto.
  //---  Escala(x)    -> Valor da escala(Pixels/metro) no eixo x.
  //---  Resolução(x) -> Valor da resolução no eixo x (em pixels).
  //---  D(x)         -> Valor do Deslocamento (tecla seta para a direita ou para a esquerda)
  //---  C(x)         -> Valor da coordenada x do ponto em metros.
  //---
  //---  C(x) = (p(x) - D(x)) / Esc(x).
  //---

  Coorx = (Ponto.x - Deltas[X]) / Escala[X];

  //---  E para y:   (O sentido é invertido...)
  //---
  //---  C(y) = (Resolucao(y) - p(y) + D(y)) / Esc[Y] ;
  //---

  Coory = (Resolucao[Y] - Ponto.y  + Deltas[Y]) / Escala[Y] ;
}

void DesPerfilGeoView::MudarEscala(int Fator)
{
  double CotaCentral(AlturaVideo / 2.0 + (Deltas[Y] / Escala[Y])),EstacaCentral(LarguraVideo / 2.0 + (-Deltas[X] / Escala[X]));

  //--- Acerta as escalas

  Escala[X] = Escala[Y] = Fator / 100.0;
  Escala[Y] /= RazaoHV;

  CalculaParametros();

  //--- Mantem o ponto central no centro do video.

  Deltas[X] = -Escala[X] * (EstacaCentral - (LarguraVideo / 2.0)); 
  Deltas[Y] = Escala[Y] * (CotaCentral - (AlturaVideo / 2.0)); 

  RedrawWindow();
}

void DesPerfilGeoView::MudarRazao(double Fator)
{
  if (Escala[X] < 0.1 ||  Escala[X] > 8.0) return;

  double CotaCentral(AlturaVideo / 2.0 + (Deltas[Y] / Escala[Y])),
         EstacaCentral(LarguraVideo / 2.0 + (-Deltas[X] / Escala[X]));

  RazaoHV = Fator;

  Escala[Y] = Escala[X]  / RazaoHV;

  CalculaParametros();

  //--- Mantem o ponto central no centro do video.

  Deltas[X] = -Escala[X] * (EstacaCentral - (LarguraVideo / 2.0)); 
  Deltas[Y] = Escala[Y] * (CotaCentral - (AlturaVideo / 2.0)); 

  RedrawWindow();
}

void DesPerfilGeoView::VisualizarPonto()
{
  CDEstacaCota DEntraEstCota(this);

  if(DEntraEstCota.DoModal() == IDOK)
  {
    //--- Acerta os deltas para deslocar o desenho.

    CEstaca EstacaDig(DEntraEstCota.m_EDEstaca);
    double CotaDig(DEntraEstCota.m_DOCota);

    CPoint P((int) (-EstacaDig.EstVirtual * Escala[X]),(int) (CotaDig * Escala[Y]));

    Deltas[X] = P.x;
    Deltas[Y] = (P.y ) - (AlturaVideo * Escala[Y]);

    Deltas[X] += LarguraVideo / 2.0 * Escala[X]; 
    Deltas[Y] += AlturaVideo / 2.0 * Escala[Y];

    RedrawWindow();

    //--- Desenha a cruz vermelha no ponto

    CPoint PPixels;

    PPixels.x = (int) (Deltas[X] + (EstacaDig.EstVirtual * Escala[X]));
    PPixels.y = (int) (Deltas[Y] + (Resolucao[Y] - (CotaDig) * Escala[Y]));

    CDC* pDC = GetDC();
    CPen PenaMalha,*pPenaAnterior; 										
    PenaMalha.CreatePen(PS_SOLID,2,RGB(255,0,0));
    pPenaAnterior = pDC->SelectObject(&PenaMalha);		

    //--- Desenha a cruz, 14 pixels de largura

    pDC->MoveTo(PPixels.x-15,PPixels.y);
    pDC->LineTo(PPixels.x+15,PPixels.y);		
    pDC->MoveTo(PPixels.x,PPixels.y-15);
    pDC->LineTo(PPixels.x,PPixels.y+15);

    pDC->Arc(PPixels.x-5,PPixels.y-5,PPixels.x+5,PPixels.y+5,0,0,0,0);    //--- Circula o ponto.

    pDC->SelectObject(pPenaAnterior); 

    ReleaseDC(pDC); 
  }
}

const void* DesPerfilGeoView::VerfPonto(double x,double y,int& TipoPonto)
{
  //--- Verifica Se o mouse esta sobre algum ponto de terreno ou de algum furo de sondagem 

  return PosicionaMaisProximo(x,y,Toleranciax,Toleranciay,TipoPonto);
}

std::string DesPerfilGeoView::PreparaDescricao(const void* pPonMouse,int Tipo,const std::string* strTipoSolo) 
{
  if(pPonMouse == NULL) return "";

  std::strstream strstreamTemp;

  strstreamTemp << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

  switch(Tipo)
  {
  case PONTOTERRENO:
    {
      strstreamTemp << " Terreno:"; 

      CEstaca* pEstaca((CEstaca*) pPonMouse);

      if (pEstaca->EstVirtual != INFINITO) strstreamTemp << "\r\n Estaca: " << *pEstaca;
      if (pEstaca->Cota  != INFINITO) strstreamTemp <<  "\r\n Cota= " << pEstaca->Cota;  

    }
    break;
  case PONTOFUROSONDAGEM:
    {
      ittysetFurosSondagem itFuroSondagem(*(ittysetFurosSondagem*)pPonMouse);

      strstreamTemp << " Furo De Sondagem:\r\n Estaca:";
      strstreamTemp << itFuroSondagem->PegaEstaca();

      if(itFuroSondagem->PegaEstaca().Cota != INFINITO) 
      {
        strstreamTemp << "\r\n Cota= " << itFuroSondagem->PegaEstaca().Cota;
        strstreamTemp << "\r\n Profundidade Total= " << itFuroSondagem->PegaEstaca().Cota -itFuroSondagem->PegaCotaFundo();
      }
    }
    break;
  case PONTOTIPOSOLO:
    {
      ittysetCTipoSoloSondagem  itTipoSolo(*(ittysetCTipoSoloSondagem*)pPonMouse);
      strstreamTemp << " Solo: Tipo " <<  itTipoSolo->PegaTipoSolo();
      strstreamTemp << "\r\n Profundidade= " << itTipoSolo->PegaProfundidade();
      strstreamTemp << "\r\n Cota= " << itTipoSolo->PegaCota();
    }
    break;
  case PONTOPERFILSONDAGEM:
    {
      typairittyPerfilTSrsetPTS* pairPontoSet((typairittyPerfilTSrsetPTS*)pPonMouse);

      ittyPerfilTipoSolo* itPerfilTipoSolo(((typairittyPerfilTSrsetPTS*)pPonMouse)->first);
      strstreamTemp << "Perfil Solo: " 
        << "\r\n Tipo = " << (*itPerfilTipoSolo)->Descricao
        << "\r\n Estaca= " << (pEstSendoMovida != NULL ? *pEstSendoMovida : (CEstaca)(**itPerfilTipoSolo))
        << "\r\n Cota = " << (pEstSendoMovida != NULL ? pEstSendoMovida->Cota : (*itPerfilTipoSolo)->Cota); 
    }
    break;

  default:  strstreamTemp << "Ponto Desconhecido"; 
  }

  strstreamTemp << std::ends;

  std::string Ret(strstreamTemp.str());
  strstreamTemp.freeze(false);

  return Ret;
}

void DesPerfilGeoView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  if (Arrastando) 
  {
    TerminarArrasto = true;
    PonMousePixelsAnterior = PonMouseClicado;
    PonMousePixelsAtual = point;

    RedrawWindow();

    return;
  }

  RedrawWindow();

  //--- O primeiro click em cima de um ponto inicia o arrasto, no segundo click
  //--- finaliza o arrasto, no primeiro click arrastandoponto fica true até que o usuário click novamente
  //--- quando Terminararrasto fica true, o arrasto termina efetivamente quando o usuário tira o dedo do 
  //--- botão do mouse no métod onlbuttonup. 

  double x,y;    
  TransfPixelsXY(point,x,y);  //--- Pega as coordenadas (em metros) do ponto local o mouse está.

  //--- Se o usuário clicou sobre um ponto do projeto,inicia o arrasto do ponto. Esta operação será terminada quando o usuário clicar novamente 
  //--- Caso ele não tenha clicado em um ponto de projeto, clicou no fundo, do mesmo modo inicia o arrasto.

  if (pPontoMouse != NULL && (!TrechoPerfilGeolCopiado && !TrechoTerrenoCopiado))   //--- O usuário clicou em um ponto e não esta copiando perfil
  {
    if(Tipo == PONTOTIPOSOLO && !(nFlags & MK_CONTROL))   //--- é um ponto no furo de sondagem?
    {
      //---   Clicou em um tipo de solo em um furo de sondagem
      //---   Se o perfil já existir:
      //---   Se estiver vindo do ultimo ponto de um perfil, e os perfís forem do mesmo tipo funde os perfís
      //---   Se não houver perfil passando por este ponto, inclui o ponto no perfil atual

      typairittyPerfilTSrsetPTS Resposta(PerfilGeologico.VerfPontoPerfilDesenho(CEstaca(x,y),Toleranciax,&itPerfilAtual,Escala[Y]));  //--- Já existe um perfil neste ponto deste furo de sendagem?

      if(Resposta.first != NULL) //--- Se não for ==  NULL a resposta é sim, existe um perfil neste ponto deste furo de sondagem
      {
        ittysetCPerfilTipoSolo pPerfilSolo(Resposta.second);   

        if(pPerfilSolo != PerfilGeologico.PegaPerfisSolos().end() && pPerfilSolo->PegaPerfil().size())   //--- Neura, não pode ser NULL pois está em um ponto de um perfil....mas o size é necessário....
        {
          ittyPerfilTipoSolo* it((ittyPerfilTipoSolo*)Resposta.first);

          if(*it == pPerfilSolo->PegaPerfil().begin())                             //--- Está no ultimo ponto de um perfil e clicou no início de outro...
          {
            if(!(nFlags & MK_SHIFT) && !MarcandoTrecho)
            { 
              PerfilGeologico.FundePerfis(itPerfilAtual,pPerfilSolo); 
              InserindoPonto = false;
              pPontoMouse = pPosAnterior = NULL;
              itPerfilAtual = PerfilGeologico.PegaPerfisSolos().end();
            }
          }
          else
          {
            if(**it == *pPerfilSolo->PegaPerfil().rbegin())                        //--- Esta num furo de sondagem mas esta no ultimo ponto do perfil, continua o perfil 
            {
              PonMousePixelsAnterior = PonMouseClicado = point; 
              itPerfilAtual = pPerfilSolo;
              EstSendoMovidaAnterior = **it;
              InserindoPonto = AlterouPerfil = true;
            }
            else AfxMessageBox("Já existe um perfil geológico passando por este ponto",MB_ICONASTERISK);
          }
        }
        else AfxMessageBox("Erro 2");
      }
      else
      {
        //--- Não há perfil com este ponto, inclui o ponto no perfil ou começa um novo perfil

        ittysetCTipoSoloSondagem itTipoSoloMouse(*(ittysetCTipoSoloSondagem*)pPontoMouse);
        ittysetFurosSondagem itSFS(PerfilGeologico.PegaFuroMaisProximo(x,y,Toleranciax*1.0,Toleranciay*1.0));

        //--- Pega de uma vez o tipo e a cor do traço

        int TipoTraco(0);
        COLORREF CorTraco(255);

        CTipoSolo TipoSolo(itTipoSoloMouse->PegaTipoSolo());     
        itsetCTipoSolo itTS(SetTiposSolos.find(TipoSolo));

        if(itTS != SetTiposSolos.end())
        {
          TipoTraco = itTS->Traco;
          CorTraco = itTS->Cor;
        }

        if(InserindoPonto == false)
        {
          CEstaca PontoInserir(itSFS->PegaEstaca().EstVirtual,itTipoSoloMouse->PegaCota(),0,itTipoSoloMouse->PegaTipoSolo().c_str());
          TIPPONSETPERFIL LocalPontoPerfil(VerfPontoPertencePerfil(PontoInserir));

          if(LocalPontoPerfil == NAO_PERTENCE/* || LocalPontoPerfil == PRIMEIROPONTO || LocalPontoPerfil == ULTIMOPONTO*/)
          {
            itPerfilAtual = (PerfilGeologico.IncluiPerfilSolo(itTipoSoloMouse->PegaTipoSolo(),PontoInserir,TipoTraco,CorTraco)).first;

            if(itSFS != PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem().end()) 
            {
              EstSendoMovidaAnterior = PontoInserir;
              InserindoPonto = AlterouPerfil = true;
              point.x = PonMouseClicado.x = (int) (Deltas[X] + (itSFS->PegaEstaca().EstVirtual*Escala[X]));               ///--- Acerta a tolerancia do ponto, nos furos de sondagem é grande
              point.y = PonMouseClicado.y = (int) (Deltas[Y] + (Resolucao[Y] - (itTipoSoloMouse->PegaCota()) * Escala[Y]));

              PonMouseClicado = point; 

              RedrawWindow();
            }
            else AfxMessageBox("erro");
          }
          else AfxMessageBox("Já existe um perfil passando por este ponto",MB_ICONASTERISK);
        }
        else    //--- Esta inserindo ponto e clicou num furo de sondagem
        {
          if(itPerfilAtual != PerfilGeologico.PegaPerfisSolos().end())
          {
            if(itPerfilAtual->PegaTipoSolo().compare("Desconhecido") != 0)
            {
              if(itTipoSoloMouse->PegaTipoSolo().compare(itPerfilAtual->PegaTipoSolo()) == 0)   //--- é do mesmo tipo de solo?
              {
                int ErroPerfil(CortouFuroSondagem(x-Toleranciax));   //--- Verifica se esta pulando algum furo de sondagem

                if(ErroPerfil == NENHUM_ERRO || ErroPerfil == ERRO_PASSOUULTIMOFURO)       //--- Não está pulando algum furo de sondagem?, agora pode passar do ultimo furo
                {
                  EstSendoMovidaAnterior = CEstaca(itSFS->PegaEstaca().EstVirtual,itTipoSoloMouse->PegaCota(),0,itTipoSoloMouse->PegaTipoSolo().c_str());  //--- Por enquanto ela ainda é a atual

                  (const_cast<CPerfilTipoSolo*>( &*itPerfilAtual))->IncluiPonto(EstSendoMovidaAnterior);    //--- Inclui o ponto no perfil, agora ela passa a ser a estaca anterior

                  if(itSFS->PegaEstaca().EstVirtual == PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem().rbegin()->PegaEstaca().EstVirtual)     //--- Se for o último furo, finaliza o perfil
                  {
                    //---  InserindoPonto = false;
                    //---  pPontoMouse = pPosAnterior = NULL;
                    //---  itPerfilAtual = PerfilGeologico.PegaPerfisSolos().end();
                  }

                  point.x = PonMouseClicado.x = (int) (Deltas[X] + (itSFS->PegaEstaca().EstVirtual*Escala[X]));               ///--- Acerta a tolerancia do ponto, nos furos de sondagem é grande
                  point.y = PonMouseClicado.y = (int) (Deltas[Y] + (Resolucao[Y] - (itTipoSoloMouse->PegaCota()) * Escala[Y]));

                  RedrawWindow();
                }
                else    //--- Houve erro o ponto clicado pulou um (ou mais) furos de sondagem
                {
                  if(ErroPerfil == ERRO_CORTOUFURO) AfxMessageBox("O perfil geológico não pode cortar furos de sondagem sem passar por um ponto dele.",MB_ICONEXCLAMATION);
                  else if(ErroPerfil == ERRO_PASSOUULTIMOFURO) AfxMessageBox("Este erro não pode ocorrer aqui. o perfil geológico só pode ser definido entre furos de sondagem"); 
                  else AfxMessageBox("Perfil geológico Erro desconhecido 1"); 
                }
              }
              else    //--- O tipo de solo não é o mesmo.
              {
                AfxMessageBox("Este ponto não pode ser ligado, o perfil geológico no furo de sondagem é diferente.",MB_ICONEXCLAMATION);
                ApagaElastico(OBJETO_LINHA);

                return;
              }
            }
            else
            {
              PerfilGeologico.ColocaTipoNoPerfil(itTipoSoloMouse->PegaTipoSolo(),itPerfilAtual,TipoTraco,CorTraco);
              EstSendoMovidaAnterior = CEstaca(itSFS->PegaEstaca().EstVirtual,itTipoSoloMouse->PegaCota(),0,itTipoSoloMouse->PegaTipoSolo().c_str());    //--- ponto atual
              const_cast<CPerfilTipoSolo*>( &*itPerfilAtual)->IncluiPonto(EstSendoMovidaAnterior);    //--- Ficou anterior agora.
              PonMouseClicado = point;

              RedrawWindow();
            }
          }
        }
        PonMousePixelsAnterior = point;
      }
    }
    else    //--- else não é um ponto em um furo de sondagem
    {
      if(!ArrastandoPonto)
      {
        //--- Pontos de terreno aqui não interessam. Verifica o ponto no perfil geológico (o perfil com o h = 0.0);  

        if(Tipo == PONTOTERRENO || Tipo == PONTOPERFILSONDAGEM)
        {
          //--- Se estiver inserindo pontos é preciso verificar com antecedencia se esta clicando em um primeiro ponto de um outro perfil
          //--- Se estiver pega este ponto no perfil geológico (para poder ligar os perfís) senão volta o ponto atual para o ponto de terreno.(Os pontos de terreno são os mesmos do perfil
          //--- geológico com h = 0.0) so que os perfil dos pontos de terreno não é mostrado 
          //--- Se não estiver inserindo pontos verifica se a tecla SHIFT foi pressionada, caso positivo pega o ponto no perfil geológico para iniciar um novo perfil
          //--- Se nada disso for verdadeiro fica no ponto de terreno mesmo para poder iniciar um novo perfil a partir deste  ponto
          //--- Obs: O perfil do terreno primitivo existe mas nao é mostrado. Ele serve apenas para quando o usuário clicar com o botão direito no perfil geológico
          //--- correspondente ao terreno primitivo ele possa ser copiado e movido para outras camadas.

          if(InserindoPonto) 
          {
            pPontoMouse = VerfPonto(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY,Tipo=PONTOPERFILSONDAGEM);               //--- Pega o ponto no perfil geológico (com h = 0.0)
            {
              typairittyPerfilTSrsetPTS RespostaMouse((typairittyPerfilTSrsetPTS)*((typairittyPerfilTSrsetPTS*)pPontoMouse));
              ittyPerfilTipoSolo* it((ittyPerfilTipoSolo*)RespostaMouse.first);

              if(**it != *((typairittyPerfilTSrsetPTS*)pPontoMouse)->second->PegaPerfil().begin())   //--- Se não é o inicio de um outro perfil termina o perfil atual neste ponto
              {    
                if(**it != *((typairittyPerfilTSrsetPTS*)pPontoMouse)->second->PegaPerfil().rbegin())
                {
                  (const_cast<CPerfilTipoSolo*>( &*itPerfilAtual))->IncluiPonto(**it);
                  InserindoPonto = false;
                  pPontoMouse = pPosAnterior = NULL;
                  itPerfilAtual = PerfilGeologico.PegaPerfisSolos().end();

                  AlterouPerfil = true;
                }
                else
                {
                  AfxMessageBox("Perfís só podem ser ligados pelo fim de um ao início de outro e devem ser do mesmo tipo.",MB_ICONEXCLAMATION);
                  return;
                }

                RedrawWindow();

                return;
              }

              //      pPontoMouse = VerfPonto(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY,Tipo=PONTOTERRENO);       //--- Se não 

              //       return;

            }
          }
          else  if((nFlags & MK_SHIFT) || InserindoPonto) pPontoMouse = VerfPonto(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY,Tipo=PONTOPERFILSONDAGEM);    //--- Se a tecla SHIFT esta pressionada prepara 
        }                                                                                                                                                   //--- para iniciar um novo perfil apontando o ponto 
        //--- Para o perfil geologico   
        if(Tipo == PONTOPERFILSONDAGEM)                   //--- Clicou em um ponto de um perfil
        {
          if(pPontoMouse != NULL)   //--- redundante se tem tipo nunca é null
          {
            //   if(!InserindoPonto || (nFlags & MK_SHIFT))    //--- Se não estiver inserindo ponto
            if(true)
            {
              itPerfilAnterior = itPerfilAtual;
              typairittyPerfilTSrsetPTS* pairPontoSet((typairittyPerfilTSrsetPTS*)pPontoMouse);
              itPerfilAtual = pairPontoSet->second;

              if(nFlags & MK_SHIFT)                     //--- Se o SHIFT estiver pressionado 
              {
                //--- Se clicou no primeiro ou no ultimo ponto com o SHIFT pressionado continua o perfil

                if(**pairPontoSet->first == *pairPontoSet->second->PegaPerfil().rbegin())   
                {
                  EstSendoMovidaAnterior = **pairPontoSet->first;
                  PonMousePixelsAnterior = PonMouseClicado = point; 
                  InserindoPonto = AlterouPerfil = true;
                }
                else   //--- Clicou em um ponto no meio de um perfil, inicia um perfil neste ponto
                {
                  //--- Inicia um perfil neswte ponto (de outro perfil)
                  //   CEstaca* pEstaca((CEstaca*) );

                //  if(true || itPerfilAtual == PerfilGeologico.PegaPerfisSolos().end())  //--- Começa um novo perfil no ponto de terreno atual
                  if(false)//true) //--FALSE--> perfis não podem ramificar
                  {
                    itPerfilAtual = PerfilGeologico.IncluiPerfilSolo("Desconhecido",**pairPontoSet->first,0,0).first;
                    InserindoPonto = AlterouPerfil = true;
                    PonMousePixelsAnterior = PonMouseClicado = point;  
                  }
                }
              }
              else
              {
                if((nFlags & MK_CONTROL) || MarcandoTrecho)
                {
                  if(!MarcandoTrecho)
                  {
                    if(**pairPontoSet->first != *pairPontoSet->second->PegaPerfil().rbegin())     //--- Não pode começar o trecho com o ultimo ponto 
                    {
                      itPerfilAnterior = itPerfilAtual;
                      itInicioTrecho = *pairPontoSet->first;
                      itPerfilSelecionado = pairPontoSet->second;
                      PonMousePixelsAnterior =  point; 
                      MarcandoTrecho = true;
                    }
                  }
                  else
                  {
                    if(itPerfilAtual == itPerfilAnterior)
                    {
                      if(*pairPontoSet->first != itInicioTrecho)
                      {
                        if(itPerfilSelecionado == pairPontoSet->second)
                        {
                          itFimTrecho = *pairPontoSet->first;

                          VerfIniFimTrecho();         //--- Troca os ponteiros caso o fim esteja antes do início

                          //++itFimTrecho;
                          MarcandoTrecho = false;
                          TrechoMarcado = true;

                          RedrawWindow();

                          OnRButtonDown(nFlags,point);
                        }
                      }
                    }
                    else 
                    {
                      AfxMessageBox("Este ponto pertence a outro perfil.",MB_ICONEXCLAMATION);
                      return;
                    }
                  }
                }   
                else
                {
                  if(!InserindoPonto)                        //--- Se não estiver Inserindo Ponto 
                  {
                    if(!ETerreno(**pairPontoSet->first))         //--- Pontos do terreno primitivo não podem ser movidos
                    {
                      if(true || !TrechoMarcado)                        //--- Se Estiver com trecho mrcado não pode arrastar
                      {
                        EstabeleceLimitesMovimento(pairPontoSet->first);
                        ittyPerfilTipoSolo itPontoPerfilAnterior(*((ittyPerfilTipoSolo*)pairPontoSet->first));              //--- Pega o ponto
                        ittysetCPerfilTipoSolo itPerfilSolo((ittysetCPerfilTipoSolo)pairPontoSet->second);                  //--- Pega o perfil ao qual ele pertence  

                        if(itPontoPerfilAnterior == itPerfilSolo->PegaPerfil().begin()) PegouPrimeiroPonto = true;           //--- Marca que esta pegando o primeiro ponto
                        else --itPontoPerfilAnterior;                                                                       

                        EstSendoMovidaAnterior = *itPontoPerfilAnterior;                                                    //--- No caso anterior é o posterior
                        pEstSendoMovida = const_cast<CEstaca*>(&*(*((ittyPerfilTipoSolo*)pairPontoSet->first)));            //--- Move o ponto do perfil
                        PontoOriginal = **pairPontoSet->first;
                        ArrastandoPonto = true;
                      }
                    }
                  }
                  else                                       //---senão: se for o primeiro ponto de um perfil une os perfís se foram do mesmo tipo  
                  {
                    typairittyPerfilTSrsetPTS RespostaMouse((typairittyPerfilTSrsetPTS)*((typairittyPerfilTSrsetPTS*)pPontoMouse));
                    ittyPerfilTipoSolo* it((ittyPerfilTipoSolo*)RespostaMouse.first);

                    if(**it == *itPerfilAtual->PegaPerfil().begin())                    //--- Pra ligar perfis tem que clicar no primeiro ponto
                      //  if(true)
                    {
                      if(VerificaInsercao(**it,EstSendoMovidaAnterior) == true)
                      {
                        if(!CruzouTerreno(**it))
                        {
                          //Verificar insersao

                          if(CortouFuroSondagem2(EstSendoMovidaAnterior.EstVirtual,(**it).EstVirtual) == NENHUM_ERRO)   //--- Verifica se esta pulando algum furo de sondagem
                          {
                            if(PerfilGeologico.FundePerfis(itPerfilAnterior,itPerfilAtual))  
                            {
                              InserindoPonto = false;
                              pPontoMouse = pPosAnterior = NULL;
                              itPerfilAnterior = itPerfilAtual = PerfilGeologico.PegaPerfisSolos().end();
                            }
                            else 
                            {
                               itPerfilAtual = PerfilGeologico.PegaPerfisSolos().end();
                               InserindoPonto = false;
                            }

                            RedrawWindow();
                          }
                          else
                          {
                            AfxMessageBox("O perfil geológico não pode cortar furos de sondagem",MB_ICONASTERISK);
                            return;
                          }
                        }
                        else AfxMessageBox("asdasdasdasd");
                      }
                    }
                  }
                }
              }
            }
            else    //--- Clicou em um ponto existente e esta criando um perfil, termina o perfil neste ponto
            {
              CEstaca PontoInserir(CEstaca(x,y,0,((CPerfilTipoSolo*)(&*itPerfilAtual))->PegaTipoSolo().c_str()));

              if(VerificaInsercao(PontoInserir,EstSendoMovidaAnterior) == true)
              {
                //--- Se for um ponto no terreno pode ser que de false devido aos eros de  arredondamentos, então não verifica se for ponto de terreno

                if(VerfPontonoTerreno(PontoInserir) != setEstacasTerreno.end()  || !CruzouTerreno(PontoInserir))
                {
                  (const_cast<CPerfilTipoSolo*>( &*itPerfilAtual))->IncluiPonto(PontoInserir);
                  InserindoPonto = false;
                  pPontoMouse = pPosAnterior = NULL;
                  itPerfilAnterior = itPerfilAtual = PerfilGeologico.PegaPerfisSolos().end();
                }
                else AfxMessageBox("Este ponto não pode ser inserido; perfís geológicos não podem cruzar o terreno",MB_ICONEXCLAMATION);
              }
              else AfxMessageBox("Este ponto não pode ser inserido; perfís geológicos não podem se cruzar",MB_ICONASTERISK);
            }
          }
          else
          {
            if(Tipo == PONTOTERRENO)                       //--- Se for em um ponto de terreno inicia um novo perfil neste ponto
            {
              CEstaca* pEstaca((CEstaca*) pPontoMouse);

              if(itPerfilAtual == PerfilGeologico.PegaPerfisSolos().end())  //--- Começa um novo perfil no ponto de terreno atual
              {
                itPerfilAtual = PerfilGeologico.IncluiPerfilSolo("Desconhecido",CEstaca(pEstaca->EstVirtual,pEstaca->Cota),0,0).first;
                EstSendoMovidaAnterior = *pEstaca;
                InserindoPonto = AlterouPerfil = true;

                PonMousePixelsAnterior = PonMouseClicado = point;  
              }
              else                                //---------- Finaliza o perfil no ponto de terreno                    
              {
                if(true)
                  //  if(itPerfilAtual->PegaTipoSolo().compare("DESCONHECIDO") != 0)
                {
                  const_cast<CPerfilTipoSolo*>( &*itPerfilAtual)->IncluiPonto(CEstaca(pEstaca->EstVirtual,pEstaca->Cota,0,((CPerfilTipoSolo*)(&*itPerfilAtual))->PegaTipoSolo().c_str()));
                  itPerfilAtual = PerfilGeologico.PegaPerfisSolos().end();
                  InserindoPonto = false;
                }
                else 
                {
                  AfxMessageBox("Um perfil deve passar em pelo menos em um furo de sondagem",MB_ICONASTERISK);    //--- não mais
                  ApagaElastico(OBJETO_LINHA);

                  RedrawWindow();

                  return;
                }
              }
              RedrawWindow();
            }
          }
        }
      }
      else
      {
        if(VerificaInsercao(*pEstSendoMovida,EstSendoMovidaAnterior) == true)
        {
          if(!CruzouTerreno(*pEstSendoMovida))
          {
            int ErroPerfil(NENHUM_ERRO);  
            int TipoTS(PONTOTIPOSOLO);
            double xTS(x),yTS(y);

            const void* pPontoTS(VerfPonto(xTS,yTS,TipoTS));                  //--- Finalmente verifica se esta sobre um ponto de um furo de sondagem

            if(TipoTS == PONTOTIPOSOLO)                //--- Esta movendo um ponto, se estiver em um tipo de solo em um furo de sondagem pega a estaca e cota do ponto
            {
              ittysetCTipoSoloSondagem itTipoSoloMouse(*(ittysetCTipoSoloSondagem*)pPontoTS);
              ittysetFurosSondagem itSFS(PerfilGeologico.PegaFuroMaisProximo(xTS,yTS,Toleranciax*1.0,Toleranciay*1.0));

              if(itSFS != PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem().end())
              {
                pEstSendoMovida->EstVirtual = itSFS->PegaEstaca().EstVirtual;
                pEstSendoMovida->Cota = itTipoSoloMouse->PegaCota();

                PonMouseClicado = point;                                                 //--- Para redesenhar a linha no locar certo
                RedrawWindow();
              }
            }
            else  ErroPerfil = CortouFuroSondagem2(PontoOriginal.EstVirtual,pEstSendoMovida->EstVirtual);  

            if(ErroPerfil == NENHUM_ERRO || ErroPerfil == ERRO_PASSOUULTIMOFURO)                     //--- Agora pode passar do ultimo furo 
            {
              (const_cast<CPerfilTipoSolo*>( &*itPerfilAtual))->IncluiPonto(*pEstSendoMovida);

              //--- Verifica se o ponto inserido foi o primeiro ponto do perfil, se for é preciso reinserir o perfil no set pois
              //--- o ponto inicial é chave

              if(PegouPrimeiroPonto)
              {
                pairtysetCPerfilTipoSoloItBool pairNovoPerfil(PerfilGeologico.IncluiPerfilSolo(itPerfilAtual->PegaTipoSolo(),*pEstSendoMovida,itPerfilAtual->PegaTraco(),itPerfilAtual->PegaCor()));

                for(register ittyPerfilTipoSolo itPontoPerfAtual = itPerfilAtual->PegaPerfil().begin() ; itPontoPerfAtual !=  itPerfilAtual->PegaPerfil().end() ; ++itPontoPerfAtual)
                {
                  (const_cast<CPerfilTipoSolo*>(&*pairNovoPerfil.first))->IncluiPonto(*itPontoPerfAtual);
                }

                PerfilGeologico.RemovePerfil(itPerfilAtual);      
                itPerfilAtual = PerfilGeologico.PegaPerfisSolos().end();
                PegouPrimeiroPonto = false;
              }

              pEstSendoMovida = NULL;
              ArrastandoPonto = InserindoPonto = false;
              AlterouPerfil = true;
            }
            else 
            {
              if(ErroPerfil == ERRO_CORTOUFURO) AfxMessageBox("O perfil geológico não pode cortar furos de sondagem",MB_ICONERROR);
              else if(ErroPerfil == ERRO_PASSOUULTIMOFURO) AfxMessageBox("O perfil geológico só pode ser definido entre furos de sondagem",MB_ICONASTERISK); 
              else AfxMessageBox("Perfil geológico Erro desconhecido PERGEO-001 - Contacte o suporte"); 
            }
          }
          else AfxMessageBox("Este ponto não pode ser inserido; perfís geológicos não podem cruzar o terreno",MB_ICONERROR);
        }
        else AfxMessageBox("Este ponto não pode ser inserido; perfís geológicos não podem se cruzar",MB_ICONERROR);
      }
      if(InserindoPonto || MarcandoTrecho)
      {
        if(Tipo != PONTOFUROSONDAGEM) PonMouseClicado = point;
      }
      else PonMouseClicado.x = PonMouseClicado.y = 0;
    }
  }
  else   //--- O usuário não clicou em um ponto existente
  {
    if(InserindoPonto)
    {
      if(itPerfilAtual != PerfilGeologico.PegaPerfisSolos().end())     //--- Existe um 
      {
        if(itPerfilAtual->PegaPerfil().size() > 0 && x > itPerfilAtual->PegaPerfil().rbegin()->EstVirtual)     //--- Perfil não pode retroceder
        {
          int ErroPerfil(CortouFuroSondagem(x-Toleranciax));

          if(ErroPerfil == NENHUM_ERRO || ErroPerfil == ERRO_PASSOUULTIMOFURO)       //--- Agora pode passar do ultimo furo
          {
            if(!(nFlags & MK_CONTROL) && !(nFlags & MK_SHIFT))                                                 
            {
              CEstaca PontoInserir(CEstaca(x,y,0,((CPerfilTipoSolo*)(&*itPerfilAtual))->PegaTipoSolo().c_str(),CEstaca::MAO_LIVRE));    //--- O ponto foi desenhado à mão livre

              if(VerificaInsercao(PontoInserir,EstSendoMovidaAnterior) == true)
              {
                if(!CruzouTerreno(PontoInserir))
                {
                  (const_cast<CPerfilTipoSolo*>( &*itPerfilAtual))->IncluiPonto(PontoInserir);
                  PonMouseClicado = point;  
                  EstSendoMovidaAnterior = PontoInserir;
                }
                else AfxMessageBox("Este ponto não pode ser inserido; perfís geológicos não podem cruzar o terreno",MB_ICONERROR);
              }
              else AfxMessageBox("Este ponto não pode ser inserido; perfís geológicos não podem se cruzar",MB_ICONERROR);

            }
            else                                             //--- Esta criando um perfil clicou no fundo com a tecla CTRL pressionada, arrasta o desenho
            {
              Arrastando = true;
              InicioArrast = point;
            }
          }
          else
          {
            if(ErroPerfil == ERRO_CORTOUFURO) AfxMessageBox("O perfil geológico não pode cortar furos de sondagem",MB_ICONERROR);
            else if(ErroPerfil == ERRO_PASSOUULTIMOFURO) AfxMessageBox("O perfil geológico só pode ser definido entre furos de sondagem",MB_ICONASTERISK); 

            ApagaElastico(OBJETO_LINHA);
          }
        }
        RedrawWindow();
      }
    }
    else
    {
      if(nFlags & MK_SHIFT)                //--- Inicia um perfil fora de um furo de sondagem ou de um ponto de terreno
      {
        itPerfilAtual = PerfilGeologico.IncluiPerfilSolo("Desconhecido",CEstaca(x,y,0,"",CEstaca::MAO_LIVRE),0,255).first;     //--- Indica que a estaca foi criada a mão livre
        EstSendoMovidaAnterior = *itPerfilAtual->PegaPerfil().begin();
        InserindoPonto = AlterouPerfil = true;

        PonMousePixelsAnterior = PonMouseClicado = point;   
      }
      else                                //--- Clicou no fundo, arrasta o desenho    
      {
        if(TrechoTerrenoCopiado)
        {
          Ponto PontoInicioArrasto,PontoFimArrasto;
          TransfPixelsXY(InicioArrast,PontoInicioArrasto.x,PontoInicioArrasto.y);
          TransfPixelsXY(point,PontoFimArrasto.x,PontoFimArrasto.y);

          if(fabs(PontoInicioArrasto.y-PontoFimArrasto.y) < 0.25)
          {
            AfxMessageBox("O valor do deslocamento no eixo y não pode ser menor que 0,25m",MB_ICONERROR);
            itPerfilMarcado = PerfilGeologico.PegaPerfisSolos().end();
            DesenharApagar(OBJETO_PERFIL_TERRENO,PonMousePixelsAnterior,PonMousePixelsAnterior,NULL,OPERACAO_APAGAR);     

            return;
          }

          if(PontoInicioArrasto.y-PontoFimArrasto.y > 0.0)        //--- O perfil do terreno primitico não muda, entao basta verificar o Deltah
          {
            if(itFimTrechoTerreno == setEstacasTerreno.end()) --itFimTrechoTerreno;

            PerfilGeologico.CriaPerfisNosTrechos(itInicioTrechoTerreno,itFimTrechoTerreno,Ponto(MoverApenasEixoy ? 0.0 : (PontoInicioArrasto.x-PontoFimArrasto.x),PontoInicioArrasto.y-PontoFimArrasto.y));
            itInicioTrechoTerreno = itFimTrechoTerreno = setEstacasTerreno.end();
            TrechoTerrenoCopiado = false;
            AlterouPerfil = true;

            RedrawWindow();
          }
          else
          {
            DesenharApagar(OBJETO_PERFIL_TERRENO,PonMousePixelsAnterior,PonMousePixelsAnterior,NULL,OPERACAO_DESENHAR);     
            AfxMessageBox("Este perfil não pode ser copiado para este local porque esta acima do terreno primitivo.",MB_ICONERROR);
            itPerfilMarcado = PerfilGeologico.PegaPerfisSolos().end();
            PonMousePixelsAnterior = PonMousePixelsAtual;
            DesenharApagar(OBJETO_PERFIL_TERRENO,PonMousePixelsAnterior,PonMousePixelsAtual,NULL);     

            return;
          }
        }
        else
        {
          if(TrechoPerfilGeolCopiado)
          {
            Ponto PontoInicioArrasto,PontoFimArrasto;
            TransfPixelsXY(InicioArrast,PontoInicioArrasto.x,PontoInicioArrasto.y);
            TransfPixelsXY(point,PontoFimArrasto.x,PontoFimArrasto.y);

            if(fabs(PontoInicioArrasto.y-PontoFimArrasto.y) < 0.25)
            {
              AfxMessageBox("O valor do deslocamento no eixo y não pode ser menor que 0,25m",MB_ICONASTERISK);
              DesenharApagar(OBJETO_PERFIL_GEOLOGICO,PonMousePixelsAnterior,PonMousePixelsAnterior,NULL,OPERACAO_APAGAR);     
              PonMousePixelsAnterior = PonMousePixelsAtual;

              return;
            }

            pairtysetCPerfilTipoSoloItBool Resp;

            if(TrechoMarcado)   //--- Se é apenas um trecho do perfil que está marcado cria um perfil com este trecho para compatibilizar com as rotunas abaixo
            {
              Resp = PerfilGeologico.IncluiPerfilSolo("@__Temporario__@",*itInicioTrecho,0,255);

              if(Resp.second)
              {
                for(register ittyPerfilTipoSolo itAtual = itInicioTrecho ; itAtual != itFimTrecho ; ++itAtual)
                {
                  (const_cast<CPerfilTipoSolo*> (&*Resp.first))->IncluiPonto(*itAtual);
                }
              }
              itPerfilMarcado = Resp.first; 
            }

            if(!TrechoMarcado && itFimTrecho == itPerfilMarcado->PegaPerfil().end()) --itFimTrecho;   //--- Trecho marcado nunca termina em end

            if(VerfInsercaoPerfilGeologico(itPerfilMarcado->PegaPerfil()))
            {
              if(VerPerfilAcimaTerrenoPrimitivo(itPerfilMarcado->PegaPerfil(),PontoInicioArrasto.y-PontoFimArrasto.y) == false)
              {
                PerfilGeologico.CriaPerfisGeologicosNosTrechos(itInicioTrecho,itFimTrecho,Ponto(MoverApenasEixoy ? 0.0 : (PontoInicioArrasto.x-PontoFimArrasto.x),PontoInicioArrasto.y-PontoFimArrasto.y));
                if(TrechoMarcado) PerfilGeologico.RemovePerfil(itPerfilMarcado);
                itPerfilMarcado = itPerfilSelecionado = PerfilGeologico.PegaPerfisSolos().end();
                TrechoPerfilGeolCopiado = TrechoMarcado = false;
                AlterouPerfil = true;

                RedrawWindow();
              }
              else
              {
                DesenharApagar(OBJETO_PERFIL_GEOLOGICO,PonMousePixelsAnterior,PonMousePixelsAnterior,NULL,OPERACAO_DESENHAR);     
                AfxMessageBox("Este perfil não pode ser copiado para este local porque ele (ou parte dele) está acima do terreno primitivo.",MB_ICONERROR);
                PonMousePixelsAnterior = PonMousePixelsAtual;
                DesenharApagar(OBJETO_PERFIL_GEOLOGICO,PonMousePixelsAnterior,PonMousePixelsAtual,NULL);   

                return;
              }

              if(TrechoMarcado)
              {
                if(itPerfilSelecionado == Resp.first) 
                {
                  itPerfilSelecionado = PerfilGeologico.PegaPerfisSolos().end();
                  ((CChildFrame*) GetParentFrame())->MudaCorPerfGeoSel(RGB(0,0,0));
                }
                PerfilGeologico.RemovePerfil(Resp.first);                   //--- Remove o perfil temporário
                itPerfilMarcado = PerfilGeologico.PegaPerfisSolos().end();
                TrechoMarcado = false;
                AlterouPerfil = true;
              }
            }
            else 
            {
              AfxMessageBox("Este perfil não pode ser copiado para este local porque causaria interseção entre perfís.",MB_ICONERROR);

              DesenharApagar(OBJETO_PERFIL_GEOLOGICO,PonMousePixelsAnterior,PonMousePixelsAnterior,NULL,OPERACAO_APAGAR);   
              PonMousePixelsAnterior = PonMousePixelsAtual;
            }
          }
          else
          {
            if(TrechoMarcado)   //--- clicou no fundo, se há um trecho marcado abandona-o e passa a arrastar o deenho
            {
              itPerfilMarcado = PerfilGeologico.PegaPerfisSolos().end();
              TrechoMarcado = false;
            }

            Arrastando = true;     
            InicioArrast = point;	
          }
        }
      }
    }
  }
}

void DesPerfilGeoView::OnLButtonUp(UINT nFlags, CPoint point) 
{
  //--- Se estiver arrastando um ponto e for o segundo click (Terminar arrasto = true)
  //--- finaliza o arrasto

  if (TerminarArrasto)      
  {
    if(ArrastandoPonto) 
    {
      // if(VerificaInsercaoPonto(*pairPerfilSolo->first,pairPerfilSolo->second,CEstaca(point.x,point.y)) == true)
      // encontrar o ponto que esta sendo movido no perfil
      // bool DesPerfilGeoView::VerificaInsercaoPonto(ittyPerfilTipoSolo ItPerfilSolo,ittysetCPerfilTipoSolo ItSetPerfTipoSolo,CEstaca& EstPonto)

      ittyPerfilTipoSolo ItEstSendoMovida(itPerfilSelecionado->PegaPerfil().find(*pEstSendoMovida));

      if(ItEstSendoMovida != itPerfilSelecionado->PegaPerfil().end())
      {
        if(VerificaInsercaoPonto(ItEstSendoMovida,itPerfilSelecionado,CEstaca(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY)) == false) return;

        pEstSendoMovida->EstVirtual = MouseXYGre->m_DOCoorX;
        pEstSendoMovida->Cota = MouseXYGre->m_DOCoorY;
      }
      ArrastandoPonto = false;
    }
    Arrastando = TerminarArrasto = false;

    pEstSendoMovida = NULL;
  }

  SetCursor(LoadCursor(NULL,CursorAtual));
}

void DesPerfilGeoView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  double x,y;
  bool NaTangente(false);
  ittyPerfilTipoSolo ItPerfAnterior;

  TransfPixelsXY(point,x,y);  //--- Transforma as coordenadas de pixels para metros.

  //--- Verifica se o ponto clicado esta sobre um perfil, se estiver muda o perfil atual para este parfil e
  //--- muda o estado para movendo ponto para continuar movendo este ponto
  //--- Caso contrario insere o ponto no perfil que esta selecionado e posteriormente faz a validações

  typairittyPerfilTSrsetPTS* pairPerfilSolo(VerificaTangente(Ponto(x,y),NaTangente));

  if(NaTangente) 
  {
    if(fabs(InterpolaCotaTerreno(**pairPerfilSolo->first) - (**pairPerfilSolo->first).Cota) < 5e-4)   //--- O perfil onde os pontos são do terreno primitivo não pode ser alterarado
    {
      itPerfilSelecionado = PerfilGeologico.PegaPerfisSolos().end();                                   //--- Desceleciona o perfil
      ((CChildFrame*) GetParentFrame())->MudaCorPerfGeoSel(RGB(0,0,0));

      return;
    }
    else           //--- Se não pega o perfil selecionado
    {
      itPerfilMarcado = PerfilGeologico.VerfPontoPerfil(CEstaca(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY),Toleranciax,Toleranciay);
      itPerfilSelecionado = itPerfilMarcado = pairPerfilSolo->second;
      OnPopupPerfilSelecionar();                   //--- Muda o perfil selecionado para o que foi selecionado agora
    }
  }

  if(itPerfilSelecionado != PerfilGeologico.PegaPerfisSolos().end() && (nFlags & MK_SHIFT) == false)
  {
    CEstaca NovoPonto(x,y);

    if((const_cast<CPerfilTipoSolo*>(&*itPerfilSelecionado))->IncluiPonto(NovoPonto))
    {
      ittyPerfilTipoSolo ItPerfPosterior(itPerfilSelecionado->PegaPerfil().lower_bound(NovoPonto));

      if(ItPerfPosterior != itPerfilSelecionado->PegaPerfil().begin() && ItPerfPosterior != itPerfilSelecionado->PegaPerfil().end())
      {
        ItPerfAnterior = ItPerfPosterior;
        pairPerfilSolo->first = &ItPerfAnterior;
        pairPerfilSolo->second = itPerfilSelecionado;
      }
      else 
      {
        //-- inserir no inicio ou no fim: Teria que se fazer uma serie de verificações aqui, fica pra uma outra versão

        const_cast<CPerfilTipoSolo*>(&*itPerfilSelecionado)->RemovePonto(NovoPonto);
        AfxMessageBox("Este ponto não pode ser inserido porque esta fora do trecho do perfil que está atualmente selecionado.");
	      pairPerfilSolo->first = NULL;
	      PiscarPerfil(itPerfilSelecionado);
        Arrastando = false;
        return;
      }
    }
  }

  if(pairPerfilSolo->first != NULL)
  {
    CEstaca NovoPonto(x,y);
    {
      if(NaTangente || VerificaInsercaoPonto(*pairPerfilSolo->first,pairPerfilSolo->second,NovoPonto) == true)
      {
        ittyPerfilTipoSolo It((const_cast<CPerfilTipoSolo*>(&*pairPerfilSolo->second))->PegaPerfil().find(NovoPonto));
        pEstSendoMovida = const_cast<CEstaca*>(&*It);
        itPerfilAtual = pairPerfilSolo->second;
        EstabeleceLimitesMovimento(&It);
        ArrastandoPonto = true;

        RedrawWindow();
      }
      else
      {
        if(!NaTangente)
        {
          const_cast<CPerfilTipoSolo*>(&*pairPerfilSolo->second)->RemovePonto(NovoPonto);
          Arrastando = false;
        }
      }

    }
    /*
    ittyPerfilTipoSolo ItPontoAnterior(*pairPerfilSolo->first),ItPontoPosterior(*pairPerfilSolo->first);

    if(!CruzouTerreno(CEstaca(x,y)))
    {
    --ItPontoAnterior;
    ++ItPontoPosterior;

    bool OK(true);

    if(ItPontoAnterior !=  pairPerfilSolo->second->PegaPerfil().end()) OK = VerificaInsercao(*ItPontoAnterior,*(*pairPerfilSolo->first));   
    if(OK && ItPontoPosterior !=  pairPerfilSolo->second->PegaPerfil().end()) OK = VerificaInsercao(*(*pairPerfilSolo->first),*ItPontoPosterior);   

    if(OK)
    {
    pEstSendoMovida = const_cast<CEstaca*>(&*(*((ittyPerfilTipoSolo*)pairPerfilSolo->first)));
    itPerfilAtual = pairPerfilSolo->second;
    EstabeleceLimitesMovimento(pairPerfilSolo->first);
    ArrastandoPonto = true;
    RedrawWindow();
    }
    else AfxMessageBox("Este ponto não pode ser inserido; perfís geológicos não podem se cruzar");
    }
    else AfxMessageBox("Este ponto não pode ser inserido; perfís geológicos não podem cruzar o terreno");
    */
  }
}

BOOL DesPerfilGeoView::PreCreateWindow(CREATESTRUCT& cs) 
{
  // cs.style |= CS_OWNDC;
  return CView::PreCreateWindow(cs);
}

void DesPerfilGeoView::OnRButtonUp(UINT nFlags, CPoint point) 
{
  if (Arrastando) return;

  //--- Se estiver em um zoom executa-o

  if (nFlags & MK_CONTROL && EZoom)
  {
    FimZoom = point;
    EZoom = false;

    //--- Zoom mínimo 4 pixels em qualquer sentido

    if (abs(InicioZoom.x - FimZoom.x) > 4 && abs(InicioZoom.y - FimZoom.y) > 4) Zoom();
    return;
  }

  static BOOL EstouAqui(FALSE);

  if (EstouAqui) return;   //--- Se já estiver entrando com um ponto ponto, volta.

  if (!(nFlags & MK_CONTROL)) return;

  EstouAqui = FALSE;	
}

void DesPerfilGeoView::OnGravarperfil() 
{
  GravarProjeto(1,true,true); 
}

void DesPerfilGeoView::OnUpdateGravarperfil(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(AlterouPerfil);	
}

BOOL DesPerfilGeoView::OnEraseBkgnd(CDC* pDC) 
{
  return FALSE;                                             
}

void DesPerfilGeoView::OnConfDesGeologico() 
{
  CRect Rect;

  GetClientRect(Rect) ;
  ClientToScreen(Rect);

  int Dados[6];

  Dados[0] = DesenharMalha;
  Dados[1] = Cotar;
  Dados[2] = EquiMalhaX;
  Dados[3] = EquiMalhaY;
  Dados[4] = EquiCotas;
  Dados[5] = CircularPontos;

  //---Mostra o dialogo, passa os ponteiros para poder alterar os toolbars  in line;

  CDConfGeologico ConfGeologico(this,Rect,&Cores[0],&Dados[0]);

  ConfGeologico.DoModal();

  DesenharMalha = Dados[0];
  Cotar = Dados[1];
  EquiMalhaX =  Dados[2];
  EquiMalhaY = Dados[3];
  EquiCotas = Dados[4];
  CircularPontos = Dados[5];

  RedrawWindow();
}

void DesPerfilGeoView::OnIrparaponto() 
{
  VisualizarPonto();
}      

void DesPerfilGeoView::DefineParametros() 
{
  //--- Calcula a escala padrão que permite a visualização todo o desenho

  EscalaDefault[X] =  Resolucao[X] / (LimiteDir - LimiteEsq);
  EscalaDefault[Y] =  Resolucao[Y] / (LimiteSup - LimiteInf);
  EscalaDefault[X] < 	EscalaDefault[Y] ? 	EscalaDefault[Y] = 	EscalaDefault[X] : 	EscalaDefault[X] = 	EscalaDefault[Y];

  if(EscalaDefault[X] < 0.1) EscalaDefault[X] = 0.1;

  EscalaDefault[Y] = EscalaDefault[X] * 10;

  RazaoHV = 0.1;
}
void DesPerfilGeoView::Zoom()    
{
  Ponto CantoInfEsq(InicioZoom.x < FimZoom.x ? InicioZoom.x : FimZoom.x,InicioZoom.y < FimZoom.y ? InicioZoom.y : FimZoom.y),
    CantoSupDir(InicioZoom.x > FimZoom.x ? InicioZoom.x : FimZoom.x,InicioZoom.y > FimZoom.y ? InicioZoom.y : FimZoom.y),
    PonGreide(CantoInfEsq.x / Escala[X] - Deltas[X] / Escala[X] ,(Resolucao[Y] - CantoInfEsq.y) / Escala[Y] + Deltas[Y] / Escala[Y]);

  double Escx = Escala[X],Escy = Escala[Y];

  //--- Calcula as escala nos 2 sentidos e separa a menor pois será a 
  //--- usada para que toda a janela caiba no video

  Escala[X] =  Resolucao[X] / (fabs((double) FimZoom.x - InicioZoom.x) / Escx);
  Escala[Y] =  Resolucao[Y] / (fabs((double) FimZoom.y - InicioZoom.y) / Escy);

  Escala[X] < (Escala[Y] * RazaoHV) ?	Escala[Y] = Escala[X] / RazaoHV: Escala[X] = Escala[Y] * RazaoHV;

  //--- Reajusta a escala no deslizante

  ((CChildFrame*) GetParentFrame())->DeslizantesGeol.MudarEscala((int) (Escala[X] * 100));

  CalculaParametros();     //--- Recalcula a altura e a largura do vídeo

  //--- Ajusta os deltas e a escala para caber todo o zoom
  //--- O ponto inferior esquerdo da janela do zoom será o ponto inferior esquerdo da janela do video 

  Deltas[X] = -PonGreide.x * Escala[X];
  Deltas[Y] = PonGreide.y * Escala[Y] - AlturaVideo * Escala[Y]  ;

  RedrawWindow();          //--- redesenha a janela, mostrando o zoom  
}

void DesPerfilGeoView::OnRButtonDown(UINT nFlags, CPoint point) 
{
  if ((nFlags & MK_CONTROL) && !EZoom)
  {
    EZoom = true;
    InicioZoom = FimZoom = point;
    return;
  }

  InserindoPonto = false;
  pPontoMouse = pPosAnterior = NULL;

  static bool MostrandoTerrenoMarcado(false);

  CMenu menu;

  if(TrechoMarcado)          //---- Mostra o menu para um trecho qualquer
  {
    if(menu.LoadMenu(IDR_MENU3))
    {
      CMenu* pPopup(menu.GetSubMenu(0));

      if(pPopup != NULL)
      {
        CPoint PAux(point);
        ClientToScreen(&PAux);
        InicioArrast = point;

        pPopup->TrackPopupMenu(TPM_TOPALIGN |TPM_LEFTALIGN , PAux.x+5, PAux.y+5, AfxGetMainWnd());

        return;
      }
    }
  }
  else
  {
    ittysetEstaca itTerrenoMarcado(VerfPontonoTerreno(CEstaca(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY)));
    itPerfilMarcado = PerfilGeologico.VerfPontoPerfil(CEstaca(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY),Toleranciax,Toleranciay);

    if(itTerrenoMarcado != setEstacasTerreno.end())     //--- Diferenciar entre todo o terreno e so um subtrecho
    {
      //--- Quando mostra o perfil do terreno: Pode ser que haja um perfil geológico por cima do terreno, se o usuário quiser vê-lo terá que clicar 2 x com o botão direito

      if(!MostrandoTerrenoMarcado || itPerfilMarcado == PerfilGeologico.PegaPerfisSolos().end())
      {
        itPerfilMarcado = PerfilGeologico.PegaPerfisSolos().end();     //--- Desmarca o perfil geológico (se ele estiver marcado)
        MostrandoTerrenoMarcado = true;
        itInicioTrechoTerreno = setEstacasTerreno.begin();
        itFimTrechoTerreno = setEstacasTerreno.end();

        RedrawWindow();
        InicioArrast = point;

        if(menu.LoadMenu(IDR_POPUPTERRENO))
        {
          CMenu* pPopup(menu.GetSubMenu(0));
          if(pPopup != NULL)
          {
            CPoint PAux(point);
            ClientToScreen(&PAux);
            pPopup->TrackPopupMenu(TPM_TOPALIGN |TPM_LEFTALIGN , PAux.x+5, PAux.y+5, AfxGetMainWnd());

            return;
          }
        }
      }
    }

    itInicioTrechoTerreno = itFimTrechoTerreno = setEstacasTerreno.end();        //--- Desmarca o terreno (Se ele estiver marcado)

    MostrandoTerrenoMarcado = false;

    itPerfilMarcado = PerfilGeologico.VerfPontoPerfil(CEstaca(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY),Toleranciax,Toleranciay);

    if(itPerfilMarcado != PerfilGeologico.PegaPerfisSolos().end())
    {
      itInicioTrecho = itPerfilMarcado->PegaPerfil().begin();
      itFimTrecho = itPerfilMarcado->PegaPerfil().end();

      RedrawWindow();

      InicioArrast = point;

      if(menu.LoadMenu(IDR_POPUPPERFIL))
      {
        CMenu* pPopup(menu.GetSubMenu(0));
        if(pPopup != NULL)
        {
          CPoint PAux(point);
          ClientToScreen(&PAux);
          pPopup->TrackPopupMenu(TPM_TOPALIGN |TPM_LEFTALIGN , PAux.x+5, PAux.y+5, AfxGetMainWnd());

          InicioArrast = point;

          return;
        }
      }
    }
    else     //--- não existe nada marcado, clicou no fundo, verifica se o usuario quer restaurar o terreno primitivo
    {
      if(menu.LoadMenu(IDR_MENU4))
      {
        itPerfilMarcado = PerfilGeologico.PegaPerfisSolos().end();     //--- Desmarca o perfil geológico (se ele estiver marcado)
        MostrandoTerrenoMarcado = true;
        itInicioTrechoTerreno = setEstacasTerreno.begin();
        itFimTrechoTerreno = setEstacasTerreno.end();

        RedrawWindow();

        CMenu* pPopup(menu.GetSubMenu(0));

        if(pPopup != NULL)
        {
          CPoint PAux(point);
          ClientToScreen(&PAux);
          pPopup->TrackPopupMenu(TPM_TOPALIGN |TPM_LEFTALIGN , PAux.x+5, PAux.y+5, AfxGetMainWnd());

          InicioArrast = point;

          return;
        }
      }
    }
  }
}

void DesPerfilGeoView::OnEnquadrar() 
{
  //--- Passa as escalas e os deltas para o default

  Escala[X] = EscalaDefault[X];
  Escala[Y] = EscalaDefault[Y];

  RazaoHV = 0.1;

  Deltas[X] = LimiteEsq * Escala[X];
  Deltas[Y] = LimiteInf * Escala[Y];

  //--- Reajusta a escala no deslizante

  ((CChildFrame*) GetParentFrame())->DeslizantesGeol.MudarEscala(Escala[X]);

  CalculaParametros();

  RedrawWindow();
}

void DesPerfilGeoView::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);

  if(cx == 0 || cy == 0) return;

  //--- Acerta os novos parametros da janela

  RECT Coord; 
  GetClientRect(&Coord);

  Resolucao[X] = Coord.right; 
  Resolucao[Y] = Coord.bottom;

  DefineParametros();
  CalculaParametros();
}

void DesPerfilGeoView::OnAlternartitulogreide() 
{
  CChildFrame* CCFrm((CChildFrame*) GetParentFrame());

  MostrarTitulo = (CCFrm->GetStyle() & WS_CAPTION) == 0;             //--- se está sem titulo seta para mostrar o titulo

  if (!MostrarTitulo) CCFrm->ModifyStyle(WS_CAPTION,0,SWP_FRAMECHANGED);    
  else CCFrm->ModifyStyle(0,WS_CAPTION,SWP_FRAMECHANGED);   

  CCFrm->ShowControlBar(&CCFrm->m_wndStatusBar,MostrarTitulo,true);
}

void DesPerfilGeoView::OnButFechar() 
{
  ((CChildFrame*) GetParentFrame())->Fechar();
}

void DesPerfilGeoView::OnUpdateGravar(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(AlterouPerfil);	
}

void DesPerfilGeoView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if(Arrastando && !ArrastandoPonto) return;

  switch(nChar)
  {
    case  VK_DELETE:                            //--- deleta um ponto sobre o mouse
    {
      if(Tipo == PONTOTERRENO || Tipo == PONTOTIPOSOLO) pPontoMouse = VerfPonto(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY,Tipo=PONTOPERFILSONDAGEM);     //--- Pegou o ponto no furo de sendagem ou no terreno promitivo:

      //--- Verifica se existe um perfil passando por ele;  

      if(pPontoMouse && Tipo == PONTOPERFILSONDAGEM)
      {
        typairittyPerfilTSrsetPTS* pairTipoSolo((typairittyPerfilTSrsetPTS*)pPontoMouse);
        CEstaca PontoRasc((CEstaca)**pairTipoSolo->first);
        ittysetCPerfilTipoSolo itPerfilSolos(pairTipoSolo->second);
        ittyPerfilTipoSolo itAnterior(itPerfilSolos->PegaPerfil().find(PontoRasc));
        register ittyPerfilTipoSolo itPosterior(itAnterior);

        if(pairTipoSolo->first != NULL)
        {
          if(AfxMessageBox("Deseja realmente remover este ponto?",MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
          {
            --itAnterior;
            ++itPosterior;

            if(itAnterior != itPerfilSolos->PegaPerfil().end())           //--- Não esta removendo  o primeiro ponto do perfil?
            {
              if(itPosterior != itPerfilSolos->PegaPerfil().end())        //--- Não esta removendo o último ponto do perfil?
              {                                                           //--- Então é preciso verificar se a reta remanescente vai interferir em algum outro perfil 
                itPerfilAtual = pairTipoSolo->second;                    

                if(VerificaInsercao(*itAnterior,*itPosterior) == false)    //--- Verifica a inserção da reta remanescente
                {
                  AfxMessageBox("Este ponto não pode ser removido porque provocaria interseção entre perfís",MB_ICONERROR);
                  pPontoMouse = NULL;
                  return;
                }
              }

              PerfilGeologico.RemovePontoPerfil(**pairTipoSolo->first,pairTipoSolo->second);
              if(pairTipoSolo->second->PegaPerfil().size() < 2) PerfilGeologico.RemovePerfil(pairTipoSolo->second);
            }
            else  //--- Se esta removendo o primeiro ponto então é preciso reinserir o perfil no set pois o primeiro ponto é chave
            {
              if(itPerfilSolos->PegaPerfil().size() > 2)   //--- Se o perfil vai ficar com apenas 1 ponto, só remove o perfil
              {
                pairtysetCPerfilTipoSoloItBool pairNovoPerfil(PerfilGeologico.IncluiPerfilSolo(itPerfilSolos->PegaTipoSolo(),*itPosterior,itPerfilSolos->PegaTraco(),itPerfilSolos->PegaCor()));

                for( ; itPosterior !=  itPerfilSolos->PegaPerfil().end() ; ++itPosterior)
                {
                  (const_cast<CPerfilTipoSolo*>(&*pairNovoPerfil.first))->IncluiPonto(*itPosterior);
                }
              }

              PerfilGeologico.RemovePerfil(itPerfilSolos);      
            }

            pPontoMouse = NULL;
            AlterouPerfil = true;
          }
        }
      }
    }
    break;
    case VK_ESCAPE :                              
    {
      if(ArrastandoPonto) if(pEstSendoMovida) *pEstSendoMovida = PontoOriginal;
      PontoOriginal.EstVirtual = PontoOriginal.Cota = INFINITO;
      pEstSendoMovida = NULL;
      pPontoMouse = pPosAnterior = NULL;
      if(itPerfilAtual != PerfilGeologico.PegaPerfisSolos().end() && itPerfilAtual->PegaPerfil().size() < 2) PerfilGeologico.RemovePerfil(itPerfilAtual);
      itPerfilAtual = itPerfilAnterior = itPerfilMarcado = PerfilGeologico.PegaPerfisSolos().end();
      InserindoPonto = Arrastando = ArrastandoPonto = false;
      itInicioTrechoTerreno = itFimTrechoTerreno = setEstacasTerreno.end(); 
      TrechoTerrenoCopiado =  CopiarApenasTrecho = false;
      TrechoPerfilGeolCopiado = false;
      PonMousePixelsAnterior = PonMouseClicado = CPoint(0,0);
      itPerfilMarcado = PerfilGeologico.PegaPerfisSolos().end();
      MarcandoTrecho = TrechoMarcado = false;
      EZoom = false;
    }
    break;
  }

  if(GetKeyState(VK_CONTROL) < 0)
  {
    switch (nChar)
    {
    case 'M': DesenharMalha = !DesenharMalha;break; 
    case 'C': Cotar = !Cotar; break;
    case 'D': MostrarDesc = !MostrarDesc;break; 
    case 'E': VarrerTerreno = !VarrerTerreno; break;
    case 'R': CircularPontos = !CircularPontos;break;
    }
  }

  RedrawWindow();
}

void DesPerfilGeoView::OnTrancarGreide()
{
}

BOOL DesPerfilGeoView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  ((CChildFrame*) GetParentFrame())->DeslizantesGeol.MouseWheelOn(nFlags, zDelta, pt);

  return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void DesPerfilGeoView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
  pDC->SetBkColor(Cores[CORFUNDO]);
  CView::OnPrepareDC(pDC, pInfo);
}

void DesPerfilGeoView::OnButarrastar()
{
  PermitirArrastar = !PermitirArrastar;
}

void DesPerfilGeoView::OnClose()
{
  GravarProjeto(false,false,true);

  CView::OnClose();
}

void DesPerfilGeoView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  double x,y;
  TransfPixelsXY(point,x,y); 
  int Tipo(1),TipoPonto(1);
}

bool DesPerfilGeoView::LeTerreno(const CString& NomeProjeto)
{
  CFile ArqCalcSec;

  //--- .pes pontos extremos da secão

  CFileException e;
  CString NomeArquivo(NomeProjeto + CString(".pes"));

  if (ArqCalcSec.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)  
  {
    monolog.mensagem(29,NomeArquivo.GetBuffer(0));

    return false;
  }

  CArchive CArqCalcSec(&ArqCalcSec,CArchive::load);

  int QSecoes(0);
  CEstaca EstacaAtual;
  Ponto PLixo;

  setEstacasTerreno.clear();

  for(CArqCalcSec >> QSecoes ; QSecoes > 0 ; --QSecoes) 
  {
    EstacaAtual << CArqCalcSec;
    PLixo << CArqCalcSec;
    PLixo << CArqCalcSec;
    PLixo << CArqCalcSec;

    if(EstacaAtual.EstVirtual != INFINITO &&  EstacaAtual.Cota != INFINITO)
    {
      setEstacasTerreno.insert(EstacaAtual);

      if(EstacaAtual.EstVirtual < LimiteEsq) LimiteEsq = EstacaAtual.EstVirtual;
      if(EstacaAtual.EstVirtual > LimiteDir) LimiteDir = EstacaAtual.EstVirtual;
      if(EstacaAtual.Cota > LimiteSup) LimiteSup = EstacaAtual.Cota;
      if(EstacaAtual.Cota < LimiteInf) LimiteInf = EstacaAtual.Cota;
    }
  }

  return true;
}

const void* DesPerfilGeoView::PosicionaMaisProximo(double x,double y,double Toleranciax,double Toleranciay,int& TipoPonto)
{
  //--- Se veio para procurar em todos os tipo procura o mais proximo em em todos os tipos senão procura o mais próximo só no tipo especificado
  //--- Retorna em tipo o tipo do ponto encontrado 

  if(TipoPonto == TODOSTIPOS || TipoPonto == PONTOFUROSONDAGEM || TipoPonto == PONTOTIPOSOLO)
  {
    static ittysetFurosSondagem itFuroSondagem;

    itFuroSondagem = PerfilGeologico.PegaFuroMaisProximo(x,y,Toleranciax*1.0,Toleranciay*1.0);

    if(itFuroSondagem != PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem().end())
    {
      static ittysetCTipoSoloSondagem itTipoSolo;

      itTipoSolo = itFuroSondagem->PegaTipoSoloMaisProximo(y,Toleranciay);

      if(itTipoSolo != itFuroSondagem->PegaSetSolos().end())
      {
        TipoPonto = PONTOTIPOSOLO;
        return &itTipoSolo;
      }

      TipoPonto = PONTOFUROSONDAGEM;
      return &itFuroSondagem; 
    }
  }

  //  if(0 && (TipoPonto == TODOSTIPOS || TipoPonto == PONTOTERRENO)) //--- O perfil do terreno não é mais mostrado, é como se ele não existisse
  if(TipoPonto == PONTOTERRENO)
  {
    static ittysetEstaca itPosterior,itAnterior;

    itAnterior = setEstacasTerreno.lower_bound(CEstaca(x-Toleranciax,y-Toleranciay));
    itPosterior = setEstacasTerreno.upper_bound(CEstaca(x+Toleranciax,y+Toleranciay));

    if(itPosterior == setEstacasTerreno.end()) --itPosterior;
    if(itAnterior == setEstacasTerreno.end()) --itAnterior;

    if((itAnterior->EstVirtual + Toleranciax) > x && (itAnterior->EstVirtual - Toleranciax) < x)
    {
      if(((itAnterior->Cota + Toleranciay) > y && (itAnterior->Cota - Toleranciay) < y)) 
      {
        TipoPonto = PONTOTERRENO;
        return &*itAnterior;
      }
    }

    if((itPosterior->EstVirtual + Toleranciax) > x && (itPosterior->EstVirtual - Toleranciax) < x)
    {
      if(((itPosterior->Cota + Toleranciay) > y && (itPosterior->Cota - Toleranciay) < y)) 
      {
        TipoPonto = PONTOTERRENO;
        return &*itPosterior;
      }
    }
  }

  if(TipoPonto == TODOSTIPOS || TipoPonto == PONTOPERFILSONDAGEM)
  {
    static typairittyPerfilTSrsetPTS pairResp;

  //  pairResp = PerfilGeologico.VerfPontoPerfilDesenho(CEstaca(x,y),Toleranciaxy_2,NULL,Escala[Y]); 
    pairResp = PerfilGeologico.VerfPontoPerfilDesenho(CEstaca(x,y),Toleranciax,NULL,Escala[Y]); 

    if(pairResp.first != NULL)
    {
      TipoPonto = PONTOPERFILSONDAGEM;
      return  &pairResp;
    }
  }

  TipoPonto = NENHUMTIPO;
  return NULL;
}

void DesPerfilGeoView::DesenhaFuroSondagem(CMMemDC* pDC)
{
  if(!pDC) return;

  POINT P,PAux;
  CPen PenaLinhaExterna,PenaLinhaPreta,PenaLinhaBranca,PenaCirculo;

  PenaLinhaExterna.CreatePen(PS_SOLID,1,RGB(127,127,127));
  PenaLinhaPreta.CreatePen(PS_SOLID,3,RGB(30,30,30));
  PenaLinhaBranca.CreatePen(PS_SOLID,3,RGB(170,170,170));
  PenaCirculo.CreatePen(PS_SOLID,1,RGB(200,20,20));

  double Delta2e50((int) (2.50 * Escala[Y]));

  const tysetFurosSondagem SetFurosSondagem(PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem());

  for(itctysetFurosSondagem itFuroSondagem = SetFurosSondagem.begin() ; itFuroSondagem != SetFurosSondagem.end() ; ++itFuroSondagem)
  {
    P.x = (int) (Deltas[X] + (itFuroSondagem->PegaEstaca().EstVirtual*Escala[X]));
    P.y = (int) (Deltas[Y] + (Resolucao[Y] - (itFuroSondagem->PegaEstaca().Cota) * Escala[Y]));

    double ProfFuro(0.0),ProfAtual(0.0);
    long YCota;

    for(itctysetCTipoSoloSondagem itTipoSolo = itFuroSondagem->PegaSetSolos().begin()  ; itTipoSolo != itFuroSondagem->PegaSetSolos().end() ; itTipoSolo++)
    {
      ProfFuro += itTipoSolo->PegaProfundidade();
      YCota = (int) (Deltas[Y] + (Resolucao[Y] - (itFuroSondagem->PegaEstaca().Cota-ProfFuro) * Escala[Y]));
      pDC->Arc(P.x-5,YCota-5,P.x+5,YCota+5,0,0,0,0); 
    }

    PAux.x = P.x - 4;
    PAux.y = P.y;

    pDC->SelectObject(PenaLinhaExterna);
    pDC->MoveTo(PAux.x,PAux.y);
    PAux.y = (int) (Deltas[Y] + (Resolucao[Y] - (itFuroSondagem->PegaEstaca().Cota-ProfFuro) * Escala[Y]));
    pDC->LineTo(PAux.x, PAux.y);

    PAux.x = P.x + 3;
    PAux.y = P.y;

    pDC->MoveTo(PAux.x,PAux.y);
    PAux.y = (int) (Deltas[Y] + (Resolucao[Y] - (itFuroSondagem->PegaEstaca().Cota-ProfFuro) * Escala[Y]));
    pDC->LineTo(PAux.x, PAux.y);

    pDC->SelectObject(PenaLinhaPreta);

    PAux.x = P.x - 2;
    PAux.y = P.y;

    pDC->MoveTo(PAux.x,PAux.y);

    bool Lado(false);

    while(ProfAtual < ProfFuro)
    {
      pDC->SelectObject(Lado ? PenaLinhaPreta : PenaLinhaBranca);
      PAux.y += Delta2e50;
      pDC->LineTo(PAux.x,PAux.y);

      Lado = !Lado;

      pDC->SelectObject(Lado ? PenaLinhaPreta : PenaLinhaBranca);

      PAux.x += 3;
      PAux.y -= Delta2e50;
      pDC->MoveTo(PAux.x,PAux.y);
      PAux.y += Delta2e50;
      pDC->LineTo(PAux.x,PAux.y);

      PAux.x -= 3;
      pDC->MoveTo(PAux.x,PAux.y);

      ProfAtual += 2.50;
    }
  }

  pDC->SelectObject(PenaCirculo);

  for(itctysetFurosSondagem itFuroSondagem = SetFurosSondagem.begin() ; itFuroSondagem != SetFurosSondagem.end() ; ++itFuroSondagem)
  {
    P.x = (int) (Deltas[X] + (itFuroSondagem->PegaEstaca().EstVirtual*Escala[X]));
    P.y = (int) (Deltas[Y] + (Resolucao[Y] - (itFuroSondagem->PegaEstaca().Cota) * Escala[Y]));

    double ProfFuro(0.0),ProfAtual(0.0);
    long YCota;

    for(itctysetCTipoSoloSondagem itTipoSolo = itFuroSondagem->PegaSetSolos().begin()  ; itTipoSolo != itFuroSondagem->PegaSetSolos().end() ; itTipoSolo++)
    {
      ProfFuro += itTipoSolo->PegaProfundidade();
      YCota = (int) (Deltas[Y] + (Resolucao[Y] - (itFuroSondagem->PegaEstaca().Cota-ProfFuro) * Escala[Y]));
      pDC->Arc(P.x-5,YCota-5,P.x+5,YCota+5,0,0,0,0); 
    }
  }
}

void DesPerfilGeoView::CalculaCotasFurosSondagem()
{
  double ProfFuro(0.0);

  const tysetFurosSondagem& SetFurosSondagem(PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem());

  for(ittysetFurosSondagem itFuroSondagem = SetFurosSondagem.begin() ; itFuroSondagem != SetFurosSondagem.end() ; ++itFuroSondagem)
  {
    (const_cast<CFuroSondagem*>(&*itFuroSondagem))->Alteracota(InterpolaCotaTerreno(itFuroSondagem->PegaEstaca()));

    if(itFuroSondagem->PegaEstaca().Cota == INFINITO)
    {
      AfxMessageBox("Nao há terreno ne estaca " + itFuroSondagem->PegaEstaca() + ". As cotas das profundidades do furo de sondagem nesta estaca não puderam ser calculadas corretamente.");
    }

    double ProfFuro(0.0);

    for(ittysetCTipoSoloSondagem itTipoSolo = itFuroSondagem->PegaSetSolos().begin()  ; itTipoSolo != itFuroSondagem->PegaSetSolos().end() ; itTipoSolo++)
    {
      ProfFuro += itTipoSolo->PegaProfundidade();
      (const_cast<CTipoSoloSondagem*>(&*itTipoSolo))->AlteraCota(itFuroSondagem->PegaEstaca().Cota - ProfFuro);  
    }
    (const_cast<CFuroSondagem*>(&*itFuroSondagem))->AlteraCotaFundo(ProfFuro);
  }
}

double DesPerfilGeoView::InterpolaCotaTerreno(const CEstaca& Estaca)
{
  if(0 || Estaca.Cota == INFINITO)   //--- fica para a proxima versão: Se não digitou a cota calcula a cota no terreno (Tem a q acertar o arquivo de digitação)
  {
    if(setEstacasTerreno.size() < 2 || Estaca.EstVirtual < setEstacasTerreno.begin()->EstVirtual || Estaca.EstVirtual > setEstacasTerreno.rbegin()->EstVirtual) return INFINITO;

    ittysetEstaca itPosterior(setEstacasTerreno.lower_bound(Estaca));

    if(itPosterior == setEstacasTerreno.begin()) return setEstacasTerreno.begin()->Cota;

    ittysetEstaca itAnterior(itPosterior);
    --itAnterior;

    double i((itPosterior->Cota - itAnterior->Cota) / (itPosterior->EstVirtual - itAnterior->EstVirtual)),
	       Cota((Estaca.EstVirtual - itAnterior->EstVirtual) * i +  itAnterior->Cota);

//    return (Estaca.EstVirtual - itAnterior->EstVirtual) * i +  itAnterior->Cota;   Proxima versão
   return Cota;
  }
  else return Estaca.Cota;
}

void DesPerfilGeoView::DesenhaPerfisGeologicos(CMMemDC* pDC,CPen* pPena)
{
  if(!pDC || SetTiposSolos.size() == 0) return;

  for(ittysetCPerfilTipoSolo itPerfilSolo=PerfilGeologico.PegaPerfisSolos().begin() ; itPerfilSolo != PerfilGeologico.PegaPerfisSolos().end() ; ++itPerfilSolo)
  {
    CTipoSolo TipoSoloAtual(itPerfilSolo->PegaTipoSolo());
    itsetCTipoSolo itTipoSolo(SetTiposSolos.find(TipoSoloAtual));

    CPen PenaPerfil,PenaSemTipo,*pPenaAUsar(NULL);

    if(itTipoSolo == SetTiposSolos.end() || itTipoSolo->Traco < 0 || itTipoSolo->Traco > 6)  
    {
      PenaSemTipo.CreatePen(PS_DOT,1,Cores[CORTERRENO]);
      pDC->SelectObject(PenaSemTipo);
    }
    else
    {
      PenaPerfil.CreatePen(itTipoSolo->TracoGDI,(itTipoSolo->Traco == 0) ? 2 : 1,itTipoSolo->Cor);
      pDC->SelectObject(PenaPerfil);
    }

    POINT P;
    register ittyPerfilTipoSolo& itPerfilTipoSolo(itPerfilSolo->PegaPerfil().begin()); 

    if( itPerfilTipoSolo != itPerfilSolo->PegaPerfil().end())
    {
      P.x = (int) (Deltas[X] + (itPerfilTipoSolo->EstVirtual*Escala[X]));
      P.y = (int) (Deltas[Y] + (Resolucao[Y] - (itPerfilTipoSolo->Cota) * Escala[Y]));

      pDC->MoveTo(P.x,P.y);
      if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0); 

      for(++itPerfilTipoSolo ; itPerfilTipoSolo != itPerfilSolo->PegaPerfil().end() ;  ++itPerfilTipoSolo)
      {  
        P.x = (int) (Deltas[X] + (itPerfilTipoSolo->EstVirtual*Escala[X]));
        P.y = (int) (Deltas[Y] + (Resolucao[Y] - (itPerfilTipoSolo->Cota) * Escala[Y]));

        pDC->LineTo(P.x,P.y); 
        if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0); 
      }
    }
  }
}

void DesPerfilGeoView::DesenharApagar(int TipoObjeto,const CPoint& LocalAnterior,const CPoint& LocalAtual,void* Objeto,int OperacaoForcada,bool PrimeiroMovimento) 
{
  CDC* pDC(GetDC());
  if (!pDC) return;   //--- Segurança, aconteçeu nos testes

  CPen Pena;
  Pena.CreatePen(PS_SOLID,PrimeiroMovimento ? 2 : 1,RGB(255,255,255));
  CPen* pPenaAnterior = NULL; 
  CPoint PontoAtual;

  //--- Apaga e redesenha apenas o objeto 

  for(int Operacao = OPERACAO_APAGAR ; Operacao <= OPERACAO_DESENHAR ; Operacao++)
  {
    //--- Se veio para fazer apenas uma operação pula a outra.

    if ((OperacaoForcada == OPERACAO_APAGAR && Operacao != OPERACAO_APAGAR) || (OperacaoForcada == OPERACAO_DESENHAR && Operacao != OPERACAO_DESENHAR)) continue;

    if (Operacao == OPERACAO_DESENHAR) 
    {
      PontoAtual = LocalAtual;

      pDC ->SetROP2(R2_NOT);
    }
    else 
    {    
      PontoAtual = LocalAnterior;

      pDC ->SetROP2(PrimeiroMovimento ? R2_BLACK : R2_NOT); 
    }

    //--- Salva a primeira pena usada, para poder restaurar na saida

    if (!pPenaAnterior) pPenaAnterior = pDC->SelectObject(&Pena);    
    else pDC->SelectObject(&Pena);

    switch(TipoObjeto)
    {
    case OBJETO_PONTO :
      {
        pDC->SetPixel(PontoAtual.x,PontoAtual.y,Operacao);

        //--- Desenha a cruz, 14 pixels de largura

        pDC->MoveTo(PontoAtual.x-7,PontoAtual.y);
        pDC->LineTo(PontoAtual.x+7,PontoAtual.y);		
        pDC->MoveTo(PontoAtual.x,PontoAtual.y-7);
        pDC->LineTo(PontoAtual.x,PontoAtual.y+7);

        //--- circula o ponto

        pDC->Arc(PontoAtual.x-3,PontoAtual.y-3,PontoAtual.x+3,PontoAtual.y+3,0,0,0,0);   	
      }
      break;
    case OBJETO_LINHA :
      {	
        pDC->MoveTo(LocalAnterior.x,LocalAnterior.y);
        pDC->LineTo(LocalAtual.x,LocalAtual.y);			
      }
      break;
    case OBJETO_PERFIL_TERRENO:
      {
        CPoint Deltas(PontoAtual-InicioArrast); 

        if(MoverApenasEixoy) Deltas.x = 0;

        register ittysetEstaca it(itInicioTrechoTerreno);

        pDC->MoveTo(TransfCoordReaisParaVideo(*it)+Deltas);

        for(++it ; it != itFimTrechoTerreno ; ++it)
        {
          pDC->LineTo(TransfCoordReaisParaVideo(*it)+Deltas);
        }
      }
      break;
    case OBJETO_PERFIL_GEOLOGICO:
      {
        CPoint Deltas(PontoAtual-InicioArrast); 

        if(MoverApenasEixoy) Deltas.x = 0;

        register ittyPerfilTipoSolo it(itInicioTrecho);

        pDC->MoveTo(TransfCoordReaisParaVideo(*it)+Deltas);

        for(++it ; it != itFimTrecho ; ++it)
        {
          pDC->LineTo(TransfCoordReaisParaVideo(*it)+Deltas);
        }
      }
      break;
    case OBJETO_TRECHO_PERFIL:
      {
        register ittyPerfilTipoSolo it(itInicioTrecho);

        pDC->MoveTo(TransfCoordReaisParaVideo(*it));

        for(++it ; it != itFimTrecho ; ++it)
        {
          pDC->LineTo(TransfCoordReaisParaVideo(*it));
        }
      }
    } 
  }
  ReleaseDC(pDC);
}

int DesPerfilGeoView::CortouFuroSondagem(double px)
{
  tysetFurosSondagem SetFS(PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem());

  if(SetFS.size() > 0)
  {
    CFuroSondagem FSTemp(px),FSUltimoPonto(itPerfilAtual->PegaPerfil().rbegin()->EstVirtual+Toleranciax );

    ittysetFurosSondagem itAnt(SetFS.upper_bound(FSUltimoPonto)),itPost(SetFS.upper_bound(FSTemp));

    if(itAnt == SetFS.end() && itPost == SetFS.end()) return ERRO_PASSOUULTIMOFURO;
    if(itAnt == itPost) return NENHUM_ERRO;

    return ERRO_CORTOUFURO;
  }

  return ERRO_PASSOUULTIMOFURO;
}

typairittyPerfilTSrsetPTS* DesPerfilGeoView::VerificaTangente(const Ponto& PAtual,bool& NaTangente)
{
  static typairittyPerfilTSrsetPTS pairPerfilSolo;
  NaTangente = false;

  for(ittysetCPerfilTipoSolo itPerfilSolo=PerfilGeologico.PegaPerfisSolos().begin() ; itPerfilSolo != PerfilGeologico.PegaPerfisSolos().end() ; ++itPerfilSolo)
  {
    if(itPerfilSolo->PegaPerfil().size() < 2) continue;

    static ittyPerfilTipoSolo itPerfilTipoSoloAnterior,itPerfilTipoSoloPosterior;

    itPerfilTipoSoloAnterior = itPerfilSolo->PegaPerfil().begin();
    itPerfilTipoSoloPosterior = itPerfilTipoSoloAnterior; 

    ++itPerfilTipoSoloPosterior;

    do    
    {  
      if(itPerfilTipoSoloAnterior->EstVirtual < PAtual.x && itPerfilTipoSoloPosterior->EstVirtual > PAtual.x)
      {
        double CoordyPonto(Mat.InterpolaY(Ponto(itPerfilTipoSoloAnterior->EstVirtual,itPerfilTipoSoloAnterior->Cota),Ponto(itPerfilTipoSoloPosterior->EstVirtual,itPerfilTipoSoloPosterior->Cota),PAtual));

        NaTangente = (fabs(CoordyPonto - PAtual.y) < Toleranciay);

        if(NaTangente)
        {
          pairPerfilSolo.first = &itPerfilTipoSoloAnterior;
          pairPerfilSolo.second = itPerfilSolo;

          return &pairPerfilSolo;
        }
      }

      ++itPerfilTipoSoloPosterior;
      ++itPerfilTipoSoloAnterior;

    } while(itPerfilTipoSoloPosterior != itPerfilSolo->PegaPerfil().end());
  }

  pairPerfilSolo.first = NULL;
  return &pairPerfilSolo;
}

bool DesPerfilGeoView::VerificaInsercao(const CEstaca& PontoAtual,const CEstaca& PontoAnterior,bool MovPerfil)
{
  Ponto Deltas(0.0,0.0);
  double Coordx,Coordy;

  if(MovPerfil) 
  {
    TransfPixelsXY(InicioArrast,Coordx,Coordy);
    Deltas.x = MoverApenasEixoy ? 0.0 :  MouseXYGre->m_DOCoorX - Coordx;
    Deltas.y = MouseXYGre->m_DOCoorY - Coordy;
  }

  Ponto Intersecao,P1(PontoAnterior.EstVirtual+Deltas.x,PontoAnterior.Cota+Deltas.y),P2(PontoAtual.EstVirtual+Deltas.x,PontoAtual.Cota+Deltas.y);
  bool Interceptou(false);

  for(ittysetCPerfilTipoSolo itPerfilSolo = PerfilGeologico.PegaPerfisSolos().begin() ; itPerfilSolo != PerfilGeologico.PegaPerfisSolos().end() ; ++itPerfilSolo)
  {
    if(itPerfilSolo->PegaPerfil().size() < 2 || itPerfilSolo == itPerfilAtual) continue;

    ittyPerfilTipoSolo itUBAnterior(itPerfilSolo->PegaPerfil().lower_bound(CEstaca(P1.x,P1.y))),      //--- Pega o limite anterior
      itUBPosterior(itPerfilSolo->PegaPerfil().upper_bound(CEstaca(P2.x,P2.y)));     //--- Pega o limite posterior 

    if(itUBAnterior == itPerfilSolo->PegaPerfil().end()) continue;                     //--- O anterior está depois do fim 
    if(itUBPosterior == itPerfilSolo->PegaPerfil().begin()) continue;                  //--- O posterior está antes do começo
    if(itUBPosterior == itPerfilSolo->PegaPerfil().end()) --itUBPosterior;             //--- o Posterior passou do fim ( mas o anterior não), pega o último ponto
    if(*itUBAnterior != PontoAnterior) --itUBAnterior;                                 //--- lower_bound sempre tem que retroceder 1 exeto que é o primeiro ponto da lista
    if(itUBAnterior == itPerfilSolo->PegaPerfil().end()) continue;                     //--- 

    ittyPerfilTipoSolo itAnterior(itUBAnterior),itAtual(itUBAnterior);
    ++itAtual;

    BOOL Interceptou(false);
    Ponto Intersecao;

    while(itAtual != itPerfilSolo->PegaPerfil().end() && !Interceptou)
    {
      Interceptou = CLPontos::CalculaIntersecao(Ponto(itAnterior->EstVirtual,itAnterior->Cota),Ponto(itAtual->EstVirtual,itAtual->Cota),P1,P2,Intersecao);

      if(Interceptou && (Intersecao == P1 || Intersecao == P2)) Interceptou = false;

      ++itAtual;
      ++itAnterior;
    }
    if(Interceptou) return false;     //--- Não pode inserir, os perfís se interceptam
  }

  return true;
}

bool DesPerfilGeoView::CruzouTerreno(const CEstaca& PontoInserir)
{
  if(setEstacasTerreno.size() < 2 || setEstacasTerreno.find(PontoInserir) != setEstacasTerreno.end()) return false; 

  ittysetEstaca itUB(setEstacasTerreno.upper_bound(PontoInserir)),itLB(setEstacasTerreno.lower_bound(PontoInserir));

  if(itUB == setEstacasTerreno.end() || itUB == setEstacasTerreno.begin()) return false;

  --itLB;

  double Cota(Mat.InterpolaY(Ponto(itLB->EstVirtual,itLB->Cota),Ponto(itUB->EstVirtual,itUB->Cota),Ponto(PontoInserir.EstVirtual)));

  return Cota < PontoInserir.Cota;
}

int DesPerfilGeoView::CortouFuroSondagem2(double xOriginal,double xAtual)
{
  tysetFurosSondagem SetFS(PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem());

  if(SetFS.size() > 0)
  {
    CFuroSondagem FSAnterior(xOriginal),FSAtual(xAtual);

    ittysetFurosSondagem itUBFSPontoAnterior(SetFS.upper_bound(FSAnterior)),itUBFSPontoAtual(SetFS.upper_bound(FSAtual));

    //    if(itUBFSPontoAtual == SetFS.end())  return ERRO_PASSOUULTIMOFURO;

    if(itUBFSPontoAnterior != itUBFSPontoAtual)  return ERRO_CORTOUFURO;
  }

  return NENHUM_ERRO;
}

void DesPerfilGeoView::OnPopupRemoverTodoPerfil()
{
  const setCDadosPerfGeologicos setDadosPerfGeol(PerfilGeologico.PegaSetSecaoPerfGeo());
  const tyPerfilTipoSolo& PerfTipoSolo(itPerfilMarcado->PegaPerfil());
  CDadosPerfisGeologicos DadosPerfTemp(itPerfilMarcado->PegaTipoSolo(),itPerfilMarcado->PegaPontoInicial().EstVirtual,NULL,NULL,NULL,NULL,1.0);
  
  itsetCDadosPerfGeologicos itSetDadosPerfGeol(setDadosPerfGeol.lower_bound(DadosPerfTemp));
  
  //--- Verifica se não é o terreno primitivo

//  if(itSetDadosPerfGeol == setDadosPerfGeol.end() || 
  //   (itSetDadosPerfGeol->PegaNomeTipoSolo() == itPerfilMarcado->PegaTipoSolo() && 
   //   itSetDadosPerfGeol->PegaEstaca() == itPerfilMarcado->PegaPontoInicial().EstVirtual &&
  //    itSetDadosPerfGeol->PegaDeltahPrimitivo() < -0.1)     

 // if(!ETerreno(itPerfilMarcado->PegaPontoInicial()))
  if(true)
  {
    if(AfxMessageBox("Confirma a remoção de todo este perfil?",MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
    {
      if(itPerfilSelecionado == itPerfilMarcado)
      {
        itPerfilSelecionado = PerfilGeologico.PegaPerfisSolos().end();   
        ((CChildFrame*) GetParentFrame())->MudaCorPerfGeoSel(RGB(0,0,0));
      }

      PerfilGeologico.RemovePerfil(itPerfilMarcado);
      itPerfilMarcado = itPerfilAtual = itPerfilSelecionado = PerfilGeologico.PegaPerfisSolos().end(); 

      AlterouPerfil = true;

      RedrawWindow();
    }
  }
  else if(itSetDadosPerfGeol->PegaDeltahPrimitivo() > -0.1) AfxMessageBox("O perfil do terreno não pode ser removido.",MB_ICONASTERISK);
}


void DesPerfilGeoView::OnPopupRemoverTrechoPerfil()
{
  ittysetCPerfilTipoSolo itPerfilRasc(itPerfilMarcado);

  if(itPerfilMarcado == PerfilGeologico.PegaPerfisSolos().end()) return;

  CEstaca Ponto(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY);

  PegaTrechoPerfGeol(Ponto,itInicioTrecho,itFimTrecho);

  if(itInicioTrecho != itPerfilMarcado->PegaPerfil().end()) 
  {
    itPerfilMarcado = PerfilGeologico.PegaPerfisSolos().end();   //--- so para mostrar so o trecho correto na tela,
    TrechoMarcado = true;
    RedrawWindow();

    if(AfxMessageBox("Confirma a remoção deste trecho do perfil?",MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
    {
      itPerfilMarcado = itPerfilRasc;     

      pairtysetCPerfilTipoSoloItBool pairResp(PerfilGeologico.IncluiPerfilSolo(itPerfilMarcado->PegaTipoSolo(),CEstaca(itFimTrecho->EstVirtual,
        itFimTrecho->Cota),itPerfilMarcado->PegaTraco(),itPerfilMarcado->PegaCor()));

      for(ittyPerfilTipoSolo it = itFimTrecho ; it != itPerfilMarcado->PegaPerfil().end() ; it++)
      {
        (const_cast<CPerfilTipoSolo*>( &*pairResp.first))->IncluiPonto(*it);
      }

      const_cast<CPerfilTipoSolo*>(&*itPerfilMarcado)->RemoveTrecho(itInicioTrecho,itPerfilMarcado->PegaPerfil().end());

      //--- Se algum dos perfís ficou com menos que 2 pontos, remove o perfil

      if(itPerfilMarcado->PegaPerfil().size() < 2) PerfilGeologico.RemovePerfil(itPerfilMarcado);
      if(pairResp.first->PegaPerfil().size() < 2) PerfilGeologico.RemovePerfil(pairResp.first);

      itPerfilMarcado = itPerfilSelecionado = itPerfilAtual = PerfilGeologico.PegaPerfisSolos().end();

      TrechoMarcado = false;
      AlterouPerfil = true;

      RedrawWindow();
    }
  }
  TrechoMarcado = false;
}

void DesPerfilGeoView::OnPopupPerfilSair()
{
  itPerfilMarcado = PerfilGeologico.PegaPerfisSolos().end();
  TrechoMarcado = false;

  RedrawWindow();
}

ittysetEstaca DesPerfilGeoView::VerfPontonoTerreno(const CEstaca& Ponto)
{
  register ittysetEstaca itAnterior(setEstacasTerreno.begin()),itPosterior(itAnterior);

  while(++itPosterior != setEstacasTerreno.end())
  {
    if((itAnterior->EstVirtual - Toleranciax) < Ponto.EstVirtual && (itPosterior->EstVirtual + Toleranciax) > Ponto.EstVirtual)
    {
      double Cota((((itPosterior->Cota - itAnterior->Cota) / (itPosterior->EstVirtual- itAnterior->EstVirtual)) * 
        (Ponto.EstVirtual - itAnterior->EstVirtual)) + itAnterior->Cota);

      if(fabs(Cota - Ponto.Cota) < Toleranciay) return itAnterior;
    }
    ++itAnterior;
  }

  return setEstacasTerreno.end();
}

void DesPerfilGeoView::OnTerrenoCopiartodooperfil()
{
  CopiarApenasTrecho = false;
  TrechoTerrenoCopiado = true;
  PonMousePixelsAtual = PonMousePixelsAnterior = PonMouseClicado = InicioArrast;
}

void DesPerfilGeoView::OnTerrenoCopiarapenasestetrecho()
{
  PegaTrechoTerreno(CEstaca(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY),itInicioTrechoTerreno,itFimTrechoTerreno);
  CopiarApenasTrecho  = TrechoTerrenoCopiado = true;
  PonMousePixelsAtual = PonMousePixelsAnterior = PonMouseClicado = InicioArrast;
}

void DesPerfilGeoView::OnTerrenoSair()
{
  itPerfilMarcado = PerfilGeologico.PegaPerfisSolos().end();
  TrechoTerrenoCopiado =  CopiarApenasTrecho = false;
  itInicioTrechoTerreno = setEstacasTerreno.end();

  RedrawWindow();
}

void DesPerfilGeoView::PegaTrechoTerreno(const CEstaca& Ponto,ittysetEstaca& itSELB,ittysetEstaca& itSEUB)
{
  pairERSFS PairERFS(PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem().equal_range(Ponto));        //--- Pega os 2 furos de sondagem, inicio e fim do trecho

  if(PairERFS.first != PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem().end())                    //--- Ajusta o início
  {
    if(PairERFS.first->PegaEstaca().EstVirtual > Ponto.EstVirtual) --PairERFS.first;
  }
  else --PairERFS.first;

  if(PairERFS.first !=  PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem().end())                   //--- Se true é o primeiro trecho antes do primeiro furo de sondagem
  {
    typairitittysetEstaca PairBoundsTerreno(setEstacasTerreno.equal_range(PairERFS.first->PegaEstaca()));   //--- Pega o limite anterior no terreno (= ao furo de sondagem inicial) 
    if(PairBoundsTerreno.first != setEstacasTerreno.end()) --PairBoundsTerreno.first;                       //--- Ajusta o início
    itSELB = PairBoundsTerreno.first;                                                                       //--- Inicio do trecho
  }
  else itSELB = setEstacasTerreno.begin();

  if(PairERFS.second !=  PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem().end())                  //--- Se true é o último trecho, depois do ultimo furo de sondagem0       
  {
    typairitittysetEstaca PairBoundsTerreno(setEstacasTerreno.equal_range(PairERFS.second->PegaEstaca()));   //--- Pega o limite posterior no terreno ( = ao furo de sendagem final)
    itSEUB = PairBoundsTerreno.second;                                                                       //--- Fim do trecho
  }
  else itSEUB = --setEstacasTerreno.end();  
}

void DesPerfilGeoView::OnButmovesoy()
{
  MoverApenasEixoy = !MoverApenasEixoy;
  PonMousePixelsAtual = PonMousePixelsAnterior = PonMouseClicado = InicioArrast;

  RedrawWindow();
}

const void* DesPerfilGeoView::VerificaTipoPerfilOrfao(ittysetCPerfilTipoSolo itPerfil,int& TipoPonto)
{
  ittyPerfilTipoSolo itPontoPerfil(itPerfil->PegaPerfil().begin());
  bool Achou(false);
  const void* pPonto(NULL);

  while(itPontoPerfil != itPerfil->PegaPerfil().end() && !Achou)
  {
    TipoPonto = PONTOFUROSONDAGEM;
    pPonto = PosicionaMaisProximo(itPontoPerfil->EstVirtual,itPontoPerfil->Cota,Toleranciax,Toleranciay,TipoPonto);
    if(pPonto!= NULL) return pPonto; 

    ++itPontoPerfil;
  }

  TipoPonto = NENHUMTIPO;

  return NULL;
}

void DesPerfilGeoView::OnPopupPerfilSelecionar()
{
  itPerfilAtual = itPerfilSelecionado = itPerfilMarcado;
  itPerfilMarcado = PerfilGeologico.PegaPerfisSolos().end();

  COLORREF Cor(itPerfilSelecionado->PegaCor());

  ((CChildFrame*) GetParentFrame())->MudaCorPerfGeoSel(itPerfilSelecionado->PegaCor());

  RedrawWindow();
}

bool DesPerfilGeoView::VerificaInsercaoPonto(ittyPerfilTipoSolo ItPerfilSolo,ittysetCPerfilTipoSolo ItSetPerfTipoSolo,CEstaca& EstPonto)
{
  ittyPerfilTipoSolo ItPontoAnterior(ItPerfilSolo),ItPontoPosterior(ItPerfilSolo);
  bool OK(false);

  if(!CruzouTerreno(EstPonto))
  {
    --ItPontoAnterior;
    ++ItPontoPosterior;

    if(ItPontoAnterior !=  ItSetPerfTipoSolo->PegaPerfil().end()) OK = VerificaInsercao(*(ItPerfilSolo),*ItPontoAnterior);   
    if(OK && ItPontoPosterior != ItSetPerfTipoSolo->PegaPerfil().end()) OK = VerificaInsercao(*ItPontoPosterior,*(ItPerfilSolo));   

    if(!OK) AfxMessageBox("Este ponto não pode ser inserido; perfís geológicos não podem se cruzar",MB_ICONERROR);
  }
  else AfxMessageBox("Este ponto não pode ser inserido; perfís geológicos não podem cruzar o terreno primitivo",MB_ICONERROR);

  return OK;
}

void DesPerfilGeoView::SelecionaPerfil(char* pTipoPerfil)
{
  bool Achou(false);
  ittysetCPerfilTipoSolo itPerfilSolo(PerfilGeologico.PegaPerfisSolos().begin());

  while(itPerfilSolo != PerfilGeologico.PegaPerfisSolos().end() && !Achou)
  {
    Achou = itPerfilSolo->PegaTipoSolo() == pTipoPerfil;
    if(!Achou) ++itPerfilSolo;
  }

  if(Achou)
  {
    itPerfilMarcado = itPerfilSolo;
    OnPopupPerfilSelecionar();
  }
}

void DesPerfilGeoView::OnPerfilCopiartodoperfil()
{
  itPerfilAtual = itPerfilSelecionado = itPerfilMarcado;
  PonMousePixelsAtual = PonMousePixelsAnterior = PonMouseClicado = InicioArrast;
  TrechoPerfilGeolCopiado = true;
}

void DesPerfilGeoView::OnPerfilCopiarapenasestetrecho()
{
  PegaTrechoPerfGeol(CEstaca(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY),itInicioTrecho,itFimTrecho);
  PonMousePixelsAtual = PonMousePixelsAnterior = PonMouseClicado = InicioArrast;
  TrechoPerfilGeolCopiado = true;
}

void DesPerfilGeoView::PegaTrechoPerfGeol(const CEstaca& Ponto,ittyPerfilTipoSolo& itInicioTrecho,ittyPerfilTipoSolo& itFimTrecho)
{
  if(!TrechoMarcado)
  {
    pairERSFS PairERFS(PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem().equal_range(Ponto));        //--- Pega os 2 furos de sondagem, inicio e fim do trecho

    if(PairERFS.first != PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem().end())                    //--- Ajusta o início
    {
      if(PairERFS.first->PegaEstaca().EstVirtual > Ponto.EstVirtual) --PairERFS.first;
    }
    else --PairERFS.first;

    if(PairERFS.first !=  PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem().end())                   //--- Se true é o primeiro trecho antes do primeiro furo de sondagem
    {
      typairitPerfilTipoSolo PairBoundsPerfGeol(itPerfilMarcado->PegaPerfil().equal_range(PairERFS.first->PegaEstaca()));   //--- Pega o limite anterior no terreno (= ao furo de sondagem inicial) 

      if(PairBoundsPerfGeol.first != itPerfilMarcado->PegaPerfil().end() && PairBoundsPerfGeol.first != itPerfilMarcado->PegaPerfil().begin()) --PairBoundsPerfGeol.first;                      //--- Ajusta o início
      itInicioTrecho = PairBoundsPerfGeol.first;                                                                       //--- Inicio do trecho
    }
    else itFimTrecho = itPerfilMarcado->PegaPerfil().begin();

    if(PairERFS.second !=  PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem().end())                  //--- Se true é o último trecho, depois do ultimo furo de sondagem0       
    {
      typairitPerfilTipoSolo PairBoundsPG(itPerfilMarcado->PegaPerfil().equal_range(PairERFS.second->PegaEstaca()));   //--- Pega o limite posterior no terreno ( = ao furo de sendagem final)
      itFimTrecho = PairBoundsPG.second;     
      if(itFimTrecho == itPerfilMarcado->PegaPerfil().end()) --itFimTrecho;                                                                 //--- Fim do trecho
    }
    else itFimTrecho = --itPerfilMarcado->PegaPerfil().end();  
  }
}

bool DesPerfilGeoView::VerfInsercaoPerfilGeologico(const tyPerfilTipoSolo& PerfilTipoSolo)
{
  if(PerfilTipoSolo.size() > 1)
  {
    register ittyPerfilTipoSolo itPTSAnterior(PerfilTipoSolo.begin()),itPTSAtual(itPTSAnterior);

    for(++itPTSAtual ; itPTSAtual != PerfilTipoSolo.end() ; ++itPTSAtual)
    {
      if(VerificaInsercao(*itPTSAtual,*itPTSAnterior,true) == false) 
      {
        return false;    //--- Interceptou, nao pode inserir  //itPerfilAtual
      }
      ++itPTSAnterior;
    }
  }

  return true;  //--- Não interceptou nenhum outro perfil, pode inserir
}

bool DesPerfilGeoView::VerPerfilAcimaTerrenoPrimitivo(const tyPerfilTipoSolo& PerfilTipoSolo,double Deltay)
{
  for(register itctyPerfilTipoSolo itPTSAtual = PerfilTipoSolo.begin() ; itPTSAtual != PerfilTipoSolo.end() ; ++itPTSAtual)
  {
    typairitittysetEstaca itLimites(setEstacasTerreno.equal_range(*itPTSAtual));

    if(itLimites.first != setEstacasTerreno.end() && itLimites.second != setEstacasTerreno.end())
    {
      if(itLimites.first->EstVirtual != itPTSAtual->EstVirtual) --itLimites.first;

      if(itLimites.first !=  setEstacasTerreno.end())
      {
        double cota = itLimites.first->EstReal * (itPTSAtual->EstVirtual - itLimites.first->EstVirtual) + itLimites.first->Cota;

        if((itLimites.first->EstReal * (itPTSAtual->EstVirtual - itLimites.first->EstVirtual) + itLimites.first->Cota) < (itPTSAtual->Cota-Deltay)) 
        {
          return true;
        }
      }
    }
  }

  return false;  //--- Não há nenhum ponto acima do terreno primitivo, pode inserir
}

void DesPerfilGeoView::CalculaIsTerrenoPrimitivo()
{
  if(setEstacasTerreno.size() > 1)
  {
    register ittysetEstaca itAnterior(setEstacasTerreno.begin()),itAtual(itAnterior);

    for(++itAtual ; itAtual != setEstacasTerreno.end() ; ++itAtual,++itAnterior)
    {
      const_cast<CEstaca*>(&*itAnterior)->EstReal = (itAtual->Cota - itAnterior->Cota) / (itAtual->EstVirtual - itAnterior->EstVirtual);
    }
  }
}

void DesPerfilGeoView::OnPopupMudarTipo()
{
  if(VerfLigacaoFurosSondagem() == false)
  {
    tylststring ListaNomesTipos;

    for(itsetCTipoSolo ItTipoSolo = SetTiposSolos.begin() ; ItTipoSolo != SetTiposSolos.end() ; ++ItTipoSolo) 
    {
      if(ItTipoSolo->ListaCampos.size() > 0)
      {
        if(CString(ItTipoSolo->ListaCampos.begin()->c_str()) != CString(itPerfilMarcado->PegaTipoSolo().c_str())) 
        {
          ListaNomesTipos.push_back(const_cast<CTipoSolo*>(&*ItTipoSolo)->ListaCampos.begin()->c_str());
        }
      }
    }

    std::string Resposta;

    CDSelTipoSolo DiaSelTipoSolo(this,ListaNomesTipos,Resposta);

    DiaSelTipoSolo.DoModal();

    itsetCTipoSolo itTipoSelecionado(SetTiposSolos.find(CTipoSolo(Resposta.c_str())));

    if(itTipoSelecionado != SetTiposSolos.end())
    {
      PerfilGeologico.ColocaTipoNoPerfil(Resposta,itPerfilMarcado,itTipoSelecionado->Traco,itTipoSelecionado->Cor);
    }

    itPerfilMarcado = PerfilGeologico.PegaPerfisSolos().end();

    AlterouPerfil = true;
  }
  else
  {
    AfxMessageBox("O Tipo deste perfil não pode ser alterado porque este perfil está ligado a furos de sondagem, para mudar o tipo deste perfil remova as ligações com os furos de sondagem.",MB_ICONERROR);
  }

  RedrawWindow();
}

void DesPerfilGeoView::AbrePerfil(typairittyPerfilTSrsetPTS* pairPontoSet)  //---não está sendo usada, o usuário pode remover um trecho ao inves de abrir o perfil
{
  ittyPerfilTipoSolo itPerfil(*pairPontoSet->first);
  const tyPerfilTipoSolo& itPerfAt(pairPontoSet->second->PegaPerfil());
  ittysetCPerfilTipoSolo itPerfilTipoSolo(pairPontoSet->second);

  //--- Insere o novo perfil

  pairtysetCPerfilTipoSoloItBool PairitNovoPerfil(PerfilGeologico.IncluiPerfilSolo(itPerfilTipoSolo->PegaTipoSolo(),*itPerfil,itPerfilTipoSolo->PegaTraco(),itPerfilTipoSolo->PegaCor()));

  if(PairitNovoPerfil.second == true)
  {
    for(itPerfil ; itPerfil != itPerfilTipoSolo->PegaPerfil().end() ; ++itPerfil)
    {
      (const_cast<CPerfilTipoSolo*> (&*PairitNovoPerfil.first))->IncluiPonto(*itPerfil);
    }
  }

  //--- remove os pontos do perfil anterior

  (const_cast<tyPerfilTipoSolo*>(&itPerfilTipoSolo->PegaPerfil())->erase(*pairPontoSet->first,itPerfilTipoSolo->PegaPerfil().end()));
}

void DesPerfilGeoView::RemoveTrechoQualquer(ittyPerfilTipoSolo itInicio,ittyPerfilTipoSolo itFim,CPerfilTipoSolo* pPerfilTSAtual)  
{
 //--- Se for terreno so pode remover a ligação entre pontos adjacentes

 ittysetEstaca itTerrenoIni(setEstacasTerreno.find(*itInicio)),itTerrenoFim(setEstacasTerreno.find(*itFim)); 

 if(itTerrenoIni != setEstacasTerreno.end() || itTerrenoFim != setEstacasTerreno.end())
 {
   if((itTerrenoIni != setEstacasTerreno.end() && fabs(itTerrenoIni->Cota-itInicio->Cota) < 1e-3) || (itTerrenoFim != setEstacasTerreno.end() && fabs(itTerrenoFim->Cota-itFim->Cota) < 1e-3))
   { 
     ittysetEstaca itTerrenoAnterior(itTerrenoIni),itTerrenoPosterior(itTerrenoIni);

     --itTerrenoAnterior;
     if(itTerrenoPosterior != setEstacasTerreno.end()) ++itTerrenoPosterior;

     if(itTerrenoFim != itTerrenoAnterior && itTerrenoFim != itTerrenoPosterior) 
     {
       AfxMessageBox("No terreno primitivo não é permitido remover pontos, para abrir o perfil marque pontos adjacentes",MB_ICONWARNING);
       return;
     }
   }
 }

 //--- Cria o novo perfil do fim do trecho até o fim do perfil 

  if(itFim != pPerfilTSAtual->PegaPerfil().end())    //--- Se o trecho está indo até o fim do perfil não há novo trecho a criar
  { 
    if(*itFim != *pPerfilTSAtual->PegaPerfil().rbegin())   //--- Se sobrou mais de um ponto cria o novo perfil
    {
      pairtysetCPerfilTipoSoloItBool PairitNovoPerfil(PerfilGeologico.IncluiPerfilSolo(pPerfilTSAtual->PegaTipoSolo(),*itFim,pPerfilTSAtual->PegaTraco(),pPerfilTSAtual->PegaCor()));

      for(register ittyPerfilTipoSolo itPerfilAtual = itFim ; itPerfilAtual!= pPerfilTSAtual->PegaPerfil().end() ; ++itPerfilAtual)
      {
        (const_cast<CPerfilTipoSolo*> (&*PairitNovoPerfil.first))->IncluiPonto(*itPerfilAtual);
      }
    }
  }

  //--- Remove o trecho do perfil Atual

  const_cast<tyPerfilTipoSolo*> (&pPerfilTSAtual->PegaPerfil())->erase(++itInicio, pPerfilTSAtual->PegaPerfil().end()); 

  //--- Se o perfil ficou com apenas um ponto remove o perfil

  if(pPerfilTSAtual->PegaPerfil().size()  < 2) PerfilGeologico.RemovePerfil(*pPerfilTSAtual);
  if(pPerfilTSAtual->PegaPerfil().size()  < 2) PerfilGeologico.RemovePerfil(*pPerfilTSAtual);

  AlterouPerfil = true;
}

void DesPerfilGeoView::OnQualquertrechoRemover()
{
  RemoveTrechoQualquer(itInicioTrecho,itFimTrecho,const_cast<CPerfilTipoSolo*>(&*itPerfilSelecionado));

  itPerfilAtual = itPerfilMarcado = itPerfilSelecionado = PerfilGeologico.PegaPerfisSolos().end();
  TrechoMarcado = false;

  RedrawWindow();
}

void DesPerfilGeoView::OnQualquertrechoCopiar()
{
  PonMousePixelsAtual = PonMousePixelsAnterior = PonMouseClicado = InicioArrast;
  TrechoPerfilGeolCopiado = true;

  RedrawWindow();
}

void DesPerfilGeoView::OnQualquertrechoSair()
{
  itPerfilMarcado = PerfilGeologico.PegaPerfisSolos().end();
  TrechoMarcado = false;
}

void DesPerfilGeoView::OnRestTodoTerreno()
{
  RemovePerfisNoTerreno();

  if(setEstacasTerreno.size() > 1)
  {
    itInicioTrechoTerreno = setEstacasTerreno.begin();
    itFimTrechoTerreno = setEstacasTerreno.end();
    --itFimTrechoTerreno;

    PerfilGeologico.CriaPerfisNosTrechos(itInicioTrechoTerreno,itFimTrechoTerreno,Ponto(0.0,0.0));
    itInicioTrechoTerreno = itFimTrechoTerreno = setEstacasTerreno.end();

    AlterouPerfil = true;
  }
}

void DesPerfilGeoView::OnRestEsteTrechoTerreno()
{
  PegaTrechoTerreno(CEstaca(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY),itInicioTrechoTerreno,itFimTrechoTerreno);

  if(setEstacasTerreno.size() > 1)
  {
    itInicioTrechoTerreno = setEstacasTerreno.begin();
    itFimTrechoTerreno = setEstacasTerreno.end();

    --itFimTrechoTerreno;

    PerfilGeologico.CriaPerfisNosTrechos(itInicioTrechoTerreno,itFimTrechoTerreno,Ponto(0.0,0.0));
    itInicioTrechoTerreno = itFimTrechoTerreno = setEstacasTerreno.end();

    AlterouPerfil = true;
  } 
}

void DesPerfilGeoView::RemovePerfisNoTerreno()
{
  for(ittysetCPerfilTipoSolo itPerfilSolo=PerfilGeologico.PegaPerfisSolos().begin() ; itPerfilSolo != PerfilGeologico.PegaPerfisSolos().end() ; ++itPerfilSolo)
  {
//    if(ETerreno(*itPerfilSolo->PegaPerfil().begin()/* && ETerreno(*itPerfilSolo->PegaPerfil().rbegin()))*/))
    {
 //     PerfilGeologico.RemovePerfil(itPerfilSolo);
    }
  }
}
void DesPerfilGeoView::OnCorPerfAtual() 
{
  itPerfilSelecionado = PerfilGeologico.PegaPerfisSolos().end();
  ((CChildFrame*) GetParentFrame())->MudaCorPerfGeoSel(RGB(0,0,0));
}

void DesPerfilGeoView::OnPopupVisualizarperfilselecionado()
{
  if(itPerfilSelecionado == PerfilGeologico.PegaPerfisSolos().end())
    AfxMessageBox("Não há perfil selecionado.",MB_ICONASTERISK);
  else
    PiscarPerfil(itPerfilSelecionado);
}

void DesPerfilGeoView::OnSetFocus(CWnd* pOldWnd)
{
  CView::OnSetFocus(pOldWnd);

  CMainFrame* MF(((CMainFrame*) AfxGetMainWnd()));

  if(MF->SelecionaRua(Trecho) == CB_ERR) 
  {
    AfxMessageBox("Houve Erro, o projeto não pôde ser selecionado, \r\n Não grave o projeto, ele esta deteriorado. contacte o suporte - VUE_ERR_MFM001");
    MF->SelecionaRua("");
  }
  
}
