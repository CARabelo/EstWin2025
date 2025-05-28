// CDeltaSuperficie.cpp: implementation of the CDeltaSuperficie class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "est.h"
#include "cponto.h"
#include <list>
#include <set>
#include "CDeltaSuperficie.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include <map>
#include "CAcidentesTopograficos.h"
#include "CSegmento.h"
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CLPontos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "CEquipDrenagem.h"
#include "superficie.h"
#include "monologo.h"

extern class monologo monolog;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef std::set<CDeltaSuperficie>* pSSuperficie;                   //--- Set de pontos da superficie.
typedef std::set<CDeltaSuperficie>::iterator itpSetSuperficie;       //--- iterador para o Set de pontos da superficie.

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CDeltaSuperficie::Serialize(CArchive& ar,void * vpSetSuper,bool SoPCentral,bool PontosCN)
{
  pSSuperficie pSetSuperficie = (pSSuperficie) vpSetSuper;

  if(ar.IsStoring())
  {
    PCentral >> ar;

    if(!SoPCentral)
    {
      ar << (unsigned int) PontosLigados.size();

      for(ittysetArestas ITpSuperfAtual = PontosLigados.begin() ; ITpSuperfAtual != PontosLigados.end() ; ITpSuperfAtual++)
      {
        const Ponto& PonLigado = ITpSuperfAtual->PontoLigado;
        ar << PonLigado.x << PonLigado.y << PonLigado.z << PonLigado.fi << PonLigado.S << PonLigado.Descricao;
        ar << (unsigned int)(*ITpSuperfAtual).PontosInterpolacao.size();

        for (itsetPontosS it = (*ITpSuperfAtual).PontosInterpolacao.begin() ; it != (*ITpSuperfAtual).PontosInterpolacao.end() ; it++)
             ar << it->x << it->y << it->z;
      }

      ar << EBordo << ERestricao << Status << TipoObjeto;
    }
  }
  else
  {
    CDeltaSuperficie DeltaSuperfAtual;
    Ponto& PCentralVizinho = DeltaSuperfAtual.PegaPCentralNC();

    PCentral << ar;             //--- Serializa o ponto central
    PCentral.Baliza2 == PontosCN ? Ponto::CURVA_NIVEL : Ponto::SEM_BALIZA; //--- O ideal é salvar este estado na geração da superficie - VERIFICAR -> CORRIGIR

    if(!SoPCentral)
    {
      unsigned int i;


      for(ar >> i ;  i ; i--)     //--- Serializa a lista de pontos ligados
      {
        ar >> PCentralVizinho.x >> PCentralVizinho.y >> PCentralVizinho.z >> PCentralVizinho.fi >> PCentralVizinho.S >> PCentralVizinho.Descricao;
        //      PCentralVizinho.Baliza >> PCentralVizinho.Inserido ;  
  
        //--- Se o ponto ligado ainda não existe no set insere-o. Se existe pega o endereço

        itpSetSuperficie ItSuperf = pSetSuperficie->find(PCentralVizinho);
        if(ItSuperf == pSetSuperficie->end()) 
		    {
		      ItSuperf = pSetSuperficie->insert(PCentralVizinho).first;
		    }
        else 
		    {
			    CDeltaSuperficie* pCDeltaSuperficie(const_cast<CDeltaSuperficie*>(&*ItSuperf)); 
			    pCDeltaSuperficie->PCentral.fi = PCentralVizinho.fi;    //--- O s esta com um valor anterior q não sei o q é , investigar
		    }

        CAresta ArestaAtual(ItSuperf->PegaPCentral());   

        int QPontosInter;
        Ponto PAtual;
      
        for (ar >> QPontosInter ; QPontosInter  ; QPontosInter--)
        {
          PAtual << ar;
          ArestaAtual.PontosInterpolacao.insert(PAtual);
        }

        PontosLigados.insert(ArestaAtual);    
      }

      ar >> EBordo >> ERestricao >> Status >> TipoObjeto;
    }
  }
}

const CDeltaSuperficie& CDeltaSuperficie::operator=(const CDeltaSuperficie& D) 
{
   PCentral = D.PCentral;
  EBordo = D.EBordo;
  ERestricao = D.ERestricao;
  Status = D.Status;
  TipoObjeto =  D.TipoObjeto;

  PontosLigados = D.PontosLigados;

  itSuperfLigadaAtual = PontosLigados.end();

  /*

  for(itSuperfLigadaAtual = PontosLigados.begin() ; 
  itSuperfLigadaAtual != PontosLigados.end() && ((CDeltaSuperficie*)(*itSuperfLigadaAtual))->PegaPCentralNC() !=  ((CDeltaSuperficie*)(*D.itSuperfLigadaAtual))->PegaPCentralNC() ;
  itSuperfLigadaAtual++);
  */

  return *this;
}

CDeltaSuperficie& CDeltaSuperficie::operator+=(const CDeltaSuperficie& D)
{
  for (ittysetArestas it = D.PontosLigados.begin(); it != D.PontosLigados.end(); it++)
  {
    PontosLigados.insert(*it);    //--- Se já existir automaticamente não insere
  }

  return *this;
}
