// CMensagem.cpp : implementation file
//

#include "stdafx.h"
#include "Est.h"
#include "CMensagem.h"
#include "afxdialogex.h"

// CMensagem dialog

IMPLEMENT_DYNAMIC(CMensagem, CDialog)

CMensagem::CMensagem(CWnd* pParent) : CDialog(CMensagem::IDD, pParent)
{
}

CMensagem::~CMensagem()
{
}

void CMensagem::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
 
  DDX_Text(pDX, IDC_STAMENSAGEM, m_strMensagem);
  DDV_MaxChars(pDX, m_strMensagem, 1000);

}

BEGIN_MESSAGE_MAP(CMensagem, CDialog)
  ON_WM_CREATE()
  ON_COMMAND(IDC_CANCEL, &CMensagem::OnCancel)
  ON_COMMAND(IDC_OK, &CMensagem::OnOk)
  ON_WM_DESTROY()
  ON_WM_NCDESTROY()
END_MESSAGE_MAP()

int CMensagem::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CDialog::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO:  Add your specialized creation code here

  return 0;
}

void CMensagem::MostraMensagem(const std::string& Msg) 
{
  SetDlgItemText(IDC_STAMENSAGEM,Msg.c_str());

  Invalidate(true);

  ShowWindow(SW_SHOW);

}

void CMensagem::Apaga() 
{
}

BOOL CMensagem::OnInitDialog()
{
  CDialog::OnInitDialog();


  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}


void CMensagem::OnCancel()
{
  DestroyWindow();
}

void CMensagem::OnOk()
{
  DestroyWindow();
}

void CMensagem::PostNcDestroy()
{
  delete this;
}
