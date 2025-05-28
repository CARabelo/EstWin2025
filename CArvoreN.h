class CArvoreN
{
  tymapNosTalvegues MapNosTalveguesTodos;
  tylstittymapNosTalvegues NosVisitados;
  LItLLDeltaSup TalveguesTalvPrincipal;

  CNoTalvegue NoRaiz;

  bool ProcuraNaLista(const Ponto& pPontoProcurado, ItLLDeltaSup pItNoAtual, int Sentido)
  {
    //--- Procura a lista no sentido invertido:
    //--- 0 = INICIO => Procura no fim
    //--- 1 = FIM => Procura no inicio

    if (Sentido == 0)
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

  public:

    CArvoreN(const Ponto& pRaiz):NoRaiz(pRaiz)
    {
      NosVisitados.clear();
    }

    int AchaCaminho(const Ponto& P,LLDeltaSup& pListaRestricoes,LDeltaSup& pTalveguePrincipal);
    void FinalizaAchaCaminho(LLDeltaSup& pListaRestricoes, LDeltaSup& pTalveguePrincipal);

};

