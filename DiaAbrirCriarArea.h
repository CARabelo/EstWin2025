#pragma once

// CDiaAbrirCriarArea dialog

class CDiaAbrirCriarArea : public CDialog
{
	DECLARE_DYNAMIC(CDiaAbrirCriarArea)

  int* pResp;

public:
	CDiaAbrirCriarArea(int* ppResposta,CWnd* pParent = NULL);   // standard constructor
	virtual ~CDiaAbrirCriarArea();

// Dialog Data
	enum { IDD = IDD_NOVARESTRICAO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

  afx_msg void OnBnClickedOk();

  int m_boolCriar;
  int m_boolAbrir;
  afx_msg void OnBnClickedCancel();
};
