// MainFrm.cpp : implementation of the CMainFrame class
//
#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include <winnetwk.h>
#include "cconfigproj.h"
#include <vector>
#include <iostream>
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "csstack.h"
#include "xymouse.h"
#include "abrirmedicao.h"
#include <string>
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "cponto.h"
#include "clpontos.h"
#include "perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include <list>
#include "csecao.h"
#include "clistasecoes.h"
#include "criarmedicao.h"
#include "carqcurverticais.h"
#include "ccurverticais.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include <set>
#include <string>
#include "carqcurhorizontais.h"
#include <map>
#include "ccurhorizontal.h"
#include "Mat.h"
#include "ccurhorizontais.h"
#include "cestacasacalcular.h"
#include "carqgradientes.h"
#include "cgradientes.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
#include <sstream>
#include <iomanip>
#include "estdoc.h"
#include "typlstvw.h"
#include "cvisada.h"
#include "carqnivelamento.h"
#include "carqterrpontos.h"
#include "cterrpontos.h"
#include "calculaterreno.h"
#include "calculamedicao.h"
#include "calcnota.h"
#include "Splash.h"
#include "CTipoSolos.h"
#include "CArqTiposSolos.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include "CArqFurosSondagem.h"
#include <algorithm>
#include "CPerfilGeologico.h"
#include "calcvolumes.h"
#include "listamedicoes.h"
#include "dvolumes.h"
#include "meleditbox.h"
#include "cdpopuppontos.h"
#include "carqpistasfixas.h"
#include "cpistasfixas.h"
#include "carqsecoesavulsas.h"
#include "csecaoavulsa.h"
#include "csecoesavulsas.h"
#include "cdeltasuperficie.h"
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CAcidentesTopograficos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "dessecaoview.h"
#include "xygreide.h"
#include "CArqOAC.h"
#include "CArqOAE.h"
#include "CSegmento.h"
#include "CEquipDrenagem.h"
#include "superficie.h"
#include "dsecionar.h"
#include "DCalcGeometrico.h"
#include "diacalcsecavulsas.h"
#include "cdcalareataludes.h"
#include "cdiacalclimpeza.h"
#include "DCalcSuperf.h"
#include <direct.h>
#include "CDiaRemoverRua.h"
#include "CDiaCriarProjeto.h"
#include "COAC.H"
#include "COAE.H"
#include "CMemDC.h"
#include "DesPerfilView.h"
#include "CDCalcTerraplProjeto.h"
#include "ConvUTMGDC\ConvUTMGDC\Gdc_Coord_3d.h"
#include "ConvUTMGDC\ConvUTMGDC\Utm_Coord_3d.h"
#include "CDiaImportarArqKmlKmz.h"
#include "CLembrete.h"
#include "CArqAciTopograficos.h"
#include "CTrecho.h"
#include "CArqTrechos.h"
#include "CTrechos.h"
#include "CRadar.h"
#include "Picture.h"
//#include "ximage.h"
//#include ".\cximage600_full\CxImage\ximage.h"
#include "DesCroquisView.h"
#include "CSecoesTerrap.h"
#include "CCamadaDGN.h"

#include "CTextoMapaIBGE.h"
#include "CCNMapaIGBE.h"
#include "CMapaCNIBGE.h"
#include "CDiaImportarArqCtrlDrones.h"
#include "DesPontosGeomView.h"
#include "Monologo.h"

#ifndef _DEBUG
  #ifndef DEMOVER 
    #include "VerfNeoKey.h"
  #endif
#endif

#ifndef DEMOVER
  #include "CalcNS\CalcNS.h"
  #include "CalcNS\CCalcNS.h"
  #include "CalcVL\CalcVL.h"
  #include "CalcVL\CCalcVL.h"
  #include "CalcSU\CalcSU.h"
  #include "CalcSU\CCCalcSU.h"
#endif

#include "HTTPRequests\HTTPRequests\HTTPRequests.h"
#include "ConvUTMGDC\ConvUTMGDC\CConvUTMGDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EVENT_CHAVE 1001

extern CCSStack VetorProjetos;
extern bool VerificaAberturaTarefa(CString& NomeProj);
extern void LeListasRuas();
extern monologo monolog;

#ifndef _DEBUG
  #ifndef DEMOVER
    extern class VerfNeoKey OVerfNeoKey;
  #endif
#endif

//--- Dados e metodos para ler e escrever a posição e formato das janelas
//--- Thanks to microsoft MFC programer´s team!

static TCHAR BASED_CODE szSection[] = _T("Settings");
static TCHAR BASED_CODE szWindowPos[] = _T("WindowPos");
static TCHAR szFormat[] = _T("%u,%u,%d,%d,%d,%d,%d,%d,%d,%d");

static BOOL PASCAL NEAR ReadWindowPlacement(LPWINDOWPLACEMENT pwp,int DialogoInicial = 0)
// read a window placement from settings section of app's ini file
{
  std::stringstream Secao;

  if(DialogoInicial) Secao << "DialogoInicial";
  else Secao << "Settings"<< std::ends;

  CString strBuffer = AfxGetApp()->GetProfileString(Secao.str().c_str(), szWindowPos);
  if (strBuffer.IsEmpty()) return FALSE;

  WINDOWPLACEMENT wp;
  int nRead = _stscanf(strBuffer, szFormat,
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

// write a window placement to settings section of app's ini file
static void PASCAL NEAR WriteWindowPlacement(LPWINDOWPLACEMENT pwp,int DialogoInicial = 0)
{
   std::stringstream Secao;

  if(DialogoInicial) Secao << "DialogoInicial";
  else Secao << "Settings" << std::ends;

  TCHAR szBuffer[sizeof("-32767")*8 + sizeof("65535")*2];

  wsprintf(szBuffer, szFormat,
    pwp->flags, pwp->showCmd,
    pwp->ptMinPosition.x, pwp->ptMinPosition.y,
    pwp->ptMaxPosition.x, pwp->ptMaxPosition.y,
    pwp->rcNormalPosition.left, pwp->rcNormalPosition.top,
    pwp->rcNormalPosition.right, pwp->rcNormalPosition.bottom);
  AfxGetApp()->WriteProfileString(Secao.str().c_str(), szWindowPos, szBuffer);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
  //{{AFX_MSG_MAP(CMainFrame)
  ON_WM_CREATE()
  ON_COMMAND(ID_PROJ_ABRIR, OnProjAbrir)
  ON_COMMAND(ID_PROJ_CLOSE, OnProjClose)
  ON_COMMAND(ID_PROJ_NOVO, OnProjNovo)
  ON_COMMAND(ID_CONFIG_PROJ, OnConfigProj)
  ON_UPDATE_COMMAND_UI(ID_PROJ_CLOSE, OnUpdateProjClose)
  ON_UPDATE_COMMAND_UI(ID_CONFIG_PROJ, OnUpdateConfigProj)
  ON_UPDATE_COMMAND_UI(ID_ARQ_ABRIR, OnUpdateArqAbrir)
  ON_UPDATE_COMMAND_UI(ID_ARQ_NOVO, OnUpdateArqNovo)
  ON_COMMAND(ID_PROJREC1, OnProjrec1)
  ON_COMMAND(ID_PROJREC2, OnProjrec2)
  ON_COMMAND(ID_PROJREC3, OnProjrec3)
  ON_COMMAND(ID_PROJREC4, OnProjrec4)
  ON_COMMAND(ID_PROJREC5, OnProjrec5)
  ON_COMMAND(ID_PROJREC6, OnProjrec6)
  ON_WM_DESTROY()
  ON_COMMAND(ID_JANELA_CENTRAR, OnJanelaCentrar)
  ON_UPDATE_COMMAND_UI(ID_JANELA_CENTRAR, OnUpdateJanelaCentrar)
  ON_UPDATE_COMMAND_UI(ID_PROX_JANELA, OnUpdateProxJanela)
  ON_COMMAND(ID_PROX_JANELA, OnProxJanela)
  ON_UPDATE_COMMAND_UI(ID_SECCIONAR, OnUpdateCalcterreno)
  ON_COMMAND(ID_CALCTERRENO, OnCalcterreno)
  ON_COMMAND(ID_MEDIO_ABRIR, OnMedioAbrir)
  ON_COMMAND(ID_MEDIO_CALCULARAMEDIO, OnMedioCalcularamedio)
  ON_UPDATE_COMMAND_UI(ID_MEDIO_CALCULARAMEDIO, OnUpdateMedioCalcularamedio)
  ON_UPDATE_COMMAND_UI(ID_MEDIO_ABRIR, OnUpdateMedioAbrir)
  ON_COMMAND(ID_MEDIO_NOVA, OnMedioNova)
  ON_UPDATE_COMMAND_UI(ID_MEDIO_NOVA, OnUpdateMedioNova)
  ON_COMMAND(ID_CLCULOS_CALCULATANOTADESERVIO, OnCalculaNotaDeServico)
  ON_COMMAND(ID_SECCIONAR, OnSeccionar)
  ON_UPDATE_COMMAND_UI(ID_CLCULOS_CALCULATANOTADESERVIO, OnUpdateClculosCalculatanotadeservio)
  ON_COMMAND(ID_CALCULO_VOLUMES, OnCalculoVolumes)
  ON_COMMAND(ID_SUPERFCIE_GERAR, OnSuperfcieGerar)
//  ON_COMMAND(ID_TERRENO_SECCIONAR, OnTerrenoSeccionar)
 // ON_COMMAND(ID_CLCULOS_CALCULAROSDADOSGEOMTRICOS, OnCalcularGeometrico) //--- Desativado até 2a ordem
 // ON_COMMAND(ID_CALC_SECOES_AVULSAS,OnCalcSecoesAvulsas)
  ON_WM_KEYDOWN()
  ON_COMMAND(ID_TERRENO_ACIDENTESTOPOGRFICOS, OnTerrenoAcidentestopogrficos)
  ON_UPDATE_COMMAND_UI(ID_SUPERFCIE_GERAR, OnUpdateSuperfcieGerar)
  ON_UPDATE_COMMAND_UI(ID_TERRENO_SECCIONAR, OnUpdateTerrenoSeccionar)
  ON_UPDATE_COMMAND_UI(ID_CALCULO_VOLUMES, OnUpdateCalculoVolumes)
  ON_UPDATE_COMMAND_UI(ID_CLCULOS_CALCULAROSDADOSGEOMTRICOS, OnUpdateClculosCalcularosdadosgeomtricos)
  ON_UPDATE_COMMAND_UI(ID_DESENHOS_SECOES, OnUpdateDesenhosSecoes)
  ON_UPDATE_COMMAND_UI(ID_EDITAR_PONTOS_GEO, OnUpdateEditarPontosGeo)
  ON_WM_CLOSE()
  ON_UPDATE_COMMAND_UI(ID_BUTFECHar, OnUpdateButFechar)
  ON_UPDATE_COMMAND_UI(ID_DESENHOS_GREIDE, OnUpdateDesenhosGreide)
  ON_UPDATE_COMMAND_UI(ID_DESDXFPROGEOM, OnUpdateDesdxfprogeom)
  ON_UPDATE_COMMAND_UI(ID_DESDXFGREIDE, OnUpdateDesdxfgreide)
  ON_UPDATE_COMMAND_UI(ID_DESDXFSECOES, OnUpdateDesdxfsecoes)
  ON_UPDATE_COMMAND_UI(ID_CALC_AREA_TALUDES, OnUpdateCalcAreaTaludes)
  ON_UPDATE_COMMAND_UI(ID_CALC_AREA_LIMPEZA, OnUpdateCalcLimpeza)
  ON_UPDATE_COMMAND_UI(ID_CALC_AREA_TALUDES, OnUpdateCalcAreaTaludes)
  ON_UPDATE_COMMAND_UI(ID_CALC_AREA_LIMPEZA, OnUpdateCalcLimpeza)
  ON_UPDATE_COMMAND_UI(ID_EDITAR_PERFIL_GEOLOGICO, OnUpdatePerfilGeol)
  ON_UPDATE_COMMAND_UI(ID_DESDXFPROGEOM, OnUpdateDXFProjGeom)
  ON_UPDATE_COMMAND_UI(ID_DESDXFGREIDE, OnUpdateDEXGreide)
  ON_UPDATE_COMMAND_UI(ID_DESDXFSECOES, OnUpdateDXFSecoes)
  ON_UPDATE_COMMAND_UI(ID_DESDXFPERFGEOL, OnUpdateDXFPerfGeo)
  ON_UPDATE_COMMAND_UI(ID_NOTADESERVI_TERRAPLANAGEM, OnUpdateNSTerraplanagem)
  ON_UPDATE_COMMAND_UI(ID_NOTADESERVI_PAVIMENTA, OnUpdateNSPavimentacao)
  ON_UPDATE_COMMAND_UI(ID_RELAT_AREA_TALUDES, OnUpdateAreaTaludes)
  ON_UPDATE_COMMAND_UI(ID_RELAT_VOL_TIPO_SOLO, OnUpdateVolumesTipoSolo)

  //}}AFX_MSG_MAP
  // Global help commands
/*  ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
  ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
  ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
  ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
  */
  ON_COMMAND(ID_HELP_FINDER,  CEstApp::OnHelp)
  ON_COMMAND(ID_HELP,  CEstApp::OnHelp)
  ON_COMMAND(ID_CONTEXT_HELP,  CEstApp::OnHelp)
  ON_COMMAND(ID_DEFAULT_HELP,  CEstApp::OnHelp)
  ON_UPDATE_COMMAND_UI(ID_RELAT_CURVASVERTICAIS, OnUpdateRelatCurvasverticais)
  ON_UPDATE_COMMAND_UI(ID_RELAT_PROJETOGEOM, OnUpdateRelatProjetogeom)
  ON_UPDATE_COMMAND_UI(ID_RELAT_PROJETOGEOM33017, OnUpdateRelatProjetogeom33017)
  ON_UPDATE_COMMAND_UI(ID_RELAT_TERRENOPRIMITIVO, OnUpdateRelatTerrenoprimitivo)
  ON_UPDATE_COMMAND_UI(ID_RELAT_VOLUMES, OnUpdateRelatVolumes)
  ON_UPDATE_COMMAND_UI(ID_NOTADESERVI_PAVIMENTA, OnUpdateNotadeserviPavimenta)
  ON_UPDATE_COMMAND_UI(ID_NOTADESERVI_TERRAPLANAGEM, OnUpdateNotadeserviTerraplanagem)
  ON_COMMAND(ID_C_SECCIONAR, OnCalcSecTranversais)
  ON_UPDATE_COMMAND_UI(ID_C_SECCIONAR, OnUpdateTerrenoSeccionar)
  //ON_COMMAND(ID_CALC_SECOES_TRANSVERSAIS, OnCalcSecTranversais)
  //ON_COMMAND(ID_CALC_SECOES_AVULSAS, OnCalcSecAvulsas)
  ON_COMMAND(ID_CALC_AREA_TALUDES, OnCalcAreaTaludes)
  ON_UPDATE_COMMAND_UI(ID_CALC_AREA_TALUDES, OnUpdateTerrenoSeccionar)
  ON_COMMAND(ID_CALC_AREA_LIMPEZA, OnCalcAreaLimpeza)
//  ON_COMMAND(ID_RELAT_AREA_LIMPEZA, OnRelatAreaLimpeza)
  ON_UPDATE_COMMAND_UI(ID_CALC_AREA_LIMPEZA, OnUpdateTerrenoSeccionar)
  ON_COMMAND(ID_FILE_SAVE, &CMainFrame::OnFileSave)
  ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, &CMainFrame::OnUpdateFileSave)
  ON_WM_TIMER()
  ON_COMMAND(ID_PROJETO_REMOVERRUA, &CMainFrame::OnProjetoRemoverrua)
  ON_UPDATE_COMMAND_UI(ID_PROJETO_INSERIRRUA, &CMainFrame::OnUpdateProjetoInserirrua)
  ON_UPDATE_COMMAND_UI(ID_PROJETO_REMOVERRUA, &CMainFrame::OnUpdateProjetoRemoverrua)
  ON_COMMAND(ID_MENU_PROJTERRAPL, &CMainFrame::OnMenuCalcProjTerrapl)
  ON_UPDATE_COMMAND_UI(ID_MENU_PROJTERRAPL, &CMainFrame::OnUpdateMenuProjterrapl)
  ON_UPDATE_COMMAND_UI(ID_DESDXFPROTERRAP, &CMainFrame::OnUpdateDesdxfproterrap)
  ON_UPDATE_COMMAND_UI(ID_REL_PROJ_TERRAPLANAGEM, &CMainFrame::OnUpdateRelProjTerraplanagem)
//  ON_COMMAND(ID_IMPORTARARQKML, &CMainFrame::OnImportarArqkml)
 // ON_UPDATE_COMMAND_UI(ID_IMPORTARARQKML, &CMainFrame::OnUpdateImportarArquivokml)
  ON_COMMAND(ID_GEOREF_IMP_PON_CONT, &CMainFrame::OnGeorefImportarPontosControle)
  ON_COMMAND(ID_GEOREF_IMP_MAPA, &CMainFrame::OnGeorefImportarMapa)
  ON_COMMAND(ID_DRONES_IMPORTARNUVEMDEPONTOS, &CMainFrame::OnDronesImportarnuvemdepontos)
  ON_COMMAND(ID_DESINSTTEMP, &CMainFrame::OnDesinstalar)
  ON_UPDATE_COMMAND_UI(ID_PROJ_IMPARQTOPOG, &CMainFrame::OnUpdateProjImparqtopog)
    END_MESSAGE_MAP()

static UINT indicators[] =
{
  ID_SEPARATOR,           // status line indicator
  ID_INDICATOR_CAPS,
  ID_INDICATOR_NUM,
  ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame() : MouseXY(NULL),GreideAtivo(FALSE),mpCDesTool1(NULL),mpCDesTool2(NULL),mpCDesTool3(NULL),mpCDesTool4(NULL),
DesPerfil(NULL),DesPontosGeo(NULL),Silencioso(false),GeometricoAlterado(false),GreideAlterado(false),SecaoAlterada(false),CalculouSuperf(false),
EstadoLicenca(true),NovoProj(true),DVolumes(NULL),m_ProjetosBox(AfxGetApp()), CasasDecimaisRelatorios(-1),TipoSuperficie(0),pDesCroquisView2(NULL),
ContaTentativas(0),ContaDias(0),VerificarChave(false), mpCToolBarTalvegues(NULL),FronteiraEditada(false), TrianImporTopografia(false),ProjetoNovo(-1),
VeioPagInicial(false),pListaArqRecentes(nullptr), pDialogoInicialView(nullptr),IniciarDesSuperf(true),SuperfCalculada(false)
{
  asdfg = 999;

  I = -1;                   //--- Ponteiro da pilha de projetos recentes.
  IP = -1;                  //--- Ponteiro para o vetor de projetos.

  for (int i = 0 ; i < 50 ; i++ ) m_strProjRec[i].Empty(); //--- Limpa a pilha de PR.

  IDENTIF[0] = ID_PROJREC1;  //--- IDs da pilha de projetos recentes
  IDENTIF[1] = ID_PROJREC2;
  IDENTIF[2] = ID_PROJREC3;
  IDENTIF[3] = ID_PROJREC4;
  IDENTIF[4] = ID_PROJREC5;
  IDENTIF[5] = ID_PROJREC6;

  Ext[0] = "IGU";            //--- Extensões dos arquivos. 
  Ext[1] = "TER";
  Ext[2] = "SEC";
  Ext[3] = "NIV";
  Ext[4] = "VER";
  Ext[5] = "HOR";
  Ext[6] = "GRA";
  Ext[7] = "OAE";
  Ext[8] = "TAC";
  Ext[9] = "TAA";
  Ext[10]= "ALS";
  Ext[11]= "OAC";
  Ext[12]= "PAV";
  Ext[13]= "TPA";
  Ext[14]= "CAN";
  Ext[15]= "LIM";
  Ext[16]= "CAN";
  Ext[17]= "FIX";
  Ext[18]= "SET";
  Ext[19]= "SAV";
  Ext[20]= "EMP";
  Ext[21]= "TPS";
  Ext[22]= "FSO";
  Ext[23]= "ATO"; 
  Ext[24]= "RAV";

  CX[0] = 380;               //--- Largura das janelas. 
  CX[1] = 890; 
  CX[2] = 747; 
  CX[3] = 590;
  CX[4] = 587;
  CX[5] = 860; 
  CX[6] = 330; 
  CX[7] = 780;
  CX[8] = 770; 
  CX[9] = 770; 
  CX[10]= 765; 
  CX[11]= 620;
  CX[12]= 350;
  CX[13]= 615; 
  CX[14]= 740;
  CX[15]= 350;
  CX[16]= 890;
  CX[17]= 890;
  CX[18]= 1100;
  CX[19]= 890;

  IDDSubtit[0] = IDD_IGUAL;  //--- Dialogos da barra de subtítulos
  IDDSubtit[1] = IDD_TERPON;
  IDDSubtit[2] = IDD_SECTIPO;
  IDDSubtit[3] = IDD_NIVELAM;
  IDDSubtit[4] = IDD_CURVER;
  IDDSubtit[5] = IDD_CURHOR; 
  IDDSubtit[6] = IDD_GRADI;
  IDDSubtit[7] = IDD_OAE;
  IDDSubtit[8] = 
  IDDSubtit[9] =
  IDDSubtit[10]= IDD_ALASAR;
  IDDSubtit[11]= IDD_OAC;
  IDDSubtit[12]= IDD_PAVIM;
  IDDSubtit[13]= IDD_TALPAVI;
  IDDSubtit[14]= IDD_CANTCEN;
  IDDSubtit[15]= IDD_LIMPA;
  IDDSubtit[16]= IDD_CANTCEN;
  IDDSubtit[17]= IDD_CANTCEN;
  IDDSubtit[18]= IDD_SECTRIAN;
  IDDSubtit[19]= IDD_SECAVUL;
  IDDSubtit[20]= IDD_EMPOLA;
  IDDSubtit[21]= IDD_SOLOS;
  IDDSubtit[22]= IDD_SONDAGEM;
  IDDSubtit[23]= IDD_ACITOPOG;
  IDDSubtit[24]= IDD_RELACADACIVUE;

  //--- Caracteres permitidos nos campos.
  /*
  CaracPerm[0] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 `-=\[];',./~!@#$%^&*()_+|{}:""<>?";
  CaracPerm[1] = "0123456789+-.";
  CaracPerm[2] = "EeDd";
  CaracPerm[3] = "CcEe";
  CaracPerm[4] = "0123456789";
  CaracPerm[5] = "PpIi";
  CaracPerm[6] = "0123456789.";
  CaracPerm[7] = "0123456789eE.+-";
  CaracPerm[8] = "CcAa";  */
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1) return -1;

  WINDOWPLACEMENT wp;

  if (ReadWindowPlacement(&wp))	SetWindowPlacement(&wp);   ///--- Posiciona a janela  

  if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
    | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
    !m_wndToolBar.LoadToolBar(IDR_EDITAR)) 
  {
    TRACE0("Failed to create toolbar\n");
    return -1;      // fail to create   
  }

  if (!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT)))
  {
    TRACE0("Failed to create status bar\n");
    return -1;      // fail to create
  }

  // TODO: Remove this if you don't want tool tips or a resizeable toolbar

  m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

  // TODO: Delete these three lines if you don't want the toolbar to be dockable

  m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

  m_wndToolBar.SetWindowText(_T("Editar"));

  EnableDocking(CBRS_ALIGN_ANY);

  DockControlBar(&m_wndToolBar);


  //---2a barra.
  /* 
  if (!m_wndToolBar2.Create(this) || !m_wndToolBar2.LoadToolBar(IDR_PROJETOS))
  {
  TRACE0("Failed to create toolbar\n");
  return -1;      // fail to create
  }
  // TODO: Remove this if you don't want tool tips or a resizeable toolbar
  */

  //  m_wndToolBar2.SetBarStyle(m_wndToolBar2.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

  //******************************************************

  if (!m_wndToolBar2.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
    | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0),IDR_PROJETOS) ||
    !m_wndToolBar2.LoadToolBar(IDR_PROJETOS)) 
  {
    TRACE0("Failed to create toolbar\n");
    return -1;      // fail to create   
  }

  //*************************************************************

  m_wndToolBar2.SetBarStyle(m_wndToolBar2.GetBarStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

  // TODO: Delete these three lines if you don't want the toolbar to be dockable

  m_wndToolBar2.EnableDocking(CBRS_ALIGN_ANY);
  m_wndToolBar2.SetWindowText(_T("Projetos"));

  //	EnableDocking(CBRS_ALIGN_ANY);

  DockControlBar(&m_wndToolBar2);

  //--- Prepara para a caixa de lista do Procurar.

  m_wndToolBar.SetButtonInfo(22, ID_SEPARATOR, TBBS_SEPARATOR, 12);
  m_wndToolBar.SetButtonInfo(23, ID_FINDCOMBO, TBBS_SEPARATOR, 150);

  CRect rect;
  m_wndToolBar.GetItemRect(23, &rect);
  rect.top = 3;
  rect.bottom = rect.top + 150;

  //--- Cria a caixa.

  if (!m_SearchBox.Create(CBS_DROPDOWN|WS_VISIBLE|WS_TABSTOP|CBS_AUTOHSCROLL|WS_VSCROLL,rect, &m_wndToolBar,ID_FINDCOMBO))
  {
    TRACE0("Falha ao criar a caixa de lista 1\n");
    return FALSE;
  }

  //--- Configura a fonte da caixa.	

  HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
  if (hFont == NULL) hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);
  m_SearchBox.SendMessage(WM_SETFONT, (WPARAM)hFont);

  //--- Prepara para a caixa de lista dos Projetos.

  m_wndToolBar2.SetButtonInfo(0, ID_SEPARATOR, TBBS_SEPARATOR, 12);
  m_wndToolBar2.SetButtonInfo(1, ID_FINDCOMBO2, TBBS_SEPARATOR, 250);
  m_wndToolBar2.SetButtonInfo(2, ID_SEPARATOR, TBBS_SEPARATOR, 12);

  m_wndToolBar2.GetItemRect(1, &rect);
  rect.top = 3;
  rect.bottom = rect.top + 150;

  //--- Cria a caixa. CarlosAR

  if (!m_ProjetosBox.Create(CBS_DROPDOWNLIST|WS_VISIBLE|WS_TABSTOP|CBS_AUTOHSCROLL|WS_VSCROLL,rect,&m_wndToolBar2, ID_FINDCOMBO2))
  {
    TRACE0("Falha ao criar a caixa de lista 1\n");
    return FALSE;
  }

  //--- Configura a fonte da caixa.	

  hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
  if (hFont == NULL) hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);
  m_ProjetosBox.SendMessage(WM_SETFONT, (WPARAM)hFont);
  m_ProjetosBox.SetHorizontalExtent(1000);
  m_ProjetosBox.SetDroppedWidth(1000);

  //------------------------------
  
  //--- Prepara para a caixa de lista de ruas.

  m_wndToolBar2.SetButtonInfo(3, ID_FINDRUA, TBBS_SEPARATOR, 150);

  m_wndToolBar2.GetItemRect(3, &rect);
  rect.top = 3;
  rect.bottom = rect.top + 100;

  //--- Cria a caixa. CarlosAR

  if (!m_RuasProjeto.Create(CBS_DROPDOWNLIST|WS_VISIBLE|WS_TABSTOP|CBS_AUTOHSCROLL|WS_VSCROLL,rect,&m_wndToolBar2, ID_FINDRUA))
  {
    TRACE0("Falha ao criar a caixa de lista 1\n");
    return FALSE;
  }

  m_RuasProjeto.Contem(CExComboBox::ID_RUAS);

  //--- Configura a fonte da caixa.	

  hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
  if (hFont == NULL) hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);
  m_RuasProjeto.SendMessage(WM_SETFONT, (WPARAM)hFont);
  m_RuasProjeto.SetHorizontalExtent(200);
  m_RuasProjeto.SetDroppedWidth(200);
  
  //----------------------------

  //--- Cria dialogo de progresso na barra de status.

  m_wndProgress.Create(IDD_PROGRESS,&m_wndStatusBar);

  LeProjetosRecentes();   //--- Lê os projetos recentes.

  char Buffer[1024]={0};
  unsigned long UltSegundos;
  
 std::string NomeArquivo("Vu&e.ini");

  ::GetPrivateProfileString(_T("DadosDesMedicao"),_T("QuanMedicoesADesenhar"),_T("2"),Buffer,5,(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual() + std::string(".ini")).c_str());
  ::GetPrivateProfileString(_T("DadosMainFrame"),_T("Status"),_T("0,0,0,0,0,50,0,1"),Buffer,1023,NomeArquivo.c_str());
  sscanf(Buffer,"%i,%i,%i,%i,%i,%i,%ld,%i",&GeometricoAlterado,&GreideAlterado,&GreideAlterado,&CalculouSuperf,&ContaTentativas,&ContaDias,&UltSegundos,&IniciarDesSuperf);

  CTime Agora(CTime::GetCurrentTime());
  time_t AgoraSegundos(Agora.GetTime());

  unsigned long TresDias(3*24*60*60);

  unsigned long Lapso(AgoraSegundos - UltSegundos);

 if(Lapso > TresDias)
 {
   VerificarChave = true;
 }

  std::string Temp(Buffer);
  size_t Pos(Temp.find_last_of(','));

  if(Pos != std::string::npos)
  {
    std::string RuaAtual(Temp.substr(Pos + 1), Temp.length() - Pos -1);
    m_RuasProjeto.SetCurSel(m_RuasProjeto.FindString(0,RuaAtual.c_str()));
  }

  QMedADesenhar = 0;// atoi(Buffer);

  LoadBarState(_T("MainFrmToolBars"));    //--- Le e seta o estados dos toolsbars

  TimerChave = SetTimer(EVENT_CHAVE,1000, NULL);   

  // CG: The following line was added by the Splash Screen component.

 // CSplashWnd::ShowSplashScreen(this);
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
  CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
  CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnProjAbrir() 
{
#ifndef _DEBUG
#ifndef DEMOVER
  EstadoLicenca = OVerfNeoKey.Verifica(); 

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada, está vencida ou está inválida.");
    return;
  } 
#endif
#endif

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada, está vencida ou está inválida inválida.");
    return;
  }

  if(!VeioPagInicial) 
  {
    ((CEstApp*)AfxGetApp())->AbrirArquivo(IDD_DIAINICIAL);
    VeioPagInicial = true;
  }

  if (IP == 10) return;

  CString filtro = "Cabeçalho (*.prj) | *.prj||",NomeProj;

  CFileDialog diaabrearq(TRUE,NULL,NULL,OFN_EXPLORER | OFN_FILEMUSTEXIST,filtro,NULL);

  diaabrearq.m_ofn.lpstrTitle = "Vias Urbanas & Estradas - Abrir projeto :";

  if (VeioPagInicial || diaabrearq.DoModal() == IDOK)
  {
    if(VeioPagInicial) NomeProj = NomeProjPagInicial.c_str();
    else NomeProj= diaabrearq.GetPathName();

    if (!NomeProj.IsEmpty())
    {
      if (!VeioPagInicial) NomeProj = NomeProj.Left(NomeProj.GetLength() - 4);  //--- Retira a extensão.

      int Ret(InsLstProjetos(NomeProj));

      if(Ret == 0)       //--- Insere o nome na caixa. 0 == ok;
      {
        IncluiProjetoNoVetor(NomeProj); //--- Insere o projeto no vetor de projetos

        m_RuasProjeto.ResetContent();
        CEstApp::LeListaRuas();
        int C(0);

        if(CEstApp::LstRuas.size() > 0)
        {
          for (ittlstwstr it = CEstApp::LstRuas.begin() ; it != CEstApp::LstRuas.end() ; it++,C++) 
          {
            m_RuasProjeto.InsertString(C,it->c_str());
          }
          SelecionaRua(RuaAtual.GetBuffer());
        }
        else
        {
          AfxMessageBox("Ainda não foram criados trechou ou ruas neste projeto, não há nada para ser feito ou visualizado nele. Crie uma rua ou um trecho para poder dar início a execução de um projeto.");
        }
      }
      else
      {
        if(Ret == 2 || Ret == 4)      //----- O projeto não existe mais
        {
          int i(0);
          bool Achou(false);

          if (I != -1)  //--- se a pilha não está vazia.
          {
            //--- Verifica se o nome já esta na pilha.


            for (int i = 0; i <= I && i < 6 && !Achou; i++)
              if (!m_strProjRec[i].Compare(RuaAtual)) Achou = TRUE;
          }

          if (!Achou)
          {
            CMenu  *pMenu = GetMenu()->GetSubMenu(0);

            pMenu->RemoveMenu(IDENTIF[i], MF_BYCOMMAND);
            RemoveLista(i);
          }
        }
      }
    }
  }
  VeioPagInicial = false;
}
int CMainFrame::InsLstProjetos(CString Nome)
{
  CString Rasc;
  int Achou = FALSE;

  if (IP == 4) return 1;

  //--- Verifica se o Nome já existe na caixa.
  
  int NumProj(0);

  for ( ; NumProj < m_ProjetosBox.GetCount() && !Achou ; NumProj++)
  {
    m_ProjetosBox.GetLBText(NumProj,Rasc);
    Achou = (!Rasc.CompareNoCase(Nome));
  }

  //--- Se não existe insere o nome na caixa e na lista de projetos recentes. 

  if (!Achou) 
  {
    CString NomeDiretorio(Nome.Left(Nome.ReverseFind('\\')));

    if(DirectoryExists(NomeDiretorio))
    {
      m_ProjetosBox.InsertString(0,Nome);
      m_ProjetosBox.SetCurSel(0);
      m_sNomeComp = Nome;
      IncluiProjetoNoVetor(Nome);    //--- Insere o projeto no vetor de projetos abertos
    }
    else
    {
      if(AfxMessageBox("Este projeto não foi encontrado, deseja removê-lo da lista de projetos recentes?",MB_YESNO|MB_ICONEXCLAMATION) == IDYES)
        return 2;
      else
        return 3;
    }
  }
  else
  {
    m_ProjetosBox.SetCurSel(NumProj-1);
    m_sNomeComp = Nome;
  }

  return 0;
}

void CMainFrame::OnProjClose() 
{
  CString      Titulo,NomeComp;
  BOOL         Maximizada;

  m_ProjetosBox.GetLBText(0,Titulo);  //--- busca o nome do projeto atual.

  //--- Varre Todas as janelas filhas e fecha as que forem do projeto atual.

  for (CMDIChildWnd *PrimCW = NULL,*CW ; (CW = MDIGetActive(&Maximizada)) && CW != PrimCW ; MDINext())
  {
    NomeComp = m_strArqAtivo.Left(m_strArqAtivo.Find('.')); //-- Nome da do arquivo da janela que foi ativada
    if (!NomeComp.CompareNoCase(Titulo))    //--- Se for igual, fecha a janela.
      CW->SendMessage(WM_CLOSE,0,0);
    else if (!PrimCW) PrimCW = CW; //--- Marca a 1a não fechada para finalizar o for
  }
  //--- Apaga o nome da lista de projetos.

  m_ProjetosBox.DeleteString(0);
  m_ProjetosBox.SetCurSel(0);
  ExcluiProjetoNoVetor(Titulo); //--- Exclui o projeto do vetor de projetos
}

void CMainFrame::ProjNovoCriaDir(const std::string& Dir) 
{
  DirRua = GetUserHomeDir() + "\\VUE\\";
  _mkdir(DirRua);

  OnProjNovo();
}

void CMainFrame::OnProjNovo() 
{
  CString DirRua(GetUserHomeDir() + "\\Documents\\VUE");
  CDiaCriarProjeto DiaCriarProjeto(this,DirRua,ProjetoNovo == 4);  //--- 4 = ProjACAD
  DiaCriarProjeto.DoModal();
  
  if(DiaCriarProjeto.OK == true)
  {
    CString NomeDir(DiaCriarProjeto.CStrDiretorio + "\\" + DiaCriarProjeto.CStrNomeProjeto), DirProjeto(NomeDir);
   /* std::string NomePrj(NomeDir + "\\" + DiaCriarProjeto.CStrNomeProjeto + ".prj");

    if (FileExists(NomePrj.c_str()))

    {
      monolog.mensagem(SEMMENSPADRAO, "Já existe um projeto com este nome neste local, o novo projeto não pôde ser criado");
          return;
        }

    int Erro(_mkdir(NomeDir));
    if (Erro == -1)
    {
      if(errno == ENOENT)
        {
          monolog.mensagem(SEMMENSPADRAO,"O diretório não pôde ser criado, verifique se você tem autorização para criar diretórios nerste local?");
          return;
      }
    }
    */

    if(!SetCurrentDirectory(NomeDir.GetBuffer())) AfxMessageBox("Houve erro ao ler o diretório do projeto");

    NomeDir += '\\';
    NomeDir += DiaCriarProjeto.CStrNomeProjeto;

    if(VerificaAberturaTarefa(NomeDir) == false) return;

    InsLstProjetos(NomeDir);
    AdProjrecentes(NomeDir);

    CConfigProj ConfigPproj(NULL, DiaCriarProjeto.CStrNomeProjeto, DirProjeto, CConfigProj::NOVO);

    CString NomeProj, Dir;

    ConfigPproj.Dialoga(NomeProj, Dir);

    if (ConfigPproj.Ok == true)
    {
      if (VerificaAberturaTarefa(NomeDir) == true)
      {
        std::string Nome(NomeDir.GetBuffer());

        monolog.mensagem(-1,"Todos projetos devem possuir pelo menos um trecho, crie agora o primeiro trecho(ou rua) deste projeto.");
        {
          do
          {
            ((CEstApp*)AfxGetApp())->OnProjetoInserirRua();

          }while(RuaAtual.GetLength() == 0);
        }
      }
    }
    else ProjetoNovo = -1;
  }
  else ProjetoNovo = -1;
}

void CMainFrame::OnConfigProj() 
{
  CString NomeArq,Dir;
  int ProjSel(m_ProjetosBox.GetCurSel());
  if(ProjSel != CB_ERR)
  {
    m_ProjetosBox.GetLBText(ProjSel,NomeArq);
    Dir = NomeArq.Left(NomeArq.ReverseFind('\\')) + "\\";
    NomeArq = NomeArq.Right(NomeArq.GetLength() - Dir.GetLength());

    CConfigProj configproj(this,NomeArq,Dir,CConfigProj::EXISTENTE);
    configproj.Dialoga(NomeArq,Dir);

    if(configproj.Diretorio.GetAt(configproj.Diretorio.GetLength()) != '\\') configproj.Diretorio += "\\";

    if (configproj.Diretorio.CompareNoCase(Dir) != 0)
    {
      InsLstProjetos(configproj.Diretorio + configproj.NomeProj);
      AdProjrecentes(configproj.Diretorio + configproj.NomeProj);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////
//
void CMainFrame::AdProjrecentes(CString Nome)
//*************************************************************************
//---  Adiciona itens na pilha de projetos recentes (m_strProjRec[6]).
{  
  BOOL        Achou = FALSE;
  CMenu       *pMenu = GetMenu()->GetSubMenu(0);

  if (!pMenu || Nome.IsEmpty()) return;

  //if(Nome.Find('\\')) Nome = Nome.Right(Nome.GetLength() - Nome.ReverseFind('\\')-1);


  if (I != -1)  //--- se a pilha não está vazia.
  {
    //--- Verifica se o nome já esta na pilha.

    for (int  i = 0 ;  i <=  I && i < 6 && !Achou; i++)
      if (!m_strProjRec[i].Compare(Nome)) Achou = TRUE;
  }

  if (!Achou) //--- O nome ainda não está na pilha.
  {
    I++;   //--- Vai incluir, então incrementa o ponteiro da pilha.
    if (I == 50)   //---Estourou a pilha?
    {
      //---  Sim, então roda a pilha, liberando a última posição.
      I--;   //--- Ponteiro da pilha aponta para o topo.

      for (int z = 0 ; z < 49 ; z++) m_strProjRec[z]= m_strProjRec[z+1]; //--- Roda a pilha

      pMenu->RemoveMenu(8,MF_BYPOSITION); //--- Remove a 1a posição do menu.
    }
    m_strProjRec[I] = Nome;  //-- Inclui o nome na última posição atual da pilha.

    //--- com o dialogo inicial não inclui mais

   // pMenu->InsertMenu(3,MF_BYPOSITION | MF_STRING,IDENTIF[I],Nome); //--- Inclui no menu.
  }
}

void CMainFrame::RemoveLista(int Num)
{
  for (int z = Num ; z < 49 ; z++) m_strProjRec[z] = m_strProjRec[z+1]; 

  m_strProjRec[50] = "";

  I--;
}

bool CMainFrame::VerfMedicao()
{
  return false; 

  CString NomeProj;
  int Corrente = m_ProjetosBox.GetCurSel();
  if (Corrente != LB_ERR)
  {
    m_ProjetosBox.GetLBText(Corrente,NomeProj);
    CString Rotulo = (NomeProj.Mid(NomeProj.GetLength() - 6,3));
    int z = Rotulo.CompareNoCase("MED");

    return Rotulo.CompareNoCase("MED") ? FALSE : TRUE;
  }
  else return FALSE;
}

void CMainFrame::PoeTopo(int Num,CString Nome)
{
  CMenu *pMenu(GetMenu()->GetSubMenu(0));

  //--- É preciso excluir o menu atual e incluir todos os ítens novamente
  //--- acertando o ID (IDENTF[i]) (Quando exclui um item os IDs nào mudam,
  //--- o 0 fica com 1, o 1 com 2...)

  for (int i = 0 ; i <= I ; i++) 
    pMenu->RemoveMenu(3,MF_BYPOSITION); //--- Remove todos os itens do menu.
  RemoveLista(Num);                     //-- Remove da lista a posiçào escolhida
  for (int i = 0 ; i <= I ; i++)
    pMenu->InsertMenu(3,MF_BYPOSITION | MF_STRING,IDENTIF[i],m_strProjRec[i]);

  AdProjrecentes(Nome); //--- Adiciona-a no topo da pilha (e da lista).
  m_sNomeComp = Nome;   //--- Atualiza o projeto atual.
}

void CMainFrame::AbreProjRec(int Num,CString* pNomeProj)
{
  CString NomeProj;
  CMenu* pMenu(GetMenu()->GetSubMenu(0));
  UINT z(pMenu->GetMenuItemCount());

  int Ret(-1);

  if(pNomeProj == nullptr)  //--- Se true veio do menu, se false veio do dialogo inicial (CDiaInicial)
  { 
    pMenu->GetMenuString(IDENTIF[Num],NomeProj,MF_BYCOMMAND);
    Ret = InsLstProjetos(NomeProj);
  }
  else Ret = InsLstProjetos(*pNomeProj);

  int Sel(CB_ERR);

  if(Ret == 0)
  {
    if (Num < I) PoeTopo(Num,NomeProj);

    Sel = m_ProjetosBox.FindStringExact(-1,NomeProj);

    if(Sel != CB_ERR) m_ProjetosBox.SetCurSel(Sel);
    else 
    {
      m_ProjetosBox.InsertString(0,NomeProj);
      m_ProjetosBox.SetCurSel(0);
    }

    std::string ArqPrj(NomeProj + ".prj");
	
    if (FileExists(ArqPrj.c_str()))
    {
      MudouProjeto(NomeProj.GetBuffer());
    }
    else
    {
      AfxMessageBox("Ainda não foi criado um projeto neste local");
      m_ProjetosBox.SetCurSel(-1);
      return;
    }
  }
  else
  {
    if(Ret == 2)    //--- O Projeto não existe mais
    {
      pMenu->RemoveMenu(IDENTIF[Num],MF_BYCOMMAND);
      if(Sel != CB_ERR) m_ProjetosBox.DeleteString(Sel);
      RemoveLista(Num);
    }
  }
}

void CMainFrame::LeProjetosRecentes()
{
  CFile    f;    
  CString  Nome;

  if (f.Open(".\\est.rec",CFile::modeRead | CFile::shareDenyNone,NULL))
  {
    CArchive ar(&f,CArchive::load);
    for (int i = 0 ; i < 50 ; i++) 
    {
      ar >> Nome;
      if (!Nome.IsEmpty()) AdProjrecentes(Nome);
    }

    ar.Close();
    f.Close();
  }
  strArquivoRec = f.GetFilePath();
}

void CMainFrame::GravaProjetosRecentes()
{
  CFile f;    

  if (f.Open(".\\est.rec",CFile::modeCreate | CFile::shareDenyNone	| CFile::modeWrite,NULL))
  {
    CArchive ar(&f,CArchive::store);

    for (int i = 0 ; i < 50 ; i++) 
      ar << m_strProjRec[i];

    ar.Close();
    f.Close();
  }
}

///////////////////////////////////////////////////////////////////////////
//ON_COMMAND_UI

void CMainFrame::OnJanelaCentrar() 
{
  BOOL Maximizada;

  //--- Varre Todas as janelas filhas e as centraliza.

  for (CMDIChildWnd *PrimCW = NULL,*CW ; (CW = MDIGetActive(&Maximizada)) != PrimCW ; MDINext())
  {
    CW->CenterWindow();
    if (!PrimCW) PrimCW = CW;  //--- Marca a primeira janela não fechada. (Para finalizar).
  }
}

void CMainFrame::OnProxJanela() 
{
  MDINext();
}

void CMainFrame::OnDestroy() 
{
  GravaProjetosRecentes();
}

void CMainFrame::OnProjrec1() 
{
  AbreProjRec(0);
}

void CMainFrame::OnProjrec2() 
{
  AbreProjRec(1);
}

void CMainFrame::OnProjrec3() 
{
  AbreProjRec(2);
}

void CMainFrame::OnProjrec4() 
{
  AbreProjRec(3);
}

void CMainFrame::OnProjrec5() 
{
  AbreProjRec(4);
}

void CMainFrame::OnProjrec6() 
{
  AbreProjRec(5);
}


//////////////////////////////////////////////////////////////////////////////
// UPDATE_COMMAND_UI

//--- Todos estes items só devem ser habilitados se houver projeto.

void CMainFrame::OnUpdateProjClose(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_ProjetosBox.GetCount());
}

void CMainFrame::OnUpdateConfigProj(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_ProjetosBox.GetCount());
}

void CMainFrame::OnUpdateArqAbrir(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCount());
}

void CMainFrame::OnUpdateArqNovo(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCount());
}

void CMainFrame::OnUpdateCalcterreno(CCmdUI* pCmdUI) 
{
   pCmdUI->Enable(m_RuasProjeto.GetCount() && !DesPerfil && /*!DesSecao &&*/ !DesPontosGeo);
}

//--- Nova Medicao : so está habilitado se o projeto corrente não for medição

void CMainFrame::OnUpdateMedioNova(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_ProjetosBox.GetCount() && !VerfMedicao());
}

//--- Calcular Medicao : so está habilitado se houver projeto e
//--- o projeto corrente for uma medição

void CMainFrame::OnUpdateMedioCalcularamedio(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_ProjetosBox.GetCount() && VerfMedicao());
}

//--- Abrir Uma Medicao : so está habilitado se houver  e
//--- projeto corrente não for medição.

void CMainFrame::OnUpdateMedioAbrir(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_ProjetosBox.GetCount() && !VerfMedicao());
}

///////////////////////////////////////////////////////////////////////////
//

//--- !!!!! Atualizar o emedicao na mudanca do cropbox de projetos e colocar aqui a variável EMmedicao,
//--- verfMedicao é muito pesada para ser UpdatecomandUI

void CMainFrame::OnUpdateJanelaCentrar(CCmdUI* pCmdUI) 
{
  BOOL Maximizada;

  if (MDIGetActive(&Maximizada)) pCmdUI->Enable(TRUE);
  else pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateProxJanela(CCmdUI* pCmdUI) 
{
  BOOL Maximizada;
  CWnd *W = MDIGetActive(&Maximizada);

  if (W && W->GetNextWindow(GW_HWNDNEXT)) pCmdUI->Enable(TRUE);
  else pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateClculosCalculatanotadeservio(CCmdUI* pCmdUI) 
{
 pCmdUI->Enable(m_RuasProjeto.GetCount() && !DesPerfil && /*!DesSecao &&*/ !DesPontosGeo && m_RuasProjeto.GetCurSel() != -1 && !VerfMedicao());
}

void CMainFrame::OnCalcterreno() 
{
  #ifndef _DEBUG
    #ifndef DEMOVER
      EstadoLicenca = OVerfNeoKey.Verifica(); 

      if(EstadoLicenca == false) 
      {
        AfxMessageBox("A chave de segurança não foi encontrada ou vencida ou está inválida.");
        return;
      }
    #endif
  #endif

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada, está vencida ou está inválida.");
    return;
  }

  CString Projeto;

  m_ProjetosBox.GetLBText(0,Projeto);
  CalculaTerreno  CalcTerre(Projeto);
  CalcTerre.DoModal();
} 

class CProjeto* CMainFrame::AchaProjetoNoVetor()
{
  return VetorProjetos.AchaProjeto(m_sNomeComp);
}

void CMainFrame::ExcluiProjetoNoVetor(CString Projeto)
{
  VetorProjetos.ExcluiProjeto(Projeto);
}

void CMainFrame::IncluiProjetoNoVetor(CString Nome)
{
  VetorProjetos.IncluiProjeto(Nome);
}

void CMainFrame::OnMedioAbrir() 
{
  class AbrirMedicao CDAbrirMedicao;

  CDAbrirMedicao.DoModal();
}

void CMainFrame::OnMedioCalcularamedio() 
{
  //--- Executa o diálogo para cálculo da mediçao.

  CString Projeto;

  m_ProjetosBox.GetLBText(0,Projeto);
  CalcMedicao CalculaMedicao(Projeto);
  CalculaMedicao.DoModal();
}

void CMainFrame::OnMedioNova() 
{
  CCriarMedicao CriarMedicao;
  CriarMedicao.DoModal();	
}

CFrameWnd* CMainFrame::GetActiveFrame() 
{
  // TODO: Add your specialized code here and/or call the base class

  return CMDIFrameWnd::GetActiveFrame();
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
  return CMDIFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
}

void CMainFrame::OnSeccionar() 
{
  #ifdef DEMOVER
  ::MessageBox(NULL,"Função indisponível na cópia de demonstração","VU&E",MB_OK);
#else
 #ifndef _DEBUG
  EstadoLicenca = OVerfNeoKey.Verifica(); 

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada, está vencida ou está inválida.");
    return;
  }
  #endif

  try
  {
    OnCalcSecTranversais();
  }
  catch(...)
  {
    MessageBox("Houve Erro na chamada da função de cálculo das seções transversais. (ERR-0010A)","VU&E");
  }
#endif
}

void CMainFrame::OnCalculaNotaDeServico() 
{
#ifdef DEMOVER
  ::MessageBox(NULL,"Função indisponível na cópia de demonstração","VU&E",MB_OK);
#else
 #ifndef _DEBUG
  EstadoLicenca = OVerfNeoKey.Verifica(); 

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada, está vencida ou está inválida.");
    return;
  }
  #endif

  CString Projeto(PegaProjetoAtual());

  try
  {
    CCalcNS::CalculaNS(Projeto,((CMainFrame*)AfxGetMainWnd())->AchaProjetoNoVetor());
  }
  catch(...)
  {
    MessageBox("Houve Erro na chamada da função de cálculo da nota de serviço, contacte o suporte. (ERR-0010)","VU&E");
  }
#endif
}

CToolBar* CMainFrame::PegaBarra(int Num)
{
  switch (Num)
  {
    case 1 : return mpCDesTool1; 
    case 2 : return mpCDesTool2;
    case 3 : return mpCDesTool3;
    case 4 : return mpCDesTool4;
    case 5 : return mpCDesToolGreide;
    case 6 : return mpToolBarPerfGeol;
    case 7 : return mpCToolBarTalvegues;
  }

 // ASSERT(0);
  exit(1);
    
  return NULL;
}

void CMainFrame::OnClose() 
{
  //--- Thanks to MFC programer´s Team!
  // SaveBarState saves everything but the number of Columns in the Palette
  // we need to do that ourseleves.

  SaveBarState(_T("MainFrmToolBars"));  //--- Salva o estado dos toolsbars

  WINDOWPLACEMENT wp;
  wp.length = sizeof wp;
  if (GetWindowPlacement(&wp)) WriteWindowPlacement(&wp);

  CTime Agora = CTime::GetCurrentTime();
  time_t AgoraSegundos = Agora.GetTime();

  int Dia = Agora.GetDay();
  int Ano = Agora.GetYear();
  int Mes = Agora.GetMonth();

  UpdateData(true);  //--- Para pegar o IniciarDesSuperf
  CString CSDados;

  std::string NomeArquivo("Vu&e.ini");

  CSDados.Format("%i,%i,%i,%i,%i,%i,%ld,%i",GeometricoAlterado,GreideAlterado,SecaoAlterada,CalculouSuperf,ContaTentativas,ContaDias, AgoraSegundos,IniciarDesSuperf);
  ::WritePrivateProfileString(_T("DadosMainFrame"),_T("Status"),CSDados,NomeArquivo.c_str());		

  while(MapDesenhosAlterados.size() > 0)
  {
    ittyMapStrVoid it(MapDesenhosAlterados.begin());

    if(it->first.find("DesSecao")!= std::string::npos)
    {
       DesSecaoView* DSV((DesSecaoView*)it->second);
       SelecionaRua(DSV->PegaRua());
       DSV->SetFocus();               //--- Não funciona aqui
       DSV->GravarSecoes();
    }
    else
    {
      if(it->first.find("DesPerfil") != std::string::npos)
      {
        DesPerfilView* DPV((DesPerfilView*)it->second);
        SelecionaRua(DPV->PegaRua());
        DPV->SetFocus();              //--- Não funciona aqui
        DPV->BringWindowToTop();      //--- Idem
        DPV->GravarProjeto();
      }
    }
  }

   /*
  int Maximizada(0);
  CMDIChildWnd *CW(MDIGetActive(&Maximizada)),*PrimCW(CW);
   
  while(CW) 
  {
    CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->GetMainWnd();

    // Get the active MDI child window.
    CMDIChildWnd *pChild = (CMDIChildWnd*)pFrame->GetActiveFrame();

    // or CMDIChildWnd *pChild = pFrame->MDIGetActive();

    // Get the active view attached to the active MDI child window.

    bool ViewConhecida(true);

//    CView *pView = (CView*)pChild->GetActiveView();

    CView *pView = (CView*)pChild->GetTopWindow();

    if(pView)
    { 
      if(pView->IsKindOf(RUNTIME_CLASS(DesSecaoView)))
      {
        DesSecaoView* DSV((DesSecaoView*)pView);

        SelecionaRua(DSV->PegaRua());
        DSV->GravarSecoes();
      //  pView->SetFocus();
      }
      else
      {
        if(pView->IsKindOf(RUNTIME_CLASS(DesPerfilView)))
        {
          SelecionaRua(((DesPerfilView*)pView)->PegaRua());
          ((DesPerfilView*)pView)->GravarProjeto();
         // pView->SetFocus();
        }
        else  ViewConhecida = false;
      }

      if(pView && ViewConhecida)
      {
        CW->SendMessage(WM_CLOSE);
      //  CW->DestroyWindow();
      }
    }

    MDINext();
    CW = MDIGetActive(&Maximizada);

    if(CW == PrimCW) CW = NULL;    //---  buffer circular.
  }
  */

  CMDIFrameWnd::OnClose();
}

const char* CMainFrame::PegaProjetoAtual() 
{
  static char Temp[1024] = {0};
  m_ProjetosBox.GetLBText(m_ProjetosBox.GetCurSel(),Temp);

  char pRua[256] = {0};

  m_RuasProjeto.GetLBText(m_RuasProjeto.GetCurSel(),pRua);

  if(strlen(pRua) == 0) strcpy(pRua,"#");

  std::string NomeProjetoRua(Temp);

  NomeProjetoRua = NomeProjetoRua.substr(0,NomeProjetoRua.find_last_of('\\'));

  NomeProjetoRua += "\\" + std::string(pRua) + "\\" + pRua;
  strcpy(Temp,NomeProjetoRua.c_str());

  return Temp;
}

const char* CMainFrame::PegaRuaAtual() 
{
  static char Temp[1024] = {0};
  m_RuasProjeto.GetLBText(m_RuasProjeto.GetCurSel(),Temp);

  return Temp;
}

void CMainFrame::RemoveRua(const std::string& Rua) 
{
  m_RuasProjeto.DeleteString(m_RuasProjeto.FindString(0,Rua.c_str()));
}


void CMainFrame::TrocaRua(const std::string& Rua)
{
  size_t Pos(Rua.find_last_of('\\')+1);
  std::string NomeCurto(Rua.substr(Pos, Rua.size() - Pos));
  m_RuasProjeto.SetCurSel(m_RuasProjeto.FindString(0, NomeCurto.c_str()));
}

void CMainFrame::OnCalculoVolumes() 
{
#ifdef DEMOVER
  ::MessageBox(NULL,"Função indisponível na cópia de demonstração","VU&E",MB_OK);
#else
  #ifndef _DEBUG
  EstadoLicenca = OVerfNeoKey.Verifica(); 

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada ou está vencida ou está inválida.");
    return;
  }
  #endif

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada, está vencida ou está inválida.");
    return;
  }

//  CString Projeto;
  CString Projeto(PegaProjetoAtual());

  try
  {
//    m_ProjetosBox.GetLBText(0,Projeto);

    CCalcVL::CalculaVL(Projeto,((CMainFrame*)AfxGetMainWnd())->AchaProjetoNoVetor());
  }
  catch(...)
  {
    MessageBox("Houve Erro na chamada da função de cálculo dos volumes, contacte o suporte. (ERR-0030)","VU&E");
  }
#endif
}

void CMainFrame::OnSuperfcieGerar() 
{
#ifdef DEMOVER
  ::MessageBox(NULL,"Função indisponível na cópia de demonstração","VU&E",MB_OK);
#else
  #ifndef _DEBUG
  EstadoLicenca = OVerfNeoKey.Verifica(); 

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada ou está vencida ou está inválida.");
    return;
  }
  #endif

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada, está vencida ou está inválida.");
    return;
  }
  CString Projeto;

  try
  {
    m_ProjetosBox.GetLBText(0,Projeto);
    char Buffer[255] = { 0 };

    //--- Pega o tipo de superficie;

    ::GetPrivateProfileString(_T("DadosProjeto"), _T("TipoSuperf"), _T("0,0,0,0"), Buffer, 511, Projeto+".ini");
    sscanf(Buffer, "%i",&TipoSuperficie);

    //--- Diálogo de cálculo da superficie

    CDCalcSuperf DiaCalcSuperfProjeto(Projeto);

    DiaCalcSuperfProjeto.DoModal();

  
  /*  CDCalcSuperf* pDiaCalcSuperf = new CDCalcSuperf(Projeto);
    pDiaCalcSuperf->Create(CDCalcSuperf::IDD);
    pDiaCalcSuperf->ShowWindow(SW_SHOW);*/
  

  //  WaitForSingleObject(pDiaCalcSuperf->PegaEventoFim(),INFINITO);

/*    do
    {
      WaitForSingleObject(CEvent(),300);
    }while(pDiaCalcSuperf->Terminou == false);
*/

  }
  catch(...)
  {
    MessageBox("Houve Erro na chamada da função de cálculo da superfície, contacte o suporte. (ERR-0020)","VU&E");
  }
#endif
}

//=== Desativado -> o cálculo é feito on line no desenho do projeto geométrico e esta dando um erro no recálculo calculo
//===               o eixo esta girando se calcular muitas vezes (> 7) seguidas, parece erro de arredondamento
void CMainFrame::OnCalcularGeometrico() 
{
#ifdef DEMOVER
  ::MessageBox(NULL,"Função indisponível na cópia de demonstração","VU&E",MB_OK);
#else
  #ifndef _DEBUG
  EstadoLicenca = OVerfNeoKey.Verifica(); 

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada ou está vencida ou está inválida.");
    return;
  }
  #endif

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada, está vencida ou está inválida.");
    return;
  }

  CString Projeto;

  m_ProjetosBox.GetLBText(0,Projeto);
  CDCalcGeometrico DCalcGeometrico(this,Projeto,((CMainFrame*)AfxGetMainWnd())->AchaProjetoNoVetor());
  DCalcGeometrico.DoModal();
#endif
}

void CMainFrame::OnCalcSecAvulsas()
{
  #ifdef DEMOVER
    ::MessageBox(NULL,"Função indisponível na cópia de demonstração","VU&E",MB_OK);
  #else
    CDiaCalcSecAvulsas DiaCalcSecAvulsas;

    DiaCalcSecAvulsas.DoModal();
#endif
}

void CMainFrame::OnCalcSecTranversais()
{
  #ifdef DEMOVER
  ::MessageBox(NULL,"Função indisponível na cópia de demonstração","VU&E",MB_OK);
  #else
 #ifndef _DEBUG
  EstadoLicenca = OVerfNeoKey.Verifica(); 

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada ou está vencida ou está inválida.");
    return;
  }
  #endif

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada, está vencida ou está inválida.");
    return;
  }

  CString Projeto;

  m_ProjetosBox.GetLBText(0,Projeto);
  CDSecionar DSecionar(Projeto,this);
  DSecionar.DoModal();

#endif
}

void CMainFrame::OnCalcAreaTaludes()
{
  #ifdef DEMOVER
    ::MessageBox(NULL,"Função indisponível na cópia de demonstração","VU&E",MB_OK);
  #else
 #ifndef _DEBUG
  EstadoLicenca = OVerfNeoKey.Verifica(); 

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada ou está vencida ou está inválida.");
    return;
  }
  #endif

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada, está vencida ou está inválida.");
    return;
  }

  CDCalAreaTaludes DCalcAreaTaludes;

  DCalcAreaTaludes.DoModal();
 #endif
}

void CMainFrame::OnCalcAreaLimpeza()
{
  #ifdef DEMOVER
    ::MessageBox(NULL,"Função indisponível na cópia de demonstração","VU&E",MB_OK);
  #else
  #ifndef _DEBUG
  EstadoLicenca = OVerfNeoKey.Verifica(); 

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada ou está vencida ou está inválida.");
    return;
  }
  #endif

  if(EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada, está vencida ou está inválida.");
    return;
  }

  CDiaCalcLimpeza DiaCalcLimpeza;

  DiaCalcLimpeza.DoModal();
  #endif
}

void CMainFrame::OnTerrenoAcidentestopogrficos() 
{
  /*
  CString Projeto;

  m_ProjetosBox.GetLBText(0,Projeto);

  CDDefBorAciTopog DefBordosAciTopograficos(this,Projeto);
  DefBordosAciTopograficos.DoModal();
  */
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  CMDIFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnUpdateSuperfcieGerar(CCmdUI* pCmdUI) 
{
 //--- TipoSuperficie: 1-> Drones 4-> Importada do Autocad
 //  enum { SUPERF_TRADICIONAL, SUPERF_DRONES, SUPERF_GOOGLEEARTH, SUPERF_GERENMAPAS, SUPERF_TOPOG_ACAD };

 //--- Foi preciso remover tb a topografia importada do autocad porque os pontos do autocad vêm com muitos erros
 //--- cotas zeradas, cotas muito altas ou baixas e isso provoca muitos erros no cálculo da superficie 
 //--- foi feito um grande esforço no sentido de tentar calcular a superfície do Autocad mas não dá
 // --- Para superficie ficam as CN do Autocad  ===> NAO PODE POQUE NAO TRIANGULA 
 //--- O código de cáculo da superficie  em CalcSU esta incluindo a o cálculo de superfície do  Autocad ==> REMOVER
 
 //--- =======>>>>> Tentar mais um pouco.

    //--- 19112023
   //--- Tentei mais um pouco e agora quando pede para calcular a superfície mais vezes troca os tipos de acidentes topograficos
   //--- fica certo na descrição mas com o desenho de outro tipo
   //--- Acertar numa proxima versão

 pCmdUI->Enable(!DesPerfil && /*!DesSecao &&*/ !DesPontosGeo && TipoSuperficie != 4 && TipoSuperficie != 1 && TipoSuperficie != 3); // && TipoSuperficie != 4);

  //pCmdUI->Enable(!DesPerfil && /*!DesSecao &&*/ !DesPontosGeo && TipoSuperficie != 1);
}

void CMainFrame::OnUpdateTerrenoSeccionar(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCount() > 0 && !DesPerfil && /*!DesSecao &&*/ !DesPontosGeo);
}

void CMainFrame::OnUpdateCalculoVolumes(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCount() > 0 && !DesPerfil && /*!DesSecao &&*/ !DesPontosGeo);
}

void CMainFrame::OnUpdateClculosCalcularosdadosgeomtricos(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCount() > 0 && !DesPerfil && /*!DesSecao &&*/ !DesPontosGeo);
}

void CMainFrame::OnUpdateDesenhosSecoes(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateEditarPontosGeo(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

CMDIChildWnd* CMainFrame::BuscaJanela(const CString& Titulo) 
{
  CString TituloAtual;
  int Maximizada;
  CMDIChildWnd *CW = MDIGetActive(&Maximizada),*PrimCW = CW;

  while(CW) 
  {
    CW->GetWindowText(TituloAtual);    

    if (TituloAtual.Find(Titulo) != -1) return CW;   

    MDINext();

      CW = MDIGetActive(&Maximizada);

    if(CW == PrimCW) CW = NULL;    //---  buffer circular.
  }

  return CW;
}

void CMainFrame::OnUpdateButFechar(CCmdUI* pCmdUI) 
{
}

void CMainFrame::OnUpdateDesenhosGreide(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateDesdxfprogeom(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);	
}

void CMainFrame::OnUpdateDesdxfgreide(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateDesdxfsecoes(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateRelatCurvasverticais(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateRelatProjetogeom(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateRelatProjetogeom33017(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateRelatTerrenoprimitivo(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateRelatVolumes(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateNotadeserviPavimenta(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateNotadeserviTerraplanagem(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateCalcLimpeza(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateCalcAreaTaludes(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1 );
}

void CMainFrame::OnUpdateDXFProjGeom(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateDEXGreide(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateDXFSecoes(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateDXFPerfGeo(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdatePerfilGeol(CCmdUI* pCmdUI) 
{
  #if defined (PACOTE_INTERMEDIARIO) || defined (PACOTE_AVANCADO)
    pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
  #else 
    pCmdUI->Enable(false);
  #endif
}

void CMainFrame::OnUpdateNSTerraplanagem(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateNSPavimentacao(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}
void CMainFrame::OnUpdateAreaTaludes(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

void CMainFrame::OnUpdateVolumesTipoSolo(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_RuasProjeto.GetCurSel() != -1);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  // TODO: Add your specialized code here and/or call the base class
 // cs.style &= ~(LONG)FWS_ADDTOTITLE;      
  return CMDIFrameWnd::PreCreateWindow(cs);
}

CDocument* CMainFrame::GetMDIActiveDocument()
{
    CDocument* pDoc(NULL);

    CWnd* pWndMain(AfxGetMainWnd());
    ASSERT(pWndMain);
    ASSERT(pWndMain->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd))); // Not an MDI app.

    CFrameWnd* pFrame(((CMDIFrameWnd*)pWndMain)->MDIGetActive());
    if(NULL != pFrame) pDoc = pFrame->GetActiveDocument(); // get the active document
  
    return pDoc;
}

void CMainFrame::OnFileSave()
{

  // não passa pelo diálogo de confirmação. tentar depois
 // CTypedPtrListView* pLsView(GetActiveView());
 //   if(pLsView) pLsView->OnBnClickedGravar();

   

 // GetMDIActiveDocument()->OnFileSave();
   // DoFileSave();
}

void CMainFrame::OnUpdateFileSave(CCmdUI *pCmdUI)
{
  bool Ligar(asdfg != IDD_DESPERFILGEOLOGICO && asdfg != IDD_DESSECOES && asdfg != IDD_DESENHARPERFIL && asdfg != IDD_DESPONTOSGEO && asdfg != 999 && asdfg != IDD_DESPERFISAVULSOS);
  if(!Ligar) 
  {
    pCmdUI->Enable(false);
    return;
  }

  pCmdUI->Enable(false);
   // não passa pelo diálogo de confirmação. tentar depois
 // CDocument* pDoc(GetMDIActiveDocument());
 // pCmdUI->Enable(pDoc && GetMDIActiveDocument()->IsModified());
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	if(nIDEvent == EVENT_CHAVE)
	{
		/*
	  if(EstadoLicenca == false) 
	  {
		  AfxMessageBox("abortando");
	  }
	  */
	}

	CMDIFrameWnd::OnTimer(nIDEvent);
}

bool CMainFrame::VerificaAberturaTarefa(CString& NomeProj)
{
  CFile ArqTeste;
  CFileException e;
  std::string NomeArquivo(NomeProj);
  NomeArquivo += ".tst";

  ArqTeste.Open(NomeArquivo.c_str(),CFile::modeCreate | CFile::modeWrite,&e);

  if(NomeProj.IsEmpty() || e.m_cause != CFileException::none)
  {
    TCHAR   szCause[255] = {0};
    e.GetErrorMessage(szCause,255);
    CString strMsg("Erro ao criar arquivos no diretório selecionado ");
    strMsg += " \r\nVocê tem permissão para usar arquivos nesta pasta?  (Chame o programa como administrador)";

    AfxMessageBox(strMsg.GetBuffer(),MB_ICONERROR);

    return false;
  }
  
  ArqTeste.Close();
  ArqTeste.Remove(NomeArquivo.c_str());

  return true;
}
  
CString CMainFrame::GetUserHomeDir()
{
   CString Retorno;

   TCHAR path[MAX_PATH];
   if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, path))) 
   {
     Retorno = path;
   }
   
   return Retorno;
}

int CMainFrame::CriaRua(const std::string& pDirNovaRua,const std::string& pNovaRua) 
{
  char bUF[512] = {0};
  CString HomeDir(GetUserHomeDir());

  if (VerificaAberturaTarefa(CString(pDirNovaRua.c_str())) == true)
  {
    CString NomeArqCab(pDirNovaRua.c_str());

    NomeArqCab += "\\" + CString(pNovaRua.c_str()) + ".cat";

    std::fstream ArqCabTrecho(NomeArqCab, std::fstream::out | std::fstream::trunc);

    ArqCabTrecho << pDirNovaRua;

    return 0;    //--- Retorno OK
  }

  return 1;     //--- Retorno NOK
}

void CMainFrame::OnProjetoRemoverrua()
{
  int C(0);

  std::string RuaRemover;

  CDiaRemoverRua DiaRemoverRua(NULL,CEstApp::LstRuas,RuaRemover);

   DiaRemoverRua.DoModal();
  {
    std::string NomeProj(PegaProjetoBase());

    if(RuaRemover.size())
    {
      CEstApp::LstRuas.remove(RuaRemover);   
      RemoveRua(RuaRemover);

      //--- Move os arquivo para o diretorio de backup e remove os arquivos do diretório original ====================

      std::string DirBackUp(NomeProj.substr(0,NomeProj.find_last_of('\\'))),Origem(DirBackUp+"\\" + RuaRemover + "\\*.*");
      DirBackUp.append("\\");
      DirBackUp.append("backup");
      int Res(_mkdir(DirBackUp.c_str()));
    
      DirBackUp.append("\\");
      DirBackUp.append(RuaRemover);
      Res = CreateDirectory(DirBackUp.c_str(),NULL);

      int C(0);

      while(!Res)
      {
        std::stringstream DirAtual;

        DirAtual << DirBackUp << "_";
        DirAtual << std::right << std::fixed << std::setfill('0') << std::setw(3);
        C++;
        DirAtual << C;

        Res = CreateDirectory(DirAtual.str().c_str(),NULL);
        if(Res) DirBackUp = DirAtual.str();
      }

      std::string Destino(DirBackUp);

      {
        std::stringstream Comando;

        Comando << "move " << "\"" << Origem << "\" " << "\"" << Destino << "\"" << std::ends;

        system(Comando.str().c_str());   //<<<<<<<<========= mudar para shell execute (não aparece a janela do dos)
      }
      {
        Origem = Origem.substr(0,Origem.find_last_of('\\')).c_str();

        std::stringstream Comando;

        Comando << "rmdir " << "\"" << Origem << "\"" << std::ends;

        system(Comando.str().c_str()); 

         // ShellExecute(NULL,"open","rmdir.exe",Comando.str().c_str(),"",SW_HIDE);  //--- Não esta funcionando verificar
      }
      
      if(C > 0)
      {
        std::stringstream Mensagem;
        Mensagem << "O diretório de backup já existe. Os Arquivos foram movidos para o diretório:\r\n" << Destino;

        AfxMessageBox(Mensagem.str().c_str());
      }

      if (DesPontosGeo != nullptr) ((DesPontosGeomView*)DesPontosGeo)->RemoveRua(RuaRemover);

      m_RuasProjeto.RedrawWindow();
    }
  }
}

void CMainFrame::OnUpdateProjetoRemoverrua(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_ProjetosBox.GetCount());
}

void CMainFrame::OnUpdateProjetoInserirrua(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_ProjetosBox.GetCount());
}

void CMainFrame::AnulaPerfil()
{
   ittyMapStrPairVoid it(MapSecoesTrecho.find(std::string(PegaRuaAtual())));
   if(it == MapSecoesTrecho.end()) return;

   it->second.second = NULL;

   if(it->second.first == NULL) MapSecoesTrecho.erase(it);
}

void CMainFrame::AnulaSecao()
{
  std::string Rua(PegaRuaAtual());

   ittyMapStrPairVoid it(MapSecoesTrecho.find(std::string(PegaRuaAtual())));
   if(it == MapSecoesTrecho.end()) return;
     
  // it->second.first = NULL;

  // if(it->second.second == NULL) MapSecoesTrecho.erase(it);
   MapSecoesTrecho.erase(it);
}

void CMainFrame::LeRuasProjeto()
{
  m_RuasProjeto.ResetContent();
  CEstApp::LeListaRuas();

  int C(0);

  for (ittlstwstr it = CEstApp::LstRuas.begin() ; it != CEstApp::LstRuas.end() ; it++,C++) 
  {
    m_RuasProjeto.InsertString(C,it->c_str());
  }

  SelecionaRua(RuaAtual.GetBuffer());
}

BOOL CMainFrame::DirectoryExists(LPCTSTR path)
{
  DWORD dwAttrib(GetFileAttributes(path));

  return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

BOOL CMainFrame::FileExists(LPCTSTR path)
{
  DWORD dwAttrib(GetFileAttributes(path));

  return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_ARCHIVE));
}

BOOL CMainFrame::RemoveArqProjeto(std::string& Extensao)
{
  std::string NomeArqTalv(std::string(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()));

  NomeArqTalv += "." + Extensao;     

  return _rmdir(NomeArqTalv.c_str());
}

void CMainFrame::OnMenuCalcProjTerrapl()
{
#ifdef DEMOVER
  ::MessageBox(NULL, "Função indisponível na cópia de demonstração", "VU&E", MB_OK);
#else
#ifndef _DEBUG
  EstadoLicenca = OVerfNeoKey.Verifica();

  if (EstadoLicenca == false)
  {
    AfxMessageBox("A chave de segurança não foi encontrada ou está vencida ou está inválida.");
    return;
  }
#endif

  if (EstadoLicenca == false)
  {
    AfxMessageBox("A chave de segurança não foi encontrada, está vencida ou está inválida.");
    return;
  }
  
  //--- Não esta mais calculando a superficie nos projetos do autocad  21112023

   if(false)  if(TipoSuperficie == 0 && SuperfCalculada == false)
  {
    AfxMessageBox("Para calcular a terraplanagem nos projetos importados do AutoCad " \
                  "a superficie deve estar calculada no modelo do Vias Urbanas & Estradas. Portanto " \
                  "solicite o cálculo da superfície antes de solicitar o cálculo da terraplanagem");
    return;
  }

  try
  {
    CString Projeto;

    m_ProjetosBox.GetLBText(0, Projeto);
    CDCalcTerraplProjeto DCalcTerrpProj(this, PegaProjetoAtual(),std::string(CString(Projeto).GetBuffer()));
    DCalcTerrpProj.DoModal();
  }
  catch (...)
  {
    AfxMessageBox("Houve erro no cálculo do projeto de terraplanagem.");
  }
#endif
}

void CMainFrame::OnUpdateMenuProjterrapl(CCmdUI *pCmdUI)
{
  #if defined (PACOTE_INTERMEDIARIO) || defined (PACOTE_AVANCADO)
    pCmdUI->Enable(m_RuasProjeto.GetCount() > 0 && !VerfMedicao() && !DesPerfil && /*!DesSecao &&*/ !DesPontosGeo);
  #else
    pCmdUI->Enable(false);
  #endif  
}

void CMainFrame::OnUpdateDesdxfproterrap(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_RuasProjeto.GetCount() > 0 && !VerfMedicao());
}

void CMainFrame::OnUpdateRelProjTerraplanagem(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_RuasProjeto.GetCount() > 0 && !VerfMedicao());
}
/*
void CMainFrame::OnImportarArqkml()
{
#ifdef DEMOVER
  ::MessageBox(NULL, "Função indisponível na cópia de demonstração", "VU&E", MB_OK);
#else
  #ifndef _DEBUG
    EstadoLicenca = OVerfNeoKey.Verifica();

    if (EstadoLicenca == false)
    {
      AfxMessageBox("A chave de segurança não foi encontrada ou está vencida ou está inválida.");
      return;
    }
  #endif

  CDiaImportarArqKmlKmz Dialogo(this, PegaProjetoBase());

  Dialogo.DoModal();
#endif
}

void CMainFrame::OnUpdateImportarArquivokml(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(m_ProjetosBox.GetCount() && this->DesPontosGeo == false);
}
*/
void CMainFrame::MudouRua(const std::string& RuaAtual)
{
  DesPontosGeomView* pGeomView((DesPontosGeomView*)DesPontosGeo);
  if(pGeomView) pGeomView->MudouRua(RuaAtual);
}

void CMainFrame::InsereRuaDesGeo(const std::string& RuaAtual)
{
  DesPontosGeomView* pGeomView((DesPontosGeomView*)DesPontosGeo);
  if (pGeomView) pGeomView->InsereRuaRecemCriada(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()).GetBuffer(),RuaAtual);
}

void CMainFrame::OnGeorefImportarPontosControle()
{
  CString filtro = "Pontos de controle (*.kml) | *.kml||", NomeProj,NomeArq;

  CFileDialog diaabrearq(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro, NULL);
  diaabrearq.m_ofn.lpstrTitle = "Vias Urbanas & Estradas - Georeferenciamento - Pontos de controle :";
  if (diaabrearq.DoModal() == IDOK)
  {
    NomeArq = diaabrearq.GetPathName();
    NomeArq.MakeUpper();
    if (!NomeArq.IsEmpty())
    {
    }
  }
}

void CMainFrame::OnGeorefImportarMapa()
{
  CString filtro = "Mapas (*.jpg) | *.jpg||", NomeArq;

  CFileDialog diaabrearq(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro, NULL);
  diaabrearq.m_ofn.lpstrTitle = "Vias Urbanas & Estradas - Georeferenciamento - Mapas :";
  if (diaabrearq.DoModal() == IDOK)
  {
    NomeArq = diaabrearq.GetPathName();
    NomeArq.MakeUpper();
    if (!NomeArq.IsEmpty())
    {
  //    NomeMapa = NomeArq;
    }
  }
}

void CMainFrame::OnDronesImportarnuvemdepontos()
{
  tylstUtm_Coord_3d LstPontosGeoRef;

  CString filtro("Drones- Nuvem Pontos (*.xyz) | *.xyz||"), NomeArq;

  CFileDialog diaabrearq(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro, NULL);
  diaabrearq.m_ofn.lpstrTitle = "Vias Urbanas & Estradas - Drones - Nuvem de Pontos:";
  if (diaabrearq.DoModal() == IDOK)
  {
    NomeArq = diaabrearq.GetPathName();
    NomeArq.MakeUpper();
    if (!NomeArq.IsEmpty())
    {
      CDiaImportarArqKmlKmz DialogoArqGoogle(this, ((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase(), true, &LstPontosGeoRef, 2);
      if (DialogoArqGoogle.DoModal() == IDCANCEL) return ;

    }
  }
}

void CMainFrame::InicializaCroquisDesGeo()
{
  DesPontosGeomView* pGeomView((DesPontosGeomView*)DesPontosGeo);
  if (pGeomView) pGeomView->InicializaCroquis();
}

int  CMainFrame::VerfInst(bool Inicial)
{
#ifdef DEMOVER
  return 1;
#endif
  char BXOR((0x01010101)), ByteAtual;

  char Mensg[] = "bnsstqude!ru`bjdXtux nT.C,q2Uq45Ps5dfnLrpla";

  std::string Cripto;

  for (int i = 0; i < 15; i++)
  {
    Cripto += Mensg[i] ^ BXOR;
  }

  CString DirWin(getenv("WinDir")),
          DirDll1(DirWin+"\\system32\\ConHTTPUv.dll"),
          DirDll2(DirWin+ "\\system32\\ConPonIn.dll");

  HINSTANCE hLib1(AfxLoadLibrary(DirDll1));
  HINSTANCE hLib2(AfxLoadLibrary(DirDll2));

   if (hLib1 == NULL || hLib2 == NULL)
  {
     if(hLib1 != NULL) FreeLibrary(hLib1);
     if(hLib2 != NULL) FreeLibrary(hLib2);

    AfxMessageBox(Cripto.c_str());
    return 0;
  }

  FreeLibrary(hLib1);
  FreeLibrary(hLib2);

  if(!Inicial) return 1; 

  ContaTentativas++;

  if (ContaTentativas >= 1)    //--- Tenta verificar a chave  a cada 30 chamadas
  {
    std::string Chave,Numcliente,Resposta;
    
    PegaChave(Chave,Numcliente);

   int Erro(VerificaChaveBoole());

    Erro = 1;
	
    switch (Erro)
    {
      case 1:                 //--- Chave OK
      {
        ContaTentativas = 0;
        return 1;
      }
      break;
      case 4:       //--- Chave vencida
      {
        return 0;
      }
      break;
      case 3:
      {
        AfxMessageBox("Erro na verificação da chave");
        return 0;
      }
      break;
      case 2:                //--- Sem conexão com a internet
      {
        if (ContaTentativas > 5)    //---(30 chamadas +) 20 tentativas sem conexão com a internet 
        {
          AfxMessageBox("Necessária a verificação da chave, conecte-se à internet");
          return 0;
        }

        return 1;   //--- Ok menos que 50 tentativas
      }
      break;
      case 0:                //--- Chave Invalida
      {
        AfxMessageBox("Chave Invalida"); 
        return 0;
      }
      break;
    }
  }

  return 1;   //--- OK ContaTentativas < 30
}

#include "CDiaDesinstalar.h"

void CMainFrame::OnDesinstalar()
{
  CDiaDesinstalar Desinstalar;

  Desinstalar.DoModal();
}

#include "HTTPRequests\HTTPRequests\CHTTPRequests.h"

int CMainFrame::VerificaChaveBoole()
{
  std::string Resp;
  std::stringstream url,url2;
  std::string Chave,NumCliente;
  
  if(PegaChave(Chave, NumCliente) == false) return false;

  if(Chave.size() < 20 || Chave.size() > 40 || NumCliente.size() !=6) return false;

  NumCliente.erase(0, NumCliente.find_first_not_of('0'));
  
  if(!SuperCString::ENumero(NumCliente.c_str())) return 0;

  url << std::right << std::fixed << std::setfill(' ') << std::setprecision(8);

  url << "http://148.72.64.68/webservice/1/"  << NumCliente << "/" << Chave;

  try
  {
    Resp = CHTTPRequests::EnviaRequisicaoPegaResposta(url.str());
  }
  catch(...)
  {
    AfxMessageBox("Erro 05");
    return 3;
  }

  if(Resp.size() == 0) return 2;  //--- Sem Internet

  std::string Status = PegaStatus(Resp);

  if (Status == "4")  //--- a chave esta instalada    
  {
    ContaTentativas = 0;
    url2.str() = "";
    url2.clear();

    //--- Verifica a validade da chave

    int C(0);

    do
    {
      url2 << "http://148.72.64.68/webservice/3/" << NumCliente << "/" << Chave;

      try
      {
        Resp = CHTTPRequests::EnviaRequisicaoPegaResposta(url2.str());
      }
      catch (...)
      {
        AfxMessageBox("Erro 06");
        return 3;
      }

      if (Resp.size() > 0)
      {
        Status = PegaStatus(Resp);

        if (Status == "1")
        {
          return 1; //---  Chave válida (dentro do prazo de validade)
        }
        else
        {
          AfxMessageBox("Chave bloqueada - Data de validade expirada");

          return 4;
        }
      }
    }while(Resp.size() == 0 && ++C < 5);
  }
  return 0;      //--- Erro de Chave 
}

std::string CMainFrame::PegaStatus(const std::string &Msg)
{
  size_t PosAtual(0), PosFinal(std::string::npos);
  std::string strTemp;

  PosAtual = Msg.find("codigo_resposta",0);

  if (PosAtual == std::string::npos) return strTemp ;

  PosAtual = Msg.find(':', ++PosAtual);

  if (PosAtual == std::string::npos) return strTemp;

  PosAtual++;

  PosFinal = Msg.find(',', PosAtual);

  if (PosFinal == std::string::npos) return strTemp;

  strTemp = Msg.substr(PosAtual, PosFinal - PosAtual);

  return strTemp;
}
int CMainFrame::PegaChave(std::string& pChave,std::string& pNumcliente)
{
  CFile ArqChave;

  pChave.clear();
  pNumcliente.clear();
  CString DirWin(getenv("WinDir")),
          NomeDLL(DirWin + "\\system32\\ConPonIn.dll");
  
  if(ArqChave.Open(NomeDLL,CFile::modeRead | CFile::typeBinary) == false) return 0;

  char Buffer[100];

  if(ArqChave.Read(Buffer,100) == 100)
  {
    ArqChave.Seek(1280,CFile::begin);

    if(ArqChave.Read(Buffer, 100) == 100)
    {
      int C(0);

      for ( ; C < 32 ; C++)
      {
        pChave += Buffer[C];
      }
      for( ; C < 38 ; C++)
      {
        pNumcliente += Buffer[C];
      }
    }
  }
  
  return 1;
}

BOOL CMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, LPCTSTR lpszMenuName, DWORD dwExStyle, CCreateContext* pContext)
{
  // TODO: Adicione seu código especializado aqui e/ou chame a classe base

  int Ret(CMDIFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext));

  if(Ret)
  {
    if(!VerfInst(true)) return FALSE;
  }

  return Ret;
}

void CMainFrame::OnUpdateProjImparqtopog(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(m_ProjetosBox.GetCount());
}

void CMainFrame::RemoveProjRecente(int Ret)
{
  if (Ret == 2 || Ret == 4)      //----- O projeto não existe mais
  {
    int i(0);
    bool Achou(false);

    if (I != -1)  //--- se a pilha não está vazia.
    {
      //--- Verifica se o nome já esta na pilha.

      for (int i = 0; i <= I && i < 50 && !Achou; i++)
        if (!m_strProjRec[i].Compare(RuaAtual)) Achou = TRUE;
    }

    if (!Achou)
    {
      CMenu* pMenu = GetMenu()->GetSubMenu(0);

      pMenu->RemoveMenu(IDENTIF[i], MF_BYCOMMAND);
      RemoveLista(i);
    }
  }
}
void CMainFrame::AbreProj(const std::string& pNomeProj,bool AbrirDesSupericie)
{
  VeioPagInicial = true;

  NomeProjPagInicial = pNomeProj;
  OnProjAbrir();
  if(AbrirDesSupericie) ((CEstApp*)AfxGetApp())->OnEditarPontosGeo();

  VeioPagInicial = false;
}

void CMainFrame::SalvaEstadoDiaInicial(WINDOWPLACEMENT* pwp,int NumTar)
{
  pwp->length = sizeof *pwp;

  ::WriteWindowPlacement(pwp,true);
}

void CMainFrame::LeEstadoDiaInicial(WINDOWPLACEMENT* pwp,int NumTar)
{
  pwp->length = sizeof(*pwp);

  ReadWindowPlacement(pwp,true);
}
