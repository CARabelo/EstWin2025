// CDiaAmpliarMapa.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "CDiaAmpliarMapa.h"
#include "afxdialogex.h"

// caixa de diálogo de CDiaAmpliarMapa

IMPLEMENT_DYNAMIC(CDiaAmpliarMapa, CDialogEx)

CDiaAmpliarMapa::CDiaAmpliarMapa(CWnd* pParent)	: CDialogEx(IDD_DIAAMPLIARMAPA, pParent),Delta(false)
{
}

CDiaAmpliarMapa::~CDiaAmpliarMapa()
{
}

void CDiaAmpliarMapa::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_SPIESCALA, m_spinAmpliarReduzir);
}

BEGIN_MESSAGE_MAP(CDiaAmpliarMapa, CDialogEx)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPIESCALA, &CDiaAmpliarMapa::OnDeltaposSpiescala)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDiaAmpliarMapa

void CDiaAmpliarMapa::OnDeltaposSpiescala(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown(reinterpret_cast<LPNMUPDOWN>(pNMHDR));

  GetParent()->SendMessageToDescendants(WM_AMPLIAR_MAPA,pNMUpDown->iDelta < 0,0);

  *pResult = 0;
}
