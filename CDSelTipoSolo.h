// CDSelTipoSolo dialog

#include "afxwin.h"

typedef std::list<std::string> tylststring;
typedef tylststring::iterator ittylststring;

class CDSelTipoSolo : public CDialogEx
{
	DECLARE_DYNAMIC(CDSelTipoSolo)

  tylststring& ListaTiposSolos;
   std::string& StrResposta;

public:
	CDSelTipoSolo(CWnd* pParent,tylststring& LstTipos,std::string& pStrResposta);   // standard constructor
	virtual ~CDSelTipoSolo();

// Dialog Data
	enum { IDD = IDD_DIATIPOPERFILGEOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CComboBox ComTipoSolo;

  virtual INT_PTR DoModal();
  virtual BOOL OnInitDialog();
};
