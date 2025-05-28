// DiaAbrirCriarArea.cpp : implementation file
//

#include "stdafx.h"
#include "Est.h"
#include "DiaAbrirCriarArea.h"
#include "afxdialogex.h"

// CDiaAbrirCriarArea dialog

IMPLEMENT_DYNAMIC(CDiaAbrirCriarArea, CDialog)

CDiaAbrirCriarArea::CDiaAbrirCriarArea(int* ppResposta,CWnd* pParent /*=NULL*/) : CDialog(CDiaAbrirCriarArea::IDD, pParent),pResp(ppResposta)
, m_boolCriar(1), m_boolAbrir(0)
{
}

CDiaAbrirCriarArea::~CDiaAbrirCriarArea()
{
}

void CDiaAbrirCriarArea::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  DDX_Check(pDX, IDC_RADABRIR, m_boolAbrir);
  DDX_Check(pDX, IDC_RADCRIAR, m_boolCriar);
}

BEGIN_MESSAGE_MAP(CDiaAbrirCriarArea, CDialog)
  ON_BN_CLICKED(IDOK, &CDiaAbrirCriarArea::OnBnClickedOk)
  ON_BN_CLICKED(IDC_CANCEL, &CDiaAbrirCriarArea::OnBnClickedCancel)
END_MESSAGE_MAP()

void CDiaAbrirCriarArea::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here

  UpdateData(TRUE);

  *pResp = m_boolCriar == 1; 

  CDialog::OnOK();
}

void CDiaAbrirCriarArea::OnBnClickedCancel()
{
    *pResp = -1;
    EndDialog(true);	
}
