#if !defined(AFX_XYGREIDE_H__DC401460_19AD_11D3_8FCF_70DC50C10E03__INCLUDED_)
#define AFX_XYGREIDE_H__DC401460_19AD_11D3_8FCF_70DC50C10E03__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// XYGreide.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CXYGreide dialog

class CXYGreide : public CDialog
{
  // Construction
public:
  //--- Este dialogo serve para o grande e para os pontos geometricos. 
  //--- Estas variáveis são para os pontos geometricos

  bool bGreide,PontosRotacionados,Geometrico;
  double VUx,VUy;      
  double m_DOCoorX,m_DOCoorY;
  CXYGreide(CWnd* pParent = NULL);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(CXYGreide)
  enum { IDD = IDD_DESGREIDETOOL };
  CString	m_CSCoorX;
  CString	m_CSCoorY;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CXYGreide)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CXYGreide)
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeCoorx();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XYGREIDE_H__DC401460_19AD_11D3_8FCF_70DC50C10E03__INCLUDED_)
