// CurVerView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCurVerView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CCurVerView : public CTypedPtrListView
{
protected:
  CCurVerView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CCurVerView)

  int Consiste(tlstwstr& ListaCampos);
  bool ConsistePIVS();

  // Form Data
public:
  //{{AFX_DATA(CCurVerView)
  enum { IDD = IDD_CURVASVERTICAIS};
  CString	m_Campo1;
  CString	m_Campo2;
  CString	m_Campo3;
  CString	m_Campo4;
  CString	m_Campo5;
  //}}AFX_DATA

  // Attributes
public:
 CEditEx EdUltimoCampo,EdPrimeiroCampo;

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCurVerView)
public:
  virtual void OnInitialUpdate();
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~CCurVerView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  //{{AFX_MSG(CCurVerView)
  afx_msg void OnAdd();
  afx_msg void OnRemove();
  afx_msg void OnUpdate();
  afx_msg void OnRemoveAll();
  afx_msg void OnInsertBefore();
  afx_msg void OnSelChangeList();
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedGravar();
};

/////////////////////////////////////////////////////////////////////////////
