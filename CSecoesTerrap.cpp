#include "stdafx.h"
#include "Estaca.h"
#include "CPonto.h"
#include <map>
#include "CSecoesTerrap.h"

CArchive& CSecaoTerrap::operator<<(CArchive& ar)
{
  PontosTerrap.clear();

  ar >> Estaca ;

  size_t QuanPontos(0);
  Ponto PAtual;

  for (ar >> QuanPontos; QuanPontos > 0; --QuanPontos)
  {
    PAtual << ar;
    PontosTerrap.emplace(PAtual);
  }

  return ar;
}

CArchive& CSecaoTerrap::operator>>(CArchive& ar)
{
  ar << Estaca << PontosTerrap.size();

  for (ittysetPonto it = PontosTerrap.begin(); it != PontosTerrap.end(); ++it)
  {
    *it >> ar;
  }

  return ar;
}

