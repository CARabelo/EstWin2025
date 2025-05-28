#pragma once
//#include "afxcmn.h"

// CDiaCalcLimpeza dialog

class CDiaCalcLimpeza : public CDialog
{
public:
  CDiaCalcLimpeza(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDiaCalcLimpeza();

// Dialog Data
	enum { IDD = IDD_DIACALCLIMPEZA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CProgressCtrl CProgLimpeza;
  afx_msg void OnBnClickedButiniciarcalclimpeza();
};
