#include "stdafx.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <set>
#include "ConvUTMGDC\ConvUTMGDC\Utm_Coord_3d.h"
#include "ConvUTMGDC\ConvUTMGDC\Gdc_Coord_3d.h"
#include "CLerKMLCN.h"

CLerKMLCN::CLerKMLCN(const std::string& NomeArq,tylstGdc_Coord_3d& LstCoordGdc)
{
  LstCoordGdc.clear();

  std::ifstream ArqKMLCN(NomeArq);
  std::ofstream ArqTXT(NomeArq+std::string(".txt"), std::ofstream::out|std::ofstream::trunc);
  std::stringstream strstrMsg;

  if (ArqKMLCN.is_open() && ArqKMLCN.good())
  {
    int ContaLinhas(0);
    tySetString SetRotulos{ "NAME","LINESTRING","LINEARRING","COORDINATES","CONTOUR" };
    std::string StrRotulos, NomeRotuloAtual, LineStringAtual, LinearRingAtual, CoordinatesAtual;
    typairstrstr TokenAtual;

    for (tyitSetString it = SetRotulos.begin(); it != SetRotulos.end(); it++)   NomeRotuloAtual += *it;

    unsigned int NumPonto(1);
    bool InicioCN(false);

    while (ArqKMLCN.good())
    {
      std::string strBufferLinha, strBufferConteudo;

      ++ContaLinhas;

      std::getline(ArqKMLCN, strBufferLinha);

      TokenAtual = ExtraiToken(SetRotulos, strBufferLinha);

      std::transform(TokenAtual.first.begin(), TokenAtual.first.end(), TokenAtual.first.begin(), ::toupper);
      std::transform(TokenAtual.second.begin(), TokenAtual.second.end(), TokenAtual.second.begin(), ::toupper);

      if (TokenAtual.first.compare("CONTOUR") == 0) InicioCN = !InicioCN;
      else if (TokenAtual.second.compare("CONTOUR") == 0) InicioCN = !InicioCN;
      else if (TokenAtual.first.compare("NAME") == 0) NomeRotuloAtual = TokenAtual.second;
      else if (TokenAtual.first.compare("LINESTRING") == 0) LineStringAtual = TokenAtual.second;
      else if (TokenAtual.first.compare("LINEARRING") == 0) LinearRingAtual = TokenAtual.second;
      else
      {
        //if (InicioCN == true && TokenAtual.first.compare("COORDINATES") == 0)  |
        if (InicioCN == true && TokenAtual.first.compare("COORDINATES") == 0)  
        {
          CoordinatesAtual = TokenAtual.second;

          if (CoordinatesAtual.empty())
          {
            std::getline(ArqKMLCN, CoordinatesAtual);
          }

          if (CoordinatesAtual.find("</") == std::string::npos)       //--- Corrdenadas vazias
          {
            std::string strPontoPontoAtual;

            *(std::remove(CoordinatesAtual.begin(), CoordinatesAtual.end(), '\t') - 1) = 0;

            std::stringstream isstrstrLinha(CoordinatesAtual);
            isstrstrLinha << std::right << std::fixed << std::setfill(' ') << std::setprecision(15);
            std::string strPAtual;

            while (isstrstrLinha.good())
            {
              getline(isstrstrLinha, strPAtual, ' ');

              if(strPAtual.empty())  getline(isstrstrLinha, strPAtual);

              std::stringstream strstrPontoRasc(strPAtual);
              std::string strBuffer;
              
              ittylstGdc_Coord_3d itGdcAtual(LstCoordGdc.emplace(LstCoordGdc.end()));

              itGdcAtual->NumPonto = NumPonto++;
              itGdcAtual->Observacoes = NomeRotuloAtual;

              std::getline(strstrPontoRasc, strBuffer, ',');
              itGdcAtual->longitude = atof(strBuffer.c_str());
              std::getline(strstrPontoRasc, strBuffer, ',');
              itGdcAtual->latitude = atof(strBuffer.c_str());
              std::getline(strstrPontoRasc, strBuffer, ',');
              itGdcAtual->elevation = atof(strBuffer.c_str());
              std::getline(strstrPontoRasc, strBuffer);
              
              std::replace(strPAtual.begin(), strPAtual.end(), ',', ' ');
              ArqTXT << NumPonto << ' ' << strPAtual.c_str() << ' ' << NomeRotuloAtual.c_str() << std::endl;
            }

            LstCoordGdc.push_back(Gdc_Coord_3d(0.0, 0.0, 0.0, 0, std::string("FimCN")));

            strPAtual = " 0.000 0.000 0.000 FimCN ";

            ArqTXT << (unsigned int) 0 << strPAtual << std::endl;

            NomeRotuloAtual = LineStringAtual = LinearRingAtual = "";
          }
        }
      }
    }
  }
}

CLerKMLCN::CLerKMLCN(const std::string& NomeArq, tylstGdc_Coord_3d& LstCoordGdc, int Tipo)
{
  LstCoordGdc.clear();

  std::ifstream ArqKMLCN(NomeArq);
  std::ofstream ArqTXT(NomeArq + std::string(".txt"), std::ofstream::out | std::ofstream::trunc);
  std::stringstream strstrMsg;

  if (ArqKMLCN.is_open() && ArqKMLCN.good())
  {
    int ContaLinhas(0);
    tySetString SetRotulos{ "NAME","LINESTRING","LINEARRING","COORDINATES","CONTOUR" };
    std::string StrRotulos, NomeRotuloAtual, LineStringAtual, LinearRingAtual, CoordinatesAtual;
    typairstrstr TokenAtual;

    for (tyitSetString it = SetRotulos.begin(); it != SetRotulos.end(); it++)   NomeRotuloAtual += *it;

    unsigned int NumPonto(1);
    bool InicioCN(false);
    int count(0);

    while (ArqKMLCN.good())
    {
      std::string strBufferLinha, strBufferConteudo;

      ++ContaLinhas;
      std::getline(ArqKMLCN, strBufferLinha);


      TokenAtual = ExtraiToken(SetRotulos, strBufferLinha);

      std::transform(TokenAtual.first.begin(), TokenAtual.first.end(), TokenAtual.first.begin(), ::toupper);
      std::transform(TokenAtual.second.begin(), TokenAtual.second.end(), TokenAtual.second.begin(), ::toupper);

      if (TokenAtual.second.find("CONTOUR") != std::string::npos) InicioCN = true;
      else if (TokenAtual.first.compare("NAME") == 0) NomeRotuloAtual = TokenAtual.second;
      else
      {
        int TipoKML(-1);

        if (InicioCN == true && TokenAtual.first.compare("COORDINATES") == 0)
        {
          if (TokenAtual.second.empty())
          {
            TipoKML = 1;
            std::getline(ArqKMLCN, CoordinatesAtual);
          }
          else
          {
            TipoKML = 2;
            CoordinatesAtual = TokenAtual.second;
          }

          if (TipoKML == 1)
          {
            while (CoordinatesAtual.find("coordinates") == std::string::npos)
            {
              std::stringstream isstrstrLinha(CoordinatesAtual);
              isstrstrLinha << std::right << std::fixed << std::setfill(' ') << std::setprecision(15);

              std::stringstream strstrPontoRasc(CoordinatesAtual);
              std::string strBuffer;

              ittylstGdc_Coord_3d itGdcAtual(LstCoordGdc.emplace(LstCoordGdc.end()));

              itGdcAtual->NumPonto = NumPonto++;
              itGdcAtual->Observacoes = "Ponto de Mapa";
              std::getline(strstrPontoRasc, strBuffer, ',');
              itGdcAtual->longitude = atof(strBuffer.c_str());
              std::getline(strstrPontoRasc, strBuffer, ',');
              itGdcAtual->latitude = atof(strBuffer.c_str());
              std::getline(strstrPontoRasc, strBuffer);
              itGdcAtual->elevation = atof(strBuffer.c_str());

              std::replace(CoordinatesAtual.begin(), CoordinatesAtual.end(), ',', ' ');
              NomeRotuloAtual = NomeRotuloAtual.substr(0, NomeRotuloAtual.find_first_of(' '));

              ArqTXT << NumPonto << ' ' << CoordinatesAtual.c_str() << ' ' << NomeRotuloAtual.c_str() << std::endl;

              std::getline(ArqKMLCN, CoordinatesAtual);
            }
          }
          else
          {
            std::string strBuffer;
            std::stringstream strstrPontoRasc(CoordinatesAtual);

            while (!strstrPontoRasc.eof())
            {
              ittylstGdc_Coord_3d itGdcAtual(LstCoordGdc.emplace(LstCoordGdc.end()));

              itGdcAtual->NumPonto = NumPonto++;
              itGdcAtual->Observacoes = "Ponto de Mapa";
              std::getline(strstrPontoRasc, strBuffer, ',');
              itGdcAtual->longitude = atof(strBuffer.c_str());
              std::getline(strstrPontoRasc, strBuffer, ',');
              itGdcAtual->latitude = atof(strBuffer.c_str());
              std::getline(strstrPontoRasc, strBuffer, ' ');
              itGdcAtual->elevation = atof(NomeRotuloAtual.substr(0, NomeRotuloAtual.find_first_of('<')).c_str());

              ArqTXT << NumPonto << ' '
                     << itGdcAtual->longitude << ' '
                     << itGdcAtual->latitude << ' '
                     << itGdcAtual->elevation << ' '
                     << NomeRotuloAtual.c_str()
                     << std::endl;

              if (TipoKML == 1)
              {
                std::getline(ArqKMLCN, CoordinatesAtual);
                if (CoordinatesAtual.find('>') != std::string::npos) strstrPontoRasc.str(std::string());
              }
            }
          }

          LstCoordGdc.push_back(Gdc_Coord_3d(0.0, 0.0, 0.0, 0, std::string("FimCN")));

          ArqTXT << (unsigned int)0 << " 0.000 0.000 0.000 FimCN " << std::endl;

          NomeRotuloAtual = LineStringAtual = LinearRingAtual = "";

          InicioCN = false;
        }
      }
    }
  }
}


CLerKMLCN::~CLerKMLCN()
{
}

typairstrstr CLerKMLCN::ExtraiToken(tySetString& Token, std::string& Linha)
{
  static typairstrstr Resposta;

  Resposta.first = Resposta.second = "";

  size_t Inicio(Linha.find('<')), Fim(Linha.find('>'));

  if (Inicio != std::string::npos && Fim != std::string::npos)
  {
    Resposta.first = Linha.substr(Inicio+1,Fim-Inicio-1);

    Inicio = Fim+1;
    Fim = Linha.find('>', Inicio);

    if (Inicio != std::string::npos && Fim != std::string::npos)
    {
      Resposta.second = Linha.substr(Inicio, Fim-Inicio);
    }
  }

  return Resposta;
}

void CLerKMLCN::GravaString(std::ofstream& Arq, std::string& str)
{
}