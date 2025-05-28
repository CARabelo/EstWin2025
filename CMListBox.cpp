#include "stdafx.h"
#include "CMListBox.h"

BEGIN_MESSAGE_MAP(CMListBox, CListBox)
	ON_WM_RBUTTONUP()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

void CMListBox::OnRButtonUp(UINT nFlags, CPoint point)
{
  CListBox::OnRButtonUp(nFlags, point);
  ((CView*)pDiaParent)->SendMessage(WM_RBUTTONUP,(DWORD) WM_BUTUP_LISTBOX_PROJETOS,(DWORD)WM_BUTUP_LISTBOX_PROJETOS);
}

void CMListBox::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if(nChar == VK_DELETE)
  {
    ((CView*)pDiaParent)->SendMessage(WM_KEYUP, nChar,(DWORD)WM_BUTUP_LISTBOX_PROJETOS);
  }
	CListBox::OnKeyUp(nChar, nRepCnt, nFlags);
}
