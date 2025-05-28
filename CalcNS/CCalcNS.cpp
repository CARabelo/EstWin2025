// CCalcNS.cpp : implementation file
//
#include "stdafx.h"
#include "..\resource.h"
#include "afxtempl.h"
#include <winnetwk.h>
#include <set>
#include <map>
#include "supercstring.h"
#include "math.h"
#include "estaca.h"
#include "cponto.h"
#include "clpontos.h"
#include "carqgradientes.h"
#include "cgradientes.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include "carqcurverticais.h"
#include "carqIgualdades.h" 
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "CCalcNS.h"
#include "ccurverticais.h"
#include "carqcurhorizontais.h"
#include "perfil.h"
#include "carqcanteiros.h"
#include "CCanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "ccurhorizontal.h"
#include "..\Mat.h"
#include "ccurhorizontais.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "cestacasacalcular.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include <list>
#include "csecao.h"
#include "clistasecoes.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
#include <afxmt.h>              //--- Para CEvent
#include "carqpistasfixas.h"
#include "cpistasfixas.h"
#include "CTipoSolos.h"
#include "CArqTiposSolos.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include "CArqFurosSondagem.h"
#include <algorithm>
#include "CPerfilGeologico.h"
#include "calculanota.h"
#include "calcnota.h"
#include "CalcNS.h"
#include "mat.h"
#include "monologo.h"

// CCalcNS

CCalcNS::CCalcNS()
{
}

CCalcNS::~CCalcNS()
{
}

int CCalcNS::CalculaNS(const CString& NomeProj,CProjeto* pProj)
{
  CalcNota* pCalculaNota;

  pCalculaNota = (CalcNota*) new CalcNota(NomeProj,NULL,pProj);

  pCalculaNota->DoModal();

  delete pCalculaNota; 

  return 9;
}
// CCalcNS message handlers

