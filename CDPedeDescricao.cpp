// CDPedeDescricao.cpp : implementation file
//

#include "stdafx.h"
#include "Est.h"
#include "CDPedeDescricao.h"
#include "afxdialogex.h"

// CDPedeDescricao dialog

IMPLEMENT_DYNAMIC(CDPedeDescricao, CDialog)

CDPedeDescricao::CDPedeDescricao(CString* pDescricao, int* pCalcCotaSuper,double* pCotaPonto,CWnd* pParent) : CDialog(CDPedeDescricao::IDD, pParent),Descricao(pDescricao),CalcCotaSuperficie(pCalcCotaSuper),CotaPonto(pCotaPonto)
{
}

CDPedeDescricao::~CDPedeDescricao()
{
}

void CDPedeDescricao::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX,IDC_EDIDESCRICAO, *Descricao);
  DDX_Text(pDX,IDC_EDICOTA, *CotaPonto);
  DDX_Check(pDX,IDC_CHECALCSUPERFICIE, *CalcCotaSuperficie);
	DDV_MinMaxInt(pDX, (int)*CotaPonto, -1000, 10000);
}

BEGIN_MESSAGE_MAP(CDPedeDescricao, CDialog)
  ON_BN_CLICKED(IDOK, &CDPedeDescricao::OnBnClickedOk)
  ON_COMMAND(IDC_CHECALCSUPERFICIE, &CDPedeDescricao::OnChecalcsuperficie)
END_MESSAGE_MAP()

// CDPedeDescricao message handlers

void CDPedeDescricao::OnBnClickedOk()
{
  UpdateData(true);

  if(*CotaPonto == 0.0 && *CalcCotaSuperficie == false) 
  {
    AfxMessageBox("Digite uma cota ou selecione para que ela seja calculada na superfície");
    return;
  }

  CDialog::OnOK();
}

void CDPedeDescricao::OnChecalcsuperficie()
{
  UpdateData(true);

  GetDlgItem(IDC_EDICOTA)->EnableWindow(*CalcCotaSuperficie);
}
