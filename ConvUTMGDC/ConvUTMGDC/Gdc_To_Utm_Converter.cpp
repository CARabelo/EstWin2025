#include "stdafx.h"
#include <list>
#include <math.h>
#include <set>
#include "Utm_Coord_3d.h"
#include "Gdc_Coord_3d.h"
#include <array>
#include "Gdc_To_Utm_Converter.h"

#define INFINITO   1.7976931348623158e+308

const double Gdc_To_Utm_Converter::RADIANS_PER_DEGREE = 0.0174532925199432957692;
const double Gdc_To_Utm_Converter::PI = 4.0 * atan(1.0);

double			  Gdc_To_Utm_Converter::A,
Gdc_To_Utm_Converter::F,
Gdc_To_Utm_Converter::C,
Gdc_To_Utm_Converter::Eps2,
Gdc_To_Utm_Converter::Eps25,
Gdc_To_Utm_Converter::Epps2,
Gdc_To_Utm_Converter::CScale = .9996,
Gdc_To_Utm_Converter::poly1b,
Gdc_To_Utm_Converter::poly2b,
Gdc_To_Utm_Converter::poly3b,
Gdc_To_Utm_Converter::poly4b,
Gdc_To_Utm_Converter::poly5b;

// CreateConstants(6378137, 298.257223563); // default to wgs 84 (Sirgas2000)
//CreateConstants(6378388, 297.0);         // corrego Alegre
                                         // CreateConstants(6378135, 297.0);         // WGS72
                                         // CreateConstants(6378160, 298.25);        // South America 1969
                                         // 6.378.137 298.257 222 100          //--- GRS80

Gdc_To_Utm_Converter::Gdc_To_Utm_Converter(int iDatum) : Datum{ typairdouble{6378137.0, 298.257223563},  //--- SIRGAS2000 
                                                                typairdouble{6378388.0, 297.0},          //--- Corrego Alegre  
                                                                typairdouble{6378135.0, 297.0},          //--- WSG72 
                                                                typairdouble{6378160.0, 298.25},         //--- South America 1969 
                                                                typairdouble{6378137.0, 298.257222100}}, //--- GRS80
                                                         NumDatum(iDatum)
{
  //Init();
  iDatum %= 5;
  CreateConstants(Datum[iDatum].first, Datum[iDatum].second);
};

//void Gdc_To_Utm_Converter::Init()
//{
 // CreateConstants(6378137, 298.257223563); // default to wgs 84 (Sirgas2000)
//  CreateConstants(6378388, 297.0);         // corrego Alegre
 // CreateConstants(6378135, 297.0);         // WGS72
 // CreateConstants(6378160, 298.25);        // South America 1969
 // 6.378.137 298.257 222 100          //--- GRS80
//}

void Gdc_To_Utm_Converter::CreateConstants(double a, double f)
{
  double polx2b, polx3b, polx4b, polx5b;

  A = a;
  F = f;

  //  Create the ERM constants.

  F = 1.0 / (F);
  C = (A) * (1.0 - F);
  Eps2 = (F) * (2.0 - F);
  Eps25 = .25 * (Eps2);
  Epps2 = (Eps2) / (1.0 - Eps2);
  polx2b = 1.0 * Eps2 + 1.0 / 4.0 * pow(Eps2, 2)
    + 15.0 / 128.0 * pow(Eps2, 3) - 455.0 / 4096.0 *
    pow(Eps2, 4);

  polx2b = 3.0 / 8.0 * polx2b;

  polx3b = 1.0 * pow(Eps2, 2) + 3.0 / 4.0 *
    pow(Eps2, 3) - 77.0 / 128.0 * pow(Eps2, 4);

  polx3b = 15.0 / 256.0 * polx3b;

  polx4b = pow(Eps2, 3) - 41.0 / 32.0 * pow(Eps2, 4);

  polx4b = polx4b * 35.0 / 3072.0;

  polx5b = -315.0 / 131072.0 * pow(Eps2, 4);

  poly1b = 1.0 - (1.0 / 4.0 * Eps2) - (3.0 / 64.0 * pow(Eps2, 2))
    - (5.0 / 256.0 * pow(Eps2, 3)) -
    (175.0 / 16384.0 * pow(Eps2, 4));

  poly2b = polx2b * -2.0 + polx3b * 4.0 - polx4b * 6.0 + polx5b * 8.0;

  poly3b = polx3b * -8.0 + polx4b * 32.0 - polx5b * 80.0;

  poly4b = polx4b * -32.0 + polx5b * 192.0;

  poly5b = polx5b * -128.0;

} // end init

void Gdc_To_Utm_Converter::ConvertGDC_UTM(const tylstGdc_Coord_3d& LstCoordGdc, tylstUtm_Coord_3d& LstCoordUTM)
{
  double source_lat, source_lon, s1, c1, tx, s12, rn, axlon0, al, al2, sm, tn2, cee, poly1, poly2;

  LstCoordUTM.clear();

  for (cittylstGdc_Coord_3d itCoordGdc = LstCoordGdc.begin() ; itCoordGdc != LstCoordGdc.end() ; itCoordGdc++)
  {
    Utm_Coord_3d PAtual;

    ittylstUtm_Coord_3d itPontoUTM(LstCoordUTM.emplace(LstCoordUTM.end(),INFINITO,INFINITO, itCoordGdc->elevation,0, itCoordGdc->latitude > 0.0, itCoordGdc->NumPonto, itCoordGdc->Observacoes,0,INFINITO,INFINITO));

    source_lat = itCoordGdc->latitude * RADIANS_PER_DEGREE;
    source_lon = itCoordGdc->longitude * RADIANS_PER_DEGREE;

    s1 = sin(source_lat);
    c1 = cos(source_lat);
    tx = s1 / c1;
    s12 = s1 * s1;

    /* USE IN-LINE SQUARE ROOT */

    rn = A / ((.25 - Eps25 * s12 + .9999944354799 / 4) + (.25 - Eps25 * s12) / (.25 - Eps25 * s12 + .9999944354799 / 4));

    /* COMPUTE UTM COORDINATES */

    /* Compute Zone */

    itPontoUTM->zone = (short)(source_lon * 30.0 / PI + 31);

    if (itPontoUTM->zone <= 0)
      itPontoUTM->zone = 1;
    else
      if (itPontoUTM->zone >= 61)
        itPontoUTM->zone = 60;

    axlon0 = (itPontoUTM->zone * 6 - 183) * RADIANS_PER_DEGREE;

    al = (source_lon - axlon0) * c1;

    sm = s1 * c1 * (poly2b + s12 * (poly3b + s12 *
      (poly4b + s12 * poly5b)));

    sm = A * (poly1b * source_lat + sm);

    tn2 = tx * tx;
    cee = Epps2 * c1 * c1;
    al2 = al * al;
    poly1 = 1.0 - tn2 + cee;
    poly2 = 5.0 + tn2 * (tn2 - 18.0) + cee * (14.0 - tn2 * 58.0);

    /* COMPUTE EASTING */

    itPontoUTM->x = CScale * rn * al * (1.0 + al2 * (.166666666666667 * poly1 + .00833333333333333 * al2 * poly2));

    itPontoUTM->x += 5.0E5;

    /* COMPUTE NORTHING */

    poly1 = 5.0 - tn2 + cee * (cee * 4.0 + 9.0);
    poly2 = 61.0 + tn2 * (tn2 - 58.0) + cee * (270.0 - tn2 * 330.0);

    itPontoUTM->y = CScale * (sm + rn * tx * al2 * (0.5 + al2 * (.0416666666666667 * poly1 + .00138888888888888 * al2 * poly2)));

    if (source_lat < 0.0)
      itPontoUTM->y += 1.0E7;
  } // end for
}

