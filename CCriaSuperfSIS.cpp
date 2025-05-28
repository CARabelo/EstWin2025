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
#include "CCriaSuperfSIG.h"

extern class Matem Mat;

void CCriaSuperfSIG::GravaArqCNs(const std::string& NomeArq)
{
  CFile Arq;
  CFileException e;

  std::string NomeArqSaidaCN(NomeArq + ".cun");

  if (Arq.Open(NomeArqSaidaCN.c_str(), CFile::modeCreate | CFile::modeWrite, &e) == TRUE)
  {
    CArchive CArcCurvasNivel(&Arq, CArchive::store);
    size_t QtdCurvasNivel(0);

    if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
    {
      CArcCurvasNivel << (double)0.0 << (double)1.0;  //EquidisCN;
      CArcCurvasNivel << CurvasNivel.size();

      ItLLPontos itLLCN(CurvasNivel.begin());

      for (ItLLPontos itLLCN = CurvasNivel.begin(); itLLCN != CurvasNivel.end(); itLLCN++)
      {
        CArcCurvasNivel << itLLCN->size();

        itLLCN->begin()->fi = INFINITO;

        *itLLCN->begin() >> CArcCurvasNivel;

        ItLPontos ItPAnterior(itLLCN->begin()), ItPAtual(ItPAnterior);

        for (ItPAtual++; ItPAtual != itLLCN->end(); ItPAnterior++, ItPAtual++)
        {
          ItPAtual->fi = Mat.CalculaAzimute(*ItPAnterior, *ItPAtual);

          *ItPAtual >> CArcCurvasNivel;
        }
      }
    }
  }
}


