#include "stdafx.h"
#include "igualdades.h"
#include "math.h"
#include "Estaca.h"
#include <string>
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

NoIgual::NoIgual(double Est1, double Dif)
{
  //--- Inicializa as propriedades do nó.

  Proximo = Anterior = NULL;

  Estaca1 = new CEstaca(Est1);
  Diferenca = Dif;
}

CIgualdades::CIgualdades()
{
  Primeiro = Ultimo = Atual = NULL;    
  Nitens = 0;
}

void CIgualdades::Criar(CString Nome_Projeto)
{
  //--- 1- Inicializa as propriedades das igualdades.
  //--- 2- Cria a lista e a preenche com todas as igualdades, caso não haja 
  //---    igualdades a lista ficará vazia.

  CEstaca Estaca1,Estaca2;
  CFileException CFe;
  CFile ArqIgual;
  ArqIgual.Open(Nome_Projeto + ".IGU",CFile::modeRead,&CFe);
  if (!CFe.m_cause)
  {
    struct igu
    {
      char           est1[12],
        est2[12];
      unsigned char  erros[2];
    }Igualdade;  

    int Tam = sizeof (struct igu);
    double DifTotal= 0.0,Dif;

    while (ArqIgual.Read(&Igualdade,Tam)) //--- Le uma igualdade.
    {
      Estaca1.Constroi(CString(Igualdade.est1),CString(""));
      Estaca2.Constroi(CString(Igualdade.est2),CString(""));
      Estaca1.EstVirtual += DifTotal;
      Estaca2.EstVirtual += DifTotal;
      Dif = Estaca1.EstVirtual - Estaca2.EstVirtual;
      DifTotal += Dif;
      Estaca1.EstVirtual += DifTotal;
      Estaca2.EstVirtual += DifTotal;
      Inclui(Estaca1.EstVirtual,Dif);   //--- Inclui a igualdade na lista.
    }
    ArqIgual.Close();
  }
}

CIgualdades::~CIgualdades()
{
  class NoIgual *Rasc,*Atual = Primeiro;

  //--- Exclui a lista de igualdades.

  while(Atual)
  {
    Rasc = Atual->Proximo;
    delete Atual;
    Atual = Rasc;
  }
}

void CIgualdades::Inclui(double Est1, double Est2,class NoIgual *Anterior)
{
  //--- Inclui um no na lista depois de <Anterior>, caso Anterior venha nulo
  //--- inclui o no criado no final da lista.

  class NoIgual *Rasc = new NoIgual(Est1,Est2);

  if (!Primeiro) Primeiro = Rasc;  //--- Se não tem primeiro, este é o primeiro.

  if (Anterior)  //--- Se veio algum ponto, unclui o ponto recem criado depois dele. 
  {
    Anterior->Proximo = Rasc;
    Rasc->Anterior = Anterior;
  }
  else   //--- Se não veio nenhum ponto....
  {
    //--- Se existe ultimo ponto na lista, insere o ponto recém criado depois dele, 
    //--- ou seja, insere o ponto criado no fim da lista.

    if (Ultimo)
    {
      Ultimo->Proximo = Rasc;
      Rasc->Anterior = Ultimo;
    }
  }

  Ultimo = Rasc;   //--- Atualiza o último ponto da lista (= ponto recém criado)
  Nitens++;        //--- Iincrementa i múmero de pontos da lista. 
}


void CIgualdades::Exclui(class NoIgual* NoaExcluir)
{
  //--- Exclui uma igualdade da lista.

  if (!NoaExcluir) return;

  if (NoaExcluir == Primeiro) Primeiro = Primeiro->Proximo;
  if (NoaExcluir == Ultimo) Ultimo = Ultimo->Anterior;
  if (NoaExcluir == Atual)
  {
    if (Atual->Proximo) Atual = Atual->Proximo;
    else Atual = Atual->Anterior;
  }
  if (NoaExcluir->Anterior) NoaExcluir->Anterior->Proximo = NoaExcluir->Proximo;
  if (NoaExcluir->Proximo) NoaExcluir->Proximo->Anterior = NoaExcluir->Anterior;

  delete NoaExcluir;
  Nitens--;
}

BOOL CIgualdades::Exclui(class NoIgual* Inicio,class NoIgual* Fim)
{
  //--- exclui uma faixa de igualdaes da lista (De inicio até Fim) se
  //--- fim não for alcancado exclui de inicio até o fim da lista

  register class NoIgual *Rasc, *Rasc2;

  if (!Inicio) return FALSE;

  for(Rasc = Inicio ; Rasc && Rasc != Fim ; Rasc = Rasc->Proximo);

  if (!Rasc) return FALSE;

  Rasc = Inicio;
  do
  {
    Rasc2 = Rasc->Proximo;
    Exclui(Rasc);
    Rasc = Rasc2;
  }while(Rasc != Rasc2);

  return TRUE;
}
class NoIgual* CIgualdades::operator++()   //--- Incrementa a lista.
{
  if (Atual) Atual = Atual->Proximo;
  return Atual;
}

class NoIgual *CIgualdades::operator--()  //--- Descrementa a lista.
{
  if (Atual) Atual = Atual->Anterior;
  return Atual;
}

double CIgualdades::BuscaDifIgualdade(int Numero_Igualdade)  //--- Busca a soma das diferencas de igualdade
{
  double Diferenca_Total = 0;

  for (class NoIgual *Atual = Primeiro ; Atual ; Atual = Atual->Proximo)
    Diferenca_Total += Atual->Diferenca;

  return Diferenca_Total;
}

double CIgualdades::BuscaEstacaReal(double EstacaVirtual,int &NumIg)
{
  if (!Primeiro) return  EstacaVirtual;

  class NoIgual *Atual = Primeiro;
  double DifTotal = 0.0;
  NumIg = 0;

  while(Atual && Atual->Estaca1->EstVirtual < EstacaVirtual)
  {
    DifTotal += Atual->Diferenca;
    Atual = Atual->Proximo;
    NumIg++;
  }
  return EstacaVirtual - DifTotal;
}