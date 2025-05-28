#if !defined(AFX_DDEFBORACITOPOG_H__00305901_7FE0_11D8_BE65_000AE6E79B25__INCLUDED_)
#define AFX_DDEFBORACITOPOG_H__00305901_7FE0_11D8_BE65_000AE6E79B25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DDefBorAciTopog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDDefBorAciTopog dialog

class CDDefBorAciTopog : public CDialog
{
	CString strNomeProj;

public:
	CDDefBorAciTopog(CWnd* pParent = NULL,CString& NomeProj = CString(""));   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDDefBorAciTopog)
	enum { IDD = IDD_DEFBOR_ACITOPOG };
	BOOL	m_bBordos;
	BOOL	m_bAciTopog;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDDefBorAciTopog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDDefBorAciTopog)
	afx_msg void OnIniciar();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DDEFBORACITOPOG_H__00305901_7FE0_11D8_BE65_000AE6E79B25__INCLUDED_)
