// CurVerView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCurVerView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CSecoesAvulsasView : public CTypedPtrListView
{
protected:
  CSecoesAvulsasView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CSecoesAvulsasView)

  int Consiste(tlstwstr& ListaCampos);

  // Form Data
public:

  enum { IDD = IDD_SECOESAVULSAS};
  // Attributes
public:

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
  virtual ~CSecoesAvulsasView();
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
  afx_msg void OnBnClickedGravar();
};

/////////////////////////////////////////////////////////////////////////////
