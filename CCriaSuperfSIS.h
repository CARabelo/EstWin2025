class CCriaSuperfSIS
{
  LLPontos& CurvasNivel,& Talvegues;
  SSuperficie Superficie;

public:

  CCriaSuperfSIS(LLPontos& pCurvasNivel,LLPontos& pTalvegues) : CurvasNivel(pCurvasNivel),Talvegues(pTalvegues) {}
  void CriaSuperfIgualTopog();
  void GravaArqCNs(const std::string& NomeArq);
  void GravaArqTalvegues(const std::string& NomeArq);
};

