// CDiaCalcLimpeza.cpp : implementation file
//
#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "CDiaCalcLimpeza.h"
#include "cponto.h"
#include "clpontos.h"
#include "math.h"
#include "estaca.h"
#include "perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "supercstring.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include <list>
#include "csecao.h"
#include "CListaSecoes.h"
#include <string>
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "monologo.h"
#include "carqhpavimento.h"
#include "CHPavimento.h"

extern class Matem Mat;
extern class monologo monolog;

// CDiaCalcLimpeza dialog

CDiaCalcLimpeza::CDiaCalcLimpeza(CWnd* pParent)	: CDialog(CDiaCalcLimpeza::IDD, pParent)
{
}

CDiaCalcLimpeza::~CDiaCalcLimpeza()
{
}

void CDiaCalcLimpeza::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_PROGCALCLIMPEZA, CProgLimpeza);
}

BEGIN_MESSAGE_MAP(CDiaCalcLimpeza, CDialog)
  ON_BN_CLICKED(IDC_BUTINICIARCALCLIMPEZA, OnBnClickedButiniciarcalclimpeza)
END_MESSAGE_MAP()

// CDiaCalcLimpeza message handlers

void CDiaCalcLimpeza::OnBnClickedButiniciarcalclimpeza()
{
  GetDlgItem(IDC_BUTINICIARCALCLIMPEZA)->EnableWindow(false);

  CMainFrame* pMainFrame=(((CMainFrame*)AfxGetMainWnd()));
  CListaSecoes ListaSecoes(CString(pMainFrame->PegaProjetoAtual()));

  CHPavimento HLimpeza(CString(pMainFrame->PegaProjetoAtual()),CString(".lim"));

  CFile ArqCalcAreaLimpeza; 
  CFileException e;

  std::string NomeCompArquivo(std::string(pMainFrame->PegaProjetoAtual()) + ".lic");

  if (ArqCalcAreaLimpeza.Open(NomeCompArquivo.data(),CFile::shareDenyNone | CFile::modeWrite  | CFile::modeCreate,&e) == 0) 
  {
    if (e.m_cause != CFileException::fileNotFound) 
    {
      monolog.mensagem(13,CString(NomeCompArquivo.data()));

      GetDlgItem(IDOK)->EnableWindow(true);
      GetDlgItem(IDCANCEL)->EnableWindow(false);
  
      SetDlgItemText(IDC_STAOBS,"Erro!!!, os cálculos NÃO foram executados com êxito.");

      return;
    }
  }
  
  if(ListaSecoes.GetCount() < 2)
  {
     SetDlgItemText(IDC_STAOBS,"Não há seções para executar os cálculos.");

     return;
  }

  CArchive CArqCalcAreaLimpeza(&ArqCalcAreaLimpeza,CArchive::store);

  CArqCalcAreaLimpeza << (int) 0;

  int ContaSecoes(0);
  double AreaAnterior(0.0),VolumeTotal(0.0);

  CProgLimpeza.SetRange(0,ListaSecoes.GetSize());
  CProgLimpeza.SetStep(1);

  double EstacaAnterior(INFINITO); 

  for(ListaSecoes.PosicionaPrimeira(); ListaSecoes.PegaAtual() !=	NULL ; ListaSecoes++)
  {
    CProgLimpeza.StepIt(); 

    CSecao&	SecaoAtual(ListaSecoes.GetAt(ListaSecoes.PegaAtual()));

    std::string strObs("Calculando a Estaca:   ");
    strObs.append(SecaoAtual.Estaca);
    SetDlgItemText(IDC_STAOBS,strObs.c_str()); 

    bool TemTerreno(SecaoAtual.Terreno.GetCount() > 0),TemProjeto(SecaoAtual.Projeto.GetCount() > 0),TemProjeTerreno(TemTerreno&TemProjeto), 
         TemOffSetE(TemProjeto && SecaoAtual.Projeto.GetHead().Descricao == "OFFSEE" ),TemOffSetD(TemProjeto && SecaoAtual.Projeto.GetTail().Descricao == "OFFSED");

    double Exesso(0.0);
    double ComprimentoAtual(0.0),AreaAtual(0.0),SemiDistancia(0.0),VolumeAtual(0.0),hLimp(HLimpeza.BuscaH(SecaoAtual.Projeto.Estaca,&Exesso));
      
    if(TemProjeto && TemOffSetE && TemOffSetD)
    {
       Ponto OffSetEsq(SecaoAtual.Projeto.GetHead()),OffSetDir(SecaoAtual.Projeto.GetTail());
       ComprimentoAtual = OffSetDir.x - OffSetEsq.x + (Exesso * 2.0);
       AreaAtual = ComprimentoAtual * hLimp;
       if(EstacaAnterior != INFINITO) SemiDistancia = (SecaoAtual.Projeto.Estaca.EstReal - EstacaAnterior) / 2.0;
       VolumeAtual = (AreaAtual + AreaAnterior) / 2.0 * SemiDistancia;
       VolumeTotal += VolumeAtual;

       CArqCalcAreaLimpeza << SecaoAtual.Projeto.Estaca.EstReal << hLimp << SemiDistancia << AreaAtual << VolumeAtual << VolumeTotal;

       AreaAnterior = AreaAtual;
       EstacaAnterior = SecaoAtual.Projeto.Estaca.EstReal;

       ContaSecoes++;
    }
  }

  CArqCalcAreaLimpeza.Flush();

  ArqCalcAreaLimpeza.SeekToBegin();
  CArqCalcAreaLimpeza << ContaSecoes;

  CArqCalcAreaLimpeza.Close();
  ArqCalcAreaLimpeza.Close();

  GetDlgItem(IDOK)->EnableWindow(true);
  GetDlgItem(IDCANCEL)->EnableWindow(false);
  
  SetDlgItemText(IDC_STAOBS,"OK, os cálculos foram executados com êxito.");
}
