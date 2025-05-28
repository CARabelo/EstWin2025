//typedef std::list<LDeltaSup*> LpLDeltaSup;
//typedef LpLDeltaSup::iterator itLpLDeltaSup;

class CNoTalvegue : public Ponto
{
  //--- Cada Nó de 2 pontos é composto por um inicio ou um fim de uma lista em comum com outro inicio ou fim de lista
  //--- É necessário juntar as listas nestes nós para que formem apenas uma lista
  //--- Existem quatro tipos de combinações diferentes no nó que podem ser:
  //--- Inicio com inicio => 2 inicios de lista se encontram no mesmo ponto
  //--- Inicio com fim    => O inicio de uma lista se encontra com o fim de outra lista
  //--- Fim com início    => O fim de uma lista se encontra com o início de outra lista
  //--- Fim com fim       => 2 fins de lista se encontram no mesmo ponto 
  //--- A lista de pontos no no (LItPontosNo) possui apenas 2 pontos (os 2 pontos do nó)

  LItLLDeltaSup LItPontosNo;
 // LpLDeltaSup LPDSTalvegues;  //--- Para o liga talvegues alinhados surgiu a necessidade de salvaras listas

  int FlagsPontosNo;    //--- TipoNo contém  a configuração dos pontos que compõem o no:
                        //--- Cada ponto na lista do no tem um flag aqui para indicar se o ponto sta no começo ou no fim mda lista
                        //--- A ordem dos pontos na lista é a mesm dos bita aqui, por exemplo Primeir o ponto da lista bit0
                        //--- segundo ponto da lista bit 1 terceiro ponto dalista bit 3 e aasim por diante.
public:

  LItLLDeltaSup::const_iterator itListaNonoAtual;
  int N1;
  Ponto NoAnterior;
  bool Visitado;   //--- Para nao voltar no mesmo nó e ficar em loop

  enum LOCAL{INICIO,FIM}; 

  CNoTalvegue(const Ponto& P) : Ponto(P), FlagsPontosNo(0),N1(0),Visitado(false){};

  CNoTalvegue(ItLLDeltaSup ItPonto, int pLocal) : Ponto(pLocal ? ItPonto->rbegin()->PCentral : ItPonto->begin()->PCentral), FlagsPontosNo(pLocal),N1(0),Visitado(false)
  {
    //--- O nó pode estar sendo criado com o inicio ou o fim de uma lista de talvegues
    //--- Se o nó foi criado com o início de uma lista o primeiro bit (2^0) fica com 0 senão fica com 1
	
    if (pLocal == INICIO)   //--- O nó foi criado com o inicio de uma lista? ( O primeiro bit vai pra 0)
    {
      //--- Local pode ser 01 ou 10  Inicio fim ou Fim Inicio
      FlagsPontosNo = INICIO;
    }
    else                    //--- Se não foi criado com o fim de ima lista (o primeiro bit vai pra 1)
    {
      FlagsPontosNo = FIM;
    }
    LItPontosNo.emplace_back(ItPonto);
  };

  const LItLLDeltaSup& PegaListaPontosNo()const { return LItPontosNo;}
  
  void TrocaInicio(ItLLDeltaSup ItNovoNo)
  {
    LItPontosNo.pop_front();
    LItPontosNo.push_front(ItNovoNo);
  }

  void TrocaFim(ItLLDeltaSup ItNovoNo)
  {
    LItPontosNo.pop_back();
    LItPontosNo.push_front(ItNovoNo);
  }

  bool VerfPontoNo(ItLLDeltaSup ItPonto)
  {
    return std::find(LItPontosNo.begin(), LItPontosNo.end(), ItPonto) != LItPontosNo.end();
  };

  void IncluiPontoNo(ItLLDeltaSup ItPonto,int Inicio_Fim)
  {
    FlagsPontosNo |= Inicio_Fim << LItPontosNo.size();  //--- Cada bit contem se o ponto da lista é inicio ou fim

    LItPontosNo.emplace_back(ItPonto);

    //-- Se o 2o ponto do nó for um inicio o 2o bit (2^1) fica com 0 senão ele fica com 1

    // TipoNo &= Inicio_Fim << 1;  //--- Acerta o segundo bit de acordo com o segundo nó da lista
  }

  void Pop_Back() { if(LItPontosNo.size()) LItPontosNo.pop_back(); }
  int PegaFlagsPontosNo() const { return FlagsPontosNo;}
};

typedef std::set<CNoTalvegue> tysetNosTalvegues;
typedef tysetNosTalvegues::iterator ittysetNosTalvegues;

typedef std::map<Ponto, CNoTalvegue> tymapNosTalvegues;
typedef tymapNosTalvegues::iterator ittymapNosTalvegues;
typedef std::pair<Ponto, CNoTalvegue> typairmapNosTalvegues;

typedef std::list< ittymapNosTalvegues> tylstittymapNosTalvegues;

