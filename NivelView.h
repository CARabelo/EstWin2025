// NivelView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNivelView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CNivelView : public CTypedPtrListView
{
protected:

  CNivelView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CNivelView)

  // Form Data
public:
  //{{AFX_DATA(CNivelView)
  enum { IDD = IDD_NIVELAMENTO };
  CString m_Campo1;
  CString m_Campo2;
  CString m_Campo3;
  CString m_Campo4;
  CString m_Campo5;
  CString	m_campo10;
  //}}AFX_DATA

  // Attributes
public:

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CNivelView)
public:
  virtual void OnInitialUpdate();
  virtual void Serialize(CArchive& ar);
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~CNivelView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  //{{AFX_MSG(CNivelView)
  afx_msg void OnAdd();
  afx_msg void OnRemove();
  afx_msg void OnUpdate();
  afx_msg void OnRemoveAll();
  afx_msg void OnInsertBefore();
  afx_msg void OnSelChangeList();
  afx_msg int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
