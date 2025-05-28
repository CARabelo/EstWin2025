// AlaSarPasView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlaSarPasView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CAlaSarPasView : public CTypedPtrListView
{
protected:
  CAlaSarPasView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CAlaSarPasView)

  int Consiste(tlstwstr& ListaCampos);

  // Form Data
public:
  enum { IDD = IDD_ALASARPAS};
  // Attributes
public:

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
public:
  virtual void OnInitialUpdate();
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  // Implementation
protected:
  CEditEx EdUltimoCampo,EdPrimeiroCampo;

  virtual ~CAlaSarPasView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  afx_msg void OnAdd();
  afx_msg void OnRemove();
  afx_msg void OnUpdate();
  afx_msg void OnRemoveAll();
  afx_msg void OnInsertBefore();
  afx_msg void OnSelChangeList();
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

  DECLARE_MESSAGE_MAP()

public:

  afx_msg void OnClickedGravar();
  afx_msg void OnClickedLimparCampos();
};

/////////////////////////////////////////////////////////////////////////////
