// MelEditBox.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "MelEditBox.h"
#include <string>
#include "cdpopuppontos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMelEditBox

CMelEditBox::CMelEditBox(COLORREF CorFun):CorFundo(CorFun),pBrush(new CBrush(CorFundo)){}

CMelEditBox::~CMelEditBox()
{
  if(pBrush) 
  {
    DeleteObject(*pBrush);
    delete pBrush;
  }
}

BEGIN_MESSAGE_MAP(CMelEditBox, CStatic)
  //{{AFX_MSG_MAP(CMelEditBox)
  ON_WM_ERASEBKGND()
  ON_WM_CTLCOLOR_REFLECT()
  ON_WM_LBUTTONUP()
  //}}AFX_MSG_MAP
  ON_WM_LBUTTONUP()
  ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMelEditBox message handlers

BOOL CMelEditBox::OnEraseBkgnd(CDC* pDC) 
{
  CStatic::OnEraseBkgnd(pDC) ;

  return TRUE;                                     // Prevents the execution of return
}

HBRUSH CMelEditBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
  //--- Troca a brocha por outra com a cor atual 
  
  pDC->SetBkColor(CorFundo);
 
  return *pBrush;
}

void CMelEditBox::TrocaCor(COLORREF Cor) 
{
  CorFundo = Cor;

  if(pBrush) 
  {
    DeleteObject(*pBrush);
    delete pBrush;
  }

  pBrush = new CBrush(CorFundo);

  Invalidate();
}

int CMelEditBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  lpCreateStruct->dwExStyle |= SS_NOTIFY;

  if (CStatic::OnCreate(lpCreateStruct) == -1)
    return -1;

  // TODO:  Add your specialized creation code here

  return 0;
}
