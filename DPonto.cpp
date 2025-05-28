// DPonto.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "math.h"
#include "estaca.h"
#include "dponto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFINITO   1.7976931348623158e+308

/////////////////////////////////////////////////////////////////////////////
// CDPonto dialog

CDPonto::CDPonto(CWnd* pParent)	: CDialog(CDPonto::IDD, pParent),Limpo(TRUE)
{
	//{{AFX_DATA_INIT(CDPonto)
	m_STi1 = _T("");
	m_STi2 = _T("");
	m_STx = _T("");
	m_STy = _T("");
	m_STE = _T("");
	//}}AFX_DATA_INIT
}

void CDPonto::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDPonto)
	DDX_Text(pDX, IDC_STI1, m_STi1);
	DDX_Text(pDX, IDC_STI2, m_STi2);
	DDX_Text(pDX, IDC_STX, m_STx);
	DDX_Text(pDX, IDC_STY, m_STy);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDPonto, CDialog)
	//{{AFX_MSG_MAP(CDPonto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//--- Esta classe apenas mostra as coordenadas do ponto na tela
//
// CDPonto message handlers
//
//
void CDPonto::Atualiza(double x, double y, double i1,double i2,int Tipo,CString& NumMed)
{
  //--- Atualiza o ponto mostrado.

	 SetWindowText(Tipo == TERRENO ? " Terreno " : Tipo == PROJETO ? " Projeto " : " Medição " + NumMed);      

  if (i1 != INFINITO) m_STi1.Format("%2.5f",i1);
	if (i2 != INFINITO) m_STi2.Format("%2.5f",i2);
	if (x != INFINITO) m_STx.Format("%8.2f",x);
	if (y != INFINITO) m_STy.Format("%8.3f",y);
  
	UpdateData(FALSE);

	Limpo = FALSE;   //--- Indica que o diálogo está mostrando um ponto.
}

void CDPonto::Atualiza(CEstaca& Estaca, double i1,double i2,CString& Desc,double E)
{
	//--- Atualiza a estaca mostrada

	SetWindowText(Desc);

  if (i1 != INFINITO) m_STi1.Format("%2.5f",i1);
	if (i2 != INFINITO) m_STi2.Format("%2.5f",i2);
	if (Estaca.EstVirtual != INFINITO) m_STx.Format("%10s",Estaca);
	if (Estaca.Cota  != INFINITO) m_STy.Format("%9.3f",Estaca.Cota);
	if (E != INFINITO) m_STE.Format("%9.3f",E);
  
	UpdateData(FALSE);

	Limpo = FALSE;    //--- Indica que o diálogo está mostrando um ponto.
}


BOOL CDPonto::Create(CWnd* pWnd)    //--- modeless.
{
  return CDialog::Create(IDD,pWnd);
}

void CDPonto::LimpaDialogo()
{
	//--- Limpa o diálogo, deve ser chamada quando não houver ponto para ser mostrado
	//--- A Propriedade <Limpo> é setada para true para evitar chamadas desnecessárias,
	//--- Ela indica que o diálogo está limpo.

  m_STi1 = m_STi2 = m_STx = m_STy = m_STE =  "";
  UpdateData(FALSE);
	SetWindowText("");
	Limpo = TRUE;
}

