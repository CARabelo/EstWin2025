#include "math.h"
#include <limits>

#define INFINITO   1.7976931348623158e+308
#define RESOLUCAO 1e-6 

class PontoAnte
{
  enum{X,Y,Z};

public:

  enum BALIZA_PONTO{SEM_BALIZA,CN_USADA,ESBOCADO = 1<<2,CURVA_NIVEL = 1<<3};

  double x,y,z,i,fi,S;
  bool Baliza;                     //--- Usada para balizar operaçoes, (desenhado ou naõ desenhado, etc)
  bool Inserido;                  //--- Baliza antiga
  int Baliza2;
  int AcidenteTopografico;               //--- Usado nos desenhos das seçoes para indicar o tipo de acidente topografico
       
  CString Nome,Descricao;

  PontoAnte(const PontoAnte& PIgual);
  PontoAnte(const PontoAnte* pIgual);

  PontoAnte(const Ponto& pIgual);
  PontoAnte(const Ponto* pIgual);

  void VerfNaN() { return ;/* if(x != x) x= INFINITO; if(y != y) y= INFINITO; if(z != z) z= INFINITO;*/}  //--- <=== Implementar na proxima versao

  PontoAnte(double OrdX=INFINITO,double OrdY=INFINITO,double OrdZ=INFINITO,double pi=INFINITO,double ps=INFINITO,double pS=INFINITO,CString& Descr=CString(""),bool pBaliza=false,bool pInserido = false,const CString& pNome=CString(""),int pAciTopog=-1,int pBaliza2=0);
  PontoAnte& operator+=(const PontoAnte& P) { x += P.x; y += P.y; if(z!= INFINITO) z += P.z; VerfNaN(); return *this; };
  PontoAnte& operator+=(const double Deltax) { x += Deltax ;VerfNaN(); return *this; } ;
  PontoAnte& operator+(const PontoAnte& P) {static PontoAnte Pres; Pres.x = x + P.x; Pres.y = y + P.y; if(Pres.z != INFINITO )Pres.z = z + P.z; VerfNaN(); return Pres;}; 
  PontoAnte& operator-=(const PontoAnte& P) { x -= P.x ; y -= P.y ;  if(z!= INFINITO) z -= P.z; VerfNaN(); return *this; };
  PontoAnte& operator=(const PontoAnte& P) 
  { 
		x = P.x;	y = P.y; z = P.z; i = P.i; fi = P.fi; S = P.S; Nome = P.Nome; Descricao = P.Descricao; 
    Baliza = P.Baliza; Inserido = P.Inserido; AcidenteTopografico = P.AcidenteTopografico;Baliza2 = P.Baliza2; return *this;
  }
  const bool operator<(const PontoAnte& P) const
  { 
    double Deltax(x - P.x),Deltay(y - P.y);     
      //   Dá pb na cgal....não deveria.
    
    return Deltax  < -RESOLUCAO ? true : Deltax > RESOLUCAO ? false :
           Deltay  < -RESOLUCAO ? true : false;
           
           
    /*
    return Deltax  < 0.0 ? true : Deltax > 0.0 ? false :
           Deltay  < 0.0 ? true : Deltay > 0.0 ? false : false;
           */
           
           
		   
	 /* ESTE É O Q ESTAVA 30-01/2020	   
    return Deltax < -1e-6 ? true : Deltax > 1e-6 ? false :
           Deltay < -1e-6 ? true : false;
		    */




       //    (z - P.z) < -1e-4 ? true : false;
  }; 
  const bool operator>(const PontoAnte& P) const
  { 
    double Deltax(x - P.x),Deltay(y - P.y);     
	
	return Deltax  > 0.0 ? true : Deltax  < 0.0 ? false :
         Deltay  > 0.0 ? true : false;

 /* ESTE É O QUE ESTAVA  
    return Deltax  > 1e-6 ? true : Deltax  < 1e-6 ? false :
           Deltay  > 1e-6 ? true : false;
    //    (z - P.z) < -1e-4 ? true : false;
	 */


    /*
    return Deltax  > 1e-4 ? true : Deltax  < -1e-4 ? false : 
           Deltay  > 1e-4 ? true : false;
       //    (z - P.z) < -1e-4 ? true : false;
       */
  }; 

  const bool operator==(const PontoAnte& P) const 
  { 
    return fabs(x - P.x) < RESOLUCAO && fabs(y - P.y) < RESOLUCAO && fabs(z - P.z) < RESOLUCAO;
  };
  const bool operator!=(const PontoAnte& P) const { return !(fabs(x - P.x) < RESOLUCAO && fabs(y - P.y) < RESOLUCAO && fabs(z - P.z) < RESOLUCAO);};

  CArchive& PontoAnte::operator<<(CArchive& ar)
  { 
    ar >> Nome >> Descricao >> x >> y >> z >> i >> fi >> S >> Baliza >> Inserido >> AcidenteTopografico;

    return ar;
  }

  CArchive& PontoAnte::operator>>(CArchive& ar) const
  {  
    ar << Nome << Descricao << x << y << z << i << fi << S << Baliza << Inserido << AcidenteTopografico;

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

    fs << Nome << ' ' << y << ' ' << x << ' ' << z << ' ' << Descricao;          //--- Os arquivos de PontoAntes estão com o y primeiro.    
    return fs;

  };

  void RotacionaPlano(double Sin,double Cos,double Angulo);
  PontoAnte RotacionaPlano(double Sin,double Cos,double Angulo,bool Diferente) const;
  const bool Compara2D(const PontoAnte& RHS) const { return fabs(x - RHS.x) < RESOLUCAO && fabs(y - RHS.y) < RESOLUCAO;};
  void MontaChaves();
};

typedef std::list<PontoAnte> lstPontoAntes;
typedef lstPontoAntes::iterator itlstPontoAntes;
typedef lstPontoAntes::const_iterator citlstPontoAntes;

typedef std::list<lstPontoAntes> lstlstPontoAntes;
typedef lstlstPontoAntes::iterator itlstlstPontoAntes;
typedef lstPontoAntes::const_iterator citlstlstPontoAntes;

typedef std::list<PontoAnte> tylstPontoAntes;
typedef tylstPontoAntes::iterator ittylstPontoAntes;

typedef PontoAnte VetorAnte;