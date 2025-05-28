// GeraSuperficie.cpp: implementation of the CGeraSuperficie class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <string>
#include "GeraAlphaShape.h"

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

extern int CalcAlphaShape(const std::string& strNomeProj,CProgressCtrl* pProgresso,CStatic* pCSTexto,unsigned long DMP,const std::string& FormArqPontos,int Tipo);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGeraAlphaShape::CGeraAlphaShape(CString& pNomeProj,CProgressCtrl* pProgress,CStatic* pCSText,unsigned long pDMP,const CString& FormArqPontos,int Tipo): 
                                strNomeProj(pNomeProj.GetBuffer()),pProgresso(pProgress),pCSTexto(pCSText),DMP(pDMP),FormatoArqPontos(FormArqPontos){}

CGeraAlphaShape::~CGeraAlphaShape(){}

bool CGeraAlphaShape::Gerar(int Tipo)
{
  return CalcAlphaShape(strNomeProj,pProgresso,pCSTexto,DMP,FormatoArqPontos,Tipo) == 0;
}
