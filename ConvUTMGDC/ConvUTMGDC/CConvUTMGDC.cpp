#include "stdafx.h"
#include <string>
#include <list>
#include "Gdc_Coord_3d.h"
#include <set>
#include "Utm_Coord_3d.h"
#include <array>
#include "Gdc_To_Utm_Converter.h"
#include "CConvUTMGDC.h"

CConvUTMGDC::CConvUTMGDC()
{
}

CConvUTMGDC::~CConvUTMGDC()
{
}

int CConvUTMGDC::ConverteGDCUTM(const tylstGdc_Coord_3d& LstGdc, tylstUtm_Coord_3d& LstUTM,int NumDatum)
{
  Gdc_To_Utm_Converter ConvGDCUTM(NumDatum);

  ConvGDCUTM.ConvertGDC_UTM(LstGdc, LstUTM);

  return 0;
}

