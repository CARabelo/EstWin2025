#include "math.h"
#include <limits>

#define INFINITO   1.7976931348623158e+308
#define RESOLUCAO 0.0 
#define RESOLUCAO_2 1e-5

class Ponto
{
  enum{X,Y,Z};

public:

  enum BALIZA_PONTO{SEM_BALIZA,PONTO_MARCADO_CN, PONTO_COTADO = 4,ESBOCADO = 4,CURVA_NIVEL = 8, CN_FOI_COTADA = 16, INTER_ACI_TOPOG = 32,FIM_BALIZA_PONTO = 128};    //--- Reservado continua no CEquipDrenagem

  double x,y,z,i,fi,S;
  bool Baliza;                     //--- Usada para balizar operaçoes, (desenhado ou naõ desenhado, etc)
  bool Inserido;                  //--- Baliza antiga
  int Baliza2;
  int AcidenteTopografico;               //--- Usado nos desenhos das seçoes para indicar o tipo de acidente topografico
       
  CString Nome,Descricao;

  Ponto(const Ponto& PIgual);
  Ponto(const Ponto* pIgual);
  Ponto(std::string& pDescr,double OrdX, double OrdY, double OrdZ,int BalizaPontoDGN=0);
  Ponto(double OrdX=INFINITO,double OrdY=INFINITO,double OrdZ=INFINITO,double pi=INFINITO,double ps=INFINITO,double pS=INFINITO,CString& Descr=CString(""),bool pBaliza=false,bool pInserido = false,const CString& pNome=CString(""),int pAciTopog=-1,int pBaliza2=0);

  void VerfNaN() { return;/* if(x != x) x= INFINITO; if(y != y) y= INFINITO; if(z != z) z= INFINITO;*/ }  //--- <=== Implementar na proxima versao

  Ponto& operator+=(const Ponto& P) { x += P.x; y += P.y; if(z!= INFINITO) z += P.z; VerfNaN(); return *this; };
  Ponto& operator+=(const double Deltax) { x += Deltax ;VerfNaN(); return *this; } ;
  Ponto& operator+(const Ponto& P) {static Ponto Pres; Pres.x = x + P.x; Pres.y = y + P.y; if(Pres.z != INFINITO )Pres.z = z + P.z; VerfNaN(); return Pres;}; 
  Ponto& operator-=(const Ponto& P) { x -= P.x ; y -= P.y ;  if(z!= INFINITO) z -= P.z; VerfNaN(); return *this; };
  Ponto& operator=(const Ponto& P) 
  { 
		x = P.x;	y = P.y; z = P.z; i = P.i; fi = P.fi; S = P.S; Nome = P.Nome; Descricao = P.Descricao; 
    Baliza = P.Baliza; Inserido = P.Inserido; AcidenteTopografico = P.AcidenteTopografico;Baliza2 = P.Baliza2; return *this;
  }
  const bool operator<(const Ponto& P) const
  { 
    double Deltax(x - P.x),Deltay(y - P.y);     
      //   Dá pb na cgal....não deveria.
  
     
    return Deltax  < -RESOLUCAO ? true : Deltax > RESOLUCAO ? false :
           Deltay  < -RESOLUCAO ? true : false;
         
  }; 
  const bool operator>(const Ponto& P) const
  { 
    double Deltax(x - P.x),Deltay(y - P.y);     
	
	return Deltax  > 0.0 ? true : Deltax  < 0.0 ? false :
         Deltay  > 0.0 ? true : false;
  }; 

  const bool operator==(const Ponto& P) const 
  { 
   // return fabs(x - P.x) < RESOLUCAO && fabs(y - P.y) < RESOLUCAO && fabs(z - P.z) < RESOLUCAO;
      return (x == P.x)  && y == P.y  &&  z == P.z;
  };

  const bool operator!=(const Ponto& P) const 
  { 
 //   return !(fabs(x - P.x) < RESOLUCAO && fabs(y - P.y) < RESOLUCAO && fabs(z - P.z) < RESOLUCAO);
    //return (x != P.x) || y != P.y || z != P.z;
    return !(*this == P);
  };

  CArchive& Ponto::operator<<(CArchive& ar)
  { 
    ar >> Nome >> Descricao >> x >> y >> z >> i >> fi >> S >> Baliza >> Baliza2 >> Inserido >> AcidenteTopografico;

    return ar;
  }

  CArchive& Ponto::operator>>(CArchive& ar) const
  {  
    ar << Nome << Descricao << x << y << z << i << fi << S << Baliza << Baliza2 << Inserido << AcidenteTopografico;

    return ar;
  }	

  std::ofstream& operator>>(std::ofstream& ofs) const
  {
    ofs << Nome << ' ' << Descricao << ' ' << x << ' ' << y << ' ' << z ;
    return ofs;
  };

  std::ifstream& operator<<(std::ifstream& ifs) 
  {
    char Espaco,pDescricao[1024]={0};

    ifs >> Nome >> Espaco >> pDescricao >> Espaco >> x >> Espaco  >> y >> Espaco >> z;

    Descricao = pDescricao;

     return ifs;
  };

  std::fstream& operator>>(std::fstream& fs)  const
  {
    //--- Seria o ideal passar todos os atributos !!!!!
	//--- Principalmente o acidente topografico!!!

    fs << Nome << ' ' << y << ' ' << x << ' ' << z << ' ' << Descricao;          //--- Os arquivos de pontos estão com o y primeiro.    
    return fs;

  };

  void RotacionaPlano(double Sin,double Cos,double Angulo);
  Ponto RotacionaPlano(double Sin,double Cos,double Angulo,bool Diferente) const;
  const bool Compara2D(const Ponto& RHS,double Resolucao = RESOLUCAO_2) const 
  { 
    return fabs(x - RHS.x) < Resolucao && fabs(y - RHS.y) < Resolucao;
  };
  void MontaChaves();
};

typedef std::list<Ponto> lstPontos;
typedef lstPontos::iterator itlstPontos;
typedef lstPontos::reverse_iterator ritlstPontos;
typedef lstPontos::const_iterator citlstPontos;

typedef std::list<lstPontos> lstlstPontos;
typedef lstlstPontos::iterator itlstlstPontos;
typedef lstlstPontos::const_iterator citlstlstPontos;
typedef lstlstPontos::reverse_iterator ritlstlstPontos;

typedef std::list<Ponto> tylstpontos;
typedef tylstpontos::iterator ittylstpontos;

typedef std::set<Ponto> setPontos;
typedef setPontos::iterator itsetPontos;

typedef std::list<Ponto> LPontos;
typedef LPontos::iterator ItLPontos;
typedef LPontos::const_iterator ItCLPontos;
typedef LPontos::reverse_iterator ItRLPontos;

typedef std::list<LPontos> LLPontos;
typedef LLPontos::iterator ItLLPontos;
typedef LLPontos::const_iterator ItCLLPontos;

typedef Ponto Vetor;