#include "stdafx.h"
#include <list>
#include "DistribuiCotas.h"

#define ALTURALETRA  20
#define DISTMINCOTAS 5
#define LIMESQ 30
#define LIMDIR 20000

static double DistEntreCotas = ALTURALETRA + DISTMINCOTAS;
tlstdouble LstXCotas;

enum { VU_E_LADOESQUERDO, VU_E_LADODIREITO, VU_E_ACABOUESPACO };

int DirecaoAtual = 0;

bool VerfDistrCotas(double XCotaAtual)
{
  if (LstXCotas.size() == 10) return true;

  if (LstXCotas.size() == 0)
  {
    LstXCotas.push_back(XCotaAtual);
    return true;
  }

  if (DirecaoAtual == VU_E_ACABOUESPACO) return false;  //--- Sem Espaço

  if (XCotaAtual < (*LstXCotas.rbegin() + ALTURALETRA + DISTMINCOTAS))
  {
    DistribuiCotas(XCotaAtual);
  }
  
  LstXCotas.push_back(XCotaAtual);

  return false;
}

void DistribuiCotas(double& XCota)
{
  double difDist(XCota - *LstXCotas.rbegin()+DISTMINCOTAS);
  bool Acabou(false);

  //--- Se Acabou espaço lado esquerdo, passa a direção pro lado direito

  if (DirecaoAtual == VU_E_LADOESQUERDO && *LstXCotas.rbegin() < (LIMESQ + ALTURALETRA + DISTMINCOTAS)) DirecaoAtual = VU_E_LADODIREITO; 

  if (DirecaoAtual == VU_E_LADOESQUERDO)
  {
    for (auto it = LstXCotas.rbegin(); it != LstXCotas.rend() && !Acabou;)
    {
      *it -= difDist;
      auto it2(it);
      it++;
      if (it == LstXCotas.rend() || (*it + ALTURALETRA + DISTMINCOTAS) < *it2) Acabou = true;  
    }
  }
  else
  {
    int Novox(((int)*LstXCotas.rbegin() + ALTURALETRA + DISTMINCOTAS));

    if (Novox < LIMDIR)  XCota = Novox;
    else DirecaoAtual = VU_E_ACABOUESPACO;  //---Acabou espaço lado direito
  }
}
