#if !defined(AFX_DDESLIZANTES_H__0071BF84_645B_11D2_8431_004033523BB2__INCLUDED_)
#define AFX_DDESLIZANTES_H__0071BF84_645B_11D2_8431_004033523BB2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define SECOES      0
#define GREIDE      1
#define GEOMETRICO  2
#define GEOLOGICO   3

// DDeslizante.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDDeslizantes dialog

class CDDeslizantes : public CDialog
{
  int m_INGreSec;     //--- Sinaliza se pertence ao greide ou as seções.

  // Construction
public:
  CDDeslizantes(void* DesSec = NULL,CWnd* pParent = NULL,int GreSec = SECOES);   // standard constructor

  void MudarEscala(double Valor);
  void MudarRazao(double Valor);

  void* m_pDesView;              //--- CView do deslizante; 

  // Dialog Data
  //{{AFX_DATA(CDDeslizantes)
  enum { IDD = IDD_DESLIZANTES};
  CEdit	m_EDRazao;
  CEdit	m_EDEscala;
  CSliderCtrl	m_SLValorRazao;
  CSliderCtrl	m_SLValorEscala;
  double m_DOEscala;
  double m_DORazao;
  //}}AFX_DATA

  void MouseWheelOn(UINT nFlags, short zDelta, CPoint pt);

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDDeslizantes)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDDeslizantes)
  virtual void OnOK();
  afx_msg void OnChangeEscala();
  virtual BOOL OnInitDialog();
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnChangeRazao();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnSetFocus(CWnd* pOldWnd);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DDESLIZANTES_H__0071BF84_645B_11D2_8431_004033523BB2__INCLUDED_)

