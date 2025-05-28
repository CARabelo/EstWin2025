//--- Estrutura para passagem de parametros das bacias hidrograficas

class CDadosBacia
{
  public:

  double Kc,                //--- Coeficiente de compacidade 
         Kf,                //--- Fator de forma
         Lt,                //--- comprimento total dos talvegues   
         Dd,                //--- Densidade de drenagem
         l,                 //--- Comprimento do canal principal  
         Diagonal,          //--- Maior diagonal da foz
         Area,              //--- Área molhada
         S,                 //--- Declividade equivalente;
         iMTP,              //--- Inclinação média do talvegue principal 
         Tc,                //--- Tempo de concentração
         C1,                //--- coeficiente de forma        
         C2,                //--- coeficiente volumétrico de escoamento
         V,                 //--- Volume de escoamento do hidrograma
         Q,                 //--- Vazao de cheia 
         Qp,                //--- Vazão máxima de projeto 
         VM,                //--- Velocidade média (Manning) 
         RH,                //--- Raio hidráulico
         Qc,                //--- Vazão (metodo continuidade) 
         QMc,               //--- Vazão (Método Manning e continuidade)
         n,                 //--- Coeficiente de rugosidade
         ic,                //--- Intensidade de chuva crítica   
         i,                 //--- Intensidade da chuva 
         t,                 //--- Tempo de duração da chuva 
         T,                 //--- Tempo de retorno da chuva 
         K,                 //--- coeficiente de distribuição espacial da chuva   
         C,                 //--- Coeficiente de escoamento superficial 
         L,                 //--- Comprimento Total das curvas de nível  
         I,                 //--- Declividade média da bacia  
         Am,                //--- Area molhada do canal na foz  
         Perimetro,         //--- comprimento total do espigão   
         Fator_X,           //--- Fator de defluivio - Q -> Ven Te Chow
         Fator_Y,           //--- Fator climático - Q -> Ven te Chow
         Fator_Z,           //--- Fator de Redução do tempo - Q-> Ven te Chow
         Fator_N,           //--- Fator de retardância - Tc -> Kerby-Hathaway
         Fator_K,           //--- FatorAdminsional de características da bacia - Tc -> D.N.O.S.
         CoefRetardo_Teta,  //--- Parâmetro do método racional corrigido       
         h,                 //--- Altura total do talvegue principal
         LCG,               //--- Distancia do CG até a foz 
         CT,                //--- Corficiente armazenamento (Kokei Uehara)
         Im,                //--- Intensidade máxima (Kokei Uehara)
         Tr,                //--- Tempo Retardamento (Kokei Uehara)
         Td,                //--- Tempo de duração da chuva (Kokei Uehara)
         h1,                //--- Intensidade Máxima da chuva (Kokei Uehara)
         h1c,               //--- Intensidade Máxima da chuva corrijida (Kokei Uehara)
         Hexc,              //--- Altura exedente de escoamento (kokei Uehara)
         Vesc,              //--- Volume de escoamento (kokei Uehara)
         Tb,                //--- Tempo base (Kokei Uehara)
         rh,                //--- Raio Hidráulico do canal na foz
         qm,                //--- Parametro hidrograma triangular sintetico
         Pr,                //--- Parametro vegetacao (George Ribeiro)
         I_Canal,           //--- Declividade do canal na foz 
         D_Bueiro;          //--- Diametro mínimo do bueiro na foz  
  int MetodoCalculoQ,       //--- Método de cálculo da vazao
      MetodoCalculoTc,      //--- Metodo de cálculo do tempo de concentração 
      TipoObraFoz,          //--- Tipo de obra na foz, 1- BSTC 2- ponte
      QtdBueiros;           //--- quantidade de bueiros na foz

  std::string NomeBacia;

  unsigned long Balizas; 
  
  CDadosBacia() : Kc(0.0), Kf(0.0), Lt(0.0), Dd(0.0), l(0.0), Diagonal(0.0), Area(0.0),S(0.0), iMTP(0.0),
                  Tc(0.0),i(0.0),C1(0.0),C2(0.0),C(0.0),V(0.0),Q(0.0),Qp(0.0),VM(0.0),RH(0.0),QMc(0.0),n(0.0),
                  K(0.0), ic(0.0),t(0.0),T(0.0),Perimetro(0.0), MetodoCalculoQ(1), MetodoCalculoTc(1),L(0.0),
                  I(0.0), Qc(0.0),Am(0.0),Balizas(0),Fator_X(0.0),Fator_Y(0.0),Fator_Z(0.0),Fator_N(0.0),
                  Fator_K(0.0),CT(0.0), Im(0.0),Tr(0.0),Td(0.0), h1(0.0),h1c(0.0),Hexc(0.0),Vesc(0.0),Tb(0.0),
                  CoefRetardo_Teta(0.0), h(0.0), LCG(0.0), rh(0.0),I_Canal(0.0), TipoObraFoz(1), D_Bueiro(0.0),
                  qm(0.0),Pr(0.0),QtdBueiros(0){}

  enum{ZERO,UNI_KM};

  CArchive& operator>>(CArchive& ar)
  {
    ar << Kc << Kf << Lt << Dd << l << Diagonal << Area << S <<  iMTP << Tc << C1 << C2 << C << V << Q 
       << Qp << VM << RH << Qc << QMc << n << ic << i << t << T << K << C << L << I << Am  << h << LCG << Fator_X << Fator_Y
       << Fator_Z << Fator_N << Fator_K << CoefRetardo_Teta << CT << Im << Tr << Td << h1 << h1c << Hexc << Vesc << Tb << rh << I_Canal
       << Perimetro << qm << Pr << MetodoCalculoQ << MetodoCalculoTc << NomeBacia  << D_Bueiro << TipoObraFoz << QtdBueiros << Balizas;

    return ar;
  }

  CArchive& operator<<(CArchive& ar)
  {
    ar >> Kc >> Kf >> Lt >> Dd >> l >> Diagonal >> Area >> S >> iMTP >> Tc >> C1 >> C2 >> C >> V >> Q
       >> Qp >> VM >> RH >> Qc >> QMc >> n >> ic >> i >> t >> T >> K >> C >> L >> I >> Am >> h >> LCG >> Fator_X >> Fator_Y
       >> Fator_Z >> Fator_N >> Fator_K >> CoefRetardo_Teta >> CT >> Im >> Tr >> Td >> h1 >> h1c >> Hexc >> Vesc >> Tb >> rh >> I_Canal
       >> Perimetro >> qm >> Pr >> MetodoCalculoQ >> MetodoCalculoTc >> NomeBacia >> D_Bueiro >> TipoObraFoz >> QtdBueiros >> Balizas;

    return ar;
  }

  const CDadosBacia& operator=(const CDadosBacia& RHS)
  {
    Kc = RHS.Kc;
    Kf = RHS.Kf;
    Lt = RHS.Lt;
    Dd = RHS.Dd;
    l = RHS.l;
    Diagonal = RHS.Diagonal;
    Area = RHS.Area;
    S = RHS.S;
    iMTP = RHS.iMTP;
    Tc = RHS.Tc;
    C1 = RHS.C1;
    C2 = RHS.C2;
    V = RHS.V;
    Q = RHS.Q;
    Qp = RHS.Qp;
    VM = RHS.VM;
    RH = RHS.RH;
    Qc = RHS.Qc;
    QMc = RHS.QMc;
    n = RHS.n;
    ic = RHS.ic;
    i = RHS.i;
    t = RHS.t;
    T = RHS.T;
    K = RHS.K;
    C = RHS.C;
    L = RHS.L;
    I = RHS.I;
    Am = RHS.Am;
    Perimetro = RHS.Perimetro;
    Fator_X = RHS.Fator_X;
    Fator_Y = RHS.Fator_Y;
    Fator_Z = RHS.Fator_Z;
    Fator_N = RHS.Fator_N;
    Fator_K = RHS.Fator_K;
    CoefRetardo_Teta = RHS.CoefRetardo_Teta;
    h = RHS.h;
    LCG = RHS.LCG;
    CT = RHS.CT;
    Im = RHS.Im;
    Tr = RHS.Tr;
    Td = RHS.Td;
    h1 = RHS.h1;
    h1c = RHS.h1c;
    Hexc = RHS.Hexc;
    Vesc = RHS.Vesc;
    Tb = RHS.Tb;
    rh = RHS.rh;
    qm = RHS.qm;
    Pr = RHS.Pr;
    I_Canal = RHS.I_Canal;
    D_Bueiro = RHS.D_Bueiro;
    MetodoCalculoQ = RHS.MetodoCalculoQ;
    MetodoCalculoTc = RHS.MetodoCalculoTc;
    TipoObraFoz = RHS.TipoObraFoz;
    QtdBueiros = RHS.QtdBueiros;
    NomeBacia = RHS.NomeBacia;
    Balizas = RHS.Balizas;

    return *this;
  }
};
