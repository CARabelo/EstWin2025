// OAEView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COAEView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class COAEView : public CTypedPtrListView
{
protected:
  COAEView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(COAEView)

  // Form Data
public:
  //{{AFX_DATA(COAEView)
  enum { IDD = IDD_OBRASARTEESPECIAIS};
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
   CEditEx EdUltimoCampo,EdPrimeiroCampo;

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides

public:
  virtual void OnInitialUpdate();
  virtual void Serialize(CArchive& ar);
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  // Implementation
protected:
  virtual ~COAEView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  int Consiste(tlstwstr& ListaCampos);

  // Generated message map functions
  afx_msg void OnAdd();
  afx_msg void OnRemove();
  afx_msg void OnUpdate();
  afx_msg void OnRemoveAll();
  afx_msg void OnInsertBefore();
  afx_msg void OnSelChangeList();
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeDescricaooae();
  afx_msg void OnBnClickedGravar();
};

/////////////////////////////////////////////////////////////////////////////
