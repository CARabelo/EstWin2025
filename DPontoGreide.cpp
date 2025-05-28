// DPontoGreide.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "math.h"
#include "estaca.h"
#include "dpontogreide.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFINITO   1.7976931348623158e+308

/////////////////////////////////////////////////////////////////////////////
// CDPontoGreide dialog


CDPontoGreide::CDPontoGreide(CWnd* pParent /*=NULL*/)
	: CDialog(CDPontoGreide::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDPontoGreide)
	m_CSE = _T("");
	m_CSI1 = _T("");
	m_CSI2 = _T("");
	m_CSCota = _T("");
	m_CSEstaca = _T("");
	m_CSY1 = _T("");
	m_CSY2 = _T("");
	//}}AFX_DATA_INIT
}


void CDPontoGreide::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDPontoGreide)
	DDX_Text(pDX, IDC_STI1, m_CSI1);
	DDX_Text(pDX, IDC_STI2, m_CSI2);
	DDX_Text(pDX, IDC_STY, m_CSCota);
	DDX_Text(pDX, IDC_STX, m_CSEstaca);
	DDX_Text(pDX, IDC_STY1, m_CSY1);
	DDX_Text(pDX, IDC_STY2, m_CSY2);
	DDX_Text(pDX, IDC_STE, m_CSE);
		//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDPontoGreide, CDialog)
	//{{AFX_MSG_MAP(CDPontoGreide)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDPontoGreide message handlers
	void CDPontoGreide::Atualiza(CEstaca& Estaca, double i1,double i2,CString& Desc,double E,double Y1,double Y2)
{
	//--- Atualiza a estaca mostrada - PROJETO

	SetWindowText(Desc);

  if (i1 != INFINITO) m_CSI1.Format("%2.7f",i1);
	if (i2 != INFINITO) m_CSI2.Format("%2.7f",i2);
	if (Estaca.EstVirtual != INFINITO) m_CSEstaca.Format("%10s",Estaca);
	if (Estaca.Cota  != INFINITO) m_CSCota.Format("%9.3f",Estaca.Cota);
	if (E != INFINITO) m_CSE.Format("%7.3f",E);
	if (Y1 != INFINITO) m_CSY1.Format("%7.3f",Y1);
	if (Y2 != INFINITO) m_CSY2.Format("%7.3f",Y2);
  
	UpdateData(FALSE);

	Limpo = FALSE;    //--- Indica que o diálogo está mostrando um ponto.
}

void CDPontoGreide::LimpaDialogo()
{
	//--- Limpa o diálogo, deve ser chamada quando não houver ponto para ser mostrado
	//--- A Propriedade <Limpo> é setada para true para evitar chamadas desnecessárias,
	//--- Ela indica que o diálogo está limpo.

  m_CSI1 = m_CSI2 = m_CSEstaca = m_CSCota = m_CSE = m_CSY1 = m_CSY2 = "";
  UpdateData(FALSE);
	SetWindowText("");
	Limpo = TRUE;
}


BOOL CDPontoGreide::Create(CWnd* pWnd)    //--- modeless.
{
  return CDialog::Create(IDD,pWnd);
}
