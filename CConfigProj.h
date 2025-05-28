#pragma once

// CConfigProj dialog

class CConfigProj : public CDialog
{
	DECLARE_DYNAMIC(CConfigProj)

  int Tipo;
  bool Ok;
  int ProjetoExistente;
  
public:

  enum TIPO{NOVO,EXISTENTE};

	CConfigProj(CWnd* pParent,const CString& pNomeProj="",const CString& pDir="",int pTipo=EXISTENTE,int pNovoProj = true);   // standard constructor

	virtual ~CConfigProj();

  bool GravaCab(const CString& NomeArquivo = "");
  bool LeArquivo(const CString& NomeProj="",const CString Diretorio="");
  void Dialoga(CString &Projeto,CString &Diretorio,int Projeto_Rua=0);
  bool ConsisteFormArqPontos();
//  std::string CConfigProj::GetUserHomeDir();
  void SalvaChave(CArchive& ar);
  void LeChave(CArchive& ar);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  
  // Dialog Data
  //{{AFX_DATA(confproj)
  enum { IDD = IDD_CONFIG_PROJ };
  CString NomeProj;
  CString Diretorio;
  double EquiParabola;
  double EquiCurHoriz;
  double EquiTangente;
  double EquiOAE;
  double ValEstaca;
  CString EstacaIni;
  CString EstacaFim;
  CString FormArqPontos;
  CString ChaveGoogleEarth;

  afx_msg void OnBnClickedOk();
    //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(confproj)
  afx_msg void OnBnClickedProcurar();
  afx_msg void OnBnClickedPorcdiret();
  virtual BOOL OnInitDialog();
};
