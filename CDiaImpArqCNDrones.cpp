// CDiaImpArqCNDrones.cpp : arquivo de implementação
//
#include "stdafx.h"
#include "Est.h"
#include "ConvUTMGDC\ConvUTMGDC\Gdc_Coord_3d.h"
#include <set>
#include "ConvUTMGDC\ConvUTMGDC\Utm_Coord_3d.h"
#include <array>
#include "ConvUTMGDC\ConvUTMGDC\Gdc_To_Utm_Converter.h"
#include "CDiaImpArqCNDrones.h"
#include <math.h>
#include "ConvUTMGDC\ConvUTMGDC\CConvUTMGDC.h"
#include "afxdialogex.h"
#include <sstream>
#include <iomanip>
#include "Monologo.h"
#include "CLerKMLCN.h"
#include "CConfigProj.h"

extern monologo monolog;

// caixa de diálogo de CDiaImpArqCNDrones

IMPLEMENT_DYNAMIC(CDiaImpArqCNDrones, CDialogEx)

CDiaImpArqCNDrones::CDiaImpArqCNDrones(CWnd* pParent,const std::string& NomeProj,int pTipoArquivo)	: CDialogEx(IDD_DIAARQCNDRONES, pParent), 
                    CStrCaminhoArqCNDrones(_T("")),
                    NomeProjeto(NomeProj),
                    TipoArquivo(pTipoArquivo)
{
}

CDiaImpArqCNDrones::~CDiaImpArqCNDrones()
{
}

void CDiaImpArqCNDrones::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDCAMINHOARQCNDRONES, CEDCaminhoArqCNDrones);
  DDX_Text(pDX, IDC_EDCAMINHOARQCNDRONES, CStrCaminhoArqCNDrones);
  DDX_Control(pDX, IDC_PROGRESSIMP, CProgresso);
  DDX_Control(pDX, IDC_STAOBSERVACOES, CStaObs);
}

BEGIN_MESSAGE_MAP(CDiaImpArqCNDrones, CDialogEx)
  ON_BN_CLICKED(IDC_BUTPEGARARQCN, &CDiaImpArqCNDrones::OnClickedButpegararqcn)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDiaImpArqCNDrones

void CDiaImpArqCNDrones::OnClickedButpegararqcn()
{
  UpdateData(true);

  if (CStrCaminhoArqCNDrones.GetLength() < 3)
  {
    AfxMessageBox("É necessário Selecionar um arquivo com as curvas de nível.");
    return;
  }

  std::string Nomecompleto(NomeProjeto + ".cnd");

  DWORD dwAttrib(GetFileAttributes(Nomecompleto.c_str()));
  bool TemCN(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_ARCHIVE));
  // int NumDatum(ComboDatum.GetCurSel());
  int NumDatum(0);

  if (TemCN)
  {
    if (AfxMessageBox("ATENÇÃO => Já existem curvas de nivel nesta superfície estas curvas de nível serão substituidas pelas que estão sendo importadas, CONFIRMA?", MB_YESNO | MB_ICONEXCLAMATION) != IDYES) return;
  }

  if (NumDatum == -1)
  {
    AfxMessageBox("Selecione um Datum");
    return;
  }

  std::string Ext, filtro;

  filtro = "Arquivos Curvas de Nível - Drones | *.kml||";

  CFileDialog diaabrearq(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro.c_str(), NULL);
  diaabrearq.m_ofn.lpstrTitle = "Vias Urbanas & Estradas - Abrir arquivo Curvas Nível Drones :";
  if (diaabrearq.DoModal() == IDOK)

  NomeArq = diaabrearq.GetPathName();
  if (!NomeArq.empty())
  {
    Ext = diaabrearq.GetFileExt();
    std::transform(Ext.begin(), Ext.end(), Ext.begin(), ::toupper);
    if (!(Ext == "KML")) monolog.mensagem(4, NomeArq.c_str()); //--- Erro, arquivo inválido.
    else
    {
      CStrCaminhoArqCNDrones = NomeArq.c_str();
      UpdateData(FALSE);
    }
  }

  tylstGdc_Coord_3d LstCoordGdc;
  tylstUtm_Coord_3d LstCoordUTM;

  CLerKMLCN LerKMLCN(NomeArq.c_str(), LstCoordGdc);

  CProgresso.SetPos(30);

  CConvUTMGDC::ConverteGDCUTM(LstCoordGdc, LstCoordUTM, NumDatum);

  if (LstCoordGdc.size() == 0)
  {
    AfxMessageBox("Não foram encontrados pontos neste arquivo.");
    return;
  }

  CConfigProj ConfigProj(NULL, NomeProjeto.c_str());
  ConfigProj.LeArquivo();

  CProgresso.SetPos(40);

 //GravaArquivoXYZ(LstCoordUTM);

  CProgresso.SetPos(100);

  GetDlgItem(IDOK)->EnableWindow(true);
  GetDlgItem(IDC_BUTTESTAR)->EnableWindow(false);
  GetDlgItem(IDCANCEL)->EnableWindow(false);

//  CStaObs = "OK, o Arquivo foi importado com êxito.";

  UpdateData(false);
   
  CProgresso.SetRange(0, 100);
  CProgresso.SetPos(0);

  //--- Arquivo da nuvem de pontos

  std::string NomeArqAtual(CStrCaminhoArqCNDrones.GetBuffer());

  tylstlstGdc_Coord_3d LstLstCoordGdc;
  tylstlstUtm_Coord_3d LstLstCoordUTM;

  LeArqCN(NomeArqAtual, LstLstCoordUTM);

  CProgresso.SetPos(30);

  CConvUTMGDC::ConverteGDCUTM(LstCoordGdc, LstCoordUTM, NumDatum);

  if (LstCoordGdc.size() == 0)
  {
    AfxMessageBox("Não foram encontrados pontos neste arquivo.");
    return;
  }

  //  CConfigProj ConfigProj(NULL, NomeProjeto.c_str());
   // ConfigProj.LeArquivo();

  CProgresso.SetPos(40);

  //GravaArquivoCN(LstCoordUTM, FormatoArqPontos);
}

bool CDiaImpArqCNDrones::LeArqCN(std::string& NomArqAtual, tylstlstUtm_Coord_3d& ListaListaCoordGdc)
{
  std::ifstream ArqCN(NomArqAtual, std::fstream::in);
  unsigned long NumPonto(0);

  ListaListaCoordGdc.clear();

  if (ArqCN.good())
  {
    Gdc_Coord_3d pAtual;
    tylstUtm_Coord_3d ListaCNAtual;

   // ittylstlstUtm_Coord_3d itlstCNAtual(ListaListaCoordGdc.insert(ListaListaCoordGdc.end(),ListaCNAtual));

    while (ArqCN.good())
    {
      ArqCN >> pAtual.longitude >> pAtual.latitude >> pAtual.elevation;

      pAtual.NumPonto = ++NumPonto;

 //     ListaListaCoordGdc.emplace_back(pAtual);
    }
  }
  else
  {
    std::stringstream Msg;

    Msg << "Houve erro na abertura do arquivo : " << NomeArq << "." << std::endl;

    AfxMessageBox(Msg.str().c_str());

    return false;
  }

  return true;
}
