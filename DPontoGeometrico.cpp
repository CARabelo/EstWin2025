// CDPontoGeometrico.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "cponto.h"
#include "DPontoGeometrico.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDPontoGeometrico dialog

CDPontoGeometrico::CDPontoGeometrico(CWnd* pParent,Ponto* PPonto):CDialog(CDPontoGeometrico::IDD, pParent),PontoED(PPonto)
{
	//{{AFX_DATA_INIT(CDPontoGeometrico)
	m_DOE = 0.0;
	m_DON = 0.0;
	m_DOCota = 0.0;
	m_CSDesc = _T("");
	//}}AFX_DATA_INIT
}


void CDPontoGeometrico::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDPontoGeometrico)
	DDX_Text(pDX, IDC_E, m_DOE);
	DDX_Text(pDX, IDC_N, m_DON);
	DDX_Text(pDX, IDC_COTA, m_DOCota);
	DDX_Text(pDX, IDC_DESCRICAO, m_CSDesc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDPontoGeometrico, CDialog)
	//{{AFX_MSG_MAP(CDPontoGeometrico)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDPontoGeometrico message handlers

BOOL CDPontoGeometrico::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_DON = PontoED->x;
	m_DOE = PontoED->y;
	m_DOCota = PontoED->z;
  m_CSDesc = PontoED->Descricao;

	this->UpdateData(false);

	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDPontoGeometrico::OnDestroy() 
{
	CDialog::OnDestroy();

	UpdateData(true);

	PontoED->x = m_DON;
	PontoED->y = m_DOE;
  PontoED->z = 	m_DOCota;
	PontoED->Descricao = m_CSDesc;
}

