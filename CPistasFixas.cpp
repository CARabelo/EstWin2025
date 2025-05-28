#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "cArqPistasFixas.h" 
#include "cponto.h"
#include "clpontos.h"
#include "perfil.h"
#include "cPistasFixas.h"
#include "math.h"

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
//--- Implementação da lista
//--- Neste objeto são criadas duas listas na memória, uma lista com os dados do 
//--- arquivo e outra lista com os dados calculados, como este objeto é genérico,
//--- as duas listas ficam na memoria e devem ser destruídas pelo recipiente 
//--- a medida do necessário. Estas listas são dinâmicas, podem ser mudadas
//--- durante a vida do objeto. Caso se deseje gravar as alteraçòes feitas
//--- deve-se chamar o metodo Serialize().
//-----------------------------------------------------------------------------------

PistasFixas::PistasFixas() {}

PistasFixas::PistasFixas(PistasFixas& PistasFix) { *this = PistasFix; }

const PistasFixas& PistasFixas::operator=(const PistasFixas &No)
{
  EstFinal = No.EstFinal;
  CLPontos::operator=(No);

  return*this;
}

CPistasFixas::CPistasFixas(CString& pNomeArquivo) : CArqPistasFixas(pNomeArquivo), LPistasFixas(), Atual(NULL), NomeArquivo(pNomeArquivo), Erro(0)
{
  Consiste();

  CEstaca EstacaAtual;
  double EstacaAnterior(INFINITO);
  POSITION At(GetHeadPosition());

  if (Erro == 0)
  {
    //--- Para cada os nos  da lista de Secoes lidas cria um nó na lista

    while (At)
    {
      ArqPistasFixas &NoArqAtual(GetNext(At));

      ittlstwstr ItCampoAtual(NoArqAtual.LstCampos.begin());

      CString Estaca(ItCampoAtual->c_str());
      ItCampoAtual++;
      CString Ig(ItCampoAtual->c_str());

      PistasFixas PistasFixasAtual;
      PistasFixasAtual.EstFinal.Constroi(Estaca, "", Ig);

      for (ItCampoAtual++; ItCampoAtual != NoArqAtual.LstCampos.end(); ItCampoAtual++)
      {
        SuperCString Coordx(ItCampoAtual->c_str());
        ItCampoAtual++;
        SuperCString Coordy(ItCampoAtual->c_str());

        if (Coordx.ENumero() == 1 && Coordy.ENumero())
        {
          Ponto PontoAtual(atof(Coordx), atof(Coordy), INFINITO, INFINITO, INFINITO, INFINITO, CString("SECTIP"));
          PistasFixasAtual.InsereOrdenada(PontoAtual);
        }
      }

      PistasFixasAtual.CalculaIs();
      InsOrdenada(PistasFixasAtual);
    }
  }
  Atual = LPistasFixas.GetHeadPosition();
}

int CPistasFixas::Compara(class PistasFixas &No1, class PistasFixas &No2)
{
  //--- Compara 2 nós da lista.

  if (No1.EstFinal.EstVirtual < No2.EstFinal.EstVirtual) return MENOR;
  else if (No1.EstFinal.EstVirtual > No2.EstFinal.EstVirtual) return MAIOR;
  else return IGUAL;
}

void CPistasFixas::InsOrdenada(class PistasFixas &No)
{
  //--- Insere um nó ordenadamente na lista.
  //--- O ponteiro para o atual é a chave da insersão, o metodo presume que o nó
  //--- a ser inserido está próximo do atual, a posicão correta do nó é
  //--- procurada a partir do no atual. para trás ou para frente. 

  Atual = LPistasFixas.GetHeadPosition();

  if (!Atual)
  {
    LPistasFixas.AddHead(No);  //--- Se a lista esta vazia , insere na cabeça.

    Atual = LPistasFixas.GetHeadPosition();
  }
  else
  {
    int Res;

    Atual = LPistasFixas.GetHeadPosition();

    //--- Se o no atual for maior que o a inserir procura para trás 
    //--- Senão se o no atual for menor que o a inserir procura para a frente
    //--- Senão se o nó atual for igual ao no a inserir insere o nó  depois do atual

    if ((Res = Compara(LPistasFixas.GetAt(Atual), No)) == MAIOR)
    {
      LPistasFixas.GetPrev(Atual);

      while (Atual && Compara(LPistasFixas.GetPrev(Atual), No) == MAIOR);

      if (!Atual) LPistasFixas.AddHead(No);
      else LPistasFixas.InsertAfter(Atual, No);
    }
    else
    {
      if (Res == MENOR)
      {
        LPistasFixas.GetNext(Atual);

        while (Atual && Compara(LPistasFixas.GetNext(Atual), No) == MENOR);

        if (!Atual) LPistasFixas.AddTail(No);
        else LPistasFixas.InsertBefore(Atual, No);

      }
      else LPistasFixas.InsertAfter(Atual, No);
    }
  }
}

int CPistasFixas::PosicionaNoMaisProximo(const CEstaca& Estaca, int Direcao)
{
  Atual = NULL;

  if (!Atual) Atual = LPistasFixas.GetHeadPosition();
  if (!Atual) return MENOR;

  // if (!(Direcao = Estaca > LPistasFixas.GetAt(Atual).EstFinal ? FRENTE : Estaca < LPistasFixas.GetAt(Atual).EstFinal ? FRENTE : FALSE)) return IGUAL;

  Direcao = FRENTE;

  if (Direcao == FRENTE)
  {
    while (Atual && LPistasFixas.GetAt(Atual).EstFinal.EstVirtual < Estaca.EstVirtual)
    {
      LPistasFixas.GetNext(Atual);
    }

    if (!Atual) Atual = LPistasFixas.GetTailPosition();
  }
  else
  {
    LPistasFixas.GetPrev(Atual);

    while (Atual && LPistasFixas.GetAt(Atual).EstFinal > Estaca) LPistasFixas.GetNext(Atual);

    if (!Atual) Atual = LPistasFixas.GetHeadPosition();
  }

  if (!Atual || LPistasFixas.GetAt(Atual).EstFinal < Estaca) return MENOR;
  if (LPistasFixas.GetAt(Atual).EstFinal > Estaca) return MAIOR;
  return IGUAL;
}

bool CPistasFixas::VerfPistaFixas(const CEstaca& Estaca,double x,int& Ini_Fim)
{
  if (PosicionaNoMaisProximo(Estaca) == MENOR) return FALSE;
  PistasFixas PistaABuscar = LPistasFixas.GetAt(Atual);
  int Achou(false);
  Ponto* PAtual = NULL;

  for(POSITION Atual = PistaABuscar.GetHeadPosition() ; Atual && !Achou ; PistaABuscar.GetNext(Atual))
  { 
    PAtual = &PistaABuscar.GetAt(Atual);

    if ((x - PAtual->x) > -1E-5 && (x - PAtual->y) < 1E-5) Achou = true;
  }

  if(Achou)
  {
    if(fabs(PAtual->x - x) < 1e-5) Ini_Fim = INICIO;
    else if(fabs(PAtual->y - x) < 1e-5) Ini_Fim = FIM;
    else Ini_Fim = MEIO;

    return true;
  }

  return false;
}


bool CPistasFixas::Consiste()
{
  POSITION Atual = GetHeadPosition();
  int i(0);

  if (!Atual) return true;

  while (Atual && Erro == 0)
  {
    i++;
    Erro = GetNext(Atual).Consiste();

    if (Erro)
    {
      CString Mensg;

      Mensg.Format("Erro no arquivo de pistas fixas: o registro %d está invalido: ",i);
      Mensg += monolog.ErrosCurHor[Erro-1];

      monolog.mensagem(15,Mensg);
    }
  }
  return Erro > 0;
}