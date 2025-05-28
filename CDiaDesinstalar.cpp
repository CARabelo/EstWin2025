// CDiaDesinstalar.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "CDiaDesinstalar.h"

// caixa de diálogo de CDiaDesinstalar

IMPLEMENT_DYNAMIC(CDiaDesinstalar, CDialog)

CDiaDesinstalar::CDiaDesinstalar(CWnd* pParent) : CDialog(IDD_DIADESINSTALAR, pParent)
{
}

CDiaDesinstalar::~CDiaDesinstalar()
{
}

void CDiaDesinstalar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDiaDesinstalar, CDialog)
	ON_BN_CLICKED(IDAJUDADESINSTALAR, &CDiaDesinstalar::OnClickedIdajudadesinstalar)
	ON_BN_CLICKED(IDCANCEL, &CDiaDesinstalar::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDiaDesinstalar::OnBnClickedOk)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDiaDesinstalar

void CDiaDesinstalar::OnClickedIdajudadesinstalar()
{
  ((CEstApp*)AfxGetApp())->ChamaHelp("InstalacaoDesinstalacao");
}

void CDiaDesinstalar::OnBnClickedCancel()
{
	CDialog::OnCancel();
}

void CDiaDesinstalar::OnBnClickedOk()
{
  if (AfxMessageBox("O programa Vias Urbanas & Estradas será desinstalado deste computador", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	{
		//Desinstalar();
	  AfxMessageBox("O programa Vias Urbanas & Estradas foi desinstalado deste computador", MB_OK | MB_ICONEXCLAMATION);

		exit(0);
	}

	CDialog::OnOK();
}

BOOL CDiaDesinstalar::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE; 
}
