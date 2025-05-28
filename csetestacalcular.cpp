#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "math.h"
#include "estaca.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqcurhorizontais.h"
#include "carqsectipo.h"
#include "cponto.h"
#include "clpontos.h"
#include "csectipo.h"
#include "ccurhorizontais.h"
#include "carqcurverticais.h"
#include "ccurverticais.h"
#include "perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "cEstacasACalcular.h"
#include "mainfrm.h" 
#include "carqigualdades.h"
#include "igualdades.h"
#include <vector>
#include <iostream>
#include "projeto.h"
#include "csstack.h"

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
#define ESPIRAL   'E'
#define MENOR     -1
#define MAIOR      1
#define IGUAL      0
#define TRAS      -1
#define FRENTE     1
#define X          0
#define Y          1
#define NULO      -2
 
extern class monologo monolog;

//----------------------------------------------------------------------------------
//--- Implementação da lista
//--- Neste objeto são criadas duas listas na memória, uma lista com os dados do 
//--- arquivo e outra lista com os dados calculados, como este objeto é genérico,
//--- as duas listas ficam na memoria e devem ser destruídas pelo recipiente 
//--- a medida do necessário. Estas listas são dinâmicas, podem ser mudadas
//--- durante a vida do objeto. Caso se deseje gravar as alteraçòes feitas
//--- deve-se chamar o metodo Serialize().
//-----------------------------------------------------------------------------------

CEstacasACalcular::CEstacasACalcular(CCurHorizontais& CurHorizontais, CCurVerticais& CurVerticais) : Atual(NULL)
{
  //--- Preenche a lista com todas as estacas que devem ser calculadas.
        //--- a partir dos dados geométricos (Pontos Notáveis)

  //--- Pontos notáveis das curvas verticais.
      
  CurVerticais.PegaPontosNotaveis(PontosACalcular);

  //--- Pontos notáveis das curvas horizontais.

  CurHorizontais.PegaPontosNotaveis(PontosACalcular);

  //--- Insere na lista as estaca que devem ser calculadas a partir dos parameros de projeto,
  //--- curvas horizontais de 10 em 10, OAE de 5 em 5, tangentes de 20 em 20...

  CurVerticais.PegaEstacasObrigatorias(PontosACalcular,10.0);
  CurHorizontais.PegaEstacasObrigatorias(PontosACalcular,10.0);

  //--- Calculas as estacas obrigatórias nas tangentes.

  CMainFrame* MainFrame = (CMainFrame*)AfxGetMainWnd();
  MainFrame->m_ProjetosBox.GetLBText(0,MainFrame->m_sNomeComp);
  CProjeto* ProjAtual = MainFrame->AchaProjetoNoVetor();

        CEstaca Inicio(PontosACalcular.GetHead()),
                Fim(PontosACalcular.GetTail());

  double Equidis(20.0);   //--- Parametrizar.

  for (double Est = Inicio.EstVirtual ; Est < Fim.EstVirtual ; Est += Equidis)
  {
    int NumIg; 
    double Int,EstReal = ProjAtual->Igualdades.BuscaEstacaReal(Est,NumIg)+1e-5,
    EstInt = EstReal / Equidis;
    modf(EstInt,&Int);
    PontosACalcular.AddTail((CEstaca) (Int * Equidis));  
  }

  //--- Finalmente insere na lista de estacas a calcular as estacas pegas.

  POSITION PontoAtual = PontosACalcular.GetHeadPosition();
  while (PontoAtual) insert((PontosACalcular.GetNext(PontoAtual)));
}

int CEstacasACalcular::Compara(CEstaca &No1, CEstaca &No2)
{
  //--- Compara 2 nós da lista.

  if (No1.EstVirtual < No2.EstVirtual) return MENOR;
        else if (No1.EstVirtual > No2.EstVirtual) return MAIOR;
        else return IGUAL; 
}

int CEstacasACalcular::PosicionaNoMaisProximo(CEstaca& Estaca)
{
  if (!size()) return MAIOR;
  
  //--- Posiciona o atual na estaca mais proxima

  itsetEstacasACalcular itMaI(PegaMaiorIgual(CSecao(Estaca))),         //--- Pega o maior ou igual
                        itMeI(RascMaI ? (RascMaI)-- : end());         //--- Pega o anterior ao maior ou igual
  
  //--- Verifica qual esta mais perto o proximo ou o anterior
  
  double DistMaI(itMaI != end() ? (*itMaI).Estaca.EstVirtual - Estaca.EstVirtual : INFINITO),
         DistMei(itMeI != end() ? Estaca.EstVirtual - (*itMeI).Estaca.EstVirtual : INFINITO);
  
  Atual = DistMaI < DistMeI ? itMaI : itMeI;   ///--- atualiza o ponteiro para o atual. Ele passa a ser o mais próximo
 
  if (Atual == end() || (*Atual).Estaca < Estaca) return MENOR;

  if ((*Atual).Estaca > Estaca) return MAIOR;

  //--- Achou igual
  
  return IGUAL;
}
