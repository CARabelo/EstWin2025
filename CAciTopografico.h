typedef std::set<Ponto> setPontos;
typedef setPontos::iterator itsetPontos;
typedef setPontos::const_iterator citsetPontos;
typedef std::pair<itsetPontos,bool> pairitboolsetPontos;
typedef std::pair<citsetPontos, bool> paircitboolsetPontos;

typedef std::list<itsetPontos> lstitsetPontos;
typedef lstitsetPontos::iterator itlstitsetPontos;
typedef lstitsetPontos::const_iterator citlstitsetPontos;
typedef lstitsetPontos::reverse_iterator ritlstitsetPontos;
typedef lstitsetPontos::const_reverse_iterator critlstitsetPontos;

class CNomeDescricao
{
    unsigned int intTipo;
    std::string strNome;

    public:

    CNomeDescricao(int pTipo,const std::string& pNome="") : intTipo(pTipo), strNome(pNome) { };

    unsigned int Tipo() const
    {
      return intTipo;
    }

    const std::string& Nome() const
    {
      return strNome;
    }

    bool operator<(const CNomeDescricao& RHS) const
    {
      return intTipo < RHS.Tipo();
    }

};


typedef std::set<CNomeDescricao> tyvecNomeDescricao;
typedef tyvecNomeDescricao::iterator ittyvecNomeDescricao;

class CAciTopografico
{
public:

  int Tipo;
  double Area,Perimetro,Estaca;

  setPontos SetPontosAciTopog;
  lstitsetPontos LstItPontos;
  CDRetangulo RetanInscrito;
  static tyvecNomeDescricao VecNomeDescricao;
  
  bool Fechada;

  enum {OK,SEM_PONTOS,TALVEGUE_FORA_BACIA,TALVEGUE_DENTRO_BACIA,TALVEGUE_CORTA_BACIA,BACIA_ABERTA,PONTO_FOZ_NO_ESPIGAO};

  enum TIPOSACITOP {
    NENHUM_ACITOPOG,
    MURO, CERCA, CASA, CANTO_MURO, CANTO_CERCA, CANTO_CASA, BORDO, MEIO_FIO, N_A, N_A_PER, ROCHA,
    PONTE, TAMPA_BUEIRO, POSTE, REDE_ELETRICA_BT, REDE_ELETRICA_AT, FRONTEIRA, EIXO, CRISTA, PE, BUEIRO,
    LINHA_FERREA, MATA, FLORESTA, PLANTACAO, ARVORE, ALAGADO, ALINHAMENTO, TALVEGUE, ESPIGAO, RAVINA,
    PVESGOTO, PVGAS, PVELETRICA, REDE_GAS, REDE_ELETRICA, REDE_ESGOTO, SOLEIRA,
    INICIO_TERRAP,
    OFF_SETS, TERRAPLANAGEM, FRONTEIRA_TERRAP, FIM_TERRAP, CRISTA_LIMPEZA, OFFSET_CORTE, OFFSET_ATERRO,
    INICIO_EQUI_DRENAGEM,
    EQUI_BUEIRO, EQUI_VALETA_TERRAPL, EQUI_VALETA_TN, EQUI_BUEIRO_RETANG, EQUI_DESCIDA_DAGUA, EQUI_CAIXA_COLETORA, EQUI_BACIA_DISSIPACAO, EQUI_SARJETA,
    EQUI_TUBULACAO, EQUI_DRENO_PROFUNDO, EQUI_CANALETA_TN, EQUI_CANALETA_TERRAPL, EQUI_EDITAR_DRENAGEM, FIM_EQUI_DRENAGEM = EQUI_EDITAR_DRENAGEM,
    FIM_ACITOPOG
  };

  CAciTopografico(int pTipo = NENHUM_ACITOPOG, double pEstaca = INFINITO) : Tipo(pTipo), Fechada(false), Estaca(pEstaca), Area(INFINITO), Perimetro(INFINITO) {};
  CAciTopografico(const CAciTopografico& pAT);
  ~CAciTopografico(void) {};

  bool InserePonto(const Ponto& pPonto);
  citlstitsetPontos InserePonto(citlstitsetPontos Local, const Ponto& pPonto);
  bool RemovePonto(const Ponto& pPonto);
  citlstitsetPontos AlteraPontoMantemPosicao(citlstitsetPontos& itPosicao, const Ponto& pPonto);
  const lstitsetPontos& PegaLista() const { return LstItPontos; };
  const setPontos& PegaSet() const { return SetPontosAciTopog; };
  const itsetPontos BuscaPonto(const Ponto& pPonto) const { return SetPontosAciTopog.find(pPonto); };
  const CDRetangulo BuscaRetanInscrito() const { return RetanInscrito; };
  int MudaObjeto(int NovoObjeto, bool pFechada) { int Ret = Tipo; Tipo = NovoObjeto; Fechada = pFechada; return Ret; };
  void Limpa() { SetPontosAciTopog.clear(); LstItPontos.clear(); Tipo = NENHUM_ACITOPOG; Fechada = false; };
  void LimpaPontos() { SetPontosAciTopog.clear(); LstItPontos.clear();};
  static const char* Nome(int TipoAciTopog) { return VecNomeDescricao.find(TipoAciTopog)->Nome().c_str(); }
  int PegaTipo() const { return Tipo; };
  double PegaArea() { return Area; };
  void CalculaAreaPerimetro();
  static bool PegaGeneroNomeEquip(int Equip) { return Equip == EQUI_BUEIRO || Equip == EQUI_BUEIRO_RETANG || Equip == EQUI_DRENO_PROFUNDO;};

  void InverteDirecaoGiro()
  {
    LstItPontos.reverse();
    Area = -Area;
  }

  const std::string& PegaNome(int pTipo) 
  {
    if (pTipo > NENHUM_ACITOPOG && pTipo < FIM_ACITOPOG)
    {
       return CAciTopografico::VecNomeDescricao.find(CNomeDescricao(Tipo, ""))->Nome();
    }

    return CAciTopografico::VecNomeDescricao.find(CNomeDescricao(NENHUM_ACITOPOG, ""))->Nome();
  }

  itlstitsetPontos PegaPontoSequencia(const Ponto& pPonto)
  {
    itlstitsetPontos it(LstItPontos.begin());

    while (it != LstItPontos.end() && **it != pPonto) ++it;

    return it;
  }

  itsetPontos AlteraPonto(itsetPontos It,Ponto& NovoValor)
  {
    if(It != SetPontosAciTopog.end())
    {
      itlstitsetPontos ItLista(std::find(LstItPontos.begin(), LstItPontos.end(), It));

      if (ItLista != LstItPontos.end())
      {  
        SetPontosAciTopog.erase(*ItLista);
        return *ItLista = SetPontosAciTopog.insert(NovoValor).first;
      }
    }
    return SetPontosAciTopog.end();
  }

  CArchive& operator<<(CArchive& ar) 
  {
    ar >> Tipo >> Fechada >> Area >> Perimetro >> Estaca;

    size_t QuanPontos(0);
    Ponto PAtual;

    for (ar >> QuanPontos ; QuanPontos > 0 ; --QuanPontos)
    {
      PAtual << ar;
      InserePonto(PAtual);
    }

    return ar;
  }

  CArchive& operator>>( CArchive& ar) 
  {
    ar << Tipo << Fechada << Area << Perimetro << Estaca << LstItPontos.size();

    //--- Tem q gravar a lista para manter a ordem sequencial dos pontos

   for (itlstitsetPontos it = LstItPontos.begin() ; it != LstItPontos.end()  ; ++it)
   {
    (Ponto)(**it) >> ar;
   }

   return ar;
  }

  bool operator<(const CAciTopografico& RHS) const
  {
    return Estaca < RHS.Estaca;
  }


  void Rotaciona(double VUx, double VUy, double Angulo)
  {
    lstPontos NovaLista;

    for (itlstitsetPontos itLst = LstItPontos.begin(); itLst != LstItPontos.end(); ++itLst)
    {
      NovaLista.emplace_back(**itLst);
    }

    SetPontosAciTopog.clear();
    LstItPontos.clear();

    for (itlstPontos it = NovaLista.begin() ; it != NovaLista.end(); ++it)
    {
      it->RotacionaPlano(VUx, VUy,Angulo);
      LstItPontos.emplace_back(SetPontosAciTopog.emplace(*it).first);
    }
  }

  const CAciTopografico& operator=(const CAciTopografico& RHS)
  {
    Limpa();

    for(auto& ItLstPontos : RHS.LstItPontos)
    {
      InserePonto(*ItLstPontos);
    }

    Tipo = RHS.Tipo;
    Fechada = RHS.Fechada;
    RetanInscrito = RHS.RetanInscrito;

    return *this;
  }
};

//typedef std::list<CAciTopografico> tylstCAcitopograficos;
//typedef tylstCAcitopograficos::iterator ittylstCAcitopograficos;
