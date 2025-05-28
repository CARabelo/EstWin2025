#include "stdafx.h"
#include <list>
#include "math.h"
#include "estaca.h"
#include "cregareataludes.h"

CRegAreaTaludes::CRegAreaTaludes(void)
{
}

CRegAreaTaludes::~CRegAreaTaludes(void)
{
}

void CRegAreaTaludes::serialize(CArchive& ar)
{
  if(ar.IsStoring())
  {
    ar << Estaca << SemiDistancia << AreaAtual <<AreaTotal  ;
  }
  else 
  {
    ar >> Estaca >> SemiDistancia >> AreaAtual >> AreaTotal;
  }
}