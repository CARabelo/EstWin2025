// CDEditarSecAvulsa.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "CPonto.h"
#include "CDEditarSecAvulsa.h"
#include "afxdialogex.h"
#include "Mat.h"

extern Matem Mat;

// caixa de diálogo de CDEditarSecAvulsa

IMPLEMENT_DYNAMIC(CDEditarSecAvulsa, CDialogEx)

CDEditarSecAvulsa::CDEditarSecAvulsa(CWnd* pParent,Ponto& PInicial,Ponto& PFinal)
 : CDialogEx(IDD_DIAEDITARPONTOSSECAV, pParent),
   PIni(PInicial),
   PFim(PFinal),
   P1Norte(Mat.Arredonda(PInicial.y,3)),
   P1Este(Mat.Arredonda(PInicial.x,3)),
   P2Norte(Mat.Arredonda(PFinal.y,3)),
   P2Este(Mat.Arredonda(PFinal.x,3))
{

}

CDEditarSecAvulsa::~CDEditarSecAvulsa()
{
}

void CDEditarSecAvulsa::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIP1NORTE, P1Norte);
  DDX_Text(pDX, IDC_EDIP1ESTE, P1Este);
  DDX_Text(pDX, IDC_EDIP2NORTE, P2Norte);
  DDX_Text(pDX, IDC_EDIP2ESTE, P2Este);
}

BEGIN_MESSAGE_MAP(CDEditarSecAvulsa, CDialogEx)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDEditarSecAvulsa

void CDEditarSecAvulsa::OnOK()
{
  UpdateData(true);

  PIni.x = P1Este;
  PIni.y = P1Norte;
  PFim.x = P2Este;
  PFim.y = P2Norte;

  CDialogEx::OnOK();
}
