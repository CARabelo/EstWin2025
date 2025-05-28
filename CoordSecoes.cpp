#include "stdafx.h"
#include ".\coordsecoes.h"

CCoordSecoes::CCoordSecoes()
{
}

CCoordSecoes::~CCoordSecoes()
{
}

PairPontoBool CCoordSecoes::PagaPontoMaisProximo(const Ponto PontoRef,double Toleranciax)
{
  Ponto PAnterior(PSobMouse),PPosterior(PSobMouse);

  PAnterior.x -= Toleranciax;
  PPosterior.x += Toleranciax;

  itMSecaoGeomPonto itMSecaoAnterior = MapaSecoesPonto.upper_bound(&PAnterior);
	itMSecaoGeomPonto itMSecaoPosterior = MapaSecoesPonto.upper_bound(&PPosterior);

	itMSecaoGeomPonto itMaisPerto =  MapaSecoesPonto.end();

  double MenorDistancia2(INFINITO),Distancia2Atual;

  for(itMSecaoGeomPonto ItMapSecatual = itMSecaoAnterior ; ItMapSecatual != itMSecaoPosterior ; ItMapSecatual++)
  {
    Distancia2Atual =  pow((*ItMapSecatual).first->x - MouseXYGeo->m_DOCoorX,2) + pow((*ItMapSecatual).first->y - MouseXYGeo->m_DOCoorY,2); 
    
    if (Distancia2Atual < MenorDistancia2) 
		{
      MenorDistancia2 = Distancia2Atual;
      itMaisPerto = ItMapSecatual;
		}             
  }

  return PairPontoBool(*itMaisPerto,MenorDistancia2 < pow(Toleranciax,2));
}

void CCoordSecoes::LeCoordSecoes() 
{
  CString NomeProjeto(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()));

  CFile ArqCalcSec;
  CFileException e;
	unsigned int QSecoes(0);
	Ponto PontoACalc;
	
	if (ArqCalcSec.Open(NomeProjeto + CString(".pes"),CFile::modeRead | CFile::shareDenyNone,&e) == 0)
	  monolog.mensagem(13,NomeProjeto + CString(".pes"));
  else
  {
    CArchive CArqCalcSec(&ArqCalcSec,CArchive::load);
		Ponto PEsq,PDir,PEixo;
    CEstaca EstacaAtual;

    //--- Se estiver relendo remove os dados anteriores

    LPSecoes.clear();
	  
    //--- Busca coordenadas de todas as seções. colocando-os numa lista (os pontos de 3 em 3 já estão na ordem de estaca)

 	  for (CArqCalcSec >> QSecoes ; QSecoes ; QSecoes--)  
    {
      EstacaAtual << CArqCalcSec;
      PDir << CArqCalcSec;
      PEixo << CArqCalcSec;
	    PEsq << CArqCalcSec;

		  LPSecoes.push_back(PDir);
			LPSecoes.push_back(PEixo);
		  LPSecoes.push_back(PEsq);

      //--- Mapa de estacas ordenado por coordenadas, usado no onmousemove para mostrar as coordenadas do ponto
      //--- Os pontos extremos poderia estar aqui tb...corrigir

      MapaSecoesGeom.insert(MSecaoGeom::value_type(EstacaAtual,PEixo));
    }

    CArqCalcSec.Close();
    ArqCalcSec.Close(); 
  }

  void DesPontosGeomView::GravaCoordSecoes() 
{
	CString NomeProjeto(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()));

  CFile ArqCalcSec;
  CFileException e;
	
	if (ArqCalcSec.Open(NomeProjeto + CString(".pes"),CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone,&e) == 0)
	  monolog.mensagem(13,NomeProjeto + CString(".pes"));
  else
  {
    CArchive CArqCalcSec(&ArqCalcSec,CArchive::store);

    //--- Grava coordenadas de todas as seções. (os pontos de 3 em 3 já estão na ordem de estaca)
    //--- O mapa de seções está sincrinizado com a lista dos pontos extremos das seções (que merda......acertar isso!!!)

    Ponto PEsq,PDir,PEixo;
    CEstaca Estaca;

    unsigned int gg =  LPSecoes.size();

    CArqCalcSec << (unsigned int) (LPSecoes.size() / 3); 

 	  for(register itMSecaoGeom itSecaoGeom = MapaSecoesGeom.begin() ; itSecaoGeom != MapaSecoesGeom.end() ; itSecaoGeom++)
    {
     //(*itSecaoGeom).first >>  CArqCalcSec;
      Estaca = (*itSecaoGeom).first;

      CArqCalcSec << Estaca;

      PEsq = *(LPSecoes.begin());
       PEsq >> CArqCalcSec;
      LPSecoes.pop_front();
       PEsq = *(LPSecoes.begin());
       PEsq >> CArqCalcSec;
      LPSecoes.pop_front();
       PEsq = *(LPSecoes.begin());
       PEsq >> CArqCalcSec;
      LPSecoes.pop_front();
    }

    CArqCalcSec.Close(); 
  }
}
//=====================

void CCoordSecoes::GravaCoordSecoes() 
{
  CString NomeProjeto(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()));

  CFile ArqCalcSec;
  CFileException e;
	unsigned int QSecoes(0);
	Ponto PontoACalc;

  if (ArqCalcSec.Open(NomeProjeto + CString(".pes"),CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone,&e) == 0)
	  monolog.mensagem(13,NomeProjeto + CString(".pes"));
  else
  {
    CArchive CArqCalcSec(&ArqCalcSec,CArchive::store);
		Ponto PEsq,PDir,PEixo;
    CEstaca EstacaAtual;

    unsigned int QSecoes(LPSecoes.size());

    
    //--- Busca coordenadas de todas as seções. colocando-os numa lista (os pontos de 3 em 3 já estão na ordem de estaca)

 	  for (CArqCalcSec << QSecoes ; QSecoes ; QSecoes--)  
    { 
      EstacaAtual = 

      EstacaAtual << CArqCalcSec;
      PDir << CArqCalcSec;
      PEixo << CArqCalcSec;
	    PEsq << CArqCalcSec;

		  LPSecoes.push_back(PDir);
			LPSecoes.push_back(PEixo);
		  LPSecoes.push_back(PEsq);

      //--- Mapa de estacas ordenado por coordenadas, usado no onmousemove para mostrar as coordenadas do ponto
      //--- Os pontos extremos poderia estar aqui tb...corrigir

      MapaSecoesGeom.insert(MSecaoGeom::value_type(EstacaAtual,PEixo));
    }

    CArqCalcSec.Close();
    ArqCalcSec.Close(); 
  }

