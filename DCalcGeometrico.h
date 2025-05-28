#include "afxcmn.h"
#include "afxwin.h"
#if !defined(AFX_DCALCGEOMETRICO_H__7AAF0361_D3CF_11D7_BE61_BFFD849C501D__INCLUDED_)
#define AFX_DCALCGEOMETRICO_H__7AAF0361_D3CF_11D7_BE61_BFFD849C501D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DCalcGeometrico.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDCalcGeometrico dialog

class CDCalcGeometrico : public CDialog
{
  // Construction

  CString Projeto;
  CDialog* pDialogo;
  bool CalcDireto;
  CProjeto* CPProjeto;

public:
  CDCalcGeometrico(CWnd* pParent = NULL,CString& pProjeto = CString(""),CProjeto* CProj=NULL);   // standard constructor

  BOOL Create(CWnd* pWnd,CDialog* ppDialogo);             //--- para não modal.

  void CalcularDireto() { CalcDireto = true ; OnIniciar();};

  // Dialog Data
  //{{AFX_DATA(CDCalcGeometrico)
  enum { IDD = IDD_CALCGEOMETRICO };
  CButton	m_buCancelar;
  CButton	m_buOK;
  CButton	m_buIniciar;
  CProgressCtrl	m_Progresso;
  CString	m_Descricao;
  double m_douErroAjuste;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDCalcGeometrico)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDCalcGeometrico)
  virtual void OnOK();
  afx_msg void OnIniciar();
  afx_msg void OnButcancelar();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  //  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  //  virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
  afx_msg void OnBnClickedCheAjustarcurvas();
  CEdit m_EdiErroMaximo;
  CButton m_cheAjustarCurvas;
public:
  afx_msg void OnStnClickedStaValorerro();
  afx_msg void OnEnKillfocusEdiErromaximo();
  afx_msg void OnEnChangeEdiErromaximo();
  afx_msg LRESULT DisablePauseButton(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCALCGEOMETRICO_H__7AAF0361_D3CF_11D7_BE61_BFFD849C501D__INCLUDED_)
