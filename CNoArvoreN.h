class CNoArvoreN;

typedef std::list<CNoArvoreN> tlstNoArvore;
typedef tlstNoArvore::iterator ittlstNoArvore;
typedef tlstNoArvore::const_iterator cittlstNoArvore;

class CNoArvoreN
{
  tlstNoArvore LstPontosNo;
  Ponto PontoNo;
  
  public:

    cittlstNoArvore ItNoAtual;
    bool Visitado;

    CNoArvoreN() : Visitado(false)
    {
    }

    CNoArvoreN(const Ponto& pPonto) :PontoNo(pPonto),Visitado(false)
    {
      ItNoAtual = LstPontosNo.begin();
    }
    
    const tlstNoArvore PegaFilhos() const { return LstPontosNo;}

    void IncluiFilho(CNoArvoreN& NoArvore)
    {
      LstPontosNo.emplace_back(NoArvore);
    }

    bool TemFilhos() { return LstPontosNo.size() > 0;}
    const Ponto& PegaPontoNo() const { return PontoNo;}
};

