#include "stdafx.h"
#include "cvisada.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFINITO   1.7976931348623158e+308

/////////////////////////////////////////////////////////////////////////////

CVisada::CVisada(CString Visada) : CString(Visada) {}

BOOL CVisada::EEstaca()
{
  //--- Verifica se um determinado string é estaca.
  //--- Separa o + e verifica se os dois lados são números.

  TrimRight();
  CVisada Int = Left(Find('+'));
  CVisada Frac = Right(GetLength() - Int.GetLength() - 1);

  return (Int.ENumero() && Frac.ENumero() ? TRUE : FALSE); 
}  

BOOL CVisada::ENumero()
{
  //--- Verifica se um determinado string pode ser transformado para número.
  //--- Se todos os caracteres dele estiverem dentro de <Carac> sim  senão não.

  CString Carac = "0123456789-+. ";
  BOOL E = FALSE;

  for (int i = 0, Tam = GetLength() ; 
    i < Tam && (E = Carac.Find(GetAt(i)) != -1);
    i++);

  return E; 
}  

BOOL CVisada::EMais()
{
  //--- Verifica se uma determinada visada é uma visada mais.
  //--- Ë caso inicie com um dos caracteres de Sinal ou contenha a palavra BIS.

  CString Sinal = "!@#$%^&",Temp = *this;

  Temp.MakeUpper();
  Temp.TrimLeft();

  return (Sinal.Find(GetAt(0)) != -1) || ((Temp.Find("BIS")) != -1) ? TRUE : FALSE;
}

