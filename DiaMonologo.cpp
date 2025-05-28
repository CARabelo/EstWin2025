// DiaMonologo.cpp : implementation file
//

#include "stdafx.h"
#include "Est.h"
#include <string>
#include "DiaMonologo.h"
//#include "afxdialogex.h"

// CDiaMonologo dialog

IMPLEMENT_DYNAMIC(CDiaMonologo, CDialog)

CDiaMonologo::CDiaMonologo(std::string strMsg,bool bMostrarIgn, CWnd* pParent)	: 
               CDialog(CDiaMonologo::IDD, pParent),strMensagem(strMsg),bMostrarIgnorar(bMostrarIgn)
{
}

void CDiaMonologo::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_STAMEMSAGEM, m_STAMensagem);
  DDX_Control(pDX, IDC_CHEIGNORAR, m_CHEIgnorar);
}

BEGIN_MESSAGE_MAP(CDiaMonologo, CDialog)
  ON_WM_KEYUP()
END_MESSAGE_MAP()

// CDiaMonologo message handlers

BOOL CDiaMonologo::OnInitDialog() 
{
  CDialog::OnInitDialog();

  if(bMostrarIgnorar) m_CHEIgnorar.ShowWindow(true);

  SetDlgItemText(IDC_STAMEMSAGEM,strMensagem.c_str());

  UpdateData(FALSE);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}
