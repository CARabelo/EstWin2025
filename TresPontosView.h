// TresPontosView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTresPontosView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CTresPontosView : public CTypedPtrListView
{
protected:
  CTresPontosView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CTresPontosView)

  CSecTipo  SecoesTipo;

  // Form Data
public:
  enum { IDD = IDD_TRESPONTOS};

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
  virtual ~CTresPontosView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  int Consiste(tlstwstr& LstDados);

  // Generated message map functions

  afx_msg void OnAdd();
  afx_msg void OnRemove();
  afx_msg void OnUpdate();
  afx_msg void OnRemoveAll();
  afx_msg void OnInsertBefore();
  afx_msg void OnSelChangeList();
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg LRESULT OnKillFocusUltimoCampo(WPARAM WP, LPARAM LP);

  DECLARE_MESSAGE_MAP()

  CEditEx EdUltimoCampo,EdPrimeiroCampo;
};

/////////////////////////////////////////////////////////////////////////////
