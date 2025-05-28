// DDefBorAciTopog.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
//#include <string>
#include "DDefBorAciTopog.h"
#include <list>
#include <set>
#include "cponto.h"
#include "cdeltasuperficie.h"
#include "superficie.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GRAVAR 0

/////////////////////////////////////////////////////////////////////////////
// CDDefBorAciTopog dialog


CDDefBorAciTopog::CDDefBorAciTopog(CWnd* pParent,CString& sNomeProjeto) : CDialog(CDDefBorAciTopog::IDD, pParent), strNomeProj(sNomeProjeto)
{
	//{{AFX_DATA_INIT(CDDefBorAciTopog)
	m_bBordos = FALSE;
	m_bAciTopog = FALSE;
	//}}AFX_DATA_INIT
}


void CDDefBorAciTopog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDDefBorAciTopog)
	DDX_Check(pDX, IDC_BORDOS, m_bBordos);
	DDX_Check(pDX, IDC_ACITOPOG, m_bAciTopog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDDefBorAciTopog, CDialog)
	//{{AFX_MSG_MAP(CDDefBorAciTopog)
	ON_BN_CLICKED(IDC_INICIAR, OnIniciar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDDefBorAciTopog message handlers

void CDDefBorAciTopog::OnIniciar() 
{
	{
    CSuperficie Superficie(strNomeProj);

    this->UpdateData(true);

	  if(m_bBordos) Superficie.DefineBordos();

		Superficie.Serialize(strNomeProj,GRAVAR);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CDefbordosacitopog dialog



/////////////////////////////////////////////////////////////////////////////
// CDefbordosacitopog message handlers

