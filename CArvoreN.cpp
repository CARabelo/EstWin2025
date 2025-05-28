#include "stdafx.h"
#include "CPonto.h"
#include <map>
#include "CDeltaSuperficie.h"
#include "CNoTalvegue.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CArvoreN.h"

int CArvoreN::AchaCaminho(const Ponto& PontoProcurado,LLDeltaSup& pListaRestricoes,LDeltaSup& pTalveguePrincipal)
{
  //--- Monta a arvore N ->  Uma arvore com n pontos en cada nó
  //--- Os pontos en cada nó são pontos de início ou de fim de uma lista de talvegues

  MapNosTalveguesTodos.clear();
  MapNosTalveguesTodos.insert(typairmapNosTalvegues(NoRaiz,NoRaiz));

  int Locais[2] = { CNoTalvegue::INICIO,CNoTalvegue::FIM };
  int FezUm(false);

  for (ItLLDeltaSup it = pListaRestricoes.begin(); it != pListaRestricoes.end(); it++)
  {
    if (it->begin()->TipoObjeto == CAciTopografico::TALVEGUE)
    {
      for (int C = 0; C < 2; C++)
      {
        CNoTalvegue NoTalvTodosIniAtual(it, Locais[C]);

        ittymapNosTalvegues ItTalvAtual(MapNosTalveguesTodos.find(NoTalvTodosIniAtual));

        if (ItTalvAtual != MapNosTalveguesTodos.end())
        {
         ItTalvAtual->second.IncluiPontoNo(it, Locais[C]); 
        }
        else
        {
          MapNosTalveguesTodos.insert(typairmapNosTalvegues(NoTalvTodosIniAtual,NoTalvTodosIniAtual));
        }
      }
    }
  }

  //--- Procura o ponto na arvore

  int N3(0);
  bool Achou(false),Fim(false);
  bool SentidoTalvFinal(CNoTalvegue::INICIO);
  CItLItLLDeltaSup LTalvFinal;

  ittymapNosTalvegues itNoAtual(MapNosTalveguesTodos.find(CNoTalvegue(NoRaiz)));

  itNoAtual->second.Visitado = true;
  itNoAtual->second.itListaNonoAtual = itNoAtual->second.PegaListaPontosNo().begin();

  NosVisitados.push_back(itNoAtual);
  
  do
  {
    if (itNoAtual != MapNosTalveguesTodos.end())
    {
      int Flags(itNoAtual->second.PegaFlagsPontosNo());
      auto itListaNonoAtual(itNoAtual->second.PegaListaPontosNo().begin());

      while (!Fim && (!Achou && itListaNonoAtual != itNoAtual->second.PegaListaPontosNo().end()))
      {
        bool Sentido(Flags & (1 << N3));  //--- 0 frente 1 pra tras (begin & rbegin)

        Achou = ProcuraNaLista(PontoProcurado,*itListaNonoAtual,Sentido);

        if (!Achou)
        {
          //--- Se não achou verifica se tem outro nó na outra ponta desta lista

          CNoTalvegue ProxNo(Sentido ? (*itListaNonoAtual)->begin()->PCentral : (*itListaNonoAtual)->rbegin()->PCentral);
          ProxNo.NoAnterior = (CNoTalvegue)itNoAtual->first;

          auto ItProximoNo(MapNosTalveguesTodos.find(ProxNo));

          if ((ItProximoNo != MapNosTalveguesTodos.end() && *ItProximoNo != *itNoAtual) &&
               ItProximoNo->second.PegaListaPontosNo().size() > 1 && ItProximoNo->second.Visitado == false)       //--- Se tem verifica nesta (próxima) lista
          {
            itNoAtual = ItProximoNo;

            ((CNoTalvegue)itNoAtual->first).NoAnterior = ProxNo.NoAnterior;

            itListaNonoAtual = itNoAtual->second.PegaListaPontosNo().begin();
            itNoAtual->second.N1 = N3 = 0;
            itNoAtual->second.itListaNonoAtual = itListaNonoAtual;
            itNoAtual->second.Visitado = true;

            Flags = itNoAtual->second.PegaFlagsPontosNo();
            NosVisitados.push_back(itNoAtual);
          }
          else                //--- Se não tem pega o proxima lista de pontos neste nó
          {
            N3++;

            itListaNonoAtual++;
            itNoAtual->second.itListaNonoAtual++;
            itNoAtual->second.N1++;

            if (itListaNonoAtual != itNoAtual->second.PegaListaPontosNo().end())
            {
              bool Sentido3((Flags & (1 << N3)));

              if ((!Sentido3 && itNoAtual->second.NoAnterior == (*(*itListaNonoAtual)->rbegin()).PCentral) ||
                 (Sentido3 && itNoAtual->second.NoAnterior == (*(*itListaNonoAtual)->begin()).PCentral))
              {
                N3++;

                itListaNonoAtual++;

                itNoAtual->second.itListaNonoAtual++;
                itNoAtual->second.N1++;
              }
            }

            while(!Fim && itListaNonoAtual == itNoAtual->second.PegaListaPontosNo().end())
            {
              NosVisitados.pop_back();

              if(NosVisitados.size() != 0 && (*NosVisitados.begin())->second.N1 <= (*NosVisitados.begin())->second.PegaListaPontosNo().size())   //--- Está voltando no nó, continua de onde parou
              {
                itNoAtual = *NosVisitados.rbegin();
                itListaNonoAtual = itNoAtual->second.itListaNonoAtual;
                N3 = itNoAtual->second.N1;
                Flags = itNoAtual->second.PegaFlagsPontosNo();

                itListaNonoAtual++;
                N3++;

                itNoAtual->second.itListaNonoAtual++;
                itNoAtual->second.N1++;

                if (itListaNonoAtual != itNoAtual->second.PegaListaPontosNo().end())
                {
                  bool Sentido4((itNoAtual->second.PegaFlagsPontosNo() & (1 << N3)));

                  if ((!Sentido4 && itNoAtual->second.NoAnterior == (*(*itListaNonoAtual)->rbegin()).PCentral) ||
                    (Sentido4 && itNoAtual->second.NoAnterior == (*(*itListaNonoAtual)->begin()).PCentral))
                  {
                    itNoAtual->second.itListaNonoAtual++;
                    itNoAtual->second.N1++;

                    itListaNonoAtual++;
                    N3++;
                  }
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

    TalveguesTalvPrincipal.clear();

    ListaTalvegue.push_back(NoRaiz);
    auto NoAtual(NoRaiz);

    for (auto& ProximoNo : NosVisitados)
    {
      Achou = false;
      int N1(0);
      auto& ItLPontosNo(ProximoNo->second.PegaListaPontosNo().begin());

      while (!Achou && ItLPontosNo != ProximoNo->second.PegaListaPontosNo().end())
      {
        bool Pos(ProximoNo->second.PegaFlagsPontosNo() & (1 << N1));

        Pos = !Pos;     //--- É a outra ponta
     
        auto& DSTalvegue(Pos == CNoTalvegue::INICIO ? *(*ItLPontosNo)->begin() : *(*ItLPontosNo)->rbegin());

        if(DSTalvegue.PCentral == NoAtual)
        {
          TalveguesTalvPrincipal.push_back(*ItLPontosNo);

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

          NoAtual = ProximoNo->second;
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
        TalveguesTalvPrincipal.push_back(*LTalvFinal);

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
      }
      return true;
    }
  }

  pTalveguePrincipal.clear();  //--- Ponto não encontrado

  return false;
}

void CArvoreN::FinalizaAchaCaminho(LLDeltaSup& pListaRestricoes, LDeltaSup& pTalveguePrincipal)
{
  //--- Remove da lista de restrições os talvegues que compõem o talvegue principal
  //--- e inclui na lista de restricões o novo talvegue principal 

  for (auto& LResAtual : TalveguesTalvPrincipal)
  {
    pListaRestricoes.erase(LResAtual);
  }

  pListaRestricoes.push_back(pTalveguePrincipal);
}