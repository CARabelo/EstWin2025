#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqterrpontos.h" 
#include "cponto.h"
#include "clpontos.h"
#include "perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "cterrpontos.h"

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

CTerrPontos::CTerrPontos(CString& NomeArquivo,CProgressCtrl* Prog) : CArqTerrPontos(NomeArquivo), LTerrPontos(),ProgCtrl(Prog)
{
  if (IsEmpty()) return;  //--- Lista vazia? retorna.

  Perfil AlargAtual;
  CEstaca EstacaAtual;
  double EstacaAnterior = -INFINITO;
  ProgCtrl->SetRange(0,GetCount());
  ProgCtrl->SetPos(0);

  //--- Para cada os nos  da lista de Curvas lidas cria um nó na lista

  POSITION Atual = GetHeadPosition();
  ArqTerrPontos NoArqAtual = GetNext(Atual);
  AlargAtual.Estaca.Constroi(NoArqAtual.EstFinal,NoArqAtual.Ig) ;

  for (	; Atual ; NoArqAtual = GetNext(Atual),ProgCtrl->StepIt()) 
  {
    EstacaAtual.Constroi(NoArqAtual.EstFinal,NoArqAtual.Ig);
    if(EstacaAnterior == -INFINITO)  EstacaAnterior = EstacaAtual.EstVirtual; 

    if (EstacaAtual.EstVirtual!= INFINITO && EstacaAtual.EstVirtual != EstacaAnterior)
    {
      LTerrPontos.InsereOrdenada(AlargAtual);  //--- O ERRO ESTA AQUI!!!!!! ESTA VOLTANDO NULO O ATUAL!!!!! NA SEGUNDA VEZ
      AlargAtual.Destroi();                    //--- Limpa a lista de pontos atuais.
      EstacaAnterior = EstacaAtual.EstVirtual;             //--- Atualiza a estaca anterior.
      AlargAtual.Estaca.Constroi(NoArqAtual.EstFinal,NoArqAtual.Ig);
    }

    class Ponto PontoAtual;

    //--- Se houve alteraçao de estaca então os pontos são de uma nova secão, insere a seçao na lista

    for (int i = 0 ; i < 3 ; i++)
    {
      PontoAtual.x = (NoArqAtual.Pontos[i].ENumero() ? atof(NoArqAtual.Pontos[i*2]) : INFINITO);
      PontoAtual.y = (NoArqAtual.Pontos[i].ENumero() ? atof(NoArqAtual.Pontos[i*2+1]) : INFINITO); 

      if (PontoAtual.x != INFINITO && PontoAtual.y != INFINITO)
        AlargAtual.InsereOrdenada(PontoAtual);
    }
  }									 
}
