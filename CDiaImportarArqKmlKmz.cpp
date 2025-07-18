// CDiaImportarArqKmlKmz.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include <string>
#include "afxdialogex.h"
#include <set>
#include "ConvUTMGDC\ConvUTMGDC\Utm_Coord_3d.h"
#include "ConvUTMGDC\ConvUTMGDC\Gdc_Coord_3d.h"
#include <array>
#include "CDiaImportarArqKmlKmz.h"
#include "ConvUTMGDC\ConvUTMGDC\Gdc_To_Utm_Converter.h"
#include <math.h>
#include "ConvUTMGDC\ConvUTMGDC\CConvUTMGDC.h"
#include "Monologo.h"
#include "CLerKML.h"
#include "HTTPRequests\HTTPRequests\CHTTPRequests.h"
#include <sstream>
#include <iomanip>
#include "CConfigProj.h"

// caixa de diálogo de CDiaImportarArqKmlKmz

IMPLEMENT_DYNAMIC(CDiaImportarArqKmlKmz, CDialogEx)

extern monologo monolog;

CDiaImportarArqKmlKmz::CDiaImportarArqKmlKmz(CWnd* pParent,const std::string& NomeProj,bool pPonControle, tylstUtm_Coord_3d* pLstPonCtrl,int pTipoArqSaida)	: CDialogEx(IDD_DIAIMPORTARKMLKMZ, pParent),
    NomeProjeto(NomeProj),
    CEdCaminho(_T("")),
    CEdCaminhoArqTerr(_T("")),
    Datum(_T("")),
    StrObs(_T("")),
    bSemCotas(FALSE),
    StaObservacoes(_T("")),
    bPontosControle(pPonControle),
    pLstPontosControle(pLstPonCtrl),
    TipoArqSaida(pTipoArqSaida)
{
}

CDiaImportarArqKmlKmz::~CDiaImportarArqKmlKmz()
{
}

void CDiaImportarArqKmlKmz::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDCAMINHO, CEdCaminho);
  DDX_Text(pDX, IDC_EDCAMINHOARQTERR, CEdCaminhoArqTerr);
  DDX_Control(pDX, IDC_PROGRESSIMP, CProProgresso);
  DDX_Control(pDX, IDC_COMDATUM, ComboDatum);
  DDX_Check(pDX, IDC_RADSEMCOTAS, bSemCotas);
  DDX_Text(pDX, IDC_STAOBSERVACOES, StaObservacoes);
}

BEGIN_MESSAGE_MAP(CDiaImportarArqKmlKmz, CDialogEx)
  ON_BN_CLICKED(IDC_BUTTESTAR, &CDiaImportarArqKmlKmz::OnBnClickedButtestar)
  ON_BN_CLICKED(IDC_BUTPEGARARQ, &CDiaImportarArqKmlKmz::OnBnClickedButpegararq)
  ON_BN_CLICKED(IDC_BUTPEGARARQTERR, &CDiaImportarArqKmlKmz::OnBnClickedButpegararqterr)
  ON_COMMAND(IDC_RADSEMCOTAS, &CDiaImportarArqKmlKmz::OnRadsemcotas)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDiaImportarArqKmlKmz

void CDiaImportarArqKmlKmz::OnBnClickedButtestar()
{
  UpdateData(true);

  if (CEdCaminho.GetLength() < 3)
  {
    AfxMessageBox("É necessário Selecionar um arquivo com os pontos de controle.");
    return;
  }

  if (CEdCaminhoArqTerr.GetLength() < 3)
  {
    AfxMessageBox("É necessário Selecionar um arquivo com os pontos de terreno.");
    return;
  }

  std::string Nomecompleto(NomeProjeto + ".sup");

  DWORD dwAttrib(GetFileAttributes(Nomecompleto.c_str()));
  bool TemSuperfCalculada(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_ARCHIVE));
  int NumDatum(ComboDatum.GetCurSel());

  for (int C = 0; C < 2; C++)
  {
    if (C == 0)
    {
      if (TemSuperfCalculada)
      {
        if (AfxMessageBox("ATENÇÃO => A superficie atual será removida, os perfis longitudinais, volumes, greide, etc estarão inválidos, CONFIRMA?", MB_YESNO | MB_ICONEXCLAMATION) != IDYES) return;
      }

      if (NumDatum == -1)
      {
        AfxMessageBox("Selecione um Datum");
        return;
      }
    }

    CProProgresso.SetRange(0, 100);
    CProProgresso.SetPos(0);

    tylstGdc_Coord_3d LstCoordGdc;
    tylstUtm_Coord_3d LstCoordUTM;

    //--- O último arquivo lido tem que ser o de pontos de controle pois eles são passados para o desenho da superfície

    std::string NomeArqAtual(C == 1 ? CEdCaminho.GetBuffer() : CEdCaminhoArqTerr.GetBuffer());

    CLerKML LerKML(NomeArqAtual.c_str(), LstCoordGdc);

    CProProgresso.SetPos(30);

    CConvUTMGDC::ConverteGDCUTM(LstCoordGdc, LstCoordUTM, NumDatum);

    if (LstCoordGdc.size() == 0)
    {
      AfxMessageBox("Não foram encontrados pontos neste arquivo.");
      return;
    }

    CConfigProj ConfigProj(NULL, NomeProjeto.c_str());
    ConfigProj.LeArquivo();

    CProProgresso.SetPos(40);

    if (!bSemCotas)
    {
      int ContaPontos(0);
      std::string json;
      ittylstGdc_Coord_3d itGdc(LstCoordGdc.begin());
      ittylstUtm_Coord_3d itUTMAtual(LstCoordUTM.begin());

      while (itGdc != LstCoordGdc.end())
      {
        std::stringstream url;

        url << std::right << std::fixed << std::setfill(' ') << std::setprecision(8);

        url << "https://maps.googleapis.com/maps/api/elevation/json?locations=";

        for (; (++ContaPontos % 500 != 0) && itGdc != LstCoordGdc.end(); itGdc++)
        {
          url << itGdc->latitude << ',' << itGdc->longitude << '|';
        }

        if (itGdc != LstCoordGdc.end())
        {
          url << itGdc->latitude << ',' << itGdc->longitude;
          itGdc++;
        }
        else  url.seekp(-1, std::ios_base::end);

        url << "&key=AIzaSyCxzD2v-qv2U02qpUGziMG0PZeUecWYv7I";

        //url << "&key=AIzaSyByfzKZp8NJnMNSDPuLUv4y9g_gTk9HO8w";                   //--- Nova Chave 18-06-2020

        url << ConfigProj.ChaveGoogleEarth;

        json = CHTTPRequests::EnviaRequisicaoPegaResposta(url.str());

        if (PegaStatus(json) == false)
        {
          if(json.size() == 0)
          {
            json = "Sem acesso a internet";

            MessageBox(json.c_str());

            StaObservacoes = "Para importar cotas do Google Earth é necessária uma conexão com a internet\r\n";
          }
          else
          {
            MessageBox(json.c_str());
          }

          StaObservacoes += "Houve erro. O arquivo NÃO foi importado com êxito.";

          UpdateData(false);

          GetDlgItem(IDOK)->EnableWindow(false);
          GetDlgItem(IDC_BUTTESTAR)->EnableWindow(false);

          return;
        }

        PegaCotas(json, itUTMAtual);

        if (CProProgresso.GetPos() < 90) CProProgresso.StepIt();
      }

      CProProgresso.SetPos(95);
    }

    if (TemSuperfCalculada && C == 1)
    {
      DeleteFile(std::string(NomeProjeto + ".sup").c_str());
      DeleteFile(std::string(NomeProjeto + ".aci").c_str());
      DeleteFile(std::string(NomeProjeto + ".bor").c_str());
      DeleteFile(std::string(NomeProjeto + ".cnd").c_str());
      DeleteFile(std::string(NomeProjeto + ".res").c_str());
    }

    TipoArqSaida = C;

    GravaArquivoXYZ(LstCoordUTM);

    if (pLstPontosControle != nullptr) *pLstPontosControle = LstCoordUTM;
  }

  CProProgresso.SetPos(100);

  GetDlgItem(IDOK)->EnableWindow(true);
  GetDlgItem(IDC_BUTTESTAR)->EnableWindow(false);
  GetDlgItem(IDCANCEL)->EnableWindow(false);

  StaObservacoes = "OK, o Arquivo foi importado com êxito.";

  UpdateData(false);
}

bool CDiaImportarArqKmlKmz::PegaCotas(const std::string& json,ittylstUtm_Coord_3d& itCoordUTM)
{
  size_t PosAtual(0),PosFinal(std::string::npos);
  std::string strTemp;
  bool Fim(false);

  while(!Fim && PosAtual != std::string::npos)
  {
    //--- Extrai a elevacao

    PosAtual = json.find("elevation", PosAtual);

    if (PosAtual == std::string::npos) break; //--- Fim

    PosAtual = json.find(':', PosAtual);

    PosAtual++;

    PosFinal = json.find(',', PosAtual);
    strTemp = json.substr(PosAtual, PosFinal-PosAtual);
    itCoordUTM->z = atof(strTemp.c_str());

    PosAtual = PosFinal;
    itCoordUTM++;   
  }
  return true;
}

bool CDiaImportarArqKmlKmz::GravaArquivoXYZ(const tylstUtm_Coord_3d& LstCoorUTM)
{
  std::string NomeArq(NomeProjeto);
  std::ofstream ArqXYZ(NomeArq + (TipoArqSaida == 0 ? ".xyz" : ".txt"), std::ofstream::out | std::ofstream::trunc);

  if (ArqXYZ.good())
  {
    ArqXYZ << std::right << std::fixed << std::setfill(' ') << std::setprecision(8);

    for (cittylstUtm_Coord_3d& citPontoUTM = LstCoorUTM.begin(); citPontoUTM != LstCoorUTM.end(); citPontoUTM++)
    {
      ArqXYZ << citPontoUTM->NumPonto << ' ' << citPontoUTM->y << ' ' << citPontoUTM->x << ' ' << citPontoUTM->z << ' ' << citPontoUTM->Observacoes << std::endl;
    }
  }
  else
  {
    std::stringstream Msg;

    Msg << "Houve erro na abertura do arquivo : " << NomeArq << "." << std::endl;

    AfxMessageBox(Msg.str().c_str());
  }

  return true;
}

void CDiaImportarArqKmlKmz::OnBnClickedButpegararq()
{
  std::string Ext, filtro;

  filtro = "Arquivos pontos Google Earth | *.kml||";
    
  CFileDialog diaabrearq(TRUE,NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro.c_str(),NULL);
  diaabrearq.m_ofn.lpstrTitle = "Vias Urbanas & Estradas - Abrir arquivo Coordenadas Geodésicas - Pontos de Controle:";
  if (diaabrearq.DoModal() == IDOK)
  {
    NomeArq = diaabrearq.GetPathName();
    if (!NomeArq.empty())
    {
      Ext = diaabrearq.GetFileExt();
      std::transform(Ext.begin(),Ext.end(),Ext.begin(), ::toupper);
      if (!(Ext == "KML")) monolog.mensagem(4, NomeArq.c_str()); //--- Erro, arquivo inválido.
      else
      {
        CEdCaminho = NomeArq.c_str();
        UpdateData(FALSE);
      }
    }
  }
}

void CDiaImportarArqKmlKmz::OnBnClickedButpegararqterr()
{
  std::string Ext, filtro;

  filtro = "Arquivos pontos Google Earth | *.kml||";

  CFileDialog diaabrearq(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro.c_str(), NULL);
  diaabrearq.m_ofn.lpstrTitle = "Vias Urbanas & Estradas - Abrir arquivo Coordenadas Geodésicas - Pontos de Terreno:";
  if (diaabrearq.DoModal() == IDOK)
  {
    NomeArq = diaabrearq.GetPathName();
    if (!NomeArq.empty())
    {
      Ext = diaabrearq.GetFileExt();
      std::transform(Ext.begin(), Ext.end(), Ext.begin(), ::toupper);
      if (!(Ext == "KML")) monolog.mensagem(4, NomeArq.c_str()); //--- Erro, arquivo inválido.
      else
      {
        CEdCaminhoArqTerr = NomeArq.c_str();
        UpdateData(FALSE);
      }
    }
  }
}

BOOL CDiaImportarArqKmlKmz::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  ComboDatum.InsertString(0,"SIRGAS2000");
  ComboDatum.InsertString(1,"Córrego Alegre");
  ComboDatum.InsertString(2,"WGS72");
  ComboDatum.InsertString(3,"South America 1969");
  ComboDatum.InsertString(4,"GRS80");

  ComboDatum.SetCurSel(0);

  if (bPontosControle)
  {
    ((CButton *)GetDlgItem(IDC_RADSEMCOTAS))->SetCheck(false);
    ((CButton *)GetDlgItem(IDC_RADSEMCOTAS))->EnableWindow(false);

    bSemCotas = false;
  }

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: as páginas de propriedade OCX devem retornar FALSE
}

void CDiaImportarArqKmlKmz::OnRadsemcotas()
{
  bSemCotas = !bSemCotas;
  UpdateData(false);
}

bool CDiaImportarArqKmlKmz::PegaStatus(const std::string& json)
{
  size_t PosAtual(0), PosFinal(std::string::npos);
  std::string strTemp;

  PosAtual = json.find("\"status\"", PosAtual);

  if (PosAtual == std::string::npos) return false; //--- ??? 

  PosAtual = json.find('"', ++PosAtual);
  PosAtual = json.find('"', ++PosAtual);

  PosAtual++;

  PosFinal = json.find('"', PosAtual);
  strTemp = json.substr(PosAtual, PosFinal - PosAtual);
  
  return strTemp == "OK";
}
