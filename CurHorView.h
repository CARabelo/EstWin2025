// CurhorView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCurHorView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CCurHorView : public CTypedPtrListView
{
  bool VerificaLC(int Tipo=0);

  int SelAtual;

protected:
  CCurHorView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CCurHorView)

  // Form Data
public:
  enum { IDD = IDD_CURVASHORIZONTAIS };
  CEdit	m_CEEdit8;
  CEdit m_CEEdit9;
  CEdit m_CEEdit10;

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
  virtual ~CCurHorView();
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
  afx_msg void OnClose();
  afx_msg void OnEnKillfocusEdit5();
};

/////////////////////////////////////////////////////////////////////////////
