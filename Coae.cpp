#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqoae.h" 
#include "cponto.h"
#include "clpontos.h"
#include "coae.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "monologo.h"

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
#define SO_X       0

extern class monologo monolog;

//----------------------------------------------------------------
//--- Implementação do nó da lista.

const class OAE &OAE::operator=(const class OAE &No)
{
  EstInicial = No.EstInicial;
  EstFinal = No.EstFinal;
  Descricao = No.Descricao;
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

COAE::COAE(const CString& NomeArquivo) : CArqOAE(NomeArquivo),Atual(NULL)
{
  if (IsEmpty()) return;  //--- Lista vazia? retorna.

  OAE OAEAtual;
  ArqOAE NoArqAtual;
  POSITION PosAtual;

  //--- Para cada os nos  da lista de Curvas lidas cria um nó na lista
  //--- de curvas calculadas, calculando o i da tangente que é a menor rampa das retas da
  //--- seçao.

  for(PosAtual = this->GetHeadPosition() ; PosAtual ; )
  {
    NoArqAtual = this->GetNext(PosAtual);

    ittlstwstr It(NoArqAtual.ListaCampos.begin());
    CString EstacaInicial(It->c_str());
    It++;
    CString IgInicial(It->c_str());
    It++;
    CString EstacaFinal(It->c_str());
    It++;
    CString IgFinal(It->c_str());
    It++;

    OAEAtual.EstInicial.Constroi(EstacaInicial,CString(""),IgInicial);
    OAEAtual.EstFinal.Constroi(EstacaFinal,CString(""),IgFinal);
    OAEAtual.Descricao = It->c_str();
    InsOrdenada(OAEAtual);
  }									 
  RemoveAll();
}

int COAE::Compara(class OAE &No1,class OAE &No2)
{
  //--- Compara 2 nós da lista.

  if (No1.EstInicial.EstVirtual < No2.EstInicial.EstVirtual) return MENOR;
  else if (No1.EstInicial.EstVirtual > No2.EstInicial.EstVirtual) return MAIOR;
  else return IGUAL; 
}

void COAE::InsOrdenada(class OAE &No)
{
  //--- Insere um nó ordenadamente na lista.
  //--- O ponteiro para o atual é a chave da insersão, o metodo presume que o nó
  //--- a ser inserido está próximo do atual, a posicão correta do nó é
  //--- procurada a partir do no atual. para trás ou para frente. 

  Atual = GetHeadPosition();

  if (!Atual) LOAE.AddHead(No);  //--- Se a lista esta vazia , insere na cabeça.
  else
  {
    int Res;

    //--- Se o no atual for maior que o a inserir procura para trás 
    //--- Senão se o no atual for menor que o a inserir procura para a frente
    //--- Senão se o nó atual for igual ao no a inserir insere o nó  depois do atual

    if ((Res = Compara(LOAE.GetAt(Atual),No)) == MAIOR)
    {
      LOAE.GetPrev(Atual); 

      while (Atual && Compara(LOAE.GetPrev(Atual),No) == MAIOR);

      if (!Atual) LOAE.AddHead(No);
      else LOAE.InsertAfter(Atual,No);
    }
    else 
    {
      if (Res == MENOR)
      {
        LOAE.GetNext(Atual); 

        while (Atual && Compara(LOAE.GetNext(Atual),No) == MENOR );

        if (!Atual) LOAE.AddTail(No);
        else LOAE.InsertBefore(Atual,No);

      }
      else LOAE.InsertAfter(Atual,No);
    }
  }
}

int COAE::PosicionaNoMaisProximo(CEstaca Estaca,int EstInicialFinal)
{
  Atual = LOAE.GetHeadPosition();
  if (!Atual) return MENOR;
  
  if (EstInicialFinal == 0)     //--- INICIAL
  {
    LOAE.GetAt(Atual);

    while(Atual && LOAE.GetAt(Atual).EstInicial < Estaca) LOAE.GetNext(Atual);

    if (!Atual) Atual = LOAE.GetTailPosition();
  }
  else                         //--- FINAL
  {
    LOAE.GetAt(Atual);

    while(Atual && LOAE.GetAt(Atual).EstFinal < Estaca) LOAE.GetNext(Atual);

    if (!Atual) Atual = LOAE.GetTailPosition();
  }

  if(EstInicialFinal == 0)
  {
    if (LOAE.GetAt(Atual).EstInicial > Estaca) return MAIOR;
    if (LOAE.GetAt(Atual).EstInicial < Estaca) return MENOR;
    return IGUAL;
  }

  if (LOAE.GetAt(Atual).EstFinal > Estaca) return MAIOR;
  if (LOAE.GetAt(Atual).EstFinal < Estaca) return MENOR;
  return IGUAL;
}

void  COAE::PegaEstacasObrigatorias(CList<CEstaca,CEstaca&>& EstObrig,double Equidis)
{
  Atual = LOAE.GetHeadPosition();

  while(Atual)
  {
    OAE OAEAtual= LOAE.GetNext(Atual);

    EstObrig.AddTail(OAEAtual.EstInicial);

    for(double DistAtual = OAEAtual.EstInicial.EstVirtual ; DistAtual < OAEAtual.EstFinal.EstVirtual ; DistAtual += Equidis)
    {
      double Multiplos;
      modf(DistAtual / Equidis,&Multiplos);
      EstObrig.AddTail((CEstaca) (Multiplos * Equidis));  
    } 
    EstObrig.AddTail(OAEAtual.EstFinal);  
  }
}

bool COAE::Consiste()
{
  POSITION Atual = GetHeadPosition();
  int i(0),Erro(0);
  CString Mensg;

  while(Atual && Erro == 0)
  {
    i++;
    Erro = GetAt(Atual).Consiste();

    if (Erro)
    {
      Mensg.Format("Erro no arquivo de obras de arte especiais: o registro %d está invalido: ",i);
      Mensg += monolog.ErrosOAE[Erro-1];

      monolog.mensagem(15,Mensg);
    }
  }
  return Erro > 0;
}

POSITION  COAE::PosicionaEstaca(double x,double y,double Toleranciax,double Toleranciay,int* InicioFim,int Tipo)
{
   //--- Posiciona na secao com estaca = x, aceitando a tolerancias x e y

  *InicioFim = 0;

  int EMaisProx(PosicionaNoMaisProximo(x,0));

  if (!Atual) return NULL;

  const CEstaca& EstacaAtual(LOAE.GetAt(Atual).EstInicial);

  POSITION Retorno(NULL);
  
  if(Tipo == SO_X) 
  Retorno = EstacaAtual.EstVirtual + Toleranciax > x && EstacaAtual.EstVirtual - Toleranciax < x  ? Atual : NULL;
  else
  Retorno = EstacaAtual.EstVirtual + Toleranciax > x && EstacaAtual.EstVirtual - Toleranciax < x &&
            EstacaAtual.Cota + Toleranciay > y && EstacaAtual.Cota - Toleranciay < y ? Atual : NULL;

  if(!Retorno)
  {
    *InicioFim = 1;

    EMaisProx = PosicionaNoMaisProximo(x,1);

    if (!Atual) return NULL;

    CEstaca& EstacaAtual2(LOAE.GetAt(Atual).EstFinal);

    if(Tipo == SO_X)
      Retorno =  EstacaAtual2.EstVirtual + Toleranciax > x && EstacaAtual2.EstVirtual - Toleranciax < x ? Atual : NULL;
    else
      Retorno = EstacaAtual2.EstVirtual + Toleranciax > x && EstacaAtual2.EstVirtual - Toleranciax < x &&
                EstacaAtual2.Cota + Toleranciay > y && EstacaAtual2.Cota - Toleranciay < y ? Atual : NULL;
  }

  return Retorno;
}