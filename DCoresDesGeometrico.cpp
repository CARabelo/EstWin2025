// DCoresDesGeometrico.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "meleditbox.h"
#include "DCoresDesGeometrico.h"
#include "afxdlgs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCoresDesGeometrico dialog

CDCoresDesGeometrico::CDCoresDesGeometrico(CWnd* pParent,COLORREF* pCores,bool bGeom) : CDialog(CDCoresDesGeometrico::IDD, pParent),pCoresRef(pCores),
m_EdCorFundo(*pCores),m_EdCorPontos(*(pCores+1)),m_EdCorMalha(*(pCores+2)), m_EdCorCristasPes(*(pCores+3)),
m_EdCorBordos(*(pCores+4)),m_EdCorArestas(*(pCores+5)),m_EdCorSecoes(*(pCores+6)), m_EdCorTexto(*(pCores+7)),m_EdCorProjeto(*(pCores+8)),m_EdCorCN(*(pCores+9)),
bGeometrico(bGeom)
{
  //{{AFX_DATA_INIT(CDCoresDesGeometrico)
  //}}AFX_DATA_INIT
}

void CDCoresDesGeometrico::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDCoresDesGeometrico)
  DDX_Control(pDX, IDC_CORFUNDO, m_EdCorFundo);
  DDX_Control(pDX, IDC_CORPONTOS, m_EdCorPontos);
  DDX_Control(pDX, IDC_CORMALHA, m_EdCorMalha);
  DDX_Control(pDX, IDC_CORCRISTASPES, m_EdCorCristasPes);
  DDX_Control(pDX, IDC_CORBORDOS, m_EdCorBordos);
  DDX_Control(pDX, IDC_CORARESTAS, m_EdCorArestas);
  DDX_Control(pDX, IDC_CORSECOES, m_EdCorSecoes);
  DDX_Control(pDX, IDC_CORTEXTO, m_EdCorTexto);
  DDX_Control(pDX, IDC_CORPROJETO, m_EdCorProjeto);
  DDX_Control(pDX, IDC_CORCN, m_EdCorCN);

  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDCoresDesGeometrico, CDialog)
  //{{AFX_MSG_MAP(CDCoresDesGeometrico)
  ON_BN_CLICKED(IDC_RADIOFUNDO, OnRadioFundo)
  ON_BN_CLICKED(IDC_RADIOPONTOS, OnRadioPontos)
  ON_BN_CLICKED(IDC_RADIOARESTAS, OnRadioArestas)
  ON_BN_CLICKED(IDC_RADIOBORDOS, OnRadioBordos)
  ON_BN_CLICKED(IDC_RADIOCRISTASPES, OnRadioCristasPes)
  ON_BN_CLICKED(IDC_RADIOMALHA, OnRadioMalha)
  ON_BN_CLICKED(IDC_RADIOSECOES, OnRadioSecoes)
  ON_BN_CLICKED(IDC_RADIOTEXTO, OnRadioTexto)
  ON_BN_CLICKED(IDC_RADIOPROJETO, OnRadioProjeto)
  ON_BN_CLICKED(IDC_RADIOCN, OnRadioCN)
  ON_WM_MOVE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCoresDesGeometrico message handlers

void CDCoresDesGeometrico::MudaCor(CMelEditBox* EditBox) 
{
  CColorDialog DCor;
  
  if (DCor.DoModal() == IDOK)
  { 
    EditBox->MudaCorFundo(DCor.GetColor());
    CDC* pDC = EditBox->GetDC();

    if(!pDC) return;     //--- Segurança, aconteçeu nos testes
    
    CRect rc;                   
    EditBox->GetClientRect(&rc); 
    pDC->FillSolidRect( rc, DCor.GetColor());		//--- Pìnta o fundo

    EditBox->ReleaseDC(pDC);
  }
}

void CDCoresDesGeometrico::OnOK() 
{
  pCoresRef[0] = m_EdCorFundo.PegaCorFundo();
  pCoresRef[1] = m_EdCorPontos.PegaCorFundo();
  pCoresRef[2] = m_EdCorMalha.PegaCorFundo();
  pCoresRef[3] = m_EdCorCristasPes.PegaCorFundo();
  pCoresRef[4] = m_EdCorBordos.PegaCorFundo();
  pCoresRef[5] = m_EdCorArestas.PegaCorFundo();
  pCoresRef[6] = m_EdCorSecoes.PegaCorFundo();
  pCoresRef[7] = m_EdCorTexto.PegaCorFundo();
  pCoresRef[8] = m_EdCorProjeto.PegaCorFundo();
  pCoresRef[9] = m_EdCorCN.PegaCorFundo();

  CDialog::OnOK();
}

void CDCoresDesGeometrico::OnRadioFundo()
{
  MudaCor(&m_EdCorFundo);
}

void CDCoresDesGeometrico::OnRadioPontos()
{
  MudaCor(&m_EdCorPontos);
}

void CDCoresDesGeometrico::OnRadioArestas()
{
  MudaCor(&m_EdCorArestas);
}

void CDCoresDesGeometrico::OnRadioBordos()
{
  MudaCor(&m_EdCorBordos);
}

void CDCoresDesGeometrico::OnRadioCristasPes()
{
  MudaCor(&m_EdCorCristasPes);
}

void CDCoresDesGeometrico::OnRadioMalha()
{
  MudaCor(&m_EdCorMalha);
}

void CDCoresDesGeometrico::OnRadioSecoes()
{
  MudaCor(&m_EdCorSecoes);
}

void CDCoresDesGeometrico::OnRadioTexto()
{
  MudaCor(&m_EdCorTexto);
}

void CDCoresDesGeometrico::OnRadioProjeto()
{
  MudaCor(&m_EdCorProjeto);
}

void CDCoresDesGeometrico::OnRadioCN()
{
  MudaCor(&m_EdCorCN);
}

/////////////////////////////////////////////////////////////////////////////
// DcoresDesGeometrico dialog

DcoresDesGeometrico::DcoresDesGeometrico(CWnd* pParent /*=NULL*/)
: CDialog(DcoresDesGeometrico::IDD, pParent)
{
  //{{AFX_DATA_INIT(DcoresDesGeometrico)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}


void DcoresDesGeometrico::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(DcoresDesGeometrico)
  // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DcoresDesGeometrico, CDialog)
  //{{AFX_MSG_MAP(DcoresDesGeometrico)
  // NOTE: the ClassWizard will add message map macros here
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DcoresDesGeometrico message handlers


BOOL CDCoresDesGeometrico::OnInitDialog()
{
  CDialog::OnInitDialog();

  if(bGeometrico) SetWindowText("Cores: Geométrico ");

  // TODO:  Add extra initialization here

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}
