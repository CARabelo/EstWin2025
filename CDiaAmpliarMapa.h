#pragma once


// caixa de diálogo de CDiaAmpliarMapa

class CDiaAmpliarMapa : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaAmpliarMapa)

  bool Delta;

public:
	CDiaAmpliarMapa(CWnd* pParent = nullptr);   // construtor padrão
	virtual ~CDiaAmpliarMapa();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAAMPLIARMAPA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
  CSpinButtonCtrl m_spinAmpliarReduzir;
  afx_msg void OnDeltaposSpiescala(NMHDR *pNMHDR, LRESULT *pResult);
};
