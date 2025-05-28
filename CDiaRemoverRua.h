#include "afxwin.h"
class CDiaRemoverRua : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaRemoverRua)

  const tlstwstr& ListaRuas;
  std::string& pRua;
  CString NomeRua;

public:
  CDiaRemoverRua::CDiaRemoverRua(CWnd* pParent,const tlstwstr& pListaRuas,std::string& ppRua);
	virtual ~CDiaRemoverRua(){};

// Dialog Data
	enum { IDD = IDD_DIAREMOVERRUA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CComboBox ComRuas;
  afx_msg void OnBnClickedOk();
  virtual BOOL OnInitDialog();
};

