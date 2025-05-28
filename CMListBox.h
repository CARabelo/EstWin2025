#pragma once
#include <afxwin.h>
class CMListBox :   public CListBox
{
  void* pDiaParent;

public:

    CMListBox(void* ppParent) : CListBox(),pDiaParent(ppParent)
    {
    }

  DECLARE_MESSAGE_MAP()
  afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

