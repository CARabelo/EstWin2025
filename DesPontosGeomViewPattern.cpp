// DesPontosGeomView.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "carqterreno.h"
#include "cponto.h"
#include <string>
#include "mat.h"
#include "estaca.h"
#include "clpontos.h"
#include "perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include <list>
#include <set>
#include <vector>
#include <algorithm>
#include "cdeltasuperficie.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include <map>
#include "CAcidentesTopograficos.h"
#include "CSegmento.h"
#include "superficie.h"
#include "xygreide.h"
#include "meleditbox.h"
#include "cdpopuppontos.h"
#include "supercstring.h"
#include "carqcurhorizontais.h" 
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "ccurhorizontal.h"
#include "ccurhorizontais.h"
#include "carqsecoesavulsas.h"
#include "csecaoavulsa.h"
#include "csecoesavulsas.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include "CLembrete.h"
#include "despontosgeomview.h"
#include "xymouse.h"
#include "xygreide.h"
#include "palette.h"
#include "ddeslizantes.h"
#include "deslizantesgeom.h"
#include "mytoolbar.h"
#include "comboprocdessecao.h"
#include "diapontosgeom.h"
#include "childfrm.h"
#include "mainfrm.h"
#include "dpontogeometrico.h"
#include "dconfdesgeom.h"
#include "deditacurhoriz.h"
#include "drotacionar.h"
#include <algorithm>    // for set_intersection, set_union
#include <functional>   // for less
#include <iomanip> 
#include <iostream>
#include <strstream>   // for sstream
#include "editpopup.h"
#include "monologo.h"
#include "calcnota.h"
#include "carqtalcorte.h"
#include "carqtalaterro.h"
#include "ctalcorte.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include "carqgradientes.h"
#include "cgradientes.h"
#include "csecao.h"
#include "clistasecoes.h"
#include "listamedicoes.h"
#include "dvolumes.h"
#include <string>
#include "carqhpavimento.h"
#include "chpavimento.h"
#include "carqpistasfixas.h"
#include "cpistasfixas.h"
#include "dessecaoview.h"
#include "dcoordenadas.h"
#include "dialogo.h"
#include "dcalcgeometrico.h"
#include "dcalcgeoseccdireto.h"
#include "dsecionar.h"
#include "DiaAbrirCriarArea.h"
#include "CMemDC.h"
#include "CDDEfineFronteira.h"
#include "CDPedeDescricao.h"
#include "CDiaLembretes.h"
#include "carqcurverticais.h"
#include "ccurverticais.h"
#include "CArqOAC.h"
#include "CArqOAE.h"
#include "DesPerfilView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define _CRT_SECURE_NO_WARNINGS

#define OPR_APAGAR 0
#define OPR_DESENHAR 1
#define TYP_BORDOS 1
#define TYP_ACDTOPOG 2 
#define ESQUERDO 0
#define DIREITO 1
#define RESTRICOES 1
#define BORDOS 0
#define ESPIRAL 0
#define CIRCULAR 1

#define LER 1
#define GRAVAR 2

#define VARRER_TERRENO 0
#define VARRER_PROJETO 0
#define VARRER_SECOES 1
#define VARRER_TUDO 1
#define OBJ_PI 1
#define OBJ_CC 2

#define CURVA_ESPIRAL 0
#define KEY_ESCAPE 27
#define KEY_DELETE 16
#define KEY_F5 116

#define MOSTRAR_NUMERO_PONTO 1
#define MOSTRAR_DESC 1 
#define MOSTRAR_COORDENADAS 1

#define	WM_CLOSE_TOOLBAR (WM_APP+100)

extern class monologo monolog;
extern class dialogo dialogar;
extern class Matem Mat;

void inline TransladaPontoPlano(double* x,double* y,double NovaOrigemX,double NovaOrigemY)
{
  *x = *x - NovaOrigemX;
  *y = *y - NovaOrigemY;
}

void inline RotacionaPontoPlano(double Sin,double Cos,double Angulo,double* x,double* y)
{
  //                   Matriz de rota��o

  /*
  x� = x . cos (fi)  -  y . sin(fi)

  y� = x . sin (fi)  +  y . cos(fi)
  */

  double xTemp(*x * Cos - *y * Sin),yTemp(*x * Sin + *y * Cos);

  *x = xTemp;
  *y = yTemp;
}

/////////////////////////////////////////////////////////////////////////////
// DesPontosGeomView

IMPLEMENT_DYNCREATE(DesPontosGeomView, CView)

  DesPontosGeomView::DesPontosGeomView():
Superficie(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()),false),
  Arrastando(false),
  ArrastandoPI(false),
  ArrastandoCC(false),
  EZoom(false),
  CriandoRestricao(false),
  CriandoBordo(false),
  PAntMouse(10000,10000),
  AlterouTerreno(false),
  AlterouProjeto(false),
  SecoesTipo(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual())),
  CurvasHorizontais(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()),SecoesTipo),
  mapCurvasHorizontaisTS(CurvasHorizontais.PegaMapCurHorizontaisTS()),
  mapCurvasHorizontaisPI(CurvasHorizontais.PegaMapCurHorizontaisPI()),
  mapCurvasHorizontaisCC(CurvasHorizontais.PegaMapCurHorizontaisCC()),
  ItCurHorizSendoArrastadaTS(mapCurvasHorizontaisTS.end()),
  ItCurHorizSendoArrastadaPI(mapCurvasHorizontaisPI.end()),
  ItCurHorizSendoArrastadaCC(mapCurvasHorizontaisCC.end()),
  DPopUpPonto(this,RGB(255,255,166)),
  AnguloRotacao (Superficie.PegaAnguloRotacao()),
  m_nTimer(NENHUM_TIMER),
  ComSecoes(false),
  DeltaSupSendoArrastada(Superficie.pSuperficieAtual->end()),
  SecoesIconizadas(false),
  TerminarArrasto(false),
  itUltimaCurvaArrastadaPI(mapCurvasHorizontaisPI.end()),
  itUltimaCurvaArrastadaCC(mapCurvasHorizontaisCC.end()),
  AlterouAngulo(false),
  Seno(sin(AnguloRotacao)),
  Coseno(cos(AnguloRotacao)),
  Inicializou(false),
  Quadrante(floor(AnguloRotacao / Mat.M_PI_2)),
  PrimeiroMovimento(true),
  Destruindo(false),
  PegandoPontoSecAvulsa(false),
  PegouPrimPonto(false),
  SecoesAvulsas(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual())),
  AlterouSecoesAvulsas(false),
  DesCNTopograficas(true),
  DefBordos(false),
  Alargamentos(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual())),
  CursorAtual(IDC_HAND),
  DesenharObjetos(false),
  DesenhandoObjetos(false),
  ObjetoAtual(0),
  ObjetoAnterior(0),
  CircularPontoIntersecao(false),
  FiltrarAciTopog(false),
  DesenharPontosInseridos(true),
  ItLLDeltaSupMarcada(Superficie.PegaListaRestricoes().end()),
  PegouPonto(false),
  DeltaSupSobMouseAnterior(Superficie.pSuperficieAtual->end()),
  InserindoPonto(false),
  PegouPontoDesabilitado(false),
  PegouRestricao(false),
  TrocandoObjeto(false),
  ItPontoDesabilitado(Superficie.SetPonDesabilitados().end()),
  InserindoLembrete(false),
  ItLembrete(SetLembretes.end()),
  MovendoLembrete(false),
  PermitirArrastar(true),
  SaltarCNs(0),
  DefinindoLinhaBase(0),
  DeltaSupAnterior(Superficie.pSuperficieAtual->end())    
{
  Escala[X] = Escala[Y] = 30.0;
  RazaoHV = 1.0;
  Deltas[X] = Deltas[Y] = 0.0;

  LeCoordSecoes();                 //--- L� as coordenadas das se��es

  LeAcidentesTopograficos(); 

  Superficie.PegaListaBordos().clear(); 

  ((CMainFrame*)AfxGetMainWnd())->DesPontosGeo = this;   //--- Ponteiro para ser usado apenas para salvar o windowplacement 
  //--- desta janela(junto com as sec�es) 
  ((CMainFrame*)AfxGetMainWnd())->DesPontosGeoComSecao = false;

  Superficie.CriaSetCurvasNivel();

  ItCurHorizSendoArrastadaPIAnterior = mapCurvasHorizontaisPI.end();

  BandeiraBMP.LoadBitmap(IDB_BITMAPFLAG);
  BandeiraMaskBMP.LoadBitmap(IDB_FLAGMASK);

  LeLembretes();

  if(Quadrante == 4) Quadrante = 3;  //--- Ajuste de erro de arredondamento 
}

DesPontosGeomView::~DesPontosGeomView()
{
  CString CSDados;

  Destruindo = true;

  CMainFrame* MainFrame =  (CMainFrame*) AfxGetMainWnd();
  std::string NomeArquivo(MainFrame->PegaProjetoAtual()+std::string(".ini"));

  SalvaLembretes();

  Ponto PDeltas(Deltas[X],Deltas[Y]);
  // if(Superficie.EstaRotacionada()) PDeltas.RotacionaPlano(sin(Superficie.PegaAnguloRotacao()),cos(Superficie.PegaAnguloRotacao()),Superficie.PegaAnguloRotacao());

  CSDados.Format("%lf,%lf,%lf,%lf,%lf",Escala[X],Escala[Y],1.0,PDeltas.x,PDeltas.y);

  ::WritePrivateProfileString(_T("DadosDesenhos"),MainFrame->DesPerfil ? _T("DadosDesPontosGeoComPerfil") : _T("DadosDesPontosGeo"),CSDados,NomeArquivo.c_str());		

  CSDados.Format("%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i",DesenharPixel,DesenharCirculo,
    DesenharCruz,DesenharSecoes,DesenharArestas,DesenharBordos,DesenharRestricoes,
    DesenharPontos,MostrarDescricao,DesenharMalha,EquiMalha,EquiCotas,DesenharProjeto,
    bAciTopog,CircularSecoes,MostrarToolBarSuperf,MostrarToolBarGeom,CotarMalha,VarrerTerreno,VarrerProjeto,VarrerCN,DesCNTopograficas,DesenharCN,CotarCN,DefBordos,
    SincronizarSecoes,NaoPermitirAlteracoes,SecoesIconizadas,DesenharObjetos,DesenhandoObjetos,DesenharPontosInseridos,ObjetoAtual,PermitirArrastar,SaltarCNs);    

  ::WritePrivateProfileString(_T("DadosDesenhos"),MainFrame->DesPerfil ? _T("DadosDesPontosGeoConfigComPerfil") :_T("DadosDesPontosGeoConfig") ,CSDados,NomeArquivo.c_str());		

  CSDados.Format("%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu",Cores[0],Cores[1],Cores[2],Cores[3],Cores[4],Cores[5],Cores[6],Cores[7],Cores[8],Cores[9]);
  ::WritePrivateProfileString(_T("DadosDesenhos"),MainFrame->DesPerfil?_T("DadosDesPontosGeoCoresComPerfil"):_T("DadosDesPontosGeoCores") ,CSDados,NomeArquivo.c_str());

  CSDados.Format("%i",MainFrame->DesPontosGeoComSecao ? true:false);
  ::WritePrivateProfileString(_T("DadosDesenhos"),_T("DesPontosComSecoes"),CSDados,NomeArquivo.c_str());		
  MainFrame->DesPontosGeoComSecao = false;

  CSDados.Format("%i",MainFrame->DesPerfil ? true:false);
  ::WritePrivateProfileString(_T("DadosDesenhos"),_T("DesPontosComPerfil"),CSDados,NomeArquivo.c_str());		

  ((CMainFrame*)AfxGetMainWnd())->DesPontosGeo = NULL;   
}

BEGIN_MESSAGE_MAP(DesPontosGeomView, CView)
  ON_WM_LBUTTONUP()
  ON_WM_RBUTTONUP()
  ON_WM_LBUTTONDOWN()
  ON_WM_RBUTTONDOWN()
  ON_WM_SIZE()
  ON_COMMAND(ID_ENQUADRAR, OnEnquadrar)
  ON_COMMAND(ID_REDESENHAR, OnRedesenhar)
  ON_UPDATE_COMMAND_UI(ID_GRAVAR, OnUpdateGravar)
  ON_WM_MOUSEMOVE()
  ON_WM_SETFOCUS()
  ON_WM_TIMER()
  ON_WM_ERASEBKGND()
  ON_WM_LBUTTONDBLCLK()
  ON_COMMAND(ID_BUTROTACIONAR, OnButrotacionar)
  ON_UPDATE_COMMAND_UI(ID_BUTROTACIONAR, OnUpdateButrotacionar)
  ON_UPDATE_COMMAND_UI(ID_REDESENHAR, OnUpdateRedesenhar)
  ON_UPDATE_COMMAND_UI(ID_ENQUADRAR, OnUpdateEnquadrar)
  ON_COMMAND(ID_DEFINEBORDOS, OnDefinebordos)
  ON_COMMAND(ID_ACITOPOG, OnAcitopog)
  ON_COMMAND(ID_BUTACITOPOG,OnDefAciTopog)
  ON_COMMAND(ID_CONFDESPONTOSGEO, OnConfdes)
  ON_COMMAND(ID_SECSEMPREVIS_ICONIZAR, OnSecsemprevisIconizar)
  ON_WM_SIZING()
  ON_COMMAND(ID_GRAVAR, OnGravar)
  ON_COMMAND(ID_IRPARAPONTO, OnIrparaponto)
  ON_COMMAND(ID_ALTERNARTITULOGREIDE, OnAlternartituloDesGeom)
  ON_COMMAND(ID_BUTFECHar, OnButFechar)
  ON_WM_CLOSE()
  //ON_COMMAND(ID_CALCGEOMETRICO, OnCalcgeometrico)  n�o tem mais, tudo est� sendo feito no desenho
  ON_COMMAND(ID_BUTSECAVULSA, OnSecaoAvulsa)
  ON_COMMAND(ID_BUTCNTOPOGRAF,OnTipoCN)
  ON_COMMAND(ID_BUTBORDOS,OnBordos)
  ON_COMMAND(ID_BUTSINCRO,OnSincronizarSecoes)
  ON_UPDATE_COMMAND_UI(ID_BUTSINCRO, OnUpdateButSincro)
  ON_COMMAND(ID_BUTLOCK,OnNaoPermitirAlteracoes)
  ON_WM_CREATE()
  ON_WM_CHAR()
  ON_WM_KEYUP()
  ON_WM_MBUTTONDBLCLK()
  ON_WM_RBUTTONDBLCLK()
  ON_WM_MOUSEWHEEL()
  ON_COMMAND(ID_BUTFRONTEIRA, &DesPontosGeomView::OnButfronteira)
  ON_COMMAND(ID_DES_MURO, &DesPontosGeomView::OnDesMuro)
  ON_COMMAND(ID_DESENHAR, &DesPontosGeomView::OnDesenhar)
  ON_COMMAND(ID_NA, &DesPontosGeomView::OnNa)
  ON_COMMAND(ID_DES_MEIOFIO, &DesPontosGeomView::OnDesMeiofio)
  ON_COMMAND(ID_DES_NA, &DesPontosGeomView::OnDesNa)
  ON_COMMAND(ID_DES_ROCHA, &DesPontosGeomView::OnDesRocha)
  ON_COMMAND(ID_DES_BUEIRO, &DesPontosGeomView::OnDesBueiro)
  ON_COMMAND(ID_DES_REDEELETRICA_AT, &DesPontosGeomView::OnDesRedeeletricaAt)
  ON_COMMAND(ID_DES_REDEELETRICA_BT, &DesPontosGeomView::OnDesRedeeletricaBt)
  ON_COMMAND(ID_DES_BORDO, &DesPontosGeomView::OnDesBordo)
  ON_COMMAND(ID_DES_CERCA, &DesPontosGeomView::OnDesCerca)
  ON_COMMAND(ID_DES_PONTE, &DesPontosGeomView::OnDesPonte)
  ON_COMMAND(ID_DES_POSTE, &DesPontosGeomView::OnDesPoste)
  ON_COMMAND(ID_DES_FRONTEIRA, &DesPontosGeomView::OnDesFronteira)
  ON_MESSAGE(WM_CLOSE_TOOLBAR, DesPontosGeomView::FechouToolBar)
  ON_COMMAND(ID_DES_FILTRAR, &DesPontosGeomView::OnDesFiltrar)
  ON_COMMAND(ID_BUTPONINS, &DesPontosGeomView::OnButponins)
  ON_COMMAND(ID_DES_EIXO, &DesPontosGeomView::OnDeseixo)
  ON_COMMAND(ID_DES_CRISTA, &DesPontosGeomView::OnDesCrista)
  ON_COMMAND(ID_DES_PE, &DesPontosGeomView::OnDesPe)
  ON_COMMAND(ID_DES_CANTO_CASA, &DesPontosGeomView::OnDesCantoCasa)
  ON_COMMAND(ID_DES_CANTO_CERCA, &DesPontosGeomView::OnDesCantoCerca)
  ON_COMMAND(ID_DES_CANTO_MURO, &DesPontosGeomView::OnDesCantoMuro)
  ON_COMMAND(ID_DES_CASA, &DesPontosGeomView::OnDesCasa)
  ON_UPDATE_COMMAND_UI(ID_DES_FRONTEIRA, &DesPontosGeomView::OnUpdateDesFronteira)
  ON_COMMAND(ID_DES_TAMPA_BUEIRO, &DesPontosGeomView::OnDesTampaBueiro)
  ON_COMMAND(ID_POPUP_DESCONSIDERAR, &DesPontosGeomView::OnPopupDesabilitar)
  ON_COMMAND(ID_POPUP_INCLUIR_PONTO, &DesPontosGeomView::OnPopupIncluirPonto)
  ON_COMMAND(ID_POPUP_HABILITAR, &DesPontosGeomView::OnPopupHabilitar)
  ON_COMMAND(ID_POPUP_SAIR, &DesPontosGeomView::OnPopupSair)
  ON_UPDATE_COMMAND_UI(ID_POPUP_DESCONSIDERAR, &DesPontosGeomView::OnUpdatePopupDesabilitar)
  ON_UPDATE_COMMAND_UI(ID_POPUP_INCLUIR_PONTO, &DesPontosGeomView::OnUpdatePopupIncluirPonto)
  ON_UPDATE_COMMAND_UI(ID_POPUP_HABILITAR, &DesPontosGeomView::OnUpdatePopupHabilitar)
  ON_COMMAND(ID_POPUP_REMOVER_PONTO, &DesPontosGeomView::OnPopupRemoverPonto)
  ON_UPDATE_COMMAND_UI(ID_POPUP_REMOVER_PONTO, &DesPontosGeomView::OnUpdatePopupRemoverPonto)
  ON_COMMAND(ID_BUTLEMBRETE, &DesPontosGeomView::OnButLembrete)
  ON_COMMAND(ID_POPUP_REMOVER, &DesPontosGeomView::OnPopupRemover)
  ON_COMMAND(ID_POPUP_TROCAR, &DesPontosGeomView::OnPopupTrocar)
  ON_UPDATE_COMMAND_UI(ID_POPUP_TROCAR, &DesPontosGeomView::OnUpdatePopupTrocar)
  ON_COMMAND(ID_BUTARRASTAR, &DesPontosGeomView::OnButarrastar)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DesPontosGeomView drawing

void DesPontosGeomView::OnDraw(CDC* ppDC)
{
  //--- Se for zoom, desenha o retangulo

  LOGBRUSH lb;
  lb.lbStyle = BS_SOLID;
  lb.lbColor = RGB (0, 255, 0);
  CPen pen;

 pen.CreatePen(PS_GEOMETRIC | PS_DASHDOT |  PS_ENDCAP_FLAT | PS_JOIN_ROUND, 26, &lb);

  CMMemDC pDC(ppDC);

WORD m_bitsBrush1[8] = { 0xee,0xed,0xeb,0xe7,0xef,0xff,0xff,0xff };

CBrush brush,*pOldbrush;
CBitmap bm;
bm.CreateBitmap(8,8,1,1,m_bitsBrush1);

LOGBRUSH logBrush;
logBrush.lbStyle = BS_PATTERN;
logBrush.lbHatch = (int) bm.GetSafeHandle();
logBrush.lbColor = RGB(0,0,0);

brush.CreateBrushIndirect(&logBrush);
int savedc = pDC.SaveDC();
pOldbrush=pDC.SelectObject(&brush);
pDC.SetBkMode(TRANSPARENT); 
pDC.SetBkColor(RGB(255,255,255));
//pDC.Rectangle(0,0,100,100);
//pDC.SelectObject(pOldbrush); 

 

//  pDC->SelectObject(pen); 

  pDC->MoveTo(0,0);
  pDC->LineTo(200,200);

  if(AlterouAngulo) 
  {
    OnAlterouAngulo(pDC);
    AlterouAngulo = false;
  }

  DesenhaNorte(pDC);

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

  CDC dcMemory;
  dcMemory.CreateCompatibleDC(pDC);

  for(ittysetLembretes itLembretes = SetLembretes.begin() ; itLembretes != SetLembretes.end() ; ++itLembretes)
  {
    if(!MovendoLembrete || ItLembrete != itLembretes)
    {
      Ponto Ppixels((itLembretes->Local().x - Deltas[X]) * Escala[X],(AlturaVideo - (itLembretes->Local().y - Deltas[Y])) * Escala[Y]);

      dcMemory.SelectObject(&BandeiraBMP);
      pDC->BitBlt(Ppixels.x,Ppixels.y,16,21,&dcMemory,0,0,SRCCOPY);
      dcMemory.SelectObject(BandeiraMaskBMP);
      pDC->BitBlt(Ppixels.x,Ppixels.y,16,21,&dcMemory,0,0,SRCAND);
    }
  }

  if(ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end())  //--- Restri��o marcada 
  {
    CPen PenaRemover;
    PenaRemover.CreatePen(PS_SOLID,4,RGB(190,190,190));
    register ItCLDeltaSup ItLRestricao(ItLLDeltaSupMarcada->begin()); 

    pDC->SelectObject(PenaRemover); 

    pDC->MoveTo(((*ItLRestricao).PCentral.x - Deltas[X]) * Escala[X],(AlturaVideo - ((*ItLRestricao).PCentral.y - Deltas[Y])) * Escala[Y]);

    while (++ItLRestricao != ItLLDeltaSupMarcada->end())
    {
      Ponto PAtual = (*ItLRestricao).PCentral;   //--- Pega o ponto atual
      Ponto Ppixels;

      Ppixels.x = ((PAtual.x - Deltas[X]) * Escala[X]);
      Ppixels.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y] ;

      pDC->LineTo(Ppixels.x,Ppixels.y);
    }   
  }         

  if(DesenharProjeto && mapCurvasHorizontaisTS.size())
  {
    CPen PenaProjeto;
    PenaProjeto.CreatePen(PS_SOLID,2,Cores[CORPROJETO]);
    pDC->SelectObject(&PenaProjeto);    

    DesenhaProjeto(mapCurvasHorizontaisTS.begin(),pDC); 
  }

  Ponto PAtual,Ppixels;

  CPen PenaRestricoes,PenaCotaMestra;                     

  PenaRestricoes.CreatePen(PS_SOLID,2,Cores[CORCRISTASPES]);
  PenaCotaMestra.CreatePen(PS_SOLID,2,Cores[CORCN]);

  if(DesenharRestricoes == true)
  {
    pDC->SelectObject(&PenaRestricoes);   
    const LLDeltaSup& ListaRestAtual(Superficie.PegaListaRestricoes());

    for(ItCLLDeltaSup LDRestricaoAtual = ListaRestAtual.begin() ; LDRestricaoAtual != ListaRestAtual.end(); ++LDRestricaoAtual)
    {
      register ItCLDeltaSup ItLRestricao((*LDRestricaoAtual).begin()); 

      unsigned int TipoRestricao(Superficie.VerfAciTopografico(LDRestricaoAtual));

      if(LDRestricaoAtual->size() != 0 && (!FiltrarAciTopog || TipoRestricao == ObjetoAtual))
      {
        if(TipoRestricao != CAciTopografico::FRONTEIRA || DesenharBordos)
        {
          if(TipoRestricao != CAciTopografico::NENHUM_ACITOPOG) CriaPenaRestri�ao(&PenaRestricoes,TipoRestricao);

          pDC->SelectObject(PenaRestricoes); 

          if(TipoRestricao != CAciTopografico::POSTE && TipoRestricao != CAciTopografico::TAMPA_BUEIRO)
          {
            pDC->MoveTo(((*ItLRestricao).PCentral.x - Deltas[X]) * Escala[X],(AlturaVideo - ((*ItLRestricao).PCentral.y - Deltas[Y])) * Escala[Y]);

            while (++ItLRestricao != (*LDRestricaoAtual).end())
            {
              PAtual = (*ItLRestricao).PCentral;   //--- Pega o ponto atual

              Ppixels.x = ((PAtual.x - Deltas[X]) * Escala[X]);
              Ppixels.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y] ;

              pDC->LineTo(Ppixels.x,Ppixels.y);
            }   
          }
          else 
          {
            if(TipoRestricao == CAciTopografico::POSTE)
            {
              while (ItLRestricao != (*LDRestricaoAtual).end())
              {
                DesenhaPoste((*ItLRestricao++).PCentral,pDC);
              }
            }
            else 
            {
              while (ItLRestricao != (*LDRestricaoAtual).end())
              {
                DesenhaBueiro((*ItLRestricao++).PCentral,&PenaRestricoes,pDC);
              }
            }
          }
        }
      }
    }
  }

  DesenhaSuperficie(Superficie.pSuperficieAtual->begin(),Superficie.pSuperficieAtual->end(),pDC);

  CPen PenaPontoInserido;
  PenaPontoInserido.CreatePen(PS_SOLID,1,RGB(200,0,0));
  pDC->SelectObject(&PenaPontoInserido); 

  //--- Desenha por cima, em vermelho, os pontos inseridos pelo usu�rio

  for(ItSSetPontos it = Superficie.SetPonInseridos().begin() ; it != Superficie.SetPonInseridos().end() ; ++it)
  {
    Ponto Ppixels;

    Ppixels.x = ((it->x - Deltas[X]) * Escala[X]);
    Ppixels.y = (AlturaVideo - (it->y - Deltas[Y])) * Escala[Y] ;

    pDC->Arc(Ppixels.x-3,Ppixels.y-3,Ppixels.x+3,Ppixels.y+3,0,0,0,0);   
  }

  CPen PenaPontoDesabilitado;
  PenaPontoDesabilitado.CreatePen(PS_SOLID,1,RGB(127,127,127));
  pDC->SelectObject(&PenaPontoDesabilitado); 

  //--- Desenha por cima, em cinza, os pontos desabilitados pelo usu�rio

  for(ItSSetPontos it = Superficie.SetPonDesabilitados().begin() ; it != Superficie.SetPonDesabilitados().end() ; ++it)
  {
    Ponto Ppixels;

    Ppixels.x = ((it->x - Deltas[X]) * Escala[X]);
    Ppixels.y = (AlturaVideo - (it->y - Deltas[Y])) * Escala[Y] ;

    pDC->Arc(Ppixels.x-3,Ppixels.y-3,Ppixels.x+3,Ppixels.y+3,0,0,0,0);   
  }

  CPen PenaMalha;

  PenaMalha.CreatePen(PS_SOLID,1,Cores[CORMALHA]);
  pDC->SelectObject(&PenaMalha);    

  if (DesenharMalha && Superficie.pSuperficieAtual->size()) DesenhaMalha(pDC);

  // CPen PenaBordos,PenaRestricoes,PenaCotaMestra;                     

  //  PenaBordos.CreatePen(PS_SOLID,2,Cores[CORBORDOS]);
  //  PenaRestricoes.CreatePen(PS_SOLID,2,Cores[CORCRISTASPES]);
  //  PenaCotaMestra.CreatePen(PS_SOLID,2,Cores[CORCN]);

  if(CircularPontoIntersecao)
  {
    pDC->SelectObject(&PenaCotaMestra);   
    Ppixels.x = (PontoIntersecao.x - Deltas[X]) * Escala[X];
    Ppixels.y = (AlturaVideo - (PontoIntersecao.y - Deltas[Y])) * Escala[Y] ;

    pDC->Arc(Ppixels.x-9,Ppixels.y-9,Ppixels.x+9,Ppixels.y+9,0,0,0,0);   	
    pDC->Arc(Ppixels.x-5,Ppixels.y-5,Ppixels.x+5,Ppixels.y+5,0,0,0,0);
  }

  //--- Desenha os objetos
  //--- Esta vazia por enquanto at� passar os bordos => acidentes topograficos
  //--- Eu passei os bordos para as restricoes. alterar===> passar tudio para acidentes topograficos

  pDC->SelectObject(&PenaRestricoes); 

  const mapAciTipog& MapaAciTopograficos(AcidentesTopograficos.PegaMapAcidentesTopograficos());

  if(0) for(register citmapAciTipog ItAciTopogAtual = MapaAciTopograficos.begin() ; ItAciTopogAtual != MapaAciTopograficos.end() ; ++ItAciTopogAtual)
  {
    const lstitsetPontos& LstItSetObjetos(ItAciTopogAtual->second.PegaLista());

    if(LstItSetObjetos.size())
    {
      register citlstitsetPontos it(LstItSetObjetos.begin());

      pDC->MoveTo(((*it)->x  - Deltas[X]) * Escala[X],(AlturaVideo - ((*it)->y - Deltas[Y])) * Escala[Y]);

      while(it!= LstItSetObjetos.end())
      {
        Ponto PAtual = **it;   //--- Pega o ponto atual

        Ppixels.x = ((PAtual.x - Deltas[X]) * Escala[X]);
        Ppixels.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y] ;

        //  Ppixels.x = (((**it).x - Deltas[X]) * Escala[X]);
        //   Ppixels.y = (AlturaVideo - ((**it).y - Deltas[Y])) * Escala[Y] ;

        pDC->LineTo(Ppixels.x,Ppixels.y);

        ++it;
      }
    }
  }

  //--- Desenha o objeto que est� sendo desenhado (ainda n�o esta no mapa de objetos)
  {
    const lstitsetPontos& LstItSetObjetos(AciTopogAtual.PegaLista());

    if(LstItSetObjetos.size())
    {
      register citlstitsetPontos it(LstItSetObjetos.begin());

      pDC->MoveTo(((*it)->x  - Deltas[X]) * Escala[X],(AlturaVideo - ((*it)->y - Deltas[Y])) * Escala[Y]);

      while(it!= LstItSetObjetos.end())
      {
        Ppixels.x = (((**it).x - Deltas[X]) * Escala[X]);
        Ppixels.y = (AlturaVideo - ((**it).y - Deltas[Y])) * Escala[Y] ;

        pDC->LineTo(Ppixels.x,Ppixels.y);

        ++it;
      }
    }
  }

  //--- Desenha as curvas de nivel

  double MultCotaMestra(Superficie.PegaEquidisCN() * 5.0);
  CPen PenaCNNormal;
  PenaCNNormal.CreatePen(PS_SOLID,1,Cores[CORCN]);

  if(DesenharCN)
  {
    if(DesCNTopograficas)
    {
      for(ItLLPontos itLLCN = Superficie.LPontosCN.begin() ; itLLCN != Superficie.LPontosCN.end() ; itLLCN++) 
      {
        if(SaltarCNs && fabs(fmod(itLLCN->begin()->z,SaltarCNs+1))) continue;

        int Mod4(0);
        POINT PBezier[4];

        if(fabs(fmod(itLLCN->begin()->z,MultCotaMestra)) < 0.0005) 
        {
          if(CotarCN) CotaCN((++(itLLCN->begin()))->fi,*(itLLCN->begin()),true,itLLCN->begin()->z,pDC,PenaCotaMestra);      
          else 
            pDC->SelectObject(PenaCotaMestra);
        }
        else 
          pDC->SelectObject(PenaCNNormal);  

        for (register ItLPontos itLCN = itLLCN->begin() ; itLCN != itLLCN->end() ; itLCN++) 
        {   
          Ppixels.x = ((itLCN->x - Deltas[X]) * Escala[X]);
          Ppixels.y = (AlturaVideo - (itLCN->y - Deltas[Y])) * Escala[Y] ;

          if(itLLCN->size() == 2 && itLCN == itLLCN->begin()) pDC->MoveTo(Ppixels.x,Ppixels.y);

          //   pDC->SetPixel(Ppixels.x,Ppixels.y,Cores[CORBORDOS]);   	//--- Pixel da interse��o

          PBezier[Mod4].x = (int)Ppixels.x;
          PBezier[Mod4].y = (int)Ppixels.y;

          if(Mod4 == 3)
          {
            pDC->PolyBezier(PBezier,4);

            PBezier[0].x = Ppixels.x;
            PBezier[0].y = Ppixels.y;

            pDC->MoveTo(Ppixels.x,Ppixels.y);

            Mod4 = 0;
          }
          Mod4++;         
        }

        if(Mod4 == 2) pDC->LineTo(PBezier[1].x,PBezier[1].y);

        if(fabs(fmod(itLLCN->begin()->z,MultCotaMestra)) < 0.0005) 
        {
          if(CotarCN) CotaCN(itLLCN->rbegin()->fi,*itLLCN->rbegin(),false,itLLCN->rbegin()->z,pDC,PenaCotaMestra);
          else pDC->SelectObject(PenaCotaMestra);
        }
      }
    }
    else
    {
      for(ItLLPontos itLLCN = Superficie.LPontosCN.begin() ; itLLCN != Superficie.LPontosCN.end() ; itLLCN++) 
      {
        if(SaltarCNs && fabs(fmod(itLLCN->begin()->z,SaltarCNs+1))) continue;

        register ItLPontos itLCN(itLLCN->begin()); 

        Ppixels.x = ((itLCN->x - Deltas[X]) * Escala[X]);
        Ppixels.y = (AlturaVideo - (itLCN->y - Deltas[Y])) * Escala[Y] ;

        pDC->MoveTo(Ppixels.x,Ppixels.y);

        if(fabs(fmod(itLLCN->begin()->z,MultCotaMestra)) < 0.0005) 
        {
          if(CotarCN) CotaCN((++(itLLCN->begin()))->fi,*(itLLCN->begin()),true,itLLCN->begin()->z,pDC,PenaCotaMestra);      
          else pDC->SelectObject(PenaCotaMestra);
        }
        else 
          pDC->SelectObject(PenaCNNormal);  

        for (itLCN++ ; itLCN != itLLCN->end() ; itLCN++) 
        {   
          Ppixels.x = ((itLCN->x - Deltas[X]) * Escala[X]);
          Ppixels.y = (AlturaVideo - (itLCN->y - Deltas[Y])) * Escala[Y] ;

          pDC->LineTo(Ppixels.x,Ppixels.y);
        }
      }
    }
  }

  //--- Desenha as se��es

  if (LPSecoes.size())
  {
    CPen PenaSec;  
    Ponto PAtual,Ppixels;

    PenaSec.CreatePen(PS_SOLID,1,Cores[CORSECOES]);
    pDC->SelectObject(&PenaSec);    

    ItLPontos itlPontos(LPSecoes.begin());

    //--- Desenha os segmentos de retas das se��es

    if (DesenharSecoes)
    {
      while(itlPontos != LPSecoes.end())
      {
        pDC->MoveTo(((*itlPontos).x - Deltas[X]) * Escala[X],(AlturaVideo - ((*itlPontos).y - Deltas[Y])) * Escala[Y]);
        itlPontos++;

        itlPontos++;

        pDC->LineTo(((*itlPontos).x - Deltas[X]) * Escala[X],(AlturaVideo - ((*itlPontos).y - Deltas[Y])) * Escala[Y]);
        itlPontos++;
      } 
    }

    itlPontos = LPSecoes.begin();

    //--- circula as estacas no eixo

    if (CircularSecoes)
    {
      while(itlPontos != LPSecoes.end())
      {
        itlPontos++;

        Ppixels.x = (((*itlPontos).x - Deltas[X]) * Escala[X]);
        Ppixels.y = (AlturaVideo - ((*itlPontos).y - Deltas[Y])) * Escala[Y] ;

        pDC->Arc(Ppixels.x-3,Ppixels.y-3,Ppixels.x+3,Ppixels.y+3,0,0,0,0);   	

        itlPontos++;
        itlPontos++;
      }
    }
  }

  const CList<CSecaoAvulsa,CSecaoAvulsa&>& ListSecAvulsasCalc(SecoesAvulsas.PegaLSecoesAvulsas());

  if (ListSecAvulsasCalc.GetSize() > 0)
  {
    CPen PenaSec;  
    Ponto PAtual,Ppixels;

    PenaSec.CreatePen(PS_SOLID,1,Cores[CORSECOES]);
    pDC->SelectObject(&PenaSec);    

    for (POSITION Atual = ListSecAvulsasCalc.GetHeadPosition() ; Atual != NULL ; ListSecAvulsasCalc.GetNext(Atual))
    {
      const CSecaoAvulsa& SecaoAvulsaAtual(ListSecAvulsasCalc.GetAt(Atual));

      pDC->MoveTo((SecaoAvulsaAtual.PInicial.x - Deltas[X]) * Escala[X],(AlturaVideo - (SecaoAvulsaAtual.PInicial.y - Deltas[Y])) * Escala[Y]);
      pDC->LineTo((SecaoAvulsaAtual.PFinal.x - Deltas[X]) * Escala[X],(AlturaVideo - ( SecaoAvulsaAtual.PFinal.y - Deltas[Y])) * Escala[Y]);

      if (CircularSecoes)
      {
        Ppixels.x = ((SecaoAvulsaAtual.PEstaca.x - Deltas[X]) * Escala[X]);
        Ppixels.y = (AlturaVideo - (SecaoAvulsaAtual.PEstaca.y - Deltas[Y])) * Escala[Y] ;

        pDC->Arc(Ppixels.x-3,Ppixels.y-3,Ppixels.x+3,Ppixels.y+3,0,0,0,0);   	
      }
    }
  }

  if(DefinindoLinhaBase > 1) DesenharCruzPonto(PontosLinhaBase[0],pDC);
  if(DefinindoLinhaBase > 2) DesenharCruzPonto(PontosLinhaBase[1],pDC);

  pDC->SelectObject(pPenaAnterior);
}

/////////////////////////////////////////////////////////////////////////////
// DesPontosGeomView diagnostics

#ifdef _DEBUG
void DesPontosGeomView::AssertValid() const
{
  CView::AssertValid();
}

void DesPontosGeomView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// DesPontosGeomView message handlers

void DesPontosGeomView::OnMouseMove(UINT nFlags, CPoint point) 
{
  static CPoint PMouseAtual(0,0);

  SetCursor(LoadCursor(NULL,CursorAtual));

  if(DefinindoLinhaBase > 0)
  {
    POINT PonScreenCoor = {0,0}; 
    ClientToScreen(&PonScreenCoor); //--- coordenadas da janela de descri��o

    if(DefinindoLinhaBase == 1) DPopUpPonto.MostraDescricao(point.x,point.y,std::string(" Escolha o primeiro ponto da linha base "));
    else if(DefinindoLinhaBase == 2) DPopUpPonto.MostraDescricao(point.x,point.y,std::string(" Escolha o segundo ponto da linha base "));
  }

  if ((DesenhandoObjetos && PegouPrimPonto) || CriandoRestricao || (PegandoPontoSecAvulsa && PegouPrimPonto) || InserindoPonto || TrocandoObjeto || InserindoLembrete || MovendoLembrete || DefinindoLinhaBase > 1) 
  {
    //---Se esta inserindo ponto e o mouse passou per cima de uma restri��o, marca a restri��o

    if(InserindoPonto)
    {
      bool EstadoAnterior(ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end());

      ItLLDeltaSupMarcada = VerificaNaTangenteRestricoes(Superficie.PegaListaRestricoes().begin(),Ponto(MouseXYGeo->m_DOCoorX,MouseXYGeo->m_DOCoorY,INFINITO),Toleranciax,&ItCLDS,&Raio);     

      bool EstadoAtual(ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end());

      if(EstadoAnterior != EstadoAtual) 
      {
        RedrawWindow();
        DesenharApagar(OBJ_PONTO,PonMousePixelsAtual,PonMousePixelsAtual,NULL,OPR_APAGAR);
      }
    }

    static CPoint PAnterior(PonMousePixelsAtual);

    if(InserindoLembrete || MovendoLembrete)
    {
      RedrawWindow();
      DesenharApagar(OBJ_IMGBITMAPFLAG,PAnterior,PonMousePixelsAtual,NULL,OPR_APAGAR);

      PAnterior = PonMousePixelsAtual;
    }
    else 
    {
      if(InserindoPonto || TrocandoObjeto || DesenhandoObjetos || DefinindoLinhaBase > 1)
      {
        if(DefinindoLinhaBase > 1)
        {
          DesenharApagar(OBJ_IMGBITMAPSETA,PAnterior,PonMousePixelsAtual,NULL,OPR_APAGAR);

          PAnterior = PonMousePixelsAtual;
        }

        DesenharApagar(InserindoPonto? OBJ_PONTO : OBJ_LINHA,PonMousePixelsAtual,point);
        if(PrimeiroMovimento)
        {
          DesenharApagar(InserindoPonto? OBJ_PONTO : OBJ_LINHA,PonMousePixelsAtual,PonMousePixelsAtual,NULL,OPR_APAGAR);
          PrimeiroMovimento = false;
        }

        if(TrocandoObjeto && ObjetoAnterior != ObjetoAtual) 
        {
          if(ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end())
          {
            Superficie.MudaTipoAciTopografico(ItLLDeltaSupMarcada,ObjetoAnterior,ObjetoAtual);
            TrocandoObjeto = false;
            ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();
          }

          RedrawWindow();

          return;
        }
      }
    }
  }

  if(PMouseAtual == point) return;

  PonMousePixelsAtual = PMouseAtual = point;

  //--- Se estiver executando um zoom, desenha o retangulo e sai

  if(EZoom)
  {
    FimZoom = point;
    RedrawWindow();

    return;
  }

  TransfPixelsXY(point,MouseXYGeo->m_DOCoorX,MouseXYGeo->m_DOCoorY);  //--- Pega as coordenadas (em metros) 
  MouseXYGeo->UpdateData(FALSE);      //--- Mostra as coordenadas de onde o mouse est�.
  MouseXYGeo->RedrawWindow();

  //--- Se estiver criando uma restri��o ou marcando uma se��o avulsa mostra o el�stico 
  //--- Redesenha apenas o elastico

  POINT PonScreenCoor = {point.x+10,point.y}; 
  ClientToScreen(&PonScreenCoor); //--- coordenadas da janela de descri��o

  Ponto PSobMouse(MouseXYGeo->m_DOCoorX,MouseXYGeo->m_DOCoorY,0.00);
  if(Superficie.EstaRotacionada()) PSobMouse.RotacionaPlano(Superficie.PegaSeno(),Superficie.PegaCoseno(),Superficie.PegaAnguloRotacao());

  //--- Estas variaveis (ArrastandoPI,ArrastandoCC) se fazem necess�rias porque quando o mouse der pulos
  //--- Maiores que a toler�ncia de aproxima��o do ponto (toler�ncias x e y) o ponto n�o 
  //--- seria mais encontrado nas chamadas a PosicionaPonto , fazendo-o parar  de ser
  //--- arrastado no meio da opera��o. Estas vari�veis asseguram o arrasto do ponto
  //--- Independentemente dos saltos do mouse, ela apenas retorna a false , no final da 
  //--- opera��o, no m�todo LPbuttonUP.

  if (ArrastandoPI || ArrastandoCC)    //--- Esta Arrastando um ponto?
  {
    if(!NaoPermitirAlteracoes)
    {
      //--- Esta arrastando um PI ou CC

      DesenharApagar(OBJ_CURVA,PAntMouse,point,ItCurHorizSendoArrastadaPI != mapCurvasHorizontaisPI.end() ? ItCurHorizSendoArrastadaPI->second : ItCurHorizSendoArrastadaCC->second,OPR_APAGAR,PrimeiroMovimento);

      PrimeiroMovimento = false;

      int MovimentoValido;

      if(ItCurHorizSendoArrastadaPI != mapCurvasHorizontaisPI.end())
      {
        MovimentoValido = CurvasHorizontais.AlteraCurvaPI(ItCurHorizSendoArrastadaPI,PSobMouse,ItUltimaCurvaAjustadaTS,NULL);
        if (MovimentoValido == CCurHorizontais::CURVA_OK && MostrarDescricao) DPopUpPonto.MostraDescricao(PonScreenCoor.x,PonScreenCoor.y,PreparaDescricao(ItCurHorizSendoArrastadaPI->second,true,OBJ_PI));
      }
      else
      {
        MovimentoValido = CurvasHorizontais.AlteraCurvaCC(ItCurHorizSendoArrastadaCC,PSobMouse,ItUltimaCurvaAjustadaTS);
        if (MovimentoValido == CCurHorizontais::CURVA_OK && MostrarDescricao) DPopUpPonto.MostraDescricao(PonScreenCoor.x,PonScreenCoor.y,PreparaDescricao(ItCurHorizSendoArrastadaCC->second,true,OBJ_CC));
      }

      DesenharApagar(OBJ_CURVA,PAntMouse,point,ItCurHorizSendoArrastadaPI != mapCurvasHorizontaisPI.end() ? ItCurHorizSendoArrastadaPI->second : ItCurHorizSendoArrastadaCC->second,OPR_DESENHAR);

      AlterouProjeto = true;

      PAntMouse = point;

      RedrawWindow();

      return;
    }
    else 
    {
      InicioArrast = point;   //--- N�o est� permitindo alterar ent�o arrasta o desenho
      if(PermitirArrastar) Arrastando = true;   //--- O sentido esta invertido
      ArrastandoPI = ArrastandoCC = false;
    }
  }
  else     //--- N�o esta arrastando um ponto, ent�o se estiver arrastando o desenho.
  {
    if (Arrastando)
    {
      Deltas[X] -= (point.x - InicioArrast.x) / Escala[X] ;  //--- As coordenadas x s�o espelhadas...
      Deltas[Y] += (point.y - InicioArrast.y) / Escala[Y];    

      OnArrastouDesenho();

      if (AnguloRotacao != 0.0) DeltasReais.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),-AnguloRotacao);

      if (CriandoRestricao) UltimoPontoPego += point - InicioArrast;

      InicioArrast = point;

      RedrawWindow();

      return;
    }
    else   //--- N�o est� arrastando nada...
    {
      if(MostrarDescricao)
      {
        CurHorizontal CurvaVirtual;
        bool SobreAlgumPonto(false);

        if (PegandoPontoSecAvulsa) 
          DPopUpPonto.MostraDescricao(PonScreenCoor.x,PonScreenCoor.y,PreparaDescricao(&PSobMouse,false,OBJ_PONSECAVULSA));
        else
        {
          static bool SobrePontoDesabilitado(false);

          //--- Verifica se o mouse est� proximo de um ponto de terreno, de um PI, CC ou em uma se��o, se estiver mostra os dados
          //---  A FAZER : ====>Embutir isso tudo nas classe de superficie

          if ((DesenharPontos && VarrerTerreno))       //--- Se os pontos de  terreno estiverem desenhados
          {
            //--- Se o mouse estiver sob0re algum ponto de terreno mostra as coordenadas do ponto.
            //--- Caso contr�rio, limpa o dialogo do ponto , caso ele n�o esteja limpo.

            //    ItSSetPontos ItPontoDesabilitado(Superficie.SetPonDesabilitados().end());

            ItSSuperficie DeltaSupSobMouse(PegaPontoMaisProximo(PSobMouse,&ItPontoDesabilitado));

            if (DeltaSupSobMouse != Superficie.pSuperficieAtual->end()  &&  (DesenharPontosInseridos || DeltaSupSobMouse->PegaPCentral().Descricao.Find("Ponto Inserido") == -1) && 
              (DeltaSupSobMouse->Status == CDeltaSuperficie::HABILITADO || DeltaSupSobMouse->Status == CDeltaSuperficie::INSERIDO_USUARIO))
            {
              SobrePontoDesabilitado = false;

              const Ponto& PonSobMouse((*DeltaSupSobMouse).PegaPCentral());

              if(!PegouPonto) 
              {
                Ima(nFlags,PonSobMouse);
                PegouPonto = true;
              }

              if (DeltaSupAnterior == Superficie.pSuperficieAtual->end() || DeltaSupAnterior != DeltaSupSobMouse) 
              { 
                if(Superficie.EstaRotacionada()) 
                {
                  Ponto PontoDesrotacionado(PonSobMouse);

                  PontoDesrotacionado.RotacionaPlano (sin(-Superficie.PegaAnguloRotacao()),cos(-Superficie.PegaAnguloRotacao()),-Superficie.PegaAnguloRotacao());
                  DPopUpPonto.MostraDescricao(PonScreenCoor.x,PonScreenCoor.y,PreparaDescricao(&PontoDesrotacionado,false,PegaTipoPonto(PonSobMouse)));
                }
                else DPopUpPonto.MostraDescricao(PonScreenCoor.x,PonScreenCoor.y,PreparaDescricao(&PonSobMouse,false,PegaTipoPonto(PonSobMouse)));

                DeltaSupAnterior = DeltaSupSobMouse;  //--- Atualiza a anterior
              }
              SobreAlgumPonto = true;
            }
            else 
            {
              static Ponto PDesabilitadoAnterior;
              //--- Verifica se o ponto esta no set de pontos desabilitados, se estiver apenas mostra os dados

              if(ItPontoDesabilitado != Superficie.SetPonDesabilitados().end())
              { 
                if(PDesabilitadoAnterior != *ItPontoDesabilitado)
                {
                  DPopUpPonto.MostraDescricao(PonScreenCoor.x,PonScreenCoor.y,PreparaDescricao(&*ItPontoDesabilitado,false,OBJ_PONDESABILITADO));
                  PDesabilitadoAnterior = *ItPontoDesabilitado;
                  SobrePontoDesabilitado = true;
                }
              }
              else
              {
                PDesabilitadoAnterior = Ponto();
                SobrePontoDesabilitado = false;
              }

              DeltaSupAnterior = Superficie.pSuperficieAtual->end();        //--- Se saiu do ponto limpa o anterior, para se voltar nele mostrar o dialogo novamente
              PegouPonto = false;
            }
          }

          //=== Verifica se o mouse est� sobre um lembrete

          if(!MovendoLembrete)
          {
            ItLembrete = PegaLembreteMaisProximo(PSobMouse,Toleranciax*2.0);

            if(ItLembrete != SetLembretes.end()) 
            {
              DPopUpPonto.MostraDescricao(PonScreenCoor.x,PonScreenCoor.y,PreparaDescricao(&ItLembrete,false,OBJ_LEMBRETE)); 
              return;
            }
          }

          //--- Verifica se o mouse est� proximo de um ponto de PI ou CC, se estiver mostra os dados 
          //--- ==>Embutir isso tudo na classe de curvas horizontais

          if (!SobreAlgumPonto && VarrerProjeto)
          {
            CurvaVirtual.PI = PSobMouse;
            ItCurHorizSendoArrastadaPI = PegaPIMaisProximo(CurvaVirtual);

            if(ItCurHorizSendoArrastadaPI != mapCurvasHorizontaisPI.end())
            {
              SobreAlgumPonto = true;
              const Ponto& PonSobMouse(ItCurHorizSendoArrastadaPI->second->PI);
              DPopUpPonto.MostraDescricao(PonScreenCoor.x,PonScreenCoor.y,PreparaDescricao(ItCurHorizSendoArrastadaPI->second,false,OBJ_PI)); 
            }
            else
            {
              CurvaVirtual.CC = PSobMouse;
              ItCurHorizSendoArrastadaCC = PegaCCMaisProximo(CurvaVirtual);

              if(ItCurHorizSendoArrastadaCC != mapCurvasHorizontaisCC.end())
              {
                SobreAlgumPonto = true;
                DPopUpPonto.MostraDescricao(PonScreenCoor.x,PonScreenCoor.y,PreparaDescricao(ItCurHorizSendoArrastadaCC->second,false,OBJ_CC));
              }
              else 
              {
                //--- Verifica se o mouse est� proximo de uma se��o, se estiver mostra os dados e sincroniza o desenho da se��o.
                //--- A FAZER : O ideal seria embutir isso tudo numa classe...

                //       DesSecaoView* pDesSecao((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->DesSecao);   //--- Para sincronizar o desenho da se��o

                if (!SobreAlgumPonto && CircularSecoes)
                {
                  Ponto PAnterior(PSobMouse),PPosterior(PSobMouse);

                  PAnterior.x -= Toleranciax;
                  PPosterior.x += Toleranciax;

                  itMSecaoGeomPonto itMSecaoAnterior(MapaSecoesPonto.upper_bound(&PAnterior));
                  itMSecaoGeomPonto itMSecaoPosterior(MapaSecoesPonto.upper_bound(&PPosterior));
                  itMSecaoGeomPonto itMaisPerto(MapaSecoesPonto.end());
                  // static itMSecaoGeomPonto itPontoAnterior(MapaSecoesPonto.end());         // Esta ocorrendo erro de iteradores incompatives s� pq este � statico....veja logo a frente itPontoAnterior == MapaSecoesPonto.end()
                  static Ponto PontoAnterior;

                  double MenorDistancia(INFINITO),DistanciaAtual;

                  for(itMSecaoGeomPonto ItMapSecatual = itMSecaoAnterior ; ItMapSecatual != itMSecaoPosterior ; ItMapSecatual++)
                  {
                    DistanciaAtual =  pow(ItMapSecatual->first->x - PSobMouse.x,2) + pow(ItMapSecatual->first->y - PSobMouse.y,2); 

                    if (DistanciaAtual < MenorDistancia) 
                    {
                      MenorDistancia = DistanciaAtual;
                      itMaisPerto = ItMapSecatual;
                    }             
                  }
                  if (MenorDistancia < pow(Toleranciax,2))    //--- Se estiver proximo a um ponto
                  {
                    SobreAlgumPonto = true;  

                    itMSecaoGeomPonto itPAnt = MapaSecoesPonto.end();     

                    //   if(itPontoAnterior == MapaSecoesPonto.end() || *itPontoAnterior != *itMaisPerto)    ///itPontoAnterior == MapaSecoesPonto.end() iteradores incompativeis????
                    //--- se n�o estiver sobre o  mesmo ponto mostra a descri��o
                    if(PontoAnterior != itMaisPerto->first)
                    {
                      //--- Sincroniza o desenho da se��o. 

                      if(SincronizarSecoes)
                      {
                        CEstaca Estaca(itMaisPerto->second->EstVirtual);

                        DesSecaoView* pDesSecao((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->DesSecao);   //--- Para sincronizar o desenho da se��o
                        DesPerfilView* pDesPerfil((DesPerfilView*)((CMainFrame*)AfxGetMainWnd())->DesPerfil);

                        if(pDesPerfil) pDesPerfil->CalculaCota(Estaca);
                        if(pDesSecao) pDesSecao->Posiciona(Estaca.EstVirtual,Estaca.Cota);
                      }
                      //--- Mostra a descri��o 

                      DPopUpPonto.MostraDescricao(PonScreenCoor.x,PonScreenCoor.y,PreparaDescricao(&itMaisPerto,false,OBJ_SECAO));

                      // itPontoAnterior = itMaisPerto; 
                      PontoAnterior = itMaisPerto->first;
                    }
                    //else itPontoAnterior = MapaSecoesPonto.end();
                    else PontoAnterior.x = PontoAnterior.y = PontoAnterior.z = INFINITO;
                    //     }
                  }
                }

                if(!SobreAlgumPonto && CircularSecoes)   //--- Procura nas secoes avulsas
                {
                  Ponto PAnterior(PSobMouse),PPosterior(PSobMouse);

                  PAnterior.x -= Toleranciax;
                  PPosterior.x += Toleranciax;

                  itmapSecoesAvulsas itMSecaoAvAnterior = SecoesAvulsas.UpperBound(&PAnterior);
                  itmapSecoesAvulsas itMSecaoAvPosterior = SecoesAvulsas.UpperBound(&PPosterior);

                  itmapSecoesAvulsas itSecAvMaisPerto =  SecoesAvulsas.End();
                  static itmapSecoesAvulsas itPonSecAvAnterior = SecoesAvulsas.End();         

                  double MenorDistancia(INFINITO),DistanciaAtual;

                  for(itmapSecoesAvulsas ItMapSecAvAtual = itMSecaoAvAnterior ; ItMapSecAvAtual != itMSecaoAvPosterior ; ItMapSecAvAtual++)
                  {
                    DistanciaAtual =  pow((*ItMapSecAvAtual).first.x - PSobMouse.x,2) + pow((*ItMapSecAvAtual).first.y - PSobMouse.y,2); 

                    if (DistanciaAtual < MenorDistancia) 
                    {
                      MenorDistancia = DistanciaAtual;
                      itSecAvMaisPerto = ItMapSecAvAtual;
                    }             
                  }

                  if (MenorDistancia < pow(Toleranciax,2))    //--- Se estiver proximo a um ponto
                  {
                    SobreAlgumPonto = true;  

                    if(itPonSecAvAnterior != itSecAvMaisPerto)    //--- se n�o estiver sobre o  mesmo ponto mostra a descri��o
                    {
                      //--- Sincroniza o desenho da se��o. 

                      DesSecaoView* pDesSecao = (DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->DesSecao;   //--- Para sincronizar o desenho da se��o

                      if(pDesSecao) (*((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->DesSecao)).Posiciona(itSecAvMaisPerto->second.Estaca.EstVirtual,0.0,false,true);

                      //--- Mostra a descri��o da se��o avulsa

                      Ponto PTemp(itSecAvMaisPerto->first);
                      PTemp.Descricao = itSecAvMaisPerto->second.Estaca;

                      DPopUpPonto.MostraDescricao(PonScreenCoor.x,PonScreenCoor.y,PreparaDescricao(&PTemp,false,OBJ_SECAV));

                      itPonSecAvAnterior = itSecAvMaisPerto; 
                    }
                    else itPonSecAvAnterior = SecoesAvulsas.End();
                  }
                }

                if(!SobreAlgumPonto && DesenharCN && VarrerCN)
                {
                  static Ponto PCurNivAnterior(INFINITO,INFINITO,INFINITO);
                  Ponto PAnterior(PSobMouse),PPosterior(PSobMouse);

                  PAnterior.x -= Toleranciax;
                  PAnterior.y -= Toleranciax;
                  PPosterior.x += Toleranciax;
                  PPosterior.y += Toleranciax;

                  LPontos LPontosExtremos(1,PAnterior);
                  LPontosExtremos.push_back(PPosterior); 

                  ItSetItLPontos itSetPontoCNAnterior(Superficie.SetPontosCurvasNivel.upper_bound(LPontosExtremos.begin())),
                    itSetPontoCNPosterior(Superficie.SetPontosCurvasNivel.upper_bound(--LPontosExtremos.end())),
                    itSetPontoMaisPerto(Superficie.SetPontosCurvasNivel.end());

                  double MenorDistancia(INFINITO),DistanciaAtual;

                  for(ItSetItLPontos ItPonCNAtual = itSetPontoCNAnterior ; ItPonCNAtual != Superficie.SetPontosCurvasNivel.end() ; ItPonCNAtual++)
                  {
                    DistanciaAtual =  pow((*ItPonCNAtual)->x - PSobMouse.x,2) + pow((*ItPonCNAtual)->y - PSobMouse.y,2); 

                    if (DistanciaAtual < MenorDistancia) 
                    {
                      MenorDistancia = DistanciaAtual;
                      itSetPontoMaisPerto = ItPonCNAtual;
                    }             
                  }

                  if (MenorDistancia < pow(Toleranciax,2))    //--- Se estiver proximo a um ponto
                  {
                    SobreAlgumPonto = true;  

                    if(*(*itSetPontoMaisPerto) != PCurNivAnterior)
                    {
                      //--- Mostra a descri��o 
                      //  Ponto PontoDesrotacionado(*itSetPontoMaisPerto);

                      //    PontoDesrotacionado.RotacionaPlano (sin(-Superficie.PegaAnguloRotacao()),cos(-Superficie.PegaAnguloRotacao()),-Superficie.PegaAnguloRotacao());
                      //    DPopUpPonto.MostraDescricao(PonScreenCoor.x,PonScreenCoor.y,PreparaDescricao(&PontoDesrotacionado,false,OBJ_PTERRENO));

                      DPopUpPonto.MostraDescricao(PonScreenCoor.x,PonScreenCoor.y,PreparaDescricao(&(**itSetPontoMaisPerto),false,OBJ_CURNI));

                      PCurNivAnterior = *(*itSetPontoMaisPerto);
                    }
                    else PCurNivAnterior = Ponto(INFINITO,INFINITO,INFINITO);
                  }
                }
              }
            }
          }

          DesSecaoView* pDesSecao((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->DesSecao);   //--- Para sincronizar o desenho da se��o

          //--- Procura nas secoes avulsas

          if(!SobreAlgumPonto && DesenharSecoes)
          {
          }

          if(!SobreAlgumPonto && DPopUpPonto.IsWindowVisible() && pDesSecao) pDesSecao->Invalidate();
          if(!SobreAlgumPonto && !SobrePontoDesabilitado &&  DPopUpPonto.IsWindowVisible()) DPopUpPonto.ShowWindow(false);
        }
      }
    }
  }
}

void DesPontosGeomView::OnLButtonUp(UINT nFlags, CPoint point) 
{
  if (TerminarArrasto)     //--- Se estiver arrastando o desenho ou terminado o arrasto de um PI ou CC, termina o arrasto e desliga o timer
  {
    TerminarArrasto = ArrastandoPI = ArrastandoCC =  Arrastando = false;

    RedrawWindow();
  }

  if(!TerminarArrasto && Arrastando) TerminarArrasto = true;  //--- Segundo click do arrasto, sinaliza para terminar o arrasto

  CursorAtual = (ArrastandoPI || ArrastandoCC || DefinindoLinhaBase) ? (NaoPermitirAlteracoes && !DefinindoLinhaBase ? IDC_NO : IDC_CROSS) : (Arrastando ?  IDC_SIZEALL : IDC_HAND);
  SetCursor(LoadCursor(NULL,CursorAtual));
}

void DesPontosGeomView::OnRButtonUp(UINT nFlags, CPoint point) 
{
  //--- Se estiver em um zoom executa-o

  if (nFlags & MK_CONTROL && EZoom)
  {
    FimZoom = point;
    EZoom = false;

    //--- Zoom m�nimo 4 pixels em qualquer sentido

    if (abs(InicioZoom.x - FimZoom.x) > 4 && abs(InicioZoom.y - FimZoom.y) > 4) Zoom();

    return;
  }

  //--- Se estiver arrastando um ponto e for o segundo click (Terminar arrasto = true)
  //--- finaliza o arrasto

  if (TerminarArrasto) 
  {
    Arrastando = ArrastandoPI = ArrastandoCC = false;
    AlterouTerreno = true;                                      //--- houve Altera��o no greide (Ent�o gravar� o novo 
  }
  else 
  {
    if (!ArrastandoPI && !ArrastandoCC) 
    {
      if (Arrastando) Arrastando = false;	    //--- Se n�o estiver arrastando um ponto  p�ra de arrastar o desenho
      else                      //--- N�o est� arrastando nada n�o est� criando restri��o e clicou no fundo com o bot�o esquerdo, dialogo de configurar
      {
        if (CriandoRestricao) 
        {
          if(Superficie.PegaListaRestricoes().size() > 0 && Superficie.PegaListaRestricoes().rbegin()->size() == 1) 
          {
            if(VerfObjAvulso(ObjetoAtual) == false )
            {
              Superficie.RemoveListaDoMapa(--(Superficie.PegaListaRestricoes().end()));
              Superficie.PegaListaRestricoes().pop_back();
            }
          }

          //--- Os objetos foram concebidos muito depois dos bordos. Teoricamente os bordos s�o objetos 
          //--->>> A FAZER tranformar os bordos em objetos
          //--- Por enquanto fica com os 2 bordos e objetos:
          //--- Passa a lista de bordos para a lista de objetos se um objeto estiver sendo desenhado

          if (0 && DesenhandoObjetos)  //--- Aguardar para quando passas asrestri�oes e os bordos para Acidentes Topograficos
          {
            if(ObjetoAtual != CAciTopografico::NENHUM_ACITOPOG)   //--- Se ja selecionou o obj)
            {
              //--- Se estiver desenhando objetos a lista do objeto sendo desenhado esta na ultima lista de Restri��es.
              //--- Pega a lista e passa para a lista de objetos removendo a da lista de restri��es
              //--- A FAZER ==>>> Colocar as restri�oes e os bordos nos objetos - Teoricamente s�o iguais
              //--- Foi feito assim pq os objetos foram concebidos muito depois das restri��es

              LLDeltaSup&	ListasAtuais(Superficie.PegaListaRestricoes());

              if(ListasAtuais.size() > 0)
              {
                ListaAtual = --(ListasAtuais.end());

                for(register ItCLDeltaSup it = ListaAtual->begin() ; it != ListaAtual->end() ; ++it)
                {
                  AciTopogAtual.InserePonto(it->PegaPCentral());
                }

                ListasAtuais.pop_back();
                AciTopogAtual.MudaObjeto(ObjetoAtual);
                AcidentesTopograficos.Insere(AciTopogAtual);  
                AciTopogAtual.Limpa();
              }
              else
              {
                //--- logar erro
              }
            }
          }
          CriandoRestricao = false;
        }
        else
        {
          if(!CriandoRestricao) OnConfdes();
          /*
          if(DesenhandoObjetos)
          {
          AcidentesTopograficos.Insere(AciTopogAtual);  
          DesenhandoObjetos = false;
          }
          else OnConfdes();
          */
        }
      }
    }
  }
  RedrawWindow();
} 

void DesPontosGeomView::CalculaParametros()
{
  //--- Calcula a Largura e a Altura do video, que depende da resolucao e da escala
  //--- em pixels por metro.
  //--- Dependendo da resolu��o do video a escala deve variar, os valores default s�o:
  //---
  //--- 1024 x 768 -> 20 pixels por metro.
  //--- 640 X 480  -> 12,5 pixels por metro (proporcionalmente), todas as escalas
  //--- tem seu velor default proporcional ao de 1024 x 768, Esta escala varia
  //--- quando o usu�rio pressiona o bot�o + ou o bot�o - (Alargar ou encolher o desenho).

  LarguraVideo = Resolucao[X] / Escala[X];
  AlturaVideo = Resolucao[Y] / Escala[Y];
  AlturaVideoPixels = AlturaVideo * Escala[X];
  LarguraVideoPixels = LarguraVideo * Escala[Y];

  //--- A escala mudou, acerta as toler�ncias (6 pixels em qualquer eixo);

  Toleranciax = 6.0 / Escala[X];
  Toleranciay = 6.0 / Escala[Y];
}

void DesPontosGeomView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();

  CMainFrame* MainFrame((CMainFrame*) AfxGetMainWnd());
  std::string NomeArquivo(MainFrame->PegaProjetoAtual() + std::string(".ini"));

  //DiaPonGeom = MainFrame->DPonGeom;
  MouseXYGeo = (CXYGreide *) MainFrame->MouseXYGeo;
  MouseXYGeo->Geometrico = true;      //--- Este dialogo � usado no greide e no geometrico, seta para o geometrico

  //--- Le o estado anterior do desenho e atualiza-os 

  char Buffer[512] = {0};

  ::GetPrivateProfileString(_T("DadosDesenhos"),MainFrame->DesPerfil ? _T("DadosDesPontosGeoConfigComPerfil"):
    _T("DadosDesPontosGeoConfig") ,_T("1,1,1,1,1,1,1,1,1,1,100,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,0,1,0,1,0"),Buffer,511,NomeArquivo.c_str());
  sscanf(Buffer,"%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i",&DesenharPixel,&DesenharCirculo,&DesenharCruz,&DesenharSecoes,&DesenharArestas,&DesenharBordos,&DesenharRestricoes,&DesenharPontos,
    &MostrarDescricao,&DesenharMalha,&EquiMalha,&EquiCotas,&DesenharProjeto,&bAciTopog,&CircularSecoes,&MostrarToolBarSuperf,&MostrarToolBarGeom,&CotarMalha,&VarrerTerreno,&VarrerProjeto,&VarrerCN,&DesCNTopograficas,&DesenharCN,&CotarCN,&DefBordos,
    &SincronizarSecoes,&NaoPermitirAlteracoes,&SecoesIconizadas,&DesenharObjetos,&DesenhandoObjetos,&DesenharPontosInseridos,&ObjetoAtual,&PermitirArrastar,&SaltarCNs);

  CChildFrame* CF((CChildFrame*) GetParentFrame());

  CF->m_wndToolBarGre.GetToolBarCtrl().SetState(ID_BUTLOCK,NaoPermitirAlteracoes);
  CF->m_wndToolBarGre.GetToolBarCtrl().SetState(ID_BUTSINCRO,SincronizarSecoes);
  CF->m_wndToolBarGre.GetToolBarCtrl().SetState(ID_SECSEMPREVIS_ICONIZAR,SecoesIconizadas);
  CF->m_wndToolBarGre.GetToolBarCtrl().SetState(ID_BUTARRASTAR,PermitirArrastar);

  CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTBORDOS,DefBordos);
  CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTACITOPOG,!DefBordos);
  CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_DESENHAR,DesenharObjetos);
  CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTPONINS,DesenharPontosInseridos);

  CF->ShowControlBar(&CF->m_wndToolBarAciTopograficos,DesenharObjetos,false);

  typedef struct 
  {
    int Acidente,Botao;

  } stMapaAcidenteBotao;

  struct ltAciBotao 
  {  
    bool operator()(const stMapaAcidenteBotao M1, const stMapaAcidenteBotao M2) const 
    { 
      return M1.Acidente < M2.Acidente;
    }
  };

  stMapaAcidenteBotao AcidenteBotao[22] = {{CAciTopografico::NENHUM_ACITOPOG,-1},{CAciTopografico::FRONTEIRA,ID_DES_FRONTEIRA},{CAciTopografico::MURO,ID_DES_MURO},{CAciTopografico::CERCA,ID_DES_CERCA},{CAciTopografico::MEIO_FIO,ID_DES_MEIOFIO},
  {CAciTopografico::BORDO,ID_DES_BORDO},{CAciTopografico::N_A,ID_DES_NA},{CAciTopografico::PONTE,ID_DES_PONTE},{CAciTopografico::TAMPA_BUEIRO,ID_DES_TAMPA_BUEIRO},{CAciTopografico::POSTE,ID_DES_POSTE},
  {CAciTopografico::REDE_ELETRICA_BT,ID_DES_REDEELETRICA_BT},{CAciTopografico::REDE_ELETRICA_AT,ID_DES_REDEELETRICA_AT},{CAciTopografico::CASA,ID_DES_CASA},
  {CAciTopografico::ROCHA,ID_DES_ROCHA},{CAciTopografico::EIXO,ID_DES_EIXO},{CAciTopografico::CRISTA,ID_DES_CRISTA},{CAciTopografico::PE,ID_DES_PE},{CAciTopografico::CANTO_MURO,ID_DES_CANTO_MURO},
  {CAciTopografico::CANTO_CERCA,ID_DES_CANTO_CERCA},{CAciTopografico::CANTO_CASA,ID_DES_CANTOCASA},{CAciTopografico::BUEIRO,ID_DES_BUEIRO},{CAciTopografico::FIM_ACITPOG,-2}};

  std::set<stMapaAcidenteBotao,ltAciBotao> SetAcidenteBotao(AcidenteBotao,AcidenteBotao+22);
  const stMapaAcidenteBotao AciAtual = {ObjetoAtual,-1};
  std::set<stMapaAcidenteBotao,ltAciBotao>::iterator it(SetAcidenteBotao.find(AciAtual));

  if(it != SetAcidenteBotao.end())
    CF->m_wndToolBarAciTopograficos.GetToolBarCtrl().SetState(it->Botao,true);

  CString Default;

  Default.Format("%lf,%lf,1,%lf,%lf",Escala[X],Escala[Y],Deltas[X],Deltas[Y]);

  //::GetPrivateProfileString(_T("DadosDesenhos"),MainFrame->DesPerfil?_T("DadosDesPontosGeoComPerfil"):_T("DadosDesPontosGeo"),Default,Buffer,511,NomeArquivo.c_str()); 
  // sscanf(Buffer,"%lf,%lf,%lf,%lf,%lf",&Escala[X],&Escala[Y],&RazaoHV,&Deltas[X],&Deltas[Y]);

  if(((CMainFrame*) AfxGetMainWnd())->CalculouSuperficie())  //--- Tem que pegar o angulo de rota��o anterior...
  {
    //  if(Superficie.EstaRotacionada()) RotacionaPontoPlano(sin(-Superficie.PegaAnguloRotacao()),cos(-Superficie.PegaAnguloRotacao()),-Superficie.PegaAnguloRotacao(),&Deltas[X],&Deltas[Y]);
    ((CMainFrame*) AfxGetMainWnd())->CalculouSuperficie(false);

  }
  else
  {
    ::GetPrivateProfileString(_T("DadosDesenhos"),MainFrame->DesPerfil?_T("DadosDesPontosGeoComPerfil"):_T("DadosDesPontosGeo"),Default,Buffer,511,NomeArquivo.c_str()); 
    sscanf(Buffer,"%lf,%lf,%lf,%lf,%lf",&Escala[X],&Escala[Y],&RazaoHV,&Deltas[X],&Deltas[Y]);
  }



  //--- Pega as cores

  ::GetPrivateProfileString(_T("DadosDesenhos"),MainFrame->DesPerfil?_T("DadosDesPontosGeoCoresComPerfil"):_T("DadosDesPontosGeoCores"),
    _T("0,16777215,4227072,1942750,16711808,8388863,8454143,4227200,182,33023"),Buffer,511,NomeArquivo.c_str());
  _stscanf(Buffer,"%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu",&Cores[0],&Cores[1],&Cores[2],&Cores[3],&Cores[4],&Cores[5],&Cores[6],&Cores[7],&Cores[8],&Cores[9]);

  //--- Inicializa os valores da escala, da raz�o e da toler�ncia.

  CF->DeslizanteGeom.MudarEscala(Escala[X]);

  //--- Parametros do tamanho da janela

  RECT Coord; 
  GetClientRect(&Coord);

  Resolucao[X] = Coord.right; 
  Resolucao[Y] = Coord.bottom;

  GetWindowRect(CRectJanPai);    	//--- Salva As novas coordenadas para saber as posi��es do pop up

  DefineParametros();
  CalculaParametros();
  CalculaNovasDimensoes();
  OnAlterouAngulo(GetDC());

  DPopUpPonto.Create();     //--- dialogo popup do ponto

  if((Deltas[X] == 0.0 || Deltas[Y] == 0.0) && Superficie.PegaQuantPontos() > 0) OnEnquadrar();


  // CF->ShowControlBar(&CF->m_wndToolBoxDesenhar,false,false);

  Inicializou = true;
}

void DesPontosGeomView::OnSize(UINT nType, int cx, int cy) 
{
  if(cx == 0 || cy == 0 || !Inicializou) return;

  RECT Coord; 
  GetClientRect(&Coord);

  Resolucao[X] = Coord.right; 
  Resolucao[Y] = Coord.bottom;

  DefineParametros();
  CalculaParametros();
  CalculaNovasDimensoes();

  AlterouAngulo = true;
}

void DesPontosGeomView::MudarEscala(int Fator)
{
  double YCentral(AlturaVideo / 2.0 + (Deltas[Y])),
    XCentral(LarguraVideo / 2.0 + (Deltas[X]));

  //--- Acerta as escalas

  Escala[X] = Escala[Y] = Fator / 100.0;

  CalculaParametros();
  OnArrastouDesenho();

  //--- Mantem o ponto central no centro do video.

  Deltas[X] = XCentral - (LarguraVideo / 2.0); 
  Deltas[Y] = YCentral - (AlturaVideo / 2.0); 

  if(DeltaSupSobMouseAnterior != Superficie.pSuperficieAtual->end())
  {
    TransfXYPixels(DeltaSupSobMouseAnterior->PegaPCentral().x,DeltaSupSobMouseAnterior->PegaPCentral().y,UltimoPontoPego);
    DesenharApagar(OBJ_LINHA,UltimoPontoPego,PonMousePixelsAtual,NULL,OPR_APAGAR,TRUE);  //--- n�o adianta!!!
  }

  RedrawWindow();
}

void DesPontosGeomView::MudarRazao(double Fator)
{
  RazaoHV = Fator;
  Escala[X] = Escala[Y] * Fator;
  CalculaParametros();
  OnArrastouDesenho();
  RedrawWindow();
}

void DesPontosGeomView::OnLButtonDown(UINT nFlags, CPoint	point) 
{
  CircularPontoIntersecao = false;

  if(Arrastando)
  {
    if(PermitirArrastar) TerminarArrasto = Arrastando = true;
    return;	
  }

  if(SecoesIconizadas) IconizaSecao();

  bool ContinuarLista(false);

  //---	Verifica se	h� um	ponto	sob	o	mouse	se houver	e	estiver	editando acidentes topograficos
  //---	Se o click foi sobre um	ponto	e	se estiver criando um	acidente topogr�fico,	inclui o ponto na	lista, selele	j� n�o estiver nela	
  //---	Se o click foi sobre um	ponto	e	n�o	estiver	criando	o	acidente topografico inicia	uma	lista	de acidente	topografico. 

  Ponto	PSobMouse(MouseXYGeo->m_DOCoorX,MouseXYGeo->m_DOCoorY,0.00);

  //---	Se a superficie	estiver	rotacionada, rotaciona o ponto sob o mouse.

  if(Superficie.EstaRotacionada()) PSobMouse.RotacionaPlano(Superficie.PegaSeno(),Superficie.PegaCoseno(),Superficie.PegaAnguloRotacao());

  if(DefinindoLinhaBase != 0)
  {
    if(DefinindoLinhaBase == 1) 
    {
      PontosLinhaBase[0] = PSobMouse;
      DefinindoLinhaBase = 2;
      UltimoPontoPego = point;

      RedrawWindow();

      return;
    }
    else
    {
      PontosLinhaBase[1] = PSobMouse;
      DefinindoLinhaBase = 3;

      RedrawWindow();

      RotacionarLinhaBase();

      return;
    }
  }

  if(InserindoPonto)
  {
    if(Superficie.VerfPontoForaSuperficie(PSobMouse) == 0 || ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end())
    {
      CString CStrDescricao("Ponto Inserido Usu�rio: ");
      CString CStrDescUsuario;
      int intCalcCotaSuper(false);
      double Cota(0.0);

      CDPedeDescricao PedeDescricao(&CStrDescUsuario,&intCalcCotaSuper,&Cota);

      if(PedeDescricao.DoModal() == IDOK) 
      {
        if(intCalcCotaSuper != 0) Cota = 0.0;
        CStrDescricao += CStrDescUsuario;

        Ponto PInserir(PSobMouse);

        if( ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end()) InserePontoNaRestricaoMarcada(ItLLDeltaSupMarcada,PInserir,intCalcCotaSuper);
        CDeltaSuperficie DeltaSuperInserida(PInserir.x,PInserir.y,Cota,INFINITO,INFINITO,INFINITO,CStrDescricao,CAciTopografico::NENHUM_ACITOPOG,CDeltaSuperficie::INSERIDO_USUARIO,Superficie.ProxNumPonInsUsuario());
        Superficie.InsereDeltaSuperficie(DeltaSuperInserida);
        Superficie.InserePontoInseridoUsuario(DeltaSuperInserida.PegaPCentral());
      }

      InserindoPonto = false;
      AlterouTerreno = true;

      ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();

    }

    RedrawWindow();

    return;
  }

  if(InserindoLembrete)
  {
    std::string Titulo,Texto;
    COLORREF Cor;
    CDiaLembretes DiaLembretes(this,Titulo,Texto,Cor);

    if(DiaLembretes.DoModal() == IDOK) 
    {
      Ponto PCoorMetros;
      CPoint PMouse(point.x,point.y-13);

      TransfPixelsXY(PMouse,PCoorMetros.x,PCoorMetros.y);  //--- Pega as coordenadas (em metros) 
      SetLembretes.insert(CLembrete(CLembrete::DES_GEOMETRICO,PCoorMetros,Texto,Titulo,Cor));
    }

    InserindoLembrete = false;

    RedrawWindow();
    return; 
  }

  if(MovendoLembrete)
  {
     Ponto PCoorMetros;
     CPoint PMouse(point.x,point.y-13);

     TransfPixelsXY(PMouse,PCoorMetros.x,PCoorMetros.y);  

    CLembrete NovoLembrete(CLembrete::DES_GEOMETRICO,PCoorMetros,ItLembrete->Texto(),ItLembrete->Titulo());
    SetLembretes.erase(ItLembrete);
    ItLembrete = SetLembretes.insert(NovoLembrete).first; 

    MovendoLembrete = false;

    RedrawWindow();

    return;
  }

  if(ItLembrete != SetLembretes.end())
  {
    MovendoLembrete = true;
    return;
  }

  ItSSuperficie	DeltaSupSobMouse(PegaPontoMaisProximo(PSobMouse));

  //--- Se pegou um ponto inserido mas os pontos inseridos nao est�o desenhados anula o ponto pego

  if(DeltaSupSobMouse !=	Superficie.pSuperficieAtual->end() &&  DeltaSupSobMouse->PegaPCentral().Inserido == true && DesenharPontosInseridos == false) 
     DeltaSupSobMouse = Superficie.pSuperficieAtual->end();

   if(DesenharPontosInseridos == true && DeltaSupSobMouse != Superficie.pSuperficieAtual->end())
   {
    if(DesenhandoObjetos)
    {
      if(ObjetoAtual == CAciTopografico::NENHUM_ACITOPOG) 
      {
        AfxMessageBox("Selecione um objeto na caixa de ferramentas.");
        return;
      }
      if(DeltaSupSobMouse->PCentral.z == 0.00) 
      {
        AfxMessageBox("Pontos sem cota n�o podem ser usados para definir acidentes topogr�ficos\nDica:\n1 - Primeiro insira o ponto na superf�cie sem que conste de nenhuma restri��o. \n"
          "2 - Calcule a superf�cie (assim a cota do ponto ser� calculada).\n"
          "3 - Ent�o insira o ponto na restri��o desejada");
        return;
      }
    }
  }

  if(DesenhandoObjetos && ObjetoAtual != CAciTopografico::NENHUM_ACITOPOG && VarrerTerreno	&& DesenharPontos	&& !ArrastandoPI &&	!ArrastandoCC	&& DeltaSupSobMouse !=	Superficie.pSuperficieAtual->end())
  {
    if(DeltaSupSobMouse->PegaPCentral().Inserido != true)
    {
      LLDeltaSup&	ListasAtuais(Superficie.PegaListaRestricoes());
      LDeltaSup	LDeltaSupAtual;
      ItLDeltaSup	itPontoRestricao;

      //--- Teoricamente as restri�oes sao um tipo de objeto, est�o separadas dos objetos porque desenhar objetos foi 
      //--- Incluido muito depois. A FAZER ===>>>> INCLUIR AS  RESTRI��ES NOS OBJETOS ou vice versa.....

      if(0 && DesenhandoObjetos)
      {
        if(ObjetoAtual != CAciTopografico::NENHUM_ACITOPOG)   //--- Se ja selecionou o obj
        {
          if(AciTopogAtual.PegaLista().size() == 0)
          {
            AciTopogAtual.Limpa();
            AciTopogAtual.MudaObjeto(ObjetoAtual);
            PegouPrimPonto = AlterouProjeto = true;
          }

          AciTopogAtual.InserePonto(DeltaSupSobMouse->PegaPCentral());
        }
      }
      else if (!CriandoRestricao	&& !PegandoPontoSecAvulsa)	
      {
        for(ListaAtual = ListasAtuais.begin()	;	ListaAtual !=	ListasAtuais.end() ; ListaAtual++)
        {
          if ((itPontoRestricao	=	std::find((*ListaAtual).begin(),(*ListaAtual).end(),*DeltaSupSobMouse))	!= (*ListaAtual).end())	
          {
            int ObjetoPontoClicado(Superficie.VerfAciTopografico(ListaAtual));

            if(ObjetoAtual == ObjetoPontoClicado)    //--- Tem que esta no mesmo objeto
            {
              //---	Se a tecla control n�o estiver pressionada remove	o	ponto	da lista,	caso contr�rio marca o ponto de	quebra

              if((nFlags & MK_SHIFT))	 
              {
                RemovePontoRestricao(ListaAtual,itPontoRestricao);
                //   ListaAtual->remove(*DeltaSupSobMouse);
                const_cast<CDeltaSuperficie*>(&*DeltaSupSobMouse)->DeixouDeSerRestricao();
                if(ListaAtual->size()	== 0 || (ListaAtual->size()	== 1 && VerfObjAvulso(ObjetoAtual) == false))	
                {
                  Superficie.RemoveListaDoMapa(ListaAtual);
                  ListasAtuais.remove(*ListaAtual);			 //--- Se	a	lista	ficou	com	apena	um ponto remove-a
                }

                AlterouProjeto = AlterouTerreno = true;
                RedrawWindow();

                return;
              }
              else	
              {	
                if(nFlags & MK_CONTROL)
                {
                  //--- quebra a lista
                  QuebraLista(ListasAtuais,ListaAtual,itPontoRestricao);
                  ContinuarLista = CriandoRestricao = true;	
                }
                else
                  if(*itPontoRestricao	== *(*ListaAtual).rbegin() && *(*ListaAtual).rbegin() != *(*ListaAtual).begin())   //---e se for fim	da lista e a lista n�o estiver fechada continua	a	lista
                  {
                    //  if(VerfObjAvulso(ObjetoAtual) == false)   //-- Se for objeto avulso n�o h� lista para continuar
                    {
                      //    ItListaRestAtual = ListasAtuais.end();
                      CriandoRestricao = true;
                      ContinuarLista = true;     //--- Quebrou lista = continuar lista 

                    }
                  }
                  else
                  {
                    if (itPontoRestricao ==	(*ListaAtual).begin() && *(*ListaAtual).rbegin() != *(*ListaAtual).begin())  //---	Sen�o, se	for	o	inicio da	lista	e a lista n�o estiver fechada reverte	a	lista	para continuar do	fim	da lista
                    {
                      (*ListaAtual).reverse();
                      CriandoRestricao = true;
                      ContinuarLista = true;     //--- Quebrou lista = continuar lista 
                    }
                    else
                    {
                      //--- inicia uma nova lista (Esta partindo de um ponto de uma lista do mesmo tipo
                    }
                  }
              }
              if(0)// else   //--- A lista esta fechada
              {        
                //--- Se Estiver definindo restri�oes verifica se o usuario quer abrir a �rea ou iniciar outra.

                if(ObjetoAtual == CAciTopografico::FRONTEIRA)    
                {
                  int Resposta;
                  CDiaAbrirCriarArea DiaAbrirCriarArea(&Resposta,this);

                  DiaAbrirCriarArea.DoModal();

                  if(Resposta == -1)  CriandoRestricao = false;	
                  else
                  {
                    if(Resposta == 0)
                    {
                      ObjetoAtual = Superficie.VerfAciTopografico(ListaAtual);
                      QuebraLista(ListasAtuais,ListaAtual,itPontoRestricao);							 
                      ContinuarLista = CriandoRestricao	=	true;	
                    }
                    else ListaAtual = ListasAtuais.end();
                  }
                }
                else   //--- Se estiver definido bordos e a tecla CTRL estiver pressionada, quebra a lista
                {
                  ObjetoAtual = Superficie.VerfAciTopografico(ListaAtual);
                  QuebraLista(ListasAtuais,ListaAtual,itPontoRestricao);	
                  ListaAtual = --ListasAtuais.end();
                  ContinuarLista = true;	
                }
              }
              break;  //--- um ponto pode pertencer a mais de uma lista tem que procurar em todas
            }
            else
            {
              //--- Se o ponto atual estiver na fronteira marca que o ponto anterior foi clicado na fronteira. 
              //--- (Para verificar se a proxima aresta vai ficar fora da fronteira, o que n�o pode)
              //--- Guardar o iterador para facilitar a pesquisa na VerfArestaForaSuperficie     <<<=================

              if(ObjetoPontoClicado == CAciTopografico::FRONTEIRA)
              {
                bool PontoAnteriorFronteira = true;    //--- Por enquanto n�o est� sendo usado
              }
            }
          }	
        }

        //---	Se chegou	aqui � porque n�o clicou no fim ou no inicio de uma lista existente, ent�o inicia 
        //--- uma nova lista, mesmo que esteja em um ponto de um lista existente, se ele n�o quebrou uma lista

        //if (ListaAtual ==	ListasAtuais.end()/* || !ContinuarLista*/|| *ListaAtual->begin() == *ListaAtual->rbegin())
        if(!ContinuarLista)
        {
          //--- Pontos inseridos por Delaunay ou Gabriel n�o podem ser usados para criar restri��es.

          //  if(DeltaSupSobMouse->PegaPCentral().Descricao.Find("Ponto Inserido") == -1|| (DeltaSupSobMouse->PegaPCentral().Descricao.Find("Delaunay")== -1 && DeltaSupSobMouse->PegaPCentral().Descricao.Find("Gabriel") == -1))
          {
            DeltaSupSobMouseAnterior = DeltaSupSobMouse;
            ListasAtuais.push_back(LDeltaSup(1,*DeltaSupSobMouse));	
            ListaAtual = ListasAtuais.end();
            ListaAtual--;

            if(DesenhandoObjetos)
            {
              if (ObjetoAtual != CAciTopografico::NENHUM_ACITOPOG)    //--- Tem um objeto escolhido?     
              {
                Superficie.InsereListaNoMapa(ListaAtual,ObjetoAtual);
                //MMapItLLDeltaSup.insert(pairmmapitLLDeltaSup(ObjetoAtual,ListaAtual));   //--- mapeia os Acidentestopograficos -> lista de bordos 
                //--- por enquanto os acidentes topograficos s�o bordos <<<  ACERTAR!!! (Passar tudo para acidentes topograficos ... ou vice versa 
              }
              else                                    
              {
                //    ListasAtuais.erase(ListaAtual);
                //--- Mensagem "Escolha um Acid. topog.  a desenhar"
              }
            }

            CriandoRestricao = true;
            //PontoBordoAnterior = DeltaSupSobMouse;

            AlterouTerreno = true;
          }
          // else AfxMessageBox("Pontos inseridos por Delaunay ou Gabriel n�o podem ser usados \npara criar restri��es ou serem inclu�dos manualmente nelas.");
        }
      }
      else					//---	else est�	criando	restri��o	ou pegando ponto sec avulsa
      {
        //---	Se o ponto clicado for o in�cio	de alguma	lista	(exeto a atual), funde as	listas em	1	e	termina	a	edi��o do	bordo
        //---	Para continuar o usu�rio dever�	clicar no	final	de uma outra lista de	bordo		 

        ItLLDeltaSup itLAtual(ListasAtuais.begin());

        for( ; itLAtual	!= ListasAtuais.end()	&& (*itLAtual).begin() !=	(*itLAtual).end()	&&	(*(*itLAtual).begin()) !=	*DeltaSupSobMouse	;	++itLAtual)	;	

        //--- Se	achou	o	ponto	clicado	no inicio	de alguma	lista	funde-as,	se for a lista atual fechou	a	poligonal	dos	bordos
        //--- A lista tem que ser do mesmo tipo e n�o pode estar fechada

        if (itLAtual !=	ListasAtuais.end() && Superficie.VerfAciTopografico(itLAtual) == ObjetoAtual && *itLAtual->begin() != *itLAtual->rbegin()) 
        {
          unsigned int TipoRestricao(Superficie.VerfAciTopografico(itLAtual));

          if(VerfObjAvulso(ObjetoAtual) == false && 
            Superficie.VerfGeraArestaForcada(TipoRestricao) == true &&
            Superficie.VerificaIntersecaoAcidentesTopog((*itLAtual).begin()->PegaPCentral(),DeltaSupSobMouse->PegaPCentral(),PontoIntersecao) == true)
          {
            AfxMessageBox("Acidentes topogr�ficos que geram arestas for�adas n�o podem se interceptar");
            CircularPontoIntersecao = true;
            DeltaSupSobMouse = DeltaSupSobMouseAnterior;
          }
          else
            if (*itLAtual->begin() != *ListaAtual->begin()) 	//--- Um ponto pode ser o inicio e fim de mais de uma lista, porisso n�o compara os iteradores		
            {
              if(TipoRestricao == ObjetoAtual && *itLAtual->begin() != *itLAtual->rbegin())  //--- O tipo de objeto � o mesmo e se a lista n�o esta fechada, funde as 2 listas e termina a edicao
              {
                (*ListaAtual).insert((*ListaAtual).end(),(*itLAtual).begin(),(*itLAtual).end());
                Superficie.RemoveListaDoMapa(itLAtual,ObjetoAtual);
                ListasAtuais.erase(itLAtual);
                CriandoRestricao = false;
              }
              else 
              {
                (*ListaAtual).push_back(*DeltaSupSobMouse);	//--- Clicou no inicio de uma lista ou de outro tipo ou que esta fechada, simplesmente insere o ponto  	
              }
            }
            else   //--- Sen�o: � o inicio da lista que esta sendo criada, insere o ponto e fecha o poligono
            {
              if((*ListaAtual).size() > 2)
              {
                (*ListaAtual).push_back(*DeltaSupSobMouse);	
                CriandoRestricao = false;
              }
            }
        }
        else 
        {
          /*---	Verifica se	o	ponto	clicado	est� no	final	de alguma	lista, se	estiver	insere a lista ao	contrario	(do	fim	pro	inicio)
          e	termina	a	edi��o.
          Caso contrario simplesmente	insere o ponto na	lista		---*/

          for(itLAtual = ListasAtuais.begin()	;	itLAtual !=	ListasAtuais.end() &&	(*(*itLAtual).rbegin())	!= *DeltaSupSobMouse ; itLAtual++) ; 

          //--- clicou	no final de	uma	lista, soma	as duas, a segunda invertida,	do come�o	para oa	fim		
          //--- A lista n�o pode estar fechada e tem que ser do mesmo tipo 

          if(itLAtual	!= ListasAtuais.end() &&  Superficie.VerfAciTopografico(itLAtual) == ObjetoAtual && *itLAtual->begin() != *itLAtual->rbegin()) 
          {
            unsigned int TipoRestricao(Superficie.VerfAciTopografico(itLAtual));

            if(itLAtual	!= ListaAtual)  
            {
              if(Superficie.VerfGeraArestaForcada(TipoRestricao) == true &&
                Superficie.VerificaIntersecaoAcidentesTopog((*ListaAtual).rbegin()->PegaPCentral(),DeltaSupSobMouse->PegaPCentral(),PontoIntersecao) == true)
              {
                AfxMessageBox("Acidentes topogr�ficos que geram arestas for�adas n�o podem se interceptar.");
                CircularPontoIntersecao = true;
                DeltaSupSobMouse = DeltaSupSobMouseAnterior;
              }
              else
                if(TipoRestricao == ObjetoAtual)  //-- Se for o mesmo tipo de objeto, funde as listas
                {
                  (*ListaAtual).insert((*ListaAtual).end(),(*itLAtual).rbegin(),(*itLAtual).rend());
                  Superficie.RemoveListaDoMapa(itLAtual,ObjetoAtual);    
                  ListasAtuais.erase(itLAtual);

                  CriandoRestricao = false;
                }
                else     //--- Sen�o simplesmente insere o ponto na lista
                {
                  DeltaSupSobMouseAnterior = DeltaSupSobMouse;
                  (*ListaAtual).push_back(*DeltaSupSobMouse);											 //--- sen�o Insere	o	ponto	na lista q est�	sendo	criada
                }
            }
            else
            {
              if(Superficie.VerfGeraArestaForcada(TipoRestricao) == false ||
                Superficie.VerificaIntersecaoAcidentesTopog((*ListaAtual).rbegin()->PegaPCentral(),DeltaSupSobMouse->PegaPCentral(),PontoIntersecao) == false)
              {
                //--- Verifica se n�o � o mesmo ponto

                if(*(*ListaAtual).rbegin() != (*DeltaSupSobMouse))
                {
                  DeltaSupSobMouseAnterior = DeltaSupSobMouse;
                  (*ListaAtual).push_back(*DeltaSupSobMouse);											 //--- sen�o Insere	o	ponto	na lista q est�	sendo	criada
                  if (itLAtual !=	ListasAtuais.end())	CriandoRestricao = false;		 //--- inseriu o inicio	da lista,	termina	a	edi��o,	pois fechou	a	poligonal	dos	bordos
                }
              }
              else
              {
                AfxMessageBox("Acidentes topogr�ficos que geram arestas for�adas n�o podem se interceptar.");
                CircularPontoIntersecao = true;
                DeltaSupSobMouse = DeltaSupSobMouseAnterior;
              }
            }
          }
          else     //--- O ponto clicado n�o esta no inicio nem no fim de uma lista. (pode estar no meio de uma lista) simplesmente insere o ponto na lista
          {
            if(VerfObjAvulso(ObjetoAtual) == true ||
              Superficie.VerfGeraArestaForcada(ObjetoAtual) == false ||             
              Superficie.VerificaIntersecaoAcidentesTopog((*ListaAtual).rbegin()->PegaPCentral(),DeltaSupSobMouse->PegaPCentral(),PontoIntersecao) == false)
            {
              //--- A Aresta vai ser inserida na lista. Por �ltimo verifica se a aresta n�o vai ficar fora da superficie.

              if(Superficie.VerfArestaForaSuperficie((*ListaAtual).rbegin()->PegaPCentral(),DeltaSupSobMouse->PegaPCentral()) == false)
              {
                if(*(*ListaAtual).rbegin() != (*DeltaSupSobMouse) &&       //--- Verifica se n�o � o mesmo ponto atual ou o ponto anterior
                  ((*ListaAtual).size() == 1 || (*++(*ListaAtual).rbegin() != (*DeltaSupSobMouse))))   
                {
                  (*ListaAtual).push_back(*DeltaSupSobMouse);			                            //--- se OK Insere o ponto	na lista que est�	sendo	criada
                  if(VerfObjCanto(ObjetoAtual) == true) CriandoRestricao = false;              //--- A lista de cantos tem s� 2 pontos

                  DeltaSupSobMouseAnterior = DeltaSupSobMouse;
                }
                else DeltaSupSobMouse = DeltaSupSobMouseAnterior;
              }
              else
              {
                DeltaSupSobMouse = DeltaSupSobMouseAnterior;
                AfxMessageBox("N�o podem ocorrer acidentes topogr�ficos fora da superf�cie.");
              }
            }
            else
            {
              AfxMessageBox("Acidentes topogr�ficos que geram arestas for�adas n�o podem se interceptar.");
              CircularPontoIntersecao = true;
              DeltaSupSobMouse = DeltaSupSobMouseAnterior;
            }
          }
        }
      }

      RedrawWindow();
      AlterouTerreno = true;

      if(DeltaSupSobMouse != Superficie.pSuperficieAtual->end()) SalvaUltimoPontoResPixels(DeltaSupSobMouse);				//---	Salva	o	ultimo ponto pego	para poder desenhar	o	el�stico
    }
    else 
    {
      if(DesenharPontosInseridos == true)
        AfxMessageBox("Pontos inseridos por n�o podem ser usados para criar\n restri��es ou serem inclu�dos manualmente nelas.");
    }
  }
  else //--- n�o est�	sobre	um ponto de	terreno ou nao esta desenhando objetos (restri�oes)
  {		 
    //   if(!CriandoRestricao)   //Se chegar aqui e estiver criando Restricoes � porque clicou no fundo, termina de criar a restricao atual(CriandoRestricao = false no else)
    {
      //---	Se clicou	com	o	bot�o	direito	no centro	da curva ou	no PI, mostra	o	dialogo	para editar	os dados da	curva, 
      //---	Se o mouse estiver sobre o pi, pega-o	e	busca	a	curva	no set de	CC,	para evitar	dobrar o c�digo	pois
      //---	a	compara��o � feita com o iterador	para o set de	CC,	como a opera��o	�	rara e lenta n�o tem problema

      //---	Verifica se	est� sobre um	PI ou	um CC

      ItCurHorizSendoArrastadaPI = PegaPIMaisProximo(CurHorizontal(INFINITO,PSobMouse.x,PSobMouse.y));

      if (ItCurHorizSendoArrastadaPI ==	mapCurvasHorizontaisPI.end())	
      {
        ItCurHorizSendoArrastadaCC = PegaCCMaisProximo(CurHorizontal(INFINITO,PSobMouse.x,PSobMouse.y));	

        //---	Se estiver com o control pressionado o usu�rio deseja	editar a curva ent�o pega	a	curva	pelo pi
        //---	apesar de	o	mouse	estar	sobre	o	centro

        if ((nFlags	&	MK_CONTROL)	&& ItCurHorizSendoArrastadaCC	!= mapCurvasHorizontaisCC.end()) 
          ItCurHorizSendoArrastadaPI =	PegaPIMaisProximo(CurHorizontal(ItCurHorizSendoArrastadaCC->second->PI.x,ItCurHorizSendoArrastadaCC->second->PI.y));
      }

      //---	O	primeiro click em	cima de	um ponto inicia	o	arrasto, no	segundo	click
      //---	finaliza o arrasto,	no primeiro	click	ArrastandoPI fica	true at� que o usu�rio click novamente
      //---	quando Terminararrasto fica	true,	o	arrasto	termina	efetivamente quando	o	usu�rio	tira o dedo	do 
      //---	bot�o	do mouse no	m�todo onlbuttonup.	

      //---	Se j�	estiver	arrastando coloca	true no	terminararrasto	para que o arrasto possa ser terminado (no m�todo	onlbuttonup) e retorna
      //---	n�o	h� nada	mais a fazer.	

      if (!ArrastandoPI)
      {	
        if(ItCurHorizSendoArrastadaPI	!= mapCurvasHorizontaisPI.end())		//---	Est� sobre algum ponto de	projeto?
        {
          if (nFlags & MK_CONTROL)
          {
            CurHorizontal	CurvaEditada(*ItCurHorizSendoArrastadaPI->second),CurvaSalva(*ItCurHorizSendoArrastadaPI->second);										

            if(Superficie.EstaRotacionada()) CurvaEditada.PI.RotacionaPlano(sin(-Superficie.PegaAnguloRotacao()),cos(-Superficie.PegaAnguloRotacao()),-Superficie.PegaAnguloRotacao());

            //--- Verifica se � o primeiro ou o ultima curva)

            ItCMCurHorizontaisTS itPrimUltimo(mapCurvasHorizontaisTS.find(ItCurHorizSendoArrastadaPI->second->EstTs));

            /*
            (NaoPermitirAlteracoes ? CDEditaCurhoriz::PROIBIDO_ALTERAR : 
            (itPrimUltimo == mapCurvasHorizontaisTS.begin() || itPrimUltimo == --mapCurvasHorizontaisTS.end()) ? CDEditaCurhoriz::PI_INICIAL_FINAL :
            CDEditaCurhoriz::PI_NORMAL);
            */

            CDEditaCurhoriz	DiaEditaCurvaHorizontal(this,&CurvaEditada,AnguloRotacao,NaoPermitirAlteracoes ? CDEditaCurhoriz::PROIBIDO_ALTERAR : (itPrimUltimo == mapCurvasHorizontaisTS.begin() || itPrimUltimo == --mapCurvasHorizontaisTS.end()) ? CDEditaCurhoriz::PI_INICIAL_FINAL : CDEditaCurhoriz::PI_NORMAL);
            if(Superficie.EstaRotacionada()) PSobMouse.RotacionaPlano(Superficie.PegaSeno(),Superficie.PegaCoseno(),Superficie.PegaAnguloRotacao());

            if (DiaEditaCurvaHorizontal.DoModal()	== IDOK)
            {
              ItCMCurHorizontaisPI ItCCurvaPI(mapCurvasHorizontaisPI.find(&ItCurHorizSendoArrastadaPI->second->PI));	

              if(CurvaEditada.PI !=	ItCurHorizSendoArrastadaPI->second->PI || CurvaEditada.Raio !=	ItCurHorizSendoArrastadaPI->second->Raio || CurvaEditada.Lc !=	ItCurHorizSendoArrastadaPI->second->Lc ||
                CurvaEditada.Se != ItCurHorizSendoArrastadaPI->second->Se || CurvaEditada.Sl != ItCurHorizSendoArrastadaPI->second->Sl)
              {
                CurvaEditada.K = CurvaEditada.Raio * CurvaEditada.Lc;
                CEstaca NovaEstTsCurAnterior;

                //--- Troca a curva anterior pela atual  

                bool Trocou(false);

                CurHorizontal CurvaRemovida(*ItCCurvaPI->second);
                int Resp(CurvasHorizontais.RemoveCurvaDosMapas(ItCCurvaPI,ItUltimaCurvaAjustadaTS,&NovaEstTsCurAnterior));

                if(Resp == true) 
                {
                  //--- No primeiro PI NovaEstTsCurAnterior fica com INFINITO

                  if(NovaEstTsCurAnterior.EstVirtual != INFINITO) CurvaEditada.EstTs = CurvaEditada.EstSc = CurvaEditada.EstCs = CurvaEditada.EstSt = CurvaEditada.EstPc = CurvaEditada.EstPt = NovaEstTsCurAnterior+0.005;
                  CurvasHorizontais.InsereCurvaNosMapas(CurvaEditada);
                  ItCurHorizSendoArrastadaPI = mapCurvasHorizontaisPI.find(&CurvaEditada.PI) ;

                  if(ItCurHorizSendoArrastadaPI == mapCurvasHorizontaisPI.end())
                  {
                     AfxMessageBox("Erro Irrecuper�vel, contate o suporte - ERRDESGEO-005"); 
                  }

                  //--- tudo se passa como se a curva tivesse sido alterada pelo PI

                  if((Resp = CurvasHorizontais.AlteraCurvaPI(ItCurHorizSendoArrastadaPI,Ponto(CurvaEditada.PI),ItUltimaCurvaAjustadaTS,&NovaEstTsCurAnterior)) != CCurHorizontais::CURVA_OK) 
                    monolog.mensagem(Resp," a altera��o n�o ser� efetuada.");	//---	Erro no	c�lculo	da curva
                  else Trocou = AlterouProjeto = true;
                }
                else monolog.mensagem(Resp," a altera��o n�o ser� efetuada.");	

                if(!Trocou)     //--- oops!!!!
                {
                  if(!CurvasHorizontais.RemoveCurvaDosMapas(ItCurHorizSendoArrastadaPI,ItUltimaCurvaAjustadaTS,&NovaEstTsCurAnterior))
                  {
                    AfxMessageBox("Erro Irrecuper�vel, contate o suporte - ERRDESGEO-001"); 
                  }
                  if (!CurvasHorizontais.InsereCurvaNosMapas(CurvaRemovida))
                  {
                    AfxMessageBox("Erro Irrecuper�vel, contate o suporte - ERRDESGEO-002"); 
                  }

                  ItCMCurHorizontaisPI itCurPI(mapCurvasHorizontaisPI.find(&CurvaRemovida.PI));

                  if(itCurPI == mapCurvasHorizontaisPI.end())
                  {
                    AfxMessageBox("Erro Irrecuper�vel, contate o suporte - ERRDESGEO-003"); 
                  }

                  if(CurvasHorizontais.AlteraCurvaPI(itCurPI,itCurPI->second->PI,ItUltimaCurvaAjustadaTS,&NovaEstTsCurAnterior) != CCurHorizontais::CURVA_OK) 
                  {
                    AfxMessageBox("Erro Irrecuper�vel, contate o suporte - ERRDESGEO-004"); 
                  }
                }
                RedrawWindow();
              }
            }
          }
          else 
          {
            //   if(!NaoPermitirAlteracoes)
            {
              BackCurvaSendoArrastada	=	*ItCurHorizSendoArrastadaPI->second;								 //--- Salva os	valores	originais	ca durva que esta	sendo	editada
              ArrastandoPI = true;
              if(NaoPermitirAlteracoes && PermitirArrastar) Arrastando = true;
            }
          }
        }
        else	 //--- n�o est�	sobre	um PI
        {	
          if(!ArrastandoCC)
          {
            if(/*!NaoPermitirAlteracoes	&&*/ ItCurHorizSendoArrastadaCC	!= mapCurvasHorizontaisCC.end())				//---	Est� sobre algum ponto CC?
            {
              BackCurvaSendoArrastada	=	*ItCurHorizSendoArrastadaCC->second;							//---	Salva	os valores originais ca	durva	que	esta sendo editada
              ArrastandoCC = true;
              if(NaoPermitirAlteracoes & PermitirArrastar) Arrastando = true;
            }
            else
            {
              //---	Clicou no	fundo, 
              //---	Ou Esta	iniciando	ou finalizando uma se��o avulsa	ou est�	inicialndo ou	finalizando	o	arrastando o desenho.
              //---	se estiver iniciando a se��o avulsa	pega i primeiro	ponto	sen�o	finaliza e pega	o	segundo	ponto.
              //---	Sen�o	se estiver arrastando	o	desenho	termina	o	arrasto, caso	contr�rio	inicia o arrasto do	desenho.

              if(PegandoPontoSecAvulsa)
              {
                if(!PegouPrimPonto)
                {
                  UltimoPontoPego	=	point;
                  TransfPixelsXY(point,PrimeiroPontoSecAvulsa.x,PrimeiroPontoSecAvulsa.y);	//---	Pega as	coordenadas	(em	metros)	

                  PegouPrimPonto = true;
                }
                else
                {
                  TransfPixelsXY(point,SegundoPontoSecAvulsa.x,SegundoPontoSecAvulsa.y); 
                  PegouPrimPonto = false;

                  CSecaoAvulsa SecAvTemp(CEstaca(),PrimeiroPontoSecAvulsa.x,PrimeiroPontoSecAvulsa.y,SegundoPontoSecAvulsa.x,SegundoPontoSecAvulsa.y,0.0,0.0,0.0,Ponto(INFINITO,INFINITO));
                  SecoesAvulsas.InsereOrdenada(SecAvTemp,true);
                  SecoesAvulsas.InsereOrdenadaArq(SecAvTemp,true);
                  AlterouSecoesAvulsas = true;

                  RedrawWindow();
                }
              }
              else
              {
                //--- Clicou no fundo. Se estiver criando algum canto cria o ponto onde o canto termina, e finaliza a lista, esta lista tem s� 2 pontos

                if(CriandoRestricao && VerfObjCanto(ObjetoAtual) == true)
                {
                  //      double Coordx(INFINITO),Coordy(INFINITO);
                  //  TransfPixelsXY(CPoint(PSobMouse.x,PSobMouse.y),Coordx,Coordx);

                  CDeltaSuperficie DeltaSuperInseridaUsuario(PSobMouse.x,PSobMouse.y,0.0,INFINITO,INFINITO,INFINITO,CString("Ponto Inserido Usu�rio - "),false,ObjetoAtual);

                  switch(ObjetoAtual)
                  {
                  case CAciTopografico::CANTO_CASA: DeltaSuperInseridaUsuario.PegaPCentralNC().Descricao += "Canto Casa.";break;
                  case CAciTopografico::CANTO_CERCA: DeltaSuperInseridaUsuario.PegaPCentralNC().Descricao += "Canto Cerca.";break;
                  case CAciTopografico::CANTO_MURO: DeltaSuperInseridaUsuario.PegaPCentralNC().Descricao += "Canto Muro.";break;
                  default : DeltaSuperInseridaUsuario.PegaPCentralNC().Descricao += "Desconhecido.";break;
                  }

                  //  DeltaSupSobMouse = Superficie.InsereDeltaSuperficie(DeltaSuperInseridaUsuario);
                  //  if(DeltaSupSobMouse != Superficie.pSuperficieAtual->end())
                  {
                    ListaAtual->push_back(DeltaSuperInseridaUsuario);
                  }
                  DeltaSupSobMouse = Superficie.pSuperficieAtual->end();
                  CriandoRestricao = false;
                  RedrawWindow();
                }
                else
                {
                  if(!TerminarArrasto && PermitirArrastar)
                  {
                    Ponto PAtual;

                    TransfPixelsXY(point,PAtual.x,PAtual.y);
                    ItCLLDeltaSup ItLRestricaoAtual(VerfClickRestricao(PAtual));

                    if(ItLRestricaoAtual != Superficie.PegaListaRestricoes().end())
                    {  
                      unsigned int TipoRestricao(Superficie.VerfAciTopografico(ItLRestricaoAtual));
                      
                      CPoint PAux(point);
                      ClientToScreen(&PAux);

                      DPopUpPonto.MostraDescricao(PAux.x,PAux.y-60,PreparaDescricao((void*)CAciTopografico::Nome(TipoRestricao),false,OBJ_TEXTO));

                    }
                    else
                    {
                      Arrastando = true;	 
                      InicioArrast = point;	
                    }
                  }
                  else Arrastando	=	false;
                }
              }
            }
          }
          else ArrastandoCC	=	false;
        }
      }
      else ArrastandoPI	=	false;
    }
    //  else CriandoRestricao = false;
  }

  //---	Se n�o estiver arrastando	PI ou	CC e n�o for o primeiro	movimento	�	porque este	foi	o	�ltimo movimento do	arrasto
  //---	Redesenha	a	janela e prepara a vari�vel	Primeiro movimento para	o	pr�ximo	arrasto.

  if(!ArrastandoPI &&	!ArrastandoCC) 
  {
    if(!PrimeiroMovimento) RedrawWindow();	 //--- Se	n�o	estiver	arrastando nada	e	n�o	for	o	primeiro movimento
    PrimeiroMovimento	=	true;	               //--- Prepara para o proximo primeiro movimento 
  }

  if(!CriandoRestricao) DeltaSupSobMouseAnterior = Superficie.pSuperficieAtual->end();
}

void DesPontosGeomView::OnEnquadrar() 
{
  //--- Passa as escalas e os deltas para o default

  Escala[X] = EscalaDefault[X];
  Escala[Y] = EscalaDefault[Y];

  DefineParametros();                   //--- Para calcular os deltas default

  DeltasReais.x = Deltas[X] = DeltasDefault[X];
  DeltasReais.y = Deltas[Y] = DeltasDefault[Y];

  RazaoHV = 1.0;

  OnArrastouDesenho();                  //--- Tudo se passa como se estivesse arrastado o desenho tb...

  //--- Reajusta a escala no deslizante

  ((CChildFrame*) GetParentFrame())->DeslizanteGeom.MudarEscala((int) (Escala[X]* 10.0));

  RedrawWindow();
}

//--- PASSAR ESTE METODO PARA A CLASSE SUPERFICIE
ItSSuperficie DesPontosGeomView::PegaPontoMaisProximo(Ponto PMouse,ItSSetPontos* pItPontoDesabMaisProximo)
{
  PMouse.x += Toleranciax;

  ItSSuperficie itDSupFinal(Superficie.pSuperficieAtual->upper_bound(PMouse)),itDSupAtual(Superficie.pSuperficieAtual->end()),itPonMaisProximo(Superficie.pSuperficieAtual->end());

  PMouse.x -= Toleranciax * 2.0;
  itDSupAtual = Superficie.pSuperficieAtual->upper_bound(PMouse);         //--- Pega o primeiro na tolerancia    
  PMouse.x += Toleranciax;                          //--- Restaura o valor original

  double MenorDistancia(INFINITO);

  //--- Separa o ponto que est� mais pr�ximo do mouse 

  while(itDSupAtual != itDSupFinal) 
  {
    Ponto PAtual((*itDSupAtual).PegaPCentral());
    double DistanciaAtual(hypot(PAtual.x - PMouse.x,PAtual.y - PMouse.y)); 
    if (DistanciaAtual < MenorDistancia) 
    {
      MenorDistancia = DistanciaAtual; 
      itPonMaisProximo = itDSupAtual;
    }

    itDSupAtual++;
  }

  //--- verifica nos pontos desabilitados

  double MenorDistanciaDes(INFINITO);
  ItSSetPontos itPontoMaisProximo(Superficie.SetPonDesabilitados().end());

  if(pItPontoDesabMaisProximo != NULL)
  {
    PMouse.x += Toleranciax;

    ItSSetPontos itDPontoFinal(Superficie.SetPonDesabilitados().upper_bound(PMouse)),itDPontoAtual(Superficie.SetPonDesabilitados().end());

    PMouse.x -= Toleranciax * 2.0;

    ItSSetPontos itDPontoDesAtual(Superficie.SetPonDesabilitados().upper_bound(PMouse));         //--- Pega o primeiro na tolerancia    

    PMouse.x += Toleranciax;                                                                   //--- Restaura o valor original

    //--- Separa o ponto que est� mais pr�ximo do mouse 

    while(itDPontoDesAtual != itDPontoFinal) 
    {
      Ponto PAtual(*itDPontoDesAtual);
      double DistanciaAtual = hypot(PAtual.x - PMouse.x,PAtual.y - PMouse.y); 
      if (DistanciaAtual < MenorDistanciaDes) 
      {
        MenorDistanciaDes = DistanciaAtual; 
        itPontoMaisProximo = itDPontoDesAtual;
      }

      itDPontoDesAtual++;
    }
  }


  //--- Separa a menor distancia entre os 2 set e retorna se ele estiver dentro da tolerancia 
  //--- caso contr�rio n�o h� ponto mais pr�ximo dentro da tolerancia

  if(MenorDistancia < MenorDistanciaDes) return MenorDistancia < Toleranciax ? itPonMaisProximo : Superficie.pSuperficieAtual->end();
  else if(pItPontoDesabMaisProximo != NULL) *pItPontoDesabMaisProximo = MenorDistanciaDes < Toleranciax ? itPontoMaisProximo : Superficie.SetPonDesabilitados().end();

  return Superficie.pSuperficieAtual->end();
}

void DesPontosGeomView::Zoom()    
{
  Ponto CantoInfEsq(InicioZoom.x < FimZoom.x ? InicioZoom.x : FimZoom.x,InicioZoom.y < FimZoom.y ? InicioZoom.y : FimZoom.y),
    CantoSupDir(InicioZoom.x > FimZoom.x ? InicioZoom.x : FimZoom.x,InicioZoom.y > FimZoom.y ? InicioZoom.y : FimZoom.y);

  double Escx = Escala[X],Escy = Escala[Y];

  //--- Ajusta os deltas e a escala para caber todo o zoom

  //--- O ponto inferior esquerdo da janela do zoom ser� o ponto inferior esquerdo da janela do video 

  Deltas[X] += CantoInfEsq.x / Escala[X];
  Deltas[Y] += AlturaVideo - (CantoSupDir.y / Escala[Y]);

  //--- Calcula as escala nos 2 sentidos e separa a menor pois ser� a 
  //--- usada para que toda a janela caiba no video

  Escala[X] =  Resolucao[X] / (fabs((double)FimZoom.x - InicioZoom.x) / Escx);
  Escala[Y] =  Resolucao[Y] / (fabs((double)FimZoom.y - InicioZoom.y) / Escy);

  Escala[X] < Escala[Y] ?	Escala[Y] = Escala[X] : Escala[X] = Escala[Y];

  //--- Reajusta a escala no deslizante

  ((CChildFrame*) GetParentFrame())->DeslizanteGeom.MudarEscala((Escala[X]));

  CalculaParametros();     //--- Recalcula a altura e a largura do v�deo
  OnArrastouDesenho();

  RedrawWindow();          //--- redesenha a janela com com zoom  
}

#include "DiaRemAciTopog.h"

void DesPontosGeomView::OnRButtonDown(UINT nFlags, CPoint point) 
{
  CPoint PDialogo(point);

  if(SecoesIconizadas) IconizaSecao();

  if (ArrastandoPI || ArrastandoCC)  return;         //--- Se estiver arrastando algum ponto retorna (� preciso promeiro terminar o arrasto)

  if(CriandoRestricao) 
  {
    if(Superficie.PegaListaRestricoes().rbegin()->size() == 1) 
    {
      if(VerfObjAvulso(ObjetoAtual) == false)
      {
        Superficie.RemoveListaDoMapa(--(Superficie.PegaListaRestricoes().end()));
        Superficie.PegaListaRestricoes().pop_back();
      }
    }
    return;
  }

  if ((nFlags & MK_CONTROL) && !EZoom)
  {
    EZoom = true;
    InicioZoom = FimZoom = point;
    return;
  }

  if (nFlags & MK_SHIFT)
  {     
    CMenu menu;
    if(menu.LoadMenu(IDR_MENU1))
    {
      CMenu* pPopup(menu.GetSubMenu(0));
      if(pPopup != NULL)
      {
        Ponto PSobMouse(MouseXYGeo->m_DOCoorX,MouseXYGeo->m_DOCoorY,0.00);
        if(Superficie.EstaRotacionada()) PSobMouse.RotacionaPlano(Superficie.PegaSeno(),Superficie.PegaCoseno(),Superficie.PegaAnguloRotacao());
        PegouPontoDesabilitado = PegouPonto = PegouPontoInserido = false;

        DeltaSupSobMousePopup = PegaPontoMaisProximo(PSobMouse,&ItPontoDesabilitado);

        if(DeltaSupSobMousePopup !=	Superficie.pSuperficieAtual->end()) 
        {
          if(DeltaSupSobMousePopup->PegaPCentral().Inserido == true) 
            PegouPontoInserido = true;
          else PegouPonto = true;
        }
        else 
        {
          if(ItPontoDesabilitado != Superficie.SetPonDesabilitados().end()) PegouPontoDesabilitado = true;
          else ItLLDeltaSupMarcada = VerificaNaTangenteRestricoes(Superficie.PegaListaRestricoes().begin(),PSobMouse,Toleranciax);     
        }

        CPoint PAux(point);
        ClientToScreen(&PAux);
        pPopup->TrackPopupMenu(TPM_TOPALIGN |TPM_LEFTALIGN , PAux.x+5, PAux.y+5, AfxGetMainWnd());

        return;
      }
    }
  }

  Ponto PAtual;

  TransfPixelsXY(point,PAtual.x,PAtual.y);  
  CRect Rect;
  GetWindowRect(&Rect);

  LLDeltaSup& ListasRestricoes(Superficie.PegaListaRestricoes());
  ItCLLDeltaSup ItLRestricaoAtual(ListasRestricoes.begin());
  PegouRestricao = false;

  while (ItLRestricaoAtual != ListasRestricoes.end())
  {
    if(Superficie.EstaRotacionada()) PAtual.RotacionaPlano(Superficie.PegaSeno(),Superficie.PegaCoseno(),Superficie.PegaAnguloRotacao());
    ItLLDeltaSupMarcada = VerificaNaTangenteRestricoes(ItLRestricaoAtual,PAtual,Toleranciax); 

    if(ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end())    
    {
      PegouRestricao = true;

      RedrawWindow();

      PDialogo.x =  point.x + 10 + Rect.left;
      PDialogo.y =  point.y + 20 + Rect.top;

      CMenu menu;
      if(menu.LoadMenu(IDR_MENU2))
      {
        CMenu* pPopup(menu.GetSubMenu(0));
        if(pPopup != NULL)
        {
          CPoint PAux(point);
          UltimoPontoPego = PAux;
          ClientToScreen(&PAux);

          pPopup->TrackPopupMenu(TPM_TOPALIGN |TPM_LEFTALIGN , PAux.x+5, PAux.y+5, AfxGetMainWnd());

          return;
        }
      }
    }
    ++ItLRestricaoAtual;
  }

  ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();

  RedrawWindow();
}               

void DesPontosGeomView::SalvaUltimoPontoResPixels(const ItSSuperficie DeltaSuperficie)
{
  Ponto PAtual = (*DeltaSuperficie).PegaPCentral();   //--- Pega o ponto atual

  UltimoPontoPego.x = ((PAtual.x - Deltas[X]) * Escala[X]);
  UltimoPontoPego.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y] ;
}

void DesPontosGeomView::DesenharApagar(int TipoObjeto,const CPoint& LocalAnterior,const CPoint& LocalAtual,void* Objeto,int OperacaoForcada,bool PrimeiroMovimento) 
{
  CDC* pDC(GetDC());
  if (!pDC) return;   //--- Seguran�a, aconte�eu nos testes

  CPen Pena;
  Pena.CreatePen(PS_SOLID,PrimeiroMovimento ? 2 : 1,Cores[CORPROJETO]);
  CPen* pPenaAnterior = NULL; 
  CPoint PontoAtual;

  //--- Apaga e redesenha apenas o objeto 

  for(int Operacao = OPR_APAGAR ; Operacao <= OPR_DESENHAR ; Operacao++)
  {
    //--- Se veio para fazer apenas uma opera��o pula a outra.

    if ((OperacaoForcada == OPR_APAGAR && Operacao != OPR_APAGAR) || (OperacaoForcada == OPR_DESENHAR && Operacao != OPR_DESENHAR)) continue;

    if (Operacao == OPR_DESENHAR) 
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
    case OBJ_PONTO :
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

    case OBJ_LINHA :
      {	
        pDC->MoveTo(UltimoPontoPego.x,UltimoPontoPego.y);
        pDC->LineTo(PontoAtual.x,PontoAtual.y);			
      }
      break;

    case OBJ_CURVA :
      {
        CurHorizontal* pCurHoriz = (CurHorizontal*) Objeto;

        ItCMCurHorizontaisTS itCHMeio = mapCurvasHorizontaisTS.find(pCurHoriz->EstTs), itCHAnterior = itCHMeio;
        ItCMCurHorizontaisTS itMCH = itCHAnterior != mapCurvasHorizontaisTS.begin() ? --itCHAnterior : itCHMeio;

        DesenhaProjeto(itMCH,pDC,OperacaoForcada,3); 

        //--- Redesenha a superf�cie envolvida que ficou deteriorada com o apagamento do projeto

        if( false && Operacao == OPR_APAGAR && PrimeiroMovimento)
        {
          pDC ->SetROP2(R2_COPYPEN); 

          //--- Pega o Limites da area deteriorada pelo redesenho das curva

          CDeltaSuperficie SuperInicial(itMCH->second.TS);         //--- Primeira curva envolvida na altera��o
          itMCH++;
          if(itMCH != mapCurvasHorizontaisTS.end()) itMCH++;  //--- Passa para a �ltima curva envolvida na altera��o

          //--- Se for  o ultimo pi que estiver sendo movido ((itSCH == end) o limite � no ST dele mesmo
          CDeltaSuperficie SuperFinal(itMCH !=  mapCurvasHorizontaisTS.end() ? itMCH->second.ST : itCHMeio->second.ST);   

          //--- como a superficie pode estar rotacionada as coordenadas podem estar negativas e na ordem do set o 
          //--- anterior seria na verdade o posterior. ent�o � preciso verificar esta possibilidade...

          ItSSuperficie itSuperAnterior(Superficie.pSuperficieAtual->upper_bound( SuperInicial.PegaPCentral() < SuperFinal.PegaPCentral() ? SuperInicial : SuperFinal)),
            itSuperPosterior(Superficie.pSuperficieAtual->upper_bound( SuperInicial.PegaPCentral() < SuperFinal.PegaPCentral() ? SuperFinal : SuperInicial));

          DesenhaSuperficie(itSuperAnterior,itSuperPosterior,pDC);
        }
        break;
      }
    case OBJ_IMGBITMAPFLAG:
      {
        CDC dcMemory;

        dcMemory.CreateCompatibleDC(pDC);

        dcMemory.SelectObject(BandeiraMaskBMP);
        pDC->BitBlt(LocalAtual.x,LocalAtual.y-14,16,21, &dcMemory, 0, 0, SRCCOPY);
      }
    }
  } 
  ReleaseDC(pDC);
}

void DesPontosGeomView::AcertaRestricoes(const CDeltaSuperficie* DeltaSupAAlterar,const CDeltaSuperficie* DeltaSupAlterada)
{
  std::list<ItLDeltaSup> ListaDeltasRemovidos;   //--- Como um mesmo ponto de restri��o pode ocorrer mais de uma vez numa lista 
  //--- quando for remo��o coloca os pontos numa outra lista para depois remov�-los
  for (int i = BORDOS ; i <= RESTRICOES ; i++)
  {
    LLDeltaSup& ListaRestAtual = i == BORDOS ? Superficie.PegaListaBordos() : Superficie.PegaListaRestricoes();

    //--- Procura em todas as listas de restri��es pelo ponto alterado, como as listas s�o pequenas e esta opera��o � rara, no problem 
    //--- (Passar para set tb, principalmente os bordos, criar o ser para busca e a lista para a busca sequencial

    for (register ItLLDeltaSup ItLLDeltaSupAtual = ListaRestAtual.begin() ; ItLLDeltaSupAtual != ListaRestAtual.end() ; ItLLDeltaSupAtual++)
    {
      for (register ItLDeltaSup ItLDeltaSupAtual = (*ItLLDeltaSupAtual).begin() ; ItLDeltaSupAtual != (*ItLLDeltaSupAtual).end() ; ItLDeltaSupAtual++)
      {
        if ((*ItLDeltaSupAtual) == *DeltaSupAAlterar) 
        {
          //--- Se o ponteiro veio nulo remove o ponto da lista de restri��o
          //--- Caso conr�rio altera-o para o valor modificado

          if (!DeltaSupAlterada)  ListaDeltasRemovidos.push_back(ItLDeltaSupAtual);
          else (*ItLDeltaSupAtual) = *DeltaSupAlterada;
        }
      }

      //--- Se for remo��o remove os pontos da restri��o atual

      if (!DeltaSupAlterada && ListaDeltasRemovidos.size())
      {
        for (std::list<ItLDeltaSup>::iterator It = ListaDeltasRemovidos.begin() ; It !=  ListaDeltasRemovidos.end() ; It++)
          (*ItLLDeltaSupAtual).remove(**It);
      }
    }
  }
}

void DesPontosGeomView::QuebraLista(LLDeltaSup& ListasAtuais,ItLLDeltaSup& itListaAtual,ItLDeltaSup itPontoRestricao) 
{
  bool ListaCircular(*itListaAtual->begin() == *itListaAtual->rbegin());
  ItLDeltaSup itPontoRestricaoOriginal(itPontoRestricao);
  ++itPontoRestricao;

  if(ListaCircular &&  itPontoRestricao == itListaAtual->end())
  { 
    itPontoRestricao = itListaAtual->begin(); 
    ++itPontoRestricao;
  }

  if(itPontoRestricao != itListaAtual->end())
  {
    while(itPontoRestricao->PegaPCentral().Descricao.Find("Ponto Inserido") != -1)
    {
      ++itPontoRestricao;
    }

    ListasAtuais.push_back(LDeltaSup(itPontoRestricao,itListaAtual->end()));
    if(0 && ListasAtuais.rbegin()->size() < 2) ListasAtuais.pop_back(); 
    else Superficie.InsereListaNoMapa(--(ListasAtuais.end()),ObjetoAtual);

    if(!ListaCircular)
    {
      //  itListaAtual->erase(itPontoRestricao,itListaAtual->end());
      itListaAtual->erase(++itPontoRestricaoOriginal,itListaAtual->end());
    }
    else 
    {
      ListasAtuais.rbegin()->insert((ListasAtuais.rbegin()->end()),++itListaAtual->begin(),++itPontoRestricaoOriginal);
      Superficie.RemoveListaDoMapa(itListaAtual);
      ListasAtuais.erase(itListaAtual);
      itListaAtual = ListasAtuais.size() > 0 ? --ListasAtuais.end() : ListasAtuais.end();  //<====
    }
  }
  else
  {
    itListaAtual->pop_back();  //-- se clicou no penultimo ponto da lista n�o quebra a lista pois a segunda lista iria ficar com 1 ponto apenas,  remove o ultimo ponto da lista
  }
}

void DesPontosGeomView::OnRedesenhar() 
{
  RedrawWindow();
}

void DesPontosGeomView::OnUpdateGravar(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(AlterouTerreno || AlterouProjeto);	
}

BOOL DesPontosGeomView::OnEraseBkgnd(CDC* ppDC) 
{
  return FALSE;
}

void DesPontosGeomView::DesenhaEspiral(const CurHorizontal& Curva,CDC* pDC)
{
  static int DELTA(5);
  //  MSecaoGeom* pMsecoesGeom = &MapaSecoesGeom;            //--- Ponteiro que apontontar� para o mapa a ser usado o definitivo ou o tempor�rio (default = definitivo)
  MSecaoGeom  MSecoesGeomTemp;                           //--- Mapa tempor�rio para ocaso de o usu�rio estar editando o geom�trico

  register itMSecaoGeom IMSecaoGeomAtual(MapaSecoesGeom.find(Curva.EstTs));     //--- Procura pelo TS no mapa de se��es

  //--- Se o Ts n�o foi encontrado no mapa de se��es, o us��rio alterou ou est� alterando o projeto geom�trico
  //--- Neste caso n�o h� pontos suficientes para desenhar a espiral (por polyline) ent�o esbo�a a espiral desenhando uma
  //--- Par�bola c�bica que passa por 3 pontos da espiral.

  //if (1 || IMSecaoGeomAtual == MapaSecoesGeom.end())      //--- Se n�o achar a estaca do TS usa o mapa tempor�rio.

  MSecaoGeom& rMsecoesGeom(const_cast<MSecaoGeom&>(Curva.mapSecoesDesenho));

  if(Curva.mapSecoesDesenho.size() == 0) CriaMapaSecoesGeomTemp(Curva,rMsecoesGeom); 

  // MSecaoGeom* pMsecoesGeom(&pMSG);

  for(int Ramo = ESQUERDO ; Ramo <= DIREITO ; Ramo++)
  {
    //--- Desenha uma Polyline entre os pontos da espirarl   TODO ---> passar para parabola c�bica ou bezier.

    if (Ramo == ESQUERDO) IMSecaoGeomAtual = rMsecoesGeom.find(Curva.EstTs);
    else IMSecaoGeomAtual = rMsecoesGeom.find(Curva.EstCs);    

    VPOINT VPontosCurva;        //--- Pontos que ser�o desenhados
    int nPontos(0);

    //--- Desenha o eixo

    while(IMSecaoGeomAtual != rMsecoesGeom.end() && (*IMSecaoGeomAtual).first <= (Ramo == ESQUERDO ? Curva.EstSc : Curva.EstSt))
    { 
      if(div(nPontos,DELTA).rem == 0) VPontosCurva.resize(VPontosCurva.size() + DELTA);       //--- aloca de 5 em 5 pontos

      VPontosCurva[nPontos].x = ((*IMSecaoGeomAtual).second.x - Deltas[X]) * Escala[X];
      VPontosCurva[nPontos].y = (AlturaVideo - ((*IMSecaoGeomAtual).second.y - Deltas[Y])) * Escala[Y] ;

      nPontos++;
      IMSecaoGeomAtual++;
    }

    if(!nPontos) return;   //--- Acontece nos movimentos do mouse 

    //--- Circula o TS e o SC ou o CS e o ST

    pDC->Arc(VPontosCurva[0].x-4,VPontosCurva[0].y+4,VPontosCurva[0].x+4,VPontosCurva[0].y-4,0,0,0,0);
    pDC->Arc(VPontosCurva[nPontos-1].x-4,VPontosCurva[nPontos-1].y+4,VPontosCurva[nPontos-1].x+4,VPontosCurva[nPontos-1].y-4,0,0,0,0);

    pDC->Polyline(&VPontosCurva.front(),nPontos);    //--- A Espiral � uma polyline  ==> TODO Passar para bezier ou par�bola c�bica
    //  pDC->PolyBezier(&VPontosCurva.front(),4);

    //--- Desenha os bordos direito e esquerdo
    //--- Calcula os pontos no bordo esquerdo e desenha a parabola cubica de 3 em 3 pontos

    for(int Lado = ESQUERDO ; Lado <= DIREITO ; Lado++)
    {
      IMSecaoGeomAtual = rMsecoesGeom.find(Ramo == ESQUERDO ? Curva.EstTs : Curva.EstCs);

      nPontos = 0;
      VPontosCurva.clear();

      Ponto PLateral;
      double Largura[2],SL(0.0);

      while(IMSecaoGeomAtual != rMsecoesGeom.end()&&(*IMSecaoGeomAtual).first <= (Ramo == ESQUERDO ? Curva.EstSc : Curva.EstSt))
      { 
        SecoesTipo.BuscaLargurasPistas((*IMSecaoGeomAtual).first,Largura[ESQUERDO],Largura[DIREITO]);
        SL = CurvasHorizontais.BuscaSuperLargura((*IMSecaoGeomAtual).first);

        if(SL != 0.0)
        {
          double ms = Curva.Sl / Curva.Lc,SLEstaca(0.0);

          if(Ramo == ESQUERDO) SLEstaca =  ms * (IMSecaoGeomAtual->first.EstReal - Curva.EstTs.EstReal);
          else SLEstaca = ms * (Curva.EstSt.EstReal - IMSecaoGeomAtual->first.EstReal);

          Largura[ESQUERDO] -= SLEstaca;
          Largura[DIREITO] += SLEstaca;
        }

        IrradiaPonto(Mat.M_PI_2,(*IMSecaoGeomAtual).second,Largura[Lado],PLateral);

        if(div(nPontos,DELTA).rem == 0) VPontosCurva.resize(VPontosCurva.size() + DELTA);       //--- aloca de 5 em 5 pontos

        VPontosCurva[nPontos].x = (PLateral.x - Deltas[X]) * Escala[X];
        VPontosCurva[nPontos].y = (AlturaVideo - (PLateral.y - Deltas[Y])) * Escala[Y] ;

        nPontos++;
        IMSecaoGeomAtual++;
      }

      pDC->Polyline(&VPontosCurva.front(),nPontos);
    }
  }
}

void DesPontosGeomView::DesenhaCircular(const CurHorizontal& Curva,CDC* pDC,bool Apagar)
{
  if (Curva.Raio < 5.0) return;    //---Raio muito pequeno.

  //--- Calcula o PI e o centro da curva

  long xPI((Curva.PI.x - Deltas[X]) * Escala[X]+.5),
    yPI((AlturaVideo - (Curva.PI.y - Deltas[Y])) * Escala[Y]+.5),
    xCC((Curva.CC.x - Deltas[X]) * Escala[X]+.5),
    yCC((AlturaVideo - (Curva.CC.y - Deltas[Y])) * Escala[Y]+.5);

  //--- Desenha (ou apaga) o centro da curva e o PI

  DesenharPonto(xPI,yPI,pDC,Apagar ? Cores[CORFUNDO] : Cores[CORPROJETO]);
  DesenharPonto(xCC,yCC,pDC,Apagar ? Cores[CORFUNDO] : Cores[CORPROJETO]);

  //--- Calcula as coordenadas em pixels do CS e do SC  

  long xSC((Curva.SC.x - Deltas[X]) * Escala[X] +.5),
    ySC((AlturaVideo - (Curva.SC.y - Deltas[Y])) * Escala[Y] + .5),
    xCS((Curva.CS.x - Deltas[X]) * Escala[X] + .5),
    yCS((AlturaVideo - (Curva.CS.y - Deltas[Y])) * Escala[Y] + .5),
    xTS((Curva.TS.x - Deltas[X]) * Escala[X] + .5),
    yTS((AlturaVideo - (Curva.TS.y - Deltas[Y])) * Escala[Y] + .5);

  //--- Verifica se o cs est� muito proximo do sc (em pixels). Caso esta dist�ncia seja menor que 4 pixels n�o desenha a curva
  //--- porque em arcos muito pequenos,a resolu��o do video pode fazer o cs ficar antes do sc que provoca
  //--- o desenho do arco complementar. Caso positivo n�o h� mais nada a desenhar. retorna

  if (abs(xCS-xSC) + abs(yCS-ySC)  < 4) return;

  //--- Calcula as coordenadas em pixels dos cantos superior esquerdo e direito do quadrado que inscreve o circulo

  long RaioPixels(Curva.Raio * Escala[X] + .5),xSE(xCC - RaioPixels),ySE(yCC + RaioPixels),xID(xCC + RaioPixels),yID(yCC - RaioPixels);

  //--- Desenha a curva

  if (Curva.Lado == DIREITO) pDC->Arc(xSE,ySE,xID,yID,xCS,yCS,xSC,ySC);
  else pDC->Arc(xSE,ySE,xID,yID,xSC,ySC,xCS,yCS);

  //--- Circula o CS e o SC

  //  pDC->Arc(xSC-4,ySC+4,xSC+4,ySC-4,0,0,0,0);  
  //  pDC->Arc(xCS-4,yCS+4,xCS+4,yCS-4,0,0,0,0); 

  double Largura[2];
  SecoesTipo.BuscaLargurasPistas(Curva.EstTs,Largura[ESQUERDO],Largura[DIREITO]);

  //--- Desenha os bordos

  for(int Lado = ESQUERDO ; Lado <= DIREITO ; Lado++)
  {
    Largura[Lado] += (Curva.Sl * ((Lado == ESQUERDO) ? -1.0 : 1.0));

    double RaioAuxPixels((Curva.Raio + Largura[Lado]) * Escala[X] * (Curva.Lado == DIREITO ? -1.0 : 1.0));   //--- O Lado esquerdo vem negativo

    Ponto SC,CS; 

    IrradiaPonto(Mat.M_PI_2,Curva.SC,Largura[Lado],SC);
    IrradiaPonto(Mat.M_PI_2,Curva.CS,Largura[Lado],CS);

    xSC = (SC.x - Deltas[X]) * Escala[X] + .5,
      ySC = (AlturaVideo - (SC.y - Deltas[Y])) * Escala[Y] + .5,
      xCS = (CS.x - Deltas[X]) * Escala[X] + .5,
      yCS = (AlturaVideo - (CS.y - Deltas[Y])) * Escala[Y] + .5,

      xSE = xCC - RaioAuxPixels,
      ySE = yCC + RaioAuxPixels,    
      xID = xCC + RaioAuxPixels,
      yID = yCC - RaioAuxPixels;

    if (Curva.Lado == DIREITO) pDC->Arc(xSE,ySE,xID,yID,xCS,yCS,xSC,ySC);  
    else  pDC->Arc(xSE,ySE,xID,yID,xSC,ySC,xCS,yCS);
  }
}

inline void DesPontosGeomView::IrradiaPonto(double Angulo,const Ponto& PInicial,double Distancia,Ponto& PCalcular)
{
  double AZ = PInicial.fi + Angulo;

  PCalcular.x =  PInicial.x + sin(AZ) * Distancia;
  PCalcular.y =  PInicial.y + cos(AZ) * Distancia;
}

inline void DesPontosGeomView::IrradiaPonto(double Angulo,double x,double y, double s,double Distancia,Ponto& PCalcular)
{
  double AZ = s + Angulo;

  PCalcular.x =  x + sin(AZ) * Distancia;
  PCalcular.y =  y + cos(AZ) * Distancia;
}

inline void DesPontosGeomView::DesenharPonto(double x,double y,CDC* pDC,COLORREF Cor)
{
  CPen Pena,*PAnterior;
  Pena.CreatePen(PS_SOLID,1,Cor);
  PAnterior = pDC->SelectObject(&Pena);    

  if (DesenharPixel) pDC->SetPixel(x,y,Cor);

  if (DesenharCirculo) pDC->Arc(x-3,y+3,x+3,y-3,0,0,0,0);   	

  if(DesenharCruz) 
  {
    //--- Desenha a cruz, 14 pixels de largura

    pDC->MoveTo(x-5,y);
    pDC->LineTo(x+5,y);		
    pDC->MoveTo(x,y-5);
    pDC->LineTo(x,y+5);
  }
  pDC->SelectObject(PAnterior); 
}

//--- Passar este metodo como propriedade de curvas horizontais

ItCMCurHorizontaisPI DesPontosGeomView::PegaPIMaisProximo(CurHorizontal& CurvaHorizontal)
{
  //--- Pega o pi <= ao pi dado menos a tolerancia x 
  //--- Vai calculando a distancia ao quadrado (para n�o precisar de tirar a raiz quadrada) e separa a menor
  //--- Se a menor estiver dentro da tolerancia ao quadrado , este � o ponto procurado

  if (!mapCurvasHorizontaisPI.size()) return mapCurvasHorizontaisPI.end();

  CurvaHorizontal.PI.x += Toleranciax;

  ItCMCurHorizontaisPI itCurAtual = mapCurvasHorizontaisPI.upper_bound(&CurvaHorizontal.PI);   //--- Pega o >= apos a da tolerancia

  if (itCurAtual == mapCurvasHorizontaisPI.end()) itCurAtual--;

  CurvaHorizontal.PI.x -= Toleranciax;               //--- Acerta o x do PI

  double MenorDistancia2 = INFINITO,Distancia2Atual;
  ItCMCurHorizontaisPI itPMaisPerto;

  //--- Procura em todos os PIs que estiverem dentro da toleranciax. 
  //--- Procura em todas as curvas at� que o x do PI da curva  seja menor que o do PI Atual menos a toleranciax
  //--- A procura � feita separando o PI que tem a menor distancia ate o ponto atual do mouse que esta no PI da curva atual

  do
  {  
    Distancia2Atual =  pow((itCurAtual->second->PI.x - CurvaHorizontal.PI.x),2) + pow((itCurAtual->second->PI.y - CurvaHorizontal.PI.y),2); 

    if (Distancia2Atual < MenorDistancia2) 
    {
      MenorDistancia2 = Distancia2Atual;
      itPMaisPerto = itCurAtual;
    } 
  }while(itCurAtual-- != mapCurvasHorizontaisPI.begin() && itCurAtual->second->PI.x > (CurvaHorizontal.PI.x - Toleranciax));

  //--- Se a menor dist�ncia estiver dentro da tolerancia este � o ponto procurado, retorna-o

  return MenorDistancia2 < pow(Toleranciax,2.0) ? itPMaisPerto : mapCurvasHorizontaisPI.end();
}

ItCMCurHorizontaisCC DesPontosGeomView::PegaCCMaisProximo(CurHorizontal& CurvaHorizontal)
{
  //--- Pega o CC <= ao pi dado menos a tolerancia x 
  //--- Vai calculando a distancia ao quadrado (para n�o precisar de tirar a raiz quadrada) e separa a menor
  //--- Se a menor estiver dentro da tolerancia , este � o ponto procurado

  if (!mapCurvasHorizontaisCC.size()) return mapCurvasHorizontaisCC.end();

  CurvaHorizontal.CC.x += Toleranciax;

  ItCMCurHorizontaisCC itCurAtual(mapCurvasHorizontaisCC.upper_bound(&CurvaHorizontal.CC));   //--- Pega o >= apos a da tolerancia
  if (itCurAtual == mapCurvasHorizontaisCC.end()) itCurAtual--;

  CurvaHorizontal.CC.x -= Toleranciax;               //--- Acerta o x do PI

  double MenorDistancia2 = INFINITO,Distancia2Atual;
  ItCMCurHorizontaisCC itPMaisPerto;

  //--- Procura em todos os PIs que estiverem dentro da toleranciax. 
  //--- Procura em todas as curvas comecando na posterior ao x do PI + a toletancia, voltando at� que o x do PI da curva  
  //--- seja menor que o do PI Atual menos a toleranciax
  //--- A procura � feita separando o PI que tem a menor distancia ate o ponto atual do mouse, que esta no PI da curva atual passada como parametro.

  do
  {  
    Distancia2Atual =  pow((itCurAtual->second->CC.x - CurvaHorizontal.CC.x),2) + pow((itCurAtual->second->CC.y - CurvaHorizontal.CC.y),2); 

    if (Distancia2Atual < MenorDistancia2) 
    {
      MenorDistancia2 = Distancia2Atual;
      itPMaisPerto = itCurAtual;
    } 

  }while(itCurAtual-- != mapCurvasHorizontaisCC.begin() && itCurAtual->second->CC.x > (CurvaHorizontal.CC.x - Toleranciax));

  //--- Se a menor dist�ncia estiver dentro da tolerancia este � o ponto procurado, retorna-o

  return (MenorDistancia2 < pow(Toleranciax,2.0)) ? itPMaisPerto : mapCurvasHorizontaisCC.end();  
}

bool DesPontosGeomView::VerificaTangente(const Ponto& PAtual,ItCMCurHorizontaisTS& ItCurvaAnterior)
{
  double Distancia(INFINITO),Raio,AzPonto;
  bool Achou(false),fim(false);
  ItCurvaAnterior = mapCurvasHorizontaisTS.begin();

  do
  {
    bool NaTangente(false);

    while(ItCurvaAnterior != mapCurvasHorizontaisTS.end() && !NaTangente)
    {
      Raio = hypot(ItCurvaAnterior->second.PI.x - PAtual.x,ItCurvaAnterior->second.PI.y - PAtual.y);
      AzPonto = CCurHorizontais::CalculaAzimute(PAtual,ItCurvaAnterior->second.PI);

      if (fabs(ItCurvaAnterior->second.ST.fi - AzPonto) < Mat.M_PI_2)   //--- Est� antes do pi anterior
      {
        Distancia = fabs(sin(AzPonto - ItCurvaAnterior->second.ST.fi) * Raio);

        NaTangente = Distancia < Toleranciax; 
      }

      ItCurvaAnterior++;
    }

    //--- Pode ser que esteja na tangente mas n�o esteja no trecho,
    //--- Aqui o anterior passa a ser o posterior devido ao �ltimo ++ do for.
    //--- Verifica se est� entre o St e o Ts

    if(NaTangente)
    {
      const CurHorizontal& CurvaPosterior(ItCurvaAnterior->second);           //--- Sai com o posterior, por causa do ++
      ItCMCurHorizontaisTS ItCurvaAnteriorRasc(ItCurvaAnterior);
      ItCurvaAnteriorRasc--;

      const CurHorizontal& CurvaAnterior(ItCurvaAnteriorRasc->second);

      //--- Verifica se est� no trecho reto

      Achou = Raio > CurvaAnterior.Ts && Raio < ((CEstaca) CurvaPosterior.EstTs - (CEstaca) CurvaAnterior.EstSt.EstVirtual + CurvaAnterior.Ts);
    }

  }while (!Achou && ItCurvaAnterior != mapCurvasHorizontaisTS.end()); 

  if(Achou) ItCurvaAnterior--;      //---ajusta o iterador para a curva anterior

  return Achou; 
}

void DesPontosGeomView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  if(nChar == KEY_F5) 
  {
    RedrawWindow();
    return;
  }

  if (nChar == KEY_ESCAPE)
  {
    //--- Restaura o que estiver sendo arrastando 

    if (ArrastandoPI)
    {
      CurvasHorizontais.AlteraCurvaPI(ItCurHorizSendoArrastadaPI,BackCurvaSendoArrastada.PI,ItUltimaCurvaAjustadaTS,NULL);
      Arrastando = ArrastandoPI = false;
      TerminarArrasto = true;
    }

    if(ArrastandoCC)
    {
      CurvasHorizontais.AlteraCurvaCC(ItCurHorizSendoArrastadaCC,BackCurvaSendoArrastada.CC,ItUltimaCurvaAjustadaTS);
      Arrastando = ArrastandoCC = false;
      TerminarArrasto = true;
    }

    if(PegandoPontoSecAvulsa) 
    {
      PegandoPontoSecAvulsa = PegouPrimPonto = false;
      ((CChildFrame*) GetParentFrame())->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTSECAVULSA,0);
    }

    if(DesenhandoObjetos) 
    {
      ItCLLDeltaSup itcListaRestAtual(--(Superficie.PegaListaRestricoes().end()));

      if(itcListaRestAtual->size() == 0 || (itcListaRestAtual->size() == 1 && !VerfObjAvulso(Superficie.VerfAciTopografico(itcListaRestAtual))))
      {
        Superficie.RemoveListaDoMapa(--(Superficie.PegaListaRestricoes().end()));
        Superficie.PegaListaRestricoes().pop_back();
      }
    }

    CriandoRestricao = Arrastando = InserindoPonto = TrocandoObjeto = InserindoLembrete = false;
    ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();

    if (TerminarArrasto) OnLButtonUp(0,CPoint());     //--- Simula o LbuttonUp s� para destruir o timer.

    DefinindoLinhaBase = 0;

    RedrawWindow();

    return;
  }

  if(nChar == VK_DELETE)
  {
    if(!ArrastandoPI && !ArrastandoCC)
    {
      CurHorizontal CurvaDel(INFINITO,MouseXYGeo->m_DOCoorX,MouseXYGeo->m_DOCoorY);

      if(Superficie.EstaRotacionada()) CurvaDel.PI.RotacionaPlano(sin(Superficie.PegaAnguloRotacao()),cos(Superficie.PegaAnguloRotacao()),Superficie.PegaAnguloRotacao());

      ItCMCurHorizontaisPI ItCCurvaPI(PegaPIMaisProximo(CurvaDel));

      if (ItCCurvaPI != mapCurvasHorizontaisPI.end())
      {
        CurHorizontal CurvaAtual =  *ItCCurvaPI->second;

        if(ItCCurvaPI != mapCurvasHorizontaisPI.end())
        {
          if(dialogar.DoModal(5) == IDOK)
          {
            if(CurvasHorizontais.RemoveCurva(ItCCurvaPI,ItUltimaCurvaAjustadaTS,NULL) ==  CCurHorizontais::CURVA_OK) AlterouProjeto = true;
          }
        }
        RedrawWindow();
      }
      else
      {
        Ponto PAnterior(MouseXYGeo->m_DOCoorX,MouseXYGeo->m_DOCoorY),PPosterior(PAnterior),PSobMouse(PAnterior);

        PAnterior.x -= Toleranciax;
        PPosterior.x += Toleranciax;

        itmapSecoesAvulsas itMSecaoAvAnterior(SecoesAvulsas.UpperBound(&PAnterior));
        itmapSecoesAvulsas itMSecaoAvPosterior(SecoesAvulsas.UpperBound(&PPosterior));
        itmapSecoesAvulsas itSecAvMaisPerto(SecoesAvulsas.End());

        double MenorDistancia2(INFINITO),Distancia2Atual(INFINITO);

        for(itmapSecoesAvulsas ItMapSecAvAtual = itMSecaoAvAnterior ; ItMapSecAvAtual != itMSecaoAvPosterior ; ItMapSecAvAtual++)
        {
          Distancia2Atual =  pow((*ItMapSecAvAtual).first.x - PSobMouse.x,2) + pow((*ItMapSecAvAtual).first.y - PSobMouse.y,2); 

          if (Distancia2Atual < MenorDistancia2) 
          {
            MenorDistancia2 = Distancia2Atual;
            itSecAvMaisPerto = ItMapSecAvAtual;
          }             
        }

        if (MenorDistancia2 < pow(Toleranciax,2))    //--- Se estiver proximo a um ponto
        {
          if(dialogar.DoModal(13) == IDOK)
          {
            SecoesAvulsas.RemoveSecaoAvulsa((*itSecAvMaisPerto).second);

            AlterouSecoesAvulsas = true;

            RedrawWindow();
          }
        }
        else   // Pontos inseridos podem ser removidos - AGUARDAR 
        {/*
         Ponto PSobMouse(MouseXYGeo->m_DOCoorX,MouseXYGeo->m_DOCoorY);
         ItSSuperficie itDeltaSupSobMouse(PegaPontoMaisProximo(PSobMouse));

         if(itDeltaSupSobMouse != Superficie.pSuperficieAtual->end())
         {
         if(itDeltaSupSobMouse->EBordo == true)
         {
         if(dialogar.DoModal(5) == IDOK)
         {
         Superficie.ExcluiDeltaSuperficie(itDeltaSupSobMouse);
         }
         }
         }*/
        }
      }
    }
  }

  if(GetKeyState(VK_CONTROL) < 0)
  {
    nChar = toupper(nChar);

    switch (nChar)
    {
    case 'N': DesenharCN = DesenharCN; break;
    case 'S': DesenharSecoes = ! DesenharSecoes;break; 
    case 'T': DesenharArestas = !DesenharArestas; break;
    case 'F': DesenharBordos = !DesenharBordos; break;
    case 'A': DesenharRestricoes = !DesenharRestricoes;break; 
    case 'P': DesenharPontos = !DesenharPontos; break;
    case 'R': DesenharProjeto = !DesenharProjeto; break;
    case 'U': CotarCN = !CotarCN;break; 
    case 'D': MostrarDescricao = !MostrarDescricao; break;
    case 'E': CircularSecoes = !CircularSecoes; break;
    case 'X': DesenharPixel = !CotarCN;break; 
    case 'C': DesenharCirculo = !DesenharCirculo; break;
    case 'Z': DesenharCruz = !DesenharCruz; break;
    case 'O': VarrerTerreno = !VarrerTerreno;break; 
    case 'J': VarrerProjeto = !VarrerProjeto; break;
    case 'V': VarrerCN = !VarrerCN; break;
    }
  }

  RedrawWindow();
  GetFocus();
}

std::string DesPontosGeomView::PreparaDescricao(const void* pObjeto,bool ArrastandoPonto,int OBJETO) 
{
  CurHorizontal *pCH(NULL),CurNaoRotacionada;
  Ponto* pPonto(NULL),PontoSecao;
  itMSecaoGeomPonto* itMSecao;

  //--- Se a superficie est� rotacionada, desrotaciona para mostrar a coordenadas reais

  //if(OBJETO != OBJ_PTERRENO && OBJETO != OBJ_SECAO && OBJETO != OBJ_PONSECAVULSA && OBJETO != OBJ_SECAV)
  if(OBJETO == OBJ_CURHR || OBJETO == OBJ_CC || OBJETO == OBJ_PI)
  {
    pCH = (CurHorizontal*) pObjeto;

    if(Superficie.EstaRotacionada())
    {
      CurNaoRotacionada = *pCH;
      CurNaoRotacionada.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),-AnguloRotacao);

      pCH = &CurNaoRotacionada;
    }
  }
  else
  {
    if(OBJETO == OBJ_SECAO)
    {
      itMSecao =  (itMSecaoGeomPonto*) pObjeto;
      PontoSecao = *((*itMSecao)->first);

      if(Superficie.EstaRotacionada()) PontoSecao.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),-AnguloRotacao);
    }
    else  
    {
      pPonto = (Ponto*) pObjeto; 
      if(OBJETO == OBJ_CURNI)
      {
        PontoSecao = *pPonto;
        if(Superficie.EstaRotacionada()) PontoSecao.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),-AnguloRotacao);
      }
    }

    /*    ==> Verificar se pode tirar
    if(OBJETO == OBJ_CURNI)
    {
    Ponto* PontoAtual =  (Ponto*) pObjeto;

    //      if(Superficie.EstaRotacionada()) PontoSecao.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),-AnguloRotacao);
    }
    else pPonto = (Ponto*) pObjeto;   =====> REPETIDO!!! 
    */
  }

  std::strstream strstreamTemp;

  strstreamTemp << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

  switch (OBJETO)
  {   
    case OBJ_TEXTO: strstreamTemp << (char*) pObjeto << std::ends;break;
    case OBJ_PI: strstreamTemp << " Ponto de Interse��o: \t Coor N= " << pCH->PI.y << " \t Coor E= " << pCH->PI.x; break;
    case OBJ_CC: strstreamTemp << " Centro de Curva: \t Coor N= " << pCH->CC.y << " \t Coor E= " << pCH->CC.x; break;
    case OBJ_PONTERRENO:  strstreamTemp << " Ponto de Terreno: "; break;
    case OBJ_PONDESABILITADO: strstreamTemp << " Desabilitado - ";
    case OBJ_PONINSUSUARIO: 
    case OBJ_PONINSDELAUNAY:
    case OBJ_PONINSGABRIEL: strstreamTemp << " N�mero: "; break;
    case OBJ_LEMBRETE : strstreamTemp << " Lembrete: "; break; 
  }

  if (OBJETO == OBJ_CC || OBJETO == OBJ_PI)
  {
    if (pCH->Ac != 0.0)
    {
      if(pCH->Tipo == CURVA_ESPIRAL) 
      {
        if(ArrastandoPonto) strstreamTemp << " \r\n Lc = " << pCH->Lc << " \t I = ";
        else strstreamTemp << " \r\n I = ";

      }
      else strstreamTemp << " \r\n Ac = "; 

      strstreamTemp << Mat.strg_gms(pCH->Ac * Matem::PI_GRAUS).c_str()
        << " \t Raio = " << pCH->Raio;
      if(!ArrastandoPonto)
      {
        if (pCH->Lc > 0.0)
          strstreamTemp << std::right << std::setfill(' ') 
          << " \t Lc = " << pCH->Lc
          << " \r\n xc = " << pCH->xEsp
          << " \t yc = " << pCH->yEsp
          << " \t Ts = " << pCH->Ts
          << " \r\n Sl = " << pCH->Sl
          << " \t Se = " << pCH->Se * 100.0
          << " \t Ac = " << Mat.strg_gms(pCH->AcCirc * Matem::PI_GRAUS).c_str()
          << " \r\n TS = " << pCH->EstTs.GetBuffer(1) 
          << " :\r\n Coor N= " <<  pCH->TS.y 
          << " \t Coor E= " <<  pCH->TS.x 
          << " \t Az= " << Mat.strg_gms(pCH->TS.fi * Matem::PI_GRAUS).c_str()
          << " \r\n SC = " << pCH->EstSc.GetBuffer(1) 
          << " :\r\n Coor N= " <<  pCH->SC.y 
          << " \t Coor E= " <<  pCH->SC.x 
          << " \t Az= " << Mat.strg_gms(pCH->SC.fi * Matem::PI_GRAUS).c_str()
          << " \r\n CS = " << pCH->EstCs.GetBuffer(1) 
          << " :\r\n Coor N= " <<  pCH->CS.y 
          << " \t Coor E= " <<  pCH->CS.x 
          << " \t Az= " << Mat.strg_gms(pCH->CS.fi * Matem::PI_GRAUS).c_str()
          << " \r\n ST = " << pCH->EstSt.GetBuffer(1) 
          << " :\r\n Coor N= " <<  pCH->ST.y 
          << " \t Coor E= " <<  pCH->ST.x 
          << " \t Az= " << Mat.strg_gms(pCH->ST.fi * Matem::PI_GRAUS).c_str();
        else 
          strstreamTemp << " \t T = " << pCH->Ts
          << " \r\n PC = " << pCH->EstTs.GetBuffer(1) 
          << " :\r\n Coor N= " <<  pCH->TS.y 
          << " \t Coor E= " <<  pCH->TS.x 
          << " \t Az= " << Mat.strg_gms(pCH->TS.fi * Matem::PI_GRAUS).c_str()
          << " \r\n PT = " << pCH->EstSt.GetBuffer(1) 
          << " :\r\n Coor N= " <<  pCH->ST.y 
          << " \t Coor E= " <<  pCH->ST.x 
          << " \t Az= " << Mat.strg_gms(pCH->ST.fi * Matem::PI_GRAUS).c_str();
      }
    }
    else strstreamTemp << " \r\n Est = " << pCH->EstTs.GetBuffer(1); 
  }
  else    //--- � ponto de terreno ou se��o de projeto
  {
    if (OBJETO == OBJ_PONTERRENO || OBJETO == OBJ_PONINSUSUARIO || OBJETO == OBJ_PONINSDELAUNAY || OBJETO == OBJ_PONINSGABRIEL || OBJETO == OBJ_PONDESABILITADO)
    {
      if(MOSTRAR_NUMERO_PONTO) strstreamTemp <<  pPonto->Nome;
      if(MOSTRAR_DESC) strstreamTemp << " \r\n  " << pPonto->Descricao.GetBuffer(1);
      if(MOSTRAR_COORDENADAS) 
      {
        strstreamTemp << " \r\n Coor N= " <<  pPonto->y
          << " \r\n Coor E= " <<  pPonto->x
          << " \r\n Cota = "; 
        if (pPonto->z == 0.0) strstreamTemp << "A ser calculada";
        else strstreamTemp << pPonto->z;
      }
    }
    else  
    {
      if(OBJETO == OBJ_PROJETO)
      {
        itMSecaoGeomPonto* itMSecao((itMSecaoGeomPonto*) pObjeto);

        strstreamTemp << " Estaca: "
          << ((CString*) ((*itMSecao)->second))->GetBuffer(1)
          << " " 
          << ((*itMSecao)->second)->Descricao 
          // << " \r\n Coor N= " << ((*itMSecao)->first)->y
          // << " \r\n Coor E= " << ((*itMSecao)->first)->x
          << " \r\n Coor N= " << PontoSecao.y
          << " \r\n Coor E= " << PontoSecao.x
          << " \r\n Az= " << Mat.strg_gms(((*itMSecao)->first)->fi * Matem::PI_GRAUS).c_str();
      }
      else   
      {
        if ( OBJETO == OBJ_PONSECAVULSA)   
        {
          if(!PegouPrimPonto) strstreamTemp << "In�cio da Se��o Avulsa:";
          else strstreamTemp << "Fim da Se��o Avulsa:";
          strstreamTemp << " " 
            << " \r\n Coor N= " << pPonto->y
            << " \r\n Coor E= " << pPonto->x;
        }
        else     
        {
          if(OBJETO == OBJ_SECAV)  //--- Ponto de eixo  se��o avulsa
          {
            strstreamTemp << "Se��o Avulsa: " 
              << pPonto->Descricao
              << " \r\n Coor N= " << pPonto->y
              << " \r\n Coor E= " << pPonto->x;
          }
          else                     //--- � ponto de curva de nivel     
          {
            if(OBJETO == OBJ_CURNI)
            {
              // ItSetItLPontos itPontoCN(*(ItSetItLPontos*) pObjeto);

              strstreamTemp << " Curva de N�vel: "
                << " " 
                << " \r\n Coor N= " << PontoSecao.y
                << " \r\n Coor E= " << PontoSecao.x
                << " \r\n Cota= " << PontoSecao.z;
            }
            else
            {
              if(OBJETO == OBJ_LEMBRETE)
              {
                strstreamTemp << (*((ittysetLembretes*)(pObjeto)))->Titulo() 
                  << "\r\n " << (*((ittysetLembretes*)(pObjeto)))->Texto(); 
              }
              else                     //--- � ponto de curva de nivel     
              {
                if(OBJETO == OBJ_SECAO)
                {
                  strstreamTemp << " Estaca: "
                    << (*(*itMSecao)->second)
                    << " " 
                    << (*itMSecao)->second->Descricao
                    << " \r\n Coor N= " << PontoSecao.y
                    << " \r\n Coor E= " << PontoSecao.x
                    << " \r\n Cota Terreno= ";
                  if((*itMSecao)->second->Cota == INFINITO)  strstreamTemp << " A Calcular";
                  else strstreamTemp << (*itMSecao)->second->Cota;
                }
              }
            }
          }
        }
      }
    }
  }

  strstreamTemp << std::ends;

  const std::string Ret = strstreamTemp.str(); 

  strstreamTemp.freeze(false);

  return Ret;
}

void DesPontosGeomView::DesenhaProjeto(ItCMCurHorizontaisTS pitMCH,CDC* pDC,int OperacaoForcada,unsigned int Quantidade) 
{
  CPen Pena,*PAnterior;     

  Pena.CreatePen(PS_SOLID,1,Cores[CORPROJETO]);
  PAnterior = pDC->SelectObject(&Pena);   

  //--- Desenha o projeto: Existem duas possibilidades ou � para desenhar todo o projeto ou � para se redesenhar as curva que est�o sendo alteradas devido
  //--- ao movimento de um pi. Para o primeiro caso "OperacaoForcada" e "Quantidade" v�m com os valores default, OPR_AMBOS E \xFFFF, Respectivamente. Para o
  //--- Caso de ser o movimento de um PI estas duas vari�veis v�m com seus respectivos valores, dependendo do caso.

  ItCMCurHorizontaisTS itMCH(pitMCH);

  if (itMCH->second.Raio < 0.001) 
  {
    long xST((itMCH->second.ST.x - Deltas[X]) * Escala[X]),
      yST((AlturaVideo - (itMCH->second.ST.y - Deltas[Y])) * Escala[Y]);

    pDC->Arc(xST-4,yST+4,xST+4,yST-4,0,0,0,0);

    //DesenharPonto(xST,yST,pDC,Cores[CORPROJETO]);
    pDC->MoveTo(xST,yST);

    itMCH++;
  }
  else  pDC->MoveTo((itMCH->second.TS.x - Deltas[X]) * Escala[X],(AlturaVideo - (itMCH->second.TS.y - Deltas[Y])) * Escala[Y]);

  //--- Desenha o eixo e os bordos das  3 curvas  envolvidas a partir da curva passada, desenha at� o fim do set ou ate a quantidade solicitada.

  long yPI,xPI;

  for(int Quant = Quantidade ; itMCH != mapCurvasHorizontaisTS.end() && Quant-- ; itMCH++) 
  {
    xPI = (itMCH->second.PI.x - Deltas[X]) * Escala[X];
    yPI = (AlturaVideo - (itMCH->second.PI.y - Deltas[Y])) * Escala[Y];

    //   pDC->Arc(xPI-4,yPI+4,xPI+4,yPI-4,0,0,0,0);

    pDC->LineTo(xPI,yPI);
    if (itMCH->second.Tipo == ESPIRAL && itMCH->second.Lc > 0.0 ) DesenhaEspiral(itMCH->second,pDC);
    if(itMCH != --mapCurvasHorizontaisTS.end()) DesenhaCircular(itMCH->second,pDC,OperacaoForcada == OPR_APAGAR);
    pDC->MoveTo(xPI,yPI);    //--- volta ao ponto para desenhar a pr�xinma tangente
  }

  pDC->Arc(xPI-4,yPI+4,xPI+4,yPI-4,0,0,0,0);

  for(int Lado = ESQUERDO ; Lado <= DIREITO ; Lado++)
  { 
    Ponto PAux;
    itMCH = pitMCH;
    double Larguras[2];

    SecoesTipo.BuscaLargurasPistas(itMCH->second.EstTs,Larguras[ESQUERDO],Larguras[DIREITO]);

    IrradiaPonto(Mat.M_PI_2,itMCH->second.ST,Larguras[Lado],PAux);

    pDC->MoveTo((PAux.x - Deltas[X]) * Escala[X],(AlturaVideo - (PAux.y - Deltas[Y])) * Escala[Y]);

    //--- Irradia os pontos nos bordos e desenha as retas

    itMCH++;

    for(int Quant = Quantidade ; itMCH != mapCurvasHorizontaisTS.end() && --Quant ; itMCH++) 
    {
      SecoesTipo.BuscaLargurasPistas(itMCH->second.EstTs,Larguras[ESQUERDO],Larguras[DIREITO]);
      IrradiaPonto(Mat.M_PI_2,itMCH->second.TS,Larguras[Lado],PAux);
      pDC->LineTo(((PAux.x - Deltas[X]) * Escala[X]),(AlturaVideo - (PAux.y - Deltas[Y])) * Escala[Y]);

      SecoesTipo.BuscaLargurasPistas(itMCH->second.EstSt,Larguras[ESQUERDO],Larguras[DIREITO]);
      IrradiaPonto(Mat.M_PI_2,itMCH->second.ST,Larguras[Lado],PAux);
      pDC->MoveTo((PAux.x - Deltas[X]) * Escala[X],(AlturaVideo - (PAux.y - Deltas[Y])) * Escala[Y]);
    }
  } 
}

void DesPontosGeomView::DesenhaSuperficie(ItSSuperficie DeltaSupInicial,ItSSuperficie DeltaSupFinal,CDC* pDC) 
{
  CPen Pena,* PAnterior;     

  Pena.CreatePen(PS_SOLID,1,Cores[CORPONTOS]);
  PAnterior = pDC->SelectObject(&Pena);    

  if (DesenharPontos)
  {
    Ponto Ppixels,PAtual;

    register ItSSuperficie DeltaSupAtual(DeltaSupInicial);

    for(; DeltaSupAtual !=  DeltaSupFinal ; DeltaSupAtual++)
    {
      if(!DesenharPontosInseridos && DeltaSupAtual->PegaPCentral().Inserido == true)  
      {
        continue;
      }

      PAtual = (*DeltaSupAtual).PegaPCentral();   //--- Pega o ponto atual

      Ppixels.x = ((PAtual.x - Deltas[X]) * Escala[X]);
      Ppixels.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y] ;

      if (DesenharPixel) pDC->SetPixel(Ppixels.x,Ppixels.y,Cores[CORPONTOS]);

      if (DesenharCruz)
      {
        //--- Desenha a cruz, 5 pixels de largura

        pDC->MoveTo(Ppixels.x-5,Ppixels.y);
        pDC->LineTo(Ppixels.x+5,Ppixels.y);		
        pDC->MoveTo(Ppixels.x,Ppixels.y-5);
        pDC->LineTo(Ppixels.x,Ppixels.y+5);
      }

      //--- circula o ponto

      if (DesenharCirculo) pDC->Arc(Ppixels.x-3,Ppixels.y-3,Ppixels.x+3,Ppixels.y+3,0,0,0,0);   	
    }
  }

  CPen PenaArestas;
  PenaArestas.CreatePen(PS_SOLID,1,Cores[CORARESTAS]);
  pDC->SelectObject(&PenaArestas);
  Ponto Ppixels;
  const Ponto* pPontoAtual ;

  if (DesenharArestas)
  {
    register ItSSuperficie DeltaSupAtual = DeltaSupInicial;

    for(; DeltaSupAtual != DeltaSupFinal ; DeltaSupAtual++)
    {
      const Ponto& PCentral = DeltaSupAtual->PegaPCentral();   //--- Pega o ponto atual
      Ponto PPixelsCentral(((PCentral.x - Deltas[X]) * Escala[X]),(AlturaVideo - (PCentral.y - Deltas[Y])) * Escala[Y]);

      for (register ittysetArestas PLigAtual = DeltaSupAtual->PontosLigados.begin() ; PLigAtual != DeltaSupAtual->PontosLigados.end() ; PLigAtual++)
      { 
        pPontoAtual = &PLigAtual->PontoLigado;

        //   if(Superficie.pSuperficieAtual->find(*pPontoAtual)->PegaPCentral().Baliza == false)

        //    if(!pPontoAtual->Baliza)             //--- Se este ponto ainda n�o foi desenhado
        {
          Ppixels.x = ((pPontoAtual->x - Deltas[X]) * Escala[X]);
          Ppixels.y = (AlturaVideo - (pPontoAtual->y - Deltas[Y])) * Escala[Y] ;

          pDC->MoveTo(PPixelsCentral.x,PPixelsCentral.y);
          pDC->LineTo(Ppixels.x,Ppixels.y);
        }
      }

      //  (const_cast<CDeltaSuperficie*>(&*DeltaSupAtual))->PegaPCentralNC().Baliza = true;         //--- sinaliza q j� foi desenhado
    }

    //  for(DeltaSupAtual = DeltaSupInicial; DeltaSupAtual != DeltaSupFinal ; DeltaSupAtual++)
    //    (const_cast<CDeltaSuperficie*>(&*DeltaSupAtual))->PegaPCentralNC().Baliza = false;         //--- Reinicializa as baliza��es, todos os pontos j� foram desenhados
  }
  pDC->SelectObject(PAnterior);
}

void DesPontosGeomView::OnButrotacionar() 
{
  int NovoAngulo;
  int Absoluto_Relativo;

  CDRotacionar DRotacionar(this,Superficie.AngRotacaoTopogGraus(),&NovoAngulo,&Absoluto_Relativo);	

  if(DefinindoLinhaBase == 3 || DRotacionar.DoModal() == IDOK)
  {
    Ponto PCentral(Deltas[X] + LarguraVideo / 2.0,Deltas[Y] +  AlturaVideo / 2.0);
    double AnguloArotacionar(0.0);

    AlterouProjeto = AlterouTerreno = AlterouAngulo = true;
    GetDocument()->SetModifiedFlag(true);  

    if(DefinindoLinhaBase != 3)
    {
      if(NovoAngulo < 0.0) NovoAngulo+= 360.0;

      NovoAngulo = 360.0 - NovoAngulo;

      AnguloArotacionar = (Absoluto_Relativo ? (NovoAngulo / 180.0 * Mat.M_PI) - Superficie.PegaAnguloRotacao() : (NovoAngulo / 180.0 * Mat.M_PI) );

      if (fabs(AnguloArotacionar) < Mat.UmGrau_Rad) return;
    }
    else
    {
      Ponto RP(Mat.R_P(PontosLinhaBase[1],PontosLinhaBase[0])); 
      RP.x -= fmod(RP.x,Mat.UmGrau_Rad);
      AnguloArotacionar = RP.x + Mat.M_PI_2;
      if(AnguloArotacionar > Mat.M_2_PI) AnguloArotacionar -= Mat.M_2_PI; 
      else  if(AnguloArotacionar < 0.0 ) AnguloArotacionar += Mat.M_2_PI; 
    }

    if(Superficie.PegaAnguloRotacao() + AnguloArotacionar < 0.0)  AnguloArotacionar += Mat.M_2_PI;
    if(Superficie.PegaAnguloRotacao() + AnguloArotacionar > Mat.M_2_PI)  AnguloArotacionar -= Mat.M_2_PI;

    double Sin(sin(AnguloArotacionar)),Cos(cos(AnguloArotacionar));
    Ponto PontoSobMouseAnterior;

    if(DeltaSupSobMouseAnterior != Superficie.pSuperficieAtual->end()) PontoSobMouseAnterior =  DeltaSupSobMouseAnterior->PegaPCentral();

    Superficie.RotacionaPlano(Sin,Cos,AnguloArotacionar);
    CurvasHorizontais.RotacionaPlano(Sin,Cos,AnguloArotacionar);
    RotacionaBordoseRestricoes(Sin,Cos,AnguloArotacionar);
    RotacionaSecoes(Sin,Cos,AnguloArotacionar);

    DeltaSupSobMouseAnterior = DeltaSupAnterior = Superficie.pSuperficieAtual->end();

    AnguloRotacao = Superficie.PegaAnguloRotacao();
    Seno = sin(AnguloRotacao);
    Coseno = cos(AnguloRotacao);
    Quadrante = floor(AnguloRotacao / Mat.M_PI_2);
    if(Quadrante == 4) Quadrante = 3;

    CriaFontes(true);

    CalculaNovasDimensoes();
    PCentral.RotacionaPlano(Sin,Cos,AnguloArotacionar);
    CentralizarTela(PCentral);    

    DeltaSupSobMouseAnterior  = DeltaSupAnterior = Superficie.pSuperficieAtual->end();   
  }
  else 
    if(DefinindoLinhaBase == 0 && NovoAngulo == -10000.0) 
    {
      CursorAtual = IDC_CROSS;
      DefinindoLinhaBase = 1;  //--- Pegando primeiro ponto
    }
}

void DesPontosGeomView::DefineParametros() 
{
  //--- Calcula a escala padr�o que permite a visualiza��o todo o desenho
  double LimiteEsq(Superficie.PegaLimiteEsq()),LimiteDir(Superficie.PegaLimiteDir()),LimiteSup(Superficie.PegaLimiteSup()),LimiteInf(Superficie.PegaLimiteInf());

  if(LimiteEsq > CurvasHorizontais.PegaLimiteEsq()) LimiteEsq = CurvasHorizontais.PegaLimiteEsq();
  if(LimiteDir < CurvasHorizontais.PegaLimiteDir()) LimiteDir = CurvasHorizontais.PegaLimiteDir();
  if(LimiteSup < CurvasHorizontais.PegaLimiteSup()) LimiteSup = CurvasHorizontais.PegaLimiteSup();
  if(LimiteInf > CurvasHorizontais.PegaLimiteInf()) LimiteInf = CurvasHorizontais.PegaLimiteInf();

  EscalaDefault[X] =  Resolucao[X] / (LimiteDir - LimiteEsq);
  EscalaDefault[Y] =  Resolucao[Y] / (LimiteSup - LimiteInf);

  EscalaDefault[X] < 	EscalaDefault[Y] ? 	EscalaDefault[Y] = 	EscalaDefault[X] : 	EscalaDefault[X] = 	EscalaDefault[Y];

  if(EscalaDefault[X]< .001)									//--- A escala est� muito pequena, provavelmente o projeto est� muito longe do terreno, erro de coordenadas.
  {																					  //--- Tenta melhorar, Recalcula a escala considerando apenas o terreno; 
    LimiteEsq = Superficie.PegaLimiteEsq();
    LimiteDir = Superficie.PegaLimiteDir();
    LimiteSup = Superficie.PegaLimiteSup();
    LimiteInf = Superficie.PegaLimiteInf();

    EscalaDefault[X] =  Resolucao[X] / (LimiteDir - LimiteEsq);
    EscalaDefault[Y] =  Resolucao[Y] / (LimiteSup - LimiteInf);

    EscalaDefault[X] < 	EscalaDefault[Y] ? 	EscalaDefault[Y] = 	EscalaDefault[X] : 	EscalaDefault[X] = 	EscalaDefault[Y];
  }

  if(EscalaDefault[X]< .001)	EscalaDefault[X] = EscalaDefault[Y] = 0.001;


  //--- coordenadas do canto inferior esquerdo, real e rotacionado

  DeltasReais.x = DeltasDefault[X] = LimiteEsq;
  DeltasReais.y = DeltasDefault[Y] = LimiteInf;

  LimiteYMetros.y = DeltasReais.y + LarguraVideo * sin(AnguloRotacao);
  LimiteYMetros.x = DeltasReais.x + AlturaVideo * sin(AnguloRotacao);

  CalculaParametros();
}

void DesPontosGeomView::OnUpdateButrotacionar(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( Superficie.pSuperficieAtual->size() > 0);
}

void DesPontosGeomView::OnUpdateRedesenhar(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( Superficie.pSuperficieAtual->size() > 0);
}

void DesPontosGeomView::OnUpdateEnquadrar(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(Superficie.pSuperficieAtual->size() > 0);
}

void DesPontosGeomView::OnDefinebordos() 
{
  /*
  if(Superficie.DefineFronteiraOuPoligono(CSuperficie::TIPOFRONTEIRA)) AlterouTerreno = true;
  if(Superficie.DefineFronteiraOuPoligono(CSuperficie::TIPOPOLIGONO)) AlterouTerreno = true;
  */

  DesenharPontosInseridos = ! DesenharPontosInseridos;

  RedrawWindow();
}

void DesPontosGeomView::CriaFontes(bool DestruirAnteriores)
{
  if (DestruirAnteriores)    //--- Se � a 2a vez � preciso destruir as anteriores
  {
    CurrierNewHorizontal.DeleteObject();
    CurrierNewVertical.DeleteObject();
  }

  CDC* pDC = GetDC();
  if(!pDC) return;                   //--- Seguran�a, aconteceu nos testes

  CFont* LF = pDC->GetCurrentFont();  
  LOGFONT Log;

  LF->GetLogFont(&Log);
  Log.lfEscapement = AnguloRotacao * 1800 / 3.14;

  strcpy(Log.lfFaceName,"Arial");
  Log.lfHeight = 18;
  CurrierNewHorizontal.CreateFontIndirect(&Log);

  Log.lfEscapement = (AnguloRotacao + 3.14/2.0)* 1800 / 3.14;
  Log.lfHeight = 18;
  strcpy(Log.lfFaceName,"Arial");

  CurrierNewVertical.CreateFontIndirect(&Log);

  ReleaseDC(pDC);
}

void DesPontosGeomView::CriaFontesCN(bool DestruirAnteriores,double Angulo)
{
  if (DestruirAnteriores)    //--- Se � a 2a vez � preciso destruir as anteriores
  {
    CourierNewCN.DeleteObject();
  }

  CDC* pDC = GetDC();
  if(!pDC) return;                   //--- Seguran�a, aconteceu nos testes

  CFont* LF = pDC->GetCurrentFont();  
  LOGFONT Log;

  LF->GetLogFont(&Log);

  Log.lfEscapement = Angulo * 1800.0 / 3.14;
  Log.lfHeight = 18;
  strcpy(Log.lfFaceName,"Arial");

  CourierNewCN.CreateFontIndirect(&Log);

  ReleaseDC(pDC);
}

void DesPontosGeomView::CalculaNovasDimensoes()
{
  AlturaRebatidaYPixels = AlturaVideoPixels * cos(AnguloRotacao);
  LarguraRebatidaYPixels = LarguraVideoPixels * -sin(AnguloRotacao);
  NovaAlturaPixels =  AlturaRebatidaYPixels + LarguraRebatidaYPixels;
  NovaLarguraPixels = LarguraVideoPixels * cos(AnguloRotacao) + AlturaVideoPixels * -sin(AnguloRotacao);
}

void DesPontosGeomView::OnSetFocus(CWnd* pOldWnd) 
{
  CView::OnSetFocus(pOldWnd);

  CMainFrame* MainFrame = (CMainFrame*)AfxGetMainWnd();

  //--- Se a janela da se��o ainda n�o foi destruida coloca-a no topo

  //--- N�o consegui usar o WS_EX_PALLETTEWINDOW nas on_Create ou no on_Create_EX, tanto nas views 
  //--- como no childfrm....

  // DesSecaoView* CW = (DesSecaoView *)MainFrame->DesSecao;
  // if (CW) CW->GetParentFrame()->BringWindowToTop();

  if (MainFrame->DesSecao)((DesSecaoView *)MainFrame->DesSecao)->GetParentFrame()->BringWindowToTop();

  //	 ((CChildFrame*) GetParentFrame())->DeslizanteGeom.SetFocus();

  //PostMessage(, WM_SETFOCUS, 0, 0);
  ((CChildFrame*) GetParentFrame())->DeslizanteGeom.PostMessage(WM_SETFOCUS, 0, 0);  //--- SetFocus anula o mousewheel

  /*    CChildFrame* pp =  (CChildFrame*)GetParentFrame();

  //if(pp->Deslizantes.GetWindow(GW_OWNER))
  {
  ((CChildFrame*) GetParentFrame())->DeslizanteGeom.SetFocus();
  }
  */  
}

void DesPontosGeomView::OnAcitopog() 
{
  AlterouTerreno = Superficie.DefineBordos();	

  RedrawWindow();
}

void DesPontosGeomView::OnDefAciTopog() 
{
  //  if(!NaoPermitirAlteracoes) DefBordos = false;

  CChildFrame* CCFrm =  (CChildFrame*) GetParentFrame();
  CCFrm->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTBORDOS,DefBordos > 0 ? TBSTATE_ENABLED : TBSTATE_PRESSED);
  CCFrm->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTACITOPOG,DefBordos > 0 ? TBSTATE_PRESSED : TBSTATE_ENABLED);

  DefBordos = false;
}

void DesPontosGeomView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  if(NaoPermitirAlteracoes) return;

  if(CriandoRestricao ||  PegandoPontoSecAvulsa)
  {
    CriandoRestricao = false;
    PegandoPontoSecAvulsa = false;

    RedrawWindow();

    return;
  }

  Ponto P;
  TransfPixelsXY(point,P.x,P.y);
  P.RotacionaPlano(Superficie.PegaSeno(),Superficie.PegaCoseno(),Superficie.PegaAnguloRotacao());

  {
    //--- Se a distancia entre o o ponto clicado e alguma tangente estiver dentro da tolerancia,
    //--- insere um pi com raio padr�o neste ponto e coloca o programa no modo de arrastar ponto, deste modo o ponto inserido 
    //--- poder� ser� arrastado com os movimentos do mouse. No itCHAnterior retorna a curva anterior � que est� sendo inserida
    //--- o pi atual tem que ser inserido apos esta curva, para isso as estaca do Ts,Sc,Cs e ST s�o feitas a principio
    //--- iguais �s do Ts da curva anterior + 0,00005. Assim que a curva for recalculada atrav�s da chamada a AlteraCurva, o estaqueamento 
    //--- ser� corrigido.

    ItCMCurHorizontaisTS ItCHAnterior(mapCurvasHorizontaisTS.begin());
    bool Inseriu(false);
    CurHorizontal CH;

    if(mapCurvasHorizontaisTS.size()  < 2)
    {
      if(mapCurvasHorizontaisTS.size()==0)  CH.EstTs = CH.EstSc = CH.EstCs = CH.EstSt = 0.0;
      else  CH.EstTs = CH.EstSc = CH.EstCs = CH.EstSt = ItCHAnterior->second.EstSt.EstVirtual + 0.01;
      CH.Raio = 0.0;

      Inseriu = true;
    }
    else
      if (VerificaTangente(P,ItCHAnterior))  
      {
        CEstaca TsAux = ItCHAnterior->second.EstSt;
        TsAux += 0.005;                             //--- Para que a curva fique na posi��o correta no set ordenado por TS

        CH.EstTs = CH.EstSc = CH.EstCs = CH.EstSt = TsAux;
        CH.Raio = 50.0;

        Inseriu = true;
      }

      if(Inseriu)
      {
        //  CurvasHorizontais.InsereCurva(CH);
        CurvasHorizontais.InsereCurvaNosMapas(CH);

        ItCurHorizSendoArrastadaPI = mapCurvasHorizontaisPI.find(&CH.PI) ;

        if(CurvasHorizontais.AlteraCurvaPI(ItCurHorizSendoArrastadaPI,P,ItUltimaCurvaAjustadaTS,NULL) !=CCurHorizontais::CURVA_OK)
          monolog.mensagem(25,-1);

        Arrastando = TerminarArrasto = false;   //--- Termina o arrasto
        OnLButtonDown(nFlags, point);
      }
  }
}

void DesPontosGeomView::OnConfdes() 
{
  int Dados[23];

  Dados[0] = DesenharPixel;
  Dados[1] = DesenharCirculo;
  Dados[2] = DesenharCruz; 
  Dados[3] = DesenharSecoes; 
  Dados[4] = DesenharArestas; 
  Dados[5] = DesenharBordos; 
  Dados[6] = DesenharRestricoes; 
  Dados[7] = DesenharPontos; 
  Dados[8] = MostrarDescricao; 
  Dados[9] = DesenharMalha;
  Dados[10] = EquiMalha;
  Dados[11] = EquiCotas;
  Dados[12] = DesenharProjeto;
  Dados[13] = CircularSecoes;
  Dados[14] = MostrarToolBarSuperf;
  Dados[15] = MostrarToolBarGeom;
  Dados[16] = CotarMalha;
  Dados[17] = VarrerTerreno;
  Dados[18] = VarrerProjeto;
  Dados[19] = VarrerCN;
  Dados[20] = DesenharCN;
  Dados[21] = CotarCN;
  Dados[22] = SaltarCNs;

  CDConfDesGeom DConfDesGeometrico(this,&Dados[0],&Cores[0],true);

  DConfDesGeometrico.DoModal();

  DesenharPixel = Dados[0] != false;
  DesenharCirculo = Dados[1] != false;
  DesenharCruz = Dados[2] != false;
  DesenharSecoes = Dados[3] != false;
  DesenharArestas = Dados[4] != false;
  DesenharBordos = Dados[5] != false;
  DesenharRestricoes = Dados[6] != false;
  DesenharPontos = Dados[7] != false;
  MostrarDescricao = Dados[8] != false;
  DesenharMalha = Dados[9] != false;
  EquiMalha = Dados[10];
  EquiCotas = Dados[11];
  DesenharProjeto = Dados[12] != false;
  CircularSecoes = Dados[13] != false;
  MostrarToolBarSuperf = Dados[14] != false;
  MostrarToolBarGeom = Dados[15] != false;
  CotarMalha = Dados[16] != false;
  VarrerTerreno = Dados[17] != false;
  VarrerProjeto = Dados[18] != false;
  VarrerCN = Dados[19] != false;
  DesenharCN = Dados[20] != false;
  CotarCN = Dados[21] != false;
  SaltarCNs = Dados[22];

  AlterouAngulo = true;

  RedrawWindow();
}

void DesPontosGeomView::IconizaSecao()
{ 
  CMainFrame* MainFrame =  (CMainFrame*) AfxGetMainWnd();
  DesSecaoView* pp = (DesSecaoView *)MainFrame->DesSecao;
  if (!pp) return;
  pp->GetParentFrame()->ShowWindow(SW_MINIMIZE);   
  pp->MostrarVolumes(false);
}

void DesPontosGeomView::IconizarRestaurarSecao(int Modo)
{ 
  CMainFrame* MainFrame =  (CMainFrame*) AfxGetMainWnd();
  DesSecaoView* CWDesSecao = (DesSecaoView *)MainFrame->DesSecao;
  if (!CWDesSecao ) return;
  CWDesSecao ->GetParentFrame()->ShowWindow(Modo);   
  CWDesSecao ->MostrarVolumes(Modo == SW_RESTORE);
}

void DesPontosGeomView::OnSecsemprevisIconizar() 
{
  SecoesIconizadas = !SecoesIconizadas;

  IconizarRestaurarSecao(SecoesIconizadas ? SW_MINIMIZE : SW_RESTORE);
}

void DesPontosGeomView::OnSizing(UINT fwSide, LPRECT pRect) 
{
  CView::OnSizing(fwSide, pRect);

  RECT Coord; 
  GetClientRect(&Coord);

  Resolucao[X] = Coord.right; 
  Resolucao[Y] = Coord.bottom;

  GetWindowRect(CRectJanPai);    	//--- Salva As novas coordenadas para saber as posi��es do pop up

  DefineParametros();
  CalculaParametros();
  CalculaNovasDimensoes();
  OnAlterouAngulo(GetDC());
}

void DesPontosGeomView::GravarSuperficie() 
{
  int Resposta,PrimeiraResposta(!IDOK);

  do
  {
    PrimeiraResposta = dialogar.DoModal(4);

    if (PrimeiraResposta != IDOK && Destruindo) 
    {
      Resposta = dialogar.DoModal(6);
    }
  }while (Destruindo && PrimeiraResposta != IDOK && Resposta  != IDOK);

  if(PrimeiraResposta == IDOK)
  {
    if (AlterouTerreno) 
    { 
      Superficie.CriaListaBordos();   //--- A fronteira agora � uma restri��o. Passa-a para a lista antiga por compatibilidade com os outros modulos
      Superficie.Serialize(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()),GRAVAR,false,false);
      GravaAcidentesTopograficos();
    }
    if (AlterouProjeto) 
    {
      ((CMainFrame*) AfxGetMainWnd())->GeometricoAlterado = true;
      CurvasHorizontais.Serialize(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()),GRAVAR);
      CurvasHorizontais.CalcGeometrico(dialogar.texto,NULL,NULL,0.0005,&CProjeto(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual())),false);  //--- <<< Nunca esta indo true
      GravaCoordSecoes(); 
    }
    if(AlterouSecoesAvulsas) SecoesAvulsas.Serialize(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()).GetBuffer(),GRAVAR);

    AlterouTerreno = AlterouProjeto = AlterouSecoesAvulsas = false;     
  }
}

void DesPontosGeomView::OnIrparaponto() 
{
  CDCoordenadas DCoordenadas(this);

  if(DCoordenadas.DoModal() == IDOK)
  {
    //--- Acerta os deltas para deslocar o desenho.

    CPoint P((int) (DCoordenadas.m_doCoorE),(int) (DCoordenadas.m_doCoorN));

    Deltas[X] = P.x;
    Deltas[Y] = P.y;

    DeltasReais.x = Deltas[X] -= LarguraVideo / 2.0; 
    DeltasReais.y = Deltas[Y] -=  AlturaVideo / 2.0;

    //CalculaParametros();

    //--- Desenha a cruz vermelha no ponto

    CPoint PPixels;

    PPixels.x = ((P.x - Deltas[X]) * Escala[X]);
    PPixels.y = (AlturaVideo - (P.y - Deltas[Y])) * Escala[Y] ;

    CDC* pDC = GetDC();
    if(!pDC) return;              //--- Seguran�a, aconteceu nos testes

    CPen PenaMalha,*pPenaAnterior; 										
    PenaMalha.CreatePen(PS_SOLID,2,RGB(255,0,0));
    pPenaAnterior = pDC->SelectObject(&PenaMalha);		

    OnArrastouDesenho();
    RedrawWindow();

    //--- Desenha a cruz, 14 pixels de largura

    pDC->MoveTo(PPixels.x-15,PPixels.y);
    pDC->LineTo(PPixels.x+15,PPixels.y);		
    pDC->MoveTo(PPixels.x,PPixels.y-15);
    pDC->LineTo(PPixels.x,PPixels.y+15);

    pDC->Arc(PPixels.x-4,PPixels.y-4,PPixels.x+4,PPixels.y+4,0,0,0,0);    //--- Circula o ponto.

    pDC->SelectObject(pPenaAnterior); 

    ReleaseDC(pDC); 
  }
}

BOOL DesPontosGeomView::PreCreateWindow(CREATESTRUCT& cs) 
{
  cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, // use any window styles
    AfxGetApp()->LoadStandardCursor(IDC_HAND),
    (HBRUSH) (COLOR_WINDOW + 1));         // background brush

  return CView::PreCreateWindow(cs);
}

int DesPontosGeomView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CView::OnCreate(lpCreateStruct) == -1) return -1;

  // Remove positions of menu
  CMenu* pTopMenu = GetSystemMenu(FALSE);
  if(pTopMenu != NULL)
  {
    pTopMenu -> RemoveMenu(SC_SIZE, MF_BYCOMMAND);      //Size
    pTopMenu -> RemoveMenu(SC_MOVE, MF_BYCOMMAND);      //Move
    pTopMenu -> RemoveMenu(SC_MAXIMIZE, MF_BYCOMMAND);  //Maximize
  }

  return 0;
}

void DesPontosGeomView::OnAlternartituloDesGeom() 
{
  CChildFrame* CCFrm((CChildFrame*) GetParentFrame());
  unsigned long MostrarTitulo(CCFrm->GetStyle() & WS_CAPTION);

  if (MostrarTitulo) CCFrm->ModifyStyle(WS_CAPTION,0,SWP_FRAMECHANGED);    
  else CCFrm->ModifyStyle(0,WS_CAPTION,SWP_FRAMECHANGED);   

  CCFrm->ShowControlBar(&CCFrm->m_wndStatusBar,!MostrarTitulo,true);
}

void DesPontosGeomView::OnButFechar() 
{
  ((CChildFrame*) GetParentFrame())->Fechar();
}

void DesPontosGeomView::CriaMapaSecoesGeomTemp(const CurHorizontal& Curva,MSecaoGeom& MSecoesGeomTemp)
{
  //--- quando o usu�rio esta alterando o geometrico as estacas dos pontos notaveis da nova curva n�o s�o mais 
  //--- encontradas no mapa de se��es (So no reseccionamento elas ser�o novamente encontradas). Esta func�o cria 
  //--- um mapa tempor�rio s� para atender este requisito para a curva passada. assim a espiral dela poder� ser 
  //--- desenhada usando-se a mesma fun��o (DesenhaEspiral). S�o inclu�dos 4 pontos intermedi�rios e exponencialmente
  //--- distanciados nas espirais para que elas possam ser desenhadas.

  static double Fatores[4] = {1.0/2.0,3.0/4.0,7.0/8.0,15.0/16.0};   //--- Os pontos est�o a : 1/2 3/4 7/8 E 15/16 ao longo da curva

  //--- Insere os pontos not�veis da curva

  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstTs,Curva.TS));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstSc,Curva.SC));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstCs,Curva.CS));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstSt,Curva.ST));

  //--- Calcula 2 pontos intermedi�rios na 1a transi��o e os insere no mapa

  double xEsp[4],yEsp[4],s[4],Corda[4],Deflexao[4];

  CCurHorizontais::CalcxyEsp(Curva,&xEsp[0],&yEsp[0],&s[0],Curva.Lc * Fatores[0]);
  Corda[0] = hypot(xEsp[0],yEsp[0]);
  Deflexao[0] = atan(xEsp[0]/yEsp[0]) * (Curva.Ac < 0.0 ? -1.0 : 1.0);

  CCurHorizontais::CalcxyEsp(Curva,&xEsp[1],&yEsp[1],&s[1],Curva.Lc * Fatores[1]);
  Corda[1] = hypot(xEsp[1],yEsp[1]);
  Deflexao[1] = atan(xEsp[1]/yEsp[1])* (Curva.Ac < 0.0 ? -1.0 : 1.0);

  CCurHorizontais::CalcxyEsp(Curva,&xEsp[2],&yEsp[2],&s[2],Curva.Lc * Fatores[2]);
  Corda[2] = hypot(xEsp[2],yEsp[2]);
  Deflexao[2] = atan(xEsp[2]/yEsp[2])* (Curva.Ac < 0.0 ? -1.0 : 1.0);

  CCurHorizontais::CalcxyEsp(Curva,&xEsp[3],&yEsp[3],&s[3],Curva.Lc * Fatores[3]);
  Corda[3] = hypot(xEsp[3],yEsp[3]);
  Deflexao[3] = atan(xEsp[3]/yEsp[3])* (Curva.Ac < 0.0 ? -1.0 : 1.0);

  Ponto P1(sin(Curva.TS.fi + Deflexao[0]) * Corda[0] + Curva.TS.x,cos(Curva.TS.fi + Deflexao[0]) * Corda[0] + Curva.TS.y,INFINITO,INFINITO,s[0]+Curva.TS.fi),
    P2(sin(Curva.TS.fi + Deflexao[1]) * Corda[1] + Curva.TS.x,cos(Curva.TS.fi + Deflexao[1]) * Corda[1] + Curva.TS.y,INFINITO,INFINITO,s[1]+Curva.TS.fi),
    P3(sin(Curva.TS.fi + Deflexao[2]) * Corda[2] + Curva.TS.x,cos(Curva.TS.fi + Deflexao[2]) * Corda[2] + Curva.TS.y,INFINITO,INFINITO,s[2]+Curva.TS.fi),
    P4(sin(Curva.TS.fi + Deflexao[3]) * Corda[3] + Curva.TS.x,cos(Curva.TS.fi + Deflexao[3]) * Corda[3] + Curva.TS.y,INFINITO,INFINITO,s[3]+Curva.TS.fi);

  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstTs.EstVirtual + Curva.Lc * Fatores[0],P1));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstTs.EstVirtual + Curva.Lc * Fatores[1],P2));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstTs.EstVirtual + Curva.Lc * Fatores[2],P3));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstTs.EstVirtual + Curva.Lc * Fatores[3],P4));

  //--- Calcula 2 pontos na 2a transi��o e os insere no mapa 

  P1.x = sin(Curva.ST.fi - Deflexao[0] + Mat.M_PI) * Corda[0] + Curva.ST.x;
  P1.y = cos(Curva.ST.fi - Deflexao[0] + Mat.M_PI) * Corda[0] + Curva.ST.y;
  P1.fi = Curva.ST.fi - s[0];
  CCurHorizontais::AcertaAngulo(&P1.fi);

  P2.x = sin(Curva.ST.fi - Deflexao[1] + Mat.M_PI) * Corda[1] + Curva.ST.x;
  P2.y = cos(Curva.ST.fi - Deflexao[1] + Mat.M_PI) * Corda[1] + Curva.ST.y;
  P2.fi = Curva.ST.fi - s[1];
  CCurHorizontais::AcertaAngulo(&P2.fi);

  P3.x = sin(Curva.ST.fi - Deflexao[2] + Mat.M_PI) * Corda[2] + Curva.ST.x;
  P3.y = cos(Curva.ST.fi - Deflexao[2] + Mat.M_PI) * Corda[2] + Curva.ST.y;
  P3.fi = Curva.ST.fi - s[2];
  CCurHorizontais::AcertaAngulo(&P3.fi);

  P4.x = sin(Curva.ST.fi - Deflexao[3] + Mat.M_PI) * Corda[3] + Curva.ST.x;
  P4.y = cos(Curva.ST.fi - Deflexao[3] + Mat.M_PI) * Corda[3] + Curva.ST.y;
  P4.fi = Curva.ST.fi - s[3];
  CCurHorizontais::AcertaAngulo(&P4.fi);

  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstSt.EstVirtual - Curva.Lc * Fatores[0],P1));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstSt.EstVirtual - Curva.Lc * Fatores[1],P2));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstSt.EstVirtual - Curva.Lc * Fatores[2],P3));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstSt.EstVirtual - Curva.Lc * Fatores[3],P4));
}

void DesPontosGeomView::OnGravar()
{
  ((CMainFrame*) AfxGetMainWnd())->GeometricoAlterado = true;
  GravarSuperficie();
}

void DesPontosGeomView::OnCalcgeometrico() 
{
  // Reavaliar o uso desta fun��o, quando o geometrico � alterado sempre ser� preciso retocar o greide, Domodo que esta o greide 
  // n�o � retocado, ficam as se�oes antigas no desenho das se��es o que n�o � interessante...

  //---->> Desligada: Quando � feita alguma altera��o no projeto geom�trico o greide muda drasticamente pois as estacas s�o reposicionadas
  //                  Deste modo o greide teria que ser todo retocado a n�o ser que se usasem igualdades que j� existem no projeto mas n�o 
  //                  est�o completamente implementadas, ent�o fica para uma segunda vers�o

  return;

  bool Resecionar(false);

  if(dialogar.DoModal(12,"",&Resecionar) == IDOK)
  {
    CMainFrame* pMainFrame =((CMainFrame*)((CMainFrame*)AfxGetMainWnd()));

    if (AlterouProjeto) CurvasHorizontais.Serialize(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()),GRAVAR);

    if (pMainFrame->DesSecao) ((DesSecaoView*)((CMainFrame*)pMainFrame)->DesSecao)->GravarSecoes(false);

    //--- Calcula o Projeto Geom�trico
    //--- O dialogo chamado � o DCalprojeto mas ele � modal ent�o n�o o mostra e usa o 
    //--- CDCalcGeoReseccDireto para mostrar o progress e as mensagens

    CDCalcGeoSeccDireto DCalcGeoSeccDireto(this);
    DCalcGeoSeccDireto.Create(this);                      //--- Direto, n�o modal
    CDCalcGeometrico DCalcGeometrico(&DCalcGeoSeccDireto,CString(pMainFrame->PegaProjetoAtual()));
    DCalcGeometrico.Create(this,&DCalcGeoSeccDireto);      //--- Direto, n�o modal
    DCalcGeoSeccDireto.ShowWindow(true);

    DCalcGeometrico.CalcularDireto();  

    //--- Agora recessiona o  terreno com os dados de projeto recem calculados 
    //--- Com a mesma altera��o nos di�logos

    CString NomeProjeto(CString(pMainFrame->PegaProjetoAtual()));

    CDSecionar DSecionar(NomeProjeto,this);

    if(Resecionar)
    {
      //--- n�o esta funcionando n�o modal, verificar

      //   DCalcGeoSeccDireto.ShowWindow(false);


      //   DSecionar.Create(0,this,&DCalcGeoSeccDireto);
      //   DSecionar.ShowWindow(true);
      //   DSecionar.CalcularDireto();

      DSecionar.DoModal();

      WaitForSingleObject(CEvent(),500);      //--- Aguarda 0,7 segundos para sair

      AlterouProjeto = AlterouTerreno = false;
    }

    //	dialogar.RadSeccionar.EnableWindow(false);
    //--- A nota de servi�o foi anulada com o novo rec�lculo do geom�trico. Ent�o
    //--- renomeia o arquivo da nota de servi�o (.not)  para .not.anterior. Em caso de erro n�o faz nada...

    /*
    CString NomeNovo(NomeProjeto + ".not.anterior"),NomeVelho(CString(NomeProjeto + ".not"));
    CFile CFNota;

    if(CFNota.Ope1n(NomeVelho,CFile::modeRead|CFile::shareExclusive))      //--- Se j� existir um arquivo .not
    {
    CFNota.Close();

    try { CFile::Remove(NomeVelho);} catch(...) {}                //--- Remove o backup anterior .not.anterior (se existir)    
    try { CFile::Rename(NomeVelho,NomeNovo);} catch(...) {}       //--- Renomeia o .not para .not.anterior
    }
    */

    LeCoordSecoes();    //--- rele as novas coordenadas das se��es

    //--- Se o desenho das secoes estiver aberto, l� novamente as se��es para atualizar os volumes e os desenho das secoes

    if (pMainFrame->DesSecao) ((DesSecaoView*)(pMainFrame->DesSecao))->ReleSecoes();

    RedrawWindow();
  }
}

void DesPontosGeomView::OnSecaoAvulsa() 
{
  PegandoPontoSecAvulsa = !PegandoPontoSecAvulsa;
  CChildFrame* CCFrm =  (CChildFrame*) GetParentFrame();
  CCFrm->m_wndToolBarPonGeo.CheckDlgButton(ID_BUTSECAVULSA,PegandoPontoSecAvulsa);
}

void DesPontosGeomView::OnTipoCN() 
{
  DesCNTopograficas = !DesCNTopograficas;
  CChildFrame* CCFrm =  (CChildFrame*) GetParentFrame();
  CCFrm->m_wndToolBarPonGeo.CheckDlgButton(ID_BUTCNTOPOGRAF,DesCNTopograficas);

  RedrawWindow();
}

void DesPontosGeomView::OnBordos() 
{
  // if(!NaoPermitirAlteracoes) DefBordos = true;

  CChildFrame* CCFrm =  (CChildFrame*) GetParentFrame();
  CCFrm->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTBORDOS,DefBordos == 0 ? TBSTATE_ENABLED : TBSTATE_PRESSED);
  CCFrm->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTACITOPOG,DefBordos == 0 ? TBSTATE_PRESSED : TBSTATE_ENABLED);

  //else ((CChildFrame*) GetParentFrame())->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTBORDOS,false);

  DefBordos = true;
}

void DesPontosGeomView::OnSincronizarSecoes()
{
  if((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->DesSecao)
  {
    //CEstApp::OnDesenhosSecoes();
    SincronizarSecoes = !SincronizarSecoes;
  }
}

void DesPontosGeomView::OnUpdateButSincro(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(((CMainFrame*)AfxGetMainWnd())->DesSecao != NULL);	
}


void DesPontosGeomView::OnNaoPermitirAlteracoes()
{
  NaoPermitirAlteracoes = !NaoPermitirAlteracoes;

  /*
  if(NaoPermitirAlteracoes) 
  {
  DefBordos = false;
  ((CChildFrame*) GetParentFrame())->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTBORDOS,false);
  }
  */
}

void DesPontosGeomView::GravaCoordSecoes() 
{
  CString NomeProjeto(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()));

  CFile ArqCalcSec;
  CFileException e;

  if (ArqCalcSec.Open(NomeProjeto + CString(".pes"),CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone,&e) == 0)
    monolog.mensagem(13,NomeProjeto + CString(".pes"));
  else
  {
    CArchive CArqCalcSec(&ArqCalcSec,CArchive::store);

    //--- Grava coordenadas de todas as se��es. (os pontos de 3 em 3 j� est�o na ordem de estaca)
    //--- O mapa de se��es est� sincronizado com a lista dos pontos extremos das se��es (que merda......acertar isso!!!)

    Ponto PEsq,PDir,PEixo;
    CEstaca Estaca;

    CArqCalcSec << (int) (LPSecoes.size() / 3); 

    for(register itMSecaoGeom itSecaoGeom = MapaSecoesGeom.begin() ; itSecaoGeom != MapaSecoesGeom.end() ; itSecaoGeom++)
    {
      Estaca = (*itSecaoGeom).first;

      //  CArqCalcSec << Estaca;
      Estaca >> CArqCalcSec;

      PEsq = *(LPSecoes.begin());
      PEsq >> CArqCalcSec;
      LPSecoes.pop_front();
      PEsq = *(LPSecoes.begin());
      PEsq >> CArqCalcSec;
      LPSecoes.pop_front();
      PEsq = *(LPSecoes.begin());
      PEsq >> CArqCalcSec;
      LPSecoes.pop_front();
    }

    CArqCalcSec.Close(); 
  }
}

void DesPontosGeomView::LeCoordSecoes() 
{
  CString NomeProjeto(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()));

  CFile ArqCalcSec;
  CFileException e;
  int QSecoes(0);
  Ponto PontoACalc;

  if (ArqCalcSec.Open(NomeProjeto + CString(".pes"),CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    if (!((CMainFrame*)AfxGetMainWnd())->PegaSilencioso())  
      monolog.mensagem(13,NomeProjeto + CString(".pes"));
  }
  else
  {
    CArchive CArqCalcSec(&ArqCalcSec,CArchive::load);
    Ponto PEsq,PDir,PEixo;
    CEstaca EstacaAtual;

    //--- Se estiver relendo remove os dados anteriores

    LPSecoes.clear();

    //--- Busca coordenadas de todas as se��es. colocando-os numa lista (os pontos de 3 em 3 j� est�o na ordem de estaca)
    try
    {
      for (CArqCalcSec >> QSecoes ; QSecoes ; QSecoes--)  
      {
        EstacaAtual << CArqCalcSec;
        PDir << CArqCalcSec;
        PEixo << CArqCalcSec;
        PEsq << CArqCalcSec;
        /*
        if(Superficie.EstaRotacionada()) 
        {
        PDir.RotacionaPlano(Seno,Coseno,AnguloRotacao);
        PEixo.RotacionaPlano(Seno,Coseno,AnguloRotacao);
        PEsq.RotacionaPlano(Seno,Coseno,AnguloRotacao);
        }
        */
        LPSecoes.push_back(PDir);
        LPSecoes.push_back(PEixo);
        LPSecoes.push_back(PEsq);

        //--- Mapa de estacas ordenado por coordenadas, usado no onmousemove para mostrar as coordenadas do ponto
        //--- Os pontos extremos poderia estar aqui tb...corrigir

        MapaSecoesGeom.insert(MSecaoGeom::value_type(EstacaAtual,PEixo));
      }
    }
    catch(...)
    {
       monolog.mensagem (13,NomeProjeto + CString(".pes") + CString(".\nReseccione todo o projeto novamente."));
    }

    CArqCalcSec.Close();
    ArqCalcSec.Close(); 
  }

  MapaSecoesPonto.clear();          

  for(register itMSecaoGeom itmapAtual = MapaSecoesGeom.begin() ; itmapAtual != MapaSecoesGeom.end() ; itmapAtual++)
    MapaSecoesPonto.insert(MSecaoGeomPonto::value_type(&(*itmapAtual).second,&(*itmapAtual).first));
}

void DesPontosGeomView::RotacionaSecoes(double Seno,double Coseno,double AngRotacao)
{
  //--- rotaciona o mapa com os 3 pontos, ordenado por estaca e o mapa ordenado pelo ponto do eixo

  for (register itlstPontos it = LPSecoes.begin() ; it != LPSecoes.end() ; it++)  
    (*it).RotacionaPlano(Seno,Coseno,0.0);

  for(register itMSecaoGeom itmapAtual = MapaSecoesGeom.begin() ; itmapAtual != MapaSecoesGeom.end() ; itmapAtual++) 
    (*itmapAtual).second.RotacionaPlano(Seno,Coseno,0.0);
}

void DesPontosGeomView::RotacionaBordoseRestricoes(double Sin,double Cos,double Angulo)
{
  for (int i = BORDOS ; i <= RESTRICOES ; i++)
  {
    LLDeltaSup& ListaRestAtual = i == BORDOS ? Superficie.PegaListaBordos() : Superficie.PegaListaRestricoes();

    for(register ItLLDeltaSup LDRestricaoAtual = ListaRestAtual.begin() ; LDRestricaoAtual != ListaRestAtual.end() ; LDRestricaoAtual++)
    {				
      for(register ItLDeltaSup ItLRestricao = (*LDRestricaoAtual).begin(); ItLRestricao != (*LDRestricaoAtual).end() ; ItLRestricao++)
        (*ItLRestricao).PCentral.RotacionaPlano(Sin,Cos,Angulo);   
    }
  }
}

void inline DesPontosGeomView::TransfPixelsXY(CPoint& Ponto,double& Coorx,double& Coory)
{
  //--- Mapeia as cordenadas do ponto que est�o em pixels para metros.

  Coorx = ((double)Ponto.x) / Escala[X] + Deltas[X];
  Coory = AlturaVideo - ((double) Ponto.y) / Escala[Y] + Deltas[Y];

  if(Superficie.EstaRotacionada()) 
  {
    double Angulo = -Superficie.PegaAnguloRotacao();

    RotacionaPontoPlano(sin(Angulo),cos(Angulo),Angulo,&Coorx,&Coory);
  }
}
void  DesPontosGeomView::DesenhaMalha(CDC* pDC)
{
  if(LPontosCoordX.size() == 0) return;

  itlistPontosMalha itPrim(LPontosCoordX.begin()),itSec(itPrim);
  itSec++;

  //--- Verifica se a malha est� muito fina se estiver n�o desenha

  if(LPontosCoordX.size() > 1 && abs((*itPrim).Pontos.first.y - (*itSec).Pontos.first.y) < 3 &&
    abs((*itPrim).Pontos.first.x - (*itSec).Pontos.first.x) < 3) return;    //--- Tem que ter pelo menos um pixel entre as coordenadas

  register itlistPontosMalha it(LPontosCoordX.begin()); 

  for(; it != LPontosCoordX.end() ; it++)
  {
    pDC->MoveTo((*it).Pontos.first.x,(*it).Pontos.first.y);
    pDC->LineTo((*it).Pontos.second.x,(*it).Pontos.second.y); 
  }

  pDC->SelectObject(&CurrierNewHorizontal);
  pDC->SetTextAlign(TA_BOTTOM | TA_LEFT);
  pDC->SetTextColor(Cores[CORTEXTO]);
  pDC->SetBkColor(Cores[CORFUNDO]);

  int Contador(0);

  for(it = LPontosCoordX.begin() ; CotarMalha && it != LPontosCoordX.end() ; it++)
    if(Contador++ % EquiCotas == 0)
      pDC->TextOut((*it).PontoCota.x,(*it).PontoCota.y,(*it).Cota.c_str());

  //----------------- coordenadas y

  if(LPontosCoordY.size() == 0) return;

  for(it = LPontosCoordY.begin() ; it != LPontosCoordY.end() ; it++)
  {
    pDC->MoveTo((*it).Pontos.first.x,(*it).Pontos.first.y);
    pDC->LineTo((*it).Pontos.second.x,(*it).Pontos.second.y); 
  }

  pDC->SelectObject(&CurrierNewVertical);
  Contador = 0;

  for(it = LPontosCoordY.begin() ; CotarMalha && it != LPontosCoordY.end() ; it++)
    if(Contador++ % EquiCotas == 0)
      pDC->TextOut((*it).PontoCota.x,(*it).PontoCota.y,(*it).Cota.c_str());
}

void  DesPontosGeomView::OnAlterouAngulo(CDC* pDC)
{
  void (DesPontosGeomView::*pFunc[4])(CDC*) ={&DesPontosGeomView::OnAlterouAnguloQ1,
    &DesPontosGeomView::OnAlterouAnguloQ2,
    &DesPontosGeomView::OnAlterouAnguloQ3,
    &DesPontosGeomView::OnAlterouAnguloQ4}; 

  (*this.*pFunc[Quadrante])(pDC);
}

void DesPontosGeomView::OnArrastouDesenho()
{
  void (DesPontosGeomView::*pFunc[4])() ={&DesPontosGeomView::OnArrastouDesenhoQ1,
    &DesPontosGeomView::OnArrastouDesenhoQ2,
    &DesPontosGeomView::OnArrastouDesenhoQ3,
    &DesPontosGeomView::OnArrastouDesenhoQ4}; 

  (*this.*pFunc[Quadrante])();
}

void  DesPontosGeomView::OnAlterouAnguloQ1(CDC* pDC)
{
  //--- Como a tela � redesenhada toda hora, o c�lculo dos pontos de in�cio e fim da malha bem como
  //--- dos pontos onde as cotas ser�o escritas � pesado e deve ser feito o m�nimo de vezes poss�vel.
  //--- Estes dados s� se alteram em arrastos  do desenho, zomms ou quando o �ngulo de rota��o � alterado.

  AnguloRotacao = Superficie.PegaAnguloRotacao();

  Seno = sin(AnguloRotacao);
  Coseno = cos(AnguloRotacao);
  Tangente = tan(AnguloRotacao);
  SenoM = sin(-AnguloRotacao);
  CosenoM = cos(-AnguloRotacao);

  Quadrante = floor(AnguloRotacao / Mat.M_PI_2);
  if(Quadrante == 4) Quadrante = 3;              //--- Ajuste de arredondamento

  CurrierNewHorizontal.DeleteObject();
  CurrierNewVertical.DeleteObject();

  CFont* LF = pDC->GetCurrentFont();  
  LOGFONT Log;

  LF->GetLogFont(&Log);
  Log.lfEscapement = AnguloRotacao * 1800.0 / 3.14;
  Log.lfHeight = 18;
  strcpy(Log.lfFaceName,"Arial");
  CurrierNewHorizontal.CreateFontIndirect(&Log);

  Log.lfEscapement = (AnguloRotacao * 1800 / 3.14)  + 2700.0;
  strcpy(Log.lfFaceName,"Arial");
  Log.lfHeight = 18;
  CurrierNewVertical.CreateFontIndirect(&Log);

  TamLetra = pDC->GetTextExtent('A');

  AlturaVideoPixels = AlturaVideo * Escala[X];
  LarguraVideoPixels = LarguraVideo * Escala[Y];
  AlturaRebatidaYPixels = AlturaVideoPixels * Coseno;
  LarguraRebatidaYPixels = LarguraVideoPixels * Seno;
  LarguraRebatidaXPixels = LarguraVideoPixels * Coseno;
  AlturaRebatidaXPixels = AlturaVideoPixels * Seno;
  NovaAlturaPixels =  AlturaRebatidaYPixels + LarguraRebatidaYPixels,
    NovaLarguraPixels = LarguraVideoPixels * Coseno + AlturaVideoPixels * Seno;

  OnArrastouDesenho();
}

void DesPontosGeomView::OnAlterouAnguloQ2(CDC* pDC)
{
  AnguloRotacao = Superficie.PegaAnguloRotacao();

  Seno = sin(AnguloRotacao);
  Coseno = cos(AnguloRotacao);
  Cotangente = 1.0 / tan(AnguloRotacao);
  Tangente = tan(AnguloRotacao);
  SenoM = sin(-AnguloRotacao);
  CosenoM = cos(-AnguloRotacao);

  Quadrante = floor(AnguloRotacao / Mat.M_PI_2);
  if(Quadrante == 4) Quadrante = 3;


  CurrierNewHorizontal.DeleteObject();
  CurrierNewVertical.DeleteObject();

  CFont* LF = pDC->GetCurrentFont();  
  LOGFONT Log;
  LF->GetLogFont(&Log);
  Log.lfEscapement = AnguloRotacao * 1800.0 / 3.14 - 900.0 ;
  Log.lfHeight = 18;
  strcpy(Log.lfFaceName,"Arial");
  CurrierNewHorizontal.CreateFontIndirect(&Log);

  Log.lfEscapement = (AnguloRotacao * 1800 / 3.14)  + 1800.0;
  Log.lfHeight = 18;
  strcpy(Log.lfFaceName,"Arial");
  CurrierNewVertical.CreateFontIndirect(&Log);

  TamLetra = pDC->GetTextExtent('A');

  AlturaVideoPixels = AlturaVideo * Escala[X];
  LarguraVideoPixels = LarguraVideo * Escala[Y];
  AlturaRebatidaYPixels = AlturaVideoPixels * Coseno;
  LarguraRebatidaYPixels = LarguraVideoPixels * Seno;
  LarguraRebatidaXPixels = LarguraVideoPixels * Coseno;
  NovaAlturaPixels =  LarguraVideoPixels * -Coseno + AlturaVideoPixels * Seno;

  OnArrastouDesenho();
}     

void DesPontosGeomView::OnAlterouAnguloQ3(CDC* pDC)
{
  AnguloRotacao = Superficie.PegaAnguloRotacao();
  Quadrante = floor(AnguloRotacao / Mat.M_PI_2);
  if(Quadrante == 4) Quadrante = 3;


  Seno = sin(AnguloRotacao);
  Coseno = cos(AnguloRotacao);
  Tangente = tan(AnguloRotacao);
  SenoM = sin(-AnguloRotacao);
  CosenoM = cos(-AnguloRotacao);

  CurrierNewHorizontal.DeleteObject();
  CurrierNewVertical.DeleteObject();

  CFont* LF = pDC->GetCurrentFont(); 

  LOGFONT Log;

  LF->GetLogFont(&Log);
  Log.lfEscapement = AnguloRotacao * 1800.0 / 3.14 - 1800.0 ;
  Log.lfHeight = 18;
  strcpy(Log.lfFaceName,"Arial");
  CurrierNewHorizontal.CreateFontIndirect(&Log);

  Log.lfEscapement = (AnguloRotacao * 1800 / 3.14)  + 900.0;
  Log.lfHeight = 18;
  strcpy(Log.lfFaceName,"Arial");
  CurrierNewVertical.CreateFontIndirect(&Log);

  TamLetra = pDC->GetTextExtent('A');

  AlturaVideoPixels = AlturaVideo * Escala[X];
  LarguraVideoPixels = LarguraVideo * Escala[Y];
  AlturaRebatidaYPixels = AlturaVideoPixels * Coseno;
  LarguraRebatidaYPixels = LarguraVideoPixels * Seno;
  LarguraRebatidaXPixels = LarguraVideoPixels * Coseno;
  AlturaRebatidaXPixels = AlturaVideoPixels * Seno;
  NovaAlturaPixels =  LarguraVideoPixels * -Coseno + AlturaVideoPixels * -Seno;
  NovaLarguraPixels = LarguraVideoPixels * -Seno + AlturaVideoPixels * -Coseno;

  OnArrastouDesenho();
}  

void DesPontosGeomView::OnAlterouAnguloQ4(CDC* pDC)
{
  AnguloRotacao = Superficie.PegaAnguloRotacao();
  Quadrante = floor(AnguloRotacao / Mat.M_PI_2);
  if(Quadrante == 4) Quadrante = 3;

  CurrierNewHorizontal.DeleteObject();
  CurrierNewVertical.DeleteObject();

  CFont* LF = pDC->GetCurrentFont();  
  LOGFONT Log;

  LF->GetLogFont(&Log);
  Log.lfEscapement = AnguloRotacao * 1800.0 / 3.14 ;
  Log.lfHeight = 18;
  strcpy(Log.lfFaceName,"Arial");
  CurrierNewHorizontal.CreateFontIndirect(&Log);

  Log.lfEscapement = (AnguloRotacao * 1800 / 3.14) + 900.0;

  Log.lfHeight = 18;
  strcpy(Log.lfFaceName,"Arial");
  CurrierNewVertical.CreateFontIndirect(&Log);

  TamLetra = pDC->GetTextExtent('A');

  Seno = sin(AnguloRotacao);
  Coseno = cos(AnguloRotacao);
  SenoM = sin(-AnguloRotacao);
  CosenoM = cos(-AnguloRotacao);
  Tangente = tan(AnguloRotacao);

  AlturaVideoPixels = AlturaVideo * Escala[X];
  LarguraVideoPixels = LarguraVideo * Escala[Y];
  AlturaRebatidaYPixels = AlturaVideoPixels * Coseno;
  AlturaRebatidaXPixel = AlturaVideoPixels * Seno;
  LarguraRebatidaYPixels = LarguraVideoPixels * Seno;
  NovaAlturaPixels =  LarguraVideoPixels * Coseno + AlturaVideoPixels * -Seno;
  NovaLarguraPixels = LarguraVideoPixels * -Seno + AlturaVideoPixels * Coseno;

  OnArrastouDesenho();
}

void DesPontosGeomView::OnArrastouDesenhoQ1()
{
  //--- Como a tela � redesenhada toda hora, o c�lculo dos pontos de in�cio e fim da malha bem como
  //--- dos pontos onde as cotas ser�o escritas � pesado e deve ser feito o m�nimo de vezes poss�vel.
  //--- Estes dados s� se alteram em arrastos  do desenho, zomms   ou quando o �ngulo de rota��o � alterado.

  //--- Ajusta as coordenadas de inicio e de fim do v�deo

  DeltasReais = Ponto(Deltas[X],Deltas[Y],AnguloRotacao);
  DeltasReais.RotacionaPlano(SenoM,CosenoM,-AnguloRotacao);

  LimiteYMetros.y = DeltasReais.y - LarguraVideo * Seno;
  LimiteYMetros.x = DeltasReais.x;

  //--- Ajusta as primeiras coordenadas e deltas

  ArestaMalha = EquiMalha *  Escala[X];
  if(ArestaMalha < 2) ArestaMalha = 2;                //--- Resolu��o m�nima da malha
  PrimeiraCotaY =  floor((LimiteYMetros.y + EquiMalha) / EquiMalha) * EquiMalha;
  PrimeiroDeltaY = (PrimeiraCotaY - LimiteYMetros.y) * Escala[Y];
  PrimeiraCotaX = floor((LimiteYMetros.x + EquiMalha) / EquiMalha) * EquiMalha;
  PrimeiroDeltaX = (PrimeiraCotaX - LimiteYMetros.x) * Escala[X];
  OrigemX = -AlturaVideoPixels * Seno;

  //--- Cria a lista com os pontos iniciais e finais das retas das coordenadas

  Ponto PMalha3,PMalha4;

  LPontosCoordX.clear();
  itlistPontosMalha it;
  double DeltaY = PrimeiroDeltaY ;

  for(; DeltaY < NovaAlturaPixels ;DeltaY += ArestaMalha)
  {     
    PMalha3.x = OrigemX; 
    PMalha4.x = NovaLarguraPixels;
    PMalha3.y = PMalha4.y = NovaAlturaPixels - DeltaY;

    PMalha3.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),0.0);
    PMalha4.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),0.0);

    it = LPontosCoordX.insert(LPontosCoordX.end(),stMalha());

    (*it).Pontos.first = PMalha3;
    (*it).Pontos.second = PMalha4;
  }

  //--- Calcula os pontos onde as coordenadas ser�o escritas

  CString Coord;
  double CoordAtual(PrimeiraCotaY);

  //--- Deslocamentos no eixo x.

  for(it = LPontosCoordX.begin(),DeltaY = PrimeiroDeltaY ;DeltaY < LarguraRebatidaYPixels ;DeltaY += ArestaMalha,CoordAtual += EquiMalha,it++)
  {     
    PMalha4.x = (LarguraRebatidaYPixels-Y) / tan(AnguloRotacao);   
    PMalha4.y = LarguraRebatidaYPixels -DeltaY - 3;
    PMalha4.RotacionaPlano(SenoM,CosenoM,0.0);

    Mat.FormataDoubleStr(CoordAtual,Coord,".,",0);

    PMalha4.x -= DeltaY / Tangente * Coseno;
    PMalha4.y += DeltaY * Coseno + AlturaVideoPixels; 

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }

  //--- Deslocamentos no eixo y.

  for(DeltaY = DeltaY - LarguraRebatidaYPixels ;DeltaY < AlturaRebatidaYPixels ;DeltaY += ArestaMalha,CoordAtual += EquiMalha,it++)
  {
    PMalha4.x = DeltaY * -tan(AnguloRotacao);
    PMalha4.y = DeltaY ;
    PMalha4.RotacionaPlano(SenoM,CosenoM,0.0);

    double RecuoLetra(TamLetra.cy * Seno + 4.0);

    PMalha4.x += RecuoLetra;
    PMalha4.y += RecuoLetra * Tangente;
    PMalha4.y = AlturaVideoPixels - PMalha4.y;

    Mat.FormataDoubleStr(CoordAtual,Coord,".,",0);

    (*it).PontoCota =  PMalha4;
    (*it).Cota = Coord;
  }

  //--- Coordenadas x
  //--- Calcula os pontos iniciais e finais das coordenadas x.

  LPontosCoordY.clear();
  double DeltaX = OrigemX + PrimeiroDeltaX ;

  for(; DeltaX < NovaLarguraPixels ; DeltaX += ArestaMalha)
  {
    PMalha3.x = DeltaX;
    PMalha3.y = NovaAlturaPixels;
    PMalha4.x = PMalha3.x;
    PMalha4.y = 0.0;

    PMalha3.RotacionaPlano(SenoM,CosenoM,0.0);
    PMalha4.RotacionaPlano(SenoM,CosenoM,0.0);

    it = LPontosCoordY.insert(LPontosCoordY.end(),stMalha());

    (*it).Pontos.first =  PMalha3;
    (*it).Pontos.second =  PMalha4;
  } 

  //--- Calcula os pontos das cotas x.

  for(CoordAtual = PrimeiraCotaX, it = LPontosCoordY.begin() ,DeltaX = OrigemX + PrimeiroDeltaX ; DeltaX < 0.0  ; DeltaX += ArestaMalha,CoordAtual += EquiMalha,it++)
  { 
    PMalha4.x = DeltaX;
    PMalha4.y = 0.0;

    PMalha4.RotacionaPlano(SenoM,CosenoM,0.0);

    PMalha4.y += (-DeltaX + 4.0) / Tangente * Coseno;
    PMalha4.x = 4.0;

    Mat.FormataDoubleStr(CoordAtual,Coord,".,",0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }

  for(; DeltaX < LarguraRebatidaXPixels  ; DeltaX += ArestaMalha,CoordAtual += EquiMalha,it++)
  {
    PMalha4.x = DeltaX;
    PMalha4.y = 0.0;
    PMalha4.RotacionaPlano(SenoM,CosenoM,0.0);

    PMalha4.x += (DeltaX * Seno) * Tangente + 4 * Tangente + TamLetra.cy * Seno * Tangente;
    PMalha4.y = 4.0 + TamLetra.cy * Seno;

    Mat.FormataDoubleStr(CoordAtual,Coord,".,",0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }
}

void DesPontosGeomView::OnArrastouDesenhoQ2()
{
  NovaLarguraPixels = LarguraVideoPixels * Seno + AlturaVideoPixels * -Coseno;

  DeltasReais = Ponto(Deltas[X],Deltas[Y],AnguloRotacao);
  DeltasReais.RotacionaPlano(SenoM,CosenoM,-AnguloRotacao);

  LimiteYMetros.y = DeltasReais.y - (LarguraVideo * Seno) + (AlturaVideo * Coseno);
  LimiteYMetros.x = DeltasReais.x + (LarguraVideo * Coseno) ;

  ArestaMalha = EquiMalha *  Escala[X];
  if(ArestaMalha < 2) ArestaMalha = 2;                //--- Resolu��o m�nima da malha
  PrimeiraCotaY =  floor((LimiteYMetros.y + EquiMalha) / EquiMalha) * EquiMalha;
  PrimeiroDeltaY = (PrimeiraCotaY - LimiteYMetros.y) * Escala[Y];
  PrimeiraCotaX = floor((LimiteYMetros.x + EquiMalha) / EquiMalha) * EquiMalha;
  PrimeiroDeltaX = (PrimeiraCotaX - LimiteYMetros.x) * Escala[X];
  OrigemX = LarguraVideoPixels * Coseno - AlturaVideoPixels * Seno;

  Ponto PMalha3,PMalha4;

  LPontosCoordY.clear();
  itlistPontosMalha it;

  for(double DeltaY = PrimeiroDeltaY ;DeltaY < NovaLarguraPixels ;DeltaY += ArestaMalha)
  {
    PMalha3.x = OrigemX; 
    PMalha4.x = 0.0;
    PMalha3.y = PMalha4.y = LarguraRebatidaYPixels -DeltaY ;

    PMalha3.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),0.0);
    PMalha4.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),0.0);

    it = LPontosCoordY.insert(LPontosCoordY.end(),stMalha());

    (*it).Pontos.first =  PMalha3;
    (*it).Pontos.second =  PMalha4;     
  }

  CString Coord;
  double CoordYAtual(PrimeiraCotaY);

  it = LPontosCoordY.begin();

  for(double DeltaY = PrimeiroDeltaY ;DeltaY < NovaLarguraPixels ;DeltaY += ArestaMalha,CoordYAtual += EquiMalha,it++)
  {
    PMalha4.x = 0.0;
    PMalha4.y = LarguraRebatidaYPixels -DeltaY ;

    PMalha4.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),0.0);

    double DeltaH = (LarguraRebatidaYPixels - DeltaY + 5.0) * -Coseno,
      RecuoLetra = TamLetra.cy * -Cotangente;

    PMalha4.y += DeltaH + RecuoLetra * Seno;
    PMalha4.x += DeltaH * -Cotangente + RecuoLetra * -Coseno;

    Mat.FormataDoubleStr(CoordYAtual,Coord,".,",0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }

  //----------------- coordenadas x

  LPontosCoordX.clear();
  double DeltaX(0.0);

  for(DeltaX = PrimeiroDeltaX ; DeltaX < NovaAlturaPixels ; DeltaX += ArestaMalha)
  {
    PMalha3.x = OrigemX + DeltaX ;
    PMalha3.y = LarguraVideoPixels;
    PMalha4.x = PMalha3.x;
    PMalha4.y = AlturaRebatidaYPixels;

    PMalha3.RotacionaPlano(SenoM,CosenoM,0.0);
    PMalha4.RotacionaPlano(SenoM,CosenoM,0.0);

    it = LPontosCoordX.insert(LPontosCoordX.end(),stMalha());

    (*it).Pontos.first =  PMalha3;
    (*it).Pontos.second =  PMalha4;     
  }

  double CoordXAtual(PrimeiraCotaX);
  it = LPontosCoordX.begin();

  for(DeltaX =PrimeiroDeltaX ; DeltaX < fabs(LarguraRebatidaXPixels); DeltaX += ArestaMalha,CoordXAtual += EquiMalha,it++)
  {
    PMalha4.x = OrigemX + DeltaX ;
    PMalha4.y = AlturaRebatidaYPixels;

    PMalha4.RotacionaPlano(SenoM,CosenoM,0.0);

    double DeltaXTemp = (LarguraRebatidaXPixels + DeltaX)  * Tangente * Seno;

    PMalha4.x += DeltaXTemp;
    PMalha4.y = AlturaVideoPixels - 5;

    Mat.FormataDoubleStr(CoordXAtual,Coord,".,",0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }	

  double  RecuoLetra = TamLetra.cy * -Coseno; 

  for( ; DeltaX < NovaAlturaPixels ; DeltaX += ArestaMalha,CoordXAtual += EquiMalha,it++)
  {
    PMalha4.x = OrigemX + DeltaX ;
    PMalha4.y = AlturaRebatidaYPixels;

    PMalha4.RotacionaPlano(SenoM,CosenoM,0.0);

    double DeltaXTemp = (DeltaX + LarguraRebatidaXPixels)  * -Coseno ;

    PMalha4.x += DeltaXTemp + RecuoLetra;
    PMalha4.y += (DeltaXTemp + RecuoLetra) * Cotangente; 

    Mat.FormataDoubleStr(CoordXAtual,Coord,".,",0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }	
}

void DesPontosGeomView::OnArrastouDesenhoQ3()
{
  DeltasReais = Ponto(Deltas[X],Deltas[Y],AnguloRotacao);
  DeltasReais.RotacionaPlano(SenoM,CosenoM,-AnguloRotacao);

  LimiteYMetros.y = DeltasReais.y + AlturaVideo * Coseno;
  LimiteYMetros.x = DeltasReais.x + (AlturaVideo * Seno) + (LarguraVideo * Coseno);

  ArestaMalha = EquiMalha *  Escala[X];
  if(ArestaMalha < 2) ArestaMalha = 2;                //--- Resolu��o m�nima da malha
  PrimeiraCotaY =  floor((LimiteYMetros.y + EquiMalha) / EquiMalha) * EquiMalha;
  PrimeiroDeltaY = (PrimeiraCotaY - LimiteYMetros.y) * Escala[Y];
  PrimeiraCotaX = floor((LimiteYMetros.x + EquiMalha) / EquiMalha) * EquiMalha;
  PrimeiroDeltaX = (PrimeiraCotaX - LimiteYMetros.x) * Escala[X];
  OrigemX = LarguraVideoPixels * Coseno;

  Ponto PMalha3,PMalha4;

  LPontosCoordX.clear();
  itlistPontosMalha it;
  double DeltaY(0.0);

  for(DeltaY = PrimeiroDeltaY ;DeltaY < NovaLarguraPixels ;DeltaY += ArestaMalha)
  {
    PMalha3.x = OrigemX; 
    PMalha4.x = -AlturaRebatidaXPixels;
    PMalha3.y = PMalha4.y = -DeltaY ;

    PMalha3.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),0.0);
    PMalha4.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),0.0);

    it = LPontosCoordX.insert(LPontosCoordX.end(),stMalha());

    (*it).Pontos.first =  PMalha3;
    (*it).Pontos.second =  PMalha4;     
  }

  CString Coord;
  double CoordYAtual(PrimeiraCotaY);
  it = LPontosCoordX.begin();

  for(DeltaY = PrimeiroDeltaY ; DeltaY < -AlturaRebatidaYPixels ;DeltaY += ArestaMalha,CoordYAtual += EquiMalha,it++)
  {    
    PMalha4.x = -AlturaRebatidaXPixels;
    PMalha4.y = -DeltaY ;

    PMalha4.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),0.0); 

    double AfastX((AlturaRebatidaYPixels + DeltaY) * Seno);

    PMalha4.x += AfastX; 
    PMalha4.y -= AfastX * Tangente;

    PMalha4.x -= TamLetra.cy * Seno - 3; 
    PMalha4.y -= (TamLetra.cy * -Seno  + 3) * Tangente;

    Mat.FormataDoubleStr(CoordYAtual,Coord,".,",0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }	

  for( ;DeltaY < NovaLarguraPixels ;DeltaY += ArestaMalha,CoordYAtual += EquiMalha,it++)
  {    
    PMalha4.x = -AlturaRebatidaXPixels;
    PMalha4.y = -DeltaY ;

    PMalha4.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),0.0); 

    double AfastX((((DeltaY + AlturaRebatidaYPixels) / Tangente)  + 3)  * -Coseno);

    PMalha4.x += AfastX; 
    PMalha4.y -= AfastX * Tangente;

    Mat.FormataDoubleStr(CoordYAtual,Coord,".,",0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }	

  //----------------- coordenadas x

  LPontosCoordY.clear();
  double DeltaX(0.0);

  for(DeltaX = PrimeiroDeltaX ; DeltaX < NovaAlturaPixels - AlturaVideoPixels * Seno; DeltaX += ArestaMalha)
  {
    PMalha3.x = OrigemX + DeltaX ;
    PMalha3.y = 0.0;
    PMalha4.x = PMalha3.x;
    PMalha4.y = AlturaRebatidaYPixels + LarguraRebatidaYPixels;

    PMalha3.RotacionaPlano(SenoM,CosenoM,0.0);
    PMalha4.RotacionaPlano(SenoM,CosenoM,0.0);

    it = LPontosCoordY.insert(LPontosCoordY.end(),stMalha());

    (*it).Pontos.first =  PMalha3;
    (*it).Pontos.second =  PMalha4;     
  }

  it = LPontosCoordY.begin();
  double CoordXAtual(PrimeiraCotaX);

  for(DeltaX = PrimeiroDeltaX ; DeltaX < NovaAlturaPixels - AlturaVideoPixels * Seno; DeltaX += ArestaMalha,CoordXAtual += EquiMalha,it++)
  {   
    PMalha4.x = OrigemX + DeltaX; 
    PMalha4.y = 0.0;

    PMalha4.RotacionaPlano(SenoM,CosenoM,0.0);

    double AfastY((LarguraRebatidaXPixels + DeltaX) * Seno);

    PMalha4.x += AfastY * Tangente;
    PMalha4.y += AfastY;

    double DeltaYLetra((TamLetra.cy + 4.0) * -Seno); 

    PMalha4.x += DeltaYLetra * Tangente + 4.0;
    PMalha4.y += DeltaYLetra;

    Mat.FormataDoubleStr(CoordXAtual,Coord,".,",0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }
}

void DesPontosGeomView::OnArrastouDesenhoQ4()
{
  Ponto DeltasReais(Deltas[X],Deltas[Y],AnguloRotacao);
  DeltasReais.RotacionaPlano(SenoM,CosenoM,-AnguloRotacao);

  LimiteYMetros.y = DeltasReais.y;
  LimiteYMetros.x = DeltasReais.x + AlturaVideo * Seno;

  ArestaMalha = EquiMalha *  Escala[X]; 
  if(ArestaMalha < 2) ArestaMalha = 2;                //--- Resolu��o m�nima da malha
  PrimeiraCotaY =  floor((LimiteYMetros.y + EquiMalha) / EquiMalha) * EquiMalha;
  PrimeiroDeltaY = (PrimeiraCotaY - LimiteYMetros.y) * Escala[Y];
  PrimeiraCotaX = floor((LimiteYMetros.x + EquiMalha) / EquiMalha) * EquiMalha;
  PrimeiroDeltaX = (PrimeiraCotaX - LimiteYMetros.x) * Escala[X];
  OrigemX = 0;

  Ponto PMalha3,PMalha4;

  LPontosCoordX.clear();
  itlistPontosMalha it;
  double DeltaY = PrimeiroDeltaY - AlturaRebatidaYPixels;

  for(; DeltaY < NovaLarguraPixels ;DeltaY += ArestaMalha)
  {
    PMalha3.x = OrigemX; 
    PMalha4.x = NovaAlturaPixels;
    PMalha3.y = PMalha4.y = -DeltaY ;

    PMalha3.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),0.0);
    PMalha4.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),0.0);

    it = LPontosCoordX.insert(LPontosCoordX.end(),stMalha());

    (*it).Pontos.first = PMalha3;
    (*it).Pontos.second = PMalha4;
  }

  CString Coord;
  double CoordAtual(PrimeiraCotaY);

  for(it = LPontosCoordX.begin(),DeltaY = PrimeiroDeltaY - AlturaRebatidaYPixels; DeltaY < 0.0 ;DeltaY += ArestaMalha,CoordAtual += EquiMalha,it++)
  {
    PMalha4.x = OrigemX; 
    PMalha4.y = -DeltaY ;

    PMalha4.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),0.0);

    PMalha4.y += ((DeltaY ) * Tangente + 4.0) * -Seno;
    PMalha4.x += (DeltaY) * Seno + 4.0;

    Mat.FormataDoubleStr(CoordAtual,Coord,".,",0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }

  for(; DeltaY < NovaLarguraPixels ;DeltaY += ArestaMalha,CoordAtual += EquiMalha,it++)
  {
    PMalha4.x = OrigemX; 
    PMalha4.y = -DeltaY ;

    PMalha4.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),0.0);

    double DeslocY((DeltaY + TamLetra.cy ) * Coseno + 4.0);

    PMalha4.y += DeslocY;
    PMalha4.x += DeslocY / -Tangente;

    Mat.FormataDoubleStr(CoordAtual,Coord,".,",0);

    (*it).PontoCota =  PMalha4;
    (*it).Cota = Coord;
  }

  //----------------- coordenadas x

  LPontosCoordY.clear();
  double DeltaX = PrimeiroDeltaX ;

  for(; DeltaX < NovaAlturaPixels ; DeltaX += ArestaMalha)
  {
    PMalha3.x = DeltaX ;
    PMalha3.y = AlturaRebatidaYPixels;
    PMalha4.x = PMalha3.x;
    PMalha4.y = LarguraRebatidaYPixels;

    PMalha3.RotacionaPlano(SenoM,CosenoM,0.0);
    PMalha4.RotacionaPlano(SenoM,CosenoM,0.0);

    it = LPontosCoordY.insert(LPontosCoordY.end(),stMalha());

    (*it).Pontos.first =  PMalha3;
    (*it).Pontos.second =  PMalha4;
  }

  CoordAtual = PrimeiraCotaX;

  for(it = LPontosCoordY.begin(),DeltaX = PrimeiroDeltaX ; DeltaX < -AlturaRebatidaXPixel ; DeltaX += ArestaMalha,CoordAtual += EquiMalha,it++)
  {
    PMalha4.x = DeltaX ;
    PMalha4.y = AlturaRebatidaYPixels;

    PMalha4.RotacionaPlano(SenoM,CosenoM,0.0);

    double DeslocMargemX((-AlturaRebatidaXPixel - DeltaX + TamLetra.cy) * Coseno + 4.0);

    PMalha4.x += DeslocMargemX;
    PMalha4.y -=  DeslocMargemX / -Tangente; 

    Mat.FormataDoubleStr(CoordAtual,Coord,".,",0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }

  for( ; DeltaX < NovaAlturaPixels ; DeltaX += ArestaMalha,CoordAtual += EquiMalha,it++)
  {
    PMalha4.x = DeltaX ;
    PMalha4.y = AlturaRebatidaYPixels;

    PMalha4.RotacionaPlano(SenoM,CosenoM,0.0);

    double DeslocMargemX((DeltaX - -AlturaRebatidaXPixel + 4.0) * -Tangente * -Seno);

    PMalha4.x += DeslocMargemX;
    PMalha4.y = AlturaVideoPixels - 4; 

    Mat.FormataDoubleStr(CoordAtual,Coord,".,",0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }
}

void DesPontosGeomView::CentralizarTela(const Ponto& PCentral,bool DesenharCruz)
{
  Deltas[X] = PCentral.x;
  Deltas[Y] = PCentral.y;

  DeltasReais.x = Deltas[X] -= LarguraVideo / 2.0; 
  DeltasReais.y = Deltas[Y] -=  AlturaVideo / 2.0;

  CalculaParametros();
  RedrawWindow();

  //--- Desenha a cruz vermelha no ponto

  if(DesenharCruz) DesenharCruzPonto(PCentral);
}

void DesPontosGeomView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
  // TODO: Add your specialized code here and/or call the base class

  if(pHint && !pSender && pHint == this &&  lHint == GRAVAR) OnGravar();
}

void DesPontosGeomView::DesenhaNorte(CDC* pDC)
{
  Ponto Origem(Resolucao[X] - 125 ,80);

  POINT Pontos[4] = {{Origem.x + 0,Origem.y + 0},
  {Origem.x + 30,Origem.y - 30},
  {Origem.x + 60,Origem.y + 0}};

  register int i(0);

  for (; i < 3 ; i++) 
  {
    double x(Pontos[i].x),y(Pontos[i].y);

    TransladaPontoPlano(&x,&y,Origem.x+30.0,Origem.y);
    RotacionaPontoPlano(SenoM,CosenoM,-AnguloRotacao,&x,&y);
    TransladaPontoPlano(&x,&y,-Origem.x-30.0,-Origem.y);

    Pontos[i].x = x;
    Pontos[i].y = y;
  }

  CPen PenaSec;  
  PenaSec.CreatePen(PS_SOLID,2,Cores[CORTEXTO]);
  pDC->SelectObject(&PenaSec);  

  CBrush brushBack(Cores[CORTEXTO]);
  CBrush* pOldBrush(pDC->SelectObject(&brushBack));

  pDC->Polygon(Pontos,3);

  pDC->SelectObject(pOldBrush); 

  pDC->Arc(Origem.x + 0,Origem.y - 30,Origem.x + 60,Origem.y + 30,0,0,0,0);

  Pontos[0].x = Origem.x-10;
  Pontos[0].y = Origem.y;
  Pontos[1].x = Origem.x+60+10;
  Pontos[1].y = Origem.y;
  Pontos[2].x = Origem.x+30;
  Pontos[2].y = Origem.y-30 -10 - TamLetra.cy;
  Pontos[3].x = Origem.x+30;
  Pontos[3].y = Origem.y+30+10;

  for (i = 0 ; i < 4 ; i++) 
  {
    double x(Pontos[i].x),y(Pontos[i].y);

    TransladaPontoPlano(&x,&y,Origem.x+30.0,Origem.y);
    RotacionaPontoPlano(SenoM,CosenoM,-AnguloRotacao,&x,&y);
    TransladaPontoPlano(&x,&y,-Origem.x-30.0,-Origem.y);

    Pontos[i].x = x;
    Pontos[i].y = y;
  }

  pDC->MoveTo(Pontos[0].x,Pontos[0].y);
  pDC->LineTo(Pontos[1].x,Pontos[1].y);
  pDC->MoveTo(Pontos[2].x,Pontos[2].y);
  pDC->LineTo(Pontos[3].x,Pontos[3].y);

  CPen PenaSec2;
  PenaSec2.CreatePen(PS_SOLID,1,Cores[CORMALHA]);
  pDC->SelectObject(&PenaSec2);
  if(Quadrante == 0 || Quadrante == 2) pDC->SelectObject(&CurrierNewHorizontal);
  else pDC->SelectObject(&CurrierNewVertical);
  pDC->SetTextColor(Cores[CORTEXTO]);
  std::strstream stmTexto;

  stmTexto << "  N "  << Superficie.AngRotacaoTopogGraus() << '�' << std::ends;
  pDC->SetTextAlign(TA_BOTTOM | TA_CENTER);
  pDC->TextOut(Pontos[2].x + TamLetra.cx - (TamLetra.cx / 2) ,Pontos[2].y,stmTexto.str());
}

void DesPontosGeomView::OnMButtonDblClk(UINT nFlags, CPoint point)
{
  CView::OnMButtonDblClk(nFlags, point);
}

void DesPontosGeomView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
  CView::OnRButtonDblClk(nFlags, point);
}

void DesPontosGeomView::CotaCN(double fi,Ponto Local,bool Inicio,double dCota,CDC* pDC,const CPen& PenaCotaMestra)
{
  Ponto Ppixels;
  double AngAjuste(0.0);

  if(fi > 3.14) fi -= 3.14;

  if(fi < 1.57) AngAjuste = 1.57 - fi;
  else if(fi < 3.14) AngAjuste = 7.85 - fi;

  CriaFontesCN(true, AngAjuste);

  pDC->SelectObject(&CourierNewCN);
  pDC->SelectObject(PenaCotaMestra);

  Ppixels.x = ((Local.x - Deltas[X]) * Escala[X]);
  Ppixels.y = (AlturaVideo - (Local.y - Deltas[Y])) * Escala[Y] ;
  pDC->SetTextAlign(TA_BOTTOM | TA_LEFT);
  TamLetra = pDC->GetTextExtent('A');

  std::strstream strstmCota;

  strstmCota << std::right << std::fixed << std::setfill(' ') << std::setprecision(fmod(dCota,1.0) > 0.0 ? 2 : 0);
  strstmCota << "  " << dCota << std::ends;

  if(Inicio) pDC->TextOut(Ppixels.x + (TamLetra.cy) / 2.0 *  sin(AngAjuste),Ppixels.y + (TamLetra.cy) / 2.0 * cos(AngAjuste),strstmCota.str());
  else pDC->TextOut(Ppixels.x - ((TamLetra.cy) / 2.0 * cos(AngAjuste)),Ppixels.y - ((TamLetra.cy) / 2.0 * sin(AngAjuste)),strstmCota.str());
}

BOOL DesPontosGeomView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  ((CChildFrame*) GetParentFrame())->DeslizanteGeom.MouseWheelOn(nFlags, zDelta, pt);

  return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void DesPontosGeomView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
  // TODO: Add your specialized code here and/or call the base class

  pDC->SetBkColor(Cores[CORFUNDO]);
  CView::OnPrepareDC(pDC, pInfo);
}

void DesPontosGeomView::OnButfronteira()
{
  if(NaoPermitirAlteracoes)
  {
    AfxMessageBox("N�o est�o sendo permitidas altera��es neste projeto.");
    return;
  }

  CDDEfineFronteira DDefineFronteira(this);

  if(DDefineFronteira.DoModal() == IDOK)
  {
    Superficie.PegaListaBordos().clear(); 
    //    int Serialize(const CString& NomeProjeto,int Tipo,bool SoRestricoes=false,bool SoCurvasNivel=false,bool GravarPontosEditados=true,bool SoBordos = false);

    Superficie.Serialize(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()),LER,TRUE,false,false,true);
    LimpaListaRestricao(CAciTopografico::FRONTEIRA);

    for (ItLLDeltaSup it = Superficie.PegaListaBordos().begin() ; it != Superficie.PegaListaBordos().end() ; ++it)
    {
      LLDeltaSup& ListaRestricoes(Superficie.PegaListaRestricoes()); 

      ListaRestricoes.push_back(LDeltaSup(*it));	
      ItLLDeltaSup itLRestricaoAtual(ListaRestricoes.end());
      itLRestricaoAtual--;

      if(Superficie.EstaRotacionada()) RotacionaBordoseRestricoes(Seno,Coseno,AnguloRotacao);;
      Superficie.InsereListaNoMapa(itLRestricaoAtual,CAciTopografico::FRONTEIRA);
    }

    Superficie.PegaListaBordos().clear(); 

    Superficie.VerfAciTopogForaSuperficie(ItLLDeltaSupMarcada);
    if(ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end()) Superficie.VerfAciTopogCruzFronteira(ItLLDeltaSupMarcada);

    AlterouTerreno = true;
    RedrawWindow();
  }
}

void DesPontosGeomView::GravaLog(const std::string& strMsg,std::ostream& Stream,bool ForcarHora)
{
  CTime Agora(CTime::GetCurrentTime());
  static CTime GravAnterior(Agora);
  std::strstream strstreamTemp;

  strstreamTemp << std::right << std::fixed << std::setfill(' ') << std::setprecision(2);

  if (ForcarHora || CTimeSpan((Agora - GravAnterior)).GetTotalSeconds() > 30L)
  {
    strstreamTemp <<  "====> " << Agora.GetDay() << '/' << Agora.GetMonth() << '/' << Agora.GetYear() << " " << Agora.GetHour() << ':' << Agora.GetMinute() << ':' << Agora.GetSecond() << ' ' << std::ends;
    Stream << strstreamTemp.str();
    GravAnterior = Agora;
  }

  Stream << strMsg.c_str() << std::endl;

  Stream.flush();
}

void DesPontosGeomView::OnDesenhar()
{
  if(NaoPermitirAlteracoes)  DesenhandoObjetos = DesenharObjetos = false;
  else DesenhandoObjetos = DesenharObjetos = !DesenharObjetos;
  CChildFrame* CF((CChildFrame*)GetParentFrame());
  CF->m_wndToolBarPonGeo.CheckDlgButton(ID_DESENHAR,DesenharObjetos);
  CF->ShowControlBar(&CF->m_wndToolBarAciTopograficos,DesenharObjetos ? SW_SHOWNORMAL : SW_HIDE,false);
}

bool DesPontosGeomView::GravaAcidentesTopograficos()
{
  CString NomeProjeto(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()));

  CFile ArqAciTopog;
  CFileException e;

  if(ArqAciTopog.Open(NomeProjeto + CString(".aci"),CFile::modeCreate | CFile::modeWrite,&e) == 0)
  {
    monolog.mensagem(13,CString(NomeProjeto + CString(".aci ") + " (Gravando os dados)"));
    return false;
  }

  CArchive ar(&ArqAciTopog,CArchive::store);

  AcidentesTopograficos >> ar;

  return true;
};

bool DesPontosGeomView::LeAcidentesTopograficos()
{
  CString NomeProjeto(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()));

  CFile ArqAciTopog;
  CFileException e;

  if(ArqAciTopog.Open(NomeProjeto + CString(".aci"),CFile::modeRead,&e) == 0)
  {
    if(e.m_cause != CFileException::fileNotFound)
    {
      monolog.mensagem(13,CString(NomeProjeto + CString(".aci ") + " (lendo os dados)"));
      return false;
    }

    return true;
  }

  CArchive ar(&ArqAciTopog,CArchive::load);

  AcidentesTopograficos << ar;

  return true;
};

void DesPontosGeomView::OnDesMuro(){ ObjetoAtual = CAciTopografico::MURO; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnNa(){ ObjetoAtual = CAciTopografico::N_A; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesMeiofio(){  ObjetoAtual = CAciTopografico::MEIO_FIO; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesNa(){  ObjetoAtual = CAciTopografico::N_A; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesRocha(){  ObjetoAtual = CAciTopografico::ROCHA; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesBueiro(){  ObjetoAtual = CAciTopografico::BUEIRO; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesRedeeletricaAt(){ ObjetoAtual = CAciTopografico::REDE_ELETRICA_AT; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesRedeeletricaBt(){ ObjetoAtual = CAciTopografico::REDE_ELETRICA_BT; OnMouseMove(0,PonMousePixelsAtual);}   
void DesPontosGeomView::OnDesBordo(){ ObjetoAtual = CAciTopografico::BORDO; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesCerca(){ ObjetoAtual = CAciTopografico::CERCA; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesPonte(){ ObjetoAtual = CAciTopografico::PONTE; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesPoste(){ ObjetoAtual = CAciTopografico::POSTE; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesFronteira(){ ObjetoAtual = CAciTopografico::FRONTEIRA; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDeseixo(){  ObjetoAtual = CAciTopografico::EIXO; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesCrista(){ ObjetoAtual = CAciTopografico::CRISTA; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesPe(){ ObjetoAtual = CAciTopografico::PE; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesCantoCasa(){ ObjetoAtual = CAciTopografico::CANTO_CASA; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesCantoCerca(){ ObjetoAtual = CAciTopografico::CANTO_CERCA; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesCantoMuro(){ ObjetoAtual = CAciTopografico::CANTO_MURO; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesCasa(){ ObjetoAtual = CAciTopografico::CASA ; OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesTampaBueiro(){ ObjetoAtual = CAciTopografico::TAMPA_BUEIRO;OnMouseMove(0,PonMousePixelsAtual);}
void DesPontosGeomView::OnDesFiltrar(){  FiltrarAciTopog = !FiltrarAciTopog; RedrawWindow();}

void DesPontosGeomView::CriaPenaRestri�ao(CPen *PenaObjeto,unsigned int Objeto)
{
  PenaObjeto->DeleteObject();

  switch(Objeto)
  {
  case CAciTopografico::BORDO :  PenaObjeto->CreatePen(PS_DASH,1,RGB(189,141,0)); break;
  case CAciTopografico::N_A : PenaObjeto->CreatePen(PS_SOLID,1,RGB(128,128,255)); break;
  case CAciTopografico::ROCHA : PenaObjeto->CreatePen(PS_DASHDOTDOT,1,RGB(160,160,160)); break;
  case CAciTopografico::CANTO_CERCA:
  case CAciTopografico::CERCA : PenaObjeto->CreatePen(PS_DASHDOT,1,RGB(192,0,0)); break;
  case CAciTopografico::CANTO_MURO :
  case CAciTopografico::MURO :  PenaObjeto->CreatePen(PS_SOLID,1,RGB(0,192,192)); break;
  case CAciTopografico::CANTO_CASA:
  case CAciTopografico::CASA :  PenaObjeto->CreatePen(PS_SOLID,2,RGB(192,192,192)); break;
  case CAciTopografico::MEIO_FIO : PenaObjeto->CreatePen(PS_DASH,1,RGB(160,160,160)); break;
  case CAciTopografico::PONTE : PenaObjeto->CreatePen(PS_SOLID,2,RGB(0,0,192)); break;
  case CAciTopografico::TAMPA_BUEIRO : PenaObjeto->CreatePen(PS_SOLID,1,RGB(192,192,192)); break;
  case CAciTopografico::POSTE : PenaObjeto->CreatePen(PS_DASHDOTDOT,1,RGB(192,0,0)); break;
  case CAciTopografico::REDE_ELETRICA_BT : PenaObjeto->CreatePen(PS_DASHDOTDOT,1,RGB(192,0,0)); break;
  case CAciTopografico::REDE_ELETRICA_AT : PenaObjeto->CreatePen(PS_DASHDOT,1,RGB(255,0,0)); break;
  case CAciTopografico::FRONTEIRA : PenaObjeto->CreatePen(PS_SOLID,2,RGB(64,64,192)); break;
  case CAciTopografico::EIXO :  PenaObjeto->CreatePen(PS_DASHDOT,1,RGB(223,223,223)); break;
  case CAciTopografico::CRISTA : PenaObjeto->CreatePen(PS_SOLID,1,RGB(189,157,126)); break;
  case CAciTopografico::PE : PenaObjeto->CreatePen(PS_DOT,1,RGB(189,157,126)); break;
  case CAciTopografico::BUEIRO : PenaObjeto->CreatePen(PS_DASHDOT,1,RGB(0,0,223)); break;
  default : PenaObjeto->CreatePen(PS_DASHDOT,1,RGB(255,255,255)); break;
  }
}

LRESULT DesPontosGeomView::FechouToolBar(WPARAM wParam, LPARAM lParam)
{
  CChildFrame* CF((CChildFrame*) GetParentFrame());

  if(lParam == IDR_TOOLDESENHAR2)
  {
      if(!(CToolBar*)((CMainFrame*)AfxGetMainWnd())->mpToolBoxDesenhar->IsWindowVisible())
      {
         CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_DESENHAR,false);
         DesenhandoObjetos = DesenharObjetos = false;
      }
  }

  return 0;
}

void DesPontosGeomView::OnButponins()
{
  DesenharPontosInseridos = ! DesenharPontosInseridos;

  RedrawWindow();
}

void DesPontosGeomView::DesenhaPoste(const Ponto& PontoPoste,CDC* pDC)
{
  CPen Pena,Pena1,* pPenaAnterior;     

  Pena.CreatePen(PS_SOLID,2,RGB(180,160,180));
  pPenaAnterior = pDC->SelectObject(&Pena);  

  pDC->MoveTo((PontoPoste.x - Deltas[X]) * Escala[X],(AlturaVideo - (PontoPoste.y - Deltas[Y])) * Escala[Y]);
  pDC->LineTo((PontoPoste.x - Deltas[X]) * Escala[X],(AlturaVideo - (PontoPoste.y - Deltas[Y])) * Escala[Y] - 12);

  Pena1.CreatePen(PS_SOLID,1,Cores[CORPONTOS]);
  pDC->SelectObject(&Pena1);  

  pDC->MoveTo((PontoPoste.x - Deltas[X]) * Escala[X] - 4,(AlturaVideo - (PontoPoste.y - Deltas[Y])) * Escala[Y] - 9);
  pDC->LineTo((PontoPoste.x - Deltas[X]) * Escala[X] + 4,(AlturaVideo - (PontoPoste.y - Deltas[Y])) * Escala[Y] - 9);

  pDC->SetPixel((PontoPoste.x - Deltas[X]) * Escala[X]-5,(AlturaVideo - (PontoPoste.y - Deltas[Y])) * Escala[Y]-8,Cores[CORPONTOS]);
  pDC->SetPixel((PontoPoste.x - Deltas[X]) * Escala[X]-6,(AlturaVideo - (PontoPoste.y - Deltas[Y])) * Escala[Y]-8,Cores[CORPONTOS]);

  pDC->SetPixel((PontoPoste.x - Deltas[X]) * Escala[X]+4,(AlturaVideo - (PontoPoste.y - Deltas[Y])) * Escala[Y]-8,Cores[CORPONTOS]);
  pDC->SetPixel((PontoPoste.x - Deltas[X]) * Escala[X]+5,(AlturaVideo - (PontoPoste.y - Deltas[Y])) * Escala[Y]-8,Cores[CORPONTOS]);

  pDC->SelectObject(pPenaAnterior);
}

void DesPontosGeomView::DesenhaBueiro(const Ponto& PontoBueiro,CPen* Pena,CDC* pDC)
{
  CPen* pPenaAnterior((CPen*)pDC->SelectObject(&Pena)); 
  Ponto Ppixels((PontoBueiro.x - Deltas[X]) * Escala[X],(AlturaVideo - (PontoBueiro.y - Deltas[Y])) * Escala[Y]);

  pDC->Arc(Ppixels.x-6,Ppixels.y-6,Ppixels.x+6,Ppixels.y+6,0,0,0,0);   	
  pDC->Arc(Ppixels.x-4,Ppixels.y-4,Ppixels.x+4,Ppixels.y+4,0,0,0,0);
}

ItCLLDeltaSup DesPontosGeomView::VerificaNaTangenteRestricoes(ItCLLDeltaSup ItCLLDeltaSupInicial,const Ponto& PAtual,double Toleranciax,ItCLDeltaSup* pItCLDS,double* pRaio)
{
  if(pItCLDS != NULL) *pItCLDS = ItCLLDeltaSupInicial->end();
  if(pRaio != NULL) *pRaio = 0.0;

  const LLDeltaSup& ListaRestAtual(Superficie.PegaListaRestricoes());
  ItCLLDeltaSup itLRestricaoAtual(ListaRestAtual.end());

  for(itLRestricaoAtual = ItCLLDeltaSupInicial ; itLRestricaoAtual != ListaRestAtual.end(); ++itLRestricaoAtual)
  {
    if(itLRestricaoAtual->size() > 1)
    {
      unsigned int TipoRestricao(Superficie.VerfAciTopografico(itLRestricaoAtual));

      if(!FiltrarAciTopog || TipoRestricao == ObjetoAtual)  
      { 
        register ItCLDeltaSup itPontoRestricaoAtual(itLRestricaoAtual->begin()); 
        ItCLDeltaSup itPontoRestricaoAnterior(itPontoRestricaoAtual);

        bool NaTangente(false);

        while (!NaTangente && ++itPontoRestricaoAtual != itLRestricaoAtual->end())
        {
          double Raio(hypot(itPontoRestricaoAnterior->PegaPCentral().x - PAtual.x,itPontoRestricaoAnterior->PegaPCentral().y - PAtual.y)),
            DistPontos(hypot(itPontoRestricaoAtual->PegaPCentral().x - itPontoRestricaoAnterior->PegaPCentral().x,
            itPontoRestricaoAtual->PegaPCentral().y - itPontoRestricaoAnterior->PegaPCentral().y)); 

          if(Raio < DistPontos)
          {
            double AzReta(CCurHorizontais::CalculaAzimute(itPontoRestricaoAtual->PegaPCentral(),itPontoRestricaoAnterior->PegaPCentral())),
              AzPonto(CCurHorizontais::CalculaAzimute(PAtual,itPontoRestricaoAnterior->PegaPCentral()));

            if(fabs(AzReta - AzPonto) < Matem::M_PI_2/4.0)    
            {
              double Distancia(fabs(sin(AzPonto - AzReta) * Raio));
              NaTangente = Distancia < Toleranciax;
              if(NaTangente)
              {
                if(pItCLDS != NULL) *pItCLDS = itPontoRestricaoAnterior;
                if(pRaio != NULL) *pRaio = Raio;
              }
            }
          }
          itPontoRestricaoAnterior = itPontoRestricaoAtual;
        }

        if(NaTangente)  return itLRestricaoAtual;
      }
    }
  }
  return Superficie.PegaListaRestricoes().end();
}

void DesPontosGeomView::Ima(UINT nFlags,const Ponto& XYPonto) 
{
  CPoint PPixels;
  TransfXYPixels(XYPonto.x,XYPonto.y,PPixels);
  ClientToScreen(&PPixels);
  SetCursorPos(PPixels.x,PPixels.y);
  CView::OnMouseMove(nFlags,PPixels);
}
/*
bool DesPontosGeomView::VerfArestaForaSuperficie(const Ponto& P1,const Ponto& P2)
{
const LLDeltaSup& ListaRestAtual(Superficie.PegaListaRestricoes());
ItCLLDeltaSup itLRestricaoAtual(ListaRestAtual.end());

for(ItCLLDeltaSup ItListaRestAtual = ListaRestAtual.begin() ; ItListaRestAtual!= ListaRestAtual.end() ; ++ItListaRestAtual)
{
unsigned int TipoRestricao(Superficie.VerfAciTopografico(ItListaRestAtual));

if(TipoRestricao == CAciTopografico::FRONTEIRA)
{
for(register ItCLDeltaSup ItPontoRestAtual = ItListaRestAtual->begin() ; ItPontoRestAtual!= ItListaRestAtual->end() ; ++ItPontoRestAtual)
{
if(*ItPontoRestAtual == P1)
{
ItCLDeltaSup ItProximo(ItPontoRestAtual),ItAnterior(ItPontoRestAtual);
--ItAnterior;
++ItProximo;

//--- Calcular o fi no c�lculo da fronteira, assim n�o ser� preciso calcular o azimute aqui

double AZPosterior(CCurHorizontais::CalculaAzimute(ItProximo->PegaPCentral(),ItPontoRestAtual->PegaPCentral())),
AZAnterior(CCurHorizontais::CalculaAzimute(ItAnterior->PegaPCentral(),ItPontoRestAtual->PegaPCentral())), 
AZRestricao(CCurHorizontais::CalculaAzimute(P2,P1));

if((Superficie.PegaArea() > 0.0 && ((AZPosterior > AZAnterior && AZRestricao < AZPosterior && AZRestricao > AZAnterior) || (AZPosterior < AZAnterior && !(AZRestricao > AZPosterior && AZRestricao < AZAnterior)))) ||
(Superficie.PegaArea() < 0.0 && ((AZPosterior < AZAnterior && AZRestricao > AZPosterior && AZRestricao < AZAnterior) || (AZPosterior > AZAnterior && !(AZRestricao < AZPosterior && AZRestricao > AZAnterior)))))
{
return true;
}
}
}
}
}
return false;
}

*/
void DesPontosGeomView::OnUpdateDesFronteira(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(DesenharBordos);
}

void DesPontosGeomView::VaiParaPonto(const CPoint& Ponto)
{
  Deltas[X] = Ponto.x;
  Deltas[Y] = Ponto.y;

  DeltasReais.x = Deltas[X] -= LarguraVideo / 2.0; 
  DeltasReais.y = Deltas[Y] -=  AlturaVideo / 2.0;
}

void DesPontosGeomView::OnPopupDesabilitar()
{
  if(!PegouRestricao)
  {
    Superficie.DesabilitaDeltaSuperficie(DeltaSupSobMousePopup);
    AlterouTerreno = true;

    RedrawWindow();
  }
  else AfxMessageBox("Pontos pertencentes a acidentes topogr�ficos n�o podem ser desabilitados.");
}

void DesPontosGeomView::OnPopupIncluirPonto()
{
  InserindoPonto = true;
}

void DesPontosGeomView::OnPopupHabilitar()
{
  Superficie.HabilitaDeltaSuperficie(ItPontoDesabilitado);
  ItPontoDesabilitado = Superficie.SetPonDesabilitados().end();
  AlterouTerreno = true;

  RedrawWindow();
}

void DesPontosGeomView::OnPopupSair()
{
  ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();

  RedrawWindow();
}

void DesPontosGeomView::OnUpdatePopupDesabilitar(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(PegouPonto);
}

void DesPontosGeomView::OnUpdatePopupHabilitar(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(PegouPontoDesabilitado);
}

void DesPontosGeomView::OnUpdatePopupIncluirPonto(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(!PegouPonto && !PegouPontoDesabilitado && !PegouPontoInserido && ItLembrete == SetLembretes.end());
}

void DesPontosGeomView::OnUpdatePopupRemoverPonto(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(PegouPontoInserido || ItLembrete != SetLembretes.end());
}

void DesPontosGeomView::OnPopupRemoverPonto()
{
  if(ItLembrete != SetLembretes.end())
  {
    SetLembretes.erase(ItLembrete);
    ItLembrete = SetLembretes.end();
    InserindoLembrete = MovendoLembrete = false;
  }
  else
  {
    RemovePontoRestricoes(*DeltaSupSobMousePopup);
    Superficie.RemovePontoInseridoUsuario(DeltaSupSobMousePopup);

    AlterouTerreno = true;
  }

  RedrawWindow();
}

void DesPontosGeomView::OnButLembrete()
{
  InserindoLembrete = true;

  RedrawWindow();
}

void DesPontosGeomView::OnUpdateButtrocaracitopog(CCmdUI *pCmdUI)
{
  // pCmdUI->Enable(ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end());
  pCmdUI->Enable(true);
}

void DesPontosGeomView::OnPopupRemover()
{
  POINT Ponto;

  GetCursorPos(&Ponto);
  CPoint PDialogo(Ponto);
  PDialogo.Offset(5,5);

  CDiaRemAciTopog DiaRemAciTopografico(this,PDialogo);

  if(DiaRemAciTopografico.DoModal() == IDOK)  
  {
    Superficie.RemoveListaDoMapa(ItLLDeltaSupMarcada);
    Superficie.PegaListaRestricoes().erase(ItLLDeltaSupMarcada);
    AlterouTerreno = true;
  }

  ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();

  RedrawWindow();
}

void DesPontosGeomView::OnPopupTrocar()
{
  ObjetoAnterior = ObjetoAtual = Superficie.VerfAciTopografico(ItLLDeltaSupMarcada);
  TrocandoObjeto = true;
}

void DesPontosGeomView::OnUpdatePopupTrocar(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
}

void DesPontosGeomView::InserePontoNaRestricaoMarcada(ItCLLDeltaSup ItCLLDeltaSupMarcada,Ponto& PInserir,bool CalcularCota)
{
  if(ItCLDS != ItCLLDeltaSupMarcada->end())      //--- Foi marcado no onmousemove
  {
    ItCLDeltaSup ItProxDS(ItCLDS);
    ++ItProxDS;
    double Az(Mat.CalculaAzimute((ItCLDS)->PegaPCentral(),ItProxDS->PegaPCentral()));

    //--- Ajusta o ponto clicado na restri��o

    Mat.IrradiaPonto(Az,ItCLDS->PegaPCentral().x,ItCLDS->PegaPCentral().y,0.0,Raio,PInserir);    //--- pRaio Foi marcado no onmousemove
    if(CalcularCota) CalculaCota(ItCLDS->PegaPCentral(),(++(ItCLDS))->PegaPCentral(),PInserir,Raio);
    (const_cast<LDeltaSup*>(&*ItCLLDeltaSupMarcada))->insert(ItProxDS,PInserir);
  }
}
/*
ItCLDeltaSup ItPontoRestricaoAnterior(ItCLLDeltaSupMarcada->begin()),ItPontoRestricaoAtual(ItPontoRestricaoAnterior);
bool Achou(false);

++ItPontoRestricaoAtual;


while (!Achou && ItPontoRestricaoAtual != ItLLDeltaSupMarcada->end())
{
if(fabs(ItPontoRestricaoAnterior->PegaPCentral().x - 616755.508) < 0.01||
fabs(ItPontoRestricaoAtual->PegaPCentral().x - 616681.548) < 0.01)
{
int i = 0;
}
if(Mat.VerfPontoNaReta(ItPontoRestricaoAnterior->PegaPCentral(),ItPontoRestricaoAtual->PegaPCentral(),PInserir) == true)
{


(const_cast<LDeltaSup*>(&*ItCLLDeltaSupMarcada))->insert(ItPontoRestricaoAnterior,PInserir);
Achou = true;
}

ItPontoRestricaoAnterior = ItPontoRestricaoAtual++;
}
*/

void DesPontosGeomView::RemovePontoRestricoes(const CDeltaSuperficie& DSRemover)
{
  LItLLDeltaSup LItRestricoesRemover;

  for(register ItLLDeltaSup ItLRestricao = Superficie.PegaListaRestricoes().begin()	;	ItLRestricao!=	Superficie.PegaListaRestricoes().end() ; ++ItLRestricao)
  {
    ItLDeltaSup itPontoRestricao(std::find(ItLRestricao->begin(),ItLRestricao->end(),DSRemover));

    if(itPontoRestricao	!=	ItLRestricao->end())	
    {
      ItLRestricao->erase(itPontoRestricao);	
      if(ItLRestricao->size()	<= 1)	
      {
        LItRestricoesRemover.push_front(ItLRestricao);
      }
    }
  }

  for(register ItLItLLDeltaSup ItLRemover = LItRestricoesRemover.begin()	;	ItLRemover !=	 LItRestricoesRemover.end() ; ++ItLRemover)
  {
    Superficie.RemoveListaDoMapa(*ItLRemover);
    Superficie.PegaListaRestricoes().erase( *ItLRemover);			 
  }
}

const ittysetLembretes DesPontosGeomView::PegaLembreteMaisProximo(const Ponto& PontoLembrete,double Toleranciax)
{
  CLembrete LembreteTemp(CLembrete::DES_GEOMETRICO,PontoLembrete);
  LembreteTemp.Deslocax(Toleranciax);

  ittysetLembretes itLembreteFinal(SetLembretes.upper_bound(LembreteTemp)),itLembreteAtual(SetLembretes.end()),itLembreteMaisProximo(SetLembretes.end());

  LembreteTemp.Deslocax(-Toleranciax * 2.0);
  itLembreteAtual = SetLembretes.upper_bound(LembreteTemp);         //--- Pega o primeiro na tolerancia    

  double MenorDistancia(INFINITO);

  //--- Separa o que est� mais pr�ximo do mouse 

  while(itLembreteAtual != itLembreteFinal) 
  {
    Ponto PAtual(itLembreteAtual->Local());
    double DistanciaAtual(hypot(PAtual.x - LembreteTemp.Local().x,PAtual.y - LembreteTemp.Local().y)); 
    if (DistanciaAtual < MenorDistancia) 
    {
      MenorDistancia = DistanciaAtual; 
      itLembreteMaisProximo = itLembreteAtual;
    }

    itLembreteAtual++;
  }

  return (MenorDistancia < Toleranciax) ? itLembreteMaisProximo :SetLembretes.end();
}

void DesPontosGeomView::SalvaLembretes()
{
  std::string NomeArquivo(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()+std::string(".lem"));
  UINT Flags;
  Flags =  CFile::modeCreate | CFile::modeWrite;

  CFile Arq;
  CFileException e;

  if (Arq.Open(NomeArquivo.c_str(),Flags,&e) == TRUE)
  {
    if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
    {
      CArchive ArcLembretes(&Arq,CArchive::store);

      ArcLembretes << SetLembretes.size();

      for(ittysetLembretes itLembreteAtual = SetLembretes.begin() ; itLembreteAtual != SetLembretes.end() ; ++itLembreteAtual)
      {
        (CLembrete&) *itLembreteAtual >> ArcLembretes;
      }
    }
  }
}

void DesPontosGeomView::LeLembretes()
{
  std::string NomeArquivo(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()+std::string(".lem"));
  UINT Flags;
  Flags =  CFile::modeRead;

  CFile Arq;
  CFileException e;

  if (Arq.Open(NomeArquivo.c_str(),Flags,&e) == TRUE)
  {
    if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
    {
      CArchive ArcLembretes(&Arq,CArchive::load);
      size_t Qtd(0);
      CLembrete Lembrete;

      for (ArcLembretes >> Qtd ; Qtd > 0 ; --Qtd)
      {
        Lembrete << ArcLembretes;
        SetLembretes.insert(Lembrete);
      }
    }
  }
}

void DesPontosGeomView::RemovePontoRestricao(ItLLDeltaSup& ItLRestricao,ItLDeltaSup ItPonto)
{
  register ItLDeltaSup ItInicio(ItPonto), ItFim(ItPonto);

  if(ItInicio != ItLRestricao->begin())
  {
    --ItInicio;

    while(ItInicio != ItLRestricao->begin() && ItInicio->PegaPCentral().Descricao.Find("Ponto Inserido") != std::string::npos)
    {
      --ItInicio;
    }

    ++ItInicio;

    do
    {
      ++ItFim;

    }while(ItFim != ItLRestricao->end() && ItFim->PegaPCentral().Descricao.Find("Ponto Inserido") != std::string::npos);

    if(ItInicio != ItLRestricao->begin()) ItLRestricao->erase(ItInicio,ItFim);
  }
  else
  {
    ItLRestricao->pop_front();
    ItLRestricao->pop_back();

    ItFim = --(ItLRestricao->end());

    while(ItFim != ItLRestricao->begin() && ItFim->PegaPCentral().Descricao.Find("Ponto Inserido") != std::string::npos);
    {
      --ItFim;
    }

    ItLRestricao->erase(ItFim,ItLRestricao->end());
    ItLRestricao->push_back(*ItLRestricao->begin());
  }
}

void DesPontosGeomView::RotacionaPontosInseridos(double Sin,double Cos,double AnguloArotacionar)
{
  for(register ItSSetPontos it = Superficie.SetPonInseridos().begin() ; it != Superficie.SetPonInseridos().end() ; ++it)
  {
    const_cast<Ponto*>(&*it)->RotacionaPlano(Sin,Cos,AnguloArotacionar);
  }
}

/*
void DesPontosGeomView::QuebraLista(LLDeltaSup& ListasAtuais,ItLLDeltaSup& itListaAtual,ItLDeltaSup itPontoRestricao) 
{
bool ListaCircular(*itListaAtual->begin() == *itListaAtual->rbegin());

++itPontoRestricao;

if(itPontoRestricao != itListaAtual->end()) 
{
while(itPontoRestricao->PegaPCentral().Descricao.Find("Ponto Inserido") != -1)
{
++itPontoRestricao;
}
}

if(ListaCircular == false)
{ 
if(itPontoRestricao != itListaAtual->end())
{
/*
ListasAtuais.push_back(LDeltaSup(itPontoRestricao,itListaAtual->end()));
Superficie.InsereListaNoMapa(--(ListasAtuais.end()),ObjetoAtual);
itListaAtual->erase(itPontoRestricao,itListaAtual->end());
*/
/*
for(register ItLDeltaSup it = ++itPontoRestricao ; it != itListaAtual->end() ; ++it)
{
itListaAtual->push_front(*itPontoRestricao);
}

itListaAtual->erase(itPontoRestricao,itListaAtual->end());
itListaAtual->pop_back();
}
}
else
{
itListaAtual->erase(itPontoRestricao,itListaAtual->end());
}

if(itListaAtual->size() == 1)
{
Superficie.RemoveListaDoMapa(itListaAtual);
ListasAtuais.erase(itListaAtual);
}
}
*/

void DesPontosGeomView::OnButarrastar()
{
  PermitirArrastar = !PermitirArrastar;
}

void DesPontosGeomView::RotacionarLinhaBase()
{
  OnButrotacionar();
  DefinindoLinhaBase = 0;
}

void DesPontosGeomView::DesenharCruzPonto(const Ponto& PCentral,CDC* ppDC)
{
  CPoint PPixels((PCentral.x - Deltas[X]) * Escala[X],(AlturaVideo - (PCentral.y - Deltas[Y])) * Escala[Y]);

  CDC* pDC(ppDC ? ppDC : GetDC());
  if(!pDC) return;              //--- Seguran�a, aconteceu nos testes

  CPen PenaMalha,*pPenaAnterior; 										
  PenaMalha.CreatePen(PS_SOLID,2,RGB(255,0,0));
  pPenaAnterior = pDC->SelectObject(&PenaMalha);		

  //--- Desenha a cruz, 14 pixels de largura

  pDC->MoveTo(PPixels.x-15,PPixels.y);
  pDC->LineTo(PPixels.x+15,PPixels.y);		
  pDC->MoveTo(PPixels.x,PPixels.y-15);
  pDC->LineTo(PPixels.x,PPixels.y+15);

  pDC->Arc(PPixels.x-4,PPixels.y-4,PPixels.x+4,PPixels.y+4,0,0,0,0);    //--- Circula o ponto.

  pDC->SelectObject(pPenaAnterior); 

  ReleaseDC(pDC); 
}

ItCLLDeltaSup DesPontosGeomView::VerfClickRestricao(Ponto& PAtual)
{
  LLDeltaSup& ListasRestricoes(Superficie.PegaListaRestricoes());
  ItCLLDeltaSup ItLRestricaoAtual(ListasRestricoes.begin()),ListaRetClicada(ListasRestricoes.end());
  PegouRestricao = false;

  while (!PegouRestricao && ItLRestricaoAtual != ListasRestricoes.end())
  {
    if(Superficie.EstaRotacionada()) PAtual.RotacionaPlano(Superficie.PegaSeno(),Superficie.PegaCoseno(),Superficie.PegaAnguloRotacao());
    ListaRetClicada = VerificaNaTangenteRestricoes(ItLRestricaoAtual,PAtual,Toleranciax); 

    if(ListaRetClicada != Superficie.PegaListaRestricoes().end()) PegouRestricao = true;
    else ++ItLRestricaoAtual;
  }

  return ListaRetClicada;
}
    
