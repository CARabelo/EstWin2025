// DVolumes.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "cponto.h"
#include "clpontos.h"
#include "math.h"
#include "estaca.h"
#include "perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "supercstring.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include "carqTaludesPavim.h" 
#include "TaludesPavim.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include <list>
#include "csecao.h"
#include <string>
#include "DVolumes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDVolumes dialog

CDVolumes::CDVolumes(CWnd* pParent,int pAmbiente,const std::string& NomeProj): CDialog(CDVolumes::IDD, pParent),Ambiente(pAmbiente),NomeProjeto(NomeProj)
{
  //{{AFX_DATA_INIT(CDVolumes)
  m_StrVolTotCorte = _T("");
  m_StrVolTotAterro = _T("");
  m_StrVolTotEmpola = _T("");
  m_StrAterro = _T("");
  m_StrCorte = _T("");
  m_strAcumAterro = _T("");
  m_strAcumCorte = _T("");
  m_strMassaAterro = _T("");
  m_strMassaCorte = _T("");
  m_strAterroEmpol = _T("");
  m_strMassaAterroEmpol = _T("");
  m_strAcumulAterroEmpol = _T("");
 
  //}}AFX_DATA_INIT
}

void CDVolumes::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDVolumes)
  DDX_Text(pDX, IDC_VOLTOTCORTE, m_StrVolTotCorte);
  DDX_Text(pDX, IDC_VOTOTATERRO, m_StrVolTotAterro);
  DDX_Text(pDX, IDC_VOTOTATERROEMP,m_StrVolTotEmpola);
  DDX_Text(pDX, IDC_VSECATERRO, m_StrAterro);
  DDX_Text(pDX, IDC_VSECCORTE, m_StrCorte);
  DDX_Text(pDX, IDC_VOACUMATERRO, m_strAcumAterro);
  DDX_Text(pDX, IDC_VOLACUMCORTE, m_strAcumCorte);
  DDX_Text(pDX, IDC_VOLMASSATERRO, m_strMassaAterro);
  DDX_Text(pDX, IDC_VOLMASSCORTE, m_strMassaCorte);
  DDX_Text(pDX, IDC_VSECATERROEMP, m_strAterroEmpol);
  DDX_Text(pDX, IDC_VOLMASSATERROEMP,m_strMassaAterroEmpol);
  DDX_Text(pDX, IDC_VOACUMATERROEMP,m_strAcumulAterroEmpol);
  DDX_Text(pDX, IDC_COMPENSACAOLATERAL,m_strCompensacaoLateral);
  
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDVolumes, CDialog)
  //{{AFX_MSG_MAP(CDVolumes)
  ON_WM_CLOSE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDVolumes message handlers

void CDVolumes::Atualiza(const CSecao &Secao,bool SecAvulsa)
{
  if (!SecAvulsa)
  {
    m_strAcumCorte.Format("%9.0f", Secao.Volume[CSecao::ACUMULADOCORTE]);
    m_strAcumAterro.Format("%9.0f", Secao.Volume[CSecao::ACUMULADOATERRO]);
    m_StrAterro.Format("%6.0f", Secao.Volume[CSecao::ATERRO]);
    m_StrCorte.Format("%6.0f", Secao.Volume[CSecao::CORTE]);
    m_strMassaCorte.Format("%9.0f", Secao.Volume[CSecao::MASSAATUALCORTE]);
    m_strMassaAterro.Format("%9.0f", Secao.Volume[CSecao::MASSAATUALATERRO]);
    m_strAterroEmpol.Format("%9.0f", Secao.Volume[CSecao::EMPOLADOATERRO]);
    m_strMassaAterroEmpol.Format("%9.0f", Secao.Volume[CSecao::MASSAEMPOLATERRO]);
    m_strAcumulAterroEmpol.Format("%9.0f", Secao.Volume[CSecao::ACUMEMPOLATERRO]);
    m_strCompensacaoLateral.Format("%6.0f", Secao.CompLateral);

    SetWindowText("Volumes (m3): " + Secao.Estaca);
  }
  else
  {
    m_strAcumCorte = m_strAcumAterro = m_StrAterro = m_StrCorte = m_strMassaCorte =
    m_strMassaAterro = m_strAterroEmpol = m_strMassaAterroEmpol = m_strAcumulAterroEmpol = "";

    SetWindowText("Secao Avulsa : " + Secao.Terreno.Estaca.Descricao);
  }

  UpdateData(FALSE);
}

BOOL CDVolumes::Create(CWnd* pWnd)    //--- modeless.
{
  return CDialog::Create(IDD,pWnd);
}

void CDVolumes::OnClose() 
{
  WINDOWPLACEMENT wp;

  //--- Lê e seta posição da janela

  if (GetWindowPlacement(&wp))	WriteWindowPlacement(&wp);

  CDialog::OnClose();
}

BOOL CDVolumes::OnInitDialog() 
{
  CDialog::OnInitDialog();

  WINDOWPLACEMENT wp;

  //--- Lê e seta posição da janela e tamanho da janela

  if (ReadWindowPlacement(&wp))	SetWindowPlacement(&wp);

  // TODO: Add extra initialization here

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDVolumes::AtualizaVolumeTotal(double VolTotCorte,double VolTotAterro,double VolTotEmpola)
{
  m_StrVolTotCorte.Format("%9.0f",VolTotCorte);
  m_StrVolTotAterro.Format("%9.0f",VolTotAterro);
  m_StrVolTotEmpola.Format("%9.0f",VolTotEmpola);
}

CDVolumes::~CDVolumes() 
{
  WINDOWPLACEMENT wp;

  //--- Lê e seta posição da janela

  if (GetWindowPlacement(&wp))	WriteWindowPlacement(&wp);
}

void CDVolumes::WriteWindowPlacement(LPWINDOWPLACEMENT pwp)
{
  CString Bloco(Ambiente == COM_DES_PONTOS ? "DiaVolumesSecaocomDesPontos" :
                Ambiente == COM_DES_PERFIL ? "DiaVolumesSecaocomDesPerfil" : "DiaVolumescomSecoes");            

  TCHAR szFormat[] = _T("%u,%u,%d,%d,%d,%d,%d,%d,%d,%d"),szBuffer[sizeof("-32767")*8 + sizeof("65535")*2];

  wsprintf(szBuffer, szFormat,pwp->flags, pwp->showCmd, pwp->ptMinPosition.x, pwp->ptMinPosition.y,pwp->ptMaxPosition.x, 
                              pwp->ptMaxPosition.y,pwp->rcNormalPosition.left, pwp->rcNormalPosition.top,
                              pwp->rcNormalPosition.right, pwp->rcNormalPosition.bottom);

::WritePrivateProfileString(Bloco,"Placement",szBuffer,(NomeProjeto + std::string(".ini").c_str()).c_str());
}

bool CDVolumes::ReadWindowPlacement(LPWINDOWPLACEMENT pwp)
{
  CString Bloco(Ambiente == COM_DES_PONTOS ? "DiaVolumesSecaocomDesPontos" :
                Ambiente == COM_DES_PERFIL ? "DiaVolumesSecaocomDesPerfil" : "DiaVolumescomSecoes");            

  TCHAR szFormat[] = _T("%u,%u,%d,%d,%d,%d,%d,%d,%d,%d");
  char Buffer[512]= {0};

  ::GetPrivateProfileString(Bloco,"Placement","",Buffer,511,(NomeProjeto + std::string(".ini")).c_str());
  if (strlen(Buffer) == 0) return FALSE;

  WINDOWPLACEMENT wp;
  int nRead = _stscanf(Buffer, szFormat,&wp.flags, &wp.showCmd,&wp.ptMinPosition.x, &wp.ptMinPosition.y,&wp.ptMaxPosition.x, 
                                        &wp.ptMaxPosition.y,&wp.rcNormalPosition.left, &wp.rcNormalPosition.top,
                                        &wp.rcNormalPosition.right, &wp.rcNormalPosition.bottom);

  if (nRead != 10)return FALSE;

  wp.length = sizeof wp;
  *pwp = wp;

  return TRUE;
}

