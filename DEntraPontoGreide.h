#if !defined(AFX_DENTRAPONTOGREIDE_H__879798C3_79BC_11D4_BDFB_00104BC6689A__INCLUDED_)
#define AFX_DENTRAPONTOGREIDE_H__879798C3_79BC_11D4_BDFB_00104BC6689A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DEntraPontoGreide.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDEntraPontoGreide dialog

class CDEntraPontoGreide : public CDialog
{
  int Posx,Posy,                     //--- Posicao da janela.
    Tipo;                          //--- Tipo do perfil do ponto, projeto o u terreno 
  CWnd* JanPai;                      //--- Ponteiro para a janela que chamou.   

  // Construction
public:
  CDEntraPontoGreide(CString& Estaca,CString& Igualdade,double& Cota,double& i1,double& i2,double& Y1,double& Y2,CPoint& Ponto,
    int TipoPonto,CWnd* pParent=NULL);
  // Dialog Data
  //{{AFX_DATA(CDEntraPontoGreide)
  enum { IDD = IDD_ENTRAPONTOGREIDE };
  CString&	m_CSEstaca;
  double&	m_doI1;
  double&	m_doI2;
  CString&	m_CSIG;
  double&	m_doY1;
  double&	m_doY2;
  double&	m_doCota;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDEntraPontoGreide)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDEntraPontoGreide)
  virtual void OnOK();
  virtual BOOL OnInitDialog();
  afx_msg void OnCancel();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeY();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DENTRAPONTOGREIDE_H__879798C3_79BC_11D4_BDFB_00104BC6689A__INCLUDED_)
