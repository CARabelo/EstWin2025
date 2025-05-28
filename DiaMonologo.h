#pragma once
#include "afxwin.h"

// CDiaMonologo dialog

class CDiaMonologo : public CDialog
{
	DECLARE_DYNAMIC(CDiaMonologo)

  std::string strMensagem;
  bool bMostrarIgnorar;

public:
  CDiaMonologo(const std::string strMsg,bool bMostrarIgn=false, CWnd* pParent=NULL);
	
  ~CDiaMonologo() {};

// Dialog Data
	enum { IDD = IDD_DIAMONOLOGO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CStatic m_STAMensagem;
  CButton m_CHEIgnorar;
  afx_msg void OnBnClickedOk();
  virtual BOOL OnInitDialog();

  BOOL CDiaMonologo::PegaIgnorar() { return m_CHEIgnorar.GetState(); }; 
};
