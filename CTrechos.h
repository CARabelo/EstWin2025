
typedef std::map<const std::string, CTrecho> tymapCTrechos;
typedef tymapCTrechos::iterator ittymapCTrechos;
typedef std::pair<ittymapCTrechos, bool> pairtymapCTrechos;

class CTrechos : public CArqTrechos
{
  public:

  tymapCTrechos  MapaTrechos;
  ittymapCTrechos itTrechoAtual;
  
  CTrechos(const std::string& pNomeTrecho, bool bTodos);
  ~CTrechos();
  bool PegaNomesTrechos(tlstwstr & NomesTrechos, const std::string & pNomeTrecho);

  const ittymapCTrechos& PegaTrechoAtual() 
  { 
	return itTrechoAtual; 
  };

  void TrocaTrecho(const std::string& strTrecho) 
  { 
    bool Achou(false);
    itTrechoAtual = MapaTrechos.begin();

    while(itTrechoAtual != MapaTrechos.end())
    {
       if(itTrechoAtual->first.find(strTrecho) != std::string::npos) break;
       else itTrechoAtual++;
    }
  };

  void InsereTrechoRecemCriado(std::string& NomeTrecho)
  {
    MapaTrechos.emplace(NomeTrecho, NomeTrecho);
  }

  bool TemTrechoAtual() 
  {
    return itTrechoAtual != MapaTrechos.end();
  };

  void RemoveTrecho(const std::string& Trecho)
  {
    if (itTrechoAtual != MapaTrechos.end())
    {
      std::string NomeTrecho(itTrechoAtual->first.substr(itTrechoAtual->first.find_last_of('\\') + 1));

      if (NomeTrecho == Trecho)
      {
        MapaTrechos.erase(itTrechoAtual);
        itTrechoAtual = MapaTrechos.begin();

        return;
      }
    }

    MapaTrechos.erase(Trecho);
  }
};

