//------------------------------------------------------------
#define CIRCULAR 1

typedef std::map<CEstaca,Ponto> MSecaoGeom;
typedef MSecaoGeom::iterator itMSecaoGeom;

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
        Fim,
        K,
        ms;
int     Lado,
        Tipo,
        Caso;

    MSecaoGeom mapSecoesDesenho;  //--- Pontos auxuliares  para desenhar a espiral


    CurHorizontal(const CurHorizontal& CurHor) ;
    CurHorizontal& operator=(const CurHorizontal& CurHor);

    CurHorizontal(const CEstaca& Estaca=CEstaca(),double x=0.0,double y=0.0) : PI(x,y),TS(x,y),SC(x,y),CS(x,y),ST(x,y),CC(x,y),Ac(0.0),
        Fi(0.0),AcCirc(0.0),Raio(0.0),xEsp(0.0),yEsp(0.0),Lc(0.0),q(0.0),p(0.0),
        Ts(0.0),Sl(0.0),Se(0.0),m1(0.0),m2(0.0),Ini(0.0),Fim(0.0),K(0.0),Lado(0),Tipo(CIRCULAR),Caso(0),
        EstTs(Estaca),EstSc(Estaca),EstCs(Estaca),EstSt(Estaca) {};

    void RotacionaPlano(double Sin,double Cos,double Angulo);
    CArchive& operator<<(CArchive& ar);
    CArchive& operator>>(CArchive& ar);
};

struct ltCurHorTS 
{ 
    bool operator()(const CEstaca& e1, const CEstaca& e2) const 
    { 
        return e1 < e2;
    }
};

struct ltCurHorPI 
{ 
    bool operator()(const Ponto* p1, const Ponto* p2) const 
    { 
        return *p1 < *p2;
    }
};

struct ltCurHorCC 
{ 
    bool operator()(const Ponto* p1, const Ponto* p2) const 
    { 
        return *p1 < *p2;
    }
};

typedef std::map<CEstaca,CurHorizontal,ltCurHorTS> MCurHorizontaisTS;            //--- Map das curva  horizontais. ordenadas por ts
typedef MCurHorizontaisTS::iterator ItMCurHorizontaisTS;
typedef MCurHorizontaisTS::const_iterator ItCMCurHorizontaisTS;

typedef std::map<Ponto*,CurHorizontal*,ltCurHorPI> MCurHorizontaisPI;            //--- Map das curva  horizontais. ordenadas por pi
typedef MCurHorizontaisPI::iterator ItMCurHorizontaisPI;
typedef MCurHorizontaisPI::const_iterator ItCMCurHorizontaisPI;

typedef std::map<Ponto*,CurHorizontal*,ltCurHorCC> MCurHorizontaisCC;            //--- Map das curva  horizontais. ordenadas por cc
typedef MCurHorizontaisCC::iterator ItMCurHorizontaisCC;
typedef MCurHorizontaisCC::const_iterator ItCMCurHorizontaisCC;

typedef std::pair<CEstaca,CurHorizontal> pairCHCE;
typedef std::pair<Ponto*,CurHorizontal*> pairpCHPO;

typedef std::pair<ItMCurHorizontaisTS,bool> pairInsMCHTS;
typedef std::pair<ItMCurHorizontaisPI,bool> pairInsMCHPI;
typedef std::pair<ItMCurHorizontaisCC,bool> pairInsMCHCC;
/*
const double M_PI= 3.141592653589793238462;
const double M_2PI = M_PI * 2.0;
const double M_PI_2	= M_PI / 2.0;
*/
const double ERROMINIMO = 0.005;
