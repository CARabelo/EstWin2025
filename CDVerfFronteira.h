#pragma once


// CDVerfFronteira dialog

class CDVerfFronteira : public CDialog
{
	DECLARE_DYNAMIC(CDVerfFronteira)

public:
	CDVerfFronteira(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDVerfFronteira();

// Dialog Data
	enum { IDD = IDD_DIAVERFFRONTEIRA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnClickedButlog();
  CString m_strMensagem;
  void MostraMensagem(const std::string& parMensagem);
};
