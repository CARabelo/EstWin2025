#include "stdafx.h"
#include "SuperCString.h"
#include "Estaca.h"
#include "CArqFurosSondagem.h"
#include <set>
#include "CTipoSolos.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"

ittysetCTipoSoloSondagem CFuroSondagem::PegaTipoSoloMaisProximo(double Cota,double Tolerancia) const
{
  ittysetCTipoSoloSondagem it(SetSolos.begin()); 

  double CotaAtual(INFINITO);

  while(it != SetSolos.end()) 
  {
    CotaAtual = it->PegaCota();

    if(CotaAtual != INFINITO)
    {
      if(CotaAtual - Tolerancia < Cota && CotaAtual + Tolerancia  > Cota) return it;
    }

    ++it;
  }

  return SetSolos.end();
}