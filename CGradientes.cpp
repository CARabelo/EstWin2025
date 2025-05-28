#include <iostream>
#include <fstream>
#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqgradientes.h" 
#include "cponto.h"
#include "clpontos.h"
#include <set>
#include <string>
#include "cgradientes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFINITO   1.7976931348623158e+308
#define ANTERIOR   0
#define POSTERIOR  1
#define ESQUERDO   0
#define DIREITO    1
#define MENOR     -1
#define MAIOR      1
#define IGUAL      0
#define TRAS      -1
#define FRENTE     1
#define X          0
#define Y          1

extern class monologo monolog;

//----------------------------------------------------------------
//--- Implementação do nó da lista.

const Gradiente &Gradiente::operator=(const Gradiente &No)
{
  EstFinal = No.EstFinal;
  x = No.x;

  return *this;
}

//----------------------------------------------------------------------------------
//--- Implementação da lista
//--- Neste objeto são criadas duas listas na memória, uma lista com os dados do 
//--- arquivo e outra lista com os dados calculados, como este objeto é genérico,
//--- as duas listas ficam na memoria e devem ser destruídas pelo recipiente 
//--- a medida do necessário. Estas listas são dinâmicas, podem ser mudadas
//--- durante a vida do objeto. Caso se deseje gravar as alteraçòes feitas
//--- deve-se chamar o metodo Serialize().
//-----------------------------------------------------------------------------------

CGradiente::CGradiente(CString& NomeArquivo) : CArqGradiente(NomeArquivo)
{
  if(Consiste()) throw -1;

  if (IsEmpty()) return;  //--- Lista vazia? nada a fazer.

  Gradiente GradiAtual;
  ArqGradiente NoArqAtual;

  //--- Para cada um dos nos  da do arquivo de gradientes cria um nó na lista
  //--- de gradientes double. (No Arquivo são strings

  POSITION Atual = GetHeadPosition();

  while(Atual) 
  {
    NoArqAtual = GetAt(Atual);
    if(NoArqAtual.ListaCampos.size() == 3)              //---- Tem q ser
    {
      ittlstwstr It(NoArqAtual.ListaCampos.begin());
      CString Estaca,Ig;

      Estaca = It->c_str();
      It++;
      Ig = It->c_str();

      GradiAtual.EstFinal.Constroi(Estaca,CString(""),Ig);

      It++;
      GradiAtual.x = atof(It->c_str());

      SetGradientes.insert(GradiAtual);

      GetNext(Atual);
    }
  }									 
  RemoveAll();  //--- Nao Será Mais Usada
}

double CGradiente::BuscaGradiente(const CEstaca& Estaca)
{
  citsetGradientes ItSetGrad = SetGradientes.upper_bound(Estaca);
  return ItSetGrad != SetGradientes.end() ? (*ItSetGrad).x : 0.0;
}

bool CGradiente::Consiste()
{
  POSITION Atual = GetHeadPosition();
  int i(0),Erro(0);

  while(Atual && Erro == 0)
  {
    i++;
    Erro = GetNext(Atual).Consiste();

    if (Erro)
    {
      CString Mensg;

      Mensg.Format("Erro no arquivo de gradientes: o registro %d está invalido: ",i);
      Mensg += monolog.ErrosCurHor[Erro-1];

      monolog.mensagem(15,Mensg);
    }
  }
  return Erro > 0;
}