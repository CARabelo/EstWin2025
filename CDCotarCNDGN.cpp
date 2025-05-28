// CDCotarCNDGN.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "CDCotarCNDGN.h"
#include "afxdialogex.h"

// caixa de diálogo de CDCotarCNDGN

IMPLEMENT_DYNAMIC(CDCotarCNDGN, CDialogEx)

CDCotarCNDGN::CDCotarCNDGN(double* ppCotaCN, CWnd* pParent)	: pCotaCN(ppCotaCN), CDialogEx(IDD_COTARCN, pParent), CotaCN(_T(""))
{
  if(*ppCotaCN != INFINITO)  Cota = *ppCotaCN;
	else Cota = 0;
}

CDCotarCNDGN::~CDCotarCNDGN()
{
}

void CDCotarCNDGN::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COTACN, Cota);
}

BEGIN_MESSAGE_MAP(CDCotarCNDGN, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDCotarCNDGN::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDCotarCNDGN::OnBnClickedCancel)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDCotarCNDGN

BOOL CDCotarCNDGN::OnInitDialog()
{
  UpdateData(false);
	return TRUE;
}
void CDCotarCNDGN::OnBnClickedOk()
{
  UpdateData(true);

	*pCotaCN = Cota;

	CDialogEx::OnOK();
}

void CDCotarCNDGN::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}
