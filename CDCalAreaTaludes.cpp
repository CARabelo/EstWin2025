// CDCalAreaTaludes.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include <string>
#include "cponto.h"
#include "clpontos.h"
#include "estaca.h"
#include "perfil.h"
#include "supercstring.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include <list>
#include "csecao.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "clistasecoes.h"
#include "mat.h"
#include "monologo.h"
#include "CDCalAreaTaludes.h"
#include "CCalcAreaTaludes.h"

// CDCalAreaTaludes dialog

IMPLEMENT_DYNAMIC(CDCalAreaTaludes, CDialog)
CDCalAreaTaludes::CDCalAreaTaludes(CWnd* pParent ): CDialog(CDCalAreaTaludes::IDD, pParent){}

CDCalAreaTaludes::~CDCalAreaTaludes(){}

void CDCalAreaTaludes::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_PROGCALCTALUDES, CProgBar);
}


BEGIN_MESSAGE_MAP(CDCalAreaTaludes, CDialog)
  ON_BN_CLICKED(IDC_BUTINICIARCALCTAL, OnBnClickedButiniciarcalctal)
END_MESSAGE_MAP()


// CDCalAreaTaludes message handlers

void CDCalAreaTaludes::OnBnClickedButiniciarcalctal()
{
  CCalcAreaTaludes CalcAreaTaludes;

  if(CalcAreaTaludes.CalculaAreas(CProgBar,GetDlgItem(IDC_STAOBS)) == true)
    SetDlgItemText(IDC_STAOBS,"OK, os cálculos foram executados com êxito.");

   GetDlgItem(IDC_BUTINICIARCALCTAL)->EnableWindow(false);
   GetDlgItem(IDOK)->EnableWindow(true);
   GetDlgItem(IDCANCEL)->EnableWindow(false);
}

