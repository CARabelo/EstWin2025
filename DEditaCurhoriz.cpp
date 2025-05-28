// DEditaCurhoriz.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "math.h"
#include "estaca.h"
#include "cponto.h"
#include <map>
#include "ccurhorizontal.h"
#include "DEditaCurhoriz.h"
#include <string>
#include "mat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class Matem Mat;

#define ESPIRAL 0
#define CIRCULAR 1

/////////////////////////////////////////////////////////////////////////////
// CDEditaCurhoriz dialog

CDEditaCurhoriz::CDEditaCurhoriz(CWnd* pParent,CurHorizontal* pPCH,double pAnguloRotacao,int pTipo) : CDialog(CDEditaCurhoriz::IDD, pParent),CH(pPCH),AnguloRotacao(pAnguloRotacao),PPI(CH->PI),Tipo(pTipo)
{
  if(AnguloRotacao != 0.0) PPI.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),-AnguloRotacao);

  //{{AFX_DATA_INIT(CDEntraPonto)
  m_DoCoorE = Mat.Arredonda(CH->PI.x,3);
  m_DoCoorN = Mat.Arredonda(CH->PI.y,3) ;
  m_DoI = Mat.Arredonda(Mat.rad_gms(CH->Ac),8);
  m_DoR = Mat.Arredonda(CH->Raio,3); 
  m_DoLc = Mat.Arredonda(CH->Lc,2);
  m_DoSe = Mat.Arredonda(CH->Se*100.0,2);
  m_DoSl = Mat.Arredonda(CH->Sl,2);
  //}}AFX_DATA_INIT
}

void CDEditaCurhoriz::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDEditaCurhoriz)
  DDX_Text(pDX, IDC_CORRDN, m_DoCoorN);
  DDX_Text(pDX, IDC_I, m_DoI);
  DDX_Text(pDX, IDC_LC, m_DoLc);
  DDX_Text(pDX, IDC_R, m_DoR);
  DDX_Text(pDX, IDC_SE, m_DoSe);
  DDX_Text(pDX, IDC_SL, m_DoSl);
  DDX_Text(pDX, IDC_COORDE, m_DoCoorE);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDEditaCurhoriz, CDialog)
  //{{AFX_MSG_MAP(CDEditaCurhoriz)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDEditaCurhoriz message handlers

void CDEditaCurhoriz::OnOK() 
{
  // TODO: Add extra validation here

  UpdateData(TRUE);

  CH->PI.x = m_DoCoorE;
  CH->PI.y = m_DoCoorN;
  CH->Ac =  Mat.dou_gms_rad(m_DoI);
  CH->Raio = m_DoR;
  CH->Lc = m_DoLc;
  CH->Se = m_DoSe/100.0;
  CH->Sl = m_DoSl;

  if(CH->Raio < 3.0)
  {
    CH->Raio = 0.0;
    CH->Lc = 0.0;
  }

  if (CH->Lc >=10.0) CH->Tipo = ESPIRAL;   //--- Lc Mínimo.
  else 
  {
    CH->Tipo = CIRCULAR; 
    CH->Lc = 0.0;
  }

  if(AnguloRotacao != 0.0)
    CH->PI.RotacionaPlano(sin(AnguloRotacao),cos(AnguloRotacao),AnguloRotacao);

  CDialog::OnOK();
}

void CDEditaCurhoriz::OnCancel() 
{
  // TODO: Add extra cleanup here

  CDialog::OnCancel();
}


BOOL CDEditaCurhoriz::OnInitDialog()
{
  CDialog::OnInitDialog();

   if(Tipo == PROIBIDO_ALTERAR)
   {
     GetDlgItem(IDC_CORRDN)->EnableWindow(false);
     GetDlgItem(IDC_COORDE)->EnableWindow(false);
   }


  if(Tipo == PI_INICIAL_FINAL || Tipo == PROIBIDO_ALTERAR)
  {
    GetDlgItem(IDC_I)->EnableWindow(false);
    GetDlgItem(IDC_LC)->EnableWindow(false);
    GetDlgItem(IDC_R)->EnableWindow(false);
    GetDlgItem(IDC_SE)->EnableWindow(false);
    GetDlgItem(IDC_SL)->EnableWindow(false);
  }


  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}
