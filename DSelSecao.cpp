// DSelSecao.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "DSelSecao.h"
#include <string>
#include <map>
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
// CDSelSecao dialog


CDSelSecao::CDSelSecao(CWnd *pParent,CToolBar *DesTool1,CToolBar *DesTool2,CToolBar *DesTool3,
                       CToolBar *DesTool4,CDialog* DVolumes,CRect& Jan,BOOL PSecoes,int** PDadosConfig,COLORREF* PCoresConfig): CDialog(CDSelSecao::IDD, pParent),Pai(pParent),
                       mpCDesTool1(DesTool1),mpCDesTool2(DesTool2),mpCDesTool3(DesTool3),mpCDesTool4(DesTool4),mpDVolumes(DVolumes),
                       pSecao(Jan),ESecoes(PSecoes),pDados(PDadosConfig),pCores(PCoresConfig)
{
  //{{AFX_DATA_INIT(CDSelSecao)
  m_uintEquidistancia = *pDados[5];
  m_uintEquidisMalha = *pDados[6];
  //}}AFX_DATA_INIT
}

void CDSelSecao::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDSelSecao)
  DDX_Control(pDX, IDC_VOLUMES, m_CBVolumes);
  DDX_Control(pDX, ID_VER_COORDENADAS, m_CBCoordenadas);
  DDX_Control(pDX, IDM_VER_ESCALAS, m_CBEscalas);
  DDX_Control(pDX, IDW_VER_PROCURAR, m_CBProcurar);
  DDX_Control(pDX, ID_VER_FERRAMENTAS, m_CBFerramentas);
  DDX_Control(pDX, IDC_DESPAVIMENTO, m_CBPavimento);
  DDX_Control(pDX, IDC_DESTERRAPLANAGEM, m_CBTerraplanagem);
  DDX_Control(pDX, IDC_DESTERRENO, m_CBTerreno);
  DDX_Control(pDX, IDC_DESMALHA, m_CBMalha);
  DDX_Control(pDX, IDC_COTARMALHA, m_CBCotarMalha);
  DDX_Control(pDX, IDC_CIRCULARPONTOS, m_CBCircularPontos);
  DDX_Control(pDX, IDC_PERFISGEOLOGICOS, m_CBPerfisGeologicos);
  DDX_Control(pDX, IDC_RAD_2OFFSETS,m_RAD2OffSets);
  DDX_Control(pDX, IDC_RAD_COMTERRENO,m_RADTerreno);
  DDX_Control(pDX, IDC_RAD_1OFFSET,m_RAD1OffSet);
  DDX_Control(pDX, IDC_RAD_COMPROJETERR,m_RADTerrenoeProjeto);
  DDX_Control(pDX, IDC_RAD_TODASASSECOES,m_RADMostrarTodasSecoes);
  DDX_Text(pDX, IDC_EQUIMALHA, m_uintEquidistancia);
  DDV_MinMaxUInt(pDX, m_uintEquidistancia, 1, 10000);
  DDX_Text(pDX, IDC_EQUICOTAS, m_uintEquidisMalha);
  DDV_MinMaxUInt(pDX, m_uintEquidisMalha, 1, 20);	
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDSelSecao, CDialog)
  //{{AFX_MSG_MAP(CDSelSecao)
  ON_BN_CLICKED(ID_VER_FERRAMENTAS, OnVerFerramentas)
  ON_BN_CLICKED(IDM_VER_ESCALAS, OnVerEscalas)
  ON_BN_CLICKED(IDW_VER_PROCURAR, OnVerProcurar)
  ON_BN_CLICKED(ID_VER_COORDENADAS, OnVerCoordenadas)
  ON_WM_DESTROY()
  ON_BN_CLICKED(IDC_MBOK, OnMbok)
  ON_BN_CLICKED(IDC_VOLUMES, OnVolumes)
  ON_BN_CLICKED(IDC_CORESGREIDE, OnCores)
  ON_WM_LBUTTONDOWN()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDSelSecao message handlers

INT_PTR CDSelSecao::DoModal() 
{
  // TODO: Add your specialized code here and/or call the base class

  return CDialog::DoModal();
}

void CDSelSecao::OnVerFerramentas() 
{
  CMainFrame* MainFrame = (CMainFrame*)AfxGetMainWnd();
  if (MainFrame->GreideAtivo) MainFrame->ShowControlBar(mpCDesTool4,m_CBFerramentas.GetCheck(), FALSE);
  else MainFrame->ShowControlBar(mpCDesTool4,m_CBFerramentas.GetCheck(), FALSE);
}
void CDSelSecao::OnVerEscalas() 
{
  ((CMainFrame*)AfxGetMainWnd())->ShowControlBar(mpCDesTool2,m_CBEscalas.GetCheck(), FALSE);	
}

void CDSelSecao::OnVerProcurar() 
{
  ((CMainFrame*)AfxGetMainWnd())->ShowControlBar(mpCDesTool3,m_CBProcurar.GetCheck(), FALSE);	
}

void CDSelSecao::OnVerCoordenadas() 
{
  ((CMainFrame*)AfxGetMainWnd())->ShowControlBar(mpCDesTool1,m_CBCoordenadas.GetCheck(), FALSE);	
}

void CDSelSecao::OnVolumes() 
{
  ((CMainFrame*)AfxGetMainWnd())->DVolumes->ShowWindow(m_CBVolumes.GetCheck());
}

BOOL CDSelSecao::OnInitDialog() 
{
  CDialog::OnInitDialog();

  //-- Calcula a posicao da janela em relação ao desenho da secao e mostra o diualogo

  CMainFrame* MainFrame = (CMainFrame*)AfxGetMainWnd();
  CRect PItem,Este;

  MainFrame->mpCDesTool1->GetItemRect(1,&PItem);  //--- Pega a posicao do item do menu
  GetWindowRect(Este);

  PItem.left += pSecao.left - (Este.right - Este.left);
  PItem.top += pSecao.top;
  SetWindowPos(&wndTop,PItem.left,PItem.top,0,0,SWP_NOSIZE);

  m_CBCoordenadas.SetCheck(*pDados[10]);
  m_CBVolumes.SetCheck(*pDados[11]);
  m_CBEscalas.SetCheck(*pDados[8]);
  m_CBProcurar.SetCheck(*pDados[9]);
  m_CBFerramentas.SetCheck(*pDados[7]);
  m_CBPavimento.SetCheck(*pDados[0]);
  m_CBTerraplanagem.SetCheck(*pDados[1]);
  m_CBTerreno.SetCheck(*pDados[2]);
  m_CBMalha.SetCheck(*pDados[3]);
  m_CBCotarMalha.SetCheck(*pDados[4]);
  m_CBCircularPontos.SetCheck(*pDados[5]);
  m_CBPerfisGeologicos.SetCheck(*pDados[14]);
  m_RADTerrenoeProjeto.SetCheck(*pDados[12] & 8);
  m_RAD1OffSet.SetCheck(*pDados[12] & 2);
  m_RAD2OffSets.SetCheck(*pDados[12] & 1);
  m_RADTerreno.SetCheck(*pDados[12] & 4);
  m_RADMostrarTodasSecoes.SetCheck(*pDados[12] & 16);

  UpdateData(true);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDSelSecao::OnDestroy() 
{
  CDialog::OnDestroy();
}

void CDSelSecao::OnMbok() 
{
  int MostrarCom(m_RADTerrenoeProjeto.GetCheck() ? 8 : m_RAD1OffSet.GetCheck() ? 2  :	m_RAD2OffSets.GetCheck() ? 1  : m_RADTerreno.GetCheck() ? 4 : 16);

  UpdateData(true);

  **pDados = m_CBPavimento.GetCheck();
  *(*(pDados+1)) = m_CBTerraplanagem.GetCheck();
  *(*(pDados+2)) = m_CBTerreno.GetCheck();
  *(*(pDados+3)) = m_CBMalha.GetCheck();
  *(*(pDados+4)) = m_CBCotarMalha.GetCheck();
  *(*(pDados+5)) = m_uintEquidistancia;
  *(*(pDados+6)) = m_uintEquidisMalha;
  *(*(pDados+7)) = m_CBFerramentas.GetCheck();
  *(*(pDados+8)) = m_CBEscalas.GetCheck();
  *(*(pDados+9)) = m_CBProcurar.GetCheck();
  *(*(pDados+10)) = m_CBCoordenadas.GetCheck();
  *(*(pDados+11)) = m_CBVolumes.GetCheck();
  *(*(pDados+12)) = MostrarCom;
  *(*(pDados+13)) = m_CBCircularPontos.GetCheck();
  *(*(pDados+14)) = m_CBPerfisGeologicos.GetCheck();

  CDialog::OnOK();	
}

void CDSelSecao::OnCores() 
{
  //--- As cores da seção são as mesmas do greide, aproveita o diálogo

  CDcoresGreide DCoresSecao(true,this,pCores);
  DCoresSecao.DoModal();

  RedrawWindow();
}
