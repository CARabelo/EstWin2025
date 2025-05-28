#pragma once

struct ltMSecPonto 
{ 
  bool operator()(const Ponto* p1, const Ponto* p2) const 
  { 
    double d(p1->x - p2->x);

    return d < -1e-5 || (d < 1e-5 &&  (p1->y - p2->y < -1e-5));
  }
};

typedef std::map<CEstaca,Ponto> MSecaoGeom;    //--- Secoes geometricas para acesso randomico, Ordenadas por estaca
typedef MSecaoGeom::iterator itMSecaoGeom;

typedef std::map<Ponto*,const CEstaca*,ltMSecPonto> MSecaoGeomPonto;  //--- Ordenado por ponto
typedef MSecaoGeomPonto::iterator itMSecaoGeomPonto;  

typedef std::pair<Ponto,bool> PairPontoBool;

class CCoordSecoes
{
  MSecaoGeom MapaSecoesGeom;         //--- Mapa de secoes geometricas. ordenadas por estaca
	MSecaoGeomPonto MapaSecoesPonto;   //--- Mapa secoes ordenadas por ponto, os elementos são ponteiros para o mapa de 
                                     //--- secoes ordenadas por Estaca "MapaSecoesGeom" 


public:
  CCoordSecoes();
  ~CCoordSecoes();

  RotacionaPlano(double Sen,double Cos,double AngRotacao);

};
