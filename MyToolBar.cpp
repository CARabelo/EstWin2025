// MyToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "MyToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define	WM_CLOSE_TOOLBAR (WM_APP+100)

/////////////////////////////////////////////////////////////////////////////
// CMyToolBar

//--- Esta classe foi criada apenas para se poder pegar as coordenadas do toolbar no fechamento
//--- Para poder posicionalas ondes estavam quando foram dechadas

CMyToolBar::CMyToolBar()
{
}

CMyToolBar::~CMyToolBar()
{

}

BEGIN_MESSAGE_MAP(CMyToolBar, CToolBar)
  ON_WM_WINDOWPOSCHANGING()
//  ON_WM_CLOSE()
  ON_WM_CLOSE()
  ON_WM_CLOSE()
  ON_WM_SHOWWINDOW()
  ON_WM_XBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyToolBar message handlers

void CMyToolBar::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
  if((lpwndpos->flags & SWP_HIDEWINDOW) && ((this->GetParentFrame())->m_hWnd !=(this->GetTopLevelFrame())->m_hWnd)) 
  { 
    if(IsFloating())
    {
      CWnd* WndOwner(GetOwner());

      if(WndOwner) WndOwner->SendMessageToDescendants(WM_CLOSE_TOOLBAR,IsWindowVisible(),GetDlgCtrlID());
    }
  } 
} 

