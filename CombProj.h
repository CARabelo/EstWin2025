// CombProj.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCombProj window

class CCombProj : public CComboBox
{
// Construction
public:
	CCombProj();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCombProj)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCombProj();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCombProj)
	afx_msg void OnProjClose();
	afx_msg void Onteste();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
