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
//--- Implementa��o do n� da lista.

Alargamento::Alargamento() {}   //--- construtor default;

Alargamento::Alargamento(Alargamento &Alarg)  //--- Construtor de C�pia.
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
//--- Implementa��o da lista
//--- Neste objeto s�o criadas duas listas na mem�ria, uma lista com os dados do 
//--- arquivo e outra lista com os dados calculados, como este objeto � gen�rico,
//--- as duas listas ficam na memoria e devem ser destru�das pelo recipiente 
//--- a medida do necess�rio. Estas listas s�o din�micas, podem ser mudadas
//--- durante a vida do objeto. Caso se deseje gravar as altera��es feitas
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
  
  //--- Para cada os nos  da lista de Curvas lidas cria um n� na lista

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
       //--- 0 n�o � numero
       //--- 1 � numero
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
  //--- Compara 2 n�s da lista.

  if (No1.EstFinal.EstVirtual < No2.EstFinal.EstVirtual) return MENOR;
  else if (No1.EstFinal.EstVirtual > No2.EstFinal.EstVirtual) return MAIOR;
  else return IGUAL; 
}

void CAlargamento::InsereOrdenada(class Alargamento &No,int INSERIRIGUAL)
{
  //--- Insere um n� ordenadamente na lista.
  //--- O ponteiro para o atual � a chave da insers�o, o metodo presume que o n�
  //--- a ser inserido est� pr�ximo do atual, a posic�o correta do n� �
  //--- procurada a partir do no atual. para tr�s ou para frente. 

  int Lado = No.Lado == "E" ? ESQUERDO : DIREITO;  

  if (!Atual[Lado]) Atual[Lado] = LAlargamento[Lado].GetHeadPosition();  //--- Se atual esta nulo volta para o inicio da lista

  if (!Atual[Lado]) LAlargamento[Lado].AddHead(No);  //--- Se a lista esta vazia , insere na cabe�a.
  else
  {
    int Res;

    //--- Se o no atual for maior que o a inserir procura para tr�s 
    //--- Sen�o se o no atual for menor que o a inserir procura para a frente
    //--- Sen�o se o n� atual for igual ao no a inserir insere o n�  depois do atual

    if ((Res = Compara(LAlargamento[Lado].GetAt(Atual[Lado]),No)) == MAIOR)
    {
      LAlargamento[Lado].GetPrev(Atual[Lado]); 

      while (Atual[Lado] && Compara(LAlargamento[Lado].GetPrev(Atual[Lado]),No) == MAIOR);

      if (!Atual[Lado]) 
      {
        LAlargamento[Lado].AddHead(No);   //-- Se voltou a lista toda, adiciona na cabe�a.
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

  if (!Atual[Lado]) return MENOR;   //-- Menor = n�o achou, lista vazia

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

      Mensg.Format("Erro no arquivo de alargamentos: o registro %d est� invalido: ",i);
      Mensg += monolog.ErrosAlarg[Erro-1];

      monolog.mensagem(15,Mensg);
    }
  }

  return Erro > 0;
 

  return (0);
}