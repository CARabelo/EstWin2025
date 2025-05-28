#pragma once


// CDiaLembretes dialog

class CDiaLembretes : public CDialog
{
	DECLARE_DYNAMIC(CDiaLembretes)

   CMelEditBox m_EdiAzul,m_EdiVermelho;

public:
//	CDiaLembretes(CWnd* pParent = NULL);   // standard constructor
  CDiaLembretes(CWnd* pParent,std::string& strTitulo,std::string& strTexto,COLORREF& CorBandeira);
	virtual ~CDiaLembretes();

// Dialog Data
	enum { IDD = IDD_DIALEMBRETE };

  std::string& strTexto,&strTitulo;
  COLORREF& Cor;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOk();
  CString m_strLembrete;
  CString m_strTitulo;
  COLORREF m_Cor;
  virtual BOOL OnInitDialog();
};
