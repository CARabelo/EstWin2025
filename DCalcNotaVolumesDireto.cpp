// DCalcNotaVolumesDireto.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "DCalcNotaVolumesDireto.h"

// CDCalcNotaVolumesDireto dialog

IMPLEMENT_DYNAMIC(CDCalcNotaVolumesDireto, CDialog)

CDCalcNotaVolumesDireto::CDCalcNotaVolumesDireto(CWnd* pParent) : CDialog(CDCalcNotaVolumesDireto::IDD, pParent){}

CDCalcNotaVolumesDireto::~CDCalcNotaVolumesDireto(){}

void CDCalcNotaVolumesDireto::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);DDX_Control(pDX, IDC_PROGRESSO, m_Progresso);
}

BEGIN_MESSAGE_MAP(CDCalcNotaVolumesDireto, CDialog)
END_MESSAGE_MAP()

// CDCalcNotaVolumesDireto message handlers
