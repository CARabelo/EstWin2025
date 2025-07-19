// CalcNota.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "supercstring.h"
#include "math.h"
#include "estaca.h"
#include "cponto.h"
#include "clpontos.h"
#include "carqgradientes.h"
#include <set>
#include <string>
#include "cgradientes.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include "carqIgualdades.h" 
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "carqcurverticais.h"
#include "ccurverticais.h"
#include "carqcurhorizontais.h"
#include "perfil.h"
#include "carqcanteiros.h"
#include "CCanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include <map>
#include "ccurhorizontal.h"
#include "Mat.h"
#include "ccurhorizontais.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "CExcomboBox.h" 
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
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
#include <afxmt.h>              //--- Para CEvent
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
#include "CalcNota.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//UINT Iniciar(LPVOID Parametros);

UINT MyControllingFunction( LPVOID pParam );

/////////////////////////////////////////////////////////////////////////////
// CalcNota dialog

CalcNota::CalcNota(const CString& pNomeProj,CWnd* pParent,CProjeto* pProj)	: CDialog(CalcNota::IDD, pParent),Pausa(false),T(NULL),NomeProj(pNomeProj),pProjeto(pProj)  
{
  //{{AFX_DATA_INIT(CalcNota)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CalcNota::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CalcNota)
  DDX_Control(pDX, IDC_OK, m_BUOK);
  DDX_Control(pDX, IDC_PAUSA, m_BUPausa);
  DDX_Control(pDX, IDC_CANCELAR, m_BUCancelar);
  DDX_Control(pDX, IDC_INICIARCALC, m_BUIniciar);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CalcNota, CDialog)
  //{{AFX_MSG_MAP(CalcNota)
  ON_BN_CLICKED(IDC_INICIARCALC, OnIniciarcalc)
  ON_BN_CLICKED(IDC_OK, OnOk)
  ON_BN_CLICKED(IDC_CANCELAR, OnCancelar)
  //}}AFX_MSG_MAP
  //ON_BN_CLICKED(IDC_PAUSA, OnBnClickedPausa)
  ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESSO, &CalcNota::OnNMCustomdrawProgresso)
  ON_BN_CLICKED(IDC_CALCTUDO, &CalcNota::OnBnClickedCalctudo)
  ON_BN_CLICKED(IDC_CALCSOCOTA, &CalcNota::OnBnClickedCalcsocota)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CalcNota message handlers

INT_PTR CalcNota::DoModal() 
{
  return CDialog::DoModal();
}

//---------------- O cálculo é executado em uma thread ------------------------------------ 

struct p     //--- Estrutura para a passagem dos parametros.
{
  CDialog* CDial=nullptr;
  CString String;
  bool bSoCotas=false;
  CEvent* EventoFim=nullptr;
  CProjeto* pProj=nullptr;
}Parametros;

BOOL CalcNota::Create(CEvent* pEvento,CWnd* pWnd,CDialog* pDialogo,CProjeto* pProjeto)     //--- modeless.
{
  Parametros.EventoFim = pEvento;            //--- NULL, TRUE, FALSE,"Calculo Nota De Serviço");
  Parametros.CDial = pDialogo;
  Parametros.pProj = pProjeto;

  return CDialog::Create(IDD,pWnd);
}

UINT MyControllingFunction(LPVOID pParam)    //--- Thread para cálculo da nota de serviço
{
  CalculaNota CalculaNotaDeServico(Parametros.String,Parametros.CDial,Parametros.bSoCotas,NULL,Parametros.pProj);

  CalculaNotaDeServico.MontaSecoes();

  if(Parametros.EventoFim) Parametros.EventoFim->SetEvent();

  return 0;   //--- Retorno padrão para <afxbeginthread()>.
}

void CalcNota::OnIniciarcalc()        //--- Botão iniciar.
{
  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());

  Parametros.String = NomeProj;
 // if (!Parametros.CDial) Parametros.CDial = this;
  Parametros.CDial = this;
  Parametros.bSoCotas = IsDlgButtonChecked(IDC_CALCSOCOTA) != 0;   
  Parametros.pProj = pProjeto;

  m_BUIniciar.EnableWindow(false);
  m_BUPausa.EnableWindow(true);
  m_BUCancelar.EnableWindow(false);

  //--- A thread é criada no modo "suspended" para a passagem de parametros, no caso o
  //--- m_MainWnd

  HANDLE event;
  event = CreateEvent(NULL, TRUE, FALSE, "CONEVENTTEST");
  int erro(0);

  ((CMainFrame*)AfxGetMainWnd())->LigaSilencioso();

  try
  {
    T = AfxBeginThread(MyControllingFunction,&Parametros,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
    T->m_pMainWnd = MainFrame;     //--- O cálculo usa o mainframe, emtão passa-o para a thread.
    T->ResumeThread();             //---  Executa a tread.
  }
  catch(...) 
  {

    SetDlgItemText(1038,"Houve Erro. Os cálculos NÃO foram executados.");

    //--- Pode ser o dialogo de volumes ou da nota de serviço, depoendendo de quem chamou,
    //--- Se for o dialogo dos volumes, os controles tem ourto nome mas mesmo assim não precisam ser ligados pois 
    //--- Neste caso o dialogo é não modal

    GetDlgItem(IDC_PAUSA)->EnableWindow(false);
    GetDlgItem(IDC_OK)->EnableWindow(true);

  }
  ((CMainFrame*)AfxGetMainWnd())->DesligaSilencioso();
}

void CalcNota::OnOk() {  EndDialog(TRUE); }

BOOL CalcNota::OnInitDialog() 
{
  CDialog::OnInitDialog();

  CheckDlgButton(IDC_CALCSOCOTA,FALSE);  
  CheckDlgButton(IDC_CALCTUDO, TRUE);  

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CalcNota::OnBnClickedPausa()
{
  Pausa = !Pausa;

  if(Pausa) T->SuspendThread();
  else T->ResumeThread();
}

void CalcNota::OnCancelar() 
{
  if(T) T->SuspendThread();

  EndDialog(true);	
}

void CalcNota::OnNMCustomdrawProgresso(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CalcNota::OnBnClickedCalctudo()
{
  if(IsDlgButtonChecked(IDC_CALCTUDO) == BST_UNCHECKED) CheckDlgButton(IDC_CALCSOCOTA,BST_CHECKED);
  else CheckDlgButton(IDC_CALCSOCOTA,BST_UNCHECKED);
}

void CalcNota::OnBnClickedCalcsocota()
{
 if(IsDlgButtonChecked(IDC_CALCSOCOTA) == BST_UNCHECKED) CheckDlgButton(IDC_CALCTUDO,BST_CHECKED);
  else CheckDlgButton(IDC_CALCTUDO,BST_UNCHECKED);
}
