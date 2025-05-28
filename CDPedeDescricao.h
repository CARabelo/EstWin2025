#pragma once

// CDPedeDescricao dialog

class CDPedeDescricao : public CDialog
{
	DECLARE_DYNAMIC(CDPedeDescricao)

public:
	CDPedeDescricao(CString* pDescricao, int* pCalcCotaSuper,double* pCotaPonto,CWnd* pParent = NULL);   
	virtual ~CDPedeDescricao();

// Dialog Data
	enum { IDD = IDD_DIADESCRICAO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CString* Descricao;
  int* CalcCotaSuperficie;
  double* CotaPonto;
  afx_msg void OnBnClickedOk();
  afx_msg void OnChecalcsuperficie();
};
