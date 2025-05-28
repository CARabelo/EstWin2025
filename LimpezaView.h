// TresPontosView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLimpezaView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CLimpezaView : public CTypedPtrListView
{
protected:
  CLimpezaView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CLimpezaView)

  int Consiste(tlstwstr& ListaCampos);

  // Form Data
public:
  enum { IDD = IDD_LIMPEZA};
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
  virtual ~CLimpezaView();
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

  DECLARE_MESSAGE_MAP()
public:
};

/////////////////////////////////////////////////////////////////////////////
