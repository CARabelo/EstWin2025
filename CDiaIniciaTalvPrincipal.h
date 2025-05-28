#include "afxdialogex.h"

// caixa de diálogo de CDiaIniciaTalvPrincipal

class CDiaIniciaTalvPrincipal : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaIniciaTalvPrincipal)

public:
	CDiaIniciaTalvPrincipal(CWnd* pParent = nullptr);   // construtor padrão
	virtual ~CDiaIniciaTalvPrincipal();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAINITALVEGUEPRINCIPAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
};
