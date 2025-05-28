#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqTalAterro.h" 
#include "cponto.h"
#include "clpontos.h"
#include "cTalAterro.h"

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

TaludeAterro::TaludeAterro() {}   //--- construtor default;

TaludeAterro::TaludeAterro(TaludeAterro &Alarg)  //--- Construtor de Cópia.
{
  *this = Alarg;
}

TaludeAterro& TaludeAterro::operator=(TaludeAterro &No)
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

CTaludesAterro::CTaludesAterro(CString& NomeArq): CArqTalAterro(NomeArq)
{
  Atual[ESQUERDO] = Atual[DIREITO] = NULL;
  POSITION At(GetHeadPosition());

  //--- Para cada dos nós digitados cria um nó na lista de taludes calculados 

  while (At)
  {
    ArqTalAterro &NoArqAtual(GetNext(At));
    ittlstwstr It(NoArqAtual.ListaCampos.begin());

    if(NoArqAtual.ListaCampos.size() > 5)
    {
      TaludeAterro TaludesAterroAtual;

      CString Estaca(It->c_str());
      It++;
      TaludesAterroAtual.EstFinal.Constroi(Estaca,*It->c_str());
      It++;
    
      TaludesAterroAtual.Lado = It->c_str();

      for (It++ ; It != NoArqAtual.ListaCampos.end() ; It++)
      {
        SuperCString Coordx(It->c_str());

        It++;

        if(It != NoArqAtual.ListaCampos.end())
        {
           SuperCString Coordy(It->c_str());

           if(Coordx.ENumero() && Coordy.ENumero())
           {
             Ponto PontoAtual(atof(Coordx),atof(Coordy),INFINITO,INFINITO,INFINITO,INFINITO,CString(TaludesAterroAtual.Lado.CompareNoCase("E") == 0 ? "TALATE" : "TALATD"));

             TaludesAterroAtual.AddTail(PontoAtual);
           }
        }
      }
      InsOrdenada(TaludesAterroAtual);
    }
  }									 
  Atual[ESQUERDO] = LTaludesAterro[ESQUERDO].GetHeadPosition();
  Atual[DIREITO] = LTaludesAterro[DIREITO].GetHeadPosition();
}

int CTaludesAterro::Compara(TaludeAterro &No1,TaludeAterro &No2)
{
  //--- Compara 2 nós da lista.

  if (No1.EstFinal.EstVirtual < No2.EstFinal.EstVirtual) return MENOR;
  else if (No1.EstFinal.EstVirtual > No2.EstFinal.EstVirtual) return MAIOR;
  else return IGUAL; 
}

void CTaludesAterro::InsOrdenada(TaludeAterro &No,int INSERIRIGUAL)
{
  //--- Insere um nó ordenadamente na lista.
  //--- O ponteiro para o atual é a chave da insersão, o metodo presume que o nó
  //--- a ser inserido está próximo do atual, a posicão correta do nó é
  //--- procurada a partir do no atual. para trás ou para frente. 

  int Lado = No.Lado == "E" || No.Lado == "e" ? ESQUERDO : DIREITO;  

  if (!Atual[Lado]) Atual[Lado] = LTaludesAterro[Lado].GetHeadPosition();  //--- Se atual esta nulo volta para o inicio da lista

  if (!Atual[Lado]) LTaludesAterro[Lado].AddHead(No);  //--- Se a lista esta vazia , insere na cabeça.
  else
  {
    int Res;

    //--- Se o no atual for maior que o a inserir procura para trás 
    //--- Senão se o no atual for menor que o a inserir procura para a frente
    //--- Senão se o nó atual for igual ao no a inserir insere o nó  depois do atual

    if ((Res = Compara(LTaludesAterro[Lado].GetAt(Atual[Lado]),No)) == MAIOR)
    {
      LTaludesAterro[Lado].GetPrev(Atual[Lado]); 

      while (Atual[Lado] && Compara(LTaludesAterro[Lado].GetPrev(Atual[Lado]),No) == MAIOR);

      if (!Atual[Lado]) LTaludesAterro[Lado].AddHead(No);   //-- Se voltou a lista toda, adiciona na cabeça.
      else LTaludesAterro[Lado].InsertAfter(Atual[Lado],No);
    }
    else 
    {
      if (Res == MENOR)
      {
        LTaludesAterro[Lado].GetNext(Atual[Lado]); 

        while (Atual[Lado] && Compara(LTaludesAterro[Lado].GetNext(Atual[Lado]),No) == MENOR);

        if (!Atual[Lado]) LTaludesAterro[Lado].AddTail(No);	//--- Se passou a lista toda adiciona no rabo. (hehe)
        else LTaludesAterro[Lado].InsertBefore(Atual[Lado],No);

      }
      else if (INSERIRIGUAL) LTaludesAterro[Lado].InsertAfter(Atual[Lado],No);
    }
  }
}

int CTaludesAterro::PosicionaNoMaisProximo(CEstaca Estaca,int Lado,int Direcao)
{
  Atual[Lado] = LTaludesAterro[Lado].GetHeadPosition();

  if (!Atual[Lado]) return MENOR;   //-- Menor = não achou, lista vazia

  while(Atual[Lado] && LTaludesAterro[Lado].GetAt(Atual[Lado]).EstFinal < Estaca) LTaludesAterro[Lado].GetNext(Atual[Lado]);

  if (!Atual[Lado]) return MENOR;  
  if (LTaludesAterro[Lado].GetAt(Atual[Lado]).EstFinal > Estaca) return MAIOR;

  return IGUAL;

  /*
  if (!Atual[Lado]) Atual[Lado] = LTaludesAterro[Lado].GetHeadPosition();
  if (!Atual[Lado]) return MAIOR;

  if (!(Direcao = Estaca > LTaludesAterro[Lado].GetAt(Atual[Lado]).EstFinal ? TRAS : Estaca < LTaludesAterro[Lado].GetAt(Atual[Lado]).EstFinal ? FRENTE : FALSE)) return IGUAL;

  if (Direcao == FRENTE)
  {
    LTaludesAterro[Lado].GetNext(Atual[Lado]);

    while(Atual[Lado] && LTaludesAterro[Lado].GetAt(Atual[Lado]).EstFinal < Estaca) LTaludesAterro[Lado].GetNext(Atual[Lado]);

    if (!Atual[Lado]) Atual[Lado] = LTaludesAterro[Lado].GetTailPosition();
  }
  else
  {
    LTaludesAterro[Lado].GetPrev(Atual[Lado]);

    while(Atual[Lado] && LTaludesAterro[Lado].GetAt(Atual[Lado]).EstFinal > Estaca) LTaludesAterro[Lado].GetNext(Atual[Lado]);

    if (!Atual[Lado]) Atual[Lado] = LTaludesAterro[Lado].GetHeadPosition();
  }

  if (LTaludesAterro[Lado].GetAt(Atual[Lado]).EstFinal > Estaca) return MAIOR;
  if (LTaludesAterro[Lado].GetAt(Atual[Lado]).EstFinal < Estaca) return MENOR;
  return IGUAL;
  */
}

BOOL CTaludesAterro::BuscaTalude(CEstaca Estaca,int Lado,CLPontos &PontosAlarg)
{
  if(PosicionaNoMaisProximo(Estaca.EstVirtual,Lado) == MENOR) return FALSE;

  if (!Atual[Lado]) return false;

  PontosAlarg = LTaludesAterro[Lado].GetAt(Atual[Lado]);
  return TRUE;
}

void CTaludesAterro::TaludesDefault()
{
  if (LTaludesAterro[ESQUERDO].IsEmpty())
  {
    TaludeAterro TaludePadrao;
    TaludePadrao.EstFinal.Constroi(999999);
    TaludePadrao.Lado = "E";
    TaludePadrao.AddTail(Ponto(0.0,0.0,0.0,INFINITO,INFINITO,INFINITO,CString("TALATE")));
    TaludePadrao.AddTail(Ponto(-1500.0,-1000.0,0.0,INFINITO,INFINITO,INFINITO,CString("TALATE")));
    LTaludesAterro[ESQUERDO].AddTail(TaludePadrao);
  }
  if (LTaludesAterro[DIREITO].IsEmpty())
  {
    TaludeAterro TaludePadrao;
    TaludePadrao.EstFinal.Constroi(999999);
    TaludePadrao.Lado = "D";
    TaludePadrao.AddTail(Ponto(0.0,0.0,0.0,INFINITO,INFINITO,INFINITO,CString("TALATD")));
    TaludePadrao.AddTail(Ponto(1500.0,-1000.0,0.0,INFINITO,INFINITO,INFINITO,CString("TALATD")));
    LTaludesAterro[DIREITO].AddTail(TaludePadrao);
  }
}


bool CTaludesAterro::Consiste()   //--- Consiste digitacao
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

      Mensg.Format("Erro no arquivo de obras de taludes de aterro: o registro %d está invalido: ",i);
      Mensg += monolog.ErrosCurHor[Erro-1];

      monolog.mensagem(15,Mensg);
    }
  }
  return Erro > 0;
}

int CTaludesAterro::ConsisteDados()   //--- consiste os pontos do talude
{
  if(LTaludesAterro[ESQUERDO].GetCount() == 0 || LTaludesAterro[DIREITO].GetCount() == 0) return 1;

  for(int Lado = ESQUERDO ; Lado < DIREITO ;  Lado++)
  {
    for(POSITION Atual = LTaludesAterro[Lado].GetHeadPosition() ; Atual != NULL ; LTaludesAterro[Lado].GetNext(Atual)) 
    {
      TaludeAterro& TalAtual(LTaludesAterro[Lado].GetAt(Atual));

      if(TalAtual.GetCount() < 2) return 1;
      {
        Ponto PontoAnterior(TalAtual.GetAt(TalAtual.GetHeadPosition()));

        if(PontoAnterior.x != 0.0 || PontoAnterior.y != 0.0) return 2;
      }
    }
  }

  return 0;
}