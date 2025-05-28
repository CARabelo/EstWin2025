#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqcanteiros.h" 
#include "cponto.h"
#include "clpontos.h"
#include "ccanteiros.h"

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

Canteiros::Canteiros() {}   //--- construtor default;

Canteiros::Canteiros(class Canteiros &Cant)  //--- Construtor de C�pia.
{
  *this = Cant;
}

Canteiros& Canteiros::operator=(Canteiros &No)
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

CCanteiros::CCanteiros(CString& NomeArquivo) : CArqCanteiros(NomeArquivo),Atual(NULL)
{
  CEstaca EstacaAtual;
  POSITION At(GetHeadPosition());
  CString LadoAnterior;

  if (Consiste()) throw -1;

  //--- Para cada os nos  da lista lida cria um n� na lista calculada

  while (At)
  {
    Canteiros CanteiroAtual;

    ArqCanteiros &NoArqAtual(GetNext(At));

    if(NoArqAtual.LstCampos.size() == 0) continue;      //--- Registro sem estaca, despreza

    CanteiroAtual.EstFinal.Constroi(NoArqAtual.LstCampos.begin()->c_str(),"");

    if(CanteiroAtual.EstFinal.EstVirtual == INFINITO) continue;  //--- Estaca errada ou em branco, despreza

    //--- Le os pontos, pode n�o ter nanhum

    if(NoArqAtual.LstCampos.size() > 3)       //--- Tem ao menos 1 ponto(4 campos)
    {
      ittlstwstr ItAtual(NoArqAtual.LstCampos.begin()),ItAnterior;   //--- Atual = y, anterior = x.

      ItAtual++;                //--- Despreza  a igualdade, n�o est� mais sendo usada;
      ItAnterior = ItAtual;     //--- x 
      bool Par(true);         

      for(ItAtual++; ItAtual !=  NoArqAtual.LstCampos.end() ; ItAtual++)
      {
        if(Par)           //--- Pega o y e insere o ponto
        {
          //--- Ultima chance, o  ponto tem q ter conteudo e tem q ser n�mero

          if (ItAtual->size() > 0 && ItAnterior->size() > 0 && SuperCString(ItAtual->c_str()).ENumero() && SuperCString(ItAnterior->c_str()).ENumero())
          {
            //--- Cria o ponto double e o insere no canteiro

            Ponto PontoAtual(atof(ItAnterior->c_str()),atof(ItAtual->c_str()),INFINITO,INFINITO,INFINITO,INFINITO,CString("CANTEI"));
            CanteiroAtual.InsereOrdenada(PontoAtual,TRUE);
          }
        }
        else Par = !Par;

        ItAnterior = ItAtual;     //--- Pega o x
      }
    }

    InsereOrdenada(CanteiroAtual);
  }

  Atual = LCanteiros.GetHeadPosition();
}

int CCanteiros::Compara(class Canteiros &No1,class Canteiros &No2)
{
  //--- Compara 2 n�s da lista.

  if (No1.EstFinal.EstVirtual < No2.EstFinal.EstVirtual) return MENOR;
  else if (No1.EstFinal.EstVirtual > No2.EstFinal.EstVirtual) return MAIOR;
  else return IGUAL; 
}

void CCanteiros::InsereOrdenada(class Canteiros &No,int INSERIRIGUAL)
{
  //--- Insere um n� ordenadamente na lista.
  //--- O ponteiro para o atual � a chave da insers�o, o metodo presume que o n�
  //--- a ser inserido est� pr�ximo do atual, a posic�o correta do n� �
  //--- procurada a partir do no atual. para tr�s ou para frente. 

  if (!Atual) Atual = LCanteiros.GetHeadPosition();  //--- Se atual esta nulo volta para o inicio da lista

  if (!Atual) LCanteiros.AddHead(No);  //--- Se a lista esta vazia , insere na cabe�a.
  else
  {
    int Res;

    //--- Se o no atual for maior que o a inserir procura para tr�s 
    //--- Sen�o se o no atual for menor que o a inserir procura para a frente
    //--- Sen�o se o n� atual for igual ao no a inserir insere o n�  depois do atual

    if ((Res = Compara(LCanteiros.GetAt(Atual),No)) == MAIOR)
    {
      LCanteiros.GetPrev(Atual); 

      while (Atual && Compara(LCanteiros.GetPrev(Atual),No) == MAIOR);

      if (!Atual) 
      {
        LCanteiros.AddHead(No);   //-- Se voltou a lista toda, adiciona na cabe�a.
        Atual = LCanteiros.GetHeadPosition();
      }
      else LCanteiros.InsertAfter(Atual,No);
    }
    else 
    {
      if (Res == MENOR)
      {
        LCanteiros.GetNext(Atual); 

        while (Atual && Compara(LCanteiros.GetNext(Atual),No) == MENOR);

        if (!Atual) 
        {
          LCanteiros.AddTail(No);	//--- Se passou a lista toda adiciona no fim.
          Atual = LCanteiros.GetTailPosition();
        }
        else LCanteiros.InsertBefore(Atual,No);
      }
      else if (INSERIRIGUAL) LCanteiros.InsertAfter(Atual,No);
    }
  }
}

int CCanteiros::PosicionaNoMaisProximo(CEstaca& Estaca)
{
  Atual = LCanteiros.GetHeadPosition();
  if (!Atual) return MENOR;   //-- Menor = n�o achou, lista vazia.

  while(Atual && LCanteiros.GetAt(Atual).EstFinal < Estaca) 
    LCanteiros.GetNext(Atual);

  if (!Atual) return MENOR;

  if (LCanteiros.GetAt(Atual).EstFinal > Estaca) return MAIOR;
  if (LCanteiros.GetAt(Atual).EstFinal < Estaca) return MENOR;
  return IGUAL;
}

bool CCanteiros::ECanteiro(CEstaca& Estaca,double Afast,int& Ini_Fim)
{
  int PosicaoAtual(PosicionaNoMaisProximo(Estaca));   //--- Procura pelo canteiro.

  //--- Se andou para tr�s retorna MENOR, caso contrario retorna MAIOR ou se  
  //--- parou exatamente na estaca retorna IGUAL.
  //--- Verifica se a estaca est� dentro de um trecho com canteiros verificando
  //--- se existem canteiros anteriores ou posteriores, dependendo da dire�ao
  //--- procurada.

  if(PosicaoAtual == MENOR) return false;   //--- N�o achou

  if (Atual)     //--- Se tem atual a estaca est� num trecho com canteiros centrais.
  {
    Canteiros& CC = LCanteiros.GetAt(Atual);

    POSITION PAtual = CC.GetHeadPosition();

    while(PAtual)
    {
      Ponto PontoAtual = CC.GetNext(PAtual);
      if (PontoAtual.x <= Afast && PontoAtual.y >= Afast) 
      {
        Ini_Fim = fabs(PontoAtual.x - Afast) < 1e-5 ? INICIO : fabs(PontoAtual.y - Afast) < 1e-5 ? FIM : MEIO;
        return TRUE;
      }
    }
  }
  return FALSE;
}

BOOL CCanteiros::BuscaCanteiros(CEstaca& Estaca,CLPontos &PontosCant)
{
  if (PosicionaNoMaisProximo(Estaca) == MENOR) return FALSE;

  PontosCant.RemoveAll();
  PontosCant.InsereLista(LCanteiros.GetAt(Atual),FALSE);

  return TRUE;
}

bool CCanteiros::Consiste()
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

      Mensg.Format("Erro no arquivo de canteiros centrais: o registro %d est� invalido: ",i);
      Mensg += monolog.ErrosCurHor[Erro-1];

      monolog.mensagem(15,Mensg);
    }
  }
  return Erro > 0;
}