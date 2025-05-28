#pragma once


// caixa de diálogo de CDiaAreaPerimetro

class CDiaAreaPerimetro : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaAreaPerimetro)

public:
	CDiaAreaPerimetro(CWnd* pParent = nullptr,double pArea = 0.0,double pPerimetro = 0.0);   // construtor padrão
	virtual ~CDiaAreaPerimetro();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAAREAPERIMETRO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CString EdiArea;
	CString EdiPerimetro;
};
