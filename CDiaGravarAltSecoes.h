#pragma once


// caixa de diálogo de CDiaGravarAltSecoes

class CDiaGravarAltSecoes : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaGravarAltSecoes)

 // bool* pIncluirPontosSuperf;

public:
	CDiaGravarAltSecoes(CWnd* pParent = nullptr,bool* pIncPontos = NULL);   // construtor padrão
	virtual ~CDiaGravarAltSecoes();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIACALCSECAVULSAS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOk();
};
