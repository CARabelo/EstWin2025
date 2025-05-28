#include "afxdialogex.h"

// caixa de diálogo de CDSelecionarCNs

class CDSelecionarCNs : public CDialogEx
{
	DECLARE_DYNAMIC(CDSelecionarCNs)

	double* pdouLimiteSuperior,*pdouLimiteInferior;
	const std::string* pProjeto;

public:
	CDSelecionarCNs(const std::string* pProj,CWnd * pParent = nullptr, double* pCotaAcima = nullptr, double* CotaAbaixo = nullptr);   // construtor padrão
	virtual ~CDSelecionarCNs();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIASELECIONARCNS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	void LimpaOutrosEdits(unsigned int Edit);

	DECLARE_MESSAGE_MAP()
public:
	double dNaCota;
	double dAcimaDaCota;
	double dAbaixoDaCota;
	double dCotaInicial;
	double dCotaFinal;
	BOOL bNaCota;
	BOOL bAcimaCota;
	BOOL bAbaixoCota;
	BOOL bNaFaixa;
	BOOL bTodas;

	afx_msg void OnBnClickedChecnnacota();
	afx_msg void OnBnClickedChecnacimadacota();
	afx_msg void OnBnClickedChecnabaixodacota();
	afx_msg void OnBnClickedChecnnafaixa();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedChetodascn();
};
