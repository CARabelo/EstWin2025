// DesPerfilView.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "carqterreno.h"
#include "math.h"
#include "Estaca.h"
#include "cponto.h" 
#include "clpontos.h" 
#include "perfil.h"
#include "supercstring.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtalcorte.h"
#include "carqtalaterro.h"
#include "ctalcorte.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include <list>
#include "csecao.h"
#include "clistaperfis.h"
#include "clistasecoes.h"
#include "xygreide.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "carqcurverticais.h"
#include "ccurverticais.h"
#include "xymouse.h"
#include <set>
#include <string>
#include "carqgradientes.h"
#include "cgradientes.h"
#include "mytoolbar.h"
#include "dselsecao.h"
#include "listamedicoes.h"
#include "dvolumes.h"
#include "calcnota.h"
#include "meleditbox.h"
#include "cdpopuppontos.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
#include "carqpistasfixas.h"
#include "cpistasfixas.h"
#include "carqcurhorizontais.h"
#include <map>
#include "ccurhorizontal.h"
#include "Mat.h"
#include "ccurhorizontais.h"
#include "CTipoSolos.h"
#include "CArqTiposSolos.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include "CArqFurosSondagem.h"
#include <algorithm>
#include "CPerfilGeologico.h"
#include "carqsecoesavulsas.h"
#include "csecaoavulsa.h"
#include "csecoesavulsas.h"
#include "cdeltasuperficie.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CAcidentesTopograficos.h"
#include "CSegmento.h"
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "dessecaoview.h"
#include "dentrapontogreide.h"
#include "comboprocdessecao.h"
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "diapontosgeom.h"
#include "CArqOAC.h"
#include "coac.h"
#include "CArqOAE.h"
#include "coae.h"
#include "CMemDC.h"
#include "CDEnsaiarGreide.h"
#include "DesPerfilView.h"
#include "dentraponto.h" 
#include "ddeslizantes.h"
#include "deslizantesgeom.h"
#include "palette.h"
#include "childfrm.h"
#include "dselgreide.h"
#include "destacacota.h"
#include <iomanip> 
#include <strstream>   // for sstream
#include <vector>
#include <iostream>
#include <sstream>
#include "dialogo.h"
#include "monologo.h"

#include "projpsimpl\psimpl_reference.h"
#include "projpsimpl\psimpl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DES_PAVIMENTO 0
#define DES_TERRAPLANAGEM 1
#define X        0
#define Y        1
#define ESQUERDO 0
#define DIREITO  1
#define CORTE    0
#define ATERRO   1
#define ANTERIOR   0
#define POSTERIOR  1
#define ESTPCV    1
#define ESTPIV    2
#define ESTPTV    3
#define SO_X      0

#define EVENT_MOUSE_STOP 1000
#define NENHUM_TIMER 0

extern class dialogo dialogar;
extern class monologo monolog;    

CDEnsaiarGreide* pDEnsaiarGreide = nullptr;
/////////////////////////////////////////////////////////////////////////////
// DesPerfilView
IMPLEMENT_DYNCREATE(DesPerfilView, CView)

DesPerfilView::DesPerfilView(int Medicao) : CurvasVerticais(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual())),
CurvasVerAnteriores(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual())),
Secoes(*(CListaSecoes*)((CMainFrame*)AfxGetMainWnd())->PegaSecoes()),ArrastandoPonto(FALSE),
Arrastando(FALSE),DeltaYMetro(0.0),PAtual(NULL),Alterou(FALSE),DesenhoSecao(*((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->PegaDesenhoSecoes())),
TerminarArrasto(FALSE),AlterouGreide(FALSE),EMedicao(((CMainFrame*) AfxGetMainWnd())->EMedicao) ,
ListaMedicoes(((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->PegaDesenhoSecoes())->PegaListaMedicoes()),
DPopUpPonto(this,RGB(255,255,166)),SecoesIconizadas(FALSE),HPavimento(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual())),
HLimpeza(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()),CString(".lim")),EZoom(false),
LimiteEsq(INFINITO),LimiteDir(-INFINITO),LimiteSup(-INFINITO),LimiteInf(INFINITO),PonMousePixelAtual(0,0),
OACs(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual())),OAEs(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual())),CursorAtual(IDC_HAND),
PermitirArrastar(true),CircularPontos(true),PerfilGeologico(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()).GetBuffer()),
DesenharPerfGeol(false),Trecho(((CMainFrame*) AfxGetMainWnd())->PegaRuaAtual()),Projeto(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()),
MetodoEnsaio(-1),ParametroEnsaio1(INFINITO), ParametroEnsaio2(INFINITO)
{
  Resolucao[X] = 1024.0 ; Resolucao[Y] = 768.0;
  Escala[X] = Escala[Y] = 100.0;
  Deltas[X] = Deltas[Y] = 0.0;

  Secoes.CalculaIsLongitudinais();            //=== Calcula as rampas do perfil longitudinal.

  //--- Calcula os is das medições

  POSITION PMedicaoAtual = ListaMedicoes.GetHeadPosition();

  while (PMedicaoAtual)
  {
    CListaSecoes& LMedAtual = *ListaMedicoes.GetNext(PMedicaoAtual); 
    LMedAtual.CalculaIsLongitudinais();
  }

  ((CMainFrame*)AfxGetMainWnd())->DesPerfil = this;   //--- Ponteiro para ser usado no desenho da secao, que 
  //--- precisa envias mensagens para os perfis porque precisa ficar atualizado sobre as alteracoes no perfil. 

  //--- Pega os limites do terreno

  POSITION SecAtual(Secoes.GetHeadPosition());

  CEstaca* EstacaAtual;   

  while(SecAtual) 
  {
    EstacaAtual = &Secoes.GetNext(SecAtual).Terreno.Estaca; 

    if (EstacaAtual->EstVirtual!= INFINITO && EstacaAtual->Cota != INFINITO)
    { 
      if (EstacaAtual->Cota > LimiteSup) LimiteSup = EstacaAtual->Cota;
      if (EstacaAtual->Cota < LimiteInf) LimiteInf = EstacaAtual->Cota;
      if (EstacaAtual->EstVirtual < LimiteEsq) LimiteEsq = EstacaAtual->EstVirtual;
      if (EstacaAtual->EstVirtual > LimiteDir) LimiteDir = EstacaAtual->EstVirtual;
    } 
  }

  //--- Atualiza os limites

  if(CurvasVerticais.PegaLimiteEsq() < LimiteEsq) LimiteEsq = CurvasVerticais.PegaLimiteEsq();
  if(CurvasVerticais.PegaLimiteDir() > LimiteDir) LimiteDir = CurvasVerticais.PegaLimiteDir();
  if(CurvasVerticais.PegaLimiteSup() > LimiteSup) LimiteSup = CurvasVerticais.PegaLimiteSup();
  if(CurvasVerticais.PegaLimiteInf() < LimiteInf) LimiteInf = CurvasVerticais.PegaLimiteInf();

  //--- Se o desenho da seções já estavam la desabilita o sysmenu

  CChildFrame* CCFrm((CChildFrame*) DesenhoSecao.GetParentFrame());

  if (CCFrm->GetStyle() & WS_SYSMENU) 
  {
    CCFrm->ModifyStyle(WS_SYSMENU,0,SWP_FRAMECHANGED);  
    DesenhoSecao.RedrawWindow();
  }
  PerfilGeologico.Serialize(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual(),0);
}

DesPerfilView::~DesPerfilView()
{
  if (pDEnsaiarGreide) delete(pDEnsaiarGreide);
  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());
  std::string NomeArquivo(MainFrame->PegaProjetoAtual() + std::string(".ini"));

  //--- Grava os parametros atuais da janela

  CString CSDados;
  std::string Secao(std::string("DadosDesenhos_") + MainFrame->PegaRuaAtual());

  CSDados.Format("%lf,%lf,%lf,%lf,%lf",Escala[X],Escala[Y],RazaoHV,Deltas[X],Deltas[Y]);

  ::WritePrivateProfileString(Secao.c_str(),_T("DadosPerfil"),CSDados,NomeArquivo.c_str());		

  //--- Grava as cores

  CSDados.Format("%lu,%lu,%lu,%lu,%lu,%lu",Cores[0],Cores[1],Cores[2],Cores[3],Cores[4],Cores[5]);
  ::WritePrivateProfileString(_T("DadosDesenhos"),_T("DadosDesPerfilCores"),CSDados,NomeArquivo.c_str());

  //--- Salva o estado  da configuração do desenho  

  CSDados.Format("%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i",DesenharPixel,DesenharCirculo,DesenharCruz,
    GabaritarProjeto,DesenharProjeto,DesenharTerreno,DesenharMalha,Cotar,EquiMalhaX,EquiMalhaY,EquiCotas,MostrarDesc,
    SincronizarSecoes,MostrarTitulo,VarrerTerreno,VarrerProjeto,AlterarGreide,SecoesIconizadas,PermitirArrastar,CircularPontos,
    DesenharPerfGeol);
  ::WritePrivateProfileString(Secao.c_str(),_T("DadosDesPerfilConfig"),CSDados,NomeArquivo.c_str());

  CSDados.Format("%i,%i",MostrarToolCoordenadas,MostrarPonto);
  ::WritePrivateProfileString(_T("SettingsChild"),_T("StatusVisualizarPerfil"),CSDados,NomeArquivo.c_str());

  //--- Se houve alteração no greide, grava o novo estado do greide

  ((CMainFrame*)AfxGetMainWnd())->GreideAtivo = FALSE;

  //--- Se estiver com o desenho geométrico anula o ponteiro no destruidoe do desenho geométrico,pois
  //--- ele é usado lá, para salvar o estado da janela do desenho geométrico

  //if(!((CMainFrame*)AfxGetMainWnd())->DesPontosGeo) ((CMainFrame*)AfxGetMainWnd())->DesPerfil = NULL;

  ((CMainFrame*)AfxGetMainWnd())->DesPerfil = NULL;
}

void DesPerfilView::GravarProjeto(int Tipo,bool Forcar,bool Perguntar)      //--- Chamado pelo menu Calcular volumes (Atualiza os arquivos antes de calcular)
{
  int Resposta(IDOK);

  if(Perguntar) Resposta = dialogar.DoModal(4, CString(" Greide - ") + CString(Trecho.c_str()));

  if (Resposta == IDOK) OnGravarperfil();
  else ((CMainFrame*)AfxGetMainWnd())->RemoveDesenhoAlterado(Trecho, std::string("DesPerfil"));
}

BEGIN_MESSAGE_MAP(DesPerfilView, CView)
  //{{AFX_MSG_MAP(DesPerfilView)
  ON_WM_MOUSEMOVE()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_RBUTTONDBLCLK()
  ON_WM_SETFOCUS()
  ON_WM_RBUTTONUP()
  ON_WM_ERASEBKGND()
  ON_COMMAND(ID_CONFDESGREIDE, OnConfDesGreide)
  ON_COMMAND(ID_SECSEMPREVIS_ICONIZAR, OnSecsemprevisIconizar)
  ON_COMMAND(ID_IRPARAPONTO, OnIrparaponto)
  ON_WM_RBUTTONDOWN()
  ON_COMMAND(ID_ENQUADRAR, OnEnquadrar)
  ON_WM_SIZE()
  ON_COMMAND(ID_ALTERNARTITULOGREIDE, OnAlternartitulogreide)
  ON_COMMAND(ID_BUTFECHar, OnButFechar)
  ON_WM_CREATE()
  ON_COMMAND(ID_GRAVAR, OnGravarperfil)
  ON_UPDATE_COMMAND_UI(ID_GRAVAR, OnUpdateGravarperfil)
  ON_COMMAND(ID_BUTSINCRO, OnSincronizarSecoes)
  ON_COMMAND(ID_BUTLOCK,OnTrancarGreide)
  ON_COMMAND(ID_ENQUADRAR, OnEnquadrar)
  ON_COMMAND(ID_CALCVOLUMES, OnCalcVolumes)
  ON_MESSAGE(WM_ENSAIARGREIDE,EnsaiarGreide)
  //}}AFX_MSG_MAP
  ON_WM_KEYUP()
  ON_WM_MOUSEWHEEL()
  ON_UPDATE_COMMAND_UI(ID_CALCVOLUMES, &DesPerfilView::OnUpdateCalcvolumes)
  ON_COMMAND(ID_BUTARRASTAR, &DesPerfilView::OnButarrastar)
  ON_WM_CLOSE()
  ON_COMMAND(ID_BUTDESENHARPERFGEOLOGICOS, &DesPerfilView::OnButdesperfgeo)
  ON_COMMAND(ID_BUTENSAIARGREIDE, &DesPerfilView::OnButensaiargreide)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DesPerfilView drawing

void DesPerfilView::OnDraw(CDC* ppDC)
{
  if (!ppDC || /*Secoes.IsEmpty() ||*/ CurvasVerticais.VerfErro()) return;      

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

    //--- cota a malha

    if(Cotar)
    {
      CFont* pFonteAnterior = pDC->SelectObject(&CurrierNewVertical);
      pDC->SetTextAlign(TA_BOTTOM | TA_LEFT);
      pDC->SetTextColor(Cores[CORTEXTO]);
      pDC->SetBkColor(Cores[CORFUNDO]);

      CString Coord;
      const unsigned long EquiCota(EquiMalhaX * EquiCotas);

      CEstaca EstacaAtual(floor(Deltas[X] / Escala[X] / (double)EquiMalhaX) * -EquiMalhaX);			//--- Valor da cota onde esta a corrdenada + 1 pixel
      if(EstacaAtual.EstVirtual > 0.0) EstacaAtual = EstacaAtual.EstVirtual - EquiMalhaX; 																																				//--- Pega o múltiplo inferior
      unsigned int QuantX(floor((double )((double)ALTVIDEOMTS / (double)DELTAYMTS)) / (double)EquiCotas), 				//--- cota 3 linhas da malha x
                   QuantY(floor((double)(LARGVIDEOMTS / (double)DELTAXMTS)) / (double)EquiCotas);			          	//--- cota 4 linhas da malha Y

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

  CPen Pena[TOTALPENAS];                      //--- Penas do projeto e do terreno.

  CMainFrame* CMF((CMainFrame*)AfxGetMainWnd());

  //--------------------------------------------------------------------------------------

  CPen PenaE;
  
  PenaE.CreatePen(PS_DASH, 2, Cores[CORPAVIMENTO]);

  pDC->SelectObject(&PenaE);

  if(GreideSimplificado.size())
  {
    CPoint P;

    ittylstpontos It = GreideSimplificado.begin();

    P.x = (int)(Deltas[X] + (It->x * Escala[X]/*+Resolucao[X]*/));
    P.y = (int)(Deltas[Y] + (Resolucao[Y] - (It->y - HLimpeza.BuscaH(It->x)) * Escala[Y]));

    pDC->MoveTo(P);

    for(It++ ; It != GreideSimplificado.end() ; It++)
    {
      {
        P.x = (int)(Deltas[X] + (It->x * Escala[X]/*+Resolucao[X]*/));
        P.y = (int)(Deltas[Y] + (Resolucao[Y] - (It->y - HLimpeza.BuscaH(It->x)) * Escala[Y]));
        pDC->LineTo(P);                              //--- Desenha a linha.
      }
    }
  }


  //-------------------------------------------------------------------------------------------


  if(DesenharTerreno && Secoes.GetCount())
  {
    //Desenha a limpeza 

    Pena[LIMPEZA].CreatePen(PS_DOT,1,Cores[CORTERRAPLANAGEM]);
    pDC->SelectObject(&Pena[LIMPEZA]);  

    POSITION SecAtual(Secoes.GetHeadPosition());

    CEstaca* EstacaAtual(&Secoes.GetNext(SecAtual).Terreno.Estaca);  //--- Estaca que abrigará as estacas das secoes.

    //--- Desenha o primeiro ponto.

    //--- Procura pela primeira estaca que tenha terreno.

    while(SecAtual && (EstacaAtual->EstVirtual == INFINITO || EstacaAtual->Cota == INFINITO))  
      EstacaAtual = &Secoes.GetNext(SecAtual).Terreno.Estaca; 

    if (EstacaAtual->EstVirtual != INFINITO) //--- Se achou pelo menos uma inicia o desenho da polyline 
    {
      CPoint P((int) (Deltas[X] + (EstacaAtual->EstVirtual*Escala[X])),
        (int) (Deltas[Y] + (Resolucao[Y] - ((EstacaAtual->Cota -  HLimpeza.BuscaH(*EstacaAtual)) *Escala[Y]))));
      pDC->MoveTo(P);                               //--- Inicializa a linha;

      //--- Desenha os pontos restantes.

      while(SecAtual)
      {
        EstacaAtual = &Secoes.GetNext(SecAtual).Terreno.Estaca;
        if (EstacaAtual->Cota != INFINITO && EstacaAtual->Cota > 1.0)   //--- A estaca pode estar sem terreno.
        {
          P.x = (int) (Deltas[X] + (EstacaAtual->EstVirtual*Escala[X]/*+Resolucao[X]*/));
          P.y = (int) (Deltas[Y] + (Resolucao[Y] - (EstacaAtual->Cota -  HLimpeza.BuscaH(*EstacaAtual)) * Escala[Y]));
          pDC->LineTo(P);                              //--- Desenha a linha.
        }
      }  
    }

    if(!DesenharPerfGeol)       //--- Ou desenha o perfil geológico ou o terreno
    {
      if (Pena[TERRENO].CreatePen(PS_SOLID,1,Cores[CORTERRENO]))
      {
        //--- Desenha o Terreno
        //*************************

        // Seleciona a respectiva pena no device context
        // e salva a pena antiga ao mesmo tempo.

        pDC->SelectObject(&Pena[TERRENO]);    

        //--- Os valores dos pixels são calculados na chamada das funções <MoveTo> e <LineTo>
        //--- A coordenada x em pixels é calculada pela fórmula:
        //--- Sendo:
        //---   px           -> Valor da coordenada x em pixels.
        //---   p(x)         -> Valor do afastamento x do ponto.
        //---   Escala(x)    -> Valor da escala(Pixels/metro) no eixo x.
        //---   Resolução(x) -> Valor da resolução no eixo x (em pixels).
        //---   D(x)         -> Valor do Deslocamento (tecla seta para a dire ou esquerda)
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
        //--- 								 |
        //--- 								 |
        //---	  							 V (y)
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

        POSITION SecAtual(Secoes.GetHeadPosition());

        CEstaca* EstacaAtual(&Secoes.GetNext(SecAtual).Terreno.Estaca);  

        //--- Desenha o primeiro ponto.

        //--- Procura pela primeira estaca que tenha terreno.

        while(SecAtual && (EstacaAtual->EstVirtual == INFINITO || EstacaAtual->Cota == INFINITO))  
          EstacaAtual = &Secoes.GetNext(SecAtual).Terreno.Estaca; 

        if (EstacaAtual->EstVirtual != INFINITO) //--- Se achou pelo menos uma inicia o desenho da polyline 
        {
          CPoint P((int) (Deltas[X] + (EstacaAtual->EstVirtual*Escala[X])),
            (int) (Deltas[Y] + (Resolucao[Y] - ((EstacaAtual->Cota) *Escala[Y]))));
          pDC->MoveTo(P);                               //--- Inicializa a linha;
          if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0);    //--- Circula o ponto.

          //--- Desenha os pontos restantes.

          while(SecAtual)
          {
            EstacaAtual = &Secoes.GetNext(SecAtual).Terreno.Estaca;
			
            if (EstacaAtual->Cota != INFINITO && EstacaAtual->Cota > 1.0)   //--- A estaca pode estar sem terreno.
            {
              P.x = (int) (Deltas[X] + (EstacaAtual->EstVirtual*Escala[X]/*+Resolucao[X]*/));
              P.y = (int) (Deltas[Y] + (Resolucao[Y] - (EstacaAtual->Cota) * Escala[Y]));
              pDC->LineTo(P);                              //--- Desenha a linha.
              if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0);   //--- circula o ponto
            }
          }  
        }
      }
    }
  }

  for (int i = DES_PAVIMENTO ; i <= DES_TERRAPLANAGEM ; i++)            //--- Desenha o greide do pavimento e da terraplanagem
  {                                        //--- 
    if(DesenharProjeto)
    {
      //--- Desenha o projeto
      //****************************************

      bool GabaritarProjeto(EMedicao && ((CMainFrame*)AfxGetMainWnd())->GabaritarProjeto);   //-- Gabaritar projeto o quando for medição

      //--- Cria a respectiva pena

      if ((i == DES_PAVIMENTO && Pena[PROJETO].CreatePen(GabaritarProjeto ? PS_DASH : PS_SOLID,1,Cores[CORPAVIMENTO])) ||
        (i == DES_TERRAPLANAGEM && Pena[TERRAPLANAGEM].CreatePen(PS_SOLID,1,Cores[CORTERRAPLANAGEM])))
      {
        POSITION Atual(CurvasVerticais.LCurvasVerticais.GetHeadPosition());     //--- Aponta para o PIV atual.

        if (!EMedicao || GabaritarProjeto)
        {
          if (Atual)     //--- Se Tem Projeto...
          {
            if (i == DES_PAVIMENTO) pDC->SelectObject(&Pena[PROJETO]);    
            else pDC->SelectObject(&Pena[TERRAPLANAGEM]);   

            CurVertical* PIVAtual = &CurvasVerticais.LCurvasVerticais.GetNext(Atual) ;  //--- PIV que abrigará os PIVs do projeto.

            //--- Desenha o primeiro PIV.

            double hPavim(i == DES_TERRAPLANAGEM ? HPavimento.BuscaH(PIVAtual->PIV) : 0.0);  //--- Se for terraplanagem busca o h do pavimento

            CPoint P((int) (Deltas[X] + (PIVAtual->PIV.EstVirtual * Escala[X])),
              (int) (Deltas[Y] + (Resolucao[Y] - ((PIVAtual->PIV.Cota-hPavim) *Escala[Y]))));
            pDC->MoveTo(P);                               //--- Inicializa a linha;
            if (i == DES_PAVIMENTO) if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0);    //--- Circula o ponto.

            //--- Desenha as tangentes.

            while(Atual)
            {
              PIVAtual = &CurvasVerticais.LCurvasVerticais.GetNext(Atual);

              if (i == DES_PAVIMENTO)
              {
                P.x = (int) (Deltas[X] + (PIVAtual->PIV.EstVirtual * Escala[X]));
                P.y = (int) (Deltas[Y] + (Resolucao[Y] - (PIVAtual->PIV.Cota-hPavim) *Escala[Y]));
                pDC->LineTo(P);                              //--- Desenha a linha.
                if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0);   //--- circula o ponto, se estiver desenhando o greide do pavimento
              }
              else    //--- No greide da terraplanagem não desenha as tangentes dentro das parábolas
              {
                P.x = (int) (Deltas[X] + (PIVAtual->PCV.EstVirtual * Escala[X]));
                P.y = (int) (Deltas[Y] + (Resolucao[Y] - (PIVAtual->PCV.Cota-hPavim) *Escala[Y]));
                pDC->LineTo(P);    //--- Desenha do PTV ao PCV.

                P.x = (int) (Deltas[X] + (PIVAtual->PTV.EstVirtual * Escala[X]));
                P.y = (int) (Deltas[Y] + (Resolucao[Y] - (PIVAtual->PTV.Cota-hPavim) *Escala[Y]));
                pDC->MoveTo(P);                              //--- Pula a tangente na parábola
              }
            }  

            //--- Desenha as parábolas

            Atual = CurvasVerticais.LCurvasVerticais.GetHeadPosition();     //--- Aponta para o PIV a desenhar a parábola.

            CurvasVerticais.LCurvasVerticais.GetNext(Atual);   //--- Pula a primeira parabola ( o Y = 0, obrigatóriamente)

            if (Atual)     //--- Para haver parábolas é preciso que haja mais de de 2 PIVs.
            {
              while(Atual)
              {
                CurVertical* PIVAtual = &CurvasVerticais.LCurvasVerticais.GetNext(Atual);
                if (PIVAtual->Y1 > 0.5) DesenhaParabola(PIVAtual,pDC,hPavim,i == DES_PAVIMENTO);       //--- Se o Y > .5m desenha a parábola
              }                                                              //--- Y < .5 não são aceitos. 
            }
          }
        }
        if (EMedicao)   		          //--- Desenha as mediçoes
        {
          Pena[MEDICAO].CreatePen(PS_SOLID,1,RGB(0,160,0));
          pDC->SelectObject(&Pena[MEDICAO]);    

          POSITION MedicaoAtual = ListaMedicoes.GetHeadPosition();

          if(MedicaoAtual) while (MedicaoAtual) 
            DesenhaPLine(*(ListaMedicoes.GetNext(MedicaoAtual)),pDC);
        }
      }

      //--- Desenha OAC

      //--- Provisório, Criar a lista com as cotas calculadas no construtor

      pDC->SelectObject(&Pena[PROJETO]);
      pDC->SetTextColor(Cores[CORTEXTO]);
      pDC->SelectObject(&CurrierNewVertical);
      
      for (POSITION Atual = OACs.LOAC.GetHeadPosition() ; Atual ; OACs.LOAC.GetNext(Atual))
      {
        CEstaca& Est(OACs.LOAC.GetAt(Atual).Estaca);
        double Cota(Est.Cota == INFINITO ? CurvasVerticais.CalculaCota(OACs.LOAC.GetAt(Atual).Estaca) : Est.Cota);

        if(Cota != INFINITO)
        {
          CPoint P((int) (Deltas[X] + (Est.EstVirtual * Escala[X])),
                   (int) (Deltas[Y] + (Resolucao[Y] - (Cota) *Escala[Y])));

          pDC->MoveTo(P.x,P.y + 100);   
          pDC->LineTo(P.x,P.y);
          if(CircularPontos) pDC->Arc(P.x-5,P.y-5,P.x+5,P.y+5,0,0,0,0);   
        }
      }

      //--- Desenha OAE
      //--- Provisório, Criar a lista com as cotas calculadas no construtor
     
      for (POSITION Atual = OAEs.LOAE.GetHeadPosition() ; Atual ; OAEs.LOAE.GetNext(Atual))
      {
         OAE& OAEAtual(OAEs.LOAE.GetAt(Atual));
        
        double CotaIni(CurvasVerticais.CalculaCota(OAEAtual.EstInicial)),
               CotaFim(CurvasVerticais.CalculaCota(OAEAtual.EstFinal));

        CPoint P((int) (Deltas[X] + (OAEAtual.EstInicial.EstVirtual * Escala[X])),
          (int) (Deltas[Y] + (Resolucao[Y] - (CotaIni) *Escala[Y])));

        if(CotaIni != INFINITO)
        {
          pDC->MoveTo(P.x,P.y+100);   
          pDC->LineTo(P.x,P.y);
          if(CircularPontos) pDC->Arc(P.x-5,P.y-5,P.x+5,P.y+5,0,0,0,0);   
        }

        P.x = (int) (Deltas[X] + (OAEAtual.EstFinal.EstVirtual * Escala[X]));
        P.y = (int) (Deltas[Y] + (Resolucao[Y] - (CotaFim) *Escala[Y]));

        if(CotaFim != INFINITO)
        {
          pDC->MoveTo(P.x,P.y + 100);//Resolucao[Y]);   
          pDC->LineTo(P.x,P.y);
          if(CircularPontos) pDC->Arc(P.x-5,P.y-5,P.x+5,P.y+5,0,0,0,0);   
        }
      }
    }
  }

  if(DesenharPerfGeol) DesenhaPerfisGeologicos(pDC);

  pDC->SelectObject(pPenaAnterior);   //--- Recoloca a pena Anterior.
}
/////////////////////////////////////////////////////////////////////////////
// DesPerfilView diagnostics

#ifdef _DEBUG
void DesPerfilView::AssertValid() const
{
  CView::AssertValid();
}

void DesPerfilView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// DesPerfilView message handlers

void DesPerfilView::DesenhaPLine(CListaSecoes& Secoes,CDC* pDC)
{
  POSITION SecAtual = Secoes.GetHeadPosition();
  if (!SecAtual) return;

  CEstaca* EstacaAtual = &Secoes.GetNext(SecAtual).Projeto.Estaca;

  while(SecAtual && EstacaAtual->EstVirtual == INFINITO) 
    EstacaAtual = &Secoes.GetNext(SecAtual).Projeto.Estaca; 

  if (EstacaAtual->EstVirtual != INFINITO) //--- Se achou pelo menos uma inicia o desenho da polyline 
  {
    CPoint P((int) (Deltas[X] + (EstacaAtual->EstVirtual*Escala[X])),
      (int) (Deltas[Y] + (Resolucao[Y] - ((EstacaAtual->Cota) *Escala[Y]))));
    pDC->MoveTo(P);                               //--- Inicializa a linha;
    if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0);    //--- Circula o ponto.

    //--- Desenha os pontos restantes.

    while(SecAtual)
    {
      EstacaAtual = &Secoes.GetNext(SecAtual).Projeto.Estaca;
      if (EstacaAtual->EstVirtual != INFINITO)   //--- A estaca pode estar sem terreno.
      {
        P.x = (int) (Deltas[X] + (EstacaAtual->EstVirtual*Escala[X]));
        P.y = (int) (Deltas[Y] + (Resolucao[Y] - (EstacaAtual->Cota) *Escala[Y]));
        pDC->LineTo(P);                              //--- Desenha a linha.
        if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0);   //--- circula o ponto
      }
    }  
  }
}

void DesPerfilView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();

  CMainFrame* MainFrame((CMainFrame*) AfxGetMainWnd());
  std::string NomeArquivo(MainFrame->PegaProjetoAtual() + std::string(".ini"));

  //--- le o estado anterior da configuração do desenho e atualiza-os 

  char Buffer[512] = {0};

  std::string Secao(std::string("DadosDesenhos_") + MainFrame->PegaRuaAtual());

  ::GetPrivateProfileString(Secao.c_str(),_T("DadosDesPerfilConfig"),_T("1,1,1,1,1,1,1,1,20,2,2,1,1,1,1,1,1,1,1,1,1"),Buffer,511,NomeArquivo.c_str());
  _stscanf(Buffer,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",&DesenharPixel,&DesenharCirculo,&DesenharCruz, &GabaritarProjeto,&DesenharProjeto,
    &DesenharTerreno,&DesenharMalha,&Cotar,&EquiMalhaX,&EquiMalhaY,&EquiCotas,&MostrarDesc,&SincronizarSecoes,&MostrarTitulo,&VarrerTerreno,&VarrerProjeto,
    &AlterarGreide,&SecoesIconizadas,&PermitirArrastar,&CircularPontos,&DesenharPerfGeol);

  ((CChildFrame*) GetParentFrame())->m_wndToolBarGre.GetToolBarCtrl().SetState(ID_BUTSINCRO,SincronizarSecoes);
  ((CChildFrame*) GetParentFrame())->m_wndToolBarGre.GetToolBarCtrl().SetState(ID_BUTLOCK,!AlterarGreide);
  ((CChildFrame*) GetParentFrame())->m_wndToolBarGre.GetToolBarCtrl().SetState(ID_SECSEMPREVIS_ICONIZAR,SecoesIconizadas); 
  ((CChildFrame*) GetParentFrame())->m_wndToolBarGre.GetToolBarCtrl().SetState(ID_BUTARRASTAR,PermitirArrastar); 
  ((CChildFrame*) GetParentFrame())->m_wndToolBarGre.GetToolBarCtrl().SetState(ID_BUTDESPERFGEO,DesenharPerfGeol); 

  //--- mostra ou não o título da janela

  if (!MostrarTitulo) ((CChildFrame*) GetParentFrame())->ModifyStyle(WS_CAPTION,0,SWP_FRAMECHANGED);  
  //--- Pega as cores

  ::GetPrivateProfileString(Secao.c_str(),_T("DadosDesPerfilCores"),_T("0,4227072,12632256,255,33023,16711680"),Buffer,511,NomeArquivo.c_str());
  _stscanf(Buffer,"%lu,%lu,%lu,%lu,%lu,%lu",&Cores[0],&Cores[1],&Cores[2],&Cores[3],&Cores[4],&Cores[5]);

  MouseXYGre = (CXYGreide *) MainFrame->MouseXYGre;

  //--- Pega o status da janela e atualiza-a

  ::GetPrivateProfileString(Secao.c_str(),_T("DadosPerfil"),_T("-1.0,10.0,1.0,0.0,0.0"),Buffer,511,NomeArquivo.c_str()); 

  _stscanf(Buffer,"%lf,%lf,%lf,%lf,%lf",&Escala[X],&Escala[Y],&RazaoHV,&Deltas[X],&Deltas[Y]);

  Escala[X] = -1.0;

  if(Escala[X] == -1.0) OnEnquadrar();

  //DefineParametros();

  //--- Inicializa os valores da escala, da razão e da tolerância.

  ((CChildFrame*) GetParentFrame())->DeslizantesGre.MudarEscala(Escala[X]*1.0);
  ((CChildFrame*) GetParentFrame())->DeslizantesGre.MudarRazao(RazaoHV);

  if(SecoesIconizadas) IconizaSecao(); 

  ::GetPrivateProfileString(_T("SettingsChild"),_T("StatusVisualizarPerfil"),_T("1,1"),Buffer,511,NomeArquivo.c_str());	

  _stscanf(Buffer,"%i,%i",&MostrarToolCoordenadas,&MostrarPonto);

  MainFrame->ShowControlBar(MainFrame->PegaBarra(5),MostrarToolCoordenadas, FALSE);	

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

  CFont* LF = pDC->GetCurrentFont();  
  LOGFONT Log;

  LF->GetLogFont(&Log);
  Log.lfEscapement = 900 ;

  strcpy(Log.lfFaceName,"Courier New");
  CurrierNewVertical.CreateFontIndirect(&Log);

  Log.lfEscapement = 0;
  CurrierNewHorizontal.CreateFontIndirect(&Log);

  CArqTiposSolos ArqTiposSolos(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual());
  ArqTiposSolos.CriaSet(SetTiposSolos);
  ArqTiposSolos.ConverteGDI(SetTiposSolos);

  if(PerfilGeologico.PegaPerfisSolos().size() == 0) this->DesenharPerfGeol = false;
}

void DesPerfilView::CalculaParametros()
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

  //--- A escala mudou, acerta as tolerâncias (5 pixels em qualquer eixo);

  Toleranciax = 4 / Escala[X];
  Toleranciay = 4 / Escala[Y];

  Toleranciaxy_2 = pow(Toleranciax,2.0) + pow(Toleranciay/RazaoHV,2.0);

}

BOOL DesPerfilView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
  // TODO: Add your specialized code here and/or call the base class

  dwStyle |= WS_MAXIMIZEBOX | WS_SYSMENU |WS_MINIMIZEBOX | WS_MAXIMIZE;

  return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void DesPerfilView::OnMouseMove(UINT nFlags, CPoint point) 
{
  SetCursor(LoadCursor(NULL,CursorAtual));
  
  //--- Se não houve movimento do mouse retorna;

  if(PonMousePixelAtual == point) return;

  PonMousePixelAtual = point;

  //--- Se estiver executando um zoom, desenha o retangulo e sai

  if(EZoom)
  {
    FimZoom = point;
    RedrawWindow();

    return;
  }

  static POSITION PosAnterior(NULL);

  //--- Mostra as coordenadas de onde o mouse está, Estaca e cota.

  TransfPixelsXY(point,MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY);  //--- Pega as coordenadas (em metros)do ponto local o mouse está.
  MouseXYGre->UpdateData(FALSE);

  if (Arrastando)     //--- Está arrastando?
  {
    bool Ok(true);

    //--- Esta variavel (ArrastandoPonto) se faz necessária porque quando o mouse der pulos
    //--- Maiores que a tolerância de aproximação do ponto (tolerâncias x e y) o ponto não 
    //--- seria mais encontrado nas chamadas a PosicionaPonto (do perfil), fazendo-o parar 
    //--- de ser arrastado no meio da operação. Esta variável assegura o arrasto do ponto
    //--- Independentemente dos saltos do mouse, ela apenas retorna a FALSE , no final da 
    //--- operação, no método LPbuttonUP.

    if (AlterarGreide && ArrastandoPonto && Tipo == PROJETO)    //--- Esta Arrastando um ponto  de projeto?
    {
      //--- Sim, Altera o ponto

      switch (TipoPonto)
      {
        case ESTPCV : Ok = CurvasVerticais.AlteraY1(MouseXYGre->m_DOCoorX,0.0,CURVER_PONMOUSE);break;
        case ESTPIV : Ok = CurvasVerticais.AlteraPIV(CEstaca(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY),PAtual);	break;
        case ESTPTV : Ok = CurvasVerticais.AlteraY2(MouseXYGre->m_DOCoorX,0.0,CURVER_PONMOUSE);break;
      }

      //--- Alterou o greide altera então o desenho da seção com as cotas já alteradas. (tudo on line....)

      if(Ok)
      {
        if(AtualizarSecoes) AcerDesSecao() ;    //--- Acerta o desenho da secao

        CRect Rect;
        GetWindowRect(&Rect);
        if(MostrarDesc) DPopUpPonto.MostraDescricao(point.x+0+Rect.left,point.y+0+Rect.top,PreparaDescricao(PAtual,PROJETO));

        if(!AlterouGreide)
        {
          AlterouGreide = true;      //--- houve Alteração no greide (Então gravará o novo estado do greide quando sair)
          ((CMainFrame*) AfxGetMainWnd())->InsereDesenhoAlterado(Trecho,std::string("DesPerfil"),this);
		}
      }
      else Arrastando = ArrastandoPonto = false;
    }
    else     //--- Não esta arrastando um ponto, então está arrastando o desenho.
    {
      if(PermitirArrastar)   
      {
        Deltas[X] += point.x - InicioArrast.x;  
        Deltas[Y] += point.y - InicioArrast.y;

        InicioArrast = point;
        DeltaYMetro = Deltas[Y] / Escala[Y];    //--- Deslocamento do Arrasto.
      }
    }
    // RedesenharJanela();   //--- Esta arrastando alguma coisa, então redesenha a janela.
    if(Ok)  RedrawWindow();
  }
  else   //--- Não está arrastando nada...
  {
    //--- Se o mouse estiver sobre algum ponto mostra as coordenadas do ponto.
    //--- Caso contrário, apaga o diaslogo popup

    //int Tipo;
    CString NumMed;

    POSITION PosMouse(VerfPonto(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY,Tipo,TipoPonto,NumMed));

    if (PosMouse || (TipoPonto == PONTOPERFILSONDAGEM && pairResp.first != NULL)) 
    {
      if((Tipo == TERRENO && VarrerTerreno) || (Tipo == PROJETO && VarrerProjeto) || (Tipo == PONTOPERFILSONDAGEM) || Tipo == PONTOOAC ||
          Tipo == PONTOOAEINICIAL || Tipo == PONTOOAEFINAL)
      {
        if (PosAnterior != PosMouse || Tipo == PONTOPERFILSONDAGEM)  //---PosAnterior != pos para evitar chamadas desnecessarias ao desenho da seção
        {
          PosAnterior = PosMouse;  //--- Atualiza a anterior

          CRect Rect;
          GetWindowRect(&Rect);
          
          if(MostrarDesc) 
            DPopUpPonto.MostraDescricao(point.x+0+Rect.left,point.y+0+Rect.top,PreparaDescricao(PosMouse,Tipo == TERRENO && DesenharPerfGeol ? PONTOPERFILSONDAGEM : Tipo));

          if (Tipo == TERRENO || Tipo == MEDICAO)                            //--- Só atualiza o desenho das seções se o ponto achado for de terreno
          {                                                                  //--- e a difernça entres as cotas for > 0.0005. Se for medição só mostra a secao  
            CEstaca Estaca(Secoes.GetAt(PosMouse).Estaca);

            if (!EMedicao)                     //--- Se não for medição recalcula a cota do greide e manda desenhar a senão, se houve 
            {                                  //--- alteração na seção ela será mostrada.
              CurvasVerticais.CalculaCota(Estaca);

              if(SincronizarSecoes)     
                DesenhoSecao.Posiciona(Estaca.EstVirtual,Estaca.Cota);  //--- posiciona o desenho da secao, verificando se houve alteração de cota
            }
            else if(SincronizarSecoes) 
              DesenhoSecao.Posiciona(Estaca.EstVirtual);           //--- É medição, só posiciona o desenho da secao
          }
        }
      }
    }
    else 
    {
      PAtual = PosAnterior = NULL;
      DPopUpPonto.ShowWindow(false);    //--- Se não estiver sobre um ponto apaga o pop up
    }
  }
}

void DesPerfilView::TransfPixelsXY(CPoint& Ponto,double& Coorx,double& Coory)
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

void DesPerfilView::MudarEscala(int Fator)
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

void DesPerfilView::MudarRazao(double Fator)
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

void DesPerfilView::VisualizarPonto()
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

POSITION DesPerfilView::VerfPonto(double x,double y,int& Tipo,int& TipoPonto,CString& NumMed)
{
  //--- Verifica Se o mouse esta sobre algum ponto de terreno, de projeto ou do
  //--- pavimento, caso esteja posiciona o ponteiro do perfil para o ponto em questão,
  //--- através da mensagem " PosicionaPonto";

  static CLPontos LPontoPerfGeo;             //--- Lista criada apenas para compatibilizar o retorno desta função com os perfís geológicos

  if((VarrerTerreno && DesenharTerreno) || DesenharPerfGeol)    //--- Mesmo que esteja mostrando só os Perfils Geol, é precisa achar o terreno para sincronizar as seções
  {
    Tipo = TERRENO;   //--- Assume que achou um ponto de terreno.

    PAtual = Secoes.PosicionaEstaca(x,y,Toleranciax,Toleranciay,TERRENO);  //--- TEM q mandar o projeto, algumas estacas de terreno estao com INFINITO

    if (PAtual) return PAtual;
  }

  Tipo = PROJETO;

  PAtual = VarrerProjeto ? CurvasVerticais.PosicionaPontoNotavel(x,y,Toleranciax,Toleranciay,TipoPonto) : NULL;

  if (PAtual) 
  {
    CurvaOriginal = CurvasVerticais.LCurvasVerticais.GetAt(PAtual);     //--- Salva a curva sendo alterada para poder restaurar
    return PAtual;
  }

  //--- Verifica nas medições

  Tipo = MEDICAO;

  POSITION MedicaoAtual = ListaMedicoes.GetHeadPosition();

  while (MedicaoAtual && !PAtual)
  {
    CListaSecoes& LMedAtual = *ListaMedicoes.GetNext(MedicaoAtual); 

    PAtual = LMedAtual.PosicionaEstaca(x,y,Toleranciax,Toleranciay,PROJETO); 
    if(PAtual) 
    {
      NumMed = LMedAtual.PegaProjeto().Right(3);
      return PAtual;
    }
  }

  //--- Verifica nos perfís geológicos

  if (DesenharPerfGeol)
  {
    Tipo = PONTOPERFILSONDAGEM;

    pairResp = PerfilGeologico.VerfPontoPerfilDesenho(CEstaca(x,y),Toleranciaxy_2,NULL,Escala[Y]); 

    if(pairResp.first != NULL) 
    {
      TipoPonto = PONTOPERFILSONDAGEM;
      return NULL;                         //--- O Ponto está em pairResp
    }
  }

  if (DesenharProjeto)
  {
    Tipo = PONTOOAC;

    PAtual = OACs.PosicionaEstaca(x,y,Toleranciax,Toleranciay,SO_X);  

    if(PAtual) 
    {
      TipoPonto = PONTOOAC;
      return PAtual;
    }
  }

  if (DesenharProjeto)
  {
    Tipo = PONTOOAEINICIAL;

    int InicioFim(0);

    PAtual = OAEs.PosicionaEstaca(x,y,Toleranciax,Toleranciay,&InicioFim,SO_X);  

    if(PAtual) TipoPonto = Tipo = InicioFim == 0 ? PONTOOAEINICIAL : PONTOOAEFINAL;
  }
  
  return PAtual;
}

std::string DesPerfilView::PreparaDescricao(POSITION Pos,int Tipo) 
{
  std::strstream strstreamTemp;

  strstreamTemp << std::right << std::fixed << std::setfill(' ') << std::setprecision(7);

  switch(Tipo)
  {
    case TERRENO:
    {
      CSecao& SecaoAtual = Secoes.GetAt(Pos);

      strstreamTemp << " Terreno:"; 

      if (SecaoAtual.Estaca.EstVirtual != INFINITO) strstreamTemp  << " \r\n Estaca: " << SecaoAtual.Estaca.GetBuffer();
      if (SecaoAtual.Estaca.Descricao.GetLength()) strstreamTemp << " - " << SecaoAtual.Estaca.Descricao;
      if (SecaoAtual.IAnte != INFINITO)  strstreamTemp << " \r\n I1= " << SecaoAtual.IAnte;
      if (SecaoAtual.IPost != INFINITO)  strstreamTemp << " \r\n I2= " << SecaoAtual.IPost;

      strstreamTemp << std::setprecision(3);

      if (SecaoAtual.Estaca.Cota  != INFINITO) strstreamTemp <<  " \r\n Cota= " << SecaoAtual.Terreno.Estaca.Cota;      
    }break;

    case PROJETO:
    {
      //--- Pega o ponto da curva Vertical, verifica qual é e mostra.

      CurVertical& PIVAtual = CurvasVerticais.LCurvasVerticais.GetAt(Pos);

      switch(TipoPonto)
      {
        case ESTPCV : 
        {
          strstreamTemp << " PCV:"
                        << " \r\n Estaca= " << PIVAtual.PCV.GetBuffer(1);
          if(PIVAtual.I1 != INFINITO) strstreamTemp << " \r\n i1= " << PIVAtual.I1;
          strstreamTemp << " \r\n Cota= " << std::setprecision(3) << PIVAtual.PCV.Cota;
        }
        break;
        case ESTPIV : 
        {
          strstreamTemp << " PIV:"
            << " \r\n Estaca= " << PIVAtual.PIV.GetBuffer(1) ;
          if(PIVAtual.I1 != INFINITO) strstreamTemp << " \r\n i1= " << PIVAtual.I1;
          if(PIVAtual.I2 != INFINITO) strstreamTemp << " \r\n i2= " << PIVAtual.I2;
          strstreamTemp << " \r\n Cota= " << std::setprecision(3) <<  PIVAtual.PIV.Cota << " \r\n Y1= " << PIVAtual.Y1 << " \r\n Y2= " << PIVAtual.Y2;
          if(PIVAtual.E != INFINITO) strstreamTemp  << " \r\n E= " << PIVAtual.E;
          if(PIVAtual.I0 != INFINITO) strstreamTemp  << " \r\n i0= " << CEstaca(PIVAtual.I0).GetBuffer();
        }
        break;
        case ESTPTV : 
        {
          strstreamTemp << " PTV:"
            << " \r\n Estaca= " << PIVAtual.PTV.GetBuffer(1);
          if(PIVAtual.I2 != INFINITO) strstreamTemp << " \r\n i2= " << PIVAtual.I2;
          strstreamTemp << " \r\n Cota= " << std::setprecision(3) << PIVAtual.PCV.Cota;
        }
        break;   
        default:
        {
          CSecao& SecaoAtual = Secoes.GetAt(Pos);

          strstreamTemp << " Medição:"
                        << " \r\n Estaca= " << SecaoAtual.Projeto.Estaca.GetBuffer(1)   
                        << " \r\n Cota= " << std::setprecision(3) <<  SecaoAtual.Projeto.Estaca.Cota
                        << " \r\n i1= " << SecaoAtual.IAnte
                        << " \r\n i2= " << SecaoAtual.IPost;
        }
      }
    }
    break;
    case PONTOPERFILSONDAGEM:
    {
      if(pairResp.first)
      {
         strstreamTemp << std::setprecision(3);

        ittyPerfilTipoSolo* itPerfilTipoSolo(pairResp.first);
        strstreamTemp << "Perfil Solo: " 
                      << "\r\n Tipo = " << (*itPerfilTipoSolo)->Descricao
                      << "\r\n Estaca= " << (CEstaca)(**itPerfilTipoSolo)
                      << "\r\n Cota = " << (*itPerfilTipoSolo)->Cota; 
      }
    }
    break;
    case PONTOOAC:
    {
      const OAC& OACAtual(OACs.LOAC.GetAt(Pos));

      strstreamTemp << " Obra de Arte Corrente: " 
                    << "\r\n Estaca: " << OACAtual.Estaca 
                    << "\r\n Descrição: " << OACAtual.Descricao;
    }
    break;
    case PONTOOAEINICIAL:
    {
      const OAE& OAEAtual(OAEs.LOAE.GetAt(Pos));

      strstreamTemp << " Início de Obra de Arte Especial: " 
                    << "\r\n Estaca : " << OAEAtual.EstInicial 
                    << "\r\n Descrição: " << OAEAtual.Descricao;
    }
    break;
    case PONTOOAEFINAL:
    {
      const OAE& OAEAtual(OAEs.LOAE.GetAt(Pos));

      strstreamTemp << " Fim de Obra de Arte Especial: " 
                    << "\r\n Estaca : " << OAEAtual.EstFinal 
                    << "\r\n Descrição: " << OAEAtual.Descricao;
    }
    break;
  }

  strstreamTemp << std::ends;

  std::string Ret(strstreamTemp.str());
  strstreamTemp.freeze(false);

  return Ret;
}

void DesPerfilView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  if (Arrastando) return;

  //--- Se a tecla CTRL estiver pressionada e o mouse estiver em cima de algum piv, mostra o diálogo de edição do piv

  if (AlterarGreide && (nFlags & MK_CONTROL))
  {
    CString NumMed;

    POSITION PosMouse(VerfPonto(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY,Tipo,TipoPonto,NumMed));

    if (PosMouse && Tipo == PROJETO) 
    {
      //--- Salva os dados antes do dialogo

      CurVertical  PIVAtual(CurvasVerticais.LCurvasVerticais.GetAt(PosMouse));
      CurVertical& PIVAntigo(CurvasVerticais.LCurvasVerticais.GetAt(PosMouse));

      //--- Pede os novos dados

      CDEntraPontoGreide DEntraPontoGreide(PIVAtual.PIV,PIVAtual.PIV.Igualdade,PIVAtual.PIV.Cota,PIVAtual.I1,PIVAtual.I2,PIVAtual.Y1,PIVAtual.Y2,point,PROJETO,this);

      //--- Executa o diálogo.

      if (DEntraPontoGreide.DoModal() == IDOK)
      {
        //--- Se Alterou alguma coisa procede a alteração.

        CEstaca EstacaAgora(PIVAtual.PIV,PIVAtual.PIV.Igualdade,"PIV",PIVAtual.PIV.Cota);    //--- costroi o valor da estaca que veio do diálogo

        if (PIVAntigo.PIV != EstacaAgora) 
        {
          CurvasVerticais.AlteraEstaca(EstacaAgora,PIVAtual.PIV);
          PIVAtual.PIV = EstacaAgora;
        }
        if (fabs(PIVAntigo.PIV.Cota - PIVAtual.PIV.Cota) > 0.0009) CurvasVerticais.AlteraCota(PIVAtual.PIV);
        if (fabs(PIVAntigo.I1 - PIVAtual.I1) > 1E-12) CurvasVerticais.AlteraI1(PIVAtual,PIVAtual.I1);
        if (fabs(PIVAntigo.I2 - PIVAtual.I2) > 1E-12) CurvasVerticais.AlteraI2(PIVAtual,PIVAtual.I2);
        if (fabs(PIVAntigo.Y1 - PIVAtual.Y1) > 0.0) CurvasVerticais.AlteraY1(PIVAtual.PIV,PIVAtual.Y1,CURVER_VALOR_Y);
        if (fabs(PIVAntigo.Y2 - PIVAtual.Y2) > 0.0) CurvasVerticais.AlteraY2(PIVAtual.PIV,PIVAtual.Y2,CURVER_VALOR_Y);

        if (!(PIVAntigo == PIVAtual)) 
        {
          if(!AlterouGreide)
          {
            AlterouGreide = true; 
            ((CMainFrame*) AfxGetMainWnd())->InsereDesenhoAlterado(Trecho,std::string("DesPerfil"),this);
          }
        }

        RedrawWindow(); 

        return;          //--- Editou o PIV, retorna
      }
    }
  }

  if(SecoesIconizadas) IconizaSecao();  //--- Se iconizar seções estiver setado, iconiza a seção

  //--- O primeiro click em cima de um ponto inicia o arrasto, no segundo click
  //--- finaliza o arrasto, no primeiro click arrastandoponto fica true até que o usuário click novamente
  //--- quando Terminararrasto fica true, o arrasto termina efetivamente quando o usuário tira o dedo do 
  //--- botão do mouse no métod onlbuttonup. 

  Arrastando = true;     //--- pressionou o botão do mouse então está arrastando alguma coisa... (ou um ponto ou o desenho...)
  InicioArrast = point;	

  double x,y;    
  TransfPixelsXY(point,x,y);  //--- Pega as coordenadas (em metros) do ponto local o mouse está.

  //--- Se o usuário clicou sobre um ponto do projeto,inicia o arrasto do ponto. Esta operação será terminada quando o usuário clicar novamente 
  //--- Caso ele não tenha clicado em um ponto de projeto, clicou no fundo, do mesmo modo inicia o arrasto.

  if (/*AlterarGreide &&*/ !EMedicao && VerfPonto(x,y,Tipo,TipoPonto)) ArrastandoPonto = true;  
}

void DesPerfilView::OnLButtonUp(UINT nFlags, CPoint point) 
{
  //--- Se estiver arrastando um ponto e for o segundo click (Terminar arrasto = true)
  //--- finaliza o arrasto

  if (TerminarArrasto)      
  {
    TerminarArrasto = false; 
    Arrastando = ArrastandoPonto = FALSE;
    RedrawWindow();
  }
  else TerminarArrasto = true;

  CursorAtual = Arrastando ? ArrastandoPonto ? AlterarGreide ? IDC_CROSS : IDC_NO : IDC_SIZEALL : IDC_HAND;
  SetCursor(LoadCursor(NULL,CursorAtual));
}

void DesPerfilView::DesenhaParabola(CurVertical* PIV,CDC* pDC,double hPavim,bool Terraplan) 
{
  //--- Primeiro determina quantas retas serão desenhadas e calcula a equidistância

  const int DistMaxPixels(5);    //--- parâmetro de configuração.
  //--- contém a distância máxima em pixels entre os 
  //--- Pontos que comporão a parábola.

  double d(PIV->PTV.EstVirtual - PIV->PCV.EstVirtual); //--- Distancia do PCV ao PTV

  UINT DistPixels((UINT)( d * Escala[X])),  //--- Distancia em pixels do PCV ao PTV
    NPontos(DistPixels / DistMaxPixels);      //--- Numero de pontos que comporão a parábola

  if (DistPixels < 3) return;       //--- Parabola muito pequena, retorna

  if (NPontos < 5) NPontos = 5;     //--- quantidade mínima de pontos, 5.

  double Equidis(d / NPontos);    //--- Equidistância dos pontos,

  //--- Vamos ao desenho.

  //--- Inicia no PCV.

  CPoint P((int) (Deltas[X] + (PIV->PCV.EstVirtual*Escala[X])),
    (int) (Deltas[Y] + (Resolucao[Y] - (PIV->PCV.Cota-hPavim) *Escala[Y])));
  pDC->MoveTo(P);                               //--- Inicializa a polyline, move para o PCV;
  if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0);    //--- Circula o ponto, PCV.

  //--- desenha as retas

  double x(PIV->PCV.EstVirtual);     //--- x do ponto a ser calculado.

  for(UINT i = 0 ; i < NPontos ; i++)
  {
    x += Equidis;   //--- Valor do x atual

    //--- Calcula a cota do ponto.

    int Lado(x > PIV->PIV.EstVirtual ? POSTERIOR : ANTERIOR);  //--- Verifica o lado da parábola

    double CotaGreideReto(Lado == ANTERIOR ? PIV->I1 * (x - PIV->PCV.EstVirtual) + PIV->PCV.Cota - hPavim :
                                             PIV->I2 * (x - PIV->PTV.EstVirtual) + PIV->PTV.Cota - hPavim), //--- cota do ponto no greide reto. 
      CotaPonto(CotaGreideReto + PIV->e(x,Lado));   //--- Ufa!!!!!

    P.x = (int) (Deltas[X] + (x * Escala[X]));
    P.y = (int) (Deltas[Y] + (Resolucao[Y] - (CotaPonto) *Escala[Y]));
    pDC->LineTo(P);                              //--- Desenha a linha.
  }
  if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0);    //--- Circula o PTV

  if(PIV->I0 != INFINITO) P.x = (int) (Deltas[X] + (PIV->I0 * Escala[X]));
}

void DesPerfilView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  if (!AlterarGreide)
  {
    AfxMessageBox("Este greide não pode ser alterado, para alterá-lo abra o cadeado.");
    return;
  }

  if (Secoes.IsEmpty() || CurvasVerticais.VerfErro())
  {
    AfxMessageBox("Não há seçoes...");
    return;
  }

  double x,y;
  TransfPixelsXY(point,x,y);  //--- Transforma as coordenadas de pixels para metros.

  //--- Inclui o PIV no perfil.

  CurVertical CurvaAInserir(x,y,0,1,1,"PIV");   //--- Cria o PIV com 2 de Y

  if(CurvasVerticais.VerfInsercao(CurvaAInserir))
  {
    CurvasVerticais.InsOrdenada(CurvaAInserir);  //--- Insere-o no perfil

    CurvasVerticais.AlteraPIV(CEstaca(x,y),CurvasVerticais.PegaAtual());  //--- Vai para a rotina de alteração para 
                                                                          //--- recálculo dos pcvs ptvc e etc.
    if(!AlterouGreide)
    {
      AlterouGreide = true;
      ((CMainFrame*) AfxGetMainWnd())->InsereDesenhoAlterado(Trecho,std::string("DesPerfil"),this);
    }
    
    Arrastando = false;
    OnLButtonDown(nFlags,point); 
    TerminarArrasto = false; 

    RedrawWindow();
  }
}

void DesPerfilView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  double x,y;
  TransfPixelsXY(point,x,y); 

  POSITION Pos(VerfPonto(x,y,Tipo,TipoPonto));

  if(AlterarGreide && Pos && Tipo == PROJETO) 
  {
    CurvasVerticais.RemovePIV(Pos);

    if(!AlterouGreide)
    {
      AlterouGreide = true;
      ((CMainFrame*) AfxGetMainWnd())->InsereDesenhoAlterado(Trecho,std::string("DesPerfil"),this);
    }

    RedrawWindow();
  }
}

BOOL DesPerfilView::PreCreateWindow(CREATESTRUCT& cs) 
{
  // cs.style |= CS_OWNDC;
  return CView::PreCreateWindow(cs);
}

void DesPerfilView::OnSetFocus(CWnd* pOldWnd) 
{
  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());

  MainFrame->DesPerfil = this;

 // MainFrame->DesSecao = MainFrame->PegaSecoes();

  /* SendMessageToDescendants( 
  WM_SETFOCUS, 
  0, 
  0, 
  TRUE, 
  TRUE);*/ 

  //--- Se a janela da seção ainda não foi destruida coloca-a no topo

  //--- Não consegui usar o WS_EX_PALLETTEWINDOW nem TOPMOST WINDOW nas on_Create ou no on_Create_EX, tanto nas views 
  //--- como no childfrm....

  //((CChildFrame*) GetParentFrame())->DeslizantesGre.BringWindowToTop();
  ((CChildFrame*) GetParentFrame())->DeslizantesGre.PostMessage(WM_SETFOCUS, 0, 0);  //--- SetFocus anula o mousewheel

   std::string NomeProjeto(Projeto.substr(0,Projeto.find_last_of('\\')));
   NomeProjeto = NomeProjeto.substr(0,NomeProjeto.find_last_of('\\'));

   int Sel(MainFrame->m_ProjetosBox.FindString(0,NomeProjeto.c_str()));

   if (Sel == LB_ERR) 
   {
     MainFrame->m_ProjetosBox.InsertString(0,NomeProjeto.c_str());
     MainFrame->m_ProjetosBox.SetCurSel(0);

   }
   else MainFrame->m_ProjetosBox.SetCurSel(Sel);

   MainFrame->m_sNomeComp = NomeProjeto.c_str();

   ((CEstApp*) AfxGetApp())->MudouProjetoSelecionado(NomeProjeto);

   MainFrame->SelecionaRua(Trecho);

   DesSecaoView* pDesSecoes((DesSecaoView *)MainFrame->PegaDesenhoSecoes());

   if (pDesSecoes)
   {
     pDesSecoes->GetParentFrame()->BringWindowToTop();
     pDesSecoes->OnSetFocus(this);
   }
}

void DesPerfilView::OnRButtonUp(UINT nFlags, CPoint point) 
{
  if (Arrastando) return;

  //--- Se estiver em um zoom executa-o

  if (EZoom)
  {
    FimZoom = point;
    EZoom = false;

    //--- Zoom mínimo 4 pixels em qualquer sentido

    if (abs(InicioZoom.x - FimZoom.x) > 4 && abs(InicioZoom.y - FimZoom.y) > 4) Zoom();
    return;
  }


  if (!(nFlags & MK_CONTROL)) 
  { 
    OnConfDesGreide();   //--- Clicou no fundo, mostra o dialogo de configuração e retorna
    return;
  }

}

void DesPerfilView::CalculaCotaEPosiciona(CEstaca& Estaca) 
{
  //--- Este método é chamado quando vem do combo procurar das secoes, como neste caso não se sabe nada 
  //--- da seção, o redesenho é obrigatório

  double E,I1,I2;

  double Cota(CurvasVerticais.CalculaCota(Estaca,E,I1,I2));

  DesenhoSecao.Posiciona(Estaca.EstVirtual,Cota);  //--- posiciona o desenho da secao
}

void DesPerfilView::AcerDesSecao() 
{
  CEstaca Estaca(Secoes.GetAt(DesenhoSecao.PegaSecAtual()).Estaca);

  if (Estaca != CEstaca(INFINITO))
  {
    double E, I1, I2;

    double CotaAnte(Estaca.Cota), Cota(CurvasVerticais.CalculaCota(Estaca, E, I1, I2));

    if (fabs(Cota - CotaAnte) > 0.0005)
    {
      DesenhoSecao.Posiciona(Estaca.EstVirtual, Cota);
    }
  }
}

void DesPerfilView::OnGravarperfil() 
{
  if (AlterouGreide) 
  {
    CurvasVerticais.Serialize();
    AlterouGreide = false;
    ((CMainFrame*) AfxGetMainWnd())->GreideAlterado = true;
    ((CMainFrame*) AfxGetMainWnd())->RemoveDesenhoAlterado(Trecho,std::string("DesPerfil"));
  }
  // if (DesenhoSecao) DesenhoSecao.GravarSecoes(); //esta estragando a movimentação do mouse cota do eixo esta ficando com infinito em alguns pontos
}

void DesPerfilView::OnUpdateGravarperfil(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(AlterouGreide);	
}

void DesPerfilView::OnUpdateDesgrevoltar(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(AlterouGreide);	
}

BOOL DesPerfilView::OnEraseBkgnd(CDC* pDC) 
{
  return FALSE;                                             
}

void DesPerfilView::OnConfDesGreide() 
{
  CRect Rect;

  GetClientRect(Rect) ;
  ClientToScreen(Rect);

  int Dados[19];

  Dados[0] = DesenharPixel;
  Dados[1] = DesenharCirculo;
  Dados[2] = DesenharCruz;
  Dados[3] = GabaritarProjeto;
  Dados[4] = DesenharProjeto;
  Dados[5] = DesenharTerreno;
  Dados[6] = DesenharMalha;
  Dados[7] = Cotar;
  Dados[8] = EquiMalhaX;
  Dados[9] = EquiCotas;
  Dados[10] = MostrarDesc; 
  Dados[11] = MostrarToolCoordenadas;
  Dados[12] = SincronizarSecoes;
  Dados[13] = VarrerTerreno;
  Dados[14] = VarrerProjeto;
  Dados[15] = EquiMalhaY;
  Dados[16] = CircularPontos;
  Dados[17] = AtualizarSecoes;
  Dados[18] = DesenharPerfGeol;

  //---Mostra o dialogo, passa os ponteiros para poder alterar os toolbars  on line;

  CDSelGreide SelGreide(this,((CMainFrame*)AfxGetMainWnd())->mpCDesToolGreide,Rect,&Cores[0],&Dados[0]);

  SelGreide.DoModal();

  DesenharPixel = Dados[0];
  DesenharCirculo = Dados[1];
  DesenharCruz  = Dados[2];
  GabaritarProjeto = Dados[3];
  DesenharProjeto = Dados[4];
  DesenharTerreno = Dados[5];
  DesenharMalha = Dados[6];
  Cotar = Dados[7];
  EquiMalhaX = Dados[8];
  EquiCotas = Dados[9];
  MostrarDesc = Dados[10];
  MostrarToolCoordenadas = Dados[11];
  SincronizarSecoes = Dados[12];
  VarrerTerreno = Dados[13];
  VarrerProjeto	= Dados[14];
  EquiMalhaY = Dados[15]; 
  CircularPontos = Dados[16];
  AtualizarSecoes = Dados[17];
  DesenharPerfGeol = Dados[18];

  RedrawWindow();
}

void DesPerfilView::IconizaSecao()
{ 
  CMainFrame* MainFrame((CMainFrame*) AfxGetMainWnd());
  DesSecaoView* pDesSecao((DesSecaoView *)MainFrame->PegaDesenhoSecoes());
  if (!pDesSecao) return;
  pDesSecao->GetParentFrame()->ShowWindow(SW_MINIMIZE);   
  pDesSecao->MostrarVolumes(false);
}

void DesPerfilView::IconizarRestaurarSecao(int Modo)
{ 
  CMainFrame* MainFrame((CMainFrame*) AfxGetMainWnd());
  DesSecaoView* CWDesSecao = (DesSecaoView *)MainFrame->PegaDesenhoSecoes();
  if (!CWDesSecao ) return;
  CWDesSecao->GetParentFrame()->ShowWindow(Modo);   
  CWDesSecao->MostrarVolumes(Modo == SW_RESTORE);
}

void DesPerfilView::OnSecsemprevisIconizar() 
{
  SecoesIconizadas = SecoesIconizadas == false;

  IconizarRestaurarSecao(SecoesIconizadas ? SW_MINIMIZE : SW_RESTORE);
}


void DesPerfilView::OnIrparaponto() 
{
  VisualizarPonto();
}      

void DesPerfilView::DefineParametros() 
{
  //--- Calcula a escala padrão que permite a visualização todo o desenho

  EscalaDefault[X] =  Resolucao[X] / (LimiteDir - LimiteEsq);
  EscalaDefault[Y] =  Resolucao[Y] / (LimiteSup - LimiteInf);
  EscalaDefault[X] < 	EscalaDefault[Y] ? 	EscalaDefault[Y] = 	EscalaDefault[X] : 	EscalaDefault[X] = 	EscalaDefault[Y];

  if(EscalaDefault[X] < 0.1) EscalaDefault[X] = 0.1;

  EscalaDefault[Y] = EscalaDefault[X] * 10;

  RazaoHV = 0.1;

}
void DesPerfilView::Zoom()    
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

  ((CChildFrame*) GetParentFrame())->DeslizantesGre.MudarEscala((int) (Escala[X] * 100));

  CalculaParametros();     //--- Recalcula a altura e a largura do vídeo

  //--- Ajusta os deltas e a escala para caber todo o zoom
  //--- O ponto inferior esquerdo da janela do zoom será o ponto inferior esquerdo da janela do video 

  Deltas[X] = -PonGreide.x * Escala[X];
  Deltas[Y] = PonGreide.y * Escala[Y] - AlturaVideo * Escala[Y]  ;

  RedrawWindow();          //--- redesenha a janela, mostrando o zoom  
}

void DesPerfilView::OnRButtonDown(UINT nFlags, CPoint point) 
{
  if ((nFlags & MK_CONTROL) && !EZoom)
  {
    EZoom = true;
    InicioZoom = FimZoom = point;
    return;
  }
}

void DesPerfilView::OnEnquadrar() 
{
  //--- Passa as escalas e os deltas para o default

  Escala[X] = EscalaDefault[X];
  Escala[Y] = EscalaDefault[Y];

  RazaoHV = 0.1;

  Deltas[X] = LimiteEsq * Escala[X];
  Deltas[Y] = LimiteInf * Escala[Y];

  //--- Reajusta a escala no deslizante

  ((CChildFrame*) GetParentFrame())->DeslizantesGre.MudarEscala(Escala[X]);

  CalculaParametros();

  RedrawWindow();
}

void DesPerfilView::OnSize(UINT nType, int cx, int cy) 
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

void DesPerfilView::OnAlternartitulogreide() 
{
  CChildFrame* CCFrm((CChildFrame*) GetParentFrame());

  MostrarTitulo = (CCFrm->GetStyle() & WS_CAPTION) == 0;             //--- se está sem titulo seta para mostrar o titulo

  if (!MostrarTitulo) CCFrm->ModifyStyle(WS_CAPTION,0,SWP_FRAMECHANGED);    
  else CCFrm->ModifyStyle(0,WS_CAPTION,SWP_FRAMECHANGED);   

  CCFrm->ShowControlBar(&CCFrm->m_wndStatusBar,MostrarTitulo,true);
}

void DesPerfilView::OnButFechar() 
{
  ((CChildFrame*) GetParentFrame())->Fechar();
}

void DesPerfilView::OnUpdateGravar(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(AlterouGreide);	
}

void DesPerfilView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if(Arrastando && !ArrastandoPonto) return;

  switch(nChar)
  {
  case  VK_DELETE:                            //--- deleta um PIV sobre o mouse
    {
      if(!Arrastando && AlterarGreide)
      {
        double x,y;
        TransfPixelsXY(PonMousePixelAtual,x,y); 

        POSITION Pos(VerfPonto(x,y,Tipo,TipoPonto));

        if(Pos && Tipo == PROJETO) 
        {
          if(dialogar.DoModal(9) == IDOK)
          {
            CurvasVerticais.RemovePIV(Pos);
            RedrawWindow();

            if(!AlterouGreide)
            {
              AlterouGreide = true;
              ((CMainFrame*) AfxGetMainWnd())->InsereDesenhoAlterado(Trecho,std::string("DesPerfil"),this);
            }
          }
        }
      }
    }
    break;
  case VK_ESCAPE :                              //--- Retorna um piv vao seu estado original
    {
      if(ArrastandoPonto && Tipo == PROJETO)
      {
        switch (TipoPonto)
        {
          case ESTPCV : CurvasVerticais.AlteraY1(CurvaOriginal.Y1,CURVER_VALOR_Y);break;
          case ESTPIV : CurvasVerticais.AlteraPIV(CurvaOriginal.PIV,PAtual);	break;
          case ESTPTV : CurvasVerticais.AlteraY2(CurvaOriginal.Y2,CURVER_VALOR_Y);break;
        }

        EZoom = ArrastandoPonto = Arrastando = FALSE;
        CursorAtual = IDC_HAND;
        OnMouseMove(0,PonMousePixelAtual);
        RedrawWindow();
      }
    }
    break;
    case VK_TAB :   
    {
       break;  

       CMainFrame* MF((CMainFrame*) AfxGetMainWnd());

       if(GetKeyState(VK_SHIFT) > 0)
       {
         if(MF->GetNextWindow(GW_HWNDNEXT)) 
         {
           MF->GetNextWindow(GW_HWNDNEXT)->BringWindowToTop();
           MF->GetNextWindow(GW_HWNDNEXT)->SetFocus();
         }
       }
       else
       {
        CWnd* pWnd(GetNextWindow(GW_HWNDNEXT)); 

        int i = 0;

        while(i++ < 6/*!(pWnd->GetStyle() & WS_OVERLAPPED*/) pWnd = pWnd->GetNextWindow(GW_HWNDNEXT);

        if(pWnd) 
        {
          pWnd->BringWindowToTop();  //---- Não funciona=====>Verificar
          pWnd->SetFocus();
        }
        return;
       }
    }
    break;
  }

  if(GetKeyState(VK_SHIFT) < 0)
  {
    switch (nChar)
    {
    case 'S': SincronizarSecoes = SincronizarSecoes == false; break;
    case 'A': AtualizarSecoes = AtualizarSecoes == false;break; 
    case 'P': DesenharProjeto = DesenharProjeto == false; break;
    case 'T': DesenharTerreno = DesenharTerreno = false; break;
    case 'M': DesenharMalha = DesenharMalha == false;break; 
    case 'C': Cotar = Cotar == false; break;
    case 'D': MostrarDesc = MostrarDesc == false;break; 
    case 'E': VarrerTerreno = VarrerTerreno == false; break;
    case 'J': VarrerProjeto = VarrerProjeto == false;break; 
    case 'R': CircularPontos = CircularPontos == false;break;
    case 'G': DesenharPerfGeol = DesenharPerfGeol == false;break;
    }
  }

  RedrawWindow();
}

void DesPerfilView::OnSincronizarSecoes()
{
  SincronizarSecoes = !SincronizarSecoes;
}

void DesPerfilView::OnTrancarGreide()
{
  AlterarGreide = AlterarGreide == false;
}

void DesPerfilView::OnCalcVolumes()
{
  DesenhoSecao.OnCalcvolumes();
}

BOOL DesPerfilView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  // TODO: Add your message handler code here and/or call default

  ((CChildFrame*) GetParentFrame())->DeslizantesGre.MouseWheelOn(nFlags, zDelta, pt);

  return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void DesPerfilView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
  // TODO: Add your specialized code here and/or call the base class

  pDC->SetBkColor(Cores[CORFUNDO]);
  //	pDC->SetMapMode(m_mapmode);
  CView::OnPrepareDC(pDC, pInfo);
}

void DesPerfilView::OnUpdateCalcvolumes(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(AlterouGreide);	
}


void DesPerfilView::OnButarrastar()
{
  PermitirArrastar = !PermitirArrastar;
}

void DesPerfilView::OnClose()
{
  if(Alterou || AlterouGreide) GravarProjeto(false,false,true);

  CView::OnClose();
}

void DesPerfilView::DesenhaPerfisGeologicos(CMMemDC* pDC)
{
  if(!pDC || SetTiposSolos.size() == 0) return;

  for(ittysetCPerfilTipoSolo itPerfilSolo=PerfilGeologico.PegaPerfisSolos().begin() ; itPerfilSolo != PerfilGeologico.PegaPerfisSolos().end() ; ++itPerfilSolo)
  {
    CTipoSolo TipoSoloAtual(itPerfilSolo->PegaTipoSolo().c_str());
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

void DesPerfilView::OnButdesperfgeo()
{
  DesenharPerfGeol = DesenharPerfGeol == false;

  RedrawWindow();
}

void DesPerfilView::OnButensaiargreide()
{
 std::string NomeProj(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()));
  pDEnsaiarGreide = new CDEnsaiarGreide(this,NomeProj , MetodoEnsaio, ParametroEnsaio1, ParametroEnsaio2);
  pDEnsaiarGreide->Create(IDD_DIAENSAIARGREIDE, this);
  pDEnsaiarGreide->ShowWindow(SW_SHOW);
}
void DesPerfilView::MostrarApagarSecao(bool Mostrar) 
{
  CMainFrame* MainFrame((CMainFrame*) AfxGetMainWnd());
  DesSecaoView* CWDesSecao((DesSecaoView *)MainFrame->PegaDesenhoSecoes());
  if (!CWDesSecao ) return;
  CWDesSecao->GetParentFrame()->ShowWindow(Mostrar ? SW_SHOW : SW_HIDE);   
}

LRESULT DesPerfilView::EnsaiarGreide(WPARAM WP, LPARAM LP)
{
  if (WP != CDEnsaiarGreide::Ensaios::TERMINAR_ENSAIOS)
  {
    if (WP != CDEnsaiarGreide::Ensaios::REMOVER_ENSAIO_ATUAL)
    {
      if (WP != CDEnsaiarGreide::Ensaios::ADOTAR_ENSAIO_ATUAL)
      {

        UpdateData(true);

        std::vector <double> PerfilOriginal;
        std::vector <double> PerfilSimplificado;

        if (DesenharTerreno && Secoes.GetCount())
        {
          POSITION SecAtual(Secoes.GetHeadPosition());

          CEstaca* EstacaAtual(&Secoes.GetNext(SecAtual).Terreno.Estaca);  //--- Estaca que abrigará as estacas das secoes.

          //--- Procura pela primeira estaca que tenha terreno.

          while (SecAtual && (EstacaAtual->EstVirtual == INFINITO || EstacaAtual->Cota == INFINITO))
            EstacaAtual = &Secoes.GetNext(SecAtual).Terreno.Estaca;

          if (EstacaAtual->EstVirtual != INFINITO) //--- Se achou pelo menos uma inicia o desenho da polyline 
          {
            CPoint P(EstacaAtual->EstVirtual, EstacaAtual->Cota - HLimpeza.BuscaH(*EstacaAtual));

            PerfilOriginal.push_back(EstacaAtual->EstVirtual);
            PerfilOriginal.push_back(EstacaAtual->Cota - HLimpeza.BuscaH(*EstacaAtual));

            //--- Insere os pontos restantes.

            while (SecAtual)
            {
              EstacaAtual = &Secoes.GetNext(SecAtual).Terreno.Estaca;
              if (EstacaAtual->Cota != INFINITO && EstacaAtual->Cota > 1.0)   //--- A estaca pode estar sem terreno.
              {
                PerfilOriginal.push_back(EstacaAtual->EstVirtual);
                PerfilOriginal.push_back(EstacaAtual->Cota - HLimpeza.BuscaH(*EstacaAtual));
              }
            }
          }
        }

        std::vector <double>::const_iterator begin = PerfilOriginal.begin();
        std::vector <double>::const_iterator end = PerfilOriginal.end();

        switch (MetodoEnsaio)
        {
        case CDEnsaiarGreide::Ensaios::ENESIMO_PONTO:
        {
          psimpl::simplify_nth_point <2>(begin, end, ParametroEnsaio1, std::back_inserter(PerfilSimplificado));
        }
        break;
        case CDEnsaiarGreide::Ensaios::DISTANCIA_PERPENDICULAR:
        {
          psimpl::simplify_perpendicular_distance <2>(begin, end, ParametroEnsaio1, std::back_inserter(PerfilSimplificado));
        }
        break;
        case CDEnsaiarGreide::Ensaios::DISTANCIA_RADIAL:
        {
          psimpl::simplify_radial_distance<2>(begin, end, ParametroEnsaio1, std::back_inserter(PerfilSimplificado));
        }
        break;
        case CDEnsaiarGreide::Ensaios::DOUGLAS_PEUCKER_VARIANTE:
        {
          psimpl::simplify_douglas_peucker_n<2>(begin, end, ParametroEnsaio1, std::back_inserter(PerfilSimplificado));
        }
        break;
        case CDEnsaiarGreide::Ensaios::DOUGLAS_PEUCKER:
        {
          psimpl::simplify_douglas_peucker<2>(begin, end, ParametroEnsaio1, std::back_inserter(PerfilSimplificado));
        }
        break;
        case CDEnsaiarGreide::Ensaios::LANG:
        {
          psimpl::simplify_lang<2>(begin, end, ParametroEnsaio1, ParametroEnsaio2, std::back_inserter(PerfilSimplificado));
        }
        break;
        case CDEnsaiarGreide::Ensaios::OPHEIM:
        {
          psimpl::simplify_opheim<2>(begin, end, ParametroEnsaio1, ParametroEnsaio2, std::back_inserter(PerfilSimplificado));
        }
        break;
        case CDEnsaiarGreide::Ensaios::REUMANN_WITKAM:
        {
          psimpl::simplify_reumann_witkam <2>(begin, end, ParametroEnsaio1, std::back_inserter(PerfilSimplificado));
        }
        break;
        case CDEnsaiarGreide::Ensaios::CARLOS_RABELO:
        {

        }
        break;
        }

        double Estaca, Cota;
        GreideSimplificado.clear();

        for (std::vector <double>::const_iterator It = PerfilSimplificado.begin(); It != PerfilSimplificado.end(); It++)
        {
          Estaca = *It++;
          Cota = *It;

          GreideSimplificado.emplace_back(Estaca, Cota);
        }
      }
      else AdotarEnsaioAtual();
    }
    else GreideSimplificado.clear();

    RedrawWindow();

  }
  else pDEnsaiarGreide = nullptr;   //--- Chamou Destruindo CDEnsaiarGreide

  return 0;
}

void DesPerfilView::AdotarEnsaioAtual()
{
  CurvasVerticais.LCurvasVerticais.RemoveAll();

  for (auto& PIAtual : GreideSimplificado)
  {
    CurvasVerticais.InsOrdenada(CurVertical(PIAtual.x,PIAtual.y, 0, 1, 1, "PIV"));   //--- Cria o PIV com 2 de Y
  }

  GreideSimplificado.clear();

  RedrawWindow();
}