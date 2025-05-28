#define INFINITO   1.7976931348623158e+308

class CDRetangulo
{
  double x1,y1,x2,y2;

public:

  CDRetangulo(double px1=INFINITO,double py1=INFINITO,double px2=-INFINITO,double py2=-INFINITO) : x1(px1),y1(py1),x2(px2),y2(py2) {};
  CDRetangulo(const CDRetangulo& pRet) : x1(pRet.x1),y1(pRet.y1),x2(pRet.x2),y2(pRet.y2) {};
  ~CDRetangulo(void);

  bool operator<(const CDRetangulo& RHS) const
  {
    return x1 < RHS.x1 ? true : x1 > RHS.x1 ? false :
           y1 < RHS.y1 ? true : y1 > RHS.y1 ? false :
           x2 < RHS.x2 ? true : x2 > RHS.x2 ? false :
           y2 < RHS.y2 ? true : y2 > RHS.y2 ? false : true; 
  };

  bool VerfSePontoE(double x,double y) const
  {
    return x < x1 ? false : x > x2 ? false :
           y < y1 ? false : y > y2 ? false : true;
  };

  void AjustaMaior(const Ponto& pPonto);

  void Ajustax(double px1)
  {
    if(px1 == x1) x1 = px1;
    else if(px1 == x2) x2 = px1; 
  }

  double Buscax1() const { return x1;};
  double Buscax2() const { return x2;};
  double Buscay1() const { return y1;};
  double Buscay2() const { return y2;};
};

