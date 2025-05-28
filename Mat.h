// Mat.h: interface for the Mat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAT_H__20ACBB63_51C6_4885_B91C_1D13D8CDF344__INCLUDED_)
#define AFX_MAT_H__20ACBB63_51C6_4885_B91C_1D13D8CDF344__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Matem  
{
public:
  static double UmGrau_Rad;
  static double M_2_PI;
  static double M_PI;
  static double M_PI_2;
  static double M_PI_4;
  static double M_PI_3_2;
  static double M_PI_DIV_180;
  static double PI_GRAUS;
  static double UM_DECIMO_GRAU;
  static double UM_SEGUNDO;
  static double UM_MINUTO;
  static std::string Algarismos;

  Matem();
  virtual ~Matem();

  double Arredonda(double Num,int CasasDec);
  Ponto& Arredonda(Ponto& Ponto,int Decimais);
  inline double gms_g(double AnguloGMS);
  inline double g_gms(double AnguloG);
  double rad_gms(double AnguloRad);
  inline double grau_rad(double AnguloGrau);
  inline Ponto& P_R(const Ponto P);                  //--- #Verificar mudar para referencia  
  inline Ponto& R_P(const Ponto P1,const Ponto P2);  //--- #Verificar mudar para referencia
  inline Ponto& R_P(double x1, double y1, double x2, double y2); //--- retangular para polar
  double CalculaAzimute(const Ponto& P1,const Ponto& P2);
  double CalculaAzimute(double x1, double y1, double x2, double y2);
  std::string strg_gms(double AnguloG);
  double str_gms_rad(std::string& strGMS);      //--- Angulo string em GMS.ss para radianos
  double dou_gms_rad(double douGMS);                  //--- Angulo string em GMS.ss para radianos
  long DoubleToLong(double Valor);
  static int ComparaDoubles(double d1,double d2,int Decimais);   //--- compara 2 deoublesa arredondando na enésima casa decimal
  void FormataDoubleStr(double Num,CString& str,std::string Separadores = ".,",int CasasDec=0);
  bool CalcInterRetaCurva(const Ponto& PAnt,const Ponto& PPost,const Ponto& CC,double Raio,Ponto& S1,Ponto& S2);
  bool CalcInterRetaEspiral(double ls,double s,const Ponto& PAnt,const Ponto& PPost,Ponto& S);
  double CalculaAreaTriangulo(const Ponto& P1,const Ponto& P2, const Ponto& P3);
  double AzTrigToTop(double Az,double Deltax,double Deltay);
  void IrradiaPonto(double Angulo,double x,double y, double s,double Distancia,Ponto& PCalcular);
  static inline double AcertaAngulo(double* Angulo);
  int ENumero(std::string strDado);
  bool VerfPontoNaReta(const Ponto& P1,const Ponto& P2,const Ponto& P3,double Precisao = 0.0005);
  bool Sinalx(double Valor) {  return Valor > 0;};
  double InterpolaY(const Ponto& P1,const  Ponto& P2,const Ponto& P3) { return ( (P2.y - P1.y) / (P2.x - P1.x)) * (P3.x - P1.x) + P1.y;};
  static double Sgn(double x) { return (x < 0) ? -1.0 : 1.0;};
  static double  CalculaZ(const Ponto& pP1, const Ponto& pP2, const Ponto& pP3, Ponto& pPSemZ);
  static bool VerfPontoMaisProximo(const Ponto& P1,const Ponto& P2,const Ponto& PCentro);
  bool CalcDistPontoReta(const Ponto& PR1,const Ponto& PR2, Ponto& PForaReta, double& Distancia);
  std::string FormataCasasEsq(double Num, int CasasInt, int CasasDec, char Carac = ' ');
  int ModDouble(double x, unsigned int Valor) { return (int(x / Valor)); }
  int VerfPontoForaPoligono(const Ponto& PontoVerificar, LPontos& Poligono);
  bool EMultiplo(double Valor, double Multiplo)
  {
    double res(Valor / Multiplo), Frac(res - (int(res)));

    return abs(Frac) < 1.0E-7;
  }
};

#endif // !defined(AFX_MAT_H__20ACBB63_51C6_4885_B91C_1D13D8CDF344__INCLUDED_)
