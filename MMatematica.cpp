// MMatematica.cpp: implementation of the MMatematica class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "est.h"
#include "math.h"
#include "cponto.h"
#include "MMatematica.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern const double M_PI;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MMatematica::MMatematica()
{

}

MMatematica::~MMatematica()
{

}

double MMatematica::Arredonda(double Num,int CasasDec) 
{
  double Fator = pow(10.0,CasasDec), Temp = Num * Fator,Temp2 = fmod(Temp,1);
  Temp -= Temp2;

  if(Temp2 > 0.50) Temp += 1.0;
  
  return Temp / Fator;
}


double MMatematica::gms_g(double AnguloGMS)
{
  double a,b,c,aux;

  aux = modf(AnguloGMS,&a);
  b = floor((aux+1e-6) * 100.0) ;
  c = (aux - b/100.0) * 10000.0;

  return(a + b / 60.0 + c / 3600.0);
}


double MMatematica::g_gms(double AnguloG)
{
  double Graus,Minutos,Segundos,Frac;

  Frac = modf(AnguloG,&Graus);
  Frac = modf(Frac * 60.0,&Minutos);
  Frac = modf(Frac * 60.0,&Segundos);

  return(Graus+Minutos/100.0+(Segundos+Frac)/10000.0);
}

double MMatematica::rad_gms(double AnguloRad)
{
  return(g_gms(AnguloRad * 180.0 / M_PI));
}

double MMatematica::gms_rad(double AnguloGMS)
{
  return(gms_g(AnguloGMS) / 180.0 * M_PI);
}

Ponto& MMatematica::P_R(const Ponto P)
{
  static Ponto Resp(0,0); 

  Resp.x = cos(P.x) * P.y;
  Resp.y = sin(P.x) * P.y;

  return(Resp);
}

Ponto& MMatematica::R_P(const Ponto P1,const Ponto P2)
{
  static Ponto Resp(0,0);
  Ponto Rasc(P2.x - P1.x,P2.y - P1.y);

  Resp.x = atan(Rasc.y / Rasc.x);
  Resp.y = hypot(Rasc.x,Rasc.y);

  return(Resp);
}
