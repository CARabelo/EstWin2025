// caixa de diálogo de CDiaDesinstalar

class CDiaDesinstalar : public CDialog
{
	DECLARE_DYNAMIC(CDiaDesinstalar)

public:
	CDiaDesinstalar(CWnd* pParent = nullptr);   // construtor padrão
	virtual ~CDiaDesinstalar();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIADESINSTALAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedIdajudadesinstalar();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
