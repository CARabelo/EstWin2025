#if !defined(AFX_DCONFDESGEOM_H__6D26CBAC_6943_49EC_8E38_EEEC72F3D150__INCLUDED_)
#define AFX_DCONFDESGEOM_H__6D26CBAC_6943_49EC_8E38_EEEC72F3D150__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DConfDesGeom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDConfDesGeom dialog

class CDConfDesGeom : public CDialog
{
  int * pDados;
  COLORREF* pCores;
  bool bGeometrico,bProjTerrap;
  int TipoSuperf;

  // Construction
public:
  CDConfDesGeom(CWnd* pParent = NULL,int *pDadosConfig = NULL,COLORREF* pPCores = NULL,bool bGeometrico = false,int pTipoSuperf=0,bool ProjTerrap = false);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(CDConfDesGeom)
  enum { IDD = IDD_CONFDESGEOM };
  BOOL	m_bDesenharArestas;
  BOOL	m_bDesenharBordos;
  BOOL	m_bDesenharCirculo;
  BOOL	m_bDesenharCristasPes;
  BOOL	m_bDesenharCruz;
  BOOL	m_bDesenharPixel;
  BOOL	m_bDesenharPontos;
  BOOL	m_bDdesenharSecoes;
  BOOL	m_bMostrarDesc;
  UINT	m_uintEquidistancia;
  BOOL	m_bDesenharMalha;
  UINT	m_uintEquidisMalha;
  BOOL	m_bDesenharProjeto;
  BOOL	m_bCircularSecoes;
  BOOL	m_bGeometrico;
  BOOL	m_bSuperficie;
  BOOL  m_bCotarMalha;
  BOOL  m_bVarrerTerreno;
  BOOL  m_bVarrerProjeto;
  BOOL  m_bVarrerCN;
  BOOL  m_bDesenharCN;
  BOOL  m_bCotarCN;
  UINT  m_uintSaltarCNs;
  BOOL  m_bTodosAlin;
  BOOL  m_bRadarTodosPontos;
  BOOL  m_bRadarPontosInteresse;
  BOOL  m_bRadarRealcarPontosInteresse;
  UINT  m_uintRadarRaio;
  BOOL  m_bOffSets;
  BOOL  m_bMapa;
  BOOL  m_bOAC;
  BOOL m_bCNTerraplanagem;
  BOOL m_bSimbologia;
  UINT m_uintDensSimbologia;
  BOOL m_bDesSecAvulsas;
  BOOL m_bRealcarCNsMestras;

  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDConfDesGeom)
public:
  virtual INT_PTR DoModal();
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDConfDesGeom)
  virtual void OnOK();
  afx_msg void OnDesarestas();
  afx_msg void OnCores();
  afx_msg void OnCheckgeometrico();
  afx_msg void OnChecksuperficie();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedCheCotarcn();
  afx_msg void OnClickedButRadar();
  virtual BOOL OnInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCONFDESGEOM_H__6D26CBAC_6943_49EC_8E38_EEEC72F3D150__INCLUDED_)
