// caixa de diálogo de CDiaGravarPonInsSecoes

class CDiaGravarPonInsSecoes : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaGravarPonInsSecoes)

public:
	CDiaGravarPonInsSecoes(CWnd* pParent = nullptr);   // construtor padrão
	virtual ~CDiaGravarPonInsSecoes();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAINCPONTOSINSSECOES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
};
