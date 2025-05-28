#if !defined(AFX_EDITPOPUP_H__00E68E01_03D4_11D9_AFF6_000AE6E79B25__INCLUDED_)
#define AFX_EDITPOPUP_H__00E68E01_03D4_11D9_AFF6_000AE6E79B25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditPopUp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditPopUp window

class CEditPopUp : public CStatic
{
  // Construction
public:
  CEditPopUp();

  // Attributes
public:

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CEditPopUp)
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~CEditPopUp();

  // Generated message map functions
protected:
  //{{AFX_MSG(CEditPopUp)
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
  afx_msg void OnClicked();
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITPOPUP_H__00E68E01_03D4_11D9_AFF6_000AE6E79B25__INCLUDED_)
