// SetProjetos.cpp : implementation file
//

#include "stdafx.h"
#include "afxtempl.h"
#include "est.h"
#include "math.h"
#include "estaca.h"
#include "cponto.h"
#include "clpontos.h"
#include "perfil.h"
#include "clistaperfis.h"
#include "supercstring.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include "carqcanteiros.h" 
#include "ccanteiros.h"
#include "string.h"
#include <map>
#include "csecao.h"
#include "clistasecoes.h"

#include "SetProjetos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetProjetos

CSetProjetos::CSetProjetos()
{
}

CSetProjetos::~CSetProjetos()
{
}

BEGIN_MESSAGE_MAP(CSetProjetos,CWnd)
	//{{AFX_MSG_MAP(CSetProjetos)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSetProjetos message handlers
