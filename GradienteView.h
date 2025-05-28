// TresPontosView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGradienteView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CGradienteView : public CTypedPtrListView
{
protected:
  
  CSecTipo SecoesTipo; 

  CGradienteView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CGradienteView)

  int Consiste(tlstwstr& ListaCampos);

  // Form Data
public:
  //{{AFX_DATA(CGradienteView)
  enum { IDD = IDD_GRADIENTES};
  //}}AFX_DATA

  // Attributes
public:
 CEditEx EdUltimoCampo,EdPrimeiroCampo;

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CGradienteView)
public:
  virtual void OnInitialUpdate();
  virtual void Serialize(CArchive& ar);
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~CGradienteView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  //{{AFX_MSG(CGradienteView)
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
