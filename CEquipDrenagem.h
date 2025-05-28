class CEquipDrenagem : public CAciTopografico
{
  double Estaca;
  double a,b,c,d,e,f,g;           //--- Dimensões
  std::string Nome;
  int QtdDimensoes;
  int ContNomes;
  lstlstPontos Alinhamentos;
  Ponto pCoordEixo,Aux[2][2];
  double Azimute,dEsconsidade;
  unsigned int TipoEquip; 

  public:

    enum Indicadores { NENHUM_INDICADOR=128, PONTO_EIXO=256, PONTO_ALA_ESQUERDA=512, PONTO_ALA_DIREITA=1024 };      //--- Atenção===>>> continuação do Ponto::FLAGS

    //--- EQUIP_AGREGADO    ----> O equipamento precisa estar agregado a outro equipamento
    //--- EQUIP_ALINHAMENTO ----> O equipamento está alinhado com algum alinhamento da terraplanagem
    //--- EQUIP_SOLTO       ----> O equipamento não esta agregado a nada e pode se posicionado livremente no projeto

    enum TipoEquip{NENHUM_EQUIP,EQUIP_AGREGADO,EQUIP_ALINHAMENTO,EQUIP_SOLTO};

    unsigned int Indicadores;

    itsetPontos ItPontoPego;    //--- aponta para ponto do set de pontos que foi pego pelo mouse

    CEquipDrenagem(int pTipo = NENHUM_ACITOPOG, std::string pNome = "",double pEstaca = INFINITO,double Az=INFINITO,double pa=0.0, double pb=0.0, 
                   double pc= 0.0, double pd= 0.0, double pe= 0.0, double pf=0.0, double pg=0.0): 
                   CAciTopografico(pTipo,pEstaca),Azimute(Az),Nome(pNome), a(pa), b(pb), c(pc), d(pf), e(pe), f(pf), g(pg),QtdDimensoes(0), ContNomes(0),
                   ItPontoPego(SetPontosAciTopog.end()),TipoEquip(EQUIP_SOLTO), Estaca(INFINITO),dEsconsidade(0.0),Indicadores(NENHUM_INDICADOR)
    {
      switch (Tipo)
      {
        case CAciTopografico::EQUI_SARJETA:
        {
          //--- a = Altura - b = largura 1 - c = largura 2 - d = expessura

          a = 1.0;
          b = 0.10;
          c = 0.30;
          d = 0.20;

          QtdDimensoes = 4;

          TipoEquip = EQUIP_ALINHAMENTO;
        }
        break;

        case CAciTopografico::EQUI_VALETA_TERRAPL: 
        case CAciTopografico::EQUI_VALETA_TN:
        {
          a = b = c = d = e = f = 0.20;

          QtdDimensoes = Tipo == EQUI_VALETA_TERRAPL ? 6 : 4;

          if(TipoEquip == CAciTopografico::EQUI_VALETA_TERRAPL) TipoEquip = EQUIP_ALINHAMENTO;
        }
        break;
        case CAciTopografico::EQUI_CANALETA_TERRAPL:
        case CAciTopografico::EQUI_CANALETA_TN:
        {
          a = 0.30;

          QtdDimensoes = 1;

          if(Tipo == CAciTopografico::EQUI_CANALETA_TERRAPL)
          {
            b = 0.50;
            QtdDimensoes = 2;
          }

          if (TipoEquip == CAciTopografico::EQUI_CANALETA_TERRAPL) TipoEquip = EQUIP_ALINHAMENTO;
        }
        break;

        case CAciTopografico::EQUI_BUEIRO:
        {
          //--- a = Qtd Elementos - b = Tipo: 0 circular 1 retangular - c = Largura - d = altura - e espacamento - f = Leito - g = ang. ala

          a = c = d = 1.0;                        
          b = 0.0;                       
          e = 0.20;
          f = 0.20;
          g = 45.0;

          QtdDimensoes = 7;
        }
        break;
        case CAciTopografico::EQUI_BUEIRO_RETANG:
        {
          //--- a = Qtd Elementos - b = Tipo: 0 circular 1 retangular - c = Largura - d = altura - e Parede - f = Leito - g = ang. ala

          a = b = c = d = 1.0;
          e = 0.20;
          f = 0.20;
          g = 45.0;

          QtdDimensoes = 7;
        }
        break;
        case CAciTopografico::EQUI_DESCIDA_DAGUA:
        {
          //--- a = Largura b =  piso degrau  c = altura Degrau - d Expessura

          a = 1.0;
          b = 0.50;
          c = 0.33;
          d = 0.08;

          QtdDimensoes = 4;
        }
        break;
        case CAciTopografico::EQUI_TUBULACAO:
        {
          //--- a = quantidade elementos b = diametro c = espaco d = leito

          a = b = 1.0;
          c = 0.10;
          d = 0.0;
         
          QtdDimensoes = 3;
        }
        break;
        case CAciTopografico::EQUI_DRENO_PROFUNDO:
        {
          //--- a = quantidade elementos b = diametro c = espaco d = leito

          a = 1.0;
          b = 0.20;
          c = 0.05;
         
          QtdDimensoes = 3;
        }
        break;
        case CAciTopografico::EQUI_CAIXA_COLETORA:
        {
          //--- a = largura - b = comprimento c = expessura 

          a = 1.0;
          b = 0.08;
                   
          QtdDimensoes = 2;

          TipoEquip = EQUIP_AGREGADO;
        }
        break;
        case CAciTopografico::EQUI_BACIA_DISSIPACAO:
        {
          a = 1.50;
          b = 2.0;
          c = 0.08;
          d = e = f = g = 0.0;      
          
          QtdDimensoes = 3;

          TipoEquip = EQUIP_AGREGADO;
        }
        break;
      }
    }

    void IncrementaNome()
    {
      size_t Pos(Nome.find_last_of('_'));

      Nome = Nome.substr(0, Nome.find_last_of('_'));

      std::stringstream strTemp(Nome);
      strTemp << std::setfill('0') << std::setw(2);

      strTemp << ++ContNomes;

      Nome = strTemp.str();
    }

    lstlstPontos& PegaAlinhamentos() { return Alinhamentos;}

    const std::string& PegaNome() const {return Nome;}

    CArchive& operator<<(CArchive& ar)
    {
      CAciTopografico::operator<<(ar);

      ar >> a >> b >> c >> d >> e >> f >> g >> ContNomes >> Azimute >> Tipo >> dEsconsidade >> Nome >> TipoEquip >> Estaca >> QtdDimensoes;

      pCoordEixo << ar;

      Alinhamentos.clear();
      size_t QtdAlinhamentos(0);

      for (ar >> QtdAlinhamentos; QtdAlinhamentos; --QtdAlinhamentos)
      {
        itlstlstPontos itAlinAtual(Alinhamentos.insert(Alinhamentos.end(),lstPontos()));

        size_t QtdPontos(0);

        for (ar >> QtdPontos ; QtdPontos ; --QtdPontos)
        {
          Ponto& PAtual = *itAlinAtual->insert(itAlinAtual->end(),Ponto());

          PAtual << ar;
        }
      }

      return ar;
    }

    CArchive& operator>>(CArchive& ar) 
    {
      CAciTopografico::operator>>(ar);

      ar << a << b << c << d << e << f << g << ContNomes << Azimute << Tipo << dEsconsidade << Nome << TipoEquip << Estaca << QtdDimensoes;

      pCoordEixo >> ar;

      ar << Alinhamentos.size();

      for (auto& rAlinhamentos : Alinhamentos)
      {
        ar << rAlinhamentos.size();

        for (auto& Ponto : rAlinhamentos)
        {
          Ponto >> ar;
        }
      }

      return ar;
    }

    bool operator<(const CEquipDrenagem& RHS) const
    {
      return pCoordEixo < RHS.pCoordEixo ? true : Nome < RHS.Nome;

     //  return Nome < RHS.Nome;
    }

    bool operator==(const CEquipDrenagem& RHS) const
    {
      return pCoordEixo == RHS.pCoordEixo && Nome == RHS.Nome;
    }

    CEquipDrenagem& operator=(const CEquipDrenagem& RHS)
    {
      CAciTopografico::operator=(RHS);

      a = RHS.a;
      b = RHS.b;
      c = RHS.c;
      d = RHS.d;
      e = RHS.e;
      f = RHS.f;
      g = RHS.g;

      Indicadores = RHS.Indicadores;
      dEsconsidade = RHS.dEsconsidade;

      Tipo = RHS.Tipo;

     return *this;
    }

    void PegaDimensoes(double* pDoubleInicial) const
    {
      *pDoubleInicial++ = a;
      *pDoubleInicial++ = b;
      *pDoubleInicial++ = c;
      *pDoubleInicial++ = d;
      *pDoubleInicial++ = e;
      *pDoubleInicial++ = f;
      *pDoubleInicial++ = g;
    }

    void PassaDimensoes(double* pDoubleInicial)
    {
      a = *pDoubleInicial++;
      b = *pDoubleInicial++;
      c = *pDoubleInicial++;
      d = *pDoubleInicial++;
      e = *pDoubleInicial++;
      f = *pDoubleInicial++;
      g = *pDoubleInicial++;
    }

    void Dimensiona(double pA, double pB, double pC, double pD, double pE, double pF)
    {
      a = pA;
      b = pB;
      c = pC;
      d = pD;
      e = pE;
      f = pF;
    }

    void MudaTipoBueiro(int NovoTipo) { TipoEquip = NovoTipo;};

    int PegaQtdDimensoes() const { return QtdDimensoes;}
    auto& PegaAuxiliares() { return Aux;}
    double& PegaEstacaEixo() { return Estaca;}
    void DefineDegraus();
    void DefineQuadriculado();
    double Az(){ return Azimute;}
    void Az(double pAz) { Azimute = pAz;}
    double& Esconsidade() { return dEsconsidade;}
    void MudaTipo(int NovoTipo){ Tipo = NovoTipo;}
    int PegaTipoEquip() {return TipoEquip;}
    Ponto& CoordEixo() { return pCoordEixo;} 

    static bool EquipAgregado(int Equip) { return Equip == CAciTopografico::EQUI_BACIA_DISSIPACAO || 
                                                  Equip == CAciTopografico::EQUI_CAIXA_COLETORA;}

    static bool EquipAlinhado(int Equip) { return Equip == CAciTopografico::EQUI_VALETA_TERRAPL ||
                                                  Equip == CAciTopografico::EQUI_CANALETA_TERRAPL ||
                                                  Equip == CAciTopografico::EQUI_SARJETA;}

    void RotacionaPlano(double VUx, double VUy, double Angulo);

    void CalcDistanciasAlasBueiro()
    {
      //--- Bueiro tem eixo então calcula a distância até o eixo para que o lado esquerdo fique negativo 
      //--- Os pontos em LstItPontos estão na ordem correta, da esquerda para a direita
      //--- As distancias até o eixo ficaram em S do ponto portanto não corrompem o std::set

      Ponto& PAlaEsq(const_cast<Ponto&>(**LstItPontos.begin()));
      Ponto& PAlaDir(const_cast<Ponto&>(**LstItPontos.rbegin()));

      PAlaEsq.S = -hypot((*LstItPontos.begin())->x - CoordEixo().x,(*LstItPontos.begin())->y - CoordEixo().y),
      PAlaDir.S = hypot((*LstItPontos.rbegin())->x - CoordEixo().x, (*LstItPontos.rbegin())->y - CoordEixo().y);

      PAlaEsq.Baliza2 = PONTO_ALA_ESQUERDA;
      PAlaDir.Baliza2 = PONTO_ALA_DIREITA;
    }
};

typedef std::map<std::string,CEquipDrenagem> tymapEquipDrenagem; 
typedef tymapEquipDrenagem::iterator ittymapEquipDrenagem;
typedef tymapEquipDrenagem::const_iterator cittymapEquipDrenagem;
typedef std::pair< ittymapEquipDrenagem,bool> typairmapEquipDrenagem;


