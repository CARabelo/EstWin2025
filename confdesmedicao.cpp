// ConfDesMedicao.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "ConfDesMedicao.h"
#include <string>
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfDesMedicao dialog


CConfDesMedicao::CConfDesMedicao(CWnd* pParent /*=NULL*/)
: CDialog(CConfDesMedicao::IDD, pParent)
{
  //{{AFX_DATA_INIT(CConfDesMedicao)
  m_ComboquanMedicoes = _T("");
  //}}AFX_DATA_INIT
}


void CConfDesMedicao::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CConfDesMedicao)
  DDX_Control(pDX, IDC_COMBO1, m_CBQuanMedicoes);
  DDX_CBString(pDX, IDC_COMBO1, m_ComboquanMedicoes);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfDesMedicao, CDialog)
  //{{AFX_MSG_MAP(CConfDesMedicao)
  ON_WM_CLOSE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfDesMedicao message handlers

void CConfDesMedicao::OnClose() 
{
  // TODO: Add your message handler code here and/or call default

  AfxGetApp()->WriteProfileString(_T("DadosDesMedicao"),_T("QuanMedicoesADesenhar"),m_ComboquanMedicoes);	

  ((CMainFrame*)AfxGetMainWnd())->QMedADesenhar = atoi(m_ComboquanMedicoes);    //--- Passa para o MainFrame

  CDialog::OnClose();
}

BOOL CConfDesMedicao::OnInitDialog() 
{
  CDialog::OnInitDialog();

  m_ComboquanMedicoes = (AfxGetApp()->GetProfileString(_T("DadosDesMedicao"),_T("QuanMedicoesADesenhar"),_T("1"))); 

  UpdateData(FALSE);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfDesMedicao::OnOK() 
{
  // TODO: Add extra validation here

  UpdateData(TRUE);

  AfxGetApp()->WriteProfileString(_T("DadosDesMedicao"),_T("QuanMedicoesADesenhar"),m_ComboquanMedicoes);	

  ((CMainFrame*)AfxGetMainWnd())->QMedADesenhar = atoi(m_ComboquanMedicoes);    //--- Passa para o MainFrame

  CDialog::OnOK();
}
