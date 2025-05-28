#if !defined(AFX_DESTACACOTA_H__C772C241_B686_11D9_A7B3_000AE6E79B25__INCLUDED_)
#define AFX_DESTACACOTA_H__C772C241_B686_11D9_A7B3_000AE6E79B25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DEstacaCota.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDEstacaCota dialog

class CDEstacaCota : public CDialog
{
// Construction
public:
	CDEstacaCota(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDEstacaCota)
	enum { IDD = IDD_ESTACACOTA };
	CEdit	m_CEDEstaca;
	double	m_DOCota;
	CString	m_EDEstaca;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDEstacaCota)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDEstacaCota)
	virtual void OnOK();
	afx_msg void OnKillfocusEdestaca();
	afx_msg void OnKillfocusEdcota();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESTACACOTA_H__C772C241_B686_11D9_A7B3_000AE6E79B25__INCLUDED_)
