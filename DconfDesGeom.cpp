// DConfDesGeom.cpp : implementation file
#include "stdafx.h"
#include "est.h"
#include "DConfDesGeom.h"
#include "Meleditbox.h"
#include "DCoresDesGeometrico.h"
#include <string>
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "CDConfigRadar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDConfDesGeom dialog

CDConfDesGeom::CDConfDesGeom(CWnd* pParent,int *pDadosConfig,COLORREF* pPCores,bool bGeom,int pTipoSuperf,bool ProjTerrap)
: CDialog(CDConfDesGeom::IDD, pParent),pDados(pDadosConfig),pCores(pPCores),bGeometrico(bGeom),TipoSuperf(pTipoSuperf),bProjTerrap(ProjTerrap),
    m_bCNTerraplanagem(FALSE), m_bSimbologia(FALSE), m_uintDensSimbologia(1)
{
  //{{AFX_DATA_INIT(CDConfDesGeom)
  m_bDesenharPixel = *pDados;
  m_bDesenharCirculo = *(pDados + 1);
  m_bDesenharCruz = *(pDados + 2);
  m_bDdesenharSecoes = *(pDados + 3);
  m_bDesenharArestas = *(pDados+4);
  m_bDesenharBordos = *(pDados+5);
  m_bDesenharCristasPes = *(pDados+6);
  m_bDesenharPontos = *(pDados + 7);
  m_bMostrarDesc = *(pDados+8);
  m_bDesenharMalha = *(pDados+9);
  m_uintEquidistancia = *(pDados + 10);
  m_uintEquidisMalha =*(pDados+11);
  m_bDesenharProjeto = *(pDados+12);
  m_bCircularSecoes = *(pDados+13);
  m_bGeometrico = *(pDados+14);
  m_bSuperficie = *(pDados+15);
  m_bCotarMalha = *(pDados+16);
  m_bVarrerTerreno = *(pDados+17);
  m_bVarrerProjeto = *(pDados+18);
  m_bVarrerCN =  *(pDados+19);
  m_bDesenharCN = *(pDados+20);
  m_bCotarCN = *(pDados+21); 
  m_uintSaltarCNs = *(pDados+22); 
  m_bTodosAlin = *(pDados + 27);
  m_bOffSets = *(pDados + 28);
  m_bMapa = *(pDados + 29);
  m_bOAC = *(pDados + 30);
  m_bSimbologia = *(pDados + 31);
  m_bCNTerraplanagem = *(pDados + 32);
  m_uintDensSimbologia = *(pDados + 33);
  m_bDesSecAvulsas = *(pDados + 34);
  m_bRealcarCNsMestras = *(pDados + 35);

  //}}AFX_DATA_INIT
}

void CDConfDesGeom::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDConfDesGeom)
    DDX_Check(pDX, IDC_DESARESTAS, m_bDesenharArestas);
    DDX_Check(pDX, IDC_DESBORDOS, m_bDesenharBordos);
    DDX_Check(pDX, IDC_DESCIRCULO, m_bDesenharCirculo);
    DDX_Check(pDX, IDC_DESCRISPE, m_bDesenharCristasPes);
    DDX_Check(pDX, IDC_DESCRUZ, m_bDesenharCruz);
    DDX_Check(pDX, IDC_DESPIXEL, m_bDesenharPixel);
    DDX_Check(pDX, IDC_DESPONTOS, m_bDesenharPontos);
    DDX_Check(pDX, IDC_DESSECOES, m_bDdesenharSecoes);
    DDX_Check(pDX, IDC_MOSTRARDESC, m_bMostrarDesc);
    DDX_Text(pDX, IDC_EQUIDISTANCIA, m_uintEquidistancia);
    DDV_MinMaxUInt(pDX, m_uintEquidistancia, 5, 100000);
    DDX_Check(pDX, IDC_DESENHARMALHA, m_bDesenharMalha);
    DDX_Text(pDX, IDC_EQUIDISTANCIACOTAS, m_uintEquidisMalha);
    DDV_MinMaxUInt(pDX, m_uintEquidisMalha, 1, 100);
    DDX_Check(pDX, IDC_DESENHARPROJETO, m_bDesenharProjeto);
    DDX_Check(pDX, IDC_CIRCULARSECOES, m_bCircularSecoes);
    DDX_Check(pDX, IDC_CHECKGEOMETRICO, m_bGeometrico);
    DDX_Check(pDX, IDC_CHECKSUPERFICIE, m_bSuperficie);
    DDX_Check(pDX, IDC_COTAR_MALHA, m_bCotarMalha);
    DDX_Check(pDX, IDC_CHE_VARRERTERRENO, m_bVarrerTerreno);
    DDX_Check(pDX, IDC_CHE_VARRERPROJETO, m_bVarrerProjeto);
    DDX_Check(pDX, IDC_CHE_VARRERCN, m_bVarrerCN);
    DDX_Check(pDX, IDC_CHE_DESENHARCN, m_bDesenharCN);
    DDX_Check(pDX, IDC_CHE_COTARCN, m_bCotarCN);
    DDX_Text(pDX, IDC_SALTARCNS, m_uintSaltarCNs);
    DDV_MinMaxUInt(pDX, m_uintSaltarCNs, 0, 20);
    DDX_Check(pDX, IDC_CHKTODOSALIN, m_bTodosAlin);
    DDX_Check(pDX, IDC_CHKALINOFFSETS, m_bOffSets);
    DDX_Check(pDX, IDC_CHE_DESENHARMAPA, m_bMapa);
    DDX_Check(pDX, IDC_CHK_OAC, m_bOAC);
    DDX_Check(pDX, IDC_CHKSIMBOLOGIA, m_bSimbologia);
    DDX_Check(pDX, IDC_CHKTALCN, m_bCNTerraplanagem);
    DDX_Check(pDX, IDC_DESENHARSECAVULSAS, m_bDesSecAvulsas);
    DDX_Check(pDX, IDC_REALCARCNSMESTRAS, m_bRealcarCNsMestras);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDConfDesGeom, CDialog)
  //{{AFX_MSG_MAP(CDConfDesGeom)
  ON_BN_CLICKED(IDC_CORES, OnCores)
  ON_BN_CLICKED(IDC_CHECKGEOMETRICO, OnCheckgeometrico)
  ON_BN_CLICKED(IDC_CHECKSUPERFICIE, OnChecksuperficie)
  //}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_BUTRADAR, &CDConfDesGeom::OnClickedButRadar)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDConfDesGeom message handlers

INT_PTR CDConfDesGeom::DoModal() 
{
  return CDialog::DoModal();
}

void CDConfDesGeom::OnOK() 
{
  UpdateData(true);

  *pDados = m_bDesenharPixel ;
  *(pDados + 1) = m_bDesenharCirculo; 
  *(pDados + 2) = m_bDesenharCruz;
  *(pDados + 3) = m_bDdesenharSecoes; 
  *(pDados + 4) = m_bDesenharArestas;
  *(pDados + 5) = m_bDesenharBordos;
  *(pDados + 6) = m_bDesenharCristasPes;
  *(pDados + 7) = m_bDesenharPontos;
  *(pDados + 8) = m_bMostrarDesc;
  *(pDados + 9) = m_bDesenharMalha;
  *(pDados + 10) = m_uintEquidistancia;
  *(pDados + 11) = m_uintEquidisMalha;
  *(pDados + 12) = m_bDesenharProjeto;
  *(pDados + 13) = m_bCircularSecoes;
  *(pDados + 14) = m_bGeometrico;
  *(pDados + 15) = m_bSuperficie;
  *(pDados + 16) = m_bCotarMalha;
  *(pDados + 17) = m_bVarrerTerreno;
  *(pDados + 18) = m_bVarrerProjeto;
  *(pDados + 19) = m_bVarrerCN;
  *(pDados + 20) = m_bDesenharCN;
  *(pDados + 21) = m_bCotarCN;
  *(pDados + 22) = m_uintSaltarCNs;
  *(pDados + 27) = m_bTodosAlin;
  *(pDados + 28) = m_bOffSets;
  *(pDados + 29) = m_bMapa;
  *(pDados + 30) = m_bOAC;
  *(pDados + 31) = m_bSimbologia;
  *(pDados + 32) = m_bCNTerraplanagem;
  *(pDados + 33) = m_uintDensSimbologia;
  *(pDados + 34) = m_bDesSecAvulsas;
  *(pDados + 35) = m_bRealcarCNsMestras;

  CDialog::OnOK();	
}

void CDConfDesGeom::OnCores() 
{
  CDCoresDesGeometrico DCores(this,pCores,bGeometrico);

  DCores.DoModal();
}

void CDConfDesGeom::OnCheckgeometrico() 
{
  UpdateData(true);

  ((CMainFrame*)AfxGetMainWnd())->ShowControlBar(((CMainFrame*)AfxGetMainWnd())->mpToolBarPonGeo,m_bGeometrico, FALSE);	
}

void CDConfDesGeom::OnChecksuperficie() 
{	
  UpdateData(true);

  ((CMainFrame*)AfxGetMainWnd())->ShowControlBar(((CMainFrame*)AfxGetMainWnd())->mpCDesToolGeo,m_bSuperficie, FALSE);	
}

void CDConfDesGeom::OnClickedButRadar()
{
  CDConfigRadar DConfigRadar(this,pDados+23);

  DConfigRadar.DoModal();
}

BOOL CDConfDesGeom::OnInitDialog()
{
  CDialog::OnInitDialog();

  if (TipoSuperf == 1 || TipoSuperf == 3)   //--- 1 == SUPERF_DRONES  3 == IBGE/Mapas
  {
    GetDlgItem(IDC_DESBORDOS)->EnableWindow(TipoSuperf == 3);
    GetDlgItem(IDC_DESPONTOS)->EnableWindow(TipoSuperf == 3);
    GetDlgItem(IDC_DESARESTAS)->EnableWindow(false);
  }

  if(bProjTerrap) GetDlgItem(IDC_DESARESTAS)->EnableWindow(false);

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: as páginas de propriedade OCX devem retornar FALSE
}
