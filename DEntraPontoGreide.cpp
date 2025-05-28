// DEntraPontoGreide.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "DEntraPontoGreide.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TERRENO 0
#define INFINITO 1.7976931348623158e+308

/////////////////////////////////////////////////////////////////////////////
// CDEntraPontoGreide dialog

CDEntraPontoGreide::CDEntraPontoGreide(CString& Estaca,CString& Igualdade,double& Cota,double& i1,double& i2,double& Y1,double& Y2,CPoint& Ponto,
                                       int TipoPonto,CWnd* pParent): CDialog(CDEntraPontoGreide::IDD, pParent),m_CSEstaca(Estaca),m_CSIG(Igualdade),
                                       m_doCota(Cota),m_doI1(i1),m_doI2(i2),m_doY1(Y1),m_doY2(Y2),Tipo(TipoPonto),JanPai(pParent),Posx(Ponto.x),
                                       Posy(Ponto.y)
{
  if(m_doI1 == INFINITO || m_doI1 == -INFINITO) m_doI1 = 0.0;  
  if(m_doI2 == INFINITO || m_doI2 == -INFINITO) m_doI2 = 0.0;  

  //{{AFX_DATA_INIT(CDEntraPontoGreide)
  //}}AFX_DATA_INIT
}

void CDEntraPontoGreide::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDEntraPontoGreide)
  DDX_Text(pDX, IDC_ESTACA, m_CSEstaca);
  DDV_MaxChars(pDX, m_CSEstaca, 15);
  DDX_Text(pDX, IDC_I1, m_doI1);
  DDV_MinMaxDouble(pDX, m_doI1, -1.0,1.0);
  DDX_Text(pDX, IDC_I2, m_doI2);  
  DDV_MinMaxDouble(pDX, m_doI2, -1.0,1.0);
  DDX_Text(pDX, IDC_IG, m_CSIG);
  DDX_Text(pDX, IDC_Y1, m_doY1);
  DDV_MinMaxDouble(pDX, m_doY1,0.0,10000.0);
  DDX_Text(pDX, IDC_Y2, m_doY2);
  DDV_MinMaxDouble(pDX, m_doY2,0.0,10000.0);
  DDX_Text(pDX, IDC_Y, m_doCota);
  DDV_MinMaxDouble(pDX, m_doCota,-1000.0,10000.0);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDEntraPontoGreide, CDialog)
  //{{AFX_MSG_MAP(CDEntraPontoGreide)
  ON_BN_CLICKED(IDC_CANCEL, OnCancel)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDEntraPontoGreide message handlers

void CDEntraPontoGreide::OnOK() 
{
  UpdateData(TRUE);

  //--- Passa os valores

  CDialog::OnOK();
}

BOOL CDEntraPontoGreide::OnInitDialog() 
{
  CDialog::OnInitDialog();

  SetWindowText(Tipo == TERRENO ? " Projeto" : " Terreno");

  CRect RectPai;

  JanPai->GetWindowRect(RectPai);

  Posx += RectPai.left;
  Posy += RectPai.top;

  SetWindowPos(&wndTop,Posx,Posy,0,0,SWP_NOSIZE);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDEntraPontoGreide::OnCancel() 
{
  CDialog::EndDialog(TRUE);
}

