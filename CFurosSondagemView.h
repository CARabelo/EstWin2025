// OAEView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTiposSolosView form view


#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CFurosSondagemView : public CTypedPtrListView
{
  void PegaCamposCombos();
  void PassaCamposCombos();


protected:
  CFurosSondagemView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CFurosSondagemView)

  // Form Data
public:
  //{{AFX_DATA(CTiposSolosView)
  enum { IDD = IDD_FUROSSONDAGEM};

  //}}AFX_DATA

  // Attributes
public:
   CEditEx EdPrimeiroCampo, EdUltimoCampo;

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
  virtual ~CFurosSondagemView();
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
  afx_msg void OnCbnSelchangeCombo2();
  CComboBox m_ComTiposSolos1;
  CComboBox m_ComTiposSolos2;
  CComboBox m_ComTiposSolos3;
  CComboBox m_ComTiposSolos4;
  afx_msg void OnEnChangeEdit();
  afx_msg void OnEnChangeEdit1();
  afx_msg LRESULT OnKillFocusUltimoCampo(WPARAM WP, LPARAM LP);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnKillfocusComtiposolo4();
};

/////////////////////////////////////////////////////////////////////////////
