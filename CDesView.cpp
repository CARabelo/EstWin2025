// CDesView.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "CDesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesView

IMPLEMENT_DYNCREATE(CDesView, CView)

CDesView::CDesView()
{
}

CDesView::~CDesView()
{
}

BEGIN_MESSAGE_MAP(CDesView, CView)
	//{{AFX_MSG_MAP(CDesView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesView drawing

void CDesView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CDesView diagnostics

#ifdef _DEBUG
void CDesView::AssertValid() const
{
	CView::AssertValid();
}

void CDesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDesView message handlers

