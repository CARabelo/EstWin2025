// caixa de diálogo de CDiaThread

class CDiaThread : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaThread)

public:
	CDiaThread(CWnd* pParent = nullptr);   // construtor padrão
	virtual ~CDiaThread();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIATHREAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
  virtual void PostNcDestroy();
public:
  CProgressCtrl Progresso;
  CString Mensagem;

  void AtualizaDialogo(std::string& strMsg, int ValorProgresso)
  {
    if(strMsg != "0")Mensagem = strMsg.c_str();
    Progresso.SetPos(ValorProgresso);

    UpdateData(FALSE);

    RedrawWindow();
  }
  virtual BOOL OnInitDialog();
};
