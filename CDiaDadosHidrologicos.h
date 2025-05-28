// caixa de diálogo de CDiaDadosHidrologicos

class CDiaDadosHidrologicos : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaDadosHidrologicos)

	bool Calcular,Calculado;
	CDadosBacia* pDadosBacia;
	CBacia& Bacia;
	CImageList ImagensTc,ImagensQc;
	void* pSuperficie;

	void MudouParametros();
	void MudouParametrosCanal();
	double CalculaTc(int Metodo);
	double CalculaQ(int Metodo);
	double CalculaKokeiUehara();

	Ponto CalculaCG();

	int Calculou;

public:
	CDiaDadosHidrologicos(CBacia& Bacia,CWnd* pParent = nullptr,int Calculou = false);  
	virtual ~CDiaDadosHidrologicos();
	void CalculaDeclividadeEquivalente();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIADADOSHIDROL };
#endif

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	double douDeclivEquiv;
	double douTempoConc_Tc;
	double douCoefForma;
	double douCoeVolEscoam;
	double douCoeEscSuperf;
	double douVolEscoHidrog;
	double douVazaoCheia;
	double douVazaoMaxProj;
	double douVelocMediaMann;
	double douRaioHidraulico;
	double douVazaoContMann;
	double douVazaoMannContinuidade;
	double douIntensChuva_i;
	double douDuracaoChuva_t;
	double douRetornoChuva_T;
	double douItenschuva_it;
	double douCoefDistrEspa_K;
	double douCoefRugosidadeCanal;
	double douAreaMolhadaCanal;
	double FatorLinear;
	double FatorArea;
	double douFatorDefluvio_X;
	double douFatorClimatico_Y;
	double douFatorRedTempo_Z;
	double douFatorRetardancia_N;
	double douFatorCaracteristicas_K;
	double douCoefRetardo_Teta;
	double douCoefArmazenamento_Ct;
	double douInsensidadeMaxima_I; 
	double douTempoRetardamento_Tr;
	double douDeclivMediaBacia_I;
	double douRaioHidraulicoCanal;
	double douDeclividadeCanal;
	double douDiaMinimoBueiro;
	double douqm;
	double douVegetacao;

	int intQtdBueiros;

	CButton ButCalcular,ButCalcularCanal;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButcalcular();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButcalcfinais();
	afx_msg void OnBnClickedButcalcularcanal();
	CImageListBox LisMetodosTc, LisMetodosQc;
	afx_msg void OnLbnSelchangeLisqc();
	afx_msg void OnLbnSelchangeListc();
	afx_msg LRESULT OnRolouImageList(WPARAM WP, LPARAM LP);
	afx_msg void OnUpdateButCalcular(CCmdUI* pCmdUI);

	CButton RadPonte;
	CButton RadBSTC;
};
