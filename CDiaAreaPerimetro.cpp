// CDiaAreaPerimetro.cpp : arquivo de implementação
//

#include "StdAfx.h"
#include "Est.h"
#include "CDiaAreaPerimetro.h"
#include "afxdialogex.h"

// caixa de diálogo de CDiaAreaPerimetro

IMPLEMENT_DYNAMIC(CDiaAreaPerimetro, CDialogEx)

CDiaAreaPerimetro::CDiaAreaPerimetro(CWnd* pParent,double pArea,double pPerimetro)
	: CDialogEx(IDD_DIAAREAPERIMETRO, pParent)
{
  EdiArea.Format("%12.2lf ", fabs(pArea));
  EdiPerimetro.Format("%12.2lf", pPerimetro);
}

CDiaAreaPerimetro::~CDiaAreaPerimetro()
{
}

void CDiaAreaPerimetro::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIAREA, EdiArea);
  DDX_Text(pDX, IDC_EDIPERIMETRO, EdiPerimetro);
}

BEGIN_MESSAGE_MAP(CDiaAreaPerimetro, CDialogEx)
END_MESSAGE_MAP()
