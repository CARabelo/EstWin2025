#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CDiaGerarCN dialog

class CDiaGerarCN : public CDialog
{
	DECLARE_DYNAMIC(CDiaGerarCN)

public:
	CDiaGerarCN(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDiaGerarCN();

// Dialog Data
	enum { IDD = IDD_DIAGERARCN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  double doEquiCN;
  afx_msg void OnBnClickedButiniciar();
  CProgressCtrl ProCalcCN;
};
