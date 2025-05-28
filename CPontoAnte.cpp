#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "CPonto.h"
#include "cpontoAnte.h"
#include <string>
#include "mat.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class Matem Mat;

//------------------------------------------------------------------------------------

PontoAnte::PontoAnte(const PontoAnte& PIgual) : x(PIgual.x),y(PIgual.y),z(PIgual.z),i(PIgual.i),fi(PIgual.fi),S(PIgual.S),Descricao(PIgual.Descricao),Baliza(PIgual.Baliza),Inserido(PIgual.Inserido),Nome(PIgual.Nome),AcidenteTopografico(PIgual.AcidenteTopografico),Baliza2(PIgual.Baliza2) {}
PontoAnte::PontoAnte(const PontoAnte* pIgual) : x(pIgual->x),y(pIgual->y),z(pIgual->z),i(pIgual->i),fi(pIgual->fi),S(pIgual->S),Descricao(pIgual->Descricao),Baliza(pIgual->Baliza),Inserido(pIgual->Inserido),Nome(pIgual->Nome),AcidenteTopografico(pIgual->AcidenteTopografico), Baliza2(pIgual->Baliza2) {}
PontoAnte::PontoAnte(double OrdX, double OrdY,double OrdZ,double pi,double ps,double pS,CString& Descr,bool pBaliza,bool pInserido,const CString& pNome,int AciTopog,int pBaliza2) : Nome(pNome),x(OrdX),y(OrdY),z(OrdZ),fi(ps),S(pS),i(pi),Descricao(Descr),Baliza(pBaliza),Inserido(pInserido),AcidenteTopografico(AciTopog),Baliza2(pBaliza2)
{
}

void PontoAnte::RotacionaPlano(double Sin,double Cos,double Angulo)
{
  //                   Matriz de rotação

  /*
  x’ = x . cos (fi)  -  y . sin(fi)

  y’ = x . sin (fi)  +  y . cos(fi)
  */

  double xTemp(x * Cos - y * Sin),yTemp(x * Sin + y * Cos);

  x = xTemp;
  y = yTemp;
  fi -= Angulo;

  if (fi > Mat.M_2_PI)  fi -= Mat.M_2_PI;
  else  if (fi < 0.0)  fi += Mat.M_2_PI;
}

PontoAnte PontoAnte::RotacionaPlano(double Sin,double Cos,double Angulo,bool Diferente) const
{
  static PontoAnte PTemp;

  //                   Matriz de rotação

  /*
  x’ = x . cos (fi)  -  y . sin(fi)

  y’ = x . sin (fi)  +  y . cos(fi)
  */

  double xTemp(x * Cos - y * Sin),yTemp(x * Sin + y * Cos),fiTemp(fi-Angulo);

  PTemp = *this;

  PTemp.x = xTemp;
  PTemp.y = yTemp;

  //return  PontoAnte(xTemp,yTemp,fiTemp > Mat.M_2_PI ? fiTemp -= Mat.M_2_PI : (fiTemp < 0.0) ? fiTemp += Mat.M_2_PI : fiTemp);

  return PTemp;
}

void PontoAnte::MontaChaves()
{
}

PontoAnte::PontoAnte(const Ponto& PIgual) : x(PIgual.x), y(PIgual.y), z(PIgual.z), i(PIgual.i), fi(PIgual.fi), S(PIgual.S), Descricao(PIgual.Descricao), Baliza(PIgual.Baliza), Inserido(PIgual.Inserido), Nome(PIgual.Nome), AcidenteTopografico(PIgual.AcidenteTopografico), Baliza2(PIgual.Baliza2) {}
PontoAnte::PontoAnte(const Ponto* pIgual) : x(pIgual->x), y(pIgual->y), z(pIgual->z), i(pIgual->i), fi(pIgual->fi), S(pIgual->S), Descricao(pIgual->Descricao), Baliza(pIgual->Baliza), Inserido(pIgual->Inserido), Nome(pIgual->Nome), AcidenteTopografico(pIgual->AcidenteTopografico), Baliza2(pIgual->Baliza2) {}
