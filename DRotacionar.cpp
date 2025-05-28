// DRotacionar.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "DRotacionar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDRotacionar dialog
CDRotacionar::CDRotacionar(CWnd* pParent,double AnguloAtual,double* NovoAngulo,int* AbsolutoRelativo):CDialog(CDRotacionar::IDD,pParent),piNovoAngulo(NovoAngulo),piAbsolutoRelativo(AbsolutoRelativo)
{
  //{{AFX_DATA_INIT(CDRotacionar)
  m_iNovoAngulo = 0;
  m_iAnguloAtual = AnguloAtual;
  //}}AFX_DATA_INIT
}

void CDRotacionar::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDRotacionar)
  DDX_Control(pDX, IDC_RARLATIVO, m_CBRelativo);
  DDX_Control(pDX, IDC_RAABSOLUTO, m_CBAbsoluto);
  DDX_Text(pDX, IDC_EDNOVOANGULO, m_iNovoAngulo);
  DDX_Text(pDX, IDC_EDANGULOATUAL, m_iAnguloAtual);
  //}}AFX_DATA_MAP
  DDV_MinMaxInt(pDX, m_iNovoAngulo, -359, 359);
}

BEGIN_MESSAGE_MAP(CDRotacionar, CDialog)
  //{{AFX_MSG_MAP(CDRotacionar)
  //}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_BUTLINHABASE, &CDRotacionar::OnBnClickedButlinhabase)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDRotacionar message handlers

void CDRotacionar::OnOK() 
{
  // TODO: Add extra validation here

  UpdateData(true);

  *piNovoAngulo = m_iNovoAngulo; 
  *piAbsolutoRelativo = m_CBAbsoluto.GetCheck();

  CDialog::OnOK();
}

void CDRotacionar::OnCancel() 
{
  CDialog::OnCancel();
}

BOOL CDRotacionar::OnInitDialog() 
{
  CDialog::OnInitDialog();

  if(*piAbsolutoRelativo == 100) 
  {
    m_CBAbsoluto.SetCheck(true);
    m_CBRelativo.EnableWindow(false);
  }
  else m_CBRelativo.SetCheck(true);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDRotacionar::OnBnClickedButlinhabase()
{
  *piNovoAngulo = -10000.0;

  OnCancel();
}
