#include "stdafx.h"
#include "CPonto.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include <map>
#include <sstream> 
#include <iomanip>
#include "Estaca.h"
#include "CEquipDrenagem.h"
#include "Mat.h"

extern class Matem Mat;

void CEquipDrenagem::DefineDegraus()
{
  itlstlstPontos itUltimoAlin(Alinhamentos.end());

  --itUltimoAlin;

  for (itlstlstPontos itAlin = Alinhamentos.begin() ; itAlin != itUltimoAlin ; ++itAlin)
  {
    lstPontos PontosDegrau;
    itlstPontos itPAnterior(itAlin->begin()),itPAtual(itPAnterior);

    ++itPAtual;

    double DistTotal(hypot(itPAtual->x-itPAnterior->x, itPAtual->y - itPAnterior->y)),
           DistAtual(0.0);

    while (DistAtual < DistTotal)
    {
      Ponto PDegrau1,PDegrau2;

      Mat.IrradiaPonto(itPAnterior->fi,itPAnterior->x,itPAnterior->y,0.0,DistAtual,PDegrau1);
      Mat.IrradiaPonto(itPAnterior->fi,PDegrau1.x,PDegrau1.y,Matem::M_PI_2,a,PDegrau2);

      DistAtual += b;

      lstPontos LstDegAtual;

      itlstlstPontos itDegrau(Alinhamentos.insert(Alinhamentos.end(),lstPontos()));

     itDegrau->emplace_back(PDegrau1);
     itDegrau->emplace_back(PDegrau2);
    }
  }
}

void CEquipDrenagem::DefineQuadriculado()
{
  itlstlstPontos itAlin(Alinhamentos.begin());

  double DistAtual(0.0), DistTotal(b-0.30);
  lstPontos PontosDegrau;
  itlstPontos itPAnterior(itAlin->begin());

  double Az = itPAnterior->fi + Matem::M_PI;

  ++itPAnterior;

  while (DistAtual < DistTotal)
  {
    Ponto PDegrau1, PDegrau2;

    DistAtual += 0.40;

    Mat.IrradiaPonto(Az, itPAnterior->x, itPAnterior->y, 0, DistAtual, PDegrau1);
    Mat.IrradiaPonto(Az, PDegrau1.x, PDegrau1.y, Matem::M_PI_2, a, PDegrau2);

    lstPontos LstDegAtual;

    itlstlstPontos itDegrau(Alinhamentos.insert(Alinhamentos.end(), lstPontos()));

    itDegrau->emplace_back(PDegrau1);
    itDegrau->emplace_back(PDegrau2);
  }

  DistAtual = 0.0;
  DistTotal = a - 0.30;

  itPAnterior = itAlin->end();
  --itPAnterior;
  --itPAnterior;
  Az += Matem::M_PI_2 + Matem::M_PI;

  while (DistAtual < DistTotal)
  {
    Ponto PDegrau1, PDegrau2;

    DistAtual += 0.40;

    Mat.IrradiaPonto(Az, itPAnterior->x, itPAnterior->y, 0, DistAtual, PDegrau1);
    Mat.IrradiaPonto(Az, PDegrau1.x, PDegrau1.y, Matem::M_PI_2, b, PDegrau2);

    lstPontos LstDegAtual;

    itlstlstPontos itDegrau(Alinhamentos.insert(Alinhamentos.end(), lstPontos()));

    itDegrau->emplace_back(PDegrau1);
    itDegrau->emplace_back(PDegrau2);
  }
}

void CEquipDrenagem::RotacionaPlano(double VUx, double VUy, double Angulo)
{
  Rotaciona(VUx,VUy,Angulo);

  for (auto& ItLLPontos : PegaAlinhamentos())
  {
    for (itlstPontos ItPonto = ItLLPontos.begin(); ItPonto != ItLLPontos.end(); ++ItPonto)
    {
      if(ItPonto->x != INFINITO && ItPonto->y != INFINITO) ItPonto->RotacionaPlano(VUx, VUy,Angulo);
    }
  }

  //--- Rotaciona a estaca

  if (CoordEixo().x != INFINITO && CoordEixo().y != INFINITO) CoordEixo().RotacionaPlano(VUx,VUy,Angulo);
}