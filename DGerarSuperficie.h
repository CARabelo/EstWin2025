#if !defined(AFX_DGERARSUPERFICIE_H__0DA0E562_C9B9_11D7_BE61_FC5E2142351B__INCLUDED_)
#define AFX_DGERARSUPERFICIE_H__0DA0E562_C9B9_11D7_BE61_FC5E2142351B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DGerarSuperficie.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDGerarSuperficie dialog

class CDGerarSuperficie : public CDialog
{
  // Construction
  CProjeto* pProjeto;
public:
  CDGerarSuperficie(CWnd* pParent,CString NomeProj);   // standard constructor
  CString NomeProjeto;

  CWinThread* T;

  // Dialog Data
  //{{AFX_DATA(CDGerarSuperficie)
  enum { IDD = IDD_SUPERFICIE };
  CButton	m_buCancelar;
  CButton	m_buGerar;
  CButton	m_buOK;
  CButton m_buPausar;
  CProgressCtrl	m_ProProgresso;
  CString	m_CSTexto;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDGerarSuperficie)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDGerarSuperficie)
  afx_msg void OnGerar();
  virtual BOOL OnInitDialog();
  afx_msg void OnButcancelar();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedPausar();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGERARSUPERFICIE_H__0DA0E562_C9B9_11D7_BE61_FC5E2142351B__INCLUDED_)
