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
  enum { IDD = IDD_OBRASARTECORRENTES};
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
  virtual ~COACView();
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
  afx_msg void OnBnClickedGravar();
};

/////////////////////////////////////////////////////////////////////////////
