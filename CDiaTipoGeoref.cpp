// CDiaTipoGeoref.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "CDiaTipoGeoref.h"
#include "afxdialogex.h"

// caixa de diálogo de CDiaTipoGeoref

IMPLEMENT_DYNAMIC(CDiaTipoGeoref, CDialogEx)

CDiaTipoGeoref::CDiaTipoGeoref(int* pTipGeoref,CWnd* pParent) : CDialogEx(IDD_DIATIPOGEOREF, pParent), 
   pTipoGeoref(pTipGeoref),
   RadGoogleEarth(false),
   RadDrones(false),
   RadGerenciadorMapa(false)
{
  *pTipoGeoref = -1;
  RadDrones = true;
}

CDiaTipoGeoref::~CDiaTipoGeoref()
{
}

void CDiaTipoGeoref::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Check(pDX, IDC_RADGOOGLEEARTH, RadGoogleEarth);
  DDX_Check(pDX, IDC_RADDRONES,RadDrones);
  DDX_Check(pDX, IDC_RADGERENMAPAS, RadGerenciadorMapa);
}

BEGIN_MESSAGE_MAP(CDiaTipoGeoref, CDialogEx)
  ON_BN_CLICKED(IDOK, &CDiaTipoGeoref::OnBnClickedOk)
  ON_BN_CLICKED(IDCANCEL, &CDiaTipoGeoref::OnBnClickedCancel)
  ON_BN_CLICKED(IDC_RADDRONES, &CDiaTipoGeoref::OnBnClickedRaddrones)
  ON_BN_CLICKED(IDC_RADGOOGLEEARTH, &CDiaTipoGeoref::OnBnClickedRadgoogleearth)
  ON_BN_CLICKED(IDC_RADGERENMAPAS, &CDiaTipoGeoref::OnBnClickedRadgerenmapas)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDiaTipoGeoref

void CDiaTipoGeoref::OnBnClickedOk()
{
  UpdateData(true);

  *pTipoGeoref = RadGoogleEarth ? 1 : RadDrones ? 2 : 3;

  CDialogEx::OnOK();
}

void CDiaTipoGeoref::OnBnClickedCancel()
{
  CDialogEx::OnCancel();
}

void CDiaTipoGeoref::OnBnClickedRaddrones()
{
  RadGoogleEarth = RadGerenciadorMapa = false;
  RadDrones = true;

  UpdateData(false);
}

void CDiaTipoGeoref::OnBnClickedRadgoogleearth()
{
  RadDrones = RadGerenciadorMapa = false;
  RadGoogleEarth = true;

  UpdateData(false);
}

void CDiaTipoGeoref::OnBnClickedRadgerenmapas()
{
    RadGoogleEarth = RadDrones = false;
    RadGerenciadorMapa = true;

    UpdateData(false);
}
