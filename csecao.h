class CSecao
{
  enum {ESQUERDO,DIREITO,NENHUM,EIXO=2};
  enum {X,Y};
  enum {PROJETO,TERRENO};
  enum {ATUAL=2,TOTAL};
  enum {CALCOK,ERROCANTEIROS1,ERROHPAVIM,ERROCALCBORDOS};

  friend class DesSecao;
  friend class CEstRel;

  double CalcCV(Ponto &PontoProj,Ponto &NoTerrAnte,Ponto &NoTerrPost);
  double CalculaAreaProjecao(Ponto PonAnt,Ponto PonPost);
  void FechaArea(Ponto NoProjAnte,Ponto NoTerrAnte);      //--- Fecha o cálculo das áreas, até a interseção   
  void CalculaTerraplanagemNormal(double HPavim, TaludesPavim& TalPavimento, int Lado, int Tipo, Ponto* pBordo, Ponto* pBordosTerraplanagem);

  double AreaAtual,HPavimento,Gradiente;
  Ponto Intersecao;					 //--- Intersecao atual;
  CLPontos Intersecoes;      //--- Lista das Intersecoes encontras durante o 

public:

  enum {CORTE,ATERRO,ACUMULADOCORTE,ACUMULADOATERRO,MASSAATUALCORTE,MASSAATUALATERRO,EMPOLADOATERRO,MASSAEMPOLATERRO,ACUMEMPOLATERRO,TOTALEMPOLATERRO};    //--- Tipos de volume
  enum {VUE_TERRENO_PRIMITIVO,VUE_PERFIL_GEOLOGICO,VUE_MEDICAO};

  BOOL CalculaOffSet(int  Lado,int TipoPerfil,CLPontos& Talude);

  Perfil Terreno,Projeto,Pavimento,Limpeza;  //---  perfís de projeto, terreno primitivo e pavimentação e limpeza
  Perfil AcidentesTopograficos,AlinhamentosTerrap; 

  CLPontos PontosRemovidosDesenho;

  SecTipo SecaoTipo;
  double Menores[2],Maiores[2],   //--- X e Y menores e maiores (usado nos desenhos).
         AreaTotal[2],            //--- Areas. 
         Area[2],
         Volume[10],CGs[2],         //--- Volumes e centros de gravidade.
         CVs[3],                  //--- Cotas Vermelhas 
         CompLateral,
         Empolamento;
  CEstaca Estaca;                  //--- Valor da estaca.
  Ponto OffSets[2],                //--- OffSets. 
        BordosTerraplanagem[2];    //--- Bordos de terrapalnagem

  //--- Dados para serialização da nota de serviço;
  double E,                 //--- E da parabola   
         SE,                //--- SuperElevaçao
         IAnte,             //--- Rampaanterior 
         IPost;             //--- Rampa posterior

  CSecao(Perfil&	Terreno = Perfil(), Perfil& Projeto=Perfil(),SecTipo& SecaoTip = SecTipo(),Perfil& Limp = Perfil()); 
  CSecao(const CSecao& Secao);
  ~CSecao();              
  void CalculaArea();      //--- Calcula as areas da seção.
  bool CalculaVolumes(const CSecao &Anterior,double Empolamento);   //--- Calcula os volumes da seção
  double CalculaCG();      //--- Calcula o centro de gravicade da seção.
  int CalculaIntersecao(Ponto& NoProjAnte, Ponto& NoProjPost, Ponto& NoTerrAnte, Ponto& NoTerrPost);  
  void PegaCVs(BOOL PAVIMENTACAO=FALSE);
 // BOOL CalculaOffSets(CTaludesCorte& TaludesCorte, CTaludesAterro& TaludesAterro,bool EMedicao = FALSE);
 // BOOL CalculaOffSets(CTaludesCorte& TaludesCorte, CTaludesAterro& TaludesAterro,int Tipo=VUE_TERRENO_PRIMITIVO,CDadosPerfisGeologicos* pStDadosPerfGeologico=NULL);
  BOOL CalculaOffSets(CTaludesCorte& TaludesCorte, CTaludesAterro& TaludesAterro,int Tipo,Ponto* rCVs = NULL,Ponto* pOffSetsE = NULL,Ponto* pOffSetsD=NULL);
  void CotaSecao(double Gradi,double HPavim,const SecTipo& SecaoTipo);
  void GravaNota(CArchive &ar,double E,double SE,double IAnte,double IPost);
  void LeNota(CArchive &ar,double& E,double& SE,double& IAnte,double& IPost);
  void Serialize(CArchive& ar,bool EMedicao = FALSE);
  void DefRetangDes();       //--- Define o retangulo mínimo para desenho (usada nos desenhos/)
  void CalculaIs();
  int Compara(CSecao& No);
  int CalculaBordos(TaludesPavim &TalPavimento,double HPavim,CCanteiros& Canteiros,bool Cancelar=false);
  void AlteraYPontos(double Deltay=INFINITO);
  void ExcluiPonTaludes();
  bool TemAlargEsquerdo() {return Projeto.GetHead().x < SecaoTipo.GetHead().x;};
  bool TemAlargDireito() {return Projeto.GetTail().x > SecaoTipo.GetTail().x;};
  void LimpaVolumes() { for (register int i = CORTE ; i <= MASSAATUALATERRO ; i++) Volume[i] = 0.0;};
  void IncluiListaLimpeza(double Limp,double ExessoLateral,Perfil& TerrenoAtual);
  void LimpaTerreno(double HLimp);
  bool PodeAlterar(double x) const;
  void Limpa() {Constroi();};
  void CalcPrimUltIntersProjTerreno(Ponto* pIntersec1,Ponto* pIntersec2);

//  void IncluiListaLimpeza(double Limpeza,double ExessoLateral,const Perfil& TerrenoAtual);

  void BuscaGradiente(Ponto& p)
  {
    POSITION Atual(Pavimento.GetHeadPosition());

    while (Atual && Pavimento.GetAt(Atual).Descricao != "GRADIE")  Pavimento.GetNext(Atual);

    if (!Atual)  p.x = p.y = INFINITO; 
    else
    {
      p.x = Pavimento.GetAt(Atual).x;
      p.y = Pavimento.GetAt(Atual).y;
    }
  }

  void CSecao::MoveTerrenoY(double dh)
  {
    for(POSITION Atual = Terreno.GetHeadPosition() ; Atual ; Terreno.GetNext(Atual))
    {
      Ponto& PAtual = Terreno.GetAt(Atual);
      PAtual.y += dh;
    }
  }

  void Constroi(Perfil& pTerreno=Perfil(),Perfil& pProjeto= Perfil(),SecTipo& pSecaoTip = SecTipo());

  CSecao& operator=(const CSecao& No);
  bool operator<(const class CSecao& RHS) const { return Estaca < RHS.Estaca; }; 
};
