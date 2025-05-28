#define _AFXDLL 
#include "stdafx.h"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_conformer_2.h>

#include <cassert>
#include <iostream>
#include <fstream>
#include <iomanip>  
#include <strstream>
#include <list>
#include "io.h"
#include "..\CPonto.h"
#include "..\CLPontos.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Exact_predicates_tag Itag;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Constrained_Delaunay_triangulation_2<K> CDT;
typedef CDT::Point Point;
typedef CDT::Vertex_handle Vertex_handle;
typedef CDT::Vertex_circulator Vertex_circulator;
typedef CDT::Face_handle Face_handle;

extern double DINFINITO;

#include "EstWinCGAL.h"

int Erro(CGAL_OK);

tysetPonto3D setP3D;             //--- Set de backup, para salvar o z (Verificar necessidade)

int Triangulo(Ponto& P1, Ponto& P2, Ponto& P3, Ponto& PCalc);
inline void IgualaPonto(const Point& PontoCGAL, Ponto& PontoEstWin);
int IniCalculaCotas(const std::string& strNomeProj);

CDT cdt;

int IniCalculaCotas(const std::string& strNomeProj)
{
  std::ifstream ArqSuperficie(strNomeProj + ".dtr");

  if (ArqSuperficie.good() == false)
  {
    Erro = 1;
    return Erro;
  }

  ArqSuperficie >> cdt;

  return Erro;
}

int Triangulo(Ponto& P1,Ponto& P2,Ponto& P3,Ponto& PCalc)
{
  CDT::Locate_type LT(CDT::VERTEX);
  int LI(CDT::VERTEX);
  Point PontoCalc(PCalc.x, PCalc.y);

  Face_handle Face(cdt.locate(PontoCalc,LT,LI));                  

  if (Face != NULL)
  {
    if (cdt.is_infinite((Face->vertex(0))))
    {
      Vertex_handle vMirror(cdt.mirror_vertex(Face, 0));
      IgualaPonto(vMirror->point(),P1);
    }
    else IgualaPonto(Face->vertex(0)->point(),P1);

    if (cdt.is_infinite((Face->vertex(1))))
    {
      Vertex_handle vMirror(cdt.mirror_vertex(Face, 1));
      IgualaPonto(vMirror->point(),P2);
    }
    else IgualaPonto(Face->vertex(1)->point(), P2);

    if (cdt.is_infinite((Face->vertex(2))))
    {
      Vertex_handle vMirror(cdt.mirror_vertex(Face, 2));
      IgualaPonto(vMirror->point(), P3);
    }
    else IgualaPonto(Face->vertex(2)->point(), P3);

    return 0;
  }

  return 3;
}

inline void IgualaPonto(const Point& PontoCGAL,Ponto& PontoEstWin)
{
  PontoEstWin.x = PontoCGAL.x();
  PontoEstWin.y = PontoCGAL.y();
  PontoEstWin.Baliza2 = true;
}
