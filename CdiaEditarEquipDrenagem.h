// caixa de diálogo de CdiaEditarEquipDrenagem

class CdiaEditarEquipDrenagem : public CDialogEx
{
	DECLARE_DYNAMIC(CdiaEditarEquipDrenagem)

	CString Esconsidade,AzGraus;
	BOOL bForcar90Graus;
	double * pDimensoes;
	std::string Nome;
	int QtdDimensoes;
	int Equipamento;
	CBitmap BueiroBMP,ValetaBMP,TubulacaoBMP,SarjetaBMP,DescidaDaguaBMP,CaixaColetoraBMP,BaciaDissipacaoBMP,
	        ValetaTerraplBMP,ValetaTNBMP, CanaletaBMP,CanaletaTerrapBMP;

	double dEsconsidade;

public:
	CdiaEditarEquipDrenagem(double* pDimensoes,int pQtdDim,const std::string& pNome,int pTipo,CWnd* pParent = nullptr,double pEsconsidade=INFINITO,double pAzimute = INFINITO);   // construtor padrão
	virtual ~CdiaEditarEquipDrenagem();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = "IDD_DIAEDITAREQUIPDRENAGEM" };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	double Dim_A, Dim_B, Dim_C, Dim_D, Dim_E, Dim_F, Dim_G,Azimute;
	CStatic CroquisBMP;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
