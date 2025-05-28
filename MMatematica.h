// MMatematica.h: interface for the MMatematica class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MMATEMATICA_H__20ACBB63_51C6_4885_B91C_1D13D8CDF344__INCLUDED_)
#define AFX_MMATEMATICA_H__20ACBB63_51C6_4885_B91C_1D13D8CDF344__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MMatematica  
{
  public:
	  MMatematica();
	  virtual ~MMatematica();

    double Arredonda(double Num,int CasasDec);
    double gms_g(double AnguloGMS);
    double g_gms(double AnguloG);
    double rad_gms(double AnguloRad);
    double gms_rad(double AnguloGMS);
    Ponto& P_R(const Ponto P);
    Ponto& R_P(const Ponto P1,const Ponto P2);
};

#endif // !defined(AFX_MMATEMATICA_H__20ACBB63_51C6_4885_B91C_1D13D8CDF344__INCLUDED_)
