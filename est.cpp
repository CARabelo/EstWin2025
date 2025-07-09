// est.cpp : Defines the class behaviors for the application.
//#define CRTDBG_MAP_ALLOC
#include "stdafx.h"
#include <stdlib.h>
#include <crtdbg.h>
#include <winnetwk.h>
#include <list>
#include "cponto.h"
#include "est.h"
#include "direct.h"
#include <string>
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "xymouse.h"
#include "xygreide.h"
#include "ddeslizantes.h"
#include "deslizantesgeom.h"
#include "palette.h"
#include "mytoolbar.h"
#include "mat.h"
#include "estaca.h"
#include "comboprocdessecao.h"
#include "diapontosgeom.h"
#include "meleditbox.h"
#include "ChildFrm.h"
#include "afxtempl.h" 
#include <sstream>
#include <iomanip>
#include "estDoc.h"
#include "estView.h"
#include "monologo.h"
#include "arqnovo.h"
#include "dialogo.h"
#include "pedirproj.h"
#include "typlstvw.h"
#include "nivelview.h"
#include "Splash.h"
#include "supercstring.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "confdesmedicao.h"
#include <vector>
#include <iostream>
#include "csstack.h"
#include "clpontos.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "perfil.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include "csecao.h"
#include "clistaperfis.h"
#include "clistasecoes.h"
#include <set>
#include "carqcurhorizontais.h"
#include "ccurhorizontal.h"
#include "ccurhorizontais.h"
#include "carqcurverticais.h"
#include <strstream>
#include "ccurverticais.h"
#include "CRegAreaTaludes.h"
#include "CLimpeza.h"
#include "cdeltasuperficie.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include <map>
#include "CAcidentesTopograficos.h"
#include "CSegmento.h"
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "CEquipDrenagem.h"
#include "estrel.h"
#include "relatorios.h"
#include "Cadlib\Interface\VC\Cadlib.h"
#include "superficie.h"
#include "dgerardxf.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
#include "CxCotas.h"
#include "dgerardxfgreide.h"
#include "CSecaoAvulsa.h"
#include "CSecoesAvulsas.h"
#include "DGerarDXFSecoes.h"
#include "DGerarDXFPerfGeol.h"
#include "DGerarDXFTerraplanagem.h"
#include "imagelistbox.h"
#include "IconComboBox.h"
#include "CDiaInsereRua.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include "CArqAciTopograficos.h"
#include "CArqTrechos.h"
#include "CTrecho.h"
#include "CTrechos.h"
#include "CDiaThread.h"
#include "CTriangulo.h"
#include "CTriangulacao.h"
#include "CImportaDXFTopog.h"
#include "CCriaSuperfIgualTopog.h"
#include "CMListBox.h"
#include "CDInicial.h"
#include "CCriaSuperfSIG.h"
#include "DGerarDXFBaciasHidrog.h"

#define SUPERF_TOPOGRAFIA 4;
#define SUPERF_SIG 6

#ifndef _DEBUG
  #include "VerfNeoKey.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------Objetos Gerais -----------------------------------

class monologo monolog;        //--- Dialogo para monologos.
class arqnovo ArquivoNovo;     //--- Dialogo para abrir um arquivo;
class dialogo dialogar;        //--- Dialogo para dialogos//.
//class CProjeto* Projetos[10];  //--- Vetor de projetos abertos;
CCSStack VetorProjetos;        //--- Vetor de projetos abertos
Matem Mat;                     //--- Funcões matematicas (GMS, arredondamento, etc)

#ifndef _DEBUG
  VerfNeoKey OVerfNeoKey;
#endif
  
LPCTSTR  Ext[22] = {{"IGU"},{"TER"},{"SEC"},{"NIV"},{"VER"},{"HOR"},{"GRA"},
                    {"OAE"},{"TAC"},{"TAA"},{"ALS"},{"OAC"},{"PAV"},{"TPA"},
                    {"CAN"},{"LIM"},{"FIX"},{"SET"},{"SAV"},{"EMP"},{"TPS"},
				           	{"FSO"}};    //-- Extensões dos arquivos.

std::list<std::string> CEstApp::LstRuas;

/////////////////////////////////////////////////////////////////////////////
// CEstApp

BEGIN_MESSAGE_MAP(CEstApp, CWinApp)
  //{{AFX_MSG_MAP(CEstApp)
  ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
  ON_COMMAND(ID_ARQ_ABRIR, OnArqAbrir)
  ON_COMMAND(ID_ARQ_NOVO, OnArqNovo)
  ON_COMMAND(ID_FILE_MRU_FILE1, OnFileMruFile1)
  ON_COMMAND(ID_FILE_MRU_FILE2, OnFileMruFile2)
  ON_COMMAND(ID_FILE_MRU_FILE3, OnFileMruFile3)
  ON_COMMAND(ID_FILE_MRU_FILE4, OnFileMruFile4)
  ON_COMMAND(ID_FILE_MRU_FILE5, OnFileMruFile5)
  ON_COMMAND(ID_FILE_MRU_FILE7, OnFileMruFile7)
  ON_COMMAND(ID_FILE_MRU_FILE8, OnFileMruFile8)
  ON_COMMAND(ID_FILE_MRU_FILE10, OnFileMruFile10)
  ON_COMMAND(ID_FILE_MRU_FILE9, OnFileMruFile9)
  ON_COMMAND(ID_FILE_MRU_FILE6, OnFileMruFile6)
  ON_COMMAND(ID_PROCURAR, OnProcurar)
  ON_COMMAND(ID_DESENHOS_GREIDE, OnDesenhosGreide)
  ON_COMMAND(ID_DESENHOS_SECOES, OnDesenhosSecoes)
  ON_COMMAND(ID_MEDIR_CONFIGURAR, OnMedirConfigurar)
  ON_COMMAND(ID_EDITAR_PONTOS_GEO, OnEditarPontosGeo)
  ON_COMMAND(ID_DESDXFPROGEOM, OnDesdxfprogeom)
  ON_COMMAND(ID_DESDXBACIASHIDROG, OnDesdxfBaciasHidrog)
  ON_COMMAND(ID_DESDXFGREIDE, OnDesdxfgreide)
  ON_COMMAND(ID_DESDXFSECOES, OnDesdxfsecoes)
  ON_COMMAND(ID_DESDXFPERFGEOL, OnDesdxfperfgeol)
  ON_COMMAND(ID_DESDXFPROTERRAP, OnDesdxfprojterrap)
  ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
  ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
  ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
  ON_COMMAND(ID_RELAT_NOTADESERVI, OnRelatNotadeservi)
  ON_COMMAND(ID_RELAT_PROJETOGEOM, OnRelatProjetogeom)
  ON_COMMAND(ID_RELAT_PROJETOGEOM33017, OnRelatProjetogeomdet)
  ON_COMMAND(ID_RELAT_CURVASVERTICAIS, OnRelatCurvasverticais)
  ON_COMMAND(ID_NOTADESERVI_TERRAPLANAGEM, OnNotadeserviTerraplanagem)
  ON_COMMAND(ID_NOTADESERVI_PAVIMENTA, OnNotadeserviPavimenta)
  ON_COMMAND(ID_RELAT_TERRENOPRIMITIVO, OnRelatTerrenoprimitivo)
  ON_COMMAND(ID_RELAT_VOLUMES, OnRelatVolumes)
  ON_COMMAND(ID_RELAT_SECAVULSAS,OnRelatSecavulsas)
  ON_COMMAND(ID_RELAT_AREA_TALUDES,OnRelatAreaTaludes)
  ON_COMMAND(ID_RELAT_AREA_LIMPEZA,OnRelatAreaLimpeza)
  ON_COMMAND(ID_RELAT_VOL_TIPO_SOLO,OnRelatVolTipoSolo)
  ON_COMMAND(ID_HELP, &CEstApp::OnHelp)
#if defined (PACOTE_INTERMEDIARIO) || defined (PACOTE_AVANCADO)
  ON_COMMAND(ID_EDITAR_PERFIL_GEOLOGICO, &CEstApp::OnEditarPerfilGeologico)
#endif
  ON_COMMAND(ID_PROJETO_INSERIRRUA, &CEstApp::OnProjetoInserirRua)
  ON_COMMAND(ID_DESENHOS_PERFISAVULSOS, &CEstApp::OnDesenhosPerfisAvulsos)
#if defined (PACOTE_INTERMEDIARIO) || defined (PACOTE_AVANCADO)
  ON_COMMAND(ID_REL_PROJ_TERRAPLANAGEM, &CEstApp::OnRelatProjTerraplanagem)
#endif
  ON_COMMAND(ID_RELAT_BACIAS_HIDROGRAF, &CEstApp::OnRelatBaciasHidrograf)
  ON_COMMAND(IDD_REL_EQUIP_DRENAGEM, &CEstApp::OnRelatEquipDrenagem)
  ON_COMMAND(ID_PROJ_IMPARQTOPOG, &CEstApp::OnImpArqTopografia)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEstApp construction

CEstApp::CEstApp() : pDiaProgresso(NULL), pMainFrame(nullptr), ErroImportacaoArquivos(0)
{
  //setlocale(LC_ALL, "pt_BR.UTF-8");
  //setlocale(LC_ALL, "");   estraga tudo!!!!  verificar
  // TODO: add construction code here
  // Place all significant initialization in InitInstance
   #ifndef DEMOVER
  
   HINSTANCE hLib=LoadLibrary(_T("CalcNS.dll")); 
   if(hLib==NULL) AfxMessageBox("Não foi possivel ler a dll CalcNS.dll",IDC_OK);       
   hLib = AfxLoadLibrary(_T("ConSO.dll"));
   if (hLib == NULL) AfxMessageBox("Não foi possivel ler a dll ConSO.dll", IDC_OK);
   hLib = LoadLibrary(_T("CalcVL.dll")); 
   if(hLib==NULL) AfxMessageBox("Não foi possivel ler a dll CalcVL.dll",IDC_OK);       
   hLib = LoadLibrary(_T("CalcSU.dll")); 
   if(hLib==NULL) AfxMessageBox("Não foi possivel ler a CalcSU.dll",IDC_OK);      
   hLib = AfxLoadLibrary(_T("CadIO.dll"));
   if(hLib==NULL) AfxMessageBox("Não foi possivel ler a dll CadIO.dll",IDC_OK); 
   hLib = AfxLoadLibrary(_T("HTTPRequests.dll"));
   if (hLib == NULL) AfxMessageBox("Não foi possivel ler a dll HTTPRequests.dll", IDC_OK);
   hLib = AfxLoadLibrary(_T("SimpPL.dll"));
   if (hLib == NULL) AfxMessageBox("Não foi possivel ler a dll SimpPL.dll", IDC_OK);
   #endif
   #ifndef _DEBUG
     #ifndef DEMOVER
   else
   {
	   hLib = AfxLoadLibrary(_T("ClientAPI64.dll"));
     if(hLib==NULL) 
     {
       AfxMessageBox("Não foi possivel ler a dll ClientAPI64.dll",IDC_OK);   
       exit(0);
     }
  }
    #endif
  #endif



  //	for (int i = 0 ; i < 10 ; i++) Projetos[i] = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CEstApp object

CEstApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CEstApp initialization

BOOL CEstApp::InitInstance()
{
  // CG: The following block was added by the Splash Screen component.
  {
    CCommandLineInfo cmdInfo;

    ParseCommandLine(cmdInfo);

    CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);

  }
  // Standard initialization
  // If you are not using these features and wish to reduce the size
  //  of your final executable, you should remove from the following
  //  the specific initialization routines you do not need.

#ifdef _AFXDLL
//  Enable3dControls();			// Call this when using MFC in a shared DLL
#else
  Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

  LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

  // Passei MRU = 0 não esta ok por causa das reticencias.

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views.
  
  CMultiDocTemplate* pDocTemplate = new CMultiDocTemplate(
    IDR_ESTTYPE,
    RUNTIME_CLASS(CEstDoc),
    RUNTIME_CLASS(CChildFrame), // custom MDI child frame
    RUNTIME_CLASS(CTypedPtrListView));

  AddDocTemplate(pDocTemplate);

  pDocTemplate = new CMultiDocTemplate(
    IDR_ESTREL,
    RUNTIME_CLASS(CEstRel),
    RUNTIME_CLASS(CChildFrame), // custom MDI child frame
    RUNTIME_CLASS(CRelatorios));

  AddDocTemplate(pDocTemplate);
 
  // create main MDI Frame window

  CMainFrame* pMainFrame2(new CMainFrame);

  if (!pMainFrame2->LoadFrame(IDR_MAINFRAME)) return FALSE;
  m_pMainWnd = pMainFrame2;

  pMainFrame = pMainFrame2;

 // if(OVerfNeoKey.Verifica() == false) 
  {
  //  AfxMessageBox("A chave de segurança não foi encontrada");
  //  exit(0);
  }

//   AfxMessageBox("A chave de segurança está ok");


  // Parse command line for standard shell commands, DDE, file open
  CCommandLineInfo cmdInfo;
  cmdInfo.m_nShellCommand = cmdInfo.FileNothing;
  ParseCommandLine(cmdInfo);

  // Dispatch commands specified on the command line
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;

  // The main window has been initialized, so show and update it.
  pMainFrame2->ShowWindow(m_nCmdShow);
  pMainFrame2->UpdateWindow();

  //First free the string allocated by MFC at CWinApp startup.
  //The string is allocated before InitInstance is called.

  //Change the name of the help file.
  //The CWinApp destructor will free the memory.
  // free((void*)m_pszHelpFilePath);
  // m_pszHelpFilePath=_tcsdup(_T("\"C:\\Users\\Carlos A. Rabelo\\Documents\\Visual Studio 2010\\Projects\\EstWin\\hlp\\EstWin.chm\""));
 //  m_pszHelpFilePath=_tcsdup(_T("C:\\Users\\Carlos A. Rabelo\\Documents\\Visual Studio 2010\\Projects\\TesteHelp\\TesteHelp\\hlp\\TTYUI.hlp"));
 //   m_pszHelpFilePath=_tcsdup(_T("C:\\EstWinDoc.chm"));
  //  AfxGetApp()->SetHelpMode(afxWinHelp);
  //  AfxGetApp()->SetHelpMode(afxHTMLHelp);
 
#ifdef _DEBUG  
//  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
//  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
 // _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
 // _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  //--- Verifica o estado da chave
//#ifndef _DEBUG
  
  std::string ChID;

 //if (OVerfNeoKey.LeID(ChID))
  {
  //  if(OVerfNeoKe
   // if(ConectaSocket)

  }
  

  /*
  LeDadosChave
  ConectaSocket
  EnviaPergunta
  RecebeResposta

    if (Resposta !OK)
    {
      responde usuário
      aborta
    }
    */


//#endif

  CDiaThread* pDiaThread(new CDiaThread(NULL));      //--- Não é thread, é não modal
  pDiaThread->Create(IDD_DIATHREAD, AfxGetMainWnd());
  pDiaProgresso = pDiaThread;

  int TipoProjeto(-1);

  //--- EM DESENVOLVIMENTO

  AbrirArquivo(IDD_DIAINICIAL);

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();

  // Dialog Data
  //{{AFX_DATA(CAboutDlg)
  enum { IDD = IDD_ABOUTBOX };
  //}}AFX_DATA

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAboutDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:
  CImageList ImageList;
  //{{AFX_MSG(CAboutDlg)
  // No message handlers
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
//  CImageListBox ImageListBox;
  virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  //}}AFX_DATA_MAP
  //  DDX_Control(pDX, IDC_LISTTESTE, ImageListBox);
}

BOOL CAboutDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
  //{{AFX_MSG_MAP(CAboutDlg)
  // No message handlers
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CEstApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CEstApp commands

void CEstApp::OnArqAbrir() 
{
  AbrirArquivo(1);  //--- 1, Arquivo existente.
}

void CEstApp::OnArqNovo() 
{
  AbrirArquivo(0);  //--- 0, arquivo novo.
}

UINT CEstApp::PegaRecurso(int NumArq)
{
  switch (NumArq)
  {
    case 0 : return IDD_IGUALDADES; break;
    case 1 : return IDD_NIVELAMENTO; break;
    case 2 : return IDD_CURVASVERTICAIS; break;
    case 3 : return IDD_CURVASHORIZONTAIS; break;
    case 6 : 
    case 7 : return IDD_TALUDES; break;
    case 10: return IDD_OBRASARTECORRENTES;break;
    case 13:
    case 9 : return IDD_TRESPONTOS ; break;
    case 8 : return IDD_TALUDESPAVIM ; break;
    case 11: return IDD_OBRASARTEESPECIAIS ; break;
    case 14: 
    case 12: return IDD_CANTCEN; break;
    case 15: return IDD_ALASARPAS; break; 
    case 4:  return IDD_LIMPEZA;break;
    case 5 :
    case 19:
    case 16: return IDD_GRADIENTES; break; 
    case 17: return IDD_TERRTRIANGULADO ; break;
    case 18: return IDD_SECOESAVULSAS ; break;
    case 20: return IDD_TIPOSSOLOS; break;
    case 21: return IDD_FUROSSONDAGEM; break;
    case 22: return IDD_CADASTROACITOPOG; break;
    case 23: return IDD_RELACADACITOPOG;break;
    default : ASSERT(0) ; break;
  }
  return 0;
}

void CEstApp::AbrirArquivo(int Tipo)
{
  //--- Se for relatório, abreo relatório e retorna

  if (Tipo > IDD_INICIO_RELATORIOS && Tipo < IDD_FIM_RELATORIOS)
  {
    ((CMainFrame*)AfxGetMainWnd())->asdfg = Tipo;

    OnFileNew(); 

    return;
  }

  if (Tipo == IDD_DESSECOES || Tipo == IDD_DESENHARPERFIL || Tipo == IDD_DESPONTOSGEO || Tipo == IDD_DESPERFILGEOLOGICO ||Tipo == IDD_DESPERFISAVULSOS)
  {
    ((CMainFrame*)AfxGetMainWnd())->asdfg = Tipo;

    OnFileNew(); 

    return;
  }

  CString Dir,NomeArq,ext,
          EXT = "IGUNIVVERHORLIMPAVTACTAATPASECOACOAECANTERFIXALSGRASETSAVEMPTPSFSOATORAV";
  CFile   ArqNovo;

  CMainFrame *MainFrame((CMainFrame*)AfxGetMainWnd());

  if (MainFrame->VerfInst() == false) exit(0);

  if (Tipo != IDD_DIAINICIAL)
  {
    NomeArq = MainFrame->PegaProjetoAtual();

    if (!NomeArq.IsEmpty())
    {
      Dir = NomeArq.Left(NomeArq.ReverseFind('\\'));

      if (!Dir.IsEmpty())
      {
        _chdir(Dir);

        NomeArq = ArquivoNovo.Dialoga(Tipo, NomeArq);

        if (!NomeArq.IsEmpty())
        {
          ext = NomeArq.Right(NomeArq.GetLength() - NomeArq.ReverseFind('.') - 1);
          ext.MakeUpper();

          int NumArq(EXT.Find((LPCTSTR)ext));

          if (NumArq != -1)
          {
            //--- Se tipo = criar cria o arquivo apenas para poder usar 
            //--- OpenDocumentFile, nao usa onfilenew, porque as rotinas 
            //--- padrao do onfilenew seriam chamadas. 

            if (Tipo == 0)
            {
              ArqNovo.Open(NomeArq, CFile::modeCreate);
              ArqNovo.Close();
            }

            MainFrame->asdfg = PegaRecurso(NumArq / 3);
            CString NomeProjTmp(MainFrame->m_sNomeComp);
            MainFrame->m_sNomeComp = NomeArq;
            OpenDocumentFile(NomeArq);
            MainFrame->m_sNomeComp = NomeProjTmp;

            //--- Remove o arquivo porque se ele ficar aberto da erro se alguma outra tarefa tentar abri-lo também (tamanho == 0)

            if (Tipo == 0)  CFile::Remove(NomeArq);

          }
          else
            monolog.mensagem(4, NomeArq); //--- Erro, arquivo inválido.
        }
      }
    }
  }
  else
  {
    MainFrame->asdfg = IDD_DIAINICIAL;
    CString NomeProjTmp(MainFrame->m_sNomeComp);
    MainFrame->m_sNomeComp = NomeArq = "PáginaInicial.pin";
    OpenDocumentFile(NomeArq);
    MainFrame->m_sNomeComp = NomeProjTmp;
  }
}

void CEstApp::AbrirArquivoMRU(int Num) 
{
  CString Nome,NomeArq,extarq;
  CString Dir,EXT("IGUNIVVERHORLIMPAVTACTAATPASECOACOAECANTERFIXALSGRASETSAVEMPRAV"),EXT2("DESREL");

  //--- Pega o string no menu.

  CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();
  CMenu *pMenu = MainFrame->GetMenu()->GetSubMenu(1);
  pMenu->GetMenuString(9+Num,Nome,MF_BYPOSITION);

  if (MainFrame->VerfInst() == false) exit(0);

  //--- Salva o nome completo e limpa a extensão. 

  Nome = NomeArq = Nome.Right(Nome.GetLength() - Nome.ReverseFind(' ') - 1);
  Nome = Nome.Left(Nome.Find('.'));
  Nome.MakeUpper();

  //--- Pega a extensão para poder pegar o número da tarefa no pegarecurso  

  extarq = NomeArq.Right(NomeArq.GetLength() - NomeArq.ReverseFind('.') -1);
  extarq.MakeUpper();

  //--- Se o projeto ainda não estiver aberto, abre o projeto, 
  //--- inserindo-o na lista de projetos abertos.

  if (MainFrame->m_ProjetosBox.FindString(0,Nome) == LB_ERR) 
  {
  //  MainFrame->m_ProjetosBox.InsertString(0,Nome);
  //  MainFrame->m_ProjetosBox.SetCurSel(0);
  }

  int NumArq( EXT.Find((LPCTSTR) extarq)); 

  if (NumArq != -1) 
  {
    MainFrame->asdfg = PegaRecurso(NumArq / 3);

    //--- Salva o nome completo

    MainFrame->m_sNomeComp = NomeArq;

    //--- Finalmente abre o arquivo.

    OpenDocumentFile(NomeArq); 
  }
  else 
  {
    NumArq = EXT2.Find((LPCTSTR) extarq); 
    
    if(NumArq!= -1)
    {
      NumArq /= 3;

      switch (NumArq)
      {
        case 0:    
        {
          if(Nome == "SECOES") OnDesenhosSecoes(); 
          else if(Nome == "GEOMETRICO") OnDesenhosGreide();
                 else if(Nome == "PERFIL") OnDesenhosGreide();
        }
        break;
        case 1:
        {
          if(Nome == "VOLUMESTERRAPL") OnRelatVolumes();
          else if(Nome == "TERRENOPRIMITIVO") OnRelatTerrenoprimitivo();
               else if(Nome == "NOTASERVICOTERRA") OnRelatNotadeservi();
                    else if(Nome == "PROJETOGEOM") OnRelatProjetogeom();
                         else if(Nome == "PROJGEOMDET") OnRelatProjetogeomdet();
                              else if(Nome == "CURVASVERTICAIS") OnRelatCurvasverticais();
                                   else if(Nome == "NOTASERVTERRA") OnNotadeserviTerraplanagem(); 
                                        else if(Nome == "NOTASERVPAVIM") OnNotadeserviPavimenta(); 
                                             else if(Nome == "TERRENOPRIMITIVO") OnRelatTerrenoprimitivo(); 
                                                   else if(Nome == "SECOEAVULSAS") OnRelatSecavulsas();
                                                         else if(Nome == "AREATALUDES") OnRelatAreaTaludes();
                                                               else if(Nome == "AREALIMPEZA") OnRelatAreaLimpeza();
                                                                    else if (Nome == "PROJTERRAP") OnRelatProjTerraplanagem();
        }
        break;                              
        default:  monolog.mensagem(4,NomeArq); //--- Erro, arquivo inválido.
      }
    }
  }
}

void CEstApp::OnProcurar() 
{
  CString ext,nomearq,Nome,filtro;
  class PedirProj PedirProjeto;
  CString Dir,EXT("IGUNIVVERHORLIMPAVTACTAATPASECOACOAECANTERFIXALSGRASAVRAV");

  if (PedirProjeto.DoModal() == IDOK) nomearq = PedirProjeto.Projeto;
  if (nomearq.IsEmpty()) nomearq = "*";

  filtro  = "Todos os arquivos de dados - "+ nomearq + " | " + nomearq + ".igu; " + nomearq +".niv; ";
  filtro += nomearq + ".ver; " + nomearq + ".hor; " + nomearq + ".lim; " + nomearq + ".pav; ";
  filtro += nomearq + ".tco; " + nomearq + ".taa; " + nomearq + ".tpa; " + nomearq + ".sec; ";
  filtro += nomearq + ".oac; " + nomearq + ".oae; " + nomearq + ".can; " + nomearq + ".tpo; ";
  filtro += nomearq + ".fix; | ";
  filtro += "Igualdades ( " + nomearq + ".igu) | " + nomearq + ".igu; |"; 
  filtro += "Nivelamento ( " + nomearq + ".niv) | " + nomearq + ".niv; |"; 
  filtro += "Curvas verticais ( " + nomearq + ".ver) | " + nomearq + ".ver; |"; 
  filtro += "Curvas Horizontais ( " + nomearq + ".hor) | " + nomearq + ".hor; |"; 
  filtro += "Limpeza ( " + nomearq + ".lim) | " + nomearq + ".lim; |"; 
  filtro += "Espessuras do pavimento ( " + nomearq + ".pav) | " + nomearq + ".pav; |"; 
  filtro += "Taludes de corte ( " + nomearq + ".tco) | " + nomearq + ".tco; |"; 
  filtro += "Taludes de aterro( " + nomearq + ".taa) | " + nomearq + ".taa; |"; 
  filtro += "Taludes do pavimento ( " + nomearq + ".tpa) | " + nomearq + ".tpa; |"; 
  filtro += "Seções tipo do pavimento ( " + nomearq + ".sec) | " + nomearq + ".sec; |"; 
  filtro += "Espessuras do pavimento ( " + nomearq + ".pav) | " + nomearq + ".pav; |"; 
  filtro += "O.A.C ( " + nomearq + ".oac) | " + nomearq + ".oac; |"; 
  filtro += "O.A.E ( " + nomearq + ".oae) | " + nomearq + ".oae; |"; 
  filtro += "Canteiros centrais ( " + nomearq + ".can) | " + nomearq + ".can; |"; 
  filtro += "Terreno por pontos ( " + nomearq + ".tpo) | " + nomearq + ".tpo; |"; 
  filtro += "O.A.C ( " + nomearq + ".oac) | " + nomearq + ".oac; |"; 
  filtro += "Pistas fixas ( " + nomearq + ".fix) | " + nomearq + ".fix; ||";
  //filtro += "Seções Avulsas ( " + nomearq + ".pea) | " + nomearq + ".pea";
  filtro += "Empolamentos ( " + nomearq + ".emp) | " + nomearq + ".emp";

  CFileDialog diaabrearq(TRUE,NULL,NULL,OFN_EXPLORER | OFN_FILEMUSTEXIST,filtro,NULL);
  diaabrearq.m_ofn.lpstrTitle = "Vias Urbanas & Estradas - Abrir arquivo :";
  if (diaabrearq.DoModal() == IDOK)
  {
    nomearq = diaabrearq.GetPathName();
    nomearq.MakeUpper();
    if (!nomearq.IsEmpty())
    {
      ext = diaabrearq.GetFileExt();
      ext.MakeUpper();
      if (!(ext == "IGU" || ext == "NIV" || ext == "VER" || ext == "HOR" || ext == "LIM" ||
        ext == "PAV" || ext == "TCO" || ext == "TAA" || ext == "TPA" || ext == "SEC" ||
        ext == "OAC" || ext == "OAE" || ext == "CAN" || ext == "TPO" || ext == "FIX" ||
        ext == "SET" || ext == "SAV" || ext == "EMP" ))
        monolog.mensagem(4,nomearq); //--- Erro, arquivo inválido.
      else
      {
        Nome = nomearq.Left(nomearq.Find('.'));      //--- Retira a extensão.

        //--- Se o projeto ainda não estiver aberto, abre o projeto, 
        //--- inserindo-o na lista de projetos abertos.

        CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();
        if (MainFrame->m_ProjetosBox.FindString(0,Nome) == LB_ERR) 
        {
          MainFrame->m_ProjetosBox.InsertString(0,Nome);
          MainFrame->m_ProjetosBox.SetCurSel(0);
        }

        //--- Finalmente abre o arquivo.

        MainFrame->m_sNomeComp = nomearq;
        MainFrame->asdfg = PegaRecurso( EXT.Find((LPCTSTR) ext) / 3);
        OpenDocumentFile(nomearq); 
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Arquivos recentes (MRU)

void CEstApp::OnFileMruFile1() 
{
  AbrirArquivoMRU(1);
}

void CEstApp::OnFileMruFile2() 
{
  AbrirArquivoMRU(2);
}

void CEstApp::OnFileMruFile3() 
{
  AbrirArquivoMRU(3);
}

void CEstApp::OnFileMruFile4() 
{
  AbrirArquivoMRU(4);
}

void CEstApp::OnFileMruFile5() 
{
  AbrirArquivoMRU(5);
}

void CEstApp::OnFileMruFile6() 
{
  AbrirArquivoMRU(6);
}

void CEstApp::OnFileMruFile7() 
{
  AbrirArquivoMRU(7);
}

void CEstApp::OnFileMruFile8() 
{
  AbrirArquivoMRU(8);
}

void CEstApp::OnFileMruFile9() 
{
  AbrirArquivoMRU(9);
}

void CEstApp::OnFileMruFile10() 
{
  AbrirArquivoMRU(10);
}

//----------------------------------------------------------------------------

BOOL CEstApp::PreTranslateMessage(MSG* pMsg)
{
  // CG: The following lines were added by the Splash Screen component.

 // if (CSplashWnd::PreTranslateAppMessage(pMsg))	return TRUE;

  return CWinApp::PreTranslateMessage(pMsg);
}

void CEstApp::OnDesenhosGreide() 
{
  CMainFrame* MainFrame((CMainFrame*) AfxGetMainWnd());

  #ifndef _DEBUG
    #ifndef DEMOVER
     MainFrame->EstadoLicenca = OVerfNeoKey.Verifica(); 

    if(MainFrame->EstadoLicenca == false) 
    {
      AfxMessageBox("A chave de segurança não foi encontrada, está vencida ou está inválida.");
      return;
    }

    if(MainFrame->VerfInst() == false) return;

    #endif
  #endif

  if (MainFrame->VerfInst() == false) exit(0);

  CString NomeProjetoBase(CString(MainFrame->PegaProjetoBase())),
          NomeProjeto(CString(MainFrame->PegaProjetoAtual())),
          NomeRuaAtual(CString(MainFrame->PegaRuaAtual()));
 
  if (!NomeProjeto.IsEmpty()) 
  {
	  CString Nom = NomeProjeto.Left(NomeProjeto.GetLength()) + ".tca";
	  UINT Flags( CFile::modeRead);    // | /*CFile::shareDenyNone |*/ (Tipo != LER ? CFile::modeWrite : 0)); 
    
    CFile Arq;
		CFileException e;

		if (Arq.Open(Nom,Flags,&e) == 0)
		{
			monolog.mensagem(14,CString(" Terreno calculado : ") +Nom,e.m_cause);
            AfxMessageBox("Este trecho não foi seccionado, primeiro seccione o trecho para depois visualizar o perfil longitudinal.");

			return;
		}

    Arq.Close();
  }

  MainFrame->GreideAtivo = true;
  MainFrame->EMedicao = MainFrame->VerfMedicao();

  MainFrame->LigaSilencioso();
  if(!MainFrame->PegaSecoes())
  {
    MainFrame->asdfg = IDD_DESSECOES; 
    CString NomeArquivo(MainFrame->PegaProjetoAtual()+CString("\\Secoes.des"));

    OpenDocumentFile(NomeArquivo,false);  //--- Esta mudando o projeto....????

    MainFrame->MudouProjeto(NomeProjetoBase.GetBuffer());
    MainFrame->SelecionaRua(NomeRuaAtual.GetBuffer());

    CWnd* pDesSecaoView((CWnd*)MainFrame->PegaDesenhoSecoes());
    if(pDesSecaoView) pDesSecaoView->BringWindowToTop();
  }

  MainFrame->asdfg=IDD_DESENHARPERFIL; 
  CString NomeArquivo(NomeProjeto+CString("\\Perfil.des"));

  OpenDocumentFile(NomeArquivo,false); 

  MainFrame->DesligaSilencioso();
}

void CEstApp::OnDesenhosSecoes() 
{
  CMainFrame* MainFrame((CMainFrame*) AfxGetMainWnd());

  #ifndef _DEBUG  
    #ifndef DEMOVER
    MainFrame->EstadoLicenca = OVerfNeoKey.Verifica(); 

    if(MainFrame->EstadoLicenca == false) 
    {
      AfxMessageBox("A chave de segurança não foi encontrada, está vencida ou está inválida.");
      return;
    }
    #endif
  #endif

  if (MainFrame->VerfInst() == false) exit(0);

  if(MainFrame->EstadoLicenca == false) 
  {
    AfxMessageBox("A chave de segurança não foi encontrada está vencida ou está inválida.");
    return;
  }

  CString NomeProjeto(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()));
 
  if (!NomeProjeto.IsEmpty()) 
  {
	  CString Nom = NomeProjeto.Left(NomeProjeto.GetLength()) + ".sec";
	  UINT Flags( CFile::modeRead);    // | /*CFile::shareDenyNone |*/ (Tipo != LER ? CFile::modeWrite : 0)); 
    
    CFile Arq;
		CFileException e;

    MainFrame->LigaSilencioso();    //-- sem erros de faltando arquivo agora tem talvegues e seções avulsas

    if(0)     //--- agora tem talvegues e seções avulsas
    {
		  if (Arq.Open(Nom,Flags,&e) == 0)
		  {
			  monolog.mensagem(14,CString(" Seções : ") +Nom,e.m_cause);
			  return;
	   	}

      Arq.Close();
    }
  }

  MainFrame->GreideAtivo = false;
  MainFrame->EMedicao = ((CMainFrame*)AfxGetMainWnd())->VerfMedicao();

  MainFrame->asdfg=IDD_DESSECOES; 

  CString NomeArquivo(MainFrame->PegaProjetoAtual()+CString("\\Secoes.des"));

  OpenDocumentFile(NomeArquivo,false); 
}

void CEstApp::OnMedirConfigurar() 
{
  CConfDesMedicao ConfDesMedicao;

  ConfDesMedicao.DoModal();
}

void CEstApp::OnEditarPontosGeo()
{
  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());

#ifndef _DEBUG
#ifndef DEMOVER
  MainFrame->EstadoLicenca = OVerfNeoKey.Verifica();

  if (MainFrame->EstadoLicenca == false)
  {
    AfxMessageBox("A chave de segurança não foi encontrada, está vencida ou está inválida.");
    return;
  }
#endif
#endif

  if (MainFrame->VerfInst() == false) exit(0);

  MainFrame->LigaSilencioso();

  //--- dialogo de progresso

 // ((CDiaThread*)pDiaProgresso)->ShowWindow(SW_SHOW);
  ((CDiaThread*)pDiaProgresso)->AtualizaDialogo(std::string(""), 0);

  CTrechos Trechos(CString(MainFrame->PegaProjetoBase()).GetBuffer(), true);
  Trechos.TrocaTrecho(MainFrame->PegaRuaAtual());

  if (Trechos.MapaTrechos.size() > 0)
  {
    if (Trechos.PegaTrechoAtual() != Trechos.MapaTrechos.end())
    {
      CString NomeProjeto(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()));

      MainFrame->GreideAtivo = false;
      MainFrame->EMedicao = ((CMainFrame*)AfxGetMainWnd())->VerfMedicao();
      MainFrame->asdfg = IDD_DESPONTOSGEO;
      MainFrame->LigaSilencioso();

      //--- VS12 não precisa mais ===>Cria um arq temporario, so para não dar erro no OpenDocumentFile que agora( VS 10) exige que o arquivo exista
      //--- A extensão tem q ser .des, para chamar as rotinas certas do VU&E

      CString NomeArquivo(MainFrame->PegaProjetoAtual());

      NomeArquivo = MainFrame->PegaProjetoBase();
      NomeArquivo = NomeArquivo.Left(NomeArquivo.ReverseFind('\\'));
      NomeArquivo += "\\Geometrico.des";

      ((CDiaThread*)pDiaProgresso)->ShowWindow(SW_SHOW);
      ((CDiaThread*)pDiaProgresso)->AtualizaDialogo(std::string("Abrindo os Arquivos..."), 25);
      OpenDocumentFile(NomeArquivo, false);
      ((CDiaThread*)pDiaProgresso)->AtualizaDialogo(std::string("0"), 50);

      //--- Janela do croquis -----------------------------------------------------------
      //--- Esta atrapalhando as mensagens de on command update ui ---> averiguar
      /*
      MainFrame->asdfg = IDD_TAREFACROQUISGEO;
      NomeArquivo = MainFrame->PegaProjetoAtual();
      NomeArquivo = NomeArquivo.Left(NomeArquivo.ReverseFind('\\'));
      NomeArquivo += "\\Croquis.des";

      OpenDocumentFile(NomeArquivo, false);

      CWnd* pDesCroquisView((CWnd*)MainFrame->PegaDesenhoCroquis());
      if (pDesCroquisView)
      {
        pDesCroquisView->BringWindowToTop();
        pDesCroquisView->GetParentFrame()->ShowWindow(SW_HIDE);
      }

      MainFrame->InicializaCroquisDesGeo();
      */


      //---------------------------------------------------------------------------------

      char Buffer[64] = { 0 };

      ::GetPrivateProfileString(_T("DadosDesenhos"), _T("DesPontosComSecoes"), _T("0"), Buffer, 63, NULL);//,(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual() + std::string(".ini")).c_str());		

      if (atoi(Buffer) == 1 && !MainFrame->PegaDesenhoSecoes())
      {
        MainFrame->asdfg = IDD_DESSECOES;

        CString NomeArquivo(MainFrame->PegaProjetoAtual());

        NomeArquivo = NomeArquivo.Left(NomeArquivo.ReverseFind('\\'));
        NomeArquivo += "\\Secoes.des";
        OpenDocumentFile(NomeArquivo);

        ((CDiaThread*)pDiaProgresso)->AtualizaDialogo(std::string("0"), 75);
      }

      ((CDiaThread*)pDiaProgresso)->ShowWindow(SW_HIDE);
    }
    else
    {
      ((CDiaThread*)pDiaProgresso)->ShowWindow(SW_HIDE);
      AfxMessageBox("Há um problema na estrutura ou nos arquivos deste projeto, um ou mais trechos não foram encontrados, verifique, os arquivos do trecho atual não foram encontrados.");
    }
  }
  else
  {
    ((CDiaThread*)pDiaProgresso)->ShowWindow(SW_HIDE);
    AfxMessageBox("Não há trechos neste projeto, não há nada para ser desenhado.");
  }

  MainFrame->DesligaSilencioso();
}

void CEstApp::OnRelatNotadeservi()
{ 
  ((CMainFrame*)AfxGetMainWnd())->asdfg=IDD_REL_NOTA_SERVICO_TERRAPLANAGEM; 
  AbreArqCriaDoc("NotaServicoTerra.rel",false);
}

void CEstApp::OnRelatProjetogeom()
{
  ((CMainFrame*)AfxGetMainWnd())->asdfg=IDD_REL_PROJ_GEOMETRICO; 
    AbreArqCriaDoc("\\ProjetoGeom.rel",false);
}

void CEstApp::OnRelatProjetogeomdet()
{
  ((CMainFrame*)AfxGetMainWnd())->asdfg=IDD_REL_PROJ_GEOMETRICO_DETALHADO; 
    AbreArqCriaDoc("\\ProjGeomDet.rel",false);
}

void CEstApp::OnRelatCurvasverticais()
{
  ((CMainFrame*)AfxGetMainWnd())->asdfg=IDD_REL_PROJ_GREIDE; 
  AbreArqCriaDoc("\\CurvasVerticais.rel",false);
}

void CEstApp::OnNotadeserviTerraplanagem()
{
  ((CMainFrame*) AfxGetMainWnd())->asdfg=IDD_REL_NOTA_SERVICO_TERRAPLANAGEM;
  AbreArqCriaDoc("\\NotaServicoTerra.rel",false);
}

void CEstApp::OnNotadeserviPavimenta()
{
   ((CMainFrame*) AfxGetMainWnd())->asdfg=IDD_REL_NOTA_SERVICO_PAVIM; 
   AbreArqCriaDoc("\\NotaServPavim.rel",false);
}

void CEstApp::OnRelatTerrenoprimitivo()
{
  ((CMainFrame*)AfxGetMainWnd())->asdfg=IDD_REL_TERRENO_PRIMITIVO; 
   AbreArqCriaDoc("\\TerrenoPrimitivo.rel",false);
}

void CEstApp::OnRelatVolumes()
{
  ((CMainFrame*)AfxGetMainWnd())->asdfg=IDD_REL_VOLUMES_TERRAPLANAGEM; 
  AbreArqCriaDoc("\\VolumesTerrapl.rel",false);
}

void CEstApp::OnRelatSecavulsas()
{
  ((CMainFrame*)AfxGetMainWnd())->asdfg=IDD_REL_SECOES_AVULSAS; 
  AbreArqCriaDoc("\\SecoeAvulsas.rel",false);
}

void CEstApp::OnRelatAreaTaludes()
{
  ((CMainFrame*)AfxGetMainWnd())->asdfg=IDD_REL_AREA_TALUDES; 
  AbreArqCriaDoc("\\AreaTaludes.rel",false);
}

void CEstApp::OnRelatAreaLimpeza()
{
  ((CMainFrame*)AfxGetMainWnd())->asdfg=IDD_REL_AREA_LIMPEZA; 
  AbreArqCriaDoc("\\AreaLimpeza.rel",false);
}

void CEstApp::OnRelatVolTipoSolo()
{
  ((CMainFrame*)AfxGetMainWnd())->asdfg=IDD_REL_VOLUMES_TIPO_SOLO; 
  AbreArqCriaDoc("\\VolumeTipoSolo.rel",false);
}

void CEstApp::OnRelatProjTerraplanagem()
{
  ((CMainFrame*)AfxGetMainWnd())->asdfg = IDD_REL_PROJ_TERRAPLANAGEM;
  AbreArqCriaDoc("\\TerrenoPrimitivo.rel", false);
}

void CEstApp::OnDesdxfprogeom() 
{
    //--- Pega os dados do mapa
    
  CMainFrame* MF(((CMainFrame*)(CMainFrame*)AfxGetMainWnd()));
  std::string NomeArquivo(MF->PegaProjetoBase() + std::string(".ini"));
  char Buffer[1024] = { 0 };
  CString Default;

  Default.Format("%lf,%lf,%lf,%i,%lf,%lf,%lf,%s", 0.0, 0.0, 1.0, 0, 1.0, 0.0, 0.0, "");
  ::GetPrivateProfileString(_T("DadosDesenhos"), MF->DesPerfil ? _T("DadosDesPontosGeo") : _T("DadosMapa"), Default, Buffer, 511, NomeArquivo.c_str());
  std::stringstream strstrBuffer(Buffer);
  char Virgula;
  double DeltasMapa[2], m_nRatio, m_nX, m_nY, RazaoEscalasJPGDes;
  int ProjetoGeoreferenciado;

  strstrBuffer >> DeltasMapa[0] >> Virgula >> DeltasMapa[1] >> Virgula >> RazaoEscalasJPGDes >> Virgula >> ProjetoGeoreferenciado >> Virgula
               >> m_nRatio >> Virgula >> m_nX >> Virgula >> m_nY >> Virgula;

  char BufferNome[1024] = { 0 };

  strstrBuffer.getline(BufferNome, 1023);

  std::string NomeMapa = BufferNome;

  CDGerarDXF DGerarDXF(NULL, CString(MF->PegaProjetoAtual()), CString(MF->PegaProjetoBase()));

  DGerarDXF.DoModal();
}

void CEstApp::OnDesdxfBaciasHidrog()
{
  CMainFrame* MF(((CMainFrame*)(CMainFrame*)AfxGetMainWnd()));

  CDGerarDXFBaciaHidrog DGerarDXFBaciasHidrog(NULL, CString(MF->PegaProjetoAtual()), CString(MF->PegaProjetoBase()));

 DGerarDXFBaciasHidrog.DoModal();
}

void CEstApp::OnDesdxfprojterrap()
{
  CMainFrame* MF(((CMainFrame*)(CMainFrame*)AfxGetMainWnd()));

  CDGerarDXFTerrapl DGerarDXFTerrap(NULL, CString(MF->PegaProjetoAtual()), CString(MF->PegaProjetoBase()));

  DGerarDXFTerrap.DoModal();
}

void CEstApp::OnDesdxfgreide() 
{
  CDGerarDXFGreide DGerarDXFGreide(NULL,CString(((CMainFrame*)(CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()));

  DGerarDXFGreide.DoModal();
}

void CEstApp::OnDesdxfsecoes() 
{
  CDGerarDXFSecoes DGerarDXFSecoes(NULL,CString(((CMainFrame*)(CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()));

  DGerarDXFSecoes.DoModal();
}

void CEstApp::OnDesdxfperfgeol() 
{
  CMainFrame* MainFrame((CMainFrame*) AfxGetMainWnd());
  CString NomeProjeto(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()));

  if (!NomeProjeto.IsEmpty()) 
  {
	  CString Nom = NomeProjeto.Left(NomeProjeto.GetLength()) + ".fso";
	  UINT Flags(CFile::modeRead); 
    
    CFile Arq;
	  CFileException e;

	  if (Arq.Open(Nom,CFile::modeRead,&e) == 0)
	  {
	    monolog.mensagem(14,CString(" Furos de sondagem : ") +Nom,e.m_cause);
		return;
	  }

    Arq.Close();
  }
  else return;

  CDGerarDXFPerfGeol DGerarDXFPerfGeol(NULL,CString(((CMainFrame*)(CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()));

  DGerarDXFPerfGeol.DoModal();
}

void CEstApp::AbreArqCriaDoc(char* pNomeArq,bool Remover,bool ELog)
{
  CMainFrame* MainFrame((CMainFrame*) AfxGetMainWnd());

  CString NomeArquivo;

  if(ELog)  
  {
    std::string NomLog(pNomeArq);

    if (NomLog == "_ProjTerraplanagem.rel")NomeArquivo = MainFrame->PegaProjetoAtual();
    else NomeArquivo = MainFrame->PegaProjetoBase();
  }
  else  
  {
    NomeArquivo = MainFrame->PegaProjetoAtual();
    NomeArquivo = NomeArquivo.Left( NomeArquivo.ReverseFind('\\'));
  }

  NomeArquivo += pNomeArq;

  //--- Cria um arq temporario, so para não dar erro no OpenDocumentFile que agora( VS 10) exige que o arquivo exista
  //--- A extensão tem q ser .des, para chamar as rotinas certas do VU&E
  if(!ELog)
  {
    CFile ArqNovo;
    ArqNovo.Open(NomeArquivo,CFile::modeCreate|CFile::shareDenyNone);
    ArqNovo.Close();
  }
  OpenDocumentFile(NomeArquivo); 
  if(Remover) CFile::Remove(NomeArquivo);
}

void CEstApp::ChamaHelp(std::string Item)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi) );
    
    CHAR LComando[512] = "\"hh.exe\" hlp\\EstWinDoc.chm";
   
    if(Item.size() > 0) 
    {
      Item += ".htm";
      strcat(LComando,"::/");
      strcat(LComando,Item.c_str());
     }

    LPTSTR szCmdline = _tcsdup(_T(LComando));

    // Start the child process. 

    if(!CreateProcess( NULL,           // No module name (use command line)
                       szCmdline,      // Command line
                       NULL,           // Process handle not inheritable
                       NULL,           // Thread handle not inheritable
                       FALSE,          // Set handle inheritance to FALSE
                       0,              // No creation flags
                       NULL,           // Use parent's environment block
                       NULL,           // Use parent's starting directory 
                       &si,            // Pointer to STARTUPINFO structure
                       &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return;
    }

    // Wait until child process exits.
 //   WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}

void CEstApp::OnHelp()
{
    ChamaHelp();
}

CEstApp::~CEstApp()
{
 //delete (CDiaThread*)pDiaProgresso;
}

void CEstApp::OnEditarPerfilGeologico()
{
  CMainFrame* MainFrame((CMainFrame*) AfxGetMainWnd());
  CString NomeProjeto(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()));

  if (!NomeProjeto.IsEmpty()) 
  {
	  CString Nom = NomeProjeto.Left(NomeProjeto.GetLength()) + ".fso";
	  UINT Flags(CFile::modeRead); 
    
    CFile Arq;
	  CFileException e;

	  if (Arq.Open(Nom,CFile::modeRead,&e) == 0)
	  {
	    monolog.mensagem(14,CString(" Furos de sondagem : ") +Nom,e.m_cause);
	    return;
	  }

    Arq.Close();
  }
  else return;

  MainFrame->GreideAtivo = false;
  MainFrame->EMedicao = ((CMainFrame*)AfxGetMainWnd())->VerfMedicao();
  MainFrame->asdfg=IDD_DESPERFILGEOLOGICO; 
  MainFrame->LigaSilencioso();

  CString NomeArquivo(MainFrame->PegaProjetoAtual());
  NomeArquivo = NomeArquivo.Left( NomeArquivo.ReverseFind('\\'));
  NomeArquivo += "\\Geologico.des";  
  
  //--- VS12 não precisa mais===> Cria um arq temporario, so para não dar erro no OpenDocumentFile que agora( VS 10) exige que o arquivo exista
  //--- A extensão tem q ser .des, para chamar as rotinas certas do VU&E

  OpenDocumentFile(NomeArquivo,false); 

  char Buffer[64]= {0};

  ::GetPrivateProfileString(_T("DadosDesenhos"),_T("Desgeologico"),_T("0"),Buffer,63,NULL);//((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual() + std::string(".ini")).c_str());		

  MainFrame->DesligaSilencioso();
}

void CEstApp::OnProjetoInserirRua()
{
  std::string NovaRua,Copiar,CopiarOutroProjeto;
  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());

  CDiaInsereRua DiaInsereRua(nullptr,LstRuas,NovaRua,Copiar,&CopiarOutroProjeto);

  int Ret(DiaInsereRua.DoModal());

  if(Ret != IDOK) 
  {
    MainFrame->RuaAtual = "IDCANCEL";
    MainFrame->ProjetoNovo = -1;

    return;
  }
    
  if (NovaRua.size() > 0)
  {
    MainFrame->RuaAtual = "";

    CString NomeProjeto(MainFrame->PegaProjetoBase());
    NomeProjeto = NomeProjeto.Left(NomeProjeto.ReverseFind('\\') + 1);
    CString DirNovaRua(NomeProjeto);

    DirNovaRua.Append(NovaRua.c_str());

    _mkdir(DirNovaRua.GetBuffer());     

    LstRuas.push_back(NovaRua);

    MainFrame->m_RuasProjeto.InsertString(0, NovaRua.c_str());
    MainFrame->m_RuasProjeto.SetCurSel(0);

    if (!Copiar.empty() || !CopiarOutroProjeto.empty())
    {
      static const char* Ext[] = { {".lim"},{".emp"},{".pav"},{".sec"},{".taa"},{".tac"},{".tpa"},{".gra"},{".tps"},{".ato"},{".rav"},NULL };

      std::string strOrigem;

      if (!CopiarOutroProjeto.empty())
      {
        strOrigem = CopiarOutroProjeto;
        //strOrigem.pop_back();
        Copiar = strOrigem.substr(strOrigem.find_last_of('\\'));
      }
      else
      {
        strOrigem = NomeProjeto.GetBuffer();
        strOrigem += Copiar;
      }

      int C(0);

      while (Ext[C] != NULL)
      {
        std::string ArqOrigem(strOrigem);
        //ArqOrigem.append("\\");
        ArqOrigem.append(Copiar);
        ArqOrigem.append(Ext[C]);

        std::string ArqDestino(DirNovaRua);
        ArqDestino.append("\\");
        ArqDestino.append(NovaRua);
        ArqDestino.append(Ext[C]);

        CopyFile(ArqOrigem.c_str(), ArqDestino.c_str(), false);

        C++;
      }
      SalvaListaRuas();
    }

    if (((CMainFrame*)AfxGetMainWnd())->CriaRua(DirNovaRua.GetBuffer(), NovaRua.c_str()) == 0)     //--- 0 = OK
    {
      CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());
      MainFrame->DirRua = NomeProjeto;
      MainFrame->RuaAtual = NovaRua.c_str();
      if (MainFrame->DesPontosGeo)
      {
        MainFrame->InsereRuaDesGeo(NovaRua);
      }
      MainFrame->MudouRua(NovaRua);
    }
    SalvaListaRuas();
  }
}

void CEstApp::SalvaListaRuas()
{
  CMainFrame* MainFrame((CMainFrame*) AfxGetMainWnd());
  std::string NomeProjeto(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoBase()));

  if (NomeProjeto.size()) 
  {
	  std::string NomeArq(NomeProjeto + ".lsr");
    
    CFile Arq;
	  CFileException e;

	  if (Arq.Open(NomeArq.c_str(),CFile::modeCreate | CFile::modeWrite,&e) == 0)
	  {
	    monolog.mensagem(14,CString(" Arquivo ") + NomeArq.c_str(),e.m_cause);
	    return;
	  }

    CArchive ar(&Arq,CArchive::store);

    ar << LstRuas.size();

    for(ittlstwstr it = LstRuas.begin() ; it != LstRuas.end() ; it++)
    {
      ar << *it;
    }

    CString RuaAtual(MainFrame->PegaRuaAtual());

    ar << RuaAtual;

    ar.Close();
    Arq.Close();
  }
  else return;
}
void CEstApp::LeListaRuas()
{
  CMainFrame* MainFrame((CMainFrame*) AfxGetMainWnd());
  std::string NomeProjeto(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoBase()));

  if (NomeProjeto.size() > 3) 
  {
	  std::string NomeArq(NomeProjeto + ".lsr");
    
    CFile Arq;
	  CFileException e;

	  if (Arq.Open(NomeArq.c_str(),CFile::modeRead,&e) == 0)
	  {
      if(e.m_cause != 2)
      {
	      monolog.mensagem(14,CString(" Arquivo ") + NomeArq.c_str(),e.m_cause);
      }

      return;
	  }

    CArchive ar(&Arq,CArchive::load);

    LstRuas.clear();
    size_t Tam(0);

    for(ar >> Tam ; Tam > 0 ; Tam--)
    {
      std::string NomeAtual;
      ar >> NomeAtual;
      LstRuas.push_back(NomeAtual);
    }

    ar >> MainFrame->RuaAtual;
    Arq.Close();
  }
  else return;
}

BOOL CEstApp::SaveAllModified()
{
  SalvaListaRuas();

  return CWinApp::SaveAllModified();
}

void CEstApp::MudouProjetoSelecionado(std::string& ProjeSelecionado)
{
 CMainFrame *MainFrame((CMainFrame*)AfxGetMainWnd());

  MainFrame->MudouProjeto(ProjeSelecionado);
}

void CEstApp::MudouRuaSelecionada(std::string& RuaSelecionada)
{
  CMainFrame *MainFrame((CMainFrame*)AfxGetMainWnd());

  MainFrame->MudouRua(RuaSelecionada);
}

void CEstApp::OnDesenhosPerfisAvulsos()
{
  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());

  if (MainFrame->EstadoLicenca == false)
	{
		AfxMessageBox("A chave de segurança não foi encontrada, está vencida ou está inválida.");
		return;
	}

	CString NomeProjeto(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()));

	MainFrame->asdfg = IDD_DESPERFISAVULSOS;
	MainFrame->LigaSilencioso();

	CString NomeArquivo(MainFrame->PegaProjetoAtual());
	NomeArquivo += "\\PerfilAvulso.des";

   //--- VS12 não precisa mais ===>Cria um arq temporario, so para não dar erro no OpenDocumentFile que agora( VS 10) exige que o arquivo exista
   //--- A extensão tem q ser .des, para chamar as rotinas certas do VU&E

   OpenDocumentFile(NomeArquivo, false);

   char Buffer[64] = { 0 };

   ::GetPrivateProfileString(_T("DadosDesenhos"), _T("DesPerfisAvulsos"), _T("0"), Buffer, 63, NULL);//,(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual() + std::string(".ini")).c_str());		

   MainFrame->DesligaSilencioso();
}

void CEstApp::OnRelatBaciasHidrograf()
{
  ((CMainFrame*)AfxGetMainWnd())->asdfg = IDD_REL_BACIAS_HIDROGRAF;
  AbreArqCriaDoc("\\BaciasHidrograf.rel", false);
}

void CEstApp::OnRelatEquipDrenagem()
{
  ((CMainFrame*)AfxGetMainWnd())->asdfg = IDD_REL_EQUIP_DRENAGEM;
  AbreArqCriaDoc("\\EquipDrenagem.rel", false);
}

#include "CDiaImpArqTopografia.h"

void CEstApp::OnImpArqTopografia()
{
  std::string ArqTriang,ArqAciTopog,CNMestra,CNInterm;

  CDiaImpArqTopografia DiaImpArqTopografia(ArqTriang, ArqAciTopog,CNMestra,CNInterm);

  if(DiaImpArqTopografia.DoModal() == IDOK)
  {
    ((CDiaThread*)pDiaProgresso)->ShowWindow(SW_SHOW);

    ImportaArqTopografia(ArqTriang,ArqAciTopog,CNMestra,CNInterm);
  }
}

void CEstApp::ImportaArqTopografia(const std::string& NomeArquivo, const std::string& NomeArqAciTopog, const std::string& pCNMestra, const std::string& pCNInterm)
{
  if (NomeArquivo.size() != 0)
  {
    CImportaDXFTopografia ImportarDXFTopografia(((CMainFrame*)AfxGetMainWnd())->m_sNomeComp.GetBuffer());

    ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("Importando os arquivos..."), 25);

    ImportarDXFTopografia.ImportaArquivo(NomeArquivo,NomeArqAciTopog);

    ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("Importando as curvas de nível..."), 50);

    ImportarDXFTopografia.ImportaCNs(NomeArqAciTopog,pCNMestra,pCNInterm);

    ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("Importando a malha triangulada..."), 75);

    GeraSuperficie(&ImportarDXFTopografia.PegaTriangulacao(),&ImportarDXFTopografia.PegaCurvasNivel(), NomeArquivo, NomeArqAciTopog);

    ((CMainFrame*)AfxGetMainWnd())->TrianImporTopografia = true;

    ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("Ok, a importação foi feita com êxito."), 100);

    ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->ShowWindow(SW_HIDE);

    CMainFrame* MF(((CMainFrame*)AfxGetMainWnd()));

   MF->TipoSuperficie = SUPERF_TOPOGRAFIA;

   //--- Atualiza o arquivo .ini, o desenho da superficie vai precisar

    char Buffer[1024] = { 0 };

    std::string Projeto(MF->PegaProjetoBase());
    std::string NomeArqIni(Projeto + std::string(".ini"));

    ::GetPrivateProfileString(_T("DadosDesenhos"), _T("DadosDesPontosGeoGeoRef"), _T("0,0,0,0,0,0"), Buffer, 511, NomeArqIni.c_str());

    std::stringstream StrstrBuffer(Buffer);
    char Virgula;
    int FaltaGeoReferenciamento, DesenharMapa, TipoSuperficie, DesenharCroquis, FronteiraEditada, TrianImporTopografia;
    std::string NomeArqAciAntigo;

    StrstrBuffer >> FaltaGeoReferenciamento >> Virgula >> DesenharMapa >> Virgula >>  TipoSuperficie >> Virgula
                 >> DesenharCroquis >> Virgula >> FronteiraEditada >> Virgula >> TrianImporTopografia >> NomeArqAciAntigo;

    MF->TipoSuperficie = TipoSuperficie = SUPERF_TOPOGRAFIA;
    MF->TrianImporTopografia = TrianImporTopografia = TRUE;

    std::stringstream StrstrBuffer2;

    StrstrBuffer2 << FaltaGeoReferenciamento << Virgula << DesenharMapa << Virgula << TipoSuperficie << Virgula
                  << DesenharCroquis << Virgula << FronteiraEditada << Virgula << TrianImporTopografia;

    ::WritePrivateProfileString(_T("DadosDesenhos"), _T("DadosDesPontosGeoGeoRef"),StrstrBuffer.str().c_str(), NomeArqIni.c_str());

    StrstrBuffer2.str("");
    StrstrBuffer2.seekg(0, StrstrBuffer.beg);

    StrstrBuffer2 << NomeArquivo.c_str() << ',' << NomeArqAciTopog.c_str();

   ::WritePrivateProfileString(_T("DadosDesenhos"), _T("ArquivosTopografia"), StrstrBuffer2.str().c_str(), NomeArqIni.c_str());
  }
}

#include "CDImportarArqSIG.h"

void CEstApp::OnImpArqSIG()
{
  std::string ArqCN, ArqTalvegues;

  ErroImportacaoArquivos = true;

  CDImportarArqSIG DiaImpArqSIG(ArqCN, ArqTalvegues, NomeArqMapaGIS,NomeArqPCTRLGIS,std::string(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()), EquidisCNQGIS, nullptr);

  if (DiaImpArqSIG.DoModal() == IDOK)
  {
    ((CDiaThread*)pDiaProgresso)->ShowWindow(SW_SHOW);

    ImportaArqSIG(ArqCN, ArqTalvegues, NomeArqMapaGIS, NomeArqMapaGIS);

    ErroImportacaoArquivos= false;

  }
}

void CEstApp::ImportaArqSIG(std::string& pArqCN,std::string& pArqTalvegues,std::string& pArqPCTRL,std::string& pArqMapa)
{
  CDImportarArqSIG  DImportarArqSIG(pArqCN, pArqTalvegues, pArqMapa,pArqPCTRL, std::string(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()), EquidisCNQGIS,nullptr);

  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("Importando os arquivos..."), 25);

  DImportarArqSIG.ImportaCNsSIG(pArqCN,"AcDbEntity");

  ErroImportacaoArquivos = DImportarArqSIG.PegaErro();

  if(ErroImportacaoArquivos == CDImportarArqSIG::ERROS::SEM_ERROS)
  {
    ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("Importando as curvas de nível..."), 50);

    ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("Ok, a importação foi feita com êxito."), 100);

    ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->ShowWindow(SW_HIDE);

    GeraSuperficieSIG(&DImportarArqSIG.PegaCurvasNivel());

    ((CMainFrame*)AfxGetMainWnd())->TrianImporTopografia = true;

    ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("Ok, a importação foi feita com êxito."), 100);

    ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->ShowWindow(SW_HIDE);

    CMainFrame* MF(((CMainFrame*)AfxGetMainWnd()));

    MF->TipoSuperficie = SUPERF_SIG;

    //--- Atualiza o arquivo .ini, o desenho da superficie vai precisar

    char Buffer[1024] = { 0 };

    std::string Projeto(MF->PegaProjetoBase());
    std::string NomeArqIni(Projeto + std::string(".ini"));

    ::GetPrivateProfileString(_T("DadosDesenhos"), _T("DadosDesPontosGeoGeoRef"), _T("0,0,0,0,0,0"), Buffer, 511, NomeArqIni.c_str());

    std::stringstream StrstrBuffer(Buffer);
    char Virgula;
    int FaltaGeoReferenciamento, DesenharMapa, TipoSuperficie, DesenharCroquis, FronteiraEditada, TrianImporTopografia;
    std::string NomeArqAciAntigo;

    StrstrBuffer >> FaltaGeoReferenciamento >> Virgula >> DesenharMapa >> Virgula >> TipoSuperficie >> Virgula
                 >> DesenharCroquis >> Virgula >> FronteiraEditada >> Virgula >> TrianImporTopografia >> NomeArqAciAntigo;

    MF->TipoSuperficie = TipoSuperficie = SUPERF_SIG;
    MF->TrianImporTopografia = TrianImporTopografia = false;

    std::stringstream StrstrBuffer2;

    StrstrBuffer2 << FaltaGeoReferenciamento << Virgula << DesenharMapa << Virgula << TipoSuperficie << Virgula
                  << DesenharCroquis << Virgula << FronteiraEditada << Virgula << TrianImporTopografia;

    ::WritePrivateProfileString(_T("DadosDesenhos"), _T("DadosDesPontosGeoGeoRef"), StrstrBuffer.str().c_str(), NomeArqIni.c_str());

    StrstrBuffer2.str("");
    StrstrBuffer2.seekg(0, StrstrBuffer.beg);

    StrstrBuffer2 << pArqCN.c_str() << ',' << pArqTalvegues.c_str() << ',' << pArqMapa.c_str();

    ::WritePrivateProfileString(_T("DadosDesenhos"), _T("ArquivosTopografia"), StrstrBuffer2.str().c_str(), NomeArqIni.c_str());
  }
  else
  {
    switch (DImportarArqSIG.PegaErro())
    {
      case CDImportarArqSIG::ERROS::CN_SEM_COTA:
      {
        monolog.mensagem(-1,"As curvas de nível estão sem cota, importação abortada");

        ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->ShowWindow(SW_HIDE);
      }
      break;
    }
  }
  return;
}

void CEstApp::GeraSuperficieSIG(void* pLLPontosCN)
{
  std::string NomeProjeto(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase());

  CCriaSuperfSIG CriaSuperfSIG(*(LLPontos*)pLLPontosCN);
}

void CEstApp::GeraSuperficie(void* pTriangulacao, void* pCurvasNivel, const std::string& NomeArquivo, const std::string& NomeArqAciTopog)
{
  std::string NomeProjeto(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase());

  CCriaSuperfIgualTopog CriaSuperf(*(CTriangulacao*)pTriangulacao, *(LLPontos*)pCurvasNivel);

  CriaSuperf.CriaSuperfIgualTopog();
  CriaSuperf.GravaArqTrian(NomeProjeto);
  CriaSuperf.GravaArqCNs(NomeProjeto);
}

void CEstApp::DialogoInicial(bool AbrirProjetoExistente)
{
  CMainFrame* pMF((CMainFrame*) pMainFrame);

  CDInicial DInicial(pMF->ProjetoNovo, (CMainFrame*)AfxGetMainWnd(), pMF->PegaProjRecentes(), AbrirProjetoExistente);

  if (DInicial.DoModal() == IDOK)
  {
  //  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->ShowWindow(SW_SHOW);
    
    if (pMF->ProjetoNovo != -1)
    {
      if (pMF->ProjetoNovo < 100)
      {
        ((CMainFrame*)AfxGetMainWnd())->OnProjNovo();
      }

      if (pMF->ProjetoNovo != -1)
      {
        switch (pMF->ProjetoNovo)
        {
		      case 1:
          case 2: 
          case 3:((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->ShowWindow(SW_SHOW);
          case 5:
          {
            OnEditarPontosGeo();
          }
          break;
          
          case 4:
          {
            ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->ShowWindow(SW_SHOW);

            OnImpArqTopografia();
            OnEditarPontosGeo();
          }
          break;
        }
      }
    }
    else
    {
      ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->ShowWindow(SW_SHOW);

      pMF->AbreProj(DInicial.ProjetoSelecionado(),true);
    }
  }

  pMF->GravaProjetosRecentes();
}

void CEstApp::AbreProjExistente(int Num, const std::string& Projeto,CView* pDiaInicial,bool IniciarDesSuperf)
{
  CMainFrame* pMF((CMainFrame*)AfxGetMainWnd());

  pMF->ProjetoNovo = pMF->TipoSuperficie = Num;

  pDiaInicial->SendMessage(SW_HIDE);
  pDiaInicial->GetParent()->PostMessage(WM_CLOSE);

  ErroImportacaoArquivos = 0;

  if (Num != -1)
  {
    if (Num < 100) ((CMainFrame*)AfxGetMainWnd())->OnProjNovo();

    switch (pMF->ProjetoNovo)
    {
      case 4:
      {
        OnImpArqTopografia();
      }
      break;
      case 6:
      {
        OnImpArqSIG();
      }
      break;
    }

    if (ErroImportacaoArquivos == 0)
    {
      OnEditarPontosGeo();
    }
  }
  else
  {
    if(IniciarDesSuperf) ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->ShowWindow(SW_SHOW);

    pMF->AbreProj(Projeto, IniciarDesSuperf);
  }
}

