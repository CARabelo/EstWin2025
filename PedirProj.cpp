// PedirProj.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "PedirProj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PedirProj dialog


PedirProj::PedirProj(CWnd* pParent /*=NULL*/)
: CDialog(PedirProj::IDD, pParent)
{
  //{{AFX_DATA_INIT(PedirProj)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT

  Projeto = "";
}


void PedirProj::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(PedirProj)
  // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PedirProj, CDialog)
  //{{AFX_MSG_MAP(PedirProj)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PedirProj message handlers

void PedirProj::OnOK() 
{
  // TODO: Add extra validation here

  GetDlgItemText(IDC_NOMEPROJ,Projeto);

  CDialog::OnOK();
}
