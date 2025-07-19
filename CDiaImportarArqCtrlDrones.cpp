// CDiaImportarArqCtrlDrones.cpp : arquivo de implementação
//
#include "stdafx.h"
#include "Est.h"
#include "CPonto.h"
#include <set>
#include "ConvUTMGDC\ConvUTMGDC\Utm_Coord_3d.h"
#include "ConvUTMGDC\ConvUTMGDC\Gdc_Coord_3d.h"
#include "CDiaImportarArqCtrlDrones.h"
#include "afxdialogex.h"
#include <sstream>
#include <iomanip>
#include "CConfigProj.h"
#include "Monologo.h"
#include <array>
#include "ConvUTMGDC\ConvUTMGDC\Gdc_To_Utm_Converter.h"
#include <math.h>
#include "ConvUTMGDC\ConvUTMGDC\CConvUTMGDC.h"
#include "CLerKMLCN.h"
#include "CLerKML.h"
#include "SimpPL\CSimpPL.h"
#include "Mat.h"
#include "CLerDXFCN.h"
#include "CTextoMapaIBGE.h"

#define SUPERF_GOOGLEEARTH 2
#define SUPERF_GERENMAPAS 3

extern class Matem Mat;
extern monologo monolog;

// caixa de diálogo de CDiaImportarArqCtrlDrones

IMPLEMENT_DYNAMIC(CDiaImportarArqCtrlDrones, CDialogEx)

CDiaImportarArqCtrlDrones::CDiaImportarArqCtrlDrones(setPontos& pSetPontosSoltos,CWnd* pParent,const std::string& NomeProj,bool bPontosControle,tylstUtm_Coord_3d* pLstPonCtrl,int pTipoArq,unsigned int& pEquidisCN,unsigned int& pPCTRLVirt)	
	: CDialogEx(IDD_DIAIMPORTARARQCTRLDRONES, pParent),
  NomeProjeto(NomeProj),
  TemPontosControle(bPontosControle),
  CEdCaminhoPCT(_T("")),
  StrObs(_T("")),
  pLstPontosControle(pLstPonCtrl),
  TipoArquivo(pTipoArq),
  SetPontosSoltos(pSetPontosSoltos),
  EquidisCN(pEquidisCN),
  PonCTRLVirtuais(pPCTRLVirt)
{

  CConfigProj ConfigProj(NULL, NomeProjeto.c_str());

  ConfigProj.LeArquivo();

  for (int C = 0; C < ConfigProj.FormArqPontos.GetLength(); C++) if (ConfigProj.FormArqPontos[C] != ' ') FormatoArqPontos.push_back(ConfigProj.FormArqPontos[C]);
}

CDiaImportarArqCtrlDrones::~CDiaImportarArqCtrlDrones()
{
}

void CDiaImportarArqCtrlDrones::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDCAMINHO, CEdCaminhoPCT);
  DDX_Text(pDX, IDC_EDCAMINHOCN, CEdCaminhoCN);
  DDX_Control(pDX, IDC_COMDATUM2, ComboDatum);
  DDX_Text(pDX, IDC_EDI_EQUIS_CN, EquidisCN);
  DDX_Control(pDX, IDC_STAOBSERVACOES, CStaObserv);
  DDX_Control(pDX, IDC_PROGRESS1, Progress);
}

BEGIN_MESSAGE_MAP(CDiaImportarArqCtrlDrones, CDialogEx)
  ON_BN_CLICKED(IDC_BUTTESTAR, &CDiaImportarArqCtrlDrones::OnBnClickedButtestar)
  ON_BN_CLICKED(IDC_BUTPEGARARQ, &CDiaImportarArqCtrlDrones::OnBnClickedButpegararq)
  ON_BN_CLICKED(IDC_BUTPEGARARQCN, &CDiaImportarArqCtrlDrones::OnClickedButpegararqcn)
  ON_EN_SETFOCUS(IDC_EDCAMINHOCN, &CDiaImportarArqCtrlDrones::OnEnSetfocusEdcaminhocn)
  ON_BN_KILLFOCUS(IDC_BUTPEGARARQ, &CDiaImportarArqCtrlDrones::OnBnKillfocusButpegararq)
  ON_EN_KILLFOCUS(IDC_EDI_EQUIS_CN, &CDiaImportarArqCtrlDrones::OnEnKillfocusEdiEquisCn)
  ON_BN_CLICKED(IDC_CHEPONCTRLVIRTUAIS, &CDiaImportarArqCtrlDrones::OnClickedCheponctrlvirtuais)
END_MESSAGE_MAP()

void CDiaImportarArqCtrlDrones::OnBnClickedButtestar()
{
  UpdateData(true);

  if (TemPontosControle && CEdCaminhoPCT.GetLength() < 8)
  {
    monolog.mensagem(SEMMENSPADRAO,"É necessário Selecionar um arquivo com os pontos de controle.");
    return;
  }

  if (EquidisCN < 1)
  {
    monolog.mensagem(SEMMENSPADRAO,"É necessário digitar a equidistância das curvas de nível.");
    return;
  }

  std::string Nomecompleto(NomeProjeto + ".sup");

  DWORD dwAttrib(GetFileAttributes(Nomecompleto.c_str()));
  bool TemSuperfCalculada(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_ARCHIVE));
  int NumDatum(ComboDatum.GetCurSel());

  if (TemSuperfCalculada)
  {
    if (AfxMessageBox("ATENÇÃO => A superficie atual será removida, os perfis longitudinais, volumes, greide, etc estarão inválidos, CONFIRMA?", MB_YESNO | MB_ICONEXCLAMATION) != IDYES) return;
  }

  if (NumDatum == -1)
  {
    monolog.mensagem(SEMMENSPADRAO,"Selecione um Datum");
    return;
  }

  //--- Arquivo dos pontos de controle

  tylstUtm_Coord_3d LstCoordUTM;

  if(TemPontosControle) LeArqPCTRL(std::string(CEdCaminhoPCT.GetBuffer()),LstCoordUTM,NumDatum);

  if (pLstPontosControle != nullptr) *pLstPontosControle = LstCoordUTM;

  if (TemSuperfCalculada)
  {
    DeleteFile(std::string(NomeProjeto + ".sup").c_str());
    DeleteFile(std::string(NomeProjeto + ".xyz").c_str());
    DeleteFile(std::string(NomeProjeto + ".aci").c_str());
    DeleteFile(std::string(NomeProjeto + ".bor").c_str());
    DeleteFile(std::string(NomeProjeto + ".cnd").c_str());
    DeleteFile(std::string(NomeProjeto + ".res").c_str());
  }

  if (CEdCaminhoCN.GetLength() > 3)
  {
    std::string Nomecompleto(NomeProjeto + ".cnd");

    DWORD dwAttrib(GetFileAttributes(Nomecompleto.c_str()));
    bool TemCN(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_ARCHIVE));
    int NumDatum(ComboDatum.GetCurSel());

    if (TemCN)
    {
      if (AfxMessageBox("ATENÇÃO => As curvas de nível atuais serão removidas, CONFIRMA?", MB_YESNO | MB_ICONEXCLAMATION) != IDYES) return;
    }

    if (NumDatum == -1)
    {
      AfxMessageBox("Selecione um Datum");
      return;
    }

    tylstGdc_Coord_3d LstCoordGdc;
    tylstUtm_Coord_3d LstCoordUTM;
    tylstlstUtm_Coord_3d LstLstCNUTM;

    lstlstPontos CNs;

    switch(TipoArquivo)
    {
      case 3:
      {
        CLerDXFCN LerDXFCN(CEdCaminhoCN.GetBuffer(), CNs,SetPontosSoltos,&Progress,this, &CStaObserv);

        //--- A rotina de ligação das CNs foi feita usando o tipo tylstGdc_Coord_3d
        //--- Como aqui os pontos estao definidos baseados na classe CPonto é preciso fazer a conversão
        //--- Ou sobreescrever a rotina usando-se a classe Ponto.
        //--- Optou-se por fazer a conversão, mais simples mas há uma sobrecarga de tempo
        //--- #AFAZER Fazer um sobrecarga da função usando-se a classe Ponto
       
       TransferePontosParaCoord3d(CNs,LstLstCNUTM);
      }
      break;
      case 2: 
      {
        //@ Colocar progresso aqui

        CLerKMLCN LerKMLCN(CEdCaminhoCN.GetBuffer(),LstCoordGdc,TipoArquivo); break;
      }
      break;
    }

    tylstGdc_Coord_3d LstCNAtual;
    int cont(0);

    if (TipoArquivo == 2)
    {
      for (ittylstGdc_Coord_3d itpCNAtual = LstCoordGdc.begin() ; itpCNAtual != LstCoordGdc.end(); ++itpCNAtual)
      {
        if (itpCNAtual->NumPonto != 0)
        {
          LstCNAtual.push_back(*itpCNAtual);
        }
        else
        {
          /*
            if((TipoArquivo == 3 || ((TipoArquivo == 2) &&
                                     (LstCNAtual.begin()->Observacoes.find("STYLE1") != std::string::npos ||
                                      LstCNAtual.begin()->Observacoes.find("STYLE2") != std::string::npos))))
                                      */
          {
            ittylstlstUtm_Coord_3d itLstCNUTMAtual(LstLstCNUTM.insert(LstLstCNUTM.end(), tylstUtm_Coord_3d()));
            CConvUTMGDC::ConverteGDCUTM(LstCNAtual, *itLstCNUTMAtual, NumDatum);
          }

          LstCNAtual.clear();
        }
      }
    }

    if ((TipoArquivo == 2 && LstCoordGdc.size() == 0) || (TipoArquivo == 3 && CNs.size() == 0))
    {
      AfxMessageBox("Não foram encontrados pontos neste arquivo.");
      return;
    }

    LstCoordGdc.clear();

    CConfigProj ConfigProj(NULL, NomeProjeto.c_str());
    ConfigProj.LeArquivo();

    if(TipoArquivo == 3) 
    {
      CStaObserv.SetWindowText("Calculando as curvas de nível");

      LigaCNsPartidas(LstLstCNUTM);  //-- Nas arquivos DGN do IBGE as cns vem partidas

      CSimpPL::Simplifica(LstLstCNUTM);  //--- Dando pb VERIFICAR

      CStaObserv.SetWindowText("Cotando as curvas de nível");

      CotaCNsIBGE(LstLstCNUTM, SetPontosSoltos);  
    }
    else
    {
      CSimpPL::Simplifica(LstLstCNUTM, TipoArquivo == 2);
    }

    switch (TipoArquivo)
    {
      case 2:
      case 3:
      {
         CStaObserv.SetWindowText("Gravando os arquivos");

         GravaArquivoPontosSoltos(SetPontosSoltos, FormatoArqPontos, NomeProjeto);
         GravaArquivoCN(LstLstCNUTM, FormatoArqPontos, NomeProjeto);;
      }
      break;
    }
  }

  GetDlgItem(IDOK)->EnableWindow(true);
  GetDlgItem(IDC_BUTTESTAR)->EnableWindow(false);
  GetDlgItem(IDCANCEL)->EnableWindow(false);

  Progress.SetPos(100);

  CStaObserv.SetWindowText("OK, os arquivos foram importados com êxito");
}

void CDiaImportarArqCtrlDrones::OnBnClickedButpegararqnuvem()
{
  std::string Ext, filtro;

  filtro = "Arquivo Nuvem de Pontos - Drones | *.nuv||";

  CFileDialog diaabrearq(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro.c_str(), NULL);
  diaabrearq.m_ofn.lpstrTitle = "VU&E- Abrir arquivo Nuvem de Pontos - Drones:";
  if (diaabrearq.DoModal() == IDOK)
  {
    NomeArq = diaabrearq.GetPathName();
    if (!NomeArq.empty())
    {
      Ext = diaabrearq.GetFileExt();
      std::transform(Ext.begin(), Ext.end(), Ext.begin(), ::toupper);
      if (!(Ext == "NUV")) monolog.mensagem(4, NomeArq.c_str()); //--- Erro, arquivo inválido.
      else
      {
        CEdCaminhoNuvem = NomeArq.c_str();
        UpdateData(FALSE);
      }
    }
  }
}

void CDiaImportarArqCtrlDrones::OnBnClickedButpegararq()
{
  std::string Ext, filtro;

  if(TipoArquivo == 2) filtro = "Pontos Controle Drones (*.pct)|*.pct|Pontos Controle Drones (*.kml)|*.kml|Todos os Arquivos (*.*)|*.*||";
  else filtro = "Pontos Controle Drones (*.pct)|*.pct|Pontos Controle Drones (*.kml)|*.kml|Todos os Arquivos (*.*)|*.*||";

  CFileDialog diaabrearq(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro.c_str(), NULL);
  diaabrearq.m_ofn.lpstrTitle = "VU&E- Abrir arquivo de pontos de controle - Drones:";
  if (diaabrearq.DoModal() == IDOK)
  {
    NomeArq = diaabrearq.GetPathName();
    if (!NomeArq.empty())
    {
      Ext = diaabrearq.GetFileExt();
      std::transform(Ext.begin(), Ext.end(), Ext.begin(), ::toupper);

      if((TipoArquivo == 2 && !(Ext == "PCT" || Ext == "KML")) || (TipoArquivo == 3 && !(Ext == "PCT" || Ext == "DXF")))
         monolog.mensagem(4, NomeArq.c_str()); //--- Erro, arquivo inválido.
      else 
      {
        CEdCaminhoPCT = NomeArq.c_str();
        UpdateData(FALSE);

        OnEnSetfocusEdcaminhocn();
      }
    }
  }
}

BOOL CDiaImportarArqCtrlDrones::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  ComboDatum.InsertString(0, "SIRGAS2000 / (WGS-84)");
  ComboDatum.InsertString(1, "Córrego Alegre");
  ComboDatum.InsertString(2, "WGS72");
  ComboDatum.InsertString(3, "South America 1969");
  ComboDatum.InsertString(4, "GRS80");

  ComboDatum.SetCurSel(0);

  if (!TemPontosControle)
  {
    GetDlgItem(IDC_EDCAMINHO)->EnableWindow(false);
    GetDlgItem(IDC_BUTPEGARARQ)->EnableWindow(false);
  }

  GetDlgItem(IDC_EDI_EQUIS_CN)->SetWindowText("");;

  if (TipoArquivo != SUPERF_GERENMAPAS)
  {
//    GetDlgItem(IDC_EDI_EQUIS_CN)->SetWindowText("");;
  //  GetDlgItem(IDC_EDI_EQUIS_CN)->EnableWindow(false);
  //  GetDlgItem(IDC_STAEQUIDIS)->EnableWindow(false);
  //  GetDlgItem(IDC_STAMETROS)->EnableWindow(false);
  }

  GetDlgItem(IDOK)->EnableWindow(false);

  Progress.SetRange(0, 100);
  Progress.SetStep(1);
  Progress.SetPos(0);

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: as páginas de propriedade OCX devem retornar FALSE
}

int CDiaImportarArqCtrlDrones::LeArquivoXYZ()
{
  std::string NomeArq(CEdCaminhoPCT.Mid(0, CEdCaminhoPCT.ReverseFind('.')));
  std::ifstream ArqXYZ(NomeArq + ".xyz", std::fstream::in);

  if (ArqXYZ.good())
  {
    int C(0);
    Utm_Coord_3d pAtual;

    while (C < 4 && ArqXYZ.good())
    {
      std::string FimLinha;

      for (std::string::iterator it = FormatoArqPontos.begin(); it != FormatoArqPontos.end(); it++)
      {
        switch (*it)
        {
          case 'n':  ArqXYZ >> pAtual.NumPonto; break;
          case 'N':  ArqXYZ >> pAtual.y; break;
          case 'E':  ArqXYZ >> pAtual.x; break;
          case 'C':  ArqXYZ >> pAtual.z; break;
          case 'O':  ArqXYZ >> pAtual.Observacoes; break;
        }

        std::getline(ArqXYZ, FimLinha);
      }

      if (pAtual.Observacoes.find("PCTRL") != std::string::npos)
      {
        pLstPontosControle->emplace_back(pAtual);
        C++;
      }
    }

    if (C < 4)
    {
      AfxMessageBox("Não foram encontrados 4 pontos de controle no arquivo. Processo abortado.");

      return false;
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

bool CDiaImportarArqCtrlDrones::GravaArquivoXYZ(tylstUtm_Coord_3d& LstCoorUTM,std::string& FormatoArqPontos)
{
  std::string NomeArq(CEdCaminhoPCT.Mid(0, CEdCaminhoPCT.ReverseFind('.')));
  std::ofstream ArqXYZ(NomeArq + ".xyz", std::fstream::out | std::fstream::trunc);

  if (ArqXYZ.good())
  {
    ArqXYZ << std::right << std::fixed << std::setfill(' ') << std::setprecision(8);

    for (ittylstUtm_Coord_3d& itPontoUTM = LstCoorUTM.begin(); itPontoUTM != LstCoorUTM.end(); itPontoUTM++)
    {
      for(std::string::iterator it = FormatoArqPontos.begin() ; it != FormatoArqPontos.end() ; ++it)
      {
        switch (*it)
        {
          case 'n':  ArqXYZ << itPontoUTM->NumPonto; break;
          case 'N':  ArqXYZ << itPontoUTM->y; break;
          case 'E':  ArqXYZ << itPontoUTM->x; break;
          case 'C':  ArqXYZ << itPontoUTM->z; break;
          case 'O':  ArqXYZ << itPontoUTM->Observacoes; break;
        }
        ArqXYZ << ' ';
      }

      ArqXYZ << std::endl;
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

bool CDiaImportarArqCtrlDrones::LeArqPCTRL(std::string& NomArq, tylstUtm_Coord_3d& ListaCoordUTM,int NumDatum)
{
  std::string CopiaNomeArq(NomArq);

  std::transform(CopiaNomeArq.begin(), CopiaNomeArq.end(), CopiaNomeArq.begin(), ::toupper);

  if (CopiaNomeArq.find(".PCT") != std::string::npos)
  {
    std::ifstream ArqCTRL(NomArq, std::fstream::in);

    ListaCoordUTM.clear();

    if (ArqCTRL.good())
    {
      Utm_Coord_3d pAtual;
      char strBuffer[1000] = {0};
      std::string Bufferstr;

      while (ArqCTRL.good() && ListaCoordUTM.size() < 4)
      {
        pAtual.x = pAtual.y = pAtual.z = INFINITO;
        pAtual.NumPonto = -1;

        for (std::string::iterator it = FormatoArqPontos.begin(); it != FormatoArqPontos.end(); it++)
        {
          switch (*it)
          {
            case 'n':  ArqCTRL >> pAtual.NumPonto; break;
            case 'N':  ArqCTRL >> pAtual.y; break;
            case 'E':  ArqCTRL >> pAtual.x; break;
            case 'C':  ArqCTRL >> pAtual.z; break;
            case 'O':
            {
              ArqCTRL >> strBuffer;
              pAtual.Observacoes = strBuffer;
              break;
            }
          }
        }

        std::getline(ArqCTRL, Bufferstr);

        if(pAtual.x != INFINITO && pAtual.y != INFINITO && pAtual.z != INFINITO && pAtual.NumPonto != -1)
        {
          ListaCoordUTM.emplace_back(pAtual);
        }
      }

      if (ListaCoordUTM.size() != 4)
      {
        AfxMessageBox("Houve erro na leitura do arquivo  de pontos de controle (.pct)");

        return false;
      }
    }
    else
    {
      std::stringstream Msg;

      Msg << "Houve erro na abertura do arquivo : " << NomeArq << "." << std::endl;

      AfxMessageBox(Msg.str().c_str());

      return false;
    }
  }
  else
  {
    tylstGdc_Coord_3d LstCoordGdc;

    CLerKML LerKML(NomArq.c_str(), LstCoordGdc);

    CConvUTMGDC::ConverteGDCUTM(LstCoordGdc, ListaCoordUTM, NumDatum);

    if (LstCoordGdc.size() == 0)
    {
      AfxMessageBox("Não foram encontrados pontos neste arquivo.");
      return false;
    }
  }

  return true;
}

void CDiaImportarArqCtrlDrones::OnClickedButpegararqcn()
{
  std::string Ext, filtro;

  if(TipoArquivo == 2) filtro = "Arquivo de curvas de nível - Drones | *.kml||";
  else filtro = "Arquivo de curvas de nível - Drones | *.dxf||";

  CFileDialog diaabrearq(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro.c_str(), NULL);

  if(TipoArquivo == 2) diaabrearq.m_ofn.lpstrTitle = "VU&E- Abrir arquivo de curvas de nível - Drones:";
  else diaabrearq.m_ofn.lpstrTitle = "VU&E- Abrir arquivo de curvas de nível - Mapas:";

  if (diaabrearq.DoModal() == IDOK)
  {
    NomeArq = diaabrearq.GetPathName();
    if (!NomeArq.empty())
    {
      Ext = diaabrearq.GetFileExt();
      std::transform(Ext.begin(), Ext.end(), Ext.begin(), ::toupper);

      if (TipoArquivo == 2 && !(Ext == "KML") || TipoArquivo == 3 && !(Ext == "DXF")) monolog.mensagem(4, NomeArq.c_str()); //--- Erro, arquivo inválido.
      else
      {
        CEdCaminhoCN = NomeArq.c_str();
        UpdateData(FALSE);
      }
    }
  }
}

bool CDiaImportarArqCtrlDrones::LeArqCN(std::string& NomArqAtual, tylstGdc_Coord_3d& ListaCoordGdcCN)
{
  std::ifstream ArqCN(NomArqAtual, std::fstream::in);
  unsigned long NumPonto(0);

  ListaCoordGdcCN.clear();

  if (ArqCN.good())
  {
    Gdc_Coord_3d pAtual;

    while (ArqCN.good())
    {
      ArqCN >> pAtual.longitude >> pAtual.latitude >> pAtual.elevation;

      pAtual.NumPonto = ++NumPonto;

      ListaCoordGdcCN.emplace_back(pAtual);
    }
  }
  else
  {
    std::stringstream Msg;

    Msg << "Houve erro na abertura do arquivo : " << NomArqAtual << "." << std::endl;

    AfxMessageBox(Msg.str().c_str());

    return false;
  }

  return true;
}

bool CDiaImportarArqCtrlDrones::GravaArquivoCN(tylstlstUtm_Coord_3d& LstLstCoorCN, std::string& FormatoArqPontos,std::string& NomeProj)
{
  CFile Arq;
  CFileException e;

  std::string NomeArq(NomeProj + ".cnd");

  if (Arq.Open(NomeArq.c_str(), CFile::modeCreate | CFile::modeWrite, &e) == 0)
  {
    std::stringstream Msg;
    Msg << "Houve erro na abertura do arquivo : " << NomeArq << "." << std::endl;
    AfxMessageBox(Msg.str().c_str());

    return false;
  }
  else
  {
    ULONGLONG QtdCN(LstLstCoorCN.size());
    CArchive CArcCurvasNivel(&Arq, CArchive::store);
    unsigned int QtdCurvasNivel(0);

    if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
    {
      tylstlstUtm_Coord_3d * pLstLstCoorCN(&LstLstCoorCN);

      //--- não precisa do desembaracaCN o simplifica aceita curvas embaraçadas

      //--- DesembaracaCNs(&pLstLstCoorCN);  //--- dando pb VERIFICAR

      //--- alguma coisa no simplifica esta estragando a triangulação, muitos pontos não estao sendo
      //--- incluidos na triangulação ===>> Verificar


      //--- @Simplifica esta simplificando demais verificar mais opçoes
	  //--- simplificando 2x
      //CSimpPL::Simplifica(*pLstLstCoorCN);  

      CArcCurvasNivel << 0.0 << (double) EquidisCN;                 //--- Angulo rotacao e equidistancia das CNs
      CArcCurvasNivel << pLstLstCoorCN->size();

      for (ittylstlstUtm_Coord_3d& itLstCN = pLstLstCoorCN->begin(); itLstCN != pLstLstCoorCN->end(); ++itLstCN)
      {
        CArcCurvasNivel << itLstCN->size();

        ittylstUtm_Coord_3d itPontoUTMAnt(itLstCN->begin()), itPontoUTMAtual(itPontoUTMAnt);

        Ponto PAnterior(itPontoUTMAnt->Observacoes, itPontoUTMAnt->x, itPontoUTMAnt->y, itPontoUTMAnt->z);
        Ponto* pPonAnt(&PAnterior);
        
        for(itPontoUTMAtual++; itPontoUTMAtual != itLstCN->end() ; ++itPontoUTMAtual)
        {
          Ponto PAtual(itPontoUTMAtual->Observacoes, itPontoUTMAtual->x, itPontoUTMAtual->y, itLstCN->begin()->z, itPontoUTMAtual->Balizas);
          Ponto PRP(Mat.R_P(PAnterior, PAtual));

          //--- Coordenadas polares em fi (Azimute) e S (Distancia)

          pPonAnt->fi = PRP.x;
          pPonAnt->S = PRP.y;

          *pPonAnt >> CArcCurvasNivel;

          pPonAnt->x = PAtual.x;
          pPonAnt->y = PAtual.y;
          pPonAnt->z = itLstCN->begin()->z;
          pPonAnt->Descricao = PAtual.Descricao;
          pPonAnt->Baliza2 = PAtual.Baliza2;
        }

        *pPonAnt >> CArcCurvasNivel;
      }
    }
  }
  return true;
}

bool CDiaImportarArqCtrlDrones::GravaArquivoCNCoorxyz(lstlstPontos& pCNs, std::string& FormatoArqPontos, std::string& NomeProj)
{
  CFile Arq;
  CFileException e;

  std::string NomeArq(NomeProj + ".cnd");

  if (Arq.Open(NomeArq.c_str(), CFile::modeCreate | CFile::modeWrite, &e) == 0)
  {
    std::stringstream Msg;
    Msg << "Houve erro na abertura do arquivo : " << NomeArq << "." << std::endl;
    AfxMessageBox(Msg.str().c_str());

    return false;
  }
  else
  {
    ULONGLONG QtdCN(pCNs.size());
    CArchive CArcCurvasNivel(&Arq, CArchive::store);
    unsigned int QtdCurvasNivel(0);

    if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
    {
     // CSimpPL::SimplificaXYZ(pCNs);  //--- Dando pb VERIFICAR

      CArcCurvasNivel << 0.0 << (double)EquidisCN;                 //--- Angulo rotacao e equidistancia das CNs
      CArcCurvasNivel << pCNs.size();

      for (itlstlstPontos itCN = pCNs.begin(); itCN != pCNs.end(); ++itCN)
      {
        CArcCurvasNivel << itCN->size();

        for (itlstPontos itPontoCN = itCN->begin(); itPontoCN != itCN->end(); ++itPontoCN)
        {
          *itPontoCN >> CArcCurvasNivel;
        }
      }
    }
  }
  return true;
}


bool CDiaImportarArqCtrlDrones::GravaArquivoPontosSoltos(setPontos& SetPontosSoltos, std::string& FormatoArqPontos, std::string& NomeProj)
{
  CFile Arq;
  CFileException e;

  std::string NomeArq(NomeProj + ".pos");

  if (Arq.Open(NomeArq.c_str(), CFile::modeCreate | CFile::modeWrite, &e) == 0)
  {
    std::stringstream Msg;
    Msg << "Houve erro na abertura do arquivo : " << NomeArq << "." << std::endl;
    AfxMessageBox(Msg.str().c_str());

    return false;
  }
  else
  {
    CArchive CArcPontosSoltos(&Arq, CArchive::store);

    if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
    {
      CArcPontosSoltos << 0.0;                 //--- Angulo rotacao 
      CArcPontosSoltos << SetPontosSoltos.size();

      for (itsetPontos itPontoSolto = SetPontosSoltos.begin() ; itPontoSolto != SetPontosSoltos.end(); ++itPontoSolto)
      {
        *itPontoSolto >> CArcPontosSoltos;
      }
    }
  }
  return true;
}

void CDiaImportarArqCtrlDrones::OnEnSetfocusEdcaminhocn()
{
  std::string CopiaNomeArq(CEdCaminhoPCT);

  std::transform(CopiaNomeArq.begin(), CopiaNomeArq.end(), CopiaNomeArq.begin(), ::toupper);
}

void CDiaImportarArqCtrlDrones::OnBnKillfocusButpegararq()
{
  OnEnSetfocusEdcaminhocn();
}

#include "CLPontos.h"

void CDiaImportarArqCtrlDrones::DesembaracaCNs(tylstlstUtm_Coord_3d** ppLstLstCoorCN)
{
  //--- Verifica se a CN está cruzando ela mesma. Se estiver remove o cruzamento

  static tylstlstUtm_Coord_3d* pListaCNsOk((tylstlstUtm_Coord_3d*) new tylstlstUtm_Coord_3d);

  for (ittylstlstUtm_Coord_3d itLLCN = (*ppLstLstCoorCN)->begin(); itLLCN != (*ppLstLstCoorCN)->end(); itLLCN++)
  {
    if (itLLCN->size() < 4)
    {
      pListaCNsOk->emplace_back(tylstUtm_Coord_3d());
      pListaCNsOk->rbegin()->insert(pListaCNsOk->rbegin()->end(), itLLCN->begin(), itLLCN->end());

      continue;
    }

    ittylstUtm_Coord_3d itP1SegAVerificar(itLLCN->begin()), itP2SegAVerificar(itP1SegAVerificar);
    ++itP2SegAVerificar;
    register ittylstUtm_Coord_3d itP1SegAtual(itP2SegAVerificar);
    ++itP1SegAtual;
    register ittylstUtm_Coord_3d itP2SegAtual(itP1SegAtual);
    ++itP2SegAtual;

    pListaCNsOk->emplace_back(tylstUtm_Coord_3d());
    tylstUtm_Coord_3d& CNOk(*pListaCNsOk->rbegin());

    CNOk.insert(CNOk.end(),itP1SegAVerificar, itP1SegAtual);

    Ponto P1(itP1SegAVerificar->x, itP1SegAVerificar->y, itP1SegAVerificar->z),
          P2(itP2SegAVerificar->x, itP2SegAVerificar->y, itP2SegAVerificar->z);

    for (; itP2SegAtual != itLLCN->end(); ++itP1SegAtual,++itP2SegAtual)
    {
      Ponto P3(itP1SegAtual->x, itP1SegAtual->y, itP1SegAtual->z),
            P4(itP2SegAtual->x, itP2SegAtual->y, itP2SegAtual->z);

      Ponto Intersecao;

      if (CLPontos::CalculaIntersecao(P1,P2,P3,P4,Intersecao,true))
      {
        if (*itLLCN->begin() == *itLLCN->rbegin())   // CN Fechada?
        {
          if (*itP1SegAtual == *itP1SegAVerificar)   //--- Ponto de fechamento da CN
          {
            continue;      //--- A interseção é igual ao ponto inicial, desconsidera
          }
        }

         //--- Desfaz o laço

        CNOk.emplace_back(*itP1SegAVerificar);

        ittylstUtm_Coord_3d itTemp(itP1SegAtual);

        while (itTemp != itP2SegAVerificar)
        {
          CNOk.emplace_back(*itTemp);
          --itTemp;
        }

        CNOk.emplace_back(*itP2SegAVerificar);
        CNOk.emplace_back(*itP2SegAtual);

        itP1SegAtual = itP2SegAtual = itP2SegAVerificar;
      }
      else
      {
        CNOk.emplace_back(*itP1SegAtual);
      }
    }
  }

  *ppLstLstCoorCN = pListaCNsOk;
}

void  CDiaImportarArqCtrlDrones::LigaCNsPartidas(tylstlstUtm_Coord_3d& LstLstCNUTM)  //-- Nas arquivos DGN do IBGE as CNs vêem partidas
{
   //--- Pode haver no máximo 2 CN com o mesmo ponto de início ou de fim, porisso o multiset

   unsigned int Cont(0);

  msetplstitiniUtm_Coord_3d MSetItInicioCNs;
  msetplstitfimUtm_Coord_3d MSetItFimCNs;

  //--- Carrega os sets com os iteradores para inicio e o fim das listas de CNS

  int C(0);
  Progress.StepIt();

  for (ittylstlstUtm_Coord_3d it = LstLstCNUTM.begin() ; it != LstLstCNUTM.end() ; it++)
  {
    if(it->size() > 1)
    {
      MSetItInicioCNs.insert(plstitini(it,*it->begin()));
      MSetItFimCNs.insert(plstitfim(it, *it->rbegin()));
    }

    if (C++ == 10)
    {
      Progress.StepIt();
      C = 0;
    }
  }

  tylstlstUtm_Coord_3d LstLstCNsLigadas;   //--- lista de curvas resultante das uniões

  //--- Liga as curvas de nível que estiverem partidas

  bool Acabou(true);                         //--- So acaba quando não houver mais ligaçoes a fazer;
  ittylstlstUtm_Coord_3d itCN(LstLstCNUTM.begin());

  do
  {
    //--- Como não poderá haver outra combinação com o fim desta curva  
    //--- Remove do set de fins de curva o ponto para o fim da curva, para não achar ela mesma - assim não poderá haver mais pontos repetidos para esta curva

    MSetItFimCNs.erase(plstitfim(itCN, *itCN->rbegin()));

    bool Fim(false);

    //--- Nas CNs partidas existem 2 CNs com o mesmo inicio (ou fim) (só podem existir 2 CNs com o mesmo ponto de início ou fim)
    //--- A CN que esta sendo feita foi removida então so pode haver uma (ou nenhuma) CN no mesmo ponto

    itmsetplstitiniUtm_Coord_3d ItPontoIni;         //--- iterador para o set de inicio das CNs
    itmsetplstitfimUtm_Coord_3d ItPontoFim;         //--- iterador para o set de fim das CNs

    plstitini IniCNAtual(plstitini(itCN, *itCN->rbegin()));    //--- elemento do set para o fim da CN atual
                                                              //--- Vai procurar outre que possio o mesmo inicio
    //---Liga as CNS que o fim de uma CN que esteja ligado ao início de outra

    int C(0);

    do
    {
      if(C == 50)
      {
        Progress.StepIt();
        C = 0;
      }

      ItPontoIni = MSetItInicioCNs.find(IniCNAtual);  //--- Existe alguma CN cujo inicio esteja ligado ao fim da CN Atual? 

      if (ItPontoIni != MSetItInicioCNs.end() && ItPontoIni->Usada == 0)   //--- achou outra CN com inicio igual ao fim da atual, liga as duas e remove das CNs a lista que foi adicionada
      {
        Cont++;

        //--- Liga as 2 CNs
        
        if(*itCN->begin() != *ItPontoIni->itLista->begin())     //=== CN circular? fechou o circulo ?
        {
          itCN->insert(itCN->end(),ItPontoIni->itLista->begin(), ItPontoIni->itLista->end());
          const_cast<plstitini*>(&* ItPontoIni)->Usada = 1;
          LstLstCNUTM.erase(ItPontoIni->itLista);    //--- Apaga da lista antiga  a CN que foi juntada com a atual
        }
        else
        {
          //--- CN Circular, fecha o círculo

          itCN->push_back(*ItPontoIni->itLista->begin());
          const_cast<plstitini*>(&*ItPontoIni)->Usada = 1;
          ItPontoIni = MSetItInicioCNs.end();               //--- Para sair do do while (Acabou a CN) 
        }

        //--- Pode haver outra ligação no final desta CN que acabou de ser unida, prepara o iterador do set de inicios
        //--- com o novo fim da atual

        IniCNAtual = plstitini(itCN, *itCN->rbegin());    //--- ajusta o novo fim da CN e procura de novo
      }
    }while(ItPontoIni != MSetItInicioCNs.end() && !ItPontoIni->Usada);   //--- Para todas as CNs
    
    itCN++;      //-- Passa para a próxima CN

  } while(itCN != LstLstCNUTM.end());   
}


#include "CalcSU\CCCalcSU.h"
#include <set>
#include <list>
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include <map>
#include "CAcidentesTopograficos.h"
#include "cdeltasuperficie.h"
#include "CSegmento.h"
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "CEquipDrenagem.h"
#include "Superficie.h"

int CDiaImportarArqCtrlDrones::TriangulaPontosCotaDGN(tysetPontosSoltosCArestaDGN& SetArestasPontosCotadosDGN,tylstlstUtm_Coord_3d& LstLstCNUTM)
{
  CString CSNomeProjeto(NomeProjeto.c_str());

  CConfigProj ConfigProjeto(NULL, CSNomeProjeto);
  ConfigProjeto.LeArquivo();

  //--- Triangula os pontos cotados

  Progress.StepIt();

  int Erro(CCalcSU::CalculaSU(CSNomeProjeto, NULL, NULL,CCalcSU::TRIANG_DELAUNAY, ConfigProjeto.FormArqPontos.GetBuffer(),CCalcSU::PONTOSARQDGN));

  if(Erro == 0)    //--- 0 => Não houve erro
  {
    CSuperficie SupPontosCotasDGN(CSNomeProjeto);  //--- so pra pegar a triangulação

    Progress.StepIt();

    //--- Passa a triangulação para o set de triangulação liberando a superficie

    for (ItSSuperficie itSU = SupPontosCotasDGN.pSuperficieAtual->begin(); itSU != SupPontosCotasDGN.pSuperficieAtual->end(); itSU++)
    {
      for (ittysetArestas itSA = itSU->PontosLigados.begin() ; itSA != itSU->PontosLigados.end(); itSA++)
      {
        //--- Se a aresta ja foi inserida no sentido contrário não precisa inserir neste sentido

        if(SetArestasPontosCotadosDGN.find(CArestaDGN(itSA->PontoLigado,itSU->PCentral)) == SetArestasPontosCotadosDGN.end())
        {
          SetArestasPontosCotadosDGN.emplace(itSU->PCentral, itSA->PontoLigado);
        }
      }
    }
    
    rename(CSNomeProjeto + ".sup", CSNomeProjeto + ".sps");    //--- Esta triangulação é temporária

    return 0;   //--- 0 Não houve erro
  }

  return Erro;
}

int CDiaImportarArqCtrlDrones::CotaCNsDGN(tysetPontosSoltosCArestaDGN& pSetArestasPontosCotadosDGN, tylstlstUtm_Coord_3d& pLstLstCNUTM)
{
  Progress.SetRange(0,100);
  Progress.SetPos(0);
  Progress.SetStep(1);

  CStaObserv.SetWindowText("Calculando pontos");

  int Erro(TriangulaPontosCotaDGN(pSetArestasPontosCotadosDGN, pLstLstCNUTM));

  if (Erro == 0)     //--- 0 Não houve erro
  {
    CStaObserv.SetWindowText("Cotando CNs");

    Progress.SetPos(0);

    CotaCurvasNivelDGN(pSetArestasPontosCotadosDGN, pLstLstCNUTM);

    CotaCNsInversoras(pSetArestasPontosCotadosDGN, pLstLstCNUTM);
  }
  
  CStaObserv.SetWindowText("Houve erro ao cotar as CNs, verifique o arquivo de log");

  return false;  
}

int CDiaImportarArqCtrlDrones::CotaCurvasNivelDGN(tysetPontosSoltosCArestaDGN& ppSetArestasPontosCotadosDGN, tylstlstUtm_Coord_3d& LstLstCNUTM)
{
  /*
  calcula todas as interseções das CNs com as arestas da triangulação;

  Para todas as Arestas da triangulação
    Pega a lista de interseçoes desta aresta
    se o número de CNs for igual ao numero de interseções, cotar as CNs
    senão TentaCotar
  fim para

  TentaCotar                              //--- Neste ponto sabe-se que há CNs repetidas nesta aresta
    Verifica se tem alguma CN cotada
    se tiver
      Pega o ponto da interseção da aresta com esta CN 
      Verifica se o numero de CNs entre este ponto e o ponto inicial (e no Final) da aresta é o numero devido
        se for cota as CNS que cortam a aresta entre a CN e o ponto (inicial ou final da aresta)
      verifica a possibilidade de cotar as CNs com o ponta da aresta oposta a esta
        se puder cota as CNs e sai ok
        se não puder sai não OK ( Pede ajuda ao usuário)
    fim se tiver

  Fim TentaCotar
  */


  //--- Calcula todas as interseções das CNs com as arestas da triangulação

  int C(0);

  tylstCInterCNArestaDGN Intersecoes;

  for (ittysetPontosSoltosCArestaDGN itAresta = ppSetArestasPontosCotadosDGN.begin(); itAresta != ppSetArestasPontosCotadosDGN.end(); itAresta++)
  {
    Progress.StepIt();

    Intersecoes.clear();

    for(ittylstlstUtm_Coord_3d itLLCN = LstLstCNUTM.begin() ;  itLLCN != LstLstCNUTM.end() ; itLLCN++)
    {
      cittylstUtm_Coord_3d itPAnterior(itLLCN->begin()), itPAtual(itPAnterior);

      Ponto Intersecao;

      for (itPAtual++; itPAtual != itLLCN->end(); itPAtual++, itPAnterior++)
      {
        if (CLPontos::CalculaIntersecao(itAresta->P1(),itAresta->P2(), Ponto(itPAnterior->x, itPAnterior->y, itPAnterior->z), Ponto(itPAtual->x, itPAtual->y, itPAtual->z),Intersecao,true,1e-5) == TRUE)
        {
          Intersecoes.emplace_back(itLLCN,Intersecao);
        }
      }
    }

    //--- Calculou as interseções com esta aresta (mas estão fora da ordem de distancia), é preciso inserir as interseções das CNs na aresta, mas eles tem q ser inseridas na ordem crescente de distância
    //--- Primeiramente, calcula as distancias das interseções até o ponto anterior a elas na aresta para poder ordenar por distância

    //--- Calcula as distâncias

    for (ittylstCInterCNArestaDGN itInterAtual = Intersecoes.begin() ; itInterAtual != Intersecoes.end() ; itInterAtual++)
    {
      itInterAtual->Intersecao.S = itInterAtual->Intersecao.x;    //---  Os pontos são ordenados por x então salva o x e coloca a distancia em x
      itInterAtual->Intersecao.x = hypot(itInterAtual->Intersecao.x - itAresta->P1().x, itInterAtual->Intersecao.y - itAresta->P1().y);
    }

    //--- Insere as interseções nas arestas (set), ordenando por distancia (que esta em x do ponto))

    CArestaDGN* pArestaAtual(const_cast<CArestaDGN*>(&*itAresta));

    pArestaAtual->LimpaIntersecoes();

    for (cittylstCInterCNArestaDGN itInterAtual = Intersecoes.begin(); itInterAtual != Intersecoes.end(); itInterAtual++)
    {
     pArestaAtual->InsereIntersecao(itInterAtual->itCNIntersecao,itInterAtual->Intersecao);  //--- set interseções por ordem de distancia
    }
  }

  CFile ArqInter;
  CFileException e;
  std::string NomeArquivo("Intersecoes.xyz");

  //--- Apenas para debug
  if (false)
  {
    if (ArqInter.Open(NomeArquivo.c_str(), CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &e) == 0)
    {
      AfxMessageBox("Deu erro");
      return 1;
    }
    else
    {
      C = 0;

      CArchive ar(&ArqInter, CArchive::store);
      ar << ppSetArestasPontosCotadosDGN.size();
      for (ittysetPontosSoltosCArestaDGN itAresta = ppSetArestasPontosCotadosDGN.begin(); itAresta != ppSetArestasPontosCotadosDGN.end(); itAresta++)
      {
        if (C++ > 50)
        {
          Progress.StepIt();
          C = 0;
        }

        ar << itAresta->P1().x << itAresta->P1().y << itAresta->P1().z;
        ar << itAresta->P2().x << itAresta->P2().y << itAresta->P2().z;
        ar << itAresta->SetIntersecoes().size();
        for (ittysetCInterCNArestaDGN itInters = itAresta->SetIntersecoes().begin(); itInters != itAresta->SetIntersecoes().end(); itInters++)
        {
          ar << itInters->Intersecao.S << itInters->Intersecao.y;
        }
      }
      ar.Close();
      ArqInter.Close();
    }
  }

  AcertaCNsRepetidas(ppSetArestasPontosCotadosDGN,LstLstCNUTM);
  
  unsigned ContaCNCotadas(0);
  int CotouAlguma(false);
  int ContaCruzCN(0);
  ittysetPontosSoltosCArestaDGN itAresta;

  do
  {
    C = 0;

    ContaCNCotadas = 0;
    CotouAlguma = false;

    //--- Para todas as arestas da tringulação
    //--- Pega o set de interseçoes desta aresta 
    //--- se o número de CNs for igual ao numero devido, cota as CNs

    for (itAresta = ppSetArestasPontosCotadosDGN.begin(); itAresta != ppSetArestasPontosCotadosDGN.end(); itAresta++)
    {
      if (C++ > 100)
      {
        Progress.StepIt();
        C = 0;
      }

      //---Pega a lista de interseçoes desta aresta
      //--- se o número de CNs for igual ao numero devido, cota as CNs deste intervalo

      int QtdCNS(int(Mat.ModDouble(itAresta->P2().z, EquidisCN)) - int(Mat.ModDouble(itAresta->P1().z, EquidisCN)));

      if (fabs(QtdCNS) == itAresta->SetIntersecoes().size())    //--- numero exato cd CN entre os 2 pontos, cota direto
      {
        double Fator(EquidisCN);

        if (itAresta->P1().z > itAresta->P2().z) Fator *= -1.0;

        double CotaAtual(Fator > 0.0 ? int(itAresta->P1().z / Fator) * Fator + Fator : int(itAresta->P1().z / Fator) * Fator);

        //--- Cota as CNs

        for (ittysetCInterCNArestaDGN itCN = itAresta->SetIntersecoes().begin(); itCN != itAresta->SetIntersecoes().end(); itCN++)
        {
          if (itCN->itCNIntersecao->begin()->z == INFINITO)
          {
            itCN->itCNIntersecao->begin()->z = CotaAtual;
            itCN->itCNIntersecao->begin()->Balizas |= Ponto::CN_FOI_COTADA;
            ContaCNCotadas++;
            CotouAlguma = true;
          }
          CotaAtual += Fator;
        }
      }
      else
      {
        TentaCotar(LstLstCNUTM, itAresta, EquidisCN, CotouAlguma, ContaCNCotadas);
      }
    }
  } while (CotouAlguma == true);         //--- Se cotou alguma repassa, ela pode ajudar a cotar outras

  if (ContaCNCotadas < LstLstCNUTM.size())
  {
    monolog.mensagem(SEMMENSPADRAO,"Algumas curvas de nível não puderam ser cotadas. Para cotá-las pressione o botão <Cotar CN> na barra de tarefas.");
  }

  return true;
}

int CDiaImportarArqCtrlDrones::TentaCotar(const tylstlstUtm_Coord_3d& pLstLstCNUTM, ittysetPontosSoltosCArestaDGN pitAresta,unsigned int pEquidisCN,int& pCotouAlguma,unsigned int &pContaCNsCotadas)
{
  //--- Verifica se tem alguma CN Cotada entre os 2 pontos
  //--- Se tiver e não houver CNs voltando entre elas cota as CNs entre elas.

  unsigned int ContaCN(0);
  double CotaBaseAtual(Mat.ModDouble(pitAresta->P1().z,pEquidisCN) * pEquidisCN);

  for (ittysetCInterCNArestaDGN itInter = pitAresta->SetIntersecoes().begin(); itInter != pitAresta->SetIntersecoes().end(); itInter++,ContaCN++)
  {
    if (itInter->itCNIntersecao->begin()->z != INFINITO)    
    {
      //--- Verifica se entre a CN que está cotada e o início (P1) o numero de CNs esta exato, se estiver achou.   

      int QtdCNS(int(Mat.ModDouble(itInter->itCNIntersecao->begin()->z, pEquidisCN) - CotaBaseAtual / pEquidisCN));

      if (QtdCNS != 0)
      {
        if(ContaCN > 1)
        {
          if (fabs(QtdCNS) == ContaCN)               //--- O número de CNs entre essas 2 interseções esta exato?
          {
            double CotaAtual(CotaBaseAtual);
            int Fator(pEquidisCN);

            if(itInter->itCNIntersecao->begin()->z < CotaBaseAtual) Fator = -Fator;

            for (ittysetCInterCNArestaDGN itInterAtual = pitAresta->SetIntersecoes().begin() ; itInterAtual != itInter ; itInterAtual++)
            {
              if(itInterAtual->itCNIntersecao->begin()->z == INFINITO)    //--- A CN pode ja ter sido cotada
              {
                itInterAtual->itCNIntersecao->begin()->z = CotaAtual;
                itInterAtual->itCNIntersecao->begin()->Balizas |= Ponto::CN_FOI_COTADA;
                pCotouAlguma = true;
                pContaCNsCotadas++;
              }
              CotaAtual += Fator;
            }
          }
        }
      }
    }
  }
  return false;
}
int CDiaImportarArqCtrlDrones::TentaCotarInvertido(const tylstlstUtm_Coord_3d& pLstLstCNUTM, ittysetPontosSoltosCArestaDGN pitAresta, unsigned int pEquidisCN, int& pCotouAlguma, unsigned int& pContaCNsCotadas)
{
  unsigned int ContaCN(0);
  double CotaBaseAtual(Mat.ModDouble(pitAresta->P2().z, pEquidisCN) * pEquidisCN);

  for (rittysetCInterCNArestaDGN ritInter = pitAresta->SetIntersecoes().rbegin(); ritInter != pitAresta->SetIntersecoes().rend(); ritInter++, ContaCN++)
  {
    if (ritInter->itCNIntersecao->rbegin()->z != INFINITO)
    {
      int QtdCNS(int(Mat.ModDouble(ritInter->itCNIntersecao->rbegin()->z, pEquidisCN) - CotaBaseAtual / pEquidisCN));

      if (QtdCNS != 0)
      {
        if (ContaCN > 1)
        {
          if (fabs(QtdCNS) == ContaCN)               //--- O numero de CNsentre essas 2 interseções esta exato?
          {
            double CotaAtual(CotaBaseAtual);
            int Fator(pEquidisCN);
            if (ritInter->itCNIntersecao->rbegin()->z < CotaBaseAtual) Fator *= -1;

            for (rittysetCInterCNArestaDGN ritInterAtual = pitAresta->SetIntersecoes().rbegin(); ritInterAtual != ritInter; ritInterAtual++)
            {
              if (ritInterAtual->itCNIntersecao->rbegin()->z == INFINITO)    //--- A CN pode ja ter sido cotada
              {
                ritInterAtual->itCNIntersecao->rbegin()->z = CotaAtual;
                pCotouAlguma = true;
                pContaCNsCotadas++;
              }
              CotaAtual += Fator;
            }
          }
        }
      }
    }
  }
  return false;
}

void CDiaImportarArqCtrlDrones::AcertaCNsRepetidas(tysetPontosSoltosCArestaDGN& ppSetArestasPontosCotadosDGN, tylstlstUtm_Coord_3d& LstLstCNUTM)
{
  for (ittysetPontosSoltosCArestaDGN itAresta = ppSetArestasPontosCotadosDGN.begin(); itAresta != ppSetArestasPontosCotadosDGN.end(); itAresta++)
  {
    if(itAresta->SetIntersecoes().size() > 1)
    {
      int C(1);
      ittysetCInterCNArestaDGN itInterAnterior(itAresta->SetIntersecoes().begin()), itInterAtual(itAresta->SetIntersecoes().begin()),
                               itPrimeiraInter(itInterAtual);
      for (itInterAtual++ ; itInterAtual != itAresta->SetIntersecoes().end() ; itInterAtual++)
      {
        if ((*itInterAtual->itCNIntersecao->begin()) == (*itInterAnterior->itCNIntersecao->begin())) C++;
        else
        {
          if (C > 1)           //--- A CN anterior cruzou a aresta mais de uma vez?
          {
            ittysetCInterCNArestaDGN it(itInterAtual);
            it--;
            if (C % 2 == 0)    //--- Se cruzou número par de vezes ficam  2 interseções (CNs Inversoras)
            {
              const_cast<CInterCNArestaDGN*>(&*itInterAnterior)->CNInversora = true;
              const_cast<CArestaDGN*>(&*itAresta)->TemCNInversora = true;
             it--;
            }
            while (it != itPrimeiraInter)
            {
              const_cast<CArestaDGN*>(&*itAresta)->RemoveIntersecao(it--);
            }
            C = 1;
          }
          itPrimeiraInter = itInterAnterior = itInterAtual;
        }
      }
    }
  }
}

void CDiaImportarArqCtrlDrones::SimplificaCNs(tylstlstUtm_Coord_3d& pLstLstCNUTM)
{
  CSimpPL::Simplifica(pLstLstCNUTM);  //--- Dando pb VERIFICAR
}

void CDiaImportarArqCtrlDrones::TransferePontosParaCoord3d(lstlstPontos& pCNs, tylstlstUtm_Coord_3d& LstLstCoordGdc)
{
  LstLstCoordGdc.clear();
  int C(1);

  for (itlstlstPontos itCN = pCNs.begin(); itCN != pCNs.end(); itCN++)
  {
    LstLstCoordGdc.emplace_back(tylstUtm_Coord_3d());

    rittylstlstUtm_Coord_3d ritCN_Coord_3d(LstLstCoordGdc.rbegin());

    for (itlstPontos itPontoCN = itCN->begin(); itPontoCN != itCN->end(); itPontoCN++)
    {
      if(itPontoCN->AcidenteTopografico != CTextoMapaIBGE::PONTO_CN_MESTRA)        //--- Os pontos de CN sé servem para cotar as CNs
      {
        ritCN_Coord_3d->emplace_front(itPontoCN->x, itPontoCN->y, itPontoCN->z,0,false,C++,std::string(itPontoCN->Descricao.GetBuffer()), itPontoCN->Baliza2,0.0,0.0);
      }
    }
  }
}

void  CDiaImportarArqCtrlDrones::CotaCNsIBGE(tylstlstUtm_Coord_3d& LstLstCNUTM, setPontos& PontosCotados)
{
 //--- A lista de pontos cotados foi desenvolvida com o tipo Utm_Coord_3d aqui ela esta como uma lstPontos
 //--- É preciso converter de lstpontos para tylstUtm_Coord_3d

  tylstUtm_Coord_3d ListaPontosCotados_Coord_3d;
  int contaPC(0),contaCN(0);

  Progress.StepIt();

  for (itsetPontos it = PontosCotados.begin(); it != PontosCotados.end(); it++)
  {
    Ponto* pPonto(const_cast<Ponto*>(&*it));

    //--- As coordenadas das cotas mestras estão em i, fi e z e as dos pontos cotados em x,y e z

    if(it->AcidenteTopografico == 2)
    {
      contaCN++;
      ListaPontosCotados_Coord_3d.emplace_back(it->x, it->y, it->z, 0, false, atoi(pPonto->Nome), std::string(pPonto->Descricao.GetBuffer()), it->AcidenteTopografico, 0.0, 0.0);
    }
    else
    {
      ListaPontosCotados_Coord_3d.emplace_back(it->x, it->y, it->z, 0, false, atoi(pPonto->Nome),std::string(pPonto->Descricao.GetBuffer()), it->AcidenteTopografico, 0.0, 0.0);
    }
  }

  Progress.StepIt();

  //--- cota as CNs Rotuladas
  //---  Para achar as cns que estão rotuladas é preciso calcular os azimutes e as 
  //--- distancias entre os pontos das CNs
  //---

  tysetPontosSoltosCArestaDGN ArestasPontosCotadosDGN;

  CStaObserv.SetWindowText("Calculando Az e distancias das CNs");

  CalculaAZdistanciaCNs(LstLstCNUTM);

  CStaObserv.SetWindowText("Calculando CNs ");

  CotaCNsRotuladas(LstLstCNUTM, ListaPontosCotados_Coord_3d);

  CStaObserv.SetWindowText("Cotando CNs ");

  CotaCNsDGN(ArestasPontosCotadosDGN,LstLstCNUTM);
}

void CDiaImportarArqCtrlDrones::CalculaAZdistanciaCNs(tylstlstUtm_Coord_3d& LstLstCNUTM)
{
  //--- Calcula os azimutes e as distancias de todos os pontos das CNS
  //--- Aproveita e cria o set com as CNs 
  
  for(ittylstlstUtm_Coord_3d itCN = LstLstCNUTM.begin() ;  itCN != LstLstCNUTM.end() ; itCN++)
  {
    ittylstUtm_Coord_3d itPCNAnterior(itCN->begin()), itPCNAtual(itCN->begin());
 
    for (itPCNAtual++ ; itPCNAtual != itCN->end() ; itPCNAnterior++, itPCNAtual++)
    {
      SetCotasCoord_3d.emplace(ParitCNDGN(itCN->begin(), itPCNAtual));

      Ponto CoordPolar(Mat.R_P(Ponto(itPCNAnterior->x, itPCNAnterior->y),Ponto(itPCNAtual->x, itPCNAtual->y)));

      itPCNAnterior->az = CoordPolar.x;
      itPCNAnterior->dist = CoordPolar.y;
    }
  }
}

void CDiaImportarArqCtrlDrones::CDiaImportarArqCtrlDrones::CotaCNsRotuladas(tylstlstUtm_Coord_3d& LstLstCNUTM, tylstUtm_Coord_3d& PontosCotados)
{
  int C(0);

  for (ittylstUtm_Coord_3d itPonCotadoAtual = PontosCotados.begin(); itPonCotadoAtual != PontosCotados.end(); itPonCotadoAtual++)
  {
    if (itPonCotadoAtual->Balizas & CTextoMapaIBGE::TIPO::PONTO_CN_MESTRA)
    {
      int C(0);
      ittylstlstUtm_Coord_3d itCNAtual(LstLstCNUTM.begin());
      bool Achou(false);

      do
      {
        if (itCNAtual->begin()->z == INFINITO)  //--- Ainda não foi cotada?
        {
          if (VerificaCotaCN(itCNAtual, itPonCotadoAtual)) Achou = true;
          else itCNAtual++;

          if(C++ == 5000)
          {
            Progress.StepIt();
            C = 0;
          }
        }
        else 
        {
          itCNAtual++;
        }

      } while (!Achou && itCNAtual != LstLstCNUTM.end());

      if (Achou) C++;
    }
  }
}

bool CDiaImportarArqCtrlDrones::VerificaCotaCN(ittylstlstUtm_Coord_3d& LstLstCNUTM, ittylstUtm_Coord_3d& itPontoCotado)
{
  ittylstUtm_Coord_3d itPontoAnterior(LstLstCNUTM->begin()),itPontoAtual(itPontoAnterior);
  
  //--- Verifica se o pontos do rótulo está na reta,  entre os 2 pontos
  //--- de uma das retas da CN, se estiver este ponto é o rótulo desta CN

  itPontoAtual++;
  bool Achou(false);

  while(!Achou && itPontoAtual != LstLstCNUTM->end())
  {
    if(
       ((itPontoAtual->x > itPontoAnterior->x && itPontoCotado->x > itPontoAnterior->x && itPontoCotado->x < itPontoAtual->x) || (itPontoAtual->x < itPontoAnterior->x && itPontoCotado->x < itPontoAnterior->x && itPontoCotado->x > itPontoAtual->x)) &&
       ((itPontoAtual->y > itPontoAnterior->y && itPontoCotado->y > itPontoAnterior->y && itPontoCotado->y < itPontoAtual->y) || (itPontoAtual->y < itPontoAnterior->y && itPontoCotado->y < itPontoAnterior->y && itPontoCotado->y > itPontoAtual->y))
      ) 
    {
    //--- Calcula a distancia que o ponto está da reta se estiver muito perto esta é a curva

      Ponto CoordPol(Mat.R_P(Ponto(itPontoAnterior->x, itPontoAnterior->y),(Ponto(itPontoCotado->x, itPontoCotado->y))));

      double Angulo(CoordPol.x - itPontoAnterior->az);
      double Dist(fabs(sin(CoordPol.x - itPontoAnterior->az) * CoordPol.y));

      if(CoordPol.y < itPontoAnterior->dist && fabs(Dist) < 1.0)    //--- Achou!!
      {
        LstLstCNUTM->begin()->z = itPontoCotado->z;   //--- cota a CN
        Achou = true;
      }
    }
    itPontoAtual++;
    itPontoAnterior++;
  }

  return Achou;
}

#define DESCENDO 0
#define SUBINDO 1
#define INDETERMINADO 2

void CDiaImportarArqCtrlDrones::CotaCNsInversoras(tysetPontosSoltosCArestaDGN& ppSetArestasPontosCotadosDGN, tylstlstUtm_Coord_3d& LstLstCNUTM)
{
  for (ittysetPontosSoltosCArestaDGN itAresta = ppSetArestasPontosCotadosDGN.begin(); itAresta != ppSetArestasPontosCotadosDGN.end(); itAresta++)
  {
    double CotaAnterior(itAresta->P1().z);
    int Sentido(INDETERMINADO);
    bool PodeDeterminar(true), PegouInversora(false);

    if (itAresta->TemCNInversora)
    {
      for (ittysetCInterCNArestaDGN itInterAtual = itAresta->SetIntersecoes().begin(); itInterAtual != itAresta->SetIntersecoes().end() && PodeDeterminar; itInterAtual++)
      {
        if (!itInterAtual->CNInversora)
        {
          if (itInterAtual->itCNIntersecao->begin()->z != INFINITO)
          {
            if (Sentido == INDETERMINADO)       //--- Determina o sentido se ele estiver indeterminado
            {
              if (!itInterAtual->CNInversora)
              {
                if (itInterAtual->itCNIntersecao->begin()->z > CotaAnterior) Sentido = SUBINDO;
                else Sentido = DESCENDO;
              }
              else PodeDeterminar = false;       //--- Se a CN inversora ocorreu antes de se poder determinar a rampa, nada a fazer...
            }

            CotaAnterior = itInterAtual->itCNIntersecao->begin()->z;
          }
          else
          {
            if (Sentido == SUBINDO)  itInterAtual->itCNIntersecao->begin()->z = CotaAnterior + EquidisCN;
            else if (Sentido == DESCENDO) itInterAtual->itCNIntersecao->begin()->z = CotaAnterior - EquidisCN;

            CotaAnterior = itInterAtual->itCNIntersecao->begin()->z;
          }
        }
        else
        {
          if (Sentido == SUBINDO)  Sentido = DESCENDO;
          else if (Sentido == DESCENDO)  Sentido = SUBINDO;
        }
      }
    }
  }
}

void CDiaImportarArqCtrlDrones::AtualizaDialogo(std::string* pstrMsg, int Operacao)
{
  if (pstrMsg != nullptr)  CStaObserv.SetWindowText(pstrMsg->c_str());

  switch(Operacao)
  {
    case 1:  Progress.SetRange(0,100);
             Progress.SetStep(1);
             Progress.SetPos(0); break;
    case 2:  Progress.StepIt(); break;
    case 3:  Progress.SetPos(100); break;
    default: Progress.SetPos(Operacao);
  }
}

void CDiaImportarArqCtrlDrones::OnEnKillfocusEdiEquisCn()
{
  UpdateData(true);

  if (CEdCaminhoPCT.GetLength() > 5 && CEdCaminhoCN.GetLength() > 5 && EquidisCN > 0)
    GetDlgItem(IDOK)->EnableWindow(true);
}
void CDiaImportarArqCtrlDrones::OnClickedCheponctrlvirtuais()
{
  PonCTRLVirtuais = (((CButton*)GetDlgItem(IDC_CHEPONCTRLVIRTUAIS))->GetCheck() == TRUE);
  (((CButton*)GetDlgItem(IDC_CHEPONCTRLVIRTUAIS))->SetCheck(PonCTRLVirtuais));
}
