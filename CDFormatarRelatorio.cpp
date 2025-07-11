// CDFormatarRelatorio.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "CDFormatarRelatorio.h"
#include "afxdialogex.h"

// caixa de diálogo de CDFormatarRelatorio

IMPLEMENT_DYNAMIC(CDFormatarRelatorio, CDialogEx)

CDFormatarRelatorio::CDFormatarRelatorio(CWnd* pParent,int* pCasasDec )
	: CDialogEx(IDD_DIAFORMATARRELAT, pParent)
  , cheFormatar(true)
  , pCasasDecimais(pCasasDec)
{
}

CDFormatarRelatorio::~CDFormatarRelatorio()
{
}

void CDFormatarRelatorio::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Check(pDX, IDC_CHEFORMATAR, cheFormatar);
  DDX_Control(pDX, IDC_COMCASASDECIMAIS, ComboCasasDecimais);
  DDX_Control(pDX, IDC_STACASASDEC, staCasasDec);
}

BEGIN_MESSAGE_MAP(CDFormatarRelatorio, CDialogEx)
ON_BN_CLICKED(IDC_CHEFORMATAR, &CDFormatarRelatorio::OnClickedCheformatar)
ON_BN_CLICKED(IDOK, &CDFormatarRelatorio::OnBnClickedOk)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDFormatarRelatorio

BOOL CDFormatarRelatorio::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  std::string strCasasDec[10] = { "0","1","2","3","4","5","6","7","8","9" };

  for (int C = 0; C < 10; C++)
  {
    ComboCasasDecimais.InsertString(C, strCasasDec[C].c_str());
  }

  if (pCasasDecimais && *pCasasDecimais > -1)
  {
    cheFormatar = true;
    ComboCasasDecimais.EnableWindow(cheFormatar);
    staCasasDec.EnableWindow(cheFormatar);
    ComboCasasDecimais.SetCurSel(*pCasasDecimais);
  }
  else
  {
    cheFormatar = false;
    ComboCasasDecimais.EnableWindow(false);
    staCasasDec.EnableWindow(false);
    ComboCasasDecimais.SetCurSel(-1);
  }

  UpdateData(false);
  

  // TODO:  Adicione inicialização extra aqui

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: as páginas de propriedade OCX devem retornar FALSE
}

void CDFormatarRelatorio::OnClickedCheformatar()
{
  cheFormatar = !cheFormatar;

  ComboCasasDecimais.EnableWindow(cheFormatar);
  staCasasDec.EnableWindow(cheFormatar);
}


void CDFormatarRelatorio::OnBnClickedOk()
{
  // TODO: Adicione seu código de manipulador de notificações de controle aqui

  if (pCasasDecimais)
  {
    *pCasasDecimais = cheFormatar ? ComboCasasDecimais.GetCurSel() : -1;
  }

  UpdateData(TRUE);

  ComboCasasDecimais.GetCurSel();

  CDialogEx::OnOK();
}
