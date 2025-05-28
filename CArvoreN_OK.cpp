#include "stdafx.h"
#include "CPonto.h"
#include <stack>
#include "CDeltaSuperficie.h"
#include "CNoTalvegue.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CArvoreN.h"

void CArvoreN::AchaCaminho(const Ponto& PontoProcurado,LLDeltaSup& pListaRestricoes,LDeltaSup& pTalveguePrincipal)
{
  //--- Monta a arvore N ->  Uma arvore com n pontos en cada nó
  //--- Os pontos en cada nó são pontos de início ou de fim de uma lista de talvegues

  SetNosTalveguesTodos.clear();
  SetNosTalveguesTodos.insert(NoRaiz);
  int Locais[2] = { CNoTalvegue::INICIO,CNoTalvegue::FIM };
  int FezUm(false);

  for (ItLLDeltaSup it = pListaRestricoes.begin(); it != pListaRestricoes.end(); it++)
  {
    if (it->begin()->TipoObjeto == CAciTopografico::TALVEGUE)
    {
      for (int C = 0; C < 2; C++)
      {
        CNoTalvegue NoTalvTodosIniAtual(it, Locais[C]);

        ittysetNosTalvegues ItTalvAtual(SetNosTalveguesTodos.find(NoTalvTodosIniAtual));

        if (ItTalvAtual != SetNosTalveguesTodos.end())
        {
          const_cast<CNoTalvegue*>(&*ItTalvAtual)->IncluiPontoNo(it, Locais[C]);  //--- Não corrompe o set 
        }
        else
        {
          SetNosTalveguesTodos.insert(NoTalvTodosIniAtual);
        }
      }
    }
  }

  //--- Procura o ponto na arvore

  int N3(0);
  bool Achou(false),Fim(false);
  bool SentidoTalvFinal(CNoTalvegue::INICIO);
  CItLItLLDeltaSup LTalvFinal;
  auto itNoAtual(SetNosTalveguesTodos.find(CNoTalvegue(NoRaiz)));  
  
  do
  {
    if (itNoAtual != SetNosTalveguesTodos.end())
    {
      int Flags(itNoAtual->PegaFlagsPontosNo());
      auto itListaNonoAtual(itNoAtual->PegaListaPontosNo().begin());

      while (!Fim && (!Achou && itListaNonoAtual != itNoAtual->PegaListaPontosNo().end()))
      {
        bool Sentido(Flags & (1 << N3));  //--- 0 frente 1 pra tras (begin & rbegin)

        Achou = ProcuraNaLista(PontoProcurado,*itListaNonoAtual,Sentido);

        if (!Achou)
        {
          //--- Se não achou verifica se tem outro no na outra ponta desta lista

          CNoTalvegue ProxNo(Sentido ? (*itListaNonoAtual)->begin()->PCentral : (*itListaNonoAtual)->rbegin()->PCentral);
          ProxNo.NoAnterior = *itNoAtual;

          auto ItProximoNo(SetNosTalveguesTodos.find(ProxNo));

          if ((ItProximoNo != SetNosTalveguesTodos.end() && *ItProximoNo != *itNoAtual) &&
               ItProximoNo->PegaListaPontosNo().size() > 1)       //--- Se tem verifica nesta (próxima) lista
          {
            itNoAtual = ItProximoNo;

            CNoTalvegue* pNoTalvegue(const_cast<CNoTalvegue*>(&*itNoAtual));

            pNoTalvegue->NoAnterior = ProxNo.NoAnterior;

            itListaNonoAtual = itNoAtual->PegaListaPontosNo().begin();
            N3 = 0;
            pNoTalvegue->N1 = N3;                                //--- Não corrompe o set
            pNoTalvegue->itListaNonoAtual = itListaNonoAtual;    //--- Não corrompe o set
            Flags = itNoAtual->PegaFlagsPontosNo();
            NosVisitados.push_back(itNoAtual);
          }
          else                //--- Se não tem pega o proxima lista de pontos neste nó
          {
            itListaNonoAtual++;
            N3++;

             CNoTalvegue* pNoTalvegue(const_cast<CNoTalvegue*>(&*itNoAtual));

             pNoTalvegue->itListaNonoAtual++;
             pNoTalvegue->N1++;

            if (itListaNonoAtual != itNoAtual->PegaListaPontosNo().end())
            {
              bool Sentido3((Flags & (1 << N3)));

              if ((!Sentido3 && itNoAtual->NoAnterior == (*(*itListaNonoAtual)->rbegin()).PCentral) ||
                 (Sentido3 && itNoAtual->NoAnterior == (*(*itListaNonoAtual)->begin()).PCentral))
              {
                itListaNonoAtual++;
                N3++;

                CNoTalvegue* pNoTalvegue(const_cast<CNoTalvegue*>(&*itNoAtual));

                pNoTalvegue->itListaNonoAtual++;
                pNoTalvegue->N1++;
              }
            }

            //if (itListaNonoAtual == itNoAtual->PegaListaPontosNo().end())
            while(!Fim && itListaNonoAtual == itNoAtual->PegaListaPontosNo().end())
            {
              NosVisitados.pop_back();

              if(NosVisitados.size() != 1)   //--- Está revisitando o nó, continua de onde parou
              {
                itNoAtual = *NosVisitados.rbegin();
                itListaNonoAtual = itNoAtual->itListaNonoAtual;
                N3 = itNoAtual->N1;
                itListaNonoAtual++;
                N3++;

                CNoTalvegue* pNoTalvegue(const_cast<CNoTalvegue*>(&*itNoAtual));

                pNoTalvegue->itListaNonoAtual++;
                pNoTalvegue->N1++;

                bool Sentido4((pNoTalvegue->PegaFlagsPontosNo() & (1 << N3)));

                if ((!Sentido4 && itNoAtual->NoAnterior == (*(*itListaNonoAtual)->rbegin()).PCentral) ||
                    (Sentido4 && itNoAtual->NoAnterior == (*(*itListaNonoAtual)->begin()).PCentral))
                {
                  pNoTalvegue->itListaNonoAtual++;
                  pNoTalvegue->N1++;

                  itListaNonoAtual++;
                  N3++;
                }
              }
              else
              {
                Fim = true;   //--- Visitou toda a árvore, não encontrou o ponto procurado
              }
            }
          }
        }
        else
        {
          LTalvFinal = itListaNonoAtual;
          SentidoTalvFinal = Sentido;
        }
      }
    }
  } while (!Fim && !Achou);

  if (Achou)
  {
    pTalveguePrincipal.clear();

    //--- Monta o talvegue principal

    LPontos ListaTalvegue;
    bool Achou(false);

    ListaTalvegue.push_back(NoRaiz);
    auto NoAtual(NoRaiz);

    for (auto& ProximoNo : NosVisitados)
    {
      Achou = false;
      int N1(0);
      auto& ItLPontosNo(ProximoNo->PegaListaPontosNo().begin());

      while (!Achou && ItLPontosNo != ProximoNo->PegaListaPontosNo().end())
      {
        bool Pos(ProximoNo->PegaFlagsPontosNo() & (1 << N1));

        Pos = !Pos;     //--- É a outra ponta
     
        auto& DSTalvegue(Pos == CNoTalvegue::INICIO ? *(*ItLPontosNo)->begin() : *(*ItLPontosNo)->rbegin());

        if(DSTalvegue.PCentral == NoAtual)
        {
          if(Pos == CNoTalvegue::INICIO)
          {
            for (auto& ItTalv = (*ItLPontosNo)->begin(); ItTalv != (*ItLPontosNo)->end(); ItTalv++)
            {
              pTalveguePrincipal.push_back(*ItTalv);
            }
          }
          else
          {
            for (auto& ItTalvr = (*ItLPontosNo)->rbegin(); ItTalvr != (*ItLPontosNo)->rend(); ItTalvr++)
            {
              pTalveguePrincipal.push_back(*ItTalvr);
            }
          }

          NoAtual = *ProximoNo;
          Achou = true;
        }
        else
        {
          ItLPontosNo++;
          N1++;
        }
      }
    } 

    
    if (Achou)    //--- ==>>> Tem que achar!!! insere o ultimo talvegue no talvegue principal
    {
      if (SentidoTalvFinal == CNoTalvegue::INICIO)
      {
        for (auto& ItTalv = (*LTalvFinal)->begin(); ItTalv != (*LTalvFinal)->end(); ItTalv++)
        {
          pTalveguePrincipal.push_back(*ItTalv);
        }
      }
      else
      {
        for (auto& ItTalvr = (*LTalvFinal)->rbegin(); ItTalvr != (*LTalvFinal)->rend(); ItTalvr++)
        {
          pTalveguePrincipal.push_back(*ItTalvr);
        }

        pTalveguePrincipal.pop_back();

      }
    }
    else
    {
      //return;//exit(1);  //--- Ponto não encontrado
    }
  }
}

bool CArvoreN::ProcuraNaLista(const Ponto& pPontoProcurado,ItLLDeltaSup pItNoAtual,int Sentido)
{
  //--- Procura a lista no sentido invertido:
  //--- 0 = INICIO => Procura no fim
  //--- 1 = FIM => Procura no inicio

  if(Sentido == 0)
  {
    if (*pItNoAtual->rbegin() == pPontoProcurado)
    {
      return true;
    }
  }
  else
  {
    if (*pItNoAtual->begin() == pPontoProcurado)
    {
      return true;
    }
  }

  return false;
}