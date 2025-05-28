// DPonto.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDPonto dialog

class CDPonto : public CDialog
{
 enum{PROJETO,TERRENO,PAVIMENTO,MEDICAO};

// Construction
public:
  BOOL Limpo;           //--- Determina se o dialogo esta limpo ou nao.

	CDPonto(CWnd* pParent = NULL);    // standard constructor
  void Atualiza(double x, double y, double i1,double i2,int  Tipo,CString& NumMed);
	void Atualiza(CEstaca& Estaca,double i1,double i2,CString& Desc,double E = INFINITO);  //--- Dados do perfil longitudinal.

  BOOL Create(CWnd* pWnd = NULL);   //--- modeless.
	void LimpaDialogo();              //--- Limpa os campos do dialogo.

// Dialog Data
	//{{AFX_DATA(CDPonto)
	enum { IDD = IDD_PONTO };
	CString	m_STi1;
	CString	m_STi2;
	CString	m_STx;
	CString	m_STy;
	CString	m_STE;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDPonto)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDPonto)
	afx_msg void OnProjeto();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
