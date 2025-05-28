#pragma once

// caixa de diálogo de CDInicial

class CDInicial : public CDialogEx
{
	DECLARE_DYNAMIC(CDInicial)

	int& TipoProjeto;
	CString* pLstProjRecentes;
	bool ProjExistente;

public:
	std::string ProjSelecionado;

	CDInicial(int& TipoProj,CWnd* pParent = nullptr,CString* ppLstProjRec = nullptr,bool ProjExistente = false);   // construtor padrão
	virtual ~CDInicial();

	const std::string& ProjetoSelecionado() const { return ProjSelecionado; };

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAINICIAL };
#endif

protected:
	void AtualizaListaProjRecentes();

	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtopdrones();
	afx_msg void OnBnClickedButtoptradicional();
	afx_msg void OnBnClickedButtopmapasibge();
	afx_msg void OnBnClickedButtopgoogle();
	afx_msg void OnBnClickedButmaistoptradi();
	afx_msg void OnBnClickedButmaisdrones();
	afx_msg void OnBnClickedButmaisibge();
	afx_msg void OnBnClickedButmaisgoogle();
	afx_msg void OnBnClickedButtopacad();
	afx_msg void OnBnClickedButmaistopautocad();
	CMListBox CLstProjExistente;
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButordenarproj();
	afx_msg void OnLbnDblclkLisprojexistente();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButprocurarproj();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};
