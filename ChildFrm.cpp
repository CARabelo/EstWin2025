// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "est.h"
#include "xymouse.h"
#include "cponto.h"
#include "afxtempl.h"
#include "clpontos.h" 
#include "math.h"
#include "Estaca.h"
#include "perfil.h"
#include "ddeslizantes.h"
#include "xygreide.h"
#include "supercstring.h"
#include "carqcurverticais.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "ccurverticais.h"
#include "carqgradientes.h"
#include <set>
#include <string>
#include "cgradientes.h"
#include "palette.h"
#include "mytoolbar.h"
#include "dselsecao.h"
#include "comboprocdessecao.h"
#include "diapontosgeom.h"
#include "deslizantesgeom.h"
#include "meleditbox.h"
#include "ChildFrm.h"
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "afxtempl.h" 
#include <sstream>
#include <iomanip>
#include "estdoc.h"
#include "typlstvw.h"
#include "CMListBox.h"
#include "CDialogoInicialView.h"
#include "CEditEx.h"
#include "curverview.h"
#include "curhorview.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h" 
#include "csectipo.h"
#include "trespontosview.h"
#include "alasarpasview.h"
#include "gradienteview.h"
#include "taludespavimview.h"
#include "cdesview.h"
#include "taludesview.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "carqtalcorte.h"
#include "carqtalaterro.h"
#include "ctalcorte.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include <list>
#include "csecao.h"
#include "clistasecoes.h"
#include "listamedicoes.h"
#include "dvolumes.h"
#include "calcnota.h"
#include <string>
#include "cdpopuppontos.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
#include "carqpistasfixas.h"
#include "cpistasfixas.h"
#include "carqcurhorizontais.h"
#include "Mat.h"
#include "ccurhorizontal.h"
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
#include "listamedicoes.h"
#include "CArqOAC.h"
#include "COAC.h"
#include "CArqOAE.h"
#include "COAE.h"
#include "CMemDC.h"
#include "CDEnsaiarGreide.h"
#include "desperfilview.h"
#include "sectriangulacao.h"
#include <set>
#include "CEquipDrenagem.h"
#include "superficie.h"
#include <vector>
#include "CLembrete.h"
#include "CRadar.h"
#include "CArqAciTopograficos.h"
#include "CTrecho.h"
#include "CArqTrechos.h"
#include "CTrechos.h"
#include "Picture.h"
//#include "ximage.h"
//#include ".\cximage600_full\CxImage\ximage.h"
#include "CSecoesTerrap.h"
#include "ConvUTMGDC\ConvUTMGDC\Utm_Coord_3d.h"
#include "ConvUTMGDC\ConvUTMGDC\Gdc_Coord_3d.h"
#include <array>
#include "ConvUTMGDC\ConvUTMGDC\Gdc_To_Utm_Converter.h"
#include <math.h>
#include "ConvUTMGDC\ConvUTMGDC\CConvUTMGDC.h"
#include "CCamadaDGN.h"
#include "CTextoMapaIBGE.h"
#include "CCNMapaIGBE.h"
#include "CMapaCNIBGE.h"
#include "CDiaImportarArqCtrlDrones.h"
#include "despontosgeomview.h"
#include "OAEView.h"
#include "COACView.h"
#include "dialogo.h"
#include <strstream>
#include "CRegAreaTaludes.h"
#include "CLimpeza.h"
#include "estrel.h"
#include "relatorios.h"
#include "csecoesavulsasview.h"
#include "limpezaview.h"
#include "ImageListBox.h"
#include "IconComboBox.h"
#include "CTiposSolosView.h"
#include "CFurosSondagemView.h"
#include <algorithm>
#include "DesPerfilGeologicoView.h"
#include "EdiAciTopogView.h"
#include "DesPerfilAvulsoView.h"
#include "CRelACADAciTopogView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SECOES     0
#define GREIDE     1
#define GEOMETRICO 2
#define GEOLOGICO  3

extern class dialogo dialogar;

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
  //{{AFX_MSG_MAP(CChildFrame)
  ON_WM_CLOSE()
  ON_WM_CREATE()
  ON_WM_SYSCOMMAND()
  ON_WM_NCMOUSEMOVE()
  ON_UPDATE_COMMAND_UI(ID_BUTFECHar, OnUpdateButFechar)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
  ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_CAPS,
  ID_INDICATOR_NUM,
  ID_INDICATOR_SCRL,
};

static TCHAR BASED_CODE szSection[] = _T("SettingsChild");
static TCHAR BASED_CODE szWindowPos[256];
static TCHAR szFormat[] = _T("%u,%u,%d,%d,%d,%d,%d,%d,%d,%d");

static BOOL PASCAL NEAR ReadWindowPlacement(LPWINDOWPLACEMENT pwp,int NumTar)
{
  if (NumTar == IDD_DIAINICIAL)
  {
    ((CMainFrame*)AfxGetMainWnd())->LeEstadoDiaInicial(pwp, NumTar);
    return TRUE;
  }

  std::stringstream Secao;

  Secao << "WindowPosChild_" << NumTar << ((CMainFrame*)AfxGetMainWnd())->PegaRuaAtual() << std::ends;

  char Buffer[512] = {0};

  ::GetPrivateProfileString(Secao.str().c_str(),
                            szWindowPos,
                            "",
                            Buffer,
                            511,
                            ((((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()) + std::string(".ini")).c_str());

  if (strlen(Buffer) == 0) return FALSE;

  WINDOWPLACEMENT wp;
  int nRead = _stscanf(Buffer, szFormat,
                       &wp.flags, &wp.showCmd,
                       &wp.ptMinPosition.x, &wp.ptMinPosition.y,
                       &wp.ptMaxPosition.x, &wp.ptMaxPosition.y,
                       &wp.rcNormalPosition.left, &wp.rcNormalPosition.top,
                       &wp.rcNormalPosition.right, &wp.rcNormalPosition.bottom);

  if (nRead != 10)return FALSE;

  wp.length = sizeof wp;
  *pwp = wp;

  return TRUE;
}

static void PASCAL NEAR WriteWindowPlacement(LPWINDOWPLACEMENT pwp,int NumTar)
{
  if(NumTar == IDD_DIAINICIAL)  
  {
    ((CMainFrame*)AfxGetMainWnd())->SalvaEstadoDiaInicial(pwp, NumTar);
    return;
  }

  std::stringstream Secao;

  Secao << "WindowPosChild_" << NumTar << ((CMainFrame*)AfxGetMainWnd())->PegaRuaAtual() << std::ends;

  TCHAR szBuffer[sizeof("-32767")*8 + sizeof("65535")*2];

  std::string NomeProfile((((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()));
  NomeProfile.append(".ini");
  
  wsprintf(szBuffer, szFormat,
    pwp->flags, pwp->showCmd,
    pwp->ptMinPosition.x, pwp->ptMinPosition.y,
    pwp->ptMaxPosition.x, pwp->ptMaxPosition.y,
    pwp->rcNormalPosition.left, pwp->rcNormalPosition.top,
    pwp->rcNormalPosition.right, pwp->rcNormalPosition.bottom);
  WritePrivateProfileString(Secao.str().c_str(), szWindowPos, szBuffer,NomeProfile.c_str());
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame() : Deslizantes(NULL,NULL,SECOES),DeslizantesGre(NULL,NULL,GREIDE),DeslizanteGeom(NULL,this),DeslizantesGeol(NULL,NULL,GEOLOGICO),NumTar(-1)
{
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  CMainFrame *MainFrame((CMainFrame*)AfxGetMainWnd());

  if ((MainFrame->asdfg < IDD_INICIO_RELATORIOS || MainFrame->asdfg > IDD_FIM_RELATORIOS) &&  MainFrame->asdfg != 999 && MainFrame->asdfg != IDD_DIAINICIAL)
  {
    if (MainFrame->asdfg != IDD_TAREFACROQUISGEO && MainFrame->asdfg != IDD_DESSECOES && MainFrame->asdfg != IDD_DESENHARPERFIL && MainFrame->asdfg != IDD_DESPONTOSGEO && MainFrame->asdfg != IDD_DESPERFILGEOLOGICO && MainFrame->asdfg != IDD_DESPERFISAVULSOS)
    {
      CString Nomeproj,Extensao;
      Nomeproj = MainFrame->m_sNomeComp;

      Extensao = Nomeproj.Right(Nomeproj.GetLength() - Nomeproj.ReverseFind('.')-1);

      for (NumTar = 0 ; NumTar <= QUANTIDADE_TAREFAS && Extensao.CompareNoCase(MainFrame->Ext[NumTar]) ; NumTar++);

      ASSERT(NumTar <= QUANTIDADE_TAREFAS);
    }
    else  
    {
      cs.style |= CS_OWNDC;           //--- Desenhos têm o DC Proprietário 
      cs.style &= ~WS_MAXIMIZEBOX;    //--- Desenhos não maximizam 

      switch (MainFrame->asdfg)
      { 
	      case IDD_DESPERFISAVULSOS:  NumTar = ID_TAREFADESPERFISAVULSOS; break;
        case IDD_DESENHARPERFIL : NumTar = ID_TAREFAGREIDE; break;
        case IDD_DESSECOES : 
	      case IDD_TAREFACROQUISGEO:

		    NumTar = MainFrame->asdfg == IDD_TAREFACROQUISGEO ? ID_TAREFACROQUISGEO : ID_TAREFASECOES;

        //--- Se for seção e o greide estiver ativo tina o menu de sistema
        //--- Se for seção e o greide não estiver ativo tira o minimize box.

        if (MainFrame->GreideAtivo) 
        {
          cs.style &= ~WS_SYSMENU;   
          cs.style &= ~WS_POPUP;   
          cs.style |= WS_CHILD;
          cs.dwExStyle |= WS_EX_TOPMOST;  // NAO FUNCIONA!!!!!
        }
        else cs.style &= ~WS_MINIMIZEBOX;

        break;

        case IDD_DESPERFILGEOLOGICO : NumTar = ID_TAREFAPERFILGEOLOGICO; break;
        case IDD_DESPONTOSGEO : NumTar = ID_TAREFAPONTOSGEO; break;
	      case IDD_CADASTROACITOPOG: NumTar = ID_TAREFAACITOPOG; break;
      }
    }
  }
  else 
  {
    NumTar = MainFrame->asdfg;
  }

  return CMDIChildWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
  CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
  CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers
BOOL CChildFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CMDIFrameWnd* pParentWnd, CCreateContext* pContext) 
{
  CMainFrame *MainFrame((CMainFrame*)AfxGetMainWnd());

  if (MainFrame->DialogoInicialView(1) != nullptr) MainFrame->DialogoInicialView(0,nullptr);

  switch (MainFrame->asdfg)
  {
    case IDD_DIAINICIAL : pContext->m_pNewViewClass = RUNTIME_CLASS(CDialogoInicialView); break;
    case IDD_IGUALDADES : pContext->m_pNewViewClass = RUNTIME_CLASS(CTypedPtrListView); break;
    case IDD_CURVASVERTICAIS : pContext->m_pNewViewClass = RUNTIME_CLASS(CCurVerView); break;
    case IDD_CURVASHORIZONTAIS : pContext->m_pNewViewClass = RUNTIME_CLASS(CCurHorView); break;
    case IDD_ALASARPAS : pContext->m_pNewViewClass = RUNTIME_CLASS(CAlaSarPasView); break;
    case IDD_CANTCEN : 
    case IDD_TRESPONTOS : pContext->m_pNewViewClass = RUNTIME_CLASS(CTresPontosView); break;
    case IDD_GRADIENTES : pContext->m_pNewViewClass = RUNTIME_CLASS(CGradienteView); break;
    case IDD_TALUDESPAVIM : pContext->m_pNewViewClass = RUNTIME_CLASS(CTaludesPavimView); break;
    case IDD_DESSECOES : pContext->m_pNewViewClass = RUNTIME_CLASS(DesSecaoView);break;
    case IDD_TALUDES :  pContext->m_pNewViewClass = RUNTIME_CLASS(CTaludesView); break;
    case IDD_DESENHARPERFIL : pContext->m_pNewViewClass = RUNTIME_CLASS(DesPerfilView); break;
    case IDD_DESPONTOSGEO : pContext->m_pNewViewClass = RUNTIME_CLASS(DesPontosGeomView); break;
    case IDD_DESPERFILGEOLOGICO :  pContext->m_pNewViewClass = RUNTIME_CLASS(DesPerfilGeoView); break;
    case IDD_TERRTRIANGULADO : pContext->m_pNewViewClass = RUNTIME_CLASS(CSecTRiangView); break;
    case IDD_OBRASARTEESPECIAIS : pContext->m_pNewViewClass = RUNTIME_CLASS(COAEView) ; break;    
    case IDD_OBRASARTECORRENTES :  pContext->m_pNewViewClass = RUNTIME_CLASS(COACView) ; break;
    case IDD_SECOESAVULSAS : pContext->m_pNewViewClass = RUNTIME_CLASS(CSecoesAvulsasView) ; break;
    case IDD_LIMPEZA:  pContext->m_pNewViewClass = RUNTIME_CLASS(CLimpezaView) ; break;
    case IDD_TIPOSSOLOS:  pContext->m_pNewViewClass = RUNTIME_CLASS(CTiposSolosView) ; break;
    case IDD_FUROSSONDAGEM: pContext->m_pNewViewClass = RUNTIME_CLASS(CFurosSondagemView) ; break;
    case IDD_CADASTROACITOPOG: pContext->m_pNewViewClass = RUNTIME_CLASS(CEdiAciTopogView) ; break;
    case IDD_DESPERFISAVULSOS: pContext->m_pNewViewClass = RUNTIME_CLASS(DesPerfilAvulsoView); break;
    case IDD_TAREFACROQUISGEO: pContext->m_pNewViewClass = RUNTIME_CLASS(DesCroquisView); break;
    case IDD_RELACADACITOPOG: pContext->m_pNewViewClass = RUNTIME_CLASS(CRelACADAciTopogView); break;
    case IDD_REL_TERRENO_PRIMITIVO:
    case IDD_REL_NOTA_SERVICO_PAVIM:
    case IDD_REL_PROJ_GREIDE :
    case IDD_REL_PROJ_GEOMETRICO_DETALHADO :
    case IDD_REL_PROJ_GEOMETRICO : 
    case IDD_REL_VOLUMES_TERRAPLANAGEM :
    case IDD_REL_SECOES_AVULSAS :
    case IDD_REL_AREA_TALUDES :
    case IDD_REL_AREA_LIMPEZA :
    case IDD_REL_LOG_CALCSUPER:
    case IDD_REL_LOG_CALCTERRAP:
    case IDD_REL_LOG_FRONTEIRA:
    case IDD_REL_VOLUMES_TIPO_SOLO:
    case IDD_REL_NOTA_SERVICO_TERRAPLANAGEM :
    case IDD_REL_LOG_SECCIONAR:
    case IDD_REL_PROJ_TERRAPLANAGEM: 
    case IDD_REL_EQUIP_DRENAGEM:
    case IDD_REL_BACIAS_HIDROGRAF: pContext->m_pNewViewClass = RUNTIME_CLASS(CRelatorios); break; 
    default : ASSERT(0);break;
  }

  //--- Cria o frame filho.

  if (MainFrame->asdfg == IDD_DESSECOES || MainFrame->asdfg == IDD_TAREFACROQUISGEO) dwStyle |= WS_EX_TOPMOST;

  if (!CMDIChildWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, pContext)) return -1;

  int Tarefa(NumTar);

  if (NumTar==ID_TAREFASECOES  && MainFrame->DesPontosGeo != NULL) Tarefa+=120;
  else if (NumTar==ID_TAREFASECOES  && MainFrame->GreideAtivo) Tarefa+=220;

  WINDOWPLACEMENT wp;

  if (MainFrame->asdfg == IDD_DIAINICIAL)
  {
    //ReadWindowPlacement da erro em alguns casos, inicializa com lixo e não mostra a janela
    //if (::ReadWindowPlacement(&wp, Tarefa)) SetWindowPlacement(&wp);
   ShowWindow(SW_MAXIMIZE);
  }
  else
  {
    if (ReadWindowPlacement(&wp,Tarefa)) SetWindowPlacement(&wp);
  }

  return TRUE;
}

int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)	 return -1;

  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());

  if(MainFrame->asdfg == IDD_DIAINICIAL) return 0;

  if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT)))
  {
    TRACE0("Failed to create status bar\n");
    return -1;      // fail to create
  }

  // DesligaStatusBar();

  if (MainFrame->asdfg != 999 && (MainFrame->asdfg < IDD_INICIO_RELATORIOS || MainFrame->asdfg > IDD_FIM_RELATORIOS))
  {
    if (MainFrame->asdfg != IDD_TAREFACROQUISGEO && MainFrame->asdfg != IDD_DESSECOES && MainFrame->asdfg != IDD_DESENHARPERFIL && MainFrame->asdfg != IDD_DESPONTOSGEO && MainFrame->asdfg != IDD_DESPERFILGEOLOGICO && MainFrame->asdfg != IDD_DESPERFISAVULSOS)  //--- Desenhos não têm subtitulo!
    {
      //--- Cria a barra de  subtitulo. 

      if (!CriaToolBar(Subtit, IDR_SUBTITULO)) return -1;

      //-- Prepara para receber o dialogo.

      Subtit.SetButtonInfo(0, MainFrame->IDDSubtit[NumTar], TBBS_SEPARATOR, 30);

      //--- Cria o dialogo na barra.(Subtítulo)

      Subtitulo.Create(MainFrame->IDDSubtit[NumTar], &Subtit);
    }
    else     //--- Senao é desenho.
    {
      //  DesligaStatusBar();

      if (MainFrame->asdfg == IDD_DESSECOES)
      {
        //--- Coloca o toolbar para os desenhos das seções.

        if (!MainFrame->GreideAtivo)
        {
          if (!CriaToolBar(m_wndToolBarDes, IDR_DESSECAO) || !CriaToolBar(m_wndToolBarDes2, IDR_DESSECAO2) ||
              !CriaToolBar(m_wndToolBarDes3, IDR_DESSECAO3) || !CriaToolBar(m_wndToolBarDes4, IDR_DESSECAO4) ||
              !CriaToolBar(m_wndToolBarTalvegues, IDR_TOOPERFTALVEGUES))
          {
            TRACE0("Failed to create toolbar\n");
            return -1;      // fail to create
          }

          //--- Titulos das janelas

          m_wndToolBarDes.SetWindowText(_T("Coordenadas"));
          m_wndToolBarDes2.SetWindowText(_T("Escalas"));
          m_wndToolBarDes3.SetWindowText(_T("Procurar"));
          m_wndToolBarDes4.SetWindowText(_T("Ferramentas"));

          //--- Cria o dialogo das coordenadas do mouse

          MouseXY.Create(IDD_DESSECOESTOOL, &m_wndToolBarDes);
          MouseXY.ShowWindow(TRUE);
          MainFrame->MouseXY = &MouseXY;  //--- Salva o ponteiro para uso posterior.

          //--- Prepara para receber o dialogo das coordenadas do mouse

          CRect Jan;
          MouseXY.GetWindowRect(&Jan);
          MouseXY.ScreenToClient(&Jan);

          //--- Toolbar 0 janela do mouse

          m_wndToolBarDes.SetButtonInfo(0, IDD_DESSECOESTOOL, TBBS_SEPARATOR, Jan.Width());

          //--- Toolbar 1 - Deslizantes 

          CRect JanDeslizante;
          Deslizantes.Create(IDD_DESLIZANTES, &m_wndToolBarDes2);
          Deslizantes.GetWindowRect(&JanDeslizante);
          Deslizantes.ScreenToClient(&JanDeslizante);
          m_wndToolBarDes2.SetButtonInfo(0, IDD_DESLIZANTES, TBBS_SEPARATOR, JanDeslizante.Width() * .9);
          Deslizantes.ShowWindow(SW_SHOW);

          //--- Toolbar 3 - Drop box do find.

          m_wndToolBarDes3.SetButtonInfo(0, ID_SEPARATOR, TBBS_SEPARATOR, 6);
          m_wndToolBarDes3.SetButtonInfo(1, ID_FINDCOMBO, TBBS_SEPARATOR, 120);

          CRect rect;
          m_wndToolBarDes3.GetItemRect(1, &rect);
          rect.bottom = 270;
          rect.right = 150;

          //--- Cria a caixa.

          if (!m_SearchBox.Create(CBS_DROPDOWNLIST | WS_VISIBLE | WS_TABSTOP | CBS_AUTOHSCROLL | WS_VSCROLL, rect, &m_wndToolBarDes3, IDS_FINDCOMBODES))
          {
            TRACE0("Falha ao criar a caixa de lista 3\n");
            return FALSE;
          }

          //--- Configura a fonte da caixa.	

          HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
          if (hFont == NULL) hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);
          m_SearchBox.SendMessage(WM_SETFONT, (WPARAM)hFont);

          // TODO: Delete these three lines if you don't want the toolbar to
          //  be dockable

          EnableDocking(CBRS_ALIGN_ANY);

          m_wndToolBarDes.EnableDocking(CBRS_ALIGN_ANY);
          DockControlBar(&m_wndToolBarDes);

          m_wndToolBarDes2.EnableDocking(CBRS_ALIGN_ANY);
          DockControlBar(&m_wndToolBarDes2);

          m_wndToolBarDes3.EnableDocking(CBRS_ALIGN_ANY);
          DockControlBar(&m_wndToolBarDes3);

          m_wndToolBarDes4.EnableDocking(CBRS_ALIGN_ANY);
          DockControlBar(&m_wndToolBarDes4);

          // EnableDocking(0);
        }
        else
        {
          //--- O greide esta ativo.          

          if (!CriaToolBar(m_wndToolBarDes, IDR_DESSECAO) || !CriaToolBar(m_wndToolBarDes2, IDR_DESSECAO2) ||
              !CriaToolBar(m_wndToolBarDes3, IDR_DESSECAO3) || !CriaToolBar(m_wndToolBarDes4, IDR_DESSECAO4) || 
              !CriaToolBar(m_wndToolBarTalvegues, IDR_TOOPERFTALVEGUES))
          {
            TRACE0("Failed to create toolbar\n");
            return -1;      // fail to create
          }

          m_wndToolBarDes.SetWindowText(_T("Coordenadas"));
          m_wndToolBarDes2.SetWindowText(_T("Escalas"));
          m_wndToolBarDes3.SetWindowText(_T("Procurar"));
          m_wndToolBarDes4.SetWindowText(_T("Ferramentas"));

          //--- Cria o dialogo das coordenadas do mouse

          MouseXY.Create(IDD_DESSECOESTOOL, &m_wndToolBarDes);
          MouseXY.ShowWindow(TRUE);
          MainFrame->MouseXY = &MouseXY;  //--- Salva o ponteiro para uso posterior.

          //--- Prepara para receber o dialogo das coordenadas do mouse

          CRect Jan;
          MouseXY.GetWindowRect(&Jan);
          MouseXY.ScreenToClient(&Jan);

          //--- Toolbar 0 janela do mouse

          m_wndToolBarDes.SetButtonInfo(0, IDD_DESSECOESTOOL, TBBS_SEPARATOR, Jan.Width());

          //--- Toolbar 1 - Deslizantes 

          CRect JanDeslizante;
          Deslizantes.Create(IDD_DESLIZANTES, &m_wndToolBarDes2);
          Deslizantes.GetWindowRect(&JanDeslizante);
          Deslizantes.ScreenToClient(&JanDeslizante);
          m_wndToolBarDes2.SetButtonInfo(0, IDD_DESLIZANTES, TBBS_SEPARATOR, JanDeslizante.Width());
          Deslizantes.ShowWindow(SW_SHOW);

          //--- Toolbar 3 - Drop box do find.

          m_wndToolBarDes3.SetButtonInfo(0, ID_SEPARATOR, TBBS_SEPARATOR, 6);
          m_wndToolBarDes3.SetButtonInfo(1, ID_FINDCOMBO, TBBS_SEPARATOR, 100);

          CRect rect;
          m_wndToolBarDes3.GetItemRect(1, &rect);
          rect.top = 3;
          rect.bottom = rect.top + 200;

          //--- Cria a caixa.

          if (!m_SearchBox.Create(CBS_DROPDOWNLIST | WS_VISIBLE | WS_TABSTOP | CBS_AUTOHSCROLL | WS_VSCROLL, rect, &m_wndToolBarDes3, IDS_FINDCOMBODES))
          {
            TRACE0("Falha ao criar a caixa de lista 3\n");
            return FALSE;
          }

          //--- Configura a fonte da caixa.	

          HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
          if (hFont == NULL) hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);
          m_SearchBox.SendMessage(WM_SETFONT, (WPARAM)hFont);

          EnableDocking(CBRS_ALIGN_ANY);

          m_wndToolBarDes.EnableDocking(CBRS_ALIGN_ANY);
          DockControlBar(&m_wndToolBarDes, AFX_IDW_DOCKBAR_TOP);

          m_wndToolBarDes2.EnableDocking(CBRS_ALIGN_ANY);
          DockControlBar(&m_wndToolBarDes2, AFX_IDW_DOCKBAR_BOTTOM);

          m_wndToolBarDes3.EnableDocking(CBRS_ALIGN_ANY);
          DockControlBar(&m_wndToolBarDes3, AFX_IDW_DOCKBAR_BOTTOM);

          m_wndToolBarDes4.EnableDocking(CBRS_ALIGN_ANY);
          DockControlBar(&m_wndToolBarDes4);
        }
        
        //--- Cria o toolbar para os perfís de  talvegues

        m_wndToolBarTalvegues.SetWindowText(_T("Talvegues"));
        m_wndToolBarTalvegues.SetButtonInfo(0, ID_BUTPARTICIONAR, TBBS_CHECKBOX, 0);
        m_wndToolBarTalvegues.EnableDocking(CBRS_ALIGN_ANY);
        DockControlBar(&m_wndToolBarTalvegues);

        //EnableDocking(CBRS_ALIGN_ANY);

        m_wndToolBarDes.SetButtonInfo(9, ID_TRANCARDESSECOES, TBBS_CHECKBOX, 9);
        m_wndToolBarDes.SetButtonInfo(10, ID_BUTPROPAGARALTERACOES, TBBS_CHECKBOX, 10);
        m_wndToolBarDes.SetButtonInfo(11, ID_BUTACITOPOG, TBBS_CHECKBOX, 11);
        m_wndToolBarDes.SetButtonInfo(12, ID_BUTSECAVULSASECOES, TBBS_CHECKBOX, 12);
        m_wndToolBarDes.SetButtonInfo(13, ID_BUTTALVEGUES, TBBS_CHECKBOX, 13);
        m_wndToolBarDes.SetButtonInfo(14, ID_BUTHIPSOGRAMA, TBBS_CHECKBOX, 14);

        //--- Salva os ponteiros para os tools bars para poder mostrar/esconder 
        //--- através do botão configurar

        MainFrame->mpCDesTool1 = &m_wndToolBarDes;
        MainFrame->mpCDesTool2 = &m_wndToolBarDes2;
        MainFrame->mpCDesTool3 = &m_wndToolBarDes3;
        MainFrame->mpCDesTool4 = &m_wndToolBarDes4;
        MainFrame->mpCToolBarTalvegues = &m_wndToolBarTalvegues;
      }
      else
      {
        if (MainFrame->asdfg < IDD_INICIO_RELATORIOS || MainFrame->asdfg > IDD_INICIO_RELATORIOS)
        {
          //--- É desenho do greide ou dos pontos ou do perfil geológico
          //--- Coloca o toolbar para os desenhos do greide.
          //--- O toolBar do perfil é igual ao tool bar do Desenho dos pontos

          if (MainFrame->asdfg == IDD_DESPERFILGEOLOGICO)
          {
            if (!CriaToolBar(m_wndToolBarPerfGeo, IDR_DESPERFGEOLOGICO))
            {
              TRACE0("Failed to create toolbar\n");
              return -1;
            }

            m_wndToolBarPerfGeo.SetParent(this);

            m_wndToolBarPerfGeo.SetWindowText(_T("Geológico"));

            MouseXYGre.Create(IDD_DESGREIDETOOL, &m_wndToolBarPerfGeo);
            MouseXYGre.ShowWindow(TRUE);
            MainFrame->MouseXYGre = &MouseXYGre;  //--- Salva o ponteiro para uso posterior.
            CRect JanMouse;
            MouseXYGre.GetWindowRect(&JanMouse);
            MouseXYGre.ScreenToClient(&JanMouse);

            m_wndToolBarPerfGeo.SetButtonInfo(11, ID_BUTLOCK, TBBS_CHECKBOX, 10);
            m_wndToolBarPerfGeo.SetButtonInfo(12, ID_BUTARRASTAR, TBBS_CHECKBOX, 11);
            m_wndToolBarPerfGeo.SetButtonInfo(13, ID_BUTMOVSOY, TBBS_CHECKBOX, 12);

            //--- Prepara para receber o dialogo das coordenadas do mouse

            m_wndToolBarPerfGeo.SetButtonInfo(4, IDD_DESSECOESTOOL, TBBS_SEPARATOR, JanMouse.Width() - 75);

            CRect JanDeslizante;
            DeslizantesGeol.Create(IDD_DESLIZANTES, &m_wndToolBarPerfGeo);
            DeslizantesGeol.GetWindowRect(&JanDeslizante);
            DeslizantesGeol.ScreenToClient(&JanDeslizante);
            m_wndToolBarPerfGeo.SetButtonInfo(2, IDD_DESLIZANTES, TBBS_SEPARATOR, JanDeslizante.Width());
            DeslizantesGeol.ShowWindow(SW_SHOW);

            //--- Cor do perfil atual

            CRect JanCorPerfil;
            m_wndToolBarPerfGeo.GetItemRect(14, &JanCorPerfil);
            JanCorPerfil.DeflateRect(7, 7, 12, 7);
            EdCorPerfAtual.Create("    ", WS_CHILD | WS_VISIBLE, JanCorPerfil, &m_wndToolBarPerfGeo, ID_BUTCORPERFATUAL);
            EdCorPerfAtual.ShowWindow(SW_SHOW);

            m_wndToolBarPerfGeo.EnableDocking(CBRS_ALIGN_ANY);
            EnableDocking(CBRS_ALIGN_ANY);
            DockControlBar(&m_wndToolBarPerfGeo);

            MainFrame->mpToolBarPerfGeol = &m_wndToolBarPerfGeo;
          }
          else
          {
            if (MainFrame->asdfg == IDD_TAREFACROQUISGEO)
            {
              ShowControlBar(&m_wndStatusBar, false, false);
              return 0;
            }

            if (!CriaToolBar(m_wndToolBarGre, IDR_DESGREIDE))   //--- Comum aos 2.
            {
              TRACE0("Failed to create toolbar\n");
              return -1;      // fail to create
            }

            CRect JanMouse;

            if (MainFrame->asdfg == IDD_DESENHARPERFIL || MainFrame->asdfg == IDD_DESPERFISAVULSOS)
            {
              m_wndToolBarGre.SetWindowText(_T("Greide"));

              MouseXYGre.Create(IDD_DESGREIDETOOL, &m_wndToolBarGre);
              MouseXYGre.ShowWindow(TRUE);
              MainFrame->MouseXYGre = &MouseXYGre;  //--- Salva o ponteiro para uso posterior.
              MouseXYGre.GetWindowRect(&JanMouse);
              MouseXYGre.ScreenToClient(&JanMouse);
            }
            else
            {
              MainFrame->GeometricoAtivo = true;
              m_wndToolBarGre.SetWindowText(_T("Geométrico"));

              //--- Janela das coordenadas do ponto (Geometrico)

              MouseXYGeo.Create(IDD_DESGREIDETOOL, &m_wndToolBarGre);
              MouseXYGeo.ShowWindow(TRUE);
              MouseXYGeo.Geometrico = true;
              MainFrame->MouseXYGeo = &MouseXYGeo;  //--- Salva o ponteiro para uso posterior.
              MouseXYGeo.GetWindowRect(&JanMouse);
              MouseXYGeo.ScreenToClient(&JanMouse);
              MouseXYGeo.MoveWindow(180, 1, 180, 20, false);
            }

            m_wndToolBarGre.SetButtonInfo(7, ID_SECSEMPREVIS_ICONIZAR, TBBS_CHECKBOX, 6);
            m_wndToolBarGre.SetButtonInfo(13, ID_BUTSINCRO, TBBS_CHECKBOX, 11);
            m_wndToolBarGre.SetButtonInfo(14, ID_BUTLOCK, TBBS_CHECKBOX, 12);
            m_wndToolBarGre.SetButtonInfo(16, ID_BUTARRASTAR, TBBS_CHECKBOX, 14);
            m_wndToolBarGre.SetButtonInfo(17, ID_BUTDESENHARPERFGEOLOGICOS, TBBS_CHECKBOX, 15);

            //--- Prepara para receber o dialogo das coordenadas do mouse

            m_wndToolBarGre.SetButtonInfo(4, IDD_DESSECOESTOOL, TBBS_SEPARATOR, JanMouse.Width() - 75);

            //--- Deslizantes (Greide ou geometrico ou geologico)

            if (MainFrame->asdfg == IDD_DESENHARPERFIL || MainFrame->asdfg == IDD_DESPERFISAVULSOS)
            {
              CRect JanDeslizante;
              DeslizantesGre.Create(IDD_DESLIZANTES, &m_wndToolBarGre);
              DeslizantesGre.GetWindowRect(&JanDeslizante);
              DeslizantesGre.ScreenToClient(&JanDeslizante);
              m_wndToolBarGre.SetButtonInfo(2, IDD_DESLIZANTES, TBBS_SEPARATOR, JanDeslizante.Width());

              DeslizantesGre.ShowWindow(SW_SHOW);
            }
            else
            {
              if (MainFrame->asdfg == IDD_DESPONTOSGEO)
              {
                CRect JanDeslizante;
                DeslizanteGeom.Create(IDD_DESLIZANTESGEOM, &m_wndToolBarGre);
                DeslizanteGeom.GetWindowRect(&JanDeslizante);
                DeslizanteGeom.ScreenToClient(&JanDeslizante);
                m_wndToolBarGre.SetButtonInfo(2, IDD_DESLIZANTESGEOM, TBBS_SEPARATOR, JanDeslizante.Width());

                DeslizanteGeom.ShowWindow(SW_SHOW);
              }
            }

            if (MainFrame->asdfg == IDD_DESPONTOSGEO)    //--- Se for desenho geometrico o dialogo do ponto é diferente...
            {
              if (!CriaToolBar(m_wndToolBarPonGeo, IDR_DESPONTOSGEO))   //--- Tool bar do dialogo das coordenadas
              {
                TRACE0("Failed to create toolbar\n");
                return -1;      // fail to create
              }

              m_wndToolBarPonGeo.SetWindowText(_T("Superfície"));
              m_wndToolBarPonGeo.SetButtonInfo(0, ID_COTARCN, TBBS_CHECKBOX, 0);
              m_wndToolBarPonGeo.SetButtonInfo(2, ID_BUTCNTOPOGRAF, TBBS_CHECKBOX, 2);
              m_wndToolBarPonGeo.SetButtonInfo(4, ID_DESENHAR, TBBS_CHECKBOX, 4);
              m_wndToolBarPonGeo.SetButtonInfo(5, ID_BUTPONINS, TBBS_CHECKBOX, 5);
              m_wndToolBarPonGeo.SetButtonInfo(7, ID_BUTRADAR, TBBS_CHECKBOX, 7);
              m_wndToolBarPonGeo.SetButtonInfo(8, ID_BUTPROJTERRAP, TBBS_CHECKBOX, 8);
              m_wndToolBarPonGeo.SetButtonInfo(13, ID_BUTMEDIR, TBBS_CHECKBOX, 13);
              m_wndToolBarPonGeo.SetButtonInfo(14, ID_ESBOCAR, TBBS_CHECKBOX, 14);
              m_wndToolBarPonGeo.SetButtonInfo(15, ID_DRENAGEM, TBBS_CHECKBOX, 15);
              m_wndToolBarPonGeo.SetButtonInfo(15, ID_DRENAGEM, TBBS_CHECKBOX, 15);

              m_wndToolBarPonGeo.EnableDocking(CBRS_ALIGN_ANY);
              EnableDocking(CBRS_ALIGN_ANY);
              DockControlBar(&m_wndToolBarPonGeo);
              MainFrame->mpToolBarPonGeo = &m_wndToolBarPonGeo;

              if (!CriaToolBar(m_wndToolBarAciTopograficos, IDR_TOOLDESENHAR2))   //--- Tool box do desenho no geometrico
              {
                TRACE0("Failed to create toolbar\n");
                return -1;      // fail to create
              }
              m_wndToolBarAciTopograficos.SetWindowText(_T("Acidentes Topográficos"));

              m_wndToolBarAciTopograficos.SetButtonInfo(0, ID_DES_MURO, TBBS_CHECKGROUP, 0);
              m_wndToolBarAciTopograficos.SetButtonInfo(1, ID_DES_CERCA, TBBS_CHECKGROUP, 1);
              m_wndToolBarAciTopograficos.SetButtonInfo(2, ID_DES_CASA, TBBS_CHECKGROUP, 2);
              m_wndToolBarAciTopograficos.SetButtonInfo(3, ID_DES_CANTO_MURO, TBBS_CHECKGROUP, 3);
              m_wndToolBarAciTopograficos.SetButtonInfo(4, ID_DES_CANTO_CERCA, TBBS_CHECKGROUP, 4);
              m_wndToolBarAciTopograficos.SetButtonInfo(5, ID_DES_CANTO_CASA, TBBS_CHECKGROUP, 5);
              m_wndToolBarAciTopograficos.SetButtonInfo(6, ID_DES_BORDO, TBBS_CHECKGROUP, 6);
              m_wndToolBarAciTopograficos.SetButtonInfo(7, ID_DES_MEIOFIO, TBBS_CHECKGROUP, 7);
              m_wndToolBarAciTopograficos.SetButtonInfo(8, ID_DES_NA, TBBS_CHECKGROUP, 8);
              m_wndToolBarAciTopograficos.SetButtonInfo(9, ID_DES_NAPER, TBBS_CHECKGROUP, 9);
              m_wndToolBarAciTopograficos.SetButtonInfo(10, ID_DES_ROCHA, TBBS_CHECKGROUP, 10);
              m_wndToolBarAciTopograficos.SetButtonInfo(11, ID_DES_PONTE, TBBS_CHECKGROUP, 11);
              m_wndToolBarAciTopograficos.SetButtonInfo(12, ID_DES_TAMPA_BUEIRO, TBBS_CHECKGROUP, 12);
              m_wndToolBarAciTopograficos.SetButtonInfo(13, ID_DES_POSTE, TBBS_CHECKGROUP, 13);
              m_wndToolBarAciTopograficos.SetButtonInfo(14, ID_DES_REDEELETRICA_BT, TBBS_CHECKGROUP, 14);
              m_wndToolBarAciTopograficos.SetButtonInfo(15, ID_DES_REDEELETRICA_AT, TBBS_CHECKGROUP, 15);
              m_wndToolBarAciTopograficos.SetButtonInfo(16, ID_DES_FRONTEIRA, TBBS_CHECKGROUP, 16);
              m_wndToolBarAciTopograficos.SetButtonInfo(17, ID_DES_EIXO, TBBS_CHECKGROUP, 17);
              m_wndToolBarAciTopograficos.SetButtonInfo(18, ID_DES_CRISTA, TBBS_CHECKGROUP, 18);
              m_wndToolBarAciTopograficos.SetButtonInfo(19, ID_DES_PE, TBBS_CHECKGROUP, 19);
              m_wndToolBarAciTopograficos.SetButtonInfo(20, ID_DES_BUEIRO, TBBS_CHECKGROUP, 20);
              m_wndToolBarAciTopograficos.SetButtonInfo(21, ID_DES_LINHAFERREA, TBBS_CHECKGROUP, 21);
              m_wndToolBarAciTopograficos.SetButtonInfo(22, ID_DES_FLORESTA, TBBS_CHECKGROUP, 22);
              m_wndToolBarAciTopograficos.SetButtonInfo(23, ID_DES_MATA, TBBS_CHECKGROUP, 23);
              m_wndToolBarAciTopograficos.SetButtonInfo(24, ID_DES_PLANTACAO, TBBS_CHECKGROUP, 24);
              m_wndToolBarAciTopograficos.SetButtonInfo(25, ID_DES_ARVORE, TBBS_CHECKGROUP, 25);
              m_wndToolBarAciTopograficos.SetButtonInfo(26, ID_DES_ALAGADO, TBBS_CHECKGROUP, 26);
              m_wndToolBarAciTopograficos.SetButtonInfo(27, ID_DES_ALINHAMENTO, TBBS_CHECKGROUP, 27);
              m_wndToolBarAciTopograficos.SetButtonInfo(28, ID_DES_TALVEGUE, TBBS_CHECKGROUP, 28);
              m_wndToolBarAciTopograficos.SetButtonInfo(29, ID_DES_ESPIGAO, TBBS_CHECKGROUP, 29);
              m_wndToolBarAciTopograficos.SetButtonInfo(30, ID_DES_RAVINA, TBBS_CHECKGROUP, 30);
              m_wndToolBarAciTopograficos.SetButtonInfo(31, ID_DES_PVESGOTO, TBBS_CHECKGROUP, 31);
              m_wndToolBarAciTopograficos.SetButtonInfo(32, ID_DES_PVGAS, TBBS_CHECKGROUP, 32);
              m_wndToolBarAciTopograficos.SetButtonInfo(33, ID_DES_PVELETRICA, TBBS_CHECKGROUP, 33);
              m_wndToolBarAciTopograficos.SetButtonInfo(34, ID_DES_REDEGAS, TBBS_CHECKGROUP, 34);
              m_wndToolBarAciTopograficos.SetButtonInfo(35, ID_DES_REDEELETRICA, TBBS_CHECKGROUP, 35);
              m_wndToolBarAciTopograficos.SetButtonInfo(36, ID_DES_REDEESGOTO, TBBS_CHECKGROUP, 36);
              m_wndToolBarAciTopograficos.SetButtonInfo(37, ID_DES_SOLEIRA, TBBS_CHECKGROUP, 37);
              m_wndToolBarAciTopograficos.SetButtonInfo(38, ID_DES_FILTRAR, TBBS_CHECKGROUP, 38);

              m_wndToolBarAciTopograficos.EnableDocking(CBRS_ALIGN_ANY);
              EnableDocking(CBRS_ALIGN_ANY);
              DockControlBar(&m_wndToolBarAciTopograficos);
              MainFrame->mpToolBoxDesenhar = &m_wndToolBarAciTopograficos;
            }

            if (!CriaToolBar(m_wndToolBarEquiDrenagem, IDR_TOOLEQUIDRENAGEM))
            {
              TRACE0("Failed to create toolbar\n");
              return -1;      // fail to create
            }

            m_wndToolBarEquiDrenagem.SetWindowText(_T("Equipamentos Drenagem"));

            m_wndToolBarEquiDrenagem.SetButtonInfo(0,ID_DES_EQUI_BUEIROCIRC,TBBS_CHECKGROUP,0);
            m_wndToolBarEquiDrenagem.SetButtonInfo(1,ID_DES_EQUI_BUEIRORETANG,TBBS_CHECKGROUP,1);
            m_wndToolBarEquiDrenagem.SetButtonInfo(2,ID_DES_EQUI_SARJETATN,TBBS_CHECKGROUP,2);
            m_wndToolBarEquiDrenagem.SetButtonInfo(3,ID_DES_EQUI_SARJETAOFF,TBBS_CHECKGROUP,3);
            m_wndToolBarEquiDrenagem.SetButtonInfo(4,ID_DES_EQUI_DESCIDA_DAGUA,TBBS_CHECKGROUP,4);
            m_wndToolBarEquiDrenagem.SetButtonInfo(5,ID_DES_EQUI_CAIXA_COLETORA,TBBS_CHECKGROUP,5);
            m_wndToolBarEquiDrenagem.SetButtonInfo(6,ID_DES_EQUI_BACIA_DISSIPACAO, TBBS_CHECKGROUP,6);
            m_wndToolBarEquiDrenagem.SetButtonInfo(7,ID_DES_EQUI_SARJETA, TBBS_CHECKGROUP,7);
            m_wndToolBarEquiDrenagem.SetButtonInfo(8,ID_DES_EQUI_TUBULACAO, TBBS_CHECKGROUP,8);
            m_wndToolBarEquiDrenagem.SetButtonInfo(9,ID_DES_EQUI_DRENO_PROFUNDO, TBBS_CHECKGROUP,9);
            m_wndToolBarEquiDrenagem.SetButtonInfo(10, ID_DES_EQUI_CANALETA, TBBS_CHECKGROUP,10);
            m_wndToolBarEquiDrenagem.SetButtonInfo(11, ID_DES_EQUI_CANALETATERRAP, TBBS_CHECKGROUP,11);
            m_wndToolBarEquiDrenagem.SetButtonInfo(12, ID_DES_MOVI_DRENAGEM,TBBS_CHECKGROUP,12);

            m_wndToolBarEquiDrenagem.EnableDocking(CBRS_ALIGN_ANY);
            EnableDocking(CBRS_ALIGN_ANY);
            DockControlBar(&m_wndToolBarEquiDrenagem);
            MainFrame->mpCToolBarEquiDrenagem = &m_wndToolBarEquiDrenagem;

            // TODO: Delete these three lines if you don't want the toolbar to
            //  be dockable

            m_wndToolBarGre.EnableDocking(CBRS_ALIGN_ANY);
            EnableDocking(CBRS_ALIGN_ANY);
            DockControlBar(&m_wndToolBarGre);
            //  if (MainFrame->asdfg == IDD_DESPONTOSGEO) DockControlBar(&m_wndToolBarGre);

            //--- Salva os ponteiros para os tools bars para poder mostrar/esconder 
            //--- através do botão configurar

            if (MainFrame->asdfg == IDD_DESENHARPERFIL || MainFrame->asdfg == IDD_DESPERFISAVULSOS)  MainFrame->mpCDesToolGreide = &m_wndToolBarGre;
            else if (MainFrame->asdfg == IDD_DESPONTOSGEO) MainFrame->mpCDesToolGeo = &m_wndToolBarGre;
            //  else if(MainFrame->asdfg == IDD_DESPERFILGEOLOGICO) MainFrame->mpToolBarPerfGeol = &m_wndToolBarPerfGeo;
          }
        }
      }
    }

    char charNumTar[10];

    if (MainFrame->asdfg == IDD_DESSECOES)
    {
      //--- Neste caso as seçoes são abertas antes do greide porisso o greiativo, o ponteiro para o 
      //--- desenho do greide ainda está null
      if (MainFrame->DesPontosGeo) sprintf(charNumTar, "TBar%i", ID_TAREFASECOES + 120);
      else if (MainFrame->GreideAtivo) sprintf(charNumTar, "TBar%i", ID_TAREFASECOES + 220);
      else  sprintf(charNumTar, "TBar%i", NumTar);
    }
    else
    {
      if (MainFrame->asdfg == IDD_DESENHARPERFIL)
      {
        if (MainFrame->DesPontosGeo) sprintf(charNumTar, "TBar%i", ID_TAREFAGREIDE + 120);
        else sprintf(charNumTar, "TBar%i", NumTar);
      }
      else
      {
        if (MainFrame->asdfg == IDD_DESPERFILGEOLOGICO) sprintf(charNumTar, "TBar%i", ID_TAREFAPERFILGEOLOGICO);
        else
          if (MainFrame->asdfg == IDD_DESPERFISAVULSOS) sprintf(charNumTar, "TBar%i", ID_TAREFADESPERFISAVULSOS);
        sprintf(charNumTar, "TBar%i", NumTar);     //--- senão é desenho geométrico
      }
    }

    std::stringstream Secao;

    Secao << MainFrame->PegaRuaAtual() << "_" << charNumTar << std::ends;

    LoadBarState(Secao.str().c_str());           //--- Carrega o estado dos toolsbars
  }

  return 0;
}

void CChildFrame::ActivateFrame(int nCmdShow) 
{
  //--- Mesmo que a view não seja do desenho não tem importância 
  //--- já que o deslizante só será usado nela.
  //--- Não ví outro lugar para colocar esta atribuição.

  if (nCmdShow) DeslizanteGeom.m_pDesView = DeslizantesGre.m_pDesView = Deslizantes.m_pDesView = DeslizantesGeol.m_pDesView = GetActiveView();

  if(((CMainFrame*)AfxGetMainWnd())->asdfg == IDD_DIAINICIAL) 
  {
   // nCmdShow = SW_SHOWMAXIMIZED; // Maximiza todas as janelas ?????
  }

  CMDIChildWnd::ActivateFrame(nCmdShow);
}

BOOL CChildFrame::CreatePaletteBar(CPaletteBar& Palette,UINT Recurso)
{
  /*(
  if (!Palette.CreateEx(this,TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
  | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED) || !Palette.LoadToolBar(Recurso))
  {
  TRACE0("Failed to create toolbar\n");
  return -1;      // fail to create
  }

  */

  if (!Palette.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
    | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED) ||
    !Palette.LoadToolBar(Recurso)) 
  {
    TRACE0("Failed to create Pallete\n");
    return FALSE;       // fail to create
  }

  Palette.SetWindowText(_T(Palette.Titulo));
  Palette.EnableDocking(0);

  m_wndSecoesPalette.SetColumns(/*Palette.GetColumns()*/2);
  FloatControlBar(&Palette,Palette.Posicao);

  return TRUE;
}

BOOL CChildFrame::CriaToolBar(CToolBar& Barra,UINT Recurso)
{
  //--- Cria o tool bar com os parametros default
  if (!Barra.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP 
    | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),Recurso) ||
    !Barra.LoadToolBar(Recurso)) 
  {
    TRACE0("Failed to create toolbar\n");
    return FALSE;      // fail to create   
  }

  /*
  if (!Barra.Create(this) || !Barra.LoadToolBar(Recurso))
  {
  TRACE0("Failed to create toolbar\n");
  return FALSE;      // fail to create
  }
  */
  return TRUE;
}


void CChildFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)
{
  //-- Thanks to Microsoft MFC programmer´s team!!!

  CRect rect;
  DWORD dw;
  UINT n;

  // get MFC to adjust the dimensions of all docked ToolBars
  // so that GetWindowRect will be accurate
  RecalcLayout();
  LeftOf->GetWindowRect(&rect);
  rect.OffsetRect(1,0);
  dw=LeftOf->GetBarStyle();
  n = 0;
  n = (dw&CBRS_ALIGN_TOP) ? AFX_IDW_DOCKBAR_TOP : n;
  n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
  n = (dw&CBRS_ALIGN_LEFT && n==0) ? AFX_IDW_DOCKBAR_LEFT : n;
  n = (dw&CBRS_ALIGN_RIGHT && n==0) ? AFX_IDW_DOCKBAR_RIGHT : n;

  // When we take the default parameters on rect, DockControlBar will dock
  // each Toolbar on a seperate line.  By calculating a rectangle, we in effect
  // are simulating a Toolbar being dragged to that location and docked.

  DockControlBar(Bar,n,&rect);
}
void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{
  // TODO: Add your message handler code here and/or call default

  CMDIChildWnd *CW(GetMDIFrame()->MDIGetActive());

  CView* pView(CW->GetActiveView());
  CMainFrame* MF((CMainFrame*)AfxGetMainWnd());
  DesSecaoView* DesSecao((DesSecaoView*)(((CMainFrame*)AfxGetMainWnd())->PegaDesenhoSecoes()));

  if (nID == SC_MINIMIZE)
  {
    if (pView == DesSecao)   
    {
      if(MF->DesPerfil) ((DesPerfilView*)(MF->DesPerfil))->MostrarApagarSecao(false);
      if(DesSecao)                                           //--- Pview DesSecao são o mesmo ponteiro mas não funciona se fizer cast no pview.....
      {
        DesSecao->MostrarVolumes(false);

        int EstToolBars(DesSecao->PegaEstadoToolBars());

        MF->ShowControlBar(MF->PegaBarra(1),!(EstToolBars & (1 << 3)), FALSE);	
        MF->ShowControlBar(MF->PegaBarra(2),!(EstToolBars & (1 << 2)), FALSE);	
        MF->ShowControlBar(MF->PegaBarra(3),!(EstToolBars & (1 << 1)), FALSE);	
        MF->ShowControlBar(MF->PegaBarra(4),!(EstToolBars & 1), FALSE);	
      }
    }
  }
  else
  {
    if (nID == SC_RESTORE || nID == SC_MAXIMIZE)
    {
      if(pView)
      {
        //DesSecaoView* DesSecao(DesSecao);

        if(DesSecao)
        {
          DesSecao->MostrarVolumes(true);
     
          int EstToolBars(DesSecao->PegaEstadoToolBars());

          MF->ShowControlBar(MF->PegaBarra(1),(EstToolBars & (1 << 3)), FALSE);	
          MF->ShowControlBar(MF->PegaBarra(2),(EstToolBars & (1 << 2)), FALSE);	
          MF->ShowControlBar(MF->PegaBarra(3),(EstToolBars & (1 << 1)), FALSE);	
          MF->ShowControlBar(MF->PegaBarra(4),(EstToolBars & 1), FALSE);	

          ((DesPerfilView*)((CMainFrame*)AfxGetMainWnd())->DesPerfil)->MostrarApagarSecao(true);
        }
      }
    }
  }

  CMDIChildWnd::OnSysCommand(nID, lParam);
}

void CChildFrame::OnClose() 
{
  //--- Se estiver fechando as secoes com o greide ativo, salva o estado dos toolsbox
  //--- salvar os estados separados bem como as posicoes

  CMainFrame *MainFrame((CMainFrame*) AfxGetMainWnd());

  switch (NumTar)
  {
    case ID_TAREFACROQUISGEO:
    {
      ((DesCroquisView*)MainFrame->pDesCroquisView2)->GetParentFrame()->ShowWindow(SW_HIDE);
      ((DesPontosGeomView*)MainFrame->DesPontosGeo)->AlternaDesenharCroquis();

      return;
    }
    case ID_TAREFAPONTOSGEO:  //--- Desenho é do geometrico mas a janela topmost é a das secoes... 
    {
      //--- como o estdoc foi feito só para os arquivos, salva as alterações aqui.

      DesPontosGeomView* pGeomView((DesPontosGeomView*)MainFrame->DesPontosGeo);

      if (!pGeomView)
      {
        pGeomView = (DesPontosGeomView*)this->GetActiveView();
        MDIActivate();
      }

      pGeomView->Destruindo = true;

      if (pGeomView->PegaAngRotacaoSuperf() != 0.0)
      {
        //  pGeomView->Desrotacionar();
      }

      if (pGeomView->SuperfAlterada()) pGeomView->GravarSuperficie();        //--- Salva as alterações

      WINDOWPLACEMENT wp;
      wp.length = sizeof wp;

      if (MainFrame->PegaDesenhoSecoes() && !MainFrame->DesPerfil)
      {
        CView* pViewSecao = (CView*)((CMainFrame*)AfxGetMainWnd())->PegaDesenhoSecoes();

        //--- Traz o desenho para o topo, para salvar a posicao e os tools bars (A Seguir);

        pViewSecao->GetParentFrame()->ActivateFrame();
        CMDIChildWnd* CW(GetMDIFrame()->MDIGetActive());

        int Tarefa(ID_TAREFASECOES + 120);

        std::stringstream Secao;

        Secao << MainFrame->PegaRuaAtual() << "_TBar" << Tarefa << std::ends;

        CW->SaveBarState(Secao.str().c_str());

        //--- Fecha e salva os dados da janela da seção.
        //--- A janela será destruída no destrutor do desenho do geométrico porque o ponteiro ainda será usado

        if (MainFrame->asdfg == IDD_DIAINICIAL)
        {
          if (CW->GetWindowPlacement(&wp)) WriteWindowPlacement(&wp, Tarefa);
        }
        else
        {
          if (CW->GetWindowPlacement(&wp)) WriteWindowPlacement(&wp, Tarefa);
        }

        MainFrame->DesPontosGeoComSecao = false;
      }

      CWnd* pDesCroquisView((CWnd*)MainFrame->PegaDesenhoCroquis());
      if (pDesCroquisView)
      {
        pDesCroquisView->GetParentFrame()->ActivateFrame();
        CMDIChildWnd* CW = GetMDIFrame()->MDIGetActive();

        if (CW->GetWindowPlacement(&wp))
        {
          WriteWindowPlacement(&wp, ID_TAREFACROQUISGEO);
        }

        pDesCroquisView->GetParentFrame()->DestroyWindow();
      }
    }

    MainFrame->pDesCroquisView2 = NULL;
    break;

    case ID_TAREFAGREIDE:
    {
      //--- como o estdoc foi feito só para os arquivos, salva as alterações aqui.

      DesPerfilView* pPerfilView((DesPerfilView*)MainFrame->DesPerfil);
      DesSecaoView* pViewSecao((DesSecaoView*)(DesSecaoView*)MainFrame->PegaDesenhoSecoes());

      if (pPerfilView)
      {
        MainFrame->SelecionaRua(pPerfilView->PegaRua());

        int AlterouProjeto(pPerfilView->AlterouProjeto());
        if (AlterouProjeto) pPerfilView->GravarProjeto(0, true);            //--- Salva as alterações tipo == 1 -> NÂO calcula os volumes
      }
      else break;

      if (!pViewSecao) break;

      if (pViewSecao->VerfAlterouSecoes()) pViewSecao->GravarSecoes(true);   //--- Grava as secoes com comfirmação

      if (pViewSecao /*&& !MainFrame->DesPontosGeo*/)
      {
        WINDOWPLACEMENT wp;
        wp.length = sizeof wp;

        //--- Traz o desenho para o topo, para salvar a posicao e os tools bars ;

        pViewSecao->GetParentFrame()->ActivateFrame();
        CMDIChildWnd* CW(GetMDIFrame()->MDIGetActive());

        int Tarefa(ID_TAREFASECOES + 220);    //--- + 220 para diferenciar o desenho das seções com geométrico e sem geometrico

        std::stringstream Secao;

        Secao << MainFrame->PegaRuaAtual() << "_TBar" << Tarefa << std::ends;

        CW->SaveBarState(Secao.str().c_str());

        //--- Salva os dados da janela da seção.

        if (CW->GetWindowPlacement(&wp)) WriteWindowPlacement(&wp, Tarefa);

        CW->MDIDestroy();    //--- Destroi a janela das secoes         

        MainFrame->AnulaSecao();
        MainFrame->AnulaPerfil();

        MainFrame->DesPerfil = NULL;
      }
    }
    break;

    case ID_TAREFASECOES:
    {

      DesPerfilView* pPerfilView((DesPerfilView*)MainFrame->DesPerfil);
      if (pPerfilView)
      {
        MainFrame->SelecionaRua(pPerfilView->PegaRua());
        //    pPerfilView->DestroyWindow();
        //pPerfilView->SendMessage(WM_CLOSE);
        // pPerfilView->DestroyWindow();
        // pPerfilView = NULL;
      }

      //--- como o estdoc foi feito só para os arquivos, salva as alterações aqui.

      DesSecaoView* pSecaoView((DesSecaoView*)MainFrame->PegaDesenhoSecoes());

      if (!pSecaoView) break;

      MainFrame->SelecionaRua(pSecaoView->PegaRua());
      if (pSecaoView && pSecaoView->VerfAlterouSecoes()) pSecaoView->GravarSecoes();

      //---Se estiver fechando o desenho da seção com o geometrico aberto salva com o número respectivo

      if (MainFrame->DesPontosGeo) NumTar += 120;

      MainFrame->AnulaSecao();
      //MainFrame->AnulaPerfil();

      pSecaoView = NULL;
      break;
    }
  }

  //--- Salva a posição e o tamanho da janela ativa (ou seja,a topmost atual)

  WINDOWPLACEMENT wp;
  wp.length = sizeof wp;
  std::stringstream Secao;
  
  Secao << MainFrame->PegaRuaAtual() << "_TBar" << NumTar << std::ends;
  
  SaveBarState(Secao.str().c_str());  //--- Salva o estado dos toolsbars

  if (GetWindowPlacement(&wp)) WriteWindowPlacement(&wp,NumTar);

  if(NumTar >= IDD_INICIO_RELATORIOS && NumTar <= IDD_FIM_RELATORIOS) GetActiveDocument()->SetModifiedFlag(false);

  CMDIChildWnd::OnClose();
}

void CChildFrame::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
  CMainFrame *MainFrame((CMainFrame*) AfxGetMainWnd());

  void* pDesSecao(MainFrame->PegaDesenhoSecoes());

  if (pDesSecao && (MainFrame->DesPontosGeo && ((DesPontosGeomView*)MainFrame->DesPontosGeo)->PegaSecoesIconizadas()) ||
     (pDesSecao && (MainFrame->DesPerfil && ((DesPerfilView*)MainFrame->DesPerfil)->PegaSecoesIconizadas())))
  {
    CMDIChildWnd* CW(GetMDIFrame()->MDIGetActive());
    if (!CW) return;
    CView* pView(CW->GetActiveView());

    if (pView == pDesSecao && CW->IsIconic())
    {
      RECT Rect;

      CW->GetWindowRect(&Rect); 

      //--- Verifica se o ponto esta na área não cliente como a janelas esta minimizada
      //--- se o ponto estiver dentro da janela ele estará na área não cliente

      if (point.x >= Rect.left && point.x <= Rect.right && point.y >= Rect.top && point.y <= Rect.bottom)
      {
        DesSecaoView* pViewSecao = (DesSecaoView*) pDesSecao;
        pViewSecao->OnNcLButtonDown2(0,point);
      }
    }
  }

  CMDIChildWnd::OnNcMouseMove(nHitTest, point);
}

void CChildFrame::OnUpdateButFechar(CCmdUI* pCmdUI) 
{
  //--- A barra é comum às 2 janelas

  CMainFrame *MainFrame = (CMainFrame*) AfxGetMainWnd();

  if(pCmdUI->m_pOther == MainFrame->mpCDesToolGreide)
  {
    if(MainFrame->DesPerfil)
      pCmdUI->Enable((((CChildFrame*)((DesPerfilView*)MainFrame->DesPerfil)->GetParentFrame())->GetStyle() & WS_CAPTION) == 0);
  }
  else
  {
    if(MainFrame->DesPontosGeo && pCmdUI->m_pOther == MainFrame->mpCDesToolGeo)
      pCmdUI->Enable((((CChildFrame*)((DesPontosGeomView*)MainFrame->DesPontosGeo)->GetParentFrame())->GetStyle() & WS_CAPTION) == 0);
  }
}

