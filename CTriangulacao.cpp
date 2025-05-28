#include "stdafx.h"
#include "CPonto.h"
#include "CTriangulo.h"
#include "CTriangulacao.h"

tylstitsetCTriangulo& CTriangulacao::PegaTriangulosComPonto(const Ponto& pPonto)
{
  //--- Retorna uma lista com todos os triangulos que possuem pPonto em P1, P2 ou P3

  static tylstitsetCTriangulo ListaResposta;

  ListaResposta.clear();

  CTriangulo Triangulo(pPonto,pPonto,pPonto);   
  auto Resp(SetTriangulos.insert(Triangulo));

  auto Resp1(SetP1.lower_bound(Resp.first)),
       Resp2(SetP2.lower_bound(Resp.first)),
       Resp3(SetP3.lower_bound(Resp.first));

  while (Resp1 != SetP1.end() && (*Resp1)->P1 == pPonto)
  {
    ListaResposta.emplace_front(*Resp1++);
  }

  while (Resp2 != SetP2.end() && (*Resp2)->P2 == pPonto)
  {
    ListaResposta.emplace_front(*Resp2++);
  }

  while (Resp3 != SetP3.end() && (*Resp3)->P3 == pPonto)
  {
    ListaResposta.emplace_front(*Resp3++);
  }

  return ListaResposta;
}
