// CCalcVL.cpp : implementation file
//

#include "stdafx.h"
#include <list>
#include "..\resource.h"    //--- Precisa do caminho, se refere au resource do EstWin não do CCalcVL
#include <set>
#include <algorithm>
#include "SimpPL.h"
#include "ConvUTMGDC\ConvUTMGDC\Utm_Coord_3d.h"
#include "..\CPonto.h"
#include "CSimpPL.h"
#include "afxtempl.h"

extern int SimplifyPL(tylstlstUtm_Coord_3d& pTerreno,int Tipo=0);
extern int SimplifyXYZ(lstlstPontos& CNs);
extern int SimplifyXY(lstPontos& LstPontos);

extern int TesteCGAL();
IMPLEMENT_DYNAMIC(CSimpPL, CWnd)

CSimpPL::CSimpPL()
{
}

CSimpPL::~CSimpPL()
{
}

int CSimpPL::Simplifica(tylstlstUtm_Coord_3d& pTerreno,int Tipo)
{
 SimplifyPL(pTerreno,Tipo);

 //SimplificaXYZ(pTerreno);


  return 0;
}

int CSimpPL::SimplificaXYZ(lstlstPontos& pCNs)
{
  SimplifyXYZ(pCNs);
  return 0;
}

int CSimpPL::SimplificaXY(lstPontos& LstPontos)
{
  SimplifyXY(LstPontos);
  return 0;
}

BEGIN_MESSAGE_MAP(CSimpPL, CWnd)
END_MESSAGE_MAP()

// CCalcVL message handlers

