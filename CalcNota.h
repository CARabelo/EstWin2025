// CalcNota.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CalcNota dialog

class CalcNota : public CDialog
{
  // Construction

  CString NomeProj;
  CProjeto* pProjeto;

public:

  CalcNota(const CString& NomeProj,CWnd* pParent = NULL,CProjeto* pProj = NULL);   // standard constructor
  BOOL Create(CEvent* Evento = NULL,CWnd* pWnd = NULL,CDialog* pDialog = NULL,CProjeto* PProjeto = NULL);        //--- opcao para  modeless.

  void CalcularDireto() {OnIniciarcalc();};


  // Dialog Data
  //{{AFX_DATA(CalcNota)
  enum { IDD = IDD_CALCNOTA };
  CButton	m_BUOK;
  CButton	m_BUPausa;
  CButton	m_BUCancelar;
  CButton	m_BUIniciar;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CalcNota)
public:
  virtual INT_PTR DoModal();
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  bool Pausa;
  // CWinThread* T; 

  // Generated message map functions
  //{{AFX_MSG(CalcNota)
  afx_msg void OnIniciarcalc();
  afx_msg void OnOk();
  virtual BOOL OnInitDialog();
  afx_msg void OnCancelar();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedPausa();

  CWinThread* T;
  afx_msg void OnNMCustomdrawProgresso(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnStnClickedExecutado();
  afx_msg void OnBnClickedCalcsocota();
  afx_msg void OnBnClickedCalctudo();
};
