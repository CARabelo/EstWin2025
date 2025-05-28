#pragma once


// caixa de diálogo de CDAlterDescPonto

class CDAlterDescPonto : public CDialogEx
{
	DECLARE_DYNAMIC(CDAlterDescPonto)

public:
	CDAlterDescPonto(CString* pDescricao,CWnd* pParent = nullptr);   // construtor padrão
	virtual ~CDAlterDescPonto();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAALTDESCPONTO };
#endif

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
