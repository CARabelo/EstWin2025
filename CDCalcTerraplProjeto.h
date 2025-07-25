#pragma once

// caixa de diálogo de CDCalcTerraplProjeto
/*
struct ltCN
{
  bool operator()(const ItLLPontos& it1, const ItLLPontos& it2) const
  {
    return *it1->begin() < *it2->begin();
  }
};

typedef std::set<ItLLPontos,ltCN> tysetItLLPontos;
typedef tysetItLLPontos::iterator ittysetItLLPontos;
*/
class CDCalcTerraplProjeto : public CDialogEx
{
	DECLARE_DYNAMIC(CDCalcTerraplProjeto)

  std::string NomeProjeto;
  std::string NomeProjetoBase;

  int AjustaOffSTerraplanagem();

  inline int LadoOffs(const Ponto& PontoOffs);
  void InsereCNTerrenoNaoUsadas(CSuperficie& pSuperficieTN, CSuperficie& pSuperficieTerrap, tysetItLLPontos& pSetCNUsadas);
  
public:
	CDCalcTerraplProjeto(CWnd* pParent,const std::string& NomeProj,std::string& ProjBase);   // construtor padrão
	virtual ~CDCalcTerraplProjeto();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALCTERRPROJETO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOk();
 // afx_msg void OnBnClickedCalcular();
  afx_msg void OnBnClickedButcalcterraplproj();
  CProgressCtrl m_Progresso;
  CStatic m_Texto;
  afx_msg void OnBnClickedButlog();
  virtual BOOL OnInitDialog();
  int m_uiDensTracos;
};

