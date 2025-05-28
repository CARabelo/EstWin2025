// CDiaImpArqTopografia.cpp : arquivo de implementação
#include "Stdafx.h"
#include "Est.h"
#include <string>
#include "CDiaImpArqTopografia.h"
#include "afxdialogex.h"

// caixa de diálogo de CDiaImpArqTopografia

IMPLEMENT_DYNAMIC(CDiaImpArqTopografia, CDialogEx)

CDiaImpArqTopografia::CDiaImpArqTopografia(std::string& NomeArqTrian, std::string& NomeArqAciTopog, std::string& pCNMestra, std::string& pCNInterm,CWnd* pParent) :
           NomeArqTriangulacao(NomeArqTrian), NomeArqAciTopograficos(NomeArqAciTopog), CNMestra(pCNMestra), CNInterm(pCNInterm),CDialogEx(IDD_DIAIMPTRIATOPOG, pParent)
{
}

CDiaImpArqTopografia::~CDiaImpArqTopografia()
{
}

void CDiaImpArqTopografia::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);

  DDX_Text(pDX, IDC_EDIARQMALHATRIANG, NomeArqTriang);
  DDX_Text(pDX, IDC_EDIARQACITOPOG, NomeArqAciTopog);
  DDX_Control(pDX, IDC_COMCM_MESTRA, CComboCN_Mestra);
  DDX_Control(pDX, IDC_COMCN_INTER, CComboCM_Interm);
  DDX_Control(pDX, IDC_EDIARQACITOPOG, CEdit_NomeArqTopog);
}

BEGIN_MESSAGE_MAP(CDiaImpArqTopografia, CDialogEx)
    ON_BN_CLICKED(IDC_BUTPEGARARQMALHA, &CDiaImpArqTopografia::OnBnClickedButpegararqmalha)
    ON_BN_CLICKED(IDC_BUTPEGARARQTOPOGRAFIA, &CDiaImpArqTopografia::OnBnClickedButpegararqtopografia)
    ON_BN_CLICKED(IDOK, &CDiaImpArqTopografia::OnBnClickedOk)
    ON_EN_KILLFOCUS(IDC_EDIARQACITOPOG, &CDiaImpArqTopografia::OnEnKillfocusEdiarqacitopog)
    ON_CBN_SETFOCUS(IDC_COMCM_MESTRA, &CDiaImpArqTopografia::OnCbnSetfocusComcmMestra)
END_MESSAGE_MAP()

void CDiaImpArqTopografia::OnBnClickedButpegararqmalha()
{
  std::string Ext, filtro("Arquivo da Malha Topográfica | *.dxf||");
  CString NomeArquivo;

  CFileDialog DiaAbreArq(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro.c_str(), NULL);
  DiaAbreArq.m_ofn.lpstrTitle = "VU&E- Importar arquivo da Malha Topográfica : ";

  if (DiaAbreArq.DoModal() == IDOK)
  {
    NomeArqTriang = DiaAbreArq.GetPathName();

    UpdateData(false);
  }
}

void CDiaImpArqTopografia::OnBnClickedButpegararqtopografia()
{
  std::string Ext, filtro("Arquivo de Acidentes Topográficos | *.dxf||");
  CString NomeArquivo;

  CFileDialog DiaAbreArq(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro.c_str(), NULL);
  DiaAbreArq.m_ofn.lpstrTitle = "VU&E- Importar arquivo de Acidentes Topográficos : ";

  if (DiaAbreArq.DoModal() == IDOK)
  {
    NomeArqAciTopog = DiaAbreArq.GetPathName();

    UpdateData(false);
  }
}

void CDiaImpArqTopografia::OnBnClickedOk()
{
  UpdateData(true);

  int SelM(CComboCN_Mestra.GetCurSel());

  if (SelM == CB_ERR)
  {
    AfxMessageBox("Selecione o nome da camada onde estão as curvas de nível mestras.");
    return;
  }

  int SelI(CComboCM_Interm.GetCurSel());

  if (SelI == CB_ERR)
  {
    AfxMessageBox("Selecione o nome da camada onde estão as curvas de nível intermediárias.");
    return;
  }

  CString CSCNMestra,CSCNInterm;

  CComboCN_Mestra.GetLBText(SelM, CSCNMestra);
  CComboCM_Interm.GetLBText(SelI, CSCNInterm);

  NomeArqTriangulacao = NomeArqTriang.GetBuffer();
  NomeArqAciTopograficos = NomeArqAciTopog.GetBuffer();
  CNMestra = CSCNMestra.GetBuffer();
  CNInterm = CSCNInterm.GetBuffer();
  
  CDialogEx::OnOK();
}

int CDiaImpArqTopografia::LeCamadasAcad(const std::string& NomeArquivo)
{
  std::ifstream ArqDXFTop(NomeArquivo);
  std::stringstream strstrMsg;

  if (!ArqDXFTop.is_open() || !ArqDXFTop.good())
  {
    std::stringstream Mensagem;

    Mensagem << "Erro na abertura do arquivo: " << NomeArquivo;

    return false;
  }

  std::string Tabelas;
  int C(0);

  do
  {
    C++;
    std::getline(ArqDXFTop, Tabelas);
    Limpa(Tabelas, "\t ");
  } while (Tabelas != "AcDbLayerTableRecord");

  CamadasAcad.clear();

  do
  {
    C++;
    std::getline(ArqDXFTop, Tabelas);
    Limpa(Tabelas, "\t ");

    if (Tabelas == "2")
    {
      C++;
      std::getline(ArqDXFTop, Tabelas);
      Limpa(Tabelas, "\t ");

      CamadasAcad.emplace_back(Tabelas);

      do
      {
        C++;
        std::getline(ArqDXFTop, Tabelas);
        Limpa(Tabelas, "\t ");
      } while (Tabelas != "AcDbLayerTableRecord" && Tabelas != "ENDSEC");
    }
  } while (Tabelas != "ENDSEC");

  return true;
}

void CDiaImpArqTopografia::OnEnKillfocusEdiarqacitopog()
{
  UpdateData(true);

  if (NomeArqAciTopog.GetLength() > 3)
  {
    LeCamadasAcad(NomeArqAciTopog.GetBuffer());

    CComboCN_Mestra.Clear();
    CComboCM_Interm.Clear();

    for (auto ItCamadas : CamadasAcad)
    {
      CComboCN_Mestra.AddString(ItCamadas.c_str());
      CComboCM_Interm.AddString(ItCamadas.c_str());
    }
  }
}

void CDiaImpArqTopografia::OnCbnSetfocusComcmMestra()
{
  UpdateData(true);

  if (NomeArqAciTopog.GetLength() > 7)
  {
    OnEnKillfocusEdiarqacitopog();

    return;
  }
}
