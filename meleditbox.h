#if !defined(AFX_MELEDITBOX_H__BC183CA2_EEDD_41F7_B834_A8E551B1757B__INCLUDED_)
#define AFX_MELEDITBOX_H__BC183CA2_EEDD_41F7_B834_A8E551B1757B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MelEditBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMelEditBox window

class CMelEditBox : public CStatic
{
  COLORREF CorFundo;
  CBrush* pBrush;

public:

  CMelEditBox(COLORREF CorFun = 0);
  void TrocaCor(COLORREF Cor);
  COLORREF PegaCorFundo() { return CorFundo;}
  // Attributes
public:

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMelEditBox)
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~CMelEditBox();
  void MudaCorFundo(COLORREF Cor){ CorFundo = Cor;};

  // Generated message map functions
protected:
  //{{AFX_MSG(CMelEditBox)
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_MELEDITBOX_H__BC183CA2_EEDD_41F7_B834_A8E551B1757B__INCLUDED_)
