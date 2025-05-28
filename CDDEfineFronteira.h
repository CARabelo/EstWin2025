#pragma once

// CDDEfineFronteira dialog

class CDDEfineFronteira : public CDialog
{
  DECLARE_DYNAMIC(CDDEfineFronteira)

  int Erro;

public:
	CDDEfineFronteira(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDDEfineFronteira();

// Dialog Data
	enum { IDD = IDD_DIACALCALPHASHAPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  unsigned long DMPIdeal;

  void WriteWindowPlacement();
  bool ReadWindowPlacement();

	DECLARE_MESSAGE_MAP()
public:
  double doDMT;
  DesPontosGeomView* pDesPonGeomView;

  afx_msg void OnEnChangeEdit1();
  afx_msg void OnEnChangeEdidmp();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedButcalcular();
  virtual BOOL OnInitDialog();
};
