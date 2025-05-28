#include "stdafx.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <set>
#include "ConvUTMGDC\ConvUTMGDC\Utm_Coord_3d.h"
#include "ConvUTMGDC\ConvUTMGDC\Gdc_Coord_3d.h"
#include "CLerKML.h"

CLerKML::CLerKML(const std::string& NomeArq,tylstGdc_Coord_3d& LstCoordGdc)
{
  LstCoordGdc.clear();

  std::ifstream ArqKML(NomeArq);
  std::ofstream ArqTXT(NomeArq+std::string(".txt"), std::ofstream::out|std::ofstream::trunc);
  std::stringstream strstrMsg;

  if (ArqKML.is_open() && ArqKML.good())
  {
    int ContaLinhas(0);
    tySetString SetRotulos{ "NAME","LINESTRING","LINEARRING","COORDINATES" };
    std::string StrRotulos, NomeRotuloAtual, LineStringAtual, LinearRingAtual, CoordinatesAtual;
    typairstrstr TokenAtual;

    for (tyitSetString it = SetRotulos.begin(); it != SetRotulos.end(); it++)   NomeRotuloAtual += *it;

    unsigned int NumPonto(1);

    while (ArqKML.good())
    {
      std::string strBufferLinha, strBufferConteudo;

      ++ContaLinhas;

      std::getline(ArqKML, strBufferLinha);

      TokenAtual = ExtraiToken(SetRotulos, strBufferLinha);

      std::transform(TokenAtual.first.begin(), TokenAtual.first.end(), TokenAtual.first.begin(), ::toupper);

      if (TokenAtual.first.compare("NAME") == 0) NomeRotuloAtual = TokenAtual.second;
      else if (TokenAtual.first.compare("LINESTRING") == 0) LineStringAtual = TokenAtual.second;
      else if (TokenAtual.first.compare("LINEARRING") == 0) LinearRingAtual = TokenAtual.second;
      else
      {
        if (TokenAtual.first.compare("COORDINATES") == 0)
        {
          CoordinatesAtual = TokenAtual.second;

          if (CoordinatesAtual.empty())
          {
            std::getline(ArqKML, CoordinatesAtual);
          }

          if (CoordinatesAtual.find("</") == std::string::npos)       //--- Corrdenadas vazias
          {
            std::string strPontoPontoAtual;

            *(std::remove(CoordinatesAtual.begin(), CoordinatesAtual.end(), '\t') - 1) = 0;

            std::stringstream isstrstrLinha(CoordinatesAtual);
            isstrstrLinha << std::right << std::fixed << std::setfill(' ') << std::setprecision(15);
            std::string strPAtual;

            while (isstrstrLinha.good() && getline(isstrstrLinha, strPAtual, ' '))
            {
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

            NomeRotuloAtual = LineStringAtual = LinearRingAtual = "";
          }
        }
      }
    }
  }
}

CLerKML::~CLerKML()
{
}

typairstrstr CLerKML::ExtraiToken(tySetString& Token, std::string& Linha)
{
  static typairstrstr Resposta;

  Resposta.first = Resposta.second = "";

  size_t Inicio(Linha.find('<')), Fim(Linha.find('>'));

  if (Fim != std::string::npos && Fim != std::string::npos)
  {
    Resposta.first = Linha.substr(Inicio+1,Fim-Inicio-1);

    Inicio = Fim+1;
    Fim = Linha.find('<', Inicio);

    if (Inicio != std::string::npos && Fim != std::string::npos)
    {
      Resposta.second = Linha.substr(Inicio, Fim-Inicio);
    }
  }

  return Resposta;
}


void CLerKML::GravaString(std::ofstream& Arq, std::string& str)
{
}