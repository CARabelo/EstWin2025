#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqTaludesPavim.h" 
#include "TaludesPavim.h"

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

TaludesPavim::TaludesPavim()  //--- construtor default;
{
  //--- Assume Valores Default 

  EstFinal = 99999.0;
  Rampas[0] = -100.0;
  Rampas[1] = 1.5;
  Rampas[2] = 100.0;
  Rampas[3] = -1.5;
} 

TaludesPavim::TaludesPavim(TaludesPavim &TalPavi)  //--- Construtor de Cópia.
{
  *this = TalPavi;
}

const TaludesPavim& TaludesPavim::operator=(const TaludesPavim &No)
{
  EstFinal = No.EstFinal;
  Rampas[0] = No.Rampas[0];
  Rampas[1] = No.Rampas[1];
  Rampas[2] = No.Rampas[2];
  Rampas[3] = No.Rampas[3];

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

CTaludesPavim::CTaludesPavim(CString& NomeArquivo) : CArqTaludesPavim(NomeArquivo)
{
  Atual = NULL;

  class TaludesPavim TaludesPavimAtual;
  POSITION At = GetHeadPosition();

  //--- Para cada os nos  da lista de Curvas lidas cria um nó na lista

  while (At)
  {
    ArqTaludesPavim &NoArqAtual(GetNext(At));

    if(NoArqAtual.ListaCampos.size() > 0)
    {
      ittlstwstr It(NoArqAtual.ListaCampos.begin()); 
      int C(0);

      CString Estaca(It->c_str());
      It++;
      CString Ig(It->c_str());

      TaludesPavimAtual.EstFinal.Constroi(Estaca,"",Ig);

      while(++It != NoArqAtual.ListaCampos.end() && C < 4) 
      {
        TaludesPavimAtual.Rampas[C] = atof(It->c_str());
        C++;
      }

      InsOrdenada(TaludesPavimAtual);
    }
  }									 

  Atual = LTaludesPavim.GetHeadPosition();
}

int CTaludesPavim::Compara(class TaludesPavim &No1,class TaludesPavim &No2)
{
  //--- Compara 2 nós da lista.

  if (No1.EstFinal.EstVirtual < No2.EstFinal.EstVirtual) return MENOR;
  else if (No1.EstFinal.EstVirtual > No2.EstFinal.EstVirtual) return MAIOR;
  else return IGUAL; 
}

void CTaludesPavim::InsOrdenada(class TaludesPavim &No,int INSERIRIGUAL)
{
  if (!Atual) Atual = LTaludesPavim.GetHeadPosition();  //--- Se atual esta nulo volta para o inicio da lista

  //--- Insere um nó ordenadamente na lista.
  //--- O ponteiro para o atual é a chave da insersão, o metodo presume que o nó
  //--- a ser inserido está próximo do atual, a posicão correta do nó é
  //--- procurada a partir do no atual. para trás ou para frente. 

  if (!Atual) LTaludesPavim.AddHead(No);  //--- Se a lista esta vazia , insere na cabeça.
  else
  {
    int Res;

    //--- Se o no atual for maior que o a inserir procura para trás 
    //--- Senão se o no atual for menor que o a inserir procura para a frente
    //--- Senão se o nó atual for igual ao no a inserir insere o nó  depois do atual

    if ((Res = Compara(LTaludesPavim.GetAt(Atual),No)) == MAIOR)
    {
      LTaludesPavim.GetPrev(Atual); 

      while (Atual && Compara(LTaludesPavim.GetPrev(Atual),No) == MAIOR);

      if (!Atual) LTaludesPavim.AddHead(No);   //-- Se voltou a lista toda, adiciona na cabeça.
      else LTaludesPavim.InsertAfter(Atual,No);
    }
    else 
    {
      if (Res == MENOR)
      {
        LTaludesPavim.GetNext(Atual); 

        while (Atual && Compara(LTaludesPavim.GetNext(Atual),No) == MENOR);

        if (!Atual) LTaludesPavim.AddTail(No);	//--- Se passou a lista toda adiciona no rabo. (hehe)
        else LTaludesPavim.InsertBefore(Atual,No);

      }
      else if (INSERIRIGUAL) LTaludesPavim.InsertAfter(Atual,No);
    }
  }
}

int CTaludesPavim::PosicionaNoMaisProximo(CEstaca Estaca,int Direcao)
{
  if (1 || !Atual) Atual = LTaludesPavim.GetHeadPosition();
  if (!Atual) return MENOR;

 // if (!(Direcao = Estaca > LTaludesPavim.GetAt(Atual).EstFinal ? TRAS : Estaca < LTaludesPavim.GetAt(Atual).EstFinal ? FRENTE : FALSE)) return IGUAL;

  Direcao = FRENTE;

  if (Direcao == FRENTE)
  {
    //LTaludesPavim.GetNext(Atual);

    while(Atual && LTaludesPavim.GetAt(Atual).EstFinal < Estaca) LTaludesPavim.GetNext(Atual);

    if (!Atual) Atual = LTaludesPavim.GetTailPosition();
  }
  else
  {
    //LTaludesPavim.GetPrev(Atual);

    while(Atual && LTaludesPavim.GetAt(Atual).EstFinal > Estaca) LTaludesPavim.GetNext(Atual);

    if (!Atual) Atual = LTaludesPavim.GetHeadPosition();
  }

  if (LTaludesPavim.GetAt(Atual).EstFinal > Estaca) return MAIOR;
  if (LTaludesPavim.GetAt(Atual).EstFinal < Estaca) return MENOR;
  return IGUAL;

}


BOOL CTaludesPavim::BuscaTaludesPavim(CEstaca& Estaca,TaludesPavim& TaludesPavimento)
{
  if(PosicionaNoMaisProximo(Estaca.EstVirtual) == MENOR) 
  {
    /*
    //--- Valores default.

    TaludesPavimento.EstFinal = 99999.0;
    TaludesPavimento.Rampas[0] = -100.0;
    TaludesPavimento.Rampas[1] = .666666;
    TaludesPavimento.Rampas[2] = 100.0;
    TaludesPavimento.Rampas[3] = -.666666;

    */

    return FALSE;
  }
  else TaludesPavimento = LTaludesPavim.GetAt(Atual);;

  return TRUE;
}
void CTaludesPavim::TaludesDefault()
{
  /*
  TaludesPavim TaludesPavimento;

  TaludesPavimento.EstFinal = 99999.0;
  TaludesPavimento.Rampas[0] = -100.0;
  TaludesPavimento.Rampas[1] = .6666666;
  TaludesPavimento.Rampas[2] = 100.0;
  TaludesPavimento.Rampas[3] = -.666666;

  LTaludesPavim.AddTail(TaludesPavimento);
  */
}

bool CTaludesPavim::Consiste()
{
  POSITION Atual = this->GetHeadPosition();
  int i(0),Erro(0);

  while(Atual && Erro == 0)
  {
    i++;
    Erro = GetNext(Atual).Consiste();

    if (Erro)
    {
      CString Mensg;

      Mensg.Format("Erro no arquivo taludes do pavimento: o registro %d está invalido: ",i);
      Mensg += monolog.ErrosCurHor[Erro-1];

      monolog.mensagem(15,Mensg);
    }
  }
  return Erro > 0;
}