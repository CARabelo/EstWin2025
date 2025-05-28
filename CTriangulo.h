
class CTriangulo
{
  public:

  Ponto P1, P2, P3;

  CTriangulo(const Ponto&pP1,const Ponto& pP2,const Ponto&pP3):P1(pP1), P2(pP2), P3(pP3) {}

  bool operator<(const CTriangulo& RHS) const
  {
    return P1 < RHS.P1 ? true : P1 > RHS.P1 ? false :
           P2 < RHS.P2 ? true : P2 > RHS.P2 ? false :
           P3 < RHS.P3 ? true : false;
  }
};

typedef std::set<CTriangulo> tysetCTriangulo;
typedef tysetCTriangulo::iterator ittysetCTriangulo;

typedef std::list<ittysetCTriangulo> tylstitsetCTriangulo;
typedef tylstitsetCTriangulo::iterator ittylstitsetCTriangulo;

struct ltCTriaP1
{
  bool operator()(const ittysetCTriangulo it1,  const ittysetCTriangulo it2) const  
  {
    return it1->P1 < it2->P1;
  }
};

struct ltCTriaP2
{
  bool operator()(ittysetCTriangulo it1, ittysetCTriangulo it2) const
  {
    return it1->P2 < it2->P2 ;
  }
};

struct ltCTriaP3
{
  bool operator()(ittysetCTriangulo it1, ittysetCTriangulo it2) const
  {
    return it1->P3 < it2->P3;
  }
};

typedef std::multiset<ittysetCTriangulo, ltCTriaP1> tysetCTrianguloP1;
typedef std::multiset<ittysetCTriangulo, ltCTriaP2> tysetCTrianguloP2;
typedef std::multiset<ittysetCTriangulo, ltCTriaP3> tysetCTrianguloP3;
