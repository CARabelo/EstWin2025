// DSelGreide.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include "DSelGreide.h"
#include "afxtempl.h"
#include "carqterreno.h"
#include "math.h"
#include "Estaca.h"
#include "cponto.h" 
#include "clpontos.h" 
#include "perfil.h"
#include "afxtempl.h"
#include "supercstring.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtalcorte.h"
#include "carqtalaterro.h"
#include "ctalcorte.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include <list>
#include "csecao.h"
#include "clistaperfis.h"
#include "clistasecoes.h"
#include "xygreide.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "carqcurverticais.h"
#include "ccurverticais.h"
#include "xymouse.h"
#include "carqgradientes.h"
#include <set>
#include <string>
#include "cgradientes.h"
#include "mytoolbar.h"
#include "dselsecao.h"
#include "listamedicoes.h"
#include "dvolumes.h"
#include "calcnota.h"
#include "meleditbox.h"
#include "cdpopuppontos.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
#include "carqpistasfixas.h"
#include "cpistasfixas.h"
#include "carqcurhorizontais.h"
#include <map>
#include "ccurhorizontal.h"
#include "Mat.h"
#include "ccurhorizontais.h"
#include "CTipoSolos.h"
#include "CArqTiposSolos.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include "CArqFurosSondagem.h"
#include <algorithm>
#include "CPerfilGeologico.h"
#include "carqsecoesavulsas.h"
#include "csecaoavulsa.h"
#include "csecoesavulsas.h"
#include "CDadosBacias.h"
#include "CDeltaSuperficie.h"
#include "SetxPontos.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "dessecaoview.h"
#include "dentrapontogreide.h"
#include "comboprocdessecao.h"
#include "CArqOAC.h"
#include "COAC.h"
#include "CArqOAE.h"
#include "COAE.h"
#include "CMemDC.h"
#include "CDEnsaiarGreide.h"
#include "DesPerfilView.h"
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "meleditbox.h"
#include "dcoresgreide.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDSelGreide dialog

CDSelGreide::CDSelGreide(CWnd* pParent,CToolBar* DesToolGre,CRect& Rect,COLORREF* PpCores,int* PpDados)	: CDialog(CDSelGreide::IDD, pParent),mpCDesGreide(DesToolGre),pSecao(Rect),pCores(PpCores),pDados(PpDados)
{
  //{{AFX_DATA_INIT(CDSelGreide)
  m_CBDesenharProjeto = FALSE;
  m_CBDesenharTerreno = FALSE;
  m_CBDesenharMalha = FALSE;
  m_CBCotar = FALSE;
  m_iEquidistancia = 0;
  m_iEquidistCotas = 0;
  m_iEquidistanciaY = 0;
  m_CBMostrarDesc = FALSE;
  m_CBGreide = FALSE;
  m_bAtualizarSecoes = FALSE;
  m_bDesenharPerfGeol = FALSE;
  //}}AFX_DATA_INIT
}
void CDSelGreide::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDSelGreide)
  DDX_Control(pDX, IDC_SINCRONIZAR, m_CBSincronizar);
  DDX_Control(pDX, IDC_ATUALIZARSECOES, m_CBAtualizarSecoes);
  DDX_Control(pDX, IDC_GABARITARPROJ, m_CBGabaritarProjeto);
  DDX_Check(pDX, IDC_DESPROJETO, m_CBDesenharProjeto);
  DDX_Check(pDX, IDC_DESTERRENO, m_CBDesenharTerreno);
  DDX_Check(pDX, IDC_DESMALHA, m_CBDesenharMalha);
  DDX_Check(pDX, IDC_COTAR, m_CBCotar);
  DDX_Check(pDX, IDC_CIRCULARPONTOS,m_CBCircularPontos);
  DDX_Text(pDX, IDC_EQUIDISTANCIA, m_iEquidistancia);
  DDV_MinMaxUInt(pDX, m_iEquidistancia, 2, 5000);
  DDX_Text(pDX, IDC_EQUIDISTANCIAY, m_iEquidistanciaY);
  DDV_MinMaxUInt(pDX, m_iEquidistanciaY, 1, 50);
  DDX_Text(pDX, IDC_EQUIDISTANCIACOTAS, m_iEquidistCotas);
  DDV_MinMaxUInt(pDX, m_iEquidistCotas, 1, 500);
  DDX_Check(pDX, IDC_MOSTRARDESC, m_CBMostrarDesc);
  DDX_Control(pDX, IDC_CHE_TERRENO, m_CBTerreno);
  DDX_Control(pDX, IDC_CHE_PROJETO, m_CBProjeto);
  DDX_Check(pDX, IDC_ATUALIZARSECOES,m_bAtualizarSecoes);
  DDX_Check(pDX, IDC_DESENHARPERFGEOL,m_bDesenharPerfGeol);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDSelGreide, CDialog)
  //{{AFX_MSG_MAP(CDSelGreide)
  ON_WM_DESTROY()
  ON_BN_CLICKED(IDC_MBOK, OnMbok)
  ON_BN_CLICKED(IDC_GABARITARPROJ, OnGabaritarproj)
  ON_BN_CLICKED(IDC_SINCRONIZAR, OnSincronizar)
  ON_BN_CLICKED(IDC_CORES, OnCores)
  ON_BN_CLICKED(IDC_CHECKGREIDE, OnCheckgreide)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDSelGreide message handlers

BOOL CDSelGreide::OnInitDialog() 
{
  CDialog::OnInitDialog();

  //-- Calcula a posicao da janela em relação ao desenho da secao e mostra o dialogo

  CMainFrame* MainFrame = (CMainFrame*)AfxGetMainWnd();
  CRect PItem,Este;

  MainFrame->mpCDesToolGreide->GetItemRect(6,&PItem);  //--- Pega a posicao do item do menu
  GetWindowRect(Este);

  PItem.left += pSecao.left - (Este.right - Este.left);
  PItem.top += pSecao.top;
  SetWindowPos(&wndTop,PItem.left,PItem.top,0,0,SWP_NOSIZE);

  m_CBGabaritarProjeto.SetCheck(1);     //--- Medição, a implementar
  m_CBDesenharProjeto = *(pDados+4);
  m_CBDesenharTerreno = *(pDados+5); 
  m_CBDesenharMalha = *(pDados+6);
  m_CBCotar = *(pDados+7);
  m_iEquidistancia =  *(pDados+8);
  m_iEquidistCotas =  *(pDados+9);
  m_CBMostrarDesc = *(pDados+10);
  m_CBGreide = *(pDados+11);
  m_CBSincronizar.SetCheck(*(pDados+12));
  m_CBTerreno.SetCheck(*(pDados+13));
  m_CBProjeto.SetCheck(*(pDados+14));
  m_iEquidistanciaY = *(pDados+15);
  m_CBCircularPontos = *(pDados+16);
  m_bAtualizarSecoes = *(pDados+17);
  m_bDesenharPerfGeol =  *(pDados+18);

  UpdateData(false);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE]
}

void CDSelGreide::OnMbok() 
{
  UpdateData(true);

  *(pDados+4) = m_CBDesenharProjeto;
  *(pDados+5) = m_CBDesenharTerreno; 
  *(pDados+6) = m_CBDesenharMalha;
  *(pDados+7) = m_CBCotar;
  *(pDados+8) = m_iEquidistancia;
  *(pDados+9) = m_iEquidistCotas;
  *(pDados+10) = m_CBMostrarDesc;
  *(pDados+11) = m_CBGreide;
  *(pDados+12) = m_CBSincronizar.GetCheck();
  *(pDados+13) = m_CBTerreno.GetCheck();
  *(pDados+14) = m_CBProjeto.GetCheck();
  *(pDados+15) = m_iEquidistanciaY;
  *(pDados+16) = m_CBCircularPontos;
  *(pDados+17) = m_bAtualizarSecoes;
  *(pDados+18) = m_bDesenharPerfGeol;

  CDialog::OnOK();	
}

void CDSelGreide::OnGabaritarproj() 
{
  ((CMainFrame*)AfxGetMainWnd())->GabaritarProjeto = m_CBGabaritarProjeto.GetCheck();

  //--- Atualiza o desenho do terreno e da seção imediatamente...

  ((CWnd*)((CMainFrame*)AfxGetMainWnd())->DesPerfil)->RedrawWindow();
  ((CWnd*)((CMainFrame*)AfxGetMainWnd())->PegaDesenhoSecoes())->RedrawWindow();
}

void CDSelGreide::OnSincronizar() 
{
  // ((CMainFrame*)AfxGetMainWnd())->Sincronizar = m_CBSincronizar.GetCheck();
}

void CDSelGreide::OnCores() 
{
  CDcoresGreide DCoresGreide(true,this,pCores);

  DCoresGreide.DoModal();
}

void CDSelGreide::OnCheckgreide() 
{
  UpdateData(true);

  ((CMainFrame*)AfxGetMainWnd())->ShowControlBar(((CMainFrame*)AfxGetMainWnd())->mpCDesToolGreide,m_CBGreide, FALSE);	
}
