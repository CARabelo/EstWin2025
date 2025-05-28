#if !defined(AFX_DPONTOSGEO_H__4F5FF741_11E2_11D8_BE61_E17C7F53C218__INCLUDED_)
#define AFX_DPONTOSGEO_H__4F5FF741_11E2_11D8_BE61_E17C7F53C218__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DPontosGeo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DPontosGeo dialog

class DPontosGeo : public CDialog
{
// Construction
public:
	DPontosGeo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DPontosGeo)
	enum { IDD = IDD_PONTOGEOM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DPontosGeo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DPontosGeo)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DPONTOSGEO_H__4F5FF741_11E2_11D8_BE61_E17C7F53C218__INCLUDED_)
