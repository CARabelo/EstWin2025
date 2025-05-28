// CDiaLembretes.cpp : implementation file
//

#include "stdafx.h"
#include "Est.h"
#include "meleditbox.h"
#include "CDiaLembretes.h"
#include "afxdialogex.h"

// CDiaLembretes dialog

IMPLEMENT_DYNAMIC(CDiaLembretes, CDialog)

CDiaLembretes::CDiaLembretes(CWnd* pParent,std::string& pstrTitulo,std::string& pstrTexto,COLORREF& CorBandeira)	: CDialog(CDiaLembretes::IDD, pParent),
                            m_strLembrete(pstrTexto.c_str()), m_strTitulo(pstrTitulo.c_str()),strTitulo(pstrTitulo),strTexto(pstrTexto),Cor(CorBandeira)
{
}

CDiaLembretes::~CDiaLembretes()
{
}

void CDiaLembretes::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDILEMBRETE, m_strLembrete);
  DDV_MaxChars(pDX, m_strLembrete, 1000);
  DDX_Text(pDX, IDC_EDITITULO, m_strTitulo);
	DDV_MaxChars(pDX, m_strTitulo, 100);
  DDX_Control(pDX, IDC_EDIAZUL,m_EdiAzul);
  DDX_Control(pDX, IDC_EDIVERMELHO, m_EdiVermelho);
}

BEGIN_MESSAGE_MAP(CDiaLembretes, CDialog)
  ON_BN_CLICKED(IDOK, &CDiaLembretes::OnBnClickedOk)
END_MESSAGE_MAP()

// CDiaLembretes message handlers

void CDiaLembretes::OnBnClickedOk()
{
  UpdateData(true);

  strTitulo = m_strTitulo.GetBuffer();
  strTexto = m_strLembrete.GetBuffer();
  Cor = ((CButton*) GetDlgItem(IDC_RADAZUL))->GetCheck() == TRUE ? RGB(255,0,0) : RGB(0,0,255);

  // TODO: Add your control notification handler code here
  CDialog::OnOK();
}


BOOL CDiaLembretes::OnInitDialog()
{
  CDialog::OnInitDialog();

  ((CButton *) GetDlgItem(IDC_RADAZUL))->SetCheck(TRUE);
  /*
  m_EdiAzul.MudaCorFundo(RGB(0,0,191));
  m_EdiVermelho.MudaCorFundo(RGB(191,0,0));

  m_EdiAzul.Invalidate();
  m_EdiVermelho.Invalidate();
  */
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}
