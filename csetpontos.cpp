//#include "stdafx.h"
//#include "afxtempl.h"
//#include "resource.h"
//#include "monologo.h"
#include <set>
#include <string>
//#include <iostream>
//#include <iomanip>
#include <fstream>

//using namespace std;

#include "cponto.h" 
#include "csetpontos.h"
#include "math.h"


#define INFINITO   1.7976931348623158e+308
#define ANTERIOR   0
#define POSTERIOR  1
#define MENOR     -1
#define MAIOR      1
#define IGUAL      0
#define TRAS      -1
#define FRENTE     1
#define X          0
#define Y          1
#define ESQUERDO   0
#define DIREITO    1
#define TODOS     -1

//extern class monologo monolog;

//----------------------------------------------------------------
//--- Implementa��o do n� da lista.
//--- Nesta lista o no � uma classe separada da lista pois pode ser criado separadamente.
//----------------------------------------------------------------------------------
//--- Implementa��o da lista
//--- Neste objeto s�o criadas duas listas na mem�ria, uma lista com os dados do 
//--- arquivo e outra lista com os dados calculados, como este objeto � gen�rico,
//--- as duas listas ficam na memoria e devem ser destru�das pelo recipiente 
//--- a medida do necess�rio. Estas lstas s�o dinamicas, podem ser mudadas
//--- durante a vida do objeto. Caso se deseje gravar as altera��es feitas
//--- deve-se chamar o metodo Serialize().
//-----------------------------------------------------------------------------------

CSetPontos::CSetPontos() : Atual(end()) {} 

void CSetPontos::InsereOrdenada(Ponto &No,int CALCULARRAMPAS)
{
  //--- Insere um n� ordenadamente na lista.
  //--- O ponteiro para o atual � a chave da insers�o, o metodo presume que o n�
  //--- a ser inserido est� pr�ximo do atual, a posic�o correta do n� �
  //--- procurada a partir do no atual. para tr�s ou para frente. 

  itSetPontos itAtual((insert(No)).first),  
              Posterior((itAtual)++),
              Anterior((itAtual)--);
        
  
  if(itAtual == end()) exit(1); //--- ???!!!
                                       
  if (Posterior != end()) CalculaRampa(itAtual);  
  if (Anterior!= end()) CalculaRampa(Anterior);
}

double CSetPontos::CalculaRampa(itSetPontos it)
{
  if (it == end()) return INFINITO;
  
  //--- Pega o pontoAtual
  
  Atual = it;
  Ponto &PontoAtual(*Atual);
  
  //--- Pega o ponto posterior
  
  Atual++ ;
  if (Atual == end()) return INFINITO;
  Ponto &ProximoPonto = *Atual;
  
  //--- Calcula as rampoas.
  
  PontoAtual.i = (ProximoPonto.y - PontoAtual.y) / (ProximoPonto.x - PontoAtual.x + 1e-10);

  if (Atual == end()) ProximoPonto.i = PontoAtual.i;    //--- Ultimo Ponto, iguala o i.

  return PontoAtual.i;
}

double CSetPontos::BuscaMenorRampa()
{ 
  double IMenor = INFINITO; 
  
  for(Atual == begin() ; Atual != end() ; Atual++)
  {
    if (Atual->i < IMenor) IMenor = Atual->i;
  }
   
  return IMenor;
}

CSetPontos& CSetPontos::operator=(CSetPontos &No)
{ 
  erase(begin(),end());
                                                                        
  (*this).operator=(No);
  
  Atual = find((*No.Atual));
                                                        
  return *this;
}

void CSetPontos::TrimLeft(double Afast)
{
  if (Atual != end() && !(Atual->x > Afast)) Atual = end();  //--- Se o atual for deletado anula-o
  
  Ponto p(Afast,0.0);
  itSetPontos pFim(PegaMenorIgual(p));
  
  erase(begin(),pFim);  
}

void CSetPontos::TrimRight(double Afast)
{
  if (Atual != end() && !(Atual->x < Afast)) Atual = end();  //--- Se o atual for deletado anula-o
  
  Ponto p(Afast,0.0);
  itSetPontos pIni(PegaMaiorIgual(p));
  
  erase(pIni,end());  
}
/*
void CSetPontos::InsereLista(CLPontos &ListaAInserir)
{
  insert(listaAInserir.begin(),ListaAInserir.end());
}
*/
void CSetPontos::CalculaIs()
{
  for(register itSetPontos it = begin() ; it != end() ; it = upper_bound(*it))
    CalculaRampa(it); 
}

void CSetPontos::RemoveTudo()
{
   erase(begin(),end());
   Atual = end();
}

bool CSetPontos::ExcluiPonto(double x)
{
  Ponto P(x,0.0);
  if (find(P) == Atual) Atual = end();
  
  return erase(P);
}

bool CSetPontos::ExcluiPonto(itSetPontos PontoAExcluir)
{
  if (PontoAExcluir == end()) PontoAExcluir = Atual;
  if (PontoAExcluir == end()) return false;
        
  if (PontoAExcluir == Atual) Atual = end();
        
  erase(PontoAExcluir);
  
  return true;
}

bool CSetPontos::CalculaIntersecao(Ponto& NoProjAnte,Ponto& NoProjPost,Ponto& NoTerrAnte,Ponto& NoTerrPost,Ponto& Intersecao)
{
  //--- Calcula o i e o B das retas de projeto de terreno

  double iProj = (NoProjPost.y - NoProjAnte.y) / (NoProjPost.x - NoProjAnte.x),
         iTerr = (NoTerrPost.y - NoTerrAnte.y) / (NoTerrPost.x - NoTerrAnte.x),

         BProj = NoProjPost.y - (iProj * NoProjPost.x),
         BTerr = NoTerrPost.y - (iTerr * NoTerrPost.x);

  // Retas paralelas, retorna false.

  if (fabs(iProj - iTerr) < 1e-10) 
  {
    Intersecao.x = Intersecao.y = INFINITO;
    return false;
  }

  //--- Calcula o x da interse��o pela f�rmula x = (B1 - B2) / (i2 - i1)
   
  double Xinte = (BProj - BTerr) / (iTerr - iProj);

  //--- Verifica se a interse��o est� dentro dos segmentos das retas dadas
  //--- Caso esteja atualiza a propriedade interse��o e retorna TRUE,
  //--- Caso contr�rio a interse��o n�o interessa, limpa a variavel interse��o 
  //--- e retorna FALSE.
  //---***************************
  //--- APRIMORAMENTO:
  //---***************************
  //--- Caso a interse��o n�o esteja dentro dos segmentos de retas dados
  //--- pode-se manter os valores de x e de y e retornar false, assim o
  //--- chamador saberia que a interse��o existe mas esta fora dos segmentos de
  //--- retas dados
  //*******************************************************************************
      
  double Erro = 1e-5;     

  //--- Erro admiss�vel no eixo x. No caso de pontos iguais a interse��o calculada
  //--- poder� ser apenas uns mil�simos diferente, por isso � necess�rio
  //--- considerar esta diferen�a, pois poderia retornar false onde deveria retornar true.

  if((Xinte+Erro >= NoProjAnte.x && Xinte-Erro <= NoProjPost.x) && 
     (Xinte+Erro >= NoTerrAnte.x && Xinte-Erro <= NoTerrPost.x))
  {
    //--- Achou!, ent�o calcula o y da interse��o.  

    double Yinte = Xinte * iProj + BProj; 

    Intersecao.x = Xinte;
    Intersecao.y = Yinte;

    return true;
  }
  else
  {
    //--- N�o achou!

    Intersecao.x = Intersecao.y = INFINITO;

    return false;
  }
}

itSetPontos CSetPontos::PosicionaPonto(double x,double y,double Toleranciax,double Toleranciay)
{
  if (!size()) return end();
  
  //--- Verifica se o <= esta dentro da tolerancia;
  
  itSetPontos it(PegaMenorIgual(Ponto(x,y)));
  
  if (it != end())
  {
    if ((*it).x + Toleranciax > x && (*it).x - Toleranciax < x &&
        (*it).y + Toleranciay > y && (*it).y - Toleranciay < y) return it;
  }
  else
  {
    //--- sen�o verifica se o > est� dentro da toler�ncia
    
    it = (PegaMaior(Ponto(x,y)));
    
    if (it != end())
    {
      if ((*it).x + Toleranciax > x && (*it).x - Toleranciax < x &&
          (*it).y + Toleranciay > y && (*it).y - Toleranciay < y) return it;
    } 
  }
}

void CSetPontos::AlteraPonto(Ponto& PNovo,itSetPontos Pos)  //--- Altera as coordenadas de um determinado ponto.
{
  if (Pos != end()) Atual = Pos;
  else Pos = Atual;
  
  if (Pos != end())
  {
    *Pos = PNovo;
    CalculaRampa(end());

    itSetPontos Anterior(PegaMenor(*Pos)),Proximo(PegaMaior(*Pos));
  
    //--- Recalcula as rampas.

    if (Anterior != end()) CalculaRampa(Anterior);
    if (Proximo != end()) CalculaRampa(Proximo);
  }
}

void CSetPontos::Serialize(fstream& ar)
{
  itSetPontos it(begin());
        
  if (/*ar.IsStoring()*/true)
  {
    ar << size();
          
    for (register itSetPontos it = begin() ; it != end() ; it++) *it >> ar;   
  }
  else
  {
    int i;
    Ponto P;
    
    for (ar >> i ; i ; i--)
    {  
      P << ar;
      insert(P);
    }
  }
}

int CSetPontos::PosicionaNoMaisProximo(double x)
{
   
  if (!size()) return MAIOR;
  
  //--- Posiciona o atual na estaca mais proxima

  itSetPontos itMaI(PegaMaiorIgual(CPonto(x))),         //--- Pega o maior ou igual
               itMeI(RascMaI ? (RascMaI)-- : end());         //--- Pega o anterior ao maior ou igual
  
  //--- Verifica qual esta mais perto o proximo ou o anterior
  
  double DistMaI(itMaI != end() ? (*itMaI).x - x : INFINITO),
         DistMei(itMeI != end() ? x - (*itMeI).x : INFINITO);
  
  Atual = DistMaI < DistMeI ? itMaI : itMeI;   ///--- atualiza o ponteiro para o atual
  
  if (Atual == end() || (*Atual).x < x) return MENOR;

  if ((*Atual).x > x) return MAIOR;

  //--- Achou igual
  
  return IGUAL;
}

double CSetPontos::Calculay(double x)
{
  if (size() < 2) return INFINITO;   //--- Tem que ter no m�nimo 2 pontos.

  int Res(PosicionaNoMaisProximo(x));   
  
  //---  se chegou no fim e n�o achou posiciona no pen�ltimo (para extrapolar)
    
  if (Atual == end()) Atual = (end()--)--; 
    
  Ponto P1(*Atual),P2(*(Atual++));

  return (P2.y - P1.y) / (P2.x - P1.x) * (x - P1.x) + P1.y;
}

void CSetPontos::AlteraRampas(double Rampa,int Lado,itSetPontos Pos)
{
  if (Pos!= end()) Atual = Pos;
  
  if (Atual == end()) return;
  
  itSetPontos PAnterior(PegaMenor(*Atual)),
              PPosterior(PegaMaior(*Atual));

  Ponto PAtual(*Atual);

  if ((Lado == ESQUERDO || Lado == TODOS) && PAnterior != end())
  {
    Ponto PAnte(*PAnterior);
    PAnte.y = (PAtual.x - PAnte.x) * Rampa + PAtual.y; 
    PAnte.i = Rampa;
    *PAnterior = PAnte;
  }
  if ((Lado == DIREITO || Lado == TODOS) && PAnterior != end())
  {
    Ponto PPost(*PPosterior);
    PPost.y = (PPost.x - PAtual.x) * Rampa + PAtual.y; 
    *PPosterior = PPost;
    PAtual.i = Rampa;
  }
}
