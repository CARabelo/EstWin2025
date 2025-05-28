#if !defined(AFX_MYTOOLBAR_H__9F51BE33_F41D_11D3_86DC_6EBD8D000000__INCLUDED_)
#define AFX_MYTOOLBAR_H__9F51BE33_F41D_11D3_86DC_6EBD8D000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MyToolBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyToolBar window

class CMyToolBar : public CToolBar
{
  // Construction
public:
  CMyToolBar();

  // Attributes
public:
  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMyToolBar)
  //}}AFX_VIRTUAL

  // Implementation
public:

  // CRect& PegaPos() { return Posicao; };

  virtual ~CMyToolBar();

  // Generated message map functions
protected:
  //{{AFX_MSG(CMyToolBar)
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTOOLBAR_H__9F51BE33_F41D_11D3_86DC_6EBD8D000000__INCLUDED_)
