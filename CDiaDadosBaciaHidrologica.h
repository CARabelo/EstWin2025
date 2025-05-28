// caixa de diálogo de CDiaDadosBaciaHidrologica

class CDiaDadosBaciaHidrologica : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaDadosBaciaHidrologica)

	CDadosBacia DadosBacia,DadosBaciaOriginal;
	CString NomeProjeto;
	CBacia& Bacia;
	tylstBacias& LstBacias;
	ittylstBacias ItBacia;
	void* pSuperficie;
	int Calculou;

	bool ReadWindowPlacement();
	void WriteWindowPlacement();
	void MostraDadosGeometricos();
	Ponto CalculaCG();

public:
	CDiaDadosBaciaHidrologica(CWnd* pParent,ittylstBacias BaciaHidrol,const CString& pNomeProjeto,std::string& NomeBacia,tylstBacias& LstBacias,void* pSuperf);

	virtual ~CDiaDadosBaciaHidrologica();

	std::string& NovoNomeBacia;

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIADADOSBACIAHIDROLOGICA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CString CSArea;
	CString CSKc;
	CString CSKf;
	CString CSLt;
	CString CSDd;
	CString CSl;
	CString CSH;
	CString CSComprimento;
	CString CSNomeBacia;
	CString EquiCNHipso;
	CString CSNCG;
	CString CSECG;
	CString CSCotaCG;
	CString CSDistCGFoz;
	CEdit CEdiNomeBacia;
	CButton ButCalcular;
	CButton ButCalcularHidrol;

	afx_msg void OnBnClickedRadmetros();
	afx_msg void OnBnClickedRadkilometros();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButDadosHidrol();
	afx_msg void OnBnClickedButcalc();
	afx_msg void OnEnChangeEdiequicnhipso();
	afx_msg void OnEnChangeEdinomebacia();

	int BOMetros;
	int BOKilometros;
	CString StrObservacoes;
	afx_msg void OnEnSetfocusEdinomebacia();
	afx_msg void OnEnSetfocusEdiequicnhipso();
};
