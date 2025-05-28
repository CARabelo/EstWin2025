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

    SetDlgItemText(IDC_STAOBS, "Gerando as Curvas de nível...");
    UpdateData(false);
    SSetPontos PontosProblema;

    if (Superficie.InterpolarCotas(Superficie.pSuperficieAtual->begin(), Superficie.pSuperficieAtual->end(), ProCalcCN, PontosProblema, doEquiCN) == true)
    {
      SetDlgItemText(IDC_STAOBS, "Gravando as curvas de nível...");
      UpdateData(false);

      if (Superficie.Serialize(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()), GRAVAR, false, true) == 1)
        SetDlgItemText(IDC_STAOBS, "OK, As curvas de nível foram geradas com êxito.");
      else SetDlgItemText(IDC_STAOBS, "Houve erro na gravação as curvas de nível.");
    }
    else
    {
      SetDlgItemText(IDC_STAOBS, "Houve erro!, As curvas de nível NÃO foram geradas com êxito.");
    }
  }
  else  SetDlgItemText(IDC_STAOBS,"Não existem bordos nesta superfície, as curvas de nível não foram geradas."); 

  GetDlgItem(IDC_BUTINICIAR)->EnableWindow(false);
  GetDlgItem(IDCANCEL)->EnableWindow(false);
  GetDlgItem(IDOK)->EnableWindow(true);
}
