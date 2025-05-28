/ Mat.cpp: implementation of the Mat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "est.h"
#include "math.h"
#include "cponto.h"
#include <string>
#include <strstream>
#include <iomanip>
#include "mat.h"
#include <string>
#include <algorithm> 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//extern const double M_PI;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


double Matem::M_PI= 3.141592653589793238462;
double Matem::M_2_PI = M_PI * 2.0;
double Matem::M_PI_2	= M_PI / 2.0;
double Matem::M_PI_4 = M_PI / 4.0;
double Matem::M_PI_3_2 = M_PI_2 * 3.0;
double Matem::M_PI_DIV_180 = M_PI / 180.0;
double Matem::UmGrau_Rad = M_PI / 180.0;
double Matem::UM_MINUTO = UmGrau_Rad / 60.0;
double Matem::UM_DECIMO_GRAU =  M_PI_DIV_180 / 10.0;
double Matem::PI_GRAUS = 180.0 / M_PI;
double Matem::UM_SEGUNDO =  M_PI_DIV_180 / 3600.0; 

/*

double Matem::UmGrau_Rad = 0.0174532925199432958;
double Matem::UM_MINUTO = UmGrau_Rad / 60.0; 
double Matem::M_2_PI = 6.2831853071795864769;
double Matem::M_PI= 3.14159265358979323846;
double Matem::M_PI_2 = 1.57079632679489661923;
double Matem::M_PI_4 = 0.78539816339744830911;
double Matem::M_PI_3_2 = 4.71238898038468985769;
double Matem::M_PI_DIV_180 = 0.01745329251994329577;
double Matem::UM_DECIMO_SEGUNDO = M_PI_DIV_180 / 10.0;
*/

std::string Matem::Algarismos = "0123456789";

Matem::Matem() {}

Matem::~Matem() {}

double Matem::Arredonda(double Num,int Decimais) 
{
  double Fator(pow(10.0, -Decimais-1)* 5.0);

  double LimiteNum((Num + Fator) * pow(10.0, Decimais));

  double Resp;

  modf(LimiteNum, &Resp);

  Resp /= (pow(10.0, Decimais));

  return Resp;
}

Ponto& Matem::Arredonda(Ponto& Ponto,int Decimais) 
{
  double Fator(pow(10.0,Decimais));
  Ponto.x = floor(Ponto.x * Fator + .5) / Fator; 
  Ponto.y = floor(Ponto.y * Fator + .5) / Fator; 
  Ponto.z = floor(Ponto.z * Fator + .5) / Fator; 

  return Ponto; 
}

double Matem::gms_g(double AnguloGMS)       //--- GMS para grau
{
  double a,b,c,aux;

  aux = modf(AnguloGMS,&a);
  b = floor((aux+1e-6) * 100.0) ;
  c = (aux - b/100.0) * 10000.0;

  return(a + b / 60.0 + c / 3600.0);
}

double Matem::g_gms(double AnguloG)       //--- Grau para Grau Minuto Segundo
{
  double Graus,Minutos,Segundos,Frac;

  Frac = modf(AnguloG,&Graus);
  Frac = modf(Frac * 60.0,&Minutos);
  Frac = modf(Frac * 60.0,&Segundos);

  return(Graus+Minutos/100.0+(Segundos/*+Frac*/)/10000.0);
}

double Matem::rad_gms(double AnguloRad)        //--- Radianos para graus
{
  return(g_gms(AnguloRad * 180.0 / M_PI));   
}

double Matem::grau_rad(double AnguloGrau)        //--- Grau para Radianos
{
  return(gms_g(AnguloGrau) / 180.0 * M_PI);   
}

Ponto& Matem::P_R(const Ponto P)               //--- Polar para retangular
{
  static Ponto Resp(0,0); 

  Resp.x = sin(P.x) * P.y;
  Resp.y = cos(P.x) * P.y;

  return(Resp);
}

Ponto& Matem::R_P(const Ponto PAnte,const Ponto PPost)   //--- retangular para polar
{
  static Ponto Resp(0,0);
  Ponto Rasc(PPost.x - PAnte.x,PPost.y - PAnte.y);

  Resp.x = CalculaAzimute(PAnte,PPost);
  Resp.y = hypot(Rasc.x,Rasc.y);

  return(Resp);
}

Ponto& Matem::R_P(double x1,double y1,double x2,double y2)   //--- retangular para polar
{
  static Ponto Resp(0, 0);
  Ponto Rasc(x2 - x1, y2 - y1);

  Resp.x = CalculaAzimute(x1,y1,x2,y2);
  Resp.y = hypot(Rasc.x, Rasc.y);

  return(Resp);
}

std::string Matem::strg_gms(double AnguloG)             //--- Angulo em grau para string GMS.ss
{
  double Graus,Minutos,Segundos,Frac;
  bool Negativo = AnguloG < 0.0;
  AnguloG = fabs(AnguloG) + 5.0/3600000;   //--- Arredondamento

  Frac = modf(AnguloG,&Graus);
  Frac = modf(Frac * 60.0,&Minutos);
  Frac = modf(Frac * 60.0,&Segundos);

  Segundos += Frac;

  std::strstream strstreamTemp;

  strstreamTemp << (Negativo ? "-" : " ") 
                << std::setprecision(0) << std::fixed << std::setw(3) 
                << Graus << "° " 
                << FormataCasasEsq(Minutos,2,0,'0')
                << "' " 
                << FormataCasasEsq(Segundos,2,2,'0')
                << "\""
                << std::ends;

  char* pResp(strstreamTemp.str());

  strstreamTemp.freeze(false);

  return pResp;
}

double Matem::str_gms_rad(std::string& strGMS)      //--- Angulo string em GMS.ss para radianos
{
  if(strGMS.length() < 6) 
  {
    strGMS.append(std::string('0',6 - strGMS.length()));
  }

  int i1((int)strGMS.find_first_of(Algarismos)),i2((int)strGMS.find_first_not_of(Algarismos,i1));

  if(i2 == -1)  i2 = (int)strGMS.length()+1; 

  std::string strTemp = strGMS.substr(i1,i2-i1);
  i1 = (int) strGMS.find_first_not_of(Algarismos,++i2);

  if ((i1-i2) == 0) 
  {
    strTemp += '0';
    i2++;
    i1+=2;
  }

  strTemp += strGMS.substr(i2,i1-i2);

  i2 = (int)strGMS.find('.',i1);

  if (i2-i1 == 2)  strTemp += '0';

  strTemp += strGMS.substr(i1+1);

  return gms_g(atof(strTemp.c_str()) / 10000.0) / 180.0 * M_PI;
}

double Matem::dou_gms_rad(double douGMS)      //--- Angulo double GMS.ss para radianos
{
  return grau_rad(gms_g(douGMS));
}

long Matem::DoubleToLong(double Valor)
{
  bool Sinal(false); 

  if(Valor < 0.0) 
  {
    Sinal = true;
    Valor = fabs(Valor);
  }

  double frac = fmod(Valor,1.0),Retorno = Valor - frac;
  if (frac >= 0.5) Retorno += 1.0;

  if (Sinal) Retorno *= -1.0;

  return (long) Retorno;
}

int Matem::ComparaDoubles(double d1,double d2,int Decimais)
{
  double D1(floor(d1*pow(0.0,Decimais)+.5)),D2(floor(d2*pow(10.0,Decimais)+.5)),Dif(D2-D1);

  //--- |Dif| pertence a N (Naturais)

  return Dif > 1.0 ? 1 : Dif < 1.0 ? -1 : 0;   //--- Menor, maior ou igual; 
}

void Matem::FormataDoubleStr(double Num,CString& str,std::string Separadores,int CasasDec)
{
  int Grandeza(log10(Num) + 1);

  std::strstream strstreamTemp;

  strstreamTemp << std::fixed << Num << std::ends;

  std::string Temp(strstreamTemp.str());

  register int i((int)Temp.find('.') - 3);

  for( ; i > 0 ; i -= 3)
    Temp.insert(i,1,Separadores[0]);

  i = (int)Temp.find_last_of('.') + CasasDec;

  if (CasasDec && Temp[i] > '4') Temp[i]++;    

  Temp.replace(Temp.find_last_of('.'),1,1,Separadores[1]);

  Temp.resize(Temp.find_last_of(Separadores[1]) + CasasDec + (CasasDec ? 1 : 0));

  str = Temp.c_str();
}
/*
double Matem::CalculaAzimute(const Ponto& P1,const Ponto& P2)
{
	double Az(atan((P2.x - P1.x) / (P2.y - P1.y)));
  if(Az < 0.0) Az += Matem::M_2_PI; 

  return Az;
}
*/
bool Matem::CalcInterRetaCurva(const Ponto& PAnt,const Ponto& PPost,const Ponto& CC,double Raio,Ponto& S1,Ponto& S2)
{
  Ponto R_PPAntPPost(R_P(PAnt,PPost)),R_PPAntCC(R_P(PAnt,CC));

  double Proj_90(fabs(sin(R_PPAntCC.x-R_PPAntPPost.x) * R_PPAntCC.y));

  if(Proj_90 > Raio) return false;

  double AngInterno(acos(Proj_90 / R_PPAntCC.y)),AngRet(acos(Proj_90 / Raio)),
         AzInter(R_PPAntCC.x - Matem::M_PI + AngInterno - AngRet);

  S1= P_R(Ponto(AzInter,Raio));
  S1.x += CC.x;
  S1.y += CC.y;
  S1.fi =  AzInter;

  AzInter += AngRet * 2.0;

  S2 = P_R(Ponto(AzInter,Raio));
  S2.x += CC.x;
  S2.y += CC.y;
  S2.fi = AzInter;

  return true; 
}

bool Matem::CalcInterRetaEspiral(double ls,double s,const Ponto& PAnt,const Ponto& PPost,Ponto& S)
{
   Ponto R_PPAntPPost(R_P(PAnt,PPost));
   double a(R_PPAntPPost.x),b(PPost.y - (a * PPost.x));

   S.x = (ls * (1.0 - (pow(s,2.0)/10.0 + pow(s,4.0)/ 216.0)) - b) / a;
   S.y = (S.x - PAnt.x) *  a + PAnt.y;

   const Ponto& PXAnt(PAnt.x < PPost.x ? PAnt : PPost),PXPost(PAnt.x > PPost.x ? PAnt : PPost); 

   return (S.x > PXAnt.x && S.x < PXPost.x);  
};

double Matem::CalculaAzimute(const Ponto& PAnt,const Ponto& PPost)
{
  double Deltax(PPost.x - PAnt.x),Deltay(PPost.y - PAnt.y),AZ = atan2(Deltax,Deltay);

  if(AZ < 0.0) return AZ + Matem::M_2_PI;
  if(AZ > Matem::M_2_PI) return AZ - Matem::M_2_PI;

  return AZ;
};

double Matem::CalculaAzimute(double x1,double y1,double x2,double y2)
{
  double Deltax(x2 - x1), Deltay(y2 - y1), AZ = atan2(Deltax, Deltay);

  if (AZ < 0.0) return AZ + Matem::M_2_PI;
  if (AZ > Matem::M_2_PI) return AZ - Matem::M_2_PI;

  return AZ;
};

double Matem::CalculaAreaTriangulo(const Ponto& P1,const Ponto& P2, const Ponto& P3)
{
  return  (((P2.y + P1.y) / 2.0) * (P2.x - P1.x)) +
          (((P3.y + P2.y) / 2.0) * (P3.x - P2.x)) +
          (((P1.y + P3.y) / 2.0) * (P1.x - P3.x));
}

double Matem::AzTrigToTop(double Az,double Deltax,double Deltay)   //--- Passa o azimute de trigonométrico para topográfico
{
   Az = M_PI_2 - abs(Az); 
 
   if(Deltax > 0.0)
   {
     if(Deltay > 0.0) return Az;
     else return M_PI - Az;
   }
   else
   {
     if(Deltay > 0.0) return M_2_PI - Az;
     else return  M_PI + Az;
   }
}
void Matem::IrradiaPonto(double Angulo,double x,double y, double s,double Distancia,Ponto& PCalcular)
{
  double AZ = s + Angulo;

  PCalcular.x =  x + sin(AZ) * Distancia;
  PCalcular.y =  y + cos(AZ) * Distancia;
}

inline double Matem::AcertaAngulo(double* Angulo)
{
  if (*Angulo > M_2_PI) *Angulo -= M_2_PI;
  else if (*Angulo < 0.0) *Angulo += M_2_PI; 

  return *Angulo; 
};

int Matem::ENumero(std::string strDado)
{
  static std::string Algarismos("0123456789");
  static std::string Sinais("+-");

  //--- Remove os brancos

  std::remove(strDado.begin(),strDado.end(),' ');

  if (strDado.size() == 0) return 2;   //--- Vazio  
  
  //-- se o primeiro algarismo não for algarismo nem sinal, retorna FALSE.

  if (Algarismos.find(*strDado.begin()) == std::string::npos && Sinais.find(*strDado.begin()) == std::string::npos) return false;

  //--- remove o ponto e os sinais, tem q ter só um de cada

  int i(0);
  size_t Tam(strDado.size());

  std::remove(strDado.begin(),strDado.end(),'.') ;
  if(Tam > (strDado.size()+1)) return false;
  Tam = strDado.size();
  std::remove(strDado.begin(),strDado.end(),'+');
  if(Tam > (strDado.size()+1)) return false;
  Tam = strDado.size();
  std::remove(strDado.begin(),strDado.end(),'-');
  if(Tam > (strDado.size()+1)) return false;

  //--- o q sobrou tem q ser algarismo

  for (std::string::iterator it = strDado.begin() ; it != strDado.end() ; ++it) 
    if (Algarismos.find(*it) == -1) return false;

  return true;
}

bool Matem::VerfPontoNaReta(const Ponto& P1,const Ponto& P2,const Ponto& P3,double Precisao) 
{
  Ponto CantoInfEsq(P1),CantoSupDir(P1);

  if(CantoInfEsq.x > P2.x) CantoInfEsq.x = P2.x;
  if(CantoInfEsq.y > P2.y) CantoInfEsq.y = P2.y;

  if(CantoSupDir.x < P2.x) CantoSupDir.x = P2.x;
  if(CantoSupDir.y < P2.y) CantoSupDir.y = P2.y;

  //if(P3.x < CantoInfEsq.x || P3.y < CantoInfEsq.y  || P3.x > CantoSupDir.x || P3.y >  CantoSupDir.y) return false; 
  
  double S(((P3.y + P1.y) / 2.0 * (P3.x - P1.x)) +
           ((P2.y + P3.y) / 2.0 * (P2.x - P3.x)) +
           ((P1.y + P2.y) / 2.0 * (P1.x - P2.x)));

  return fabs(S) < Precisao;   //0.005;
}

double Matem::CalculaZ(const Ponto& pP1,const Ponto& pP2,const Ponto& pP3, Ponto& pPSemZ)
{
  if(pP1.z == INFINITO || pP2.z == INFINITO || pP3.z == INFINITO) return INFINITO;
  int Metodo(1);  //--- Metodo vetorial , 0 metodo trigonométrico

  if(Metodo == 1) 
  {
    Ponto Origem(pP1);
   double MaiorCota(pP1.z);

   //--- Organiza os vetores

  if (pP2.x < Origem.x) Origem.x = pP2.x;
  if (pP3.x < Origem.x) Origem.x = pP3.x;

  if (pP2.y < Origem.y) Origem.y = pP2.y;
  if (pP3.y < Origem.y) Origem.y = pP3.y;

  if (pP2.z < Origem.z) Origem.z = pP2.z;
  if (pP3.z < Origem.z) Origem.z = pP3.z;

  if (pP2.z > MaiorCota) MaiorCota = pP2.z;
  if (pP3.z > MaiorCota) MaiorCota = pP3.z;

  Ponto P1(pP1), P2(pP2), P3(pP3), PontoSemCota(pPSemZ);

  Vetor V1(P2.x - P1.x, P2.y - P1.y, P2.z - P1.z),
        V2(P3.x - P1.x, P3.y - P1.y, P3.z - P1.z),
        Vx((V1.y * V2.z) - (V1.z * V2.y), -((V1.x * V2.z) - (V1.z * V2.x)), (V1.x * V2.y) - (V1.y * V2.x));
  double d(Vx.x * P1.x + Vx.y * P1.y + Vx.z * P1.z);

  pPSemZ.z = (-(Vx.x * PontoSemCota.x) - (Vx.y * PontoSemCota.y) + d) / Vx.z;

  //--- no caso de drones (pontos muito proximos está calculando errado) tira a média...VERIFICAR

  if (pPSemZ.z < Origem.z - 0.005 || pPSemZ.z > MaiorCota + 0.005)
  {
    pPSemZ.z = (pP1.z + pP2.z + pP3.z) / 3.0;
  }

  return pPSemZ.z;
  }
  else
  {
    //--- Não foi muito testado
  
    //--- pega os pontos das arestas do triangulo  que cortam reta  y= pPSemZ.y, são sempre 2;
    //--- Calcula o z da reta formada por estes 2 pontos. Este é o z do ponto sem z

    const Ponto* pP[4] = {&pP1,&pP2,&pP3,&pP1}, *pAtual(&pP1);
    Ponto PInter[2];
    int ContaPontosInter(0);

    for (int C = 1; C < 4; C++)
    {
      //--- Verifica se a reta y= pPSemZ.y corta esta aresta do triangulo

      if ((pP[C]->y > pPSemZ.y && pP[C-1] ->y < pPSemZ.y) || (pP[C]->y < pPSemZ.y && pP[C - 1]->y > pPSemZ.y))
      {
        //--- Se corta calcula o ponto de interseção entre a reta y = pPSemZ.y com a aresta atual do triangulo

        //--- Calcula a interseção no plano:

        double i((pP[C]->y - pP[C-1]->y) / (pP[C]->x - pP[C - 1]->x)),    
               Deltay(pPSemZ.y - pP[C-1]->y),
               Deltax(Deltay / i);

        PInter[ContaPontosInter].x = pP[C - 1]->x + Deltax,
        PInter[ContaPontosInter].y = pP[C - 1]->y + Deltay;

        //--- Calcula a cota da interseção

        double dP1PI(sqrt((Deltay*Deltay) + (Deltax * Deltax))),
               DeltaxP1P2(pP2.x-pP1.x),DeltayP1P2(pP2.y-pP1.y),
               dP1P2(sqrt((DeltaxP1P2 * DeltaxP1P2) + (DeltayP1P2 * DeltayP1P2))),
               izP1P2((pP2.z - pP1.z) / dP1P2);
        
        PInter[ContaPontosInter++].z = izP1P2 * dP1PI + pP1.z;
      }
      pAtual = pP[C];
    }

    //--- Calcula a cota de pPSemz na reta y= pPSemZ

    double i((PInter[1].z - PInter[0].z) / (PInter[0].x - PInter[1].x));

    pPSemZ.z = i * (pPSemZ.x - PInter[0].x) + PInter[0].z;

    return pPSemZ.z;
  }
}


bool Matem::VerfPontoMaisProximo(const Ponto& P1, const Ponto& P2, const Ponto& PCentro)
{
  return (hypot(PCentro.x - P1.x,PCentro.y-P1.y) < hypot(PCentro.x - P2.x, PCentro.y - P2.y));
}

bool Matem::CalcDistPontoReta(const Ponto& PR1,const Ponto& PR2, Ponto& PForaReta,double& Distancia)
{
  Distancia = INFINITO;

  Ponto PRPReta(R_P(PR1,PR2)),PRPPonto(R_P(PR1, PForaReta));

  if(PRPReta.y < PRPPonto.y) return false;

  Distancia = sin(PRPPonto.x - PRPReta.x) * PRPPonto.y;

  return true;
}

std::string Matem::FormataCasasEsq(double Num,int CasasInt,int CasasDec,char Carac)
{
  std::stringstream strstrResp;

  int CasasNum(log10(Num)+1);

  for(int i = 0 ; i < (CasasInt-CasasNum) ; i++) strstrResp << Carac;

  strstrResp  << Arredonda(Num, CasasDec);

  return strstrResp.str();
}

int Matem::VerfPontoForaPoligono(const Ponto& PontoVerificar,LPontos& Poligono)
{

  if (Poligono.size() < 3) return 2;   
  //---  ponto estivesse dentro da superfície.     
  register  ItLPontos ItPAnterior(Poligono.begin());
  register ItLPontos ItPAtual(ItPAnterior);
  unsigned int Contador(0);
  double dy(1e-5);

  if (PontoVerificar == *ItPAtual) return false;                //--- Primeiro Ponto Pertence a fronteira tb

  for (++ItPAtual; ItPAtual != Poligono.end(); ++ItPAtual)
  {
    if (PontoVerificar != *ItPAtual)
    {
      //--- Se o ponto estiver no y do ponto a verificar conta 2 x, nesta reta e na proxima
      //--- Esse if evita essa contagem dobrada.

      if (fabs(ItPAtual->y - PontoVerificar.y) < dy) continue;

      //--- @Verificar a necessidade de considerar a area

      if (ItPAnterior->x > PontoVerificar.x || ItPAtual->x > PontoVerificar.x)
      {
        if ((PontoVerificar.y >= (ItPAnterior->y - dy) && PontoVerificar.y <= (ItPAtual->y + dy)) || (PontoVerificar.y <= (ItPAnterior->y + dy) && PontoVerificar.y >= (ItPAtual->y - dy)) ||
            (PontoVerificar.y <= (ItPAnterior->y + dy) && PontoVerificar.y >= (ItPAtual->y - dy)) || (PontoVerificar.y <= (ItPAnterior->y + dy) && PontoVerificar.y >= (ItPAtual->y - dy)))

        {
          ++Contador;
        }
      }
      ItPAnterior = ItPAtual;
    }
    else return false;         //--- Ponto pertence ao poligono
  }

  return (Contador % 2) == 0;
}
