// dialogo.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// dialogo dialog

//#include "afxwin.h"
//#include "stdafx.h"
class dialogo : public CDialog
{
  int NumeroMensagem;
  bool* pResec;
  // Construction
public:
  dialogo(CWnd* pParent = NULL);	// standard constructor
  CString texto,complemento; 

  // Dialog Data
  //{{AFX_DATA(dialogo)
  enum { IDD = IDD_DIASNC };
  // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA

  // Implementation
protected:         

  char far * mensagens[16];		          

  virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

  // Generated message map functions
  //{{AFX_MSG(dialogo)
  virtual BOOL OnInitDialog();
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnCancelar();

  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  virtual INT_PTR DoModal(int Mensagem,const CString& Complemento = CString(""),bool* Resecionar = NULL);

  afx_msg void OnBnClickedRadSeccionar();
  int RadSeccionar;
  CButton CBRadSeccionar;
  afx_msg void OnDestroy();
};


