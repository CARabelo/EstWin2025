// CDMensagem.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "CDMensagem.h"
#include "afxdialogex.h"


// caixa de diálogo de CDMensagem

IMPLEMENT_DYNAMIC(CDMensagem, CDialogEx)
CDMensagem::CDMensagem(CWnd* pParent)	: CDialog(IDD_MENSAGEM,pParent)
{
}

CDMensagem::~CDMensagem()
{
}

void CDMensagem::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_STAMENSAGEM, StaMensagem);
}

BEGIN_MESSAGE_MAP(CDMensagem, CDialog)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDMensagem

BOOL CDMensagem::OnInitDialog()
{
  CDialog::OnInitDialog();

  // TODO:  Adicione inicialização extra aqui

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: as páginas de propriedade OCX devem retornar FALSE
}
BOOL CDMensagem::Create(CWnd * pParent)
{
  return CDialog::Create(IDD_MENSAGEM, pParent);
}