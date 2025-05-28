// DPontosGeo.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "DPontosGeo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DPontosGeo dialog


DPontosGeo::DPontosGeo(CWnd* pParent): CDialog(DPontosGeo::IDD, pParent)
{
	//{{AFX_DATA_INIT(DPontosGeo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DPontosGeo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DPontosGeo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DPontosGeo, CDialog)
	//{{AFX_MSG_MAP(DPontosGeo)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DPontosGeo message handlers
