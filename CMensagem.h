#pragma once


// CMensagem dialog

class CMensagem : public CDialog
{
	DECLARE_DYNAMIC(CMensagem)

public:
	CMensagem(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMensagem();

// Dialog Data
	enum { IDD = IDD_MENSAGEM};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  void MostraMensagem(const std::string& Msg);
  void Apaga();

  CString m_strMensagem;

  virtual BOOL OnInitDialog();
  afx_msg void OnCancel();
  afx_msg void OnOk();
  virtual void PostNcDestroy();

  BOOL Create(CWnd * pWnd = NULL) { return CDialog::Create(IDD, pWnd); }

};
