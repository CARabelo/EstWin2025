#include "stdafx.h"
#include <list>
#include <set>
#include "CPonto.h"
#include "CRetangulo.h"
#include "Estaca.h"
#include "string.h"
#include "CAciTopografico.h"
#include <algorithm>
#include "CLPontos.h"
#include "Mat.h"

extern class Matem Mat;

tyvecNomeDescricao CAciTopografico::VecNomeDescricao =
{
   {NENHUM_ACITOPOG,"Nenhum_Acitopog "},
   {MURO,"Muro "},{CERCA,"C�rca "},{CASA,"Casa "},{CANTO_MURO,"Canto  Muro "},{CANTO_CERCA,"Canto C�rca "},{CANTO_CASA,"Canto Casa "},{BORDO,"Bordo "},
   {MEIO_FIO,"Meio-Fio "},{N_A,"N.A. "},{N_A_PER,"N.A. Peri�dico "},{ROCHA,"Rocha "},{PONTE,"Ponte "},{TAMPA_BUEIRO,"Tampa de Bueiro "},{POSTE,"Poste "},
   {REDE_ELETRICA_BT,"Rede El�trica B.T"},{REDE_ELETRICA_AT,"Rede El�trica A.T"},{FRONTEIRA,"Fronteira "},{EIXO,"Eixo "},{CRISTA,"Crista "},{PE,"P� "},
   {BUEIRO,"Bueiro "},{LINHA_FERREA,"Linha F�rrea "},{MATA,"Mata "},{FLORESTA,"Floresta"},{PLANTACAO,"Planta��o "},{ARVORE,"�rvore"},{ALAGADO,"Alagado "},
   {ALINHAMENTO,"Alinhamento "},{TALVEGUE,"Talvegue "},{ESPIGAO,"Espig�o "},{RAVINA,"Ravina "},
   {PVESGOTO,"PV Esgoto "},{PVGAS,"PV G�s "},{PVELETRICA,"PV El�trica "},{REDE_GAS,"Rede G�s "},{REDE_ELETRICA,"Rede El�trica "},{REDE_ESGOTO,"Rede Esgoto "},
   {SOLEIRA,"Soleira "},
   {INICIO_TERRAP,"In�cio Terraplanagem "},
   {OFF_SETS,"Off-Sets "},{TERRAPLANAGEM,"Terraplanagem "},{FRONTEIRA_TERRAP,"Fronteira Terraplanagem "},{FIM_TERRAP,"Fim Terralplanagem "},{CRISTA_LIMPEZA,"Crista Limpeza "},
   {OFFSET_CORTE,"Off-set de Corte "},{OFFSET_ATERRO,"Off-Set de Aterro "},
   {INICIO_EQUI_DRENAGEM,"Inicio Equip. Drenagem"},
   {EQUI_BUEIRO,"Bueiro "},{EQUI_VALETA_TERRAPL,"Valeta de Terraplanagem "},{EQUI_VALETA_TN,"Valeta OffSet/Talude "},{EQUI_BUEIRO_RETANG,"Bueiro Retangular "},{EQUI_DESCIDA_DAGUA,"Descida D�agua"},
   {EQUI_CAIXA_COLETORA,"Caixa Coletora "},{EQUI_BACIA_DISSIPACAO,"Bacia de Dissipa��o "},{EQUI_SARJETA,"Sarjeta "},{EQUI_TUBULACAO,"Tubula��o "},{EQUI_DRENO_PROFUNDO,"Dreno Profundo "},
   {EQUI_CANALETA_TN,"Canaleta Off-Set/Talude"},{EQUI_CANALETA_TERRAPL,"Canaleta Terraplanagem "},{EQUI_EDITAR_DRENAGEM,"Editar Drenagem "},{FIM_EQUI_DRENAGEM,"Fim Equip. Drenagem "},
   {FIM_ACITOPOG,"Fim Acidentes Topogr�ficos"}
};
    
CAciTopografico::CAciTopografico(const CAciTopografico& pAT) : Tipo(pAT.Tipo),Fechada(pAT.Fechada),Area(INFINITO),Perimetro(INFINITO)
{
  for(citlstitsetPontos it = pAT.LstItPontos.begin() ; it != pAT.LstItPontos.end() ; ++it)
  {
    InserePonto((**it));
  }

  CalculaAreaPerimetro();
};

bool CAciTopografico::InserePonto(const Ponto& pPonto)
{
  pairitboolsetPontos itpairSet(SetPontosAciTopog.insert(pPonto));
  if(itpairSet.second != false)
  {
    LstItPontos.push_back(itpairSet.first);
    RetanInscrito.AjustaMaior(*(*LstItPontos.rbegin()));

    ritlstitsetPontos ItPAtual(LstItPontos.rbegin());
    ritlstitsetPontos ItPAnterior(ItPAtual);

    ++ItPAnterior;

    if (LstItPontos.size() > 1) 
    {
      Ponto& PAnterior(const_cast<Ponto&>(**ItPAnterior)),
           & PAtual(const_cast<Ponto&>(**ItPAtual));

      PAnterior.fi = PAtual.fi = Mat.CalculaAzimute(**ItPAtual,**ItPAnterior);
    }
  }

  return false;
  //--- logar erro de inser��o
}

citlstitsetPontos CAciTopografico::InserePonto(citlstitsetPontos itLocal,const Ponto& pPonto)
{
  //--- insere o ponto no set e o coloca em uma determinada posi��o na lista (O proximo de  itLocal)

  pairitboolsetPontos itpairSet(SetPontosAciTopog.insert(pPonto));   //--- se o ponto ja existir retorna um iterador para  o ponto existente

  if (itpairSet.second != true)              //--- Se true o ponto j� existe, remove-o (o iterador da lista passar� a apontar para a nova posi��o)
  {
    LstItPontos.remove(itpairSet.first);
  }

  RetanInscrito.AjustaMaior(pPonto);
  return LstItPontos.insert(itLocal, itpairSet.first);
}

citlstitsetPontos CAciTopografico::AlteraPontoMantemPosicao(citlstitsetPontos& itPosicao, const Ponto& pPonto)
{
  //--- insere o ponto no set e o mant�m em sua posi��o na lista

  SetPontosAciTopog.erase(*itPosicao);

  pairitboolsetPontos itpairSet(SetPontosAciTopog.insert(pPonto));
  //if (itpairSet.second != false)
  {
    //--- O ponteiro da lista deve apontar para o novo elemento (mantendo a posi��o na lista, mesmo ponteiro)

    *(const_cast<itsetPontos*>(&*itPosicao)) = itpairSet.first;

    return itPosicao;
  }

  return LstItPontos.end();
}

bool CAciTopografico::RemovePonto(const Ponto& Ponto)
{
  itsetPontos itSet(SetPontosAciTopog.find(Ponto));

  if(itSet != SetPontosAciTopog.end())
  {
    itlstitsetPontos itLst(std::find(LstItPontos.begin(),LstItPontos.end(),itSet));
    if(itLst != LstItPontos.end())
    {
      LstItPontos.erase(itLst);
      SetPontosAciTopog.erase(itSet);

      if(SetPontosAciTopog.size())
      {
        //--- Teria que acertar o y tb mas todo o set teria que ser varrido...

        RetanInscrito.Ajustax(Ponto.x);
      }

       return true;
    }

    //--- ArqLog << "SetPontos/Lista corrompidos:" << Ponto
  }

  return false;
}

void CAciTopografico::CalculaAreaPerimetro()
{
  double Deltax(0.0),Deltay(0.0);

  Perimetro = Area = 0.0;

  if(LstItPontos.size() < 2) return;
  
  citlstitsetPontos itAnterior(LstItPontos.begin()),itAtual(itAnterior);

  for (++itAtual ; itAtual != LstItPontos.end(); ++itAtual)
  {
    Deltax = (*itAtual)->x - (*itAnterior)->x;
    Deltay = (*itAtual)->y + (*itAnterior)->y;

    Area += ((*itAtual)->y + (*itAnterior)->y) / 2.0 * Deltax;
    Perimetro += hypot((*itAtual)->y - (*itAnterior)->y, Deltax);

    itAnterior = itAtual;
  }

}

 /*
int CAciTopografico::VerfTalvegue(ItLDeltaSup AciTopog)
{
  if (!Fechada) return BACIA_ABERTA;

  bool PontoDentro(false), PontoFora(false);

  for (itlstitsetPontos itPonVerificarAtual = LstSetPontos.begin(); itPonVerificarAtual != LstSetPontos.end(); ++itPonVerificarAtual)
  {
    citlstitsetPontos itPontoAnt(LstItPontos.begin()), itPontoAtual(itPontoAnt);
    int Contador(0);

    PontoDentro = false;

    for (++itPontoAtual; itPontoAtual != LstItPontos.end(); ++itPontoAtual, ++itPontoAnt)
    {
      if ((Area < 0.0 && (*itPontoAnt)->x >(*itPonVerificarAtual)->x && (*itPontoAtual)->x > (*itPonVerificarAtual)->x) ||
        (Area > 0.0 && (*itPontoAnt)->x < (*itPonVerificarAtual)->x && (*itPontoAtual)->x < (*itPonVerificarAtual)->x))
      {
        if (((*itPonVerificarAtual)->y > (*itPontoAnt)->y && (*itPonVerificarAtual)->y < (*itPontoAtual)->y) || ((*itPonVerificarAtual)->y < (*itPontoAnt)->y && (*itPonVerificarAtual)->y >(*itPontoAtual)->y))
        {
          ++Contador;
        }
      }
      else
      {
        if (((*itPontoAnt)->x < (*itPonVerificarAtual)->x && (*itPontoAtual)->x >(*itPonVerificarAtual)->x) || ((*itPontoAnt)->x > (*itPonVerificarAtual)->x && (*itPontoAtual)->x < (*itPonVerificarAtual)->x))
        {
          if (((*itPontoAnt)->y > (*itPonVerificarAtual)->y && (*itPontoAtual)->y < (*itPonVerificarAtual)->y) || ((*itPontoAnt)->y < (*itPonVerificarAtual)->y && (*itPontoAtual)->y >(*itPonVerificarAtual)->y))
          {
            Ponto PontoIntersecao;

            if (CLPontos::CalculaIntersecao(**itPontoAnt, **itPontoAtual, **itPonVerificarAtual, Ponto((*itPonVerificarAtual)->x + 1000.0, (*itPonVerificarAtual)->y), PontoIntersecao, false) == TRUE)
            {
              if ((Area < 0.0 && PontoIntersecao.x >(*itPonVerificarAtual)->x) || (Area > 0.0 && PontoIntersecao.x < (*itPonVerificarAtual)->x))
              {
                ++Contador;
              }
            }
          }
        }
      }
    }

    if (Contador % 2 == 0) PontoFora = true;
    else PontoDentro = true;
  }

  if (PontoDentro == false)
  {
    if (PontoFora == false) SEM_PONTOS;                    //---- Talvegue ou bacia sem pontos
    else  return TALVEGUE_FORA_BACIA;                     //-- O talvegue est� completamente fora da bacia
  }
  else
  {
    if (PontoFora == false) return TALVEGUE_DENTRO_BACIA; //---- Talvegue esta completamente dentro da bacia
    else return TALVEGUE_CORTA_BACIA;                     //-- O talvegue cruza a bacia
  }
}
*/



/*
int CAciTopografico::VerfTalvegue(lstitsetPontos& LstSetPontos)
{
  if(!Fechada) return BACIA_ABERTA;

  bool PontoDentro(false),PontoFora(false);

  for (itlstitsetPontos itPonVerificarAtual = LstSetPontos.begin(); itPonVerificarAtual != LstSetPontos.end(); ++itPonVerificarAtual)
  {
    citlstitsetPontos itPontoAnt(LstItPontos.begin()),itPontoAtual(itPontoAnt);
    int Contador(0);

    PontoDentro = false;

    for (++itPontoAtual;  itPontoAtual != LstItPontos.end() ; ++itPontoAtual, ++itPontoAnt)
    {
      if ((Area < 0.0 && (*itPontoAnt)->x > (*itPonVerificarAtual)->x && (*itPontoAtual)->x > (*itPonVerificarAtual)->x) ||
          (Area > 0.0 && (*itPontoAnt)->x < (*itPonVerificarAtual)->x && (*itPontoAtual)->x < (*itPonVerificarAtual)->x))
      {
        if (((*itPonVerificarAtual)->y > (*itPontoAnt)->y && (*itPonVerificarAtual)->y < (*itPontoAtual)->y) || ((*itPonVerificarAtual)->y < (*itPontoAnt)->y && (*itPonVerificarAtual)->y >(*itPontoAtual)->y))
        {
          ++Contador;
        }
      }
      else
      {
        if (((*itPontoAnt)->x < (*itPonVerificarAtual)->x && (*itPontoAtual)->x >(*itPonVerificarAtual)->x) || ((*itPontoAnt)->x > (*itPonVerificarAtual)->x && (*itPontoAtual)->x < (*itPonVerificarAtual)->x))
        {
          if (((*itPontoAnt)->y > (*itPonVerificarAtual)->y && (*itPontoAtual)->y < (*itPonVerificarAtual)->y) || ((*itPontoAnt)->y < (*itPonVerificarAtual)->y && (*itPontoAtual)->y >(*itPonVerificarAtual)->y))
          {
            Ponto PontoIntersecao;

            if (CLPontos::CalculaIntersecao(**itPontoAnt, **itPontoAtual, **itPonVerificarAtual, Ponto((*itPonVerificarAtual)->x + 1000.0, (*itPonVerificarAtual)->y), PontoIntersecao, false) == TRUE)
            {
              if ((Area < 0.0 && PontoIntersecao.x >(*itPonVerificarAtual)->x) || (Area > 0.0 && PontoIntersecao.x < (*itPonVerificarAtual)->x))
              {
                ++Contador;
              }
            }
          }
        }
      }
    }

    if (Contador % 2 == 0) PontoFora = true;
    else PontoDentro = true;
  }  

  if (PontoDentro == false)  
  {
    if(PontoFora == false) SEM_PONTOS;                    //---- Talvegue ou bacia sem pontos
    else  return TALVEGUE_FORA_BACIA;                     //-- O talvegue est� completamente fora da bacia
  }
  else
  {
    if (PontoFora == false) return TALVEGUE_DENTRO_BACIA; //---- Talvegue esta completamente dentro da bacia
    else return TALVEGUE_CORTA_BACIA;                     //-- O talvegue cruza a bacia
  }
}
*/
