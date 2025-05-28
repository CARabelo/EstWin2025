#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include <vector>
#include <iostream>
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include <map>
#include "CPonto.h"
#include "ccurHorizontal.h"
#include "carqcurhorizontais.h" 

CurHorizontal::CurHorizontal(const CurHorizontal& CurHor) 
{
  *this = CurHor;
}

CurHorizontal& CurHorizontal::operator=(const CurHorizontal &No)
{
  TS = No.TS;                
  SC = No.SC;
  CS = No.CS;
  ST = No.ST;
  PI = No.PI;
  CC = No.CC;
  EstTs = No.EstTs;
  EstSc	= No.EstSc;
  EstCs	= No.EstCs;
  EstSt	= No.EstSt;
  EstPc	= No.EstPc;
  EstPt	= No.EstPt;
  Ac = No.Ac; 
  Raio = No.Raio; 
  xEsp = No.xEsp;
  yEsp = No.yEsp;
  AcCirc = No.AcCirc;
  Fi = No.Fi;
  Lc = No.Lc;
  p = No.p;
  q = No.q,
  Ts = No.Ts;
  Sl = No.Sl;
  Se = No.Se;
  m1 = No.m1;
  m2 = No.m2;
  Ini = No.Ini;
  Fim  = No.Fim;  
  K = No.K;
  Lado = No.Lado;
  Tipo = No.Tipo;
  Caso = No.Caso;

  mapSecoesDesenho = No.mapSecoesDesenho;

  return *this;
}

CArchive& CurHorizontal::operator>>(CArchive& ar)
{
  TS >> ar;
  SC >> ar;
  CS >> ar;
  ST >> ar;
  PI >> ar;
  CC >> ar;

  EstTs.operator>>(ar);
  EstSc.operator>>(ar); 
  EstCs.operator>>(ar); 
  EstSt.operator>>(ar); 
  EstPc.operator>>(ar);
  EstPt.operator>>(ar); 

  ar << Ac << Raio << xEsp << yEsp << AcCirc << Fi << Lc << p << q << Ts << Sl << Se << m1 << m2 
    << Ini << Fim << K << Lado << Tipo << Caso; 

  return ar;
}

CArchive& CurHorizontal::operator<<(CArchive& ar)
{
  TS << ar;
  SC << ar;
  CS << ar;
  ST << ar;
  PI << ar;
  CC << ar;

  EstTs.operator<<(ar);
  EstSc.operator<<(ar); 
  EstCs.operator<<(ar); 
  EstSt.operator<<(ar); 
  EstPc.operator<<(ar); 
  EstPt.operator<<(ar); 

  ar >> Ac >> Raio >> xEsp >> yEsp >> AcCirc >> Fi >> Lc >> p >> q >> Ts >> Sl >> Se >> m1 >> m2 
    >> Ini >> Fim >> K >> Lado >> Tipo >> Caso; 

  mapSecoesDesenho.clear();     //--- Se for usado, recalcula

  return ar;
}

void CurHorizontal::RotacionaPlano(double Sin,double Cos,double Angulo)
{
  TS.RotacionaPlano(Sin,Cos,Angulo);
  SC.RotacionaPlano(Sin,Cos,Angulo);
  CS.RotacionaPlano(Sin,Cos,Angulo);
  ST.RotacionaPlano(Sin,Cos,Angulo);
  PI.RotacionaPlano(Sin,Cos,Angulo);
  CC.RotacionaPlano(Sin,Cos,Angulo);

  mapSecoesDesenho.clear();    //--- Se for usado, recalcula
}


