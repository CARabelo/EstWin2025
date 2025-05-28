// CDConfigRadar dialog

#include "afxwin.h"
#include "afxcolorbutton.h"

class CDConfigRadar : public CDialogEx
{
	DECLARE_DYNAMIC(CDConfigRadar)

  int* pDados;

public:
	CDConfigRadar(CWnd* pParent,int* pDados);   // standard constructor
	virtual ~CDConfigRadar();

// Dialog Data
	enum { IDD = IDD_DIACONFIGRADAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  int Raio;
  unsigned long CorTela;
  BOOL m_bTodosPontos;
  BOOL m_bSoPontosInteresse;
  BOOL m_bRealcarPontosInteresse;
  afx_msg void OnBnClickedOk();
  COLORREF m_CorTela;
  CMFCColorButton m_butCor;
};
