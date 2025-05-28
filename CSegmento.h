
class CSegmento
{
public:

    Ponto P1,P2;
    
  const bool operator<(const CSegmento& RHS) const
  {
    return (P1 < RHS.P1 ? true : P1 > RHS.P1 ? false :
            P2 < RHS.P2 ? true : false) ;
  }
  /*
  const bool operator>(const CSegmento& RHS) const
  {
    return (P1 > RHS.P1 ? true : P1 < RHS.P1 ? false :
            P2 > RHS.P2 ? true : false) ;
  }
  */
  CSegmento(const Ponto& pP1,const Ponto& pP2) : P1(pP1),P2(pP2) {};

  ~CSegmento(void) {};
};

