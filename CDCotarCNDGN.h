// caixa de diálogo de CDCotarCNDGN

class CDCotarCNDGN : public CDialogEx
{
	DECLARE_DYNAMIC(CDCotarCNDGN)

	double* pCotaCN;

public:
	CDCotarCNDGN(double* ppCotaCN,CWnd* pParent = nullptr);   // construtor padrão
	virtual ~CDCotarCNDGN();
	void MoverPara(int x, int y)
	{

		CRect rect;
		GetWindowRect(rect);
		int dx = rect.Width();
		int dy = rect.Height();
		GetWindowRect(rect);
		rect.left = rect.right - dx;
		rect.bottom = rect.top + dy;
		MoveWindow(rect);
//		ShowWindow(SW_SHOW);








		/*

		 RECT Ret;
		 GetWindowRect(&Ret);

		 Ret.left = x;
		 Ret.top = y;

		 MoveWindow(&Ret, true);
		 */
	}

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COTARCN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	UINT Cota;
	CString CotaCN;
};
