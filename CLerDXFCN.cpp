#include "stdafx.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <set>
#include "ConvUTMGDC\ConvUTMGDC\Utm_Coord_3d.h"
#include "ConvUTMGDC\ConvUTMGDC\Gdc_Coord_3d.h"
#include "CPonto.h"
#include "CLerDXFCN.h"
#include "CTextoMapaIBGE.h"
#include "CCNMapaIGBE.h"
#include "CMapaCNIBGE.h"
#include "Mat.h"
#include "CDiaImportarArqCtrlDrones.h"

extern Matem Mat;

CLerDXFCN::CLerDXFCN(const std::string& NomeArq, lstlstPontos& CNsVUE, setPontos& PontosSoltos,CProgressCtrl* pProg,void * pDialog,CStatic* pObserv) : pProgresso(pProg),pDialogo(pDialog), pObservacoes(pObserv)
{
  //--- Le os arquivos

  pObservacoes->SetWindowText("Lendo os arquivos DXF...");

  CMapaCNIBGE MapaIBGE;

  pProg->SetStep(1);
  pProg->SetRange(0,100);

  MapaIBGE.LeArquivoDXF_DGN(NomeArq, pProg);   
  CNsVUE.clear();
  PontosSoltos.clear();

  //--- Importa as CNs

  size_t count(0),PassoProg((MapaIBGE.CurvasNivel().size()/100));

  for (ittylstCN itCNsIBGE = MapaIBGE.CurvasNivel().begin(); itCNsIBGE != MapaIBGE.CurvasNivel().end(); itCNsIBGE++)
  {
    count++;

    if(count == PassoProg)
    {
      ((CDiaImportarArqCtrlDrones*)pDialogo)->Progress.StepIt();
      count = 0;
    }

    CNsVUE.emplace_back(lstPontos());

    ritlstlstPontos ritCNVUEAtual(CNsVUE.rbegin());

    for (itlstPontos itPontoCN = itCNsIBGE->LstPontos().begin(); itPontoCN != itCNsIBGE->LstPontos().end(); itPontoCN++)
    {
      itPontoCN->x *= 1000.0;
      itPontoCN->y *= 1000.0;
      ritCNVUEAtual->emplace_back(*itPontoCN);
    }
  }

  //--- Importa os textos

  PassoProg = (MapaIBGE.Textos().size() / 50);
  count = 0;

  if(count == PassoProg)
  {
    pProgresso->StepIt();
    count = 0;
  }

  pObservacoes->SetWindowText("Importando os Pontos...");

  unsigned int ContaPontoSolto(1);
  std::stringstream strNumPonto;

  for (ittylstCTextos it = MapaIBGE.Textos().begin(); it != MapaIBGE.Textos().end(); it++)
  {
    count++;

    if (count == PassoProg)
    {
      pProgresso->StepIt();
      count = 0;
    }

    if(it->xCentro != INFINITO)     //--- Cota de CN
    {
      strNumPonto << ContaPontoSolto++;
      PontosSoltos.emplace(it->xCentro * 1000.0, it->yCentro * 1000.0, atof(it->Texto.c_str()), INFINITO, INFINITO,INFINITO,CString("Ponto Solto "),false,false, strNumPonto.str().c_str(),it->Tipo);
      strNumPonto.str("");
    }
    else
    {
      strNumPonto << ContaPontoSolto++;
      PontosSoltos.emplace(it->x * 1000.0, it->y * 1000.0, atof(it->Texto.c_str()), it->xCentro * 1000.0, it->yCentro * 1000.0, INFINITO, CString("Ponto Solto "), false, false, strNumPonto.str().c_str(), it->Tipo);
      strNumPonto.str("");
    }
  }
}

CLerDXFCN::~CLerDXFCN()
{
}

int CLerDXFCN::LePontosCN(tylstGdc_Coord_3d& pLstCoordGdc, std::ifstream & pArqDXFCN, std::string & pBufferLinha, std::ofstream& pArqTXT)
{
  static unsigned int NumPonto(1);
  std::string LabelxPonto("10"),BufferLinha;
  std::string x,y;

  while (pArqDXFCN.good() && pBufferLinha.compare("10") == 0)
  {
    std::getline(pArqDXFCN,x);              //--- Valor de x
    std::getline(pArqDXFCN, BufferLinha);   //---  20 -> y
    std::getline(pArqDXFCN,y);              //--- Valor de y

    pLstCoordGdc.emplace_back(Gdc_Coord_3d(atof(x.c_str()),atof(y.c_str()),INFINITO,NumPonto, std::string("Ponto CN Mapa")));

    std::getline(pArqDXFCN, pBufferLinha);   //--- 10 -> x
    Limpa(pBufferLinha);
    pArqTXT << NumPonto++ << ' ' << x << ' ' << y << ' ' << (double) 0.0 << " Ponto CN Mapa" << std::endl;
  } 

  pLstCoordGdc.emplace_back(Gdc_Coord_3d(0.00,0.00,INFINITO,0,std::string("Fim CN")));

  pArqTXT << (double) 0.0 << ' ' << (double) 0.0 << ' ' << (double) 0.0 << "Fim CN" << std::endl;
  return 0;
}

int CLerDXFCN::LePontosSoltos(lstPontos& PontosSoltos, std::ifstream& pArqDXFCN, std::string& pBufferLinha,std::ofstream& pArqTXT)
{
  static unsigned int NumPonto(1);
  std::string Labelx("10"),LabelCota("1"),LabelLandscape("landscape"), BufferLinha;
  std::string x, y,Cota;
  pArqTXT << "PONTOS SOLTOS: " << std::endl;
  pArqTXT << std::setprecision(3);

  PontosSoltos.clear();
  pArqDXFCN.clear();

  pArqDXFCN.seekg(0, pArqDXFCN.beg);
  if (pArqDXFCN.good())
  {
    unsigned int ContaPontoSolto(1);
    PontosSoltos.clear();
    if (!BuscaLabel(LabelLandscape, pArqDXFCN, pBufferLinha, CLerDXFCN::IGUAL)) pBufferLinha.clear();

    while (pArqDXFCN.good() && pBufferLinha.compare("landscape") == 0)
    {
      if (BuscaLabel(Labelx, pArqDXFCN, pBufferLinha, CLerDXFCN::IGUAL))
      {
        std::getline(pArqDXFCN, x);             //--- Valor de x  (Nas coord geodésica é trocado x com y)
        std::getline(pArqDXFCN, BufferLinha);   //---  20 -> y
        std::getline(pArqDXFCN, y);             //--- Valor de y  (Nas coord geodésica é trocado x com y)

        if (BuscaLabel(LabelCota, pArqDXFCN, pBufferLinha, CLerDXFCN::IGUAL))
        {
          std::getline(pArqDXFCN, Cota);

          PontosSoltos.emplace_back(Ponto(std::string("Ponto Solto"), atof(x.c_str()), atof(y.c_str()), atof(Cota.c_str())));

          pArqTXT << ContaPontoSolto++ << ' ' << x << ' ' << y << ' ' << Cota << " Ponto Solto" << std::endl;
          if (!BuscaLabel(LabelLandscape, pArqDXFCN, pBufferLinha, CLerDXFCN::IGUAL)) pBufferLinha.clear();
          else Limpa(pBufferLinha);
        }
      }
    }

    if(PontosSoltos.size() > 0) PontosSoltos.erase(PontosSoltos.begin());    //--- o primeiro é 0,0

    return 0;
  }

  return 1;
}
