#if !defined(AFX_DCORESDESGEOMETRICO_H__B512AD7F_33D3_4A07_83A7_5A8F73B6F7BF__INCLUDED_)
#define AFX_DCORESDESGEOMETRICO_H__B512AD7F_33D3_4A07_83A7_5A8F73B6F7BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DCoresDesGeometrico.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDCoresDesGeometrico dialog

class CDCoresDesGeometrico : public CDialog
{
  // Construction

  COLORREF* pCoresRef;
  void MudaCor(CMelEditBox* EditBox);
  bool bGeometrico;

public:
  CDCoresDesGeometrico(CWnd* pParent = NULL,COLORREF* pCores = NULL,bool bGeometrico=false);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(CDCoresDesGeometrico)
  enum { IDD = IDD_CORESDESGEOMETRICO };
  CMelEditBox	m_EdCorPontos;
  CMelEditBox	m_EdCorFundo;
  CMelEditBox	m_EdCorMalha;
  CMelEditBox	m_EdCorCristasPes;
  CMelEditBox	m_EdCorBordos;
  CMelEditBox	m_EdCorArestas;
  CMelEditBox m_EdCorSecoes;
  CMelEditBox m_EdCorTexto;
  CMelEditBox m_EdCorProjeto;
  CMelEditBox m_EdCorCN;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDCoresDesGeometrico)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual void OnOK();

  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDCoresDesGeometrico)
  afx_msg void OnRadioFundo();
  afx_msg void OnRadioPontos();
  afx_msg void OnRadioArestas();
  afx_msg void OnRadioBordos();
  afx_msg void OnRadioCristasPes();
  afx_msg void OnRadioMalha();
  afx_msg void OnRadioSecoes();
  afx_msg void OnRadioTexto();
  afx_msg void OnRadioProjeto();
  afx_msg void OnRadioCN();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedRadiocn();
  virtual BOOL OnInitDialog();
};

/////////////////////////////////////////////////////////////////////////////
// DcoresDesGeometrico dialog

class DcoresDesGeometrico : public CDialog
{
  // Construction
public:
  DcoresDesGeometrico(CWnd* pParent = NULL);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(DcoresDesGeometrico)
  enum { IDD = IDD_CALCTERR };
  // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(DcoresDesGeometrico)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(DcoresDesGeometrico)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCORESDESGEOMETRICO_H__B512AD7F_33D3_4A07_83A7_5A8F73B6F7BF__INCLUDED_)
