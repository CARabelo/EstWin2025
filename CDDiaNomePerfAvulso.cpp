// CDDiaNomePerfAvulso.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "CDDiaNomePerfAvulso.h"
#include "afxdialogex.h"

// caixa de diálogo de CDDiaNomePerfAvulso

IMPLEMENT_DYNAMIC(CDDiaNomePerfAvulso, CDialogEx)

CDDiaNomePerfAvulso::CDDiaNomePerfAvulso(std::string& pstrNomePerf, CWnd* pParent)
	: CDialogEx(IDD_DIANOMEPERFAVULSO, pParent)
{

}

CDDiaNomePerfAvulso::~CDDiaNomePerfAvulso()
{
}

void CDDiaNomePerfAvulso::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDNOMEPERFAVULSO, strNome);
	DDV_MaxChars(pDX, strNome, 50);
}


BEGIN_MESSAGE_MAP(CDDiaNomePerfAvulso, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDDiaNomePerfAvulso::OnBnClickedOk)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDDiaNomePerfAvulso

void CDDiaNomePerfAvulso::OnBnClickedOk()
{
  UpdateData(true);
//  strNomePerfil = strNome.GetBuffer();
  CDialogEx::OnOK();
}
