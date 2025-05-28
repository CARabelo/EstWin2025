// DEntraPonto.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDEntraPonto dialog

class CDEntraPonto : public CDialog
{
  double* m_pdoi1, *m_pdoi2, *m_pdox, *m_pdoy;  //--- Usados na passagem de parametros.
  int Posx,Posy,                                //--- Posicao da janela.
    Tipo;                                     //--- Tipo do perfil do ponto, projeto o u terreno 
  CWnd* JanPai;

  // Construction
public:
  CDEntraPonto(double* i1,double* i2,double* x,double* y,CPoint& Ponto,
    int TipoPerfil,CWnd* pParent = NULL);  

  // Dialog Data
  //{{AFX_DATA(CDEntraPonto)
  enum { IDD = IDD_ENTRAPONTO };
  double	m_doi1;
  double	m_doi2;
  double	m_dox;
  double	m_doy;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDEntraPonto)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDEntraPonto)
  virtual void OnOK();
  virtual BOOL OnInitDialog();
  afx_msg void OnCancel();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeX();
};
