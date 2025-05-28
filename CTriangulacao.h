class CTriangulacao
{
  public:

  setPontos SetPontosSuperf;
  tysetCTriangulo SetTriangulos;
  tysetCTrianguloP1 SetP1;
  tysetCTrianguloP2 SetP2;
  tysetCTrianguloP3 SetP3;

  void InsereTriangulo(Ponto& pP1,Ponto& pP2,Ponto& pP3)
  {
    //--- Claro, os pontos repetidos serão descartados

    SetPontosSuperf.insert(pP1);
    SetPontosSuperf.insert(pP2);
    SetPontosSuperf.insert(pP3);

    auto Resp(SetTriangulos.emplace(pP1,pP2,pP3));

    SetP1.insert(Resp.first);
    SetP2.insert(Resp.first);
    SetP3.insert(Resp.first);
  }

  tylstitsetCTriangulo& PegaTriangulosComPonto(const Ponto& pPonto);
};

