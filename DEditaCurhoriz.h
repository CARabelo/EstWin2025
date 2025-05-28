#if !defined(AFX_DEDITACURHORIZ_H__65E6B941_FF6C_11D8_AFF6_000AE6E79B25__INCLUDED_)
#define AFX_DEDITACURHORIZ_H__65E6B941_FF6C_11D8_AFF6_000AE6E79B25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DEditaCurhoriz.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDEditaCurhoriz dialog

class CDEditaCurhoriz : public CDialog
{
  // Construction

  CurHorizontal* CH;
  double AnguloRotacao;
  Ponto PPI;
  int Tipo;

public:

  enum VUE_TIPO_ALT_CURHORIZ{PI_NORMAL,PI_INICIAL_FINAL,PROIBIDO_ALTERAR};

  CDEditaCurhoriz(CWnd* pParent= NULL,CurHorizontal* pPCH = NULL,double pAnguloRotacao = 0.0,int TipoAlter = PI_NORMAL);

  // Dialog Data
  //{{AFX_DATA(CDEditaCurhoriz)
  enum { IDD = IDD_EDITACH };
  double	m_DoCoorN;
  double	m_DoI;
  double	m_DoLc;
  double	m_DoR;
  double	m_DoSe;
  double	m_DoSl;
  double	m_DoCoorE;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDEditaCurhoriz)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDEditaCurhoriz)
  virtual void OnOK();
  virtual void OnCancel();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEDITACURHORIZ_H__65E6B941_FF6C_11D8_AFF6_000AE6E79B25__INCLUDED_)
