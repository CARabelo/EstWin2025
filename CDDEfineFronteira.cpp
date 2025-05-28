// CDDEfineFronteira.cpp : implementation file
//

#include "stdafx.h"
#include "Est.h"
//#include "CDDEfineFronteira.h"
#include "afxdialogex.h"
#include <string>
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "CalcSU\CalcSU.h"
#include <set>
#include "CPonto.h"
#include "CalcSU\CCCalcSU.h"
//---->  Estes includes são apenas para poder visualizar o DesPonGeomView ---
#include "afxtempl.h"
#include "carqterreno.h"
#include <string>
#include "mat.h"
#include "estaca.h"
#include "clpontos.h"
#include "perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include <list>
#include <set>
#include <vector>
#include <algorithm>
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
#include "superficie.h"
#include "xygreide.h"
#include "meleditbox.h"
#include "cdpopuppontos.h"
#include "supercstring.h"
#include "carqcurhorizontais.h" 
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "ccurhorizontal.h"
#include "ccurhorizontais.h"
#include "carqsecoesavulsas.h"
#include "csecaoavulsa.h"
#include "csecoesavulsas.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include "CLembrete.h"
#include "CRadar.h"
#include "CArqAciTopograficos.h"
#include "CTrecho.h"
#include "CArqTrechos.h"
#include "CTrechos.h"
#include "Picture.h"
//#include "ximage.h"
//#include ".\cximage600_full\CxImage\ximage.h"
#include "CSecoesTerrap.h"
#include "ConvUTMGDC\ConvUTMGDC\Utm_Coord_3d.h"
#include "ConvUTMGDC\ConvUTMGDC\Gdc_Coord_3d.h"
#include <array>
#include "ConvUTMGDC\ConvUTMGDC\Gdc_To_Utm_Converter.h"
#include <math.h>
#include "ConvUTMGDC\ConvUTMGDC\CConvUTMGDC.h"
#include "CCamadaDGN.h"

#include "CTextoMapaIBGE.h"
#include "CCNMapaIGBE.h"
#include "CMapaCNIBGE.h"
#include "CDiaImportarArqCtrlDrones.h"
#include "despontosgeomview.h"
//---------------------------------------------------------------------------------
#include "CDDEfineFronteira.h"

// CDDEfineFronteira dialog

extern Matem Mat;

IMPLEMENT_DYNAMIC(CDDEfineFronteira, CDialog)

extern int CalcAS(CString& NomeProj,CProgressCtrl* pProgresso,CStatic* pCSTexto,unsigned long DMP);

CDDEfineFronteira::CDDEfineFronteira(CWnd* pParent) : CDialog(CDDEfineFronteira::IDD, pParent), doDMT(20.0), DMPIdeal(0),pDesPonGeomView((DesPontosGeomView*)pParent),Erro(0)
{
}

CDDEfineFronteira::~CDDEfineFronteira()
{
  WriteWindowPlacement();
}

void CDDEfineFronteira::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIDMP, doDMT);
	DDV_MinMaxUInt(pDX, doDMT, 0.001, 10000.0);
}

BEGIN_MESSAGE_MAP(CDDEfineFronteira, CDialog)
  ON_BN_CLICKED(IDOK, &CDDEfineFronteira::OnBnClickedOk)
  ON_BN_CLICKED(IDC_BUTCALCULAR, &CDDEfineFronteira::OnBnClickedButcalcular)
END_MESSAGE_MAP()

void CDDEfineFronteira::OnBnClickedOk()
{
   if(!Erro) CDialog::OnOK();
   else CDialog::OnCancel();
}

#include "CConfigProj.h"

void CDDEfineFronteira::OnBnClickedButcalcular()
{
  double AnguloRotacao(0.0);
  
  if (pDesPonGeomView)
  {
    AnguloRotacao = pDesPonGeomView->PegaAngRotacaoSuperf();

    if (AnguloRotacao != 0.0)
    {
      pDesPonGeomView->Destruindo = true;          //--- Simula o destruindo para a tela não piscar
      pDesPonGeomView->Desrotacionar();
    }
  }

  CString Nome(((CMainFrame*) AfxGetMainWnd())->PegaProjetoBase());
  CProgressCtrl* pProgress((CProgressCtrl *) GetDlgItem(IDC_PROALPHASHAPE));
  CStatic* pMensagem((CStatic *) GetDlgItem(IDC_STAMENSAGEM));

  pProgress->SetRange(0,100);

  UpdateData(true);

  CConfigProj ConfigProjeto(NULL, Nome);
  ConfigProjeto.LeArquivo();

  Erro = CCalcSU::CalculaAS(Nome,pProgress,pMensagem,doDMT * 100.0,ConfigProjeto.FormArqPontos, ((CMainFrame*)AfxGetMainWnd())->TipoSuperficie);

  if (Erro == 0)
  {
    if (AnguloRotacao != 0.0)
    {
      pDesPonGeomView->Destruindo = true;             //--- Simula o destruindo para não piscar a tela no rotacionar;
      pDesPonGeomView->Rotacionar(AnguloRotacao);
    }

    pDesPonGeomView->Destruindo = false;

    ((CButton*)GetDlgItem(IDOK))->EnableWindow(true);

    ((CButton*)GetDlgItem(IDC_BUTCALCULAR))->EnableWindow(false);
    ((CButton*)GetDlgItem(IDCANCEL))->EnableWindow(false);
    pProgress->SetPos(100);
  }
}

void CDDEfineFronteira::WriteWindowPlacement()
{
  CString Bloco("DiaDefFronteira");            
  TCHAR szFormat[] = _T("%d"),szBuffer[100];

  wsprintf(szBuffer,szFormat,(int)(doDMT*100.0));

  CString NomeProjeto(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual());

  ::WritePrivateProfileString(Bloco,"Valores",szBuffer,NomeProjeto + ".ini");
}

bool CDDEfineFronteira::ReadWindowPlacement()
{
  CString Bloco("DiaDefFronteira");            

  TCHAR szFormat[] = _T("%d");
  char Buffer[512]= {0};

  CString NomeProjeto(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual());

  ::GetPrivateProfileString(Bloco,"Valores","",Buffer,511,(NomeProjeto + ".ini")); 
  if (strlen(Buffer) == 0) return FALSE;

  int Valor(0);

  int nRead = _stscanf(Buffer, szFormat,&Valor);

  if (nRead != 1)return FALSE;

  doDMT = Valor / 100.0;

  UpdateData(false);

  return TRUE;
}

BOOL CDDEfineFronteira::OnInitDialog()
{
  CDialog::OnInitDialog();

  ReadWindowPlacement();
  
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}
