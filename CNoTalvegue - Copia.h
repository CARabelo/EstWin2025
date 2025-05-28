class CNoTalvegue : public Ponto
{
  //--- Cada Nó é composto por um inicio ou um fim de uma lista em comum com outro inicio ou fim de lista
  //--- É necessário juntar as listas nestes nós para que formem apenas uma lista
  //--- Existem quatro tipos de combinações diferentes no nó que podem ser:
  //--- Inicio com inicio => 2 inicios de lista se encontram no mesmo ponto
  //--- Inicio com fim    => O inicio de uma lista se encontra com o fim de outra lista
  //--- Fim com início    => O fim de uma lista se encontra com o início de outra lista
  //--- Fim com fim       => 2 fins de lista se encontram no mesmo ponto 
  //--- A lista de pontos no no (LItPontosNo) possui apenas 2 pontos (os 2 pontos do nó)

  LItLLDeltaSup LItPontosNo;
                              //--- TipoNo contém  a configuração dos 2 pontos que compõem o no:
  int TipoNo;                 //--- Pode ser 00 01 10 e 11 mas so importam os 01 e10 (0 = INICIO da lista, 1 = FIM da lista)
                              //--- 00 INICIO_INICIO 01 FIM_INICIO 10 FIM_INICIO 11 FIM_FIM Bit0= 1o no bit1= 2o no (da esquerda para a direita)
public:

  enum LOCAL{INICIO,FIM}; //--- 0 e 1
  //enum POSICOES{Nenhuma, INICIO_FIM,FIM_INICIO};
  /*
  CNoTalvegue(Ponto& P,int pLocal) : Ponto(P),Local(pLocal) 
  {
    LItPontosNo.emplace_back(P);
  };
  */

  CNoTalvegue(ItLLDeltaSup ItPonto, int pLocal) : Ponto(pLocal ? ItPonto->rbegin()->PCentral : ItPonto->begin()->PCentral), TipoNo(pLocal)
  {
    //--- O no pode estar sendo criado com o inicio ou o fim de uma lista de talvegues
    //--- Se o no foi criado com o início de uma lista o primeiro bit (2^0) fica com 0 senão fica com 1

    if (pLocal == INICIO)   //--- O nó foi criado com o inicio de uma lista? ( O primeiro bit vai pra 0)
    {
      //--- Local pode ser 01 ou 10  Inicio fim ou Fim Inicio
     TipoNo = INICIO;
    }
    else                    //--- Se não foi criado com o fim de ima lista (o primeiro bit vai pra 1)
    {
      TipoNo = FIM;
    }

    LItPontosNo.emplace_back(ItPonto);
  };

  const LItLLDeltaSup& PegaListaPontosNo()const { return LItPontosNo;}

  bool VerfPontoNo(ItLLDeltaSup ItPonto)
  {
    return std::find(LItPontosNo.begin(), LItPontosNo.end(), ItPonto) != LItPontosNo.end();
  };

  void IncluiPontoNo(ItLLDeltaSup ItPonto,int Inicio_Fim)
  {
    LItPontosNo.emplace_back(ItPonto);

    //-- Se o 2o ponto do nó for um inicio o 2o bit (2^1) fica com 0 senão ele fica com 1

    TipoNo &= Inicio_Fim << 1;  //--- Acerta o segundo bit de acordo com o segundo nó da lista
  }

  void Pop_Back() { if(LItPontosNo.size()) LItPontosNo.pop_back(); }
  int PegaTipoNo() const { return TipoNo;}
};

typedef std::set<CNoTalvegue> tysetNosTalvegues;
typedef tysetNosTalvegues::iterator ittysetNosTalvegues;
