// GeraSuperficie.cpp: implementation of the CGeraSuperficie class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <string>
#include "GeraSuperficie.h"

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

extern int CalcSuperficie(const std::string& strNomeProj,CProgressCtrl* pProgresso,CStatic* pCSTexto);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGeraSuperficie::CGeraSuperficie(CString& NomeProj,CProgressCtrl* pProgress,CStatic* pCSText): 
                                strNomeProj(NomeProj.GetBuffer()),
								pProgresso(pProgress),
								pCSTexto(pCSText),
								nPontos(0){}


CGeraSuperficie::~CGeraSuperficie(){}

bool CGeraSuperficie::Gerar()
{
  CalcSuperficie(strNomeProj,pProgresso,pCSTexto);
	
  return true;
}
