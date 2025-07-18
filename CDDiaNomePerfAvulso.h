#pragma once

// caixa de diálogo de CDDiaNomePerfAvulso

class CDDiaNomePerfAvulso : public CDialogEx
{
	DECLARE_DYNAMIC(CDDiaNomePerfAvulso)

public:
	CDDiaNomePerfAvulso(std::string& pstrNomePerf, CWnd* pParent = nullptr);   // construtor padrão
	virtual ~CDDiaNomePerfAvulso();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIANOMEPERFAVULSO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CString strNome;
	afx_msg void OnBnClickedOk();
};
