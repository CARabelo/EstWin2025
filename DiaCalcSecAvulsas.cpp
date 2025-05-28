// DiaCalcSecAvulsas.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include "diacalcsecavulsas.h"
#include "supercstring.h"
#include "math.h"
#include "estaca.h"
#include "afxtempl.h"
#include "cponto.h"
#include "clpontos.h"
#include <string>
#include "CSecaoAvulsa.h"
#include "CArqSecoesAvulsas.h"
#include <map>
#include "CSecoesAvulsas.h"
#include "ccurhorizontal.h"
#include "carqcurhorizontais.h"
#include <set>
#include "perfil.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include <map>
#include "Mat.h"
#include "ccurhorizontais.h"
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"

extern class Matem Mat;

// CDiaCalcSecAvulsas dialog

IMPLEMENT_DYNAMIC(CDiaCalcSecAvulsas, CDialog)  CDiaCalcSecAvulsas::CDiaCalcSecAvulsas(CWnd* pParent /*=NULL*/)	: CDialog(CDiaCalcSecAvulsas::IDD, pParent)
{
}

CDiaCalcSecAvulsas::~CDiaCalcSecAvulsas()
{
}

void CDiaCalcSecAvulsas::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDiaCalcSecAvulsas, CDialog)
  ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDiaCalcSecAvulsas message handlers

void CDiaCalcSecAvulsas::OnBnClickedOk()
{
 // Ponto PR1(500,1000),PR2(500,800), CC(450,913.397),S1,S2;

 // Mat.CalcInterRetaCurva(PR1,PR2,CC,70,S1,S2);
	/*
  CSecTipo SecoesTipo(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()));
  CSecoesAvulsas SecoesAvulsas(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()));
  CCurHorizontais CurHorizontais(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()),SecoesTipo);

  CList<CSecaoAvulsa,CSecaoAvulsa&>& LSecAvulsas = SecoesAvulsas.PegaLSecoesAvulsas();

  for(POSITION Atual= LSecAvulsas.GetHeadPosition() ; Atual != NULL ; LSecAvulsas.GetNext(Atual))
  {
    CSecaoAvulsa& SecAvulsa(LSecAvulsas.GetAt(Atual));
    Ponto PInter;

    CEstaca EstacaEstEixo(CurHorizontais.CalculaIntersecaoEixo(SecAvulsa.PInicial,SecAvulsa.PFinal,PInter));
  
  //  if(EstacaEstEixo.EstReal != INFINITO)
    {
      SecAvulsa.Estaca = EstacaEstEixo;
      SecAvulsa.Angulo = Mat.CalculaAzimute(SecAvulsa.PInicial,SecAvulsa.PFinal) * 180.0 / Matem::M_PI;
      SecAvulsa.Angulo = Mat.g_gms(SecAvulsa.Angulo);
      SecAvulsa.LadoEsq = PInter.z;
      SecAvulsa.LadoDir = PInter.i;
      SecAvulsa.PEstaca = PInter;
    }
  }

  SecoesAvulsas.SerializeSecoesCalculadas(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()).GetBuffer(),2);
  */
}
