// DiaRemAciTopog.cpp : implementation file
//

#include "stdafx.h"
#include "Est.h"
#include "DiaRemAciTopog.h"
#include "afxdialogex.h"


// CDiaRemAciTopog dialog

IMPLEMENT_DYNAMIC(CDiaRemAciTopog, CDialog)

CDiaRemAciTopog::CDiaRemAciTopog(CWnd* pParent,CPoint pLocal,const std::string& pTexto) : CDialog(CDiaRemAciTopog::IDD, pParent), Local(pLocal),Texto(pTexto)
{
}

CDiaRemAciTopog::~CDiaRemAciTopog()
{
}

void CDiaRemAciTopog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDiaRemAciTopog, CDialog)
END_MESSAGE_MAP()

BOOL CDiaRemAciTopog::OnInitDialog()
{
  CDialog::OnInitDialog();

  CRect dialogRect;
  GetWindowRect(&dialogRect);
  ScreenToClient(dialogRect);

  MoveWindow(Local.x,Local.y,dialogRect.right,dialogRect.bottom);

  if(Texto.size() > 0) GetDlgItem(IDC_STATIC)->SetWindowText(Texto.c_str());

  GetDlgItem(IDCANCEL)->SetFocus();
  
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}
