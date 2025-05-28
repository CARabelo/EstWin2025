// TalCorteView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTalCorteView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CTalCorteView : public CTypedPtrListView
{
protected:
	CTalCorteView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTalCorteView)

// Form Data
public:
	//{{AFX_DATA(CTalCorteView)
	enum { IDD = IDD_TRESPONTOS};
	CString	m_Campo1;
	CString	m_Campo2;
	CString	m_Campo3;
	CString	m_Campo4;
	CString	m_Campo5;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTalCorteView)
	public:
	virtual void OnInitialUpdate();
	virtual void Serialize(CArchive& ar);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTalCorteView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTalCorteView)
	  afx_msg void OnAdd();
    afx_msg void OnRemove();
    afx_msg void OnUpdate();
    afx_msg void OnRemoveAll();
    afx_msg void OnInsertBefore();
		afx_msg void OnSelChangeList();
	  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
