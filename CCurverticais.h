#define CURVER_PONMOUSE 1
#define CURVER_VALOR_Y  2
//------------------------------------------------------------

class CurVertical
{
  int Erro;

public:

  CEstaca PCV,
    PIV,
    PTV;
  double Y1,
    Y2,
    I1,
    I2,
    E,
    I0;

  CurVertical(double x =0.0, double y = 0.0,int ig = 0,double y1=INFINITO,double y2=INFINITO,CString Desc = "",double pI0=INFINITO);
  double e(double Estaca,int Lado);
  bool OK;
  inline double CalculaE();
  inline double CalculaCotaPCV();
  inline double CalculaCotaPTV();
  int Consiste();
  bool operator==(const CurVertical& RHS) ;
  int verfOK () { return Erro; };

  CString Erros[20];

};

//------------------------------------------------------------

class CCurVerticais : public CArqCurVerticais
{
  POSITION Atual;
  double LimiteEsq,LimiteInf,LimiteSup,LimiteDir;
  int Erro;

public:

  int VerfErro() { return Erro;};

  CList<CurVertical,CurVertical&> LCurvasVerticais;

  CCurVerticais(CString& NomeArquivo);
  ~CCurVerticais();
  double CalculaCota(CEstaca& Estaca,double &E,double& IAnte,double& IPost);
  double CalculaCota(CEstaca& Estaca);
  void InsOrdenada(CurVertical& No);
  int Compara(CurVertical &No1,CurVertical &No2);
  int PosicionaNoMaisProximo(CEstaca Estaca,int Direcao = 0,bool DESDEOINICIO=false);
  double PegaPontoNotavel(CEstaca &Anterior);
  void PegaPontosNotaveis(CList<CEstaca,CEstaca&>& PontosNotaveis);
  void PegaEstacasObrigatorias(CList<CEstaca,CEstaca&>& EstObrig,double Equidis,CProjeto* ProjAtual=NULL);
  POSITION PosicionaPontoNotavel(double x,double y,double Toleranciax,double Toleranciay,int& Tipo);
  //		void AlteraPIV(Ponto& PIVAtual, POSITION P);
  bool AlteraPIV(const CEstaca& Est, POSITION P);
  //void AlteraPIV(const CString& Estaca,const CString& Ig,POSITION PAtual);
  const POSITION PegaAtual() { return Atual; };
  void RemovePIV(POSITION Pos);
  void AlteraI1(const CurVertical& PIV,double Valor,POSITION No=NULL);
  void AlteraI2(const CurVertical& PIV,double Valor,POSITION No=NULL);
  void AlteraCota(const CEstaca& EstPIV,POSITION No=NULL);
  void AlteraEstaca(const CEstaca& Est,const CEstaca& EstAnterior, POSITION No = NULL);
  void AlteraEstaca(double Valor, int Ig, POSITION No = NULL);
  bool AlteraY1(const CEstaca& EstPIV,double Valor,int Tipo = CURVER_VALOR_Y, POSITION No = NULL);
  bool AlteraY2(const CEstaca& EstPIV,double Valor,int Tipo = CURVER_VALOR_Y,POSITION No = NULL);
  int Consiste();
  bool VerfInsercao(const CurVertical& CurvaInserir);

  BOOL Serialize();
  CCurVerticais& operator=(CCurVerticais& CurAIgualar);

  void AtualizaLimites(CurVertical& CurvaTemp)
  {
    if (CurvaTemp.PIV.EstVirtual < LimiteEsq) LimiteEsq = CurvaTemp.PIV.EstVirtual;
    if (CurvaTemp.PIV.EstVirtual > LimiteDir) LimiteDir = CurvaTemp.PIV.EstVirtual;
    if (CurvaTemp.PIV.Cota > LimiteSup) LimiteSup = CurvaTemp.PIV.Cota;
    if (CurvaTemp.PIV.Cota < LimiteInf) LimiteInf = CurvaTemp.PIV.Cota;
  };

  double PegaLimiteEsq() { return LimiteEsq;};
  double PegaLimiteDir() { return LimiteDir;};
  double PegaLimiteInf() { return LimiteInf;};
  double PegaLimiteSup() { return LimiteSup;};

protected:

  CString NomeArquivo;
};
