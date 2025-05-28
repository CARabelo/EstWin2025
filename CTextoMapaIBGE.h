class CTextoMapaIBGE
{
  public:

  enum TIPO{NENHUM_TIPO,PONTO_TEXTO, PONTO_CN_MESTRA, PONTO_COTADO=4};
  double x,y,xCentro,yCentro;
  std::string Texto;
  int Tipo;

  CTextoMapaIBGE(double px=INFINITO,double py=INFINITO,double pxCentro=INFINITO, double pyCentro=INFINITO,const std::string& pTexto="") : x(px), y(py), xCentro(pxCentro), yCentro(pyCentro), Texto(pTexto),Tipo(NENHUM_TIPO) {}

  void PegaDados(double& px, double& py, double& pxCent, double& pyCent, std::string& pTexto)
  {
    px = x;
    py = y;
    pxCent = xCentro;
    pyCent = yCentro;
    pTexto = Texto;
  }

  const CTextoMapaIBGE& operator=(CTextoMapaIBGE& RHS)
  {
    x = RHS.x;
    y = RHS.y;
    xCentro = RHS.xCentro;
    yCentro = RHS.yCentro;
    Texto = RHS.Texto;

    return *this;
  }
};

typedef std::list<CTextoMapaIBGE> tylstCTextos;
typedef tylstCTextos::iterator ittylstCTextos;
typedef tylstCTextos::reverse_iterator rittylstCTextos;



