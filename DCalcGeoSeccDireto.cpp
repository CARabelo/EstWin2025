// DCalcGeoSeccDireto.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "DCalcGeoSeccDireto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCalcGeoSeccDireto dialog


CDCalcGeoSeccDireto::CDCalcGeoSeccDireto(CWnd* pParent) : CDialog(CDCalcGeoSeccDireto::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDCalcGeoSeccDireto)
  m_CSExecutando = _T("");
  m_CSExecutando2 = _T("");
  //}}AFX_DATA_INIT
}


void CDCalcGeoSeccDireto::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDCalcGeoSeccDireto)
  DDX_Control(pDX, IDC_PROGRESS1, m_Progresso);
  DDX_Text(pDX, IDC_EXECUTANDO, m_CSExecutando);
  DDX_Text(pDX, IDC_EXECUTANDO2, m_CSExecutando2);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDCalcGeoSeccDireto, CDialog)
  //{{AFX_MSG_MAP(CDCalcGeoSeccDireto)
  // NOTE: the ClassWizard will add message map macros here
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCalcGeoSeccDireto message handlers
