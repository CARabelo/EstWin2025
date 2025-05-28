// dialogo.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "dialogo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dialogo dialog

dialogo::dialogo(CWnd* pParent): CDialog(dialogo::IDD, pParent)
{     
  mensagens[0] = "Est�o os dados corretos?"; 
  mensagens[1] = "Diret�rio n�o encontrado, criar?";
  mensagens[2] = "Projeto n�o encontrado, criar?";
  mensagens[3] = "Arquivo inv�lido.";
  mensagens[4] = "Houve altera��o de dados, deseja grav�-las?";
  mensagens[5] = "Confirma a remo��o desta curva?";
  mensagens[6] = "Deseja mesmo perder todas as altera��es feitas \r\n nesta edi��o???";
  mensagens[7] = "Os volumes n�o ficar�o de acordo com as se��es (voc� dever� solicitar o c�lculo posteriormente), confirma?";
  mensagens[8] = "Deseja sobrescrever o relat�rio existente?";
  mensagens[9] = "Confirma a remo��o deste PIV?";
  mensagens[10] = "Confirma o c�lculo dos volumes? (As altera��es feitas ser�o gravadas.)";
  mensagens[11] = "As altera��es ser�o gravadas, confirma?"; 
  mensagens[12] = "Confirma o c�lculo geom�trico? (O greide precisar� de ser reajustado e a nota de servi�o sera cancelada)";
  mensagens[13] = "Confirma a remo��o desta se��o avulsa?";
  mensagens[14] = "";

  //{{AFX_DATA_INIT(dialogo)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void dialogo::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(dialogo)
  // NOTE: the ClassWizard will add DDX and DDV calls here
  DDX_Control(pDX, IDC_RAD_SECCIONAR, CBRadSeccionar);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(dialogo, CDialog)
  //{{AFX_MSG_MAP(dialogo)
  ON_WM_CREATE()
  //}}AFX_MSG_MAP
  ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dialogo message handlers

int dialogo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CDialog::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO: Add your specialized creation code here

  return 0;
}

BOOL dialogo::OnInitDialog()     
{
  CDialog::OnInitDialog();

  if(NumeroMensagem == 12)
  {
    CBRadSeccionar.EnableWindow(false);
    CBRadSeccionar.ShowWindow(SW_HIDE);
  }
  else 
  {
    CBRadSeccionar.EnableWindow(false);
    CBRadSeccionar.ShowWindow(SW_HIDE);
  }
  SetDlgItemText(IDC_STATIC,texto); 
  SetDlgItemText(IDC_STATIC1,complemento);

  return TRUE;  // return TRUE  unless you set the focus to a control
}

INT_PTR dialogo::DoModal(int NumMensagem,const CString& Complemento,bool* Resecionar)
{
  NumeroMensagem = NumMensagem;
  if(NumMensagem != -1) texto = mensagens[NumMensagem];                      
  else texto.Empty();
  complemento = Complemento;

  pResec = Resecionar;

  return CDialog::DoModal();
}

void dialogo::OnDestroy()
{
  if(pResec && CBRadSeccionar.GetCheck()) *pResec = true;

  CDialog::OnDestroy();
}
