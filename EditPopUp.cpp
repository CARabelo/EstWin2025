// EditPopUp.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "EditPopUp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditPopUp

CEditPopUp::CEditPopUp()
{
}

CEditPopUp::~CEditPopUp()
{
}

BEGIN_MESSAGE_MAP(CEditPopUp, CStatic)
  //{{AFX_MSG_MAP(CEditPopUp)
  ON_WM_ERASEBKGND()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEACTIVATE()
  ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
  ON_WM_SETFOCUS()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditPopUp message handlers
/*
BOOL CEditPopUp::OnEraseBkgnd(CDC* pDC) 
{

CBrush brNew(RGB(255,255,255));                              //--- Cria a brocha
CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brNew);

CRect rc;
GetClientRect(&rc);                                         //--- Pega a área cliente     
pDC->FillSolidRect( rc,RGB(255,255,255));		                //--- Pìnta o fundo

return TRUE;                                     
}

*/

BOOL CEditPopUp::OnEraseBkgnd(CDC* pDC) 
{
  // TODO: Add your message handler code here and/or call default

  CBrush brNew(RGB(255,255,255));                              //--- Cria a brocha
  CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&brNew);

  CRect rc;
  GetClientRect(&rc);                                         //--- Pega a área cliente     
  pDC->FillSolidRect( rc,RGB(255,255,255));		                //--- Pìnta o fundo

  pDC->SelectObject(pOldBrush);

  return TRUE;                                     


  //	return CStatic::OnEraseBkgnd(pDC);
}

void CEditPopUp::OnLButtonUp(UINT nFlags, CPoint point) 
{
  // TODO: Add your message handler code here and/or call default

  CStatic::OnLButtonUp(nFlags, point);
}

int CEditPopUp::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
  // TODO: Add your message handler code here and/or call default

  return CStatic::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CEditPopUp::OnClicked() 
{
  // TODO: Add your control notification handler code here
  int i = 0;

}

void CEditPopUp::OnSetFocus(CWnd* pOldWnd) 
{
  CStatic::OnSetFocus(pOldWnd);

  // TODO: Add your message handler code here

}
