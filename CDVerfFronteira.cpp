// CDVerfFronteira.cpp : implementation file
//

#include "stdafx.h"
#include "Est.h"
#include "CDVerfFronteira.h"
#include "afxdialogex.h"


// CDVerfFronteira dialog

IMPLEMENT_DYNAMIC(CDVerfFronteira, CDialog)

CDVerfFronteira::CDVerfFronteira(CWnd* pParent /*=NULL*/)
	: CDialog(CDVerfFronteira::IDD, pParent)
{

  m_strMensagem = _T("");
}

CDVerfFronteira::~CDVerfFronteira()
{
}

void CDVerfFronteira::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_STAMENSAGEM, m_strMensagem);
	DDV_MaxChars(pDX, m_strMensagem, 500);
}


BEGIN_MESSAGE_MAP(CDVerfFronteira, CDialog)
  ON_BN_CLICKED(IDC_BUTLOG, &CDVerfFronteira::OnClickedButlog)
END_MESSAGE_MAP()


void CDVerfFronteira::OnClickedButlog()
{
  // TODO: Add your control notification handler code here
}

void CDVerfFronteira::MostraMensagem(const std::string& parMensagem)
{
  m_strMensagem = parMensagem.c_str();

  UpdateData(FALSE);
}
