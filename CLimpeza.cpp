#include "stdafx.h"
#include <list>
#include "math.h"
#include "estaca.h"
#include "climpeza.h"

CLimpeza::CLimpeza(void)
{
}

CLimpeza::~CLimpeza(void)
{
}

void CLimpeza::serialize(CArchive& ar)
{
  if(ar.IsStoring())
  {
    ar << Estaca << h << SemiDistancia << AreaAtual << VolumeAtual << VolumeTotal;
  }
  else 
  {
    ar >> Estaca >> h >> SemiDistancia >> AreaAtual >> VolumeAtual >> VolumeTotal;
  }
}