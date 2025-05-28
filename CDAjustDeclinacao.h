#pragma once


// caixa de diálogo de CDAjustDeclinacao

class CDAjustDeclinacao : public CDialogEx
{
	DECLARE_DYNAMIC(CDAjustDeclinacao)

	DesPontosGeomView* pDesGeom;
	bool Rotacionar,Transladar;

public:
	CDAjustDeclinacao(CWnd* pParent, DesPontosGeomView* ppDesGeom);   // construtor padrão
	virtual ~CDAjustDeclinacao();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAAJUSTDECLIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButrotacionar();
	afx_msg void OnBnClickedButtransladar();
	CButton ButRotacionar;
	CButton ButTransladar;
};
