// xyMouse.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "xyMouse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// xyMouse dialog


xyMouse::xyMouse(CWnd* pParent)	: CDialog(xyMouse::IDD, pParent),m_DOCoorX(0.0),m_DOCoorY(0.0)
{
  //{{AFX_DATA_INIT(xyMouse)
  m_EDCoorX = _T("");
  m_EDCoorY = _T("");
  m_STSecao = _T("");
  //}}AFX_DATA_INIT
}

void xyMouse::DoDataExchange(CDataExchange* pDX)
{
  //--- formata os números para exibir.

  m_EDCoorX.Format("%8.2f",m_DOCoorX);
  m_EDCoorY.Format("%8.3f",m_DOCoorY);

  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(xyMouse)
  DDX_Control(pDX, IDC_RATERRENO, m_RATerreno);
  DDX_Control(pDX, IDC_RAPROJETO, m_RAProjeto);
  DDX_Control(pDX, IDC_RAMEDICAO, m_RAMedicao);
  DDX_Text(pDX, IDC_COORX, m_EDCoorX);
  DDX_Text(pDX, IDC_COORY, m_EDCoorY);
  DDX_Text(pDX, IDC_SECAO, m_STSecao);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(xyMouse, CDialog)
  //{{AFX_MSG_MAP(xyMouse)
  ON_BN_CLICKED(IDC_RATERRENO, OnRaterreno)
  ON_BN_CLICKED(IDC_RAPROJETO, OnRaprojeto)
  ON_BN_CLICKED(IDC_RAMEDICAO, OnRamedicao)

  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// xyMouse message handlers

void xyMouse::OnRaterreno()
{
  m_RAProjeto.SetCheck(FALSE);
  m_RAMedicao.SetCheck(FALSE);
}

void xyMouse::OnRaprojeto()
{
  m_RATerreno.SetCheck(FALSE);
  m_RAMedicao.SetCheck(FALSE);
}

void xyMouse::OnRamedicao()
{
  m_RATerreno.SetCheck(FALSE);
  m_RAProjeto.SetCheck(FALSE);
}

void xyMouse::OnOK(){}    //--- Anula o OnOK
