#pragma once


// CDiaRemAciTopog dialog

class CDiaRemAciTopog : public CDialog
{
	DECLARE_DYNAMIC(CDiaRemAciTopog)

  CPoint Local;
	std::string Texto;

public:
	CDiaRemAciTopog(CWnd* pParent,CPoint Ponto,const std::string& pTexto = std::string(""));   // standard constructor
	virtual ~CDiaRemAciTopog();

// Dialog Data
	enum { IDD = IDD_DIAREMRESTRICAO};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
};
