// EstRel.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include <string>
#include "afxtempl.h"
#include "cponto.h"
#include "clpontos.h"
#include "estaca.h"
#include "perfil.h"
#include "supercstring.h"
#include "carqtalaterro.h"
#include "carqtalcorte.h"
#include "ctalaterro.h"
#include "ctalcorte.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include <list>
#include "csecao.h"
#include "clistaperfis.h"
#include "clistasecoes.h"
#include <string>
#include <set>
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "carqcurhorizontais.h"
#include <map>
#include "ccurhorizontal.h"
#include "Mat.h"
#include "ccurhorizontais.h"
#include <list>
#include "carqcurverticais.h" 
#include "ccurverticais.h"
#include "Relatorios.h"
#include <strstream>
#include <iomanip>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "monologo.h"
#include <strstream>
#include "CRegAreaTaludes.h"
#include "CLimpeza.h"
#include <sstream>  
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CAcidentesTopograficos.h"
#include "CDeltasuperficie.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "CEquipDrenagem.h"
#include "EstRel.h"
#include "dialogo.h"
#include "mat.h"
#include "CTipoSolos.h"
#include "CArqTiposSolos.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include "CArqFurosSondagem.h"
#include <algorithm>
#include "CPerfilGeologico.h"
#include <array>

extern class monologo monolog;
extern class dialogo dialogar;
extern class Matem Mat;

#define CURVA_ESPIRAL 0
#define VOLCORTE 1
#define LER 0
#define ESQUERDO 0
#define DIREITO 1

//--- Métodos de cálculo do tempo de concentração

#define TC_VEN_TE_CHOW 1
#define KIRPICH 2 
#define PICKING 3
#define KERBY_HATHAWAY 4
#define DNOS 5

//--- Métodos de cálculo da vazão

#define RACIONAL 1
#define RACIONAL_CORRIGIDO 2
#define I_PAI_WU 3
#define QC_VEN_TE_CHOW 4
#define MCMATH 5
#define KOKEI_UEHARA 6
#define HIDROG_TRIANG_SINTETICO 7

// CEstRel

IMPLEMENT_DYNCREATE(CEstRel, CDocument)

CEstRel::CEstRel() :pListaProjeto(NULL), pCurHorizontais(NULL), pCurvasVerticais(NULL), pListaSecoesTerreno(NULL),
                    IDRelatorio(((CMainFrame*)AfxGetMainWnd())->asdfg), pMapPontosTerrplProjeto(NULL),Formatar(true),
                    pBaciasHidrograficas(nullptr), pEquipDrenagem(nullptr)
{
  CMainFrame* pMainFrame((((CMainFrame*)AfxGetMainWnd())));

  switch (IDRelatorio)
  {
    case IDD_REL_VOLUMES_TERRAPLANAGEM:
    case IDD_REL_NOTA_SERVICO_PAVIM:
    case IDD_REL_NOTA_SERVICO_TERRAPLANAGEM: pListaProjeto = new CListaSecoes(CString(pMainFrame->PegaProjetoAtual())); break;
    case IDD_REL_PROJ_GEOMETRICO:
    {
      CSecTipo SecoesTipo(CString(pMainFrame->PegaProjetoAtual()));
      pCurHorizontais = new CCurHorizontais(CString(pMainFrame->PegaProjetoAtual()), SecoesTipo); break;
    }
    case IDD_REL_PROJ_GEOMETRICO_DETALHADO:
    {
      CString NomeProjeto(CString(pMainFrame->PegaProjetoAtual()));

      CFile ArqCalcSec;
      CFileException e;
      unsigned int QSecoes(0);
      Ponto PontoACalc;

      CFile ArqCalcGeo;

      if (ArqCalcGeo.Open(NomeProjeto + CString(".geo"), CFile::modeRead, &e) == 0)
      {
        monolog.mensagem(1, NomeProjeto + CString(".geo"));

        break;
      }

      CArchive CArqCalcGeo(&ArqCalcGeo, CArchive::load);

      Ponto CoordEstaca;
      CEstaca EstacaAtual;

      //--- Cria o mapa com as secoes ordenadas para acesso randomico

      for (CArqCalcGeo >> QSecoes; QSecoes; QSecoes--)
      {
        EstacaAtual << CArqCalcGeo;
        PontoACalc << CArqCalcGeo;

        MapaSecoesGeom.insert(MSecaoGeom::value_type(EstacaAtual, PontoACalc));
      }
    }
    break;
    case IDD_REL_PROJ_GREIDE: pCurvasVerticais = new CCurVerticais(CString(pMainFrame->PegaProjetoAtual())); break;
    case IDD_REL_TERRENO_PRIMITIVO: pListaSecoesTerreno = new CLPerfis(CString(pMainFrame->PegaProjetoAtual()) + ".tca"); break;
    case IDD_REL_SECOES_AVULSAS: pListaSecoesAvulsas = new CLPerfis(CString(pMainFrame->PegaProjetoAtual()) + ".sat"); break;
    case IDD_REL_AREA_TALUDES:
    {
      CString NomeProjeto(CString(pMainFrame->PegaProjetoAtual()));

      CFile ArqAreaTaludes;
      CFileException e;
      Ponto PontoACalc;

      if (ArqAreaTaludes.Open(NomeProjeto + CString(".tal"), CFile::modeRead, &e) == 0)
      {
        monolog.mensagem(1, NomeProjeto + CString(".tal"));
        break;
      }

      CArchive CArqAreaTaludes(&ArqAreaTaludes, CArchive::load);
      int QSecoes(0);
      CRegAreaTaludes RegAreaTaludes;

      //--- Cria o mapa com as secoes ordenadas para acesso randomico

      for (CArqAreaTaludes >> QSecoes; QSecoes; QSecoes--)
      {
        RegAreaTaludes.serialize(CArqAreaTaludes);
        ListaAreaTaludes.push_back(RegAreaTaludes);
      }
    }
    break;
    case IDD_REL_AREA_LIMPEZA:
    {
      CString NomeProjeto(CString(pMainFrame->PegaProjetoAtual()));

      CFile ArqAreaLimpeza;
      CFileException e;
      Ponto PontoACalc;

      if (ArqAreaLimpeza.Open(NomeProjeto + CString(".lic"), CFile::modeRead, &e) == 0)
      {
        monolog.mensagem(1, NomeProjeto + CString(".tal"));

        break;
      }

      CArchive CArqAreaLimpeza(&ArqAreaLimpeza, CArchive::load);
      int QSecoes(0);
      CLimpeza RegLimpeza;

      //--- Cria o mapa com as secoes ordenadas para acesso randomico

      for (CArqAreaLimpeza >> QSecoes; QSecoes; QSecoes--)
      {
        RegLimpeza.serialize(CArqAreaLimpeza);
        ListaLimpeza.push_back(RegLimpeza);
      }
    }
    break;
    case IDD_REL_PROJ_TERRAPLANAGEM:
    {
      pMapPontosTerrplProjeto = new tymapEstlstPontos;

      CFileException e;
      CFile ArqPonTerPro;

      CString NomeProjeto(CString(pMainFrame->PegaProjetoAtual()));

      if (ArqPonTerPro.Open(NomeProjeto + CString(".ptp"), CFile::modeRead, &e) == 0)   //--- Pontos Terraplanagem Projeto
      {
        monolog.mensagem(1, NomeProjeto + CString(".ptp"));

        break;
      }

      size_t QuanReg(0);

      CArchive CArqPonTerPro(&ArqPonTerPro, CArchive::load);
      ittymapEstlstPontos it;

      for (CArqPonTerPro >> QuanReg; QuanReg; QuanReg--)
      {
        pairtymapEstlstPontos PairRegAtual;

        PairRegAtual.first << CArqPonTerPro;

        pairitbooltymapEstlstPontos PairSecAtual(pMapPontosTerrplProjeto->emplace(PairRegAtual));

        size_t QuanPontos(0);

        for (CArqPonTerPro >> QuanPontos; QuanPontos; QuanPontos--)
        {
          Ponto PontoAtual;

          PontoAtual << CArqPonTerPro;
          PairSecAtual.first->second.emplace_back(PontoAtual);
        }
      }

      CFile ArqPonPassagem;

      std::string NomeCompArquivo(NomeProjeto + ".pas");

      if (ArqPonPassagem.Open(NomeCompArquivo.data(), CFile::modeRead, &e) == 0)
      {
        monolog.mensagem(13, CString(NomeCompArquivo.data()) + " (Lendo os dados)");

        throw e.m_cause;
      }
      else
      {
        CArchive ar(&ArqPonPassagem, CArchive::load);

        for (int LADO = ESQUERDO; LADO <= DIREITO; LADO++)
        {
          size_t Tam(0);

          for (ar >> Tam; Tam; Tam--)
          {
            Ponto PAtual;

            PAtual << ar;
            PontosPassagem[LADO].push_back(PAtual);
          }
        }
      }
    }
    break;
    case IDD_REL_LOG_CALCSUPER:
    {
      CString NomeProjeto(CString(pMainFrame->PegaProjetoBase()));

      tylststring lstLinhasLog;
      std::ifstream ifArqLog(NomeProjeto + "_CalcSuperficie.rel", std::fstream::in);

      do
      {
        std::string strLine;
        std::getline(ifArqLog, strLine);
        strstrArqLog << strLine << std::endl;
      } while (ifArqLog.good());
    }
    break;
    case IDD_REL_LOG_CALCTERRAP:
    {
      CString NomeProjeto(CString(pMainFrame->PegaProjetoAtual()));

      tylststring lstLinhasLog;
      std::ifstream ifArqLog(NomeProjeto + "_ProjTerraplanagem.rel", std::fstream::in);

      do
      {
        std::string strLine;
        std::getline(ifArqLog, strLine);
        strstrArqLog << strLine << std::endl;
      } while (ifArqLog.good());
    }
    break;
    case IDD_REL_LOG_FRONTEIRA:
    {
      CString NomeProjeto(CString(pMainFrame->PegaProjetoAtual()));

      tylststring lstLinhasLog;
      std::ifstream ifArqLog(NomeProjeto + "_DefFronteira.log", std::fstream::in);

      do
      {
        std::string strLine;
        std::getline(ifArqLog, strLine);
        strstrArqLog << strLine << std::endl;
      } while (ifArqLog.good());
    }
    case IDD_REL_LOG_SECCIONAR:
    {
      CString NomeProjeto(CString(pMainFrame->PegaProjetoBase()));

      tylststring lstLinhasLog;
      std::ifstream ifArqLog(NomeProjeto + "_Seccionar.log", std::fstream::in);

      do
      {
        std::string strLine;
        std::getline(ifArqLog, strLine);
        strstrArqLog << strLine << std::endl;
      } while (ifArqLog.good());
    }
    break;
    case IDD_REL_BACIAS_HIDROGRAF:
    {
      pBaciasHidrograficas = new tylstBacias;

      std::string NomeProjeto(CString(pMainFrame->PegaProjetoBase()));
      LeBaciasHidrograficas(NomeProjeto);
    }
    break;
    case IDD_REL_EQUIP_DRENAGEM:
    {
      pEquipDrenagem = new tymapEquipDrenagem;

      std::string NomeProjeto(CString(pMainFrame->PegaProjetoBase()));
      LeEquipDrenagem(NomeProjeto);
    }
    break;
  }
}

BOOL CEstRel::OnNewDocument()
{
  if (!CDocument::OnNewDocument()) return FALSE;
  return TRUE;
}

CEstRel::~CEstRel()
{
  switch (IDRelatorio)
  {
    case IDD_REL_VOLUMES_TERRAPLANAGEM:
    case IDD_REL_NOTA_SERVICO_PAVIM:
    case IDD_REL_NOTA_SERVICO_TERRAPLANAGEM: if (pListaProjeto) delete pListaProjeto; break;
    case IDD_REL_PROJ_GEOMETRICO: if (pCurHorizontais) delete pCurHorizontais; break;
    case IDD_REL_PROJ_GREIDE: if (pCurvasVerticais) delete pCurvasVerticais; break;
    case IDD_REL_TERRENO_PRIMITIVO: if (pListaSecoesTerreno) delete pListaSecoesTerreno; break;
    case IDD_REL_PROJ_TERRAPLANAGEM: if (pMapPontosTerrplProjeto) delete pMapPontosTerrplProjeto; break;
    case IDD_REL_SECOES_AVULSAS: if (pListaSecoesAvulsas) delete pListaSecoesAvulsas; break;
    case IDD_REL_BACIAS_HIDROGRAF: if (pBaciasHidrograficas) delete pBaciasHidrograficas; break;
    case IDD_REL_EQUIP_DRENAGEM:  if (pEquipDrenagem) delete pEquipDrenagem; break;
  }
}

BEGIN_MESSAGE_MAP(CEstRel, CDocument)
END_MESSAGE_MAP()

// CEstRel diagnostics

#ifdef _DEBUG
void CEstRel::AssertValid() const
{
  CDocument::AssertValid();
}

void CEstRel::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

// CEstRel serialization

void CEstRel::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    // TODO: add storing code here
  }
  else
  {
    // TODO: add loading code here
  }
}

// CEstRel commands

BOOL CEstRel::OnOpenDocument(LPCTSTR lpszPathName)
{
  // if (!CDocument::OnOpenDocument(lpszPathName))
  return TRUE;

  // TODO:  Add your specialized creation code here

  // return TRUE;
}

BOOL CEstRel::OnSaveDocument(LPCTSTR lpszPathName)
{
  if (IDRelatorio != IDD_REL_LOG_CALCSUPER && IDRelatorio != IDD_REL_LOG_FRONTEIRA && IDRelatorio != IDD_REL_LOG_CALCTERRAP && IDRelatorio != IDD_REL_LOG_SECCIONAR)
  {
    CFileException e;
    CFile ArquivoRel;

    if (ArquivoRel.Open(lpszPathName, CFile::modeWrite, &e) != 0)
    {
      if (dialogar.DoModal(8) != IDOK) return false;
      else ArquivoRel.Close();
    }

    if (ArquivoRel.Open(lpszPathName, CFile::modeWrite | CFile::modeCreate, &e) == 0)
    {
      if (e.m_cause == CFileException::sharingViolation)
        monolog.mensagem(1, lpszPathName);
      return false;
    }
    else
    {
      std::strstream Relatorio;
      GeraRelatorio(Relatorio, IDRelatorio, true);

      ArquivoRel.Write(Relatorio.str(), Relatorio.pcount());
    }
  }
  return true;
}

void CEstRel::GeraRelatorio(std::strstream& steBuffer, int IDRelatorio, bool TerminarLinha)
{
  SYSTEMTIME lt;
  GetLocalTime(&lt);

  CMainFrame* MF((CMainFrame*)AfxGetMainWnd());

  int CasasDecimais(((CMainFrame*)AfxGetMainWnd())->CasasDecimaisRelatorios);

  //Formatar = CasasDecimais > -1;

  Formatar = false;

  //--- fixed Não funciona VERIFICAR

  steBuffer << std::right << std::fixed;
  steBuffer << std::setw(2) << std::setfill('0') << "Emitido em " << lt.wDay << "/" << lt.wMonth << "/";
  steBuffer << std::setw(4) << lt.wYear << " ";
  steBuffer << std::setw(2) << lt.wHour << ":" << lt.wMinute << ":" << lt.wSecond;

  if (TerminarLinha) steBuffer << "\r" << std::endl;
  else steBuffer << std::endl;

  //--- Gera o relatório txt, colocando tudo num buffer, que será usado  para gravar um arquivo ou para 
  //--- mostrar o relatorio na tela

  CString NomeProjeto;

  if (IDRelatorio != IDD_REL_LOG_CALCSUPER && IDRelatorio != IDD_REL_LOG_FRONTEIRA || IDRelatorio != IDD_REL_LOG_CALCTERRAP || IDRelatorio != IDD_REL_LOG_SECCIONAR)
  {
    steBuffer << "Vias Urbanas & Estradas Versão 5.30 - 2022 \r\nProjeto: " << (CString((((CMainFrame*)AfxGetMainWnd()))->PegaProjetoAtual()));

    if (TerminarLinha) steBuffer << "\r" << std::endl;
    else steBuffer << std::endl;
  }

  switch (IDRelatorio)
  {
    case IDD_REL_NOTA_SERVICO_TERRAPLANAGEM:
    {
      steBuffer << std::right << std::fixed << std::setfill(' ');

      steBuffer << "RELATÓRIO DA NOTA DE SERVIÇO DE TERRAPLANAGEM: \r\n";

      for (pListaProjeto->PosicionaPrimeira(); pListaProjeto->PegaAtual() != NULL; (*(pListaProjeto))++)
      {
        CSecao& SecaoAtual(pListaProjeto->GetAt(pListaProjeto->PegaAtual()));

        if (SecaoAtual.Projeto.GetCount() > 1)
        {
          CSecao& SecaoAtual = pListaProjeto->GetAt(pListaProjeto->PegaAtual());

          SecaoAtual.Estaca.Descricao.Append("   ", 3 - SecaoAtual.Estaca.Descricao.GetLength());

          steBuffer << SecaoAtual.Estaca.GetBuffer() << ' ' << SecaoAtual.Estaca.Descricao;

          Ponto PProjAnterior(SecaoAtual.Projeto.GetHead());

          for (POSITION PProjAtual = SecaoAtual.Projeto.GetHeadPosition(); PProjAtual != NULL;)
          {
            Ponto PProjetoAtual(SecaoAtual.Projeto.GetNext(PProjAtual));

            PProjetoAtual.Descricao.Append("         ", 8 - SecaoAtual.Estaca.Descricao.GetLength());

            std::streamsize Precisao(Formatar ? CasasDecimais : 8);

            steBuffer << ' ' << PProjetoAtual.Descricao.GetBuffer()
              << ' ' << std::setw(Formatar ? 8 : 15) << std::setprecision(Formatar ? 2 : 8) << PProjetoAtual.x
              << ' ' << std::setw(Formatar ? 9 : 15) << std::setprecision(Formatar ? CasasDecimais : 8) << PProjetoAtual.y;
            if (PProjetoAtual != SecaoAtual.Projeto.GetHead())
              steBuffer << std::setw(Formatar ? 9 : 15) << std::setprecision(Formatar ? 2 : 8)
              << (((PProjetoAtual.y - PProjAnterior.y) * 100.0) / (PProjetoAtual.x - PProjAnterior.x)) << '%';

            PProjAnterior = PProjetoAtual;
          }

          if (TerminarLinha) steBuffer << "\r" << std::endl;
          else steBuffer << std::endl;
        }
      }
    }
    break;
    case IDD_REL_PROJ_GEOMETRICO:
    {
      int NumCurva(0);

      steBuffer << "RELATÓRIO DO PROJETO GEOMÉTRICO:\r\n";

      MCurHorizontaisTS MapCurvasHorizontaisTS = pCurHorizontais->PegaMapCurHorizontaisTS();

      for (ItCMCurHorizontaisTS itMCHTS = MapCurvasHorizontaisTS.begin(); itMCHTS != MapCurvasHorizontaisTS.end(); itMCHTS++)
      {
        steBuffer << std::right << std::fixed << std::setfill(' ') << std::setprecision(Formatar ? 3 : 8);

        steBuffer << " PI= " << std::setw(3) << ++NumCurva;

        if (itMCHTS->second.Tipo == CURVA_ESPIRAL)  steBuffer << "  I= ";
        else steBuffer << " Ac= ";
        steBuffer << Mat.strg_gms(itMCHTS->second.Ac * Matem::PI_GRAUS).c_str()
          << " Raio= " << std::setw(8) << itMCHTS->second.Raio;

        if (itMCHTS->second.Raio > 1.0)
        {
          if (itMCHTS->second.Lc > 1.0)
            steBuffer << " Lc= " << itMCHTS->second.Lc
            << " xc= " << itMCHTS->second.xEsp
            << " yc= " << itMCHTS->second.yEsp
            << " Ts= " << itMCHTS->second.Ts
            << " Sl= " << itMCHTS->second.Sl
            << " Se= " << itMCHTS->second.Se
            << " Ac= " << Mat.strg_gms(itMCHTS->second.AcCirc * Matem::PI_GRAUS).c_str()
            << std::setw(12)
            << " Coor N= " << itMCHTS->second.PI.y
            << " Coor E= " << itMCHTS->second.PI.x
            << " TS= " << itMCHTS->second.EstTs
            << " Coor N= " << itMCHTS->second.TS.y
            << " Coor E= " << itMCHTS->second.TS.x
            << " Az= " << Mat.strg_gms(itMCHTS->second.TS.fi * Matem::PI_GRAUS).c_str()
            << " SC= " << itMCHTS->second.EstSc
            << " Coor N= " << itMCHTS->second.SC.y
            << " Coor E= " << itMCHTS->second.SC.x
            << " Az= " << Mat.strg_gms(itMCHTS->second.SC.fi * Matem::PI_GRAUS).c_str()
            << " CS= " << itMCHTS->second.EstCs
            << " Coor N= " << itMCHTS->second.CS.y
            << " Coor E= " << itMCHTS->second.CS.x
            << " Az= " << Mat.strg_gms(itMCHTS->second.CS.fi * Matem::PI_GRAUS).c_str()
            << " ST= " << itMCHTS->second.EstSt
            << " Coor N= " << itMCHTS->second.ST.y
            << " Coor E= " << itMCHTS->second.ST.x
            << " Az= " << Mat.strg_gms(itMCHTS->second.ST.fi * Matem::PI_GRAUS).c_str();
          else
            steBuffer << "  T= " << std::setw(8) << itMCHTS->second.Ts
            << std::setw(12)
            << " Coor N= " << itMCHTS->second.PI.y
            << " Coor E= " << itMCHTS->second.PI.x
            << " PC= " << itMCHTS->second.EstTs
            << " Coor N= " << itMCHTS->second.TS.y
            << " Coor E= " << itMCHTS->second.TS.x
            << " Az= " << Mat.strg_gms(itMCHTS->second.TS.fi * Matem::PI_GRAUS).c_str()
            << " PT= " << itMCHTS->second.EstSt
            << " Coor N= " << itMCHTS->second.ST.y
            << " Coor E= " << itMCHTS->second.ST.x
            << " Az= " << Mat.strg_gms(itMCHTS->second.ST.fi * Matem::PI_GRAUS).c_str();
        }
        else    //--- raio = 0
        {
          steBuffer << std::setw(12)
            << " Coor N= " << itMCHTS->second.PI.y
            << " Coor E= " << itMCHTS->second.PI.x;
          if (NumCurva > 1)
            steBuffer << " Az1= " << Mat.strg_gms(itMCHTS->second.TS.fi * Matem::PI_GRAUS).c_str();
          steBuffer << " Az2= " << Mat.strg_gms(itMCHTS->second.ST.fi * Matem::PI_GRAUS).c_str()
            << " EstPI= " << itMCHTS->second.EstSt;
        }

        if (TerminarLinha) steBuffer << "\r" << std::endl;
        steBuffer << std::endl;
      }
    }
    break;
    case IDD_REL_PROJ_GEOMETRICO_DETALHADO:
    {
      steBuffer << std::right << std::fixed << std::setfill(' ') << std::setprecision(Formatar ? 3 : 8);

      steBuffer << "RELATÓRIO DO PROJETO GEOMÉTRICO DETALHADO:\r\n";

      for (itMSecaoGeom itMSecoesGeom = MapaSecoesGeom.begin(); itMSecoesGeom != MapaSecoesGeom.end(); itMSecoesGeom++)
      {
        CEstaca Estaca((*itMSecoesGeom).first);

        Estaca.Descricao.Append("   ", 3 - Estaca.Descricao.GetLength());

        steBuffer << Estaca.Descricao.GetBuffer()
          << Estaca.GetBuffer()
          << std::setw(12)
          << " Coor N= " << (*itMSecoesGeom).second.y
          << " Coor E= " << (*itMSecoesGeom).second.x
          << " Az= " << Mat.strg_gms((*itMSecoesGeom).second.fi * Matem::PI_GRAUS);

        if (TerminarLinha) steBuffer << "\r" << std::endl;
        else steBuffer << std::endl;
      }
    }
    break;
    case IDD_REL_PROJ_GREIDE:
    {
      int NLinhas = 0;

      steBuffer << "RELATÓRIO DO PROJETO DO GREIDE:\r\n";

      for (POSITION posCurAtual = pCurvasVerticais->LCurvasVerticais.GetHeadPosition(); posCurAtual != NULL; NLinhas++)
      {
        CurVertical& CurvaVerticalAtual(pCurvasVerticais->LCurvasVerticais.GetNext(posCurAtual));

        steBuffer << std::right << std::fixed << std::setfill(' ') << std::setprecision(Formatar ? 3 : 8);

        steBuffer << "  " << NLinhas + 1
          << " PIV= " << CurvaVerticalAtual.PIV.GetBuffer()
          << " COTA= " << CurvaVerticalAtual.PIV.Cota
          << " PCV= " << CurvaVerticalAtual.PCV.GetBuffer()
          << " COTA= " << CurvaVerticalAtual.PCV.Cota
          << " PTV= " << CurvaVerticalAtual.PTV.GetBuffer()
          << " COTA= " << CurvaVerticalAtual.PTV.Cota;

        //--- No primeiro PIV i1 == infinito, no último i2 == infinito e nos pivs sem parábolas E == infinito  

        steBuffer << std::setprecision(Formatar ? 6 : 8);

        if (Formatar) steBuffer << std::setw(8) << std::setprecision(3);

        if (CurvaVerticalAtual.I1 != INFINITO) steBuffer << " I1= " << std::setw(8) << CurvaVerticalAtual.I1 * 100.0 << '%';
        if (CurvaVerticalAtual.I2 != INFINITO) steBuffer << " I2= " << std::setw(8) << CurvaVerticalAtual.I2 * 100.0 << '%';

        steBuffer << std::setprecision(Formatar ? 2 : 8);

        steBuffer << " Y1= " << std::setw(8) << CurvaVerticalAtual.Y1
          << " Y2= " << std::setw(8) << CurvaVerticalAtual.Y2;

        steBuffer << std::setprecision(3);

        steBuffer << " E= " << std::setw(8) << (CurvaVerticalAtual.E == INFINITO ? 0.0 : CurvaVerticalAtual.E);

        if (TerminarLinha) steBuffer << "\r" << std::endl;
        else steBuffer << std::endl;
      }
    }
    break;
    case IDD_REL_NOTA_SERVICO_PAVIM:
    {
      steBuffer << "RELATÓRIO DA NOTA DE SERVIÇO DE PAVIMENTAÇÃO:\r\n";

      for (pListaProjeto->PosicionaPrimeira(); pListaProjeto->PegaAtual() != NULL; (*(pListaProjeto))++)
      {
        CSecao& SecaoAtual(pListaProjeto->GetAt(pListaProjeto->PegaAtual()));

        if (SecaoAtual.Projeto.GetCount() > 1)
        {
          steBuffer << std::right << std::fixed << std::setfill(' ') << std::setprecision(Formatar ? 2 : 8);
          SecaoAtual.Estaca.Descricao.Append(" ", 3 - SecaoAtual.Estaca.Descricao.GetLength());
          steBuffer << SecaoAtual.Estaca.GetBuffer() << ' ' << SecaoAtual.Estaca.Descricao;

          Ponto PProjAnterior(SecaoAtual.Pavimento.GetHead());

          for (POSITION PProjAtual = SecaoAtual.Pavimento.GetHeadPosition(); PProjAtual != NULL;)
          {
            Ponto PProjetoAtual = SecaoAtual.Projeto.GetNext(PProjAtual);

            steBuffer << ' ' << PProjetoAtual.Descricao.GetBuffer()
              << std::setprecision(Formatar ? 2 : 8)
              << ' ' << PProjetoAtual.x
              << ' ' << std::setprecision(Formatar ? 3 : 8) << PProjetoAtual.y;

            if (PProjetoAtual != SecaoAtual.Projeto.GetHead())
            {
              if (fabs((PProjetoAtual.x - PProjAnterior.x)) > 0.0001)
                steBuffer << ' ' << ((PProjetoAtual.y - PProjAnterior.y) * 100.0) / (PProjetoAtual.x - PProjAnterior.x) << '%';
            }

            PProjAnterior = PProjetoAtual;
          }
          if (TerminarLinha) steBuffer << "\r" << std::endl;
          else steBuffer << std::endl;
        }
      }
    }
    break;
    case IDD_REL_TERRENO_PRIMITIVO:
    {
      steBuffer << "RELATÓRIO DO TERRENO PRIMITIVO:\r\n";

      for (POSITION posLSecoesTerreno = pListaSecoesTerreno->GetHeadPosition(); posLSecoesTerreno != NULL; pListaSecoesTerreno->GetNext(posLSecoesTerreno))
      {
        Perfil& PerfilAtual(pListaSecoesTerreno->GetAt(posLSecoesTerreno));

        steBuffer << std::right << std::fixed << std::setfill(' ');

        PerfilAtual.Estaca.Descricao.Append("    ", 3 - PerfilAtual.Estaca.Descricao.GetLength());

        steBuffer << PerfilAtual.Estaca.GetBuffer() << ' ' << PerfilAtual.Estaca.Descricao.GetBuffer();

        for (POSITION posLPontosAtual = PerfilAtual.GetHeadPosition(); posLPontosAtual != NULL; PerfilAtual.GetNext(posLPontosAtual))
        {
          Ponto& PontoAtual = PerfilAtual.GetAt(posLPontosAtual);

          steBuffer << ' ' << std::setw(8) << std::setprecision(Formatar ? 2 : 8) << PontoAtual.x
            << ' ' << std::setw(9) << std::setprecision(Formatar ? 3 : 8) << PontoAtual.y;
        }
        if (TerminarLinha) steBuffer << "\r" << std::endl;
        else steBuffer << std::endl;
      }
    }
    break;
    case IDD_REL_VOLUMES_TERRAPLANAGEM:
    {
      double Buck(0.0);

      steBuffer << std::right << std::fixed << std::setfill(' ') << std::setprecision(Formatar ? 2 : 8);

      steBuffer << "RELATÓRIO DOS VOLUMES DE TERRAPLANAGEM:\r\n";

      for (pListaProjeto->PosicionaPrimeira(); pListaProjeto->PegaAtual() != NULL; (*(pListaProjeto))++)
      {
        CSecao& SecaoAtual(pListaProjeto->GetAt(pListaProjeto->PegaAtual()));

        Buck -= SecaoAtual.Volume[CSecao::CORTE];
        Buck -= SecaoAtual.Volume[CSecao::ATERRO] * (1.0 - (SecaoAtual.Empolamento / 100.0));

        steBuffer << std::right << std::fixed << std::setfill(' ') << std::setw(Formatar ? 9 : 11);

        SecaoAtual.Estaca.Descricao.Append("    ", 3 - SecaoAtual.Estaca.Descricao.GetLength());

        steBuffer << SecaoAtual.Estaca.GetBuffer() << ' '
          << SecaoAtual.Estaca.Descricao.GetBuffer()
          << " ArCt= " << std::setprecision(Formatar ? 2 : 8) << std::setw(Formatar ? 7 : 15) << SecaoAtual.AreaTotal[CSecao::CORTE]
          << " ArAt= " << std::setw(Formatar ? 7 : 15) << SecaoAtual.AreaTotal[CSecao::ATERRO]
          << " VolCt= " << std::setprecision(Formatar ? 0 : 8) << std::setw(Formatar ? 7 : 15) << SecaoAtual.Volume[CSecao::CORTE]
          << " VolAt= " << std::setw(Formatar ? 7 : 15) << SecaoAtual.Volume[CSecao::ATERRO]
          << " VolAtEmpo= " << std::setw(Formatar ? 7 : 15) << SecaoAtual.Volume[CSecao::EMPOLADOATERRO]
          << " VolMassCt= " << std::setw(Formatar ? 7 : 15) << SecaoAtual.Volume[CSecao::MASSAATUALCORTE]
          << " VolMassAt= " << std::setw(Formatar ? 7 : 15) << SecaoAtual.Volume[CSecao::MASSAATUALATERRO]
          << " VolMassAtEmpo= " << std::setw(Formatar ? 7 : 15) << SecaoAtual.Volume[CSecao::MASSAEMPOLATERRO]
          << " VolTotCt= " << std::setw(Formatar ? 7 : 15) << SecaoAtual.Volume[CSecao::ACUMULADOCORTE]
          << " VolTotAt= " << std::setw(Formatar ? 7 : 15) << SecaoAtual.Volume[CSecao::ACUMULADOATERRO]
          << " VolTotAtEmpo= " << std::setw(Formatar ? 7 : 15) << SecaoAtual.Volume[CSecao::TOTALEMPOLATERRO]
          << " CGCt= "; //<< std::setw(Formatar ? 7 : 15) << std::setprecision(Formatar ? 2 : 8);

        if (SecaoAtual.CGs[CSecao::CORTE] != INFINITO) steBuffer << CEstaca(SecaoAtual.CGs[CSecao::CORTE]).GetBuffer();
        else steBuffer << (Formatar ? "           " : " ");

        steBuffer << " CGAt= ";

        if (SecaoAtual.CGs[CSecao::ATERRO] != INFINITO) steBuffer << CEstaca(SecaoAtual.CGs[CSecao::ATERRO]).GetBuffer();
        else steBuffer << (Formatar ? "          " : " ");

        steBuffer << " CompLat= " << std::setprecision(Formatar ? 0 : 8) << std::setw(Formatar ? 8 : 15) << SecaoAtual.CompLateral
          << " Buck= " << std::setprecision(Formatar ? 0 : 8) << std::setw(Formatar ? 8 : 15) << Buck;

        if (TerminarLinha) steBuffer << "\r" << std::endl;
        else steBuffer << std::endl;
      }
    }
    break;
    case IDD_REL_SECOES_AVULSAS:
    {
      steBuffer << "RELATÓRIO DAS SECÕES AVULSAS:\r\n";

      for (POSITION posLSecoesAvulsas = pListaSecoesAvulsas->GetHeadPosition(); posLSecoesAvulsas != NULL; pListaSecoesAvulsas->GetNext(posLSecoesAvulsas))
      {
        Perfil& PerfilAtual = pListaSecoesAvulsas->GetAt(posLSecoesAvulsas);

        steBuffer << std::right << std::fixed << std::setfill(' ') << std::setprecision(Formatar ? 2 : 8);

        steBuffer << PerfilAtual.Estaca.GetBuffer(1) << std::string("=");

        for (POSITION posLPontosAtual = PerfilAtual.GetHeadPosition(); posLPontosAtual != NULL; PerfilAtual.GetNext(posLPontosAtual))
        {
          Ponto& PontoAtual = PerfilAtual.GetAt(posLPontosAtual);

          steBuffer << ' ' << std::setprecision(Formatar ? 2 : 8) << PontoAtual.x << ' ' << std::setprecision(Formatar ? 3 : 8) << PontoAtual.y;
        }
        if (TerminarLinha) steBuffer << "\r" << std::endl;
        else steBuffer << std::endl;
      }
    }
    break;
    case IDD_REL_AREA_TALUDES:
    {
      steBuffer << "RELATÓRIO DA ÁREA DOS TALUDES:\r\n";

      for (ittlstCRegAreaTaludes itLAreaTaludes = ListaAreaTaludes.begin(); itLAreaTaludes != ListaAreaTaludes.end(); itLAreaTaludes++)
      {
        steBuffer << std::right << std::fixed << std::setfill(' ') << std::setprecision(Formatar ? 3 : 8)
          << " Estaca= " << ((*itLAreaTaludes).Estaca)
          << " SemiDist= " << std::setprecision(Formatar ? 2 : 8) << std::setw(Formatar ? 7 : 15) << ((*itLAreaTaludes).SemiDistancia)
          << " AreaAtual= " << ((*itLAreaTaludes).AreaAtual)
          << " AreaTotal= " << ((*itLAreaTaludes).AreaTotal);

        if (TerminarLinha) steBuffer << "\r" << std::endl;
        else steBuffer << std::endl;
      }
    }
    break;
    case IDD_REL_AREA_LIMPEZA:
    {
      steBuffer << "RELATÓRIO DA ÁREA E VOLUMES DE LIMPEZA:\r\n";

      for (ittlstCLimpeza itLLimpeza = ListaLimpeza.begin(); itLLimpeza != ListaLimpeza.end(); itLLimpeza++)
      {
        steBuffer << std::right << std::fixed << std::setfill(' ');

        steBuffer << " Estaca= " << CEstaca(itLLimpeza->Estaca)
          << " Altura= "
          << std::setw(Formatar ? 8 : 15) << std::setprecision(Formatar ? 2 : 8)
          << itLLimpeza->h
          << " SemiDist= "
          << std::setw(Formatar ? 8 : 15) << std::setprecision(Formatar ? 2 : 8)
          << itLLimpeza->SemiDistancia
          << " AreaAtual= "
          << std::setw(Formatar ? 8 : 15) << std::setprecision(Formatar ? 2 : 8)
          << itLLimpeza->AreaAtual
          << " VolumeAtual= "
          << std::setw(Formatar ? 9 : 15) << std::setprecision(Formatar ? 0 : 8)
          << itLLimpeza->VolumeAtual
          << " VolumeTotal= "
          << std::setw(Formatar ? 9 : 15) << std::setprecision(Formatar ? 0 : 8)
          << itLLimpeza->VolumeTotal;

        if (TerminarLinha) steBuffer << "\r" << std::endl;
        else steBuffer << std::endl;
      }
    }
    break;
    case IDD_REL_PROJ_TERRAPLANAGEM:
    {
      steBuffer << "RELATÓRIO DO PROJETO DE TERRAPLANAGEM:\r\n";

      for (ittymapEstlstPontos itSecao = pMapPontosTerrplProjeto->begin(); itSecao != pMapPontosTerrplProjeto->end(); itSecao++)
      {
        steBuffer << std::right << std::fixed << std::setfill(' ');

        std::string Descricao(itSecao->first.Descricao);

        Descricao.append(7 - Descricao.size(), ' ');

        steBuffer << CEstaca(itSecao->first.x, itSecao->first.y, 0, Descricao.c_str());

        for (itlstPontos itPontos = itSecao->second.begin(); itPontos != itSecao->second.end(); itPontos++)
        {
          Descricao = itPontos->Descricao;
          Descricao.append((Formatar ? 7 : 15) - Descricao.size(), ' ');

          steBuffer << "   Afast = "
            << std::setw(Formatar ? 7 : 15) << std::setprecision(Formatar ? 2 : 8)
            << itPontos->i << "   "
            << std::setprecision(Formatar ? 3 : 8)
            << itPontos->x << "   "
            << itPontos->y << "   "
            << itPontos->z << "   "
            << Descricao << "   ";
        }
        steBuffer << std::endl;
      }

      steBuffer << "Pontos de Passagem:" << std::endl;

      for (int Lado = ESQUERDO; Lado <= DIREITO; Lado++)
      {
        if (Lado == ESQUERDO) steBuffer << " Lado Esquerdo:";
        else steBuffer << " Lado Direito:";

        steBuffer << std::setprecision(2) << std::endl;

        for (itlstPontos it = PontosPassagem[Lado].begin(); it != PontosPassagem[Lado].end(); it++)
        {
          steBuffer << CEstaca(it->x) << std::endl;
        }
      }
    }
    break;
    case IDD_REL_LOG_CALCTERRAP:
    {
      steBuffer << "RELATÓRIO DO LOG DO CÁLCULO DA TERRAPLANAGEM: \r\n";

      steBuffer << strstrArqLog.str();
    }
    break;
    case IDD_REL_LOG_CALCSUPER:
    {
      steBuffer << "RELATÓRIO DO LOG DO CÁLCULO DA SUPERFÍCIE: \r\n";

      steBuffer << strstrArqLog.str();
    }
    break;
    case IDD_REL_LOG_FRONTEIRA:
    {
      steBuffer << "RELATÓRIO DO LOG DA DEFINIÇÃO DA FRONTEIRA: \r\n";

      steBuffer << strstrArqLog.str();
    }
    break;
    case IDD_REL_LOG_SECCIONAR:
    {
      steBuffer << "RELATÓRIO DO LOG DO SECCIONAMENTO : \r\n";

      steBuffer << strstrArqLog.str();
    }
    break;

    case IDD_REL_VOLUMES_TIPO_SOLO:
    {
      CPerfilGeologico PerfilGeologico(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual());
      PerfilGeologico.Serialize(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual(), LER);

      steBuffer << "RELATÓRIO DOS VOLUMES POR TIPO DE SOLO:\r\n";

      //   PerfilGeologico.CalculaVolumes();

      setitCDadosPerfGeologicos SetItDadosPerfisPorTipo;

      //--- Monta o set por tipo de solo

      for (itsetCDadosPerfGeologicos itPerfGeo = PerfilGeologico.PegaSetSecaoPerfGeo().begin(); itPerfGeo != PerfilGeologico.PegaSetSecaoPerfGeo().end(); ++itPerfGeo)
      {
        SetItDadosPerfisPorTipo.insert(itPerfGeo);
      }

      //--- Imprime os volumes de todos os tipos de solos

      CArqTiposSolos ArqTiposSolos(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual());
      tysetCTipoSolo SetTiposSolos;
      ArqTiposSolos.CriaSet(SetTiposSolos);

      steBuffer << std::right << std::fixed << std::setfill(' ') << std::setprecision(Formatar ? 3 : 8);

      itsetitCDadosPerfGeologicos itPerfGeo(SetItDadosPerfisPorTipo.begin());

      while (itPerfGeo != SetItDadosPerfisPorTipo.end())
      {
        std::string TipoSoloAtual((*itPerfGeo)->PegaNomeTipoSolo());

        while (itPerfGeo != SetItDadosPerfisPorTipo.end() && TipoSoloAtual.compare((*itPerfGeo)->PegaNomeTipoSolo()) == 0)
        {
          std::string TipoSoloAtual((*itPerfGeo)->PegaNomeTipoSolo());
          double VolumeAcumulado(0.0), EstacaAnterior((*itPerfGeo)->PegaEstaca()), AreaAnterior(0.0);

          steBuffer << "SOLO TIPO: " << TipoSoloAtual << "\r\n";

          while (itPerfGeo != SetItDadosPerfisPorTipo.end() && (*itPerfGeo)->PegaNomeTipoSolo().compare(TipoSoloAtual) == 0)
          {
            double SemiDistancia = ((*itPerfGeo)->PegaEstaca() - EstacaAnterior) / 2.0,
              AreaAtual = (const_cast<CDadosPerfisGeologicos*>(&**itPerfGeo))->PegaArea(0),
              VolumeAtual = (AreaAnterior + AreaAtual) * SemiDistancia;

            VolumeAcumulado += VolumeAtual;

            steBuffer << "Estaca: " << CEstaca((*itPerfGeo)->PegaEstaca())
              << " SemiDistancia = "
              << std::setw(Formatar ? 8 : 15) << std::setprecision(Formatar ? 2 : 8) << SemiDistancia
              << " Area Corte= "
              << std::setw(Formatar ? 8 : 15) << std::setprecision(Formatar ? 2 : 8) << AreaAtual
              << " Volume Seção= "
              << std::setw(Formatar ? 8 : 15) << std::setprecision(Formatar ? 1 : 8) << VolumeAtual
              << " VolumeAcumulado="
              << std::setw(Formatar ? 8 : 15) << std::setprecision(Formatar ? 0 : 8) << VolumeAcumulado;

            if (TerminarLinha) steBuffer << "\r" << std::endl;
            else steBuffer << std::endl;

            AreaAnterior = AreaAtual;
            EstacaAnterior = (*itPerfGeo)->PegaEstaca();

            ++itPerfGeo;
          }
        }
        if (itPerfGeo != SetItDadosPerfisPorTipo.end()) TipoSoloAtual = (*itPerfGeo)->PegaNomeTipoSolo();
      }
    }
    break;
    case IDD_REL_BACIAS_HIDROGRAF:
    {
      static std::array<std::string,5> ArrMetodosCalculoTc = {"Ven Te Chow ","Kirpick ", "Picking ","Kerby-Hataway ","D.N.O.S. "};
      static std::array<std::string,7> ArrMetodosCalculoQ = {"Racional ","Racional Corrigido ","I Pai Wu ","Ven Te Chow ","McMath ","Kokei Uehara "," Hidrograma Triangular Sintético "};

      steBuffer << "RELATÓRIO DAS BACIAS HIDROGRAFICAS:" << std::endl;

      for (ittylstBacias itBH = pBaciasHidrograficas->begin(); itBH != pBaciasHidrograficas->end(); itBH++)
      {
        steBuffer << std::right << std::fixed << std::setfill(' ') << std::setprecision(Formatar ? 3 : 8);

        CDadosBacia& DadosBacia(itBH->PegaDadosBacia());

        steBuffer << "Nome = " << DadosBacia.NomeBacia << std::endl
                  << "DADOS GEOMÉTRICOS:" << std::endl
                  << "Pontos do Espigão = ";

        for (citlstitsetPontos it = itBH->EspigaoBacia.PegaLista().begin(); it != itBH->EspigaoBacia.PegaLista().end(); it++)
        {
          steBuffer << (*it)->x << ' ' << (*it)->y << ' ' << (*it)->z << ';';
        }

        steBuffer << std::endl;

        int NumTalv(1);

        steBuffer << "Pontos dos Talvegues:" << std::endl;

        for (ItLLDeltaSup itTalv = itBH->Talvegues.begin(); itTalv != itBH->Talvegues.end(); itTalv++)
        {
          steBuffer << "Talvegue " << NumTalv++ << " = ";

          for (ItLDeltaSup itPTalv = itTalv->begin(); itPTalv != itTalv->end(); itPTalv++)
          {
            steBuffer << itPTalv->PCentral.x << ' ' << itPTalv->PCentral.y << ' ' << itPTalv->PCentral.z << ';';
          }

          steBuffer << std::endl;
        }

        if (itBH->Calculada() == false)
        {
          steBuffer << "=====================================================================================================" << std::endl
                    << "ATENÇÃO => OS DADOS DESTA BACIA ESTÃO OBSOLETOS. SERÃO APRESENTADOS OS DADOS DO ÚLTIMO CÁLCULO FEITO." << std::endl
                    << "=====================================================================================================" << std::endl;
        }

        steBuffer << "Area (m2) = " << itBH->EspigaoBacia.Area << std::endl
                  << "Perímetro (m) = " << itBH->EspigaoBacia.Perimetro <<  std::endl
				          << "Altura do talvegue principal = " <<  DadosBacia.h << std::endl
                  << "Coeficiente de Compacidade = " << DadosBacia.Kc << std::endl
                  << "Fator de Forma = " << DadosBacia.Kf << std::endl
                  << "Comprimento Total dos Talvegues (Lt) (m) = " << DadosBacia.Lt << std::endl
                  << "Densidade de Drenagem (m/m2) = " << DadosBacia.Dd << std::endl
                  << "Percurso Médio de Escoamento (L) (m) =" << DadosBacia.L << std::endl
                  << "DADOS HIDROLÓGICOS:" << std::endl
                  << "Intensidade da Chuva (i) = " << DadosBacia.i << std::endl
                  << "Tempo de Duração da Chuve (t) = " << DadosBacia.t << std::endl
                  << "Tempo de Retorno da Chuva (T) = " << DadosBacia.T << std::endl
                  << "Coeficiente de Distribuição Espacial (K) = " << DadosBacia.K << std::endl
                  << "Coeficiente de Escoamento Superficial (C) = " << DadosBacia.C << std::endl
                  << "Coeficiente Volumétrico de Escoamento (C2) = " << DadosBacia.C2 << std::endl
                  << "Declividade média da bacia (I) = " << DadosBacia.I << std::endl 
                  << "Volume de Escoamento do Hidrograma (V) (m3) = " << DadosBacia.V << std::endl
                  << "Declividade Equivalente (S) (m/m) = " << DadosBacia.S << std::endl
                  << "Vazão de Cheia (Q) (m3/s) = " << DadosBacia.Q << std::endl
                  << "Tempo de Concentração (Tc) (min) =  " << DadosBacia.Tc << std::endl
                  << "Vazão Máxima de Projeto (Qp) (m3/s) = " << DadosBacia.Qp << std::endl
                  << "Coeficiente de Forma (C1) (mm/h) = " << DadosBacia.C1 << std::endl
                  << "Raio Hidráulico da Bacia(RH) (m) = " << DadosBacia.RH << std::endl
                  << "Declividade Mádia da Bacia (i) (m/m) = " << DadosBacia.i << std::endl
                  << "Vazão (Equação da Continuidade) (Q) (m3/s) = " << DadosBacia.Q << std::endl
                  << "Velocidade Mádia (Manning) (V) (m/s) = " << DadosBacia.V << std::endl
                  << "Vazão (Manning e continuidade) (Q) (m3/s) = " << DadosBacia.Q << std::endl
                  << "Fator de deflúivio (X) (Ven Te Chow) = " << DadosBacia.Fator_X << std::endl
                  << "Fator climático (Y) (Ven Te Chow) = "<< DadosBacia.Fator_Y << std::endl
                  << "Fator de Redução do tempo (Ven te Chow) = " << DadosBacia.Fator_Z << std::endl
                  << "Fator de retardância  (Kerby-Hathaway) = " << DadosBacia.Fator_N << std::endl
                  << "Fator admensional das características da bacia (D.N.O.S.) = " << DadosBacia.Fator_K << std::endl
                  << "Coeficiente de retardo (Teta) (Racional Corrigido) = " << DadosBacia.CoefRetardo_Teta << std::endl
                  << "Distancia do Centro de gravidade até a foz (LCG) = " << DadosBacia.LCG << std::endl
                  << "Coeficiente armazenamento (Ct) (Kokei Uehara) = " << DadosBacia.CT << std::endl
                  << "Intensidade máxima (Im) (Kokei Uehara) = " << DadosBacia.Im << std::endl
                  << "Tempo Retardamento (Tr) (Kokei Uehara) = " << DadosBacia.Tr << std::endl
                  << "Tempo de duração da chuva (Td) (Kokei Uehara) = " << DadosBacia.Td << std::endl 
                  << "Intensidade Máxima da chuva (h1) (Kokei Uehara) = " << DadosBacia.h1 << std::endl         
                  << "Intensidade Máxima da chuva corrijida (h1c) (Kokei Uehara) = " << DadosBacia.h1c << std::endl       
                  << "Altura exedente de escoamento (Hexc) (Kokei Uehara) = " << DadosBacia.Hexc << std::endl    
                  << "Volume de escoamento (Vesc)(kokei Uehara) = " << DadosBacia.Vesc << std::endl  
                  << "Tempo base (Tb) (Kokei Uehara) = " << DadosBacia.Tb << std::endl    
                  << "Precipitação Efetiva (Hidrograma Triangular sintético) = " << DadosBacia.qm << std::endl 
                  << "Raio Hidráulico do canal na foz = " << DadosBacia.rh << std::endl
                  << "Declividade do canal na foz  = " << DadosBacia.I_Canal << std::endl
                  << "Coeficiente de Rugosidade do canal na foz(n) = " << DadosBacia.n << std::endl
                  << "Area molhada do canal = " << DadosBacia.Am <<  std::endl 
                  << "Foz = " << itBH->PegaFoz().x << ' ' << itBH->PegaFoz().y << ' ' << itBH->PegaFoz().z << std::endl
                  << "Método de cálculo do tempo de concentração (Tc) = " << ArrMetodosCalculoTc[DadosBacia.MetodoCalculoTc-1] << std::endl
                  << "Método de cálculo da vazão (Q) = " << ArrMetodosCalculoQ[DadosBacia.MetodoCalculoQ-1] << std::endl
                  << "Centro de Gravidade (CG) =  " << itBH->CG().x  << " " << itBH->CG().y << " ";

        if (itBH->CG().z != INFINITO) steBuffer << itBH->CG().z << std::endl;
        else steBuffer << "Cota a ser calculada" << std::endl;

        steBuffer << "Distância do CG até a foz (L) = ";

        if (itBH->CG().S != INFINITO) steBuffer << itBH->CG().S << std::endl;
        else steBuffer << " A ser calculada" << std::endl;
                         
        steBuffer << "Hipsograma = ";

        for (ittysetCotasArea itHist = itBH->PegaSetCotasAreas().begin() ; itHist != itBH->PegaSetCotasAreas().end() ; itHist++)
        {
          steBuffer << itHist->Cota << ' ' << itHist->AreaCN << ';';
        }

        steBuffer << std::endl << "Perfil Talvegue Principal = ";

        if(itBH->PegaPerfTalvPrincipal().size() > 1)
        {
          itsetPontos itTP(itBH->PegaPerfTalvPrincipal().begin());

          steBuffer << itTP->x << ' ' << itTP->y << ';';

          for (itTP++ ; *itTP != *itBH->PegaPerfTalvPrincipal().rbegin() ; itTP++)
          {
            if(Mat.EMultiplo(itTP->y,1.0/* itBH->EquidistCNHistograma()*/))
            {
              steBuffer << itTP->x << ' ' << itTP->y << ';';
            }
          }

          steBuffer << itTP->x << ' ' << itTP->y << ';';
        }

        steBuffer << std::endl << "Partições do talvegue principal (" << itBH->PegaParticoesTalveguePrincipal().size() << ") = ";

        for (itsetPontos it = itBH->PegaParticoesTalveguePrincipal().begin(); it != itBH->PegaParticoesTalveguePrincipal().end(); ++it)
        {
          steBuffer << it->x ; 
          if(it->i != INFINITO)  steBuffer << " i = " << it->i <<  "; "; 
          else steBuffer << " A ser calculado ; ";
        }

        steBuffer << std::endl;
      }
    }
    break;
    case IDD_REL_EQUIP_DRENAGEM:
    {
      int Contador(0);

      steBuffer << "RELATÓRIO DAS OBRAS DE ARTE CORRENTES" << std::endl;

      steBuffer << std::right << std::fixed << std::setfill(' ') << std::setprecision((Formatar == false) ? 2 : 3);

      steBuffer << "Quantidade de Obras de Arte= " << pEquipDrenagem->size() << std::endl;

      for (ittymapEquipDrenagem itED = pEquipDrenagem->begin(); itED != pEquipDrenagem->end(); itED++)
      {
        Contador++;

        steBuffer << "N= " << Contador <<  ",Nome= " << itED->second.PegaNome() << ',';

        CEstaca Estaca(itED->second.PegaEstacaEixo());

        double dDimensoes[7];

        itED->second.PegaDimensoes(&dDimensoes[0]);

        if(Estaca.EstReal != INFINITO) steBuffer << "Estaca = " << Estaca << ',';
               
        steBuffer << "a= " << dDimensoes[0] << ',' << "b= " << dDimensoes[1] << ',' << "c= " << dDimensoes[2] << ','
                  << "d= " << dDimensoes[3] << ',' << "e= " << dDimensoes[4] << ',' << "f= " << dDimensoes[5] << ',' 
                  << "g= " << dDimensoes[6];

        if(itED->second.Az() != INFINITO)
        {
          steBuffer << ',' << "Azimute= " <<  (Mat.strg_gms(itED->second.Az() * Matem::PI_GRAUS));
        }
       
        if (itED->second.Esconsidade() != INFINITO && itED->second.Esconsidade() > .5)
        {
          steBuffer << ',' << "Esconsidade= " << (Mat.strg_gms(itED->second.Esconsidade() * Matem::PI_GRAUS));
        }

        if (itED->second.Perimetro != INFINITO)
        {
          steBuffer << ',' << "Comprimento= " << itED->second.Perimetro;
        }

        steBuffer << std::endl;
      }
    }
    break;
  }
}

void CEstRel::OnCloseDocument()
{
  if (AfxMessageBox("Deseja gravar o arquivo?", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
  {
    SetModifiedFlag(true);
    OnSaveDocument(m_strPathName);
  }

  CDocument::OnCloseDocument();
}

void CEstRel::LeEquipDrenagem(const std::string& NomeProjBase)
{
  pEquipDrenagem->clear();

  CString NomeProjeto(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()));

  CString Nom(NomeProjeto + CString(".edr"));
  CFile Arq;
  CFileException e;

  if (Arq.Open(Nom, CFile::modeRead, &e) == 0)
  {
    monolog.mensagem(1,Nom);

    return;
  }

  CArchive CArcEquipDrenagem(&Arq, CArchive::load);

  size_t Tam(0);
  std::string Nome;
  double AnguloDeRotacao(0.0);

  for (CArcEquipDrenagem >> Tam; Tam; --Tam)
  {
    CArcEquipDrenagem >> Nome;

    pEquipDrenagem->emplace(Nome,CEquipDrenagem());

    CEquipDrenagem& EquipAtual((pEquipDrenagem->rbegin())->second);

    EquipAtual << CArcEquipDrenagem;
  }
}

void CEstRel::LeBaciasHidrograficas(const std::string& NomeProjBase)
{
  pBaciasHidrograficas->clear();

  CString Nom(NomeProjBase.c_str() + CString(".bah"));
  CFile Arq;
  CFileException e;

  if (Arq.Open(Nom, CFile::modeRead, &e) == TRUE)
  {
    size_t QuanBacias(0);
    CArchive CArcBacias(&Arq, CArchive::load);
    double AnguloDeRotacao;

    CArcBacias >> AnguloDeRotacao;

    for (CArcBacias >> QuanBacias; QuanBacias > 0; --QuanBacias)
    {
      pBaciasHidrograficas->push_back(CBacia());
      CBacia& BaciaAtual(*pBaciasHidrograficas->rbegin());

      BaciaAtual << CArcBacias;
    }
  }
}
