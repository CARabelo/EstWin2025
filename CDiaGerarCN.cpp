// CDiaGerarCN.cpp : implementation file
//

#include "stdafx.h"
#include "afxtempl.h"
#include "est.h"
#include "CDiaGerarCN.h"
#include "cponto.h"
#include <string>
#include "estaca.h"
#include "clpontos.h"
#include "perfil.h"
#include <list>
#include <set>
#include "cdeltasuperficie.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include <map>
#include "CAcidentesTopograficos.h"
#include "CSegmento.h"
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "CEquipDrenagem.h"
#include "Superficie.h"
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"

// CDiaGerarCN dialog

IMPLEMENT_DYNAMIC(CDiaGerarCN, CDialog)

CDiaGerarCN::CDiaGerarCN(CWnd* pParent)	: CDialog(CDiaGerarCN::IDD, pParent), doEquiCN(1.0)
{
}

CDiaGerarCN::~CDiaGerarCN()
{
}

void CDiaGerarCN::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIEQUICN, doEquiCN);
  DDV_MinMaxDouble(pDX, doEquiCN, 0.05, 1000.0);
  DDX_Control(pDX, IDC_PROCALCCN, ProCalcCN);
}

BEGIN_MESSAGE_MAP(CDiaGerarCN, CDialog)
  ON_BN_CLICKED(IDC_BUTINICIAR, &CDiaGerarCN::OnBnClickedButiniciar)
END_MESSAGE_MAP()

// CDiaGerarCN message handlers

#define GRAVAR 2  // CSuperficie

void CDiaGerarCN::OnBnClickedButiniciar()
{
  UpdateData(true);

  SetDlgItemText(IDC_STAOBS,"Lendo os arquivos..."); 
  UpdateData(false);

  ProCalcCN.SetRange(0,2);
  ProCalcCN.SetStep(1);
  ProCalcCN.SetPos(1);

  CSuperficie Superficie(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()));

  ProCalcCN.StepIt();

  if (Superficie.TemBordos())
  {
    Superficie.AtribuiEquidisCN(doEquiCN);

    SetDlgItemText(IDC_STAOBS, "Gerando as Curvas de n�vel...");
    UpdateData(false);
    SSetPontos PontosProblema;

    if (Superficie.InterpolarCotas(Superficie.pSuperficieAtual->begin(), Superficie.pSuperficieAtual->end(), ProCalcCN, PontosProblema, doEquiCN) == true)
    {
      SetDlgItemText(IDC_STAOBS, "Gravando as curvas de n�vel...");
      UpdateData(false);

      if (Superficie.Serialize(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()), GRAVAR, false, true) == 1)
        SetDlgItemText(IDC_STAOBS, "OK, As curvas de n�vel foram geradas com �xito.");
      else SetDlgItemText(IDC_STAOBS, "Houve erro na grava��o as curvas de n�vel.");
    }
    else
    {
      SetDlgItemText(IDC_STAOBS, "Houve erro!, As curvas de n�vel N�O foram geradas com �xito.");
    }
  }
  else  SetDlgItemText(IDC_STAOBS,"N�o existem bordos nesta superf�cie, as curvas de n�vel n�o foram geradas."); 

  GetDlgItem(IDC_BUTINICIAR)->EnableWindow(false);
  GetDlgItem(IDCANCEL)->EnableWindow(false);
  GetDlgItem(IDOK)->EnableWindow(true);
}
