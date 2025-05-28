// PogressToolMF.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "PogressToolMF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPogressToolMF dialog


CPogressToolMF::CPogressToolMF(CWnd* pParent /*=NULL*/)
	: CDialog(CPogressToolMF::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPogressToolMF)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPogressToolMF::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPogressToolMF)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPogressToolMF, CDialog)
	//{{AFX_MSG_MAP(CPogressToolMF)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPogressToolMF message handlers
