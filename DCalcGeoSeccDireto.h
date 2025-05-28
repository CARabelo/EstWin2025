#if !defined(AFX_DCALCGEOSECCDIRETO_H__AEBAD23E_ADDC_46CA_9778_BDE752167403__INCLUDED_)
#define AFX_DCALCGEOSECCDIRETO_H__AEBAD23E_ADDC_46CA_9778_BDE752167403__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DCalcGeoSeccDireto.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDCalcGeoSeccDireto dialog

class CDCalcGeoSeccDireto : public CDialog
{
  // Construction
public:
  CDCalcGeoSeccDireto(CWnd* pParent = NULL);   // standard constructor

  BOOL Create(CWnd* pWnd) { return CDialog::Create(IDD,pWnd);}   //-- Para não modal

  // Dialog Data
  //{{AFX_DATA(CDCalcGeoSeccDireto)
  enum { IDD = IDD_CALCGEORESECCIONADIRETO };
  CProgressCtrl	m_Progresso;
  CString	m_CSExecutando;
  CString	m_CSExecutando2;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDCalcGeoSeccDireto)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDCalcGeoSeccDireto)
  // NOTE: the ClassWizard will add member functions here
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCALCGEOSECCDIRETO_H__AEBAD23E_ADDC_46CA_9778_BDE752167403__INCLUDED_)
