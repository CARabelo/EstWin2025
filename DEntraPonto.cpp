// DEntraPonto.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "DEntraPonto.h"
#include "CPonto.h"
#include <string>
#include "Mat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TERRENO 0

extern class Matem Mat;

/////////////////////////////////////////////////////////////////////////////
// CDEntraPonto dialog

CDEntraPonto::CDEntraPonto(double* pi1,double* pi2,double* px,double* py,CPoint& Ponto,int TipoPerfil,CWnd* pParent)
:CDialog(CDEntraPonto::IDD, pParent),m_pdoi1(pi1), m_pdoi2(pi2), m_pdox(px), m_pdoy(py),
m_doi1(*pi1), m_doi2(*pi2), m_dox(*px), m_doy(*py),Posx(Ponto.x),Posy(Ponto.y),
Tipo(TipoPerfil),JanPai(pParent)
{
  m_dox = Mat.Arredonda(m_dox,2);
  m_doy = Mat.Arredonda(m_doy,3);
  m_doi1 = Mat.Arredonda(m_doi1,7);
  m_doi2 = Mat.Arredonda(m_doi2,7);

  //{{AFX_DATA_INIT(CDEntraPonto)
  //}}AFX_DATA_INIT
}

void CDEntraPonto::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDEntraPonto)
  DDX_Text(pDX, IDC_I1, m_doi1);
  DDX_Text(pDX, IDC_I2, m_doi2);
  DDX_Text(pDX, IDC_X, m_dox);
  DDX_Text(pDX, IDC_Y, m_doy);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDEntraPonto, CDialog)
  //{{AFX_MSG_MAP(CDEntraPonto)
  ON_BN_CLICKED(IDC_CANCEL, OnCancel)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDEntraPonto message handlers

void CDEntraPonto::OnOK() 
{
  UpdateData(TRUE);

  //--- Passa os valores.

  *m_pdoi1 = m_doi1;
  *m_pdoi2 = m_doi2;
  *m_pdox = m_dox;
  *m_pdoy = m_doy;

  CDialog::OnOK();
}

BOOL CDEntraPonto::OnInitDialog() 
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

void CDEntraPonto::OnCancel() 
{
  CDialog::EndDialog(TRUE);	
}

