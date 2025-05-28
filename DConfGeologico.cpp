// DSelGreide.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include "DConfGeologico.h"
#include "afxtempl.h"
#include "xymouse.h"
#include <set>
#include <list>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDConfGeologico dialog

CDConfGeologico::CDConfGeologico(CWnd* pParent,CRect& Rect,COLORREF* PpCores,int* PpDados)	: CDialog(CDConfGeologico::IDD, pParent),pCores(PpCores),pDados(PpDados)
{
  //{{AFX_DATA_INIT(CDConfGeologico)
  m_CBDesenharMalha = FALSE;
  m_CBCotar = FALSE;
  m_iEquidistancia = 0;
  m_iEquidistCotas = 0;
  m_iEquidistanciaY = 0;
  m_CBCircularPontos = 0;

//  m_CBGreide = FALSE;
  //}}AFX_DATA_INIT
}
void CDConfGeologico::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Check(pDX, IDC_CHECIRCPONTOS, m_CBDesenharMalha);
  DDX_Check(pDX, IDC_DESMALHA, m_CBDesenharMalha);
  DDX_Check(pDX, IDC_COTARMALHA, m_CBCotar);
  DDX_Check(pDX, IDC_CHECIRCPONTOS, m_CBCircularPontos);
  DDX_Text(pDX, IDC_EQUIDISTANCIA, m_iEquidistancia);
  DDV_MinMaxUInt(pDX, m_iEquidistancia, 1, 500);
  DDX_Text(pDX, IDC_EQUIDISTANCIACOTAS2, m_iEquidistCotas);
  DDV_MinMaxUInt(pDX, m_iEquidistCotas, 1, 500);
  DDX_Text(pDX, IDC_EQUIDISTANCIAY, m_iEquidistanciaY);
  DDV_MinMaxUInt(pDX, m_iEquidistanciaY, 1, 50);

  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDConfGeologico, CDialog)
  //{{AFX_MSG_MAP(CDConfGeologico)
  ON_WM_DESTROY()
  ON_BN_CLICKED(IDC_MBOK, OnMbok)
  ON_BN_CLICKED(IDC_CORES, OnCores)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDConfGeologico message handlers

BOOL CDConfGeologico::OnInitDialog() 
{
  CDialog::OnInitDialog();

  //-- Calcula a posicao da janela em relação ao desenho da secao e mostra o dialogo

  CRect PItem,Este;

  m_CBDesenharMalha = *(pDados+0);
  m_CBCotar = *(pDados+1);
  m_iEquidistancia =  *(pDados+2);
  m_iEquidistanciaY = *(pDados+3);
  m_iEquidistCotas =  *(pDados+4);
  m_CBCircularPontos = *(pDados+5);

  UpdateData(false);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE]
}

void CDConfGeologico::OnMbok() 
{
  UpdateData(true);

  *(pDados+0) = m_CBDesenharMalha;
  *(pDados+1) = m_CBCotar;
  *(pDados+2) = m_iEquidistancia;
  *(pDados+3) = m_iEquidistanciaY;
  *(pDados+4) = m_iEquidistCotas;
  *(pDados+5) = m_CBCircularPontos;

  CDialog::OnOK();	
}

void CDConfGeologico::OnCores() 
{
 // CDCoresGeologico DCoresGeologico(true,this,pCores);

 // DCoresGeologico.DoModal();
}

