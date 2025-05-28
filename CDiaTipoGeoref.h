// caixa de diálogo de CDiaTipoGeoref

class CDiaTipoGeoref : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaTipoGeoref)

  int* pTipoGeoref;

public:
	CDiaTipoGeoref(int* pTipGeoref, CWnd* pParent = nullptr);   // construtor padrão
	virtual ~CDiaTipoGeoref();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIATIPOGEOREF };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
  BOOL RadGoogleEarth;
  BOOL RadDrones;
  BOOL RadGerenciadorMapa;
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedRaddrones();
  afx_msg void OnBnClickedRadgoogleearth();
  afx_msg void OnBnClickedRadgerenmapas();
};
