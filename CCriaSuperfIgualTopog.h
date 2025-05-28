class CCriaSuperfIgualTopog
{
  CTriangulacao& TriangTopografia;
  LLPontos& CurvasNivel;
  SSuperficie Superficie;

  public:

  void GravaArqTrian(const std::string& NomeArq);
  void GravaArqCNs(const std::string& NomeArq);

  CCriaSuperfIgualTopog(CTriangulacao& TriangTopog,LLPontos& pCurvasNivel) : TriangTopografia(TriangTopog),CurvasNivel(pCurvasNivel) {}
  void CriaSuperfIgualTopog();


};

