// OAEView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COACView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class COACView : public CTypedPtrListView
{
protected:
  COACView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(COACView)

  // Form Data
public:
  //{{AFX_DATA(COACView)
  enum { IDD = IDD_OBRASARTECORRENTES};
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
  //{{AFX_VIRTUAL(COACView)
public:
  virtual void OnInitialUpdate();
  virtual void Serialize(CArchive& ar);
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~COACView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  int Consiste(CString* Dados);


  // Generated message map functions
  //{{AFX_MSG(COACView)
  afx_msg void OnAdd();
  afx_msg void OnRemove();
  afx_msg void OnUpdate();
  afx_msg void OnRemoveAll();
  afx_msg void OnInsertBefore();
  afx_msg void OnSelChangeList();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedGravar();
};

/////////////////////////////////////////////////////////////////////////////
