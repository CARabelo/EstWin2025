// CDiaInsererua dialog

#include "afxwin.h"
class CDiaInsereRua : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaInsereRua)

  const tlstwstr& ListaRuas;
  std::string& pNovaRua,&pCopiar,
             * ptCopiarOutroProjeto;
  CString NomeRua;

public:
	CDiaInsereRua(CWnd* pParent,const tlstwstr& ListaRuas,std::string& ppNovaRua,std::string& pCopiar,std::string* pCopiarOutroProjeto);   // standard constructor
	virtual ~CDiaInsereRua(){};

// Dialog Data
	enum { IDD = IDD_DIAINSERIRRUA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CEdit NomeRuaTrecho;
  CComboBox ComCopiarArquivos;
  afx_msg void OnBnClickedOk();
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedButoutroprojeto();
};
