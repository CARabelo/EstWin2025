// Instalar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Instalar dialog

class Instalar : public CDialog
{
// Construction
public:
	Instalar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Instalar)
	enum { IDD = IDD_INSTALAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Instalar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Instalar)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
