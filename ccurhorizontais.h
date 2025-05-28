//------------------------------------------------------------
/*
typedef std::map<CEstaca,CurHorizontal,ltCurHorTS> MCurHorizontaisTS;            //--- Map das curva  horizontais. ordenadas por ts
typedef MCurHorizontaisTS::iterator ItMCurHorizontaisTS;
typedef MCurHorizontaisTS::const_iterator ItCMCurHorizontaisTS;

typedef std::map<Ponto*,CurHorizontal*,ltCurHorPI> MCurHorizontaisPI;            //--- Map das curva  horizontais. ordenadas por ts
typedef MCurHorizontaisPI::iterator ItMCurHorizontaisPI;
typedef MCurHorizontaisPI::const_iterator ItCMCurHorizontaisPI;

typedef std::map<Ponto*,CurHorizontal*,ltCurHorCC> MCurHorizontaisCC;            //--- Map das curva  horizontais. ordenadas por ts
typedef MCurHorizontaisCC::iterator ItMCurHorizontaisCC;
typedef MCurHorizontaisCC::const_iterator ItCMCurHorizontaisCC;

typedef std::pair<CEstaca,CurHorizontal> pairCHCE;
typedef std::pair<Ponto*,CurHorizontal*> pairpCHPO;

typedef std::pair<ItMCurHorizontaisTS,bool> pairInsMCHTS;
typedef std::pair<ItMCurHorizontaisPI,bool> pairInsMCHPI;
typedef std::pair<ItMCurHorizontaisCC,bool> pairInsMCHCC;
*/
class CCurHorizontais : public CArqCurHorizontais 
{
  ItMCurHorizontaisTS itAtual;
  CSecTipo &SecoesTipo;
  int Erro;
  double AngRotacao;
  CString NomeProjeto;
 
  CCanteiros Canteiros;

  int EstudaCaso(CurHorizontal &CurvaAtual,CurHorizontal &CurvaAnterior);
  void InsOrdenada(CurHorizontal &No);
  void CalculaCircular(Ponto& PontoACalc,const CEstaca& Estaca,const CurHorizontal& CurvaAtual);
  void inline CalculaTangente(Ponto& PontoACalc,const CEstaca& Estaca,const CurHorizontal& CurvaAtual,int CurvaRef = POSTERIOR);
  void ConstroiEspiralDaCircular(CurHorizontal& CurvaAtual); 
  int CalculaPontosNotaveis();
  void CalculaDadosComplEspiral(CurHorizontal& CurvaAtual);
  bool Consiste();
  double BuscaMenorRampaPista(CEstaca& Estaca,SecTipo& Secao);

  //--- As curvas horizontais ordenadas por ts sao usadas no desenho, as ordenadas por pi e por centro das curvas circulares sao usadas na busca do
  //--- PI ou do Centro mais proximo no movimento do mouse.

  MCurHorizontaisTS mapCurHorizontaisTS; 
  MCurHorizontaisPI mapCurHorizontaisPI;
  MCurHorizontaisCC mapCurHorizontaisCC;

public:

  enum{LACOS_TRANSPASSANDO = 19,AC_MENOR_PERMITIDO,CURVAS_TRANSPASSANDO,CURVA_OK,CURVA_NOK,CURVA_AJUSTADA,DESENV_NAOCONFERE,PI_AJUSTADO};
  enum{CALCULO_OK,ERRO_CRIAR_ARQUIVO_GEO,ERRO_CRIAR_ARQUIVO_CHC,ERRO_CALCULO_CURVA,ERRO_NAO_HA_CH,ERRO_NAO_HA_PN,ALTEROU_ESTACAINI,ERRO_INSERIR_MAPA,CALCULO_NOK,ERRO_LER_ARQUIVO,FALTA_SEC_TIPO};
  enum{PRIMEIRO_RAMO = 1,SEGUNDO_RAMO};

  CCurHorizontais(CString& pNomeArquivo,CSecTipo &SecTipo);
  double BuscaSuperElevacao(CEstaca& Estaca,int& Lado);
  double BuscaSuperLargura(const CEstaca& Estaca);
  void PegaPontosNotaveis(CList<CEstaca,CEstaca&>& PontosNotaveis);
  void PegaEstacasObrigatorias(CList<CEstaca,CEstaca&>& EstObrig,double Equidis,CProjeto* CProjAtual);
  int CalcGeometrico(CString& Descricao,CProgressCtrl* Progresso,void * FFF,double ErroMaximo,CProjeto* CProj,bool SoSeccionar = false);
  int CalculaDadosCurva(CurHorizontal& CurvaAtual,CurHorizontal& CurvaAnterior,CurHorizontal* CurvaPosterior,bool ComAc = false,double ErroMaximo=CEstaca::ERRO_MINIMO);
  void PegaDadosGeometricos(Ponto& Ponto,const CEstaca& EStaca);
  static void CalcxyEsp(const CurHorizontal& CurvaAtual,double *x,double *y,double *s,double ls);
  int VerfErro() {return Erro;};
  bool InsereCurvaNosMapas(CurHorizontal& CurvaAtual);
  static bool CalculaEspiral(int RAMO,Ponto& Ponto,const CEstaca& Estaca,const CurHorizontal& CurvaAtual);
  double PegaAngRotacao() { return AngRotacao;};
  bool BackupArquivos();
  bool VoltaBackupArquivos();
  bool EstaRotacionada() const { return AngRotacao != 0.0;};
  bool RemoveCurvaDosMapas(ItCMCurHorizontaisPI ItCCurvaPIRemover,ItMCurHorizontaisTS ItUltimaCurvaAjustadaTS,CEstaca* pNovaEstCurvaAnterior);
  int CalculaDadosComplementares();

  double LimiteE,LimiteS,LimiteD,LimiteI;

  double PegaLimiteEsq() { return LimiteE;};
  double PegaLimiteDir() { return LimiteD;};
  double PegaLimiteInf() { return LimiteI;};
  double PegaLimiteSup() { return LimiteS;};

  static inline double AcertaAngulo(double* Angulo)
  {
    if (*Angulo > Matem::M_2_PI) *Angulo -= Matem::M_2_PI;
    else if (*Angulo < 0.0) *Angulo += Matem::M_2_PI; 

    return *Angulo; 
  };
  static inline double CalculaAzimute(const Ponto& PPost,const Ponto& PAnte)
  {
    double Deltax(PPost.x - PAnte.x),Deltay(PPost.y - PAnte.y),AZ = atan2(Deltax,Deltay);
    return   AZ < 0.0 ? (AZ + Matem::M_2_PI) : AZ;
  };

  static inline double CalculaAzimuteUTM(const Ponto& PAnte, const Ponto& PPost)
  {
    double Deltax(PPost.x - PAnte.x), Deltay(PPost.y - PAnte.y), AZ = atan2(Deltax, Deltay);
    return   AZ < 0.0 ? (AZ + Matem::M_2_PI) : AZ;
  };

  CEstaca PegaUltimoSt() 
  { 
    return  mapCurHorizontaisTS.size() ? (*(mapCurHorizontaisTS.rbegin())).second.EstSt : INFINITO;
  }

  CEstaca PegaPrimeiroTs() 
  { 
    return  mapCurHorizontaisTS.size() ? (*(mapCurHorizontaisTS.begin())).second.EstSt : INFINITO;
  }

  /*
  void InsereCurva(CurHorizontal& CH) 
  {
     pairCHCE pCHCE(CH.EstTs,CH);
     pairInsMCHTS itCHTS = mapCurHorizontaisTS.insert(pCHCE);
     mapCurHorizontaisPI.insert(pairpCHPO(&pCHCE.second.PI,&pCHCE.second));
     mapCurHorizontaisCC.insert(pairpCHPO(&pCHCE.second.CC,&pCHCE.second));

   //  mapCurHorizontaisCC.insert(*((&(*(mapCurHorizontaisTS.insert(pCHCE)).first))).first);



    AtualizaLimites(CH);
  };
  */

  int PegaQuantCurvas() { return (int) GetCount();}
  const MCurHorizontaisTS& PegaMapCurHorizontaisTS() { return  mapCurHorizontaisTS;};
  const MCurHorizontaisPI& PegaMapCurHorizontaisPI() { return  mapCurHorizontaisPI;};
  const MCurHorizontaisCC& PegaMapCurHorizontaisCC() { return  mapCurHorizontaisCC;};
  int AlteraCurvaPI(ItCMCurHorizontaisPI& ItCurSendoAlteradaPI,Ponto& NovoPI,ItMCurHorizontaisTS& ItUltimaCurvaAjustadaTS,CEstaca* pEstTsCurvaAnterior);
  int AlteraCurvaCC(ItCMCurHorizontaisCC& ItCurSendoAlteradaCC,Ponto& NovoCC,ItMCurHorizontaisTS& ItUltimaCurvaAjustadaTS,double NovoLc = INFINITO,double NovoRaio = INFINITO);
  int RemoveCurva(ItCMCurHorizontaisPI ItConstCurRemHorizPI,ItMCurHorizontaisTS ItUltimaCurvaAjustadaTS,CEstaca* pNovaEstCurvaAnterior);
  void ReEstaqueia(ItMCurHorizontaisTS itSCHInicial,double UltimoSt,ItCMCurHorizontaisPI& ItCSendoAlteradaPI);
  void RotacionaPlano(double Sin,double Cos,double Angulo);
  const CEstaca& CalculaIntersecaoEixo(const Ponto& P1,const Ponto& P2,Ponto& Intersecao);

  CArchive& operator<<(CArchive& ar);
  CArchive& operator>>(CArchive& ar);

  int Serialize(const CString& NomeProj,int Tipo);

  void AtualizaLimites(const CurHorizontal& CH)
  {
    const Ponto& PCentAtual(CH.PI);

    if (PCentAtual.x < LimiteE) LimiteE = PCentAtual.x;
    if (PCentAtual.x > LimiteD) LimiteD = PCentAtual.x;
    if (PCentAtual.y > LimiteS) LimiteS = PCentAtual.y;
    if (PCentAtual.y < LimiteI) LimiteI = PCentAtual.y;
  };

  void RedefineLimites()
  {
    ItCMCurHorizontaisTS itCH(mapCurHorizontaisTS.begin());

    LimiteE = LimiteI = INFINITO;
    LimiteS = LimiteD = -INFINITO;

    for(register size_t QuanPontos = mapCurHorizontaisTS.size(); QuanPontos ; QuanPontos--,itCH++)
    {
      AtualizaLimites(itCH->second);
    }
  };

protected:

  enum {NORMAL,CASO1,CASO2,CASO3};
  enum {ANTERIOR_A_ANTERIOR,ANTERIOR,ATUAL,POSTERIOR,POSTERIOR_A_POSTERIOR,QTD_TOTAL_CURVAS};

  int Compara(CurHorizontal &No1,CurHorizontal &No2);
  int PosicionaNoMaisProximo(const CEstaca& Estaca,int Direcao = FALSE);

};
