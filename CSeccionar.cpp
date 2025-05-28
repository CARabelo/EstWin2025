// CSeccionar.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "cponto.h"
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
#include "CLPontos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "CEquipDrenagem.h"
#include "Superficie.h"
#include "DSecionar.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqgradientes.h"
#include <string>
#include "cgradientes.h"
#include "monologo.h"
#include "dcalcgeoseccdireto.h"
#include "carqsecoesavulsas.h"
#include "csecaoavulsa.h"
#include "csecoesavulsas.h"
#include <afxmt.h>   
#include "Perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "CSeccionar.h"
#include "DSecionar.h"
#include <strstream>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "CArqTrechos.h"
#include <sstream>
#include <iomanip>  
#include "Mat.h"
#include "CalcSU\CCCalcSU.h"

#define LER 1
#define GRAVAR 2

const double M_PI_2 = 1.57079632679489661923;

extern class monologo monolog;
extern class Matem Mat;

CSeccionar::CSeccionar(CString &pNomeProj, CDialog* Diag, CEvent* Evento, bool pOtimizar, double pMinDist, double pMinIncl,bool pTodosTrechos) :
  NomeProjeto(pNomeProj), 
  pDialogo(Diag), 
  EventoFim(Evento),
  Otimizar(pOtimizar), 
  douDistMinima(pMinDist), 
  douIncMinima(pMinIncl),
  Superficie(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase())),
  SuperficieTerrap(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual())),
  PerfisAvulsos(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual())),
  TodosTrechos(pTodosTrechos),
  TemArqTriaSuperf(false)
{
  TemArqTriaSuperf = CCalcSU::IniciaCalculaCotas(((CMainFrame*)AfxGetMainWnd())->m_sNomeComp.GetBuffer()) == 0;
}

CSeccionar::~CSeccionar()
{
}

void CSeccionar::CalculaPontosExtremos(const Ponto& PontoEixo, Ponto& PontoEsq, Ponto& PontoDir)
{
  double m_intEquidis(((CDSecionar*)pDialogo)->m_iEquidis);

  double az(PontoEixo.fi - M_PI_2);
  double Deltax(sin(az) * m_intEquidis), Deltay(cos(az) * m_intEquidis);

  PontoEsq.x = PontoEixo.x + Deltax;
  PontoEsq.y = PontoEixo.y + Deltay;
  PontoDir.x = PontoEixo.x - Deltax;
  PontoDir.y = PontoEixo.y - Deltay;

  Ponto PontoEsqDesrot(PontoEsq), PontoDirDesrot(PontoDir);

  if (Superficie.EstaRotacionada())
  {
    double AnguloRotacao(-Superficie.PegaAnguloRotacao());
    
    PontoEsqDesrot.RotacionaPlano(sin(AnguloRotacao), cos(AnguloRotacao), AnguloRotacao);
    PontoDirDesrot.RotacionaPlano(sin(AnguloRotacao), cos(AnguloRotacao), AnguloRotacao);
  }

  //--- A fronteira está rotacionada mas a superfície não esta....

  if(!Superficie.VerfPontoForaSuperficie(PontoEsq)) CalcCotaSuperf(PontoEsq);
  if(!Superficie.VerfPontoForaSuperficie(PontoDir)) CalcCotaSuperf(PontoDir);
}

void CSeccionar::Interpola(setPontos& SSetxSecao, Ponto& PCalc)      //--- Insere o ponto em x na lista
{
  itsetPontos itSetxPatual(SSetxSecao.upper_bound(Ponto(PCalc.x, 0.0)));

  if (itSetxPatual == SSetxSecao.end()) return;

  const Ponto& P = *itSetxPatual;

  if ((itSetxPatual == SSetxSecao.begin() && PCalc.x < P.x) || (itSetxPatual == (SSetxSecao.end())-- && PCalc.x > P.x)) return;

  if (PCalc.x > P.x)
  {
    itSetxPatual++;
    const Ponto& PProx = *itSetxPatual;
    PCalc.i = (PProx.y - P.y) / (PProx.x - P.x);
  }
  else
  {
    itSetxPatual--;
    const Ponto& PAnte = *itSetxPatual;
    PCalc.i = (PAnte.y - P.y) / (PAnte.x - P.x);
  }

  PCalc.y = PCalc.i * (PCalc.x - P.x - PCalc.x) + P.y;

  SSetxSecao.insert(PCalc);
}

bool CSeccionar::Iniciar(bool bTodas)
{
  CFile ArqCalcGeo;
  CFileException e;

  std::stringstream strstrMensagem;

  strstrMensagem << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

  std::ofstream ArqLog((CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase())) + "_Seccionar.log", std::ofstream::out | std::ofstream::trunc);

  GravaLog("Vias Urbanas & Estradas Versão 4.51 - 2019\n", ArqLog,true);
  GravaLog("ARQUIVO DE LOG DO SECCIONAMENTO DO TERRENO - INÍCIO ========================", ArqLog);

  strstrMensagem << std::endl << "Configuração: " << (bTodas ? "Seccionar todas as estacas" : "Seccionar apenas as estacas que ainda não estejam seccionadas") << std::endl;

  GravaLog(strstrMensagem.str(), ArqLog);

  if (Superficie.pSuperficieAtual->size() < 3)
  {
    strstrMensagem << "Não há pontos na superfície" << std::endl;

    monolog.mensagem(-1,strstrMensagem.str().c_str());
    GravaLog(strstrMensagem.str(), ArqLog);

    return false;
  }

  if (Superficie.pSuperficieAtual->size() == 0 || Superficie.pSuperficieAtual->begin()->PontosLigados.size() == 0)
  {
    strstrMensagem.str("");
    strstrMensagem << "A superfície não está gerada, primeiro calcule a superfície para depois seccioná-la." << std::endl;
    monolog.mensagem(-1,strstrMensagem.str().c_str());
    GravaLog(strstrMensagem.str(), ArqLog);

    return false;
  }

  if (Superficie.TemFronteira() == false)
  {
    strstrMensagem.str("");
    strstrMensagem << "A fronteira não está definida." << std::endl;
    monolog.mensagem(-1,strstrMensagem.str().c_str());
    GravaLog(strstrMensagem.str(), ArqLog);
    return false;
  }
  
  int ContaCertos(0), ContaErrados(0),ContaGeraisCertas(0),ContaAvulsasCertas(0);
  bool HouveErro(false);

  NomeTrecho = NomeProjeto.Right(NomeProjeto.GetLength() - NomeProjeto.ReverseFind('\\') - 1);

  CArqTrechos  ArqTrechos(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase(), true);

  for (ittymapNomesTrechos it = ArqTrechos.MapaNomesTrechos.begin(); it != ArqTrechos.MapaNomesTrechos.end(); it++)
  {
    CString TrechoAtual(it->first.c_str());
    TrechoAtual = TrechoAtual.Right(TrechoAtual.GetLength() - TrechoAtual.ReverseFind('\\') - 1);

    if (!TodosTrechos && TrechoAtual != NomeTrecho) continue;

    CString NomeArquivo(it->first.c_str() + CString(".geo"));
    CLPerfis TerrenoAnterior(it->first.c_str() + CString(".tca"));
    tysetpPerfis SetTerrenoAnterior;

    if (ArqCalcGeo.Open(NomeArquivo, CFile::modeRead, &e) == 0)
    {
      ContaErrados++;

      strstrMensagem.str("");
      strstrMensagem << "Houve erro na abertura do arquivo: " << NomeArquivo << ". O trecho não foi seccionado." << std::endl;

      //AfxMessageBox(strstrMensagem.str().c_str(), MB_ICONERROR);
      GravaLog(strstrMensagem.str(), ArqLog);

      VoltaBackupArquivos();

      //continue;
      HouveErro = true;
    }

   
    int ContaSecoesSeccionadas(0), ContaSecoesPassadas(0), QSecoes(0);

    if (!HouveErro)    //--- Se não, não tem seções ou houve erro na abertura dos arquivos das seções
    {
      BackupArquivos();

      CArchive CArqCalcGeo(&ArqCalcGeo, CArchive::load);

      CFile ArqCalcSec;

      //--- .pes pontos extremos da secão

      NomeArquivo = it->first.c_str() + CString(".pes");

      if (ArqCalcSec.Open(NomeArquivo, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone, &e) == 0)
      {
        ContaErrados++;

        strstrMensagem.str("");
        strstrMensagem << "Houve erro na abertura do arquivo: " << NomeArquivo << ". O trecho não foi seccionado." << std::endl;

        //AfxMessageBox(strstrMensagem.str().c_str(), MB_ICONERROR);
        GravaLog(strstrMensagem.str(), ArqLog);

        VoltaBackupArquivos();

        //continue;
      }

      CArchive CArqCalcSec(&ArqCalcSec, CArchive::store);

      NomeArquivo = it->first.c_str() + CString(".tca");

      SetTerrenoAnterior.clear();

      if (!bTodas) CriaSetTerrenoAnterior(TerrenoAnterior, SetTerrenoAnterior);

      CFile ArqSecTria;

      if (ArqSecTria.Open(NomeArquivo, CFile::modeWrite | CFile::modeCreate, &e) == 0)
      {
        ContaErrados++;

        strstrMensagem.str("");
        strstrMensagem << "Houve erro na abertura do arquivo: " << NomeArquivo << ". O trecho não foi seccionado." << std::endl;

        //AfxMessageBox(strstrMensagem.str().c_str(), MB_ICONERROR);
        GravaLog(strstrMensagem.str(), ArqLog);

        VoltaBackupArquivos();

        //continue;
      }

      CArchive CArqSecTria(&ArqSecTria, CArchive::store);

      CEstaca EstacaAtual;
      int QSecoes;

      // CSuperficie Superficie(NomeProjeto);

      CArqCalcGeo >> QSecoes;
      CArqCalcSec << QSecoes;

      //--- So se seberá quantas seções foram efetivamente seccionadas no final desta rotina. Então inicia com 0
      //--- e no final grava-se a quantidade de seções no início do arquivo,

      CArqSecTria << (int)0;
      CArqSecTria << ((CDSecionar*)pDialogo)->m_iEquidis;
      CArqSecTria.Flush();

      //  SSetxPontos SSetxSecao;

      NomeArquivo = it->first.c_str();

      CGradiente Gradientes(NomeArquivo);     //--- Prepara os gradientes, serão usado para calcular as cotas da s estacas

      double Equidis_2(pow(((CDSecionar*)pDialogo)->m_iEquidis, 2.0));

      CProgressCtrl* Prog = NULL;
      if (pDialogo)
      {
        Prog = (CProgressCtrl*)pDialogo->GetDlgItem(IDC_PROGRESS1);
        Prog->SetRange(0, QSecoes);
        Prog->SetStep(1);
        Prog->SetPos(0);
      }

      pDialogo->SetDlgItemText(IDC_DESCTRECHO, TrechoAtual);

      strstrMensagem.str("");
      strstrMensagem << NomeTrecho << std::endl << "Seccionando as estacas... " << NomeArquivo << std::endl;
      pDialogo->SetDlgItemText(IDC_DESCRICAO1, strstrMensagem.str().c_str());
      GravaLog(strstrMensagem.str(), ArqLog);

      for (int ContaSecoes = 0; ContaSecoes < QSecoes; ContaSecoes++)  //--- Secciona todas as estacas.
      {
        Ponto PontoACalc, PEsq, PDir;

        if (Prog) Prog->StepIt();

        EstacaAtual << CArqCalcGeo;
        PontoACalc << CArqCalcGeo;

        pDialogo->SetDlgItemText(IDC_DESCRICAO1, "\tSeccionando...\t" + EstacaAtual);

        //--- Se for para secionar apenas as seçoes que não existem passa a seção se ela existe.

        ittysetpPerfis itTerrenoAnterior(bTodas ? SetTerrenoAnterior.end() : SetTerrenoAnterior.find(&Perfil(EstacaAtual)));

        //--- Se os pontos extremos fossem calculados no momento em que se calcula o ponto no eixo (arquivo .geo) esse arquivo .pes não seria necessário.
        //--- <<======= ALTERAR

        CalculaPontosExtremos(PontoACalc, PEsq, PDir);

        setPontos SSetxSecao;

        if (itTerrenoAnterior == SetTerrenoAnterior.end())
        {
          Superficie.Seccionar(PEsq, PDir, PontoACalc, SSetxSecao, Equidis_2, bTodas,0.0,true,false,false);
        }
        else PassaTerrenoAnteriorSet(itTerrenoAnterior, SSetxSecao);

        if (Otimizar) LimpaSecao(SSetxSecao);

        //--- Calcula a cota da estaca (será desenhada no desenho do greide)

        Ponto PCalc(Gradientes.BuscaGradiente(EstacaAtual), INFINITO);
        Interpola(SSetxSecao, PCalc);  // não esta interpolando nada!!!
        EstacaAtual.Cota = PCalc.y;

        //--- Insere os acidentes topográficos nas seções

        CLPontos AcidentesTopograficos;

        InsereAcidentesTopograficos(Superficie, pDialogo, PEsq, PDir, AcidentesTopograficos, CSecaoAvulsa::SECAO_COMUM, nullptr);

        EstacaAtual >> CArqCalcSec;
        PDir >> CArqCalcSec;
        PontoACalc >> CArqCalcSec;
        PEsq >> CArqCalcSec;

        //--- serializa a seção 

        if (SSetxSecao.size())
        {
          if (itTerrenoAnterior == SetTerrenoAnterior.end()) ++ContaSecoesSeccionadas;         //--- conta as seções efetivamente seccionadas
          else ++ContaSecoesPassadas;

          EstacaAtual >> CArqSecTria;
          CArqSecTria << (int)SSetxSecao.size();

          while (SSetxSecao.begin() != SSetxSecao.end())
          {
            *(const_cast<Ponto*>(&*SSetxSecao.begin())) >> CArqSecTria;
            SSetxSecao.erase(SSetxSecao.begin());
          }

          EstacaAtual >> CArqSecTria;
          AcidentesTopograficos.Serialize(CArqSecTria);
        }
      }

      //-- Finalmente grava a quantidade de seções

      pDialogo->SetDlgItemText(IDC_DESCRICAO1, "\tGravando os dados...\t");

      CArqSecTria.Flush();

      ArqSecTria.SeekToBegin();

      CArqSecTria << (ContaSecoesSeccionadas + ContaSecoesPassadas);

      //--- Fecha os arquivos

      CArqSecTria.Close();
      ArqSecTria.Close();

      CArqCalcGeo.Close();
      ArqCalcGeo.Close();

      CArqCalcSec.Close();
      ArqCalcSec.Close();

      ContaCertos++;


    }
    //---  Define os perfís avulsos

    strstrMensagem.str("");
    strstrMensagem << "Definindo os perfís avulsos " << NomeArquivo << std::endl;

    pDialogo->SetDlgItemText(IDC_DESCRICAO1,strstrMensagem.str().c_str());
    GravaLog(strstrMensagem.str(), ArqLog);

    int ContaPerfAvulsos(0);

    DefinirPerfisAvulsos(ContaPerfAvulsos, bTodas);

    PerfisAvulsos.Serialize((((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()), GRAVAR);

    strstrMensagem.str("");

    if (pDialogo)
    {
      if (ContaSecoesSeccionadas || ContaPerfAvulsos)
      {
        strstrMensagem << "OK, o trecho/rua foi seccionado com êxito." << std::endl 
                       << "Secões tranversais seccionadas: " << ContaSecoesSeccionadas << std::endl
                       << "Seções avulsas seccionadas: " << ContaPerfAvulsos << std::endl;
      }
      else strstrMensagem << "Nenhuma seção nova foi encontrada para ser seccionada." << std::endl;

      if (QSecoes != ContaSecoesPassadas) strstrMensagem << "Foram removidas " << (QSecoes - ContaSecoesPassadas) << " seções." << std::endl;
      else strstrMensagem << "Nenhuma seção foi removida" << std::endl;

      strstrMensagem << "=====> Fim do seccionamento do trecho/Rua " << NomeArquivo << std::endl;

      GravaLog(strstrMensagem.str(), ArqLog);

      ContaGeraisCertas += ContaSecoesSeccionadas;
      ContaAvulsasCertas += ContaPerfAvulsos;
    }

    RemoveBackupArquivos();
  }

  strstrMensagem.str("");
  strstrMensagem << "Trechos/Ruas Seccionados com Exito: " << ContaCertos << std::endl
                 << "Trechos/Ruas com Erro: " << ContaErrados << std::endl
                 << "Seções Tranversais Seccionadas: " << ContaGeraisCertas << std::endl
                 << "Seções Avulsas Seccionadas: " << ContaAvulsasCertas << std::endl;

  GravaLog(strstrMensagem.str(), ArqLog);
  GravaLog("====> FIM DO ARQUIVO DE LOG.", ArqLog);

  strstrMensagem << "Para detalhes veja o arquivo de log.";

  pDialogo->SetDlgItemText(IDC_DESCRICAO1, strstrMensagem.str().c_str());
  
  ((CDSecionar*)pDialogo)->m_buOK.EnableWindow(TRUE);
  ((CDSecionar*)pDialogo)->m_butPausar.EnableWindow(FALSE);

  return true;
}

void CSeccionar::CriaSetTerrenoAnterior(CLPerfis& TerrenoAnterior, tysetpPerfis& SetTerrenoAnterior)
{
  for (register POSITION Atual = TerrenoAnterior.GetHeadPosition(); Atual != NULL; )
  {
    SetTerrenoAnterior.insert(&TerrenoAnterior.GetNext(Atual));
  }
}

void CSeccionar::PassaTerrenoAnteriorSet(ittysetpPerfis itPerfilAnterior, setPontos& SSetxSecao)
{
  for (register POSITION Atual = (*itPerfilAnterior)->GetHeadPosition(); Atual != NULL; (*itPerfilAnterior)->GetNext(Atual))
  {
    SSetxSecao.insert((*itPerfilAnterior)->GetAt(Atual));
  }
}

bool CSeccionar::BackupArquivos()
{
  if (NomeProjeto.GetLength())
  {
    std::string strNomeArquivo(NomeProjeto.GetBuffer());
    strNomeArquivo = strNomeArquivo.substr(0, strNomeArquivo.rfind('.'));

    if (strNomeArquivo.size())
    {
      CopyFile(NomeProjeto + ".geo", NomeProjeto + ".geobak", false);
      CopyFile(NomeProjeto + ".pes", NomeProjeto + ".pesbak", false);
    }
  }

  return true;
}

bool CSeccionar::VoltaBackupArquivos()
{
  if (NomeProjeto.GetLength())
  {
    std::string strNomeArquivo(NomeProjeto.GetBuffer());
    strNomeArquivo = strNomeArquivo.substr(0, strNomeArquivo.rfind('.'));

    if (strNomeArquivo.size())
    {
      CopyFile(NomeProjeto + ".geobak", NomeProjeto + ".geo", false);
      CopyFile(NomeProjeto + ".pesbak", NomeProjeto + ".pes", false);
    }
  }

  return true;
}

bool CSeccionar::RemoveBackupArquivos()
{
  if (NomeProjeto.GetLength())
  {
    std::string strNomeArquivo(NomeProjeto.GetBuffer());
    strNomeArquivo = strNomeArquivo.substr(0, strNomeArquivo.rfind('.'));

    if (strNomeArquivo.size())
    {
      DeleteFile(NomeProjeto + ".geobak");
      DeleteFile(NomeProjeto + ".pesbak");
    }
  }

  return true;
}

void CSeccionar::LimpaSecao(setPontos& SetXPontos)
{
  if (SetXPontos.size() < 3) return;

  std::list<itsetPontos> LstRemover;
  std::list<itsetPontos>::iterator ItLstRemover;

  itsetPontos ItAnterior(SetXPontos.begin()), ItAtual(ItAnterior);
  ++ItAtual;
  double RampaAnterior((ItAtual->y - ItAnterior->y) / (ItAtual->x - ItAnterior->x)), RampaAtual(INFINITO);
  register itsetPontos ItProximo(ItAtual);

  for (++ItProximo; ItProximo != SetXPontos.end(); ++ItProximo)
  {
    if(ItProximo->Descricao == "GRADIE") continue;  //--- O gradiente da terraplanagem pode estar nas condições de remoção mas não pode ser removido

    RampaAtual = (ItProximo->y - ItAtual->y) / (ItProximo->x - ItAtual->x);

    if ((ItAtual->x - ItAnterior->x) < douDistMinima && fabs(RampaAtual - RampaAnterior) < douIncMinima)
    {
      LstRemover.push_back(ItAtual);
      ItAtual = ItProximo;
      RampaAnterior = (ItProximo->y - ItAnterior->y) / (ItProximo->x - ItAnterior->x);
    }
    else
    {
      ItAnterior = ItAtual;
      ItAtual = ItProximo;
      RampaAnterior = RampaAtual;
    }
  }
  for (register std::list<itsetPontos>::iterator it = LstRemover.begin(); it != LstRemover.end(); ++it)
  {
    //--- Pontos das alas dos bueiros não podem ser removidos

    if(!((*it)->Baliza2 & (CEquipDrenagem::PONTO_ALA_ESQUERDA | CEquipDrenagem::PONTO_ALA_DIREITA)))
    {
      SetXPontos.erase(*it);
    }
  }
}

bool CSeccionar::LeAcidentesTopograficos()
{
  CString NomeProjeto(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()));

  CFile ArqAciTopog;
  CFileException e;

  if (ArqAciTopog.Open(NomeProjeto + CString(".aci"), CFile::modeRead, &e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound)
    {
      monolog.mensagem(13, CString(NomeProjeto + CString(".aci ") + " (lendo os dados)"));
      return false;
    }

    return true;
  }

  CArchive ar(&ArqAciTopog, CArchive::load);

  //  AcidentesTopograficos << ar;

  return true;
};

void CSeccionar::DefinirPerfisAvulsos(int& ContaPerfAv,bool Forcar)
{
  Superficie.SerializarEquipDrenagem(LER, ((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual());

  CList<CSecaoAvulsa, CSecaoAvulsa&>&	PerfAvulsos(PerfisAvulsos.PegaLSecoesAvulsas());

  if (PerfAvulsos.GetSize() > 0)
  {
    for (POSITION Atual = PerfAvulsos.GetHeadPosition(); Atual != NULL; PerfAvulsos.GetNext(Atual))
    {
      CSecaoAvulsa& SecAvAtual(PerfAvulsos.GetAt(Atual));
      setPontos SetPontos, SetPontosProj;

      if (Forcar || SecAvAtual.GetCount() == 0)            //---Perfil novo? secciona-o
      {
        ContaPerfAv++;

        Ponto CPolar(0.0,-INFINITO);  //--- -INFINITO se o perfil não tem eixo.

        //--- Calcula a distância até o eixo para que o lado esquerdo fique negativo 
        //--- Se nao tiver eixo o eixo é o ponto da esquerda

        CPolar = Mat.R_P(Ponto(SecAvAtual.PInicial.x, SecAvAtual.PInicial.y), Ponto(SecAvAtual.PInicial.i, SecAvAtual.PFinal.i));
        CPolar.y *= CPolar.y; //-- é ao quadrado.

        if (SecAvAtual.Tipo == CSecaoAvulsa::PERFIL_BUEIRO)    //--- Se for bueiro prolonga o perfil 5m no inicio e no fim
        {
          //--- Inclui na seção de terreno os pontos dos encabeçamentos do bueiro

          ittymapEquipDrenagem itEquipDrenagemAtual(Superficie.PegaObrasDrenagem().find(SecAvAtual.Nome));

          if(itEquipDrenagemAtual != Superficie.PegaObrasDrenagem().end())     //--- Tem que achar...
          {
            Ponto PontoAlaEsq(*itEquipDrenagemAtual->second.PegaSet().begin()), PontoAlaDir(*itEquipDrenagemAtual->second.PegaSet().rbegin());

            //--- Insere os pontos das alas do bueiro.

//            SetPontos.insert(PontoAlaEsq);
 //           SetPontos.insert(PontoAlaDir);

            //--- Insere as alas do bueiro nos acidentes topográficos  (Plano xy)

//            PontoAlaEsq.x = PontoAlaEsq.S;
//            PontoAlaEsq.y = PontoAlaEsq.z;
//            PontoAlaEsq.z = INFINITO;

      //      SecAvAtual.AcidentesTopograficos.AddTail(PontoAlaEsq);

        //    PontoAlaDir.x = PontoAlaDir.S;
          //  PontoAlaDir.y = PontoAlaDir.z;
           // PontoAlaDir.z = INFINITO;

            //SecAvAtual.AcidentesTopograficos.AddTail(PontoAlaDir);
          }
        }
        //else
        {
          //--- Seções avulsas não tem a cota ns pontos inicial e final, calcula-as aqui
         //if(SecAvAtual.Tipo == 2)
		 //--- Agora tem ...??? verificar 07/07/2023
          { 
          //  CalcCotaSuperf(SecAvAtual.PInicial);
         //   CalcCotaSuperf(SecAvAtual.PFinal);
          }
        }

        Ponto CoordEixo(SecAvAtual.PInicial.i, SecAvAtual.PFinal.i, SecAvAtual.PInicial.z);    //--- AS coordenadas do eixo estão nos is

        Superficie.Seccionar(SecAvAtual.PInicial, SecAvAtual.PFinal, CoordEixo, SetPontos,CPolar.y, SecAvAtual.Tipo == CSecaoAvulsa::PERFIL_BUEIRO,0.0,true,false,true);
        SuperficieTerrap.Seccionar(SecAvAtual.PInicial, SecAvAtual.PFinal, CoordEixo, SetPontosProj,CPolar.y,false, 0.0,false,true,true);
        InsereAcidentesTopograficos(Superficie, pDialogo, SecAvAtual.PInicial, SecAvAtual.PFinal, SecAvAtual.AcidentesTopograficos, SecAvAtual.Tipo,&CoordEixo);

        if (Otimizar)
        {
          LimpaSecao(SetPontos);
          LimpaSecao(SetPontosProj);
        }

        //--- Passa o set para a lista de pontos na seção

        SecAvAtual.RemoveAll();

        for (itsetPontos it = SetPontos.begin(); it != SetPontos.end(); ++it)
        {
          SecAvAtual.AddTail(Ponto(*it));
        }

        SecAvAtual.LstPontosProjeto.RemoveAll();

        for (itsetPontos it = SetPontosProj.begin(); it != SetPontosProj.end(); ++it)
        {
         SecAvAtual.LstPontosProjeto.AddTail(Ponto(*it));
        }

        AcertaOffSets(SecAvAtual);  //--- A superficie de terraplanagem nas bordas é um pouco diferente do terreno.
      }
    }
  }
}

void CSeccionar::InsereAcidentesTopograficos(CSuperficie& pSuperficie,CDialog* pDialogo,Ponto& PEsq,Ponto& PDir,CLPontos& AciTopograficos,int TipoSecao,Ponto* pEixo)
{
  //--- Insere os acidentes topográficos na seção ou perfil avulso

  //--- Para todos os acidentes topográficos
  //--- Verifica se e seção intercepta algum acidente topografico
  //--- Se o ponto existir no set de pontos da seção marca nele o tipo de acidente topografico q ele é
  //--- Caso contrario insere o ponto (ocasionalmente o ponto pode não existir por causa da limpeza das seções 
  //--- que é feita feito quando o seccionamento é solicitado)    

  AciTopograficos.LimpaTudo();

  double LadoEsqBueiro(INFINITO);    //--- Comprimento do lado esquerdo do bueiro ao quadrado;

  if(pEixo != NULL)
  {
    LadoEsqBueiro = hypot(PEsq.x-pEixo->x,PEsq.y-pEixo->y);  
  }

  const LLDeltaSup& ListaRestAtual(pSuperficie.PegaListaRestricoes());

  for (ItCLLDeltaSup LDRestricaoAtual = ListaRestAtual.begin(); LDRestricaoAtual != ListaRestAtual.end(); ++LDRestricaoAtual)
  {
    unsigned int TipoRestricao(pSuperficie.VerfAciTopografico(LDRestricaoAtual));

    if(TipoRestricao > CAciTopografico::INICIO_TERRAP && TipoRestricao < CAciTopografico::FIM_TERRAP) continue;  //--- Acidentes da Terrap não interessam

    register ItCLDeltaSup ItLRestricao((*LDRestricaoAtual).begin());

    const Ponto* pPontoAnterior(&(*ItLRestricao).PegaPCentral());

    while (++ItLRestricao != (*LDRestricaoAtual).end())
    {
      const Ponto* pPontoAtual(&(*ItLRestricao).PegaPCentral());
      Ponto Intersecao;

      if (CLPontos::CalculaIntersecao(pPontoAnterior, pPontoAtual, PEsq, PDir, Intersecao))
      {
        double DistRestricao(hypot(pPontoAtual->x - pPontoAnterior->x, pPontoAtual->y - pPontoAnterior->y)),
               DistRestInter(hypot(Intersecao.x - pPontoAnterior->x, Intersecao.y - pPontoAnterior->y));

        Intersecao.z = (pPontoAtual->z - pPontoAnterior->z) / DistRestricao * DistRestInter + pPontoAnterior->z;

        double DistanciaSecao(hypot(Intersecao.x - PEsq.x, Intersecao.y - PEsq.y));

        if(pEixo != NULL)
        {
          DistanciaSecao = hypot(Intersecao.x - PEsq.x, Intersecao.y - PEsq.y);

          if (DistanciaSecao < LadoEsqBueiro) DistanciaSecao = -(LadoEsqBueiro - DistanciaSecao);
          else DistanciaSecao -= LadoEsqBueiro;
        }
        else
        {
          if (DistanciaSecao < ((CDSecionar*)pDialogo)->m_iEquidis) DistanciaSecao = (((CDSecionar*)pDialogo)->m_iEquidis - DistanciaSecao) * -1.0;
          else DistanciaSecao -= ((CDSecionar*)pDialogo)->m_iEquidis;
        }

        Intersecao.x = DistanciaSecao;
        Intersecao.y = Intersecao.z;
        Intersecao.AcidenteTopografico = TipoRestricao;
        AciTopograficos.InsereOrdenada(Intersecao, FALSE);
      }
      pPontoAnterior = pPontoAtual;
    }
  }
}

void CSeccionar::GravaLog(const std::string& strMsg, std::ostream& Stream, bool ForcarHora)
{
  CTime Agora(CTime::GetCurrentTime());
  static CTime GravAnterior(0);
  std::stringstream strstreamTemp;

  strstreamTemp << std::right << std::fixed << std::setfill('0') << std::setprecision(2);

  if (ForcarHora || CTimeSpan((Agora - GravAnterior)).GetTotalSeconds() > 30L)
  {
    strstreamTemp << "=====> " << Agora.GetDay() << '-' << Agora.GetMonth() << '-' << Agora.GetYear() << ' ' << Agora.GetHour() << ':' << Agora.GetMinute() << ':' << Agora.GetSecond() << ' ' << std::ends;
    Stream << strstreamTemp.str();
    GravAnterior = Agora;
  }

  Stream << strMsg.c_str() << std::endl;
}

//--- A superficie de terraplanagem nas bordas é um pouco diferente do terreno,
//--- e os offsets saem um pouco fora do terreno. para "Acertar" inclui o ponto de offset
//--- no terreno

void CSeccionar::AcertaOffSets(CSecaoAvulsa& SecAvAtual)
{
  SecAvAtual.AcertaOffSets();
}

void CSeccionar::CalcCotaSuperf(Ponto& PSemCota)
{
  Ponto P1, P2, P3,PDesrot(PSemCota);

  if (Superficie.EstaRotacionada())
  {
    double AngRot(-Superficie.PegaAnguloRotacao()), Sin(sin(AngRot)), Cos(cos(AngRot));

    PDesrot.RotacionaPlano(Sin, Cos, AngRot);
  }

  if (CCalcSU::PegaTriangulo(P1, P2, P3, PDesrot) == 0)
  {
    //--- Em SU superficie está desrotacionada

    if (Superficie.EstaRotacionada())
    {
      double AngRot(Superficie.PegaAnguloRotacao()),Sin(sin(AngRot)),Cos(cos(AngRot));

      P1.RotacionaPlano(Sin,Cos,AngRot);
      P2.RotacionaPlano(Sin,Cos,AngRot);
      P3.RotacionaPlano(Sin,Cos,AngRot);
    }

    ItSSuperficie ItSP1(Superficie.Find(P1,1e-3));
    if (ItSP1 != Superficie.pSuperficieAtual->end())
    {
      ItSSuperficie ItSP2(Superficie.Find(P2,1e-3));
      if (ItSP2 != Superficie.pSuperficieAtual->end())
      {
        ItSSuperficie ItSP3(Superficie.Find(P3,1e-3));
        if (ItSP3 != Superficie.pSuperficieAtual->end())
        {
          Matem::CalculaZ(ItSP1->PCentral,ItSP2->PCentral,ItSP3->PCentral,PSemCota);
        }
      }
    }
  }
}
