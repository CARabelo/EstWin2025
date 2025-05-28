#include "stdafx.h"
#include "CPonto.h"
#include <list>
#include <tuple>
#include "CAlinhamento.h"
#include <map>
#include "CAlinhamentos.h"
#include <set>
#include "CRetangulo.h"
#include "CAciTopografico.h"

CAlinhamentos::CAlinhamentos(){}

CAlinhamentos::~CAlinhamentos(){}

void CAlinhamentos::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    ar << MapAlinhamentos.size();

    for (ittymapCAlinhamentos it = MapAlinhamentos.begin(); it != MapAlinhamentos.end(); it++)
    {
      int Tipo(CAciTopografico::TERRAPLANAGEM);

      if (it->first.PegaDescricao() == "OFFSEE" || it->first.PegaDescricao() == "OFFSED")
      {
        if (it->second.LstPontos.begin()->AcidenteTopografico == 899) Tipo = CAciTopografico::OFFSET_CORTE;
        else Tipo = CAciTopografico::OFFSET_ATERRO;

        //--- A mudança de cor ficou para a proxima versão
        //--- Teria-se que quebrar a lista de offsets em várias outras a partir dos pontos de passagem.

        Tipo = CAciTopografico::OFF_SETS;  
      }                                     

      it->second.Serialize(ar,Tipo);
    }

    ar << LstPontosBordo.size();

    for (ittylstpontos it = LstPontosBordo.begin(); it != LstPontosBordo.end(); ++it)
    {
      it->AcidenteTopografico = CAciTopografico::FRONTEIRA_TERRAP;
      *it >> ar;
    }
  }
  else
  {
    MapAlinhamentos.clear();
    size_t QuanReg(-1);

    CAlinhamento AlinhamentoAtual;

    for (ar >> QuanReg ; QuanReg ; --QuanReg)
    {
      AlinhamentoAtual.Serialize(ar,0);
      MapAlinhamentos.insert(typairchavealinCAlinhamentos(AlinhamentoAtual.PegaChave(),AlinhamentoAtual));
    }

    LstPontosBordo.clear();
    Ponto PontoAtual;
    size_t QuanPontos(0);

    for (ar >> QuanPontos ; QuanPontos ; --QuanPontos)
    {
      PontoAtual >> ar;
      LstPontosBordo.push_back(PontoAtual);
    }
  }
}
