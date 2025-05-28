// CDImportarArqSIG.cpp : arquivo de implementação
//
#include "stdafx.h"
#include "Est.h"
#include <string>
#include "afxdialogex.h"
#include "CPonto.h"
#include "ConvUTMGDC\ConvUTMGDC\Gdc_Coord_3d.h"
#include "ConvUTMGDC\ConvUTMGDC\Utm_Coord_3d.h"
#include "CDImportarArqSIG.h"
#include "Monologo.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "Mat.h"
#include "CTriangulo.h"
#include "CTriangulacao.h"
#include "CDeltaSuperficie.h"
#include "CConfigProj.h"
#include "SimpPL\CSimpPL.h"

extern monologo monolog;
extern Matem Mat;

// caixa de diálogo de CDImportarArqSIG

IMPLEMENT_DYNAMIC(CDImportarArqSIG, CDialogEx)

CDImportarArqSIG::CDImportarArqSIG(std::string& pstrArqCN,std::string& pstrArqTalvegues,std::string& pstrArqMapa,std::string& pstrArqPCTRL,
                                   std::string& pNomeProjeto, double& pEquidisCN, void* pLstPontosGeoRef,CWnd* pParent)	: CDialogEx(IDD_DIAIMPARQSIG, pParent),
    strArqCN(pstrArqCN),
    strArqMapa(pstrArqMapa),
    strArqPCTRL(pstrArqPCTRL),
    CSArqCN(_T("")),
	CSArqMapa(_T("")),
    CSArqPontosControle(_T("")),
    NomeProjeto(pNomeProjeto),
    FormatoArqPontos(_T("")),
    pLstPontosCTRL(pLstPontosGeoRef),
    EquidisCN(pEquidisCN),
    ErroImportacao(0)
{
  CConfigProj ConfigProj(NULL, NomeProjeto.c_str());

  ConfigProj.LeArquivo();

  for (int C = 0; C < ConfigProj.FormArqPontos.GetLength(); C++) if (ConfigProj.FormArqPontos[C] != ' ') FormatoArqPontos.push_back(ConfigProj.FormArqPontos[C]);
}

CDImportarArqSIG::~CDImportarArqSIG()
{
}

void CDImportarArqSIG::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIARQMALHATRIANG, CSArqCN);
    DDX_Text(pDX, IDC_EDIARQMAPASIG, CSArqMapa);
    DDX_Text(pDX, IDC_EDIARQPONTOSCONTROLE, CSArqPontosControle);
    DDX_Text(pDX, IDC_EDIEQUIDISCN, EquidisCN);
}

BEGIN_MESSAGE_MAP(CDImportarArqSIG, CDialogEx)
	ON_BN_CLICKED(IDC_BUTPEGARARQCN, &CDImportarArqSIG::OnBnClickedButpegararqcn)
	ON_BN_CLICKED(IDC_BUTPEGARMAPASIG, &CDImportarArqSIG::OnBnClickedButpegarmapasig)
  ON_BN_CLICKED(IDC_BUTPEGARPCT, &CDImportarArqSIG::OnBnClickedButpegarpontoscontrole)
	ON_BN_CLICKED(IDOK, &CDImportarArqSIG::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDImportarArqSIG::OnBnClickedCancel)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDImportarArqSIG

void CDImportarArqSIG::OnBnClickedButpegararqcn()
{
	std::string Ext, filtro("Arquivo de Curvas de Nível (SIG) | *.dxf||");
	CString NomeArquivo;

	CFileDialog DiaAbreArq(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro.c_str(), NULL);
	DiaAbreArq.m_ofn.lpstrTitle = "VU&E- Importar arquivo de Acidentes Topográficos : ";

	if (DiaAbreArq.DoModal() == IDOK)
	{
		CSArqCN = DiaAbreArq.GetPathName();

		UpdateData(false);
	}
}

void CDImportarArqSIG::OnBnClickedButpegarmapasig()
{
	std::string Ext, filtro("Arquivo com o mapa | *.jpg||");
	CString NomeArquivo;

	CFileDialog DiaAbreArq(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro.c_str(), NULL);
	DiaAbreArq.m_ofn.lpstrTitle = "VU&E- Importar o Mapa : ";

	if (DiaAbreArq.DoModal() == IDOK)
	{
		CSArqMapa = DiaAbreArq.GetPathName();

		UpdateData(false);
	}
}

void CDImportarArqSIG::OnBnClickedButpegarpontoscontrole()
{
  std::string Ext, filtro("Arquivo com os Pontos de Controle | *.pct||");
  CString NomeArquivo;

  CFileDialog DiaAbreArq(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro.c_str(), NULL);
  DiaAbreArq.m_ofn.lpstrTitle = "VU&E- Importar os Pontos de Controle : ";

  if (DiaAbreArq.DoModal() == IDOK)
  {
    CSArqPontosControle = DiaAbreArq.GetPathName();

    UpdateData(false);
  }
}

bool CDImportarArqSIG::ImportaCNsSIG(const std::string& NomeArquivo, const std::string& strEntity)
{
  std::ifstream ArqDXFCNSIG(NomeArquivo);
  std::stringstream strstrMsg;

  if (!ArqDXFCNSIG.is_open() || !ArqDXFCNSIG.good())
  {
    monolog.mensagem(13, CString(NomeArquivo.c_str()) + " (lendo os dados)");
    return false;
  }

  unsigned int C(0);
  CurvasNivel.clear();
  std::string Entitiy;

  do
  {
    C++;
    std::getline(ArqDXFCNSIG, Entitiy);
    Limpa(Entitiy, "\t ");
  } while (Entitiy != "ENTITIES");

  std::string Label, Valor;
  int ContaCN(0);

  do
  {
    C++;

    std::getline(ArqDXFCNSIG, Entitiy);
    Limpa(Entitiy, "\t ");

    if (Entitiy == strEntity)
    {
      Ponto PAtual;
      double CotaAtual(INFINITO);

      if (CurvasNivel.size() > 1 && CurvasNivel.rbegin()->size() < 2)
      {
        CurvasNivel.pop_back();
      }

      CurvasNivel.emplace_back(LPontos());

      LPontos& CNAtual(*CurvasNivel.rbegin());

      do
      {
        C++;
        std::getline(ArqDXFCNSIG, Label);
        Limpa(Label, "\t ");

        if (Label == "0") break;

        std::getline(ArqDXFCNSIG, Valor);
        Limpa(Valor, "\t ");

        if (Label == "0") break;

        if (Label == "38") CotaAtual = atof(Valor.c_str());
        else if (Label == "10") PAtual.x = atof(Valor.c_str());
        else if (Label == "20")
        {
          if(CotaAtual == INFINITO)
          {
            ErroImportacao = CN_SEM_COTA;
            return false;
          }

          PAtual.y = atof(Valor.c_str());
          PAtual.z = CotaAtual;

          CNAtual.emplace_back(PAtual);
        }
      } while (true);    //--- Sai no break
    }
  } while (Entitiy != "ENDSEC");// && CurvasNivel.size() < 1000);

  CSimpPL::SimplificaXYZ(CurvasNivel);

  GravaArqCNs();

  return true;
}

void CDImportarArqSIG::OnBnClickedOk()
{
  tylstUtm_Coord_3d* pLstPonCTRL((tylstUtm_Coord_3d*)pLstPontosCTRL);
  
  strArqCN = CSArqCN.GetBuffer();
  strArqMapa = CSArqMapa.GetBuffer();
  strArqPCTRL = CSArqPontosControle.GetBuffer();

  bool Ok(true);

  if (strArqCN.size() < 3)
  {
    monolog.mensagem(SEMMENSPADRAO, "Os nome do arquivo de CNs deve ser preenchido");

    return;
  }

  if (Ok)
  {
    if (strArqMapa.size() < 3)
    {
      Ok = AfxMessageBox("O mapa nao esta sendo importado, confirma?", MB_YESNO | MB_ICONEXCLAMATION) == IDYES;
    }
  }

  if (Ok)
  {
    if(strArqPCTRL.size() < 3)
    {
      AfxMessageBox("O Arquivo com os pontos de controle deve ser informado.");
      Ok = false;
    }
  }

  if (Ok)
  {
    UpdateData(true);

    if (EquidisCN <= 0.0)
    {
      AfxMessageBox("Digita a equidistância entre as curvas de nível.");
      Ok = false;
    }
  }

  if(Ok)
  {
    CDialog::OnOK();
  }
}

void CDImportarArqSIG::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CDImportarArqSIG::GravaArqCNs()
{
  CFile Arq;
  CFileException e;

  std::string NomeArqSaidaCN(NomeProjeto);

  NomeArqSaidaCN += ".cun";

  if (Arq.Open(NomeArqSaidaCN.c_str(), CFile::modeCreate | CFile::modeWrite, &e) == TRUE)
  {
    CArchive CArcCurvasNivel(&Arq, CArchive::store);
    size_t QtdCurvasNivel(0);

    if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
    {
      CArcCurvasNivel << (double)0.0 << EquidisCN;
      CArcCurvasNivel << CurvasNivel.size();

      ItLLPontos itLLCN(CurvasNivel.begin());

      for (ItLLPontos itLLCN = CurvasNivel.begin(); itLLCN != CurvasNivel.end(); itLLCN++)
      {
        CArcCurvasNivel << itLLCN->size();

        itLLCN->begin()->fi = INFINITO;

        *itLLCN->begin() >> CArcCurvasNivel;

        ItLPontos ItPAnterior(itLLCN->begin()), ItPAtual(ItPAnterior);

        for (ItPAtual++; ItPAtual != itLLCN->end(); ItPAnterior++, ItPAtual++)
        {
          ItPAtual->fi = Mat.CalculaAzimute(*ItPAnterior, *ItPAtual);

          *ItPAtual >> CArcCurvasNivel;
        }
      }
    }
  }
}

bool CDImportarArqSIG::LeArqPCTRL(std::string& NomArq,void* ppLstPCT)
{
  std::string CopiaNomeArq(NomArq);

  std::transform(CopiaNomeArq.begin(), CopiaNomeArq.end(), CopiaNomeArq.begin(), ::toupper);

  if (CopiaNomeArq.find(".PCT") == std::string::npos)
  {
    return false;
  }

  std::ifstream ArqCTRL(NomArq, std::fstream::in);

  tylstUtm_Coord_3d* pLstPCT((tylstUtm_Coord_3d *) ppLstPCT);

  pLstPCT->clear();

  if (ArqCTRL.good())
  {
    Utm_Coord_3d pAtual;
    char strBuffer[1000] = { 0 };
    std::string Bufferstr;

    while (ArqCTRL.good() && pLstPCT->size() < 4)
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

      if (pAtual.x != INFINITO && pAtual.y != INFINITO && pAtual.z != INFINITO && pAtual.NumPonto != -1)
      {
        pLstPCT->emplace_back(pAtual);
      }
    }

    if (pLstPCT->size() != 4)
    {
      AfxMessageBox("Houve erro na leitura do arquivo  de pontos de controle (.pct)");

      return false;
    }
  }
  else
  {
    std::stringstream Msg;

    Msg << "Houve erro na abertura do arquivo : " << NomArq << "." << std::endl;

    AfxMessageBox(Msg.str().c_str());

    return false;
  }
  return true;
}


