// confproj.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// confproj dialog

#include "afxwin.h"

class confproj : public CDialog
{
  // Construction

public:

  confproj(CWnd* pParent = NULL,const CString& Projeto="",const CString& Dir = "",int Tipo = EXISTENTE);   // standard constructor
  void dialoga(CString &Projeto,CString &Diretorio);
  bool LeArquivo(const CString& NomeProj="",const CString Diretorio="");
  void GravaPIAZInicial(const CString& NomeProj, double AzInicial,double y, double x,double AngRot);
  void PegaPIAZInicial(const CString& NomeProj, double& AzInicial,double& y, double& x,double& AngRot,CString& EstIni);
  void GravaAngRotacao(const CString& NomeProj,double AngRot);
  void PegaAngRotacao(const CString& NomeProj, double& AngRotacao);

  enum {NOVO,EXISTENTE};

  // Dialog Data
  //{{AFX_DATA(confproj)
  enum { IDD = IDD_CONFIG_PROJ };
  CEdit	m_cednomeProj;
  CEdit	m_cedDir;
  double	m_azimute;
  double	m_coore;
  double	m_coorn;
  CString	m_dir;
  CString	m_empresa;
  double	m_equicurhor;
  double	m_equioae;
  double	m_equipar;
  double	m_equitan;
  CString	m_linha1;
  CString	m_linha2;
  CString	m_linha3;
  CString	m_nomelogra;
  CString	m_nomeproj;
  double	m_valest;
  CString	m_CSEstIni;
  CString	m_CSEstFim;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(confproj)
public:
  virtual int DoModal();
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL
  // Implementation
protected:

  int gravacab(const CString& NomeArquivo = CString(""));
  int Tipo;
  double AngRotacao;

  // Generated message map functions
  //{{AFX_MSG(confproj)
  virtual BOOL OnInitDialog();
  afx_msg void OnProcurar();
  afx_msg void OnKillfocusNomeproj();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOk();
protected:
  // contém o nome do projeto
  //CEdit ceNomeProj;
public:
  afx_msg void OnEnChangeNomeproj();
  afx_msg void OnEnChangeCoorn();
  CButton CBProcurar;
  CButton CButProcDiret;
  afx_msg void OnBnClickedPorcdiret();
};
