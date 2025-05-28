
// CalculaTerreno.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CCalculaTerreno dialog

class DadDouble
{
public:

  CEstaca PontoLido;
  double PlRef,Cota,Leitura,Afastamento;
  int Ig;

  void Converte(CVisada Vis,CString PlRefStr,CString CotaStr, CString LeituraStr,CString IgStr);
};

//------------------------------------------------------------------------------------

class CalculaTerreno : public CDialog
{
  DadDouble DadosDouble;
  CLPerfis TerrenoNivelado;
  CEstaca EstacaInicial,EstacaFinal;
  CString NomeProjeto;
  CProgressCtrl* Prog;
  BOOL TIPO,USARNIV,USARTER,PRIORIDADE;

  void GeraListaNivelamento();
  void GravaTerreno();

  // Construction
public:
  CalculaTerreno(CString Nomeproj,CWnd* pParent = NULL);   // standard constructor

  //Dialog Data
  //{{AFX_DATA(CalculaTerreno)
  enum { IDD = IDD_CALCTERRENO};
  int		m_IgFim;
  int		m_IgIni;
  CString	m_EstFim;
  CString	m_EstIni;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CalculaTerreno)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  enum {CALCULAR,DIGITADA};
  enum {X,Y};
  enum {TERRPONTOS,NIVELAMENTO};
  enum {INICIAL,FINAL};
  enum {MARCADO,DESMARCADO};
  enum {MENOR,MAIOR,MENORIGUAL,MAIORIGUAL};

  // Generated message map functions
  //{{AFX_MSG(CalculaTerreno)
  virtual BOOL OnInitDialog();
  afx_msg void OnCompleto();
  afx_msg void OnCheniv();
  afx_msg void OnCheter();
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnKillFocus(CWnd* pNewWnd);
  afx_msg void OnIniciar();
  afx_msg void OnOk();
  afx_msg void OnCalcnovoterr();
  afx_msg void OnCancelar();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
