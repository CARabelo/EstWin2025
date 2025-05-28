
#if !defined(AFX_DSELGREIDE_H__30C55F01_775F_11D4_BDFB_00104BC6689A__INCLUDED_)
#define AFX_DSELGREIDE_H__30C55F01_775F_11D4_BDFB_00104BC6689A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DSelGreide.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDSelGreide dialog

class CDSelGreide : public CDialog
{
  CToolBar* mpCDesGreide;        //--- Ponteiros para o toolbar do greide. 
  //--- usado quando o usuário seleciona se quer 
  //--- visualizar o toolbar ou não
  CRect &pSecao;  
  COLORREF* pCores;             //--- Cores do dialogo
  int* pDados;                  //--- Estados das configuraçoes

  // Construction
public:
  CDSelGreide(CWnd* pParent = NULL,CToolBar *DesToolGreide = NULL,CRect& Rect = CRect(10,10,10,10),COLORREF* PpCores=NULL,int* PpDados = NULL);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(CDSelGreide)
  enum { IDD = IDD_SELGREIDE };
  CButton	m_CBSincronizar;
  CButton m_CBAtualizarSecoes;
  CButton	m_CBGabaritarProjeto;
  CButton	m_CBCoordenadas;
  BOOL	m_CBDesenharProjeto;
  BOOL	m_CBDesenharTerreno;
  BOOL	m_CBDesenharMalha;
  BOOL	m_CBCotar;
  BOOL  m_CBCircularPontos;
  BOOL  m_bAtualizarSecoes;
  UINT	m_iEquidistancia;
  UINT	m_iEquidistanciaY;
  UINT	m_iEquidistCotas;
  BOOL	m_CBMostrarDesc;
  BOOL  m_CBGreide;
  CButton  m_CBTerreno;
  CButton  m_CBProjeto;
  BOOL m_bDesenharPerfGeol;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDSelGreide)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDSelGreide)
  afx_msg void OnVerCoordenadas();
  virtual BOOL OnInitDialog();
  afx_msg void OnMbok();
  afx_msg void OnGabaritarproj();
  afx_msg void OnSincronizar();
  afx_msg void OnCores();
  afx_msg void OnCheckgreide();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedRadTodasassecoes();
  afx_msg void OnBnClickedRadComproj();
  CString m_EquidistanciaY;
};
#endif //AFX_DSELGREIDE_H__30C55F01_775F_11D4_BDFB_00104BC6689A__INCLUDED_
