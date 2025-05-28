#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqIgualdades.h" 
#include "cponto.h"
#include "clpontos.h"
#include "cIgualdade.h"

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
#define NULO      -2

extern class monologo monolog;

//----------------------------------------------------------------
//--- Implementação do nó da lista.

const class Igualdade &Igualdade::operator=(const class Igualdade &No)
{
  Estaca1 = No.Estaca1;
  Estaca2 = No.Estaca2;
  Diferenca = No.Diferenca;
  DifAcumulada = No.DifAcumulada;

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

CIgualdade::CIgualdade(CString& NomeArquivo) : CArqIgualdade(NomeArquivo), LIgualdade()
{
  if(Consiste()) throw -1;

  if (IsEmpty()) return;  //--- Lista vazia? retorna.

  //--- Para cada os nos  da lista de Curvas lidas cria um nó na lista
  //--- de curvas calculadas, calculando i i da tangente que é a menor rampa das retas da
  //--- seçao.

  Atual = GetHeadPosition();

  for (	class ArqIgualdade NoArqAtual= GetNext(Atual);
    Atual ; 
    NoArqAtual = GetNext(Atual)) 
  {
    double DifAcum = 0.0;
    class Igualdade IgualAtual;

    //  IgualAtual.Estaca1.Constroi(NoArqAtual.Estaca1,0);
    //  IgualAtual.Estaca2.Constroi(NoArqAtual.Estaca2,0);
    IgualAtual.Diferenca = IgualAtual.Estaca1.EstVirtual - IgualAtual.Estaca2.EstVirtual;
    IgualAtual.DifAcumulada = DifAcum += IgualAtual.Diferenca;
    InsOrdenada(IgualAtual);
  }									 
}

int CIgualdade::Compara(class Igualdade &No1,class Igualdade &No2)
{
  //--- Compara 2 nós da lista.

  if (No1.Estaca1.EstVirtual < No2.Estaca1.EstVirtual) return MENOR;
  else if (No1.Estaca1.EstVirtual > No2.Estaca1.EstVirtual) return MAIOR;
  else return IGUAL; 
}

void CIgualdade::InsOrdenada(class Igualdade &No)
{
  //--- Insere um nó ordenadamente na lista.
  //--- O ponteiro para o atual é a chave da insersão, o metodo presume que o nó
  //--- a ser inserido está próximo do atual, a posicão correta do nó é
  //--- procurada a partir do no atual. para trás ou para frente. 

  if (!Atual) LIgualdade.AddHead(No);  //--- Se a lista esta vazia , insere na cabeça.
  else
  {
    int Res;

    //--- Se o no atual for maior que o a inserir procura para trás 
    //--- Senão se o no atual for menor que o a inserir procura para a frente
    //--- Senão se o nó atual for igual ao no a inserir insere o nó  depois do atual

    if ((Res = Compara(LIgualdade.GetAt(Atual),No)) == MAIOR)
    {
      LIgualdade.GetPrev(Atual); 

      while (Atual && Compara(LIgualdade.GetPrev(Atual),No) == MAIOR);

      if (!Atual) LIgualdade.AddHead(No);
      else LIgualdade.InsertAfter(Atual,No);
    }
    else 
    {
      if (Res == MENOR)
      {
        LIgualdade.GetNext(Atual); 

        while (Atual && Compara(LIgualdade.GetNext(Atual),No) == MENOR );

        if (!Atual) LIgualdade.AddTail(No);
        else LIgualdade.InsertBefore(Atual,No);

      }
      else LIgualdade.InsertAfter(Atual,No);
    }
  }
}

int CIgualdade::PosicionaCurvaProxima(double Estaca,int Direcao)
{
  if (!Atual) Atual = LIgualdade.GetHeadPosition(); //--- Se não tem atual, atual = inicio.
  if (!Atual) return NULO;  //--- Lista vazia, retorna NULO.

  Igualdade &NoAtual = LIgualdade.GetAt(Atual);

  //--- Se direcão veio FALSE, verifica a direçao a caminhar na lista,
  //--- se o nó atual for o nó igual, retorna o no atual.

  if (!Direcao && !(Direcao = Estaca < NoAtual.Estaca1.EstVirtual ? TRAS : 
    Estaca > NoAtual.Estaca1.EstVirtual ? FRENTE : FALSE)) return IGUAL;

  //--- Posiciona o atual no igual ou mais proximo da estaca desejada
  //--- no sentido indicado:
  //---   Se a procura foi feita para tras e não foi encontrado o nó
  //--- igual, o atual é posicionado no nó anterior, se a procura for 
  //--- feita para frente e o no igual não foi encontrado o atual é 
  //--- posicionado no nó posterior, se o no igual foi encontrado o 
  //--- atual é posicionado no nó encontrado.

  for ( ; Atual && (Direcao == FRENTE ? NoAtual.Estaca1.EstVirtual < Estaca : NoAtual.Estaca1.EstVirtual > Estaca) ;  
    (Direcao == FRENTE ? NoAtual = LIgualdade.GetNext(Atual) : NoAtual = LIgualdade.GetPrev(Atual)));

    //--- Os dados estão no NoAtual, retora se o que esta em NoAtual e menor maior ou igual (ou NULO).

    if (NoAtual.Estaca1.EstVirtual < Estaca) return MENOR;
  if (NoAtual.Estaca1.EstVirtual > Estaca) return MAIOR;

  return IGUAL;
}

bool CIgualdade::Consiste()
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

      Mensg.Format("Erro no arquivo de igualdades: o registro %d está invalido: ",i);
      Mensg += monolog.ErrosCurHor[Erro-1];

      monolog.mensagem(15,Mensg);
    }
  }
  return Erro > 0;
}
