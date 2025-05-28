#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "carqterreno.h" 
#include "cterreno.h"
#include "string.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class monologo monolog;
//----------------------------------------------------------------
//--- Implementação da lista

CTerreno::CTerreno(CString& NomeArq) : CArqTerreno(NomeArq){}

CTerreno::~CTerreno(){}

POSITION CTerreno::PosProximaSecao(POSITION &Sec)
{
  //--- Posiciona o Ponteiro "Secao"na proxima secao (no proximo
  //--- NoAtual.tipo == "EST").

  for(ArqTerreno& NoAtual = GetNext(Sec); 
    Sec && strcmp(NoAtual.Tipo,"EST") ; 
    NoAtual = GetNext(Sec));

  return Sec;
}

