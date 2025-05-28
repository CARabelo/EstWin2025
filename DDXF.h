#pragma once


// DDXF dialog

class DDXF : public CDialog
{
	DECLARE_DYNAMIC(DDXF)

public:
	DDXF(CWnd* pParent = NULL);   // standard constructor
	virtual ~DDXF();

// Dialog Data
	enum { IDD = IDD_TEST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClicked1001();
};
