#if !defined(AFX_DROTACIONAR_H__143F1501_3E29_11D9_AFF6_000AE6E79B25__INCLUDED_)
#define AFX_DROTACIONAR_H__143F1501_3E29_11D9_AFF6_000AE6E79B25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DRotacionar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDRotacionar dialog

class CDRotacionar : public CDialog
{
  // Construction
public:
  CDRotacionar(CWnd* pParent = NULL,double AnguloAtual = 0.0,double* NovoAngulo = NULL,int*Absoluto_Relativo=0);   // standard constructor
  double* piNovoAngulo;
  int* piAbsolutoRelativo;
  // Dialog Data
  //{{AFX_DATA(CDRotacionar)
  enum { IDD = IDD_ROTACIONAR };
  CButton	m_CBRelativo;
  CButton	m_CBAbsoluto;
  int		m_iNovoAngulo;
  int		m_iAnguloAtual;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDRotacionar)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDRotacionar)
  virtual void OnOK();
  virtual void OnCancel();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeEdnovoangulo();
  afx_msg void OnBnClickedButlinhabase();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DROTACIONAR_H__143F1501_3E29_11D9_AFF6_000AE6E79B25__INCLUDED_)
