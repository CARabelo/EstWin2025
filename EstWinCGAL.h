typedef std::list<Ponto> tylstcponto;
typedef tylstcponto::iterator ittylstcponto;

#define ERRO_CGAL 2
#define CGAL_OK  0

class CMyPoint : public Point
{
public:
  double z;

  CMyPoint(double px, double py, double pz) : Point(px, py), z(pz) {};
  const bool operator<(const CMyPoint& RHS) const
  {
    double dx(this->x() - RHS.x()), dy(this->y() - RHS.y()), dz(z - RHS.z);

    return dx < -1e-4 ? true : dx > 1e-4 ? false :
           dy < -1e-4 ? true : dy > 1e-4 ? false :
           dz < -1e-4 ? true : false;
  }
};

struct stltCMyPoint         //--- Comparação em 2 dimensões
{
  bool operator()(const CMyPoint& P1, const CMyPoint& P2) const
  {
    double dx(P1.x() - P2.x()),dy(P1.y() - P2.y());

    return dx < -1e-4 ? true : dx > 1e-4 ? false :
           dy < -1e-4 ? true : false;
  }
};

class CPoint3D
{
public:
  enum ENUTIPOPONTO { PONTO_SUPERF, PONTO_BORDO, PONTO_RESTRICAO, PONTO_INSERIDO_DELAUNAY, PONTO_INSERIDO_GABRIEL, PONTO_TEMP };

  CString CSDescricao, CSNome;
  double x, y, z,i,fi,S;
  unsigned int Tipo;

  CPoint3D() {};
  CPoint3D(double px, double py, double pz = 0.0,double pi=DINFINITO,double pfi=DINFINITO,double pS=DINFINITO, int pTipo = PONTO_TEMP,
           const CString& pCstrDescricao = CString(""), const CString& pNomePonto = CString("")) :x(px), y(py), z(pz),i(pi),fi(pfi),S(pS), Tipo(pTipo), CSDescricao(pCstrDescricao), CSNome(pNomePonto) {};
  CPoint3D(const Point& pP) : x(pP.x()), y(pP.y()), z(DINFINITO),i(DINFINITO),fi(DINFINITO),S(DINFINITO),Tipo(PONTO_TEMP) {};
  CPoint3D(const CPoint3D& pP) :x(pP.x), y(pP.y), z(pP.z),i(pP.i),fi(pP.fi),S(pP.S), Tipo(pP.Tipo), CSDescricao(pP.CSDescricao), CSNome(pP.CSNome) {};
  CPoint3D(const CMyPoint& pCMP) :x(pCMP.x()), y(pCMP.y()), z(pCMP.z), i(DINFINITO), fi(DINFINITO), S(DINFINITO) {};

  CPoint3D CPoint3D::operator-(const CPoint3D& RHS) const
  {
    return CPoint3D(x - RHS.x, y - RHS.y, z - RHS.z);
  }

  inline CPoint3D& operator=(const CPoint3D& RHS)
  {
    CSDescricao = RHS.CSDescricao;
    CSNome = RHS.CSNome;
    x = RHS.x;
    y = RHS.y;
    z = RHS.z;
    i = RHS.i;
    fi = RHS.fi;
    S = RHS.S;
    Tipo = RHS.Tipo;

    return *this;
  }
};

struct ltPoint_3D_2D
{
  bool operator()(const CPoint3D& p1, const CPoint3D& p2) const
  {
    double dx(p1.x - p2.x), dy(p1.y - p2.y);

    return dx < -1E-4 ? true : dx > 1e-4 ? false :
           dy < -1e-4 ? true : false;
  }
};

typedef std::set<CPoint3D, ltPoint_3D_2D>  tysetPonto3D;      //--- Set de pontos da superficie
typedef tysetPonto3D::iterator ittystPonto3D;

struct ltPoint_CGAL
{
  bool operator()(const Point& p1, const Point& p2) const
  {
    double dx(p1.x() - p2.x()), dy(p1.y() - p2.y());

    return dx < -1E-4 ? true : dx > 1e-4 ? false :
           dy < -1e-4 ? true : false;
  }
};

struct ltitPoint_3D_2D
{
  bool operator()(const ittystPonto3D it1, const ittystPonto3D it2) const
  {
    double dx(it1->x - it2->x), dy(it1->y - it2->y);

    return dx < -1E-4 ? true : dx > 1e-4 ? false :
           dy < -1e-4 ? true : false;
  }
};

typedef std::set<CMyPoint, stltCMyPoint> tySetPonSuperf;
typedef tySetPonSuperf::iterator ittySetPonSuperf;

typedef std::set<ittystPonto3D, ltitPoint_3D_2D>  tyitsetPonto3D;     //--- Set de iteradores para o set de pontos da superficie
typedef tyitsetPonto3D::iterator ittyitsetPonto3D;

typedef std::list<ittystPonto3D>  tylstitPonto3D;                     //--- Lista de iteradores para o set de iteradores para o set da superficie 
typedef tylstitPonto3D::iterator ittypstPonto3D;

typedef std::list<tylstitPonto3D*> tylstptylstPonto3D;                //--- Lista de ponteiros para a lista de iteradores para o set de iteradores para o set da superficie 
typedef tylstptylstPonto3D::iterator ittylpsetPontos3D;

typedef std::multimap<unsigned int, tylstitPonto3D*> tymmlstplstPonto3D;
typedef tymmlstplstPonto3D::iterator ittymmlstplstPonto3D;
typedef tymmlstplstPonto3D::const_iterator cittymmlstplstPonto3D;
typedef std::pair<unsigned int, tylstitPonto3D*> pairmmlstplstPonto3D;

typedef std::list<std::string> tylstNomeArquivos;
typedef tylstNomeArquivos::iterator tyitlstNomeArquivos;

typedef std::list<std::string> tlstwstr;

typedef std::set<int> tysetint;
typedef tysetint::iterator ittysetint;

