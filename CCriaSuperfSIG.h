class CCriaSuperfSIG
{
  LLPontos& CurvasNivel;
  SSuperficie Superficie;

public:

  CCriaSuperfSIG(LLPontos& pCurvasNivel) : CurvasNivel(pCurvasNivel) {}
  void GravaArqCNs(const std::string& NomeArq);
};

