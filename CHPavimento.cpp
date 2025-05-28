#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqhpavimento.h" 
#include "cponto.h"
#include "clpontos.h"
#include "chpavimento.h"

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
//--- Implementa��o do n� da lista.

const HPavimento &HPavimento::operator=(const HPavimento &No)
{
  EstFinal = No.EstFinal;
  h = No.h;
  Exesso = No.Exesso;

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

CHPavimento::CHPavimento(CString& NomeArquivo,CString& ext) : CArqHPavimento(NomeArquivo,ext),LHPavimento(),Ext(ext),Atual(NULL)
{
  Ext.MakeUpper();

  if(Consiste() == 0)
  {   
    //--- Para cada um dos nos  da lista de Gradientes lidos cria um n� na lista
    //--- de Gradientes double.

    POSITION At(GetHeadPosition());

    while (At)
    {
      ArqHPavimento NoArqAtual(GetNext(At));

      if(((Ext == ".PAV" || Ext == ".EMP") && (NoArqAtual.ListaCampos.size() == 3) ||  (Ext == ".LIM" && (NoArqAtual.ListaCampos.size() == 4))))   
      {
        HPavimento PaviAtual;

        ittlstwstr It(NoArqAtual.ListaCampos.begin());
        std::string Estaca(*It);
        It++; 

        PaviAtual.EstFinal.Constroi(Estaca.c_str(),"",It->c_str());
        It++;
        PaviAtual.h = atof(It->c_str());

        if(Ext == ".LIM")
        {
          It++;
          PaviAtual.Exesso = atof(It->c_str());
        }

        InsereOrdenada(PaviAtual);
      }
      else 
      {
        AfxMessageBox("erro 4329-8 - contacte o suporte",MB_ICONERROR);
      }
    }									 
    RemoveAll();  //--- Nao Ser� Mais Usada
  }
  Atual = LHPavimento.GetHeadPosition(); 
}

int CHPavimento::Compara(class HPavimento &No1,class HPavimento &No2)
{
  //--- Compara 2 n�s da lista.

  if (No1.EstFinal.EstVirtual < No2.EstFinal.EstVirtual) return MENOR;
  else if (No1.EstFinal.EstVirtual > No2.EstFinal.EstVirtual) return MAIOR;
  else return IGUAL; 
}

void CHPavimento::InsereOrdenada(class HPavimento &No,int INSERIRIGUAL)
{
  //--- Insere um n� ordenadamente na lista.
  //--- O ponteiro para o atual � a chave da insers�o, o metodo presume que o n�
  //--- a ser inserido est� pr�ximo do atual, a posic�o correta do n� �
  //--- procurada a partir do no atual. para tr�s ou para frente. 

  if (!Atual) Atual = LHPavimento.GetHeadPosition();
  if (!Atual) LHPavimento.AddHead(No);  //--- Se a lista esta vazia , insere na cabe�a.
  else
  {
    int Res;

    //--- Se o no atual for maior que o a inserir procura para tr�s 
    //--- Sen�o se o no atual for menor que o a inserir procura para a frente
    //--- Sen�o se o n� atual for igual ao no a inserir  e for para inserir,
    //--- insere o n� depois do atual.

    if ((Res = Compara(LHPavimento.GetAt(Atual),No)) == MAIOR)
    {
      LHPavimento.GetPrev(Atual); 

      while (Atual && (Res = Compara(LHPavimento.GetAt(Atual),No)) == MAIOR) LHPavimento.GetPrev(Atual);

      if (Res == IGUAL && ! INSERIRIGUAL) return;

      if (!Atual)
      {
        LHPavimento.AddHead(No);
        Atual = LHPavimento.GetHeadPosition();
      }
      else LHPavimento.InsertAfter(Atual,No);
    }
    else 
    {
      if (Res == MENOR)
      {
        while (Atual && (Res = Compara(LHPavimento.GetAt(Atual),No)) == MENOR) LHPavimento.GetNext(Atual);

        if (Res == IGUAL && !INSERIRIGUAL) return;

        if (!Atual) 
        {
          LHPavimento.AddTail(No);
          Atual = LHPavimento.GetTailPosition();
        }
        else LHPavimento.InsertBefore(Atual,No);
      }
      else if (INSERIRIGUAL) LHPavimento.InsertAfter(Atual,No);
    }
  }
}

int CHPavimento::PosicionaNoMaisProximo(const CEstaca& Estaca,int Direcao)
{
  Atual = LHPavimento.GetHeadPosition();
  if (!Atual) return MENOR;

  //if (!(Direcao = Estaca > LHPavimento.GetAt(Atual).EstFinal ? TRAS : Estaca < LHPavimento.GetAt(Atual).EstFinal ? FRENTE : FALSE)) return IGUAL;
  Direcao = FRENTE;

  if (Direcao == FRENTE)
  {
    while(Atual && LHPavimento.GetAt(Atual).EstFinal < Estaca) LHPavimento.GetNext(Atual);

    if (!Atual) Atual = LHPavimento.GetTailPosition();
  }
  else
  {
    LHPavimento.GetPrev(Atual);

    while(Atual && LHPavimento.GetAt(Atual).EstFinal > Estaca) LHPavimento.GetNext(Atual);

    if (!Atual) Atual = LHPavimento.GetHeadPosition();
  }

  if (LHPavimento.GetAt(Atual).EstFinal > Estaca) return MAIOR;
  if (LHPavimento.GetAt(Atual).EstFinal < Estaca) return MENOR;

  return IGUAL;
}

double CHPavimento::BuscaH(const CEstaca& Estaca,double* pExesso)
{
  if(pExesso) *pExesso = 0.0;

  if (PosicionaNoMaisProximo(Estaca) == MENOR) return 0.0;

  if(pExesso) *pExesso = LHPavimento.GetAt(Atual).Exesso;

  return LHPavimento.GetAt(Atual).h;
}

bool CHPavimento::Consiste()
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

      Mensg.Format("Erro no arquivo de alturas do pavimento: o registro %d est� invalido: ",i);
      Mensg += monolog.ErrosHPavim[Erro-1];

      monolog.mensagem(15,Mensg);
    }
  }
  return Erro > 0;
}
