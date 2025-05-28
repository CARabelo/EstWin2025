#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Projection_traits_xy_3.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Constrained_triangulation_plus_2.h>
#include <CGAL/Polyline_simplification_2/simplify.h>
#include <CGAL/Polyline_simplification_2/Squared_distance_cost.h>
#include <CGAL/Polyline_simplification_2/Scaled_squared_distance_cost.h>
#include <CGAL/Constrained_triangulation_2.h>

namespace PS = CGAL::Polyline_simplification_2;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Epic;
typedef CGAL::Projection_traits_xy_3<Epic>  K;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef PS::Vertex_base_2<K> Vb;
typedef CGAL::Constrained_triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> TDS;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, CGAL::Exact_predicates_tag> CDT;
typedef CGAL::Constrained_triangulation_plus_2<CDT> CT;
typedef CT::Point Point;
typedef CT::Constraint_iterator Constraint_iterator;
typedef CT::Vertices_in_constraint_iterator Vertices_in_constraint_iterator;
typedef CT::Points_in_constraint_iterator Points_in_constraint_iterator;
typedef PS::Stop_below_count_ratio_threshold Stop;
typedef PS::Squared_distance_cost CostSquare;
typedef PS::Hybrid_squared_distance_cost<double> CostHybrid;
typedef CGAL::Polygon_2<K> Polygon_2_2;

#define _AFXDLL 
#include "..\stdafx.h"
#include <set>
#include "..\ConvUTMGDC\ConvUTMGDC\Utm_Coord_3d.h"
#include "afxtempl.h"
#include "..\CPonto.h"

#define INFINITO   1.7976931348623158e+308

int TesteCGAL();
int SimplifyPL(tylstlstUtm_Coord_3d& pTerreno,int Tipo=0);
int SimplifyXYZ(lstlstPontos& CNs);
int SimplifyXY(lstPontos& LstPontos);

int SimplifyPL(tylstlstUtm_Coord_3d& pTerreno,int Tipo)
{
  if(Tipo == 1)
  {  
     for (ittylstlstUtm_Coord_3d itLstCN = pTerreno.begin(); itLstCN != pTerreno.end(); ++itLstCN)
     {
       ittylstUtm_Coord_3d itPontoUTMAnterior(itLstCN->begin()), itPontoUTMAtual(itPontoUTMAnterior);

       for (++itPontoUTMAtual ; itLstCN->size() > 3 && itPontoUTMAtual != itLstCN->end(); )   //--- Tamanho minimo 3 e o primeiro e o último ponto são obrigatórios
       {
         if (!(*itPontoUTMAtual == *(itLstCN->rbegin())) &&
             (hypot(itPontoUTMAtual->x - itPontoUTMAnterior->x, itPontoUTMAtual->y - itPontoUTMAnterior->y) < 5.0))
         {
           itPontoUTMAtual = itLstCN->erase(itPontoUTMAtual);
         }
         else
         {
           ++itPontoUTMAtual;
           ++itPontoUTMAnterior;
         }
       }
     }
  }

 if(Tipo == 1)   return 0;

  tylstlstUtm_Coord_3d LstCurvasNaoSiplificadas;
  CT ct;
  int i(0);
  tysetUtm_Coord_3d SetUTM_Coord_3d;   //--- Para recuperar a baliza original

  for (ittylstlstUtm_Coord_3d itLstCN = pTerreno.begin(); itLstCN != pTerreno.end(); ++itLstCN)
  {
    i++;
 //   if ((i % 3) == 0)
    {
      Polygon_2_2 P;
	  
      for (ittylstUtm_Coord_3d itPontoUTM = itLstCN->begin(); itPontoUTM != itLstCN->end(); ++itPontoUTM)
      {
        SetUTM_Coord_3d.emplace(*itPontoUTM);

        P.push_back(Point(itPontoUTM->x, itPontoUTM->y, itPontoUTM->z));
      }

      try
      {
        ct.insert_constraint(P.vertices_begin(), P.vertices_end(), *itLstCN->begin() == *itLstCN->rbegin());
      }
      catch (...)    //--- Pula as CN  q estão dando erro mas mesmo assim ocorre erro no simplify
      {
      }
    }
  }
  PS::simplify(ct, CostSquare(), Stop(0.5),true);   //--- @Ajustar o custo, nos arquivos DGN as CN podem estar muito proximas

  pTerreno.clear();

  for (Constraint_iterator cit = ct.constraints_begin() ; cit != ct.constraints_end() ; ++cit) 
  {
    tylstUtm_Coord_3d CNSimplified;

    for (Points_in_constraint_iterator vit = ct.points_in_constraint_begin(*cit) ; vit != ct.points_in_constraint_end(*cit) ; ++vit)
    {
      int Balizas(0);

      auto PAtual(Utm_Coord_3d(vit->x(), vit->y(), vit->z(), 0, false, 0, "", 0, 0.0, 0.0));

      auto ItPontoOriginal(SetUTM_Coord_3d.find(PAtual));   //--- Recupera as balizas

      if (ItPontoOriginal != SetUTM_Coord_3d.end())
      {
        Balizas = ItPontoOriginal->Balizas;
      }

      CNSimplified.push_back(Utm_Coord_3d(vit->x(), vit->y(), vit->z(), 0, false, 0, "",Balizas,0.0,0.0));
    }

    pTerreno.push_back(CNSimplified);
  }
  return 0;
}
int TesteCGAL()
{
  std::ifstream ArqTrian("C:\\Users\\carlo\\Documents\\CampoGolf.pon", std::fstream::in);
  if (!ArqTrian.good())
  {
    AfxMessageBox("Error reading file");
    exit(0);
  }
  int C(0);
  size_t QtdPoints(0);
  char Separator;
  int i(0), Closed(0);
  CT ct;
  while (ArqTrian.good())
  {
    ArqTrian >> i >> Separator >> Closed >> Separator;
    Polygon_2_2 P;
    double x, y, z;
    for (ArqTrian >> QtdPoints; QtdPoints; --QtdPoints)
    {
      ArqTrian >> x >> Separator >> y >> Separator >> z;
      P.push_back(Point(x, y, z));
    }
    try
    {
      ct.insert_constraint(P.vertices_begin(), P.vertices_end(), (bool)Closed);
    }
    catch (...)
    {
    }
  }
  return 0;
}
int SimplifyXYZ(lstlstPontos& CNs)
{
  //--- Remove os pontos com distância < 5.0m

  if (0) for (itlstlstPontos itCN = CNs.begin(); itCN != CNs.end(); ++itCN)
  {
    itlstPontos itPontoCNAnterior(itCN->begin()), itPontoCNAtual(itPontoCNAnterior);

    for (++itPontoCNAtual; itCN->size() > 3 && itPontoCNAtual != itCN->end(); )   //--- Tamanho minimo 3 e o primeiro e o último ponto são obrigatórios
    {
      if (!(*itPontoCNAtual == *(itCN->rbegin())) &&
        (hypot(itPontoCNAtual->x - itPontoCNAnterior->x, itPontoCNAtual->y - itPontoCNAnterior->y) < 5.0))
      {
        itPontoCNAtual = itCN->erase(itPontoCNAtual);
      }
      else
      {
        ++itPontoCNAtual;
        ++itPontoCNAnterior;
      }
    }
  }

  if (0)   return 0;
  //--- Esta dando erro para superficies muito grandes( 450 mil pontos, campo golf)
  //--- Parece que pontos muito pertos dão erro mas a rotina acima resolveu
  //--- So a rotina acima reduziu 30% dos pontos da superficie de drones (campo golf)
  //--- Atendeu.
  //--- VEJA ERRO ABAIXO 26-12-2020
  
  tylstlstUtm_Coord_3d LstCurvasNaoSiplificadas;

  CT ct;

  int i(0);

  for (itlstlstPontos itCN = CNs.begin(); itCN != CNs.end(); ++itCN)
  {
    Polygon_2_2 P;

    for (itlstPontos itPontoCN = itCN->begin(); itPontoCN != itCN->end(); ++itPontoCN)
    {
      P.push_back(Point(itPontoCN->x, itPontoCN->y, itPontoCN->z));
    }

    try
    {
      ct.insert_constraint(P.vertices_begin(), P.vertices_end(), *itCN->begin() == *itCN->rbegin());
    }
    catch (...)    //--- Pula as CN  q estão dando erro mas mesmo assim ocorre erro no simplify
    {
    }
  }

  PS::simplify(ct, CostSquare(), Stop(0.5), true);

  CNs.clear();

  for (Constraint_iterator cit = ct.constraints_begin(); cit != ct.constraints_end(); ++cit)
  {
    CNs.emplace_back(lstPontos());
    lstPontos& itCNsimplificada(*CNs.rbegin());

    for (Points_in_constraint_iterator vit = ct.points_in_constraint_begin(*cit); vit != ct.points_in_constraint_end(*cit); ++vit)
    {
      itCNsimplificada.emplace_back(vit->x(), vit->y(), vit->z());
    }
  }

  return 0;
}

int SimplifyXY(lstPontos& LstPontos)
{
  CT ct;

  if(LstPontos.size() > 2)
  {
    Polygon_2_2 P;

    for (itlstPontos itPontoCN = LstPontos.begin(); itPontoCN != LstPontos.end(); ++itPontoCN)
    {
      P.push_back(Point(itPontoCN->x, itPontoCN->y, itPontoCN->z));
    }

    try
    {
      ct.insert_constraint(P.vertices_begin(), P.vertices_end(), *LstPontos.begin() == *LstPontos.rbegin());
    }
    catch (...)    //---  Tratar erro
    {
    }
  }
  
  PS::simplify(ct, CostSquare(), Stop(0.5), true);

  LstPontos.clear();

  for (Constraint_iterator cit = ct.constraints_begin(); cit != ct.constraints_end(); ++cit)
  {
    LstPontos.clear();
    lstPontos& itCNsimplificada(LstPontos);

    for (Points_in_constraint_iterator vit = ct.points_in_constraint_begin(*cit); vit != ct.points_in_constraint_end(*cit); ++vit)
    {
      itCNsimplificada.emplace_back(vit->x(), vit->y(), vit->z());
    }
  }

  return 0;
}
