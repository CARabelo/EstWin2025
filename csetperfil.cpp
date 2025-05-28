//#include "stdafx.h"
//#include "afxtempl.h"
//#include "resource.h"
//#include "monologo.h"
#include "math.h"
#include "estaca.h"
//#include "supercstring.h"
#include "carqperfis.h" 
#include "cponto.h"
#include "csetpontos.h"
#include "csetperfil.h"
/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/
#define INFINITO   1.7976931348623158e+308
#define ANTERIOR   0
#define POSTERIOR  1
#define ESQUERDO   0
#define DIREITO    1
#define ESPIRAL   'E'
#define MENOR     -1
#define MAIOR      1
#define IGUAL      0
#define TRAS      -1
#define FRENTE     1
#define X          0
#define Y          1
#define NULO      -2

//extern class monologo monolog;

//----------------------------------------------------------------------

Perfil::Perfil(CEstaca& Est) : Estaca(Est) {}

Perfil::Perfil(double Est,double Cota,int Ig) : Estaca(Est,Cota,Ig) {}   //--- construtor default;

Perfil::Perfil(class Perfil &Perf)  //--- Construtor de Cópia.
{
  *this = Perf;
}

CLPontos &Perfil::operator=(CSetPontos& L)
{
        CLPontos::operator=(L);

        return *this;
}

void Perfil::Constroi(double Est,double CotaEixo)
{
  Estaca.Constroi(Est,CotaEixo);
}

void Perfil::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
          Estaca >> ar;
        else
          Estaca << ar; 

        CSetPontos::Serialize(ar);
}

