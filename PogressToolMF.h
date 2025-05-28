#if !defined(AFX_POGRESSTOOLMF_H__E8CC05E3_271C_11D7_84BF_0004ACE3FAF3__INCLUDED_)
#define AFX_POGRESSTOOLMF_H__E8CC05E3_271C_11D7_84BF_0004ACE3FAF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PogressToolMF.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPogressToolMF dialog

class CPogressToolMF : public CDialog
{
// Construction
public:
	CPogressToolMF(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPogressToolMF)
	enum { IDD = IDD_PROGRESS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPogressToolMF)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPogressToolMF)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POGRESSTOOLMF_H__E8CC05E3_271C_11D7_84BF_0004ACE3FAF3__INCLUDED_)
