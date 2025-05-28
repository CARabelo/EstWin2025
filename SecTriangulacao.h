// TresPontosView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSecTRiangView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CSecTRiangView : public CTypedPtrListView
{
protected:
  CSecTRiangView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CSecTRiangView)

  // Form Data
public:
  //{{AFX_DATA(CSecTRiangView)
  enum { IDD = IDD_TERRTRIANGULADO};
  CString	m_Campo1;
  CString	m_Campo2;
  CString	m_Campo3;
  CString	m_Campo4;
  CString	m_Campo5;
  CString	m_Campo6;
  CString	m_Campo7;
  CString	m_Campo8;
  CString	m_Campo9;
  CString	m_Campo10;
  CString m_Campo11;
  //}}AFX_DATA

  // Attributes
public:

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSecTRiangView)
public:
  virtual void OnInitialUpdate();
  virtual void Serialize(CArchive& ar);
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~CSecTRiangView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  int Consiste(tlstwstr& ListaCampos);

  // Generated message map functions
  //{{AFX_MSG(CSecTRiangView)
  afx_msg void OnAdd();
  afx_msg void OnRemove();
  afx_msg void OnUpdate();
  afx_msg void OnRemoveAll();
  afx_msg void OnInsertBefore();
  afx_msg void OnSelChangeList();
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
