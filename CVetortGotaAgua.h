class CPontoxyz
{
  public:
  double x,y,z,fi;

  CPontoxyz(double px=0.0, double py=0.0, double pz=0.0,double pfi=0.0) : x(px), y(py), z(pz), fi(pfi)
  {
  }

  CPontoxyz& operator=(const CPontoxyz& RHS)
  {
    x = RHS.x;
    y = RHS.y;
    z = RHS.z; 
  }

  bool operator<(const CPontoxyz& RHS)
  {
    return x < RHS.x ? true : x > RHS.x ? false :
           y < RHS.y ? true : y > RHS.y ? false : 
           z < RHS.z ? true : false ;
  }
};

//typedef std::list<CPontoxyz> tylstCPontoxyz;
//typedef tylstCPontoxyz::iterator ittylstCPontoxyz;

class CVetortGotaAgua// : public CObject
{
  bool Fim;

  double Velocidade,DistTrajetoria,DistAtual,UnidadeMovimento, TamBlocoTom,TamBlocoTomX, TamBlocoTomY;
  lstPontos Trajetoria,VetorAtual;
  Ponto LocalAtual;

  CMMemDC* pDC;

  unsigned int TamanhoTotal, TamanhoAtual;

  itlstPontos itp1RetaTraj, itp2RetaTraj;
  CPontoxyz PosAtual;

  static COLORREF Tons[5];

  //void bresenham2(int x1, int y1, int x2, int y2);
  void Inicia();
  void Move();
  void DistribuiTons();
  void Desenha();
  Ponto CalcCoordxyVetor();
  void PegaProxPontoVetor();
  void DesenhaLinha(Ponto& PAnt,Ponto& PPost, COLORREF& Tom);

  public: 
  
  CVetortGotaAgua(double pVel, lstPontos& pTraj, CMMemDC* ppDC);
  void MoveVetor();

};
