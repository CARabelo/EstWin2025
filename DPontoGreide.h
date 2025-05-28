#if !defined(AFX_DPONTOGREIDE_H__879798C4_79BC_11D4_BDFB_00104BC6689A__INCLUDED_)
#define AFX_DPONTOGREIDE_H__879798C4_79BC_11D4_BDFB_00104BC6689A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DPontoGreide.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDPontoGreide dialog

class CDPontoGreide : public CDialog
{
  // Construction
public:

  CDPontoGreide(CWnd* pParent = NULL);   // standard constructor
  BOOL Create(CWnd* pWnd = NULL);        //--- modeless.
  void LimpaDialogo();                   //--- limpa os campos do dialogo
  void Atualiza(CEstaca& Estaca,double i1,double i2,CString& Desc=CString(""),double E=INFINITO,double Y1=INFINITO,double Y2=INFINITO);

  BOOL Limpo;

  // Dialog Data
  //{{AFX_DATA(CDPontoGreide)
  enum { IDD = IDD_PONTOGREIDE };
  CString	m_CSE;
  CString	m_CSI1;
  CString	m_CSI2;
  CString	m_CSCota;
  CString	m_CSEstaca;
  CString	m_CSY1;
  CString	m_CSY2;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDPontoGreide)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDPontoGreide)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DPONTOGREIDE_H__879798C4_79BC_11D4_BDFB_00104BC6689A__INCLUDED_)
