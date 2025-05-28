#include "stdafx.h"
#include "CPonto.h"
#include "CTriangulo.h"
#include "CTriangulacao.h"
#include "CDeltaSuperficie.h"
#include <map>
#include "CSegmento.h"
#include "CCotasAreas.h"
#include "CDadosBacias.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CBacia.h"
#include "CEquipDrenagem.h"
#include "CAcidentesTopograficos.h"
#include "Superficie.h"
#include "CCriaSuperfIgualTopog.h"
#include "Mat.h"

extern class Matem Mat;

void CCriaSuperfIgualTopog::CriaSuperfIgualTopog()
{
  //--- Cria a delta superfície com os pontos ligados e a insere na superficie

  itsetPontos itTaAnt(TriangTopografia.SetPontosSuperf.end());
  Superficie.clear();

  for (itsetPontos itT = TriangTopografia.SetPontosSuperf.begin(); itT != TriangTopografia.SetPontosSuperf.end(); itT++)
  {
    PaircitbooSSuperficie RespS;

    CDeltaSuperficie DeltaSupAtual(*itT);

    if (Superficie.size() != 0)    //--- Ja inseriu o primeiro ponto?
    {
      //--- Verifica se o ponto atual está muito proximo do anterior
      //--- Se estiver não insere na superficie pois acarreta uma série de problemas no VUE
      //--- Pontos que deveriam ser encontrados na superfície não são mais e vice versa

      if (fabs(itT->x - itTaAnt->x) < 1e-3 && fabs(itT->y - itTaAnt->y) < 1e-3) continue;
    } 

    RespS = Superficie.insert(DeltaSupAtual);
    itTaAnt = itT;

    CDeltaSuperficie* pCDSAtual(const_cast<CDeltaSuperficie*>(&*RespS.first));

    auto Resp(TriangTopografia.SetTriangulos.emplace(*itT,*itT,*itT));     //--- Temporario, so pra pegar o iterador

    auto RespP1(TriangTopografia.SetP1.equal_range(Resp.first)),
         RespP2(TriangTopografia.SetP2.equal_range(Resp.first)),
         RespP3(TriangTopografia.SetP3.equal_range(Resp.first));

    for (auto it1 = RespP1.first; it1 != RespP1.second; it1++)
    {
      Ponto R_P2(Mat.R_P(*itT, (*it1)->P2)),
            R_P3(Mat.R_P(*itT, (*it1)->P3));

      CTriangulo * pTri(const_cast<CTriangulo*>(&**it1));   //--- não vai corromper o set ele esta ordenado por xyz não por fi

      //--- no set de pontos ligados está ordenado por fi
       
      pTri->P2.fi = R_P2.x;
      pTri->P3.fi = R_P3.x;

      pCDSAtual->PontosLigados.insert((*it1)->P2);
      pCDSAtual->PontosLigados.insert((*it1)->P3);
    }

    for (auto it2 = RespP2.first; it2 != RespP2.second; it2++)
    {
      Ponto R_P1(Mat.R_P(*itT, (*it2)->P1)),
            R_P3(Mat.R_P(*itT, (*it2)->P3));

      CTriangulo* pTri(const_cast<CTriangulo*>(&**it2)); 

      pTri->P1.fi = R_P1.x;
      pTri->P3.fi = R_P3.x;

      pCDSAtual->PontosLigados.insert((*it2)->P1);
      pCDSAtual->PontosLigados.insert((*it2)->P3);
    }

    for (auto it3 = RespP3.first; it3 != RespP3.second; it3++)
    {
      Ponto R_P1(Mat.R_P(*itT, (*it3)->P1)),
            R_P2(Mat.R_P(*itT, (*it3)->P2));

      CTriangulo* pTri(const_cast<CTriangulo*>(&**it3));  

      pTri->P1.fi = R_P1.x;
      pTri->P3.fi = R_P2.x;

      pCDSAtual->PontosLigados.insert((*it3)->P1);
      pCDSAtual->PontosLigados.insert((*it3)->P2);
    }

    TriangTopografia.SetTriangulos.erase(Resp.first);    //--- Temporario, era so pra pegar o iterador
  }
  //--- Nos arquivos grados pelo Autocad são gerados ponto muito proximos um do outro, na ordem de 1-10,
  //--- Isto provoca uma série de problemas no VUE pois pontos que deveriam ser encontrados não são e vice versa
  //--- Para resolver este pb removemos da superfície os pontos que estiverem a menos de 1 mm de distância 
  //--- (na coordenada) um do outro

  ItSSuperficie ItAnt(Superficie.begin()), ItPos(ItAnt);

  for (ItPos++; ItPos != Superficie.end(); )
  {
    if (fabs(ItAnt->PCentral.x - ItPos->PCentral.x) < 1e-3 &&
        fabs(ItAnt->PCentral.y - ItPos->PCentral.y) < 1e-3)
    {
      Superficie.erase(ItPos++);
    }
    else
    {
      ItPos++;
      ItAnt++;
    }
  }
}

void CCriaSuperfIgualTopog::GravaArqTrian(const std::string& NomeArq)
{
  CFile Arq;
  CFileException e;

  std::string NomeArqSaidaTrian(NomeArq + ".sup");

  if (Arq.Open(NomeArqSaidaTrian.c_str(), CFile::modeCreate | CFile::modeWrite, &e) == TRUE)
  {
    CArchive CArcSuperficie(&Arq, CArchive::store);
    size_t QDeltasSup(Superficie.size());
    ItSSuperficie ItSSupAtual(Superficie.begin());

    CArcSuperficie << 0.0 << 0.0;     //--- Angulo de rotação e declinação

    for (CArcSuperficie << QDeltasSup; QDeltasSup; QDeltasSup--, ItSSupAtual++)
    {
      CDeltaSuperficie* pCDS(const_cast<CDeltaSuperficie*>(&*ItSSupAtual));
      pCDS->Serialize(CArcSuperficie, ((SSuperficie*)this), false);
    }
  }

  //--- Arquivo de pontos xyz
  //--- Os pontos estão sem descrição porque são os pontos da malha triangulada
  //--- Os pontos com descrição serão importados na importação dos 
  //--- acidentes topográficos. Este arquivo serve pra mostrar a triangulação
  //--- Posteriormente quando os acidentes topográficos forem importados
  //--- a descrição do acidente topográfico será inserida neles
    
  std::string NomeArqSaidaTopog(NomeArq+"Triang.xyz");

  std::ofstream ofs(NomeArqSaidaTopog, std::ofstream::out | std::ofstream::trunc);

  ofs << std::right << std::fixed << std::setfill(' ') << std::setprecision(6);

  unsigned int C(1);

  /*
  for (itsetPontos itT = TriangTopografia.SetPontosSuperf.begin(); itT != TriangTopografia.SetPontosSuperf.end(); itT++)
  {
    ofs << C++ << " "  << itT->y << " " << itT->x << " " << itT->z << " " << "Ponto de terreno" << std::endl;
  }
  */

  for (ItSSuperficie itS = Superficie.begin(); itS != Superficie.end(); itS++)
  {
    ofs << C++ << " " << itS->PCentral.y << " " << itS->PCentral.x << " " << itS->PCentral.z << " " << "Ponto de terreno" << std::endl;
  }
}

void CCriaSuperfIgualTopog::GravaArqCNs(const std::string& NomeArq)
{
  CFile Arq;
  CFileException e;

  std::string NomeArqSaidaCN(NomeArq + ".cnd");

  if (Arq.Open(NomeArqSaidaCN.c_str(), CFile::modeCreate | CFile::modeWrite, &e) == TRUE)
  {
    CArchive CArcCurvasNivel(&Arq, CArchive::store);
    size_t QtdCurvasNivel(0);

    if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
    {
      CArcCurvasNivel << (double) 0.0 << (double) 1.0;  //EquidisCN;
      CArcCurvasNivel << CurvasNivel.size();

      ItLLPontos itLLCN(CurvasNivel.begin());

      for (ItLLPontos itLLCN = CurvasNivel.begin(); itLLCN != CurvasNivel.end(); itLLCN++)
      {
        CArcCurvasNivel << itLLCN->size();

        itLLCN->begin()->fi = INFINITO;

        *itLLCN->begin() >> CArcCurvasNivel;

        ItLPontos ItPAnterior(itLLCN->begin()), ItPAtual(ItPAnterior);

        for (ItPAtual++ ; ItPAtual != itLLCN->end() ; ItPAnterior++, ItPAtual++)
        {
          ItPAtual->fi = Mat.CalculaAzimute(*ItPAnterior, *ItPAtual);

          *ItPAtual >> CArcCurvasNivel;
        }
      }
    }
  }
}






