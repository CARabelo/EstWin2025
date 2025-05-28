// CCalcSU.cpp : implementation file
//
#include "stdafx.h"
#include <set>
#include <list>
#include "resource.h"
#include "math.h"
#include "afxtempl.h"
#include "supercstring.h"
#include "estaca.h"
#include "carqIgualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "cponto.h"
#include "classsetpontos.h"
#include "gerasuperficie.h"
#include "CGeraSuperficieProjTerrapl.h"
#include "cdeltasuperficie.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include <map>
#include "CAcidentesTopograficos.h"
#include "CSegmento.h"
#include "CConfigProj.h"
#include "CDadosBacias.h"
#include "CLPontos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "CEquipDrenagem.h"
#include <algorithm>    // std::transform
#include "superficie.h"
#include "GeraAlphaShape.h"
//#include "CExComboBox.h"
#include <string>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "CalcSU.h"
#include <set>
#include "CCCalcSU.h"

// CalcSU

#define ERRO_CGAL 2
#define CGAL_OK  0

extern int IniCalculaCotas(const std::string& pNomeProj);
extern int Triangulo(Ponto& P1, Ponto& P2, Ponto& P3, Ponto& PCalc);

CCalcSU::CCalcSU()
{
}

CCalcSU::~CCalcSU()
{
}

int CCalcSU::CalculaSU(CString& pNomeProj,CStatic* pStatic,CProgressCtrl* pProgress,int ConfigSuper,const std::string& FormArqPontos,int TipoSuperficie)
{
  int res(0);

  try
  {
    CGeraSuperficie GeraSuperf(pNomeProj,pProgress,pStatic,ConfigSuper,FormArqPontos.c_str());
    res =  GeraSuperf.Gerar(TipoSuperficie,pProgress);
   }
  catch(std::exception& e)
  {
    res = ERRO_CGAL;
  }

  return res;
}

int CCalcSU::CalculaAS(CString& pNomeProj,CProgressCtrl* pProgress,CStatic* pStatic,unsigned long DMP,const CString& FormatoArqPontos,int Tipo)
{
  CGeraAlphaShape GeraAlphaShape(pNomeProj,pProgress,pStatic,DMP,FormatoArqPontos,Tipo); 

  return  GeraAlphaShape.Gerar(Tipo);
}

int CCalcSU::CalculaSUProjTerrpl(CString& pNomeProj, CStatic* pStatic, CProgressCtrl* pProgress, int ConfigSuper, tylstcponto& LstPontosRemovidos,SSetPontos& SetPontosProblema,double AreaTerrap,tylstcponto& LstFronteiraTerrap,const std::string& FormatoArqPontos, int ProjGeoReferenciado,int TipoProjeto)
{
  int res(0);

  try
  {
    CGeraSuperficieProjTerrpl GeraSuperfProjTerrp(pNomeProj, pProgress, pStatic, ConfigSuper);
    res = GeraSuperfProjTerrp.Gerar(LstPontosRemovidos,SetPontosProblema,AreaTerrap,LstFronteiraTerrap,FormatoArqPontos, ProjGeoReferenciado,TipoProjeto);

  }
  catch (std::exception& e) 
  { 
    std::cerr << "ERROR: exception " << e.what() << '\n'; 
    res = ERRO_CGAL;
  } 
  catch (...)
  {
    res = ERRO_CGAL;
  }
  return res;
}

int CCalcSU::IniciaCalculaCotas(const std::string& pNomeProj)
{
  int res(0);

  try
  {
    res = IniCalculaCotas(pNomeProj);
  }
  catch (...)
  {
    res = ERRO_CGAL;
  }

  return res;
}

int CCalcSU::PegaTriangulo(Ponto& P1, Ponto& P2, Ponto& P3, Ponto& PCalc)
{
  int res(0);

  try
  {
    res = Triangulo(P1,P2,P3,PCalc);
  }
  catch (...)
  {
    res = ERRO_CGAL;
  }

  return res;
}