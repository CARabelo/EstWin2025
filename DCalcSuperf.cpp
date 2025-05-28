// DCalcSuperf.cpp : implementation file
//

#include "stdafx.h"
#include "Est.h"
#include "afxdialogex.h"
#include "CalcSU\CalcSU.h"
#include <set>
#include "cponto.h"
#include "CalcSU\CCCalcSU.h"
#include <set>
#include <list>
#include "afxtempl.h"
#include "supercstring.h"
#include "math.h"
#include "Estaca.h"
#include "carqIgualdades.h" 
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "classsetpontos.h"
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
#include "superficie.h"
#include "DCalcSuperf.h"
#include "carqsectipo.h"
#include "Perfil.h"
#include "CSecTipo.h"
#include "carqcurhorizontais.h" 
#include "CArqCanteiros.h"
#include "CCanteiros.h"
#include "ccurhorizontal.h"
#include "Mat.h"
#include "ccurhorizontais.h"
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include <iomanip>  
#include <iomanip>  
#include <strstream>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include "io.h"
#include "CConfigProj.h"
#include "HTTPRequests\HTTPRequests\CRequisicao.h"
#include "Monologo.h"

#define GRAVAR   2

static void GravaLog(const std::string& strMsg,std::ostream& Stream,bool ForcarHora=false);
static void GravaLogPontosProblema(SSetPontos& PonProblema,std::ostream& Stream,bool ForcarHora=false);
static void LeCoordSecoes(CString& NomeProjeto);
static void RotacionaSecoes(double Seno,double Coseno,double AngRotacao);
static void GravaCoordSecoes(CString& NomeProjeto);

extern class monologo monolog;

typedef std::list<Ponto> lstPontos;
typedef lstPontos::iterator itlstPontos;

typedef std::list<CEstaca> lstEstacas;
typedef lstEstacas::iterator itlstEstacas;

lstPontos LPSecoes;
lstEstacas LEstacas;

struct param     //--- Estrutura para a passagem dos parametros.
{
  CStatic* StaMensagem; 
  CProgressCtrl* ProProgresso;
  CDialog* CDial;
  CEvent* EventoFim;
  CString* CSProj;
  double DouEquidisCN;
  int ConfigSuper;
  std::string FormArqPontos;
  int TipoSuperf;
  int ImpTrianTopog;
};

struct param ParametrosGerarSuperficie;

UINT ThreadGerarSuperf(LPVOID pParam);

UINT ThreadGerarSuperf(LPVOID pParam)    //--- Thread calculo da superfície
{
  struct param* pParametros((struct param*) pParam);
  int Erro(0);
  std::stringstream Msg;
  SSetPontos PontosProblema;
  CSuperficie* pSuperficie(new CSuperficie(*pParametros->CSProj));    //---- Vai precisar usar 2 superficies, pra economizar memoria 

  std::ofstream ArqLog((*pParametros->CSProj + "_CalcSuperficie.rel"));

  if (pParametros->ImpTrianTopog == false)
  {
    if (ArqLog.good() == false) pParametros->StaMensagem->SetWindowText("Erro ao abrir o arquivo de log. Programa abortado.");
    else
    {
      GravaLog((*pParametros->CSProj).GetBuffer(), ArqLog);
      GravaLog("====> Verificando a superfície...", ArqLog);

      pParametros->EventoFim->ResetEvent();
      pParametros->StaMensagem->SetWindowText("Verificando a superfície...");

      Msg << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

      if (pSuperficie->PegaAnguloRotacao() != 0.0)
      {
        Msg.str("A superficie está rotacionada, desrotacione-a antes de solicitar o cálculo da terraplanagem.");
        AfxMessageBox(Msg.str().c_str(), MB_ICONEXCLAMATION);
        GravaLog(Msg.str(), ArqLog);
        Erro = 4;
      }
      else
      {
        ItCLLDeltaSup ItAciTopogProblema(pSuperficie->PegaListaRestricoes().end());

        pParametros->ProProgresso->SetPos(10);

        pSuperficie->CriaSetSegmentosRestricoes(CAciTopografico::FRONTEIRA);

        int Resposta(0);

        if (pSuperficie->VerfFronteira(PontosProblema, false, &Resposta) == false)
        {
          if (Resposta == 1)
          {
            Msg.str("");
            Msg << "A Fronteira não está fechada.";
            Msg << " Pontos de início e de fim da fronteira são:" << std::ends;
            GravaLog(Msg.str(), ArqLog);
            GravaLogPontosProblema(PontosProblema, ArqLog);
          }
          else
          {
            if (Resposta == 0)
            {
              Msg.str("");
              Msg << "A Fronteira não está definida.";
              Msg << " Não foram encontrados pontos definindo a fronteira" << std::ends;
              GravaLog(Msg.str(), ArqLog);
            }
            else
            {
              Msg.str("");
              Msg << "Foram encontrados " << Resposta << " segmentos definindo a fronteira.";
              Msg << " Seguem pontos de inicio e de fim de cada segmento:" << std::ends;
              GravaLog(Msg.str(), ArqLog);
              GravaLogPontosProblema(PontosProblema, ArqLog);
            }
          }

          Erro = 100;
        }
        else
        {
          if (pSuperficie->VerfAciTopogCruzFronteira(ItAciTopogProblema, PontosProblema))  /// agora esta verificando todos, não só a fronteira
          {
            if (PontosProblema.size() != 0)
            {
              Msg.str("");
              Msg << "Foram encontrados cruzamentos entre acidentes topográficos que geram arestas forçadas.";
              Msg << " Verifique próximo aos pontos:" << std::ends;
              GravaLog(Msg.str(), ArqLog);
              GravaLogPontosProblema(PontosProblema, ArqLog);
            }

            Erro = 1;
          }
          else
          {
            pParametros->ProProgresso->SetPos(25);

            PontosProblema.clear();
            int Resposta(0);
            pSuperficie->CriaSetPontosFronteira();

            if (pSuperficie->VerfPontosForaSuperficie(PontosProblema, &Resposta) == false)
            {
              Msg.str("");

              if (Resposta == 1) Msg << "Existem pontos fora da superfície:";
              else if (Resposta == 10) Msg << "A fronteira não está definida:";
              else if (Resposta == 11) Msg << " A fronteira não está fechada:";
              else Msg << "Há " << Resposta - 10 << " fronteiras definidas:";

              Msg << " Verifique próximo aos pontos:" << std::ends;
              GravaLog(Msg.str(), ArqLog);
              GravaLogPontosProblema(PontosProblema, ArqLog);

              Erro = 2;
            }
            else
            {
              PontosProblema.clear();

              pParametros->ProProgresso->SetPos(50);

              if (pSuperficie->VerfAciTopogForaSuperficie(PontosProblema))
              {
                if (PontosProblema.size() != 0)
                {
                  Msg.str("");
                  Msg << "Foram encontrados acidentes topográficos fora da superfície" << std::endl;
                  Msg << " Verifique próximo aos pontos:" << std::ends;
                  GravaLog(Msg.str(), ArqLog);
                  GravaLogPontosProblema(PontosProblema, ArqLog);

                  Erro = 3;
                }
              }
            }
          }
        }
      }

      Msg.str("");
      Msg << "Verificações finalizadas.";
      GravaLog(Msg.str(), ArqLog, true);

      delete pSuperficie;
    }
  }

  if (Erro == 0)
  {
    if (pParametros->ImpTrianTopog == false)
    {
      ArqLog << "Superficie OK, continuando..." << std::endl;

      pParametros->ProProgresso->SetPos(100);

      Msg.str("");
      if ((pParametros->ConfigSuper & 4) > 0) Msg << "Calculando a triangulação de Delaunay..." << std::ends;
      else Msg << "Calculando a triangulação de Delaunay-Gabriel..." << std::ends;

      pParametros->StaMensagem->SetWindowText(Msg.str().c_str());
      GravaLog(Msg.str(), ArqLog);

      DeleteFile(*pParametros->CSProj + "_PontosInseridos.xyz");

      int Res = CCalcSU::CalculaSU(*pParametros->CSProj, pParametros->StaMensagem, pParametros->ProProgresso, pParametros->ConfigSuper, pParametros->FormArqPontos, pParametros->TipoSuperf);

      if(Res !=0)
      {
        if(Res > 2)
        {
          Msg.str("");
          Msg << "A superfície não pode ser calculada com estes parametros, verifique o arquivo de log" << std::ends;
        }
        else
        {
          Msg.str("");
          Msg << "Houve erro na abertura do arquivo de acidentes topográficos, verifique o arquivo de log)" << std::ends;
        }
        
        AfxMessageBox(Msg.str().c_str());
        GravaLog(Msg.str(), ArqLog);
        
        return 1;
      }
      else
      {
        pParametros->ProProgresso->SetPos(75);

        if (pParametros->TipoSuperf == 0)
        {
          PontosProblema.clear();

          //--- CORRIGIR: Caso o usuário remova da caderneta de pontos um ponto que pertença a uma restrição este ponto
          //--- será reincluido na superfície pois as restriçoes sao lidas primeiro
          //--- Por isso a verificação abaixo nunca ocorrerá true

          if (0 && pSuperficie->VerfPontosAcidentesTopograficos(PontosProblema) == false)
          {
            Msg.str("");
            Msg << "Foram encontrados pontos nos acidentes topográficos que não pertencem mais à superficie." << std::endl;
            Msg << "Verifique próximo aos pontos:" << std::ends;
            GravaLog(Msg.str(), ArqLog);
            GravaLogPontosProblema(PontosProblema, ArqLog);

            //       return Erro = 4;
          }
          else
          {
            Msg.str("");
            Msg << "Triangulando a fronteira..." << std::ends;
            pParametros->StaMensagem->SetWindowText(Msg.str().c_str());
            GravaLog(Msg.str(), ArqLog);
          }
        }
      }
    }

    CSuperficie SuperficieNova(*pParametros->CSProj);

    if (SuperficieNova.DefineFronteiraOuPoligono(CSuperficie::TIPOFRONTEIRA) == TRUE)
    {
      Msg.str("");
      Msg << "Definindo as curvas de nível..." << std::ends;
      pParametros->StaMensagem->SetWindowText(Msg.str().c_str());
      GravaLog(Msg.str(), ArqLog);

      SuperficieNova.AtribuiEquidisCN(pParametros->DouEquidisCN);
      SuperficieNova.Serialize(*pParametros->CSProj, GRAVAR, false, false, false);

      if (SuperficieNova.InterpolarCotas(SuperficieNova.pSuperficieAtual->begin(), SuperficieNova.pSuperficieAtual->end(), *pParametros->ProProgresso, PontosProblema, pParametros->DouEquidisCN) == true)
      {
         //--- Desrotaciona as curvas horizontais

         //--- @ PRA QUE ISSO AQUI???? VERIFICAR

         ((CMainFrame*)AfxGetMainWnd())->LigaSilencioso();
         
         CSecTipo SecoesTipo(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()));
         CCurHorizontais CurvasHorizontais(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()), SecoesTipo);

         double AngRotacao(CurvasHorizontais.PegaAngRotacao());

         if (AngRotacao != 0.0)
         {
           CurvasHorizontais.RotacionaPlano(sin(-CurvasHorizontais.PegaAngRotacao()), cos(-CurvasHorizontais.PegaAngRotacao()), -CurvasHorizontais.PegaAngRotacao());
           CurvasHorizontais.Serialize(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()), GRAVAR);

           //--- Desrotaciona os pontos das seções
           //--- As coordenadas das seções estão sem classe, as funções abaixo foram copiadas de DesPontosGeometricoview

           LeCoordSecoes((CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual())));
           RotacionaSecoes(sin(-AngRotacao), cos(-AngRotacao), -AngRotacao);
           GravaCoordSecoes((CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual())));
         }

         //---@Esta simplificando D+ procurar outro algoritimo
        /// CSimpPL::SimplificaXYZ(SuperficieNova.LPontosCN);  //--- Dando pb VERIFICAR

         pParametros->StaMensagem->SetWindowText("Gravando os arquivos...");
         SuperficieNova.Serialize(*pParametros->CSProj, GRAVAR, false, true);

         Msg.str("");
         Msg << "A superfície foi gerada com êxito." << std::ends;
         pParametros->StaMensagem->SetWindowText(Msg.str().c_str());
         GravaLog(Msg.str(), ArqLog);

         pParametros->CDial->GetDlgItem(ID_MOK)->EnableWindow(true);
         pParametros->CDial->GetDlgItem(IDC_BUTPAUSAR)->EnableWindow(false);
         pParametros->CDial->GetDlgItem(IDC_BUTLOG)->EnableWindow(true);
         pParametros->EventoFim->SetEvent();

         ((CMainFrame*)AfxGetMainWnd())->CalculouSuperficie(true);

         return 0;   //--- retorno OK
      }
      else
      {
        Msg.str("");
        Msg << "Houve erro na definição das curvas de nível. Existem pontos com diferenças de cotas muito elevadas ( > 100.00m)" << std::endl;
        pParametros->StaMensagem->SetWindowText(Msg.str().c_str());
        Msg << "Verifique próximo aos pontos:" << std::ends;
        GravaLog(Msg.str(), ArqLog);
        GravaLogPontosProblema(PontosProblema, ArqLog);
      }
    }
    else
    {
      Msg.str("");
      Msg << "Houve erro na definição da Fronteira." << std::endl;
      pParametros->StaMensagem->SetWindowText(Msg.str().c_str());
      GravaLog(Msg.str(), ArqLog);
    }
  }
  else
  {
    Msg.str("");
    Msg << "Houve erro na verificação da superficie. A superficie Não foi gerada ou está inválida.";
    GravaLog(Msg.str(), ArqLog);
    GravaLog("====> Fim do arquivo ", ArqLog);
    GravaLog("", ArqLog, true);
    Msg << "Verifique o arquivo de log" << std::ends;
    pParametros->StaMensagem->SetWindowText(Msg.str().c_str());

    pParametros->CDial->GetDlgItem(IDC_BUTLOG)->EnableWindow(true);
    pParametros->EventoFim->SetEvent();
    pParametros->CDial->GetDlgItem(ID_MOK)->EnableWindow(true);
    pParametros->CDial->GetDlgItem(IDC_BUTPAUSAR)->EnableWindow(false);
  }
  return 1;   //--- Retorno NOK 
}
// CDCalcSuperf dialog

IMPLEMENT_DYNAMIC(CDCalcSuperf, CDialogEx)

CDCalcSuperf::CDCalcSuperf(const CString& pCSProjeto,CWnd* pParent,int* pTriaTopog)	: CDialogEx(CDCalcSuperf::IDD, pParent),CSProjeto(pCSProjeto), m_doEquidisCN(1.0),Evento(FALSE,TRUE),Terminou(false),T(NULL)
, bDelaunay(true),bForcarDelaunay(false),bGabriel(false), pTriangTopografia(pTriaTopog), TrImpTopog(false)
{
}

CDCalcSuperf::~CDCalcSuperf()
{
  WriteWindowPlacement();
}

void CDCalcSuperf::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_PROCALCSUPERF, ProCalcSuperf);
  DDX_Control(pDX, IDC_STAMSGCALCSUPERF, StaMsgCalcSuperf);
  DDX_Control(pDX, IDC_BUTCALCSUPERF, m_ButGerar);
  DDX_Control(pDX, IDC_BUTPAUSAR, m_ButPausar);
  DDX_Control(pDX, IDCANCEL, m_ButCancelar);
  DDX_Control(pDX, ID_MOK, m_ButOK);
  DDX_Text(pDX, IDC_EDIEQUIDIS, m_doEquidisCN);
}

BEGIN_MESSAGE_MAP(CDCalcSuperf, CDialogEx)
  ON_BN_CLICKED(IDC_BUTCALCSUPERF, &CDCalcSuperf::OnBnClickedButcalcsuperf)
  ON_BN_CLICKED(IDC_BUTPAUSAR, &CDCalcSuperf::OnBnClickedButpausar)
  ON_BN_CLICKED(IDCANCEL, &CDCalcSuperf::OnBnClickedCancel)
  ON_BN_CLICKED(IDC_RADGABRIEL, &CDCalcSuperf::OnBnClickedRadgabriel)
  ON_BN_CLICKED(IDC_RADDELAUNAY, &CDCalcSuperf::OnBnClickedRaddelaunay)
  ON_BN_CLICKED(IDC_BUTLOG, &CDCalcSuperf::OnClickedButlog)
  ON_BN_CLICKED(IDC_RADIMPTRIATOPOG, &CDCalcSuperf::OnBnClickedRadimptriatopog)
  ON_BN_CLICKED(ID_MOK, &CDCalcSuperf::OnBnClickedOK)
END_MESSAGE_MAP()

// CDCalcSuperf message handlers

void CDCalcSuperf::OnBnClickedButcalcsuperf()
{
  m_ButCancelar.EnableWindow(false);
  m_ButPausar.EnableWindow(true);
  m_ButOK.EnableWindow(false);
  m_ButGerar.EnableWindow(false);

  UpdateData(true);

  bDelaunay = (((CButton *) GetDlgItem(IDC_RADDELAUNAY))->GetCheck() == TRUE);
  bForcarDelaunay = (((CButton *) GetDlgItem(IDC_CHEFORCARDELAUNAY))->GetCheck() == TRUE);
  bGabriel = (((CButton *) GetDlgItem(IDC_RADGABRIEL))->GetCheck() == TRUE);
  TrImpTopog = (((CButton*)GetDlgItem(IDC_RADIMPTRIATOPOG))->GetCheck() == TRUE);

  if(pTriangTopografia) *pTriangTopografia = TrImpTopog;

  WriteWindowPlacement();

  CString NomProjBase(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase());
  CConfigProj ConfigProj(NULL,NomProjBase);
  ConfigProj.LeArquivo();

  char Buffer[512];

  ::GetPrivateProfileString(_T("DadosProjeto"), _T("TipoSuperf"), _T("0"), Buffer, 511, (NomProjBase + ".ini"));
  _stscanf(Buffer, "%d", &ParametrosGerarSuperficie.TipoSuperf);

  //--- Superficie Google (2) o cálculo é igual superficie tradicional (0)
  if (ParametrosGerarSuperficie.TipoSuperf == 2)
  {
    ParametrosGerarSuperficie.TipoSuperf = 0;
  }
  ParametrosGerarSuperficie.CSProj = &CSProjeto;
  ParametrosGerarSuperficie.StaMensagem = &StaMsgCalcSuperf;
  ParametrosGerarSuperficie.ProProgresso = &ProCalcSuperf;
  ParametrosGerarSuperficie.DouEquidisCN = m_doEquidisCN;
  ParametrosGerarSuperficie.CDial = this;
  ParametrosGerarSuperficie.EventoFim = &Evento;
  ParametrosGerarSuperficie.ConfigSuper = (bDelaunay << 2) + (bGabriel << 1) + bForcarDelaunay;
  ParametrosGerarSuperficie.FormArqPontos = ConfigProj.FormArqPontos;
  ParametrosGerarSuperficie.ImpTrianTopog = false; //TrImpTopog;

 // if (pTriangTopografia == nullptr || *pTriangTopografia == false)
  if(true)
  {
    try
    {
      T = AfxBeginThread(ThreadGerarSuperf, &ParametrosGerarSuperficie, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
      T->m_pMainWnd = AfxGetMainWnd();
      T->ResumeThread();             //---  Executa a tread.
    }
    catch (...)
    {
      SetDlgItemText(IDC_TEXTO, "Houve Erro ao gerar a superfície. A superfície NÃO foi gerada ou apresenta erros.");

      m_ButOK.EnableWindow(true);
      m_ButPausar.EnableWindow(false);
      m_ButCancelar.EnableWindow(false);

      UpdateData(false);

      return;
    }
  }

  ((CMainFrame*)AfxGetMainWnd())->SuperfCalculada = true;
  m_ButPausar.EnableWindow(true);
  m_ButCancelar.EnableWindow(false);
  m_ButOK.EnableWindow(true);
}

void CDCalcSuperf::OnBnClickedButpausar()
{
 static bool Estado(true);

 Estado = !Estado;

  if(Estado) 
  {
    m_ButPausar.SetWindowText("Pausar");
    m_ButCancelar.EnableWindow(FALSE);
    T->ResumeThread();    
  }
  else 
  {
    m_ButPausar.SetWindowText("Continuar");  
    m_ButCancelar.EnableWindow(TRUE);
    T->SuspendThread();   
  }

  WaitForSingleObject(CEvent(),200);
}

void CDCalcSuperf::OnBnClickedCancel()
{
  if(T) T->SuspendThread();

  CDialogEx::OnCancel();
}

BOOL CDCalcSuperf::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  ReadWindowPlacement();

  ((CButton *) GetDlgItem(IDC_CHEFORCARDELAUNAY))->EnableWindow(bDelaunay);
  ((CButton *) GetDlgItem(IDC_RADDELAUNAY))->SetCheck(bDelaunay);
  ((CButton *) GetDlgItem(IDC_RADGABRIEL))->SetCheck(bGabriel);
  ((CButton *) GetDlgItem(IDC_CHEFORCARDELAUNAY))->SetCheck(bForcarDelaunay);
  ((CButton *) GetDlgItem(IDC_RADIMPTRIATOPOG))->SetCheck(TrImpTopog);
  GetDlgItem(IDC_BUTLOG)->EnableWindow(false);

  UpdateData(false);

  if (TrImpTopog)
  {
    ((CButton*)GetDlgItem(IDC_BUTCALCSUPERF))->EnableWindow(false);
    ((CButton*)GetDlgItem(ID_MOK))->EnableWindow(true);
  }


  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDCalcSuperf::OnBnClickedRadgabriel()
{
  UpdateData(true);

  ((CButton *) GetDlgItem(IDC_CHEFORCARDELAUNAY))->SetCheck(false);
  ((CButton *) GetDlgItem(IDC_CHEFORCARDELAUNAY))->EnableWindow(false);
  ((CButton *) GetDlgItem(IDC_BUTCALCSUPERF))->EnableWindow(true);
  ((CButton *) GetDlgItem(ID_MOK))->EnableWindow(false);
}

void CDCalcSuperf::OnBnClickedRaddelaunay()
{
  UpdateData(true);
  ((CButton *) GetDlgItem(IDC_CHEFORCARDELAUNAY))->EnableWindow(true);
  ((CButton*)GetDlgItem(IDC_BUTCALCSUPERF))->EnableWindow(true);
  ((CButton*)GetDlgItem(ID_MOK))->EnableWindow(false);
}

void CDCalcSuperf::WriteWindowPlacement()
{
  CString Bloco("DiaCalcSuperf");            
  TCHAR szFormat[] = _T("%d %d"),szBuffer[100];

  wsprintf(szBuffer,szFormat,(int)(TrImpTopog << 3)+(bDelaunay << 2)+(bForcarDelaunay << 1)+(bGabriel),(int)(m_doEquidisCN*100.0));

  CString NomeProjeto(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual());

  ::WritePrivateProfileString(Bloco,"Valores",szBuffer,NomeProjeto + ".ini");
}

bool CDCalcSuperf::ReadWindowPlacement()
{
  CString Bloco("DiaCalcSuperf");            

  TCHAR szFormat[] = _T("%d%d");
  char Buffer[512]= {0};

  CString NomeProjeto(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual());

  ::GetPrivateProfileString(Bloco,"Valores","",Buffer,511,(NomeProjeto + ".ini")); 
  if (strlen(Buffer) == 0) return FALSE;

  int Valor(0),iEquidis(0);

  if(_stscanf(Buffer, szFormat,&Valor,&iEquidis) != 2) return false;

  m_doEquidisCN = iEquidis / 100.0;
  TrImpTopog = false;                    //---(Valor & 8) > 0;
  bDelaunay = (Valor & 4) > 0;
  bForcarDelaunay = (Valor & 2) > 0;
  bGabriel = (Valor & 1) > 0;

  return TRUE;
}

void GravaLog(const std::string& strMsg,std::ostream& Stream,bool ForcarHora)
{
  CTime Agora(CTime::GetCurrentTime());
  static CTime GravAnterior(0);
  std::strstream strstreamTemp;

  strstreamTemp << std::right << std::fixed << std::setfill('0') << std::setprecision(2);

  if (ForcarHora || CTimeSpan((Agora - GravAnterior)).GetTotalSeconds() > 30L)
  {
    strstreamTemp <<  "====> " << Agora.GetDay() << '/' << Agora.GetMonth() << '/' << Agora.GetYear() << ' ' << Agora.GetHour() << ':' << Agora.GetMinute() << ':' << Agora.GetSecond() << ' ' << std::ends;
    Stream << strstreamTemp.str();
    GravAnterior = Agora;
  }

  Stream << strMsg.c_str() << std::endl;
}

void GravaLogPontosProblema(SSetPontos& PonProblema,std::ostream& Stream,bool ForcarHora)
{
  Ponto PontoDesrotacionado;
  int C(1);

  for (ItSSetPontos itPontosProblema = PonProblema.begin() ; itPontosProblema != PonProblema.end() && C < 201 ; itPontosProblema++, C++)
  {
    std::stringstream Msg;

    Msg << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

    double Cota(itPontosProblema->z);

      Msg << C << ' ' << itPontosProblema->Nome << ", " << itPontosProblema->y << ", " << itPontosProblema->x;

    if(Cota != INFINITO)   Msg << ", " << Cota;

    Msg << std::ends;

    GravaLog(Msg.str(),Stream);
  }
}

void LeCoordSecoes(CString& NomeProjeto) //--- Duplicado do DesPontosGeometricoView -- As coordenadas da seção estão sem classe CORRIGIR
{
  CFile ArqCalcSec;
  CFileException e;
  int QSecoes(0);
  Ponto PontoACalc;

  if (ArqCalcSec.Open(NomeProjeto + CString(".pes"),CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    AfxMessageBox("Erro ao abrir o arquivo:\n" + NomeProjeto + CString(".pes")); 

    return;
  }
  else
  {
    CArchive CArqCalcSec(&ArqCalcSec,CArchive::load);
    Ponto PEsq,PDir,PEixo;
    CEstaca EstacaAtual;

    //--- Se estiver relendo remove os dados anteriores

    LPSecoes.clear();
    LEstacas.clear();

    //--- Busca coordenadas de todas as seções. colocando-os numa lista (os pontos de 3 em 3 já estão na ordem de estaca)
    try
    {
      for (CArqCalcSec >> QSecoes ; QSecoes ; QSecoes--)  
      {
        EstacaAtual << CArqCalcSec;
        PDir << CArqCalcSec;
        PEixo << CArqCalcSec;
        PEsq << CArqCalcSec;

        LEstacas.push_back(EstacaAtual);
     
        LPSecoes.push_back(PDir);
        LPSecoes.push_back(PEixo);
        LPSecoes.push_back(PEsq);
      }
    }
    catch(...)
    {
      AfxMessageBox("Erro ao gravar no arquivo:\n" + NomeProjeto + CString(".pes"));
    }

    CArqCalcSec.Close();
    ArqCalcSec.Close(); 
  }
}

void RotacionaSecoes(double Seno,double Coseno,double AngRotacao)  //--- Duplicado do DesPontosGeometricoView -- As coordenadas da seção estão sem classe CORRIGIR
{
  //--- rotaciona o mapa com os 3 pontos, ordenado por estaca e o mapa ordenado pelo ponto do eixo

  for (register itlstPontos it = LPSecoes.begin() ; it != LPSecoes.end() ; it++)  
    (*it).RotacionaPlano(Seno,Coseno,0.0);
}

void GravaCoordSecoes(CString& NomeProjeto) //--- Duplicado do DesPontosGeometricoView -- As coordenadas da seção estão sem classe CORRIGIR
{
  CFile ArqCalcSec;
  CFileException e;
  int QSecoes(0);
  Ponto PontoACalc;

  if (ArqCalcSec.Open(NomeProjeto + CString(".pes"),CFile::modeWrite | CFile::shareDenyNone,&e) == 0)
  {
    AfxMessageBox("Erro ao abrir o arquivo para gravação:\n" + NomeProjeto + CString(".pes"));
  }
  else
  {
    CArchive CArqCalcSec(&ArqCalcSec,CArchive::store);
    Ponto PEsq,PDir,PEixo;
    CEstaca EstacaAtual;

    itlstPontos itPSecoes(LPSecoes.begin());

    //--- Busca coordenadas de todas as seções. colocando-os numa lista (os pontos de 3 em 3 já estão na ordem de estaca)
    try
    {
      CArqCalcSec << (int) LEstacas.size();

      for(itlstEstacas itEstacas = LEstacas.begin() ; itEstacas != LEstacas.end() ; itEstacas++)
      {
        *itEstacas >> CArqCalcSec;

        for(int C = 0 ; itPSecoes != LPSecoes.end() && C < 3  ; C++)
        {
          *itPSecoes++ >> CArqCalcSec;
        }
      }
    }
    catch(...)
    {
      AfxMessageBox("Erro ao gravar no arquivo:\n" + NomeProjeto + CString(".pes"));
    }

    CArqCalcSec.Close();
    ArqCalcSec.Close(); 
  }
}

void CDCalcSuperf::OnClickedButlog()
{
  ((CMainFrame*)AfxGetMainWnd())->asdfg = IDD_REL_LOG_CALCSUPER; 
  ((CEstApp*)AfxGetApp())->AbreArqCriaDoc("_CalcSuperficie.rel",false,true);
}

void CDCalcSuperf::OnBnClickedRadimptriatopog()
{
  UpdateData(true);

  ((CButton*)GetDlgItem(IDC_CHEFORCARDELAUNAY))->SetCheck(false);
  ((CButton*)GetDlgItem(IDC_CHEFORCARDELAUNAY))->EnableWindow(false);
  ((CButton*)GetDlgItem(IDC_RADDELAUNAY))->SetCheck(false);
  ((CButton*)GetDlgItem(IDC_BUTCALCSUPERF))->EnableWindow(false);
  ((CButton*)GetDlgItem(ID_MOK))->EnableWindow(true);

  bDelaunay = bGabriel = false;
}

void CDCalcSuperf::OnBnClickedOK()
{
  UpdateData(true);

  TrImpTopog = (((CButton*)GetDlgItem(IDC_RADIMPTRIATOPOG))->GetCheck() == TRUE);

  if (TrImpTopog)
  {
    AfxMessageBox("Para importar a triangulação da topografia vá em projetos->Importar Arquivos da Topografia",MB_OK);	//---	Erro no	cálculo	da curva
  }

  WriteWindowPlacement();
  CDialog::OnOK();

}
