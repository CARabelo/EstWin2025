#include "stdafx.h"
#include "CPonto.h"
#include <tuple>
#include "CAlinhamento.h"
#include <set>
#include "CRetangulo.h"
#include "CAciTopografico.h"

CAlinhamento::CAlinhamento(double Est, double pAfast, std::string& Desc,int pTipo) : Chave(Est, pAfast, Desc,pTipo), Ativo(false)
{
}

CAlinhamento::~CAlinhamento()
{
}

void CAlinhamento::Serialize(CArchive& ar,int Tipo)
{
  if (ar.IsStoring())
  {
    Chave.Serialize(ar);

    ar << LstPontos.size();

    for (register ittyplstPontos it = LstPontos.begin(); it != LstPontos.end(); ++it)
    {
      it->AcidenteTopografico = Tipo;
      *it >> ar;
    }
  }
  else
  {
    Chave.Serialize(ar);

    register size_t QuanPontos(-1);
    
    for (ar >> QuanPontos ; QuanPontos ; --QuanPontos)
    {
      *LstPontos.insert(LstPontos.end(),Ponto()) >> ar;
    }
  }
}
