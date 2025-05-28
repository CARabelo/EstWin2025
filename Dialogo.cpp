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
  mensagens[0] = "Estão os dados corretos?"; 
  mensagens[1] = "Diretório não encontrado, criar?";
  mensagens[2] = "Projeto não encontrado, criar?";
  mensagens[3] = "Arquivo inválido.";
  mensagens[4] = "Houve alteração de dados, deseja gravá-las?";
  mensagens[5] = "Confirma a remoção desta curva?";
  mensagens[6] = "Deseja mesmo perder todas as alterações feitas \r\n nesta edição???";
  mensagens[7] = "Os volumes não ficarão de acordo com as seções (você deverá solicitar o cálculo posteriormente), confirma?";
  mensagens[8] = "Deseja sobrescrever o relatório existente?";
  mensagens[9] = "Confirma a remoção deste PIV?";
  mensagens[10] = "Confirma o cálculo dos volumes? (As alterações feitas serão gravadas.)";
  mensagens[11] = "As alterações serão gravadas, confirma?"; 
  mensagens[12] = "Confirma o cálculo geométrico? (O greide precisará de ser reajustado e a nota de serviço sera cancelada)";
  mensagens[13] = "Confirma a remoção desta seção avulsa?";
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
