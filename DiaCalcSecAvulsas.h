#pragma once

// CDiaCalcSecAvulsas dialog

class CDiaCalcSecAvulsas : public CDialog
{
	DECLARE_DYNAMIC(CDiaCalcSecAvulsas)

public:
	CDiaCalcSecAvulsas(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDiaCalcSecAvulsas();

// Dialog Data
	enum { IDD = IDD_DIACALCSECAVULSAS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOk();
};
