#if !defined(AFX_DIAPONTOSGEOM_H__9E6EA361_11EA_11D8_BE61_E17C7F53C218__INCLUDED_)
#define AFX_DIAPONTOSGEOM_H__9E6EA361_11EA_11D8_BE61_E17C7F53C218__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiapontosGeom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DiapontosGeom dialog

class DiapontosGeom : public CDialog
{
  // Construction
public:
  DiapontosGeom(CWnd* pParent = NULL);   // standard constructor
  int* pProjeto,*pAcitopog,*pTipoCN;

  // Dialog Data
  //{{AFX_DATA(DiapontosGeom)
  enum { IDD = IDD_PONTOSGEO };
  int		m_iAcidTopog;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(DiapontosGeom)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(DiapontosGeom)
  afx_msg void OnActopog();
  afx_msg void OnBordos();
  afx_msg void OnTipoCN();

  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAPONTOSGEOM_H__9E6EA361_11EA_11D8_BE61_E17C7F53C218__INCLUDED_)
