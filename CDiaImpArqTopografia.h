
// caixa de diálogo de CDiaImpArqTopografia

class CDiaImpArqTopografia : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaImpArqTopografia)

	tlstwstr CamadasAcad;

	std::string& NomeArqTriangulacao,&NomeArqAciTopograficos,&CNMestra,&CNInterm;

public:
	CDiaImpArqTopografia(std::string& NomeArqTrian,std::string& NomeArqAciTopog,
	                     std::string& pCNMestra,std::string& pCNInterm,
						 CWnd* pParent = nullptr);   

	virtual ~CDiaImpArqTopografia();
	int LeCamadasAcad(const std::string& NomeArquivo);

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAIMPTRIATOPOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:

	CString NomeArqTriang, NomeArqAciTopog;

	afx_msg void OnBnClickedButpegararqmalha();
	afx_msg void OnBnClickedButpegararqtopografia();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnKillfocusEdiarqacitopog();

	CComboBox CComboCN_Mestra;
	CComboBox CComboCM_Interm;
	CEdit CEdit_NomeArqTopog, CE_NomeArqTrian;
	afx_msg void OnCbnSetfocusComcmMestra();
};
