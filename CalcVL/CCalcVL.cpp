// CCalcVL.cpp : implementation file
//

#include "stdafx.h"
#include <list>
#include "..\resource.h"    //--- Precisa do caminho, se refere au resource do EstWin não do CCalcVL
#include "math.h"
#include "afxtempl.h"
#include "supercstring.h"
#include "estaca.h"
#include "carqIgualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include <set>
#include "cponto.h"
#include "clpontos.h"
#include "perfil.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include "csecao.h"
#include "clistaperfis.h"
#include "clistasecoes.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
#include "CTipoSolos.h"
#include "CArqTiposSolos.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include "CArqFurosSondagem.h"
#include <algorithm>
#include "CPerfilGeologico.h"
#include "CalcVolumes.h"
#include "CalcVL.h"
#include "CCalcVL.h"

// CCalcVL

IMPLEMENT_DYNAMIC(CCalcVL, CWnd)
CCalcVL::CCalcVL()
{
}

CCalcVL::~CCalcVL()
{
}

int CCalcVL::CalculaVL(CString& pNomeProj,CProjeto* pProj)
{
  CalcVolumes*  pDCalcVolumes((CalcVolumes*) new CalcVolumes(pNomeProj));
  
  pDCalcVolumes->DoModal();

  delete pDCalcVolumes;

  return 9;
}

BEGIN_MESSAGE_MAP(CCalcVL, CWnd)
END_MESSAGE_MAP()

// CCalcVL message handlers

