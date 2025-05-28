// DSecionar.cpp : implementation file
//
#include "stdafx.h"
#include "afxtempl.h"
#include "est.h"
#include "cponto.h"
#include <list>
#include <set>
#include "cdeltasuperficie.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include <map>
#include "CAcidentesTopograficos.h"
#include "CSegmento.h"
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CLPontos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "CEquipDrenagem.h"
#include "Superficie.h"
#include "DSecionar.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqgradientes.h"
#include <string>
#include "cgradientes.h"
#include "monologo.h"
#include "dcalcgeoseccdireto.h"
#include "carqsecoesavulsas.h"
#include "csecaoavulsa.h"
#include "csecoesavulsas.h"
#include <afxmt.h> 
#include "Perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "CSeccionar.h"
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "CCurhorizontal.h"
#include "CArqCurHorizontais.h"
#include "CArqSecTipo.h"
#include "CSecTipo.h"
#include "CArqCanteiros.h"
#include "CCanteiros.h"
#include "Mat.h"
#include "CCurHorizontais.h"
//#include "carqsecoesavulsas.h"
//#include "csecaoavulsa.h"
//#include "csecoesavulsas.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include "CLembrete.h"
#include "CRadar.h"
#include "CArqAciTopograficos.h"
#include "CTrecho.h"
#include "CArqTrechos.h"
#include "CTrechos.h"
#include <sstream>

#define  FALTAM_SECOES_TIPO 100
#define  FALTAM_CURVAS_HORIZONTAIS 101

const double M_PI_2	= 1.57079632679489661923;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class monologo monolog;        //--- Dialogo para monologos.
extern class dialogo dialogar;

//---------------- O cálculo é executado em uma thread ------------------------------------ 

struct p     //--- Estrutura para a passagem dos parametros.
{
  CDialog* CDial;
  CString String;
  CEvent* EventoFim;
  bool bTodasSecoes;
  bool Otimizar;
  bool TodosTrechos;
  double MenorDistancia,
         MenorRampa;
}ParametrosSeccionar;

UINT ThreadFunction( LPVOID pParam );

/////////////////////////////////////////////////////////////////////////////
// CDSecionar dialog

BOOL CDSecionar::Create(CEvent* Evento,CWnd* pWnd,CDialog* pDialog)        //--- opcao para  modeless.
{
  ParametrosSeccionar.EventoFim = Evento;            //--- NULL, TRUE, FALSE,"Calculo Nota De Serviço");
  ParametrosSeccionar.CDial = pDialog;
 
  pDialogo = pDialog;

  return CDialog::Create(IDD,pWnd);
}

UINT ThreadFunction(LPVOID pParam)    //--- Thread calculo das secões
{
  CSeccionar Seccionar(ParametrosSeccionar.String,ParametrosSeccionar.CDial,NULL,ParametrosSeccionar.Otimizar,
                       ParametrosSeccionar.MenorDistancia,ParametrosSeccionar.MenorRampa,ParametrosSeccionar.TodosTrechos);

  Seccionar.BackupArquivos();
  if(Seccionar.Iniciar(ParametrosSeccionar.bTodasSecoes) == false) 
  {
     Seccionar.VoltaBackupArquivos();
     return 1;   //-- returno com erro
  }

  Seccionar.RemoveBackupArquivos();

  if(ParametrosSeccionar.EventoFim) ParametrosSeccionar.EventoFim->SetEvent();

  return 0;   //--- Retorno OK (padrão para <afxbeginthread()>).
}

CDSecionar::CDSecionar(const CString& NomProj,CWnd* pParent): CDialog(CDSecionar::IDD, pParent), NomeProjeto(NomProj),pDialogo(this),
            bolOtimizar(true), douMenorDist(5.0), douMenorInclinacao(5.0)
{
  //{{AFX_DATA_INIT(CDSecionar)
  m_iEquidis = 30;
  m_Descricao1 = _T("");
  //}}AFX_DATA_INIT
}

void CDSecionar::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_BUTCANCELAR, m_buCancelar);
  DDX_Control(pDX, IDOK, m_buOK);
  DDX_Control(pDX, IDC_SECCIONAR, m_buSeccionar);
  DDX_Control(pDX, IDC_PROGRESS1, m_Progresso1);
  DDX_Text(pDX, IDC_DESCRICAO1, m_Descricao1);
  DDX_Control(pDX, IDC_BUTPAUSA, m_butPausar);
  DDX_Control(pDX, IDC_BUTLOG, m_butLog);
  DDX_Text(pDX, IDC_EQUIDIS, m_iEquidis);
  DDX_Text(pDX, IDC_EDIMENORDIST, douMenorDist);
  DDX_Text(pDX, IDC_EDIMENORINCLINACAO, douMenorInclinacao);
  DDX_Text(pDX, IDC_EQUIDIS, m_iEquidis);
  DDX_Control(pDX, IDC_EDIMENORDIST, ediMenorDistancia);
  DDX_Control(pDX, IDC_EDIMENORINCLINACAO, ediMenorRampa);
  DDV_MinMaxDouble(pDX, douMenorDist, 0.50, 500.0);
  DDV_MinMaxDouble(pDX, douMenorInclinacao, 1.0, 100.0);
  DDX_Control(pDX, IDC_CHETODOSTRECHOS, CheTodosTrechos);
  DDX_Control(pDX, IDC_CHEAPENASTRECHOATUAL, CheApenasTrechoAtual);
}

BEGIN_MESSAGE_MAP(CDSecionar, CDialog)
  ON_BN_CLICKED(IDC_SECCIONAR, OnSeccionar)
  ON_BN_CLICKED(IDC_BUTCANCELAR, OnButcancelar)
  ON_WM_CREATE()
  ON_BN_CLICKED(IDC_BUTPAUSA, OnBnClickedButpausa)
  ON_BN_CLICKED(IDC_RADSOFALTANTES, &CDSecionar::OnBnClickedRadsofaltantes)
  ON_BN_CLICKED(IDC_RADTODAS, &CDSecionar::OnBnClickedRadtodas)
  ON_BN_CLICKED(IDC_CHEOTIMIZARPERFIL, &CDSecionar::OnBnClickedCheotimizarperfil)
  ON_BN_CLICKED(IDC_CHEAPENASTRECHOATUAL, &CDSecionar::OnBnClickedCheapenastrechoatual)
  ON_BN_CLICKED(IDC_CHETODOSTRECHOS, &CDSecionar::OnBnClickedChetodostrechos)
  ON_BN_CLICKED(IDC_BUTLOG, &CDSecionar::OnBnClickedButlog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDSecionar message handlers

void CDSecionar::OnOK() 
{
  CDialog::OnOK();
}

void CDSecionar::OnSeccionar()
{
  CProgressCtrl* Limpezao(NULL);
  CString* pDescricao(NULL);

  UpdateData(true);

  //-- Recessiona o terreno -------------------------------------------------------------------------------------------------------

  CMainFrame* MF((CMainFrame*)AfxGetMainWnd());

  int Erro(0);
  bool TodosTrechos(((CButton*)GetDlgItem(IDC_CHETODOSTRECHOS))->GetCheck() == BST_CHECKED);
  if (!TodosTrechos)
  {
    NomeProjeto = NomeProjeto.Left(NomeProjeto.ReverseFind('\\')+1);
    NomeProjeto += CString(MF->PegaRuaAtual()) + "\\" + CString(MF->PegaRuaAtual());
  }
  
  CTrechos Trechos(std::string(NomeProjeto.GetBuffer()),TodosTrechos);

  for (ittymapCTrechos it = Trechos.MapaTrechos.begin(); !Erro && it != Trechos.MapaTrechos.end(); it++)
  {
    if (!TodosTrechos && it->first.find(((CMainFrame*)AfxGetMainWnd())->PegaRuaAtual()) == std::string::npos) continue;

    //CString NomeProjeto(it->first.c_str());
   // CSecTipo SecoesTipo(NomeProjeto);
    //Erro = SecoesTipo.VerfErro();

	 // if (SecoesTipo.GetCount() == 0) Erro = FALTAM_SECOES_TIPO;    //--- Pode seccionar sem as seções tipo

    //SetDlgItemText(IDC_DESCRICAO1, NomeProjeto);

    if (1 || Erro == 0)
    {
      //--- recalcula as coordenadas das estacas

	    if (it->second.CurvasHorizontais.GetCount() < 2)
	    {
		    monolog.mensagem(-1,"Faltam curvas horizontais.");
		    Erro = FALTAM_CURVAS_HORIZONTAIS;
	    }
	    else Erro = it->second.CurvasHorizontais.VerfErro();
	  
      if (Erro == 0)
      {
        it->second.CurvasHorizontais.BackupArquivos();
        Erro = it->second.CurvasHorizontais.CalcGeometrico(m_Descricao1, &m_Progresso1, this, 0.0005, ((CMainFrame*)AfxGetMainWnd())->AchaProjetoNoVetor(), !MF->GeometricoAlterado || MF->GreideAlterado);
        if (Erro != 0)  it->second.CurvasHorizontais.VoltaBackupArquivos();
      }
      else
      {
        monolog.mensagem(-1,"Houve Erro nas curvas horizontais. Os cálculos deste trecho NÃO foram executados.");
      }
    }
    else monolog.mensagem(-1,"Houve Erro ao ler o arquivo de seções tipo. Os cálculos geométricos deste trecho NÃO foram executados.");
  }

  //-----------------------------------------------------------------------------------------------------------------------------

  if (1 || Erro == 0)
  {
    CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());

    ParametrosSeccionar.String = MainFrame->PegaProjetoAtual();
    ParametrosSeccionar.bTodasSecoes = ((CButton *)GetDlgItem(IDC_RADTODAS))->GetCheck() == BST_CHECKED;
    if (!ParametrosSeccionar.CDial) ParametrosSeccionar.CDial = this;
    ParametrosSeccionar.Otimizar = bolOtimizar;
    ParametrosSeccionar.MenorDistancia = douMenorDist;
    ParametrosSeccionar.MenorRampa = douMenorInclinacao / 100.0;
    ParametrosSeccionar.TodosTrechos = TodosTrechos;

    if (pDialogo == this)       //--- se falso significa que é não modal (calcular direto) senão é modal  
    {
      Limpezao = &m_Progresso1;
      pDescricao = &m_Descricao1;

      m_buCancelar.EnableWindow(FALSE);    //--- Desabilita o botão pausa.
      m_buSeccionar.EnableWindow(FALSE);   //--- Desabilita o botão Seccionar.
      m_butPausar.EnableWindow(TRUE);      //--- Habilita o botão pausar.
    }
    else
    {
      pDescricao = &((CDCalcGeoSeccDireto*)pDialogo)->m_CSExecutando2;
      Limpezao = &((CDCalcGeoSeccDireto*)pDialogo)->m_Progresso;
    }

    pDialogo->UpdateData(false);

    //--- A thread é criada no modo "suspended" para a passagem de parametros, no caso o
    //--- m_MainWnd

    HANDLE event;
    event = CreateEvent(NULL, TRUE, FALSE, "CONEVENTTEST");

    try
    {
      T = AfxBeginThread(ThreadFunction, &ParametrosSeccionar, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      T->m_pMainWnd = MainFrame;     //--- O cálculo usa o mainframe, emtão passa-o para a thread.
      T->ResumeThread();             //---  Executa a tread.
    }
    catch (...)
    {
      monolog.mensagem(-1,"Houve Erro. Os cálculos NÃO foram executados.");
      Erro = 2;
    }
  }
  else 
  {
     monolog.mensagem(Erro," Houve Erro no cálculo das curvas horizontais.");
    SetDlgItemText(IDC_DESCRICAO1, "Houve Erro. O trecho não foi seccionado.");
  }

  //--- Cálculo executado com sucesso

  m_butPausar.EnableWindow(FALSE);
  m_buCancelar.EnableWindow(FALSE);
  m_butLog.EnableWindow(TRUE);
  m_buOK.EnableWindow(Erro == 0);
}
BOOL CDSecionar::OnInitDialog() 
{
  CDialog::OnInitDialog();

  if(pDialogo == this) 
  {
    m_buOK.EnableWindow(FALSE);  
    m_butPausar.EnableWindow(FALSE);
    m_butLog.EnableWindow(FALSE);
    ((CButton *) GetDlgItem(IDC_RADSOFALTANTES))->SetCheck(true);
  }
  if(0 && ((CMainFrame*) AfxGetMainWnd())->GeometricoAlterado)    //--- Precisa de um controle maior, fica para a próxima versão
  {
    ((CButton *) GetDlgItem(IDC_RADTODAS))->SetCheck(true);
    ((CButton *) GetDlgItem(IDC_RADSOFALTANTES))->EnableWindow(false);
  }
  else  ((CButton *) GetDlgItem(IDC_RADSOFALTANTES))->SetCheck(true);

  ReadWindowPlacement();

  ediMenorDistancia.EnableWindow(bolOtimizar);
  ediMenorRampa.EnableWindow(bolOtimizar);

  ((CButton *)GetDlgItem(IDC_CHEOTIMIZARPERFIL))->SetCheck(bolOtimizar);

  CheApenasTrechoAtual.SetCheck(BST_CHECKED);
  
  UpdateData(false);

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CDSecionar::OnButcancelar() 
{
   if(NomeProjeto.GetLength())
  {
    std::string strNomeArquivo(NomeProjeto.GetBuffer());

    if(strNomeArquivo.size())    //---- Volta os arquivos anteriores
    {
       CopyFile(NomeProjeto+".geobak",NomeProjeto+".geo",false);
       CopyFile(NomeProjeto+".pesbak",NomeProjeto+".pes",false);
    }
  }

  EndDialog(true);	
}

void CDSecionar::OnBnClickedButpausa()
{
 static bool Estado(true);

 Estado = !Estado;

  if(Estado) 
  {
    m_butPausar.SetWindowText("Pausar");
    m_buCancelar.EnableWindow(FALSE);
    T->ResumeThread();    
  }
  else 
  {
    m_butPausar.SetWindowText("Continuar");  
    m_buCancelar.EnableWindow(TRUE);
    T->SuspendThread();   
  }
}

void CDSecionar::WriteWindowPlacement()
{
  CString Bloco("DiaSeccionar");            
  TCHAR szFormat[] = _T("%d,%3d,%2d,%3d,%2d"),szBuffer[100];

  wsprintf(szBuffer,szFormat,m_iEquidis,int(douMenorDist),(int)((douMenorDist-long(douMenorDist))*100.0),(int)(douMenorInclinacao),
           (int)((douMenorInclinacao-long(douMenorInclinacao))*100.0));

  CString NomeProjeto(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual());

  ::WritePrivateProfileString(Bloco,"Valores",szBuffer,NomeProjeto + ".ini");
}

bool CDSecionar::ReadWindowPlacement()
{
  CString Bloco("DiaSeccionar");            

  TCHAR szFormat[] = _T("%d,%3d,%2d,%3d,%2d");
  char Buffer[512]= {0};

  CString NomeProjeto(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual());

  ::GetPrivateProfileString(Bloco,"Valores","",Buffer,511,(NomeProjeto + ".ini")); 
  if (strlen(Buffer) == 0) return FALSE;

  int  IntDist(0),IntInclinacao(0),DecimaisDist(0),DecimaisInclinacao(0);

  int nRead(_stscanf(Buffer, szFormat,&m_iEquidis,&IntDist,&DecimaisDist,&IntInclinacao,&DecimaisInclinacao));

  if (nRead != 5) return FALSE;

  douMenorDist = IntDist + (DecimaisDist/100.0);  
  douMenorInclinacao = IntInclinacao + (DecimaisInclinacao/100.0);

  UpdateData(false);

  return TRUE;
}

 CDSecionar::~CDSecionar()
 {
   WriteWindowPlacement();
 }

 void CDSecionar::OnBnClickedRadtodas()
 {
   ((CButton *) GetDlgItem(IDC_RADSOFALTANTES))->SetCheck(false);
   ((CButton *) GetDlgItem(IDC_RADTODAS))->SetCheck(true);
 }

 void CDSecionar::OnBnClickedRadsofaltantes()
 {
    ((CButton *) GetDlgItem(IDC_RADTODAS))->SetCheck(false);
    ((CButton *) GetDlgItem(IDC_RADSOFALTANTES))->SetCheck(true);
 }

 void CDSecionar::OnBnClickedCheotimizarperfil()
 {
   bolOtimizar = !bolOtimizar;  

   ediMenorDistancia.EnableWindow(bolOtimizar);
   ediMenorRampa.EnableWindow(bolOtimizar);
 }

 void CDSecionar::OnBnClickedCheapenastrechoatual()
 {
   CheApenasTrechoAtual.SetCheck(BST_CHECKED);
   CheTodosTrechos.SetCheck(BST_UNCHECKED);
 }

 void CDSecionar::OnBnClickedChetodostrechos()
 {
   CheTodosTrechos.SetCheck(BST_CHECKED);
   CheApenasTrechoAtual.SetCheck(BST_UNCHECKED);
 }

 void CDSecionar::OnBnClickedButlog()
 {
   ((CMainFrame*)AfxGetMainWnd())->asdfg = IDD_REL_LOG_SECCIONAR;
   ((CEstApp*)AfxGetApp())->AbreArqCriaDoc("_Seccionar.rel", false, true);   //--- é .log passa .rel so para abrir os arquivos
 }
