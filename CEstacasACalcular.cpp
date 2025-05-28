#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "math.h"
#include "estaca.h"
#include "monologo.h"
#include "supercstring.h"
#include <string>
#include "cponto.h"
#include "Mat.h"
#include "carqcurhorizontais.h"
#include "clpontos.h"
#include "perfil.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include <set> 
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include <map>
#include "ccurhorizontal.h"
#include "ccurhorizontais.h"
#include <vector>
#include <iostream>
#include "carqcurverticais.h"
#include "ccurverticais.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "cEstacasACalcular.h"
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h" 
#include "csstack.h"
#include "carqoae.h"
#include "coae.h"
#include "carqoac.h"
#include "coac.h"
#include <string>
#include <winnetwk.h>
#include "cconfigproj.h"

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

//----------------------------------------------------------------------------------
//--- Implementação da lista
//--- Neste objeto são criadas duas listas na memória, uma lista com os dados do 
//--- arquivo e outra lista com os dados calculados, como este objeto é genérico,
//--- as duas listas ficam na memoria e devem ser destruídas pelo recipiente 
//--- a medida do necessário. Estas listas são dinâmicas, podem ser mudadas
//--- durante a vida do objeto. Caso se deseje gravar as alteraçòes feitas
//--- deve-se chamar o metodo Serialize().
//-----------------------------------------------------------------------------------

CEstacasACalcular::CEstacasACalcular(CCurHorizontais* CurHorizontais,CCurVerticais* CurVerticais,const CString& NomeProj,CProjeto* CProj) : Atual(NULL),Projeto(CProj)
{
  //--- Preenche a lista com todas as estacas que devem ser calculadas.
  //--- a partir dos dados geométricos (Pontos Notáveis)

  char ProjBase[1024] = { 0 };

  CConfigProj ConfigProjeto(NULL, ((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase(ProjBase));
  ConfigProjeto.LeArquivo();

  //--- Pontos notáveis das curvas verticais.

  CList <CEstaca, CEstaca&> PontosACalcular;
  CurVerticais->PegaPontosNotaveis(PontosACalcular);

  //--- Pontos notáveis das curvas horizontais.

  CurHorizontais->PegaPontosNotaveis(PontosACalcular);

  //--- Obras de arte Especiais

  COAE OAEs(NomeProj);
 
  //--- Obras de arte correntes

  COAC OACs(NomeProj); 
  OACs.PegaEstacasObrigatorias(PontosACalcular);

  //--- Insere na lista as estaca que devem ser calculadas a partir dos parameros de projeto,
  //--- curvas horizontais de 10 em 10, OAE de 5 em 5, tangentes de 20 em 20...
  
  CurVerticais->PegaEstacasObrigatorias(PontosACalcular,ConfigProjeto.EquiParabola/*,ProjAtual*/);
  CurHorizontais->PegaEstacasObrigatorias(PontosACalcular,ConfigProjeto.EquiCurHoriz,Projeto);
  OAEs.PegaEstacasObrigatorias(PontosACalcular,ConfigProjeto.EquiOAE);
  OACs.PegaEstacasObrigatorias(PontosACalcular);

  //RemoveEstacasForaGeom(CurHorizontais, PontosACalcular);

  //--- Calculas as estacas obrigatórias nas tangentes.

   CEstaca Inicio(INFINITO),Fim(-INFINITO);

   //--- O trecho é definido pelo projeto geométrico

   /*

  if (CurVerticais->LCurvasVerticais.GetCount() > 1)
  {
    Inicio = CurVerticais->LCurvasVerticais.GetHead().PIV.EstVirtual;
    Fim = CurVerticais->LCurvasVerticais.GetTail().PIV.EstVirtual;
  }
  */

  Inicio =CurHorizontais->PegaPrimeiroTs();
  Fim = CurHorizontais->PegaUltimoSt();

  //--- Pega as estacas na tangente

  double Equidis(ConfigProjeto.EquiTangente);

  if(Inicio.EstVirtual != INFINITO && Inicio.EstVirtual != -INFINITO && Fim.EstVirtual != INFINITO  && Fim.EstVirtual != -INFINITO)
  {
    for (double Est = Inicio.EstVirtual ; Est <= Fim.EstVirtual ; Est += ConfigProjeto.EquiTangente)
    {
      double Int,EstReal(Est),EstInt(EstReal / ConfigProjeto.EquiTangente);

      modf(EstInt,&Int);
      PontosACalcular.AddTail((CEstaca) (Int * ConfigProjeto.EquiTangente));  
    }
  }

  //--- Finalmente insere na lista de estacas a calcular as estacas pegas.

  CEstaca EstacaAtual;
  POSITION PontoAtual(PontosACalcular.GetHeadPosition());
  while (PontoAtual)
  {
    EstacaAtual = PontosACalcular.GetNext(PontoAtual);

    if(!(EstacaAtual.EstVirtual > Fim.EstVirtual))
      InsOrdenada(EstacaAtual);
  }

  for (POSITION Atual = OACs.LOAC.GetHeadPosition() ; Atual ; OACs.LOAC.GetNext(Atual))
  {
    CEstaca& Est(OACs.LOAC.GetAt(Atual).Estaca);
    if(PosicionaNoMaisProximo(Est) == IGUAL) 
    {
      Est.Cota = GetAt(Atual).Cota;
    }
    else Est.Cota = INFINITO;
  }
  if(OACs.LOAC.GetCount() > 0) OACs.GravarArquivo();
}

int CEstacasACalcular::Compara(CEstaca &No1, CEstaca &No2)
{
  //--- Compara 2 nós da lista.

  double Dx(No1.EstVirtual - No2.EstVirtual);

  if (Dx < -5e-3) return MENOR;
  else if (Dx > 5e-3) return MAIOR;
       else return IGUAL; 
}

void CEstacasACalcular::InsOrdenada(CEstaca &No, int INSERIRIGUAL)
{
  //--- Insere um nó ordenadamente na lista.
  //--- O ponteiro para o atual é a chave da insersão, o metodo presume que o nó
  //--- a ser inserido está próximo do atual, a posicão correta do nó é
  //--- procurada a partir do no atual. para trás ou para frente. 

  if (No == CEstaca("") || No.EstVirtual == INFINITO) return;    //--- isto jamais poderia estar acontecendo... mas está, verificar

  if (!Atual) Atual = GetHeadPosition();
  if (!Atual) AddHead(No);  //--- Se a lista esta vazia , insere na cabeça.
  else
  {
    int Res;

    //--- Se o no atual for maior que o a inserir procura para trás 
    //--- Senão se o no atual for menor que o a inserir procura para a frente
    //--- Senão se o nó atual for igual ao no a inserir  e for para inserir,
    //--- insere o nó depois do atual.

    if ((Res = Compara(GetAt(Atual),No)) == MAIOR)
    {
      GetPrev(Atual); 

      while (Atual && (Res = Compara(GetAt(Atual),No)) == MAIOR) GetPrev(Atual);

      if (Res == IGUAL && ! INSERIRIGUAL) return;

      if (!Atual)
      {
        AddHead(No);
        Atual = GetHeadPosition();
      }
      else InsertAfter(Atual,No);
    }
    else 
    {
      if (Res == MENOR)
      {
        while (Atual && (Res = Compara(GetAt(Atual),No)) == MENOR) GetNext(Atual);

        if (Res == IGUAL && !INSERIRIGUAL) return;

        if (!Atual) 
        {
          AddTail(No);
          Atual = GetTailPosition();
        }
        else InsertBefore(Atual,No);
      }
      else if (INSERIRIGUAL) InsertAfter(Atual,No);
    }
  }
}

int CEstacasACalcular::PosicionaNoMaisProximo(CEstaca& Estaca,int Direcao)
{
  Atual = GetHeadPosition();
  if (!Atual) return MENOR;

  Direcao = FRENTE;

  if (Direcao == FRENTE)
  {
    while(Atual && GetAt(Atual) < Estaca) GetNext(Atual);

    if (!Atual) Atual = GetTailPosition();
  }
  else
  {
    GetPrev(Atual);

    while(Atual && GetAt(Atual) > Estaca) GetNext(Atual);

    if (!Atual) Atual = GetHeadPosition();
  }

  return GetAt(Atual) > Estaca ? MAIOR : GetAt(Atual) < Estaca ? MENOR : IGUAL;
}

void CEstacasACalcular::RemoveEstacasForaGeom(CCurHorizontais* pCCurHoriz,CList <CEstaca, CEstaca&>& ListaEstacasACalcular)
{
  POSITION PUltimaCurva(pCCurHoriz->GetTailPosition()),
           PEstacaAtual(ListaEstacasACalcular.GetHeadPosition());

  CEstaca EstFim(pCCurHoriz->PegaUltimoSt());

  while (ListaEstacasACalcular.GetAt(PEstacaAtual) < EstFim)  ListaEstacasACalcular.GetNext(PEstacaAtual);

  while (PEstacaAtual) ListaEstacasACalcular.RemoveAt(PEstacaAtual++);
}