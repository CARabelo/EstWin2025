#if !defined(AFX_DESLIZANTESGEOM_H__AB3FB2DE_9C68_4BEA_9B2A_ED6F28CB0239__INCLUDED_)
#define AFX_DESLIZANTESGEOM_H__AB3FB2DE_9C68_4BEA_9B2A_ED6F28CB0239__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeslizantesGeom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeslizantesGeom dialog

class CDeslizantesGeom : public CDialog
{
  // Construction
public:
  CDeslizantesGeom(void* DesGeo,CWnd* pParent = NULL);   // standard constructor

  void MudarEscala(double Valor);
  void MudarRazao(double Valor);

  void* m_pDesView;              //--- CView do deslizante; 

  // Dialog Data
  //{{AFX_DATA(CDeslizantesGeom)
  enum { IDD = IDD_DESLIZANTESGEOM };
  CSliderCtrl	m_SLValorEscala;
  CEdit	m_CEEscala;
  UINT	m_intEscala;
  //}}AFX_DATA

  void MouseWheelOn(UINT nFlags, short zDelta, CPoint pt);// { return OnMouseHWheel(nFlags,zDelta,pt);};

  int CDeslizantesGeom::PegaEscala() { return m_intEscala; }

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDeslizantesGeom)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDeslizantesGeom)
  virtual void OnOK();
  afx_msg void OnChangeEscala();
  virtual BOOL OnInitDialog();
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
//  afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg void OnSetFocus(CWnd* pOldWnd);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESLIZANTESGEOM_H__AB3FB2DE_9C68_4BEA_9B2A_ED6F28CB0239__INCLUDED_)
