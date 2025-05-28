
#include "stdafx.h"
#include <list>
#include "cponto.h" 
#include "clpontos.h"
#include <set>
#include <map>
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CAcidentesTopograficos.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include "ConvUTMGDC\ConvUTMGDC\Utm_Coord_3d.h"
#include "ConvUTMGDC\ConvUTMGDC\Gdc_Coord_3d.h"
#include <array>
#include "ConvUTMGDC\ConvUTMGDC\Gdc_To_Utm_Converter.h"
#include <math.h>
#include "ConvUTMGDC\ConvUTMGDC\CConvUTMGDC.h"
#include "CLerKMLCN.h"
#include "CCamadaDGN.h"

CCamadaDGN::CCamadaDGN()
{
}

int CCamadaDGN::LerKML()
{
  std::string NomeArquivo("C:\\Users\\Carlo.Rabelo\\Downloads\\0425732hpOK\\0425732hp.kml");
  //std::string NomeArquivo("C:\\Users\\carlo\\Documents\\Visual Studio 2019\\EstWin\\Projetos\\CampoGolf\\GolfCN.kml");

  tylstGdc_Coord_3d LstCoordGdc;
  tylstUtm_Coord_3d LstCoordUTM;

  CLerKMLCN LerKMLCN(NomeArquivo, LstCoordGdc, 1);   //--- Coordenadas geodésicas

  tylstGdc_Coord_3d LstCNAtual;
 
  LstLstCNUTM.clear();

  for (ittylstGdc_Coord_3d itpCNAtual = LstCoordGdc.begin(); itpCNAtual != LstCoordGdc.end(); ++itpCNAtual)
  {
    if (itpCNAtual->NumPonto != 0)
    {
      LstCNAtual.push_back(*itpCNAtual);   //--- Lista CNs Geodésicas
    }
    else
    {
      int NumDatum(0);   //--- SIRGAS 2000 no combobox

      ittylstlstUtm_Coord_3d itLstCNUTMAtual(LstLstCNUTM.insert(LstLstCNUTM.end(), tylstUtm_Coord_3d()));
      CConvUTMGDC::ConverteGDCUTM(LstCNAtual, *itLstCNUTMAtual, NumDatum);         //--- converte godésicas para UTM
      LstCNAtual.clear();
    }
  }

  if (LstCoordGdc.size() == 0)
  {
    AfxMessageBox("Não foram encontrados pontos neste arquivo.");
    return 1;
  }

  return 0;
}
