#include "afxwin.h"
#if !defined(AFX_DSECIONAR_H__F030F6A1_D02F_11D7_BE61_444553540000__INCLUDED_)
#define AFX_DSECIONAR_H__F030F6A1_D02F_11D7_BE61_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DSecionar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDSecionar dialog

class CDSecionar : public CDialog
{
  CString NomeProjeto;
  CDialog* pDialogo;

  bool ReadWindowPlacement();
  void WriteWindowPlacement();

  // Construction
public:
  CDSecionar(const CString& NomProj,CWnd* pParent = NULL);   // standard constructor

  BOOL Create(CEvent* Evento = NULL,CWnd* pWnd = NULL,CDialog* pDialog = NULL);        //--- opcao para  modeless.

  void CalcularDireto() {OnSeccionar();};

  // Dialog Data
  //{{AFX_DATA(CDSecionar)
  enum { IDD = IDD_SECCIONAR };
  CButton	m_buCancelar;
  CButton	m_buOK;
  CButton	m_buSeccionar;
  CProgressCtrl	m_Progresso1;
//  UINT	m_intEquidis;
  CString	m_Descricao1;
  //}}AFX_DATA

  CWinThread* T;

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDSecionar)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDSecionar)
  virtual void OnOK();
  afx_msg void OnSeccionar();
  virtual BOOL OnInitDialog();
  afx_msg void OnButcancelar();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedButpausa();

  ~CDSecionar();

  // Pausa a thread.
  CButton m_butPausar,m_butLog;
  CEdit ediMenorDistancia;
  CEdit ediMenorRampa;
  int m_iEquidis;
  bool bolOtimizar;
  double douMenorDist;
  double douMenorInclinacao;
  afx_msg void OnBnClickedRadsofaltantes();
  afx_msg void OnBnClickedRadtodas();
  afx_msg void OnBnClickedCheotimizarperfil();

  CButton CheTodosTrechos;
  CButton CheApenasTrechoAtual;
  afx_msg void OnBnClickedCheapenastrechoatual();
  afx_msg void OnBnClickedChetodostrechos();
  afx_msg void OnBnClickedButlog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSECIONAR_H__F030F6A1_D02F_11D7_BE61_444553540000__INCLUDED_)
