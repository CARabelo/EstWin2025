#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqalargamentos.h" 
#include "cponto.h"
#include "clpontos.h"
#include "calargamentos.h"

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

Alargamento::Alargamento() {}   //--- construtor default;

Alargamento::Alargamento(Alargamento &Alarg)  //--- Construtor de Cópia.
{
  *this = Alarg;
}

Alargamento& Alargamento::operator=(Alargamento &No)
{
  EstFinal = No.EstFinal;
  Lado = No.Lado;

  RemoveAll();

  CLPontos::operator=(No);

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

CAlargamento::CAlargamento(CString& NomeArquivo) : CArqAlargamento(NomeArquivo),Erro(0)
{
  Atual[ESQUERDO] = Atual[DIREITO] = NULL;
  CEstaca EstacaAtual;
  double EstacaAnterior(INFINITO);
  POSITION At(GetHeadPosition());

  Erro = Consiste();

  if(Erro) return; 
  
  //--- Para cada os nos  da lista de Curvas lidas cria um nó na lista

  while (At)
  {
    ArqAlargamento &NoArqAtual(GetNext(At));

    ittlstwstr It(NoArqAtual.LstCampos.begin());

    CString Estaca(It->c_str());
    It++;
    CString Ig(It->c_str());
    It++;

    Alargamento AlargamentoAtual;

    AlargamentoAtual.EstFinal.Constroi(Estaca,"",Ig);

    if(*It == "e") *It = "E";
    else if(*It == "d") *It = "D";

    AlargamentoAtual.Lado = It->c_str();
    //--- Se a primeira secao iguala o anterior com o atual.
    				
    for(It++ ; It != NoArqAtual.LstCampos.end() ; It++)   
    {
       //--- 0 não é numero
       //--- 1 é numero
       //--- 2 string vazio

       SuperCString Coordx(It->c_str());
       It++;                                   
       SuperCString Coordy(It->c_str());

       if(Coordx.ENumero() == 1 && Coordy.ENumero() == 1)
       {
         Ponto PontoAtual(atof(Coordx.GetBuffer()),atof(Coordy.GetBuffer()),INFINITO,INFINITO,INFINITO,INFINITO,CString(AlargamentoAtual.Lado == "E" ? "ALARGE" : "ALARGD"));
         AlargamentoAtual.Insere(PontoAtual);
       }
    }
 
    InsereOrdenada(AlargamentoAtual);
  }									 
  Atual[ESQUERDO] = LAlargamento[ESQUERDO].GetHeadPosition();
  Atual[DIREITO] = LAlargamento[DIREITO].GetHeadPosition();
}

int CAlargamento::Compara(class Alargamento &No1,class Alargamento &No2)
{
  //--- Compara 2 nós da lista.

  if (No1.EstFinal.EstVirtual < No2.EstFinal.EstVirtual) return MENOR;
  else if (No1.EstFinal.EstVirtual > No2.EstFinal.EstVirtual) return MAIOR;
  else return IGUAL; 
}

void CAlargamento::InsereOrdenada(class Alargamento &No,int INSERIRIGUAL)
{
  //--- Insere um nó ordenadamente na lista.
  //--- O ponteiro para o atual é a chave da insersão, o metodo presume que o nó
  //--- a ser inserido está próximo do atual, a posicão correta do nó é
  //--- procurada a partir do no atual. para trás ou para frente. 

  int Lado = No.Lado == "E" ? ESQUERDO : DIREITO;  

  if (!Atual[Lado]) Atual[Lado] = LAlargamento[Lado].GetHeadPosition();  //--- Se atual esta nulo volta para o inicio da lista

  if (!Atual[Lado]) LAlargamento[Lado].AddHead(No);  //--- Se a lista esta vazia , insere na cabeça.
  else
  {
    int Res;

    //--- Se o no atual for maior que o a inserir procura para trás 
    //--- Senão se o no atual for menor que o a inserir procura para a frente
    //--- Senão se o nó atual for igual ao no a inserir insere o nó  depois do atual

    if ((Res = Compara(LAlargamento[Lado].GetAt(Atual[Lado]),No)) == MAIOR)
    {
      LAlargamento[Lado].GetPrev(Atual[Lado]); 

      while (Atual[Lado] && Compara(LAlargamento[Lado].GetPrev(Atual[Lado]),No) == MAIOR);

      if (!Atual[Lado]) 
      {
        LAlargamento[Lado].AddHead(No);   //-- Se voltou a lista toda, adiciona na cabeça.
        Atual[Lado] = LAlargamento[Lado].GetHeadPosition();
      }
      else LAlargamento[Lado].InsertAfter(Atual[Lado],No);
    }
    else 
    {
      if (Res == MENOR)
      {
        LAlargamento[Lado].GetNext(Atual[Lado]); 

        while (Atual[Lado] && Compara(LAlargamento[Lado].GetNext(Atual[Lado]),No) == MENOR);

        if (!Atual[Lado]) 
        {
          LAlargamento[Lado].AddTail(No);	//--- Se passou a lista toda adiciona no fim.
          Atual[Lado] = LAlargamento[Lado].GetTailPosition();
        }
        else LAlargamento[Lado].InsertBefore(Atual[Lado],No);
      }
      else if (INSERIRIGUAL) LAlargamento[Lado].InsertAfter(Atual[Lado],No);
    }
  }
}

int CAlargamento::PosicionaNoMaisProximo(CEstaca& Estaca,int Lado,int Direcao)
{
  Atual[Lado] = LAlargamento[Lado].GetHeadPosition();

  if (!Atual[Lado]) return MENOR;   //-- Menor = não achou, lista vazia

  while(Atual[Lado] && LAlargamento[Lado].GetAt(Atual[Lado]).EstFinal < Estaca) LAlargamento[Lado].GetNext(Atual[Lado]);

  if (!Atual[Lado]) return MENOR;  
  if (LAlargamento[Lado].GetAt(Atual[Lado]).EstFinal > Estaca) return MAIOR;

  return IGUAL;
}

BOOL CAlargamento::BuscaAlargamento(CEstaca Estaca, int Lado, CLPontos &PontosAlarg)
{
  if (PosicionaNoMaisProximo(Estaca,Lado) == MENOR) return FALSE;

  PontosAlarg.InsereLista(LAlargamento[Lado].GetAt(Atual[Lado]),TRUE);

  return TRUE;
}

bool CAlargamento::Consiste()
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

      Mensg.Format("Erro no arquivo de alargamentos: o registro %d está invalido: ",i);
      Mensg += monolog.ErrosAlarg[Erro-1];

      monolog.mensagem(15,Mensg);
    }
  }

  return Erro > 0;
 

  return (0);
}