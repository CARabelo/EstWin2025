#define CIRCULAR 1

class CurHorizontal
{
	public:
	
  CEstaca EstTs,
		      EstSc,
		      EstCs,
		      EstSt,
					EstPc,
					EstPt;
	Ponto   TS,                  
		      SC,
					CS,
					ST,
					PI,
          CC;
	double  Ac,
		      Fi,
					AcCirc,
		      Raio,
					xEsp,
					yEsp,
		      Lc,
					q,
					p,
          Ts,
		      Sl,
		      Se,
		      m1,
		      m2,
		      Ini,
		      Fim;
  int     Lado,
	        Tipo,
          Caso;

  int     Erro;

 	CurHorizontal(const CurHorizontal& CurHor) ;
	CurHorizontal& operator=(const CurHorizontal& CurHor);
  CurHorizontal(const CEstaca& Estaca=CEstaca(),double x=0.0,double y=0.0) : PI(x,y),TS(x,y),SC(x,y),CS(x,y),ST(x,y),CC(x,y),Ac(0.0),
		      Fi(0.0),AcCirc(0.0),Raio(0.0),xEsp(0.0),yEsp(0.0),Lc(0.0),q(0.0),p(0.0),
          Ts(0.0),Sl(0.0),Se(0.0),m1(0.0),m2(0.0),Ini(0.0),Fim(0.0),Lado(0),Tipo(CIRCULAR),Caso(0),
          EstTs(Estaca),EstSc(Estaca),EstCs(Estaca),EstSt(Estaca) {};

  CArchive& operator<<(CArchive& ar);
  CArchive& operator>>(CArchive& ar);

  void inline RotacionaPlano(double VUx,double VUy,double Angulo);
  int Consiste();

  bool AlteraCurvaRaio(CurHorizontal& Curva,double RaioAnterior);
};

