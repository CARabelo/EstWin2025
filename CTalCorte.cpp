#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqTalcorte.h" 
#include "cponto.h"
#include "clpontos.h"
#include "perfil.h"
#include "cTalCorte.h"

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

TaludeCorte::TaludeCorte() {}   //--- construtor default;

TaludeCorte::TaludeCorte(TaludeCorte &Alarg)  //--- Construtor de Cópia.
{
  *this = Alarg;
}

TaludeCorte& TaludeCorte::operator=(TaludeCorte &No)
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

CTaludesCorte::CTaludesCorte(CString& NomeArq): CArqTalCorte(NomeArq)
{
  Atual[ESQUERDO] = Atual[DIREITO] = NULL;
  POSITION At(GetHeadPosition());

  //--- Para cada os nos  da lista de Curvas lidas cria um nó na lista

  while (At)
  {
    ArqTalCorte &NoArqAtual(GetNext(At));
    ittlstwstr It(NoArqAtual.ListaCampos.begin());

    if(NoArqAtual.ListaCampos.size() > 5)
    {
      TaludeCorte TaludesCorteAtual;

      CString Estaca(It->c_str());
      It++;

      TaludesCorteAtual.EstFinal.Constroi(Estaca,*It->c_str());
      It++;
      TaludesCorteAtual.Lado = It->c_str();

      for (It++ ; It != NoArqAtual.ListaCampos.end() ; It++)
      {
        SuperCString Coordx(It->c_str());

        It++;

        if(It != NoArqAtual.ListaCampos.end())
        {
           SuperCString Coordy(It->c_str());

           if(Coordx.ENumero() && Coordy.ENumero())
           {
             Ponto PontoAtual(atof(Coordx),atof(Coordy),INFINITO,INFINITO,INFINITO,INFINITO,CString(TaludesCorteAtual.Lado.CompareNoCase("E") == 0 ? "TALCTE" : "TALCTD"));

             TaludesCorteAtual.AddTail(PontoAtual);
           }
        }
      }
      InsOrdenada(TaludesCorteAtual);
    }
  }									 
  Atual[ESQUERDO] = LTaludesCorte[ESQUERDO].GetHeadPosition();
  Atual[DIREITO] = LTaludesCorte[DIREITO].GetHeadPosition();
}

int CTaludesCorte::Compara(TaludeCorte &No1,TaludeCorte &No2)
{
  //--- Compara 2 nós da lista.

  if (No1.EstFinal.EstVirtual < No2.EstFinal.EstVirtual) return MENOR;
  else if (No1.EstFinal.EstVirtual > No2.EstFinal.EstVirtual) return MAIOR;
  else return IGUAL; 
}

void CTaludesCorte::InsOrdenada(TaludeCorte &No,int INSERIRIGUAL)
{
  //--- Insere um nó ordenadamente na lista.
  //--- O ponteiro para o atual é a chave da insersão, o metodo presume que o nó
  //--- a ser inserido está próximo do atual, a posicão correta do nó é
  //--- procurada a partir do no atual. para trás ou para frente. 

  No.Lado.MakeUpper();

  int Lado = No.Lado == "E" ? ESQUERDO : DIREITO;  

  if (!Atual[Lado]) Atual[Lado] = LTaludesCorte[Lado].GetHeadPosition();  //--- Se atual esta nulo volta para o inicio da lista

  if (!Atual[Lado]) LTaludesCorte[Lado].AddHead(No);  //--- Se a lista esta vazia , insere na cabeça.
  else
  {
    int Res;

    //--- Se o no atual for maior que o a inserir procura para trás 
    //--- Senão se o no atual for menor que o a inserir procura para a frente
    //--- Senão se o nó atual for igual ao no a inserir insere o nó  depois do atual

    if ((Res = Compara(LTaludesCorte[Lado].GetAt(Atual[Lado]),No)) == MAIOR)
    {
      LTaludesCorte[Lado].GetPrev(Atual[Lado]); 

      while (Atual[Lado] && Compara(LTaludesCorte[Lado].GetPrev(Atual[Lado]),No) == MAIOR);

      if (!Atual[Lado]) 
      {
        LTaludesCorte[Lado].AddHead(No);   //-- Se voltou a lista toda, adiciona na cabeça.
        Atual[Lado] = LTaludesCorte[Lado].GetHeadPosition();
      }
      else LTaludesCorte[Lado].InsertAfter(Atual[Lado],No);
    }
    else 
    {
      if (Res == MENOR)
      {
        LTaludesCorte[Lado].GetNext(Atual[Lado]); 

        while (Atual[Lado] && Compara(LTaludesCorte[Lado].GetNext(Atual[Lado]),No) == MENOR);

        if (!Atual[Lado]) 
        {
          LTaludesCorte[Lado].AddTail(No);	//--- Se passou a lista toda adiciona no fim.
          Atual[Lado] = LTaludesCorte[Lado].GetTailPosition();
        }
        else LTaludesCorte[Lado].InsertBefore(Atual[Lado],No);

      }
      else if (INSERIRIGUAL) LTaludesCorte[Lado].InsertAfter(Atual[Lado],No);
    }
  }
}

int CTaludesCorte::PosicionaNoMaisProximo(CEstaca Estaca,int Lado,int Direcao)
{
  Atual[Lado] = LTaludesCorte[Lado].GetHeadPosition();

  if (!Atual[Lado]) return MENOR;   //-- Menor = não achou, lista vazia

  CEstaca Esta = LTaludesCorte[Lado].GetAt(Atual[Lado]).EstFinal;

  while(Atual[Lado] && LTaludesCorte[Lado].GetAt(Atual[Lado]).EstFinal < Estaca) 
  {
    LTaludesCorte[Lado].GetNext(Atual[Lado]);
    if(Atual[Lado]) Esta = LTaludesCorte[Lado].GetAt(Atual[Lado]).EstFinal;
  }

  if (!Atual[Lado]) return MENOR;  
  if (LTaludesCorte[Lado].GetAt(Atual[Lado]).EstFinal > Estaca) return MAIOR;

  return IGUAL;

/*

  if (!Atual[Lado]) Atual[Lado] = LTaludesCorte[Lado].GetHeadPosition();
  if (!Atual[Lado]) return MAIOR;

  if (!(Direcao = Estaca > LTaludesCorte[Lado].GetAt(Atual[Lado]).EstFinal ? TRAS : Estaca < LTaludesCorte[Lado].GetAt(Atual[Lado]).EstFinal ? FRENTE : FALSE)) return IGUAL;

  if (Direcao == FRENTE)
  {
    LTaludesCorte[Lado].GetNext(Atual[Lado]);

    while(Atual[Lado] && LTaludesCorte[Lado].GetAt(Atual[Lado]).EstFinal < Estaca) LTaludesCorte[Lado].GetNext(Atual[Lado]);

    if (!Atual[Lado]) Atual[Lado] = LTaludesCorte[Lado].GetTailPosition();
  }
  else
  {
    LTaludesCorte[Lado].GetPrev(Atual[Lado]);

    while(Atual[Lado] && LTaludesCorte[Lado].GetAt(Atual[Lado]).EstFinal > Estaca) LTaludesCorte[Lado].GetNext(Atual[Lado]);

    if (!Atual[Lado]) Atual[Lado] = LTaludesCorte[Lado].GetHeadPosition();
  }

  if (LTaludesCorte[Lado].GetAt(Atual[Lado]).EstFinal > Estaca) return MAIOR;
  if (LTaludesCorte[Lado].GetAt(Atual[Lado]).EstFinal < Estaca) return MENOR;
  return IGUAL;
  */

}

BOOL CTaludesCorte::BuscaTalude(CEstaca &Estaca,int Lado,CLPontos &PontosAlarg)
{
  if(PosicionaNoMaisProximo(Estaca.EstVirtual,Lado) == MENOR) return FALSE;

  if (!Atual[Lado]) return false;

  PontosAlarg = LTaludesCorte[Lado].GetAt(Atual[Lado]);

  return TRUE;
}

void CTaludesCorte::TaludesDefault()
{
  //--- Caso não sejam encontrados os arquivos dos taludas , coloca estes taludes como o padrão

  if (LTaludesCorte[ESQUERDO].IsEmpty())
  {
    TaludeCorte TaludePadrao;
    TaludePadrao.EstFinal.Constroi(999999);
    TaludePadrao.Lado = "E";
    TaludePadrao.AddTail(Ponto(0.0,0.0,0.0,INFINITO,INFINITO,INFINITO,CString("TALCOE")));
    TaludePadrao.AddTail(Ponto(-1000.0,1000.0,0.0,INFINITO,INFINITO,INFINITO,CString("TALCOE")));
    LTaludesCorte[ESQUERDO].AddTail(TaludePadrao);
  }
  if (LTaludesCorte[DIREITO].IsEmpty())
  {
    TaludeCorte TaludePadrao;
    TaludePadrao.EstFinal.Constroi(999999);
    TaludePadrao.Lado = "D";
    TaludePadrao.AddTail(Ponto(0.0,0.0,0.0,INFINITO,INFINITO,INFINITO,CString("TALCOD")));
    TaludePadrao.AddTail(Ponto(1000.0,1000.0,0.0,INFINITO,INFINITO,INFINITO,CString("TALCOD")));
    LTaludesCorte[DIREITO].AddTail(TaludePadrao);
  }
}


bool CTaludesCorte::Consiste()   //--- Consiste digitacao
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

      Mensg.Format("Erro no arquivo de obras de taludes de corte: o registro %d está invalido: ",i);
      Mensg += monolog.ErrosCurHor[Erro-1];

      monolog.mensagem(15,Mensg);
    }
  }
  return Erro > 0;
}

int CTaludesCorte::ConsisteDados()   //--- consistes os pontos do talude
{
   if(LTaludesCorte[ESQUERDO].GetCount() == 0 || LTaludesCorte[DIREITO].GetCount() == 0) return 1;

  for(int Lado = ESQUERDO ; Lado < DIREITO ;  Lado++)
  {
    for(POSITION Atual = LTaludesCorte[Lado].GetHeadPosition() ; Atual != NULL ; LTaludesCorte[Lado].GetNext(Atual))
    {
      TaludeCorte& TalAtual(LTaludesCorte[Lado].GetAt(Atual));

      if(TalAtual.GetCount() < 2) return 1;
      {
        Ponto PontoAnterior(TalAtual.GetAt(TalAtual.GetHeadPosition()));

        if(PontoAnterior.x != 0.0 || PontoAnterior.y != 0.0) return 2;
      }
    }
  }

  return 0;
}