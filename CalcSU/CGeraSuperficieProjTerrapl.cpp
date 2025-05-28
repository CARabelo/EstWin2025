// GeraSuperficie.cpp: implementation of the CGeraSuperficie class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <string>
#include <list>
#include <set>
#include "CPonto.h"
#include "CGeraSuperficieProjTerrapl.h"

#define INFINITO   1.7976931348623158e+308
/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/

#define SIM 1
#define NAO 0

extern int CalcSuperficieProjTerrpl(std::string& strNomeProj, CProgressCtrl* pProgresso, CStatic* pCSTexto, int ConfigSuper, tylstcponto& LstPontosRemovidos,SSetPontos& PontosProblema,double AreaTerrap, tylstcponto& LstFronteira,const std::string& FormatoArqPontos,int GeoRef,int TipoProj);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGeraSuperficieProjTerrpl::CGeraSuperficieProjTerrpl(CString& NomeProj, CProgressCtrl* pProgress, CStatic* pCSText, int pConfigSuper) :
  strNomeProj(NomeProj.GetBuffer()), pProgresso(pProgress), pCSTexto(pCSText), ConfigSuper(pConfigSuper) {}

CGeraSuperficieProjTerrpl::~CGeraSuperficieProjTerrpl() {}

bool CGeraSuperficieProjTerrpl::Gerar(tylstcponto& LstPontosRemovidos,SSetPontos& PontosProblema,double AreaTerrap,tylstcponto& LstFronteiraTerrap,const std::string& FormatoArqPontos,int GeoRef,int TipoProj)
{
  return CalcSuperficieProjTerrpl(strNomeProj, pProgresso, pCSTexto, ConfigSuper,LstPontosRemovidos,PontosProblema,AreaTerrap,LstFronteiraTerrap,FormatoArqPontos,GeoRef,TipoProj);
  return true;
}
