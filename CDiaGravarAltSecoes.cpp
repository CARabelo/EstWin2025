// CDiaGravarAltSecoes.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "CDiaGravarAltSecoes.h"
#include "afxdialogex.h"

// caixa de diálogo de CDiaGravarAltSecoes

IMPLEMENT_DYNAMIC(CDiaGravarAltSecoes, CDialogEx)

CDiaGravarAltSecoes::CDiaGravarAltSecoes(CWnd* pParent,bool* pIncPontos)
	: CDialogEx(IDD_DIACALCSECAVULSAS, pParent)//,pIncluirPontosSuperf(pIncPontos)
{

}

CDiaGravarAltSecoes::~CDiaGravarAltSecoes()
{
}

void CDiaGravarAltSecoes::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDiaGravarAltSecoes, CDialogEx)
  ON_BN_CLICKED(IDOK, &CDiaGravarAltSecoes::OnBnClickedOk)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDiaGravarAltSecoes

void CDiaGravarAltSecoes::OnBnClickedOk()
{

 // *pIncluirPontosSuperf = ((CButton*)GetDlgItem(IDC_RADINCLUIRPONTOSSUPERF))->GetCheck();
    
    CDialogEx::OnOK();
}
