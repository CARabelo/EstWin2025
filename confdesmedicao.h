#if !defined(AFX_CONFDESMEDICAO_H__7D395BB1_3549_11D7_84D6_0004ACE3FAF3__INCLUDED_)
#define AFX_CONFDESMEDICAO_H__7D395BB1_3549_11D7_84D6_0004ACE3FAF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfDesMedicao.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfDesMedicao dialog

class CConfDesMedicao : public CDialog
{
// Construction
public:
	CConfDesMedicao(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfDesMedicao)
	enum { IDD = IDD_CONFDESMEDICAO };
	CComboBox	m_CBQuanMedicoes;
	CString	m_ComboquanMedicoes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfDesMedicao)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfDesMedicao)
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFDESMEDICAO_H__7D395BB1_3549_11D7_84D6_0004ACE3FAF3__INCLUDED_)
