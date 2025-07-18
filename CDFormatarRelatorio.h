#pragma once


// caixa de diálogo de CDFormatarRelatorio

class CDFormatarRelatorio : public CDialogEx
{
	DECLARE_DYNAMIC(CDFormatarRelatorio)
  int * pCasasDecimais;

public:
	CDFormatarRelatorio(CWnd* pParent=nullptr,int* pCasasDecomais=nullptr);   // construtor padrão
	virtual ~CDFormatarRelatorio();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAFORMATARRELAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
  BOOL cheFormatar;
  CComboBox ComboCasasDecimais;
  virtual BOOL OnInitDialog();
//  afx_msg void OnClickedCheformatar();
  afx_msg void OnClickedCheformatar();
  CStatic staCasasDec;
  afx_msg void OnBnClickedOk();
};
