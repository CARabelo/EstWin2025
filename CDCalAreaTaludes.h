// CDCalAreaTaludes dialog

#include "afxcmn.h"
class CDCalAreaTaludes : public CDialog
{
	DECLARE_DYNAMIC(CDCalAreaTaludes)

public:
	CDCalAreaTaludes(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDCalAreaTaludes();

// Dialog Data
	enum { IDD = IDD_DIACALCTALUDES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedButiniciarcalctal();
  CProgressCtrl CProgBar;
};
