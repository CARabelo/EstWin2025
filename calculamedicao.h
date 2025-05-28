// CalcMedicao.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CalcMedicao dialog

class CalcMedicao : public CDialog
{
  enum{CORTE,ATERRO}; 
  enum{ESQUERDO,DIREITO};
  enum{X,Y};


  CLPerfis MedicaoAnterior,
    MedicaoAtual;
  CListaSecoes Secoes;
  BOOL UsarEditadas,Acumulada; 
  CString NomeProjeto;
  CProgressCtrl *Prog;

  void CalculaMedicao();
  void CalculaOffSets();
  void CalculaVolumes();
  int LeMedicaoAnterior();
  int LeMedicaoAtual();
  void MontaSecoes();
  void GravaArqNota();
  void GravaArqVolumes();
  void CalculaAreas();
  //	void MergeTerrenoNatural(const CString& NomeArquivo);
  void GravaArqTerrProxMed();


  // Construction
public:
  CalcMedicao(CString& NomeProjeto,CWnd* pParent = NULL);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(CalcMedicao)
  enum { IDD = IDD_CALCMEDICAO };
  // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CalcMedicao)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CalcMedicao)
  afx_msg void OnIniciarcalcmed();
  virtual BOOL OnInitDialog();
  afx_msg void OnOk();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

};

