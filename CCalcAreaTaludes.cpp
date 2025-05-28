// CCalcAreaTaludes.cpp : implementation file
//
#include "stdafx.h"
#include "afxtempl.h"
#include "est.h"
#include <string>
#include "CCalcAreaTaludes.h"
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "cponto.h"
#include "clpontos.h"
#include "mat.h"
#include "estaca.h"
#include "perfil.h"
#include "supercstring.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include <list>
#include "csecao.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "clistasecoes.h"
#include "mat.h"
#include "monologo.h"

#define ESQUERDO 0
#define DIREITO 1
#define ATERRO 0
#define CORTE 1

extern class Matem Mat;
extern class monologo monolog;

// CCalcAreaTaludes

CCalcAreaTaludes::CCalcAreaTaludes()
{
}

CCalcAreaTaludes::~CCalcAreaTaludes()
{
}

bool CCalcAreaTaludes::CalculaAreas(CProgressCtrl& CProgBar,CWnd* Edit)
{
  CMainFrame* pMainFrame=(((CMainFrame*)AfxGetMainWnd()));
  CListaSecoes ListaSecoes(CString(pMainFrame->PegaProjetoAtual()));

  CFile ArqCalcAreaTaludes; 
  CFileException e;

  std::string NomeCompArquivo(std::string(pMainFrame->PegaProjetoAtual()) + ".tal");

  if (ArqCalcAreaTaludes.Open(NomeCompArquivo.data(),CFile::shareDenyNone | CFile::modeWrite  | CFile::modeCreate,&e) == 0) 
  {
    if (e.m_cause != CFileException::fileNotFound) 
    {
      monolog.mensagem(13,CString(NomeCompArquivo.data()));
      return false;
    }
  }
  
  CArchive CArqCalcAreaTaludes(&ArqCalcAreaTaludes,CArchive::store);

  CArqCalcAreaTaludes << (int) 0;

  int ContaSecoes(0);
  double AreaTotal(0.0);
  double ComprimentoAnterior(0.0);

  CProgBar.SetRange(0,ListaSecoes.GetSize());
  CProgBar.SetStep(1);

  double EstacaAnterior(INFINITO);
  std::string strObs;

  if(ListaSecoes.GetCount() < 2)
  {
     Edit->SetWindowText("Não há seções para executar os cálculos.");
     return false;
  }

  for(ListaSecoes.PosicionaPrimeira(); ListaSecoes.PegaAtual() !=	NULL ; ListaSecoes++)
  {
    CProgBar.StepIt(); 

    CSecao&	SecaoAtual(ListaSecoes.GetAt(ListaSecoes.PegaAtual()));

    strObs = "Calculando a Estaca:   ";
    strObs.append(SecaoAtual.Estaca);

    Edit->SetWindowText(strObs.c_str());

    bool TemTerreno(SecaoAtual.Terreno.GetCount() > 0),TemProjeto(SecaoAtual.Projeto.GetCount() > 0),TemProjeTerreno(TemTerreno&TemProjeto), 
         TemOffSetE(TemProjeto && SecaoAtual.Projeto.GetHead().Descricao == "OFFSEE" ),
         TemOffSetD(TemProjeto && SecaoAtual.Projeto.GetTail().Descricao == "OFFSED");

    double ComprimentoAtual(0.0);
    double AreaAtual(0.0);
    int LadoAtual(ESQUERDO);
    double SemiDistancia(0.0);
    
    if(TemProjeto && (TemOffSetE || TemOffSetD))
    {
      if(TemOffSetE)
      {
        POSITION PProjAtual(SecaoAtual.Projeto.GetHeadPosition());
        Ponto ProjetoAnte(SecaoAtual.Projeto.GetNext(PProjAtual));

        if(SecaoAtual.Projeto.GetAt(PProjAtual).Descricao != "BORTEE")     //--- OffSet dentro da pavimentação, não conta
        {
          while (ProjetoAnte.Descricao != "BORPIE")
          {
            // Calcula somatorio comprimento
            Ponto PProjPost(SecaoAtual.Projeto.GetNext(PProjAtual));
            ComprimentoAtual += Mat.R_P(ProjetoAnte, PProjPost).y;

            ProjetoAnte = PProjPost;
          }
        }
      }
    
      if(TemOffSetD)
      {
        POSITION PProjAtual(SecaoAtual.Projeto.GetTailPosition());
        Ponto ProjetoAnte(SecaoAtual.Projeto.GetPrev(PProjAtual));

        if (SecaoAtual.Projeto.GetAt(PProjAtual).Descricao != "BORTED")     //--- OffSet dentro da pavimentação, não conta
        {
          while (ProjetoAnte.Descricao != "BORPID")
          {
            // Calcula somatorio comprimento
            Ponto PProjPost(SecaoAtual.Projeto.GetPrev(PProjAtual));
            ComprimentoAtual += Mat.R_P(ProjetoAnte, PProjPost).y;

            ProjetoAnte = PProjPost;
          }
        }
      }
    
      if(EstacaAnterior != INFINITO) SemiDistancia = (SecaoAtual.Projeto.Estaca.EstReal - EstacaAnterior) / 2.0;
      AreaAtual = (ComprimentoAtual + ComprimentoAnterior)  * SemiDistancia;
      AreaTotal += AreaAtual;

      CArqCalcAreaTaludes << SecaoAtual.Projeto.Estaca.EstReal << SemiDistancia << AreaAtual << AreaTotal;
      
      ComprimentoAnterior = ComprimentoAtual;
      ComprimentoAtual = 0.0;
      EstacaAnterior = SecaoAtual.Projeto.Estaca.EstReal;
      ContaSecoes++;
    }
  }

  CArqCalcAreaTaludes.Flush();

  ArqCalcAreaTaludes.SeekToBegin();
  CArqCalcAreaTaludes << ContaSecoes;

  CArqCalcAreaTaludes.Close();
  ArqCalcAreaTaludes.Close();

  return true;
}
