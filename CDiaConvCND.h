typedef std::list<Ponto> tyLstPontos;
typedef tyLstPontos::iterator ittyLstPontos;

typedef std::list<tyLstPontos> tyLstLstPontos;
typedef tyLstLstPontos::iterator ittyLstLstPontos;

// caixa de diálogo de CDiaConvCND

class CDiaConvCND : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaConvCND)

public:
	CDiaConvCND(CWnd* pParent = nullptr);   // construtor padrão
	virtual ~CDiaConvCND();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	int GravaArqCN_Atual(const std::string& NomeProj, tyLstLstPontos& LCN);
	int LeArqCN_Ante(const std::string& NomeProj, tyLstLstPontos& LCN);


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButconverter();
};
