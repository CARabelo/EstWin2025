#pragma once


// caixa de diálogo de CDMensagem  ===>>> NÃO ESTÁ FUNCIONANDO A MENSAGEM NÃO APARECE

class CDMensagem : public CDialog
{
	DECLARE_DYNAMIC(CDMensagem)

  std::string strMensagem;

public:
  BOOL Create(CWnd* pParent=NULL);
	CDMensagem(CWnd* pParent=NULL);  
	virtual ~CDMensagem();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MENSAGEM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
  CString StaMensagem;

  void MostraMensagem(std::string& Mensagem)
  {
    StaMensagem = Mensagem.c_str();

    UpdateData(true);
  }
};
