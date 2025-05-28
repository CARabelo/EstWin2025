// DGerarSuperficie.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include <set>
#include <list>
#include "afxtempl.h"
#include "supercstring.h"
#include "math.h"
#include "Estaca.h"
#include "carqIgualdades.h" 
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "cponto.h"
#include "DGerarSuperficie.h"
#include "classsetpontos.h"
#include "gerasuperficie.h"
#include "cdeltasuperficie.h"
#include "superficie.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GRAVAR   2
#define OK 1

//---------------- O cálculo é executado em uma thread ------------------------------------ 

struct p     //--- Estrutura para a passagem dos parametros.
{
  CDialog* CDial;
  CString String;
  CEvent* EventoFim;
  CProjeto* pProj;
}ParametrosGerarSuperficie;

UINT ThreadGerarSuperf( LPVOID pParam );

UINT ThreadGerarSuperf(LPVOID pParam)    //--- Thread calculo das secões
{
/*  CGeraSuperficie	GeraSuperficie(ParametrosGerarSuperficie.String,ParametrosGerarSuperficie.CDial);

  if(!GeraSuperficie.Gerar())
  {
    ParametrosGerarSuperficie.CDial->SetDlgItemText(IDC_TEXTO, "Houve erro, a superfície NÂO foi gerada\n com êxito.");

    return 1;  //--- retorno NOK
  }
  else 
  {
   
    CSuperficie Superficie(ParametrosGerarSuperficie.String,false);
    if(Superficie.DefineFronteiraOuPoligono() == TRUE) 
    {
      ParametrosGerarSuperficie.CDial->SetDlgItemText(IDC_TEXTO, "A superfície foi gerada com êxito.");
      Superficie.Serialize(ParametrosGerarSuperficie.String,GRAVAR);

      return 0;   //--- retorno OK
    }  
    else 
      return 0;

  }

  ParametrosGerarSuperficie.CDial->SetDlgItemText(IDC_TEXTO, "Houve erro, a fronteira não foi gerada\n com êxito.");
  */
  return 1;   //--- Retorno NOK 
}

/////////////////////////////////////////////////////////////////////////////
// CDGerarSuperficie dialog
CDGerarSuperficie::CDGerarSuperficie(CWnd* pParent,CString NomeProj) : CDialog(CDGerarSuperficie::IDD, pParent), NomeProjeto(NomeProj),pProjeto(NULL) 
{
  //{{AFX_DATA_INIT(CDGerarSuperficie)
  m_CSTexto = _T("");
  //}}AFX_DATA_INIT
}

void CDGerarSuperficie::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDGerarSuperficie)
  DDX_Control(pDX, IDC_BUTCANCELAR, m_buCancelar);
  DDX_Control(pDX, IDC_GERAR, m_buGerar);
  DDX_Control(pDX, IDOK, m_buOK);
  DDX_Control(pDX, IDC_PROGRESS, m_ProProgresso);
  DDX_Text(pDX, IDC_TEXTO, m_CSTexto);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_BUTPAUSA, m_buPausar);
}

BEGIN_MESSAGE_MAP(CDGerarSuperficie, CDialog)
  //{{AFX_MSG_MAP(CDGerarSuperficie)
  ON_BN_CLICKED(IDC_GERAR, OnGerar)
  ON_BN_CLICKED(IDC_BUTCANCELAR, OnButcancelar)
  //}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_BUTPAUSA, OnBnClickedPausar)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDGerarSuperficie message handlers

void CDGerarSuperficie::OnGerar() 
{
  m_buCancelar.EnableWindow(false);
  m_buPausar.EnableWindow(true);
  m_buOK.EnableWindow(false);

  SetClassLong(m_hWnd,GCL_HCURSOR,(LONG)LoadCursor(NULL, IDC_APPSTARTING));

  ParametrosGerarSuperficie.String = NomeProjeto;
  ParametrosGerarSuperficie.CDial = this;

  HANDLE event;
  event = CreateEvent(NULL, TRUE, FALSE, "CONEVENTTEST");
  int erro(0);

  try
  {
    T = AfxBeginThread(ThreadGerarSuperf,&ParametrosGerarSuperficie,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
    T->ResumeThread();             //---  Executa a tread.
  }
  catch(...) 
  {
    SetDlgItemText(IDC_TEXTO,"Houve Erro ao gerar a superficie. O arquivo DXF NÃO foi gerado ou apresenta erros.");

    m_buOK.EnableWindow(true);  
    m_buPausar.EnableWindow(false);
    m_buCancelar.EnableWindow(false);

    UpdateData(false);

    return;
  }

  m_buOK.EnableWindow(true);
  m_buCancelar.EnableWindow(false);
  m_buGerar.EnableWindow(false);
  m_buPausar.EnableWindow(false);

  UpdateData(false);
}

BOOL CDGerarSuperficie::OnInitDialog() 
{
  CDialog::OnInitDialog();

  m_buOK.EnableWindow(false);
  m_buPausar.EnableWindow(false);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDGerarSuperficie::OnButcancelar() 
{
  if(T) T->SuspendThread();

  EndDialog(true);	
}

void CDGerarSuperficie::OnBnClickedPausar()
{
 static bool Estado(true);

 Estado = !Estado;

  if(Estado) 
  {
    m_buPausar.SetWindowText("Pausar");
    m_buCancelar.EnableWindow(FALSE);
    T->ResumeThread();    
  }
  else 
  {
    m_buPausar.SetWindowText("Continuar");  
    m_buCancelar.EnableWindow(TRUE);
    T->SuspendThread();   
  }
}
