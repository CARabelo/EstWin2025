// CDAlterDescPonto.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "CDAlterDescPonto.h"
#include "afxdialogex.h"

// caixa de diálogo de CDAlterDescPonto

IMPLEMENT_DYNAMIC(CDAlterDescPonto, CDialogEx)

CDAlterDescPonto::CDAlterDescPonto(CString* pDescricao, CWnd* pParent)	: CDialogEx(IDD_DIAALTDESCPONTO, pParent)
{
}

CDAlterDescPonto::~CDAlterDescPonto()
{
}

void CDAlterDescPonto::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDAlterDescPonto, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDAlterDescPonto::OnBnClickedOk)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDAlterDescPonto

void CDAlterDescPonto::OnBnClickedOk()
{
  UpdateData(true);

	CDialogEx::OnOK();
}

