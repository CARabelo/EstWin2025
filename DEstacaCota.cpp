// DEstacaCota.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "DEstacaCota.h"
#include "supercstring.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDEstacaCota dialog


CDEstacaCota::CDEstacaCota(CWnd* pParent):CDialog(CDEstacaCota::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDEstacaCota)
  m_DOCota = 0.0;
  m_EDEstaca = _T("");
  //}}AFX_DATA_INIT
}


void CDEstacaCota::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDEstacaCota)
  DDX_Control(pDX, IDC_EDESTACA, m_CEDEstaca);
  DDX_Text(pDX, IDC_EDCOTA, m_DOCota);
  DDX_Text(pDX, IDC_EDESTACA, m_EDEstaca);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDEstacaCota, CDialog)
  //{{AFX_MSG_MAP(CDEstacaCota)
  ON_EN_KILLFOCUS(IDC_EDESTACA, OnKillfocusEdestaca)
  ON_EN_KILLFOCUS(IDC_EDCOTA, OnKillfocusEdcota)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDEstacaCota message handlers

void CDEstacaCota::OnOK() 
{
  CDialog::OnOK();
}

void CDEstacaCota::OnKillfocusEdestaca() 
{
  UpdateData(true);

  SuperCString EstacaDigitada(m_EDEstaca);

  if(!EstacaDigitada.EEstaca())
  {
    AfxMessageBox("Estaca Inválida");
    m_CEDEstaca.SetFocus();
  }
}

void CDEstacaCota::OnKillfocusEdcota() 
{
  // TODO: Add your control notification handler code here

}

