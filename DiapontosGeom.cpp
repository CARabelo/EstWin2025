// DiapontosGeom.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "cponto.h"
#include "DiapontosGeom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DiapontosGeom dialog

DiapontosGeom::DiapontosGeom(CWnd* pParent): CDialog(DiapontosGeom::IDD, pParent),pProjeto(NULL),pAcitopog(NULL),pTipoCN(NULL)
{             
  //{{AFX_DATA_INIT(DiapontosGeom)
  //}}AFX_DATA_INIT
}

void DiapontosGeom::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(DiapontosGeom)
  DDX_Radio(pDX, IDC_ACTOPOG, m_iAcidTopog);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(DiapontosGeom, CDialog)
  //{{AFX_MSG_MAP(DiapontosGeom)
  ON_BN_CLICKED(ID_BUTCNTOPOGRAF, OnTipoCN)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DiapontosGeom message handlers


void DiapontosGeom::OnTipoCN() 
{
  if (pTipoCN) *pTipoCN = !*pTipoCN;
}

