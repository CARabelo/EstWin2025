#if !defined(AFX_CDPONTOGEOMETRICO_H__2F24B2C1_2189_11D8_BE61_B80B66D93819__INCLUDED_)
#define AFX_CDPONTOGEOMETRICO_H__2F24B2C1_2189_11D8_BE61_B80B66D93819__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDPontoGeometrico.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDPontoGeometrico dialog

class CDPontoGeometrico : public CDialog
{
// Construction
public:
  
	Ponto* PontoED;  

	CDPontoGeometrico(CWnd* pParent = NULL,Ponto* PPonto = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDPontoGeometrico)
	enum { IDD = IDD_PONGEOMETRICO };
	double	m_DOE;
	double	m_DON;
	double	m_DOCota;
	CString	m_CSDesc;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDPontoGeometrico)
	public:
	virtual INT_PTR DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDPontoGeometrico)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDPONTOGEOMETRICO_H__2F24B2C1_2189_11D8_BE61_B80B66D93819__INCLUDED_)
