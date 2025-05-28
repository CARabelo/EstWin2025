// DDXF.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "DDXF.h"
#include ".\ddxf.h"


// DDXF dialog

IMPLEMENT_DYNAMIC(DDXF, CDialog)
DDXF::DDXF(CWnd* pParent /*=NULL*/)
	: CDialog(DDXF::IDD, pParent)
{
}

DDXF::~DDXF()
{
}

void DDXF::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DDXF, CDialog)
  ON_BN_CLICKED(1001, OnBnClicked1001)
END_MESSAGE_MAP()


// DDXF message handlers

void DDXF::OnBnClicked1001()
{
  // TODO: Add your control notification handler code here
  int i = 0;
}
