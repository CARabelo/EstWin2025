// DesPontosGeomView.cpp : implementation file
#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "cponto.h"
#include <string>
#include "mat.h"
#include "estaca.h"
#include "clpontos.h"
#include "perfil.h"
#include "clistaperfis.h"
#include <list>
#include <set>
#include <vector>
#include <algorithm>
#include "cdeltasuperficie.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include <tuple>
#include "CAlinhamento.h"
#include <map>
#include "CAlinhamentos.h"
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
//#include ".\cximage600_full\CxImage\ximage.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "ConvUTMGDC\ConvUTMGDC\Gdc_Coord_3d.h"
#include "ConvUTMGDC\ConvUTMGDC\Utm_Coord_3d.h"
#include "CSecoesTerrap.h"
#include "CCamadaDGN.h"
#include "CTextoMapaIBGE.h"
#include "CCNMapaIGBE.h"
#include "CMapaCNIBGE.h"
#include "CDiaImportarArqCtrlDrones.h"
#include "despontosgeomview.h"
#include "xymouse.h"
#include "xygreide.h"
#include "palette.h"
#include "ddeslizantes.h"
#include "deslizantesgeom.h"
#include "mytoolbar.h"
#include "comboprocdessecao.h"
#include "diapontosgeom.h"
#include "childfrm.h"
#include "CExcomboBox.h"
#include "MainFrm.h"
#include "dpontogeometrico.h"
#include "dconfdesgeom.h"
#include "deditacurhoriz.h"
#include "drotacionar.h"
#include <functional>   
#include <iomanip> 
#include <iostream>
#include <strstream>   
#include "editpopup.h"
#include "monologo.h"
#include "calcnota.h"
#include "carqtalcorte.h"
#include "carqtalaterro.h"
#include "ctalcorte.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include "carqgradientes.h"
#include "cgradientes.h"
#include "csecao.h"
#include "clistasecoes.h"
#include "listamedicoes.h"
#include "dvolumes.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
#include "carqpistasfixas.h"
#include "cpistasfixas.h"
#include "CTipoSolos.h"
#include "CArqTiposSolos.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include "CArqFurosSondagem.h"
#include "CPerfilGeologico.h"
#include "dessecaoview.h"
#include "CMEdit.h"
#include "dcoordenadas.h"
#include "dialogo.h"
#include "dcalcgeometrico.h"
#include "dcalcgeoseccdireto.h"
#include "dsecionar.h"
#include "DiaAbrirCriarArea.h"
#include "CMemDC.h"
#include "CDDEfineFronteira.h"
#include "CDPedeDescricao.h"
#include "CDiaLembretes.h"
#include "carqcurverticais.h"
#include "ccurverticais.h"
#include "CArqOAC.h"
#include "COAC.h"
#include "CArqOAE.h"
#include "COAE.h"
#include "CDEnsaiarGreide.h"
#include "DesPerfilView.h"
#include "CMensagem.h"
#include "CDDiaNomePerfAvulso.h"
#include "CDEditarSecAvulsa.h"
#include "CDiaImportarArqKmlKmz.h"
#include "CDiaAmpliarMapa.h"
#include "CDiaThread.h"
#include "io.h"
#include "CalcSU\CCCalcSU.h"
#include "CDAjustDeclinacao.h"
#include "CVetortGotaAgua.h"
#include "SimpPL\CSimpPL.h"
#include "CDSelecionarCNs.h"
#include "CDImportarArqSIG.h"
#include "CDiaIniciaTalvPrincipal.h"

//#include "projpsimpl/psimpl.h"
//#include "Projpsimpl/psimpl_reference.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define OPR_APAGAR 0
#define OPR_DESENHAR 1
#define TYP_BORDOS 1
#define TYP_ACDTOPOG 2 
#define ESQUERDO 0
#define DIREITO 1
#define RESTRICOES 1
#define BORDOS 0
#define ESPIRAL 0
#define CIRCULAR 1

#define LER 1
#define GRAVAR 2

#define VARRER_TERRENO 0
#define VARRER_PROJETO 0
#define VARRER_SECOES 1
#define VARRER_TUDO 1
#define OBJ_PI 1
#define OBJ_CC 2

#define CURVA_ESPIRAL 0
#define KEY_ESCAPE 27
#define KEY_DELETE 16
#define KEY_F5 116

#define MOSTRAR_NUMERO_PONTO 1
#define MOSTRAR_DESC 1 
#define MOSTRAR_COORDENADAS 1

#define FRENTE 1
#define TRAS 0

#define PONTO_FOZ_BACIA 1
#define PONTO_CG_BACIA 2

#define	WM_CLOSE_TOOLBAR (WM_APP+100)
#define EVENT_MOUSE_STOP 1000

extern class monologo monolog;
extern class dialogo dialogar;
extern class Matem Mat;
CDCoordenadas* pDCoordenadas = nullptr;

void inline TransladaPontoPlano(double* x, double* y, double NovaOrigemX, double NovaOrigemY)
{
  *x = *x - NovaOrigemX;
  *y = *y - NovaOrigemY;
}

void inline TransladaPontoPlano(double& x, double& y, double NovaOrigemX, double NovaOrigemY)
{
  x = x - NovaOrigemX;
  y = y - NovaOrigemY;
}

void inline RotacionaPontoPlano(double Sin, double Cos, double Angulo, double* x, double* y)
{
  //                   Matriz de rotação

  /*
  x’ = x . cos (fi)  -  y . sin(fi)

  y’ = x . sin (fi)  +  y . cos(fi)
  */

  double xTemp(*x * Cos - *y * Sin), yTemp(*x * Sin + *y * Cos);

  *x = xTemp;
  *y = yTemp;
}

/////////////////////////////////////////////////////////////////////////////
// DesPontosGeomView

IMPLEMENT_DYNCREATE(DesPontosGeomView, CView)

DesPontosGeomView::DesPontosGeomView(const std::string& pTrecho) :
  Trechos(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()).GetBuffer(), true),
  Superficie(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()), false),
  Arrastando(false),
  ArrastandoPI(false),
  ArrastandoCC(false),
  EZoom(false),
  CriandoRestricao(false),
  CriandoBordo(false),
  PAntMouse(10000, 10000),
  AlterouTerreno(false),
  AlterouProjeto(false),
  SecoesTipo(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual())),
  ItCurHorizSendoArrastadaPI(nullptr),
  ItCurHorizSendoArrastadaCC(nullptr),
  DPopUpPonto(NULL, RGB(255, 255, 166)),
  AnguloRotacao(Superficie.PegaAnguloRotacao()),
  ComSecoes(false),
  DeltaSupSendoArrastada(Superficie.pSuperficieAtual->end()),
  SecoesIconizadas(false),
  TerminarArrasto(false),
  AlterouAngulo(false),
  Seno(sin(AnguloRotacao)),
  Coseno(cos(AnguloRotacao)),
  Inicializou(false),
  Quadrante(floor(AnguloRotacao / Mat.M_PI_2)),
  PrimeiroMovimento(true),
  Destruindo(false),
  PegandoPontoSecAvulsa(false),
  PegouPrimPonto(false),
  SecoesAvulsas(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual())),
  AlterouSecoesAvulsas(false),
  DesCNTopograficas(true),
  DefBordos(false),
  Alargamentos(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual())),
  CursorAtual(IDC_HAND),
  DesenharObjetos(false),
  DesenhandoObjetos(false),
  EsbocandoObjetos(false),
  ObjetoAtual(0),
  ObjetoAnterior(0),
  CircularPontoIntersecao(false),
  FiltrarAciTopog(false),
  DesenharPontosInseridos(true),
  ItLLDeltaSupMarcada(Superficie.PegaListaRestricoes().end()),
  PegouPonto(false),
  DeltaSupSobMouseAnterior(Superficie.pSuperficieAtual->end()),
  InserindoPonto(false),
  PegouPontoDesabilitado(false),
  PegouRestricao(false),
  TrocandoObjeto(false),
  ItPontoDesabilitado(Superficie.SetPonDesabilitados().end()),
  InserindoLembrete(false),
  ItLembrete(SetLembretes.end()),
  MovendoLembrete(false),
  PermitirArrastar(true),
  SaltarCNs(0),
  DefinindoLinhaBase(0),
  m_nTimer(NENHUM_TIMER),
  MostrarRadar(false),
  Radar(10),
  RadarTodosPontos(false),
  RadarPontosInteresse(true),
  RadarRealcarPontosInteresse(false),
  strTrechoAtual(((CMainFrame*)AfxGetMainWnd())->PegaRuaAtual()),
  ProjetoAtual(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()),
  ArquivoAciTopograficos(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual())),
  ItSecaoAvulsaAtual(SecoesAvulsas.End()),
  MostrarAciTopogSuperficie(true),
  MostrarTerrap(false),
  DesenharAlinTerrap(false),
  DesenharOffSets(false),
  ZerarAzimute(false),
  AnguloRotacaoAnterior(0.0),
  VerificouCota(false),
  RazaoEscalasJPGDes(INFINITO),
  FaltaGeoReferenciamento(true),
  DesenharMapa(true),
  DeltaJPG(0.0),
  ProjetoGeoreferenciado(SUPERF_TRADICIONAL),
  SincPontosControle(false),
  NumPonCtrl(0),
  Medindo(0),
  MapaSolto(false),  //--- Usado somente em testes
  DesenharCroquis(false),
  pDesCroquisView(false),
  ItListaRestAtual(Superficie.PegaListaRestricoes().end()),
  UltPontoInserido(0),
  UltPontoEsbocado(0),
  ObjetosDrenagem(false),
  IniciouSarjeta(false),
  PegouEquipDrenagem(false),
  RemovendoDrenagem(false),
  RemovendoEspigao(false),
  ArrastandoEquipDrenagem(false),
  DefinindoObjDrenagem(false),
  TemArqTriaSuperf(false),
  DesenharEquipDrenagem(true),
  DesenharSimbolTerapl(false),
  DesenharCNTaludes(true),
  DesenharSecoesAvulsas(true),
  DensidadeSimbologia(4),
  ListaAtual(Superficie.PegaListaRestricoes().end()),
  TemMapa(false),
  Transladando(false),
  CotandoCNDGN(false),
  RealcarCNsMestras(true),
  EquidisCN(1),
  AlterouSuperficie(false),
  ImportouAciTopogAcadOK(false),
  IniciouTranslado(false),
  TransladandoPorTeclas(false),
  DesTodasCN(true),
  DesCNNaCota(false),
  DesCNAcimaCota(false),
  DesCNAbaixoCota(false),
  DesCNNaFaixa(false),
  AbaixoDaCota(-INFINITO),
  AcimaDaCota(INFINITO),
  JuntandoEspigoes(false),
  ItInicioJuntandoEspigoes(Superficie.pSuperficieAtual->end()),
  ItFimJuntandoEspigoes(Superficie.pSuperficieAtual->end()),
  ItLLEspigaoMarcado(Superficie.PegaListaRestricoes().end()),
  ProcurarEspigaoDoInicio(false),
  ItCNLigar(Superficie.SetItPontosCNs().end()), 
  ItPontoCNAtual(Superficie.SetItPontosCNs().end()),
  AlterouBacias(false),
  DefinindoTalveguePrincipal(false),
  ItBaciaMarcada(Superficie.PegaBacias().end()),
  ItListaIniTalvPrincipal(Superficie.PegaListaRestricoes().end())//,
{
  Escala[X] = Escala[Y] = 30.0;
  Escala[X] = Escala[Y] = 30.0;
  RazaoHV = 1.0;
  Deltas[X] = Deltas[Y] = 100.0;
  DeltasIniTranslacao[X] = DeltasIniTranslacao[Y]= 0.0;
  DeltaSupSobMouseAnterior = Superficie.pSuperficieAtual->end();

  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("0"), 10);
  LeCoordSecoes();               
  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("0"), 20);
  LeAcidentesTopograficos();
  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("0"), 30);
  LeCotasVermelhas();
  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("0"), 40);
  LePontosCotadosSuperf();
  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("0"), 45);
  Superficie.MontaChaves();
  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("0"), 50);
  Superficie.PegaListaBordos().clear();
  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("0"), 60);
  Superficie.CriaMSetNomes();
  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("0"), 70);
  Superficie.LeArqPontosSoltosDGN();
  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("0"), 80);
  Superficie.CriaSetCurvasNivel();
  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("0"), 90);

  Superficie.LeSetForcarAresta();

  ((CMainFrame*)AfxGetMainWnd())->DesPontosGeoComSecao = false;

  BandeiraBMP.LoadBitmap(IDB_BITMAPFLAG);
  BandeiraMaskBMP.LoadBitmap(IDB_FLAGMASK);

  LeLembretes();

  if (Quadrante == 4) Quadrante = 3;  //--- Ajuste de erro de arredondamento 

  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("0"), 95);

  int C(0);

  for (POSITION Pos = ArquivoAciTopograficos.GetHeadPosition(); Pos != NULL; ArquivoAciTopograficos.GetNext(Pos))
  {
    typairittysetRegAciTopograficosbool it = SetAciTopograficos.emplace(ArquivoAciTopograficos.GetAt(Pos));

    MapAciTopograficos.insert(pairIntittysetRegAciTopograficos(C++, it.first));
  }

  Projeto = ProjetoAtual.substr(0, ProjetoAtual.find_last_of('\\'));
  Projeto = Projeto.substr(0, Projeto.find_last_of('\\'));

  //--- Le este arquivo quando não há restriçoes na superficie de terraplanagem, sinal e que houve um erro
  //--- no cálculo da terraplanagem. Estes alinhamentos podem ajudar ao usuário a visualizar o erro.

  LeArqTPR(CString(ProjetoAtual.c_str()));            //--- Teraplanagem Projeto Restrições 

  GeraSecoesTerraplEstaca();                          //--- Para desenhar as sarjetas é preciso da terraplanagem ordenada por estaca

  for (ittymapCTrechos it(Trechos.MapaTrechos.begin()); it != Trechos.MapaTrechos.end(); it++)
  {
    it->second.SuperficieTerr.SerializarEquipDrenagem(LER, ((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual());
  }

  //--- Insere na superfície os pontos q foram inseridos nas seçoes
  //--- Se ja foram inseridos no probrema
  //--- Os pontos inseridos na seção devem se portar como pontos inseridos na superficie pois
  //--- podem ser usados nas fronteira restricoes etc
  //--- Incluí-los aqui agora é a forma mas facil de resolver o pb mas na proxima versão pensar melhor

  for (ItSSetPontos it = Superficie.SetPonInsSecoes().begin() ; it != Superficie.SetPonInsSecoes().end() ; it++)
  {
    Superficie.InsereDeltaSuperficie(*it);
  }

  CFile ArqInter;
  CFileException e;
  std::string NomeArquivo = "Intersecoes.xyz";

  //--- So para debug

  if (0)
  {
  if (ArqInter.Open(NomeArquivo.c_str(), CFile::modeRead, &e) == 0)
  {
      AfxMessageBox("Arquivo .xyz nao encontrado");
    return;
  }
  else
  {
    CArchive ar(&ArqInter, CArchive::load);
    size_t C;
    Ponto P1,P2;

    for( ar >> C ; C > 0 ; C--)
    {
      ar >> P1.x >> P1.y >> P1.z;
      ar >> P2.x >> P2.y >> P2.z;
      ittysetPontosSoltosCArestaDGN itArestaAtual(SetArestas.emplace(P1,P2).first);
      CArestaDGN* pArestaAtual(const_cast<CArestaDGN*>(&*itArestaAtual));
      ittylstlstUtm_Coord_3d itCN;
      size_t C2;
      for (ar >> C2 ; C2 > 0 ; C2--)
      {
        ar >> P1.x >> P1.y;
        pArestaAtual->InsereIntersecao(itCN,P1);
        }
      }
    }
  }
  LeSetForcarAresta();
  Superficie.AjustaPontosInseridosUsuario();
}

DesPontosGeomView::~DesPontosGeomView()
{
  CString CSDados;

  Destruindo = true;

  if (pDCoordenadas) delete(pDCoordenadas);
  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());
  MainFrame->DesPontosGeo = NULL;
  std::string NomeProjBase(Projeto),NomeArquivo;
  
  NomeArquivo = Projeto + Projeto.substr(Projeto.find_last_of('\\')) + std::string(".ini");
  
  SalvaLembretes();

  Ponto PDeltas(Deltas[X], Deltas[Y]);

  CSDados.Format("%lf,%lf,%lf,%lf,%lf,%lf,%lf,%u,%u,%u", Escala[X], Escala[Y], 1.0, PDeltas.x, PDeltas.y, AnguloRotacaoAnterior, RazaoEscalasJPGDes,UltPontoEsbocado,UltPontoInserido,EquidisCN);
  ::WritePrivateProfileString(_T("DadosDesenhos"),_T("DadosDesPontosGeo"), CSDados, NomeArquivo.c_str());

  CSDados.Format("%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i", DesenharPixel, DesenharCirculo,
    DesenharCruz, DesenharSecoes, DesenharArestas, DesenharBordos, DesenharRestricoes, DesenharPontos, MostrarDescricao, DesenharMalha, EquiMalha, EquiCotas, DesenharProjeto,
    bAciTopog, CircularSecoes, MostrarToolBarSuperf, MostrarToolBarGeom, CotarMalha, VarrerTerreno, VarrerProjeto, VarrerCN, DesCNTopograficas, DesenharCN, CotarCN, DefBordos,
    SincronizarSecoes, NaoPermitirAlteracoes, SecoesIconizadas, DesenharObjetos, DesenhandoObjetos, DesenharPontosInseridos, ObjetoAtual, PermitirArrastar, SaltarCNs, MostrarTerrap,
    DesenharAlinTerrap, DesenharOffSets, VerificouCota, PrimeiraVisualizacao, DesenharEquipDrenagem,DesenharSimbolTerapl,DesenharCNTaludes,DensidadeSimbologia,DesenharSecoesAvulsas,RealcarCNsMestras,
    ImportouAciTopogAcadOK, DefinindoTalveguePrincipal);

  MostrarTerrap = false;   //--- Melhor, em alguns casos não mostra nada...

  ::WritePrivateProfileString(_T("DadosDesenhos"), MainFrame->DesPerfil ? _T("DadosDesPontosGeoConfigComPerfil") : _T("DadosDesPontosGeoConfig"), CSDados, NomeArquivo.c_str());

  CSDados.Format("%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu", Cores[0], Cores[1], Cores[2], Cores[3], Cores[4], Cores[5], Cores[6], Cores[7], Cores[8], Cores[9]);
  ::WritePrivateProfileString(_T("DadosDesenhos"), MainFrame->DesPerfil ? _T("DadosDesPontosGeoCoresComPerfil") : _T("DadosDesPontosGeoCores"), CSDados, NomeArquivo.c_str());

  CSDados.Format("%i", MainFrame->DesPontosGeoComSecao ? true : false);
  ::WritePrivateProfileString(_T("DadosDesenhos"), _T("DesPontosComSecoes"), CSDados, NomeArquivo.c_str());
  MainFrame->DesPontosGeoComSecao = false;

  CSDados.Format("%i", MainFrame->DesPerfil ? true : false);
  ::WritePrivateProfileString(_T("DadosDesenhos"), _T("DesPontosComPerfil"), CSDados, NomeArquivo.c_str());

  CSDados.Format("%i,%i,%i,%i,%i,%lu", MostrarRadar, Radar.PegaRaio(), RadarTodosPontos, RadarPontosInteresse, RadarRealcarPontosInteresse, Radar.PegaCorTela());
  ::WritePrivateProfileString(_T("DadosDesenhos"), MainFrame->DesPerfil ? _T("DadosDesPontosGeoComPerfilRadar") : _T("DadosDesPontosGeoRadar"), CSDados, NomeArquivo.c_str());

  CSDados.Format("%i,%i,%i", 0/*MainFrame->TipoSuperficie*/, MainFrame->TrianImporTopografia, MainFrame->SuperfCalculada);

  ::WritePrivateProfileString(_T("DadosProjeto"), _T("TipoSuperf"), CSDados, NomeArquivo.c_str());

  //--- Dados do mapa

  NomeMapa = NomeMapa.Right(NomeMapa.GetLength() - NomeMapa.ReverseFind('\\') - 1);

 CSDados.Format("%lf,%lf,%lf,%lf,%lf,%i,%lf,%lf,%lf,%s", DeltasCN[X], DeltasCN[Y],DeltasCarta[X],DeltasCarta[Y],RazaoEscalasJPGDes, ProjetoGeoreferenciado, m_nRatio, m_nX, m_nY, NomeMapa.GetBuffer());
  ::WritePrivateProfileString(_T("DadosDesenhos"),_T("DadosMapa"), CSDados, NomeArquivo.c_str());

  CSDados.Format("%i,%i,%i,%i,%i,%i", FaltaGeoReferenciamento, DesenharMapa, MainFrame->TipoSuperficie, DesenharCroquis, MainFrame->FronteiraEditada,MainFrame->TrianImporTopografia);
  ::WritePrivateProfileString(_T("DadosDesenhos"), _T("DadosDesPontosGeoGeoRef"), CSDados, NomeArquivo.c_str());
}

BEGIN_MESSAGE_MAP(DesPontosGeomView, CView)
  ON_COMMAND(ID_ACITOPOG, OnAcitopog)
  ON_COMMAND(ID_ALTERNARTITULOGREIDE, OnAlternartituloDesGeom)
  ON_COMMAND(ID_BUTACITOPOG, OnDefAciTopog)
  ON_COMMAND(ID_BUTARRASTAR, &DesPontosGeomView::OnButarrastar)
  ON_COMMAND(ID_BUTBORDOS, OnBordos)
  ON_COMMAND(ID_BUTCNTOPOGRAF, OnTipoCN)
  ON_COMMAND(ID_BUTFECHar, OnButFechar)
  ON_COMMAND(ID_BUTFRONTEIRA, &DesPontosGeomView::OnButfronteira)
  ON_COMMAND(ID_BUTLEMBRETE, &DesPontosGeomView::OnButLembrete)
  ON_COMMAND(ID_BUTLOCK, OnNaoPermitirAlteracoes)
  ON_COMMAND(ID_BUTPONINS, &DesPontosGeomView::OnButponins)
  ON_COMMAND(ID_BUTRADAR, &DesPontosGeomView::OnButRadar)
  ON_COMMAND(ID_BUTROTACIONAR, OnButrotacionar)
  ON_COMMAND(ID_BUTSECAVULSA, OnSecaoAvulsa) 
  ON_COMMAND(ID_BUTSINCRO, OnSincronizarSecoes)
  ON_COMMAND(ID_CALCGEOMETRICO, OnCalcgeometrico)
  ON_COMMAND(ID_CONFDESPONTOSGEO, OnConfdes)
  ON_COMMAND(ID_DEFINEBORDOS, OnDefinebordos)
  ON_COMMAND(ID_BUTPROJTERRAP, &DesPontosGeomView::OnButProjTerrap)
  ON_COMMAND(ID_DES_ESPIGAO, &DesPontosGeomView::OnDesEspigao)
  ON_COMMAND(ID_DES_RAVINA, &DesPontosGeomView::OnDesRavina)
  ON_COMMAND(ID_DES_TALVEGUE, &DesPontosGeomView::OnDesTalvegue)
  ON_COMMAND(ID_DES_ALAGADO, &DesPontosGeomView::OnDesAlagado)
  ON_COMMAND(ID_DES_ALINHAMENTO, &DesPontosGeomView::OnDesAlinhamento)
  ON_COMMAND(ID_DES_ARVORE, &DesPontosGeomView::OnDesArvore)
  ON_COMMAND(ID_DES_BORDO, &DesPontosGeomView::OnDesBordo)
  ON_COMMAND(ID_DES_BUEIRO, &DesPontosGeomView::OnDesBueiro)
  ON_COMMAND(ID_DES_CANTO_CASA, &DesPontosGeomView::OnDesCantoCasa)
  ON_COMMAND(ID_DES_CANTO_CERCA, &DesPontosGeomView::OnDesCantoCerca)
  ON_COMMAND(ID_DES_CANTO_MURO, &DesPontosGeomView::OnDesCantoMuro)
  ON_COMMAND(ID_DES_CASA, &DesPontosGeomView::OnDesCasa)
  ON_COMMAND(ID_DES_CERCA, &DesPontosGeomView::OnDesCerca)
  ON_COMMAND(ID_DES_CRISTA, &DesPontosGeomView::OnDesCrista)
  ON_COMMAND(ID_DES_EIXO, &DesPontosGeomView::OnDeseixo)
  ON_COMMAND(ID_DES_FILTRAR, &DesPontosGeomView::OnDesFiltrar)
  ON_COMMAND(ID_DES_FLORESTA, &DesPontosGeomView::OnDesFloresta)
  ON_COMMAND(ID_DES_FRONTEIRA, &DesPontosGeomView::OnDesFronteira)
  ON_COMMAND(ID_DES_LINHAFERREA, &DesPontosGeomView::OnDesLinhaFerrea)
  ON_COMMAND(ID_DES_MATA, &DesPontosGeomView::OnDesMata)
  ON_COMMAND(ID_DES_MEIOFIO, &DesPontosGeomView::OnDesMeiofio)
  ON_COMMAND(ID_DES_MURO, &DesPontosGeomView::OnDesMuro)
  ON_COMMAND(ID_DES_NA, &DesPontosGeomView::OnDesNa)
  ON_COMMAND(ID_DES_NAPER, &DesPontosGeomView::OnNaPeriodico)
  ON_COMMAND(ID_DES_PE, &DesPontosGeomView::OnDesPe)
  ON_COMMAND(ID_DES_PLANTACAO, &DesPontosGeomView::OnDesPlantacao)
  ON_COMMAND(ID_DES_PONTE, &DesPontosGeomView::OnDesPonte)
  ON_COMMAND(ID_DES_POSTE, &DesPontosGeomView::OnDesPoste)
  ON_COMMAND(ID_DES_REDEELETRICA_AT, &DesPontosGeomView::OnDesRedeeletricaAt)
  ON_COMMAND(ID_DES_REDEELETRICA_BT, &DesPontosGeomView::OnDesRedeeletricaBt)
  ON_COMMAND(ID_DES_ROCHA, &DesPontosGeomView::OnDesRocha)
  ON_COMMAND(ID_DES_TAMPA_BUEIRO, &DesPontosGeomView::OnDesTampaBueiro)
  ON_COMMAND(ID_DES_PVELETRICA, &DesPontosGeomView::OnDesPVEletrica)
  ON_COMMAND(ID_DES_PVESGOTO, &DesPontosGeomView::OnDesPVEsgoto)
  ON_COMMAND(ID_DES_PVGAS, &DesPontosGeomView::OnDesPVGas)
  ON_COMMAND(ID_DES_REDEGAS, &DesPontosGeomView::OnDesRedeGas)
  ON_COMMAND(ID_DES_REDEELETRICA, &DesPontosGeomView::OnDesRedeEletrica)
  ON_COMMAND(ID_DES_REDEESGOTO, &DesPontosGeomView::OnDesRedeEsgoto)
  ON_COMMAND(ID_DES_SOLEIRA, &DesPontosGeomView::OnDesSoleira)
  ON_COMMAND(ID_DES_EQUI_BUEIROCIRC, &DesPontosGeomView::OnDesEquiBueiro)
  ON_COMMAND(ID_DES_EQUI_BUEIRORETANG, &DesPontosGeomView::OnDesEquiBueiroRetangular)
  ON_COMMAND(ID_DES_EQUI_SARJETAOFF, &DesPontosGeomView::OnDesEquiSarjetaOff)
  ON_COMMAND(ID_DES_EQUI_SARJETATN, &DesPontosGeomView::OnDesEquiSarjetaTN)
  ON_COMMAND(ID_DES_EQUI_CANALETATERRAP, &DesPontosGeomView::OnDesEquiCanaletaOff)
  ON_COMMAND(ID_DES_EQUI_CANALETA, &DesPontosGeomView::OnDesEquiCanaleta)
  ON_COMMAND(ID_DES_EQUI_DESCIDA_DAGUA, &DesPontosGeomView::OnDesDescidaDagua)
  ON_COMMAND(ID_DES_EQUI_CAIXA_COLETORA, &DesPontosGeomView::OnDesCaixaColetora)
  ON_COMMAND(ID_DES_EQUI_BACIA_DISSIPACAO, &DesPontosGeomView::OnDesBaciaAmortecimento)
  ON_COMMAND(ID_DES_EQUI_SARJETA, &DesPontosGeomView::OnDesEquiSarjeta)
  ON_COMMAND(ID_DES_EQUI_TUBULACAO, &DesPontosGeomView::OnDesEquiTubulacao)
  ON_COMMAND(ID_DES_EQUI_DRENO_PROFUNDO, &DesPontosGeomView::OnDesEquiDrenoProfundo)
  ON_COMMAND(ID_DES_MOVI_DRENAGEM, &DesPontosGeomView::OnDesMovimentarEquiDrenagem)
  ON_COMMAND(ID_DESENHAR, &DesPontosGeomView::OnDesenhar)
  ON_COMMAND(ID_ESBOCAR, &DesPontosGeomView::OnEsbocar)  
  ON_COMMAND(ID_DRENAGEM, &DesPontosGeomView::OnDrenagem)
  ON_COMMAND(ID_ENQUADRAR, OnEnquadrar)
  ON_COMMAND(ID_GRAVAR, OnGravar)
  ON_COMMAND(ID_IRPARAPONTO, OnIrparaponto)
  ON_COMMAND(ID_NA, &DesPontosGeomView::OnNa)
  ON_COMMAND(ID_POPUP_DESCONSIDERAR, &DesPontosGeomView::OnPopupDesabilitar)
  ON_COMMAND(ID_POPUP_HABILITAR, &DesPontosGeomView::OnPopupHabilitar)
  ON_COMMAND(ID_POPUP_INCLUIR_PONTO, &DesPontosGeomView::OnPopupIncluirPonto)
  ON_COMMAND(ID_POPUP_REMOVER, &DesPontosGeomView::OnPopupRemover)
  ON_COMMAND(ID_POPUP_REMOVER_PONTO, &DesPontosGeomView::OnPopupRemoverPonto)
  ON_COMMAND(ID_POPUP_SAIR, &DesPontosGeomView::OnPopupSair)
  ON_COMMAND(ID_POPUP_DRENAGEM_SAIR, &DesPontosGeomView::OnPopupDrenagemSair)
  ON_COMMAND(ID_POPUP_TROCAR, &DesPontosGeomView::OnPopupTrocar)
  ON_COMMAND(ID_POPUP_EDITAR, &DesPontosGeomView::OnPopupEditar)
  ON_COMMAND(ID_POPUP_EDIDRENAGEM, &DesPontosGeomView::OnPopupEditarDrenagem)
  ON_COMMAND(ID_POPUP_REMOVERDRENAGEM, &DesPontosGeomView::OnPopupRemoverDrenagem)
  ON_COMMAND(ID_FINALIZARADEFTALVPRINCIPAL, &DesPontosGeomView::OnPopupFinTalvPrincipal)
  ON_COMMAND(ID_REDESENHAR, OnRedesenhar)
  ON_COMMAND(ID_SECSEMPREVIS_ICONIZAR, OnSecsemprevisIconizar)
  ON_COMMAND(ID_BUTSINCPONTOSCONTROLE, OnSincPontosControle)
  ON_COMMAND(ID_BUTBAIXARMAPA, OnBaixarMapa)
  ON_COMMAND(ID_BUTREMOVERGEOREF, &DesPontosGeomView::OnRemovergeoreferenciamento)
  ON_COMMAND(ID_BUTAMPLIARMAPA, &DesPontosGeomView::OnButAmpliarMapa)
  ON_COMMAND(ID_BUTMEDIR, &DesPontosGeomView::OnButMedir)
  ON_COMMAND(ID_POPUP_AREA_PERIMETRO, &DesPontosGeomView::OnPopupAreaPerimetro)
  ON_COMMAND(ID_BUTVISCROQUIS, &DesPontosGeomView::OnButVisCroquis)
  ON_COMMAND(ID_COTARCN, &DesPontosGeomView::OnCotarCNs)
  ON_COMMAND(ID_BUTIMPACIAUTOCAD, &DesPontosGeomView::OnImportarAciAutoCad)
  ON_COMMAND(ID_BUTIMPBACIAHIDROG, &DesPontosGeomView::OnImportarBaciasHidrog)
  ON_COMMAND(ID_BUTIMPORTARTALVEGUES, &DesPontosGeomView::OnImportarTalvegues)
  ON_MESSAGE(WM_AMPLIAR_MAPA, &DesPontosGeomView::AmpliarMapa)     
  ON_MESSAGE(WM_CLOSE_TOOLBAR, DesPontosGeomView::FechouToolBar)
  ON_MESSAGE(WM_MOSTRAR_PONTOS_USUARIO, &DesPontosGeomView::MostraListaPontos)
  ON_MESSAGE(VUE_USRMSG_CLICOU_CROQUIS, &DesPontosGeomView::OnClicouCroquis)
  ON_UPDATE_COMMAND_UI(ID_BUTVISCROQUIS, OnUpdateButVisCroquis)
  ON_UPDATE_COMMAND_UI(ID_BUTROTACIONAR, OnUpdateButrotacionar)
  ON_UPDATE_COMMAND_UI(ID_BUTSINCRO, OnUpdateButSincro)
  ON_UPDATE_COMMAND_UI(ID_CALCGEOMETRICO, &DesPontosGeomView::OnUpdateCalcgeometrico)
  ON_UPDATE_COMMAND_UI(ID_DES_FRONTEIRA, &DesPontosGeomView::OnUpdateDesFronteira)
  ON_UPDATE_COMMAND_UI(ID_ENQUADRAR, OnUpdateEnquadrar)
  ON_UPDATE_COMMAND_UI(ID_GRAVAR, OnUpdateGravar)
  ON_UPDATE_COMMAND_UI(ID_POPUP_DESCONSIDERAR, &DesPontosGeomView::OnUpdatePopupDesabilitar)
  ON_UPDATE_COMMAND_UI(ID_POPUP_HABILITAR, &DesPontosGeomView::OnUpdatePopupHabilitar)
  ON_UPDATE_COMMAND_UI(ID_POPUP_REMOVER_PONTO, &DesPontosGeomView::OnUpdatePopupRemoverPonto)
  ON_UPDATE_COMMAND_UI(ID_POPUP_EDITAR, &DesPontosGeomView::OnUpdatePopupEditar)
  ON_UPDATE_COMMAND_UI(ID_REDESENHAR, OnUpdateRedesenhar)
  ON_UPDATE_COMMAND_UI(ID_BUTPROJTERRAP, OnUpdateButProjTerrap)
  ON_UPDATE_COMMAND_UI(ID_BUTSINCPONTOSCONTROLE, OnUpdateButSinPontosCtrl)
  ON_UPDATE_COMMAND_UI(ID_BUTBAIXARMAPA, OnUpdateBaixarMapa)
  ON_UPDATE_COMMAND_UI(ID_BUTAMPLIARMAPA, OnUpdateButAmpliarMapa)
  ON_UPDATE_COMMAND_UI(ID_BUTREMOVERGEOREF, OnUpdateButRemoverGeoRef)
  ON_UPDATE_COMMAND_UI(ID_BUTMEDIR, OnUpdateButMedir)
  ON_UPDATE_COMMAND_UI(ID_BUTRADAR, OnUpdateButradar)
  ON_UPDATE_COMMAND_UI(ID_BUTPONINS, OnUpdateButPonIns)
  ON_UPDATE_COMMAND_UI(ID_BUTSECAVULSA, OnUpdateButSecAvulsa)
  ON_UPDATE_COMMAND_UI(ID_POPUP_REMOVER, &DesPontosGeomView::OnUpdatePopupRemover)
  ON_UPDATE_COMMAND_UI(ID_POPUP_TROCAR, &DesPontosGeomView::OnUpdatePopupRemover)
  ON_UPDATE_COMMAND_UI(ID_COTARCN, &DesPontosGeomView::OnUpdateCotarCNs)
  ON_UPDATE_COMMAND_UI(ID_BUTIMPACIAUTOCAD, &DesPontosGeomView::OnUpdateImportarAciAutoCad)
  ON_UPDATE_COMMAND_UI(ID_DESENHAR, OnUpdateButAciTopog)
  ON_UPDATE_COMMAND_UI(ID_ESBOCAR, OnUpdateButEsbocar)
  ON_UPDATE_COMMAND_UI(ID_DESENHAR, OnUpdateButFronteira)
  ON_UPDATE_COMMAND_UI(ID_BUTIMPBACIAHIDROG, OnUpdateImportarBaciasHidrog)
  ON_UPDATE_COMMAND_UI(ID_BUTIMPORTARTALVEGUES, OnUpdateImportarTalvegues)

  ON_WM_CHAR()
  ON_WM_CLOSE()
  ON_WM_CREATE()
  ON_WM_ERASEBKGND()
  ON_WM_KEYUP()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_LBUTTONDOWN()
  ON_WM_MBUTTONDBLCLK()
  ON_WM_MOUSEMOVE()
  ON_WM_MOUSEWHEEL()
  ON_WM_RBUTTONDBLCLK()
  ON_WM_RBUTTONDOWN()
  ON_WM_RBUTTONUP()
  ON_WM_SETFOCUS()
  ON_WM_SIZE()
  ON_WM_SIZING()
  ON_WM_LBUTTONUP()
  ON_WM_TIMER()
  ON_WM_SHOWWINDOW()
  ON_WM_ACTIVATE()
  ON_WM_SHOWWINDOW()
  ON_COMMAND(ID_POPUP_INSERIR_PONTO, &DesPontosGeomView::OnPopupInserirPonto)
  ON_UPDATE_COMMAND_UI(ID_POPUP_INSERIR_PONTO, &DesPontosGeomView::OnUpdatePopupInserirPonto)
  ON_COMMAND(ID_POPUP_SAIR, &DesPontosGeomView::OnPopupSair)
  //ON_COMMAND(ID_POPUP_AREA_PERIMETRO, &DesPontosGeomView::OnPopupAreaPerimetro)
  ON_COMMAND(ID_POPUPAREAPERIM, &DesPontosGeomView::OnPopupAreaPerimetroAci)
  ON_UPDATE_COMMAND_UI(ID_POPUPAREAPERIM, &DesPontosGeomView::OnUpdatePopupAreaPerimAci)
  ON_COMMAND(ID_POPUPDEFINIRBACIA, &DesPontosGeomView::OnPopupDefinirBacia)
  ON_COMMAND(ID_POPUPINCLUIRTALVPRINCIPAL, &DesPontosGeomView::OnPopupIncTalvPrincipal)
  ON_COMMAND(ID_POPUPEXCLUIRTALVPRINCIPAL,&DesPontosGeomView::OnPopupExcTalvPrincipal)
  ON_UPDATE_COMMAND_UI(ID_POPUPINCLUIRTALVPRINCIPAL, &DesPontosGeomView::OnUpdatePopupIncTalvPrincipal)
  ON_UPDATE_COMMAND_UI(ID_POPUPEXCLUIRTALVPRINCIPAL, &DesPontosGeomView::OnUpdatePopupExcTalvPrincipal)
  ON_UPDATE_COMMAND_UI(ID_POPUPDEFINIRBACIA, &DesPontosGeomView::OnUpdatePopupDefinirBacia)
  ON_UPDATE_COMMAND_UI(ID_MENUCOTARCN, &DesPontosGeomView::OnUpdateMenucotarcn)
  ON_COMMAND(ID_POPUP_COTARCN, &DesPontosGeomView::OnPopupCotarcn)
  ON_COMMAND(ID_POPUP_SAIRCN, &DesPontosGeomView::OnPopupSaircn)
  ON_COMMAND(ID_POPUP_SAIRPONTO, &DesPontosGeomView::OnPopupPontoSair)
  ON_COMMAND(ID_POPUP_DESCPONTO, &DesPontosGeomView::OnPopupDescPonto)
  ON_UPDATE_COMMAND_UI(ID_DRENAGEM, &DesPontosGeomView::OnUpdateOnDrenagem)
  ON_COMMAND(ID_SELECIONARCNS, &DesPontosGeomView::OnSelecionarCNs)
  ON_COMMAND(ID_MENUREMOVERBACIAHIDROL, &DesPontosGeomView::OnMenuremoverbaciahidrol)
  ON_COMMAND(ID_MENUEDITARBACIAHIDROL, &DesPontosGeomView::OnMenueditarbaciahidrol)
  ON_COMMAND(ID_MENUREMOVERESPIGAO, &DesPontosGeomView::OnMenuremoverespigao)
  ON_UPDATE_COMMAND_UI(ID_MENUREMOVERBACIAHIDROL, &DesPontosGeomView::OnUpdateMenuremoverbaciahidrol)
  ON_COMMAND(ID_MENUBACIASAIR, &DesPontosGeomView::OnMenubaciasair)
  ON_COMMAND(ID_POPUP_CRIARTALVPRINCIPAL, &DesPontosGeomView::OnPopupCriarTalvPrincipal)
  ON_UPDATE_COMMAND_UI(ID_POPUP_CRIARTALVPRINCIPAL, &DesPontosGeomView::OnUpdatePopupCriarTalvPrincipal)
  END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DesPontosGeomView drawing

void DesPontosGeomView::OnDraw(CDC* ppDC)
{
  //--- Tive que colocar esta atribuição aqui porque no onsetfocus não esta 
  //--- funcionando parece que o  destrutor da janela anterior esta sendo 
  //--- chamado depois do onsetfocus da proxima janela

  ((CMainFrame*)AfxGetMainWnd())->DesPontosGeo = this;

  CMMemDC pDC(ppDC);

  if (DesenharMapa)   //--- O georeferenciamento foi removido
  {
    pDC->SetBkMode(TRANSPARENT);
    m_cPicture.Draw(pDC, CPoint(m_nX, m_nY), m_nRatio);
      //m_cPicture.Draw(pDC, CPoint(0,0), m_nRatio);

    //    Mapa.Draw(pDC, (long)m_nX, (long)m_nY);
  }

  if(0)
  {
   CFont m_FontCourier15;

    m_FontCourier15.CreatePointFont(120, _T("LucidaConsole"), pDC);

    pDC->SelectObject(&m_FontCourier15);
    pDC->SetTextAlign(TA_BOTTOM | TA_LEFT);
    pDC->SetTextColor(0);
    pDC->SetBkColor(RGB(255, 255, 255));
    {
      std::stringstream strTexto;
      strTexto << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

      strTexto << " DeltamapaX= " << DeltasCN[X] 
               << " DeltamapaY= " << DeltasCN[Y] << std::endl << std::ends;
      pDC->TextOut(100, 100, strTexto.str().c_str());

    }
    {
      std::stringstream strTexto;
      strTexto << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

      strTexto << " m_nX= " << m_nX
               << " m_nY= " << m_nY << std::endl << std::ends;
      pDC->TextOut(100, 130, strTexto.str().c_str());

      strTexto.str("");
      strTexto.seekg(0, strTexto.beg);
      strTexto << "Escala= " << Escala[X];
      pDC->TextOut(100, 190, strTexto.str().c_str());

      strTexto.str("");
      strTexto.seekg(0, strTexto.beg);
      strTexto << " DEltasCartaX= " << DeltasCarta[X]
               << " DeltasCartaY= " << DeltasCarta[Y];
      pDC->TextOut(100, 210, strTexto.str().c_str());

      strTexto.str("");
      strTexto.seekg(0, strTexto.beg);
      strTexto << " DEltasX= " << Deltas[X]
               << " DeltasY= " << Deltas[Y];
      pDC->TextOut(100, 240, strTexto.str().c_str());

      strTexto.str("");
      strTexto.seekg(0, strTexto.beg);
      strTexto << " InicioArrastX= " << InicioArrast.x
               << " InicioArrastY= " << InicioArrast.y;
      pDC->TextOut(100, 270, strTexto.str().c_str());

      strTexto.str("");
      strTexto.seekg(0, strTexto.beg);
      strTexto << " InicioTransX= " << DeltasIniTranslacao[X]
               << " InicioTransY= " << DeltasIniTranslacao[Y];
      pDC->TextOut(100, 300, strTexto.str().c_str());

      strTexto.str("");
      strTexto.seekg(0, strTexto.beg);
      strTexto << " IniciouTranslado= " << IniciouTranslado
               << " Arrastando= " << Arrastando
               << " TerminarArrast= " << TerminarArrasto
               << "Transladando= " << Transladando;
      pDC->TextOut(100, 330, strTexto.str().c_str());
    }
  }

  if (SincPontosControle)
  {
    CFont m_FontCourier15;

    m_FontCourier15.CreatePointFont(150, _T("LucidaConsole"), pDC);

    pDC->SelectObject(&m_FontCourier15);
    pDC->SetTextAlign(TA_BOTTOM | TA_LEFT);
    pDC->SetTextColor(0);
    pDC->SetBkColor(RGB(255, 255, 255));

    std::strstream Dados;
    Dados << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);
    Dados << " PCTRL : " << NumPonCtrl + 1 << ' ' << std::ends;
    pDC->TextOut(20, 40, Dados.str());

    m_FontCourier15.DeleteObject();

    return;
  }

  if (AlterouAngulo)
  {
    OnAlterouAngulo(pDC);
    AlterouAngulo = false;
  }

  if (MostrarRadar) Radar.Draw(pDC, PonMousePixelsAtual.x, PonMousePixelsAtual.y);

  DesenhaNorte(pDC);

  CPen* pPenaAnterior, PenaZoom;

  PenaZoom.CreatePen(PS_DASH, 2, Cores[CORMALHA]);
  pPenaAnterior = pDC->SelectObject(&PenaZoom);

  if (EZoom)
  {
    CRect RectZoom(InicioZoom, FimZoom);
    CBrush brushBack(Cores[CORFUNDO]);
    CBrush* pOldBrush(pDC->SelectObject(&brushBack));
    pDC->Rectangle(&RectZoom);

    pDC->SelectObject(pOldBrush);
  }

  CDC dcMemory;
  dcMemory.CreateCompatibleDC(pDC);

  for (ittysetLembretes itLembretes = SetLembretes.begin(); itLembretes != SetLembretes.end(); ++itLembretes)
  {
    if (!MovendoLembrete || ItLembrete != itLembretes)
    {
      Ponto Ppixels((itLembretes->Local().x - Deltas[X]) * Escala[X], (AlturaVideo - (itLembretes->Local().y - Deltas[Y])) * Escala[Y]);

      dcMemory.SelectObject(BandeiraMaskBMP);
      pDC->BitBlt(Ppixels.x, Ppixels.y, 16, 21, &dcMemory, 0, 0, SRCAND);
      dcMemory.SelectObject(&BandeiraBMP);
      pDC->BitBlt(Ppixels.x, Ppixels.y, 16, 21, &dcMemory, 0, 0, SRCPAINT);
    }
  }

  if (ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end())  //--- Restrição marcada 
  {
    CPen PenaRemover;
    PenaRemover.CreatePen(PS_SOLID, 4, RGB(190, 190, 190));
    register ItCLDeltaSup ItLRestricao(ItLLDeltaSupMarcada->begin());

    pDC->SelectObject(PenaRemover);

    pDC->MoveTo(((*ItLRestricao).PCentral.x - Deltas[X]) * Escala[X], (AlturaVideo - ((*ItLRestricao).PCentral.y - Deltas[Y])) * Escala[Y]);

    while (++ItLRestricao != ItLLDeltaSupMarcada->end())
    {
      Ponto PAtual = (*ItLRestricao).PCentral;   //--- Pega o ponto atual
      Ponto Ppixels;

      Ppixels.x = ((PAtual.x - Deltas[X]) * Escala[X]);
      Ppixels.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y];

      pDC->LineTo(Ppixels.x, Ppixels.y);
    }
  }
  /*
  if (ItEquiDrenagemMarcada != Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end()) 
  {
    DesenhaAciDrenagemMarcada(pDC);
  }
  */
 
  if (DesenharProjeto)
  {
    CPen PenaProjeto;
    PenaProjeto.CreatePen(PS_SOLID, 2, Cores[CORPROJETO]);
    pDC->SelectObject(&PenaProjeto);

    if(Trechos.TemTrechoAtual()) DesenhaProjeto(Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS.begin(), pDC);
  }

  Ponto PAtual, Ppixels;
  CPen PenaRestricoes, PenaCotaMestra;

  PenaRestricoes.CreatePen(PS_SOLID, 2, Cores[CORCRISTASPES]);
  PenaCotaMestra.CreatePen(PS_SOLID, 2, Cores[CORCN]);

  //--- Se não é para desenhar a terraplanagem é pra desenhar a superficie

  if (!MostrarTerrap)
  {
    DesenhaSuperficie(Superficie.pSuperficieAtual->begin(), Superficie.pSuperficieAtual->end(), pDC);
    DesenhaRestricoes(&Superficie, pDC, &PenaRestricoes, SUPERF_TERRENO_NATURAL);    //--- Desenha as restrições do TN
  }
  else
  {
    DesenhaSuperficie(Trechos.PegaTrechoAtual()->second.SuperficieTerr.pSuperficieAtual->begin(), Trechos.PegaTrechoAtual()->second.SuperficieTerr.pSuperficieAtual->end(), pDC, &Trechos.PegaTrechoAtual()->second.SuperficieTerr);
    DesenhaRestricoes(&Trechos.PegaTrechoAtual()->second.SuperficieTerr, pDC, &PenaRestricoes, SUPERF_TERRAPLANAGEM);  //--- Desenha as restrições da terraplanagem 
    DesenhaRestricoes(&Superficie, pDC, &PenaRestricoes, SUPERF_TERRENO_NATURAL,true);    //--- Desenha as restrições do TN para a terraplanagem ( sem pontos inseridos) (ultimo true)
  }

  CPen PenaPontoInserido;
  PenaPontoInserido.CreatePen(PS_SOLID, 1, RGB(200, 0, 0));
  pDC->SelectObject(&PenaPontoInserido);

  //--- Desenha por cima, em vermelho, os pontos inseridos pelo usuário

  if (DesenharPontos)
  {
    for (ItSSetPontos it = Superficie.SetPonInseridos().begin(); it != Superficie.SetPonInseridos().end(); ++it)
    {
      Ponto Ppixels;

      Ppixels.x = ((it->x - Deltas[X]) * Escala[X]);
      Ppixels.y = (AlturaVideo - (it->y - Deltas[Y])) * Escala[Y];

      pDC->Arc(Ppixels.x - 3, Ppixels.y - 3, Ppixels.x + 3, Ppixels.y + 3, 0, 0, 0, 0);
    }

    CPen PenaPontoDesabilitado;
    PenaPontoDesabilitado.CreatePen(PS_SOLID, 1, RGB(127, 127, 127));
    pDC->SelectObject(&PenaPontoDesabilitado);

    //--- Desenha por cima, em cinza, os pontos desabilitados pelo usuário

    for (ItSSetPontos it = Superficie.SetPonDesabilitados().begin(); it != Superficie.SetPonDesabilitados().end(); ++it)
    {
      Ponto Ppixels;

      Ppixels.x = (it->x - Deltas[X]) * Escala[X];
      Ppixels.y = (AlturaVideo - (it->y - Deltas[Y])) * Escala[Y];

      pDC->Arc(Ppixels.x - 3, Ppixels.y - 3, Ppixels.x + 3, Ppixels.y + 3, 0, 0, 0, 0);
    }
  }

  CPen PenaMalha;

  PenaMalha.CreatePen(PS_SOLID, 1, Cores[CORMALHA]);
  pDC->SelectObject(&PenaMalha);

  if (DesenharMalha && Superficie.pSuperficieAtual->size()) DesenhaMalha(pDC);

  pDC->SelectObject(&CurrierNewHorizontal);
  pDC->SetTextAlign(TA_BOTTOM | TA_LEFT);
  pDC->SetTextColor(Cores[CORTEXTO]);
  pDC->SetBkColor(Cores[CORFUNDO]);

  if(0)
  {
    std::stringstream strstrMsg;

    strstrMsg << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

    strstrMsg << "m_mx: " << m_nX << "  m_nY: " << m_nY;
    pDC->TextOut(50, 50, strstrMsg.str().c_str());

    strstrMsg.str("");
    strstrMsg << "DeltaCNX: " << DeltasCN[X] << "   DeltaCNY: " << DeltasCN[Y];
    pDC->TextOut(50, 70, strstrMsg.str().c_str());

    strstrMsg.str("");
    strstrMsg << "DeltaX: " << Deltas[X] << "   DeltaY: " << Deltas[Y];
    pDC->TextOut(50, 90, strstrMsg.str().c_str());
  }

  // CPen PenaBordos,PenaRestricoes,PenaCotaMestra;                     

  //  PenaBordos.CreatePen(PS_SOLID,2,Cores[CORBORDOS]);
  //  PenaRestricoes.CreatePen(PS_SOLID,2,Cores[CORCRISTASPES]);
  //  PenaCotaMestra.CreatePen(PS_SOLID,2,Cores[CORCN]);

  if (CircularPontoIntersecao)
  {
    pDC->SelectObject(&PenaCotaMestra);
    Ppixels.x = (PontoIntersecao.x - Deltas[X]) * Escala[X];
    Ppixels.y = (AlturaVideo - (PontoIntersecao.y - Deltas[Y])) * Escala[Y];

    pDC->Arc(Ppixels.x - 9, Ppixels.y - 9, Ppixels.x + 9, Ppixels.y + 9, 0, 0, 0, 0);
    pDC->Arc(Ppixels.x - 5, Ppixels.y - 5, Ppixels.x + 5, Ppixels.y + 5, 0, 0, 0, 0);
  }

  //--- Desenha os objetos
  //--- Esta vazia por enquanto até passar os bordos => acidentes topograficos
  //--- Eu passei os bordos para as restricoes. alterar===> passar tudo para acidentes topograficos

  pDC->SelectObject(&PenaRestricoes);

  const mapAciTipog& MapaAciTopograficos(AcidentesTopograficos.PegaMapAcidentesTopograficos());

  if (0) for (register citmapAciTipog ItAciTopogAtual = MapaAciTopograficos.begin(); ItAciTopogAtual != MapaAciTopograficos.end(); ++ItAciTopogAtual)
  {
    const lstitsetPontos& LstItSetObjetos(ItAciTopogAtual->second.PegaLista());

    if (LstItSetObjetos.size())
    {
      register citlstitsetPontos it(LstItSetObjetos.begin());

      pDC->MoveTo(((*it)->x - Deltas[X]) * Escala[X], (AlturaVideo - ((*it)->y - Deltas[Y])) * Escala[Y]);

      while (it != LstItSetObjetos.end())
      {
        Ppixels.x = (((**it).x - Deltas[X]) * Escala[X]);
        Ppixels.y = (AlturaVideo - ((**it).y - Deltas[Y])) * Escala[Y] ;

        pDC->LineTo(Ppixels.x, Ppixels.y);

        ++it;
      }
    }
  }

  //--- Desenha o objeto que está sendo desenhado (ainda não esta no mapa de objetos)
  {
    const lstitsetPontos& LstItSetObjetos(AciTopogAtual.PegaLista());

    if (LstItSetObjetos.size())
    {
      register citlstitsetPontos it(LstItSetObjetos.begin());

      pDC->MoveTo(((*it)->x - Deltas[X]) * Escala[X], (AlturaVideo - ((*it)->y - Deltas[Y])) * Escala[Y]);

      while (it != LstItSetObjetos.end())
      {
        Ppixels.x = (((**it).x - Deltas[X]) * Escala[X]);
        Ppixels.y = (AlturaVideo - ((**it).y - Deltas[Y])) * Escala[Y];

        pDC->LineTo(Ppixels.x, Ppixels.y);

        ++it;
      }
    }
  }

  //--- Desenha as curvas de nivel

  double MultCotaMestra(Superficie.PegaEquidisCN() * 5.0);
  CPen PenaCNNormal;
  PenaCNNormal.CreatePen(PS_SOLID, 1, Cores[CORCN]);

  for (int C = 0; C < 2; C++)
  {
    if (C == 0)
    {
      if (!MostrarTerrap) DesenhaCN(&Superficie, pDC);
    }
    else
    {
      if (MostrarTerrap)
      {
        for (ittymapCTrechos it = Trechos.MapaTrechos.begin(); it != Trechos.MapaTrechos.end(); it++)
        {
          //--- Só pode mostrar a terraplanagem de um trecho porque es CN de outro trecho superpoem a do
          //--- Trecho atual

          if (it->first.compare((((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual())) == 0)
          {
            DesenhaCN(&it->second.SuperficieTerr, pDC, DesenharSimbolTerapl);
          }
        }
      }
    }
  }

  //--- Desenha as seções

  CPen PenaSec;
  PenaSec.CreatePen(PS_SOLID, 1, Cores[CORSECOES]);
  pDC->SelectObject(&PenaSec);

  //--- Desenha os segmentos de retas das seções

  if (DesenharSecoes)
  {
    for (ittymapCTrechos it = Trechos.MapaTrechos.begin(); it != Trechos.MapaTrechos.end(); it++)
    {
      ItLPontos itlPontos(it->second.LPSecoes.begin());

      while (itlPontos != it->second.LPSecoes.end())
      {
        pDC->MoveTo(((*itlPontos).x - Deltas[X]) * Escala[X], (AlturaVideo - ((*itlPontos).y - Deltas[Y])) * Escala[Y]);
        itlPontos++;

        itlPontos++;

        pDC->LineTo(((*itlPontos).x - Deltas[X]) * Escala[X], (AlturaVideo - ((*itlPontos).y - Deltas[Y])) * Escala[Y]);
        itlPontos++;
      }
    }
  }

  for (ittymapCTrechos it = Trechos.MapaTrechos.begin(); it != Trechos.MapaTrechos.end(); it++)
  {
    ItLPontos itlPontos(it->second.LPSecoes.begin());

    //--- circula as estacas no eixo

    if (CircularSecoes)
    {
      while (itlPontos != it->second.LPSecoes.end())
      {
        itlPontos++;

        Ppixels.x = (((*itlPontos).x - Deltas[X]) * Escala[X]);
        Ppixels.y = (AlturaVideo - ((*itlPontos).y - Deltas[Y])) * Escala[Y];

        pDC->Arc(Ppixels.x - 3, Ppixels.y - 3, Ppixels.x + 3, Ppixels.y + 3, 0, 0, 0, 0);

        itlPontos++;
        itlPontos++;
      }
    }
  }

  const CList<CSecaoAvulsa, CSecaoAvulsa&>& ListSecAvulsasCalc(SecoesAvulsas.PegaLSecoesAvulsas());

  if (DesenharSecoesAvulsas && (ListSecAvulsasCalc.GetSize() > 0))
  {
    CPen PenaSec;
    Ponto PAtual, Ppixels;

    PenaSec.CreatePen(PS_SOLID, 1, Cores[CORSECOES]);
    pDC->SelectObject(&PenaSec);

      for (POSITION Atual = ListSecAvulsasCalc.GetHeadPosition(); Atual != NULL; ListSecAvulsasCalc.GetNext(Atual))
      {
        const CSecaoAvulsa& SecaoAvulsaAtual(ListSecAvulsasCalc.GetAt(Atual));

        if (SecaoAvulsaAtual.Tipo != CSecaoAvulsa::PERFIL_BUEIRO)  //--- Não desenha a seção avulsa do bueiro
        {
          pDC->MoveTo((SecaoAvulsaAtual.PInicial.x - Deltas[X]) * Escala[X], (AlturaVideo - (SecaoAvulsaAtual.PInicial.y - Deltas[Y])) * Escala[Y]);
          pDC->LineTo((SecaoAvulsaAtual.PFinal.x - Deltas[X]) * Escala[X], (AlturaVideo - (SecaoAvulsaAtual.PFinal.y - Deltas[Y])) * Escala[Y]);

          Ppixels.x = ((SecaoAvulsaAtual.PInicial.x - Deltas[X]) * Escala[X]);
          Ppixels.y = (AlturaVideo - (SecaoAvulsaAtual.PInicial.y - Deltas[Y])) * Escala[Y];

          pDC->Arc(Ppixels.x - 4, Ppixels.y - 4, Ppixels.x + 4, Ppixels.y + 4, 0, 0, 0, 0);
        }
      }
    }

  if (LPontosMedidos.size() > 0)
  {
    pDC->SelectObject(&CurrierNewHorizontal);
    pDC->SetTextAlign(TA_BOTTOM | TA_LEFT);
    pDC->SetTextColor(0);
    pDC->SetBkColor(RGB(255, 255, 166));

    CPen PenaLinha;
    PenaLinha.CreatePen(PS_SOLID, 1, Cores[CORSECOES]);
    pDC->SelectObject(&PenaLinha);

    int C(0);

    for (register itlstPontos it = LPontosMedidos.begin(); it != LPontosMedidos.end(); ++it, ++C)
    {
      if ((C % 2) == 0)  pDC->MoveTo((it->x - Deltas[X]) * Escala[X], (AlturaVideo - (it->y - Deltas[Y])) * Escala[Y]);
      else
      {
        pDC->LineTo((it->x - Deltas[X]) * Escala[X], (AlturaVideo - (it->y - Deltas[Y])) * Escala[Y]);
        pDC->TextOut((int)it->fi, (int)it->i, it->Descricao);
      }
    }

    for (register itlstPontos it = LPontosMedidos.begin(); it != LPontosMedidos.end(); ++it)
    {
      DesenharCruzPonto(Ponto(it->x, it->y), pDC, true);
    }
  }

  if (PegandoPontoSecAvulsa)
  {
    if (PegouPrimPonto)
    {
      DesenharCruzPonto(PrimeiroPontoSecAvulsa, pDC, true);
    }
  }

  CPen PenaLinha2;
  PenaLinha2.CreatePen(PS_SOLID, 1, RGB(200,200,200));
  pDC->SelectObject(&PenaLinha2);

  bool LevPena(true);

  //--- Desenha a simbologia da terraplanagem

  if(MostrarTerrap && DesenharSimbolTerapl)
  {
    for (itlstPontos it = SimbologiaTerrap.begin(); it != SimbologiaTerrap.end(); ++it)
    { 
      if (it->x != INFINITO)
      {
        Ponto Ppixels;

        Ppixels.x = ((it->x - Deltas[X]) * Escala[X]);
        Ppixels.y = (AlturaVideo - (it->y - Deltas[Y])) * Escala[Y];

        if (LevPena) pDC->MoveTo(Ppixels.x, Ppixels.y);
        else
        {
          pDC->LineTo(Ppixels.x, Ppixels.y);
        }

        LevPena = !LevPena;
      }
    }
  } 

  //--- desenha as fozes e os CGs das bacias

  CPen PenaLinha, PenaCG;
  PenaLinha.CreatePen(PS_SOLID, 4, RGB(127 + 64, 0, 0));
  PenaCG.CreatePen(PS_SOLID, 2, RGB(127 + 64, 0, 0));

  for (ittylstBacias it = Superficie.PegaBacias().begin(); it != Superficie.PegaBacias().end(); it++)
  {
   if(it->EstaCalculada())
   {
     Ponto Ppixels;

     Ppixels.x = ((it->PegaFoz().x - Deltas[X]) * Escala[X]);
     Ppixels.y = (AlturaVideo - (it->PegaFoz().y - Deltas[Y])) * Escala[Y];

     pDC->SelectObject(&PenaLinha);
     pDC->Arc(Ppixels.x - 5, Ppixels.y - 5, Ppixels.x + 5, Ppixels.y + 5, 0, 0, 0, 0);

     Ppixels.x = ((it->CG().x - Deltas[X]) * Escala[X]);
     Ppixels.y = (AlturaVideo - (it->CG().y - Deltas[Y])) * Escala[Y];
     pDC->SelectObject(&PenaCG);

     pDC->Arc(Ppixels.x - 5, Ppixels.y - 5, Ppixels.x + 5, Ppixels.y + 5, 0, 0, 0, 0);
   }

   DesenhaTalveguePrincipalBacia(*it, pDC);
  }
  
  //--- mostra as interseções do espigão com as CNs
  //--- em desenvolvimento (Bacias hidrograficas - areas das CNs)

  if (Superficie.PegaBacias().size() > 0)
  {
    if (Superficie.PegaBacias().begin()->LstIntersecoesEspigao.size() > 0)
    {
      for (ittylstIntersecoes it = Superficie.PegaBacias().begin()->LstIntersecoesEspigao.begin(); it != Superficie.PegaBacias().begin()->LstIntersecoesEspigao.end(); it++)
      {
        {
          Ponto Ppixels;

          Ppixels.x = ((it->PontoIntersecao.x - Deltas[X]) * Escala[X]);
          Ppixels.y = (AlturaVideo - (it->PontoIntersecao.y - Deltas[Y])) * Escala[Y];

          pDC->Arc(Ppixels.x - 5, Ppixels.y - 5, Ppixels.x + 5, Ppixels.y + 5, 0, 0, 0, 0);
        }
      }
    }
  }

  if(DesenharEquipDrenagem) DesenhaEquiDrenagem(pDC);

  //--- Somente para debugação

  if(1) for (ittysetPontosSoltosCArestaDGN it = SetArestas.begin() ; it != SetArestas.end() ; it++)
  {
    Ponto Ppixels;

    Ppixels.x = ((it->P1().x - Deltas[X]) * Escala[X]);
    Ppixels.y = (AlturaVideo - (it->P1().y - Deltas[Y])) * Escala[Y];

    pDC->MoveTo(Ppixels.x, Ppixels.y);
    pDC->Arc(Ppixels.x - 5, Ppixels.y - 5, Ppixels.x + 5, Ppixels.y + 5, 0, 0, 0, 0);

    Ppixels.x = ((it->P2().x - Deltas[X]) * Escala[X]);
    Ppixels.y = (AlturaVideo - (it->P2().y - Deltas[Y])) * Escala[Y];

    pDC->LineTo(Ppixels.x, Ppixels.y);
    pDC->Arc(Ppixels.x - 5, Ppixels.y - 5, Ppixels.x + 5, Ppixels.y + 5, 0, 0, 0, 0);

    CArestaDGN* pArestaAtual(const_cast<CArestaDGN*>(&*it));

    if(1) for (cittysetCInterCNArestaDGN it2 = pArestaAtual->SetIntersecoes().begin() ; it2 != pArestaAtual->SetIntersecoes().end() ; it2++)
    {
      Ppixels.x = ((it2->Intersecao.x - Deltas[X]) * Escala[X]);
      Ppixels.y = (AlturaVideo - (it2->Intersecao.y - Deltas[Y])) * Escala[Y];

      pDC->Arc(Ppixels.x - 5, Ppixels.y - 5, Ppixels.x + 5, Ppixels.y + 5, 0, 0, 0, 0);
    }
  }

  if(SetPontoCotadosSuperf.size()) DesenhaPontosCotadosSuperf(pDC);
  if(ListaAJuntar.size()) DesenhaEspigaoAIncluir(pDC);
  if(TalvegueMontado.size()) DesenhaTalvegueMontado(pDC);
  if(TalveguePrincipal.size()) DesenhaTalveguePrincipal(pDC);

  DesenhaPontosProcurados(pDC);
  DesenhaEspigaoMarcado(pDC);

  pDC->SelectObject(pPenaAnterior);
}

/////////////////////////////////////////////////////////////////////////////
// DesPontosGeomView diagnostics

#ifdef _DEBUG
void DesPontosGeomView::AssertValid() const
{
  CView::AssertValid();
}

void DesPontosGeomView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// DesPontosGeomView message handlers

void DesPontosGeomView::OnMouseMove(UINT nFlags, CPoint point)
{
  static CPoint PMouseAtual(0, 0);

  SetCursor(LoadCursor(NULL, CursorAtual));

  if (DefinindoLinhaBase > 0)
  {
    POINT PonScreenCoor = { 0,0 };
    ClientToScreen(&PonScreenCoor); //--- coordenadas da janela de descrição

    if (DefinindoLinhaBase == 1) DPopUpPonto.MostraDescricao(point.x, point.y, std::string(" Escolha o primeiro ponto da linha base "));
    else if (DefinindoLinhaBase == 2) DPopUpPonto.MostraDescricao(point.x, point.y, std::string(" Escolha o segundo ponto da linha base "));
  }

  if (Medindo)
  {
    if (Medindo == 1)
    {
      DPopUpPonto.MostraDescricao(point.x, point.y, std::string(" Escolha o ponto inicial "));
    }

    if (Medindo == 2)
    {
      double Dist(hypot(UltimoPontoPego.x - point.x, UltimoPontoPego.y - point.y) / Escala[X]);

      std::stringstream strTemp;

      strTemp << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);
      strTemp << Dist;

      POINT PonScreenCoor(point);
      ClientToScreen(&PonScreenCoor); //--- coordenadas da janela de descrição

      DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, strTemp.str());
    }
    else
    {
      if (Medindo == 3)
      {
        std::stringstream strTemp;

        strTemp << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);
        strTemp << " " << LPontosMedidos.back().z << " ";

        LPontosMedidos.back().Descricao = strTemp.str().c_str();

        Medindo = 1;
      }
    }
  }

  if(DefinindoTalveguePrincipal)
  {
    DPopUpPonto.MostraDescricao(point.x, point.y, std::string(" Definindo o talvegue principal "));
  }
  
  static CPoint PAnterior(PonMousePixelsAtual);

  if (MostrarRadar)
  {
    if (PrimeiroMovimento)
    {
      if (m_nTimer != NENHUM_TIMER) m_nTimer = KillTimer(m_nTimer);

      //m_nTimer = SetTimer(EVENT_MOUSE_STOP, 50, NULL);        //--- Evento para parada do mouse

      PrimeiroMovimento = FALSE;
      RedrawWindow();
    }
    else
    {
      DesenharApagar(OBJ_RADAR, PAnterior, PAnterior, NULL, OPR_APAGAR);
      DesenharApagar(OBJ_RADAR, PAnterior, PonMousePixelsAtual, NULL, OPR_APAGAR);
    }

    PAnterior = PonMousePixelsAtual;

    if (RadarPontosInteresse || RadarRealcarPontosInteresse)
    {
      if (Superficie.EstaRotacionada())
      {
        Ponto PSobMouse(MouseXYGeo->m_DOCoorX, MouseXYGeo->m_DOCoorY, 0.00);
        PSobMouse.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());
        SeparaPontosInteresse(PSobMouse.x, PSobMouse.y);
      }
      else SeparaPontosInteresse(MouseXYGeo->m_DOCoorX, MouseXYGeo->m_DOCoorY);
    }
  }

  if (((DesenhandoObjetos || EsbocandoObjetos) && PegouPrimPonto) || CriandoRestricao || (PegandoPontoSecAvulsa && PegouPrimPonto) || InserindoPonto || TrocandoObjeto || InserindoLembrete || MovendoLembrete || DefinindoLinhaBase > 1 || Medindo > 1)
  {
    //---Se esta inserindo ponto e o mouse passou por cima de uma restrição, marca a restrição

    if (InserindoPonto && Superficie.PegaListaRestricoes().size() > 0)
    {
      bool EstadoAnterior(ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end());

      ItLLDeltaSupMarcada = VerificaNaTangenteRestricoes(Superficie.PegaListaRestricoes().begin(), Ponto(MouseXYGeo->m_DOCoorX, MouseXYGeo->m_DOCoorY, INFINITO), Toleranciax, &ItCLDS, &Raio);

      bool EstadoAtual(ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end());

      if (EstadoAnterior != EstadoAtual)
      {
        RedrawWindow();
        DesenharApagar(OBJ_PONTO, PonMousePixelsAtual, PonMousePixelsAtual, NULL, OPR_APAGAR);
      }
    }

    if (InserindoLembrete || MovendoLembrete)
    {
      RedrawWindow();
      DesenharApagar(OBJ_IMGBITMAPFLAG, PAnterior, PonMousePixelsAtual, NULL, OPR_APAGAR);

      PAnterior = PonMousePixelsAtual;
    }
    else
    {
      if(!ArrastandoEquipDrenagem)
      if (InserindoPonto || TrocandoObjeto || DesenhandoObjetos  ||EsbocandoObjetos || ObjetosDrenagem || DefinindoLinhaBase > 1 || Medindo == 2 || (PegandoPontoSecAvulsa && PegouPrimPonto))
      {
        if (DefinindoLinhaBase > 1 || Medindo == 2)
        {
          DesenharApagar(OBJ_IMGBITMAPSETA, PAnterior, PonMousePixelsAtual, NULL, OPR_APAGAR);

          PAnterior = PonMousePixelsAtual;
        }

        DesenharApagar((InserindoPonto ? OBJ_PONTO : OBJ_LINHA), PonMousePixelsAtual, point);

        if (PrimeiroMovimento)
        {
          DesenharApagar(InserindoPonto ? OBJ_PONTO : OBJ_LINHA, PonMousePixelsAtual, PonMousePixelsAtual, NULL, OPR_APAGAR);
          PrimeiroMovimento = false;
        }

        if (TrocandoObjeto && ObjetoAnterior != ObjetoAtual)
        {
          if (ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end())
          {
            Superficie.MudaTipoAciTopografico(ItLLDeltaSupMarcada, ObjetoAnterior, ObjetoAtual);
            if (Superficie.VerfGeraArestaForcada(ObjetoAtual) && VerfMudancaTipoAcitopog(ItLLDeltaSupMarcada) == false)
            {
              monolog.mensagem(-1,"Esta mudança não pode ser feita porque causará interseção entre acidentes topográficos que geram arestas forçadas");
              Superficie.MudaTipoAciTopografico(ItLLDeltaSupMarcada, ObjetoAtual, ObjetoAnterior);
            }
            TrocandoObjeto = false;
            ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();
            AlterouTerreno = true;
          }

          RedrawWindow();

          return;
        }
      }
    }
  }

  //--- Se estiver executando um zoom, desenha o retangulo e sai

  if (EZoom)
  {
    FimZoom = point;
    RedrawWindow();

    return;
  }

  //---Se estiver desenhando equipamentos de drenagem Desenha o elástico da drenagem

  if (ArrastandoEquipDrenagem || (ObjetosDrenagem && ItInicioSarjeta != Trechos.PegaTrechoAtual()->second.SuperficieTerr.pSuperficieAtual->end()))
  {
    if (0 && ArrastandoEquipDrenagem)  //--- não está ok
    {
	    DesenharApagar(OBJ_OAC, PonMousePixelsAtual, point);
	
      if (PrimeiroMovimento)
      {
        DesenharApagar(OBJ_OAC, PonMousePixelsAtual, PonMousePixelsAtual, &ItEquiDrenagemMarcada, OPR_APAGAR);
        PrimeiroMovimento = false;
      }
    }
	else
    {
	   DesenharApagar(OBJ_LINHA, PonMousePixelsAtual, point);
	
      if (PrimeiroMovimento)
      {
        DesenharApagar(OBJ_LINHA, PonMousePixelsAtual, PonMousePixelsAtual, NULL, OPR_APAGAR);
        PrimeiroMovimento = false;   
	    }
    }
  }

  PonMousePixelsAtual = PMouseAtual = point;

  TransfPixelsXY(point, MouseXYGeo->m_DOCoorX, MouseXYGeo->m_DOCoorY);  //--- Pega as coordenadas (em metros) 
  MouseXYGeo->UpdateData(FALSE);      //--- Mostra as coordenadas de onde o mouse está.
  MouseXYGeo->RedrawWindow();

  if(Medindo) return;   //--- Veio atá aqui so para mostrar o elástico;
  //--- Se estiver criando uma restrição ou marcando uma seção avulsa mostra o elástico 
  //--- Redesenha apenas o elastico

  POINT PonScreenCoor = { point.x + 10,point.y };
  ClientToScreen(&PonScreenCoor); //--- coordenadas da janela de descrição

  Ponto PSobMouse(MouseXYGeo->m_DOCoorX, MouseXYGeo->m_DOCoorY, 0.00);
  if (Superficie.EstaRotacionada()) PSobMouse.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());

  //--- Estas variaveis (ArrastandoPI,ArrastandoCC) se fazem necessárias porque quando o mouse der pulos
  //--- Maiores que a tolerância de aproximação do ponto (tolerâncias x e y) o ponto não 
  //--- seria mais encontrado nas chamadas a PosicionaPonto , fazendo-o parar  de ser
  //--- arrastado no meio da operação. Estas variáveis asseguram o arrasto do ponto
  //--- Independentemente dos saltos do mouse, ela apenas retorna a false , no final da 
  //--- operação, no método LPbuttonUP.

  if (ArrastandoPI || ArrastandoCC)    //--- Esta Arrastando um ponto?
  {
    if (!NaoPermitirAlteracoes)
    {
      //--- Esta arrastando um PI ou CC

      DesenharApagar(OBJ_CURVA, PAntMouse, point, ItCurHorizSendoArrastadaPI ? (*ItCurHorizSendoArrastadaPI)->second : (*ItCurHorizSendoArrastadaCC)->second, OPR_APAGAR, PrimeiroMovimento);

      PrimeiroMovimento = false;

      int MovimentoValido;

      if (ItCurHorizSendoArrastadaPI)
      {
        MovimentoValido = Trechos.PegaTrechoAtual()->second.CurvasHorizontais.AlteraCurvaPI(*ItCurHorizSendoArrastadaPI, PSobMouse, ItUltimaCurvaAjustadaTS, NULL);
        if (MovimentoValido == CCurHorizontais::CURVA_OK && MostrarDescricao) DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao((*ItCurHorizSendoArrastadaPI)->second, true, OBJ_PI));
      }
      else
      {
        MovimentoValido = Trechos.PegaTrechoAtual()->second.CurvasHorizontais.AlteraCurvaCC(*ItCurHorizSendoArrastadaCC, PSobMouse, ItUltimaCurvaAjustadaTS);
        if (MovimentoValido == CCurHorizontais::CURVA_OK && MostrarDescricao) DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao((*ItCurHorizSendoArrastadaCC)->second, true, OBJ_CC));
      }

      DesenharApagar(OBJ_CURVA, PAntMouse, point, ItCurHorizSendoArrastadaPI ? (*ItCurHorizSendoArrastadaPI)->second : (*ItCurHorizSendoArrastadaCC)->second, OPR_DESENHAR);

      AlterouProjeto = true;

      PAntMouse = point;

      RedrawWindow();

      return;
    }
    else
    {
      InicioArrast = point;                      //--- Não está permitindo alterar então arrasta o desenho
      if (PermitirArrastar) Arrastando = true;                       //--- O sentido esta invertido

      ArrastandoPI = ArrastandoCC = false;
    }
  }
  else     //--- Não esta arrastando um ponto, então se estiver arrastando o desenho.
  {
    if (Arrastando || Transladando)
    {
      if(!TransladandoPorTeclas)  //--- No translado por teclas o deslocamento so pode ser feiro por teclas
      {
        Deltas[X] -= (((double)point.x) - InicioArrast.x) / Escala[X];  //--- As coordenadas x são espelhadas...
        Deltas[Y] += (((double)point.y) - InicioArrast.y) / Escala[Y];
      }

      if (Medindo == 3)
      {
        PCotaDistancia.x += point.x - InicioArrast.x;
        PCotaDistancia.y += point.y - InicioArrast.y;
      }

      OnArrastouDesenho();

      if (AnguloRotacao != 0.0) DeltasReais.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), -AnguloRotacao);

      if (CriandoRestricao) UltimoPontoPego += point - InicioArrast;

      if (RazaoEscalasJPGDes != INFINITO)
      {
       // if (!MapaSolto)   //  usado somente em testes e debugação
         if(Arrastando && !Transladando)
        {
          m_nX += (((double)point.x) - InicioArrast.x);
          m_nY += (((double)point.y) - InicioArrast.y);
        }
      }

      InicioArrast = point;

      RedrawWindow();
      
      return;
    }
    else   //--- Não está arrastando nada...
    {
      if (DefinindoTalveguePrincipal) 
      {
        if (Arrastando) TerminarArrasto = true;
        else
        {
          if (nFlags & MK_SHIFT)
          {
            Arrastando = true;
            InicioArrast = point;
          }
        }
        return;
      }

      if (SincPontosControle)
      {
        DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&NumPonCtrl, false, OBJ_SINPONCTRL));
        return;
      }

      if (MostrarDescricao)
      {
        CurHorizontal CurvaVirtual;
        bool SobreAlgumPonto(false), SobrePontoDesabilitado(false), SobrePontoInserido(false);

        if (PegandoPontoSecAvulsa)
          DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&PSobMouse, false, OBJ_PONSECAVULSA));
        else
        {
          //--- Verifica se o mouse está proximo de um ponto de terreno, de um PI, CC ou em uma seção, se estiver mostra os dados
          //---  A FAZER : ====>Embutir isso tudo nas classe de superficie

          std::string TrechoAtualTemp;
          int TipoSup(((CMainFrame*)AfxGetMainWnd())->TipoSuperficie);

          //if ((DesenharPontos || TipoSup == SUPERF_GERENMAPAS || TipoSup == SUPERF_DRONES) && VarrerTerreno)       //--- Se os pontos de  terreno estiverem desenhados
          if ((DesenharPontos && VarrerTerreno) || TipoSup == SUPERF_GERENMAPAS || TipoSup == SUPERF_DRONES || TipoSup == SUPERF_SIG)
          {
            //--- Se o mouse estiver sobre algum ponto de terreno mostra as coordenadas do ponto.
            //--- Caso contrário, limpa o dialogo do ponto , caso ele não esteja limpo.

            ItPontoDesabilitado = Superficie.SetPonDesabilitados().end();
            ItPontoInseridoUsuario = Superficie.SetPonInseridos().end();
            Ponto* PFozBacia(nullptr);
            ittymapEquipDrenagem POAC(Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end());
            ItSSetPontos ItPontoSolto(Superficie.PegaPontosSoltosMapa().end());
            itlstPontos ItPontoProcurado(LPontosProcurados.end());

            ItSSuperficie DeltaSupSobMouse(PegaPontoMaisProximo(PSobMouse, &ItPontoDesabilitado, &ItPontoInseridoUsuario,&PFozBacia,&POAC,nullptr,&ItPontoSolto,&ItPontoProcurado));

            //--- Se estiver sobre uma foz, apenas mostra as coordenadas e sai

            if(PFozBacia != nullptr)
            {
              if (Superficie.EstaRotacionada())
              {
                Ponto PontoDesrotacionado(*PFozBacia);

                PontoDesrotacionado.RotacionaPlano(sin(-Superficie.PegaAnguloRotacao()), cos(-Superficie.PegaAnguloRotacao()), -Superficie.PegaAnguloRotacao());
                DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&PontoDesrotacionado, false, PFozBacia->Baliza2 == PONTO_FOZ_BACIA ? OBJ_PONFOZBACIAHIDROG : OBJ_PONCGBACIAHIDROG));
              }                                                                                                                                                                    
              else DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(PFozBacia, false, PFozBacia->Baliza2 == PONTO_FOZ_BACIA ?  OBJ_PONFOZBACIAHIDROG : OBJ_PONCGBACIAHIDROG));

              return;
            }

            //--- Se estiver sobre uma OAC, apenas mostra as coordenadas e sai

            if (POAC != Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end())
            {
              if (Superficie.EstaRotacionada())
              {
                Ponto PontoDesrotacionado(POAC->second.CoordEixo());

                PontoDesrotacionado.RotacionaPlano(sin(-Superficie.PegaAnguloRotacao()), cos(-Superficie.PegaAnguloRotacao()), -Superficie.PegaAnguloRotacao());
                DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&POAC, false, OBJ_OAC));
                
              }
              else DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&POAC, false, OBJ_OAC));

              return;
            }

            if(ItPontoProcurado != LPontosProcurados.end())
            {
              if (Superficie.EstaRotacionada())
              {
                Ponto PontoDesrotacionado(*ItPontoProcurado);

                PontoDesrotacionado.RotacionaPlano(sin(-Superficie.PegaAnguloRotacao()), cos(-Superficie.PegaAnguloRotacao()), -Superficie.PegaAnguloRotacao());
                DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&PontoDesrotacionado, false, OBJ_PONTO_PROCURADO));
              }
              else DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&*ItPontoProcurado, false, OBJ_PONTO_PROCURADO));

              return;
            }
			
            if (ItPontoSolto != Superficie.PegaPontosSoltosMapa().end())
            {
              if (Superficie.EstaRotacionada())
              {
                Ponto PontoDesrotacionado(*ItPontoSolto);
                PontoDesrotacionado.RotacionaPlano(sin(-Superficie.PegaAnguloRotacao()), cos(-Superficie.PegaAnguloRotacao()), -Superficie.PegaAnguloRotacao());
                DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&PontoDesrotacionado, false, OBJ_PONTO_COTA_IBGE));
              }
              else
              {
                DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&*ItPontoSolto, false, OBJ_PONTO_COTA_IBGE));
              }
              return;
		    }
			//--- Adaptação para o campo golf
			
            /*
            if (DeltaSupSobMouse != Superficie.pSuperficieAtual->end())
            {
              if(PegaTipoPonto((*DeltaSupSobMouse).PegaPCentral()) == OBJ_PONTERRENO)
              {
                DeltaSupSobMouse = Superficie.pSuperficieAtual->end();
              }
            }
            */

            if (DeltaSupSobMouse != Superficie.pSuperficieAtual->end())  //--- Pegou ponto comum  da superficie
            {
              SobrePontoDesabilitado = false;

              const Ponto& PonSobMouse((*DeltaSupSobMouse).PegaPCentral());

              static CPoint PMouseImaAnterior(-1, -1);

              if (!PegouPonto)
              {
                if (0 && abs(PMouseImaAnterior.x - point.x) > 6 && abs(PMouseImaAnterior.y - point.y) > 6)
                {
                  Ima(nFlags, PonSobMouse);
                  PMouseImaAnterior = point;
                  PegouPonto = true;
                }
              }
              else
              {
                if (abs(PMouseImaAnterior.x - point.x) < 5 && abs(PMouseImaAnterior.y - point.y) < 5)
                {
                  // Ima(nFlags,PonSobMouse);
                  return;
                }
                else
                {
                  PegouPonto = false;
                  PMouseImaAnterior.x = -1;
                  PMouseImaAnterior.y = -1;
                }
              }

              if (!MostrarRadar || RadarTodosPontos || (RadarPontosInteresse && SetPontosRadarInteresse.find(DeltaSupSobMouse) != SetPontosRadarInteresse.end()))
              {
                if (DeltaSupAnterior == Superficie.pSuperficieAtual->end() || DeltaSupAnterior != DeltaSupSobMouse)
                {
                  if (Superficie.EstaRotacionada())
                  {
                    Ponto PontoDesrotacionado(PonSobMouse);

                    PontoDesrotacionado.RotacionaPlano(sin(-Superficie.PegaAnguloRotacao()), cos(-Superficie.PegaAnguloRotacao()), -Superficie.PegaAnguloRotacao());
                    DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&PontoDesrotacionado, false, PegaTipoPonto(PonSobMouse)));
                  }
                  else 
                  {
                    //--- Nos drones os pontos de terreno são pontos de CN
                    
                    if (((CMainFrame*)AfxGetMainWnd())->TipoSuperficie != SUPERF_DRONES || VarrerCN)
                    {
                      if (!SaltarCNs || fabs(fmod(PonSobMouse.z, SaltarCNs + 1)) == 0)
                      DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&PonSobMouse, false, PegaTipoPonto(PonSobMouse)));
                    }
                  }

                  DeltaSupAnterior = DeltaSupSobMouse;  //--- Atualiza a anterior
                }
                SobreAlgumPonto = true;
              }
            }
            else
            {
              if (ItPontoDesabilitado != Superficie.SetPonDesabilitados().end())   //--- Pegou ponto desabilitado
              {
                DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&*ItPontoDesabilitado, false, OBJ_PONDESABILITADO));
                SobrePontoDesabilitado = true;
              }
              else
              {
                if (ItPontoInseridoUsuario != Superficie.SetPonInseridos().end())   //--- Pegou ponto inserido pelo usuário
                {
                  //--- O ponto esta no set de pontos inseridos, se estiver apenas mostra os dados
                  //--- So existem pontos inseridos na superficie do terreno natural (SuperfAtual == 0)

                  if(ItPontoInseridoUsuario->AcidenteTopografico != 41)
                  {
                    if(!MostrarTerrap) //--- 04072023 --- os inseridos não pode ser mostrados na terraplanagem, tem o onmousemoveterrap e esta dando conflito o dialigo aparece cinza                    
                    {
                      DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&*ItPontoInseridoUsuario, false, OBJ_PONINSUSUARIO));
                      SobrePontoInserido = true;
                    }
                  }
                  else
                  {
                    if (POAC != Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end())
                    {
                      DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&POAC, false, OBJ_OAC));
                    }
                  }
                }
				/*
                else
                {
                  if (PInsSecoes != Superficie.SetPonInsSecoes().end())   //--- Pegou ponto inserido nas seções
                  {
                    //--- Verifica se o ponto esta no set de pontos inseridos nas seções, se estiver apenas mostra os dados

                    DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&*PInsSecoes, false, OBJ_PONINSSECOES));
                    SobrePontoInserido = true;
                  }
                }
				*/
              }
            }
            DeltaSupAnterior = Superficie.pSuperficieAtual->end();        //--- Se saiu do ponto limpa o anterior, para se voltar nele mostrar o dialogo novamente
            PegouPonto = false;
          }
          if(!SobreAlgumPonto)
          {
            if (MostrarTerrap && DesenharPontos)
            //if (MostrarTerrap)
            {
              //--- Verificar aqui 10-07-2023

              SobreAlgumPonto = OnMouseMoveTerrap(PSobMouse, PonScreenCoor, point);
            }
            //=== Verifica se o mouse está sobre um lembrete

            if (!MovendoLembrete)
            {
              ItLembrete = PegaLembreteMaisProximo(PSobMouse, Toleranciax * 3.0);

              if (ItLembrete != SetLembretes.end())
              {
                DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&ItLembrete, false, OBJ_LEMBRETE));
                return;
              }
            }
          }

          //--- Verifica se o mouse está proximo de um ponto de PI ou CC, se estiver mostra os dados 
          //--- ==>Embutir isso tudo na classe de curvas horizontais

          if (!SobreAlgumPonto && VarrerProjeto)
          {
#undef PI  //--- What a fock!!!
            CurvaVirtual.PI = PSobMouse;
            ItCurHorizSendoArrastadaPI = PegaPIMaisProximo(CurvaVirtual, TrechoAtualTemp);

            if (ItCurHorizSendoArrastadaPI)
            {
              SobreAlgumPonto = true;
              const Ponto& PonSobMouse((*ItCurHorizSendoArrastadaPI)->second->PI);
              DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao((*ItCurHorizSendoArrastadaPI)->second, false, OBJ_PI, &TrechoAtualTemp));
            }
            else
            {
              CurvaVirtual.CC = PSobMouse;
              ItCurHorizSendoArrastadaCC = PegaCCMaisProximo(CurvaVirtual, TrechoAtualTemp);

              if (ItCurHorizSendoArrastadaCC)
              {
                SobreAlgumPonto = true;
                DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao((*ItCurHorizSendoArrastadaCC)->second, false, OBJ_CC, &TrechoAtualTemp));
              }
              else
              {
                //--- Verifica se o mouse está proximo de uma seção, se estiver mostra os dados e sincroniza o desenho da seção.
                //--- A FAZER : O ideal seria embutir isso tudo numa classe...

                //       DesSecaoView* pDesSecao((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->DesSecao);   //--- Para sincronizar o desenho da seção

                bool SobrePN(SobreAlgumPonto);   //--- Sinaliza que já está sobre um ponto notavel (que é uma seçao tb)

                if (!SobreAlgumPonto || CircularSecoes)
                {
                  Ponto PAnterior(PSobMouse), PPosterior(PSobMouse);

                  PAnterior.x -= Toleranciax;
                  PPosterior.x += Toleranciax;

                  for (ittymapCTrechos it = Trechos.MapaTrechos.begin(); it != Trechos.MapaTrechos.end(); it++)
                  {
                    itMSecaoGeomPonto itMSecaoAnterior(it->second.MapaSecoesPonto.upper_bound(&PAnterior));
                    itMSecaoGeomPonto itMSecaoPosterior(it->second.MapaSecoesPonto.upper_bound(&PPosterior));
                    itMSecaoGeomPonto itMaisPerto(it->second.MapaSecoesPonto.end());
                    // static itMSecaoGeomPonto itPontoAnterior(MapaSecoesPonto.end());         // Esta ocorrendo erro de iteradores incompatives só pq este é statico....veja logo a frente itPontoAnterior == MapaSecoesPonto.end()
                    static Ponto PontoAnterior;

                    double MenorDistancia(INFINITO), DistanciaAtual;

                    for (itMSecaoGeomPonto ItMapSecatual = itMSecaoAnterior; ItMapSecatual != itMSecaoPosterior; ItMapSecatual++)
                    {
                      DistanciaAtual = pow(ItMapSecatual->first->x - PSobMouse.x, 2) + pow(ItMapSecatual->first->y - PSobMouse.y, 2);

                      if (DistanciaAtual < MenorDistancia)
                      {
                        MenorDistancia = DistanciaAtual;
                        itMaisPerto = ItMapSecatual;
                      }
                    }
                    if (MenorDistancia < pow(Toleranciax, 2))    //--- Se estiver proximo a um ponto
                    {
                      SobreAlgumPonto = true;

                      itMSecaoGeomPonto itPAnt(it->second.MapaSecoesPonto.end());

                      //   if(itPontoAnterior == MapaSecoesPonto.end() || *itPontoAnterior != *itMaisPerto)    ///itPontoAnterior == MapaSecoesPonto.end() iteradores incompativeis????
                      //--- se não estiver sobre o  mesmo ponto mostra a descrição

                      if (itMaisPerto != it->second.MapaSecoesPonto.end() && PontoAnterior != itMaisPerto->first)
                      {
                        //--- Sincroniza o desenho da seção. 

                        if (SincronizarSecoes)
                        {
                          CEstaca Estaca(itMaisPerto->second->EstVirtual);
                          CMainFrame* pMF((CMainFrame*)AfxGetMainWnd());

                          DesSecaoView* pDesSecao((DesSecaoView*)(pMF)->PegaDesenhoSecoes());   //--- Para sincronizar o desenho da seção
                          DesPerfilView* pDesPerfil((DesPerfilView*)(pMF)->DesPerfil);

                          if (pDesSecao)
                          {
                            if (pDesSecao->PegaNomeProjeto().Compare(it->first.c_str()) == 0)
                            {
                              if (pDesPerfil) pDesPerfil->CalculaCota(Estaca);
                              pDesSecao->Posiciona(Estaca.EstVirtual, Estaca.Cota);
                            }
                          }
                        }
                        //--- Mostra a descrição 

                        if(!SobrePN) 
                        {
                          DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&itMaisPerto, false, OBJ_SECAO));
                        }

                        PontoAnterior = itMaisPerto->first;
                      }
                      else PontoAnterior.x = PontoAnterior.y = PontoAnterior.z = INFINITO;
                    }
                  }
                }

                if (!SobreAlgumPonto && CircularSecoes)   //--- Procura nas secoes avulsas
                {
                  Ponto PAnterior(PSobMouse), PPosterior(PSobMouse);

                  PAnterior.x -= Toleranciax;
                  PPosterior.x += Toleranciax;

                  itmapSecoesAvulsas itMSecaoAvAnterior(SecoesAvulsas.UpperBound(&PAnterior));
                  itmapSecoesAvulsas itMSecaoAvPosterior(SecoesAvulsas.UpperBound(&PPosterior));
                  itmapSecoesAvulsas itSecAvMaisPerto(SecoesAvulsas.End());
                  itmapSecoesAvulsas itPonSecAvAnterior(SecoesAvulsas.End());    //---- Static não esta funcionando quando abre o desenho de novo     

                  double MenorDistancia(INFINITO), DistanciaAtual;

                  for (itmapSecoesAvulsas ItMapSecAvAtual = itMSecaoAvAnterior; ItMapSecAvAtual != itMSecaoAvPosterior; ItMapSecAvAtual++)
                  {
                    DistanciaAtual = pow((*ItMapSecAvAtual).first.x - PSobMouse.x, 2) + pow((*ItMapSecAvAtual).first.y - PSobMouse.y, 2);

                    if (DistanciaAtual < MenorDistancia)
                    {
                      MenorDistancia = DistanciaAtual;
                      itSecAvMaisPerto = ItMapSecAvAtual;
                    }
                  }

                  if (MenorDistancia < pow(Toleranciax, 2))    //--- Se estiver proximo a um ponto
                  {
                    SobreAlgumPonto = true;

                    if (itPonSecAvAnterior == SecoesAvulsas.End() || !(itPonSecAvAnterior->first == itSecAvMaisPerto->first))    //--- se não estiver sobre o  mesmo ponto mostra a descrição
                    {
                      //--- Sincroniza o desenho da seção. 

                      if (SincronizarSecoes)
                      {
                        DesSecaoView* pDesSecao((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->PegaDesenhoSecoes());   //--- Para sincronizar o desenho da seção

                        if (pDesSecao)
                        {
                          if (SecoesAvulsas.PegaLSecoesAvulsas().GetAt(itSecAvMaisPerto->second).GetCount() == 0)
                          {
                            monolog.mensagem(-1,"Esta Secão Avulsa ainda não foi definida. Para definí-la seccione a superfície. ");
                          }
                          else
                          {
                            pDesSecao->Posiciona(INFINITO, INFINITO, false, true, &SecoesAvulsas.PegaLSecoesAvulsas().GetAt(itSecAvMaisPerto->second).Nome);
                          }
                        }
                      }

                      //--- Mostra a descrição da seção avulsa

                      Ponto PTemp(itSecAvMaisPerto->first);
                      PTemp.Descricao = SecoesAvulsas.PegaLSecoesAvulsas().GetAt(itSecAvMaisPerto->second).Nome.c_str();

                      DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&itSecAvMaisPerto, false, OBJ_SECAV));

                      itPonSecAvAnterior = itSecAvMaisPerto;
                    }
                    else itPonSecAvAnterior = SecoesAvulsas.End();
                  }
                }

                //--- Verifica nas CNs

                if (!SobreAlgumPonto && DesenharCN && VarrerCN)
                {
                  static Ponto PCurNivAnterior(INFINITO, INFINITO, INFINITO);
                  Ponto PAnterior(PSobMouse), PPosterior(PSobMouse);

                  PAnterior.x -= Toleranciax;
                  PPosterior.x += Toleranciax;

                  LPontos LPontosExtremos(1, PAnterior);
                  LPontosExtremos.push_back(PPosterior);

                  CSuperficie* pSuperfAtual(MostrarTerrap ? &Trechos.PegaTrechoAtual()->second.SuperficieTerr : &Superficie);

                  ItSetItLPontos itSetPontoCNAnterior(pSuperfAtual->SetItPontosCNs().upper_bound(LPontosExtremos.begin())),
                                 itSetPontoCNPosterior(pSuperfAtual->SetItPontosCNs().upper_bound(--LPontosExtremos.end())),
                                 itSetPontoMaisPerto(pSuperfAtual->SetItPontosCNs().end());

                  double MenorDistancia(INFINITO), DistanciaAtual;

                  //  for (register ItSetItLPontos ItPonCNAtual = itSetPontoCNAnterior; ItPonCNAtual != pSuperfAtual->SetPontosCurvasNivel.end(); ItPonCNAtual++)
                  for (register ItSetItLPontos ItPonCNAtual = itSetPontoCNAnterior; ItPonCNAtual != itSetPontoCNPosterior; ItPonCNAtual++)
                  {
                    if (SaltarCNs && fabs(fmod((*ItPonCNAtual)->z, SaltarCNs + 1))) continue;

                    DistanciaAtual = pow((*ItPonCNAtual)->x - PSobMouse.x, 2) + pow((*ItPonCNAtual)->y - PSobMouse.y, 2);

                    if (DistanciaAtual < MenorDistancia)
                    {
                      MenorDistancia = DistanciaAtual;
                      itSetPontoMaisPerto = ItPonCNAtual;
                    }
                  }

                  if (MenorDistancia < pow(Toleranciax, 2))    //--- Se estiver proximo a um ponto
                  {
                    SobreAlgumPonto = true;

                    if(1)//if (*(*itSetPontoMaisPerto) != PCurNivAnterior)    //--- Evita repetiçoes teve q tirar porcausa das cns nos espigoes   01082023
                    {
                      DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&(**itSetPontoMaisPerto), false, OBJ_CURNI));

                      PCurNivAnterior = *(*itSetPontoMaisPerto);
                      ItPontoCNAtual = itSetPontoMaisPerto;
                    }
                    else 
                    {
                      PCurNivAnterior = Ponto(INFINITO, INFINITO, INFINITO);
                  //    ItPontoCNAtual = pSuperfAtual->SetItPontosCNs().end();
                    }
                  }
                }
              }
            }
          }

          DesSecaoView* pDesSecao((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->PegaDesenhoSecoes());   //--- Para sincronizar o desenho da seção

          if (!SobreAlgumPonto && DPopUpPonto.IsWindowVisible() && pDesSecao) pDesSecao->Invalidate();
          if (!SobreAlgumPonto && !SobrePontoDesabilitado && !SobrePontoInserido && DPopUpPonto.IsWindowVisible())
          {
            if(!DPopUpPonto.MostrandoProgresso())
            {
              if(!DefinindoTalveguePrincipal) 
              {
			          DPopUpPonto.ShowWindow(SW_HIDE);
			        }
            }
          }
        }
      }
    }
  }
}

void DesPontosGeomView::OnLButtonUp(UINT nFlags, CPoint point)
{
  CMainFrame* pMainFrame((CMainFrame*)AfxGetMainWnd());
  std::string NomeBase(pMainFrame->PegaProjetoBase());

  if (SincPontosControle && !Arrastando)
  {
    //--- NumPonCtrl = 100 usa os pontos de controle existentes;

    if (NumPonCtrl != 100) PontosControleGeoRef[NumPonCtrl++] = CPoint(point.x - m_nX, point.y - m_nY);
    else NumPonCtrl = 4;

    bool Desistiu(false);

    if (NumPonCtrl == 4 || !TemMapa)
    {
      int TipoSuperf(((CMainFrame*)AfxGetMainWnd())->TipoSuperficie);

      if (0)
      {
        //--- Grava os pontos de controle

        std::fstream fstPonCTRL(NomeBase + "_PontosCTRLPix.txt", std::fstream::out | std::fstream::trunc);

        fstPonCTRL << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

        fstPonCTRL << PontosControleGeoRef[0].x << ' ' << PontosControleGeoRef[0].y << ' '
                   << PontosControleGeoRef[1].x << ' ' << PontosControleGeoRef[1].y << ' '
                   << PontosControleGeoRef[2].x << ' ' << PontosControleGeoRef[2].y << ' '
                   << PontosControleGeoRef[3].x << ' ' << PontosControleGeoRef[3].y << ' ';
      }

      SincPontosControle = false;
      NumPonCtrl = 0;
      ClipCursor(0);    //--- Libera o cursor para toda a tela

      //--- Deltacarta pra poder acertar o arrasto  de ajuste do usuário

      DeltasCarta[X] = DeltasCarta[Y] = 0.0;

      if (CalculaUTMpRef() == IDOK)   //--- Calcula os pontos de controle para o mapa
      {
        if (TipoSuperf != SUPERF_GOOGLEEARTH)
        {
          DefineParametros();
          OnEnquadrar();
          
          //--- Acerta o deslocamento do mapa em relação às CNs
          //--- O Mapa pode estar dentro do arquivo DGN estas coordenas são o 
          //--- deslocamento do mapa em relação às CNs (Arquivo DGN)
       
          //--- Deslocamento da carta em relação ao mapa DGN
          //--- A carta pode estar dentro do mapa.

          DeltasCarta[X] = m_nX / Escala[X];
          DeltasCarta[Y] = m_nY / Escala[X];

          AnguloRotacaoAnterior = 0.0;
          Superficie.LimpaAngRotacao();

          //--- @Verificar - deveria ser = drones

          //if(TipoSuperf != SUPERF_GERENMAPAS)  DefineFronteiraDireto();
		  
		      //--- VERIFICAR AQUI ESTA SIMPLIFICANDO ???
          
		      DefineFronteiraDireto(TipoSuperf);

          DPopUpPonto.ShowWindow(SW_SHOW);
          DPopUpPonto.MostraDescricao(300, 200, std::string(" \r\n  Calculando a Superficie...  \r\n "));
          DPopUpPonto.RedrawWindow();

    //      CSimpPL::SimplificaXYZ(Superficie.LPontosCN);

          //--- Se = 5 superficie SIG -> Nao grava pontos editados que são os talvegues e estão sem cota (esta com INFINITO)
          
          if(TipoSuperf == 5)  
          {
            EquidisCN = ((CEstApp*)AfxGetApp())->PegaEquidisCNQGIS();
            Superficie.AtribuiEquidisCN(EquidisCN);
            Superficie.Serialize(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()), GRAVAR,false,false,false);
          }
          else
          {
            Superficie.AtribuiEquidisCN(EquidisCN);
            Superficie.Serialize(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()), GRAVAR);
          }

          if (TipoSuperf != SUPERF_GERENMAPAS && TipoSuperf != SUPERF_SIG)
          {
            if(1)
            {
              // CSimpPL::Simplifica();

              //--- Durante o ajuste do mapa não pode ampliar ocorrem muitos erros
              //--- Verificar projeto EstWinTentativaTransladoTeclaAmpliacao

              ((CChildFrame*)GetParentFrame())->DeslizanteGeom.m_CEEscala.EnableWindow(false);

              if (AjustaDeclinacao() == true) AjustaTranslacao();
              else Desistiu = true;

              ((CChildFrame*)GetParentFrame())->DeslizanteGeom.m_CEEscala.EnableWindow(true);
            }
            else
            {
              CalculaSuperficie(DPopUpPonto.Progresso(),TipoSuperf);
            }
          }
          else
          {
            CalculaSuperficie(DPopUpPonto.Progresso(),TipoSuperf);

            Superficie.CriaSetCurvasNivel();                         //--- Para pegar as CNs no mousemove
            CalculaNovasDimensoes();
            Ponto PCentral(Deltas[X] + LarguraVideo / 2.0, Deltas[Y] + AlturaVideo / 2.0);
            CentralizarTela(PCentral);

            AlterouTerreno = true;//DesenharPontos = true;

            RedrawWindow();
          }
        }
      }
      //--- A superficie foi calculada então pode-se calcular cotas de pontos nela

      TemArqTriaSuperf = CCalcSU::IniciaCalculaCotas(((CMainFrame*)AfxGetMainWnd())->m_sNomeComp.GetBuffer()) == 0;

      RedrawWindow();

      DPopUpPonto.MostraDescricao(300, 200, std::string(" \r\n  Ok, O projeto foi importado com êxito..    \r\n "));
      DPopUpPonto.RedrawWindow();
      WaitForSingleObject(CEvent(), 200);      //--- Mostra as mensagem por 2.5 segundos

      ProjetoGeoreferenciado = TipoSuperf;
    }

    if (Desistiu)
    {
      ProjetoGeoreferenciado = false;
      TipoGeoref = -1;
      TemMapa = false;
      DesenharMapa = false;
      NumPonCtrl = 0;
     
      OnEnquadrar();

      return;
    }

    return;
  }

  if (TerminarArrasto)     //--- Se estiver arrastando o desenho ou terminado o arrasto de um PI ou CC, termina o arrasto e desliga o timer
  {
    TerminarArrasto = ArrastandoPI = ArrastandoCC = Arrastando = false;

    //--- Se o usuário for transladar o inicio da translação é o fim do arrasto atual

    if(!Transladando)
    {
      DeltasIniTranslacao[X] = Deltas[X];
      DeltasIniTranslacao[Y] = Deltas[Y];
    }

    DeleteFile((NomeBase + ".cnd").c_str());     //--- Remove o arquivo CNs geradas somente para importar as CNs dos drones,  as CNs agora estão no terreno da na superficie  

    RedrawWindow();

    if (m_nTimer != NENHUM_TIMER)
    {
      KillTimer(m_nTimer);
      m_nTimer = NENHUM_TIMER;
    }
  }
  else if(Arrastando) TerminarArrasto = true;  //--- Segundo click do arrasto, sinaliza para terminar o arrasto

  CursorAtual = (ArrastandoPI || ArrastandoCC || DefinindoLinhaBase) ? (NaoPermitirAlteracoes && !DefinindoLinhaBase ? IDC_NO : IDC_CROSS) : (Arrastando ? IDC_SIZEALL : IDC_HAND);
  SetCursor(LoadCursor(NULL, CursorAtual));

  if (1)//!TransladandoPorTeclas)
  {
    if (Transladando && AfxMessageBox("Esta posição das curvas de nível em relação ao mapa esta ok?", MB_YESNO) == IDYES)
    {
      FinalizaTranslacao();
      ClipCursor(0);
      Transladando = TransladandoPorTeclas = false;
    }

    if (Transladando)   //--- Se continuar true está transladando o translado atual não foi aceito, reinicia o translado
    {
      IniciouTranslado = true;
      Transladando = false;
    }
    else
    {
      if (IniciouTranslado)    //--- 1 click para iniciar efetivamente o translado 
      {
        //--- Se estava transladando por teclas é preciso fechar esta translação e iniciar outra
        //--- porque as variaveis envolvidas em cada tipo de translação são diferentes:
        //--- No translado por mouse o deltaX e que muda e no translado por teclas o m_nX é que muda.
        /*
        if (TransladandoPorTeclas)
        {
          DeltasCarta[X] -= (DeltasIniTranslTeclas[X] - m_nX) / Escala[X];
          DeltasCarta[Y] -= (DeltasIniTranslTeclas[Y] - m_nY) / Escala[X];

          FinalizaTranslacao(true);

          TransladandoPorTeclas = false;
        }
        */

        DeltasIniTranslacao[X] = Deltas[X];
        DeltasIniTranslacao[Y] = Deltas[Y];

        IniciouTranslado = false;
        Transladando = true;
      }
    }
    if(DefinindoTalveguePrincipal)
    {
      if(ItListaIniTalvPrincipal != Superficie.PegaListaRestricoes().end())
      {
        DefineTalveguePrincipalSIG(DeltaSupSobMouseAnterior->PCentral);
      }
    }
  }
}

void DesPontosGeomView::OnRButtonUp(UINT nFlags, CPoint point)
{
  //--- Se estiver em um zoom executa-o

  if (/*nFlags & MK_CONTROL &&*/ EZoom)
  {
    FimZoom = point;
    EZoom = false;

    //--- Zoom mínimo 4 pixels em qualquer sentido

    if (abs(InicioZoom.x - FimZoom.x) > 4 && abs(InicioZoom.y - FimZoom.y) > 4) Zoom();

    return;
  }

  //--- Se estiver arrastando um ponto e for o segundo click (Terminar arrasto = true)
  //--- finaliza o arrasto

  if (TerminarArrasto)
  {
    Arrastando = ArrastandoPI = ArrastandoCC = false;
    AlterouTerreno = true;                                      //--- houve Alteração no greide (Então gravará o novo 
  }
  else
  {
    if (!ArrastandoPI && !ArrastandoCC)
    {
      if (Arrastando) Arrastando = false;	    //--- Se não estiver arrastando um ponto  pára de arrastar o desenho
      else                      //--- Não está arrastando nada não está criando restrição e clicou no fundo com o botão esquerdo, dialogo de configurar
      {
        if (CriandoRestricao)  //--- Parece que nunca fica true pois fica false no r button down
        {
          if (Superficie.PegaListaRestricoes().size() > 0 && Superficie.PegaListaRestricoes().rbegin()->size() == 1)
          {
            if (VerfObjAvulso(ObjetoAtual) == false)
            {
              Superficie.RemoveListaDoMapa(--(Superficie.PegaListaRestricoes().end()));
              Superficie.PegaListaRestricoes().pop_back();
            }
          }

          //--- Os objetos foram concebidos muito depois dos bordos. Teoricamente os bordos são objetos 
          //--->>> A FAZER tranformar os bordos em objetos
          //--- Por enquanto fica com os 2 bordos e objetos:
          //--- Passa a lista de bordos para a lista de objetos se um objeto estiver sendo desenhado

          if (0 && (DesenhandoObjetos || EsbocandoObjetos))  //--- Aguardar para quando passas asrestriçoes e os bordos para Acidentes Topograficos
          {
            if (ObjetoAtual != CAciTopografico::NENHUM_ACITOPOG)   //--- Se ja selecionou o obj)
            {
              //--- Se estiver desenhando objetos a lista do objeto sendo desenhado esta na ultima lista de Restrições.
              //--- Pega a lista e passa para a lista de objetos removendo a da lista de restrições
              //--- A FAZER ==>>> Colocar as restriçoes e os bordos nos objetos - Teoricamente são iguais
              //--- Foi feito assim pq os objetos foram concebidos muito depois das restrições

              LLDeltaSup& ListasAtuais(Superficie.PegaListaRestricoes());

              if (ListasAtuais.size() > 0)
              {
                ListaAtual = --(ListasAtuais.end());

                for (register ItCLDeltaSup it = ListaAtual->begin(); it != ListaAtual->end(); ++it)
                {
                  AciTopogAtual.InserePonto(it->PegaPCentral());
                }

                ListasAtuais.pop_back();
                AciTopogAtual.MudaObjeto(ObjetoAtual,false);
                AcidentesTopograficos.Insere(AciTopogAtual);
                AciTopogAtual.Limpa();
              }
              else
              {
                //--- logar erro
              }
            }
          }
          
          if (CriandoRestricao) 
          {
            CriandoRestricao = false;
            NomeEquiAtual.clear();     //--- Se for uma sarjeta, limpa o nome da sarjeta atual
          }
          else
          {
            //--- Se estiver esbocando objetos finaliza este objeto mas continua esbocando objetos, o proximo
            //--- click iniciará outro objeto esboçado, so finaliza de esboçar objetos quando o usuário 
            //--- clicar novamente o botão Esboçar Objetos, desabilitando-o

            if (EsbocandoObjetos) ItListaRestAtual = Superficie.PegaListaRestricoes().end();
          };
        }
        else
        {
          if (CriandoRestricao && !EsbocandoObjetos)
          {
            //--- Verifica se esta sob um ponto de perfil avulso    

            //Ponto	PSobMouse(MouseXYGeo->m_DOCoorX, MouseXYGeo->m_DOCoorY, 0.00);

            //SecoesAvulsas.VerfPontoSecaoAvulsa(PSobMouse, Toleranciax);


            OnConfdes();
          }
          else
          {
            ItListaRestAtual = Superficie.PegaListaRestricoes().end();
            if(CriandoRestricao) CriandoRestricao = false;
          }
        }
      }
    }
  }
  RedrawWindow();
}

void DesPontosGeomView::CalculaParametros()
{
  //--- Calcula a Largura e a Altura do video, que depende da resolucao e da escala
  //--- em pixels por metro.
  //--- Dependendo da resolução do video a escala deve variar, os valores default são:
  //---
  //--- 1024 x 768 -> 20 pixels por metro.
  //--- 640 X 480  -> 12,5 pixels por metro (proporcionalmente), todas as escalas
  //--- tem seu velor default proporcional ao de 1024 x 768, Esta escala varia
  //--- quando o usuário pressiona o botão + ou o botão - (Alargar ou encolher o desenho).

  LarguraVideo = Resolucao[X] / Escala[X];
  AlturaVideo = Resolucao[Y] / Escala[Y];
  AlturaVideoPixels = AlturaVideo * Escala[X];
  LarguraVideoPixels = LarguraVideo * Escala[Y];
  AlturaVideoPixels = AlturaVideo * Escala[X];
  LarguraVideoPixels = LarguraVideo * Escala[Y];
  AlturaRebatidaYPixels = AlturaVideoPixels * Coseno;
  LarguraRebatidaYPixels = LarguraVideoPixels * Seno;
  LarguraRebatidaXPixels = LarguraVideoPixels * Coseno;
  AlturaRebatidaXPixels = AlturaVideoPixels * Seno;
  NovaAlturaPixels = AlturaRebatidaYPixels + LarguraRebatidaYPixels,
  NovaLarguraPixels = LarguraVideoPixels * Coseno + AlturaVideoPixels * Seno;

  //--- A escala mudou, acerta as tolerâncias (6 pixels em qualquer eixo);

  Toleranciax = 8.0 / Escala[X];
  Toleranciay = 8.0 / Escala[Y];
}

void DesPontosGeomView::OnInitialUpdate()
{
  CView::OnInitialUpdate();

  ((CChildFrame*)GetParentFrame())->ShowWindow(SW_HIDE);

  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());

  std::string TipoSuperf;
  Trechos.TrocaTrecho(strTrechoAtual);
 
  if (Trechos.PegaTrechoAtual() != Trechos.MapaTrechos.end())
  {
    std::string NomeArquivo(Projeto + Projeto.substr(Projeto.find_last_of('\\')) + std::string(".ini"));

    MouseXYGeo = (CXYGreide*)MainFrame->MouseXYGeo;
    MouseXYGeo->Geometrico = true;      //--- Este dialogo é usado no greide e no geometrico, seta para o geometrico

    //--- Dados do mapa

    char Buffer[1024] = { 0 };
    CString Default;

    Default.Format("%lf,%lf,%lf,%lf,%lf,%i,%lf,%lf,%lf,%s", 0.0, 0.0, 0.0, 0.0, 1.0,0, 1.0,0.0,0.0, "");
    ::GetPrivateProfileString(_T("DadosDesenhos"), _T("DadosMapa"), Default, Buffer, 511,NomeArquivo.c_str());

    std::stringstream strstrBuffer(Buffer);
    char Virgula;

    strstrBuffer >> DeltasCN[X] >> Virgula >> DeltasCN[Y] >> Virgula >> DeltasCarta[X] >> Virgula 
                 >> DeltasCarta[Y] >> Virgula >> RazaoEscalasJPGDes >> Virgula >> ProjetoGeoreferenciado >> Virgula 
                 >> m_nRatio >> Virgula >> m_nX >> Virgula >> m_nY >> Virgula;

    char BufferNome[1024] = { 0 };

    strstrBuffer.getline(BufferNome, 1023);

    if(ProjetoGeoreferenciado)
    {
      NomeMapa = Projeto.c_str();
      NomeMapa += "\\";
      NomeMapa += BufferNome;
    }

    ::GetPrivateProfileString(_T("DadosDesenhos"), _T("DadosDesPontosGeoGeoRef"), _T("0,0,0,0,0,0"), Buffer, 511, NomeArquivo.c_str());
    sscanf(Buffer, "%i,%i,%i,%i,%i,%i", &FaltaGeoReferenciamento, &DesenharMapa, &MainFrame->TipoSuperficie, &DesenharCroquis, &MainFrame->FronteiraEditada,&MainFrame->TrianImporTopografia);

    if(MainFrame->TipoSuperficie == SUPERF_GERENMAPAS)  Superficie.CriaSetCurvasNivelNaoCotadasDGN(true);

    //--- Le o estado anterior do desenho e atualiza-os 

    CString Padrao(_T("0,1,0,1,0,1,1,1,1,1"));
    Padrao += MainFrame->ProjetoNovo == (SUPERF_SIG+1) ? ",1000," : ",100,";    //--- +1???? verificar
    Padrao += _T("3,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,0,0,1,0,1,0,0,1,1,0,1,1,0,1,1,1,1,0,0");

    ::GetPrivateProfileString(_T("DadosDesenhos"), MainFrame->DesPerfil ? _T("DadosDesPontosGeoConfigComPerfil") :
      _T("DadosDesPontosGeoConfig"),Padrao,Buffer, 511, NomeArquivo.c_str());
      sscanf(Buffer, "%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i", &DesenharPixel, &DesenharCirculo, &DesenharCruz, &DesenharSecoes, &DesenharArestas, &DesenharBordos, &DesenharRestricoes, &DesenharPontos,
      &MostrarDescricao, &DesenharMalha, &EquiMalha, &EquiCotas, &DesenharProjeto, &bAciTopog, &CircularSecoes, &MostrarToolBarSuperf, &MostrarToolBarGeom, &CotarMalha, &VarrerTerreno, &VarrerProjeto, &VarrerCN, &DesCNTopograficas, &DesenharCN, &CotarCN, &DefBordos,
      &SincronizarSecoes, &NaoPermitirAlteracoes, &SecoesIconizadas, &DesenharObjetos, &DesenhandoObjetos, &DesenharPontosInseridos, &ObjetoAtual, &PermitirArrastar, &SaltarCNs, &MostrarTerrap,
      &DesenharAlinTerrap, &DesenharOffSets, &VerificouCota,&PrimeiraVisualizacao,&DesenharEquipDrenagem,&DesenharSimbolTerapl,&DesenharCNTaludes,&DensidadeSimbologia,&DesenharSecoesAvulsas,&RealcarCNsMestras,&ImportouAciTopogAcadOK,&DefinindoTalveguePrincipal);

#ifdef PACOTE_BASICO
    MostrarTerrap = false;
#endif

    if (ProjetoGeoreferenciado) 
    {
      DesenharArestas = false;
      if(ProjetoGeoreferenciado == 3) DesenharBordos = false;                        //DesenharPontos = false;//DesenharBordos = false;
    }

    CChildFrame* CF((CChildFrame*)GetParentFrame());

    CF->m_wndToolBarGre.GetToolBarCtrl().SetState(ID_BUTLOCK, NaoPermitirAlteracoes);
    CF->m_wndToolBarGre.GetToolBarCtrl().SetState(ID_BUTSINCRO, SincronizarSecoes);
    CF->m_wndToolBarGre.GetToolBarCtrl().SetState(ID_SECSEMPREVIS_ICONIZAR, SecoesIconizadas);
    CF->m_wndToolBarGre.GetToolBarCtrl().SetState(ID_BUTARRASTAR, PermitirArrastar);
    CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTBORDOS, DefBordos);
    CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTACITOPOG, !DefBordos);
    CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_DESENHAR, DesenharObjetos);
    CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTPONINS, DesenharPontosInseridos);
    CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTPROJTERRAP, MostrarTerrap);

    CF->ShowControlBar(&CF->m_wndToolBarAciTopograficos, DesenharObjetos, false);

    typedef struct
    {
      int Acidente, Botao;
    } stMapaAcidenteBotao;

    struct ltAciBotao
    {
      bool operator()(const stMapaAcidenteBotao M1, const stMapaAcidenteBotao M2) const
      {
        return M1.Acidente < M2.Acidente;
      }
    };

    stMapaAcidenteBotao AcidenteBotao[37] =
    { {CAciTopografico::NENHUM_ACITOPOG,-1},{CAciTopografico::FRONTEIRA,ID_DES_FRONTEIRA},
      {CAciTopografico::MURO,ID_DES_MURO},{CAciTopografico::CERCA,ID_DES_CERCA},{CAciTopografico::MEIO_FIO,ID_DES_MEIOFIO},
      {CAciTopografico::BORDO,ID_DES_BORDO},{CAciTopografico::N_A,ID_DES_NA},{CAciTopografico::PONTE,ID_DES_PONTE},
      {CAciTopografico::TAMPA_BUEIRO,ID_DES_TAMPA_BUEIRO},{CAciTopografico::POSTE,ID_DES_POSTE}, {CAciTopografico::REDE_ELETRICA_BT,ID_DES_REDEELETRICA_BT},
      {CAciTopografico::REDE_ELETRICA_AT,ID_DES_REDEELETRICA_AT},{CAciTopografico::CASA,ID_DES_CASA},{CAciTopografico::ROCHA,ID_DES_ROCHA},
      {CAciTopografico::EIXO,ID_DES_EIXO},{CAciTopografico::CRISTA,ID_DES_CRISTA},{CAciTopografico::PE,ID_DES_PE},
      {CAciTopografico::CANTO_MURO,ID_DES_CANTO_MURO},{CAciTopografico::CANTO_CERCA,ID_DES_CANTO_CERCA},{CAciTopografico::CANTO_CASA,ID_DES_CANTOCASA},
      {CAciTopografico::BUEIRO,ID_DES_BUEIRO},{CAciTopografico::N_A_PER,ID_DES_NAPER},{CAciTopografico::LINHA_FERREA,ID_DES_LINHAFERREA},
      {CAciTopografico::MATA,ID_DES_MATA},{CAciTopografico::FLORESTA,ID_DES_FLORESTA},{CAciTopografico::PLANTACAO,ID_DES_PLANTACAO},
      {CAciTopografico::ALAGADO,ID_DES_ALAGADO},{CAciTopografico::ARVORE,ID_DES_ARVORE},{CAciTopografico::ALINHAMENTO,ID_DES_ALINHAMENTO},
      {CAciTopografico::PVELETRICA,ID_DES_PVELETRICA},{CAciTopografico::PVESGOTO,ID_DES_PVESGOTO},{CAciTopografico::PVGAS,ID_DES_PVGAS},
      {CAciTopografico::PVGAS,ID_DES_REDEGAS},{CAciTopografico::PVGAS,ID_DES_REDEELETRICA},{CAciTopografico::PVGAS,ID_DES_REDEESGOTO},
      {CAciTopografico::SOLEIRA,ID_DES_SOLEIRA},{CAciTopografico::FIM_ACITOPOG,-2} };

    std::set<stMapaAcidenteBotao, ltAciBotao> SetAcidenteBotao(AcidenteBotao, AcidenteBotao + 22);
    const stMapaAcidenteBotao AciAtual = { (int)ObjetoAtual,-1 };
    std::set<stMapaAcidenteBotao, ltAciBotao>::iterator it(SetAcidenteBotao.find(AciAtual));

    if (it != SetAcidenteBotao.end())
      CF->m_wndToolBarAciTopograficos.GetToolBarCtrl().SetState(it->Botao, true);

    Default.Format("%lf,%lf,%lf,%lf,%lf,%lf,%lf,%u,%u,%u", Escala[X], Escala[Y],RazaoHV, Deltas[X], Deltas[Y], AnguloRotacaoAnterior, RazaoEscalasJPGDes,UltPontoEsbocado,UltPontoInserido,1);
    ::GetPrivateProfileString(_T("DadosDesenhos"),_T("DadosDesPontosGeo"), Default, Buffer, 511, NomeArquivo.c_str());
    sscanf(Buffer,"%lf,%lf,%lf,%lf,%lf,%lf,%lf,%u,%u,%u", &Escala[X], &Escala[Y], &RazaoHV, &Deltas[X], &Deltas[Y], &AnguloRotacaoAnterior,&RazaoEscalasJPGDes,&UltPontoEsbocado, &UltPontoInserido,&EquidisCN);
   
    if (!NomeMapa.IsEmpty())
    {
      if (m_cPicture.Load(NomeMapa))
        //if (Mapa.Load(NomeMapa,CxImage::GetTypeIdFromName("jpg")))
      {
        if (RazaoEscalasJPGDes == INFINITO) m_nRatio = 1.0;
        else FaltaGeoReferenciamento = false;

        //     DesenhoCroquisView.AtribuiPicture(&m_cPicture);
      }
      else
      {
        if(ProjetoGeoreferenciado != 6)
        {
        std::stringstream strMsg;

        strMsg << "O mapa " << NomeMapa << " não foi encontrado." << std::ends;

        monolog.mensagem(-1,(strMsg.str().c_str()));
        }
      }
    }
    //--- Pega as cores

    ::GetPrivateProfileString(_T("DadosDesenhos"), MainFrame->DesPerfil ? _T("DadosDesPontosGeoCoresComPerfil") : _T("DadosDesPontosGeoCores"),
      _T("0,16777215,4227072,1942750,16711808,8388863,8454143,4227200,182,33023"), Buffer, 511, NomeArquivo.c_str());
    _stscanf(Buffer, "%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu,%lu", &Cores[0], &Cores[1], &Cores[2], &Cores[3], &Cores[4], &Cores[5], &Cores[6], &Cores[7], &Cores[8], &Cores[9]);

    //--- Dados do Radar

    unsigned long CorTela(0), Raio(50);

    ::GetPrivateProfileString(_T("DadosDesenhos"), MainFrame->DesPerfil ? _T("DadosDesPontosGeoComPerfilRadar") : _T("DadosDesPontosGeoRadar"),
      _T("0,50,0,1,1,16711808"), Buffer, 511, NomeArquivo.c_str());
    _stscanf(Buffer, "%i,%i,%i,%i,%i,%lu", &MostrarRadar, &Raio, &RadarTodosPontos, &RadarPontosInteresse, &RadarRealcarPontosInteresse, &CorTela);

    CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTRADAR, MostrarRadar);

    Radar.MudaRaio(Raio, Escala[0]);
    Radar.MudaCorTela(&(COLORREF)CorTela);

    //--- Inicializa os valores da escala, da razão e da tolerância.

    CF->DeslizanteGeom.MudarEscala(Escala[X]);

    //--- Na inicialização está mudando este ponteiro para a outra superfície, acertando o iterador aqui.

    DeltaSupSobMouseAnterior = Superficie.pSuperficieAtual->end();

    //--- Parametros do tamanho da janela

    RECT Coord;
    GetClientRect(&Coord);

    Resolucao[X] = Coord.right;
    Resolucao[Y] = Coord.bottom;

    GetWindowRect(CRectJanPai);    	//--- Salva As novas coordenadas para saber as posições do pop up

    DefineParametros();
    CalculaParametros();
    CalculaNovasDimensoes();
    OnAlterouAngulo(GetDC());

    if (PrimeiraVisualizacao)
    {
      if (ProjetoGeoreferenciado != SUPERF_DRONES)
        DesenharPontos = VarrerTerreno = true;
      else
        VarrerTerreno = DesenharPontos = DesenharArestas = false;
      
      if (Superficie.pSuperficieAtual->size() > 0)
      {
        PrimeiraVisualizacao = false;
        OnEnquadrar();
      }
    }

    if (!Trechos.TemTrechoAtual())
    {
      if (ProjetoGeoreferenciado == SUPERF_DRONES) MostrarTerrap = CotarCN = DesenharPontos = DesenharArestas = false;
      else if (Trechos.PegaTrechoAtual() != Trechos.MapaTrechos.end() && Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaQuantPontos() == 0) MostrarTerrap = true;
    }

    YCentral = (AlturaVideo / 2.0 + (Deltas[Y])),
    XCentral = (LarguraVideo / 2.0 + (Deltas[X]));

    DPopUpPonto.Create();     //--- dialogo popup do ponto

    Radar.MudaRaio(Raio, Escala[X]);

    AnguloRotacaoAnterior = 0.0;

    if (AnguloRotacaoAnterior != 0.0)
    {
      OnButrotacionar();
      AnguloRotacaoAnterior = 0.0;
    }

    VerificouCota = false;

    if (0 && !VerificouCota && Superficie.MaiorCota > 10000.00)
    {
      if (AfxMessageBox("Os valores das cotas estão muito elevados (Algumas cotas superam 10000.000 metros). Tem certeza que estes valores estão corretos?", MB_YESNO) == IDYES) VerificouCota = true;
      else AfxMessageBox("Verifique os arquivos de pontos bem como o formato do arquivos de pontos na configuração do projeto.");
    }

    TemArqTriaSuperf = CCalcSU::IniciaCalculaCotas(MainFrame->m_sNomeComp.GetBuffer()) == 0;

    Inicializou = true;

    CF->ShowControlBar(&CF->m_wndToolBarEquiDrenagem,SW_HIDE, false);

  }
  else
  {
    monolog.mensagem(-1,"Não há trechos nesta superfície.");

    return;
  }

  DeltaSupAnterior = Superficie.pSuperficieAtual->end();

  ItInicioSarjeta = ItFimSarjeta = Trechos.PegaTrechoAtual()->second.SuperficieTerr.pSuperficieAtual->end();

  ItEquiDrenagemMarcada = Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end();

  std::string NomeProjeto(Projeto + Projeto.substr(Projeto.find_last_of('\\')));

  LeArqSimbologia(SimbologiaTerrap, Trechos.PegaTrechoAtual()->first);

  tylstUtm_Coord_3d LstPontosGeoRef;
  setPontos SetPontosSoltos;

  std::string NomeProjetoBasico(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase());

  CDiaImportarArqCtrlDrones DialogoArqDrones(SetPontosSoltos,this, NomeProjetoBasico, true, &LstPontosGeoRef,3,EquidisCN);

  std::string Formato("n N E C O");

  if(MainFrame->ProjetoNovo != -1)
  {
    ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->ShowWindow(SW_HIDE);
	
    ((CChildFrame*)GetParentFrame())->ShowWindow(SW_MAXIMIZE);

    switch(MainFrame->ProjetoNovo)
    {
      case 1: ((CMainFrame*)AfxGetMainWnd())->TipoSuperficie = SUPERF_TRADICIONAL; break;
      case 2:
      case 3:
      case 5: 
      case 6:
      {
        OnBaixarMapa();
      }
      break;
      case 4:
      {
        MainFrame->TipoSuperficie = SUPERF_TOPOG_ACAD;
        ((CChildFrame*)GetParentFrame())->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTIMPACIAUTOCAD,TRUE);

        OnImportarAciAutoCad();

        MainFrame->ProjetoNovo = -1;
      }
      break;
    }
  }

  SerializaTalveguePrincipal(LER);

  //--- Verifica se a bacia deste talvegue ja foi calculada, se ja foi este talvegue passou para a bacia
  //--- e nao precisa mais ser mostrado, remove-o

 if(TalveguePrincipal.size()) 
 {
   if(Superficie.AchaBaciaTalvPrincipal(TalveguePrincipal))
   {
     TalveguePrincipal.clear();

     ((CMainFrame*)AfxGetMainWnd())->RemoveArqProjeto(std::string(".tae"));
   }
 }

#ifndef PACOTE_AVANCADO
  #ifndef DEMOVER
  if (ProjetoGeoreferenciado)
  {
    monolog.mensagem(-1, "Projetos georeferenciados não podem ser abertos neste tipo de pacote\r\n "
      "adquira o pacote avançado ");

    ((CMDIChildWnd*)this->GetParentFrame())->CloseWindow();
  }
  #endif
#endif
}

void DesPontosGeomView::OnSize(UINT nType, int cx, int cy)
{
  if (cx == 0 || cy == 0 || !Inicializou) return;

  RECT Coord;
  GetClientRect(&Coord);

  Resolucao[X] = Coord.right;
  Resolucao[Y] = Coord.bottom;

  DefineParametros();
  CalculaParametros();
  CalculaNovasDimensoes();

  if (ProjetoGeoreferenciado > SUPERF_TRADICIONAL)     //--- Drone ou Google Earth
  {
    YCentral = (AlturaVideo / 2.0 + (Deltas[Y])),
    XCentral = (LarguraVideo / 2.0 + (Deltas[X]));

    m_nX = (DeltasCN[X] - Deltas[X]) * Escala[X];
    m_nY = AlturaVideoPixels - ((DeltasCN[Y] - Deltas[Y]) * Escala[Y]);   //--- o DeltaYMapa está no topo do mapa (y= 0 do video)
  }

  OnArrastouDesenho();
}

void DesPontosGeomView::MudarEscala(double Fator, double zDelta)
{
  if(SincPontosControle) return;
  YCentral = (AlturaVideo / 2.0 + (Deltas[Y]));
  XCentral = (LarguraVideo / 2.0 + (Deltas[X]));

  //--- Acerta as escalas

  Escala[X] = Escala[Y] = Fator / 100.0;

  CalculaParametros();
  CalculaNovasDimensoes();

  Deltas[X] = XCentral - (LarguraVideo / 2.0);
  Deltas[Y] = YCentral - (AlturaVideo / 2.0);

  if (RazaoEscalasJPGDes != INFINITO)
  {
    m_nRatio = Escala[X] / RazaoEscalasJPGDes;

    //--- Posiciona o mapa no 0,0 em relação as CNs

    m_nX = (DeltasCN[X] - Deltas[X]) * Escala[X];
    m_nY = AlturaVideoPixels - ((DeltasCN[Y] - Deltas[Y]) * Escala[Y]);   //--- o DeltaYMapa está no topo do mapa

    //--- Posiciona o mapa dentro do arquivo de curvas de nível (DGN) (pode ser que ele não esteja no canto superior esquerdo)
   
    m_nX += DeltasCarta[X] * Escala[X];
    m_nY += DeltasCarta[Y] * Escala[X];

  }

  //--- =====> Está dando muito erro nesta comparação, deve ser porque troca a superficie, verificar
  //--- =====> É sim é porque troca a superficie para terrapanagem
  //--- =====> Não é não, mesmo sem trocar a superficie o erro acontece
  //--- =====> #VERIFICAR
  /*
  if (DeltaSupSobMouseAnterior != Superficie.pSuperficieAtual->end())
  {
    TransfXYPixels(DeltaSupSobMouseAnterior->PegaPCentral().x, DeltaSupSobMouseAnterior->PegaPCentral().y, UltimoPontoPego);
    DesenharApagar(OBJ_LINHA, UltimoPontoPego, PonMousePixelsAtual, NULL, OPR_APAGAR, TRUE); 
  }
  */

  if (LPontosMedidos.size() > 1) AcertaCotasPontosMedidos();

  OnArrastouDesenho();

  pDesCroquisView = ((CMainFrame*)AfxGetMainWnd())->pDesCroquisView2;

  if (pDesCroquisView) pDesCroquisView->MudouTamDesenho(AlturaVideo, LarguraVideo);

  AlterouAngulo = true;
}

void DesPontosGeomView::MudarRazao(double Fator)
{
  RazaoHV = Fator;
  Escala[X] = Escala[Y] * Fator;
  CalculaParametros();
  OnArrastouDesenho();
  RedrawWindow();
}

void DesPontosGeomView::OnLButtonDown(UINT nFlags, CPoint	point)
{

  /*//===> Teste prodgress dialogo popuppontos

   static int GG = 1;

   if(GG == 1)
   {
     DPopUpPonto.MostraDescricao(300, 200, std::string(" \r\n  Calculando a Superficie...  \r\n "), true);
     GG =0;
   }


 //  DPopUpPonto.Progresso()->SetPos(0);
 //  DPopUpPonto.Progresso()->SetPos(0);

  // for (int C = 0 ; C < 100 ; C++)
   {
    // Sleep(25);
     DPopUpPonto.Progresso()->StepIt();
  //   DPopUpPonto.MostraDescricao(300, 200, std::string(" \r\n  Calculando a Superficie...  \r\n "), true);
  // Sleep(1000);

     //DPopUpPonto.RedrawWindow();
   }

     return;

   */

  CircularPontoIntersecao = false;

  if (SincPontosControle)
  {
    if (Arrastando) TerminarArrasto = true;
    else
    {
      if (nFlags & MK_SHIFT)
      {
        Arrastando = true;
        InicioArrast = point;
      }
    }
    return;
  }

  if (Arrastando)
  {
    if (PermitirArrastar) TerminarArrasto = true;
    return;
  }

  if (SecoesIconizadas) IconizaSecao();

  bool ContinuarLista(false);

  //---	Verifica se	há um	ponto	sob	o	mouse	se houver	e	estiver	editando acidentes topograficos
  //---	Se o click foi sobre um	ponto	e	se estiver criando um	acidente topográfico,	inclui o ponto na	lista, selele	já não estiver nela	
  //---	Se o click foi sobre um	ponto	e	não	estiver	criando	o	acidente topografico inicia	uma	lista	de acidente	topografico. 

  Ponto	PSobMouse(MouseXYGeo->m_DOCoorX, MouseXYGeo->m_DOCoorY, 0.00);

  //---	Se a superficie	estiver	rotacionada, rotaciona o ponto sob o mouse.

  if (Superficie.EstaRotacionada()) PSobMouse.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());

  if (DefinindoLinhaBase != 0)
  {
    if (DefinindoLinhaBase == 1)
    {
      PontosLinhaBase[0] = PSobMouse;
      DefinindoLinhaBase = 2;
      UltimoPontoPego = point;

      RedrawWindow();

      return;
    }
    else
    {
      PontosLinhaBase[1] = PSobMouse;
      DefinindoLinhaBase = 3;

      RedrawWindow();

      RotacionarLinhaBase();

      return;
    }
  }

  if (Medindo == 1)
  {
    PontosMedir[0] = PSobMouse;
    UltimoPontoPego = point;
    Medindo = 2;

    LPontosMedidos.emplace_back(PSobMouse);

    RedrawWindow();

    return;
  }
  else
  {
    if (Medindo == 2)
    {
      PontosMedir[1] = PSobMouse;

      PSobMouse.z = hypot(PontosMedir[0].x - PontosMedir[1].x, PontosMedir[0].y - PontosMedir[1].y);
      PSobMouse.fi = (((PontosMedir[0].x + PontosMedir[1].x) / 2.0) - Deltas[X]) * Escala[X];
      PSobMouse.i = PCotaDistancia.y = (AlturaVideo - (((PontosMedir[0].y + PontosMedir[1].y) / 2.0) - Deltas[Y])) * Escala[Y];

      LPontosMedidos.emplace_back(PSobMouse);

      Medindo = 3;

      OnMouseMove(nFlags, point);

      RedrawWindow();

      return;
    }
  }

  if (InserindoPonto)
  {
    int Resp(Superficie.VerfPontoForaSuperficie(PSobMouse));

    if (Resp == 0 || Resp == 2 || ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end())  //--- Resp == 0 = Ponto dentro da superficie, Resp == 2 a fronteira ainda não está definida
    {
      CString CStrDescricao("Ponto Inserido Usuário: ");
      CString CStrDescUsuario;
      int intCalcCotaSuper(false);
      double Cota(0.0);

      CDPedeDescricao PedeDescricao(&CStrDescUsuario, &intCalcCotaSuper, &Cota);

      if (PedeDescricao.DoModal() == IDOK)
      {
        if (intCalcCotaSuper == 1 && Resp == 2)
        {
          monolog.mensagem(-1, "Como a fronteira não esta definida, é obrigatória a digitação da cota.");
          return;
        }
        else
        {
          if (intCalcCotaSuper == 1)  Cota = 0.0;

          PSobMouse.z = Cota;
          CStrDescUsuario.Replace('/', '|');   //--- Nao pode ter barra direita, troca por pipe
          CStrDescricao += CStrDescUsuario;
          Mat.Arredonda(PSobMouse, 4);
          if (ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end()) InserePontoNaRestricaoMarcada(ItLLDeltaSupMarcada, PSobMouse, intCalcCotaSuper == 1);
          CDeltaSuperficie DeltaSuperInserida(PSobMouse.x, PSobMouse.y, PSobMouse.z, INFINITO, INFINITO, INFINITO, CStrDescricao, CAciTopografico::NENHUM_ACITOPOG, CDeltaSuperficie::INSERIDO_USUARIO, Superficie.ProxNumPonInsUsuario());
          Superficie.InsereDeltaSuperficie(DeltaSuperInserida);
        }
      }

      InserindoPonto = false;
      AlterouTerreno = true;

      ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();
    }
    else monolog.mensagem(-1, "Não é permitida a inclusão de pontos fora da Fronteira da superfície.");

    RedrawWindow();
    return;
  }

  if (InserindoLembrete)
  {
    std::string Titulo, Texto;
    COLORREF Cor;
    CDiaLembretes DiaLembretes(this, Titulo, Texto, Cor);

    if (DiaLembretes.DoModal() == IDOK)
    {
      Ponto PCoorMetros;
      CPoint PMouse(point.x, point.y - 13);

      TransfPixelsXY(PMouse, PCoorMetros.x, PCoorMetros.y);  //--- Pega as coordenadas (em metros) 
      SetLembretes.insert(CLembrete(CLembrete::DES_GEOMETRICO, PCoorMetros, Texto, Titulo, Cor));
    }

    InserindoLembrete = false;

    RedrawWindow();
    return;
  }

  if (MovendoLembrete)
  {
    Ponto PCoorMetros;
    CPoint PMouse(point.x, point.y - 13);

    TransfPixelsXY(PMouse, PCoorMetros.x, PCoorMetros.y);

    CLembrete NovoLembrete(CLembrete::DES_GEOMETRICO, PCoorMetros, ItLembrete->Texto(), ItLembrete->Titulo());
    SetLembretes.erase(ItLembrete);
    ItLembrete = SetLembretes.insert(NovoLembrete).first;

    MovendoLembrete = false;

    RedrawWindow();
    return;
  }

  if (ItLembrete != SetLembretes.end())
  {
    MovendoLembrete = true;
    return;
  }

  ItSSetPontos PInsSecoes(Superficie.SetPonInsSecoes().end());
  ItSSuperficie ItPontoTerrap(Trechos.PegaTrechoAtual()->second.SuperficieTerr.pSuperficieAtual->end()),
    DeltaSupSobMouse(PegaPontoMaisProximo(PSobMouse, nullptr, nullptr, nullptr, nullptr, &ItPontoTerrap));

  //--- Em todos os casos pega o ponto mais próximo exceto se estiver na superficie de drones e estiver esboçando objetos
  //--- pois o ponto esboçado terá que ser inserido na superficie. Se pegasse o ponto mais proximo o ponto não seria inserido
  //--- na superficie.

  //--- Trata as exeções --------------------------------------------------------------

  //--- Não pega ponto de CN na superficie drones (por enquanto, quando simplificar as CN poderá pegar)

  if (DeltaSupSobMouse != Superficie.pSuperficieAtual->end())
  {
  /*
    if (DefinindoTalveguePrincipal)
    {
      if (DeltaSupSobMouse->TipoObjeto == CAciTopografico::TALVEGUE)
      {
        DefineTalveguePrincipalSIG(DeltaSupSobMouse->PCentral);

      }
    }
	*/
    if (DefinindoTalveguePrincipal)
    {
      if (DeltaSupSobMouse->TipoObjeto == CAciTopografico::TALVEGUE)
      {
        DeltaSupSobMouseAnterior = DeltaSupSobMouse;   //--- Salva para pegar no buttonup
      }
      else return;
    }


    if (((CMainFrame*)AfxGetMainWnd())->TipoSuperficie == SUPERF_DRONES &&
      DeltaSupSobMouse->TipoObjeto == 0)                                       //--- 0 = ponto de CN = terreno na superf drones
    {
      DeltaSupSobMouse = Superficie.pSuperficieAtual->end();
    }

    //--- EsbocandoObjetos ((Espigões, talvegues etc) so pode pegar pontos do mesmo tipo de objeto. 

    if (EsbocandoObjetos && DeltaSupSobMouse != Superficie.pSuperficieAtual->end())
    {
      if (DeltaSupSobMouse->TipoObjeto != ObjetoAtual)
      {
        DeltaSupSobMouse = Superficie.pSuperficieAtual->end();
      }
    }
  }

  //--- Caixas coletoras e bacias de dissipação não podem ser movidas da erro VERIFICAR !!!

  if (DeltaSupSobMouse != Superficie.pSuperficieAtual->end())
  {
    if (ObjetoAtual == CAciTopografico::EQUI_CAIXA_COLETORA || ObjetoAtual == CAciTopografico::EQUI_BACIA_DISSIPACAO)
    {
      if (DeltaSupSobMouse->TipoObjeto == ObjetoAtual)
      {
        return;
      }
    }
  }

  //--- Pontos de espigões não podem ser repetidos em outros espigões apenas o primeiro ponto
  //--- do mesmo espigão, para fechá-lo
  if (0)
  {

    if (DeltaSupSobMouse != Superficie.pSuperficieAtual->end() &&
      ObjetoAtual == CAciTopografico::ESPIGAO &&
      DeltaSupSobMouse->TipoObjeto == CAciTopografico::ESPIGAO)    //--- Esta criando um espigão e clicou num ponto de espigão
    {                                                                  //--- Só aceita se for o primeiro ponto deste espigão, para fechá-lo
      LLDeltaSup& ListasAtuais(Superficie.PegaListaRestricoes());
      ItLLDeltaSup itLAtual(Superficie.PegaListaRestricoes().begin());

      //--- Verifica se é o primeiro ponto da lista que esta sendo editada (no caso o Espigão atual) se for permite pois esta fechando o espigão

      for (; itLAtual != ListasAtuais.end() && (*itLAtual).begin() != (*itLAtual).end() && (*(*itLAtual).begin()) != *DeltaSupSobMouse; ++itLAtual);

      if (itLAtual == ListasAtuais.end()) return;   //--- não é o primeiro ponto da lista atual (O Espigão atual), retorna, não permite a inserção no espigão
    }
  }
  //-------------------------------------------------------------------------------------------

  //--- Se estiver inserindo um objeto de drenagem  so pode pegar a deltasuperficie se for do mesmo tipo, paca cair na alteração do equipamento de drenagem.
  //--- Se não for do mesmo tipo ele insere um equipamento de drenagem do tipo que está selecionado, para isso 
  //--- DeltaSupSobMouse tem q ser igual a Superficie.pSuperficieAtual->end();

  bool InserindoObjDrengem(false), EditandoObjDrenagem(false);

  if (DeltaSupSobMouse != Superficie.pSuperficieAtual->end() && ObjetosDrenagem && ObjetoAtual != DeltaSupSobMouse->TipoObjeto)
  {
    //--- Se o objeto atual é diferente do clicado está inserindo o equip drenagem:

    //-- Se for equip agregado tem q manter a deltasupsobmouse para achar o equipamento a agregar
    //-- se nao anula a DeltaSupSobMouse para poder inserir o equipamento de drenagem

    if (!CEquipDrenagem::EquipAgregado(ObjetoAtual))
    {
      DeltaSupSobMouse = Superficie.pSuperficieAtual->end();
      InserindoObjDrengem = true;
    }
  }
  else
  {
    //--- Se o objeto atual é igual do clicado está editando o equip drenagem :
    //--- Se estiver no modo de equipamentos de  drenagem esta editando equip de drenagem.

    EditandoObjDrenagem = ObjetosDrenagem;
  }
  /*
*    Adaptação para o campo de golf ajustado funcionar nas bacias hidrograficas
*
*
  if(DeltaSupSobMouse != Superficie.pSuperficieAtual->end())
  {
    if (PegaTipoPonto((*DeltaSupSobMouse).PegaPCentral()) == OBJ_PONTERRENO)
    {
      if(DeltaSupSobMouse->TipoObjeto != 37)
      {
        DeltaSupSobMouse = Superficie.pSuperficieAtual->end();
      }
    }
  }
  */

  //--- Pegando pontos seções avulsas tem prioridade sobre objetos de drenagem ou EsbocandoObjetos

  if (!PegandoPontoSecAvulsa && (ObjetosDrenagem || EsbocandoObjetos) && DeltaSupSobMouse == Superficie.pSuperficieAtual->end())
  {
    if (!(nFlags & MK_SHIFT))
    {
      if (ObjetoAtual == CAciTopografico::EQUI_EDITAR_DRENAGEM) return; //-- clicou no fundo editando a drenagem, não faz nada

      //     if((ObjetosDrenagem ||  EsbocandoObjetos) && ItPontoTerrap == Trechos.PegaTrechoAtual()->second.SuperficieTerr.pSuperficieAtual->end())
      if (ObjetoAtual != CAciTopografico::NENHUM_ACITOPOG)
      {
        if (ObjetosDrenagem || EsbocandoObjetos)
        {
          if (ObjetoAtual != CAciTopografico::EQUI_CANALETA_TERRAPL)   //--- Sarjeta de terraplanagem tem que clicar em um ponto de terraplanagem (existente)
          {
            if (!ArrastandoEquipDrenagem && !CEquipDrenagem::EquipAgregado(ObjetoAtual) && !CEquipDrenagem::EquipAlinhado(ObjetoAtual))
            {
              if (ObjetoAtual == (CAciTopografico::TALVEGUE))
              {
                ittylstBacias ItBaciaAtual(Superficie.ProcuraTalvegueBacia(PSobMouse));

                if (ItBaciaAtual != Superficie.PegaBacias().end())
                {
                  if (AfxMessageBox("Este talvegue pertence a uma bacia hidrográfica, ao alterá-lo a bacia deverá ser calculada novamente, confirma?", MB_OKCANCEL) == IDCANCEL)
                  {
                    return;
                  }
                  else
                  {
                    Superficie.PegaBacias().erase(ItBaciaAtual);
                  }
                }
              }

              PSobMouse.Nome.Format("PDES-%d", ++UltPontoEsbocado);
              PSobMouse.Baliza2 = Ponto::ESBOCADO;
              PSobMouse.Descricao = CAciTopografico::VecNomeDescricao.find(ObjetoAtual)->Nome().c_str();
              PSobMouse.AcidenteTopografico = ObjetoAtual;
              PSobMouse.z = INFINITO;

              Ponto PSobMouseRot(PSobMouse);

              if (Superficie.EstaRotacionada())   //--- se estiver desrotaciona
              {
                PSobMouseRot.RotacionaPlano(sin(-Superficie.PegaAnguloRotacao()), cos(-Superficie.PegaAnguloRotacao()), Superficie.PegaAnguloRotacao());
              }

              Ponto P1, P2, P3;

              if (CCalcSU::PegaTriangulo(P1, P2, P3, PSobMouseRot) == 0)  //--- Aqui a superficie não esta rotacionada
              {
                if (Superficie.EstaRotacionada())   //--- se estiver rotaciona os pontos do triangulo
                {
                  P1.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());
                  P2.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());
                  P3.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());
                }

                bool Calculou(false);

                //--- VERIFICAR !!!!!!!

                if (1)     //--- O cálculo vetorial das cotas não esta muito certo
                {
                  ItSSuperficie ItSP1(Superficie.Find(P1, 1e-3));     //--- Esta superficie ela esta rotacionada. Acertar essa bagunça!!!
                  if (ItSP1 != Superficie.pSuperficieAtual->end())
                  {
                    ItSSuperficie ItSP2(Superficie.Find(P2, 1e-3));
                    if (ItSP2 != Superficie.pSuperficieAtual->end())
                    {
                      ItSSuperficie ItSP3(Superficie.Find(P3, 1e-3));
                      if (ItSP3 != Superficie.pSuperficieAtual->end())
                      {
                        Matem::CalculaZ(ItSP1->PCentral, ItSP2->PCentral, ItSP3->PCentral, PSobMouse);
                        if (PSobMouse.z == INFINITO)
                        {
                          monolog.mensagem(-1, "A Cota deste ponto não pode ser calculada, caso este seja um projeto "
                            "com cartas topográficas (do IBGE por exemplo), verifique se não há alguma curva de nível sem "
                            "ser cotada nas proximidades, caso haja, cote-as através do botão <Cotar CN> "
                            "e tente novamente.");
                          DeltaSupSobMouse = Superficie.pSuperficieAtual->end();
                          return;
                        }
                        Calculou = true;
                      }
                    }
                  }
                }
                if (!Calculou)
                {
                  CalculaCotaPontoCN(PSobMouse);    //--- Calcula a cota pelas CNs
                }
              }

              //      if (!(DeltaSupSobMouse != Superficie.pSuperficieAtual->end() && ObjetoAtual == CAciTopografico::ESPIGAO && DeltaSupSobMouse->TipoObjeto == CAciTopografico::ESPIGAO))
              {
                //--- Verifica se esta inserindo ponto detalvegue dentro de alguma bacia
        /*====>>> DEPOIS :) ======

                if(ObjetoAtual == CAciTopografico::TALVEGUE)
                {
                  this->LstBacias
                  int CBacia::VerfTalvegue(ItLLDeltaSup AciTopog)

                  {
                    if (AfxMessageBox("Este talvegue está sendo inserido dentro de uma bacia existente, para inserí-lo a bacia terá que ser recalculada, confirma?", MB_YESNO) != IDYES)
                      return;
                  }
                }
        */

                Superficie.InsereDeltaSuperficie(CDeltaSuperficie(PSobMouse, PSobMouse.AcidenteTopografico));

                //--- Pega o ponto que acabou de ser inserido

                DeltaSupSobMouse = PegaPontoMaisProximo(PSobMouse, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, true);
              }
            }
          }
        }
      }
      else
      {
        monolog.mensagem(-1, "Selecione uma O.A.C. na caixa de ferramentas.");
        return;
      }
    }
    else
    {
      Arrastando = true;
      InicioArrast = point;

      return;
    }
  }

  //--- Fazer uma tabela verdade e melhorar esta logica. (usar equações booleanas)

  bool PegouPonto(DeltaSupSobMouse != Superficie.pSuperficieAtual->end() && (DeltaSupSobMouse->PegaPCentral().Inserido == false || DesenharPontosInseridos != 0)),
    PegouPontoInserido(DeltaSupSobMouse != Superficie.pSuperficieAtual->end() && DeltaSupSobMouse->PegaPCentral().Descricao.Find("Ponto Inserido") != -1),
    PegouPontoInseridoUsuario(DeltaSupSobMouse != Superficie.pSuperficieAtual->end() && DeltaSupSobMouse->PegaPCentral().Descricao.Find("Ponto Inserido Usuário") != -1);

  PegouPonto |= PegouPontoInseridoUsuario;
  PegouPonto &= ((ObjetosDrenagem || DesenharPontos) && VarrerTerreno) || (((CMainFrame*)AfxGetMainWnd())->TipoSuperficie == SUPERF_DRONES) ||
    (((CMainFrame*)AfxGetMainWnd())->TipoSuperficie == SUPERF_TOPOG_ACAD) ||
    (((CMainFrame*)AfxGetMainWnd())->TipoSuperficie == SUPERF_SIG);
  // PegouPonto &= !EsbocandoObjetos;  //--- Verificar a necessidade  de verificar o PegouPontoInseridoUsuario e o PegouPontoInserido

  if ((!ArrastandoPI && !ArrastandoCC) && PegouPonto)
  {
    if (DesenhandoObjetos || EsbocandoObjetos || ObjetosDrenagem)
    {
      if (ObjetoAtual == CAciTopografico::NENHUM_ACITOPOG)
      {
        monolog.mensagem(-1, "Selecione um objeto na caixa de ferramentas.");
        return;
      }
      if ((!EsbocandoObjetos && !ObjetosDrenagem) && DeltaSupSobMouse->PCentral.z == 0.00)
      {
        monolog.mensagem(-1, "Pontos sem cota não podem ser usados para definir acidentes topográficos:\n1 - Primeiro insira o ponto na superfície sem que conste de nenhum acidente topográfico. \n"
          "2 - Calcule a superfície (assim a cota do ponto será calculada).\n"
          "3 - Por fim, insira o ponto no acidente topográfico desejado.\n\n"
          "Obs.: Caso o ponto tenha sido inserido em um acidente topográfico a cota pode ser calculada sem que seja preciso calcular a superfície.");
        return;
      }
    }

    if (EsbocandoObjetos || ObjetosDrenagem || DefinindoTalveguePrincipal || (DesenhandoObjetos && ObjetoAtual != CAciTopografico::NENHUM_ACITOPOG && DesenharPontos && !ArrastandoPI && !ArrastandoCC))
    {
      if (!MostrarRadar || !RadarPontosInteresse || DefinindoTalveguePrincipal || SetPontosRadarInteresse.find(DeltaSupSobMouse) != SetPontosRadarInteresse.end())
      {
        if (!PegouPontoInserido || PegouPontoInseridoUsuario)
        {
          LLDeltaSup& ListasAtuais(Superficie.PegaListaRestricoes());
          LDeltaSup	LDeltaSupAtual;
          ItLDeltaSup	itPontoRestricao;
          
          //--- Teoricamente as restriçoes sao um tipo de objeto, estão separadas dos objetos porque desenhar objetos foi 
          //--- Incluido muito depois. A FAZER ===>>>> INCLUIR AS  RESTRIÇÕES NOS OBJETOS ou vice versa.....

          if (0 && (DesenhandoObjetos || EsbocandoObjetos || ObjetosDrenagem))
          {
            if (ObjetoAtual != CAciTopografico::NENHUM_ACITOPOG)   //--- Se ja selecionou o obj
            {
              if (AciTopogAtual.PegaLista().size() == 0)
              {
                AciTopogAtual.Limpa();
                AciTopogAtual.MudaObjeto(ObjetoAtual, false);
                PegouPrimPonto = AlterouProjeto = true;
              }

              AciTopogAtual.InserePonto(DeltaSupSobMouse->PegaPCentral());
            }
          }
          else if (!CriandoRestricao && !PegandoPontoSecAvulsa)
          {
            for (ListaAtual = ListasAtuais.begin(); ListaAtual != ListasAtuais.end(); ListaAtual++)
            {
              itPontoRestricao = FindListas(*ListaAtual, DeltaSupSobMouse->PCentral);
              //     if ((itPontoRestricao = std::find((*ListaAtual).begin(), (*ListaAtual).end(), *DeltaSupSobMouse)) != (*ListaAtual).end())
              if (itPontoRestricao != (*ListaAtual).end())
              {
                int ObjetoPontoClicado(Superficie.VerfAciTopografico(ListaAtual));

                ItListaIniTalvPrincipal = ListaAtual;

                if (DefinindoTalveguePrincipal)
                {
                  if (ObjetoPontoClicado == CAciTopografico::TALVEGUE)
                  {
                    //--- Apenas os pontos no início ou no fim das listas dos talvegues são candidatos ao início de um talvegue

                    if (DeltaSupSobMouse->PCentral == ListaAtual->begin()->PCentral || DeltaSupSobMouse->PCentral == ListaAtual->rbegin()->PCentral)
                    {
                      ItListaIniTalvPrincipal = ListaAtual;
                      return;
                    }
                  }
                }
              
                if (ObjetoAtual == ObjetoPontoClicado)    //--- Tem que esta no mesmo objeto
                {
                  //---	Se a tecla control não estiver pressionada remove	o	ponto	da lista,	caso contrário marca o ponto de	quebra

                  if ((nFlags & MK_SHIFT))
                  {
                    if (RemovePontoRestricao(ListaAtual, itPontoRestricao))
                    {
                      //--- Pontos esboçados tem q ser removidos da superfície tb
                      //--- (Já foram em RemovePontoRestricao())

                      if (!EsbocandoObjetos)
                      {
                        if (ObjetoAtual == CAciTopografico::FRONTEIRA)
                        {
                          ((CMainFrame*)AfxGetMainWnd())->FronteiraEditada = true;
                        }

                        ItLLDeltaSup ItLLDS;
                        ItLDeltaSup itPonto;

                        if (Superficie.VerfPontoRestricao(DeltaSupSobMouse->PegaPCentral(), ItLLDS, itPonto) == false) const_cast<CDeltaSuperficie*>(&*DeltaSupSobMouse)->DeixouDeSerRestricao();
                        if (ObjetoAtual == CAciTopografico::BORDO) const_cast<CDeltaSuperficie*>(&*DeltaSupSobMouse)->DeixouDeSerBordo();

                        if (ListaAtual->size() == 0 || (ListaAtual->size() == 1 && VerfObjAvulso(ObjetoAtual) == false))
                        {
                          Superficie.RemoveListaDoMapa(ListaAtual);
                          ListasAtuais.remove(*ListaAtual);			 //--- Se	a	lista	ficou	com	apena	um ponto remove-a
                        }
                        else
                        {
                          //--- Se o ponto é esboçado tem q remover da superficie tb

                          if (PSobMouse.Baliza2 & Ponto::ESBOCADO)  Superficie.RemovePonto(DeltaSupSobMouse->PegaPCentral());
                          if (fabs(DeltaSupSobMouse->PegaPCentral().S - -100.0) < 0.1) Superficie.RemovePonto(DeltaSupSobMouse->PegaPCentral());
                        }
                      }
                      AlterouProjeto = AlterouTerreno = true;
                      RedrawWindow();

                      return;
                    }
                    else return;
                  }
                  else
                  {
                    if (nFlags & MK_CONTROL)
                    {
                      //--- Verifica se esta altarendo uma bacia, se estiver é preciso marcar a bacia como alterada, para recalcular

                      if (ObjetoAtual == CAciTopografico::ESPIGAO)
                      {
                        ittylstBacias ItBaciaAtual(Superficie.ProcuraBacia(ListaAtual));

                        if (ItBaciaAtual != Superficie.PegaBacias().end())
                        {
                          if (AfxMessageBox("Este espigão pertence a uma bacia hidrográfica, ao alterá-lo a bacia hidrográfica deverá ser calculada novamente, confirma?", MB_OKCANCEL) == IDCANCEL)
                          {
                            return;
                          }
                          else
                          {
                            Superficie.PegaBacias().erase(ItBaciaAtual);
                          }
                        }
                      }
                      else
                      {
                        //--- Verifica se esta alterando o talvegue de alguma bacia

                        if (ObjetoAtual == CAciTopografico::TALVEGUE)
                        {
                          ittylstBacias ItBaciaAtual(Superficie.ProcuraTalvegueBacia(DeltaSupSobMouse->PCentral));

                          if (ItBaciaAtual != Superficie.PegaBacias().end())
                          {
                            if (AfxMessageBox("Este talvegue pertence a uma bacia hidrográfica, ao alterá-lo a bacia deverá ser calculada novamente, confirma?", MB_OKCANCEL) == IDCANCEL)
                            {
                              return;
                            }
                            else
                            {
                              ItBaciaAtual->Calculada() = false;
                            }
                          }
                        }
                      }

                      //--- quebra a lista
                      QuebraLista(ListasAtuais, ListaAtual, itPontoRestricao);
                      ContinuarLista = CriandoRestricao = true;
                      DeltaSupSobMouse = Superficie.pSuperficieAtual->find(*itPontoRestricao);
                    }
                    else
                    {
                      if (*itPontoRestricao == *(*ListaAtual).rbegin() && *(*ListaAtual).rbegin() != *(*ListaAtual).begin())   //---e se for fim	da lista e a lista não estiver fechada continua	a	lista
                      {
                        //  if(VerfObjAvulso(ObjetoAtual) == false)   //-- Se for objeto avulso não há lista para continuar
                        {
                          //    ItListaRestAtual = ListasAtuais.end();
                          CriandoRestricao = true;
                          ContinuarLista = true;     //--- Quebrou lista = continuar lista 

                        }
                      }
                      else
                      {
                        if (itPontoRestricao == (*ListaAtual).begin() && *(*ListaAtual).rbegin() != *(*ListaAtual).begin())  //---	Senão, se	for	o	inicio da	lista	e a lista não estiver fechada reverte	a	lista	para continuar do	fim	da lista
                        {
                          (*ListaAtual).reverse();
                          CriandoRestricao = true;
                          ContinuarLista = true;     //--- Quebrou lista = continuar lista 
                        }
                        else
                        {
                          //--- inicia uma nova lista (Esta partindo de um ponto de uma lista do mesmo tipo
                        }
                      }
                    }
                  }
                  if (0)// else   //--- A lista esta fechada
                  {
                    //--- Se Estiver definindo restriçoes verifica se o usuario quer abrir a área ou iniciar outra.

                    if (ObjetoAtual == CAciTopografico::FRONTEIRA)
                    {
                      int Resposta;
                      CDiaAbrirCriarArea DiaAbrirCriarArea(&Resposta, this);

                      DiaAbrirCriarArea.DoModal();

                      if (Resposta == -1)
                      {
                        CriandoRestricao = false;  //--- Se estiver esboçando objetos continua esbocando
                        ItListaRestAtual = Superficie.PegaListaRestricoes().end();
                      }
                      else
                      {
                        if (Resposta == 0)
                        {
                          ObjetoAtual = Superficie.VerfAciTopografico(ListaAtual);
                          QuebraLista(ListasAtuais, ListaAtual, itPontoRestricao);
                          ContinuarLista = CriandoRestricao = true;
                        }
                        else ListaAtual = ListasAtuais.end();
                      }
                    }
                    else   //--- Se estiver definido bordos e a tecla CTRL estiver pressionada, quebra a lista
                    {
                      ObjetoAtual = Superficie.VerfAciTopografico(ListaAtual);
                      QuebraLista(ListasAtuais, ListaAtual, itPontoRestricao);
                      ListaAtual = --ListasAtuais.end();
                      ContinuarLista = true;
                    }
                  }
                  break;  //--- um ponto pode pertencer a mais de uma lista tem que procurar em todas
                }
                else
                {
                  //--- Se o ponto atual estiver na fronteira marca que o ponto anterior foi clicado na fronteira. 
                  //--- (Para verificar se a proxima aresta vai ficar fora da fronteira, o que não pode)
                  //--- Guardar o iterador para facilitar a pesquisa na VerfArestaForaSuperficie     <<<=================

                  if (ObjetoPontoClicado == CAciTopografico::FRONTEIRA)
                  {
                    bool PontoAnteriorFronteira = true;    //--- Por enquanto não está sendo usado
                  }
                }
              }
            }

            //---	Se chegou	aqui é porque não clicou no fim ou no inicio de uma lista existente, então inicia 
            //--- uma nova lista, mesmo que esteja em um ponto de um lista existente, se ele não quebrou uma lista

            //if (ListaAtual ==	ListasAtuais.end()/* || !ContinuarLista*/|| *ListaAtual->begin() == *ListaAtual->rbegin())
            if (!ContinuarLista)
            {
              //--- Pontos inseridos por Delaunay ou Gabriel não podem ser usados para criar restrições.

              //  if(DeltaSupSobMouse->PegaPCentral().Descricao.Find("Ponto Inserido") == -1|| (DeltaSupSobMouse->PegaPCentral().Descricao.Find("Delaunay")== -1 && DeltaSupSobMouse->PegaPCentral().Descricao.Find("Gabriel") == -1))
              //if (ObjetoAtual != CAciTopografico::ESPIGAO)

              //--- Se estiver inserindo espigão, não pode incluir pontos de outro espigão

              if (DeltaSupSobMouse != Superficie.pSuperficieAtual->end() && ObjetoAtual == CAciTopografico::ESPIGAO && DeltaSupSobMouse->TipoObjeto == CAciTopografico::ESPIGAO)
              {
                //          return;
              }


              //            if(1)
              {
                //--- Insere um novo acidente topográfico

                DeltaSupSobMouseAnterior = DeltaSupSobMouse;
                ListasAtuais.push_back(LDeltaSup(1, CDeltaSuperficie(DeltaSupSobMouse->PCentral, ObjetoAtual)));
                ListaAtual = ListasAtuais.end();
                ListaAtual--;

                //--- Todo equipamento de drenagem está  agregado a um acidente topográfico
                //--- A caixa coletora é posicionada com apenas um ponto e tem q estar ligada a outro objeto de drenagem uma escada de descida 
                //--- um bueiro ou uma sarjeta
                //--- O segundo ponto da caixa é definido na construção da caixa

                if (ObjetoAtual == CAciTopografico::EQUI_CAIXA_COLETORA || ObjetoAtual == CAciTopografico::EQUI_BACIA_DISSIPACAO)
                {
                  //--- Objetos que podem ter caixa coletora ou bacia de dissipação

                  if (DeltaSupSobMouse->TipoObjeto == CAciTopografico::EQUI_DESCIDA_DAGUA || DeltaSupSobMouse->TipoObjeto == CAciTopografico::EQUI_TUBULACAO ||
                    DeltaSupSobMouse->TipoObjeto == CAciTopografico::EQUI_SARJETA || DeltaSupSobMouse->TipoObjeto == CAciTopografico::EQUI_CANALETA_TN ||
                    DeltaSupSobMouse->TipoObjeto == CAciTopografico::EQUI_CANALETA_TERRAPL || DeltaSupSobMouse->TipoObjeto == CAciTopografico::EQUI_VALETA_TERRAPL ||
                    DeltaSupSobMouse->TipoObjeto == CAciTopografico::EQUI_VALETA_TN || DeltaSupSobMouse->TipoObjeto == CAciTopografico::EQUI_VALETA_TERRAPL)
                  {
                    tymapEquipDrenagem& EquipamentosDrenagem(Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem());
                    ittymapEquipDrenagem It(EquipamentosDrenagem.begin());

                    while (It != EquipamentosDrenagem.end())
                    {
                      if (VerificaNaTangenteEquipDrenagem(It, DeltaSupSobMouse->PCentral, Toleranciax * 2))
                      {
                        Superficie.InsereListaNoMapa(ListaAtual, ObjetoAtual);

                        if (ObjetoAtual == CAciTopografico::EQUI_CAIXA_COLETORA)
                        {
                          DefineCaixaColetora(NomeEquiAtual, DeltaSupSobMouse, It, ListaAtual);
                        }
                        else
                        {
                          DefineBaciaAmortecimento(NomeEquiAtual, DeltaSupSobMouse, It, ListaAtual);
                        }

                        //--- Insere o 2o ponto da bacia na superficie

                        Ponto& Ponto2Bacia(ListaAtual->rbegin()->PCentral);

                        Ponto2Bacia.Nome.Format("PDES-%d", ++UltPontoEsbocado);
                        Ponto2Bacia.Baliza2 = Ponto::ESBOCADO;
                        Ponto2Bacia.Descricao = CAciTopografico::VecNomeDescricao.find(ObjetoAtual)->Nome().c_str();
                        Ponto2Bacia.AcidenteTopografico = ObjetoAtual;
                        Ponto2Bacia.S = -100.0;                                            //--- Indica ponto esboçado, a baliza 2 não esta sendo gravada no arquivo
                        Ponto2Bacia.z = 0.0;

                        CalculaCotaPonto(Ponto2Bacia);

                        Superficie.InsereDeltaSuperficie(CDeltaSuperficie(Ponto2Bacia, ObjetoAtual));

                        DeltaSupSobMouseAnterior = Superficie.pSuperficieAtual->end();

                        return;
                      }
                      ++It;
                    }
                  }
                  else
                  {
                    //--- Caixas coletoras e bacias de dissipação tem q estar ligadas a uma descida dágua
                    //--- como as listas foram inseridas antes desta verificação remove-a aqui

                      //--- Mover caixas de descida da erro aqui, VERIFICAR (Porisso não esta podendo mover caixas de coleta)

                    if (ListaAtual != ListasAtuais.end())
                    {
                      if (DeltaSupSobMouse != Superficie.pSuperficieAtual->end())
                      {
                        Superficie.RemovePonto(DeltaSupSobMouse->PCentral);
                      }
                      Superficie.RemoveListaDoMapa(ListaAtual);
                      ListasAtuais.remove(*ListaAtual);
                    }

                    DeltaSupSobMouseAnterior = Superficie.pSuperficieAtual->end();

                    return;
                  }
                }

                //    DeltaSupSobMouseAnterior = DeltaSupSobMouse;
                    /*ListasAtuais.push_back(LDeltaSup(1, *DeltaSupSobMouse));
                    ListaAtual = ListasAtuais.end();
                    ListaAtual--;

                    */

                    //--- Se Estiver esboçado (=Desenhando) objetos marca o ponto como ESBOÇADO (=DESENHADO)

                if (DesenhandoObjetos || EsbocandoObjetos || ObjetosDrenagem)
                {
                  if (ObjetoAtual != CAciTopografico::NENHUM_ACITOPOG)    //--- Tem um objeto escolhido?     
                  {
                    if (!ArrastandoEquipDrenagem)
                    {
                      Superficie.InsereListaNoMapa(ListaAtual, ObjetoAtual);
                      //MMapItLLDeltaSup.insert(pairmmapitLLDeltaSup(ObjetoAtual,ListaAtual));   //--- mapeia os Acidentestopograficos -> lista de bordos 
                      //--- por enquanto os acidentes topograficos são bordos <<<  ACERTAR!!! (Passar tudo para acidentes topograficos ... ou vice versa 
                    }
                  }
                  else
                  {
                    //    ListasAtuais.erase(ListaAtual);
                    //--- Mensagem "Escolha um Acid. topog.  a desenhar"
                  }
                }

                CriandoRestricao = true;
                //PontoBordoAnterior = DeltaSupSobMouse;

                AlterouTerreno = true;
              }
              // else AfxMessageBox("Pontos inseridos por Delaunay ou Gabriel não podem ser usados \npara criar restrições ou serem incluídos manualmente nelas.");
            }
            else
            {
              //--- Verifica se esta continuando um talvegue dentro de uma bacia hidrográfica
              //--- Se estiver avisa pra remover a bacia

              if (ObjetoAtual == (CAciTopografico::TALVEGUE))
              {
                ittylstBacias ItBaciaAtual(Superficie.ProcuraTalvegueBacia(DeltaSupSobMouse->PCentral));

                if (ItBaciaAtual != Superficie.PegaBacias().end())
                {
                  if (AfxMessageBox("Este talvegue pertence a uma bacia hidrográfica, ao alterá-lo a bacia deverá ser calculada novamente, confirma?", MB_OKCANCEL) == IDCANCEL)
                  {
                    return;
                  }
                  else
                  {
                    ItBaciaAtual->RemoveFoz();
                    ItBaciaAtual->Calculada() = false;
                  }
                }
              }
            }
          }
          else					//---	else está	criando	restrição	ou pegando ponto sec avulsa
          {
            //---	Se o ponto clicado for o início	de alguma	lista	(exeto a atual), funde as	listas em	1	e	termina	a	edição do	bordo
            //---	Para continuar o usuário deverá	clicar no	final	de uma outra lista 
            //--- Se estiver juntando espigões este fica sendo o ponto de início

            ItLLDeltaSup itLAtual(ListasAtuais.begin());

            for (; itLAtual != ListasAtuais.end() &&
              (*((*itLAtual).begin()) == *((*itLAtual).rbegin()) ||
                PontoXYIgual((*itLAtual).begin()->PCentral, DeltaSupSobMouse->PCentral) == false); //--- Pontos não são iguais
              ++itLAtual)
            {
            }

            //--- Se	achou	o	ponto	clicado	 numa lista do tipo atual e a lista não estiver fechada

            if (itLAtual != ListasAtuais.end() && Superficie.VerfAciTopografico(itLAtual) == ObjetoAtual && *itLAtual->begin() != *itLAtual->rbegin())
            {
              unsigned int TipoRestricao(Superficie.VerfAciTopografico(itLAtual));

              if (VerfObjAvulso(ObjetoAtual) == false &&
                Superficie.VerfGeraArestaForcada(TipoRestricao) == true &&
                Superficie.VerificaIntersecaoAcidentesTopog((*itLAtual).begin()->PegaPCentral(), DeltaSupSobMouse->PegaPCentral(), PontoIntersecao, CAciTopografico::FIM_ACITOPOG) == true)
              {
                monolog.mensagem(-1, "Acidentes topográficos que geram arestas forçadas não podem se interceptar");
                CircularPontoIntersecao = true;
                DeltaSupSobMouse = DeltaSupSobMouseAnterior;
              }
              else
              {
                if (*itLAtual->begin() != *ListaAtual->begin()) 	//--- Um ponto pode ser o inicio e fim de mais de uma lista, porisso não compara os iteradores		
                {
                  if (TipoRestricao == ObjetoAtual && *itLAtual->begin() != *itLAtual->rbegin())  //--- O tipo de objeto é o mesmo e se a lista não esta fechada, funde as 2 listas e termina a edicao
                  {
                    //--- Verifica se o segmento ja existe na lista

                    if (Superficie.VerfInsercaoSgmento(DeltaSupSobMouseAnterior->PCentral, DeltaSupSobMouse->PCentral, ObjetoAtual))
                    {
                      //--- verifica se a inserção é recursiva

                   //   if(*itLAtual->begin() != DeltaSupSobMouse->PCentral)
                      if (1)
                      {
                        //--- OK, funde as listas

                        if (!JuntandoEspigoes)
                        {
                          (*ListaAtual).insert((*ListaAtual).end(), (*itLAtual).begin(), (*itLAtual).end());
                          Superficie.RemoveListaDoMapa(itLAtual, ObjetoAtual, false);
                          ListasAtuais.erase(itLAtual);
                          CriandoRestricao = false;  //--- Se estiver esbocando objetos, continua esboçando
                        }
                      }
                      else
                      {
                        monolog.mensagem(-1, "Fusões recursivas não são permitidas");
                        DeltaSupSobMouse = DeltaSupSobMouseAnterior;
                      }
                    }
                    else DeltaSupSobMouse = DeltaSupSobMouseAnterior;
                  }
                  else
                  {
                    //--- A lista pode ser a mesma mas o segmento não

                    if (Superficie.VerfInsercaoSgmento(DeltaSupSobMouseAnterior->PCentral, DeltaSupSobMouse->PCentral, ObjetoAtual))
                    {
                      (*ListaAtual).push_back(*DeltaSupSobMouse);
                      DeltaSupSobMouseAnterior = DeltaSupSobMouse;
                    }
                  }
                }
                else   //--- Senão: é o inicio da lista que esta sendo criada, insere o ponto e fecha o poligono
                {
                  if ((*ListaAtual).size() > 2)
                  {
                    if (Superficie.VerfInsercaoSgmento(DeltaSupSobMouseAnterior->PCentral, DeltaSupSobMouse->PCentral, ObjetoAtual))
                    {
                      (*ListaAtual).push_back(*DeltaSupSobMouse);
                      DeltaSupSobMouseAnterior = Superficie.pSuperficieAtual->end();
                      CriandoRestricao = false;   //--- Se estiver esboçando objetos continua esboçando
                    }
                  }
                }
              }
            }
            else
            {
              //--- Se estiver juntando espigões não habilita acrescentar ponto no final da lista
              //--- itLAtual continua NULL

              if (!EsbocandoObjetos || !JuntandoEspigoes)
              {
                /*---	Verifica se	o	ponto	clicado	está no	final	de alguma	lista, se	estiver	insere a lista ao	contrario	(do	fim	pro	inicio)
                e	termina	a	edição.
                Caso contrario simplesmente	insere o ponto na	lista		---*/

                for (itLAtual = ListasAtuais.begin(); itLAtual != ListasAtuais.end() && (*(*itLAtual).rbegin()).PCentral.Compara2D(DeltaSupSobMouse->PCentral, 1e-3) == false; itLAtual++);

                //--- lista circular não tem fim, se a lista for circular não pega 

                if (itLAtual != ListasAtuais.end() && *itLAtual->begin() == *itLAtual->rbegin())
                {
                  itLAtual = ListasAtuais.end();
                }
              }

              //--- clicou	no final de	uma	lista, soma	as duas, a segunda invertida,	do começo	para oa	fim		
              //--- A lista não pode estar fechada e tem que ser do mesmo tipo 

              if (!JuntandoEspigoes && itLAtual != ListasAtuais.end())
              {
                unsigned int TipoRestricao(Superficie.VerfAciTopografico(itLAtual));

                if (itLAtual != ListaAtual)
                {
                  if (ObjetosDrenagem == false &&
                    Superficie.VerfGeraArestaForcada(TipoRestricao) == true &&
                    Superficie.VerificaIntersecaoAcidentesTopog((*ListaAtual).rbegin()->PegaPCentral(), DeltaSupSobMouse->PegaPCentral(), PontoIntersecao) == true)
                  {
                    monolog.mensagem(-1, "Acidentes topográficos que geram arestas forçadas não podem se interceptar.");
                    CircularPontoIntersecao = true;
                    DeltaSupSobMouse = DeltaSupSobMouseAnterior;
                  }
                  else
                  {
                    if (ObjetoAtual != CAciTopografico::ESPIGAO)   //--- Espigões são um caso especial por causa do JuntandoEspigões.
                    {
                      if (TipoRestricao == ObjetoAtual)   //-- Se for o mesmo tipo de objeto, funde as listas, Tratamento especial para espigoes que (JuntandoEspigoes)
                      {
                        if (Superficie.VerfInsercaoSgmento(DeltaSupSobMouseAnterior->PCentral, DeltaSupSobMouse->PCentral, ObjetoAtual))
                        {
                          (*ListaAtual).insert((*ListaAtual).end(), (*itLAtual).rbegin(), (*itLAtual).rend());
                          Superficie.RemoveListaDoMapa(itLAtual, ObjetoAtual, false);
                          ListasAtuais.erase(itLAtual);

                          CriandoRestricao = false;  //--- Se estiver esboçando objetos continua esboçando
                          DeltaSupSobMouse = Superficie.pSuperficieAtual->end();
                        }
                        else DeltaSupSobMouse = DeltaSupSobMouseAnterior;
                      }
                      else     //--- Senão simplesmente insere o ponto na lista
                      {
                        if (Superficie.VerfInsercaoSgmento(DeltaSupSobMouseAnterior->PCentral, DeltaSupSobMouse->PCentral, ObjetoAtual))
                        {
                          DeltaSupSobMouseAnterior = DeltaSupSobMouse;
                          (*ListaAtual).push_back(*DeltaSupSobMouse);											 //--- senão Insere	o	ponto	na lista q está	sendo	criada
                          (*ListaAtual).back().TipoObjeto = ObjetoAtual;  //--- O ponto pego pode ser parte de outro objeto
                        }
                      }
                    }
                    else
                    {
                      DeltaSupSobMouseAnterior = DeltaSupSobMouse;
                      (*ListaAtual).push_back(*DeltaSupSobMouse);
                    }
                  }
                }
                else
                {
                  if (ObjetosDrenagem == true &&
                    (Superficie.VerfGeraArestaForcada(TipoRestricao) == false ||
                      Superficie.VerificaIntersecaoAcidentesTopog((*ListaAtual).rbegin()->PegaPCentral(), DeltaSupSobMouse->PegaPCentral(), PontoIntersecao) == false))
                  {
                    //--- Verifica se não é o mesmo ponto

                    if (*(*ListaAtual).rbegin() != (*DeltaSupSobMouse))
                    {
                      if (Superficie.VerfInsercaoSgmento(DeltaSupSobMouseAnterior->PCentral, DeltaSupSobMouse->PCentral, ObjetoAtual))
                      {
                        DeltaSupSobMouseAnterior = DeltaSupSobMouse;
                        (*ListaAtual).push_back(*DeltaSupSobMouse);											 //--- senão Insere	o	ponto	na lista q está	sendo	criada
                        if (itLAtual != ListasAtuais.end())
                        {
                          DeltaSupSobMouseAnterior = Superficie.pSuperficieAtual->end();
                          CriandoRestricao = false;		 //--- inseriu o inicio	da lista,	termina	a	edição,	pois fechou	a	poligonal	dos	bordos
                        }                              //--- Se estiver esboçando objetos continua esboçando
                      }
                    }
                  }
                  else
                  {
                    monolog.mensagem(-1, "Acidentes topográficos que geram arestas forçadas não podem se interceptar.");
                    CircularPontoIntersecao = true;
                    DeltaSupSobMouse = DeltaSupSobMouseAnterior;
                  }
                }
              }
              else     //--- O ponto clicado não esta no inicio nem no fim de uma lista. (pode estar no meio de uma lista) simplesmente insere o ponto na lista
              {
                if (ListaAtual != Superficie.PegaListaRestricoes().end())
                {
                  if (ObjetosDrenagem == true ||
                    VerfObjAvulso(ObjetoAtual) == true ||
                    Superficie.VerfGeraArestaForcada(ObjetoAtual) == false ||
                    Superficie.VerificaIntersecaoAcidentesTopog((*ListaAtual).rbegin()->PegaPCentral(),
                      DeltaSupSobMouse->PegaPCentral(),
                      PontoIntersecao,
                      CAciTopografico::NENHUM_ACITOPOG,
                      false,
                      true,
                      nullptr,
                      &ListaAtual,
                      &ListaAtual->end()) == false)
                  {
                    //--- A Aresta vai ser inserida na lista. verifica se a aresta não vai ficar fora da superficie.

                    if (Superficie.VerfGeraArestaForcada(ObjetoAtual) == false ||
                      (Superficie.VerfArestaForaSuperficie((*ListaAtual).rbegin()->PegaPCentral(), DeltaSupSobMouse->PegaPCentral()) == false))
                    {
                      //--- Verifica se o ponto clicado não é o inicio da lista, se for fecha a lista e sai

                      if (*(*ListaAtual).begin() == (*DeltaSupSobMouse)/* && ObjetoAtual != CAciTopografico::ESPIGAO*/)   //--- Espigões não podem possuir pontos de outros espigões, nem o primeiro (este seria o primeiro)
                      {
                        if ((*ListaAtual).size() > 1)    //--- Clicou no mesmo ponto do inicio mas a lista tem q ser > 1
                        {
                          if (Superficie.VerfInsercaoSgmento(DeltaSupSobMouseAnterior->PCentral, DeltaSupSobMouse->PCentral, ObjetoAtual))
                          {
                            (*ListaAtual).push_back(*DeltaSupSobMouse);
                            DeltaSupSobMouseAnterior = Superficie.pSuperficieAtual->end();
                            CriandoRestricao = false;    //--- Se estiver esboçando objetos continua esbocando
                          }
                        }
                      }
                      else
                      {
                        if (*(*ListaAtual).rbegin() != (*DeltaSupSobMouse) &&                            //--- Verifica se não é o mesmo ponto atual ou o ponto anterior
                          ((*ListaAtual).size() == 1 || (*++(*ListaAtual).rbegin() != (*DeltaSupSobMouse))))
                        {
                          if (DeltaSupSobMouseAnterior != Superficie.pSuperficieAtual->end())
                          {
                            lstItCLLDeltaSup LstRestricoesNoPonto;
                            Superficie.PegaRestricoesNoPonto(DeltaSupSobMouse->PCentral, LstRestricoesNoPonto);

                            if (LstRestricoesNoPonto.size() > 0)
                            {
                              if (EsbocandoObjetos && ObjetoAtual == CAciTopografico::ESPIGAO && DeltaSupSobMouse->TipoObjeto == ObjetoAtual)
                              {
                                if (!JuntandoEspigoes)
                                {
                                  JuntandoEspigoes = true;
                                  IniciaJuntandoEspigoes_2(*LstRestricoesNoPonto.begin());
                                  ItInicioJuntandoEspigoes = DeltaSupSobMouse;
                                }
                                else
                                {
                                  if (!FinalizaJuntandoEspigoes_2(LstRestricoesNoPonto, DeltaSupSobMouse))
                                  {
                                    LstRestricoesNoPonto.pop_front();
                                    if (LstRestricoesNoPonto.size() > 0)    //--- Existem mais espigões a verificar?
                                    {
                                      JuntandoEspigoes = false;
                                    }
                                    else
                                    {
                                      //--- Não existe um espigão entre os dois pontos passados
                                      AfxMessageBox("Caminho inexistente, tente clicando nas biburcações entre o ponto inicial e o ponto final");
                                      LstRestricoesNoPonto.clear();
                                      JuntandoEspigoes = false;
                                    }
                                  }
                                  else
                                  {
                                    LstRestricoesNoPonto.clear();
                                    JuntandoEspigoes = false;
                                  }
                                }
                              }
                              else
                              {
                                //--- se OK Insere o ponto	na lista que está	sendo	criada
                                //--- DeltaSupSobMouse = Superficie.pSuperficieAtual->end();
                                ListaAtual->push_back(CDeltaSuperficie(DeltaSupSobMouse->PCentral, ObjetoAtual));
                              }
                            }
                            else
                            {
                              ListaAtual->push_back(CDeltaSuperficie(DeltaSupSobMouse->PCentral, ObjetoAtual));
                            }

                            DefinindoObjDrenagem = true;

                            switch (ObjetoAtual)
                            {
                            case CAciTopografico::EQUI_CANALETA_TERRAPL:
                            case CAciTopografico::EQUI_CANALETA_TN: DefineCanaleta(ListaAtual, NomeEquiAtual); break;
                            case CAciTopografico::EQUI_VALETA_TERRAPL:
                            case CAciTopografico::EQUI_VALETA_TN: DefineSarjetaTN(ListaAtual, NomeEquiAtual); break;
                            case CAciTopografico::EQUI_DRENO_PROFUNDO:
                            case CAciTopografico::EQUI_TUBULACAO: DefineTubulacao(ListaAtual, NomeEquiAtual); break;
                            case CAciTopografico::EQUI_BUEIRO: DefineBueiro(ListaAtual, NomeEquiAtual); break;
                            case CAciTopografico::EQUI_BUEIRO_RETANG: DefineBueiroRetangular(ListaAtual, NomeEquiAtual); break;
                            case CAciTopografico::EQUI_DESCIDA_DAGUA: DefineDescidaDagua(ListaAtual, NomeEquiAtual); break;
                            }

                            if (VerfObjCanto(ObjetoAtual) == true)
                            {
                              DeltaSupSobMouseAnterior = Superficie.pSuperficieAtual->end();
                              CriandoRestricao = false;               //--- A lista de cantos tem só 2 pontos
                            }                                         //--- Se estiver esboçando objetos continua esbocando  
                            else DeltaSupSobMouseAnterior = DeltaSupSobMouse;
                          }
                        }
                      }
                    }
                    else
                    {
                      DeltaSupSobMouse = DeltaSupSobMouseAnterior;
                      monolog.mensagem(-1, "Não podem ocorrer acidentes topográficos que geram arestas forçadas fora da superfície.");
                    }
                  }
                  else
                  {
                    monolog.mensagem(-1, "Acidentes topográficos que geram arestas forçadas não podem se interceptar.");
                    CircularPontoIntersecao = true;
                    DeltaSupSobMouse = DeltaSupSobMouseAnterior;
                  }
                }
              }
            }
          }
          RedrawWindow();
          AlterouTerreno = true;

          if (PegouPonto) SalvaUltimoPontoResPixels(DeltaSupSobMouse);				//---	Salva	o	ultimo ponto pego	para poder desenhar	o	elástico

          //if (DeltaSupSobMouseAnterior == Superficie.pSuperficieAtual->end()) //--- Está ocorrendo Map sets iterator incompatible as vezes
          DeltaSupSobMouseAnterior = DeltaSupSobMouse;
        }
        else
        {
          if (DesenharPontosInseridos != 0)
            monolog.mensagem(-1, "Pontos inseridos por Delaunay ou por Delaunay-Gabriel não podem ser usados para criar restrições ou serem incluídos manualmente nelas.");
        }
      }
    }
  }
  else //--- não está	sobre	um ponto de	terreno ou nao esta desenhando objetos (restriçoes)
   //   if(!CriandoRestricao)   //Se chegar aqui e estiver criando Restricoes é porque clicou no fundo, termina de criar a restricao atual(CriandoRestricao = false no else)
  {
    //---	Se clicou	com	o	botão	direito	no centro	da curva ou	no PI, mostra	o	dialogo	para editar	os dados da	curva, 
    //---	Se o mouse estiver sobre o pi, pega-o	e	busca	a	curva	no set de	CC,	para evitar	dobrar o código	pois
    //---	a	comparação é feita com o iterador	para o set de	CC,	como a operação	é	rara e lenta não tem problema

    //---	Verifica se	está sobre um	PI ou	um CC. Verifica em todos os trechos

    std::string strTrechoAtualPICC;

    ItCurHorizSendoArrastadaPI = PegaPIMaisProximo(CurHorizontal(INFINITO, PSobMouse.x, PSobMouse.y), strTrechoAtualPICC);

    if (!ItCurHorizSendoArrastadaPI)
    {
      ItCurHorizSendoArrastadaCC = PegaCCMaisProximo(CurHorizontal(INFINITO, PSobMouse.x, PSobMouse.y), strTrechoAtualPICC);

      //---	Se estiver com o control pressionado o usuário deseja	editar a curva então pega	a	curva	pelo pi
      //---	apesar de	o	mouse	estar	sobre	o	centro

      if ((nFlags & MK_CONTROL) && ItCurHorizSendoArrastadaCC)
        ItCurHorizSendoArrastadaPI = PegaPIMaisProximo(CurHorizontal((*ItCurHorizSendoArrastadaCC)->second->PI.x, (*ItCurHorizSendoArrastadaCC)->second->PI.y), strTrechoAtualPICC);
    }

    //---	O	primeiro click em	cima de	um ponto inicia	o	arrasto, no	segundo	click
    //---	finaliza o arrasto,	no primeiro	click	ArrastandoPI fica	true até que o usuário click novamente
    //---	quando Terminararrasto fica	true,	o	arrasto	termina	efetivamente quando	o	usuário	tira o dedo	do 
    //---	botão	do mouse no	método onlbuttonup.	

    //---	Se já estiver	arrastando coloca	true no	terminararrasto	para que o arrasto possa ser terminado (no método	onlbuttonup) e retorna
    //---	não	há nada	mais a fazer.	

    if (!ArrastandoPI)
    {
      if (ItCurHorizSendoArrastadaPI)		//---	Está sobre algum ponto de	projeto?
      {
        if (nFlags & MK_CONTROL)
        {
          CurHorizontal	CurvaEditada(*(*ItCurHorizSendoArrastadaPI)->second),
            CurvaSalva(*(*ItCurHorizSendoArrastadaPI)->second);

          if (Superficie.EstaRotacionada()) CurvaEditada.PI.RotacionaPlano(sin(-Superficie.PegaAnguloRotacao()), cos(-Superficie.PegaAnguloRotacao()), -Superficie.PegaAnguloRotacao());

          //--- Verifica se é o primeiro ou o ultima curva)

          ItCMCurHorizontaisTS itPrimUltimo(Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS.find((*ItCurHorizSendoArrastadaPI)->second->EstTs));

          /*
          (NaoPermitirAlteracoes ? CDEditaCurhoriz::PROIBIDO_ALTERAR :
          (itPrimUltimo == mapCurvasHorizontaisTS.begin() || itPrimUltimo == --mapCurvasHorizontaisTS.end()) ? CDEditaCurhoriz::PI_INICIAL_FINAL :
          CDEditaCurhoriz::PI_NORMAL);
          */

          CDEditaCurhoriz	DiaEditaCurvaHorizontal(this, &CurvaEditada, AnguloRotacao, NaoPermitirAlteracoes ? CDEditaCurhoriz::PROIBIDO_ALTERAR : (itPrimUltimo == Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS.begin() || itPrimUltimo == --Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS.end()) ? CDEditaCurhoriz::PI_INICIAL_FINAL : CDEditaCurhoriz::PI_NORMAL);
          if (Superficie.EstaRotacionada()) PSobMouse.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());

          if (DiaEditaCurvaHorizontal.DoModal() == IDOK)
          {
            ((CMainFrame*)AfxGetMainWnd())->TrocaRua(strTrechoAtualPICC);
            Trechos.TrocaTrecho(strTrechoAtualPICC);  //--- O PI pode ser de outra rua q não a atual

            ItCMCurHorizontaisPI ItCCurvaPI(Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisPI.find(&(*ItCurHorizSendoArrastadaPI)->second->PI));

            if (CurvaEditada.PI != (*ItCurHorizSendoArrastadaPI)->second->PI || CurvaEditada.Raio != (*ItCurHorizSendoArrastadaPI)->second->Raio || CurvaEditada.Lc != (*ItCurHorizSendoArrastadaPI)->second->Lc ||
              CurvaEditada.Se != (*ItCurHorizSendoArrastadaPI)->second->Se || CurvaEditada.Sl != (*ItCurHorizSendoArrastadaPI)->second->Sl)
            {
              CurvaEditada.K = CurvaEditada.Raio * CurvaEditada.Lc;
              CEstaca NovaEstTsCurAnterior;

              //--- Troca a curva anterior pela atual  

              bool Trocou(false);

              CurHorizontal CurvaRemovida(*ItCCurvaPI->second);
              int Resp((Trechos.PegaTrechoAtual()->second.CurvasHorizontais.RemoveCurvaDosMapas(ItCCurvaPI, ItUltimaCurvaAjustadaTS, &NovaEstTsCurAnterior)));

              if (Resp == (int)true)
              {
                //--- No primeiro PI NovaEstTsCurAnterior fica com INFINITO

                if (NovaEstTsCurAnterior.EstVirtual != INFINITO) CurvaEditada.EstTs = CurvaEditada.EstSc = CurvaEditada.EstCs = CurvaEditada.EstSt = CurvaEditada.EstPc = CurvaEditada.EstPt = NovaEstTsCurAnterior + 0.005;
                Trechos.PegaTrechoAtual()->second.CurvasHorizontais.InsereCurvaNosMapas(CurvaEditada);

                static ItCMCurHorizontaisPI itCMPI;

                itCMPI = (Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisPI.find(&CurvaEditada.PI));

                //--- em teste <<<<<<<<<<<<<<<<<<<<<<<<<<<<

                ItCurHorizSendoArrastadaPI = &itCMPI;

                //--- tudo se passa como se a curva tivesse sido alterada pelo PI

                if ((Resp = Trechos.PegaTrechoAtual()->second.CurvasHorizontais.AlteraCurvaPI(*ItCurHorizSendoArrastadaPI, Ponto(CurvaEditada.PI), ItUltimaCurvaAjustadaTS, &NovaEstTsCurAnterior)) != CCurHorizontais::CURVA_OK)
                  monolog.mensagem(Resp, " a alteração não será efetuada.");	//---	Erro no	cálculo	da curva
                else Trocou = AlterouProjeto = true;
              }
              else monolog.mensagem(Resp, " a alteração não será efetuada.");

              if (!Trocou)     //--- oops!!!!
              {
                if (!Trechos.PegaTrechoAtual()->second.CurvasHorizontais.RemoveCurvaDosMapas(*ItCurHorizSendoArrastadaPI, ItUltimaCurvaAjustadaTS, &NovaEstTsCurAnterior))
                {
                  AfxMessageBox("Erro Irrecuperável, contate o suporte - ERRDESGEO-001");
                }

                if (!Trechos.PegaTrechoAtual()->second.CurvasHorizontais.InsereCurvaNosMapas(CurvaRemovida))
                {
                  AfxMessageBox("Erro Irrecuperável, contate o suporte - ERRDESGEO-002");
                }
                ItCMCurHorizontaisPI itCurPI(Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisPI.find(&CurvaRemovida.PI));

                if (itCurPI == Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisPI.end())
                {
                  AfxMessageBox("Erro Irrecuperável, contate o suporte - ERRDESGEO-003");
                }

                if (Trechos.PegaTrechoAtual()->second.CurvasHorizontais.AlteraCurvaPI(itCurPI, itCurPI->second->PI, ItUltimaCurvaAjustadaTS, &NovaEstTsCurAnterior) != CCurHorizontais::CURVA_OK)
                {
                  AfxMessageBox("Erro Irrecuperável, contate o suporte - ERRDESGEO-004");
                }
              }
              RedrawWindow();
            }
          }
        }
        else
        {
          //   if(!NaoPermitirAlteracoes)
          {
            BackCurvaSendoArrastada = *(*ItCurHorizSendoArrastadaPI)->second;								 //--- Salva os	valores	originais	ca durva que esta	sendo	editada
            ArrastandoPI = true;
            if (NaoPermitirAlteracoes && PermitirArrastar) Arrastando = true;
            ((CMainFrame*)AfxGetMainWnd())->TrocaRua(strTrechoAtualPICC);
            Trechos.TrocaTrecho(strTrechoAtualPICC);
          }
        }
      }
      else	 //--- não está	sobre	um PI
      {
        if (!ArrastandoCC)
        {
          if (/*!NaoPermitirAlteracoes	&&*/ ItCurHorizSendoArrastadaCC)				//---	Está sobre algum ponto CC?
          {
            BackCurvaSendoArrastada = *(*ItCurHorizSendoArrastadaCC)->second;							//---	Salva	os valores originais ca	durva	que	esta sendo editada
            ArrastandoCC = true;
            if (NaoPermitirAlteracoes & PermitirArrastar) Arrastando = true;
            ((CMainFrame*)AfxGetMainWnd())->TrocaRua(strTrechoAtualPICC);
            Trechos.TrocaTrecho(strTrechoAtualPICC);
          }
          else
          {
            //---	Clicou no	fundo, 
            //---	Ou Esta	iniciando	ou finalizando uma seção avulsa	ou está	inicialndo ou	finalizando	o	arrastando o desenho.
            //---	se estiver iniciando a seção avulsa	pega i primeiro	ponto	senão	finaliza e pega	o	segundo	ponto.
            //---	Senão	se estiver arrastando	o	desenho	termina	o	arrasto, caso	contrário	inicia o arrasto do	desenho.

            if (PegandoPontoSecAvulsa)
            {
              if (!PegouPrimPonto)
              {
                UltimoPontoPego = point;
                TransfPixelsXY(point, PrimeiroPontoSecAvulsa.x, PrimeiroPontoSecAvulsa.y);	//---	Pega as	coordenadas	(em	metros)	
                PegouPrimPonto = true;

                RedrawWindow();
              }
              else
              {
                TransfPixelsXY(point, SegundoPontoSecAvulsa.x, SegundoPontoSecAvulsa.y);
                PegouPrimPonto = PegandoPontoSecAvulsa = false;
                AlterouProjeto = true;

                CSecaoAvulsa SecAvTemp(strNomeSecAvulsa, PrimeiroPontoSecAvulsa.x, PrimeiroPontoSecAvulsa.y, SegundoPontoSecAvulsa.x, SegundoPontoSecAvulsa.y, 0, 0);
                Ponto CoordPolares(Mat.R_P(PrimeiroPontoSecAvulsa, SegundoPontoSecAvulsa));

                Ponto PEixo;
                double EstacaEixo;

                std::pair<double, bool> CoorPolares(VerfEsconsidade(SecAvTemp.PInicial, SecAvTemp.PFinal, EstacaEixo, PEixo));

                SecAvTemp.Azimute = CoordPolares.x;
                SecAvTemp.Distancia = CoordPolares.y;
                SecAvTemp.PInicial.i = PEixo.x;                       //--- As coordenadas do eixo vão em i do pinicial e final
                SecAvTemp.PFinal.i = PEixo.y;
                SecoesAvulsas.InsereOrdenada(SecAvTemp, true);
                AlterouSecoesAvulsas = true;

                RedrawWindow();
              }
            }
            else
            {
              //--- Se estiver criando algum canto cria o ponto onde o canto termina, e finaliza a lista, esta lista tem só 2 pontos

              if (CriandoRestricao && VerfObjCanto(ObjetoAtual) == true)
              {
                //      double Coordx(INFINITO),Coordy(INFINITO);
                //  TransfPixelsXY(CPoint(PSobMouse.x,PSobMouse.y),Coordx,Coordx);

                CDeltaSuperficie DeltaSuperInseridaUsuario(PSobMouse.x, PSobMouse.y, 0.0, INFINITO, INFINITO, INFINITO, CString("Ponto Inserido Usuário - "), false, ObjetoAtual);

                switch (ObjetoAtual)
                {
                case CAciTopografico::CANTO_CASA: DeltaSuperInseridaUsuario.PegaPCentralNC().Descricao += "Canto Casa."; break;
                case CAciTopografico::CANTO_CERCA: DeltaSuperInseridaUsuario.PegaPCentralNC().Descricao += "Canto Cerca."; break;
                case CAciTopografico::CANTO_MURO: DeltaSuperInseridaUsuario.PegaPCentralNC().Descricao += "Canto Muro."; break;
                default: DeltaSuperInseridaUsuario.PegaPCentralNC().Descricao += "Desconhecido."; break;
                }

                //  DeltaSupSobMouse = Superficie.InsereDeltaSuperficie(DeltaSuperInseridaUsuario);
                //  if(DeltaSupSobMouse != Superficie.pSuperficieAtual->end())
                {
                  //ListaAtual->push_back(DeltaSuperInseridaUsuario);
                  ListaAtual->push_back(CDeltaSuperficie(DeltaSuperInseridaUsuario.PCentral, ObjetoAtual));

                }
                DeltaSupSobMouse = Superficie.pSuperficieAtual->end();
                CriandoRestricao = false;                               //--- Se estiver esboçando objetos continua esbocando
                RedrawWindow();
              }
              else
              {
                //--- Por ultimo verifica se esta inserido sarjetas nos offsets

                if (ObjetosDrenagem && ItPontoTerrap != Trechos.PegaTrechoAtual()->second.SuperficieTerr.pSuperficieAtual->end())
                {
                  // if(!CEquipDrenagem::EquipAgregado(ObjetoAtual))
                  {
                    if (!IniciouSarjeta)
                    {
                      ItInicioSarjeta = ItPontoTerrap;
                      IniciouSarjeta = true;
                      PonMousePixelsAtual = point;
                      PrimeiroMovimento = true;
                      SalvaUltimoPontoResPixels(ItPontoTerrap, true);
                    }
                    else
                    {
                      ItFimSarjeta = ItPontoTerrap;
                      NomeEquiAtual.clear();
                      DefineEquipDrenAlinhamento();
                      IniciouSarjeta = PrimeiroMovimento = false;
                    }
                  }
                  return;
                }

                //--- Clicou no fundo, arrasta o desenho

                Ponto PAtual;

                TransfPixelsXY(point, PAtual.x, PAtual.y);

                if (!TerminarArrasto && PermitirArrastar)
                {
                  ItCLLDeltaSup ItLRestricaoAtual(VerfClickRestricao(PAtual));

                  if (ItLRestricaoAtual != Superficie.PegaListaRestricoes().end())
                  {
                    unsigned int TipoRestricao(Superficie.VerfAciTopografico(ItLRestricaoAtual));

                    CPoint PAux(point);
                    ClientToScreen(&PAux);

                    DPopUpPonto.MostraDescricao(PAux.x, PAux.y - 60, PreparaDescricao((void*)CAciTopografico::Nome(TipoRestricao), false, OBJ_TEXTO));
                  }
                  else
                  {
                    if (!OnLButtonDownTerr(point))
                    {
                      if (!Transladando)
                      {
                        if (!Arrastando && !ArrastandoEquipDrenagem)
                        {
                          Arrastando = true;
                          InicioArrast = point;
                        }
                        else
                        {
                          Arrastando = false;
                        }
                      }
                    }
                  }
                }
                else Arrastando = false;
              }
            }
          }
        }
        else ArrastandoCC = false;
      }
    }
    else ArrastandoPI = false;
  }

  //  else CriandoRestricao = EsbocandoObjetos = false;
  //---	Se não estiver arrastando	PI ou	CC e não for o primeiro	movimento	é	porque este	foi	o	último movimento do	arrasto
  //---	Redesenha	a	janela e prepara a variável	Primeiro movimento para	o	próximo	arrasto.

  if (!ArrastandoPI && !ArrastandoCC)
  {
    if (!SincPontosControle)
    {
      if (!PrimeiroMovimento) RedrawWindow();	 //--- Se	não	estiver	arrastando nada	e	não	for	o	primeiro movimento
      PrimeiroMovimento = true;	               //--- Prepara para o proximo primeiro movimento 
    }

    Ponto PAtual;

    TransfPixelsXY(point, PAtual.x, PAtual.y);

    if (Superficie.EstaRotacionada()) PAtual.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());

    CRect Rect;
    GetWindowRect(&Rect);

    if (!DefinindoObjDrenagem)
    {
      if (ArrastandoEquipDrenagem)
      {
        PAtual.Nome.Format("PDES-%d", ++UltPontoEsbocado);
        PAtual.Baliza2 = Ponto::ESBOCADO;
        PAtual.Descricao = CAciTopografico::VecNomeDescricao.find(ObjetoAtual)->Nome().c_str();
        PAtual.AcidenteTopografico = ObjetoAtual;
        PAtual.S = -100.0;                              //--- Indica ponto esboçado, a baliza 2 não esta sendo gravada no arquivo
        PAtual.z = 0.0;

        CalculaCotaPonto(PAtual, Superficie.EstaRotacionada());

        Ponto PPego(*ItEquiDrenagemMarcada->second.ItPontoPego);

        RedefineEquipDrenagem(ItEquiDrenagemMarcada, PAtual);

        //--- Remove da superf o ponto anterior  (q foi alterado)

       // Superficie.RemovePontoInseridoUsuario(PegaPontoMaisProximo(PPego));

        ArrastandoEquipDrenagem = CriandoRestricao = false;

        DeltaSupSobMouseAnterior = Superficie.pSuperficieAtual->end();  //--- Deltasupsobmouse anterior fica apontando para outra superficie, acerta aqui

        RedrawWindow();

        return;
      }

      Ponto PAtual2(PAtual);

      //if(Superficie.EstaRotacionada()) PAtual2.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());

      if (ObjetosDrenagem && (EditandoObjDrenagem && VerificaEquipDrenagem(point, PAtual2, Rect)))  //--- Para arrastar obj drrenagem não pode estar criando obj drenagem
      {
        if (1)//  ObjetoRasc == ObjetoAtual)    //--- So arrasta se estiver no mesmo tipo
        {
          //ObjetoAtual = ObjetoRasc;
          ArrastandoEquipDrenagem = true;
          UltimoPontoPego = point;

          //--- Clicou no fundo editando drenagem então
          //--- Não é o inicio de um equipamento de drenagem novo, deleta o acidente topografico agregado que 
          //--- Está sendo criado (a lista so tem 1 ponto)

          if (Superficie.PegaListaRestricoes().rbegin()->size() == 1)
          {
            Superficie.RemoveListaDoMapa(--(Superficie.PegaListaRestricoes().end()));
            Superficie.PegaListaRestricoes().pop_back();
            ListaAtual = Superficie.PegaListaRestricoes().end();
          }
          //--- é o primeiro ponto que já está na superfície
        }
        else ItEquiDrenagemMarcada = Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end();
      }
    }
  }

  if (!CriandoRestricao)
    DeltaSupSobMouseAnterior = Superficie.pSuperficieAtual->end();

  //--- Termina os objetos de drenagem compostos por 2 pontos;

  if (DefinindoObjDrenagem && ObjetosDrenagem)
  {
    switch (ObjetoAtual)
    {
    case CAciTopografico::EQUI_BUEIRO:
    case CAciTopografico::EQUI_BUEIRO_RETANG:
    case CAciTopografico::EQUI_DESCIDA_DAGUA:
    {
      DefinindoObjDrenagem = false;
    }
    }
  }

  //--- Clicou sobre o mouse com o botão esquerdo + shift, mostra o menu de alterar descrição
  //--- Senão clicou no fundo.

  if ((nFlags & MK_SHIFT) && PegouPonto)
  {
    CMenu Menu;

    if (Menu.LoadMenu(IDR_MENUPONTO))
    {
      CMenu* pPopup(Menu.GetSubMenu(0));
      if (pPopup != NULL)
      {
        CPoint PAux(point);
        UltimoPontoPego = PAux;
        ClientToScreen(&PAux);
        pPopup->TrackPopupMenu(TPM_TOPALIGN | TPM_LEFTALIGN, PAux.x + 5, PAux.y + 5, AfxGetMainWnd());
        return;
      }
    }
  }

  //--- timer para pegar a parada do mouse, criando o evento ON_MOUSE_STOP

  if (m_nTimer == NENHUM_TIMER && (Arrastando || ArrastandoPI))
  {
    m_nTimer = SetTimer(EVENT_MOUSE_STOP, 100, NULL);
  }
}

void DesPontosGeomView::OnEnquadrar()
{
  //--- Passa as escalas e os deltas para o default

  Escala[X] = EscalaDefault[X];
  Escala[Y] = EscalaDefault[Y];

  DefineParametros();                   //--- Para calcular os deltas default

  DeltasReais.x = Deltas[X] = DeltasDefault[X];
  DeltasReais.y = Deltas[Y] = DeltasDefault[Y];

  RazaoHV = 1.0;

  OnArrastouDesenho();                  //--- Tudo se passa como se estivesse arrastado o desenho tb...

  //--- Reajusta a escala no deslizante

  ((CChildFrame*)GetParentFrame())->DeslizanteGeom.MudarEscala(Escala[X]);

  MudarEscala(Escala[X] * 100.0);    //--- Acerta o mapa

  RedrawWindow();
}

//--- PASSAR ESTE METODO PARA A CLASSE SUPERFICIE
ItSSuperficie DesPontosGeomView::PegaPontoMaisProximo(Ponto PMouse, 
                                                      ItSSetPontos* pItPontoDesabMaisProximo, 
                                                      ItSSetPontos* pItPontoInsMaisProximo,
                                                      Ponto** pFozBacia, 
                                                      ittymapEquipDrenagem* pOAC, 
                                                      ItSSuperficie* pItPTerrap,
                                                      ItSSetPontos* pItPontoSoltoMaisProximo,
                                                      itlstPontos* pItPontoProcMaisProx,
                                                      int Forcar)
{
  double MenorDistancia(INFINITO);
  int TipoSuperf(((CMainFrame*)AfxGetMainWnd())->TipoSuperficie);
  itPonMaisProximo = Superficie.pSuperficieAtual->end();

  //--- Primeiramente verifica se está em alguma foz/CG de uma bacia hidrografica, se estiver este ponto é prioritário

  ittylstBacias it(Superficie.PegaBacias().begin());

  for (; it != Superficie.PegaBacias().end(); it++)
  {
    if (it->EstaCalculada())
    {
      if (fabs(it->PegaFoz().x - PMouse.x) < Toleranciax && fabs(it->PegaFoz().y - PMouse.y) < Toleranciax) 
      {
        PMouse.Baliza2 = PONTO_FOZ_BACIA;
        break;
      }
      else
      if (fabs(it->CG().x - PMouse.x) < Toleranciax && fabs(it->CG().y - PMouse.y) < Toleranciax) 
      {
        PMouse.Baliza2 = PONTO_CG_BACIA;
        break;
      }
    }
  }

  if (it != Superficie.PegaBacias().end())  //--- Está em uma foz ou CG de uma bacia, dá prioridade a este ponto
  {
    static Ponto PontoFoz_CG;

    if(PMouse.Baliza2 == PONTO_FOZ_BACIA)  PontoFoz_CG = it->PegaFoz();
    else PontoFoz_CG = it->CG();

    *pFozBacia = &PontoFoz_CG;

    PontoFoz_CG.Baliza2 = PMouse.Baliza2;

    return Superficie.pSuperficieAtual->end();
  }

  //--- Se estiver desenhando uma sargeta de equipamentos de drenagem verifica se clicou em algum offset de terraplanagem
  //--- se clicou inicia (ou termina a sargeta nele)

  double MenorDistanciaTerr(INFINITO);

  ItSSuperficie ItPontoMaisProxTerr(Trechos.PegaTrechoAtual()->second.SuperficieTerr.pSuperficieAtual->end());
  SSuperficie* pSupTerr(Trechos.PegaTrechoAtual()->second.SuperficieTerr.pSuperficieAtual);  //--- Para pontos da terraplanagem
  tymapEquipDrenagem OACs(Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem());
  ittymapEquipDrenagem ItOACMaisProx(OACs.end());
  ItSSuperficie itDSupTerrFinal(pSupTerr->end()), itDSupTerrAtual(pSupTerr->end());          //--- Idem      Idem
  itlstPontos ItpProcuradoMaisProximo(LPontosProcurados.end());

  if (ObjetosDrenagem && pItPTerrap)   //--- Está desenhando a drenagem é pra considerar a terraplanagem?, se sim verifica se pegou ponto na Terraplanagem, para desenhar sarjetas no offset
  {
    *pItPTerrap = pSupTerr->end();

    PMouse.x += Toleranciax;

    ItSSuperficie itDSupTerrFinal = pSupTerr->upper_bound(PMouse);

    PMouse.x -= Toleranciax * 2.0;
    itDSupTerrAtual = pSupTerr->upper_bound(PMouse);         //--- Pega o primeiro na tolerancia    
    PMouse.x += Toleranciax;                                 //--- Restaura o valor original

    //--- Separa o ponto de terraplanagem que está mais próximo do mouse 

    while (itDSupTerrAtual != itDSupTerrFinal)
    {
      //--- Desconsidera os pontos de terreno que estão na terraplanagem

      if (itDSupTerrAtual->TipoObjeto > CAciTopografico::INICIO_TERRAP && itDSupTerrAtual->TipoObjeto < CAciTopografico::FIM_TERRAP)
      {
        Ponto PAtual(itDSupTerrAtual->PegaPCentral());

        double DistanciaAtual(hypot(PAtual.x - PMouse.x, PAtual.y - PMouse.y));
        if (DistanciaAtual < MenorDistanciaTerr)
        {
          MenorDistanciaTerr = DistanciaAtual;
          ItPontoMaisProxTerr = itDSupTerrAtual;
        }
      }
      ++itDSupTerrAtual;
    }
  }

  PMouse.x += Toleranciax;     //--- Restaura o valor original

  //--- Procura nos pontos de terreno:
  // 
  //--- SUPERF_TRADICIONAL e SUPERF_TOPOGRAFIA => Tem pontos => VarrerTerreno tem q estar ligado
  //--- SUPERF_DRONES e SUPERF_GERENMAPAS  => Tem pontos CN => VarrerCN tem q estar ligado
  //--- Forcar => Forçado

  if(((TipoSuperf ==  SUPERF_GOOGLEEARTH || TipoSuperf == SUPERF_TRADICIONAL || TipoSuperf == SUPERF_TOPOG_ACAD) && (ObjetosDrenagem || VarrerTerreno)) ||
     (TipoSuperf == SUPERF_DRONES && (ObjetosDrenagem || VarrerCN)) ||
     (TipoSuperf == SUPERF_GERENMAPAS || TipoSuperf == SUPERF_SIG) && (ObjetosDrenagem || VarrerCN || EsbocandoObjetos || DefinindoTalveguePrincipal))
  {
    ItSSuperficie itDSupFinal(Superficie.pSuperficieAtual->end()), itDSupAtual(itDSupFinal);

    //--- Procura nos pontos da superficie

    itDSupFinal = Superficie.pSuperficieAtual->upper_bound(PMouse);
    PMouse.x -= Toleranciax * 2.0;
    itDSupAtual = Superficie.pSuperficieAtual->upper_bound(PMouse);
    PMouse.x += Toleranciax;

    //--- Separa o ponto que está mais próximo do mouse 

    while (itDSupAtual != itDSupFinal)
    {
      Ponto PAtual((*itDSupAtual).PegaPCentral());

      double DistanciaAtual(hypot(PAtual.x - PMouse.x, PAtual.y - PMouse.y));

      if (DistanciaAtual < MenorDistancia)
      {
        MenorDistancia = DistanciaAtual;
        itPonMaisProximo = itDSupAtual;
      }

      itDSupAtual++;
    }
  }

  //--- 
  //--- Procura nas CNs

  //--- Se for ponto de CN Verifica se é para varrer as CNS e se a curva pega esta visível
  //--- Na superficie de drones a densidade de pontos é alta então só pega os pontos marcados (de 4 em 4)
  //--- Para não ficar mostrando a janela de cota da CN toda hora

  //Não esta funcionando direito para mapas 

  if(0) if(VarrerCN && itPonMaisProximo != Superficie.pSuperficieAtual->end())
  {
    if(itPonMaisProximo->PCentral.Baliza2 & Ponto::CURVA_NIVEL)
    {
      if (itPonMaisProximo->PCentral.z != INFINITO)
      {
        if (SaltarCNs == 0 || fabs(fmod(itPonMaisProximo->PCentral.z, (SaltarCNs + 1.0) * Superficie.PegaEquidisCN())) != 0)
        {
          if (((CMainFrame*)AfxGetMainWnd())->TipoSuperficie != SUPERF_DRONES || itPonMaisProximo->PCentral.Baliza2 & Ponto::PONTO_MARCADO_CN)
          {
           //itPonMaisProximo = Superficie.pSuperficieAtual->end();  //Não esta funcionando direito para mapas 
          }
        }
      }
    }
  }

  //--- Fim Procura nos pontos da superfície

  //--- Procura nos pontos desabilitados

  double MenorDistanciaDes(INFINITO);
  ItSSetPontos itPontoMaisProximo(Superficie.SetPonDesabilitados().end());

  if (pItPontoDesabMaisProximo != NULL)
  {
    PMouse.x += Toleranciax;

    ItSSetPontos itDPontoFinal(Superficie.SetPonDesabilitados().upper_bound(PMouse)), itDPontoAtual(Superficie.SetPonDesabilitados().end());

    PMouse.x -= Toleranciax * 2.0;

    ItSSetPontos itDPontoDesAtual(Superficie.SetPonDesabilitados().upper_bound(PMouse));         //--- Pega o primeiro na tolerancia    

    PMouse.x += Toleranciax;                                                                   //--- Restaura o valor original

    //--- Separa o ponto que está mais próximo do mouse 

    while (itDPontoDesAtual != itDPontoFinal)
    {
      Ponto PAtual(*itDPontoDesAtual);
      double DistanciaAtual = hypot(PAtual.x - PMouse.x, PAtual.y - PMouse.y);
      if (DistanciaAtual < MenorDistanciaDes)
      {
        MenorDistanciaDes = DistanciaAtual;
        itPontoMaisProximo = itDPontoDesAtual;
      }
      itDPontoDesAtual++;
    }
  }

  //--- Fim Procura nos pontos desabilitados

  //--- Procura nos pontos inseridos pelo usuario
  //--- Os pontos dos acidentes topográficos pelo usuário estão definidos nos pontos inseridos pelo usuário

  double MenorDistanciaIns(INFINITO);
  ItSSetPontos itPontoMaisProximoIns(Superficie.SetPonInseridos().end());

  if (pItPontoInsMaisProximo != NULL)
  {
    PMouse.x += Toleranciax;

    ItSSetPontos itDPontoFinal(Superficie.SetPonInseridos().upper_bound(PMouse)), itDPontoAtual(Superficie.SetPonInseridos().end());

    PMouse.x -= Toleranciax * 2.0;

    ItSSetPontos itDPontoInsAtual(Superficie.SetPonInseridos().upper_bound(PMouse));         //--- Pega o primeiro na tolerancia    

    PMouse.x += Toleranciax;                                                                   //--- Restaura o valor original

    //--- Separa o ponto que está mais próximo do mouse 

    while (itDPontoInsAtual != itDPontoFinal)
    {
      Ponto PAtual(*itDPontoInsAtual);
      double DistanciaAtual = hypot(PAtual.x - PMouse.x, PAtual.y - PMouse.y);
      if (DistanciaAtual < MenorDistanciaIns)
      {
        MenorDistanciaIns = DistanciaAtual;
      //  itPontoMaisProximo = itDPontoInsAtual;
        itPontoMaisProximo = itDPontoInsAtual;
      }
      itDPontoInsAtual++;
    }
  }

  //--- Fim Procura nos pontos inseridos pelo usuario

  //--- Procura nas OAC

  double MenorDistanciaED(INFINITO);

  tymapEquipDrenagem ObrasDrenagem(Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem());

  if (pOAC != NULL)
  {
    double xInicial(PMouse.x - Toleranciax), xFinal(PMouse.x + Toleranciax);

    for (ittymapEquipDrenagem itEquipAtual = Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().begin();
      itEquipAtual != Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end();
      ++itEquipAtual)
    {
      itEquipAtual->second.Indicadores &= !(CEquipDrenagem::PONTO_EIXO & CEquipDrenagem::PONTO_ALA_ESQUERDA & CEquipDrenagem::PONTO_ALA_DIREITA);

      const Ponto& PAtual(itEquipAtual->second.CoordEixo());

      if (PAtual.x != INFINITO && PAtual.y != INFINITO)
      {
        if (PAtual.x > xInicial && PAtual.x < xFinal)
        {
          double DistanciaAtual(hypot(PAtual.x - PMouse.x, PAtual.y - PMouse.y));

          if (DistanciaAtual < MenorDistanciaED)
          {
            itEquipAtual->second.Indicadores &= CEquipDrenagem::PONTO_EIXO;
            MenorDistanciaED = DistanciaAtual;
            ItOACMaisProx = itEquipAtual;
          }
        }
      }
      //--- Procura nas alas do bueiro

      itlstitsetPontos ItAlaBueiro(itEquipAtual->second.LstItPontos.begin());

      if (ItAlaBueiro != itEquipAtual->second.LstItPontos.end())
      {
        if ((*ItAlaBueiro)->x != INFINITO && (*ItAlaBueiro)->y != INFINITO)
        {
          if ((*ItAlaBueiro)->x > xInicial && (*ItAlaBueiro)->x < xFinal)
          {
            double DistanciaAtual(hypot((*ItAlaBueiro)->x - PMouse.x, (*ItAlaBueiro)->y - PMouse.y));

            if (DistanciaAtual < MenorDistanciaED)
            {
              MenorDistanciaED = DistanciaAtual;
              ItOACMaisProx = itEquipAtual;
              ItOACMaisProx->second.Indicadores = (*ItAlaBueiro)->Baliza2;
            }
          }
        }
        ritlstitsetPontos ItrAlaBueiro(itEquipAtual->second.LstItPontos.rbegin());

        if (ItrAlaBueiro != itEquipAtual->second.LstItPontos.rend())
        {
          if ((*ItrAlaBueiro)->x != INFINITO && (*ItrAlaBueiro)->y != INFINITO)
          {
            if ((*ItrAlaBueiro)->x > xInicial && (*ItrAlaBueiro)->x < xFinal)
            {
              double DistanciaAtual(hypot((*ItrAlaBueiro)->x - PMouse.x, (*ItrAlaBueiro)->y - PMouse.y));

              if (DistanciaAtual < MenorDistanciaED)
              {
                MenorDistanciaED = DistanciaAtual;
                ItOACMaisProx = itEquipAtual;
                ItOACMaisProx->second.Indicadores = (*ItrAlaBueiro)->Baliza2;

              }
            }
          }
        }
      }
    }
  }

  double MenorDistanciaProcurados(INFINITO),DeltaMaximo(Toleranciax * 1.41426);

  if(pItPontoProcMaisProx != nullptr && LPontosProcurados.size() >0)
  {
    bool Achou(false);
    itlstPontos ItpProcAtual(LPontosProcurados.begin());
    double DistanciaAtual(INFINITO);

    while(ItpProcAtual != LPontosProcurados.end() && !Achou)
    {
      double Dx(PMouse.x - ItpProcAtual->x), Dy(PMouse.y - ItpProcAtual->y);

      if(fabs(Dx < DeltaMaximo) && fabs(Dy < DeltaMaximo))
      {
        DistanciaAtual = hypot(Dx,Dy);

        if(DistanciaAtual < MenorDistanciaProcurados)
        {
          MenorDistanciaProcurados = DistanciaAtual;
          ItpProcuradoMaisProximo = ItpProcAtual;
        }
      }
      ItpProcAtual++;
    }
  }

  //--- FIM Procura nos Procurados

  double MenorDistanciaPontosSoltos(INFINITO);
  ItSSetPontos itPontoSoltoMaisProximo(Superficie.PegaPontosSoltosMapa().end());

  //--- Pontos soltos DGN não mostra

  if (0 && TipoSuperf == SUPERF_GERENMAPAS)
  {
    if (pItPontoSoltoMaisProximo != nullptr)
    {
      double TolxPontoSolto(Toleranciax*2.0);

      PMouse.x += TolxPontoSolto;
      ItSSetPontos itDPontoSoltoFinal(Superficie.PegaPontosSoltosMapa().upper_bound(PMouse));
      PMouse.x -= TolxPontoSolto * 2.0;
      ItSSetPontos itDPontoSoltoAtual(Superficie.PegaPontosSoltosMapa().upper_bound(PMouse));         //--- Pega o primeiro na tolerancia    
      PMouse.x += TolxPontoSolto;  //--- Restaura o valor original

      while (itDPontoSoltoAtual != itDPontoSoltoFinal)
      {
        Ponto PAtual(*itDPontoSoltoAtual);
        double DistanciaAtual(hypot(PAtual.x - PMouse.x, PAtual.y - PMouse.y));
        if (DistanciaAtual < MenorDistanciaPontosSoltos)
        {
          MenorDistanciaPontosSoltos = DistanciaAtual;
          itPontoSoltoMaisProximo = itDPontoSoltoAtual;
        }
        itDPontoSoltoAtual++;
      }
    }
  }
  //--- Separa a menor distancia entre os encontrados  e retorna se ele estiver dentro da tolerancia 
  //--- caso contrário não há ponto mais próximo dentro da tolerancia

  double MenorDistGeral(MenorDistancia);
  int Tipo(1);                                        //--- 1-> Ponto Comum 2 -> Ponto Desabilitado 3 -> Ponto Inserido

  MenorDistGeral += 1e-5;                             //--- Se ocorrer um ponto desabilitado ou inserido ele tem q prevalecer em relação ao ponto comum porque ele pode estar nos 2 sets 
                                                      //--- deste modo a distancia dele será um pouco menor que q distancia do ponto comum (geral)
  if (MenorDistanciaDes < MenorDistGeral)
  {
    Tipo = 2;
    MenorDistGeral = MenorDistanciaDes;
  }

  if (MenorDistanciaTerr < MenorDistGeral)
  {
    Tipo = 3;
    MenorDistGeral = MenorDistanciaTerr;
  }

  if (MenorDistanciaED < MenorDistGeral)
  {
    Tipo = 4;
    MenorDistGeral = MenorDistanciaED;
  }

  if (MenorDistanciaIns < MenorDistGeral)
  {
    Tipo = 5;
    MenorDistGeral = MenorDistanciaIns;
  }

  /*
   if (MenorDistanciaPIS < MenorDistGeral)
   {
     Tipo = 6;
     MenorDistGeral = MenorDistanciaPIS;
   }
   */

  if (MenorDistanciaPontosSoltos < MenorDistGeral)
  {
    Tipo = 7;
    MenorDistGeral = MenorDistanciaPontosSoltos;
  }

  if (MenorDistanciaProcurados < MenorDistGeral)
  {
    Tipo = 8;
    MenorDistGeral = MenorDistanciaProcurados;
  }

  if (pItPTerrap) *pItPTerrap = pSupTerr->end();

  if (MenorDistGeral < Toleranciax)
  {
    switch (Tipo)
    {
      case 1: return itPonMaisProximo; break;
      case 2: *pItPontoDesabMaisProximo = itPontoMaisProximo; break;
      case 3: *pItPTerrap = ItPontoMaisProxTerr; break;
      case 4: *pOAC = ItOACMaisProx; break;
      case 5: *pItPontoInsMaisProximo = itPontoMaisProximo;break;//itPontoMaisProximoIns; break;
      case 7: *pItPontoSoltoMaisProximo = itPontoSoltoMaisProximo; break;
      case 8: *pItPontoProcMaisProx = ItpProcuradoMaisProximo;

    }
  }

  return Superficie.pSuperficieAtual->end();
}

void DesPontosGeomView::Zoom()
{
  Ponto CantoInfEsq(InicioZoom.x < FimZoom.x ? InicioZoom.x : FimZoom.x, InicioZoom.y < FimZoom.y ? InicioZoom.y : FimZoom.y),
        CantoSupDir(InicioZoom.x > FimZoom.x ? InicioZoom.x : FimZoom.x, InicioZoom.y > FimZoom.y ? InicioZoom.y : FimZoom.y);

  double Escx = Escala[X], Escy = Escala[Y];

  //--- Ajusta os deltas e a escala para caber todo o zoom

  //--- O ponto inferior esquerdo da janela do zoom será o ponto inferior esquerdo da janela do video 

  Deltas[X] += CantoInfEsq.x / Escala[X];
  Deltas[Y] += AlturaVideo - (CantoSupDir.y / Escala[Y]);

  //--- Calcula as escala nos 2 sentidos e separa a menor pois será a 
  //--- usada para que toda a janela caiba no video

  Escala[X] = Resolucao[X] / (fabs((double)FimZoom.x - InicioZoom.x) / Escx);
  Escala[Y] = Resolucao[Y] / (fabs((double)FimZoom.y - InicioZoom.y) / Escy);

  Escala[X] < Escala[Y] ? Escala[Y] = Escala[X] : Escala[X] = Escala[Y];

  //--- Reajusta a escala no deslizante

  ((CChildFrame*)GetParentFrame())->DeslizanteGeom.MudarEscala(Escala[X]);

  CalculaParametros();     //--- Recalcula a altura e a largura do vídeo
  OnArrastouDesenho();
  MudarEscala(Escala[X]*100.0);

  RedrawWindow();          //--- redesenha a janela com com zoom  
}

#include "DiaRemAciTopog.h"

void DesPontosGeomView::OnRButtonDown(UINT nFlags, CPoint point)
{
  CircularPontoIntersecao = false;

  CPoint PDialogo(point);

  if (SecoesIconizadas) IconizaSecao();

  if (ArrastandoPI || ArrastandoCC)  return;         //--- Se estiver arrastando algum ponto retorna (é preciso primeiro terminar o arrasto)

  if (CriandoRestricao)
  {
    if ( ListaAtual != Superficie.PegaListaRestricoes().end() && ListaAtual->size() == 1)             //--- Ficou apenas 1 ponto na lista, remove-a
    {
      //--- Nos projetos georeferenciados os pontos de superficie são pontos de CN, não pode remover
      //--- Somente os pontos esboçados podem ser removidos da superficie pois não fazem parte da triangulação

      if(EsbocandoObjetos)
      {
        //--- O ponto foi inserido com ponto inserido pelo usuário... foi???
      
        ItSSuperficie DeltaSupRemover(PegaPontoMaisProximo(ListaAtual->begin()->PCentral, nullptr, nullptr));
        Superficie.RemovePontoInseridoUsuario(DeltaSupRemover);
      }

      if (VerfObjAvulso(ObjetoAtual) == false)
      {
        if(ObjetosDrenagem && ListaAtual->size() == 1)
        {
          Superficie.RemovePonto(ListaAtual->begin()->PCentral);
          //Superficie.RemoveListaDoMapa(ListaAtual, ObjetoAtual);
          //Superficie.PegaListaRestricoes().erase(ListaAtual);
        }
      }

//      Superficie.MMapItLLDeltaSup.erase(*ListaAtual);
      Superficie.RemoveListaDoMapa(ListaAtual,ObjetoAtual);
      Superficie.PegaListaRestricoes().remove(*ListaAtual);

      RedrawWindow();
    }

    CriandoRestricao = DefinindoObjDrenagem = false;
    NomeEquiAtual.clear();

    return;
  }
  else
  {
    if (EsbocandoObjetos || ObjetosDrenagem)
    {
      if (ItListaRestAtual != Superficie.PegaListaRestricoes().end())
      {
        if(ItListaRestAtual->size() == 1 && VerfObjAvulso(ObjetoAtual) == false)
        {
          Superficie.RemoveListaDoMapa(ItListaRestAtual, ObjetoAtual);
          Superficie.PegaListaRestricoes().erase(ItListaRestAtual);
        }
        return;
      }
    }
  }

  if ((nFlags & MK_CONTROL) && !EZoom)
  {
    EZoom = true;
    InicioZoom = FimZoom = point;
    return;
  }

  CMenu menu;

  if (nFlags & MK_SHIFT)
  {
    Ponto PSobMouse(MouseXYGeo->m_DOCoorX, MouseXYGeo->m_DOCoorY, 0.00);

    if (Superficie.EstaRotacionada()) PSobMouse.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());
    PegouPontoDesabilitado = PegouPonto = PegouPontoInserido = PegouPontoPerfilAvulso = false;

    if (SecoesAvulsas.VerfPontoSecaoAvulsa(PSobMouse, Toleranciax, ItSecaoAvulsaAtual))
    {
      PegouPontoPerfilAvulso = true;
    }
    else
    {
      DeltaSupSobMousePopup = PegaPontoMaisProximo(PSobMouse, &ItPontoDesabilitado);

      if (DeltaSupSobMousePopup != Superficie.pSuperficieAtual->end())
      {
        if (DeltaSupSobMousePopup->PegaPCentral().Inserido == true) PegouPontoInserido = true;
        else PegouPonto = true;
      }
      else
      {
        if (ItPontoDesabilitado != Superficie.SetPonDesabilitados().end()) PegouPontoDesabilitado = true;
        else
        ItLLDeltaSupMarcada = VerificaNaTangenteRestricoes(Superficie.PegaListaRestricoes().begin(), PSobMouse, Toleranciax);
      }
    }

    //--- Verifica se esta sobre algum ponto para mostrar o menu de pontos

    if (PegouPontoDesabilitado || PegouPonto || PegouPontoInserido || PegouPontoPerfilAvulso)
    {
      if (menu.LoadMenu(IDR_MENU1))
      {
        RedrawWindow();

        CMenu* pPopup(menu.GetSubMenu(0));

        if (pPopup != NULL)
        {
          CPoint PAux(point);
          ClientToScreen(&PAux);
          pPopup->TrackPopupMenu(TPM_TOPALIGN | TPM_LEFTALIGN, PAux.x + 5, PAux.y + 5, AfxGetMainWnd());

          return;
        }
      }
    }
  }
  else  //--- Se não está sobre um ponto verifica se esta sobre uma restrição, se estiver mostra o menu de restrições
  {
    if (1) // if (!ObjetosDrenagem)   //--- Se estiver editando drenagem não pode pegar o acidente tropográficodo equipamento de drenagem
    {
      CRect Rect;
      GetWindowRect(&Rect);

      Ponto PAtual;

      TransfPixelsXY(point, PAtual.x, PAtual.y);
      if (Superficie.EstaRotacionada()) PAtual.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());
 
      if (!VerificaEquipDrenagem(point, PAtual, Rect, &menu))
      {
        if (ArrastandoEquipDrenagem)           //--- Clicou no fundo com botão direito, desistiu do arrasto
        {
          ArrastandoEquipDrenagem = false;
          ItEquiDrenagemMarcada = Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end();

          return;
        }

        LLDeltaSup& ListasRestricoes(Superficie.PegaListaRestricoes());
        ItCLLDeltaSup ItLRestricaoAtual(ListasRestricoes.begin());
        PegouRestricao = false;

        if (!ObjetosDrenagem)  //--- Se estiver editando a drenagem não pode pegr o acidente topográfico que fa parte do equipamento de drenagem
        {
          while (ItLRestricaoAtual != ListasRestricoes.end())
          {
            //        if (Superficie.EstaRotacionada()) PAtual.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());
            ItLLDeltaSupMarcada = VerificaNaTangenteRestricoes(ItLRestricaoAtual, PAtual, Toleranciax);

            if (ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end())
            {
              PegouRestricao = true;

              RedrawWindow();

              PDialogo.x = point.x + 10 + Rect.left;
              PDialogo.y = point.y + 20 + Rect.top;

              if (ItLLDeltaSupMarcada->begin()->TipoObjeto == CAciTopografico::ESPIGAO)
              {
                RedrawWindow();

                if (menu.LoadMenu(IDR_MENU7))
                {
                  CMenu* pPopup(menu.GetSubMenu(0));

                  if (pPopup != NULL)
                  {
                    CPoint PAux(point);
                    UltimoPontoPego = PAux;
                    ClientToScreen(&PAux);

                    pPopup->TrackPopupMenu(TPM_TOPALIGN | TPM_LEFTALIGN, PAux.x + 5, PAux.y + 5, AfxGetMainWnd());
                    
                    return;
                  }
                }
              }
              else
              {
                if (menu.LoadMenu(IDR_MENU2))
                {
                  CMenu* pPopup(menu.GetSubMenu(0));

                  if (pPopup != NULL)
                  {
                    CPoint PAux(point);
                    UltimoPontoPego = PAux;
                    ClientToScreen(&PAux);

                    pPopup->TrackPopupMenu(TPM_TOPALIGN | TPM_LEFTALIGN, PAux.x + 5, PAux.y + 5, AfxGetMainWnd());

                    return;
                  }
                }
              }
            }
            ++ItLRestricaoAtual;
          }
        }

        if (CotandoCNDGN)
        {
          if (menu.LoadMenu(IDR_MENUCN))
          {
            CMenu* pPopup(menu.GetSubMenu(0));
            if (pPopup != NULL)
            {
              CPoint PAux(point);
              UltimoPontoPego = PAux;
              ClientToScreen(&PAux);
              pPopup->TrackPopupMenu(TPM_TOPALIGN | TPM_LEFTALIGN, PAux.x + 5, PAux.y + 5, AfxGetMainWnd());
              return;
            }
          }
        }
        if(DefinindoTalveguePrincipal)
        {
          if (menu.LoadMenu(IDR_MENU8))
          {
            CMenu* pPopup(menu.GetSubMenu(0));
            if (pPopup != NULL)
            {
              CPoint PAux(point);
              UltimoPontoPego = PAux;
              ClientToScreen(&PAux);
              pPopup->TrackPopupMenu(TPM_TOPALIGN | TPM_LEFTALIGN, PAux.x + 5, PAux.y + 5, AfxGetMainWnd());
              return;
            }
          }
        }
      }
    }
  }
}

void DesPontosGeomView::SalvaUltimoPontoResPixels(const ItSSuperficie DeltaSuperficie,bool PTerrap)
{
  if((!PTerrap && DeltaSuperficie != Superficie.pSuperficieAtual->end()) ||
     (PTerrap &&  DeltaSuperficie!= Trechos.PegaTrechoAtual()->second.SuperficieTerr.pSuperficieAtual->end()))
  {
    Ponto PAtual((*DeltaSuperficie).PegaPCentral());   //--- Pega o ponto atual

    UltimoPontoPego.x = ((PAtual.x - Deltas[X]) * Escala[X]);
    UltimoPontoPego.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y];
  }
}

void DesPontosGeomView::DesenharApagar(int TipoObjeto, const CPoint& LocalAnterior, const CPoint& LocalAtual, void* Objeto, int OperacaoForcada, bool PrimeiroMovimento)
{
  CDC* pDC(GetDC());
  if (!pDC) return;   //--- Segurança, aconteçeu nos testes

  CPen Pena;
  Pena.CreatePen(PS_SOLID, PrimeiroMovimento ? 2 : 1, Cores[CORPROJETO]);
  CPen* pPenaAnterior = NULL;
  CPoint PontoAtual;

  //--- Apaga e redesenha apenas o objeto 

  for (int Operacao = OPR_APAGAR; Operacao <= OPR_DESENHAR; Operacao++)
  {
    //--- Se veio para fazer apenas uma operação pula a outra.

    if ((OperacaoForcada == OPR_APAGAR && Operacao != OPR_APAGAR) || (OperacaoForcada == OPR_DESENHAR && Operacao != OPR_DESENHAR)) continue;
    if (Operacao == OPR_DESENHAR)
    {
      PontoAtual = LocalAtual;

      pDC->SetROP2(R2_NOT);
    }
    else
    {
      PontoAtual = LocalAnterior;

      pDC->SetROP2(PrimeiroMovimento ? R2_BLACK : R2_NOT);
    }

    //--- Salva a primeira pena usada, para poder restaurar na saida

    if (!pPenaAnterior) pPenaAnterior = pDC->SelectObject(&Pena);
    else pDC->SelectObject(&Pena);

    switch (TipoObjeto)
    {
      case OBJ_PONTO:
      {
        pDC->SetPixel(PontoAtual.x, PontoAtual.y, Operacao);

        //--- Desenha a cruz, 14 pixels de largura

        pDC->MoveTo(PontoAtual.x - 7, PontoAtual.y);
        pDC->LineTo(PontoAtual.x + 7, PontoAtual.y);
        pDC->MoveTo(PontoAtual.x, PontoAtual.y - 7);
        pDC->LineTo(PontoAtual.x, PontoAtual.y + 7);

        //--- circula o ponto

        pDC->Arc(PontoAtual.x - 3, PontoAtual.y - 3, PontoAtual.x + 3, PontoAtual.y + 3, 0, 0, 0, 0);
      }
      break;

      case OBJ_LINHA:
      {
        pDC->MoveTo(UltimoPontoPego.x, UltimoPontoPego.y);
        pDC->LineTo(PontoAtual.x, PontoAtual.y);
      }
      break;

      case OBJ_CURVA:
      {
        CurHorizontal* pCurHoriz((CurHorizontal*)Objeto);

        ItCMCurHorizontaisTS itCHMeio(Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS.find(pCurHoriz->EstTs)), itCHAnterior = itCHMeio;
        ItCMCurHorizontaisTS itMCH(itCHAnterior != Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS.begin() ? --itCHAnterior : itCHMeio);

        DesenhaProjeto(itMCH, pDC, OperacaoForcada, 3);

        //--- Redesenha a superfície envolvida que ficou deteriorada com o apagamento do projeto

        if (false && Operacao == OPR_APAGAR && PrimeiroMovimento)
        {
          pDC->SetROP2(R2_COPYPEN);

          //--- Pega o Limites da area deteriorada pelo redesenho das curva

          CDeltaSuperficie SuperInicial(itMCH->second.TS);         //--- Primeira curva envolvida na alteração
          itMCH++;
          if (itMCH != Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS.end()) itMCH++;  //--- Passa para a última curva envolvida na alteração

          //--- Se for  o ultimo pi que estiver sendo movido ((itSCH == end) o limite é no ST dele mesmo
          CDeltaSuperficie SuperFinal(itMCH != Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS.end() ? itMCH->second.ST : itCHMeio->second.ST);

          //--- como a superficie pode estar rotacionada as coordenadas podem estar negativas e na ordem do set o 
          //--- anterior seria na verdade o posterior. então é preciso verificar esta possibilidade...

          ItSSuperficie itSuperAnterior(Superficie.pSuperficieAtual->upper_bound(SuperInicial.PegaPCentral() < SuperFinal.PegaPCentral() ? SuperInicial : SuperFinal)),
            itSuperPosterior(Superficie.pSuperficieAtual->upper_bound(SuperInicial.PegaPCentral() < SuperFinal.PegaPCentral() ? SuperFinal : SuperInicial));

          DesenhaSuperficie(itSuperAnterior, itSuperPosterior, pDC);
        }
        break;
      }
      case OBJ_IMGBITMAPFLAG:
      {
        CDC dcMemory;

        dcMemory.CreateCompatibleDC(pDC);

        dcMemory.SelectObject(BandeiraMaskBMP);
        pDC->BitBlt(LocalAtual.x, LocalAtual.y - 14, 16, 21, &dcMemory, 0, 0, SRCCOPY);
      }
      break;
      case OBJ_RADAR:
      {
        POINT cursorPos;
        GetCursorPos(&cursorPos);

        CPen PenaRadar;

        PenaRadar.CreatePen(PS_SOLID, 1, (RGB(44, 65, 47)));
        //  pDC ->SetROP2(R2_XORPEN);
        //    Radar.Draw(pDC,LocalAtual.x,LocalAtual.y);
        RedrawWindow();
      }
      break;
      case OBJ_OAC: //--- Não está  ok
      {
        auto HH = ItEquiDrenagemMarcada->second.PegaLista().begin();

        while (*HH != ItEquiDrenagemMarcada->second.ItPontoPego) HH++;

       Ponto* pp(const_cast<Ponto*>(&**HH));

        pp->x =  MouseXYGeo->m_DOCoorX;
        pp->y = MouseXYGeo->m_DOCoorY;

        DesenhaAciDrenagemMarcada(pDC);
      }
      break;
    }
  }
  ReleaseDC(pDC);
}

void DesPontosGeomView::AcertaRestricoes(const CDeltaSuperficie* DeltaSupAAlterar, const CDeltaSuperficie* DeltaSupAlterada)
{
  std::list<ItLDeltaSup> ListaDeltasRemovidos;   //--- Como um mesmo ponto de restrição pode ocorrer mais de uma vez numa lista 
                                                 //--- quando for remoção coloca os pontos numa outra lista para depois removê-los
  for (int i = BORDOS; i <= RESTRICOES; i++)
  {
    LLDeltaSup& ListaRestAtual = i == BORDOS ? Superficie.PegaListaBordos() : Superficie.PegaListaRestricoes();

    //--- Procura em todas as listas de restrições pelo ponto alterado, como as listas são pequenas e esta operação é rara, no problem 
    //--- (Passar para set tb, principalmente os bordos, criar o ser para busca e a lista para a busca sequencial

    for (ItLLDeltaSup ItLLDeltaSupAtual = ListaRestAtual.begin(); ItLLDeltaSupAtual != ListaRestAtual.end(); ItLLDeltaSupAtual++)
    {
      for (register ItLDeltaSup ItLDeltaSupAtual = (*ItLLDeltaSupAtual).begin(); ItLDeltaSupAtual != (*ItLLDeltaSupAtual).end(); ItLDeltaSupAtual++)
      {
        if ((*ItLDeltaSupAtual) == *DeltaSupAAlterar)
        {
          //--- Se o ponteiro veio nulo remove o ponto da lista de restrição
          //--- Caso conrário altera-o para o valor modificado

          if (!DeltaSupAlterada)  ListaDeltasRemovidos.push_back(ItLDeltaSupAtual);
          else (*ItLDeltaSupAtual) = *DeltaSupAlterada;
        }
      }

      //--- Se for remoção remove os pontos da restrição atual

      if (!DeltaSupAlterada && ListaDeltasRemovidos.size())
      {
        for (std::list<ItLDeltaSup>::iterator It = ListaDeltasRemovidos.begin(); It != ListaDeltasRemovidos.end(); It++)
          (*ItLLDeltaSupAtual).remove(**It);
      }
    }
  }
}

void DesPontosGeomView::QuebraLista(LLDeltaSup& ListasAtuais, ItLLDeltaSup& itListaAtual, ItLDeltaSup itPontoRestricao,ItLLDeltaSup* ppItListaQuebrada,int CasoEspecial)
{
  bool ListaCircular(*itListaAtual->begin() == *itListaAtual->rbegin());
  register ItLDeltaSup itPontoRestricaoOriginal(itPontoRestricao);

  if (ListaCircular)                                        //--- Ao se quebrar a lista circular a lista passa a ser uma lista comum
  {
    if (0 && *itPontoRestricao == *itListaAtual->begin())   //--- Lista circular e clicou no 1o ponto da lista
    {
      itListaAtual->pop_back();
    }
    else
    {
      ItLDeltaSup itUltPonto(itListaAtual->end());

      itUltPonto--;
      itUltPonto--;                          //--- O utimo ponto é igual ao primeiro para fechar o circulo

      //--- Pula os pontos inseridos (Exceto os inseridos pelo usuário)

      while (itUltPonto->PegaPCentral().Descricao.Find("Ponto Inserido") != -1 && itUltPonto->PegaPCentral().Descricao.Find("Usuário") == -1)
      {
        --itUltPonto;
      }

      if (*itPontoRestricao == *itUltPonto)    //--- Lista circular e clicou no ultimo ponto da lista
      {
        itListaAtual->pop_back();

        //--- Remove tb os pontos inseridos neste segmento 

        while (itListaAtual->back().PegaPCentral().Descricao.Find("Ponto Inserido") != -1 && itListaAtual->back().PegaPCentral().Descricao.Find("Usuário") == -1)
        {
          itListaAtual->pop_back();
        }
      }
      else      //--- Clicou no primeiro ponto ou no meio da lista, quebra a lista 
      {
        //--- Pula os pontos inseridos

        ++itPontoRestricao;

        while (itPontoRestricao->PegaPCentral().Descricao.Find("Ponto Inserido") != -1 && itPontoRestricao->PegaPCentral().Descricao.Find("Usuário") == -1)
        {
          ++itPontoRestricao;
        }

        //--- Quebra a lista 

        Superficie.RemoveListaDoMapa(itListaAtual, ObjetoAtual,false);                              //--- Remove a lista atual

        itListaAtual->pop_back();                                                            //--- Quebrou a lista, remove o elo (O último ponto =e = ao primeiro)  

        //--- Os pontos inseridos não são considerados, remove-os tb (exeto os inseridos pelo usuáro)

        while (itListaAtual->back().PegaPCentral().Descricao.Find("Ponto Inserido") != -1 && itListaAtual->back().PegaPCentral().Descricao.Find("Usuário") == -1)
        {
          itListaAtual->pop_back();
        }

        LDeltaSup LDS(itPontoRestricao, itListaAtual->end());                                //--- Cria uma lista com a 2a parte
        itListaAtual->erase(itPontoRestricao, itListaAtual->end());                          //--- Remove a 2a parte da lista atual 

        //--- Se houver pontos inseridos neste último segmento remove-os. Os pontos inseridos só são usados na triangulação, para efeito de acidentes topograficos
        //--- eles não são considerados (os inseridos pelo usuáro são pontos válidos)

        while (itListaAtual->back().PegaPCentral().Descricao.Find("Ponto Inserido") != -1 && itListaAtual->back().PegaPCentral().Descricao.Find("Usuário") == -1)
        {
          itListaAtual->pop_back();
        }

        itListaAtual->insert(itListaAtual->begin(), LDS.begin(), LDS.end());                  //--- Insere a 2a parte invertida na lista atual
        Superficie.InsereListaNoMapa(itListaAtual, ObjetoAtual);                             //--- Insere a novalista
        //if(0) for (auto& rDS : *itListaAtual)
        for (auto& rDS : *itListaAtual)
        {
          Superficie.InserePontoInseridoUsuario(rDS.PCentral);
        }
      }
    }
  }
  else    //---- Para listas que não são circulares, so pode ter clicado no meio da lista, as verificações de inicio e fim ja foram feitas
  {
    //--- Passa os pontos inseridos

    ++itPontoRestricao;

    if (itPontoRestricao != itListaAtual->end())     //--- Mandou quebrar no fim da lista...
    {
      while (itPontoRestricao->PegaPCentral().Descricao.Find("Ponto Inserido") != -1 && itPontoRestricao->PegaPCentral().Descricao.Find("Usuário") == -1)
      {
        ++itPontoRestricao;
      }

      //--- Quebra a lista

      if (CasoEspecial || *itPontoRestricao != *itListaAtual->rbegin())   //--- Se for o ultimo ponto da lista a lista não pode ser quebrada pois 1 delas ficaria com apenas 1 ponto
      {
        ListasAtuais.push_back(LDeltaSup(itPontoRestricao, itListaAtual->end()));
        Superficie.InsereListaNoMapa(--(ListasAtuais.end()), ObjetoAtual);

        if(ppItListaQuebrada) *ppItListaQuebrada = (--(ListasAtuais.end()));
      }
      itListaAtual->erase(++itPontoRestricaoOriginal, itListaAtual->end());     //--- apaga da lista atual a lista que foi criada
    }
  }
}

void DesPontosGeomView::OnRedesenhar()
{
  RedrawWindow();
}

void DesPontosGeomView::OnUpdateButVisCroquis(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!SincPontosControle);
}

void DesPontosGeomView::OnUpdateGravar(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(AlterouTerreno || AlterouProjeto || AlterouBacias);
}

BOOL DesPontosGeomView::OnEraseBkgnd(CDC* ppDC)
{
  return FALSE;
}

void DesPontosGeomView::DesenhaEspiral(const CurHorizontal& Curva, CDC* pDC)
{
  static int DELTA(5);
  //  MSecaoGeom* pMsecoesGeom = &MapaSecoesGeom;            //--- Ponteiro que apontontará para o mapa a ser usado o definitivo ou o temporário (default = definitivo)
  MSecaoGeom  MSecoesGeomTemp;                               //--- Mapa temporário para ocaso de o usuário estar editando o geométrico

  register itMSecaoGeom IMSecaoGeomAtual(Trechos.PegaTrechoAtual()->second.MapaSecoesGeom.find(Curva.EstTs));     //--- Procura pelo TS no mapa de seções

  //--- Se o Ts não foi encontrado no mapa de seções, o usúário alterou ou está alterando o projeto geométrico
  //--- Neste caso não há pontos suficientes para desenhar a espiral (por polyline) então esboça a espiral desenhando uma
  //--- Parábola cúbica que passa por 3 pontos da espiral.

  //if (1 || IMSecaoGeomAtual == MapaSecoesGeom.end())      //--- Se não achar a estaca do TS usa o mapa temporário.

  MSecaoGeom& rMsecoesGeom(const_cast<MSecaoGeom&>(Curva.mapSecoesDesenho));

  if (Curva.mapSecoesDesenho.size() == 0) CriaMapaSecoesGeomTemp(Curva, rMsecoesGeom);

  // MSecaoGeom* pMsecoesGeom(&pMSG);

  for (int Ramo = ESQUERDO; Ramo <= DIREITO; Ramo++)
  {
    //--- Desenha uma Polyline entre os pontos da espirarl   TODO ---> passar para parabola cúbica ou bezier.

    if (Ramo == ESQUERDO) IMSecaoGeomAtual = rMsecoesGeom.find(Curva.EstTs);
    else IMSecaoGeomAtual = rMsecoesGeom.find(Curva.EstCs);

    VPOINT VPontosCurva;        //--- Pontos que serão desenhados
    int nPontos(0);

    //--- Desenha o eixo

    while (IMSecaoGeomAtual != rMsecoesGeom.end() && (*IMSecaoGeomAtual).first <= (Ramo == ESQUERDO ? Curva.EstSc : Curva.EstSt))
    {
      if (div(nPontos, DELTA).rem == 0) VPontosCurva.resize(VPontosCurva.size() + DELTA);       //--- aloca de 5 em 5 pontos

      VPontosCurva[nPontos].x = ((*IMSecaoGeomAtual).second.x - Deltas[X]) * Escala[X];
      VPontosCurva[nPontos].y = (AlturaVideo - ((*IMSecaoGeomAtual).second.y - Deltas[Y])) * Escala[Y];

      nPontos++;
      IMSecaoGeomAtual++;
    }

    if (!nPontos) return;   //--- Acontece nos movimentos do mouse 

    //--- Circula o TS e o SC ou o CS e o ST

    pDC->Arc(VPontosCurva[0].x - 4, VPontosCurva[0].y + 4, VPontosCurva[0].x + 4, VPontosCurva[0].y - 4, 0, 0, 0, 0);
    pDC->Arc(VPontosCurva[nPontos - 1].x - 4, VPontosCurva[nPontos - 1].y + 4, VPontosCurva[nPontos - 1].x + 4, VPontosCurva[nPontos - 1].y - 4, 0, 0, 0, 0);

    pDC->Polyline(&VPontosCurva.front(), nPontos);    //--- A Espiral é uma polyline  ==> TODO Passar para bezier ou parábola cúbica
    //  pDC->PolyBezier(&VPontosCurva.front(),4);

    //--- Desenha os bordos direito e esquerdo
    //--- Calcula os pontos no bordo esquerdo e desenha a parabola cubica de 3 em 3 pontos

    for (int Lado = ESQUERDO; Lado <= DIREITO; Lado++)
    {
      IMSecaoGeomAtual = rMsecoesGeom.find(Ramo == ESQUERDO ? Curva.EstTs : Curva.EstCs);

      nPontos = 0;
      VPontosCurva.clear();

      Ponto PLateral;
      double Largura[2], SL(0.0);

      while (IMSecaoGeomAtual != rMsecoesGeom.end() && (*IMSecaoGeomAtual).first <= (Ramo == ESQUERDO ? Curva.EstSc : Curva.EstSt))
      {
        SecoesTipo.BuscaLargurasPistas((*IMSecaoGeomAtual).first, Largura[ESQUERDO], Largura[DIREITO]);
        SL = Trechos.PegaTrechoAtual()->second.CurvasHorizontais.BuscaSuperLargura((*IMSecaoGeomAtual).first);

        if (SL != 0.0)
        {
          double ms = Curva.Sl / Curva.Lc, SLEstaca(0.0);

          if (Ramo == ESQUERDO) SLEstaca = ms * (IMSecaoGeomAtual->first.EstReal - Curva.EstTs.EstReal);
          else SLEstaca = ms * (Curva.EstSt.EstReal - IMSecaoGeomAtual->first.EstReal);

          Largura[ESQUERDO] -= SLEstaca;
          Largura[DIREITO] += SLEstaca;
        }

        IrradiaPonto(Mat.M_PI_2, (*IMSecaoGeomAtual).second, Largura[Lado], PLateral);

        if (div(nPontos, DELTA).rem == 0) VPontosCurva.resize(VPontosCurva.size() + DELTA);       //--- aloca de 5 em 5 pontos

        VPontosCurva[nPontos].x = (PLateral.x - Deltas[X]) * Escala[X];
        VPontosCurva[nPontos].y = (AlturaVideo - (PLateral.y - Deltas[Y])) * Escala[Y];

        nPontos++;
        IMSecaoGeomAtual++;
      }

      pDC->Polyline(&VPontosCurva.front(), nPontos);
    }
  }
}

void DesPontosGeomView::DesenhaCircular(const CurHorizontal& Curva, CDC* pDC,ittymapCTrechos itT,bool Apagar)
{
  if (Curva.Raio < 5.0) return;    //---Raio muito pequeno.

  //--- Calcula o PI e o centro da curva

  long xPI((Curva.PI.x - Deltas[X]) * Escala[X] + .5),
       yPI((AlturaVideo - (Curva.PI.y - Deltas[Y])) * Escala[Y] + .5),
       xCC((Curva.CC.x - Deltas[X]) * Escala[X] + .5),
       yCC((AlturaVideo - (Curva.CC.y - Deltas[Y])) * Escala[Y] + .5);

  //--- Desenha (ou apaga) o centro da curva e o PI

  DesenharPonto(xPI, yPI, pDC, Apagar ? Cores[CORFUNDO] : Cores[CORPROJETO]);
  DesenharPonto(xCC, yCC, pDC, Apagar ? Cores[CORFUNDO] : Cores[CORPROJETO]);

  //--- Calcula as coordenadas em pixels do CS e do SC  

  long xSC((Curva.SC.x - Deltas[X]) * Escala[X] + .5),
       ySC((AlturaVideo - (Curva.SC.y - Deltas[Y])) * Escala[Y] + .5),
       xCS((Curva.CS.x - Deltas[X]) * Escala[X] + .5),
       yCS((AlturaVideo - (Curva.CS.y - Deltas[Y])) * Escala[Y] + .5),
       xTS((Curva.TS.x - Deltas[X]) * Escala[X] + .5),
       yTS((AlturaVideo - (Curva.TS.y - Deltas[Y])) * Escala[Y] + .5);

  //--- Verifica se o cs está muito proximo do sc (em pixels). Caso esta distância seja menor que 4 pixels não desenha a curva
  //--- porque em arcos muito pequenos,a resolução do video pode fazer o cs ficar antes do sc que provoca
  //--- o desenho do arco complementar. Caso positivo não há mais nada a desenhar. retorna

  if (abs(xCS - xSC) + abs(yCS - ySC) < 4) return;

  //--- Calcula as coordenadas em pixels dos cantos superior esquerdo e direito do quadrado que inscreve o circulo

  long RaioPixels(Curva.Raio * Escala[X] + .5), xSE(xCC - RaioPixels), ySE(yCC + RaioPixels), xID(xCC + RaioPixels), yID(yCC - RaioPixels);

  //--- Desenha a curva

  if (Curva.Lado == DIREITO) pDC->Arc(xSE, ySE, xID, yID, xCS, yCS, xSC, ySC);
  else pDC->Arc(xSE, ySE, xID, yID, xSC, ySC, xCS, yCS);

  //--- Circula o CS e o SC

  //  pDC->Arc(xSC-4,ySC+4,xSC+4,ySC-4,0,0,0,0);  
  //  pDC->Arc(xCS-4,yCS+4,xCS+4,yCS-4,0,0,0,0); 

  double Largura[2]{0.0,0.0};
  itT->second.SecoesTipo.BuscaLargurasPistas(Curva.EstTs, Largura[ESQUERDO], Largura[DIREITO]);

  //--- Desenha os bordos

  for (int Lado = ESQUERDO; Lado <= DIREITO; Lado++)
  {
    Largura[Lado] += (Curva.Sl * ((Lado == ESQUERDO) ? -1.0 : 1.0));

    double RaioAuxPixels((Curva.Raio + Largura[Lado]) * Escala[X] * (Curva.Lado == DIREITO ? -1.0 : 1.0));   //--- O Lado esquerdo vem negativo

    Ponto SC, CS;

    IrradiaPonto(Mat.M_PI_2, Curva.SC, Largura[Lado], SC);
    IrradiaPonto(Mat.M_PI_2, Curva.CS, Largura[Lado], CS);

    xSC = (SC.x - Deltas[X]) * Escala[X] + .5,
    ySC = (AlturaVideo - (SC.y - Deltas[Y])) * Escala[Y] + .5,
    xCS = (CS.x - Deltas[X]) * Escala[X] + .5,
    yCS = (AlturaVideo - (CS.y - Deltas[Y])) * Escala[Y] + .5,

    xSE = xCC - RaioAuxPixels,
    ySE = yCC + RaioAuxPixels,
    xID = xCC + RaioAuxPixels,
    yID = yCC - RaioAuxPixels;

    if (Curva.Lado == DIREITO) pDC->Arc(xSE, ySE, xID, yID, xCS, yCS, xSC, ySC);
    else  pDC->Arc(xSE, ySE, xID, yID, xSC, ySC, xCS, yCS);
  }
}

inline void DesPontosGeomView::IrradiaPonto(double Angulo, const Ponto& PInicial, double Distancia, Ponto& PCalcular,bool Somar)
{
  double AZ(Somar ? PInicial.fi + Angulo : Angulo);

  AZ = fmod(AZ,Matem::M_2_PI);

  PCalcular.x = PInicial.x + sin(AZ) * Distancia;
  PCalcular.y = PInicial.y + cos(AZ) * Distancia;
}

inline void DesPontosGeomView::IrradiaPonto(double Angulo, double x, double y, double s, double Distancia, Ponto& PCalcular)
{
  double AZ = s + Angulo;

  AZ = fmod(AZ, Matem::M_2_PI);

  PCalcular.x = x + sin(AZ) * Distancia;
  PCalcular.y = y + cos(AZ) * Distancia;
}

inline void DesPontosGeomView::DesenharPonto(double x, double y, CDC* pDC, COLORREF Cor)
{
  CPen Pena, * PAnterior;
  Pena.CreatePen(PS_SOLID, 1, Cor);
  PAnterior = pDC->SelectObject(&Pena);

  if (DesenharPixel) pDC->SetPixel(x, y, Cor);

  if (DesenharCirculo) pDC->Arc(x - 3, y + 3, x + 3, y - 3, 0, 0, 0, 0);

  if (DesenharCruz)
  {
    //--- Desenha a cruz, 14 pixels de largura

    pDC->MoveTo(x - 5, y);
    pDC->LineTo(x + 5, y);
    pDC->MoveTo(x, y - 5);
    pDC->LineTo(x, y + 5);
  }
  pDC->SelectObject(PAnterior);
}

//--- Passar este metodo como propriedade de curvas horizontais

ItCMCurHorizontaisPI* DesPontosGeomView::PegaPIMaisProximo(CurHorizontal& CurvaHorizontal, std::string& TrechoPI)
{
  bool Achou(false);
  ittymapCTrechos it(Trechos.MapaTrechos.begin());
  static ItCMCurHorizontaisPI itPMaisPerto;

  for (ittymapCTrechos it(Trechos.MapaTrechos.begin()); !Achou && it != Trechos.MapaTrechos.end(); it++)
  {
    //--- Pega o pi <= ao pi dado menos a tolerancia x 
    //--- Vai calculando a distancia ao quadrado (para não precisar de tirar a raiz quadrada) e separa a menor
    //--- Se a menor estiver dentro da tolerancia ao quadrado , este é o ponto procurado

    if (it->second.mapCurvasHorizontaisPI.size())
    {
      CurvaHorizontal.PI.x += Toleranciax;

      ItCMCurHorizontaisPI itCurAtual(it->second.mapCurvasHorizontaisPI.upper_bound(&CurvaHorizontal.PI));   //--- Pega o >= apos a da tolerancia

      if (itCurAtual == it->second.mapCurvasHorizontaisPI.end()) itCurAtual--;

      CurvaHorizontal.PI.x -= Toleranciax;               //--- Acerta o x do PI

      double MenorDistancia2(INFINITO), Distancia2Atual;

      //--- Procura em todos os PIs que estiverem dentro da toleranciax. 
      //--- Procura em todas as curvas até que o x do PI da curva  seja menor que o do PI Atual menos a toleranciax
      //--- A procura é feita separando o PI que tem a menor distancia ate o ponto atual do mouse que esta no PI da curva atual

      do
      {
        Distancia2Atual = pow((itCurAtual->second->PI.x - CurvaHorizontal.PI.x), 2) + pow((itCurAtual->second->PI.y - CurvaHorizontal.PI.y), 2);

        if (Distancia2Atual < MenorDistancia2)
        {
          MenorDistancia2 = Distancia2Atual;
          itPMaisPerto = itCurAtual;
          TrechoPI = it->first;
        }
      } while (itCurAtual-- != it->second.mapCurvasHorizontaisPI.begin() && itCurAtual->second->PI.x > (CurvaHorizontal.PI.x - Toleranciax));

      //--- Se a menor distância estiver dentro da tolerancia este é o ponto procurado

      Achou = MenorDistancia2 < pow(Toleranciax, 2.0);
    }
  }

  return Achou ? &itPMaisPerto : nullptr;
}

ItCMCurHorizontaisCC* DesPontosGeomView::PegaCCMaisProximo(CurHorizontal& CurvaHorizontal, std::string& TrechoCC)
{
  bool Achou(false);
  ittymapCTrechos it(Trechos.MapaTrechos.begin());
  static ItCMCurHorizontaisCC itPMaisPerto;

  for (ittymapCTrechos it = Trechos.MapaTrechos.begin(); !Achou && it != Trechos.MapaTrechos.end(); it++)
  {
    //--- Pega o CC <= ao pi dado menos a tolerancia x 
    //--- Vai calculando a distancia ao quadrado (para não precisar de tirar a raiz quadrada) e separa a menor
    //--- Se a menor estiver dentro da tolerancia , este é o ponto procurado

    if (it->second.mapCurvasHorizontaisCC.size())
    {
      CurvaHorizontal.CC.x += Toleranciax;

      ItCMCurHorizontaisCC itCurAtual(it->second.mapCurvasHorizontaisCC.upper_bound(&CurvaHorizontal.CC));   //--- Pega o >= apos a da tolerancia
      if (itCurAtual == it->second.mapCurvasHorizontaisCC.end()) itCurAtual--;

      CurvaHorizontal.CC.x -= Toleranciax;               //--- Acerta o x do PI

      double MenorDistancia2 = INFINITO, Distancia2Atual;

      //--- Procura em todos os PIs que estiverem dentro da toleranciax. 
      //--- Procura em todas as curvas comecando na posterior ao x do PI + a toletancia, voltando até que o x do PI da curva  
      //--- seja menor que o do PI Atual menos a toleranciax
      //--- A procura é feita separando o PI que tem a menor distancia ate o ponto atual do mouse, que esta no PI da curva atual passada como parametro.

      do
      {
        Distancia2Atual = pow((itCurAtual->second->CC.x - CurvaHorizontal.CC.x), 2) + pow((itCurAtual->second->CC.y - CurvaHorizontal.CC.y), 2);

        if (Distancia2Atual < MenorDistancia2)
        {
          MenorDistancia2 = Distancia2Atual;
          itPMaisPerto = itCurAtual;
          TrechoCC = it->first;
        }
      } while (itCurAtual-- != it->second.mapCurvasHorizontaisCC.begin() && itCurAtual->second->CC.x > (CurvaHorizontal.CC.x - Toleranciax));

      Achou = MenorDistancia2 < pow(Toleranciax, 2.0);
    }
  }
  //--- Se a menor distância estiver dentro da tolerancia este é o ponto procurado, retorna-o

  return Achou ? &itPMaisPerto : nullptr;
}

bool DesPontosGeomView::VerificaTangente(const Ponto& PAtual, ItCMCurHorizontaisTS& ItCurvaAnterior, std::string* TrechoAtual)
{
  double Distancia(INFINITO), Raio, AzPonto;
  bool Achou(false), fim(false);
  ittymapCTrechos it(Trechos.MapaTrechos.begin());

  do
  {
    ItCurvaAnterior = it->second.mapCurvasHorizontaisTS.begin();
    do
    {
      bool NaTangente(false);

      while (ItCurvaAnterior != it->second.mapCurvasHorizontaisTS.end() && !NaTangente)
      {
        AzPonto = CCurHorizontais::CalculaAzimute(PAtual, ItCurvaAnterior->second.PI);

        if (fabs(ItCurvaAnterior->second.ST.fi - AzPonto) < Mat.M_PI_2)   //--- Está antes do pi anterior
        {
          Raio = hypot(ItCurvaAnterior->second.PI.x - PAtual.x, ItCurvaAnterior->second.PI.y - PAtual.y);
          Distancia = fabs(sin(AzPonto - ItCurvaAnterior->second.ST.fi) * Raio);

          NaTangente = Distancia < Toleranciax;
        }

        ItCurvaAnterior++;
      }

      //--- Pode ser que esteja na tangente mas não esteja no trecho,
      //--- Aqui o anterior passa a ser o posterior devido ao último ++ do for.
      //--- Verifica se está entre o St e o Ts

      if (NaTangente)
      {
        const CurHorizontal& CurvaPosterior(ItCurvaAnterior->second);           //--- Sai com o posterior, por causa do ++
        ItCMCurHorizontaisTS ItCurvaAnteriorRasc(ItCurvaAnterior);
        ItCurvaAnteriorRasc--;

        const CurHorizontal& CurvaAnterior(ItCurvaAnteriorRasc->second);

        //--- Verifica se está no trecho reto

        Achou = Raio > CurvaAnterior.Ts&& Raio < ((CEstaca)CurvaPosterior.EstTs - (CEstaca)CurvaAnterior.EstSt.EstVirtual + CurvaAnterior.Ts);
      }
    } while (!Achou && ItCurvaAnterior != it->second.mapCurvasHorizontaisTS.end());
  } while (!Achou && ++it != Trechos.MapaTrechos.end());

  if (Achou)
  {
    ItCurvaAnterior--;      //---ajusta o iterador para a curva anterior
    *TrechoAtual = it->first;
  }

  return Achou;
}

void DesPontosGeomView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (nChar == KEY_F5)
  {
    RedrawWindow();
    return;
  }

  if (nChar == KEY_ESCAPE)
  {
    //--- Restaura o que estiver sendo arrastando 

    if (ArrastandoPI)
    {
      Trechos.PegaTrechoAtual()->second.CurvasHorizontais.AlteraCurvaPI(*ItCurHorizSendoArrastadaPI, BackCurvaSendoArrastada.PI, ItUltimaCurvaAjustadaTS, NULL);
      Arrastando = ArrastandoPI = false;
      TerminarArrasto = true;
    }

    if (ArrastandoCC)
    {
      Trechos.PegaTrechoAtual()->second.CurvasHorizontais.AlteraCurvaCC(*ItCurHorizSendoArrastadaCC, BackCurvaSendoArrastada.CC, ItUltimaCurvaAjustadaTS);
      Arrastando = ArrastandoCC = false;
      TerminarArrasto = true;
    }

    if (PegandoPontoSecAvulsa)
    {
      PegandoPontoSecAvulsa = PegouPrimPonto = false;
      ((CChildFrame*)GetParentFrame())->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTSECAVULSA, 0);
    }

    if (DesenhandoObjetos || EsbocandoObjetos || ObjetosDrenagem)
    {
      ItCLLDeltaSup itcListaRestAtual(--(Superficie.PegaListaRestricoes().end()));

      if (itcListaRestAtual->size() == 0 || (itcListaRestAtual->size() == 1 && !VerfObjAvulso(Superficie.VerfAciTopografico(itcListaRestAtual))))
      {
        if ((EsbocandoObjetos || ObjetosDrenagem) && itcListaRestAtual->size() == 1)
        {
          if (itcListaRestAtual->begin()->PegaPCentral().Baliza2 & Ponto::ESBOCADO)
          {
            Superficie.RemovePonto(itcListaRestAtual->begin()->PegaPCentral());
          }
        }

        ItEquiDrenagemMarcada = Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end();

        ListaAtual = Superficie.PegaListaRestricoes().end();

        Superficie.RemoveListaDoMapa(--(Superficie.PegaListaRestricoes().end()));
        Superficie.PegaListaRestricoes().pop_back();
      }
    }

    CriandoRestricao = IniciouSarjeta = Arrastando = InserindoPonto = TrocandoObjeto = InserindoLembrete = false;
    ItLLDeltaSupMarcada = ItListaRestAtual = Superficie.PegaListaRestricoes().end();

    if (Transladando)
    {
      MudarEscala(Escala[X] * 100.0);   //--- Muda a escala para a escala atual só para atualizqar as variáveis no estado inicial
    }

    DefinindoLinhaBase = 0;

    EZoom = false;

    if (SincPontosControle)
    {
      if (NumPonCtrl == 0)
      {
        DesenharMapa = SincPontosControle = false;
        ProjetoGeoreferenciado = SUPERF_TRADICIONAL;
        ClipCursor(CRect(-1, -1, -1, -1));  //--- Libera o cursor para toda a tela
      }
      else
      {
        if (NumPonCtrl > 0)
        {
          NumPonCtrl--;
          DPopUpPonto.MostraDescricao(PonMousePixelsAtual.x, PonMousePixelsAtual.y, PreparaDescricao(&NumPonCtrl, false, OBJ_SINPONCTRL));
        }
        else SincPontosControle = false;
      }
    }

    if (Medindo > 0)
    {
      Medindo = 1;
      LPontosMedidos.clear();
    }

    //--- Esboçando Objetos

    if (EsbocandoObjetos)
    {
      if (ListaAtual != Superficie.PegaListaRestricoes().end() && ListaAtual->size() == 1)
      {
        if (VerfObjAvulso(ObjetoAtual) == false)
        {
          Superficie.RemoveListaDoMapa(ListaAtual, ObjetoAtual);
          Superficie.PegaListaRestricoes().erase(ListaAtual);
        }
      }
    }

    if (ArrastandoEquipDrenagem)
    {
      ArrastandoEquipDrenagem = false;
      ItEquiDrenagemMarcada = Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end();
    }

    if (JuntandoEspigoes)
    {
      JuntandoEspigoes = false;
      ItLLEspigaoMarcado = Superficie.PegaListaRestricoes().end();
      ItInicioJuntandoEspigoes = ItFimJuntandoEspigoes = Superficie.pSuperficieAtual->end();
      ListaAJuntar.clear();
    }

    if(DefinindoTalveguePrincipal)
    {
      if(dialogar.DoModal(-1, "Deseja Reiniciar a definição do talvegue principal (Este talvegue será excluido") == IDOK)
      {
        TalveguePrincipal.clear();
        TalveguesDoTalveguePrincipal.clear();
        ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();

        RedrawWindow();
      }
    }

    DeltaSupSobMouseAnterior = Superficie.pSuperficieAtual->end();

    LPontosProcurados.clear();

    RedrawWindow();

    return;
  }

  if (nChar == VK_DELETE)
  {
    if (!ArrastandoPI && !ArrastandoCC)
    {
      CurHorizontal CurvaDel(INFINITO, MouseXYGeo->m_DOCoorX, MouseXYGeo->m_DOCoorY);

      if (Superficie.EstaRotacionada()) CurvaDel.PI.RotacionaPlano(sin(Superficie.PegaAnguloRotacao()), cos(Superficie.PegaAnguloRotacao()), Superficie.PegaAnguloRotacao());

      std::string strTrechoAtualRemover;

      ItCMCurHorizontaisPI* ItCCurvaPI(PegaPIMaisProximo(CurvaDel, strTrechoAtualRemover));

      if (ItCCurvaPI)
      {
        CurHorizontal CurvaAtual(*(*ItCCurvaPI)->second);

        ((CMainFrame*)AfxGetMainWnd())->TrocaRua(strTrechoAtualRemover);
        Trechos.TrocaTrecho(strTrechoAtualRemover);

        if (dialogar.DoModal(5) == IDOK)
        {
          if (Trechos.PegaTrechoAtual()->second.CurvasHorizontais.RemoveCurva(*ItCCurvaPI, ItUltimaCurvaAjustadaTS, NULL) == CCurHorizontais::CURVA_OK) AlterouProjeto = true;
        }
        RedrawWindow();
      }
      else
      {
        Ponto PAnterior(MouseXYGeo->m_DOCoorX, MouseXYGeo->m_DOCoorY), PPosterior(PAnterior), PSobMouse(PAnterior);

        PAnterior.x -= Toleranciax;
        PPosterior.x += Toleranciax;

        itmapSecoesAvulsas itMSecaoAvAnterior(SecoesAvulsas.UpperBound(&PAnterior));
        itmapSecoesAvulsas itMSecaoAvPosterior(SecoesAvulsas.UpperBound(&PPosterior));
        itmapSecoesAvulsas itSecAvMaisPerto(SecoesAvulsas.End());

        double MenorDistancia2(INFINITO), Distancia2Atual(INFINITO);

        for (itmapSecoesAvulsas ItMapSecAvAtual = itMSecaoAvAnterior; ItMapSecAvAtual != itMSecaoAvPosterior; ItMapSecAvAtual++)
        {
          Distancia2Atual = pow((*ItMapSecAvAtual).first.x - PSobMouse.x, 2) + pow((*ItMapSecAvAtual).first.y - PSobMouse.y, 2);

          if (Distancia2Atual < MenorDistancia2)
          {
            MenorDistancia2 = Distancia2Atual;
            itSecAvMaisPerto = ItMapSecAvAtual;
          }
        }

        if (MenorDistancia2 < pow(Toleranciax, 2))    //--- Se estiver proximo a um ponto
        {
          if (dialogar.DoModal(13) == IDOK)
          {
            SecoesAvulsas.RemoveSecaoAvulsa((*itSecAvMaisPerto).second);
            AlterouSecoesAvulsas = AlterouProjeto = true;

            RedrawWindow();
          }
        }
        else
        {
          if (ItLembrete != SetLembretes.end())
          {
            POINT Ponto;
            GetCursorPos(&Ponto);
            CPoint PDialogo(Ponto);
            PDialogo.Offset(5, 5);

            CDiaRemAciTopog DiaRemAciTopografico(this, PDialogo, "Confirma a remoção deste lembrete?");

            if (DiaRemAciTopografico.DoModal() == IDOK)
            {
              SetLembretes.erase(ItLembrete);
              AlterouProjeto = true;

              RedrawWindow();
            }
          }

          // Pontos inseridos podem ser removidos - AGUARDAR 
         /*
         Ponto PSobMouse(MouseXYGeo->m_DOCoorX,MouseXYGeo->m_DOCoorY);
         ItSSuperficie itDeltaSupSobMouse(PegaPontoMaisProximo(PSobMouse));

         if(itDeltaSupSobMouse != Superficie.pSuperficieAtual->end())
         {
         if(itDeltaSupSobMouse->EBordo == true)
         {
         if(dialogar.DoModal(5) == IDOK)
         {
         Superficie.ExcluiDeltaSuperficie(itDeltaSupSobMouse);
         }
         }
         }*/
        }
      }
    }
  }

  if (IniciouTranslado == 1)    //--- 1a tecla enter  é do afxmessagebox, ele nao consome a tecla Enter, est faze é para consumir a tecla enter do afxmessagebox
  {
    IniciouTranslado = 2;
    return;
  }

  if (IniciouTranslado == 2)
  {
    if (nChar == VK_RETURN)
    {
      DeltasIniTranslacao[X] = Deltas[X];
      DeltasIniTranslacao[Y] = Deltas[Y];

      IniciouTranslado = false;
      Transladando = true;
    }
    return;
  }

  if (Transladando)
  {
    double DeltasTemp[2] = { 0.0,0.0 }, MtsPixel(1.0 / Escala[X]);
    static double DeltasIniTranslTeclas[2] = { 0.0,0.0 };

    if (!TransladandoPorTeclas)
    {
     // if (nChar == VK_UP || nChar == VK_RIGHT || nChar == VK_DOWN || nChar == VK_LEFT)
      if(0)    //--- Transladando por teclas está dando erro quando muda  a
               //--- escala do desenho depois do translado (roda do mouse) VERIFICAR
      {
        TransladandoPorTeclas = TRUE;
        DeltasIniTranslTeclas[X] = m_nX;
        DeltasIniTranslTeclas[Y] = m_nY;
      }
    }

    //if (GetKeyState(VK_SHIFT) == 0)
    if(0)
    {
      switch (nChar)
      {
        case VK_UP: m_nY--; break;
        case VK_RIGHT: m_nX++; break;
        case VK_DOWN: m_nY++; break;
        case VK_LEFT: m_nX--; break;
      }
    }
    else
    {
      /*
      switch (nChar)
      {
        case VK_UP: m_nY -= 5; break;
        case VK_RIGHT: m_nX += 5; break;
        case VK_DOWN: m_nY += 5; break;
        case VK_LEFT: m_nX -= 5; break;
      }
      */
    }

      if(0)      
    //if (TransladandoPorTeclas && nChar == VK_RETURN)
    {
      DeltasCarta[X] -= (DeltasIniTranslTeclas[X] - m_nX) / Escala[X];
      DeltasCarta[Y] -= (DeltasIniTranslTeclas[Y] - m_nY) / Escala[X];

      if (AfxMessageBox("Esta posição do mapa em relação às curvas de nível está ok?", MB_YESNO) == IDYES)
      {
        FinalizaTranslacao(true);
        ClipCursor(0);
        Transladando = TransladandoPorTeclas = false;
      }
    }

    RedrawWindow();

    return;
  }

  if (GetKeyState(VK_SHIFT) < 0)
  {
    nChar = toupper(nChar);

    switch (nChar)
    {
      case 'N': DesenharCN = DesenharCN == false  ; break;
      case 'S': DesenharSecoes = DesenharSecoes  == false  ; break;
      case 'T': DesenharArestas = DesenharArestas  == false  ; break;
      case 'F': DesenharBordos = DesenharBordos  == false  ; break;
      case 'A': DesenharRestricoes = DesenharRestricoes  == false  ; break;
      case 'P': DesenharPontos = DesenharPontos  == false  ; break;
      case 'R': DesenharProjeto = DesenharProjeto  == false  ; break;
      case 'M': DesenharMalha = DesenharMalha  == false  ; break;
      case 'L': CotarMalha = CotarMalha  == false  ; break;
      case 'U': CotarCN = CotarCN  == false  ; break;
      case 'D': MostrarDescricao = MostrarDescricao  == false  ; break;
      case 'E': CircularSecoes = CircularSecoes  == false  ; break;
      case 'G': DesenharOffSets = DesenharOffSets  == false  ; break;
      case 'X': DesenharPixel = CotarCN  == false  ; break;
      case 'C': DesenharCirculo = DesenharCirculo  == false  ; break;
      case 'Z': DesenharCruz = DesenharCruz  == false  ; break;
      case 'O': VarrerTerreno = VarrerTerreno  == false  ; break;
      case 'J': VarrerProjeto = VarrerProjeto  == false  ; break;
      case 'V': VarrerCN = VarrerCN  == false  ; break;
      case 'B': DesenharMapa = DesenharMapa  == false  ; break;
      case 'I': DesenharSimbolTerapl = DesenharSimbolTerapl  == false  ; break;
      case 'K': DesenharCNTaludes = DesenharCNTaludes  == false  ; break;
      case 'H': DesenharSecoesAvulsas = DesenharSecoesAvulsas  == false  ; break;
      case 'Q': RealcarCNsMestras = RealcarCNsMestras  == false  ; break;
    };

    if (!DesenharCN) VarrerCN = false;

    RedrawWindow();
  }

  if (ProjetoGeoreferenciado > SUPERF_TRADICIONAL && FaltaGeoReferenciamento)   //--- Se sim está ajustando o georeferenciamento?
  {
    switch (nChar)
    {
    case VK_RIGHT:
      if (GetKeyState(VK_SHIFT) < 0)
      {
        Escala[Y] = Escala[X] += 0.001;
      }
      else Deltas[X] -= 1.1 / Escala[X];
      break;

    case VK_LEFT:
      if (GetKeyState(VK_SHIFT) < 0)
      {
        Escala[Y] = Escala[X] -= 0.001;
      }
      else Deltas[X] += 1.1 / Escala[X];
      break;

    case VK_UP: Deltas[Y] -= 1.1 / Escala[X]; break;
    case VK_DOWN: Deltas[Y] += 1.1 / Escala[X]; break;
    }
    RedrawWindow();
  }
}

std::string DesPontosGeomView::PreparaDescricao(const void* pObjeto, bool ArrastandoPonto, int OBJETO, std::string* pRua)
{
  CurHorizontal* pCH(NULL), CurNaoRotacionada;
  Ponto* pPonto(NULL), PontoSecao;
  itMSecaoGeomPonto* itMSecao;

  if (OBJETO == OBJ_CURHR || OBJETO == OBJ_CC || OBJETO == OBJ_PI)
  {
    pCH = (CurHorizontal*)pObjeto;

    if (Superficie.EstaRotacionada())
    {
      CurNaoRotacionada = *pCH;
      CurNaoRotacionada.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), -AnguloRotacao);

      pCH = &CurNaoRotacionada;
    }
  }
  else
  {
    if (OBJETO == OBJ_SECAO)
    {
      itMSecao = (itMSecaoGeomPonto*)pObjeto;
      PontoSecao = *((*itMSecao)->first);

      if (Superficie.EstaRotacionada()) PontoSecao.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), -AnguloRotacao);
    }
    else
    {
      pPonto = (Ponto*)pObjeto;
      if (OBJETO == OBJ_CURNI)
      {
        PontoSecao = *pPonto;
        if (Superficie.EstaRotacionada()) PontoSecao.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), -AnguloRotacao);
      }
    }
  }

  std::string NomeRua;

  if (pRua)
  {
    NomeRua = pRua->substr(pRua->find_last_of('\\') + 1, pRua->size());
    NomeRua += " : ";
  }

  std::stringstream strstreamTemp;

  strstreamTemp << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

  switch (OBJETO)
  {
  case OBJ_TEXTO: strstreamTemp << (char*)pObjeto << std::ends; break;
  case OBJ_PI:
  {
    if (pRua) strstreamTemp << NomeRua << " \r\n";
    strstreamTemp << " Ponto de Interseção: \t Coor N= " << pCH->PI.y << " \t Coor E= " << pCH->PI.x;
    break;
  }
  case OBJ_CC:
  {
    if (pRua) strstreamTemp << NomeRua << " \r\n ";
    strstreamTemp << " Centro de Curva: \t Coor N= " << pCH->CC.y << " \t Coor E= " << pCH->CC.x;
    break;
  }
  case OBJ_PONTERRENO:
    if (((CMainFrame*)AfxGetMainWnd())->TipoSuperficie == SUPERF_DRONES || 
        ((CMainFrame*)AfxGetMainWnd())->TipoSuperficie == SUPERF_GERENMAPAS ||
        ((CMainFrame*)AfxGetMainWnd())->TipoSuperficie == SUPERF_SIG) //--- os pontos de terreno da superfícide por drone ou mapas  são as CNs
      strstreamTemp << " Curva de Nível:" << " ";
    else
      strstreamTemp << " Ponto de Terreno: ";
    break;
  case OBJ_PONDESABILITADO: strstreamTemp << " Desabilitado - ";
  case OBJ_PONINSUSUARIO:
  case OBJ_PONINSDELAUNAY:
  case OBJ_PONINSGABRIEL: strstreamTemp << " Número: "; break;
  case OBJ_LEMBRETE: strstreamTemp << " Lembrete: "; break;
  case OBJ_GRADITERRAP: strstreamTemp << " Terraplanagem - Gradiente: "; break;
  case OBJ_TERRAPLAN: strstreamTemp << " Terraplanagem: "; break;
  case OBJ_PONFOZBACIAHIDROG: strstreamTemp << " Foz da Bacia Hidrográfica: "; break;
  case OBJ_PONCGBACIAHIDROG: strstreamTemp << " Centro de Gravidade da Bacia Hidrográfica: "; break;
  case OBJ_OAC: strstreamTemp << " Obra de Arte Corrente: "; break;
  case OBJ_PONTO_COTA_IBGE :  strstreamTemp << " Ponto Cotado: ";break;
  case OBJ_PONTO_PROCURADO : strstreamTemp << " Ponto Procurado: "; break;
  }

  if (OBJETO == OBJ_CC || OBJETO == OBJ_PI)
  {
    if (pCH->Ac != 0.0)
    {
      if (pCH->Tipo == CURVA_ESPIRAL)
      {
        if (ArrastandoPonto) strstreamTemp << " \r\n Lc = " << pCH->Lc << " \t I = ";
        else strstreamTemp << " \r\n I = ";
      }
      else strstreamTemp << " \r\n Ac = ";

      strstreamTemp << Mat.strg_gms(pCH->Ac * Matem::PI_GRAUS).c_str();
      if (pCH->Tipo == CURVA_ESPIRAL) strstreamTemp << "\tSc = " << Mat.strg_gms(pCH->Fi * Matem::PI_GRAUS).c_str();
      strstreamTemp << " \t Raio = " << pCH->Raio;
      if (!ArrastandoPonto)
      {
        if (pCH->Lc > 0.0)
          strstreamTemp << std::right << std::setfill(' ')
          << " \t Lc = " << pCH->Lc
          << " \r\n xc = " << pCH->xEsp
          << " \t\t\t\t yc = " << pCH->yEsp
          << " \t\t\t\t Ts = " << pCH->Ts
          << " \r\n Sl = " << pCH->Sl
          << " \t\t\t\t Se = " << pCH->Se * 100.0
          << " \t\t\t\t Ac = " << Mat.strg_gms(pCH->AcCirc * Matem::PI_GRAUS).c_str()
          << " \r\n TS = " << pCH->EstTs.GetBuffer(1)
          << " :\r\n Coor N= " << pCH->TS.y
          << " \t Coor E= " << pCH->TS.x
          << " \t\t Az= " << Mat.strg_gms(pCH->TS.fi * Matem::PI_GRAUS).c_str()
          << " \r\n SC = " << pCH->EstSc.GetBuffer(1)
          << " :\r\n Coor N= " << pCH->SC.y
          << " \t Coor E= " << pCH->SC.x
          << " \t\t Az= " << Mat.strg_gms(pCH->SC.fi * Matem::PI_GRAUS).c_str()
          << " \r\n CS = " << pCH->EstCs.GetBuffer(1)
          << " :\r\n Coor N= " << pCH->CS.y
          << " \t Coor E= " << pCH->CS.x
          << " \t\t Az= " << Mat.strg_gms(pCH->CS.fi * Matem::PI_GRAUS).c_str()
          << " \r\n ST = " << pCH->EstSt.GetBuffer(1)
          << " :\r\n Coor N= " << pCH->ST.y
          << " \t Coor E= " << pCH->ST.x
          << " \t\t Az= " << Mat.strg_gms(pCH->ST.fi * Matem::PI_GRAUS).c_str();
        else
          strstreamTemp << " \t\t\t T = " << pCH->Ts
          << " \r\n PC = " << pCH->EstTs.GetBuffer(1)
          << " :\r\n Coor N= " << pCH->TS.y
          << " \t Coor E= " << pCH->TS.x
          << " \t\t Az= " << Mat.strg_gms(pCH->TS.fi * Matem::PI_GRAUS).c_str()
          << " \r\n PT = " << pCH->EstSt.GetBuffer(1)
          << " :\r\n Coor N= " << pCH->ST.y
          << " \t Coor E= " << pCH->ST.x
          << " \t\t Az= " << Mat.strg_gms(pCH->ST.fi * Matem::PI_GRAUS).c_str();
      }
    }
    else strstreamTemp << " \r\n Est = " << pCH->EstTs.GetBuffer(1);
  }
  else    //--- é ponto de terreno ou seção de projeto
  {
    if (OBJETO == OBJ_PONTERRENO || OBJETO == OBJ_PONINSUSUARIO || OBJETO == OBJ_PONINSDELAUNAY || OBJETO == OBJ_PONINSGABRIEL ||
      OBJETO == OBJ_PONDESABILITADO || OBJETO == OBJ_GRADITERRAP || OBJETO == OBJ_TERRAPLAN || OBJETO == OBJ_ESBOCO)// || OBJETO == OBJ_PONINSSECOES)
    {
      if (MOSTRAR_NUMERO_PONTO) strstreamTemp << pPonto->Nome;
      if (MOSTRAR_DESC) strstreamTemp << " \r\n " << pPonto->Descricao.GetBuffer();

      if (OBJETO == OBJ_GRADITERRAP && pPonto->S != INFINITO)
      {
        CEstaca Estaca(pPonto->S);
        strstreamTemp << " \r\n Estaca = " << Estaca.GetBuffer();
        itsetPontos it(SetCotasVermelhas.find(Estaca.EstVirtual));
        if (it != SetCotasVermelhas.end() && it->fi < 100.0)  strstreamTemp << " \r\n CV = " << std::setprecision(2) << it->fi << std::setprecision(3);
      }

      if (MOSTRAR_COORDENADAS)
      {
        strstreamTemp << " \r\n Coor N= " << pPonto->y
          << " \r\n Coor E= " << pPonto->x;

        if (OBJETO != OBJ_ESBOCO)
        {
          strstreamTemp << " \r\n Cota = ";
          if (pPonto->z == 0.0 || pPonto->z == INFINITO) strstreamTemp << "A ser definida";
          else strstreamTemp << pPonto->z;
        }
      }
    }
    else
    {
      if (OBJETO == OBJ_PROJETO)
      {
        itMSecaoGeomPonto* itMSecao((itMSecaoGeomPonto*)pObjeto);

        strstreamTemp << " Estaca: "
          << ((CString*)((*itMSecao)->second))->GetBuffer(1)
          << " "
          << ((*itMSecao)->second)->Descricao
          << " \r\n Coor N= " << PontoSecao.y
          << " \r\n Coor E= " << PontoSecao.x
          << " \r\n Az= " << Mat.strg_gms(((*itMSecao)->first)->fi * Matem::PI_GRAUS).c_str();
      }
      else
      {
        if (OBJETO == OBJ_PONSECAVULSA)
        {
          if (!PegouPrimPonto) strstreamTemp << "Início da Seção Avulsa:";
          else strstreamTemp << "Fim da Seção Avulsa:";
          strstreamTemp << " "
            << " \r\n Coor N= " << pPonto->y
            << " \r\n Coor E= " << pPonto->x;
        }
        else
        {
          if (OBJETO == OBJ_SECAV)  //--- Ponto de seção avulsa
          {
            itmapSecoesAvulsas itSecAvulsa(*((itmapSecoesAvulsas*)pObjeto));

            CSecaoAvulsa& SecAvAtual(SecoesAvulsas.PegaLSecoesAvulsas().GetAt(itSecAvulsa->second));

            strstreamTemp << " Perfil Avulso: "
              << " \r\n Nome: " << SecAvAtual.Nome
              << " \r\n Ponto Inicial:"
              << " \r\n Coor N= " << SecAvAtual.PInicial.y
              << " \r\n Coor E= " << SecAvAtual.PInicial.x
                          << " \r\n Cota= " << (double)(SecAvAtual.PInicial.z != INFINITO ? SecAvAtual.PInicial.z : 0.00)
              << " \r\n Ponto Final:"
              << " \r\n Coor N= " << SecAvAtual.PFinal.y
              << " \r\n Coor E= " << SecAvAtual.PFinal.x
                          << " \r\n Cota= " << (double ) (SecAvAtual.PFinal.z != INFINITO ? SecAvAtual.PFinal.z : 0.00)
              << " \r\n Distância = " << SecAvAtual.Distancia
              << " \r\n Az = " << Mat.strg_gms(SecAvAtual.Azimute * Matem::PI_GRAUS).c_str();
          }
          else                     //--- é ponto de curva de nivel     
          {
            if (OBJETO == OBJ_CURNI)
            {
              strstreamTemp << " Curva de Nível: "
                            << " "
                            << " \r\n Coor N= " << PontoSecao.y
                            << " \r\n Coor E= " << PontoSecao.x;

            if(PontoSecao.z == INFINITO)
              strstreamTemp << "  \r\n Cota a definir";
            else
              strstreamTemp << " \r\n Cota= " << PontoSecao.z;
            }
            else
            {
              if (OBJETO == OBJ_LEMBRETE)
              {
                strstreamTemp << (*((ittysetLembretes*)(pObjeto)))->Titulo()
                  << "\r\n " << (*((ittysetLembretes*)(pObjeto)))->Texto();
              }
              else                     //--- é ponto de curva de nivel     
              {
                if (OBJETO == OBJ_SECAO)
                {
                  strstreamTemp << " Estaca: "
                    << (*(*itMSecao)->second)
                    << " "
                    << (*itMSecao)->second->Descricao
                    << " \r\n Coor N= " << PontoSecao.y
                    << " \r\n Coor E= " << PontoSecao.x
                    << " \r\n Cota Terreno= ";
                  if ((*itMSecao)->second->Cota == INFINITO)  strstreamTemp << " A Calcular";
                  else strstreamTemp << (*itMSecao)->second->Cota;
                }
                else
                {
                  if (OBJETO == OBJ_SINPONCTRL)
                  {
                    strstreamTemp << "PCTRL" << NumPonCtrl + 1;
                  }
                  else
                  {
                    if (OBJETO == OBJ_PONCGBACIAHIDROG || OBJETO == OBJ_PONFOZBACIAHIDROG || OBJETO == OBJ_PONTO_COTA_IBGE || OBJETO == OBJ_PONTO_PROCURADO)
                    {
                      strstreamTemp << " \r\n\r\n Coor N= " << pPonto->y
                                    << " \r\n Coor E= " << pPonto->x;

                      if(OBJETO != OBJ_PONTO_PROCURADO)
                      {
                        strstreamTemp << "\r\n Cota = ";

                        if (pPonto->z == INFINITO) strstreamTemp << " A ser definida ";
                        else strstreamTemp << pPonto->z;

                        strstreamTemp  << std::setprecision(1);

                        if(OBJETO == OBJ_PONCGBACIAHIDROG && pPonto->S != INFINITO) strstreamTemp << "\r\n Distância até a foz = " << pPonto->S;
                      }
                    }
                    else
                    {
                      if (OBJETO == OBJ_OAC)
                      {
                        ittymapEquipDrenagem* itOAC((ittymapEquipDrenagem*)pObjeto);

                        CEstaca Estaca((*itOAC)->second.PegaEstacaEixo());
                        itlstitsetPontos itpPonto = (*itOAC)->second.LstItPontos.begin();
                        Ponto PontoCerto;

                        if ((*itOAC)->second.Indicadores & CEquipDrenagem::PONTO_ALA_ESQUERDA)   //--- Pegou o ponto da ala esquerda
                        {
                          //--- 26-02-2022
                          //--- Não sei porque o ponto pode ser o primeiro ou o 2o ponto da lista, deveria estar em ordem da esquerda para a direita
                          // --- #VERIFICAR -> Verificar quando calcula o azimute na definição do bueiro e a funç~´ao VerifInvertido() ou coisa assim
                          // 
                          //--- Se ele for o 1o ponto pega o begin, se for o segundo pega o end

                          if ((*(*itOAC)->second.LstItPontos.begin())->Baliza2 & CEquipDrenagem::PONTO_ALA_ESQUERDA)
                            PontoCerto = *(*(*itOAC)->second.LstItPontos.begin());
                          else
                            PontoCerto = *(*(*itOAC)->second.LstItPontos.rbegin());

                          if (Superficie.EstaRotacionada())
                          {
                            PontoCerto.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), -AnguloRotacao);
                          }
                          strstreamTemp << "\r\n " << (*itOAC)->second.PegaNome() << " - Ala Esquerda"
                                        << "\r\n Coord N = " << PontoCerto.y
                                        << "\r\n Coord E = " << PontoCerto.x
                                        << "\r\n Cota = " << PontoCerto.z

                                        << "\r\n Dist = " << std::setprecision(2) << PontoCerto.S << std::setprecision(3)
                                        << "\r\n Azimute = " << Mat.strg_gms((*(*itOAC)->second.LstItPontos.begin())->fi * Matem::PI_GRAUS).c_str()
                                        << "\r\n Esconsidade = " << Mat.strg_gms((*itOAC)->second.Esconsidade() * Matem::PI_GRAUS).c_str();
                        }
                        else
                        {
                          if ((*itOAC)->second.Indicadores & CEquipDrenagem::PONTO_ALA_DIREITA)
                          {
                            //--- Se ele for o 1o ponto pega o begin, se for o segundo pega o end

                            if ((*(*itOAC)->second.LstItPontos.begin())->Baliza2 & CEquipDrenagem::PONTO_ALA_DIREITA)
                              PontoCerto = *(*(*itOAC)->second.LstItPontos.begin());
                            else
                              PontoCerto = *(*(*itOAC)->second.LstItPontos.rbegin());

                            if (Superficie.EstaRotacionada())
                            {
                              PontoCerto.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), -AnguloRotacao);
                            }
                            strstreamTemp << "\r\n " << (*itOAC)->second.PegaNome() << " - Ala Direita"
                              << "\r\n Coord N = " << PontoCerto.y
                              << "\r\n Coord E = " << PontoCerto.x
                              << "\r\n Cota = " << PontoCerto.z
                              << "\r\n Dist = " << std::setprecision(2) << PontoCerto.S << std::setprecision(3)
                              << "\r\n Azimute = " << Mat.strg_gms((*(*itOAC)->second.LstItPontos.begin())->fi * Matem::PI_GRAUS).c_str()
                              << "\r\n Esconsidade = " << Mat.strg_gms((*itOAC)->second.Esconsidade() * Matem::PI_GRAUS).c_str();
                          }
                          else            //--- Ponto no eixo - OAC
                          {
                            strstreamTemp << "\r\n " << (*itOAC)->second.PegaNome();

                            if (CEstaca((*itOAC)->second.PegaEstacaEixo()).Find("INF") == -1)
                              strstreamTemp << "\r\n Estaca" << CEstaca((*itOAC)->second.PegaEstacaEixo());

                            Ponto POAC(**((*itOAC)->second.LstItPontos.begin()));
                            if(Superficie.EstaRotacionada())
                            {
                              POAC.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), -AnguloRotacao);
                            }
                            strstreamTemp << "\r\n Coord N = " << POAC.y
                              << "\r\n Coord E = " << POAC.x;

                            if((*itOAC)->second.PegaTipoEquip() == CAciTopografico::EQUI_BUEIRO || (*itOAC)->second.PegaTipoEquip() == CAciTopografico::EQUI_BUEIRO_RETANG)
                            {
                            if((*itOAC)->second.Esconsidade() != INFINITO)
                              strstreamTemp << "\r\n Esconsidade = " << Mat.strg_gms((*itOAC)->second.Esconsidade() * Matem::PI_GRAUS).c_str();
                            }
                            strstreamTemp << "\r\n Azimute = " << Mat.strg_gms((*(*itOAC)->second.LstItPontos.begin())->fi * Matem::PI_GRAUS).c_str();

                            if ((*itOAC)->second.Perimetro != INFINITO)
                              strstreamTemp << "\r\n Comprimento = " << std::setprecision(2) << (*itOAC)->second.Perimetro << std::setprecision(3);
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  strstreamTemp << std::ends;

  const std::string Ret = strstreamTemp.str();

  return Ret;
}

void DesPontosGeomView::DesenhaProjeto(ItCMCurHorizontaisTS pitMCH, CDC* pDC, int OperacaoForcada, unsigned int Quantidade)
{
  CPen PenaProjeto,* PAnterior;

  PenaProjeto.CreatePen(PS_SOLID, 2, Cores[CORPROJETO]);
  PAnterior = pDC->SelectObject(&PenaProjeto);

  //--- Desenha o projeto: Existem duas possibilidades ou é para desenhar todo o projeto ou é para se redesenhar as curva que estão sendo alteradas devido
  //--- ao movimento de um pi. Para o primeiro caso "OperacaoForcada" e "Quantidade" vêm com os valores default, OPR_AMBOS E \xFFFF, Respectivamente. Para o
  //--- Caso de ser o movimento de um PI estas duas variáveis vêm com seus respectivos valores, dependendo do caso.

  ItCMCurHorizontaisTS itMCH(pitMCH);

  for (ittymapCTrechos it = Trechos.MapaTrechos.begin(); it != Trechos.MapaTrechos.end(); it++)
  {
    if (it->second.mapCurvasHorizontaisTS.size() == 0) continue;    //--- O trecho pode ter sido recém criado, está sem projeto

    //--- Se !OPR_AMBAS está movendo um pi, redesenha apenas as curvas envolvidas do trecho atual
    //--- A primeira curva veio em itMCH

    if (OperacaoForcada != OPR_AMBAS)
    {
      if (it != Trechos.PegaTrechoAtual()) continue;
    }
    else itMCH = it->second.mapCurvasHorizontaisTS.begin();       //--- Senão desenha todo o projeto do trecho

    if (itMCH->second.Raio < 0.001)
    {
      long xST((itMCH->second.ST.x - Deltas[X]) * Escala[X]),
           yST((AlturaVideo - (itMCH->second.ST.y - Deltas[Y])) * Escala[Y]);

      pDC->Arc(xST - 4, yST + 4, xST + 4, yST - 4, 0, 0, 0, 0);

      //DesenharPonto(xST,yST,pDC,Cores[CORPROJETO]);
      pDC->MoveTo(xST, yST);

      itMCH++;
    }
    else  pDC->MoveTo((itMCH->second.TS.x - Deltas[X]) * Escala[X], (AlturaVideo - (itMCH->second.TS.y - Deltas[Y])) * Escala[Y]);

    //--- Desenha o eixo e os bordos das  3 curvas  envolvidas a partir da curva passada, desenha até o fim do set ou ate a quantidade solicitada.

    long yPI, xPI;

    for (int Quant = Quantidade; itMCH != it->second.mapCurvasHorizontaisTS.end() /* mapCurvasHorizontaisTS.end()*/ && Quant--; itMCH++)
    {
      xPI = (itMCH->second.PI.x - Deltas[X]) * Escala[X];
      yPI = (AlturaVideo - (itMCH->second.PI.y - Deltas[Y])) * Escala[Y];

      //   pDC->Arc(xPI-4,yPI+4,xPI+4,yPI-4,0,0,0,0);

      pDC->LineTo(xPI, yPI);
      if (itMCH->second.Tipo == ESPIRAL && itMCH->second.Lc > 0.0) DesenhaEspiral(itMCH->second, pDC);
      if (itMCH != /*--mapCurvasHorizontaisTS.end()*/--it->second.mapCurvasHorizontaisTS.end())
      {
        DesenhaCircular(itMCH->second, pDC, it, OperacaoForcada == OPR_APAGAR);
      }
      pDC->MoveTo(xPI, yPI);    //--- volta ao ponto para desenhar a próxima tangente

      pDC->Arc(xPI - 4, yPI + 4, xPI + 4, yPI - 4, 0, 0, 0, 0);
    }

    for (int Lado = ESQUERDO; Lado <= DIREITO; Lado++)
    {
      Ponto PAux;
      //  itMCH = pitMCH;
      itMCH = it->second.mapCurvasHorizontaisTS.begin();       //--- Senão desenha todo o projeto do trecho

      double Larguras[2];

      it->second.SecoesTipo.BuscaLargurasPistas(itMCH->second.EstTs, Larguras[ESQUERDO], Larguras[DIREITO]);

      IrradiaPonto(Mat.M_PI_2, itMCH->second.ST, Larguras[Lado], PAux);

      pDC->MoveTo((PAux.x - Deltas[X]) * Escala[X], (AlturaVideo - (PAux.y - Deltas[Y])) * Escala[Y]);

      //--- Irradia os pontos nos bordos e desenha as retas

      itMCH++;

      for (int Quant = Quantidade; itMCH != it->second.mapCurvasHorizontaisTS.end()/* mapCurvasHorizontaisTS.end()*/ && --Quant; itMCH++)
      {
        it->second.SecoesTipo.BuscaLargurasPistas(itMCH->second.EstTs, Larguras[ESQUERDO], Larguras[DIREITO]);
        IrradiaPonto(Mat.M_PI_2, itMCH->second.TS, Larguras[Lado], PAux);
        pDC->LineTo(((PAux.x - Deltas[X]) * Escala[X]), (AlturaVideo - (PAux.y - Deltas[Y])) * Escala[Y]);

        it->second.SecoesTipo.BuscaLargurasPistas(itMCH->second.EstSt, Larguras[ESQUERDO], Larguras[DIREITO]);
        IrradiaPonto(Mat.M_PI_2, itMCH->second.ST, Larguras[Lado], PAux);
        pDC->MoveTo((PAux.x - Deltas[X]) * Escala[X], (AlturaVideo - (PAux.y - Deltas[Y])) * Escala[Y]);
      }
    }
  }
}

void DesPontosGeomView::DesenhaSuperficie(ItSSuperficie DeltaSupInicial, ItSSuperficie DeltaSupFinal, CDC* pDC, CSuperficie* pSuperfTerrapl)
{
  CPen Pena, PenaR,* PAnterior;
  Ponto Ppixels,PAtual;
  const Ponto* pPontoAtual;

  Pena.CreatePen(PS_SOLID, 1, Cores[CORPONTOS]);
  PenaR.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

  PAnterior = pDC->SelectObject(&Pena);

  //--- Projeto drones e superficie do terreno não desenha os pontos (porque estão nas CNs)

  int Tiposuperf((((CMainFrame*)AfxGetMainWnd())->TipoSuperficie));

  if (Tiposuperf == SUPERF_TRADICIONAL || Tiposuperf == SUPERF_TOPOG_ACAD || Tiposuperf == SUPERF_GOOGLEEARTH)
  {
    if (DesenharPontos)
    {
      register ItSSuperficie DeltaSupAtual(DeltaSupInicial);
      bool PontoRadar(false);

      for (; DeltaSupAtual != DeltaSupFinal; DeltaSupAtual++)
      {
        if (DeltaSupAtual->PegaPCentral().Inserido == true && !DesenharPontosInseridos)
        {
          continue;
        }

        PontoRadar = false;

        if (MostrarRadar && SetPontosNoRadar.find(DeltaSupAtual) != SetPontosNoRadar.end())
        {
          PontoRadar = true;
        }

        if (PontoRadar == false)
        {
          PAtual = (*DeltaSupAtual).PegaPCentral();

          Ppixels.x = ((PAtual.x - Deltas[X]) * Escala[X]);
          Ppixels.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y];

          if (DesenharPixel) pDC->SetPixel(Ppixels.x, Ppixels.y, Cores[CORPONTOS]);

          if (DesenharCruz)
          {
            //--- Desenha a cruz, 5 pixels de largura

            pDC->MoveTo(Ppixels.x - 5, Ppixels.y);
            pDC->LineTo(Ppixels.x + 5, Ppixels.y);
            pDC->MoveTo(Ppixels.x, Ppixels.y - 5);
            pDC->LineTo(Ppixels.x, Ppixels.y + 5);
          }

          //--- circula o ponto

          if (DesenharCirculo) pDC->Arc(Ppixels.x - 3, Ppixels.y - 3, Ppixels.x + 3, Ppixels.y + 3, 0, 0, 0, 0);
        }
        else
        {
          PAtual = (*DeltaSupAtual).PegaPCentral();

          Ppixels.x = ((PAtual.x - Deltas[X]) * Escala[X]);
          Ppixels.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y];

          if (RadarTodosPontos)  pDC->Arc(Ppixels.x - 3, Ppixels.y - 3, Ppixels.x + 3, Ppixels.y + 3, 0, 0, 0, 0);

          if (RadarRealcarPontosInteresse)
          {
            if (SetPontosRadarInteresse.find(DeltaSupAtual) != SetPontosRadarInteresse.end())
            {
              if (!RadarTodosPontos) pDC->Arc(Ppixels.x - 3, Ppixels.y - 3, Ppixels.x + 3, Ppixels.y + 3, 0, 0, 0, 0);

              pDC->SelectObject(&PenaR);
              pDC->Arc(Ppixels.x - 5, Ppixels.y - 5, Ppixels.x + 5, Ppixels.y + 5, 0, 0, 0, 0);
              pDC->SelectObject(&Pena);
            }
          }
        }
      }
    }
  }
  else
  {
    if (DesenharPontos)  
    {
      //--- Não mostrar os pontos soltos DGN

      if (0 && Superficie.PegaPontosSoltosMapa().size() > 0)
      {
        pDC->SelectObject(&Pena);
        for (register itsetPontos itPAtual = Superficie.PegaPontosSoltosMapa().begin(); itPAtual != Superficie.PegaPontosSoltosMapa().end(); itPAtual++)
        {
          Ppixels.x = ((itPAtual->x - Deltas[X]) * Escala[X]);
          Ppixels.y = (AlturaVideo - (itPAtual->y - Deltas[Y])) * Escala[Y];

          if (DesenharPixel) pDC->SetPixel(Ppixels.x, Ppixels.y, Cores[CORPONTOS]);
          if (DesenharCruz)
          {
            pDC->SelectObject(&Pena);
            pDC->MoveTo(Ppixels.x - 7, Ppixels.y);
            pDC->LineTo(Ppixels.x + 7, Ppixels.y);
            pDC->MoveTo(Ppixels.x, Ppixels.y - 7);
            pDC->LineTo(Ppixels.x, Ppixels.y + 7);
          }
          if (DesenharCirculo) pDC->Arc(Ppixels.x - 4, Ppixels.y - 4, Ppixels.x + 4, Ppixels.y + 4, 0, 0, 0, 0);
        }
      }
    }
  }

 // if(1)
 if (Tiposuperf != SUPERF_DRONES && Tiposuperf != SUPERF_GERENMAPAS && Tiposuperf != SUPERF_SIG)
  {
   // if(1)
    if (DesenharArestas && !pSuperfTerrapl)
    {
      CPen PenaArestas;
      PenaArestas.CreatePen(PS_SOLID, 1, Cores[CORARESTAS]);
      pDC->SelectObject(&PenaArestas);

      register ItSSuperficie DeltaSupAtual(DeltaSupInicial);

      for (; DeltaSupAtual != DeltaSupFinal; ++DeltaSupAtual)
      {
        const Ponto& PCentral(DeltaSupAtual->PegaPCentral());   //--- Pega o ponto atual
        Ponto PPixelsCentral(((PCentral.x - Deltas[X]) * Escala[X]), (AlturaVideo - (PCentral.y - Deltas[Y])) * Escala[Y]);

        for (register ittysetArestas PLigAtual = DeltaSupAtual->PontosLigados.begin(); PLigAtual != DeltaSupAtual->PontosLigados.end(); PLigAtual++)
        {
          pPontoAtual = &PLigAtual->PontoLigado;

          //--- É MAIS RÁPIDO DESENHAR A ARESTAS VARIAS VEZES QUE CONTROLAR SE JA DESENHOU OU NAO ...
          //---===> isso foi há 15 anos atras, refazer o teste e verificar qual é o mais rápido nos comutadores de hoje

          //        if(Superficie.pSuperficieAtual->find(*pPontoAtual)->PegaPCentral().Baliza == false)  //--- Se o ponto ainda nao foi desenhado
          {
            Ppixels.x = ((pPontoAtual->x - Deltas[X]) * Escala[X]);
            Ppixels.y = (AlturaVideo - (pPontoAtual->y - Deltas[Y])) * Escala[Y];

            pDC->MoveTo(PPixelsCentral.x, PPixelsCentral.y);
            pDC->LineTo(Ppixels.x, Ppixels.y);
          }
        }
      }
    }
  }

  if (pSuperfTerrapl && pSuperfTerrapl->PegaListaRestricoes().size() == 0)
  {
    DesenhaAlinhamentosTerrapl(pDC);
  }

  if (0 && Superficie.SetPonInsSecoes().size() > 0)
  {
    CPen PenaAr;
    PenaAr.CreatePen(PS_SOLID, 2, RGB(255,255,255));
    pDC->SelectObject(&PenaAr);

    for (register itsetPontos itPAtual = Superficie.SetPonInsSecoes().begin(); itPAtual != Superficie.SetPonInsSecoes().end(); itPAtual++)
    {
      Ppixels.x = ((itPAtual->x - Deltas[X]) * Escala[X]);
      Ppixels.y = (AlturaVideo - (itPAtual->y - Deltas[Y])) * Escala[Y];

      if (DesenharPixel) pDC->SetPixel(Ppixels.x, Ppixels.y, Cores[CORPONTOS]);

      if (DesenharCruz)
      {
        //--- Desenha a cruz, 5 pixels de largura

        pDC->MoveTo(Ppixels.x - 5, Ppixels.y);
        pDC->LineTo(Ppixels.x + 5, Ppixels.y);
        pDC->MoveTo(Ppixels.x, Ppixels.y - 5);
        pDC->LineTo(Ppixels.x, Ppixels.y + 5);
      }

      //--- circula o ponto

      if (DesenharCirculo) pDC->Arc(Ppixels.x - 3, Ppixels.y - 3, Ppixels.x + 3, Ppixels.y + 3, 0, 0, 0, 0);
    }
  }

  pDC->SelectObject(PAnterior);
}

void DesPontosGeomView::OnButrotacionar() 
{
  double NovoAngulo(0.0);
  int Absoluto_Relativo(0);

  CDRotacionar DRotacionar(this, Superficie.AngRotacaoTopogGraus(), &NovoAngulo, &Absoluto_Relativo);

  if (ZerarAzimute || AnguloRotacaoAnterior != 0.0 || DefinindoLinhaBase == 3 || DRotacionar.DoModal() == IDOK)
  {
    Rotaciona(NovoAngulo, Absoluto_Relativo);
    /*
    SetCursor(LoadCursor(NULL, IDC_APPSTARTING));

    Ponto PCentral(Deltas[X] + LarguraVideo / 2.0, Deltas[Y] + AlturaVideo / 2.0);

    double AngComplementar(Mat.M_2_PI - Superficie.PegaAnguloRotacao()),
	       AnguloArotacionar(0.0);

    PCentral.RotacionaPlano(sin(AngComplementar), cos(AngComplementar), -AngComplementar);

    AlterouProjeto = AlterouTerreno = AlterouAngulo = true;
    GetDocument()->SetModifiedFlag(true);

    if (DefinindoLinhaBase != 3)
    {
      if (!ZerarAzimute && AnguloRotacaoAnterior == 0.0)       //--- se true && true o ângulo veio do diálogo
      {
        if (NovoAngulo < 0.0) NovoAngulo += 360.0;
        NovoAngulo = 360.0 - NovoAngulo;
      }
      else                    //--- Ou é para zerar o azimute ou é para rotacionar o angulo de rotaçao anterior (Está lendo a superfície rotacionada)
      {
        if (ZerarAzimute)
        {
          NovoAngulo = 0.0;
          Absoluto_Relativo = true;
        }
        else  NovoAngulo = AnguloRotacaoAnterior * 180.0 / Mat.M_PI;
      }

      AnguloArotacionar = (Absoluto_Relativo ? (NovoAngulo / 180.0 * Mat.M_PI) - Superficie.PegaAnguloRotacao() : (NovoAngulo / 180.0 * Mat.M_PI));

      if (fabs(AnguloArotacionar) < Mat.UmGrau_Rad) return;
    }
    else
    {
      Ponto RP(Mat.R_P(PontosLinhaBase[1], PontosLinhaBase[0]));
      RP.x -= fmod(RP.x, Mat.UmGrau_Rad);
      AnguloArotacionar = RP.x + Mat.M_PI_2;
      if (AnguloArotacionar > Mat.M_2_PI) AnguloArotacionar -= Mat.M_2_PI;
      else  if (AnguloArotacionar < 0.0) AnguloArotacionar += Mat.M_2_PI;
    }

    if (Superficie.PegaAnguloRotacao() + AnguloArotacionar < 0.0)  AnguloArotacionar += Mat.M_2_PI;
    if (Superficie.PegaAnguloRotacao() + AnguloArotacionar > Mat.M_2_PI)  AnguloArotacionar -= Mat.M_2_PI;

    double Sin(sin(AnguloArotacionar)), Cos(cos(AnguloArotacionar));
    Ponto PontoSobMouseAnterior;

    if (DeltaSupSobMouseAnterior != Superficie.pSuperficieAtual->end()) PontoSobMouseAnterior = DeltaSupSobMouseAnterior->PegaPCentral();

    Superficie.RotacionaPlano(Sin, Cos, AnguloArotacionar);
    RotacionaBordoseRestricoes(Sin, Cos, AnguloArotacionar);
    RotacionaSecoes(Sin, Cos, AnguloArotacionar);
    RotacionaSecoesAvulsas(Sin, Cos, AnguloArotacionar);
    RotacionaSecoesTerrap(Sin, Cos, AnguloArotacionar);
    RotacionaSimbologia(Sin, Cos, AnguloArotacionar);

    for (ittymapCTrechos it = Trechos.MapaTrechos.begin(); it != Trechos.MapaTrechos.end(); it++)
    {
      it->second.SuperficieTerr.RotacionaPlano(Sin, Cos, AnguloArotacionar);
      it->second.CurvasHorizontais.RotacionaPlano(Sin, Cos, AnguloArotacionar);
    }

    DeltaSupSobMouseAnterior = DeltaSupAnterior = Superficie.pSuperficieAtual->end();

    SetPontosNoRadar.clear();
    SetPontosRadarInteresse.clear();

    ItInicioSarjeta = ItFimSarjeta = Trechos.PegaTrechoAtual()->second.SuperficieTerr.pSuperficieAtual->end();

    AnguloArotacionar = AnguloRotacao = Superficie.PegaAnguloRotacao();
    Sin = Seno = sin(AnguloRotacao);
    Cos = Coseno = cos(AnguloRotacao);
    Quadrante = floor(AnguloRotacao / Mat.M_PI_2);
    if (Quadrante == 4) Quadrante = 3;

    CriaFontes(true);

    SetCursor(LoadCursor(NULL, IDC_ARROW));

    CalculaNovasDimensoes();
    PCentral.RotacionaPlano(Sin, Cos, AnguloArotacionar);
    CentralizarTela(PCentral);

    DeltaSupSobMouseAnterior = DeltaSupAnterior = Superficie.pSuperficieAtual->end();

    AlterouTerreno = true;
    */
  }
  else
    if (DefinindoLinhaBase == 0 && NovoAngulo == -10000.0)
    {
      CursorAtual = IDC_CROSS;
      DefinindoLinhaBase = 1;  //--- Pegando primeiro ponto
    }
}

void DesPontosGeomView::DefineParametros()
{
  //--- Calcula a escala padrão que permite a visualização todo o desenho

  double LimiteEsq(Superficie.PegaLimiteEsq()), LimiteDir(Superficie.PegaLimiteDir()), LimiteSup(Superficie.PegaLimiteSup()), LimiteInf(Superficie.PegaLimiteInf());

  if (Trechos.TemTrechoAtual() && Trechos.PegaTrechoAtual()->second.CurvasHorizontais.GetCount() > 1)
  {
    if (LimiteEsq > Trechos.PegaTrechoAtual()->second.CurvasHorizontais.PegaLimiteEsq()) LimiteEsq = Trechos.PegaTrechoAtual()->second.CurvasHorizontais.PegaLimiteEsq();
    if (LimiteDir < Trechos.PegaTrechoAtual()->second.CurvasHorizontais.PegaLimiteDir()) LimiteDir = Trechos.PegaTrechoAtual()->second.CurvasHorizontais.PegaLimiteDir();
    if (LimiteSup < Trechos.PegaTrechoAtual()->second.CurvasHorizontais.PegaLimiteSup()) LimiteSup = Trechos.PegaTrechoAtual()->second.CurvasHorizontais.PegaLimiteSup();
    if (LimiteInf > Trechos.PegaTrechoAtual()->second.CurvasHorizontais.PegaLimiteInf()) LimiteInf = Trechos.PegaTrechoAtual()->second.CurvasHorizontais.PegaLimiteInf();
  }

  EscalaDefault[X] = Resolucao[X] / (LimiteDir - LimiteEsq);
  EscalaDefault[Y] = Resolucao[Y] / (LimiteSup - LimiteInf);

  EscalaDefault[X] < EscalaDefault[Y] ? EscalaDefault[Y] = EscalaDefault[X] : EscalaDefault[X] = EscalaDefault[Y];

  if (EscalaDefault[X] < .001)									//--- A escala está muito pequena, provavelmente o projeto está muito longe do terreno, erro de coordenadas.
  {																					  //--- Tenta melhorar, Recalcula a escala considerando apenas o terreno; 
    LimiteEsq = Superficie.PegaLimiteEsq();
    LimiteDir = Superficie.PegaLimiteDir();
    LimiteSup = Superficie.PegaLimiteSup();
    LimiteInf = Superficie.PegaLimiteInf();

    EscalaDefault[X] = Resolucao[X] / (LimiteDir - LimiteEsq);
    EscalaDefault[Y] = Resolucao[Y] / (LimiteSup - LimiteInf);

    EscalaDefault[X] < EscalaDefault[Y] ? EscalaDefault[Y] = EscalaDefault[X] : EscalaDefault[X] = EscalaDefault[Y];
  }

  if (EscalaDefault[X] < .001)	EscalaDefault[X] = EscalaDefault[Y] = 0.01;

  //--- coordenadas do canto inferior esquerdo, real e rotacionado

  DeltasReais.x = DeltasDefault[X] = LimiteEsq;
  DeltasReais.y = DeltasDefault[Y] = LimiteInf;

  LimiteYMetros.y = DeltasReais.y + LarguraVideo * sin(AnguloRotacao);
  LimiteYMetros.x = DeltasReais.x + AlturaVideo * sin(AnguloRotacao);

  CalculaParametros();
}

void DesPontosGeomView::OnUpdateButrotacionar(CCmdUI* pCmdUI)
{
 pCmdUI->Enable(Superficie.pSuperficieAtual->size() > 0 && !GeoReferenciado());
}

void DesPontosGeomView::OnUpdateRedesenhar(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(Superficie.pSuperficieAtual->size() > 0);
}

void DesPontosGeomView::OnUpdateEnquadrar(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(Superficie.pSuperficieAtual->size() > 0);
}

void DesPontosGeomView::OnUpdateButProjTerrap(CCmdUI* pCmdUI)
{
  #if defined (PACOTE_INTERMEDIARIO) || defined (PACOTE_AVANCADO) || defined(DEMOVER)
    pCmdUI->Enable(Trechos.TemTrechoAtual() && Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaQuantPontos() > 0);
  #else
    pCmdUI->Enable(FALSE);
  #endif
}

void DesPontosGeomView::OnUpdateButFronteira(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(ProjetoGeoreferenciado != 3);
}

void DesPontosGeomView::OnUpdateButAciTopog(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!MostrarTerrap);
}


void DesPontosGeomView::OnUpdateButEsbocar(CCmdUI* pCmdUI)
{
#if defined (PACOTE_AVANCADO) || defined(DEMOVER)
  pCmdUI->Enable(!MostrarTerrap);
#else
  pCmdUI->Enable(FALSE);
#endif
}
void DesPontosGeomView::OnUpdateButSinPontosCtrl(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(NomeMapa.GetLength() > 0 && RazaoEscalasJPGDes == INFINITO);
}

void DesPontosGeomView::OnUpdateBaixarMapa(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(false);

  if(false)
  {
    #ifdef PACOTE_AVANCADO
      pCmdUI->Enable(ProjetoGeoreferenciado == SUPERF_TRADICIONAL);
    #else
    pCmdUI->Enable(false);
    #endif
  }
}

void DesPontosGeomView::OnUpdateButradar(CCmdUI* pCmdUI)
{
//  pCmdUI->Enable(ProjetoGeoreferenciado == SUPERF_TRADICIONAL);
  pCmdUI->Enable(true);
}

void DesPontosGeomView::OnUpdateButPonIns(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!MostrarTerrap);
}

void DesPontosGeomView::OnDefinebordos()
{
  DesenharPontosInseridos = !DesenharPontosInseridos;

  RedrawWindow();
}

void DesPontosGeomView::CriaFontes(bool DestruirAnteriores)
{
  if (DestruirAnteriores)    //--- Se é a 2a vez é preciso destruir as anteriores
  {
    CurrierNewHorizontal.DeleteObject();
    CurrierNewVertical.DeleteObject();
  }

  CDC* pDC = GetDC();
  if (!pDC) return;                   //--- Segurança, aconteceu nos testes

  CFont* LF = pDC->GetCurrentFont();
  LOGFONT Log;

  LF->GetLogFont(&Log);
  Log.lfEscapement = AnguloRotacao * 1800 / 3.14;

  strcpy(Log.lfFaceName, "Arial");
  Log.lfHeight = 18;
  CurrierNewHorizontal.CreateFontIndirect(&Log);

  Log.lfEscapement = (AnguloRotacao + 3.14 / 2.0) * 1800 / 3.14;
  Log.lfHeight = 18;
  strcpy(Log.lfFaceName, "Arial");

  CurrierNewVertical.CreateFontIndirect(&Log);

  ReleaseDC(pDC);
}

void DesPontosGeomView::CriaFontesCN(bool DestruirAnteriores, double Angulo)
{
  if (DestruirAnteriores)    //--- Se é a 2a vez é preciso destruir as anteriores
  {
    CourierNewCN.DeleteObject();
  }

  CDC* pDC = GetDC();
  if (!pDC) return;                   //--- Segurança, aconteceu nos testes

  CFont* LF = pDC->GetCurrentFont();
  LOGFONT Log;

  LF->GetLogFont(&Log);

  Log.lfEscapement = Angulo * 1800.0 / 3.14;
  Log.lfHeight = 18;
  strcpy(Log.lfFaceName, "Arial");

  CourierNewCN.CreateFontIndirect(&Log);

  ReleaseDC(pDC);
}

void DesPontosGeomView::CalculaNovasDimensoes()
{
  AlturaRebatidaYPixels = AlturaVideoPixels * cos(AnguloRotacao);
  LarguraRebatidaYPixels = LarguraVideoPixels * -sin(AnguloRotacao);
  NovaAlturaPixels = AlturaRebatidaYPixels + LarguraRebatidaYPixels;
  NovaLarguraPixels = LarguraVideoPixels * cos(AnguloRotacao) + AlturaVideoPixels * -sin(AnguloRotacao);
}

void DesPontosGeomView::OnSetFocus(CWnd* pOldWnd)
{
  //  CView::OnSetFocus(pOldWnd);

  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());

  //--- DesPontosGeo é usado no CChildfrm::OnClose

 // if (MainFrame->DesPontosGeo != this) 

  MainFrame->DesPontosGeo = this;

  DesSecaoView* pDesSecoes((DesSecaoView*)MainFrame->PegaDesenhoSecoes());
  if (pDesSecoes) pDesSecoes->GetParentFrame()->BringWindowToTop();

  DesCroquisView* pDesCroquis((DesCroquisView*)MainFrame->PegaDesenhoCroquis());
  if (pDesCroquis) pDesCroquis->GetParentFrame()->BringWindowToTop();

  ((CChildFrame*)GetParentFrame())->DeslizanteGeom.PostMessage(WM_SETFOCUS, 0, 0);  //--- SetFocus anula o mousewheel

  if (1) //0 && MainFrame->SelecionaRua(strTrechoAtual) == CB_ERR)
  {
    {
      int Sel(MainFrame->m_ProjetosBox.FindString(0, Projeto.c_str()));

      if (Sel == LB_ERR)
      {
        MainFrame->m_ProjetosBox.InsertString(0, Projeto.c_str());
        MainFrame->m_ProjetosBox.SetCurSel(0);
        MainFrame->m_sNomeComp = Projeto.c_str();
      }
      else
      {
        if (Sel != 0)    //--- Se não é o projeto atual
        {
          MainFrame->m_ProjetosBox.SetCurSel(Sel);
          MainFrame->m_sNomeComp = Projeto.c_str();
          ((CEstApp*)AfxGetApp())->MudouProjetoSelecionado(Projeto);
        }
      }
    }
  }
}

void DesPontosGeomView::OnAcitopog()
{
  AlterouTerreno = Superficie.DefineBordos();

  RedrawWindow();
}

void DesPontosGeomView::OnDefAciTopog()
{
  CChildFrame* CCFrm((CChildFrame*)GetParentFrame());
  CCFrm->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTBORDOS, DefBordos > 0 ? TBSTATE_ENABLED : TBSTATE_PRESSED);
  CCFrm->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTACITOPOG, DefBordos > 0 ? TBSTATE_PRESSED : TBSTATE_ENABLED);

  DefBordos = false;
}

void DesPontosGeomView::OnTalveguePrincipal()
{
  DefinindoTalveguePrincipal = DefinindoTalveguePrincipal == false;

  if(!DefinindoTalveguePrincipal && TalveguePrincipal.size() > 0)
  {
    MontaTalveguePrincipal();
    ItLLEspigaoMarcado = Superficie.PegaListaRestricoes().end();
    ItBaciaMarcada = Superficie.PegaBacias().end();
  }
  else
  {
    ItBaciaMarcada = VerfClicouBaciaHidrografica(ItLLDeltaSupMarcada);

    if (ItBaciaMarcada != Superficie.PegaBacias().end())
    {
      if(dialogar.DoModal(-1,"Definindo o talvegue principal a bacia terá que ser redefinida, confirma?") == IDOK)
      {
        TalveguePrincipal.clear();
        TalveguesDoTalveguePrincipal.clear();

        Superficie.PegaBacias().erase(ItBaciaMarcada);
      }
    }

    dialogar.DoModal(-1,"Clique no último ponto a montante do talvegue principal"); //--- Aqui

    RedrawWindow();

    ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();
  }
}

void DesPontosGeomView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  if (NaoPermitirAlteracoes) return;

  if (CriandoRestricao || PegandoPontoSecAvulsa || EsbocandoObjetos || ObjetosDrenagem)
  {
    CriandoRestricao = EsbocandoObjetos = ObjetosDrenagem = PegandoPontoSecAvulsa = false;
    ItListaRestAtual = Superficie.PegaListaRestricoes().end();
    AlterouSecoesAvulsas = AlterouProjeto = true;

    RedrawWindow();

    return;
  }

  if (Trechos.TemTrechoAtual())
  {
    Ponto P;
    TransfPixelsXY(point, P.x, P.y);
    P.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());

    //--- Se a distancia entre o o ponto clicado e alguma tangente estiver dentro da tolerancia,
    //--- insere um pi com raio padrão neste ponto e coloca o programa no modo de arrastar ponto, deste modo o ponto inserido 
    //--- poderá será arrastado com os movimentos do mouse. No itCHAnterior retorna a curva anterior à que está sendo inserida
    //--- o pi atual tem que ser inserido apos esta curva, para isso as estaca do Ts,Sc,Cs e ST são feitas a principio
    //--- iguais às do Ts da curva anterior + 0,00005. Assim que a curva for recalculada através da chamada a AlteraCurva, o estaqueamento 
    //--- será corrigido.

    std::string strTrechoAtual(((CMainFrame*)AfxGetMainWnd())->PegaRuaAtual());
    Trechos.TrocaTrecho(strTrechoAtual);

    Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS.begin();

    ItCMCurHorizontaisTS ItCHAnterior(Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS.begin());
    bool Inseriu(false);
    CurHorizontal CH;

    if (Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS.size() < 2)
    {
      if (Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS.size() == 0)  CH.EstTs = CH.EstSc = CH.EstCs = CH.EstSt = 0.0;
      else  CH.EstTs = CH.EstSc = CH.EstCs = CH.EstSt = ItCHAnterior->second.EstSt.EstVirtual + 0.01;
      CH.Raio = 0.0;

      Inseriu = true;
    }
    else
      if (VerificaTangente(P, ItCHAnterior, &strTrechoAtual))
      {
        Trechos.TrocaTrecho(strTrechoAtual);
        ((CMainFrame*)AfxGetMainWnd())->TrocaRua(strTrechoAtual);

        CEstaca TsAux = ItCHAnterior->second.EstSt;
        TsAux += 0.005;                             //--- Para que a curva fique na posição correta no set ordenado por TS

        CH.EstTs = CH.EstSc = CH.EstCs = CH.EstSt = TsAux;
        CH.Raio = 50.0;

        Inseriu = true;
      }

    if (Inseriu)
    {
      Trechos.PegaTrechoAtual()->second.CurvasHorizontais.InsereCurvaNosMapas(CH);

      static ItCMCurHorizontaisPI itCMCHPI;

      itCMCHPI = Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisPI.find(&CH.PI);

      ItCurHorizSendoArrastadaPI = &itCMCHPI;

      if (Trechos.PegaTrechoAtual()->second.CurvasHorizontais.AlteraCurvaPI(*ItCurHorizSendoArrastadaPI, P, ItUltimaCurvaAjustadaTS, NULL) != CCurHorizontais::CURVA_OK)
      {
        //--- FAZER UM ROLLBACK => #VERIFICAR
        monolog.mensagem(25, -1);
      }

      Arrastando = TerminarArrasto = false;   //--- Termina o arrasto
      OnLButtonDown(nFlags, point);
    }
  }
  else
  {
    monolog.mensagem(-1,"Não há uma rua ou trecho definido, primeiro insira uma rua ou trecho para projetar");
    TerminarArrasto = true;                   //--- Evita o arrasto
    OnLButtonUp(nFlags, point);
  }
}

void DesPontosGeomView::OnConfdes()
{
  int Dados[37];

  Dados[0] = DesenharPixel;
  Dados[1] = DesenharCirculo;
  Dados[2] = DesenharCruz;
  Dados[3] = DesenharSecoes;
  Dados[4] = DesenharArestas;
  Dados[5] = DesenharBordos;
  Dados[6] = DesenharRestricoes;
  Dados[7] = DesenharPontos;
  Dados[8] = MostrarDescricao;
  Dados[9] = DesenharMalha;
  Dados[10] = EquiMalha;
  Dados[11] = EquiCotas;
  Dados[12] = DesenharProjeto;
  Dados[13] = CircularSecoes;
  Dados[14] = MostrarToolBarSuperf;
  Dados[15] = MostrarToolBarGeom;
  Dados[16] = CotarMalha;
  Dados[17] = VarrerTerreno;
  Dados[18] = VarrerProjeto;
  Dados[19] = VarrerCN;
  Dados[20] = DesenharCN;
  Dados[21] = CotarCN;
  Dados[22] = SaltarCNs;
  Dados[23] = RadarTodosPontos;
  Dados[24] = RadarPontosInteresse;
  Dados[25] = RadarRealcarPontosInteresse;
  Dados[26] = Radar.PegaRaio();
  Dados[27] = DesenharAlinTerrap;
  Dados[28] = DesenharOffSets;
  Dados[29] = DesenharMapa;
  Dados[30] = DesenharEquipDrenagem;
  Dados[31] = DesenharSimbolTerapl;
  Dados[32] = DesenharCNTaludes;
  Dados[33] = DensidadeSimbologia;
  Dados[34] = DesenharSecoesAvulsas;
  Dados[35] = RealcarCNsMestras;

  COLORREF* CR = (COLORREF*)&Dados[36];

  *CR = Radar.PegaCorTela();

  int TipoSuperf(((CMainFrame*)AfxGetMainWnd())->TipoSuperficie);

  CDConfDesGeom DConfDesGeometrico(this,&Dados[0],&Cores[0],true,TipoSuperf,MostrarTerrap);

  DConfDesGeometrico.DoModal();

  DesenharPixel = Dados[0] != false;
  DesenharCirculo = Dados[1] != false;
  DesenharCruz = Dados[2] != false;
  DesenharSecoes = Dados[3] != false;
  DesenharArestas = Dados[4] != false;
  DesenharBordos = Dados[5] != false;
  DesenharRestricoes = Dados[6] != false;
  DesenharPontos = Dados[7] != false;
  MostrarDescricao = Dados[8] != false;
  DesenharMalha = Dados[9] != false;
  EquiMalha = Dados[10];
  EquiCotas = Dados[11];
  DesenharProjeto = Dados[12] != false;
  CircularSecoes = Dados[13] != false;
  MostrarToolBarSuperf = Dados[14] != false;
  MostrarToolBarGeom = Dados[15] != false;
  CotarMalha = Dados[16] != false;
  VarrerTerreno = Dados[17] != false;
  VarrerProjeto = Dados[18] != false;
  VarrerCN = Dados[19] != false;
  DesenharCN = Dados[20] != false;
  CotarCN = Dados[21] != false;
  SaltarCNs = Dados[22];
  RadarTodosPontos = Dados[23] != false;
  RadarPontosInteresse = Dados[24] != false;
  RadarRealcarPontosInteresse = Dados[25] != false;
  Radar.MudaRaio((unsigned int)Dados[26], Escala[X]);
  DesenharAlinTerrap = Dados[27] != false;
  DesenharOffSets = Dados[28] != false;
  DesenharMapa = Dados[29] != false;
  DesenharEquipDrenagem = Dados[30] != false;
  DesenharSimbolTerapl = Dados[31] != false;
  DesenharCNTaludes = Dados[32] != false;
  DensidadeSimbologia = Dados[33];
  DesenharSecoesAvulsas = Dados[34] != false;
  RealcarCNsMestras = Dados[35] != false;

  Radar.MudaCorTela((COLORREF*)&Dados[36]);

//  if (((CMainFrame*)AfxGetMainWnd())->TipoSuperficie == SUPERF_DRONES)  DesenharPontos = DesenharArestas = false;

  AlterouAngulo = true;

  RedrawWindow();
}

void DesPontosGeomView::IconizaSecao()
{
  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());
  DesSecaoView* DSView((DesSecaoView*)MainFrame->PegaDesenhoSecoes());
  if (!DSView) return;
  DSView->GetParentFrame()->ShowWindow(SW_MINIMIZE);
  DSView->MostrarVolumes(false);
}

void DesPontosGeomView::IconizarRestaurarSecao(int Modo)
{
  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());
  DesSecaoView* CWDesSecao((DesSecaoView*)MainFrame->PegaDesenhoSecoes());
  if (!CWDesSecao) return;
  CWDesSecao->GetParentFrame()->ShowWindow(Modo);
  CWDesSecao->MostrarVolumes(Modo == SW_RESTORE);
}

void DesPontosGeomView::OnSecsemprevisIconizar()
{
  SecoesIconizadas = SecoesIconizadas == false;

  IconizarRestaurarSecao(SecoesIconizadas ? SW_MINIMIZE : SW_RESTORE);
}

void DesPontosGeomView::OnSizing(UINT fwSide, LPRECT pRect)
{
  CView::OnSizing(fwSide, pRect);

  RECT Coord;
  GetClientRect(&Coord);

  Resolucao[X] = Coord.right;
  Resolucao[Y] = Coord.bottom;

  GetWindowRect(CRectJanPai);    	//--- Salva As novas coordenadas para saber as posições do pop up

  DefineParametros();
  CalculaParametros();
  CalculaNovasDimensoes();
  OnAlterouAngulo(GetDC());
}

bool DesPontosGeomView::GravarSuperficie()
{
#ifdef DEMOVER
  ::MessageBox(NULL, "Função indisponível na cópia de demonstração", "VU&E", MB_OK);
  return true;
#endif

  int Resposta, PrimeiraResposta(!IDOK);

  if (!ZerarAzimute)  //--- Se estiver destruindo e a superficie estiver rotacionada, ZerarAzimute == true;
  {
    do
    {
      PrimeiraResposta = dialogar.DoModal(4, CString(" Geométrico - ") + CString(strTrechoAtual.c_str()));

      if (PrimeiraResposta != IDOK && Destruindo)
      {
        Resposta = dialogar.DoModal(6);
      }
    } while (Destruindo && PrimeiraResposta != IDOK && Resposta != IDOK);
  }
  else PrimeiraResposta = IDOK;

  if (PrimeiraResposta == IDOK)
  {
    if (AlterouTerreno)
    {
      Superficie.CriaListaBordos();   //--- A fronteira agora é uma restrição. Passa-a para a lista antiga por compatibilidade com os outros modulos
      Superficie.Serialize(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()), GRAVAR, false, false);

      for (ittymapCTrechos it(Trechos.MapaTrechos.begin()); it != Trechos.MapaTrechos.end(); it++)
      {
        it->second.SuperficieTerr.SerializarEquipDrenagem(GRAVAR,it->first);
      }

      GravaAcidentesTopograficos();
    }
    if (AlterouProjeto)
    {
      for (ittymapCTrechos it(Trechos.MapaTrechos.begin()); it != Trechos.MapaTrechos.end(); it++)
      {
        ((CMainFrame*)AfxGetMainWnd())->GeometricoAlterado = true;
        if (it->second.CurvasHorizontais.CalculaDadosComplementares() == CCurHorizontais::CALCULO_NOK) return false;
        it->second.CurvasHorizontais.Serialize(it->first.c_str(), GRAVAR);

        GravaCoordSecoes();

        it->second.SuperficieTerr.Serialize(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()), GRAVAR, false, false,true);
      }
    }
    if(AlterouBacias)
    {
      Superficie.SerializarBacias(GRAVAR,(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()));
    }

    if (TalveguePrincipal.size() && !Superficie.VerfBaciaImportada(TalveguePrincipal.begin()->PCentral))
    {
      SerializaTalveguePrincipal(GRAVAR);    //--- Guarda o talvegue principal para continuar a edução
    }
    
    SecoesAvulsas.Serialize(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()).GetBuffer(), GRAVAR);

    AlterouTerreno = AlterouProjeto = AlterouSecoesAvulsas = AlterouBacias = false;

    return true;
  }

  return false;
}

void DesPontosGeomView::OnIrparaponto()
{
  auto MF(((CMainFrame*)AfxGetMainWnd()));

   pDCoordenadas = new CDCoordenadas(this, MF->m_sNomeComp.GetBuffer(), &BufferPontosProcurados);

   pDCoordenadas->Create(IDD_ENTRACOORDXY, this);

   pDCoordenadas->ShowWindow(SW_SHOW);
}

BOOL DesPontosGeomView::PreCreateWindow(CREATESTRUCT& cs)
{
  cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, // use any window styles
    AfxGetApp()->LoadStandardCursor(IDC_HAND),
    (HBRUSH)(COLOR_WINDOW + 1));         // background brush

  return CView::PreCreateWindow(cs);
}

int DesPontosGeomView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CView::OnCreate(lpCreateStruct) == -1) return -1;

  // Remove positions of menu
  CMenu* pTopMenu = GetSystemMenu(FALSE);
  if (pTopMenu != NULL)
  {
    pTopMenu->RemoveMenu(SC_SIZE, MF_BYCOMMAND);      //Size
    pTopMenu->RemoveMenu(SC_MOVE, MF_BYCOMMAND);      //Move
    pTopMenu->RemoveMenu(SC_MAXIMIZE, MF_BYCOMMAND);  //Maximize
  }

  return 0;
}

void DesPontosGeomView::OnAlternartituloDesGeom()
{
  CChildFrame* CCFrm((CChildFrame*)GetParentFrame());
  unsigned long MostrarTitulo(CCFrm->GetStyle() & WS_CAPTION);

  if (MostrarTitulo) CCFrm->ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);
  else CCFrm->ModifyStyle(0, WS_CAPTION, SWP_FRAMECHANGED);

  CCFrm->ShowControlBar(&CCFrm->m_wndStatusBar, !MostrarTitulo, true);
}

void DesPontosGeomView::OnButFechar()
{
  ((CChildFrame*)GetParentFrame())->Fechar();
}

void DesPontosGeomView::CriaMapaSecoesGeomTemp(const CurHorizontal& Curva, MSecaoGeom& MSecoesGeomTemp)
{
  //--- quando o usuário esta alterando o geometrico as estacas dos pontos notaveis da nova curva não são mais 
  //--- encontradas no mapa de seções (So no reseccionamento elas serão novamente encontradas). Esta funcão cria 
  //--- um mapa temporário só para atender este requisito para a curva passada. assim a espiral dela poderá ser 
  //--- desenhada usando-se a mesma função (DesenhaEspiral). São incluídos 4 pontos intermediários e exponencialmente
  //--- distanciados nas espirais para que elas possam ser desenhadas.

  static double Fatores[4] = { 1.0 / 2.0,3.0 / 4.0,7.0 / 8.0,15.0 / 16.0 };   //--- Os pontos estão a : 1/2 3/4 7/8 E 15/16 ao longo da curva PG.

  //--- Insere os pontos notáveis da curva

  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstTs, Curva.TS));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstSc, Curva.SC));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstCs, Curva.CS));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstSt, Curva.ST));

  //--- Calcula 2 pontos intermediários na 1a transição e os insere no mapa

  double xEsp[4], yEsp[4], s[4], Corda[4], Deflexao[4];

  CCurHorizontais::CalcxyEsp(Curva, &xEsp[0], &yEsp[0], &s[0], Curva.Lc * Fatores[0]);
  Corda[0] = hypot(xEsp[0], yEsp[0]);
  Deflexao[0] = atan(xEsp[0] / yEsp[0]) * (Curva.Ac < 0.0 ? -1.0 : 1.0);

  CCurHorizontais::CalcxyEsp(Curva, &xEsp[1], &yEsp[1], &s[1], Curva.Lc * Fatores[1]);
  Corda[1] = hypot(xEsp[1], yEsp[1]);
  Deflexao[1] = atan(xEsp[1] / yEsp[1]) * (Curva.Ac < 0.0 ? -1.0 : 1.0);

  CCurHorizontais::CalcxyEsp(Curva, &xEsp[2], &yEsp[2], &s[2], Curva.Lc * Fatores[2]);
  Corda[2] = hypot(xEsp[2], yEsp[2]);
  Deflexao[2] = atan(xEsp[2] / yEsp[2]) * (Curva.Ac < 0.0 ? -1.0 : 1.0);

  CCurHorizontais::CalcxyEsp(Curva, &xEsp[3], &yEsp[3], &s[3], Curva.Lc * Fatores[3]);
  Corda[3] = hypot(xEsp[3], yEsp[3]);
  Deflexao[3] = atan(xEsp[3] / yEsp[3]) * (Curva.Ac < 0.0 ? -1.0 : 1.0);

  Ponto P1(sin(Curva.TS.fi + Deflexao[0]) * Corda[0] + Curva.TS.x, cos(Curva.TS.fi + Deflexao[0]) * Corda[0] + Curva.TS.y, INFINITO, INFINITO, s[0] + Curva.TS.fi),
        P2(sin(Curva.TS.fi + Deflexao[1]) * Corda[1] + Curva.TS.x, cos(Curva.TS.fi + Deflexao[1]) * Corda[1] + Curva.TS.y, INFINITO, INFINITO, s[1] + Curva.TS.fi),
        P3(sin(Curva.TS.fi + Deflexao[2]) * Corda[2] + Curva.TS.x, cos(Curva.TS.fi + Deflexao[2]) * Corda[2] + Curva.TS.y, INFINITO, INFINITO, s[2] + Curva.TS.fi),
        P4(sin(Curva.TS.fi + Deflexao[3]) * Corda[3] + Curva.TS.x, cos(Curva.TS.fi + Deflexao[3]) * Corda[3] + Curva.TS.y, INFINITO, INFINITO, s[3] + Curva.TS.fi);

  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstTs.EstVirtual + Curva.Lc * Fatores[0], P1));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstTs.EstVirtual + Curva.Lc * Fatores[1], P2));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstTs.EstVirtual + Curva.Lc * Fatores[2], P3));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstTs.EstVirtual + Curva.Lc * Fatores[3], P4));

  //--- Calcula 2 pontos na 2a transição e os insere no mapa 

  P1.x = sin(Curva.ST.fi - Deflexao[0] + Mat.M_PI) * Corda[0] + Curva.ST.x;
  P1.y = cos(Curva.ST.fi - Deflexao[0] + Mat.M_PI) * Corda[0] + Curva.ST.y;
  P1.fi = Curva.ST.fi - s[0];
  CCurHorizontais::AcertaAngulo(&P1.fi);

  P2.x = sin(Curva.ST.fi - Deflexao[1] + Mat.M_PI) * Corda[1] + Curva.ST.x;
  P2.y = cos(Curva.ST.fi - Deflexao[1] + Mat.M_PI) * Corda[1] + Curva.ST.y;
  P2.fi = Curva.ST.fi - s[1];
  CCurHorizontais::AcertaAngulo(&P2.fi);

  P3.x = sin(Curva.ST.fi - Deflexao[2] + Mat.M_PI) * Corda[2] + Curva.ST.x;
  P3.y = cos(Curva.ST.fi - Deflexao[2] + Mat.M_PI) * Corda[2] + Curva.ST.y;
  P3.fi = Curva.ST.fi - s[2];
  CCurHorizontais::AcertaAngulo(&P3.fi);

  P4.x = sin(Curva.ST.fi - Deflexao[3] + Mat.M_PI) * Corda[3] + Curva.ST.x;
  P4.y = cos(Curva.ST.fi - Deflexao[3] + Mat.M_PI) * Corda[3] + Curva.ST.y;
  P4.fi = Curva.ST.fi - s[3];
  CCurHorizontais::AcertaAngulo(&P4.fi);

  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstSt.EstVirtual - Curva.Lc * Fatores[0], P1));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstSt.EstVirtual - Curva.Lc * Fatores[1], P2));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstSt.EstVirtual - Curva.Lc * Fatores[2], P3));
  MSecoesGeomTemp.insert(MSecaoGeom::value_type(Curva.EstSt.EstVirtual - Curva.Lc * Fatores[3], P4));
}

void DesPontosGeomView::OnGravar()
{
#ifdef DEMOVER
  ::MessageBox(NULL, "Função indisponível na cópia de demonstração", "VU&E", MB_OK);
#else
  ((CMainFrame*)AfxGetMainWnd())->GeometricoAlterado = true;
  GravarSuperficie();
#endif
}

void DesPontosGeomView::OnCalcgeometrico()
{
  // Reavaliar o uso desta função, quando o geometrico é alterado sempre será preciso retocar o greide, Do modo que esta o greide 
  // não é retocado, ficam as seçoes antigas no desenho das seções o que não é interessante...

  //---->> Desligada: Quando é feita alguma alteração no projeto geométrico o greide muda drasticamente pois as estacas são reposicionadas
  //                  Deste modo o greide teria que ser todo retocado a não ser que se usasem igualdades que já existem no projeto mas não 
  //                  estão completamente implementadas, então fica para uma segunda versão

  //--- Religada: Muitas vezes fazemos pequenas alterações no geométrico como por exemplo apra ajustar a pista a um corte existente
  //              ou a uma ponte existente, nesca caso tem que recalcular o geométrico e por conseguinte reseccionar todo o trecho

//  return;

  bool Resecionar(true);

  if (dialogar.DoModal(12, "", &Resecionar) == IDOK)
  {
    CMainFrame* pMainFrame(((CMainFrame*)((CMainFrame*)AfxGetMainWnd())));

    if (AlterouProjeto) Trechos.PegaTrechoAtual()->second.CurvasHorizontais.Serialize(CString(pMainFrame->PegaProjetoAtual()), GRAVAR);
    if (pMainFrame->PegaDesenhoSecoes()) ((DesSecaoView*)((CMainFrame*)pMainFrame)->PegaDesenhoSecoes())->GravarSecoes(false);

    //--- Calcula o Projeto Geométrico
    //--- O dialogo chamado é o DCalprojeto mas ele é modal então não o mostra e usa o 
    //--- CDCalcGeoReseccDireto para mostrar o progress e as mensagens

    //--- NÃO PRECISA MAIS, no reseccionamente recalcula o geometrico se ele foi alterado.

    /*
    CDCalcGeoSeccDireto DCalcGeoSeccDireto(this);
    DCalcGeoSeccDireto.Create(this);                      //--- Direto, não modal
    CDCalcGeometrico DCalcGeometrico(&DCalcGeoSeccDireto,CString(pMainFrame->PegaProjetoAtual()));
    DCalcGeometrico.Create(this,&DCalcGeoSeccDireto);      //--- Direto, não modal
    DCalcGeoSeccDireto.ShowWindow(true);

    DCalcGeometrico.CalcularDireto();

    WaitForSingleObject(CEvent(),500);      //--- Aguarda 0,5 segundos o cálculo é muito rápido
    */
    //--- Agora recessiona o  terreno com os dados de projeto recem calculados 
    //--- Com a mesma alteração nos diálogos

    CString NomeProjeto(CString(pMainFrame->PegaProjetoAtual()));

    CDSecionar DSecionar(NomeProjeto, this);

    if (Resecionar)
    {
      //--- não esta funcionando não modal, verificar

      pMainFrame->GeometricoAlterado = true;

      //    DCalcGeoSeccDireto.ShowWindow(false);

        //  DSecionar.Create(0,this,&DCalcGeoSeccDireto);
        //  DSecionar.Create(0,this);
        //  DSecionar.ShowWindow(true);
      //    DSecionar.CalcularDireto();

      DSecionar.DoModal();

      WaitForSingleObject(CEvent(), 500);      //--- Aguarda 0,5 segundos para sair

      AlterouProjeto = AlterouTerreno = false;
    }

    //	dialogar.RadSeccionar.EnableWindow(false);
    //--- A nota de serviço foi anulada com o novo recálculo do geométrico. Então
    //--- renomeia o arquivo da nota de serviço (.not)  para .not.anterior. Em caso de erro não faz nada...

    /*
    CString NomeNovo(NomeProjeto + ".not.anterior"),NomeVelho(CString(NomeProjeto + ".not"));
    CFile CFNota;

    if(CFNota.Ope1n(NomeVelho,CFile::modeRead|CFile::shareExclusive))      //--- Se já existir um arquivo .not
    {
    CFNota.Close();

    try { CFile::Remove(NomeVelho);} catch(...) {}                //--- Remove o backup anterior .not.anterior (se existir)
    try { CFile::Rename(NomeVelho,NomeNovo);} catch(...) {}       //--- Renomeia o .not para .not.anterior
    }
    */

    LeCoordSecoes();    //--- rele as novas coordenadas das seções

    //--- Se o desenho das secoes estiver aberto, lê novamente as seções para atualizar os volumes e os desenho das secoes

    DesSecaoView* pDesSecoes((DesSecaoView*)pMainFrame->PegaDesenhoSecoes());
    if (pMainFrame->PegaDesenhoSecoes()) pDesSecoes->ReleSecoes();

    RedrawWindow();
  }
}

void DesPontosGeomView::OnSecaoAvulsa()
{
  CDDiaNomePerfAvulso DiaNomePerfAvulso(strNomeSecAvulsa, this);

  if (DiaNomePerfAvulso.DoModal() == IDOK)
  {
    strNomeSecAvulsa = DiaNomePerfAvulso.strNome.GetBuffer();

    PegandoPontoSecAvulsa = true;
    CChildFrame* CCFrm = (CChildFrame*)GetParentFrame();
    CCFrm->m_wndToolBarPonGeo.CheckDlgButton(ID_BUTSECAVULSA, PegandoPontoSecAvulsa);
  }
}

void DesPontosGeomView::OnUpdateButSecAvulsa(CCmdUI* pCmdUI)
{
  #if defined (PACOTE_INTERMEDIARIO) || defined (PACOTE_AVANCADO)
    pCmdUI->Enable(true);
  #else 
    pCmdUI->Enable(false);
  #endif
}

void DesPontosGeomView::OnTipoCN()
{
  DesCNTopograficas = !DesCNTopograficas;
  CChildFrame* CCFrm = (CChildFrame*)GetParentFrame();
  CCFrm->m_wndToolBarPonGeo.CheckDlgButton(ID_BUTCNTOPOGRAF, DesCNTopograficas);

  RedrawWindow();
}

void DesPontosGeomView::OnBordos()
{
  // if(!NaoPermitirAlteracoes) DefBordos = true;

  CChildFrame* CCFrm = (CChildFrame*)GetParentFrame();
  CCFrm->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTBORDOS, DefBordos == 0 ? TBSTATE_ENABLED : TBSTATE_PRESSED);
  CCFrm->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTACITOPOG, DefBordos == 0 ? TBSTATE_PRESSED : TBSTATE_ENABLED);

  //else ((CChildFrame*) GetParentFrame())->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_BUTBORDOS,false);

  DefBordos = true;
}

void DesPontosGeomView::OnSincronizarSecoes()
{
  if ((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->PegaDesenhoSecoes())
  {
    SincronizarSecoes = SincronizarSecoes == false;
  }
}

void DesPontosGeomView::OnUpdateButSincro(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(((CMainFrame*)AfxGetMainWnd())->PegaDesenhoSecoes() != NULL);
}

#include "CDiaConvCND.h"

void DesPontosGeomView::OnNaoPermitirAlteracoes()
{
 // CDiaConvCND DiaConvCND;

  //DiaConvCND.DoModal();

  NaoPermitirAlteracoes = !NaoPermitirAlteracoes;
}

void DesPontosGeomView::GravaCoordSecoes()
{
  for (ittymapCTrechos it = Trechos.MapaTrechos.begin(); it != Trechos.MapaTrechos.end(); it++)
  {
    CString NomeProjeto(it->first.c_str());

    CFile ArqCalcSec;
    CFileException e;

    if (ArqCalcSec.Open(NomeProjeto + CString(".pes"), CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &e) == 0)
      monolog.mensagem(13, NomeProjeto + CString(".pes"));
    else
    {
      CArchive CArqCalcSec(&ArqCalcSec, CArchive::store);

      //--- Grava coordenadas de todas as seções. (os pontos de 3 em 3 já estão na ordem de estaca)
      //--- O mapa de seções está sincronizado com a lista dos pontos extremos das seções (que merda......acertar isso!!!)

      Ponto PEsq, PDir, PEixo;
      CEstaca Estaca;
      size_t Tam(it->second.LPSecoes.size() / 3);       //--- Houve erro de sincronização durante os testes. Não estavam do mesmo tamanho LPSecoes estava zerada. VERIFICAR

      CArqCalcSec << (int)(it->second.LPSecoes.size() / 3);

      for (register itMSecaoGeom itSecaoGeom = it->second.MapaSecoesGeom.begin(); Tam && itSecaoGeom != it->second.MapaSecoesGeom.end(); Tam--, itSecaoGeom++)
      {
        Estaca = (*itSecaoGeom).first;

        Estaca >> CArqCalcSec;

        PEsq = *(it->second.LPSecoes.begin());
        PEsq >> CArqCalcSec;
        it->second.LPSecoes.pop_front();
        PEsq = *(it->second.LPSecoes.begin());
        PEsq >> CArqCalcSec;
        it->second.LPSecoes.pop_front();
        PEsq = *(it->second.LPSecoes.begin());
        PEsq >> CArqCalcSec;
        it->second.LPSecoes.pop_front();
      }

      CArqCalcSec.Close();
    }
  }
}

void DesPontosGeomView::LeCoordSecoes()
{
  for (ittymapCTrechos it = Trechos.MapaTrechos.begin(); it != Trechos.MapaTrechos.end(); it++)
  {
    CString NomeProjeto(it->first.c_str());

    CFile ArqCalcSec;
    CFileException e;
    int QSecoes(0);
    Ponto PontoACalc;

    if (ArqCalcSec.Open(NomeProjeto + CString(".pes"), CFile::modeRead | CFile::shareDenyNone, &e) == 0)
    {
      if (!((CMainFrame*)AfxGetMainWnd())->PegaSilencioso()) monolog.mensagem(13, NomeProjeto + CString(".pes"));
    }
    else
    {
      CArchive CArqCalcSec(&ArqCalcSec, CArchive::load);
      Ponto PEsq, PDir, PEixo;
      CEstaca EstacaAtual;

      //--- Se estiver relendo remove os dados anteriores

      it->second.LPSecoes.clear();

      //--- Busca coordenadas de todas as seções. colocando-os numa lista (os pontos de 3 em 3 já estão na ordem de estaca)
      try
      {
        for (CArqCalcSec >> QSecoes; QSecoes; QSecoes--)
        {
          EstacaAtual << CArqCalcSec;
          PDir << CArqCalcSec;
          PEixo << CArqCalcSec;
          PEsq << CArqCalcSec;

          it->second.LPSecoes.push_back(PDir);
          it->second.LPSecoes.push_back(PEixo);
          it->second.LPSecoes.push_back(PEsq);

          //--- Mapa de estacas ordenado por coordenadas, usado no onmousemove para mostrar as coordenadas do ponto
          //--- Os pontos extremos poderiam estar aqui tb...corrigir

          it->second.MapaSecoesGeom.insert(MSecaoGeom::value_type(EstacaAtual, PEixo));
        }
      }
      catch (...)
      {
        monolog.mensagem(13, NomeProjeto + CString(".pes") + CString(".\nReseccione todo o projeto novamente."));
      }
    }

    // ArqCalcSec.Close();

    it->second.MapaSecoesPonto.clear();

    for (register itMSecaoGeom itmapAtual = it->second.MapaSecoesGeom.begin(); itmapAtual != it->second.MapaSecoesGeom.end(); itmapAtual++)
      it->second.MapaSecoesPonto.insert(MSecaoGeomPonto::value_type(&(*itmapAtual).second, &(*itmapAtual).first));
  }
}

void DesPontosGeomView::RotacionaSecoes(double Seno, double Coseno, double AngRotacao)
{
  for (ittymapCTrechos ittr = Trechos.MapaTrechos.begin(); ittr != Trechos.MapaTrechos.end(); ittr++)
  {
    //--- rotaciona o mapa com os 3 pontos, ordenado por estaca e o mapa ordenado pelo ponto do eixo como tb o mapa de seções ordenado por coordenadas

    for (register itlstPontos it = ittr->second.LPSecoes.begin(); it != ittr->second.LPSecoes.end(); it++)
    {
      (*it).RotacionaPlano(Seno, Coseno, 0.0);
    }

    ittr->second.MapaSecoesPonto.clear();

    for (register itMSecaoGeom itmapAtual = ittr->second.MapaSecoesGeom.begin(); itmapAtual != ittr->second.MapaSecoesGeom.end(); itmapAtual++)
    {
      (*itmapAtual).second.RotacionaPlano(Seno, Coseno, 0.0);
      ittr->second.MapaSecoesPonto.insert(MSecaoGeomPonto::value_type(&(*itmapAtual).second, &(*itmapAtual).first));
    }
  }
}

void DesPontosGeomView::RotacionaBordoseRestricoes(double Sin, double Cos, double Angulo)
{
  CSuperficie* pSuperficieAtual(&Superficie);
  bool Corrigir(fabs(Superficie.PegaAnguloRotacao() < Matem::UmGrau_Rad));  ///--- Problema na CGAL

  ittymapCTrechos it(Trechos.MapaTrechos.begin());

  while (pSuperficieAtual)
  {
    for (int i = BORDOS; i <= RESTRICOES; i++)
    {
      LLDeltaSup& ListaRestAtual(i == BORDOS ? pSuperficieAtual->PegaListaBordos() : pSuperficieAtual->PegaListaRestricoes());

      for (register ItLLDeltaSup LDRestricaoAtual = ListaRestAtual.begin(); LDRestricaoAtual != ListaRestAtual.end(); LDRestricaoAtual++)
      {
        for (register ItLDeltaSup ItLRestricao = (*LDRestricaoAtual).begin(); ItLRestricao != (*LDRestricaoAtual).end(); ItLRestricao++)
        {
          (*ItLRestricao).PCentral.RotacionaPlano(Sin, Cos, Angulo);

          if (Corrigir)   //--- Erro na CGAL   - Só ocorre nas restrições
          {
            Mat.Arredonda((*ItLRestricao).PCentral, 7);
          }
        }
      }
    }

    if (it != Trechos.MapaTrechos.end())
    {
      pSuperficieAtual = &it->second.SuperficieTerr;
      DeltaSupSobMouseAnterior = Superficie.pSuperficieAtual->end();
      it++;
    }
    else pSuperficieAtual = nullptr;
  }
}

void DesPontosGeomView::RotacionaBordos(double Sin, double Cos, double Angulo, LLDeltaSup& ListaBordos)
{
  for (register ItLLDeltaSup LDRestricaoAtual = ListaBordos.begin(); LDRestricaoAtual != ListaBordos.end(); LDRestricaoAtual++)
  {
    for (register ItLDeltaSup ItLRestricao = (*LDRestricaoAtual).begin(); ItLRestricao != (*LDRestricaoAtual).end(); ItLRestricao++)
    {
      (*ItLRestricao).PCentral.RotacionaPlano(Sin, Cos, Angulo);
    }
  }
}

void DesPontosGeomView::RotacionaSecoesAvulsas(double Sin, double Cos, double Angulo)
{
  CList<CSecaoAvulsa, CSecaoAvulsa&>& ListSecAvulsasCalc(SecoesAvulsas.PegaLSecoesAvulsas());

  for (POSITION Atual = ListSecAvulsasCalc.GetHeadPosition(); Atual != NULL; ListSecAvulsasCalc.GetNext(Atual))
  {
    ListSecAvulsasCalc.GetAt(Atual).RotacionaPlano(Sin,Cos,Angulo);
  }
}

void inline DesPontosGeomView::TransfPixelsXY(CPoint& Ponto, double& Coorx, double& Coory)
{
  //--- Mapeia as cordenadas do ponto que estão em pixels para metros.

  Coorx = ((double)Ponto.x) / Escala[X] + Deltas[X];
  Coory = AlturaVideo - ((double)Ponto.y) / Escala[Y] + Deltas[Y];

  if (Superficie.EstaRotacionada())
  {
    double Angulo = -Superficie.PegaAnguloRotacao();

    RotacionaPontoPlano(sin(Angulo), cos(Angulo), Angulo, &Coorx, &Coory);
  }
}

void  DesPontosGeomView::DesenhaMalha(CDC* pDC)
{
  if (LPontosCoordX.size() == 0) return;

  itlistPontosMalha itPrim(LPontosCoordX.begin()), itSec(itPrim);
  itSec++;

  //--- Verifica se a malha está muito fina se estiver não desenha

  if (LPontosCoordX.size() > 1 && abs((*itPrim).Pontos.first.y - (*itSec).Pontos.first.y) < 3 &&
    abs((*itPrim).Pontos.first.x - (*itSec).Pontos.first.x) < 3) return;    //--- Tem que ter pelo menos um pixel entre as coordenadas

  pDC->SetBkMode(TRANSPARENT);
  register itlistPontosMalha it(LPontosCoordX.begin());

   for (; it != LPontosCoordX.end(); it++)
  {
    pDC->MoveTo((*it).Pontos.first.x, (*it).Pontos.first.y);
    pDC->LineTo((*it).Pontos.second.x, (*it).Pontos.second.y);
  }

  pDC->SelectObject(&CurrierNewHorizontal);
  pDC->SetTextAlign(TA_BOTTOM | TA_LEFT);
  pDC->SetTextColor(Cores[CORTEXTO]);
  pDC->SetBkColor(Cores[CORFUNDO]);

  if (LPontosCoordX.size() == 0) return;
  int Contador(0);

  for (it = LPontosCoordX.begin(); CotarMalha && it != LPontosCoordX.end(); it++)
    if (Contador++ % EquiCotas == 0)
      pDC->TextOut((*it).PontoCota.x, (*it).PontoCota.y, (*it).Cota.c_str());

  //----------------- coordenadas y

  if (LPontosCoordY.size() == 0) return;

  for (it = LPontosCoordY.begin(); it != LPontosCoordY.end(); it++)
  {
    pDC->MoveTo((*it).Pontos.first.x, (*it).Pontos.first.y);
    pDC->LineTo((*it).Pontos.second.x, (*it).Pontos.second.y);
  }

  pDC->SelectObject(&CurrierNewVertical);
  Contador = 0;

  for (it = LPontosCoordY.begin(); CotarMalha && it != LPontosCoordY.end(); it++)
    if (Contador++ % EquiCotas == 0)
      pDC->TextOut((*it).PontoCota.x, (*it).PontoCota.y, (*it).Cota.c_str());
}

void  DesPontosGeomView::OnAlterouAngulo(CDC* pDC)
{
  void (DesPontosGeomView:: * pFunc[4])(CDC*) = { &DesPontosGeomView::OnAlterouAnguloQ1,
    &DesPontosGeomView::OnAlterouAnguloQ2,
    &DesPontosGeomView::OnAlterouAnguloQ3,
    &DesPontosGeomView::OnAlterouAnguloQ4 };

  (*this.*pFunc[Quadrante])(pDC);
}

void DesPontosGeomView::OnArrastouDesenho()
{
  void (DesPontosGeomView:: * pFunc[4])() = { &DesPontosGeomView::OnArrastouDesenhoQ1,
    &DesPontosGeomView::OnArrastouDesenhoQ2,
    &DesPontosGeomView::OnArrastouDesenhoQ3,
    &DesPontosGeomView::OnArrastouDesenhoQ4 };

  (*this.*pFunc[Quadrante])();
}

void  DesPontosGeomView::OnAlterouAnguloQ1(CDC* pDC)
{
  //--- Como a tela é redesenhada toda hora, o cálculo dos pontos de início e fim da malha bem como
  //--- dos pontos onde as cotas serão escritas é pesado e deve ser feito o mínimo de vezes possível.
  //--- Estes dados só se alteram em arrastos  do desenho, zomms ou quando o ângulo de rotação é alterado.

  AnguloRotacao = Superficie.PegaAnguloRotacao();

  Seno = sin(AnguloRotacao);
  Coseno = cos(AnguloRotacao);
  Tangente = tan(AnguloRotacao);
  SenoM = sin(-AnguloRotacao);
  CosenoM = cos(-AnguloRotacao);

  Quadrante = floor(AnguloRotacao / Mat.M_PI_2);
  if (Quadrante == 4) Quadrante = 3;              //--- Ajuste de arredondamento

  CurrierNewHorizontal.DeleteObject();
  CurrierNewVertical.DeleteObject();

  CFont* LF = pDC->GetCurrentFont();
  LOGFONT Log;

  LF->GetLogFont(&Log);
  Log.lfEscapement = AnguloRotacao * 1800.0 / 3.14;
  Log.lfHeight = 18;
  strcpy(Log.lfFaceName, "Arial");
  CurrierNewHorizontal.CreateFontIndirect(&Log);

  Log.lfEscapement = (AnguloRotacao * 1800 / 3.14) + 2700.0;
  strcpy(Log.lfFaceName, "Arial");
  Log.lfHeight = 18;
  CurrierNewVertical.CreateFontIndirect(&Log);

  TamLetra = pDC->GetTextExtent('A');

  AlturaVideoPixels = AlturaVideo * Escala[X];
  LarguraVideoPixels = LarguraVideo * Escala[Y];
  AlturaRebatidaYPixels = AlturaVideoPixels * Coseno;
  LarguraRebatidaYPixels = LarguraVideoPixels * Seno;
  LarguraRebatidaXPixels = LarguraVideoPixels * Coseno;
  AlturaRebatidaXPixels = AlturaVideoPixels * Seno;
  NovaAlturaPixels = AlturaRebatidaYPixels + LarguraRebatidaYPixels,
  NovaLarguraPixels = LarguraVideoPixels * Coseno + AlturaVideoPixels * Seno;

  OnArrastouDesenho();
}

void DesPontosGeomView::OnAlterouAnguloQ2(CDC* pDC)
{
  AnguloRotacao = Superficie.PegaAnguloRotacao();

  Seno = sin(AnguloRotacao);
  Coseno = cos(AnguloRotacao);
  Cotangente = 1.0 / tan(AnguloRotacao);
  Tangente = tan(AnguloRotacao);
  SenoM = sin(-AnguloRotacao);
  CosenoM = cos(-AnguloRotacao);

  Quadrante = floor(AnguloRotacao / Mat.M_PI_2);
  if (Quadrante == 4) Quadrante = 3;

  CurrierNewHorizontal.DeleteObject();
  CurrierNewVertical.DeleteObject();

  CFont* LF = pDC->GetCurrentFont();
  LOGFONT Log;
  LF->GetLogFont(&Log);
  Log.lfEscapement = AnguloRotacao * 1800.0 / 3.14 - 900.0;
  Log.lfHeight = 18;
  strcpy(Log.lfFaceName, "Arial");
  CurrierNewHorizontal.CreateFontIndirect(&Log);

  Log.lfEscapement = (AnguloRotacao * 1800 / 3.14) + 1800.0;
  Log.lfHeight = 18;
  strcpy(Log.lfFaceName, "Arial");
  CurrierNewVertical.CreateFontIndirect(&Log);

  TamLetra = pDC->GetTextExtent('A');

  AlturaVideoPixels = AlturaVideo * Escala[X];
  LarguraVideoPixels = LarguraVideo * Escala[Y];
  AlturaRebatidaYPixels = AlturaVideoPixels * Coseno;
  LarguraRebatidaYPixels = LarguraVideoPixels * Seno;
  LarguraRebatidaXPixels = LarguraVideoPixels * Coseno;
  NovaAlturaPixels = LarguraVideoPixels * -Coseno + AlturaVideoPixels * Seno;

  OnArrastouDesenho();
}

void DesPontosGeomView::OnAlterouAnguloQ3(CDC* pDC)
{
  AnguloRotacao = Superficie.PegaAnguloRotacao();
  Quadrante = floor(AnguloRotacao / Mat.M_PI_2);
  if (Quadrante == 4) Quadrante = 3;

  Seno = sin(AnguloRotacao);
  Coseno = cos(AnguloRotacao);
  Tangente = tan(AnguloRotacao);
  SenoM = sin(-AnguloRotacao);
  CosenoM = cos(-AnguloRotacao);

  CurrierNewHorizontal.DeleteObject();
  CurrierNewVertical.DeleteObject();

  CFont* LF = pDC->GetCurrentFont();

  LOGFONT Log;

  LF->GetLogFont(&Log);
  Log.lfEscapement = AnguloRotacao * 1800.0 / 3.14 - 1800.0;
  Log.lfHeight = 18;
  strcpy(Log.lfFaceName, "Arial");
  CurrierNewHorizontal.CreateFontIndirect(&Log);

  Log.lfEscapement = (AnguloRotacao * 1800 / 3.14) + 900.0;
  Log.lfHeight = 18;
  strcpy(Log.lfFaceName, "Arial");
  CurrierNewVertical.CreateFontIndirect(&Log);

  TamLetra = pDC->GetTextExtent('A');

  AlturaVideoPixels = AlturaVideo * Escala[X];
  LarguraVideoPixels = LarguraVideo * Escala[Y];
  AlturaRebatidaYPixels = AlturaVideoPixels * Coseno;
  LarguraRebatidaYPixels = LarguraVideoPixels * Seno;
  LarguraRebatidaXPixels = LarguraVideoPixels * Coseno;
  AlturaRebatidaXPixels = AlturaVideoPixels * Seno;
  NovaAlturaPixels = LarguraVideoPixels * -Coseno + AlturaVideoPixels * -Seno;
  NovaLarguraPixels = LarguraVideoPixels * -Seno + AlturaVideoPixels * -Coseno;

  OnArrastouDesenho();
}

void DesPontosGeomView::OnAlterouAnguloQ4(CDC* pDC)
{
  AnguloRotacao = Superficie.PegaAnguloRotacao();
  Quadrante = floor(AnguloRotacao / Mat.M_PI_2);
  if (Quadrante == 4) Quadrante = 3;

  CurrierNewHorizontal.DeleteObject();
  CurrierNewVertical.DeleteObject();

  CFont* LF = pDC->GetCurrentFont();
  LOGFONT Log;

  LF->GetLogFont(&Log);
  Log.lfEscapement = AnguloRotacao * 1800.0 / 3.14;
  Log.lfHeight = 18;
  strcpy(Log.lfFaceName, "Arial");
  CurrierNewHorizontal.CreateFontIndirect(&Log);

  Log.lfEscapement = (AnguloRotacao * 1800 / 3.14) + 900.0;

  Log.lfHeight = 18;
  strcpy(Log.lfFaceName, "Arial");
  CurrierNewVertical.CreateFontIndirect(&Log);

  TamLetra = pDC->GetTextExtent('A');

  Seno = sin(AnguloRotacao);
  Coseno = cos(AnguloRotacao);
  SenoM = sin(-AnguloRotacao);
  CosenoM = cos(-AnguloRotacao);
  Tangente = tan(AnguloRotacao);

  AlturaVideoPixels = AlturaVideo * Escala[X];
  LarguraVideoPixels = LarguraVideo * Escala[Y];
  AlturaRebatidaYPixels = AlturaVideoPixels * Coseno;
  AlturaRebatidaXPixel = AlturaVideoPixels * Seno;
  LarguraRebatidaYPixels = LarguraVideoPixels * Seno;
  NovaAlturaPixels = LarguraVideoPixels * Coseno + AlturaVideoPixels * -Seno;
  NovaLarguraPixels = LarguraVideoPixels * -Seno + AlturaVideoPixels * Coseno;

  OnArrastouDesenho();
}

void DesPontosGeomView::OnArrastouDesenhoQ1()
{
  //--- Como a tela é redesenhada toda hora, o cálculo dos pontos de início e fim da malha bem como
  //--- dos pontos onde as cotas serão escritas é pesado e deve ser feito o mínimo de vezes possível.
  //--- Estes dados só se alteram em arrastos  do desenho, zomms   ou quando o ângulo de rotação é alterado.

  //--- Ajusta as coordenadas de inicio e de fim do vídeo

  DeltasReais = Ponto(Deltas[X], Deltas[Y], AnguloRotacao);
  DeltasReais.RotacionaPlano(SenoM, CosenoM, -AnguloRotacao);

  LimiteYMetros.y = DeltasReais.y - LarguraVideo * Seno;
  LimiteYMetros.x = DeltasReais.x;

  //--- Ajusta as primeiras coordenadas e deltas

  ArestaMalha = EquiMalha * Escala[X];
  if (ArestaMalha < 2) ArestaMalha = 2;                //--- Resolução mínima da malha
  PrimeiraCotaY = floor((LimiteYMetros.y + EquiMalha) / EquiMalha) * EquiMalha;
  PrimeiroDeltaY = (PrimeiraCotaY - LimiteYMetros.y) * Escala[Y];
  PrimeiraCotaX = floor((LimiteYMetros.x + EquiMalha) / EquiMalha) * EquiMalha;
  PrimeiroDeltaX = (PrimeiraCotaX - LimiteYMetros.x) * Escala[X];
  OrigemX = -AlturaVideoPixels * Seno;

  //--- Cria a lista com os pontos iniciais e finais das retas das coordenadas

  Ponto PMalha3, PMalha4;

  LPontosCoordX.clear();
  itlistPontosMalha it;
  double DeltaY = PrimeiroDeltaY;

  for (; DeltaY < NovaAlturaPixels; DeltaY += ArestaMalha)
  {
    PMalha3.x = OrigemX;
    PMalha4.x = NovaLarguraPixels;
    PMalha3.y = PMalha4.y = NovaAlturaPixels - DeltaY;

    PMalha3.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), 0.0);
    PMalha4.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), 0.0);

    it = LPontosCoordX.insert(LPontosCoordX.end(), stMalha());

    (*it).Pontos.first = PMalha3;
    (*it).Pontos.second = PMalha4;
  }

  //--- Calcula os pontos onde as coordenadas serão escritas

  CString Coord;
  double CoordAtual(PrimeiraCotaY);

  //--- Deslocamentos no eixo x.

  for (it = LPontosCoordX.begin(), DeltaY = PrimeiroDeltaY; DeltaY < LarguraRebatidaYPixels; DeltaY += ArestaMalha, CoordAtual += EquiMalha, it++)
  {
    PMalha4.x = (LarguraRebatidaYPixels - Y) / tan(AnguloRotacao);
    PMalha4.y = LarguraRebatidaYPixels - DeltaY - 3;
    PMalha4.RotacionaPlano(SenoM, CosenoM, 0.0);

    Mat.FormataDoubleStr(CoordAtual, Coord, ".,", 0);

    PMalha4.x -= DeltaY / Tangente * Coseno;
    PMalha4.y += DeltaY * Coseno + AlturaVideoPixels;

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }

  //--- Deslocamentos no eixo y.

  for (DeltaY = DeltaY - LarguraRebatidaYPixels; DeltaY < AlturaRebatidaYPixels; DeltaY += ArestaMalha, CoordAtual += EquiMalha, it++)
  {
    PMalha4.x = DeltaY * -tan(AnguloRotacao);
    PMalha4.y = DeltaY;
    PMalha4.RotacionaPlano(SenoM, CosenoM, 0.0);

    double RecuoLetra(TamLetra.cy * Seno + 4.0);

    PMalha4.x += RecuoLetra;
    PMalha4.y += RecuoLetra * Tangente;
    PMalha4.y = AlturaVideoPixels - PMalha4.y;

    Mat.FormataDoubleStr(CoordAtual, Coord, ".,", 0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }

  //--- Coordenadas x
  //--- Calcula os pontos iniciais e finais das coordenadas x.

  LPontosCoordY.clear();
  double DeltaX = OrigemX + PrimeiroDeltaX;

  for (; DeltaX < NovaLarguraPixels; DeltaX += ArestaMalha)
  {
    PMalha3.x = DeltaX;
    PMalha3.y = NovaAlturaPixels;
    PMalha4.x = PMalha3.x;
    PMalha4.y = 0.0;

    PMalha3.RotacionaPlano(SenoM, CosenoM, 0.0);
    PMalha4.RotacionaPlano(SenoM, CosenoM, 0.0);

    it = LPontosCoordY.insert(LPontosCoordY.end(), stMalha());

    (*it).Pontos.first = PMalha3;
    (*it).Pontos.second = PMalha4;
  }

  //--- Calcula os pontos das cotas x.

  for (CoordAtual = PrimeiraCotaX, it = LPontosCoordY.begin(), DeltaX = OrigemX + PrimeiroDeltaX; DeltaX < 0.0; DeltaX += ArestaMalha, CoordAtual += EquiMalha, it++)
  {
    PMalha4.x = DeltaX;
    PMalha4.y = 0.0;

    PMalha4.RotacionaPlano(SenoM, CosenoM, 0.0);

    PMalha4.y += (-DeltaX + 4.0) / Tangente * Coseno;
    PMalha4.x = 4.0;

    Mat.FormataDoubleStr(CoordAtual, Coord, ".,", 0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }

  for (; DeltaX < LarguraRebatidaXPixels; DeltaX += ArestaMalha, CoordAtual += EquiMalha, it++)
  {
    PMalha4.x = DeltaX;
    PMalha4.y = 0.0;
    PMalha4.RotacionaPlano(SenoM, CosenoM, 0.0);

    PMalha4.x += (DeltaX * Seno) * Tangente + 4 * Tangente + TamLetra.cy * Seno * Tangente;
    PMalha4.y = 4.0 + TamLetra.cy * Seno;

    Mat.FormataDoubleStr(CoordAtual, Coord, ".,", 0);

    if(it == LPontosCoordY.end()) break;  //--- Nunca pode acontecer mas esta acontecendo ao importar o mapa - #VERIFICAR 16-01-2022

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }
}

void DesPontosGeomView::OnArrastouDesenhoQ2()
{
  NovaLarguraPixels = LarguraVideoPixels * Seno + AlturaVideoPixels * -Coseno;

  DeltasReais = Ponto(Deltas[X], Deltas[Y], AnguloRotacao);
  DeltasReais.RotacionaPlano(SenoM, CosenoM, -AnguloRotacao);

  LimiteYMetros.y = DeltasReais.y - (LarguraVideo * Seno) + (AlturaVideo * Coseno);
  LimiteYMetros.x = DeltasReais.x + (LarguraVideo * Coseno);

  ArestaMalha = EquiMalha * Escala[X];
  if (ArestaMalha < 2) ArestaMalha = 2;                //--- Resolução mínima da malha
  PrimeiraCotaY = floor((LimiteYMetros.y + EquiMalha) / EquiMalha) * EquiMalha;
  PrimeiroDeltaY = (PrimeiraCotaY - LimiteYMetros.y) * Escala[Y];
  PrimeiraCotaX = floor((LimiteYMetros.x + EquiMalha) / EquiMalha) * EquiMalha;
  PrimeiroDeltaX = (PrimeiraCotaX - LimiteYMetros.x) * Escala[X];
  OrigemX = LarguraVideoPixels * Coseno - AlturaVideoPixels * Seno;

  Ponto PMalha3, PMalha4;

  LPontosCoordY.clear();
  itlistPontosMalha it;

  for (double DeltaY = PrimeiroDeltaY; DeltaY < NovaLarguraPixels; DeltaY += ArestaMalha)
  {
    PMalha3.x = OrigemX;
    PMalha4.x = 0.0;
    PMalha3.y = PMalha4.y = LarguraRebatidaYPixels - DeltaY;

    PMalha3.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), 0.0);
    PMalha4.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), 0.0);

    it = LPontosCoordY.insert(LPontosCoordY.end(), stMalha());

    (*it).Pontos.first = PMalha3;
    (*it).Pontos.second = PMalha4;
  }

  CString Coord;
  double CoordYAtual(PrimeiraCotaY);

  it = LPontosCoordY.begin();

  for (double DeltaY = PrimeiroDeltaY; DeltaY < NovaLarguraPixels; DeltaY += ArestaMalha, CoordYAtual += EquiMalha, it++)
  {
    PMalha4.x = 0.0;
    PMalha4.y = LarguraRebatidaYPixels - DeltaY;

    PMalha4.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), 0.0);

    double DeltaH = (LarguraRebatidaYPixels - DeltaY + 5.0) * -Coseno,
      RecuoLetra = TamLetra.cy * -Cotangente;

    PMalha4.y += DeltaH + RecuoLetra * Seno;
    PMalha4.x += DeltaH * -Cotangente + RecuoLetra * -Coseno;

    Mat.FormataDoubleStr(CoordYAtual, Coord, ".,", 0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }

  //----------------- coordenadas x

  LPontosCoordX.clear();
  double DeltaX(0.0);

  for (DeltaX = PrimeiroDeltaX; DeltaX < NovaAlturaPixels; DeltaX += ArestaMalha)
  {
    PMalha3.x = OrigemX + DeltaX;
    PMalha3.y = LarguraVideoPixels;
    PMalha4.x = PMalha3.x;
    PMalha4.y = AlturaRebatidaYPixels;

    PMalha3.RotacionaPlano(SenoM, CosenoM, 0.0);
    PMalha4.RotacionaPlano(SenoM, CosenoM, 0.0);

    it = LPontosCoordX.insert(LPontosCoordX.end(), stMalha());

    (*it).Pontos.first = PMalha3;
    (*it).Pontos.second = PMalha4;
  }

  double CoordXAtual(PrimeiraCotaX);
  it = LPontosCoordX.begin();

  for (DeltaX = PrimeiroDeltaX; DeltaX < fabs(LarguraRebatidaXPixels); DeltaX += ArestaMalha, CoordXAtual += EquiMalha, it++)
  {
    PMalha4.x = OrigemX + DeltaX;
    PMalha4.y = AlturaRebatidaYPixels;

    PMalha4.RotacionaPlano(SenoM, CosenoM, 0.0);

    double DeltaXTemp = (LarguraRebatidaXPixels + DeltaX) * Tangente * Seno;

    PMalha4.x += DeltaXTemp;
    PMalha4.y = AlturaVideoPixels - 5;

    Mat.FormataDoubleStr(CoordXAtual, Coord, ".,", 0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }

  double  RecuoLetra = TamLetra.cy * -Coseno;

  for (; DeltaX < NovaAlturaPixels; DeltaX += ArestaMalha, CoordXAtual += EquiMalha, it++)
  {
    PMalha4.x = OrigemX + DeltaX;
    PMalha4.y = AlturaRebatidaYPixels;

    PMalha4.RotacionaPlano(SenoM, CosenoM, 0.0);

    double DeltaXTemp = (DeltaX + LarguraRebatidaXPixels) * -Coseno;

    PMalha4.x += DeltaXTemp + RecuoLetra;
    PMalha4.y += (DeltaXTemp + RecuoLetra) * Cotangente;

    Mat.FormataDoubleStr(CoordXAtual, Coord, ".,", 0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }
}

void DesPontosGeomView::OnArrastouDesenhoQ3()
{
  DeltasReais = Ponto(Deltas[X], Deltas[Y], AnguloRotacao);
  DeltasReais.RotacionaPlano(SenoM, CosenoM, -AnguloRotacao);

  LimiteYMetros.y = DeltasReais.y + AlturaVideo * Coseno;
  LimiteYMetros.x = DeltasReais.x + (AlturaVideo * Seno) + (LarguraVideo * Coseno);

  ArestaMalha = EquiMalha * Escala[X];
  if (ArestaMalha < 2) ArestaMalha = 2;                //--- Resolução mínima da malha
  PrimeiraCotaY = floor((LimiteYMetros.y + EquiMalha) / EquiMalha) * EquiMalha;
  PrimeiroDeltaY = (PrimeiraCotaY - LimiteYMetros.y) * Escala[Y];
  PrimeiraCotaX = floor((LimiteYMetros.x + EquiMalha) / EquiMalha) * EquiMalha;
  PrimeiroDeltaX = (PrimeiraCotaX - LimiteYMetros.x) * Escala[X];
  OrigemX = LarguraVideoPixels * Coseno;

  Ponto PMalha3, PMalha4;

  LPontosCoordX.clear();
  itlistPontosMalha it;
  double DeltaY(0.0);

  for (DeltaY = PrimeiroDeltaY; DeltaY < NovaLarguraPixels; DeltaY += ArestaMalha)
  {
    PMalha3.x = OrigemX;
    PMalha4.x = -AlturaRebatidaXPixels;
    PMalha3.y = PMalha4.y = -DeltaY;

    PMalha3.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), 0.0);
    PMalha4.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), 0.0);

    it = LPontosCoordX.insert(LPontosCoordX.end(), stMalha());

    (*it).Pontos.first = PMalha3;
    (*it).Pontos.second = PMalha4;
  }

  CString Coord;
  double CoordYAtual(PrimeiraCotaY);
  it = LPontosCoordX.begin();

  for (DeltaY = PrimeiroDeltaY; DeltaY < -AlturaRebatidaYPixels; DeltaY += ArestaMalha, CoordYAtual += EquiMalha, it++)
  {
    PMalha4.x = -AlturaRebatidaXPixels;
    PMalha4.y = -DeltaY;

    PMalha4.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), 0.0);

    double AfastX((AlturaRebatidaYPixels + DeltaY) * Seno);

    PMalha4.x += AfastX;
    PMalha4.y -= AfastX * Tangente;

    PMalha4.x -= TamLetra.cy * Seno - 3;
    PMalha4.y -= (TamLetra.cy * -Seno + 3) * Tangente;

    Mat.FormataDoubleStr(CoordYAtual, Coord, ".,", 0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }

  for (; DeltaY < NovaLarguraPixels; DeltaY += ArestaMalha, CoordYAtual += EquiMalha, it++)
  {
    PMalha4.x = -AlturaRebatidaXPixels;
    PMalha4.y = -DeltaY;

    PMalha4.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), 0.0);

    double AfastX((((DeltaY + AlturaRebatidaYPixels) / Tangente) + 3) * -Coseno);

    PMalha4.x += AfastX;
    PMalha4.y -= AfastX * Tangente;

    Mat.FormataDoubleStr(CoordYAtual, Coord, ".,", 0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }

  //----------------- coordenadas x

  LPontosCoordY.clear();
  double DeltaX(0.0);

  for (DeltaX = PrimeiroDeltaX; DeltaX < NovaAlturaPixels - AlturaVideoPixels * Seno; DeltaX += ArestaMalha)
  {
    PMalha3.x = OrigemX + DeltaX;
    PMalha3.y = 0.0;
    PMalha4.x = PMalha3.x;
    PMalha4.y = AlturaRebatidaYPixels + LarguraRebatidaYPixels;

    PMalha3.RotacionaPlano(SenoM, CosenoM, 0.0);
    PMalha4.RotacionaPlano(SenoM, CosenoM, 0.0);

    it = LPontosCoordY.insert(LPontosCoordY.end(), stMalha());

    (*it).Pontos.first = PMalha3;
    (*it).Pontos.second = PMalha4;
  }

  it = LPontosCoordY.begin();
  double CoordXAtual(PrimeiraCotaX);

  for (DeltaX = PrimeiroDeltaX; DeltaX < NovaAlturaPixels - AlturaVideoPixels * Seno; DeltaX += ArestaMalha, CoordXAtual += EquiMalha, it++)
  {
    PMalha4.x = OrigemX + DeltaX;
    PMalha4.y = 0.0;

    PMalha4.RotacionaPlano(SenoM, CosenoM, 0.0);

    double AfastY((LarguraRebatidaXPixels + DeltaX) * Seno);

    PMalha4.x += AfastY * Tangente;
    PMalha4.y += AfastY;

    double DeltaYLetra((TamLetra.cy + 4.0) * -Seno);

    PMalha4.x += DeltaYLetra * Tangente + 4.0;
    PMalha4.y += DeltaYLetra;

    Mat.FormataDoubleStr(CoordXAtual, Coord, ".,", 0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }
}

void DesPontosGeomView::OnArrastouDesenhoQ4()
{
  Ponto DeltasReais(Deltas[X], Deltas[Y], AnguloRotacao);
  DeltasReais.RotacionaPlano(SenoM, CosenoM, -AnguloRotacao);

  LimiteYMetros.y = DeltasReais.y;
  LimiteYMetros.x = DeltasReais.x + AlturaVideo * Seno;

  ArestaMalha = EquiMalha * Escala[X];
  if (ArestaMalha < 2) ArestaMalha = 2;                //--- Resolução mínima da malha
  PrimeiraCotaY = floor((LimiteYMetros.y + EquiMalha) / EquiMalha) * EquiMalha;
  PrimeiroDeltaY = (PrimeiraCotaY - LimiteYMetros.y) * Escala[Y];
  PrimeiraCotaX = floor((LimiteYMetros.x + EquiMalha) / EquiMalha) * EquiMalha;
  PrimeiroDeltaX = (PrimeiraCotaX - LimiteYMetros.x) * Escala[X];
  OrigemX = 0;

  Ponto PMalha3, PMalha4;

  LPontosCoordX.clear();
  itlistPontosMalha it;
  double DeltaY = PrimeiroDeltaY - AlturaRebatidaYPixels;

  for (; DeltaY < NovaLarguraPixels; DeltaY += ArestaMalha)
  {
    PMalha3.x = OrigemX;
    PMalha4.x = NovaAlturaPixels;
    PMalha3.y = PMalha4.y = -DeltaY;

    PMalha3.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), 0.0);
    PMalha4.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), 0.0);

    it = LPontosCoordX.insert(LPontosCoordX.end(), stMalha());

    (*it).Pontos.first = PMalha3;
    (*it).Pontos.second = PMalha4;
  }

  CString Coord;
  double CoordAtual(PrimeiraCotaY);

  for (it = LPontosCoordX.begin(), DeltaY = PrimeiroDeltaY - AlturaRebatidaYPixels; DeltaY < 0.0; DeltaY += ArestaMalha, CoordAtual += EquiMalha, it++)
  {
    PMalha4.x = OrigemX;
    PMalha4.y = -DeltaY;

    PMalha4.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), 0.0);

    PMalha4.y += ((DeltaY)*Tangente + 4.0) * -Seno;
    PMalha4.x += (DeltaY)*Seno + 4.0;

    Mat.FormataDoubleStr(CoordAtual, Coord, ".,", 0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }

  for (; DeltaY < NovaLarguraPixels; DeltaY += ArestaMalha, CoordAtual += EquiMalha, it++)
  {
    PMalha4.x = OrigemX;
    PMalha4.y = -DeltaY;

    PMalha4.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), 0.0);

    double DeslocY((DeltaY + TamLetra.cy) * Coseno + 4.0);

    PMalha4.y += DeslocY;
    PMalha4.x += DeslocY / -Tangente;

    Mat.FormataDoubleStr(CoordAtual, Coord, ".,", 0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }

  //----------------- coordenadas x

  LPontosCoordY.clear();
  double DeltaX = PrimeiroDeltaX;

  for (; DeltaX < NovaAlturaPixels; DeltaX += ArestaMalha)
  {
    PMalha3.x = DeltaX;
    PMalha3.y = AlturaRebatidaYPixels;
    PMalha4.x = PMalha3.x;
    PMalha4.y = LarguraRebatidaYPixels;

    PMalha3.RotacionaPlano(SenoM, CosenoM, 0.0);
    PMalha4.RotacionaPlano(SenoM, CosenoM, 0.0);

    it = LPontosCoordY.insert(LPontosCoordY.end(), stMalha());

    (*it).Pontos.first = PMalha3;
    (*it).Pontos.second = PMalha4;
  }

  CoordAtual = PrimeiraCotaX;

  if (LPontosCoordY.size() > 0) 
  for (it = LPontosCoordY.begin(), DeltaX = PrimeiroDeltaX ; it != LPontosCoordY.end() && DeltaX < -AlturaRebatidaXPixel ; DeltaX += ArestaMalha, CoordAtual += EquiMalha, it++)
  {
    PMalha4.x = DeltaX;
    PMalha4.y = AlturaRebatidaYPixels;

    PMalha4.RotacionaPlano(SenoM, CosenoM, 0.0);

    double DeslocMargemX((-AlturaRebatidaXPixel - DeltaX + TamLetra.cy) * Coseno + 4.0);

    PMalha4.x += DeslocMargemX;
    PMalha4.y -= DeslocMargemX / -Tangente;

    Mat.FormataDoubleStr(CoordAtual, Coord, ".,", 0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }

  for (; DeltaX < NovaAlturaPixels; DeltaX += ArestaMalha, CoordAtual += EquiMalha, it++)
  {
    PMalha4.x = DeltaX;
    PMalha4.y = AlturaRebatidaYPixels;

    PMalha4.RotacionaPlano(SenoM, CosenoM, 0.0);

    double DeslocMargemX((DeltaX - -AlturaRebatidaXPixel + 4.0) * -Tangente * -Seno);

    PMalha4.x += DeslocMargemX;
    PMalha4.y = AlturaVideoPixels - 4;

    Mat.FormataDoubleStr(CoordAtual, Coord, ".,", 0);

    (*it).PontoCota = PMalha4;
    (*it).Cota = Coord;
  }
}

void DesPontosGeomView::CentralizarTela(const Ponto& PCentral, bool DesenharCruz)
{
  Deltas[X] = PCentral.x;
  Deltas[Y] = PCentral.y;

  DeltasCN[X] = DeltasReais.x = Deltas[X] -= LarguraVideo / 2.0;
  DeltasCN[Y] = DeltasReais.y = Deltas[Y] -= AlturaVideo / 2.0;
  DeltasCN[Y] += AlturaVideo;

  CalculaParametros();
  if (!Destruindo) RedrawWindow();   //--- Evita a tela de piscar quando estiver destruindo a janela

  //--- Desenha a cruz vermelha no ponto

  if (DesenharCruz) DesenharCruzPonto(PCentral);
}

void DesPontosGeomView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
  // TODO: Add your specialized code here and/or call the base class

  if (pHint && !pSender && pHint == this && lHint == GRAVAR) OnGravar();
}

void DesPontosGeomView::DesenhaNorte(CDC* pDC)
{
  POINT Origem = { ((long)Resolucao[X]) - 125, 80 },
        Pontos[4] = {{Origem.x,Origem.y},
                     {Origem.x + 30,Origem.y - 30},
                     {Origem.x + 60,Origem.y} };

  register int i(0);

  for (; i < 3; i++)
  {
    double x(Pontos[i].x), y(Pontos[i].y);

    TransladaPontoPlano(&x, &y, Origem.x + 30.0, Origem.y);
    RotacionaPontoPlano(SenoM, CosenoM, -AnguloRotacao, &x, &y);
    TransladaPontoPlano(&x, &y, -Origem.x - 30.0, -Origem.y);

    Pontos[i].x = x;
    Pontos[i].y = y;
  }

  CPen PenaSec;
  PenaSec.CreatePen(PS_SOLID, 2, Cores[CORTEXTO]);
  pDC->SelectObject(&PenaSec);

  CBrush brushBack(Cores[CORTEXTO]);
  CBrush* pOldBrush(pDC->SelectObject(&brushBack));

  pDC->Polygon(Pontos, 3);

  pDC->SelectObject(pOldBrush);

  pDC->Arc(Origem.x + 0, Origem.y - 30, Origem.x + 60, Origem.y + 30, 0, 0, 0, 0);

  Pontos[0].x = Origem.x - 10;
  Pontos[0].y = Origem.y;
  Pontos[1].x = Origem.x + 60 + 10;
  Pontos[1].y = Origem.y;
  Pontos[2].x = Origem.x + 30;
  Pontos[2].y = Origem.y - 30 - 10 - TamLetra.cy;
  Pontos[3].x = Origem.x + 30;
  Pontos[3].y = Origem.y + 30 + 10;

  for (i = 0; i < 4; i++)
  {
    double x(Pontos[i].x), y(Pontos[i].y);

    TransladaPontoPlano(&x, &y, Origem.x + 30.0, Origem.y);
    RotacionaPontoPlano(SenoM, CosenoM, -AnguloRotacao, &x, &y);
    TransladaPontoPlano(&x, &y, -Origem.x - 30.0, -Origem.y);

    Pontos[i].x = x;
    Pontos[i].y = y;
  }

  pDC->MoveTo(Pontos[0].x, Pontos[0].y);
  pDC->LineTo(Pontos[1].x, Pontos[1].y);
  pDC->MoveTo(Pontos[2].x, Pontos[2].y);
  pDC->LineTo(Pontos[3].x, Pontos[3].y);

  CPen PenaSec2;
  PenaSec2.CreatePen(PS_SOLID, 1, Cores[CORMALHA]);
  pDC->SelectObject(&PenaSec2);
  if (Quadrante == 0 || Quadrante == 2) pDC->SelectObject(&CurrierNewHorizontal);
  else pDC->SelectObject(&CurrierNewVertical);
  pDC->SetTextColor(Cores[CORTEXTO]);
  double Declinacao(Superficie.Declinacao());
  std::stringstream  stmTexto;

  stmTexto << std::right << std::fixed << std::setfill(' ') << std::setprecision(0); // Passar para GMS #VERIFICAR

  stmTexto << "  N " << (Superficie.AngRotacaoTopogGraus()-Declinacao) << 'º' << std::ends;
  pDC->SetTextAlign(TA_BOTTOM | TA_CENTER);
  pDC->TextOut(Pontos[2].x + TamLetra.cx - (TamLetra.cx / 2), Pontos[2].y, stmTexto.str().c_str());

  if(Declinacao != 0.0)
  {
    stmTexto.str("");

    stmTexto  << " Dec " << Declinacao << 'º' << std::ends;
    pDC->SetTextAlign(TA_BOTTOM | TA_CENTER);
    pDC->TextOut(Pontos[3].x + TamLetra.cx - (TamLetra.cx / 2), Pontos[3].y+30, stmTexto.str().c_str());
  }
}

void DesPontosGeomView::CotaCN(double fi, Ponto Local, bool Inicio, double dCota, CDC* pDC, const CPen& PenaCotaMestra)
{
  Ponto Ppixels;
  double AngAjuste(0.0);

  if (fi > 3.14) fi -= 3.14;

  if (fi < 1.57) AngAjuste = 1.57 - fi;
  else if (fi < 3.14) AngAjuste = 7.85 - fi;

  CriaFontesCN(true, AngAjuste);

  pDC->SelectObject(&CourierNewCN);
  pDC->SelectObject(PenaCotaMestra);

  TamLetra = pDC->GetTextExtent('A');

  int Quadrante(AngAjuste / Matem::M_PI_2);

  Ppixels.x = ((Local.x - Deltas[X]) * Escala[X]);
  Ppixels.y = (AlturaVideo - (Local.y - Deltas[Y])) * Escala[Y];

  if (Quadrante == 0 || Quadrante == 2)
  {
    if (Quadrante == 0) pDC->SetTextAlign(TA_BOTTOM | TA_LEFT);
    else pDC->SetTextAlign(TA_BOTTOM | TA_RIGHT);
  }
  else
  {
    if (Quadrante == 1) pDC->SetTextAlign(TA_BOTTOM | TA_RIGHT);
    else pDC->SetTextAlign(TA_BOTTOM | TA_LEFT);
  }

  std::strstream strstmCota;

  strstmCota << std::right << std::fixed << std::setfill(' ') << std::setprecision(fmod(dCota, 1.0) > 0.0 ? 2 : 0);
  strstmCota << "  " << dCota << "  " << std::ends;

  pDC->SetBkColor(Cores[CORFUNDO]);

  if (Inicio) pDC->TextOut(Ppixels.x + (TamLetra.cy) / 2.0 * sin(AngAjuste), Ppixels.y + (TamLetra.cy) / 2.0 * cos(AngAjuste), strstmCota.str());
  else pDC->TextOut(Ppixels.x - ((TamLetra.cy) / 2.0 * cos(AngAjuste)), Ppixels.y - ((TamLetra.cy) / 2.0 * sin(AngAjuste)), strstmCota.str());
}

BOOL DesPontosGeomView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  //--- Durante o ajuste do mapa não pode ampliar porque ao final do ajuste se
  //--- o mapa for ampliado novamente ele se desloca
  //--- Verificar projeto EstWinTentativaTransladoTeclaAmpliacao

  if(!Transladando && !IniciouTranslado)  
  {
    ((CChildFrame*)GetParentFrame())->DeslizanteGeom.MouseWheelOn(nFlags, zDelta, pt);
    Radar.RecalculaRaioPixels(Escala[0]);

    TransfPixelsXY(this->PonMousePixelsAtual, MouseXYGeo->m_DOCoorX, MouseXYGeo->m_DOCoorY);

    if (RadarPontosInteresse)
    {
      SeparaPontosInteresse(MouseXYGeo->m_DOCoorX, MouseXYGeo->m_DOCoorY);
    }

    Invalidate(true);

  }
  return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void DesPontosGeomView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
  // TODO: Add your specialized code here and/or call the base class

  pDC->SetBkColor(Cores[CORFUNDO]);
  CView::OnPrepareDC(pDC, pInfo);
}

#include "CDMensagem.h"

void DesPontosGeomView::OnButfronteira()
{  
  if (NaoPermitirAlteracoes)
  {
    monolog.mensagem(-1,"Não estão sendo permitidas alterações neste projeto.(Veja o botão <Alterar>");
    return;
  }

  if(AlterouTerreno || AlterouProjeto)
  {
    OnGravar();
  }
  AnguloRotacaoAnterior = Superficie.PegaAnguloRotacao();

  bool HouveErros(false);
  
  CDDEfineFronteira DDefineFronteira(this);

  int TipoSuperf(((CMainFrame*)AfxGetMainWnd())->TipoSuperficie);    //--- 0 -> Convencional 1 -> Drones

  if (TipoSuperf == SUPERF_DRONES || TipoSuperf == SUPERF_GERENMAPAS || TipoSuperf == SUPERF_SIG || DDefineFronteira.DoModal() == IDOK)
  {
    //--- Continua o log

    CString NomeProjeto(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()));
    std::ofstream ofArqLog(NomeProjeto + "_DefFronteira.log", std::fstream::app);
    std::stringstream MsgLog;

    MsgLog << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

    ((CMainFrame*)AfxGetMainWnd())->FronteiraEditada = false;

    if (TipoSuperf == SUPERF_TRADICIONAL)
    {
      MsgLog.str("");
      MsgLog << "\r\n" << "    Verificando interferências na fronteira gerada..." << "\r\n";
      GravaLog(MsgLog.str(),ofArqLog);

      DPopUpPonto.MostraDescricao(300, 200, MsgLog.str());
      DPopUpPonto.RedrawWindow();

      Sleep(1000);
    }

    Superficie.PegaListaBordos().clear();
    Superficie.Serialize(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()), LER, TRUE, false, false, true,false,TipoSuperf != 1);
    LimpaListaRestricao(CAciTopografico::FRONTEIRA);

    ((CMainFrame*)AfxGetMainWnd())->FronteiraEditada = false;

    //--- A fronteira foi calculada agora e não esta rotacionada

    if (Superficie.EstaRotacionada())
    {
     // if(TipoSuperf != SUPERF_DRONES || !Transladando)
      {
        RotacionaBordos(Seno, Coseno, AnguloRotacao, Superficie.PegaListaBordos());
      }
    }

    for (ItLLDeltaSup it = Superficie.PegaListaBordos().begin(); it != Superficie.PegaListaBordos().end(); ++it)
    {
      LLDeltaSup& ListaRestricoes(Superficie.PegaListaRestricoes());

      ListaRestricoes.push_back(LDeltaSup(*it));
      ItLLDeltaSup itLRestricaoAtual(ListaRestricoes.end());
      itLRestricaoAtual--;

      Superficie.InsereListaNoMapa(itLRestricaoAtual, CAciTopografico::FRONTEIRA);
    }

    Superficie.CalculaArea();

    Superficie.PegaListaBordos().clear();
    SSetPontos PontosProblema;

    if (TipoSuperf == SUPERF_TRADICIONAL)
    {
      //--- Não precisa verifica isso aqui, se um ponto estiver fora da superficie o acidente topog esta fora da superf
      //--- Pontos fora da superficie já foram verificados
      //--- Precisa sim pode haver arestas ligando externamente 2 pontos da superficie

      if (Superficie.VerfAciTopogForaSuperficie(PontosProblema, &ItLLDeltaSupMarcada) == false)
      {
        MsgLog.str("");
        MsgLog << "Ficaram acidentes topograficos fora da fronteira, verifique o acidente topografico que está marcado bem como o arquivo de log.";

        for(auto& It: PontosProblema)
        {
           MsgLog << std::endl << It.x << " " << It.y << " " << It.z;
        }

        GravaLog(MsgLog.str(),ofArqLog);
        AfxMessageBox(MsgLog.str().c_str());
        HouveErros = true;
      }

      PontosProblema.clear();

      if (Superficie.VerfAciTopogCruzFronteira(ItLLDeltaSupMarcada, PontosProblema) == true)
      {
        MsgLog.str("");

        MsgLog << "Ficaram cruzamentos entre acidentes topográficos que geram arestas forçadas, verifique o ponto marcado bem como o arquivo de log.";
        GravaLog(MsgLog.str(), ofArqLog);

        for (auto& It : PontosProblema)
        {
          MsgLog << std::endl << It.x << " " << It.y << " " << It.z;
        }

        AfxMessageBox(MsgLog.str().c_str());

        if (PontosProblema.size() > 0) PontoIntersecao = *PontosProblema.rbegin();
        CircularPontoIntersecao = true;

        HouveErros = true;
      }
    }

    AlterouProjeto = AlterouTerreno = true;

    AnguloRotacaoAnterior = 0.0;

    if (AnguloRotacaoAnterior != 0.0)
    {
      OnButrotacionar();
      AnguloRotacaoAnterior = 0.0;
    }

    RedrawWindow();

    if (((CMainFrame*)AfxGetMainWnd())->TipoSuperficie == SUPERF_TRADICIONAL && !HouveErros)
    {
      MsgLog.str("");
      MsgLog << "\r\n" << "   OK, não foram encontradas interferências na fronteira gerada." << "\r\n";

      DPopUpPonto.MostraDescricao(300, 200,MsgLog.str());
      DPopUpPonto.ShowWindow(SW_SHOW);
      DPopUpPonto.RedrawWindow();

      Sleep(2000);
    }
  }
}

void DesPontosGeomView::GravaLog(const std::string& strMsg, std::ostream& Stream, bool ForcarHora)
{
  CTime Agora(CTime::GetCurrentTime());
  static CTime GravAnterior(Agora);
  std::strstream strstreamTemp;

  strstreamTemp << std::right << std::fixed << std::setfill(' ') << std::setprecision(2);

  if (ForcarHora || CTimeSpan((Agora - GravAnterior)).GetTotalSeconds() > 30L)
  {
    strstreamTemp << "====> " << Agora.GetDay() << '/' << Agora.GetMonth() << '/' << Agora.GetYear() << " " << Agora.GetHour() << ':' << Agora.GetMinute() << ':' << Agora.GetSecond() << ' ' << std::ends;
    Stream << strstreamTemp.str();
    GravAnterior = Agora;
  }

  Stream << strMsg.c_str() << std::endl;

  Stream.flush();
}

void DesPontosGeomView::OnDesenhar()
{
  DesenhandoObjetos = !DesenhandoObjetos;

  if(DesenhandoObjetos) EsbocandoObjetos = false;

  CChildFrame* CF((CChildFrame*)GetParentFrame());

  CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_ESBOCAR, EsbocandoObjetos);
  CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_DESENHAR, DesenhandoObjetos);
  CF->ShowControlBar(&CF->m_wndToolBarAciTopograficos, (DesenhandoObjetos || EsbocandoObjetos) ? SW_SHOWNORMAL : SW_HIDE, false);
}

void DesPontosGeomView::OnEsbocar()
{
  EsbocandoObjetos = EsbocandoObjetos == false;

  ItListaRestAtual = Superficie.PegaListaRestricoes().end();

  if (EsbocandoObjetos)
  {
    DesenhandoObjetos = CriandoRestricao = false;
  }

  CChildFrame* CF((CChildFrame*)GetParentFrame());

  CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_ESBOCAR, EsbocandoObjetos);
  CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_DESENHAR, DesenhandoObjetos);
  CF->m_wndToolBarAciTopograficos.GetToolBarCtrl().SetState(ID_DES_FRONTEIRA, EsbocandoObjetos ? TBSTATE_INDETERMINATE : TBSTATE_ENABLED);
  CF->ShowControlBar(&CF->m_wndToolBarAciTopograficos, (EsbocandoObjetos || DesenharObjetos) ? SW_SHOWNORMAL : SW_HIDE, false);
}

void DesPontosGeomView::OnDrenagem()
{
  if(MostrarTerrap)
  {
    ObjetosDrenagem = !ObjetosDrenagem;

    ItListaRestAtual = Superficie.PegaListaRestricoes().end();

    if (ObjetosDrenagem)
    {
      EsbocandoObjetos = DesenhandoObjetos = CriandoRestricao = false;
      ObjetoAtual = CAciTopografico::NENHUM_ACITOPOG;
    }

    CChildFrame* CF((CChildFrame*)GetParentFrame());

    CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_DRENAGEM, ObjetosDrenagem);
    CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_DESENHAR, DesenhandoObjetos);
    CF->ShowControlBar(&CF->m_wndToolBarEquiDrenagem, ObjetosDrenagem ? SW_SHOWNORMAL : SW_HIDE, false);
  }
}

bool DesPontosGeomView::GravaAcidentesTopograficos()
{
  CString NomeProjeto(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()));

  CFile ArqAciTopog;
  CFileException e;

  if (ArqAciTopog.Open(NomeProjeto + CString(".aci"), CFile::modeCreate | CFile::modeWrite, &e) == 0)
  {
    monolog.mensagem(13, CString(NomeProjeto + CString(".aci ") + " (Gravando os dados)"));
    return false;
  }

  CArchive ar(&ArqAciTopog, CArchive::store);

  AcidentesTopograficos >> ar;

  return true;
};

bool DesPontosGeomView::LeAcidentesTopograficos()
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

  AcidentesTopograficos << ar;

  return true;
};

bool DesPontosGeomView::LePontosCotadosSuperf()
{
  CString NomeProjeto(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()));

  CFile ArqPontosCotadosSuperf;
  CFileException e;

  if (ArqPontosCotadosSuperf.Open(NomeProjeto + CString(".pcs"), CFile::modeRead, &e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound)
    {
      monolog.mensagem(13, CString(NomeProjeto + CString(".pcs") + " (lendo os dados)"));
      return false;
    }
    else return true;
  }

  CArchive ar(&ArqPontosCotadosSuperf, CArchive::load);

  SetPontoCotadosSuperf.clear();

  size_t QPontos(0);
  Ponto PAtual;

  for(ar >> QPontos ; QPontos ; QPontos--)
  {
    PAtual << ar;

    SetPontoCotadosSuperf.emplace(PAtual);
  }
    
  return true;
};

void DesPontosGeomView::OnDesMuro() { ObjetoAtual = CAciTopografico::MURO; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnNa() { ObjetoAtual = CAciTopografico::N_A; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnNaPeriodico() { ObjetoAtual = CAciTopografico::N_A_PER; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesMeiofio() { ObjetoAtual = CAciTopografico::MEIO_FIO; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesNa() { ObjetoAtual = CAciTopografico::N_A; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesRocha() { ObjetoAtual = CAciTopografico::ROCHA; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesBueiro() { ObjetoAtual = CAciTopografico::BUEIRO; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesRedeeletricaAt() { ObjetoAtual = CAciTopografico::REDE_ELETRICA_AT; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesRedeeletricaBt() { ObjetoAtual = CAciTopografico::REDE_ELETRICA_BT; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesBordo() { ObjetoAtual = CAciTopografico::BORDO; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesCerca() { ObjetoAtual = CAciTopografico::CERCA; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesPonte() { ObjetoAtual = CAciTopografico::PONTE; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesPoste() { ObjetoAtual = CAciTopografico::POSTE; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesFronteira() { ObjetoAtual = CAciTopografico::FRONTEIRA; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDeseixo() { ObjetoAtual = CAciTopografico::EIXO; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesCrista() { ObjetoAtual = CAciTopografico::CRISTA; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesPe() { ObjetoAtual = CAciTopografico::PE; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesCantoCasa() { ObjetoAtual = CAciTopografico::CANTO_CASA; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesCantoCerca() { ObjetoAtual = CAciTopografico::CANTO_CERCA; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesCantoMuro() { ObjetoAtual = CAciTopografico::CANTO_MURO; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesCasa() { ObjetoAtual = CAciTopografico::CASA; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesTampaBueiro() { ObjetoAtual = CAciTopografico::TAMPA_BUEIRO; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesLinhaFerrea() { ObjetoAtual = CAciTopografico::LINHA_FERREA;  OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesAlagado() { ObjetoAtual = CAciTopografico::ALAGADO; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesAlinhamento() { ObjetoAtual = CAciTopografico::ALINHAMENTO; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesArvore() { ObjetoAtual = CAciTopografico::ARVORE; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesFloresta() { ObjetoAtual = CAciTopografico::FLORESTA; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesMata() { ObjetoAtual = CAciTopografico::MATA; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesPlantacao() { ObjetoAtual = CAciTopografico::PLANTACAO; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesTalvegue() { ObjetoAtual = CAciTopografico::TALVEGUE; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesEspigao() { ObjetoAtual = CAciTopografico::ESPIGAO; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesRavina() { ObjetoAtual = CAciTopografico::RAVINA; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesEquiBueiro() { ObjetoAtual = CAciTopografico::EQUI_BUEIRO; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesEquiSarjetaOff() { ObjetoAtual = CAciTopografico::EQUI_VALETA_TERRAPL; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesEquiBueiroRetangular() { ObjetoAtual = CAciTopografico::EQUI_BUEIRO_RETANG; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesEquiSarjetaTN() { ObjetoAtual = CAciTopografico::EQUI_VALETA_TN; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesDescidaDagua() { ObjetoAtual = CAciTopografico::EQUI_DESCIDA_DAGUA; OnMouseMove(0, PonMousePixelsAtual);} 
void DesPontosGeomView::OnDesCaixaColetora() { ObjetoAtual = CAciTopografico::EQUI_CAIXA_COLETORA; OnMouseMove(0, PonMousePixelsAtual);}
void DesPontosGeomView::OnDesBaciaAmortecimento() { ObjetoAtual = CAciTopografico::EQUI_BACIA_DISSIPACAO; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesEquiSarjeta() { ObjetoAtual = CAciTopografico::EQUI_SARJETA; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesEquiTubulacao() { ObjetoAtual = CAciTopografico::EQUI_TUBULACAO; OnMouseMove(0, PonMousePixelsAtual); } 
void DesPontosGeomView::OnDesEquiDrenoProfundo() { ObjetoAtual = CAciTopografico::EQUI_DRENO_PROFUNDO; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesEquiCanaleta() { ObjetoAtual = CAciTopografico::EQUI_CANALETA_TN; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesEquiCanaletaOff() { ObjetoAtual = CAciTopografico::EQUI_CANALETA_TERRAPL; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesMovimentarEquiDrenagem() { ObjetoAtual = CAciTopografico::EQUI_EDITAR_DRENAGEM; OnMouseMove(0, PonMousePixelsAtual);};
void DesPontosGeomView::OnDesPVEletrica() { ObjetoAtual = CAciTopografico::PVELETRICA; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesPVEsgoto() { ObjetoAtual = CAciTopografico::PVESGOTO; OnMouseMove(0, PonMousePixelsAtual);}
void DesPontosGeomView::OnDesPVGas() { ObjetoAtual = CAciTopografico::PVGAS; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesRedeGas() { ObjetoAtual = CAciTopografico::REDE_GAS; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesRedeEletrica() { ObjetoAtual = CAciTopografico::REDE_ELETRICA; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesRedeEsgoto() { ObjetoAtual = CAciTopografico::REDE_ESGOTO; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesSoleira() { ObjetoAtual = CAciTopografico::SOLEIRA; OnMouseMove(0, PonMousePixelsAtual); }
void DesPontosGeomView::OnDesFiltrar() { FiltrarAciTopog = FiltrarAciTopog == false; RedrawWindow(); }

void DesPontosGeomView::CriaPenaRestriçao(CPen* PenaObjeto, unsigned int Objeto)
{
  PenaObjeto->DeleteObject();

  LOGBRUSH lb;
  lb.lbStyle = BS_SOLID;
  lb.lbColor = RGB(0, 255, 0);
  CPen pen;

  int Style(PS_GEOMETRIC | PS_ENDCAP_FLAT | PS_JOIN_ROUND);

  switch (Objeto)
  {
    case CAciTopografico::BORDO:  PenaObjeto->CreatePen(PS_DASH, 1, RGB(189, 141, 0)); break;
    case CAciTopografico::N_A: lb.lbColor = RGB(0, 0, 255); PenaObjeto->CreatePen(Style | PS_SOLID, 1, &lb); break;
    case CAciTopografico::N_A_PER: lb.lbColor = RGB(0, 0, 255); PenaObjeto->CreatePen(Style | PS_DOT, 1, &lb); break;
    case CAciTopografico::ROCHA: lb.lbColor = RGB(160, 160, 160); PenaObjeto->CreatePen(Style | PS_DASHDOTDOT, 1, &lb); break;
    case CAciTopografico::CANTO_CERCA:
    case CAciTopografico::CERCA: lb.lbColor = RGB(127, 127, 127); PenaObjeto->CreatePen(Style | PS_SOLID, 1, &lb); break;
    case CAciTopografico::CANTO_MURO:
    case CAciTopografico::MURO:  lb.lbColor = RGB(127, 127, 127); PenaObjeto->CreatePen(Style | PS_SOLID, 1,&lb); break;
    case CAciTopografico::CANTO_CASA:
    case CAciTopografico::CASA:  lb.lbColor = RGB(192, 192, 192); PenaObjeto->CreatePen(Style | PS_SOLID, 2, &lb); break;
    case CAciTopografico::MEIO_FIO: lb.lbColor = RGB(0, 0, 192); PenaObjeto->CreatePen(Style | PS_DASH, 2, &lb); break;
    case CAciTopografico::PONTE: lb.lbColor = RGB(189, 141, 0); PenaObjeto->CreatePen(Style | PS_SOLID, 2, &lb); break;
    case CAciTopografico::TAMPA_BUEIRO: lb.lbColor = RGB(192, 192, 192); PenaObjeto->CreatePen(Style | PS_SOLID, 1,&lb); break;
    case CAciTopografico::POSTE: lb.lbColor = RGB(192, 0, 0); PenaObjeto->CreatePen(Style | PS_DASHDOTDOT, 1, &lb); break;
    case CAciTopografico::REDE_ELETRICA_BT: lb.lbColor = RGB(192, 0, 0); PenaObjeto->CreatePen(Style | PS_DASHDOTDOT, 1,&lb); break;
    case CAciTopografico::REDE_ELETRICA_AT: lb.lbColor = RGB(255, 0, 0); PenaObjeto->CreatePen(Style | PS_DASHDOT, 1, &lb); break;
    case CAciTopografico::FRONTEIRA: lb.lbColor = RGB(64, 64, 192); PenaObjeto->CreatePen(Style | PS_SOLID, 2,&lb); break;
    case CAciTopografico::EIXO: lb.lbColor = RGB(223, 223, 223); PenaObjeto->CreatePen(Style | PS_DASHDOT, 1, &lb); break;
    case CAciTopografico::CRISTA: lb.lbColor = RGB(189, 157, 126); PenaObjeto->CreatePen(Style | PS_SOLID, 1, &lb); break;
    case CAciTopografico::PE: lb.lbColor = RGB(189, 157, 126); PenaObjeto->CreatePen(Style | PS_DOT, 1, &lb); break;
    case CAciTopografico::BUEIRO: lb.lbColor = RGB(0, 0, 223); PenaObjeto->CreatePen(Style | PS_SOLID, 1, &lb); break;
    case CAciTopografico::LINHA_FERREA: lb.lbColor = RGB(127, 127, 127); PenaObjeto->CreatePen(Style | PS_SOLID, 1,&lb); break;
    case CAciTopografico::ALAGADO:  lb.lbColor = RGB(128, 128, 255); PenaObjeto->CreatePen(Style | PS_DASHDOTDOT, 1, &lb); break;
    case CAciTopografico::ALINHAMENTO: lb.lbColor = RGB(50, 200,50); PenaObjeto->CreatePen(Style | PS_DASHDOT, 2, &lb); break;
    case CAciTopografico::ARVORE: lb.lbColor = RGB(0, 127, 0); PenaObjeto->CreatePen(Style | PS_SOLID, 1, &lb); break;
    case CAciTopografico::FLORESTA: lb.lbColor = RGB(0, 127, 0); PenaObjeto->CreatePen(Style | PS_SOLID, 1, &lb); break;
    case CAciTopografico::MATA: lb.lbColor = RGB(0, 127, 0); PenaObjeto->CreatePen(Style | PS_DASHDOT, 1, &lb); break;
    case CAciTopografico::PLANTACAO: lb.lbColor = RGB(0, 127, 0); PenaObjeto->CreatePen(Style | PS_DASHDOTDOT, 1,&lb); break;
    case CAciTopografico::OFF_SETS: PenaObjeto->CreatePen(PS_DOT, 1, Cores[CORCN]); break;
    case CAciTopografico::TERRAPLANAGEM: PenaObjeto->CreatePen(PS_DASHDOTDOT, 1, Cores[CORCN]); break;
     case CAciTopografico::TALVEGUE: lb.lbColor = RGB(0, 100, 255); PenaObjeto->CreatePen(Style | PS_SOLID, 2, &lb); break;
    case CAciTopografico::ESPIGAO: lb.lbColor = RGB(164,16,16); PenaObjeto->CreatePen(Style | PS_SOLID, 4,&lb); break;
    case CAciTopografico::RAVINA: lb.lbColor = RGB(128, 0, 64); PenaObjeto->CreatePen(Style | PS_DASHDOTDOT, 2, &lb); break;
    case CAciTopografico::PVELETRICA: lb.lbColor = RGB(255,0,0 ); PenaObjeto->CreatePen(Style | PS_SOLID, 1, &lb); break;
    case CAciTopografico::PVESGOTO: lb.lbColor = RGB(128, 0, 64); PenaObjeto->CreatePen(Style | PS_SOLID, 1, &lb); break;
    case CAciTopografico::PVGAS: lb.lbColor = RGB(255,255,0); PenaObjeto->CreatePen(Style | PS_SOLID, 1, &lb); break;
    case CAciTopografico::REDE_GAS: lb.lbColor = RGB(255, 255, 0); PenaObjeto->CreatePen(Style | PS_DASHDOT, 2, &lb); break;
    case CAciTopografico::REDE_ELETRICA: lb.lbColor = RGB(255,0, 0); PenaObjeto->CreatePen(Style | PS_DASHDOT, 2, &lb); break;
    case CAciTopografico::REDE_ESGOTO: lb.lbColor = RGB(64, 128, 128); PenaObjeto->CreatePen(Style | PS_DASHDOTDOT, 2, &lb); break;
    case CAciTopografico::SOLEIRA: lb.lbColor = RGB(255, 0, 0); PenaObjeto->CreatePen(Style | PS_SOLID, 2, &lb); break;

    default: PenaObjeto->CreatePen(PS_DASHDOTDOT, 2, RGB(255, 255, 255)); break;
  }
}

LRESULT DesPontosGeomView::FechouToolBar(WPARAM wParam, LPARAM lParam)
{
  CChildFrame* CF((CChildFrame*)GetParentFrame());

 if (lParam == IDR_TOOLDESENHAR2)
  {
    if ((((CMainFrame*)AfxGetMainWnd())->mpToolBoxDesenhar->IsWindowVisible()) == false)
    {
      CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_DESENHAR, false);
      DesenhandoObjetos = DesenharObjetos = EsbocandoObjetos = false;
    }
  }
  else
  {
    if (lParam == IDR_TOOLEQUIDRENAGEM)
    {
      if ((((CMainFrame*)AfxGetMainWnd())->mpCToolBarEquiDrenagem->IsWindowVisible()) == false)
      {
        ObjetosDrenagem = DesenhandoObjetos = DesenharObjetos = EsbocandoObjetos = false;
        CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_DRENAGEM,false);
      }
    }
  }
  return 0;
}

LRESULT DesPontosGeomView::AmpliarMapa(WPARAM wParam, LPARAM lParam)
{
  if (wParam > 0)
  {
    Escala[X] = Escala[Y] *= 1.1;
    m_nRatio *= 1.1;
  }
  else
  {
    Escala[X] = Escala[Y] *= 0.9;
    m_nRatio *= 0.9;
  }

  MudarEscala(Escala[X] * 100.0);

  RedrawWindow();


 // FaltaGeoReferenciamento = true;

  return 0;
}

void DesPontosGeomView::OnButponins()
{
  DesenharPontosInseridos = !DesenharPontosInseridos;

  RedrawWindow();
}

void DesPontosGeomView::DesenhaPoste(const Ponto& PontoPoste, CDC* pDC)
{
  CPen Pena, Pena1, * pPenaAnterior;

  Pena.CreatePen(PS_SOLID, 2, RGB(180, 160, 180));
  pPenaAnterior = pDC->SelectObject(&Pena);

  pDC->MoveTo((PontoPoste.x - Deltas[X]) * Escala[X], (AlturaVideo - (PontoPoste.y - Deltas[Y])) * Escala[Y]);
  pDC->LineTo((PontoPoste.x - Deltas[X]) * Escala[X], (AlturaVideo - (PontoPoste.y - Deltas[Y])) * Escala[Y] - 12);

  Pena1.CreatePen(PS_SOLID, 1, Cores[CORPONTOS]);
  pDC->SelectObject(&Pena1);

  pDC->MoveTo((PontoPoste.x - Deltas[X]) * Escala[X] - 4, (AlturaVideo - (PontoPoste.y - Deltas[Y])) * Escala[Y] - 9);
  pDC->LineTo((PontoPoste.x - Deltas[X]) * Escala[X] + 4, (AlturaVideo - (PontoPoste.y - Deltas[Y])) * Escala[Y] - 9);

  pDC->SetPixel((PontoPoste.x - Deltas[X]) * Escala[X] - 5, (AlturaVideo - (PontoPoste.y - Deltas[Y])) * Escala[Y] - 8, Cores[CORPONTOS]);
  pDC->SetPixel((PontoPoste.x - Deltas[X]) * Escala[X] - 6, (AlturaVideo - (PontoPoste.y - Deltas[Y])) * Escala[Y] - 8, Cores[CORPONTOS]);

  pDC->SetPixel((PontoPoste.x - Deltas[X]) * Escala[X] + 4, (AlturaVideo - (PontoPoste.y - Deltas[Y])) * Escala[Y] - 8, Cores[CORPONTOS]);
  pDC->SetPixel((PontoPoste.x - Deltas[X]) * Escala[X] + 5, (AlturaVideo - (PontoPoste.y - Deltas[Y])) * Escala[Y] - 8, Cores[CORPONTOS]);

  pDC->SelectObject(pPenaAnterior);
}


void DesPontosGeomView::DesenhaBueiro(const Ponto& PontoBueiro, CPen* Pena, CDC* pDC)
{
  CPen PenaAzul, PenaVerm;

  PenaVerm.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
  PenaAzul.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

  CPen* pPenaAnterior((CPen*)pDC->SelectObject(&PenaVerm));

  Ponto Ppixels((PontoBueiro.x - Deltas[X]) * Escala[X], (AlturaVideo - (PontoBueiro.y - Deltas[Y])) * Escala[Y]);

  pDC->Arc(Ppixels.x - 7, Ppixels.y - 7, Ppixels.x + 7, Ppixels.y + 7, 0, 0, 0, 0);
  pDC->Arc(Ppixels.x - 6, Ppixels.y - 6, Ppixels.x + 6, Ppixels.y + 6, 0, 0, 0, 0);
  pDC->Arc(Ppixels.x - 5, Ppixels.y - 5, Ppixels.x + 5, Ppixels.y + 5, 0, 0, 0, 0);

  pDC->SelectObject(&PenaAzul);

  pDC->Arc(Ppixels.x - 4, Ppixels.y - 4, Ppixels.x + 4, Ppixels.y + 4, 0, 0, 0, 0);
  pDC->Arc(Ppixels.x - 3, Ppixels.y - 3, Ppixels.x + 3, Ppixels.y + 3, 0, 0, 0, 0);

  pDC->SelectObject(pPenaAnterior);
}

void DesPontosGeomView::DesenhaArvore(const Ponto& PontoArvore, CDC* pDC)
{
  CPen Pena, Pena1, * pPenaAnterior;
  COLORREF Cor(RGB(0, 128, 0));

  Pena.CreatePen(PS_SOLID, 1, Cor);
  pPenaAnterior = pDC->SelectObject(&Pena);

  Ponto Ppixels((PontoArvore.x - Deltas[X]) * Escala[X], (AlturaVideo - (PontoArvore.y - Deltas[Y])) * Escala[Y]);

  pDC->Arc(Ppixels.x - 6, Ppixels.y - 6, Ppixels.x + 6, Ppixels.y + 6, 0, 0, 0, 0);

  pDC->SelectObject(pPenaAnterior);
}

void DesPontosGeomView::DesenhaPVEsgoto(const Ponto& PontoPVEsgoto, CPen* Pena, CDC* pDC)
{
  CPen PenaAzul,PenaVerm;

  PenaVerm.CreatePen(PS_SOLID, 1, RGB(255,0,0));
  PenaAzul.CreatePen(PS_SOLID, 1, RGB(0,0,255));

  CPen* pPenaAnterior((CPen*)pDC->SelectObject(&PenaVerm));

  Ponto Ppixels((PontoPVEsgoto.x - Deltas[X]) * Escala[X], (AlturaVideo - (PontoPVEsgoto.y - Deltas[Y])) * Escala[Y]);

  pDC->Arc(Ppixels.x - 4, Ppixels.y - 4, Ppixels.x + 4, Ppixels.y + 4, 0, 0, 0, 0);
 // pDC->Arc(Ppixels.x - 5, Ppixels.y - 5, Ppixels.x + 5, Ppixels.y + 5, 0, 0, 0, 0);

  pDC->SelectObject(&PenaAzul);

  pDC->Arc(Ppixels.x - 3, Ppixels.y - 3, Ppixels.x + 3, Ppixels.y + 3, 0, 0, 0, 0);
  pDC->Arc(Ppixels.x - 2, Ppixels.y - 2, Ppixels.x + 2, Ppixels.y + 2, 0, 0, 0, 0);

  pDC->SelectObject(pPenaAnterior);
}

void DesPontosGeomView::DesenhaPVEletrica(const Ponto& PontoPVEletrica, CPen* Pena, CDC* pDC)
{
  CPen* pPenaAnterior((CPen*)pDC->SelectObject(&Pena));
  Ponto Ppixels((PontoPVEletrica.x - Deltas[X]) * Escala[X], (AlturaVideo - (PontoPVEletrica.y - Deltas[Y])) * Escala[Y]);

  CBrush brush(RGB(255, 255, 0));

  pDC->SelectObject(brush);

  pDC->Arc(Ppixels.x - 7, Ppixels.y - 7, Ppixels.x + 7, Ppixels.y + 7, 0, 0, 0, 0);
  pDC->Arc(Ppixels.x - 6, Ppixels.y - 6, Ppixels.x + 6, Ppixels.y + 6, 0, 0, 0, 0);
  pDC->Arc(Ppixels.x - 5, Ppixels.y - 5, Ppixels.x + 5, Ppixels.y + 5, 0, 0, 0, 0);
  pDC->Arc(Ppixels.x - 4, Ppixels.y - 4, Ppixels.x + 4, Ppixels.y + 4, 0, 0, 0, 0);
  pDC->Arc(Ppixels.x - 3, Ppixels.y - 3, Ppixels.x + 3, Ppixels.y + 3, 0, 0, 0, 0);

  pDC->SelectObject(pPenaAnterior);
}

void DesPontosGeomView::DesenhaPVGas(const Ponto& PontoPVGas, CPen* Pena, CDC* pDC)
{
  pDC->SelectObject(Pena);

  Ponto Ppixels((PontoPVGas.x - Deltas[X]) * Escala[X], (AlturaVideo - (PontoPVGas.y - Deltas[Y])) * Escala[Y]);

  CBrush brush(RGB(255,255,0));

  pDC->SelectObject(brush);

  pDC->Rectangle(int(Ppixels.x - 4),int(Ppixels.y - 4),int(Ppixels.x + 4),int(Ppixels.y + 4));
}

void DesPontosGeomView::DesenhaSoleira(const Ponto& PontoSoleira, CPen* Pena, CDC* pDC)
{
  Ponto Ppixels((PontoSoleira.x - Deltas[X]) * Escala[X], (AlturaVideo - (PontoSoleira.y - Deltas[Y])) * Escala[Y]);

  CPen* pPenaAnterior((CPen*)pDC->SelectObject(&Pena));

  pDC->MoveTo(Ppixels.x-7, Ppixels.y);
  pDC->LineTo(Ppixels.x+7, Ppixels.y);

  pDC->Arc(Ppixels.x-3, Ppixels.y-3, Ppixels.x+3, Ppixels.y+3, 0, 0, 0, 0);
}

ItCLLDeltaSup DesPontosGeomView::VerificaNaTangenteRestricoes(ItCLLDeltaSup ItCLLDeltaSupInicial, const Ponto& PAtual, double Toleranciax, ItCLDeltaSup* pItCLDS, double* pRaio, CSuperficie* pSuperfTerrap)
{
  if (pItCLDS != nullptr) *pItCLDS = ItCLLDeltaSupInicial->end();
  if (pRaio != nullptr) *pRaio = 0.0;

  CSuperficie* pSuperfAtual(pSuperfTerrap ? pSuperfTerrap : &Superficie);

  const LLDeltaSup& ListaRestAtual(pSuperfAtual->PegaListaRestricoes());
  ItCLLDeltaSup itLRestricaoAtual(ListaRestAtual.end());

  for (itLRestricaoAtual = ItCLLDeltaSupInicial; itLRestricaoAtual != ListaRestAtual.end(); ++itLRestricaoAtual)
  {

    if (itLRestricaoAtual->size() > 1)
    {
      unsigned int TipoRestricao(pSuperfAtual->VerfAciTopografico(itLRestricaoAtual));

      if (!FiltrarAciTopog || TipoRestricao == ObjetoAtual)
      {
        register ItCLDeltaSup itPontoRestricaoAtual(itLRestricaoAtual->begin());
        ItCLDeltaSup itPontoRestricaoAnterior(itPontoRestricaoAtual);

        bool NaTangente(false);

        while (!NaTangente && ++itPontoRestricaoAtual != itLRestricaoAtual->end())
        {
          double Raio(hypot(itPontoRestricaoAnterior->PegaPCentral().x - PAtual.x, itPontoRestricaoAnterior->PegaPCentral().y - PAtual.y));

          if (Raio < Toleranciax)  
          {
            NaTangente = true;  
          }
          else
          {
            double DistPontos(hypot(itPontoRestricaoAtual->PegaPCentral().x - itPontoRestricaoAnterior->PegaPCentral().x, itPontoRestricaoAtual->PegaPCentral().y - itPontoRestricaoAnterior->PegaPCentral().y));

            if (Raio < (DistPontos + Toleranciax))
            {
              double AzReta(CCurHorizontais::CalculaAzimute(itPontoRestricaoAtual->PegaPCentral(), itPontoRestricaoAnterior->PegaPCentral())),
                     AzPonto(CCurHorizontais::CalculaAzimute(PAtual, itPontoRestricaoAnterior->PegaPCentral()));

              if (fabs(AzReta - AzPonto) < Matem::M_PI_2 / 4.0)
              {
                double Distancia(fabs(sin(AzPonto - AzReta) * Raio));
                NaTangente = Distancia < Toleranciax;
              }
            }
          }

          if(NaTangente)
          {
            if (pItCLDS != NULL) *pItCLDS = itPontoRestricaoAnterior;
            if (pRaio != NULL) *pRaio = Raio;
          }
          itPontoRestricaoAnterior = itPontoRestricaoAtual;
        }

        if (NaTangente)  
        {
          return itLRestricaoAtual;
        }
      }
    }
    else
    {
      if(itLRestricaoAtual->size() > 0)
      {
        double Raio(hypot(itLRestricaoAtual->begin()->PegaPCentral().x - PAtual.x, itLRestricaoAtual->begin()->PegaPCentral().y - PAtual.y));

        if (Raio < Toleranciax) return itLRestricaoAtual;
      }
    }
  }

  return pSuperfAtual->PegaListaRestricoes().end();
}

void DesPontosGeomView::Ima(UINT nFlags, const Ponto& XYPonto)
{
  CPoint PPixels;
  TransfXYPixels(XYPonto.x, XYPonto.y, PPixels);
  ClientToScreen(&PPixels);
  SetCursorPos(PPixels.x, PPixels.y);
  CView::OnMouseMove(nFlags, PPixels);
}
/*
bool DesPontosGeomView::VerfArestaForaSuperficie(const Ponto& P1,const Ponto& P2)
{
const LLDeltaSup& ListaRestAtual(Superficie.PegaListaRestricoes());
ItCLLDeltaSup itLRestricaoAtual(ListaRestAtual.end());

for(ItCLLDeltaSup ItListaRestAtual = ListaRestAtual.begin() ; ItListaRestAtual!= ListaRestAtual.end() ; ++ItListaRestAtual)
{
unsigned int TipoRestricao(Superficie.VerfAciTopografico(ItListaRestAtual));

if(TipoRestricao == CAciTopografico::FRONTEIRA)
{
for(register ItCLDeltaSup ItPontoRestAtual = ItListaRestAtual->begin() ; ItPontoRestAtual!= ItListaRestAtual->end() ; ++ItPontoRestAtual)
{
if(*ItPontoRestAtual == P1)
{
ItCLDeltaSup ItProximo(ItPontoRestAtual),ItAnterior(ItPontoRestAtual);
--ItAnterior;
++ItProximo;

//--- Calcular o fi no cálculo da fronteira, assim não será preciso calcular o azimute aqui

double AZPosterior(CCurHorizontais::CalculaAzimute(ItProximo->PegaPCentral(),ItPontoRestAtual->PegaPCentral())),
AZAnterior(CCurHorizontais::CalculaAzimute(ItAnterior->PegaPCentral(),ItPontoRestAtual->PegaPCentral())),
AZRestricao(CCurHorizontais::CalculaAzimute(P2,P1));

if((Superficie.PegaArea() > 0.0 && ((AZPosterior > AZAnterior && AZRestricao < AZPosterior && AZRestricao > AZAnterior) || (AZPosterior < AZAnterior && !(AZRestricao > AZPosterior && AZRestricao < AZAnterior)))) ||
(Superficie.PegaArea() < 0.0 && ((AZPosterior < AZAnterior && AZRestricao > AZPosterior && AZRestricao < AZAnterior) || (AZPosterior > AZAnterior && !(AZRestricao < AZPosterior && AZRestricao > AZAnterior)))))
{
return true;
}
}
}
}
}
return false;
}

*/
void DesPontosGeomView::OnUpdateDesFronteira(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(DesenharBordos);
}

void DesPontosGeomView::VaiParaPonto(const CPoint& Ponto)
{
  Deltas[X] = Ponto.x;
  Deltas[Y] = Ponto.y;

  DeltasReais.x = Deltas[X] -= LarguraVideo / 2.0;
  DeltasReais.y = Deltas[Y] -= AlturaVideo / 2.0;
}

void DesPontosGeomView::OnPopupDesabilitar()
{
  ItLLDeltaSup ItLLDS;
  ItLDeltaSup itPonto;

  if (!Superficie.VerfPontoRestricao(DeltaSupSobMousePopup->PegaPCentral(), ItLLDS, itPonto) ||
      !Superficie.VerfGeraArestaForcada(DeltaSupSobMousePopup->TipoObjeto))
  {
    Superficie.DesabilitaDeltaSuperficie(DeltaSupSobMousePopup);
    AlterouTerreno = true;

    RedrawWindow();
  }
  else monolog.mensagem(-1,"Pontos pertencentes a acidentes topográficos não podem ser desabilitados.");
}

void DesPontosGeomView::OnPopupIncluirPonto()
{
  InserindoPonto = true;
}

void DesPontosGeomView::OnPopupHabilitar()
{
  Superficie.HabilitaDeltaSuperficie(ItPontoDesabilitado);
  ItPontoDesabilitado = Superficie.SetPonDesabilitados().end();
  AlterouTerreno = true;

  RedrawWindow();
}

void DesPontosGeomView::OnButAmpliarMapa()
{
  CDiaAmpliarMapa DiaAmpliarMapa(this);

  DiaAmpliarMapa.DoModal();

  CalculaUTMpRef(1);
  RazaoEscalasJPGDes = INFINITO;

  ((CChildFrame*)GetParentFrame())->DeslizanteGeom.m_CEEscala.EnableWindow(true);
}

void DesPontosGeomView::OnUpdateButAmpliarMapa(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(1 || ProjetoGeoreferenciado != SUPERF_TRADICIONAL && !NomeMapa.IsEmpty());
}

void DesPontosGeomView::OnUpdateButRemoverGeoRef(CCmdUI* pCmdUI)
{
  #ifdef PACOTE_AVANCADO
    pCmdUI->Enable(ProjetoGeoreferenciado != SUPERF_TRADICIONAL);
  #else
    pCmdUI->Enable(FALSE);
  #endif
}

void DesPontosGeomView::OnUpdatePopupDesabilitar(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(PegouPonto);
}

void DesPontosGeomView::OnUpdatePopupHabilitar(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(PegouPontoDesabilitado);
}

void DesPontosGeomView::OnUpdatePopupIncluirPonto(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(!PegouPonto && !PegouPontoDesabilitado && !PegouPontoInserido && !PegouPontoPerfilAvulso && ItLembrete == SetLembretes.end());
}

void DesPontosGeomView::OnUpdatePopupRemoverPonto(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(PegouPontoInserido || PegouPontoPerfilAvulso || ItLembrete != SetLembretes.end());
}

void DesPontosGeomView::OnUpdatePopupRemover(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end() && 
                 ProjetoGeoreferenciado == SUPERF_TRADICIONAL || ItLLDeltaSupMarcada->begin()->TipoObjeto != 1);
}

void DesPontosGeomView::OnPopupRemoverPonto()
{
  if (ItLembrete != SetLembretes.end())
  {
    SetLembretes.erase(ItLembrete);
    ItLembrete = SetLembretes.end();
    InserindoLembrete = MovendoLembrete = false;
  }
  else
  {
    if (PegouPontoPerfilAvulso)
    {
      RemovePerfilAvulso(ItSecaoAvulsaAtual);
      RedrawWindow();

      return;
    }

    if (RemovePontoRestricoes(*DeltaSupSobMousePopup))
    {
      Superficie.RemovePontoInseridoUsuario(DeltaSupSobMousePopup);
    }
    else
    {
      monolog.mensagem(-1,"A remoção deste ponto não pode ser feita pois causará interseção \nentre acidentes topográficos que geram arestas forçadas.");
      CircularPontoIntersecao = true;
    }
  }

  RedrawWindow();
}

void DesPontosGeomView::OnButLembrete()
{
  InserindoLembrete = true;

  RedrawWindow();
}

void DesPontosGeomView::OnButRadar()
{
  MostrarRadar = MostrarRadar == false;

  RedrawWindow();
}

void DesPontosGeomView::OnButProjTerrap()
{
  MostrarTerrap = MostrarTerrap == false;

  if(!MostrarTerrap)    //-- So pode inserir objetos de drenagem sobre o projeto de terraplanagem 
  {
    ObjetosDrenagem = DesenhandoObjetos = false;

    CChildFrame* CF((CChildFrame*)GetParentFrame());

    CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_DRENAGEM, ObjetosDrenagem);
    CF->m_wndToolBarPonGeo.GetToolBarCtrl().SetState(ID_DESENHAR, DesenhandoObjetos);
    CF->ShowControlBar(&CF->m_wndToolBarEquiDrenagem, ObjetosDrenagem ? SW_SHOWNORMAL : SW_HIDE, false);
  }

  RedrawWindow();
}

void DesPontosGeomView::OnUpdateButtrocaracitopog(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end());
  //pCmdUI->Enable(true);
}

void DesPontosGeomView::OnPopupRemover()
{
  POINT Ponto;

  GetCursorPos(&Ponto);
  CPoint PDialogo(Ponto);
  PDialogo.Offset(5, 5);

  CDiaRemAciTopog DiaRemAciTopografico(this, PDialogo);

  if (RemovendoEspigao || RemovendoDrenagem || DiaRemAciTopografico.DoModal() == IDOK)
  {
    unsigned int TipoRestricao(Superficie.VerfAciTopografico(ItLLDeltaSupMarcada));

    if (TipoRestricao == CAciTopografico::FRONTEIRA)
    {
      std::string Msg("Removendo a fronteira você a estará removendo de todos os trechos deste projeto.\r\n\r\n");
      Msg += "                          Tem certeza que é isso que deseja?";

      if (((CMainFrame*)AfxGetMainWnd())->FronteiraEditada)
      {
        if (AfxMessageBox("Foram feitas edições manuais nesta fronteira, ao removê-la estas edições serão perdidas e muito provavelmente terão que ser refeitas, confirma?", MB_YESNO | MB_ICONEXCLAMATION) != IDYES)

          return;
      }
      if (AfxMessageBox(Msg.c_str(), MB_OKCANCEL) == IDCANCEL)
      {
        ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();
        RedrawWindow();
        return;
      }

      //--- Remove da Terraplanagem também

      Trechos.PegaTrechoAtual()->second.SuperficieTerr.RemoveListasDoTipo(CAciTopografico::FRONTEIRA);

      std::string NomeArq(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase());
      NomeArq += ".bor";

      DeleteFile(NomeArq.c_str());
    }

    //--- Se for um acidente de pontos esboçados remove os pontos esboçados da superficie
    //--- S = -100.0 indica ponto esboçado
    
    if(ItLLDeltaSupMarcada == Superficie.PegaListaRestricoes().end()) return;

    ItCLDeltaSup itFim(ItLLDeltaSupMarcada->end());
    
    if (*ItLLDeltaSupMarcada->begin() == *ItLLDeltaSupMarcada->rbegin()) --itFim;     

    //--- Verifica os talvegues e espigões das bacias hidrograicas

    ittylstBacias ItBaciaAtual(VerfClicouBaciaHidrografica(ItLLDeltaSupMarcada));

    if(ItBaciaAtual != Superficie.PegaBacias().end())  
    {
      if (AfxMessageBox("Este Espigão está associado a uma bacia hidrográfica, sua remoção removerá a bacia hidrográfica também, confirma?", MB_OKCANCEL) == IDOK)
      {
        Superficie.PegaBacias().erase(ItBaciaAtual) ;
      }
      else return;
    }
    else
    {
      ItBaciaAtual = VerfClicouTalvegueBacia(ItLLDeltaSupMarcada);

      if (ItBaciaAtual != Superficie.PegaBacias().end())
      {
        if(AfxMessageBox(" Este talvegue pertence a uma bacia hidrográfica, ao removê-lo a bacia hidrográfica terá que ser calculada novamente, confirma? ", MB_OKCANCEL) == IDOK)
        { 
		       ItBaciaAtual->Calculada() = false;
        }
        else 
		    {
		      return;
		    }
      }
    }

    //--- Verifica nos equipamentos de drenagem.

    if (ItEquiDrenagemMarcada != Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end())
    {
      for (ItCLDeltaSup it = ItLLDeltaSupMarcada->begin(); it != itFim; ++it)
      {
        if (it->PegaPCentral().Baliza2 & Ponto::ESBOCADO)
        {
          if (fabs(it->PegaPCentral().S - -100.0) < 0.1)
          {
            if (VerfPontoOutroEquipamento(it->PegaPCentral()) == false)           //--- se este ponto pertencer a outros equipamento não o deleta
            {
              Superficie.RemovePonto(it->PegaPCentral());
            }
          }
        }
      }
    }

    DeltaSupSobMouseAnterior = Superficie.pSuperficieAtual->end();

    //--- Verifica se o ponto pertence a outro acidente topografico esboçado, se pertencer não remove

    if(ItLLDeltaSupMarcada->begin()->TipoObjeto == CAciTopografico::ESPIGAO)
    {
      for (ItCLDeltaSup it = ItLLDeltaSupMarcada->begin(); it != ItLLDeltaSupMarcada->end() ; ++it)
      {
        if (it->PegaPCentral().Baliza2 & Ponto::ESBOCADO)
        {
          lstItCLLDeltaSup LstRestNoPonto;
          Superficie.PegaRestricoesNoPonto(it->PCentral, LstRestNoPonto);

          if (LstRestNoPonto.size() < 2)
          {
            Superficie.RemovePonto(it->PegaPCentral());
          }
        }
      }
    }
    //--- finalmente remove o acidente topográfico

    Superficie.RemoveListaDoMapa(ItLLDeltaSupMarcada, ItLLDeltaSupMarcada->begin()->TipoObjeto,ItLLDeltaSupMarcada->begin()->TipoObjeto != CAciTopografico::ESPIGAO);   //--- Não remover os pontos ( Já removeu osque podemser removidos)
    Superficie.PegaListaRestricoes().erase(ItLLDeltaSupMarcada);

    AlterouTerreno = true;
  }

  ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();

  RedrawWindow();
}

void DesPontosGeomView::OnPopupTrocar()
{
  ObjetoAnterior = ObjetoAtual = Superficie.VerfAciTopografico(ItLLDeltaSupMarcada);
  TrocandoObjeto = true;
}

void DesPontosGeomView::OnPopupEditar()
{
  if (ItSecaoAvulsaAtual != SecoesAvulsas.End())
  {
    CSecaoAvulsa& SecaoAvAtual(SecoesAvulsas.PegaLSecoesAvulsas().GetAt(ItSecaoAvulsaAtual->second));

    Ponto P1(SecaoAvAtual.PInicial), P2(SecaoAvAtual.PFinal);

    CDEditarSecAvulsa DEditarSecAvulsa(this, SecaoAvAtual.PInicial, SecaoAvAtual.PFinal);

    DEditarSecAvulsa.DoModal();

    RedrawWindow();

    AlterouSecoesAvulsas = true;
  }
}

void DesPontosGeomView::OnUpdatePopupEditar(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(PegouPontoPerfilAvulso);
}

void DesPontosGeomView::InserePontoNaRestricaoMarcada(ItCLLDeltaSup ItCLLDeltaSupMarcada, Ponto& PInserir, bool CalcularCota)
{
  if (ItCLDS != ItCLLDeltaSupMarcada->end())      //--- Foi marcado no onmousemove
  {
    ItCLDeltaSup ItProxDS(ItCLDS);
    ++ItProxDS;
    double Az(Mat.CalculaAzimute((ItCLDS)->PegaPCentral(), ItProxDS->PegaPCentral()));

    //--- Ajusta o ponto clicado na restrição

    Mat.IrradiaPonto(Az, ItCLDS->PegaPCentral().x, ItCLDS->PegaPCentral().y, 0.0, Raio, PInserir);    //--- pRaio Foi marcado no onmousemove
    if (CalcularCota) CalculaCota(ItCLDS->PegaPCentral(), ItProxDS->PegaPCentral(), PInserir, Raio);
    Mat.Arredonda(PInserir, 3);
    (const_cast<LDeltaSup*>(&*ItCLLDeltaSupMarcada))->insert(ItProxDS, PInserir);
  }
}

bool DesPontosGeomView::RemovePontoRestricoes(const CDeltaSuperficie& DSRemover)
{
  typedef struct stIterListasRemover
  {
    ItLLDeltaSup itListaRestricao;
    ItLDeltaSup itRemover;

    stIterListasRemover(ItLLDeltaSup pitLstRest, ItLDeltaSup pitRem) :itListaRestricao(pitLstRest), itRemover(pitRem) {};

  }stIteradoresListasRemover;

  std::list<stIteradoresListasRemover> ListaItARemover;
  LItLLDeltaSup LItRestricoesRemover;

  bool Ok(true);

  for (ItLLDeltaSup ItLRestricao = Superficie.PegaListaRestricoes().begin(); ItLRestricao != Superficie.PegaListaRestricoes().end(); ++ItLRestricao)
  {
    ItLDeltaSup itPontoRestricao(std::find(ItLRestricao->begin(), ItLRestricao->end(), DSRemover));

    if (itPontoRestricao != ItLRestricao->end())
    {
      stIteradoresListasRemover stIter(ItLRestricao, itPontoRestricao);
      if (VerfRemocaoPontoRestricao(ItLRestricao, itPontoRestricao) == true) ListaItARemover.push_back(stIteradoresListasRemover(ItLRestricao, itPontoRestricao));
      else
      {
        Ok = false;
      }
    }
  }

  if (Ok)
  {
    for (std::list<stIteradoresListasRemover>::iterator it = ListaItARemover.begin(); Ok&& it != ListaItARemover.end(); ++it)
    {
      it->itListaRestricao->erase(it->itRemover);
      if (it->itListaRestricao->size() < 2)	LItRestricoesRemover.push_front(it->itListaRestricao);
    }

    for (register ItLItLLDeltaSup ItLRemover = LItRestricoesRemover.begin(); ItLRemover != LItRestricoesRemover.end(); ++ItLRemover)
    {
      Superficie.RemoveListaDoMapa(*ItLRemover);
      Superficie.PegaListaRestricoes().erase(*ItLRemover);
    }
  }

  return Ok;
}

const ittysetLembretes DesPontosGeomView::PegaLembreteMaisProximo(const Ponto& PontoLembrete, double Toleranciax)
{
  CLembrete LembreteTemp(CLembrete::DES_GEOMETRICO, PontoLembrete);
  LembreteTemp.Deslocax(Toleranciax);

  ittysetLembretes itLembreteFinal(SetLembretes.upper_bound(LembreteTemp)), itLembreteAtual(SetLembretes.end()), itLembreteMaisProximo(SetLembretes.end());

  LembreteTemp.Deslocax(-Toleranciax * 2.0);
  itLembreteAtual = SetLembretes.upper_bound(LembreteTemp);         //--- Pega o primeiro na tolerancia    

  double MenorDistancia(INFINITO);

  //--- Separa o que está mais próximo do mouse 

  while (itLembreteAtual != itLembreteFinal)
  {
    Ponto PAtual(itLembreteAtual->Local());
    double DistanciaAtual(hypot(PAtual.x - LembreteTemp.Local().x, PAtual.y - LembreteTemp.Local().y));
    if (DistanciaAtual < MenorDistancia)
    {
      MenorDistancia = DistanciaAtual;
      itLembreteMaisProximo = itLembreteAtual;
    }

    itLembreteAtual++;
  }

  return (MenorDistancia < Toleranciax) ? itLembreteMaisProximo : SetLembretes.end();
}

void DesPontosGeomView::SalvaLembretes()
{
  std::string NomeArquivo(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual() + std::string(".lem"));
  UINT Flags;
  Flags = CFile::modeCreate | CFile::modeWrite;

  CFile Arq;
  CFileException e;

  if (Arq.Open(NomeArquivo.c_str(), Flags, &e) == TRUE)
  {
    if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
    {
      CArchive ArcLembretes(&Arq, CArchive::store);

      ArcLembretes << SetLembretes.size();

      for (ittysetLembretes itLembreteAtual = SetLembretes.begin(); itLembreteAtual != SetLembretes.end(); ++itLembreteAtual)
      {
        (CLembrete&)*itLembreteAtual >> ArcLembretes;
      }
    }
  }
}

void DesPontosGeomView::LeLembretes()
{
  std::string NomeArquivo(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual() + std::string(".lem"));
  UINT Flags;
  Flags = CFile::modeRead;

  CFile Arq;
  CFileException e;

  if (Arq.Open(NomeArquivo.c_str(), Flags, &e) == TRUE)
  {
    if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
    {
      CArchive ArcLembretes(&Arq, CArchive::load);
      size_t Qtd(0);
      CLembrete Lembrete;

      for (ArcLembretes >> Qtd; Qtd > 0; --Qtd)
      {
        Lembrete << ArcLembretes;
        SetLembretes.insert(Lembrete);
      }
    }
  }
}

bool DesPontosGeomView::VerfRemocaoPontoRestricao(ItLLDeltaSup& ItLRestricao, ItLDeltaSup ItPonto)
{
  if (ItLRestricao->size() > 2)
  {
    if (*ItPonto != *ItLRestricao->begin() && *ItPonto != *ItLRestricao->rbegin())
    {
      Ponto P1((--ItPonto)->PCentral), P2((++(++ItPonto))->PCentral);

      return !Superficie.VerificaIntersecaoAcidentesTopog(P1, P2, PontoIntersecao, false, true);  //--- o certo aqui é nenhum acitopog em vez de false//=== corrigir
    }
  }

  return true;
}

bool DesPontosGeomView::RemovePontoRestricao(ItLLDeltaSup& ItLRestricao, ItLDeltaSup ItPonto)
{
  ittylstBacias ItBacia(VerfClicouBaciaHidrografica(ItLRestricao));

  if(ItBacia != Superficie.PegaBacias().end()) 
  {
    monolog.mensagem(-1," A bacia hidrográfica à qual este talvegue pertence precisará de ser calculada novamente.");
    ItBacia->Calculada() = false;
  }

  bool ListaCircular(*ItLRestricao->begin() == *ItLRestricao->rbegin()),
       RemPrimPontoListaCircular(ListaCircular && *ItPonto == *ItLRestricao->begin());

  if ((ListaCircular && ItLRestricao->size() < 4) || (!ListaCircular && ItLRestricao->size() < 3)) 
  {
    return false;
  }

  if (!RemPrimPontoListaCircular && (*ItPonto == *ItLRestricao->begin() || *ItPonto == *ItLRestricao->rbegin())) 
  {
    return false;
  }

  register ItLDeltaSup ItInicio(ItPonto), ItFim(ItPonto);

  do
  {
    ++ItFim;

  } while (ItFim != ItLRestricao->end() && (ItFim->PegaPCentral().Descricao.Find("Ponto Inserido") != std::string::npos && ItFim->PegaPCentral().Descricao.Find("Usuário") == std::string::npos));

  if (ItFim == ItLRestricao->end())
  {
    AfxMessageBox("Erro DPGV-001 - Contacte o suporte");

    return false;
  }

  if (RemPrimPontoListaCircular) 
  {
    ItInicio = --(ItLRestricao->end());
  }
  else
  {
    --ItInicio;
  }

  while (ItInicio != ItLRestricao->begin() && (ItInicio->PegaPCentral().Descricao.Find("Ponto Inserido") != std::string::npos && ItInicio->PegaPCentral().Descricao.Find("Usuário") == std::string::npos))
  {
    if (ItInicio == ItLRestricao->begin())
    {
      AfxMessageBox("Erro DPGV-0002 - Contacte o suporte.");

      return false;
    }
    --ItInicio;
  }

  if (!Superficie.VerificaIntersecaoAcidentesTopog(ItInicio->PCentral, ItFim->PCentral, PontoIntersecao, false, true))  //--- O certo aqui é nenhumacitopof aio inves de false corrigir
  {
    if (!RemPrimPontoListaCircular) 
    {
      ++ItInicio;

      //--- Se for ponto esboçado remove da superfície tb 
      //--- (os outros tipos de acidentes topográficos os pontos ficam na superfície)

      Ponto PSuperf(ItInicio->PegaPCentral());

      if (ItInicio->PCentral.Baliza2 & Ponto::ESBOCADO)
      {
        Superficie.RemovePonto(PSuperf);
      }
        
      ItLRestricao->erase(ItInicio, ItFim);
    }
    else
    {
      ItLRestricao->pop_front();
      ItLRestricao->pop_back();

      if (ItLRestricao->size() > 2) ItLRestricao->push_back(*ItLRestricao->begin());    //--- fecha o círculo
    }

    if (ListaCircular && ItLRestricao->size() == 3) 
    {
      ItLRestricao->pop_back();   //--- Com  3 pontos a lista deixa de ser circular  
    }
  }
  else
  {
    monolog.mensagem(-1,"A remoção deste ponto não pode ser feita pois causará interseção \nentre acidentes topográficos que geram arestas forçadas.");
    CircularPontoIntersecao = true;
    RedrawWindow();

    return false;
  }

  return true;
}

void DesPontosGeomView::OnButarrastar()
{
  PermitirArrastar = PermitirArrastar == false;
  PermitirArrastar = true;
}

void DesPontosGeomView::RotacionarLinhaBase()
{
  OnButrotacionar();
  DefinindoLinhaBase = 0;
}

void DesPontosGeomView::Rotacionar(double Angulo)
{
  AnguloRotacaoAnterior = Angulo;
  ZerarAzimute = false;
  OnButrotacionar();
}

void DesPontosGeomView::DesenharCruzPonto(const Ponto& PCentral, CDC* ppDC, int TipoPonto)
{
  CPoint PPixels(PCentral.x, PCentral.y);

  PPixels.x = (PCentral.x - Deltas[X]) * Escala[X];
  PPixels.y = (AlturaVideo - (PCentral.y - Deltas[Y])) * Escala[Y];

  CDC* pDC(ppDC ? ppDC : GetDC());
  if (!pDC) return;              //--- Segurança, aconteceu nos testes

  CPen PenaMalha, * pPenaAnterior;
  PenaMalha.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
  pPenaAnterior = pDC->SelectObject(&PenaMalha);

  //--- Desenha a cruz, 14 pixels de largura

  pDC->MoveTo(PPixels.x - 15, PPixels.y);
  pDC->LineTo(PPixels.x + 15, PPixels.y);
  pDC->MoveTo(PPixels.x, PPixels.y - 15);
  pDC->LineTo(PPixels.x, PPixels.y + 15);

  pDC->Arc(PPixels.x - 4, PPixels.y - 4, PPixels.x + 4, PPixels.y + 4, 0, 0, 0, 0);    //--- Circula o ponto.

  pDC->SelectObject(pPenaAnterior);

  ReleaseDC(pDC);
}

ItCLLDeltaSup DesPontosGeomView::VerfClickRestricao(Ponto& PAtual, CSuperficie* pSuperfTerrap)
{
  CSuperficie* pSuperficieAtual(pSuperfTerrap ? pSuperfTerrap : &Superficie);

  return pSuperficieAtual->PegaListaRestricoes().end();
  LLDeltaSup& ListasRestricoes(pSuperficieAtual->PegaListaRestricoes());
  ItCLLDeltaSup ItLRestricaoAtual(ListasRestricoes.begin()), ListaRetClicada(ListasRestricoes.end());
  PegouRestricao = false;

  while (!PegouRestricao && ItLRestricaoAtual != ListasRestricoes.end())
  {
    if (pSuperficieAtual->EstaRotacionada()) PAtual.RotacionaPlano(pSuperficieAtual->PegaSeno(), pSuperficieAtual->PegaCoseno(), pSuperficieAtual->PegaAnguloRotacao());
    ListaRetClicada = VerificaNaTangenteRestricoes(ItLRestricaoAtual, PAtual, Toleranciax, NULL, NULL, pSuperfTerrap);

    if (ListaRetClicada != pSuperficieAtual->PegaListaRestricoes().end()) PegouRestricao = true;
    else ++ItLRestricaoAtual;
  }

  return ListaRetClicada;
}

void DesPontosGeomView::DesenhaCerca(const Ponto& PInicial, const Ponto& PFinal, CDC* ppDC)
{
  //--- Determina pontos do X
  //--- Calcula Azimute
  //--- Calcula Deltas para 20 pixels
  //--- Rotaciona x
  //--- DesenhaLinha
  //--- Desenha o x

  Ponto Pontosx[5] = { Ponto(-2.0,4.0),Ponto(2.0,4.0),Ponto(-2.0,0.0),Ponto(2.0,0.0),Ponto(0.0,4.0) },       //--- Coordenadas do X
        PontoAnterior(PInicial), PontoAtual,
        CoordPolar(Mat.R_P(PFinal, PInicial)), Is((PFinal.x - PInicial.x) / CoordPolar.y, (PFinal.y - PInicial.y) / CoordPolar.y),
    Deltas(Is.x * 20.0, Is.y * 20.0);

  //@Verificar

    if(fabs(PFinal.x - PInicial.x) < 0.001) return;
    if(fabs(PFinal.y - PInicial.y) < 0.001) return;
    if(fabs(Is.x) < 0.001) return;
    if (fabs(Is.y) < 0.001) return;

  int Quadrante(CoordPolar.x < Matem::M_PI_2 ? 0 : CoordPolar.x < Matem::M_PI ? 1 : CoordPolar.x < Matem::M_PI_3_2 ? 2 : 3);

  for (register int C = 0; C < 5; C++)
  {
    RotacionaPontoPlano(sin(CoordPolar.x), cos(CoordPolar.x), 0.0, &Pontosx[C].x, &Pontosx[C].y);
    Pontosx[C].y *= -1.0;
  }

  bool Acabou(false);

  do
  {
    PontoAtual.x = PontoAnterior.x + Deltas.x;
    PontoAtual.y = PontoAnterior.y + Deltas.y;

    //--- Acabou??? acerta o final

    if ((Quadrante == 0 && (PontoAtual.x < PFinal.x || PontoAtual.y < PFinal.y)) ||
        (Quadrante == 1 && (PontoAtual.x < PFinal.x || PontoAtual.y > PFinal.y)) ||
        (Quadrante == 2 && (PontoAtual.x > PFinal.x || PontoAtual.y > PFinal.y)) ||
        (Quadrante == 3 && (PontoAtual.x > PFinal.x || PontoAtual.y < PFinal.y)))
    {
      PontoAtual.x = PFinal.x;
      PontoAtual.y = PFinal.y;
      Acabou = true;
    }

    ppDC->MoveTo(PontoAnterior.x, PontoAnterior.y);
    ppDC->LineTo(PontoAtual.x, PontoAtual.y);

    PontoAnterior.x = PontoAtual.x + Pontosx[4].x;
    PontoAnterior.y = PontoAtual.y + Pontosx[4].y;

    //--- Desenha o x

    if (!Acabou)
    {
      ppDC->MoveTo(PontoAtual.x + Pontosx[0].x, PontoAtual.y + Pontosx[0].y);
      ppDC->LineTo(PontoAtual.x + Pontosx[3].x, PontoAtual.y + Pontosx[3].y);
      ppDC->MoveTo(PontoAtual.x + Pontosx[1].x, PontoAtual.y + Pontosx[1].y);
      ppDC->LineTo(PontoAtual.x + Pontosx[2].x, PontoAtual.y + Pontosx[2].y);
    }
  } while (!Acabou);
}

void DesPontosGeomView::DesenhaMuro(const Ponto& PInicial, const Ponto& PFinal, CDC* ppDC)
{
  //--- Calcula Azimute
  //--- Rotaciona pontas
  //--- DesenhaLinha 1
  //--- Desenha linha 2

  Ponto Pontosx[2] = { Ponto(-1.0,0.0),Ponto(1.0,0.0) },   //--- Coordenadas do inicio (e do fim) do muro
    CoordPolar(Mat.R_P(PFinal, PInicial)), Is((PFinal.x - PInicial.x) / CoordPolar.y, (PFinal.y - PInicial.y) / CoordPolar.y);

  //-- Rotaciona as pontas

  for (register int C = 0; C < 2; C++)
  {
    RotacionaPontoPlano(sin(CoordPolar.x), cos(CoordPolar.x), 0.0, &Pontosx[C].x, &Pontosx[C].y);
    Pontosx[C].y *= -1.0;
  }

  //--- Desenha as linhas

  for (register int C = 0; C < 2; C++)
  {
    ppDC->MoveTo(PInicial.x + Pontosx[C].x, PInicial.y + Pontosx[C].y);
    ppDC->LineTo(PFinal.x + Pontosx[C].x, PFinal.y + Pontosx[C].y);
  }
}

void DesPontosGeomView::DesenhaLinhaFerrea(const Ponto& PInicial, const Ponto& PFinal, CDC* ppDC)
{
  //--- Determina pontos das linhas e dos traços
  //--- Calcula Azimute
  //--- Calcula Deltas para 6 pixels
  //--- Rotaciona Pontos
  //--- DesenhaLinha
  //--- Desenha os traços

  Ponto Pontosx[5] = { Ponto(-1.0,0.0),Ponto(1.0,0.0),Ponto(-6.0,6.0),Ponto(7.0,6.0),Ponto(0.0,9.0) },
    PontoAtual(PInicial), CoordPolar(Mat.R_P(PFinal, PInicial)), Is((PFinal.x - PInicial.x) / CoordPolar.y, (PFinal.y - PInicial.y) / CoordPolar.y);

  int Quadrante(CoordPolar.x < Matem::M_PI_2 ? 0 : CoordPolar.x < Matem::M_PI ? 1 : CoordPolar.x < Matem::M_PI_3_2 ? 2 : 3);

  for (register int C = 0; C < 5; C++)
  {
    RotacionaPontoPlano(sin(CoordPolar.x), cos(CoordPolar.x), 0.0, &Pontosx[C].x, &Pontosx[C].y);
    Pontosx[C].y *= -1.0;
  }

  //--- Desenha as 2 linhas

  for (register int C = 0; C < 2; C++)
  {
    ppDC->MoveTo(PInicial.x + Pontosx[C].x, PInicial.y + Pontosx[C].y);
    ppDC->LineTo(PFinal.x + Pontosx[C].x, PFinal.y + Pontosx[C].y);
  }

  //--- Desenha os traços

  do
  {
    ppDC->MoveTo(PontoAtual.x + Pontosx[2].x, PontoAtual.y + Pontosx[2].y);
    ppDC->LineTo(PontoAtual.x + Pontosx[3].x, PontoAtual.y + Pontosx[3].y);

    PontoAtual.x += Pontosx[4].x;
    PontoAtual.y += Pontosx[4].y;

  } while (!((Quadrante == 0 && (PontoAtual.x < PFinal.x || PontoAtual.y < PFinal.y)) ||
    (Quadrante == 1 && (PontoAtual.x < PFinal.x || PontoAtual.y > PFinal.y)) ||
    (Quadrante == 2 && (PontoAtual.x > PFinal.x || PontoAtual.y > PFinal.y)) ||
    (Quadrante == 3 && (PontoAtual.x > PFinal.x || PontoAtual.y < PFinal.y))));
}
/*
double DesPontosGeomView::CalculaAreaSuperficie()
{
  double Area(0.0);

  for (ItCLLDeltaSup LDRestricaoAtual = Superficie.PegaListaRestricoes().begin(); LDRestricaoAtual != Superficie.PegaListaRestricoes().end(); ++LDRestricaoAtual)
  {
    register ItCLDeltaSup ItLRestricaoAnterior((*LDRestricaoAtual).begin()),ItLRestricaoAtual(ItLRestricaoAnterior);
    unsigned int TipoRestricao(Superficie.VerfAciTopografico(LDRestricaoAtual));

    if (TipoRestricao == CAciTopografico::FRONTEIRA)
    {
      for (++ItLRestricaoAtual; ItLRestricaoAtual  != (*LDRestricaoAtual).end(); ++ItLRestricaoAnterior, ++ItLRestricaoAtual)
      {
        Area += (ItLRestricaoAnterior->PCentral.y + ItLRestricaoAtual->PCentral.y) * (ItLRestricaoAtual->PCentral.x - ItLRestricaoAnterior->PCentral.x) / 2.0;
      }
    }
    break;
  }

  return Area;
}
*/
bool DesPontosGeomView::VerfMudancaTipoAcitopog(ItCLLDeltaSup& LLDS, int Tipo)
{
  ItCLDeltaSup ItCLDSAnterior(LLDS->begin()), ItCLDSAtual(ItCLDSAnterior);

  for (++ItCLDSAtual; ItCLDSAtual != LLDS->end(); ++ItCLDSAtual, ++ItCLDSAnterior)
  {
    if (Superficie.VerificaIntersecaoAcidentesTopog(ItCLDSAnterior->PegaPCentral(), ItCLDSAtual->PegaPCentral(), PontoIntersecao, Tipo) == true)
    {
      CircularPontoIntersecao = true;

      return false;
    }
  }

  return true;
}

void DesPontosGeomView::VisualizarLogFronteira()
{
  ((CMainFrame*)AfxGetMainWnd())->asdfg = IDD_REL_LOG_FRONTEIRA;
  ((CEstApp*)AfxGetApp())->AbreArqCriaDoc("_Log.rel", false);
}

void DesPontosGeomView::OnUpdateCalcgeometrico(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(AlterouProjeto);
}

void DesPontosGeomView::OnTimer(UINT_PTR nIDEvent)
{
  if (nIDEvent != EVENT_MOUSE_STOP) return;

  static CPoint PMouseAnterior(0, 0);
  static bool MOUSE_PAROU(0);
  POINT pMouse = { 0,0 };
  GetCursorPos(&pMouse);

  //--- força o redesenho da janela, se o mouse parou;

 // if(PMouseAnterior == PonMousePixelsAtual)     //--- Se estes dois pontos são iguais o mouse parou
  if (PMouseAnterior == pMouse)                    //--- Se estes dois pontos são iguais o mouse parou
  {
    //--- Se o mouse parou faz o redesenha a janela na última posição. 

    if (!MOUSE_PAROU)
    {
      RedrawWindow();
      //     SendMessage(WM_MOUSEMOVE,0,(PonMousePixelsAtual.y << 16) + PonMousePixelsAtual.x);
        //   SendMessage(WM_MOUSEMOVE,0,(pMouse.y << 16) + pMouse.x);
      MOUSE_PAROU = PrimeiroMovimento = true;

      //    if(Arrastando) DPopUpPonto.Invalidate();
      //DPopUpPonto.Invalidate();

      return;
    }
  }
  else MOUSE_PAROU = false;

  // PMouseAnterior = PonMousePixelsAtual;
  PMouseAnterior = pMouse;

  CView::OnTimer(nIDEvent);
}

void DesPontosGeomView::SeparaPontosInteresse(double Px, double Py)
{
  tylstitSSuperficie LstPontosCandidatos;
  unsigned int Raio(Radar.PegaRaio());

  CRect Janela(Px - Raio, Py + Raio, Px + Raio, Py - Raio);

  const ittymapItRegAciTopograficos it(MapAciTopograficos.find(ObjetoAtual-1));

  CString CodigosObjAtuais;

  if (it != MapAciTopograficos.end() && it->second->ListaCampos.size() > 1)
  {
    CodigosObjAtuais = (++(it->second->ListaCampos.begin()))->c_str();
    static std::string Separadores(" ,;-|");
    bool Achou(true);

    tlstwstr& LstTokens(PegaTokens(CodigosObjAtuais));
    CString ChaveAtual;

    SetPontosNoRadar.clear();
    SetPontosRadarInteresse.clear();

    for (ittlstwstr itstr = LstTokens.begin(); itstr != LstTokens.end(); ++itstr)
    {
      ChaveAtual = itstr->c_str();

      ItSSuperficie ItSuperfIni(Superficie.pSuperficieAtual->upper_bound(CDeltaSuperficie(Px - Raio, Py + Raio))),
        ItSuperfFim(Superficie.pSuperficieAtual->upper_bound(CDeltaSuperficie(Px + Raio, Py - Raio)));

      for (register ItSSuperficie it = ItSuperfIni; it != ItSuperfFim; ++it)
      {
        if (it->PCentral.x > Janela.left&& it->PCentral.x < Janela.right && it->PCentral.y < Janela.top && it->PCentral.y > Janela.bottom)
        {
          LstPontosCandidatos.push_back(it);
        }
      }

      double Raio_2(Raio * Raio);

      for (register ittylstitSSuperficie itl = LstPontosCandidatos.begin(); itl != LstPontosCandidatos.end(); ++itl)
      {
        if (pow((*itl)->PCentral.x - Px, 2) + pow((*itl)->PCentral.y - Py, 2) < Raio_2)  //--- O ponto etá dentro da área do radar?
        {
          SetPontosNoRadar.insert(*itl);
          if (RadarRealcarPontosInteresse)
          {
            if ((*itl)->PCentral.Descricao.Left(10).Find(ChaveAtual) != -1)
            {
              SetPontosRadarInteresse.insert(*itl);
            }
          }
        }
      }
    }
  }
}


/*
void DesPontosGeomView::CriaMapaChaves()
{
  int Objeto(0);

  for(register POSITION Atual = ArquivoAciTopograficos.GetHeadPosition() ; Atual ; ArquivoAciTopograficos.GetNext(Atual),++Objeto)
  {
    size_t PosAnterior(0);

    static std::string Separadores(" ,;-|");
    CString DescricaoAtual(ArquivoAciTopograficos.GetAt(Atual).PegaCodigo().c_str());

    for(size_t PosAtual = 0 ; PosAtual  != -1 && PosAtual < DescricaoAtual.GetLength() ; )
    {
      CString CodigoAtual;
      std::string::iterator itstr(Separadores.begin());
      size_t PosChave(std::string::npos);

      while(itstr!= Separadores.end() && PosChave == std::string::npos)
      {
        char ch(*itstr);

        PosChave = DescricaoAtual.Find(ch,(int)PosAnterior);
        ++itstr;
      }

      PosAtual = PosChave;

      if(PosAtual != -1) CodigoAtual = DescricaoAtual.Mid((int)PosAnterior,(int)(PosAtual-PosAnterior));
      else  CodigoAtual = DescricaoAtual.Mid((int)PosAnterior);

      MapaChavesAciTopograficos.insert(pairstrint(CodigoAtual,Objeto));

      PosAnterior = PosAtual+1;
    }
  }
}
*/
tlstwstr& DesPontosGeomView::PegaTokens(CString& Descricao)
{
  static tlstwstr Resposta;
  static std::string Separadores(" ,;-|");
  size_t PosAnterior(0);

  Resposta.clear();

  for (size_t PosAtual = 0; PosAtual != -1 && PosAtual < Descricao.GetLength(); )
  {
    CString CodigoAtual;
    std::string::iterator itstr(Separadores.begin());
    size_t PosChave(std::string::npos);

    while (itstr != Separadores.end() && PosChave == std::string::npos)
    {
      char ch(*itstr);

      PosChave = Descricao.Find(ch, (int)PosAnterior);
      ++itstr;
    }

    PosAtual = PosChave;

    if (PosAtual != -1) CodigoAtual = Descricao.Mid((int)PosAnterior, (int)(PosAtual - PosAnterior));
    else  CodigoAtual = Descricao.Mid((int)PosAnterior);

    Resposta.push_back(CodigoAtual.GetBuffer());

    PosAnterior = PosAtual + 1;
  }
  return Resposta;
}


/*
void DesPontosGeomView::CriaMapaChaves()
{
  for(ittysetRegAciTopograficos it = SetAciTopograficos.begin() ; it != SetAciTopograficos.end() ; ++it)
  {
    int PosAnterior(0);

    static std::string Separadores(" ,;-|");
    CString DescricaoAtual(it->PegaCodigo().c_str());

    for(int PosAtual = 0 ; PosAtual  != -1 && PosAtual < DescricaoAtual.GetLength() ; )
    {
      CString CodigoAtual;
      std::string::iterator itstr(Separadores.begin());
      int PosChave(std::string::npos);

      while(itstr!= Separadores.end() && PosChave == std::string::npos)
      {
        char ch(*itstr);

        PosChave = DescricaoAtual.Find(ch,PosAnterior);
        ++itstr;
      }

      PosAtual = PosChave;

      if(PosAtual != -1) CodigoAtual = DescricaoAtual.Mid(PosAnterior,PosAtual-PosAnterior);
      else  CodigoAtual = DescricaoAtual.Mid(PosAnterior);

      MapaChavesAciTopograficos.insert(pairstrittysetRegAciTopograficos(CodigoAtual,it));

      PosAnterior = PosAtual+1;
    }
  }
}
*/
/*
#include <sstream>

void DesPontosGeomView::Calcula()
{
   int n;
    std::cin >> n;
    std::vector<int> a(n);

    for(int a_i = 0;a_i < n;a_i++)
    {
       std::cin >> a[a_i];

       std::sort(a.begin(),a.begin()+(a_i+1));

       double Result;
       int Median((a_i+1)/2);

        if((a_i) % 2 == 1)
        {
          Result = double (a[Median-1] + a[Median]) / 2.0;
        }
        else
        {
         Result = a[Median];
        }

        std::stringstream strResult;

        strResult.precision(1);

        strResult << std::fixed;

        strResult << Result << std::endl;

        std::cout << strResult.str();
    }
}


bool DesPontosGeomView::is_balanced(std::string expression)
{
    std::stack<char> stk;
    std::string strAbre("({[");
    std::string strFecha(")}]");

    for(std::string::iterator it = expression.begin() ; it != expression.end() ; it++)
    {
       if(strFecha.find(*it) != -1)
       {
         if(stk.size() == 0) return false;

         char carac(stk.top());

         if(strAbre.find(carac) != strFecha.find(*it)) return false;
         else stk.pop();
       }
       else
       {
         stk.push(*it);
       }
    }

    return stk.size() == 0;

}


//check arvore binaria

/*
bool checkBST(Node* root)
   {
       if (root != NULL)
       {
         checkBST(root->left);
         if(root->left != NULL)
         {
           if(root->data < root->left->data)   return false;
         }
         if(root->right != NULL)
         {
           if(root->data > root->right->data)   return false;
         }
         checkBST(root->right);
       }
    return true;
   }
  */
  /*
  void DesPontosGeomView::Bits()
  {
    struct list_node* Atual=HEAD->next,*Prev = HEAD;

      HEAD->next = NULL;

      for(; Atual != NULL ; Atual = Atual->next)
      {
        Atual->next = Prev;
        Prev = Atual;
      }

      HEAD = Prev;

      for(Atual = HEAD ; Atual != NULL ; Atual = Atual->next)
      {
        int Num =*((int*) Atual->data);

        printf("%d,",Num);
      }
  }



  //===========================
    int ff = 10;

    int* yy = &ff;
    char Resposta[100];

    int ggg = *yy;

    sprintf(Resposta,"%d,",*yy);

    char Resp[9] = {0},
         Zero[2] = {'0',0},
         Um[2]= {'1',0};

    int byte = 2;

    for(int i = 7 ; i >= 0 ; i--)
    {
      int kkk = 1 << i;

      int fff = byte & (1<<i);
      if((byte & (1<<i)) > 0)
      {
        strcat(Resp,Um);
      }
      else strcat(Resp,Zero);
    }

    Resp[8] = 0;

    sprintf("%s",Resp);
  }
  //================

  /*
  for(register ittylstitSSuperficie itl = LstPontosCandidatos.begin() ; itl != LstPontosCandidatos.end() ; ++itl)
    {
    if(pow((*itl)->PCentral.x-Px,2) + pow((*itl)->PCentral.y-Py,2) < Raio)
      {
         if((*itl)->TipoObjeto == ObjetoAtual) SetPontosRadar.insert(*itl);
      }
    }

  */

bool DesPontosGeomView::OnMouseMoveTerrap(Ponto& PSobMouse, POINT& PonScreenCoor, CPoint& point)
{
  //--- Se o mouse estiver sob0re algum ponto de terreno mostra as coordenadas do ponto.
  //--- Caso contrário, limpa o dialogo do ponto , caso ele não esteja limpo.

  ItSSuperficie* pDeltaSupSobMouseTerr(PegaPontoMaisProximoTerrap(PSobMouse));

  if (pDeltaSupSobMouseTerr)
  {
    const Ponto& PonSobMouse((*pDeltaSupSobMouseTerr)->PegaPCentral());

    static CPoint PMouseImaAnterior(-1, -1);

    if (!PegouPonto)
    {
      /*
      if (abs(PMouseImaAnterior.x - point.x) > 6 && abs(PMouseImaAnterior.y - point.y) > 6)
      {
        Ima(nFlags, PonSobMouse);
        PMouseImaAnterior = point;
        PegouPonto = true;
      }
      */
    }
    else
    {
      if (abs(PMouseImaAnterior.x - point.x) < 5 && abs(PMouseImaAnterior.y - point.y) < 5)
      {
        // Ima(nFlags,PonSobMouse);
        return true;
      }
      else
      {
        PegouPonto = false;
        PMouseImaAnterior.x = -1;
        PMouseImaAnterior.y = -1;
      }
    }

    //  if (!MostrarRadar || RadarTodosPontos || (RadarPontosInteresse && SetPontosRadarInteresse.find(DeltaSupSobMouse) != SetPontosRadarInteresse.end()))
    {
      //    if (DeltaSupAnterior == Superficie.pSuperficieAtual->end() || DeltaSupAnterior != DeltaSupSobMouse)
      {
        if (Superficie.EstaRotacionada())
        {
          Ponto PontoDesrotacionado(PonSobMouse);

          PontoDesrotacionado.RotacionaPlano(sin(-Superficie.PegaAnguloRotacao()), cos(-Superficie.PegaAnguloRotacao()), -Superficie.PegaAnguloRotacao());
          DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&PontoDesrotacionado, false, PegaTipoPonto(PonSobMouse, true)));
        }
        else
        {
          DPopUpPonto.MostraDescricao(PonScreenCoor.x, PonScreenCoor.y, PreparaDescricao(&PonSobMouse, false, PegaTipoPonto(PonSobMouse, true)));

          if (PonSobMouse.Descricao.Compare("GRADIE") == 0)
          {
            DesSecaoView* pDesSecao((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->PegaDesenhoSecoes());

            if (SincronizarSecoes && pDesSecao) pDesSecao->Posiciona(PonSobMouse.S);
          }
        }
      }
    }
    return true;
  }
  else
  {
    DeltaSupAnterior = Superficie.pSuperficieAtual->end();        //--- Se saiu do ponto limpa o anterior, para se voltar nele mostrar o dialogo novamente
    PegouPonto = false;
  }

  return false;
}

ItSSuperficie* DesPontosGeomView::PegaPontoMaisProximoTerrap(Ponto PMouse)
{
  static ItSSuperficie itPonMaisProximo;
  double MenorDistancia(INFINITO);

  // for (ittymapCTrechos it = Trechos.MapaTrechos.begin(); it != Trechos.MapaTrechos.end(); it++)
  ittymapCTrechos it(Trechos.PegaTrechoAtual());
  if (it != Trechos.MapaTrechos.end())
  {
    PMouse.x += Toleranciax;

    ItSSuperficie itDSupFinal(it->second.SuperficieTerr.pSuperficieAtual->upper_bound(PMouse)),
      itDSupAtual(it->second.SuperficieTerr.pSuperficieAtual->end());

    itPonMaisProximo = it->second.SuperficieTerr.pSuperficieAtual->end();

    PMouse.x -= Toleranciax * 2.0;
    itDSupAtual = it->second.SuperficieTerr.pSuperficieAtual->upper_bound(PMouse);         //--- Pega o primeiro na tolerancia    
    PMouse.x += Toleranciax;                          //--- Restaura o valor original

    MenorDistancia = INFINITO;

    //--- Separa o ponto que está mais próximo do mouse 

    while (itDSupAtual != itDSupFinal)
    {
      Ponto PAtual((*itDSupAtual).PegaPCentral());
      double DistanciaAtual(hypot(PAtual.x - PMouse.x, PAtual.y - PMouse.y));
      if (DistanciaAtual < MenorDistancia)
      {
        MenorDistancia = DistanciaAtual;
        itPonMaisProximo = itDSupAtual;
      }

      itDSupAtual++;
    }

    //--- Separa a menor distancia entre os 2 set e retorna se ele estiver dentro da tolerancia 
    //--- caso contrário não há ponto mais próximo dentro da tolerancia
    return MenorDistancia < Toleranciax ? &itPonMaisProximo : nullptr;
  }

  return nullptr;
}

bool DesPontosGeomView::OnLButtonDownTerr(CPoint& point)
{
  Ponto PAtual;

  TransfPixelsXY(point, PAtual.x, PAtual.y);

  for (ittymapCTrechos it = Trechos.MapaTrechos.begin(); it != Trechos.MapaTrechos.end(); it++)
  {
    ItCLLDeltaSup ItLRestricaoAtual(VerfClickRestricao(PAtual, &it->second.SuperficieTerr));

    if (ItLRestricaoAtual != it->second.SuperficieTerr.PegaListaRestricoes().end())
    {
      unsigned int TipoRestricao(it->second.SuperficieTerr.VerfAciTopografico(ItLRestricaoAtual));

      CPoint PAux(point);
      ClientToScreen(&PAux);

      DPopUpPonto.MostraDescricao(PAux.x, PAux.y - 60, PreparaDescricao((void*)CAciTopografico::Nome(TipoRestricao), false, OBJ_TEXTO));

      return true;
    }
  }
  return false;
}

void DesPontosGeomView::LeArqTPR(CString& NomeProj)
{
  CFileException e;
  CFile Arq;

  //--- Le este arquivo quando não há superficie de terraplanagem,
  //--- Se houve erros no cálculo da terraplanagem, este aquivos contém os alinhamentos da
  //--- Terraplanagem que podem ajudar o usuário a visualizar o erro.
  //--- Se a terraplanagem ainda não foi calculada, tudo bem, nada será lido
  //-- e a lista de alinhamentos ficará vazia

  std::string strNomeArq(NomeProj + ".tpr");
  unsigned int Flags(CFile::modeRead);

  if (Arq.Open(strNomeArq.c_str(), Flags, &e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound)
    {
      //--- Ok, o arquivo ainda não foi gerado     

      return;
    }
  }
  else
  {
    CArchive CArcAlinhamentos(&Arq, CArchive::load);
    size_t QRestricoes(0);
    double x1(INFINITO), y1(INFINITO), x2(INFINITO), y2(INFINITO), z(INFINITO), i(INFINITO), fi(INFINITO), S(INFINITO), Sin(0.0), Cos(0.0);

    size_t QuanAlinhamentos(0), QuanPontos(0);
    bool GeraArestaForcada(true);             //--- Todos os alinhamentos geram arestas forçadas,exceto a fronteira por enquanto... 
    unsigned int TipoAciTopog(0);

    for (CArcAlinhamentos >> QuanAlinhamentos; QuanAlinhamentos; --QuanAlinhamentos)
    {
      double Estaca, x;
      std::string Descricao;

      CArcAlinhamentos >> Estaca >> x;
      CArcAlinhamentos >> Descricao;

      //--- Pega o primeiro ponto

      CString CSNome, CSDescricao;
      int AcidenteTopografico,Baliza2;
      bool Baliza, ERestricao;

      CArcAlinhamentos >> QuanPontos >> CSNome >> CSDescricao >> x1 >> y1 >> z >> i >> fi >> S >> Baliza >> Baliza2 >> ERestricao >> AcidenteTopografico;

      if (QuanPontos < 2) continue;

      AlinhamentosTerrapl.emplace_back(LDeltaSup());

      AlinhamentosTerrapl.rbegin()->emplace_back(CDeltaSuperficie(x1, y1, z, i, fi, S, CSDescricao, CDeltaSuperficie::HABILITADO, AcidenteTopografico, CSNome));

      for (--QuanPontos; QuanPontos; --QuanPontos)       //--- Para todos os pontos da restrição atual
      {
        CArcAlinhamentos >> CSNome >> CSDescricao >> x2 >> y2 >> z >> i >> fi >> S >> Baliza >> Baliza2 >> ERestricao >> AcidenteTopografico;

        AlinhamentosTerrapl.rbegin()->emplace_back(CDeltaSuperficie(x2, y2, z, i, fi, S, CSDescricao, CDeltaSuperficie::HABILITADO, AcidenteTopografico, CSNome));
      }
    }
  }
}

void DesPontosGeomView::DesenhaAlinhamentosTerrapl(CDC* pDC)
{
  CPen Pena, * PAnterior;

  Pena.CreatePen(PS_DASH, 1, Cores[CORCN]);

  PAnterior = pDC->SelectObject(&Pena);
  int C = 0;

  for (ItLLDeltaSup itAlinhamentos = AlinhamentosTerrapl.begin(); itAlinhamentos != AlinhamentosTerrapl.end(); itAlinhamentos++)
  {
    if (itAlinhamentos->size() > 1)
    {
      ItLDeltaSup itDSAtual(itAlinhamentos->begin());

      pDC->MoveTo((itDSAtual->PCentral.x - Deltas[X]) * Escala[X], (AlturaVideo - (itDSAtual->PCentral.y - Deltas[Y])) * Escala[Y]);

      while (++itDSAtual != itAlinhamentos->end())
      {
        Ponto Ppixels;

        Ppixels.x = ((itDSAtual->PCentral.x - Deltas[X]) * Escala[X]);
        Ppixels.y = (AlturaVideo - (itDSAtual->PCentral.y - Deltas[Y])) * Escala[Y];

        pDC->LineTo(Ppixels.x, Ppixels.y);

        DesenharPonto(Ppixels.x, Ppixels.y, pDC, Cores[CORPONTOS]);
      }
    }
  }
}

inline void DesPontosGeomView::PulaBrancos(std::ifstream& Stream)
{
  register char Carac(0);
  do
  {
    Carac = Stream.get();
  } while ((Carac == ' ' && Carac != '\x10') && !Stream.eof());

  if (Carac != ' ')   Stream.putback(Carac);
}

void  DesPontosGeomView::GetLine(std::ifstream& ifs, std::string& Buffer, const std::string& strDelim)
{
  Buffer.clear();
  char Carac;

  if (!ifs.eof())
  {
    do
    {
      Carac = ifs.get();

      if (Carac == -1)    //--- não esta setando eof
      {
        //--- ifs.eof não funciona!!! porisso estas 3 linhas---  MSVC2017 na msvc2010 funciona

        std::ios::iostate iost(ifs.rdstate());
        iost |= std::ios::eofbit;
        ifs.setstate(iost);

        //-----------------------------------------------------------
      }

      Buffer.push_back(Carac);

    } while (strDelim.find(*Buffer.rbegin()) == std::string::npos && !ifs.eof());  //---- O windows não esta setando eof, carac vem com -1 neste caso

    if (!ifs.eof()) ifs.putback(*Buffer.rbegin());
    Buffer.pop_back();
  }
}

inline void DesPontosGeomView::PegaProximoAlfaNum(std::ifstream& Stream)
{
  do
  {
    register char Carac(Stream.get());

    //  if (iswalnum(Carac) && Carac != ' ')
    if ((iswalnum(Carac) && Carac != ' ') || Carac == '-' || Carac == '+')
    {
      Stream.putback(Carac);
      return;
    }
  } while (!Stream.eof());
}

void DesPontosGeomView::LeArquivosBordos()
{
  CString NomeArquivoSup(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase())),
    NomeArquivoTer(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()));

  for (int i = 0; i < 2; i++)
  {
    CFileException e;
    CString NomeArqAtual[2] = { _T("C:\\Temp\\Exemplo102Atual.res"),
                                _T("C:\\Temp\\Exemplo102Ante.res") };

    std::ofstream ArqLog(NomeArqAtual[i] + ".COMP", std::ofstream::out | std::ofstream::trunc);
    if (ArqLog.good() == false) ASSERT(0);
    ArqLog << std::right << std::fixed << std::setfill('0') << std::setprecision(15);

    CFile Arq;

    if (Arq.Open(NomeArqAtual[i], CFile::modeRead, &e) == 0)
    {
      if (e.m_cause != CFileException::fileNotFound)
      {
        continue;
      }
    }
    else
    {
      CArchive CArcRestricoes(&Arq, CArchive::load);
      double AngRotacaoBordos;

      if (e.m_cause != CFileException::fileNotFound)
      {
        lstPontos ListaAtual;

        CArcRestricoes >> AngRotacaoBordos;

        ArqLog << AngRotacaoBordos << std::endl;

        size_t QRestricoes(0);

        CArcRestricoes >> QRestricoes;

        ArqLog << QRestricoes << std::endl;

        for (; QRestricoes; QRestricoes--)
        {
          size_t QPontos(0);
          unsigned int Tipo(0);

          CArcRestricoes >> Tipo;

          try
          {
            Ponto PAtual;

            CArcRestricoes >> QPontos;

            ArqLog << QPontos << std::endl;

            for (; QPontos; QPontos--)       //--- Para todos os pontos da restrição atual
            {
              PAtual << CArcRestricoes;
              ListaAtual.push_back(PAtual);
            }
          }
          catch (CArchiveException * e)
          {
            if (i == 0) monolog.mensagem(-1,"Houve erro na leitura do arquivo de restrições.");
            else  monolog.mensagem(-1,"Houve erro na leitura do arquivo de bordos.");

            e->Delete();

            continue;
          }
        }

        std::strstream strstreamTemp;

        ArqLog << NomeArqAtual[i].GetBuffer() << std::endl;

        ArqLog << AngRotacaoBordos << std::endl;

        for (itlstPontos it = ListaAtual.begin(); it != ListaAtual.end(); it++)
        {
          ArqLog << it->x << " " << it->y << " " << it->z << std::endl;
          ArqLog.flush();
        }
      }
    }
  }
}

bool DesPontosGeomView::LeCotasVermelhas()
{
  CFileException e;
  CFile ArqPonTerPro;

  //--- Le o arquivo de pontos de terraplanagem só para pegar as cotas vermelhas

  CString NomeProjeto(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()));

  if (ArqPonTerPro.Open(std::string(NomeProjeto + ".ptp").c_str(), CFile::modeRead | CFile::shareDenyNone, &e) == 0)   //--- Pontos Terraplanagem Projeto
  {
    return false;
  }

  CArchive CArqPonTerPro(&ArqPonTerPro, CArchive::load);

  SetCotasVermelhas.clear();
  size_t QuanSecoes(0);
  Ponto PontoAtual;

  for (CArqPonTerPro >> QuanSecoes; QuanSecoes; QuanSecoes--)
  {
    PontoAtual << CArqPonTerPro;

    SetCotasVermelhas.emplace(PontoAtual);

    size_t QuanPontos(0);

    for (CArqPonTerPro >> QuanPontos; QuanPontos; QuanPontos--)
    {
      PontoAtual << CArqPonTerPro;      //--- Os pontos nao interessam.
    }
  }

  return true;
}

void DesPontosGeomView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
  CView::OnActivate(nState, pWndOther, bMinimized);

  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());

  MainFrame->DesPontosGeo = this;
}

void DesPontosGeomView::OnShowWindow(BOOL bShow, UINT nStatus)
{
  CView::OnShowWindow(bShow, nStatus);

  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());

  MainFrame->DesPontosGeo = this;
}

void DesPontosGeomView::DesenhaCN(CSuperficie* pSuperficie, CDC* pDC,bool Simbologia)
{
  if (DesenharCN)
  {
    double MultCotaMestra(pSuperficie->PegaEquidisCN() * 5.0);
    Ponto Ppixels;

    COLORREF CorCNMestra(RGB(GetRValue(Cores[CORCN]) - 72, GetGValue(Cores[CORCN]) - 72, GetBValue(Cores[CORCN])));

    CPen PenaCNNormal, PenaCotaMestra,PenaVerde;
    PenaCNNormal.CreatePen(PS_SOLID, 1, Cores[CORCN]);
    PenaCotaMestra.CreatePen(PS_SOLID, RealcarCNsMestras ? 2 : 1,CorCNMestra);
    PenaVerde.CreatePen(PS_SOLID, 1, RGB(20,200,20));

    LLPontos& LLCN(MostrarTerrap && !DesenharCNTaludes ? pSuperficie->LLPontosCNSimbTerrapl : pSuperficie->LPontosCN);

    if (DesCNTopograficas)
    {
      for (ItLLPontos itLLCN = LLCN.begin(); itLLCN != LLCN.end(); itLLCN++)
      {
        if (itLLCN->begin()->z != INFINITO && SaltarCNs && fabs(fmod(itLLCN->begin()->z, (SaltarCNs + 1) * Superficie.PegaEquidisCN()))) continue;

        if (itLLCN->begin()->z < AbaixoDaCota || itLLCN->begin()->z > AcimaDaCota) continue;

        int Mod4(0);
        POINT PBezier[4];

        if (fabs(fmod(itLLCN->begin()->z, MultCotaMestra)) < 0.0005) pDC->SelectObject(PenaCotaMestra);
        else pDC->SelectObject(PenaCNNormal);

        if (CotandoCNDGN && itLLCN->begin()->z == INFINITO)
          pDC->SelectObject(PenaVerde);

        for (register ItLPontos itLCN = itLLCN->begin(); itLCN != itLLCN->end(); itLCN++)
        {
          Ppixels.x = ((itLCN->x - Deltas[X]) * Escala[X]);
          Ppixels.y = (AlturaVideo - (itLCN->y - Deltas[Y])) * Escala[Y];

          if (itLLCN->size() == 2 && itLCN == itLLCN->begin()) pDC->MoveTo(Ppixels.x, Ppixels.y);

          //--- pDC->SetPixel(Ppixels.x,Ppixels.y,Cores[CORBORDOS]);   	//--- Pixel da interseção

         // pDC->Arc(Ppixels.x - 3, Ppixels.y - 3, Ppixels.x + 3, Ppixels.y + 3, 0, 0, 0, 0);

          PBezier[Mod4].x = (int)Ppixels.x;
          PBezier[Mod4].y = (int)Ppixels.y;

          if (Mod4 == 3)
          {
            pDC->PolyBezier(PBezier, 4);

            PBezier[0].x = Ppixels.x;
            PBezier[0].y = Ppixels.y;

            pDC->MoveTo(Ppixels.x, Ppixels.y);

            Mod4 = 0;
          }
          Mod4++;
        }

        switch(Mod4)
        {
          case 2:
          {
           pDC->LineTo(PBezier[1].x, PBezier[1].y);
          }
          break;
          case 3:
          {
            pDC->MoveTo(PBezier[0].x, PBezier[0].y);
            pDC->LineTo(PBezier[1].x, PBezier[1].y);
            pDC->LineTo(PBezier[2].x, PBezier[2].y);
          }
          break;
        }

        if (fabs(fmod(itLLCN->begin()->z, MultCotaMestra)) < 0.0005) 
        {
          if (CotarCN)
          {
            CotaCN((++(itLLCN->begin()))->fi, *(itLLCN->begin()), true, itLLCN->begin()->z, pDC, PenaCotaMestra);
            CotaCN((++(itLLCN->begin()))->fi, *(itLLCN->rbegin()), true, itLLCN->rbegin()->z, pDC, PenaCotaMestra);
          }

          pDC->SelectObject(PenaCotaMestra);
        }
      }
    }
    else
    {
      for (ItLLPontos itLLCN = LLCN.begin(); itLLCN != LLCN.end(); itLLCN++)
      {
        if (SaltarCNs && fabs(fmod(itLLCN->begin()->z, (SaltarCNs + 1) * Superficie.PegaEquidisCN()))) continue;

        if (itLLCN->begin()->z < AbaixoDaCota || itLLCN->begin()->z > AcimaDaCota) continue;

        register ItLPontos itLCN(itLLCN->begin());

        Ppixels.x = ((itLCN->x - Deltas[X]) * Escala[X]);
        Ppixels.y = (AlturaVideo - (itLCN->y - Deltas[Y])) * Escala[Y];

        pDC->MoveTo(Ppixels.x, Ppixels.y);

        if (fabs(fmod(itLLCN->begin()->z, MultCotaMestra)) < 0.0005) pDC->SelectObject(PenaCotaMestra);
        else pDC->SelectObject(PenaCNNormal);

        if (CotandoCNDGN && itLLCN->begin()->z == INFINITO)
          pDC->SelectObject(PenaVerde);

        for (itLCN++; itLCN != itLLCN->end(); itLCN++)
        {
          Ppixels.x = ((itLCN->x - Deltas[X]) * Escala[X]);
          Ppixels.y = (AlturaVideo - (itLCN->y - Deltas[Y])) * Escala[Y];

          pDC->LineTo(Ppixels.x, Ppixels.y);

        //  pDC->Arc(Ppixels.x - 3, Ppixels.y - 3, Ppixels.x + 3, Ppixels.y + 3, 0, 0, 0, 0);
        }

        if (fabs(fmod(itLLCN->begin()->z, MultCotaMestra)) < 0.0005)
        {
          if (CotarCN) 
          {
            CotaCN((++(itLLCN->begin()))->fi, *(itLLCN->begin()), true, itLLCN->begin()->z, pDC, PenaCotaMestra);
            CotaCN((++(itLLCN->begin()))->fi, *(itLLCN->rbegin()), true, itLLCN->rbegin()->z, pDC, PenaCotaMestra);
          }
        }
      }
    }
  }
}

void DesPontosGeomView::DesenhaRestricoes(CSuperficie* pSuperficieAtual, CDC* pDC, CPen* pPenaRestricoes, int TipoSuperf,bool ParaTerraplanagem)
{
  pDC->SelectObject(*pPenaRestricoes);
  const LLDeltaSup& ListaRestAtual(pSuperficieAtual->PegaListaRestricoes());

  for (ItCLLDeltaSup LDRestricaoAtual = ListaRestAtual.begin(); LDRestricaoAtual != ListaRestAtual.end(); ++LDRestricaoAtual)
  {
    Ponto Ppixels;

    register ItCLDeltaSup ItLRestricao((*LDRestricaoAtual).begin());

    unsigned int TipoRestricao(pSuperficieAtual->VerfAciTopografico(LDRestricaoAtual));

    bool Desenhar = false;

    if (TipoSuperf == SUPERF_TERRENO_NATURAL)  //--- 0 Superficie terreno 1 Terraplanagem
    {
      Desenhar = (DesenharBordos && TipoRestricao == CAciTopografico::FRONTEIRA) ||
                 (DesenharRestricoes && TipoRestricao != CAciTopografico::FRONTEIRA);
    }
    else
    {
      Desenhar = (DesenharOffSets && TipoRestricao == CAciTopografico::OFF_SETS) ||
                 (DesenharAlinTerrap && TipoRestricao != CAciTopografico::OFF_SETS) &&
                 (TipoRestricao != CAciTopografico::FRONTEIRA_TERRAP);
    }


    if (Desenhar)
    {
      if (LDRestricaoAtual->size() != 0 && (!FiltrarAciTopog || TipoRestricao == ObjetoAtual))
      {
        if (TipoRestricao != CAciTopografico::FRONTEIRA || DesenharBordos)
        {
          //--- As sarjetas são desenhadas nos equipamentos de drenagem, estes acidentes topográficos existem
          //--- apenas para orientar a construção ou edição dos equipamentos de drenagem e não são devem ser desenhados

          if(TipoRestricao < CAciTopografico::INICIO_EQUI_DRENAGEM)  //--- Nos Equipamentos de drenagem o Acidente topográfico associado não é desenhado
          {
            if (TipoRestricao != CAciTopografico::NENHUM_ACITOPOG) CriaPenaRestriçao(pPenaRestricoes, TipoRestricao);

            pDC->SelectObject(*pPenaRestricoes);

            if (TipoRestricao != CAciTopografico::POSTE && TipoRestricao != CAciTopografico::TAMPA_BUEIRO && TipoRestricao != CAciTopografico::ARVORE &&
                TipoRestricao != CAciTopografico::PVELETRICA &&TipoRestricao != CAciTopografico::PVESGOTO && TipoRestricao != CAciTopografico::PVGAS && 
                TipoRestricao != CAciTopografico::SOLEIRA)
            {
              if (TipoRestricao != CAciTopografico::CERCA && TipoRestricao != CAciTopografico::CANTO_CERCA && TipoRestricao != CAciTopografico::MURO &&
                  TipoRestricao != CAciTopografico::CANTO_MURO && TipoRestricao != CAciTopografico::LINHA_FERREA)
              {
                Ppixels.x = ((*ItLRestricao).PCentral.x - Deltas[X]) * Escala[X];
                Ppixels.y = (AlturaVideo - ((*ItLRestricao).PCentral.y - Deltas[Y])) * Escala[Y];

                pDC->MoveTo(Ppixels.x, Ppixels.y);

                if (TipoRestricao > CAciTopografico::FRONTEIRA && DesenharPontos)
                {
                  DesenharPonto(Ppixels.x, Ppixels.y, pDC, Cores[CORPONTOS]);
                }

                while (++ItLRestricao != (*LDRestricaoAtual).end())
                {

                  Ponto  PAtual((*ItLRestricao).PCentral);   //--- Pega o ponto atual

                  Ppixels.x = ((PAtual.x - Deltas[X]) * Escala[X]);
                  Ppixels.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y];

                  pDC->LineTo(Ppixels.x, Ppixels.y);

                  //DesenharPonto(Ppixels.x, Ppixels.y, pDC, Cores[CORPONTOS]);
                }
              }
              else
              {
                if ((*LDRestricaoAtual).size() > 1)
                {
                  Ponto PInicial(((*ItLRestricao).PCentral.x - Deltas[X]) * Escala[X], (AlturaVideo - ((*ItLRestricao).PCentral.y - Deltas[Y])) * Escala[Y]);

                  while (++ItLRestricao != (*LDRestricaoAtual).end())
                  {
                    Ponto PFinal(((*ItLRestricao).PCentral.x - Deltas[X]) * Escala[X], (AlturaVideo - ((*ItLRestricao).PCentral.y - Deltas[Y])) * Escala[Y]);

                    if (TipoRestricao == CAciTopografico::CERCA || TipoRestricao == CAciTopografico::CANTO_CERCA) DesenhaCerca(PInicial, PFinal, pDC);
                    else if (TipoRestricao == CAciTopografico::MURO || TipoRestricao == CAciTopografico::CANTO_MURO) DesenhaMuro(PInicial, PFinal, pDC);
                    else DesenhaLinhaFerrea(PInicial, PFinal, pDC);

                    PInicial = PFinal;
                  }
                }
              }
            }
            else
            {
              if (TipoRestricao == CAciTopografico::ARVORE)
              {
                while (ItLRestricao != (*LDRestricaoAtual).end())
                {
                  DesenhaArvore((*ItLRestricao++).PCentral, pDC);
                }
              }
              else
              {
                if (TipoRestricao == CAciTopografico::POSTE)
                {
                  while (ItLRestricao != (*LDRestricaoAtual).end())
                  {
                    DesenhaPoste((*ItLRestricao++).PCentral, pDC);
                  }
                }
                else
                {
                  if(TipoRestricao == CAciTopografico::TAMPA_BUEIRO)
                  {
                    while (ItLRestricao != (*LDRestricaoAtual).end())
                    {
                      DesenhaBueiro((*ItLRestricao++).PCentral, pPenaRestricoes, pDC);
                    }
                  }
                  else
                  {
                    if (TipoRestricao == CAciTopografico::PVELETRICA)
                    {
                      while (ItLRestricao != (*LDRestricaoAtual).end())
                      {
                        DesenhaPVEletrica((*ItLRestricao++).PCentral, pPenaRestricoes, pDC);
                      }
                    }
                    else
                    {
                      if (TipoRestricao == CAciTopografico::PVESGOTO)
                      {
                        while (ItLRestricao != (*LDRestricaoAtual).end())
                        {
                          DesenhaPVEsgoto((*ItLRestricao++).PCentral, pPenaRestricoes, pDC);
                        }
                      }
                      else
                      {
                        if (TipoRestricao == CAciTopografico::PVGAS)
                        {
                          while (ItLRestricao != (*LDRestricaoAtual).end())
                          {
                            DesenhaPVGas((*ItLRestricao++).PCentral, pPenaRestricoes, pDC);
                          }
                        }
                        else
                        {
                          if (TipoRestricao == CAciTopografico::SOLEIRA)
                          {
                            while (ItLRestricao != (*LDRestricaoAtual).end())
                            {
                              DesenhaSoleira((*ItLRestricao++).PCentral, pPenaRestricoes, pDC);
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          else
          {
            //--- Desenha apenas os pontos dos acidente topográficos associados aos equipamentos de drenagem

            if(DesenharPontos)
            {
              COLORREF CorPontosDrenagem(RGB(255, 160, 160));

              Ppixels.x = ((*ItLRestricao).PCentral.x - Deltas[X]) * Escala[X];
              Ppixels.y = (AlturaVideo - ((*ItLRestricao).PCentral.y - Deltas[Y])) * Escala[Y];

              DesenharPonto(Ppixels.x, Ppixels.y, pDC, Cores[CORPONTOS]);

              while (++ItLRestricao != (*LDRestricaoAtual).end())
              {
                Ponto  PAtual((*ItLRestricao).PCentral);   //--- Pega o ponto atual

                Ppixels.x = ((PAtual.x - Deltas[X]) * Escala[X]);
                Ppixels.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y];

                DesenharPonto(Ppixels.x, Ppixels.y, pDC,CorPontosDrenagem);
              }
            }
          }
        }
      }
    }
  }
}

void DesPontosGeomView::InsereRuaRecemCriada(const std::string& ProjBase, const std::string& NovaRua)
{
  std::string Nome(ProjBase.substr(0, ProjBase.find_last_of('\\') + 1));

  ((CMainFrame*)AfxGetMainWnd())->LigaSilencioso();
  Trechos.InsereTrechoRecemCriado(Nome + NovaRua + '\\' + NovaRua);
  ((CMainFrame*)AfxGetMainWnd())->DesligaSilencioso();
}

void DesPontosGeomView::OnSincPontosControle()
{
  SincPontosControle = true;

  ((CChildFrame*)GetParentFrame())->DeslizanteGeom.m_CEEscala.EnableWindow(false);

  if (((CMainFrame*)AfxGetMainWnd())->ProjetoNovo != 6)    //--- 6 -> QGIS   (Já leu o nome do arquivo do mapa
  {
    CString filtro = "Mapas de georef. (*.jpg) | *.jpg||", NomeProj;

    CFileDialog DiaAbreMapa(TRUE, NULL, ((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase(), OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro, NULL);
    DiaAbreMapa.m_ofn.lpstrTitle = "Vias Urbanas & Estradas - Mapas de Georeferenciamento :";

    if (DiaAbreMapa.DoModal() == IDOK)
    {
      NomeMapa = DiaAbreMapa.GetPathName();
    }
    else
    {
      //--- O Usuario desistiu

      TipoGeoref = -1;
      SincPontosControle = false;

      return;
    }
  }
  else   //--- QGIS o nome do mapa já foi lido
  {
    NomeMapa = ((CEstApp*)AfxGetApp())->PegaNomeArqMapaQGIS().c_str();
  }

  if (!NomeMapa.IsEmpty())
  {
    m_nX = m_nY = 0.0;
    DesenharMapa = true;
    Escala[X] = Escala[Y] = RazaoEscalasJPGDes = m_nRatio = 1.0;
    Deltas[X] = DeltasCN[X];
    Deltas[Y] = DeltasCN[Y];
    ((CChildFrame*)GetParentFrame())->DeslizanteGeom.MudarEscala(Escala[X]);

    m_cPicture.Load(NomeMapa);

    RedrawWindow();

    monolog.mensagem(-1, "Para concluir o georeferenciamento sincronize os pontos de controle. Para reiniciar a sincronização pressione ESC.");

    CRect WinRect(0, 0, 0, 0);
    GetWindowRect(&WinRect);
    WinRect.DeflateRect(2, 2);
    ClipCursor(WinRect);   //--- confina o cursor na área cliente (-2 pixels)
    if (pDesCroquisView) pDesCroquisView->GetParentFrame()->ShowWindow(SW_HIDE);
    TemMapa = true;
  }
}

#include "CDiaTipoGeoref.h"

void DesPontosGeomView::OnBaixarMapa()
{
#ifdef DEMOVER

  AfxMessageBox("Função indisponível na cópia de demonstração.");
  return;

#endif 

  TipoGeoref = -1;

  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());

  if (MainFrame->ProjetoNovo != -1)
  {
    switch (MainFrame->ProjetoNovo)
    {
    case 2:MainFrame->TipoSuperficie = SUPERF_DRONES; break;
    case 3:MainFrame->TipoSuperficie = SUPERF_GERENMAPAS; break;
    case 5:MainFrame->TipoSuperficie = SUPERF_GOOGLEEARTH; break;
    case 6:MainFrame->TipoSuperficie = SUPERF_SIG; break;
    }
  }

  OnSincPontosControle();
}

#include "CConfigProj.h"

int DesPontosGeomView::CalculaUTMpRef(int Origem)
{
  tylstUtm_Coord_3d LstPontosGeoRef;
  Utm_Coord_3d PontosControleUTM[4];
  SSetPontos& SetPontosSoltos(Superficie.PegaPontosSoltosMapa());

  std::string NomeProjetoBasico(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase());

  CMainFrame* pMainFrame((CMainFrame*)AfxGetMainWnd());

  TipoGeoref = ((CMainFrame*)AfxGetMainWnd())->ProjetoNovo;

  switch(TipoGeoref)
  {
    case 5:
    {
      CDiaImportarArqKmlKmz DialogoArqGoogle(this, NomeProjetoBasico, true, &LstPontosGeoRef, 1);
      if (DialogoArqGoogle.DoModal() == IDCANCEL) return -1;
    }
    break;
    case 2:
    {
      CDiaImportarArqCtrlDrones DialogoArqDrones(SetPontosSoltos,this, NomeProjetoBasico, true, &LstPontosGeoRef, TipoGeoref,EquidisCN);
      if (DialogoArqDrones.DoModal() == IDCANCEL) return -1;
    }
    break;
    case 3:
    {
      CDiaImportarArqCtrlDrones DialogoArqDrones(SetPontosSoltos,this, NomeProjetoBasico, true, &LstPontosGeoRef, TipoGeoref,EquidisCN);
      if (DialogoArqDrones.DoModal() == IDCANCEL) return -1;
    }
    break;
    case 6:    //--- QGIS -> Já leu o nome de todos os arquivos
    {
      LeArqPCTRL(NomeProjetoBasico,&LstPontosGeoRef);
    }
    break;
  }

  //--- Calcula a escala do mapa

  if (TemMapa)
  {
    int C(0);

    if (LstPontosGeoRef.size() > 3)
    {
      CConfigProj ConfigProjeto(NULL, NomeProjetoBasico.c_str());
      ConfigProjeto.LeArquivo();

      for (ittylstUtm_Coord_3d it = LstPontosGeoRef.begin(); it != LstPontosGeoRef.end(); it++)
      {
        std::string Obs(it->Observacoes);

        std::transform(Obs.begin(), Obs.end(), Obs.begin(), ::toupper);

        if (C < 4 && Obs.find("PCTRL", 0) != std::string::npos)
        {
          PontosControleUTM[C++] = *it;
        }
      }

      if (C == 4)
      {
        //---  Calcula a razão entre a descala do mapa e a escala do desenho

        double
          Dx1p(hypot(PontosControleGeoRef[1].x - PontosControleGeoRef[0].x, PontosControleGeoRef[1].y - PontosControleGeoRef[0].y)),
          Dx2p(hypot(PontosControleGeoRef[2].x - PontosControleGeoRef[1].x, PontosControleGeoRef[2].y - PontosControleGeoRef[1].y)),
          Dy3p(hypot(PontosControleGeoRef[3].x - PontosControleGeoRef[2].x, PontosControleGeoRef[3].y - PontosControleGeoRef[2].y)),
          Dy4p(hypot(PontosControleGeoRef[3].x - PontosControleGeoRef[0].x, PontosControleGeoRef[3].y - PontosControleGeoRef[0].y)),

          Dx1m(hypot(PontosControleUTM[1].x - PontosControleUTM[0].x, PontosControleUTM[1].y - PontosControleUTM[0].y)),
          Dx2m(hypot(PontosControleUTM[2].x - PontosControleUTM[1].x, PontosControleUTM[2].y - PontosControleUTM[1].y)),
          Dy3m(hypot(PontosControleUTM[3].x - PontosControleUTM[2].x, PontosControleUTM[3].y - PontosControleUTM[2].y)),
          Dy4m(hypot(PontosControleUTM[3].x - PontosControleUTM[0].x, PontosControleUTM[3].y - PontosControleUTM[0].y)),

          Mediap(Dx1p + Dx2p + Dy3p + Dy4p),
          Mediamt(Dx1m + Dx2m + Dy3m + Dy4m);

        Escala[X] = Escala[Y] = RazaoEscalasJPGDes = (Mediap / Mediamt);

        CalculaParametros();

        //--- Deltax, inicio do video a esquerda,deltay fim do video em baixo.
        //--- Inicialmente o mapa fica no canto esquerdo superior do vídeo então é preciso calcular os deltas em relação a este ponto
        //--- para desenhar o levantamento e as curvas de nível. Para aprimorar o cálculo calcula em relação ao 1o e ao último ponto 
        //--- tirando a média.

        //--- Deltas pelo PCTRL1

        Deltas[X] = PontosControleUTM[0].x - PontosControleGeoRef[0].x / Escala[X];
        Deltas[Y] = PontosControleUTM[0].y - (AlturaVideoPixels - PontosControleGeoRef[0].y) / Escala[X];

        //--- Calcula a média

        DeltasCN[X] = Deltas[X];
        DeltasCN[Y] = Deltas[Y] + AlturaVideo;

        DesenharMapa = true;
        ProjetoGeoreferenciado = TipoGeoref;

        ((CChildFrame*)GetParentFrame())->DeslizanteGeom.m_CEEscala.EnableWindow(false);

        m_nX = m_nY = 0.0;
        YCentral = (AlturaVideo / 2.0 + (Deltas[Y])),
        XCentral = (LarguraVideo / 2.0 + (Deltas[X]));

        ((CChildFrame*)GetParentFrame())->DeslizanteGeom.MudarEscala(Escala[X]);

        LstPontosGeoRef.clear();
        Superficie.Clear();
        Superficie.Constroi();

        Superficie.AtribuiEquidisCN(EquidisCN);

        pDesCroquisView = ((CMainFrame*)AfxGetMainWnd())->pDesCroquisView2;

        if (pDesCroquisView)
        {
          pDesCroquisView->SetParent(this);

          double LimiteEsq(Superficie.PegaLimiteEsq()), LimiteDir(Superficie.PegaLimiteDir()), LimiteSup(Superficie.PegaLimiteSup()), LimiteInf(Superficie.PegaLimiteInf());

          RECT Rect;

          pDesCroquisView->GetWindowRect(&Rect);

          pDesCroquisView->AlturaCroquisPixels = (Rect.bottom - Rect.top);
          pDesCroquisView->LarguraCroquisPixels = (Rect.right - Rect.left);

          pDesCroquisView->AlturaDesenhoMetros = LimiteSup - LimiteInf;
          pDesCroquisView->LarguraDesenhoMetros = LimiteDir - LimiteEsq;

          pDesCroquisView->Escala = pDesCroquisView->AlturaCroquisPixels / pDesCroquisView->AlturaDesenhoMetros;
          double Temp = pDesCroquisView->LarguraCroquisPixels / pDesCroquisView->LarguraDesenhoMetros;

          if (Temp < pDesCroquisView->Escala)  pDesCroquisView->Escala = Temp;

          pDesCroquisView->Deltas[0] = LimiteEsq;
          pDesCroquisView->Deltas[1] = LimiteInf;

          pDesCroquisView->TamVideoOriginal(AlturaVideo, LarguraVideo);
          pDesCroquisView->DefineEscala();

          pDesCroquisView->JanelaDesGeom(this);
          pDesCroquisView->Tipo(DesCroquisView::TIPO_MAPA);

          pDesCroquisView->GetParentFrame()->ShowWindow(SW_SHOW);

          pDesCroquisView->AtribuiPicture(&m_cPicture);
          pDesCroquisView->DesenharMapa(&DesenharMapa);

          pDesCroquisView->Iniciou = true;

          pDesCroquisView->RedrawWindow();
        }

        RedrawWindow();

        pMainFrame->ProjetoNovo = -1;

        AlterouTerreno = AlterouSuperficie = true;
        return true;
      }
      else monolog.mensagem(-1,"Não foram encontrados 4 pontos de controle , processo abortado");
    }
    else monolog.mensagem(-1,"Para sincronizar o mapa são necessários 4 pontos de controle, processo abortado");

    return false;
  }

  pMainFrame->ProjetoNovo = -1;

  return true;
}

void DesPontosGeomView::OnRemovergeoreferenciamento()
{
#ifdef DEMOVER

  AfxMessageBox("Função indisponível na cópia de demonstração.");
  return;

#endif

  if (AfxMessageBox("Tem certeza que deseja remover o georeferenciamento?", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
  {
    NomeMapa.Empty();
    RazaoEscalasJPGDes = INFINITO;
    DesenharMapa = SincPontosControle = DesenharPontos = DesenharArestas = false;
    ProjetoGeoreferenciado = SUPERF_TRADICIONAL;
    ((CChildFrame*)GetParentFrame())->DeslizanteGeom.m_CEEscala.EnableWindow(!ProjetoGeoreferenciado);
    FaltaGeoReferenciamento = true;
    ((CMainFrame*)AfxGetMainWnd())->TipoSuperficie = SUPERF_TRADICIONAL;

    //--- Desrotaciona a superfície

   // Desrotacionar();

    //--- O Desrotacionar flega para zera o azimute
    //--- pois a rotacção é feita no Rotacionar(),
    //--- neste caso nao é pra zerar, é pra desrotacionar sem zerar o azimute

    //ZerarAzimute = false;  

    RedrawWindow();
  }
}

void DesPontosGeomView::OnButMedir()
{
  if (Medindo == 0) Medindo = 1;
  else
  {
    Medindo = 0;
    LPontosMedidos.clear();
  }

  DesenhandoObjetos = EsbocandoObjetos = ObjetosDrenagem = false;

  RedrawWindow();
}

void DesPontosGeomView::OnUpdateButMedir(CCmdUI* pCmdUI)
{
  pCmdUI->Enable((int)Superficie.pSuperficieAtual->size());
}

void DesPontosGeomView::OnBnClickedButpegararqnuvem()
{
}

void DesPontosGeomView::OnBnClickedButpegararq()
{
}

void DesPontosGeomView::AcertaCotasPontosMedidos()
{
  register itlstPontos itAtual(LPontosMedidos.begin()), itAnterior(itAtual);

  for (++itAtual; itAtual != LPontosMedidos.end(); ++itAtual, itAnterior = itAtual)
  {
    itAtual->fi = (((itAnterior->x + itAtual->x) / 2.0) - Deltas[X]) * Escala[X];
    itAtual->i = (AlturaVideo - (((itAnterior->y + itAtual->y) / 2.0) - Deltas[Y])) * Escala[Y];
  }
}

#include "CalcSU\CalcSU.h"
#include "CalcSU\CCCalcSU.h"

int DesPontosGeomView::DefineFronteiraDireto(int TipoSuperf)
{
  CString Nome(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase());

  CConfigProj ConfigProjeto(NULL, Nome);
  ConfigProjeto.LeArquivo();

  DPopUpPonto.ShowWindow(SW_SHOW);

  DPopUpPonto.MostraDescricao(300, 200, std::string(" \r\n  Definindo a fronteira...    \r\n "));

  double DistMediaPontos((TipoSuperf == SUPERF_GERENMAPAS || TipoSuperf == SUPERF_SIG) ? 100000.0 : 20.0);

  int Erro(CCalcSU::CalculaAS(Nome, NULL, NULL, DistMediaPontos * 100.0, ConfigProjeto.FormArqPontos, ((CMainFrame*)AfxGetMainWnd())->TipoSuperficie));

  AlterouTerreno = false;
  OnButfronteira();

  DPopUpPonto.ShowWindow(SW_HIDE);

  return Erro;
}

int DesPontosGeomView::CalculaSuperficie(CProgressCtrl* pProgress,int pTipoSuperficie)
{
  CString Nome(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase());

  CConfigProj ConfigProjeto(NULL, Nome);
  ConfigProjeto.LeArquivo();

  int Erro(CCalcSU::CalculaSU(Nome, NULL,pProgress, CCalcSU::TRIANG_DELAUNAY, ConfigProjeto.FormArqPontos.GetBuffer(), pTipoSuperficie));

  if (Erro)
  {
    AfxMessageBox("Houve erro no Cálculo da Superfície");
  }

  Superficie.LimpaRestricoeseBordos();
  Superficie.Serialize(Nome, LER);           //---Le a superficie que acabou de ser calculada
  Superficie.LeArqPontosSoltosDGN();         //--- Le os pontos soltos do mapa
  
  if(((CMainFrame*)AfxGetMainWnd())->TipoSuperficie != SUPERF_TRADICIONAL) Superficie.AtribuiEquidisCN(EquidisCN);     

  //Rotaciona(AnguloRotacao,1);      

  AlterouProjeto = AlterouTerreno = true;

  RedrawWindow();

  return Erro;
}

void DesPontosGeomView::OnButVisCroquis()
{
  DesenharCroquis = DesenharCroquis == false;

  pDesCroquisView = ((CMainFrame*)AfxGetMainWnd())->pDesCroquisView2;

  if (pDesCroquisView)
  {
//    ::SetWindowPos(pDesCroquisView->m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    pDesCroquisView->GetParentFrame()->ShowWindow(DesenharCroquis ? SW_SHOW : SW_HIDE);
  }
}

LRESULT DesPontosGeomView::OnClicouCroquis(WPARAM WP, LPARAM LP)
{
  CPoint PCood(WP, LP);

  IrParaPonto(CPoint(WP, LP));

  return 0;
}

void DesPontosGeomView::IrParaPonto(CPoint& Ponto)
{
  double DeltasAnteriores[2] = { Deltas[X],Deltas[Y] };

  Deltas[X] = Ponto.x;
  Deltas[Y] = Ponto.y;

  DeltasReais.x = Deltas[X] -= LarguraVideo / 2.0;
  DeltasReais.y = Deltas[Y] -= AlturaVideo / 2.0;

  if (!SincPontosControle)
  {
    m_nX = (DeltasCN[X] - Deltas[X]) * Escala[X];
    m_nY = AlturaVideoPixels - ((DeltasCN[Y] - Deltas[Y]) * Escala[Y]);
  }
  else
  {
    double EscalaCroquis(pDesCroquisView ? pDesCroquisView->PegaEscala() : 1.0);

    m_nX = (DeltasCN[X] - Deltas[X]) * Escala[X];
    m_nY = AlturaVideoPixels - ((DeltasCN[Y] - Deltas[Y]) * Escala[Y]);
  }

  OnArrastouDesenho();
  Invalidate();
  RedrawWindow();
}

void DesPontosGeomView::OnPopupInserirPonto()
{
  InserindoPonto = true;
}

void DesPontosGeomView::OnUpdatePopupInserirPonto(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!PegouPonto && !PegouPontoDesabilitado && !PegouPontoInserido && !PegouPontoPerfilAvulso && ItLembrete == SetLembretes.end());
}

void DesPontosGeomView::OnPopupSair()
{
  ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();
  RedrawWindow();
}

void DesPontosGeomView::OnPopupDrenagemSair()
{
  ItEquiDrenagemMarcada = Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end();
  RedrawWindow();
}

#include "CDiaAreaPerimetro.h"

void DesPontosGeomView::OnPopupAreaPerimetro()
{
  CDiaAreaPerimetro DiaAreaPerimetro(this, Superficie.PegaArea(), Superficie.PegaPerimetro());
  DiaAreaPerimetro.DoModal();
}

void DesPontosGeomView::InicializaCroquis()
{
  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());

  pDesCroquisView = MainFrame->pDesCroquisView2;

  if (pDesCroquisView)
  {
    double LimiteEsq(Superficie.PegaLimiteEsq()), LimiteDir(Superficie.PegaLimiteDir()), LimiteSup(Superficie.PegaLimiteSup()), LimiteInf(Superficie.PegaLimiteInf());

    RECT Rect;

    pDesCroquisView->GetWindowRect(&Rect);

    pDesCroquisView->AlturaCroquisPixels = (Rect.bottom - Rect.top);
    pDesCroquisView->LarguraCroquisPixels = (Rect.right - Rect.left);

    pDesCroquisView->AlturaDesenhoMetros = LimiteSup - LimiteInf;
    pDesCroquisView->LarguraDesenhoMetros = LimiteDir - LimiteEsq;

    pDesCroquisView->Escala = pDesCroquisView->AlturaCroquisPixels / pDesCroquisView->AlturaDesenhoMetros;
    double Temp = pDesCroquisView->LarguraCroquisPixels / pDesCroquisView->LarguraDesenhoMetros;

    if (Temp < pDesCroquisView->Escala)  pDesCroquisView->Escala = Temp;

    pDesCroquisView->Deltas[0] = LimiteEsq;
    pDesCroquisView->Deltas[1] = LimiteInf;

    pDesCroquisView->TamVideoOriginal(AlturaVideo, LarguraVideo);
    pDesCroquisView->DefineEscala();

    pDesCroquisView->JanelaDesGeom(this);

    pDesCroquisView->PegaDados(Superficie.PegaPontFronteiraSuperf(), NULL, NULL, NULL, NULL);   //--- Fronteira, projeto e foto

    pDesCroquisView->AlturaCroquisPixels /= 1.15;
    pDesCroquisView->LarguraCroquisPixels /= 1.15;
    pDesCroquisView->Tipo(DesCroquisView::TIPO_PONTOS);
    pDesCroquisView->DefineEscala();

    pDesCroquisView->AtribuiPicture(&m_cPicture);
    pDesCroquisView->DesenharMapa(&DesenharMapa);

    pDesCroquisView->Iniciou = true;

    if (DesenharCroquis)
    {
      pDesCroquisView->GetParentFrame()->ShowWindow(DesenharCroquis ? SW_SHOW : SW_HIDE);
      pDesCroquisView->RedrawWindow();
    }
  }
}

int DesPontosGeomView::PegaNomesArquivosPIS(tysetNomeArquivos& setNomeArquivos, CString NomeProjeto)
{
  struct _finddata_t c_file;
  intptr_t hFile;
  int i(0);

  CString NomeArquivoBase(NomeProjeto.Mid(0, NomeProjeto.ReverseFind('\\')+1));

  setNomeArquivos.clear();

  if ((hFile = _findfirst((char*)LPCTSTR(NomeProjeto+ "*PonInsSec*.xyz"), &c_file)) == -1L) return 0;  //--- Arquivos PIS
  else
  {
    do
    {
      setNomeArquivos.insert((NomeArquivoBase + c_file.name).GetBuffer());
    } while (++i < 1000 && _findnext(hFile, &c_file) == 0);

    _findclose(hFile);
  }

  return i;
}

void DesPontosGeomView::RemoveRua(const std::string& NomeRua)
{
  Trechos.RemoveTrecho(NomeRua);

  RedrawWindow();
}

void DesPontosGeomView::OnPopupAreaPerimetroAci()
{
  double Area(0.0),Perimetro(0.0),Deltax(0.0);

  ItCLDeltaSup itAnte(ItLLDeltaSupMarcada->begin()), itAtual(itAnte);

  for (++itAtual ; itAtual != ItLLDeltaSupMarcada->end(); ++itAtual)
  {
    Deltax = itAtual->PegaPCentral().x - itAnte->PegaPCentral().x;
 
    Area += (itAtual->PegaPCentral().y + itAnte->PegaPCentral().y) * Deltax;
    Perimetro += hypot((itAtual->PegaPCentral().y - itAnte->PegaPCentral().y), Deltax);

    itAnte = itAtual;
  }

  Area /= 2.0;

  CDiaAreaPerimetro DiaAreaPerimetro(this,Area,Perimetro);

  DiaAreaPerimetro.DoModal();
}

void DesPontosGeomView::OnUpdatePopupAreaPerimAci(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end() && *ItLLDeltaSupMarcada->begin() == *ItLLDeltaSupMarcada->rbegin());
}

#include "CDiaDadosBaciaHidrologica.h"

void DesPontosGeomView::OnPopupDefinirBacia()
{
  tylstBacias& LstBaciasHidrograficas(Superficie.PegaBacias());

  ittylstBacias ItBaciaAtual(Superficie.ProcuraBacia(ItLLDeltaSupMarcada));

  //--- Se a bacia ja existe o espigão pode ter sido alterado e pode estar com erros
  //--- é preciso reconstruir a bacia

  if (ItBaciaAtual == LstBaciasHidrograficas.end())
  {
    LstBaciasHidrograficas.clear();
    LstBaciasHidrograficas.emplace_back(CBacia(*ItLLDeltaSupMarcada));
    ItBaciaAtual--;
  }

  //--- Erros de construção

  if (ItBaciaAtual->Erro != CBacia::BACIA_OK)
  {
    std::string Mensagem;
    
    switch (ItBaciaAtual->Erro)
    {
      case CAciTopografico::BACIA_ABERTA: Mensagem = "Espigão aberto, As bacias devem ser delimitadas por um espigão fechado."; break;
      case CBacia::FALTA_ESPIGAO: Mensagem = "Falta o espigão para esta bacia."; break;
      default: Mensagem = "Houve um erro no cálculo da bacia hidrográfica";
    }

    monolog.mensagem(-1, Mensagem.c_str());

    LstBaciasHidrograficas.pop_back();

    return;
  }
  else AlterouBacias = true;
 
  std::string Mensagem;

  if (ItBaciaAtual->Erro != CBacia::BACIA_OK)
  {
    //--- Erros de cálculo

    switch (ItBaciaAtual->Erro)
    {
      case CBacia::MUITAS_SAIDAS: Mensagem = "Existem muitas saídas de talvegues nesta bacia (Cada bacia deve possuir apenas uma saida (Foz))."; break;
      case CBacia::SEM_SAIDA: Mensagem = "Esta bacia não tem saída (foz), toda bacia deve possuir uma e apenas uma foz."; break;
      case CBacia::ERRO_CN_ESPIGAO:  Mensagem = "Há curvas de nível cortando o espigão várias vezes, verifique o arquivo de log."; break;
      default: Mensagem = "Houve um erro no cálculo da bacia hidrográfica";
    }

    monolog.mensagem(-1, Mensagem.c_str());

    return;
  }

  //--- Calcula cota da foz

  std::string NovoNomeBacia(ItBaciaAtual->PegaDadosBacia().NomeBacia);

  if (NovoNomeBacia.empty())
  {
    if (((CMainFrame*)AfxGetMainWnd())->TipoSuperficie == SUPERF_SIG)
    {
      if(BaciaHidrografica.size() > 3)
      { 
        NovoNomeBacia = BaciaHidrografica.begin()->Descricao;
        size_t pNome(NovoNomeBacia.find_last_of('\\')+1);
        if(pNome != std::string::npos) 
        {
          NovoNomeBacia = NovoNomeBacia.substr(pNome, NovoNomeBacia.size() - pNome);
        }

        if (NovoNomeBacia.size() > 0)
        {
          pNome = NovoNomeBacia.find_last_of('.');
          if(pNome != std::string::npos) 
          {
            NovoNomeBacia = NovoNomeBacia.substr(0,pNome);
          }
        }
      }
    }
  }

  CDiaDadosBaciaHidrologica DiaBaciaHidrol(this, ItBaciaAtual, ((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual(), NovoNomeBacia, LstBaciasHidrograficas, &Superficie);

  if (DiaBaciaHidrol.DoModal() == IDOK)
  {
    if (NovoNomeBacia != ItBaciaAtual->PegaDadosBacia().NomeBacia) ItBaciaAtual->MudaNomeBacia(NovoNomeBacia);

    NovoNomeBacia.insert(0, " Espigão ");

    const_cast<Ponto*>((Ponto*)&ItLLDeltaSupMarcada->begin()->PegaPCentral())->Descricao = NovoNomeBacia.c_str();   //--- Marca no talvegue que ele pertence a esta bacia

    CalculaCotaPonto(ItBaciaAtual->CG());

    AlterouTerreno = true;
  }
  
  TalveguePrincipal.clear();     //--- O tavegue principal passou para a bacia

  RedrawWindow();
}

void DesPontosGeomView::OnUpdatePopupDefinirBacia(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end() &&
                (*ItLLDeltaSupMarcada->begin()).TipoObjeto == CAciTopografico::ESPIGAO);   
}

void DesPontosGeomView::OnPopupIncTalvPrincipal()
{
  bool OK(true);

  if (ItLLDeltaSupMarcada->begin()->TipoObjeto == CAciTopografico::TALVEGUE)
  {
    if (std::find(TalveguesDoTalveguePrincipal.begin(), TalveguesDoTalveguePrincipal.end(), ItLLDeltaSupMarcada) == TalveguesDoTalveguePrincipal.end())
    {
      if (TalveguePrincipal.size() == 0 || *TalveguePrincipal.rbegin() == *ItLLDeltaSupMarcada->begin())
      {
        TalveguePrincipal.insert(TalveguePrincipal.end(), ItLLDeltaSupMarcada->begin(), ItLLDeltaSupMarcada->end());
      }
      else
      {
        if (*TalveguePrincipal.rbegin() == *ItLLDeltaSupMarcada->rbegin())
        {
          TalveguePrincipal.insert(TalveguePrincipal.end(), ItLLDeltaSupMarcada->rbegin(), ItLLDeltaSupMarcada->rend());
        }
        else
        {
          if (*TalveguePrincipal.begin() == *ItLLDeltaSupMarcada->rbegin())
          {
            for (auto ItLDS = ItLLDeltaSupMarcada->rbegin(); ItLDS != ItLLDeltaSupMarcada->rend(); ItLDS++)
            {
              TalveguePrincipal.push_front(*ItLDS);
            }

            //--- Não Funciona ---- em alguns casos esta incluindo antes do begin...

            //TalveguePrincipal.insert(TalveguePrincipal.begin(), ItLLDeltaSupMarcada->rbegin(), ItLLDeltaSupMarcada->rend());
          }
          else
          {
            if (*TalveguePrincipal.begin() == *ItLLDeltaSupMarcada->begin())
            {
              for (auto ItLDS = ItLLDeltaSupMarcada->begin(); ItLDS != ItLLDeltaSupMarcada->end(); ItLDS++)
              {
                TalveguePrincipal.push_front(*ItLDS);
              }

              //--- Não Funciona ---- em alguns casos esta incluindo antes do begin...

             // TalveguePrincipal.insert(TalveguePrincipal.begin(), ItLLDeltaSupMarcada->begin(), ItLLDeltaSupMarcada->end());
            }
            else
            {
              monolog.mensagem(-1, "Estes Talvegues não se tocam");

              OK = false;
            }
          }
        }
      }
    }
    else
    {
      monolog.mensagem(-1, "Este segmento já faz parte deste talvegue");
    }
  }
  if (OK)
  {
    TalveguesDoTalveguePrincipal.push_back(ItLLDeltaSupMarcada);
    AlterouBacias = true;
  }

  RedrawWindow();
}

void DesPontosGeomView::OnUpdatePopupIncTalvPrincipal(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(DefinindoTalveguePrincipal);
}

void DesPontosGeomView::CalculaCotaPonto(Ponto& pPonto,bool Desrotacionar)
{
  //--- ERRO-> Calculou superficie não esta sendo pega na primeirissima vez ACERTAR 06-07-2024
  //if (((CMainFrame*)AfxGetMainWnd())->CalculouSuperficie())
  if(1)
  {
    Ponto PAux(pPonto);

    if (Desrotacionar)
    {
      PAux.RotacionaPlano(sin(-AnguloRotacao), cos(-AnguloRotacao), -AnguloRotacao);
    }

    Ponto P1, P2, P3;

    if (CCalcSU::PegaTriangulo(P1,P2,P3,PAux) == 0)   //--- Aqui a superfície não está rotacionada
    {
      if (Superficie.EstaRotacionada())   //--- se estiver rotaciona os pontos do triangulo
      {
        P1.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());
        P2.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());
        P3.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());
      }

      ItSSuperficie ItSP1(Superficie.Find(P1,1e-4));        //--- Aqui a superfície está rotacionada
      if (ItSP1 != Superficie.pSuperficieAtual->end())
      {
        ItSSuperficie ItSP2(Superficie.Find(P2,1e-4));
        if (ItSP2 != Superficie.pSuperficieAtual->end())
        {
          ItSSuperficie ItSP3(Superficie.Find(P3,1e-4));
          if (ItSP3 != Superficie.pSuperficieAtual->end())
          {
            Matem::CalculaZ(ItSP1->PCentral, ItSP2->PCentral, ItSP3->PCentral, pPonto);
          }
        }
      }
    }
    else
    {
     // if (CalculaCotaPontoCN(pPonto) == false)   //--- Agora até os drones tem superficie portanto está função nunca deverá ser chamada #VERIFICAR ->Testar
      {
       // monolog.mensagem(-1,"A cota do ponto não pôde ser calculada...");
      }
    }
  }
  else
  {
    monolog.mensagem(-1,"A cota do ponto não pôde ser calculada porque a superfície não foi calculada");
  }
}

void DesPontosGeomView::OnPopupExcTalvPrincipal()
{
  itlstItCLLDeltaSup Resp(std::find(TalveguesDoTalveguePrincipal.begin(),TalveguesDoTalveguePrincipal.end(),ItLLDeltaSupMarcada));

  if (Resp != TalveguesDoTalveguePrincipal.end())
  {
    if(*TalveguesDoTalveguePrincipal.rbegin() == ItLLDeltaSupMarcada)
    {
      if(dialogar.DoModal(-1,"Confirma a exclusão do último segmento do talvegue principal") == IDOK)
      {
        for(const CDeltaSuperficie& DSMarcada : *ItLLDeltaSupMarcada)

        for (ItLDeltaSup ItDSTalvAtual = TalveguePrincipal.begin() ; ItDSTalvAtual != TalveguePrincipal.end() ; ItDSTalvAtual++)
        {
          if (*ItDSTalvAtual == DSMarcada)
          {
            TalveguePrincipal.erase(ItDSTalvAtual);
            break;
          }
        }

        TalveguesDoTalveguePrincipal.pop_back();
        RedrawWindow();
      }
    }
    else
    {
      monolog.mensagem(-1,"Somente o último segmento pode ser excluído do talvegue principal");
    }
  }
}

void DesPontosGeomView::OnUpdatePopupExcTalvPrincipal(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(DefinindoTalveguePrincipal && TalveguesDoTalveguePrincipal.size() > 0 && TalveguePrincipal.size() > 0);
}

void DesPontosGeomView::DefineEquipDrenAlinhamento()  //--- todo click em pontos de terraplanagem cai aqui, não so quando é sarjeta
{
  if (ItInicioSarjeta->PCentral.S > ItFimSarjeta->PCentral.S)  //--- A estaca inicial tem que ser a menor
  {
    ItSSuperficie ItTemp(ItFimSarjeta);
    ItFimSarjeta = ItInicioSarjeta;
    ItInicioSarjeta = ItTemp;
  }

  ittysetSecoesTerrap ItIni(SecoesTerraplanagem.find(CSecaoTerrap(ItInicioSarjeta->PCentral.S))),
                      ItFim(SecoesTerraplanagem.find(CSecaoTerrap(ItFimSarjeta->PCentral.S)));

  if (ItIni != SecoesTerraplanagem.end() && ItFim != SecoesTerraplanagem.end())
  {
    CEstaca Estaca(ItInicioSarjeta->PCentral.S);

    std::string DescPonto(ItInicioSarjeta->PCentral.Descricao);

    if (ItIni->PontosTerrap.begin()->Descricao.Find(" Ponto") != -1)
    {
      DescPonto = DescPonto.substr(0,DescPonto.find(" Ponto")-2);
    }

    std::string NomeSarjeta(Estaca.GetBuffer());
    NomeSarjeta += "-SARJ-";
    NomeSarjeta += DescPonto;
    Limpa(NomeSarjeta);

    CEquipDrenagem ObraDrenagemAtual(ObjetoAtual, NomeSarjeta);

    if (Superficie.EstaRotacionada())
    {
      Ponto PRotacionado(ItInicioSarjeta->PCentral);
      //PRotacionado.RotacionaPlano(sin(-AnguloRotacao),cos(-AnguloRotacao),-AnguloRotacao);
      ObraDrenagemAtual.InserePonto(PRotacionado);
    }
    else ObraDrenagemAtual.InserePonto(ItInicioSarjeta->PCentral);

    ++ItFim;

    while (++ItIni != ItFim)
    {
      ittysetPonto ItPontoAtual(ItIni->PontosTerrap.begin());

      while (ItPontoAtual != ItIni->PontosTerrap.end() && ItPontoAtual->Descricao.Find(DescPonto.c_str()) == -1)
      {
        ++ItPontoAtual;
      }

      if (ItPontoAtual != ItIni->PontosTerrap.end())
      {
        if(Superficie.EstaRotacionada())
        {
          Ponto PDesRotacionado(*ItPontoAtual);

          //=======PDesRotacionado.RotacionaPlano(cos(-AnguloRotacao),sin(-AnguloRotacao),-AnguloRotacao);

          ObraDrenagemAtual.InserePonto(PDesRotacionado);
        }
        else ObraDrenagemAtual.InserePonto(*ItPontoAtual);
      }
    }

    DefineAlinEquipDrenagem(ObraDrenagemAtual);
    CalculaCotasAlinhamento(*ObraDrenagemAtual.PegaAlinhamentos().begin());

    //--- Os calculos dos pontos são feitos com a superficie sem rotacionar mas pra desenhar é preciso rotacionar

    if(Superficie.EstaRotacionada()) 
    {
      //asdasdasdObraDrenagemAtual.Rotaciona(sin(AnguloRotacao),cos(AnguloRotacao),AnguloRotacao);         //--- Rotaciona o acidente topografico
      //ObraDrenagemAtual.RotacionaPlano(sin(AnguloRotacao), cos(AnguloRotacao), AnguloRotacao);  //--- rotaciona os alinhamentos
    }

    Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().emplace(ObraDrenagemAtual.PegaNome(), ObraDrenagemAtual);

    ItInicioSarjeta = Trechos.PegaTrechoAtual()->second.SuperficieTerr.pSuperficieAtual->end();

    AlterouTerreno = true;

    RedrawWindow();
  }
}

void DesPontosGeomView::DefineDescidaDagua(ItLLDeltaSup ItLstPontos, std::string& NomeDescida)
{
  std::stringstream Temp;
  int Contador(0);

  if (NomeDescida.size() == 0)
  {
    do
    {
      std::stringstream Temp;

      if (Contador++ > 0) Temp << Contador;

      NomeDescida = ItLstPontos->begin()->PCentral.Nome.GetBuffer();
      NomeDescida += "-DESC" + Temp.str();
      Limpa(NomeDescida);

    } while (Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().find(NomeDescida) != Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end());
  }

  CEquipDrenagem EquipDrenagem(CAciTopografico::EQUI_DESCIDA_DAGUA, NomeDescida);

  bool Invertido(PegaPontoMaisProximoTangente(ItLstPontos->begin()->PCentral,&ItLstPontos->rbegin()->PCentral) == false);

  if (Invertido)      //--- Na descida dágua o primeiro ponto tem q ser o mais próximo da via
  {
    Ponto PTemp(ItLstPontos->begin()->PCentral);
    *ItLstPontos->begin() = *ItLstPontos->rbegin();
    *ItLstPontos->rbegin() = PTemp;
  }

  EquipDrenagem.Az(Mat.CalculaAzimute(ItLstPontos->rbegin()->PCentral, ItLstPontos->begin()->PCentral));  //--- O azimute no sentido para a via

  for (auto& rPAtual : *ItLstPontos)
  {
    EquipDrenagem.InserePonto(rPAtual.PCentral);
  }

  DefineAlinEquipDrenagem(EquipDrenagem);

  CalculaCotasAlinhamento(*EquipDrenagem.PegaAlinhamentos().begin());

  typairmapEquipDrenagem pEqDrn(Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().emplace(EquipDrenagem.PegaNome(), EquipDrenagem));

  ItObraDrenagemAtual = pEqDrn.first;

  AlterouTerreno = true;
  CriandoRestricao = false;
  NomeEquiAtual.clear();

  RedrawWindow();
}

void DesPontosGeomView::DefineBueiro(ItLLDeltaSup ItLstPontos, std::string& NomeBueiro)
{
  std::stringstream Temp;
  int Contador(0);

  if (NomeBueiro.size() == 0)
  {
    do
    {
      std::stringstream Temp;

      if (Contador++ > 0) Temp << Contador;

      NomeBueiro = ItLstPontos->begin()->PCentral.Nome.GetBuffer();
      NomeBueiro += "-BUEI" + Temp.str();
      Limpa(NomeBueiro);

    } while (Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().find(NomeBueiro) != Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end());
  }

  CEquipDrenagem EquipDrenagem(CAciTopografico::EQUI_BUEIRO, NomeBueiro);

  for (auto& rPAtual : *ItLstPontos)
  {
    EquipDrenagem.InserePonto(rPAtual.PCentral);
  }

  DefineAlinEquipDrenagem(EquipDrenagem);
  CalculaCotasAlinhamento(*EquipDrenagem.PegaAlinhamentos().begin());

  typairmapEquipDrenagem pEqDrn;

  pEqDrn = Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().emplace(EquipDrenagem.PegaNome(), EquipDrenagem);

  ItObraDrenagemAtual = pEqDrn.first;

  AlterouTerreno = true;
  CriandoRestricao = false;
  NomeEquiAtual.clear();

  RedrawWindow();
}

void DesPontosGeomView::DefineBueiroRetangular(ItLLDeltaSup ItLstPontos, std::string& NomeBueiro)
{
  std::stringstream Temp;
  int Contador(0);

  if (NomeBueiro.size() == 0)
  {
    do
    {
      std::stringstream Temp;

      if (Contador++ > 0) Temp << Contador;

      NomeBueiro = ItLstPontos->begin()->PCentral.Nome.GetBuffer();
      NomeBueiro += "-BUEI_RET" + Temp.str();
      Limpa(NomeBueiro);

    } while (Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().find(NomeBueiro) != Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end());
  }

  CEquipDrenagem EquipDrenagem(CAciTopografico::EQUI_BUEIRO_RETANG, NomeBueiro);

  for (auto& rPAtual : *ItLstPontos)
  {
    EquipDrenagem.InserePonto(rPAtual.PCentral);
  }

  DefineAlinEquipDrenagem(EquipDrenagem);

  CalculaCotasAlinhamento(*EquipDrenagem.PegaAlinhamentos().begin());

  typairmapEquipDrenagem pEqDrn;

  pEqDrn = Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().emplace(EquipDrenagem.PegaNome(), EquipDrenagem);

  ItObraDrenagemAtual = pEqDrn.first;

  AlterouTerreno = true;
  CriandoRestricao = false;
  NomeEquiAtual.clear();

  RedrawWindow();
}

void DesPontosGeomView::DefineCanaleta(ItLLDeltaSup ItLstPontos, std::string& NomeCanaleta)
{
  std::stringstream Temp;
  int Contador(0);

  if (NomeCanaleta.size() == 0)
  {
    do
    {
      std::stringstream Temp;

      if (Contador++ > 0) Temp << Contador;

      NomeCanaleta = ItLstPontos->begin()->PCentral.Nome.GetBuffer();
      NomeCanaleta += "-CANA" + Temp.str();
      Limpa(NomeCanaleta);

    } while (Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().find(NomeCanaleta) != Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end());
  }

  CEquipDrenagem EquipDrenagem(CAciTopografico::EQUI_CANALETA_TN, NomeCanaleta);

  for (auto& rPAtual : *ItLstPontos)
  {
    EquipDrenagem.InserePonto(rPAtual.PCentral);
  }

  DefineAlinEquipDrenagem(EquipDrenagem);
  CalculaCotasAlinhamento(*EquipDrenagem.PegaAlinhamentos().begin());

//  EquipDrenagem.Az(Mat.CalculaAzimute(ItLstPontos->rbegin()->PCentral, ItLstPontos->begin()->PCentral));  //--- O azimute no sentido para a via

  Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().erase(EquipDrenagem.PegaNome());

  typairmapEquipDrenagem pEqDrn;

  pEqDrn = Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().emplace(EquipDrenagem.PegaNome(), EquipDrenagem);

  ItObraDrenagemAtual = pEqDrn.first;

  AlterouTerreno = true;

  RedrawWindow();
}

void DesPontosGeomView::DefineSarjetaTN(ItLLDeltaSup ItLstPontos,std::string& NomeSarjeta)
{
  std::stringstream Temp;
  int Contador(0);

  if(NomeSarjeta.size() == 0)
  {
    do
    {
      std::stringstream Temp;

      if(Contador++ > 0) Temp << Contador;

      NomeSarjeta = ItLstPontos->begin()->PCentral.Nome.GetBuffer();
      NomeSarjeta += "-SARJ" + Temp.str();
      Limpa(NomeSarjeta);

    }while(Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().find(NomeSarjeta) != Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end());
  }

  CEquipDrenagem EquipDrenagem(CAciTopografico::EQUI_VALETA_TN, NomeSarjeta);

  for( auto& rPAtual : *ItLstPontos)
  {
    EquipDrenagem.InserePonto(rPAtual.PCentral);
  }

  DefineAlinEquipDrenagem(EquipDrenagem);
  CalculaCotasAlinhamento(*EquipDrenagem.PegaAlinhamentos().begin());

  Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().erase(EquipDrenagem.PegaNome());

  typairmapEquipDrenagem pEqDrn;

  pEqDrn = Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().emplace(EquipDrenagem.PegaNome(), EquipDrenagem);
 
  ItObraDrenagemAtual = pEqDrn.first;

  AlterouTerreno = true;

  RedrawWindow();
}

void DesPontosGeomView::DefineTubulacao(ItLLDeltaSup ItLstPontos, std::string& NomeTubulacao)
{
  std::stringstream Temp;
  int Contador(0);

  if (NomeTubulacao.size() == 0)
  {
    do
    {
      std::stringstream Temp;

      if (Contador++ > 0) Temp << Contador;

      NomeTubulacao = ItLstPontos->begin()->PCentral.Nome.GetBuffer();
      NomeTubulacao += (ObjetoAtual == CAciTopografico::EQUI_TUBULACAO ? "-TUBU" : "-DRENO") + Temp.str();

      Limpa(NomeTubulacao);

    } while (Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().find(NomeTubulacao) != Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end());
  }

  CEquipDrenagem EquipDrenagem(ObjetoAtual,NomeTubulacao);

  //if(ObjetoAtual == CAciTopografico::EQUI_TUBULACAO) EquipDrenagem.Dimensiona(1.0, 1.0, 0.10, 0.20, 0.0, 0.0);
  //else EquipDrenagem.Dimensiona(1.0,0.20,0.05,0.10,1.0,0.0);

  for (auto& rPAtual : *ItLstPontos)
  {
    EquipDrenagem.InserePonto(rPAtual.PCentral);
  }

  EquipDrenagem.Az(Mat.CalculaAzimute(ItLstPontos->begin()->PCentral, ItLstPontos->rbegin()->PCentral));

  DefineAlinEquipDrenagem(EquipDrenagem);

  CalculaCotasAlinhamento(*EquipDrenagem.PegaAlinhamentos().begin());

  Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().erase(EquipDrenagem.PegaNome());

  typairmapEquipDrenagem pEqDrn;

  pEqDrn = Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().emplace(EquipDrenagem.PegaNome(), EquipDrenagem);

  ItObraDrenagemAtual = pEqDrn.first;

  AlterouTerreno = true;

  RedrawWindow();
}


void DesPontosGeomView::EditarSarjeta()
{
  CEquipDrenagem SarjetaAtual(ItEquiDrenagemMarcada->second);
   
  DefineAlinEquipDrenagem(ItEquiDrenagemMarcada->second);

  CalculaCotasAlinhamento(*SarjetaAtual.PegaAlinhamentos().begin());

  AlterouTerreno = true;

  RedrawWindow();
}

void DesPontosGeomView::GeraSecoesTerraplEstaca()  
{
  for (ItLLDeltaSup ItLL = AlinhamentosTerrapl.begin() ; ItLL != AlinhamentosTerrapl.end() ; ++ItLL)
  {
    for (ItLDeltaSup ItL = ItLL->begin(); ItL != ItLL->end(); ++ItL)
    {
      typairsetSecoesTerrap ItPairSecTerrap(SecoesTerraplanagem.emplace(ItL->PCentral.S));
      
      CSecaoTerrap* pSecTerrap(const_cast<CSecaoTerrap*>(&*ItPairSecTerrap.first));

      pSecTerrap->PontosTerrap.emplace(ItL->PegaPCentral());

      if(ItL->PegaPCentral().i == 0.0) 
      {
        pSecTerrap->MudaCoordEixo(ItL->PegaPCentral());   
      }
    }
  }

  //--- Remove as seções que não foram encontradas
  /*
  for(ittysetSecoesTerrap it = SecoesTerraplanagem.begin() ; it != SecoesTerraplanagem.end() ; )
  {
    if (it->PegaCoordEixo().x == INFINITO || it->PegaCoordEixo().y == INFINITO || it->PegaCoordEixo().z == INFINITO)
    {
      SecoesTerraplanagem.erase(it++);
    }
    else it++;
  }
  */
}

void DesPontosGeomView::DesenhaEquiDrenagem(CDC* pDC, ittymapEquipDrenagem* pEquipADesenhar)
{
  CPen PenaDrenagem,PenaTracejado;
  PenaDrenagem.CreatePen(PS_SOLID, 1, RGB(160,160,160));
  PenaTracejado.CreatePen(PS_DASH, 1, RGB(160,160,160));
  pDC->SelectObject(&PenaDrenagem);

  tymapEquipDrenagem& EquipamentosDrenagem(Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem());

  Ponto Ppixels;

  for (auto& rEquipAtual : EquipamentosDrenagem)
  {
    if(pEquipADesenhar && rEquipAtual != **pEquipADesenhar) continue;    //--- se for para desenhar um equipamento especifico pula os outros

    int Tipo(rEquipAtual.second.PegaTipo());

    if(Tipo == CAciTopografico::EQUI_DRENO_PROFUNDO)  pDC->SelectObject(&PenaTracejado);
    else pDC->SelectObject(&PenaDrenagem);

    for(auto& ItLLPontos : rEquipAtual.second.PegaAlinhamentos())
    {
      if(ItLLPontos.empty()) continue;

      itlstPontos ItPonto(ItLLPontos.begin());

      if(ItPonto->x == INFINITO) ++ItPonto;       //--- Marca a mudanca de poligonal (Reinicia o traço)

      Ppixels.x = ((ItPonto->x - Deltas[X]) * Escala[X]);
      Ppixels.y = (AlturaVideo - (ItPonto->y - Deltas[Y])) * Escala[Y];

      pDC->MoveTo(Ppixels.x, Ppixels.y);
     //--- Para Debug pDC->Arc(Ppixels.x - 3, Ppixels.y - 3, Ppixels.x + 3, Ppixels.y + 3, 0, 0, 0, 0);

      for(++ItPonto ; ItPonto != ItLLPontos.end() ; ++ItPonto)
      {
        bool Fim(ItPonto->x == INFINITO);
        
        if(Fim) ++ItPonto;   // Marca a mudanca de poligonal (Reinicia o traço)

        Ppixels.x = ((ItPonto->x - Deltas[X]) * Escala[X]);
        Ppixels.y = (AlturaVideo - (ItPonto->y - Deltas[Y])) * Escala[Y];

        if (Fim)
          pDC->MoveTo(Ppixels.x, Ppixels.y);
        else
          pDC->LineTo(Ppixels.x, Ppixels.y);

      //--- Para debug pDC->Arc(Ppixels.x - 3, Ppixels.y - 3, Ppixels.x + 3, Ppixels.y + 3, 0, 0, 0, 0); 
      }
    }

    //--- Desenha as estacas nos equipamentos de drenagem

    CPen PenaSec;
    PenaSec.CreatePen(PS_SOLID, 1, Cores[CORSECOES]);
    pDC->SelectObject(&PenaSec);

    for (auto& rEquipAtual : EquipamentosDrenagem)
    {
      if(rEquipAtual.second.CoordEixo().x != INFINITO && rEquipAtual.second.CoordEixo().y != INFINITO)
      {
        Ppixels.x = (rEquipAtual.second.CoordEixo().x - Deltas[X]) * Escala[X];
        Ppixels.y = (AlturaVideo - (rEquipAtual.second.CoordEixo().y - Deltas[Y])) * Escala[Y];

        pDC->Arc(Ppixels.x - 3, Ppixels.y - 3, Ppixels.x + 3, Ppixels.y + 3, 0, 0, 0, 0);
      }
    }
  }
}

void DesPontosGeomView::DefineAlinEquipDrenagem(CEquipDrenagem& EquipDrenagem)
{
   const lstitsetPontos& LstItSetObjetos(EquipDrenagem.PegaLista());

  if (LstItSetObjetos.size())
  {
    citlstitsetPontos it(LstItSetObjetos.begin()),itRasc(it);

    int Lado(CENTRO);

    if((*it)->Descricao.GetLength() > 3)
    {
      char CHLado((*it)->Descricao[(*it)->Descricao.GetLength()-3]);

      Lado = ((*it)->Descricao[5] == 'D' ? LADO_DIREITO :
              (*it)->Descricao[5] == 'E' ? LADO_ESQUERDO :
                           CHLado == 'D' ? LADO_DIREITO :
                           CHLado == 'E' ? LADO_ESQUERDO : CENTRO);
    }
    
    itRasc++;

    switch (EquipDrenagem.PegaTipo())
    {
      case CAciTopografico::EQUI_CANALETA_TERRAPL:
      case CAciTopografico::EQUI_CANALETA_TN:
      {
        enum{ DIAMETRO,AFASTAMENTO };

        double Dimensoes[7], Afast(0.0);

        EquipDrenagem.PegaDimensoes(&Dimensoes[DIAMETRO]);

        if (Lado == CENTRO)
        {
          Afast = Dimensoes[DIAMETRO] / 2.0;
        }
        else
        {
          if (EquipDrenagem.PegaTipo() == CAciTopografico::EQUI_CANALETA_TERRAPL) Afast = Dimensoes[AFASTAMENTO] + Dimensoes[DIAMETRO];
          if (Lado == ESQUERDO) Afast = -Afast;
        }

        tylstpontos LstPontosDesenho;
        EquipDrenagem.PegaAlinhamentos().clear();
        int QtdLinhas(2), Linha(0);

        LstPontosDesenho.clear();

        CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end());
        EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);

        Afast += Lado == ESQUERDO ? Dimensoes[DIAMETRO] : -Dimensoes[DIAMETRO];

        CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end());
        EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);
      }
      break;
      case CAciTopografico::EQUI_SARJETA:
      case CAciTopografico::EQUI_VALETA_TERRAPL:
      case CAciTopografico::EQUI_VALETA_TN:
      {
        double Dimensoes[7], Afast(0.0);

        EquipDrenagem.PegaDimensoes(&Dimensoes[0]);

        if (Lado == CENTRO)
        {
          if(EquipDrenagem.PegaTipo() != CAciTopografico::EQUI_TUBULACAO)
          {
            if(EquipDrenagem.PegaTipo() != CAciTopografico::EQUI_SARJETA)
            {
              Afast = (Dimensoes[0] + Dimensoes[1] + Dimensoes[2]) / 2.0;
              Dimensoes[3] = 0.0;
            }
          }
          else
          {
            Afast = Dimensoes[4] / 2.0;

            tylstpontos LstPontosDesenho;
            CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end());
            EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);
          }
        }
        else
        {
          if (EquipDrenagem.PegaTipo() == CAciTopografico::EQUI_VALETA_TERRAPL) Afast = Dimensoes[4];
          if(Lado == ESQUERDO) Afast = -Afast;
        }

        tylstpontos LstPontosDesenho;
        EquipDrenagem.PegaAlinhamentos().clear();
        int QtdLinhas(0),Linha(0);
        
        switch (EquipDrenagem.PegaTipo())
        {
          case CAciTopografico::EQUI_TUBULACAO: QtdLinhas = 1; break;
          case CAciTopografico::EQUI_SARJETA: QtdLinhas = 3;break;
          default: QtdLinhas = 4;
        }

        for (; Linha < QtdLinhas; ++Linha)
        {
          if (Linha == 0 && EquipDrenagem.PegaTipo() == CAciTopografico::EQUI_SARJETA)
          {
            Afast -= Lado == LADO_DIREITO ? 0.1 : -0.1;    //--- Arreda um pouco o a linha para aparecer o traco do bordo e terraplanagem
          }
          else
          {
            if(EquipDrenagem.PegaTipo() == CAciTopografico::EQUI_SARJETA)
            {
              Afast -= Lado == LADO_DIREITO ? Dimensoes[Linha] : -Dimensoes[Linha];
            }
            else
            {
              if (Linha != 0)
              {
                Afast += Lado == LADO_DIREITO ? Dimensoes[Linha-1] : -Dimensoes[Linha-1];
              }
            }
          }
          
          LstPontosDesenho.clear();

          CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end());

          EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);
        }
      }
      break;
      case CAciTopografico::EQUI_BUEIRO:
      {
        enum{QTD_ELEMENTOS,TIPO,LARGURA,ALTURA,AFASTAMENTO,LEITO,ANG_ALA};

        double Dimensoes[7], Afast(0.0);  

        EquipDrenagem.PegaDimensoes(&Dimensoes[0]);

        std::pair<double,bool> Esconsidade_Tipo(VerfEsconsidade(**it,**itRasc, EquipDrenagem.PegaEstacaEixo(), EquipDrenagem.CoordEixo()));
              
        EquipDrenagem.Esconsidade() = Esconsidade_Tipo.first == INFINITO ? Matem::M_PI_2 : Esconsidade_Tipo.first;

        if (Esconsidade_Tipo.second == true)          //--- O sentido está invertido?
        {
          EquipDrenagem.InverteDirecaoGiro();                //--- Desinverte, (isto Invalida o it e o itRasc)
          it = itRasc = EquipDrenagem.PegaLista().begin();   //--- Acerta o it e o itRasc
          itRasc++;
        }

        //--- Se não cortou o eixo da pista, para todos os efeitos o eixo da pista é o ponto inicial

        if (EquipDrenagem.CoordEixo().x == INFINITO) EquipDrenagem.CoordEixo() = **it;
        else EquipDrenagem.CalcDistanciasAlasBueiro();

        tylstpontos LstPontosDesenho;
        EquipDrenagem.PegaAlinhamentos().clear();

        Afast -= (Dimensoes[QTD_ELEMENTOS] * Dimensoes[LARGURA] + (Dimensoes[QTD_ELEMENTOS] -1.0) * Dimensoes[AFASTAMENTO]) / 2.0 ;

        CalculaPoligonalTraco(it,LstPontosDesenho, Afast, LstItSetObjetos.end(), -EquipDrenagem.Esconsidade());
        EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);

        for (int Linha = 0; Linha < Dimensoes[QTD_ELEMENTOS]; ++Linha)
        {
          Afast += Dimensoes[LARGURA];
          LstPontosDesenho.clear();
          CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end(),-EquipDrenagem.Esconsidade());   //--- No bueiro só 2 pontos
          EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);

          if(Dimensoes[QTD_ELEMENTOS] > 1.0 && Dimensoes[AFASTAMENTO] > 0.0 && Linha < Dimensoes[QTD_ELEMENTOS] - 1.0)   //--- Afastamento entre os tubos
          {
            Afast += Dimensoes[AFASTAMENTO];
            LstPontosDesenho.clear();
            CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end(),EquipDrenagem.Esconsidade());   //--- No bueiro só 2 pontos

            EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);
          }
        }

        LstPontosDesenho.clear();
        //CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end(), EquipDrenagem.Esconsidade());   //--- No bueiro só 2 pontos
        //EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);
        
        double Largura(Afast);

        LstPontosDesenho.clear();

        CalculaPontosBacias(it, LstPontosDesenho, Largura, LstItSetObjetos.end(),Dimensoes[ALTURA], EquipDrenagem.Esconsidade(),Dimensoes[ANG_ALA] /180.0 * Matem::M_PI);   //--- No bueiro só 2 pontos
        EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);
        DefineSecAvulsaBueiro(EquipDrenagem, false);
      }

      break;
     
      case CAciTopografico::EQUI_BUEIRO_RETANG:
      {
        enum {QTD_ELEMENTOS,TIPO,LARGURA,ALTURA,PAREDE,LEITO};   

        double Dimensoes[7], Afast(0.0);

        EquipDrenagem.PegaDimensoes(&Dimensoes[0]);

        std::pair<double, bool> Esconsidade_Tipo(VerfEsconsidade(**it, **itRasc, EquipDrenagem.PegaEstacaEixo(), EquipDrenagem.CoordEixo()));

        EquipDrenagem.Esconsidade() = Esconsidade_Tipo.first == INFINITO ? Matem::M_PI_2 : Esconsidade_Tipo.first;

        if (Esconsidade_Tipo.second == true)          //--- O sentido está invertido?
        {
          EquipDrenagem.InverteDirecaoGiro();                //--- Desinverte, (isto Invalida o it e o itRasc)
          it = itRasc = EquipDrenagem.PegaLista().begin();   //--- Acerta o it e o itRasc
          itRasc++;
        }

        //--- Se não cortou o eixo da pista, para todos os efeitos o eixo da pista é o ponto inicial

        if (EquipDrenagem.CoordEixo().x == INFINITO) EquipDrenagem.CoordEixo() = **it;
        else EquipDrenagem.CalcDistanciasAlasBueiro();

        tylstpontos LstPontosDesenho;
        EquipDrenagem.PegaAlinhamentos().clear();

        //--- Centraliza o bueiro
                                                                
        Afast -= (Dimensoes[QTD_ELEMENTOS] * Dimensoes[LARGURA] + Dimensoes[PAREDE] * (Dimensoes[QTD_ELEMENTOS]+1)) / 2.0;

        CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end(),-EquipDrenagem.Esconsidade());   //--- 1a linha

        EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);

        for (int Linha = 0 ; Linha < Dimensoes[QTD_ELEMENTOS] ; ++Linha)
        {
          LstPontosDesenho.clear();

          Afast += Dimensoes[PAREDE];
          CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end(),-EquipDrenagem.Esconsidade());
          EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);

          LstPontosDesenho.clear();
          Afast += Dimensoes[LARGURA];
          CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end(),-EquipDrenagem.Esconsidade());
          EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);
        }

        Afast += Dimensoes[PAREDE];
        CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end(),-EquipDrenagem.Esconsidade());
        EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);

        LstPontosDesenho.clear();
        CalculaPontosBacias(it, LstPontosDesenho, Afast, LstItSetObjetos.end(), Dimensoes[ALTURA],EquipDrenagem.Esconsidade());   //--- No bueiro geralmente só 2 pontos
        EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);
        DefineSecAvulsaBueiro(EquipDrenagem, false);
      }
      break;
      case CAciTopografico::EQUI_DESCIDA_DAGUA:
      {
        enum {LARGURA, ALTURA, DEGRAU,ESCONSIDADE,ESPESSURA};

        double Dimensoes[7], Afast(0.0);

        EquipDrenagem.PegaDimensoes(&Dimensoes[0]);

        EquipDrenagem.Esconsidade() = VerfEsconsidade(**it, **itRasc, EquipDrenagem.PegaEstacaEixo(),EquipDrenagem.CoordEixo()).first;

        tylstpontos LstPontosDesenho;
        EquipDrenagem.PegaAlinhamentos().clear();

        Afast -= Dimensoes[LARGURA] / 2.0;

        CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end(),0.0);  // EquipDrenagem.Esconsidade());
        EquipDrenagem.Az((*it)->fi);                                                              //--- Neste caso salva o azimute
        EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);
        
        Afast += Dimensoes[LARGURA];
        LstPontosDesenho.clear();
        CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end(),0.0);// EquipDrenagem.Esconsidade());
        EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);

        EquipDrenagem.DefineDegraus();
      }
      break;
      case CAciTopografico::EQUI_CAIXA_COLETORA:
      {
        enum { LARGURA,ESPESSURA };

        double Dimensoes[7], Afast(0.0);

        EquipDrenagem.PegaDimensoes(&Dimensoes[0]);

        lstPontos PontosCaixa;

        EquipDrenagem.PegaAlinhamentos().clear();

        //--- distancias que vão definir os 5 pontos da caixa (começa no meio)

        double Distancias[5] = { Dimensoes[LARGURA] / 2.0,Dimensoes[LARGURA],Dimensoes[LARGURA],Dimensoes[LARGURA],Dimensoes[LARGURA] };

        Ponto PBacia[5] = { **it };

        itlstPontos itPAnterior(PontosCaixa.insert(PontosCaixa.end(), **it));
        itPAnterior->fi = EquipDrenagem.Az(); 
        itPAnterior->fi += Matem::M_PI;
        itlstPontos itX[4];

        for (int C = 0; C < 5; C++)
        {
          itlstPontos itPAtual(PontosCaixa.insert(PontosCaixa.end(), Ponto()));

          IrradiaPonto(Matem::M_PI_2, *itPAnterior, Distancias[C], *itPAtual, true);

          itPAtual->fi = itPAnterior->fi + Matem::M_PI_2;

          if(C < 4) itX[C] = itPAtual;

          itPAnterior++;
        }

        EquipDrenagem.PegaAlinhamentos().emplace_back(PontosCaixa);

        //--- Desenha o X da caixa

        PontosCaixa.emplace(PontosCaixa.end(), Ponto());  //--- Separador ( levanta a pena)

        PontosCaixa.insert(PontosCaixa.end(), *itX[0]);
        PontosCaixa.insert(PontosCaixa.end(), *itX[2]);

        PontosCaixa.insert(PontosCaixa.end(), Ponto());

        PontosCaixa.insert(PontosCaixa.end(), *itX[1]);
        PontosCaixa.insert(PontosCaixa.end(), *itX[3]);

        EquipDrenagem.PegaAlinhamentos().emplace_back(PontosCaixa);
      }
      break;
      case CAciTopografico::EQUI_BACIA_DISSIPACAO:
      {
        enum { LARGURA, COMPRIMENTO, ESPESSURA };

        double Dimensoes[7], Afast(0.0);

        EquipDrenagem.PegaDimensoes(&Dimensoes[0]);

        EquipDrenagem.Esconsidade() = VerfEsconsidade(**it, **itRasc, EquipDrenagem.PegaEstacaEixo(),EquipDrenagem.CoordEixo()).first;

        tylstpontos LstPontosDesenho;
        EquipDrenagem.PegaAlinhamentos().clear();

        lstPontos PontosBacia;

        //--- distancias que vão definir os 5 pontos da caixa (começa no meio)

        double Distancias[5] = { Dimensoes[LARGURA]/2.0,Dimensoes[COMPRIMENTO],Dimensoes[LARGURA],Dimensoes[COMPRIMENTO],Dimensoes[LARGURA] };   

        Ponto PBacia[5] = { **it };

        itlstPontos itPAnterior(PontosBacia.insert(PontosBacia.end(), **it));
        itPAnterior->fi = EquipDrenagem.Az(); // ItAgregado->second.Az();
        if (*itPAnterior == **EquipDrenagem.PegaLista().rbegin())
        {
       //   itPAnterior->fi += Matem::M_PI;
        }
       // if (*itPAnterior == **it) itPAnterior->fi += Matem::M_PI;   //--- Verifica o início ou o fim do onjeto agregado

        itPAnterior->fi += Matem::M_PI;

        for (int C = 0; C < 5; C++)
        {
          itlstPontos itPAtual(PontosBacia.insert(PontosBacia.end(), Ponto()));

          IrradiaPonto(Matem::M_PI_2, *itPAnterior, Distancias[C], *itPAtual, true);

          itPAtual->fi = itPAnterior->fi + Matem::M_PI_2;

          itPAnterior++;
        }

        EquipDrenagem.PegaAlinhamentos().emplace_back(PontosBacia);

        /*
        Afast -= Dimensoes[LARGURA] / 2.0;

        CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end(), Esconsidade);
        EquipDrenagem.Az((*it)->fi);                                                              //--- Neste caso salva o azimute
        EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);

        Afast += Dimensoes[LARGURA];
        LstPontosDesenho.clear();
        CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end(), Esconsidade);
        EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);
        */

        EquipDrenagem.DefineQuadriculado();

      }
      break;
      case CAciTopografico::EQUI_DRENO_PROFUNDO:
      case CAciTopografico::EQUI_TUBULACAO:
      {
        enum {QTD_ELEMENTOS,DIAMETRO,ESPAÇO,PAREDE,AFASTAMENTO};

        double Dimensoes[7], Afast(0.0);

        EquipDrenagem.PegaDimensoes(&Dimensoes[0]);

        Afast = -(Dimensoes[QTD_ELEMENTOS] * (Dimensoes[DIAMETRO]) / 2.0);

        //--- Afast -= Lado == ESQUERDO ?  -Dimensoes[AFASTAMENTO] : Dimensoes[AFASTAMENTO];  

        EquipDrenagem.Esconsidade() = VerfEsconsidade(**it, **itRasc, EquipDrenagem.PegaEstacaEixo(), EquipDrenagem.CoordEixo()).first;

        tylstpontos LstPontosDesenho;
        EquipDrenagem.PegaAlinhamentos().clear();

        for (int Linha = 0; Linha < Dimensoes[QTD_ELEMENTOS]; ++Linha)
        {
          LstPontosDesenho.clear();
          CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end());
          EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);

          Afast += Dimensoes[DIAMETRO];

          LstPontosDesenho.clear();
          CalculaPoligonalTraco(it, LstPontosDesenho, Afast, LstItSetObjetos.end());
          EquipDrenagem.PegaAlinhamentos().emplace_back(LstPontosDesenho);

          Afast += Dimensoes[ESPAÇO];
        }
      }
      break;

    }
  }
}

bool DesPontosGeomView::VerificaEquipDrenagem(CPoint point,Ponto PAtual,CRect Rect,CMenu* pmenu)
{
 // TransfPixelsXY(point, PAtual.x, PAtual.y);
  GetWindowRect(&Rect);

  tymapEquipDrenagem& EquipamentosDrenagem(Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem());

  ittymapEquipDrenagem It(EquipamentosDrenagem.begin());

  while(It != EquipamentosDrenagem.end())
  {
    const lstitsetPontos& LstItSetObjetos(It->second.PegaLista());
    //if (Superficie.EstaRotacionada()) PAtual.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());

    itsetPontos*  pItPontoPego;

    if (!ObjetosDrenagem || (It->second.Tipo == ObjetoAtual))
    {
      if (VerificaNaTangenteEquipDrenagem(It, PAtual, Toleranciax, &pItPontoPego))
      {
        //if(It->second.PegaTipo() == ObjetoAtual)   //--- Se clicou no objeto do tipo selecionado mostra o menu de edição
        {
          PegouEquipDrenagem = true;
          ItEquiDrenagemMarcada = It;

          RedrawWindow();

          if (pmenu)
          {
            if (pmenu->LoadMenu(IDR_MENU6))
            {
              CMenu* pPopup(pmenu->GetSubMenu(0));

              if (pPopup != NULL)
              {
                CPoint PAux(point);
                UltimoPontoPego = PAux;
                ClientToScreen(&PAux);

                pPopup->TrackPopupMenu(TPM_TOPALIGN | TPM_LEFTALIGN, PAux.x + 5, PAux.y + 5, AfxGetMainWnd());
              }
            }
          }
          return true;
        }
      }
    }
    ++It;
  }

  return false;
}

bool DesPontosGeomView::VerificaNaTangenteEquipDrenagem(ittymapEquipDrenagem& It, const Ponto& pAtual, double ToleranciaX,itsetPontos** ppItPontoPego)
{
  unsigned int TipoRestricao(It->second.PegaTipo());
  bool NaTangente(false);

  lstitsetPontos ItLPontos(It->second.PegaLista());

  itlstitsetPontos ItPAnterior(ItLPontos.begin()),ItPAtual(ItPAnterior);

  if(ItLPontos.size() == 1)    //--- Alguns objetos possuem apenas 1 ponto, caixas coletoras por exemplo
  {
    return (hypot((*ItPAnterior)->x - pAtual.x, (*ItPAnterior)->y - pAtual.y) < Toleranciax);
  }

  while(!NaTangente && ++ItPAtual != ItLPontos.end())
  {
    double Raio(hypot((*ItPAnterior)->x - pAtual.x, (*ItPAnterior)->y - pAtual.y)),
           DistPontos(hypot((*ItPAtual)->x - (*ItPAnterior)->x, (*ItPAtual)->y - (*ItPAnterior)->y)),
           DispP2Reta(hypot((*ItPAtual)->x - pAtual.x, (*ItPAtual)->y - pAtual.y));

    if (Raio < Toleranciax) 
    {
      It->second.ItPontoPego = *ItPAnterior;
      return true;                                    //--- Clicou no 1o ponto 
    }

    if((DispP2Reta + Raio - Toleranciax * 2.0) < DistPontos)          //--- O ponto clicado tem q estar dentro da reta do equipamento de drenagem
    {
      double AzReta(CCurHorizontais::CalculaAzimute(**ItPAtual,**ItPAnterior)),
             AzPonto(CCurHorizontais::CalculaAzimute(pAtual,**ItPAnterior)),
             Distancia(fabs(sin(AzPonto - AzReta) * Raio));
    
      NaTangente = Distancia < Toleranciax;

      if (NaTangente)
      {
        It->second.ItPontoPego = *ItPAtual;
        //if(ppItPontoPego) **ppItPontoPego = *ItPAtual;
        return true;
      }
    }
    ItPAnterior = ItPAtual;
  }

  return false;
}

#include "CDiaEditarEquipDrenagem.h"

void DesPontosGeomView::OnPopupEditarDrenagem()
{
  double Dimensoes[8], DimensoesRasc[8];

  ItEquiDrenagemMarcada->second.PegaDimensoes(&Dimensoes[0]);
  ItEquiDrenagemMarcada->second.PegaDimensoes(&DimensoesRasc[0]);

  CdiaEditarEquipDrenagem DiaEditarEquipDrenagem(&Dimensoes[0], ItEquiDrenagemMarcada->second.PegaQtdDimensoes(),
                          ItEquiDrenagemMarcada->first, ItEquiDrenagemMarcada->second.PegaTipo(),this, 
                          ItEquiDrenagemMarcada->second.Esconsidade(), (*ItEquiDrenagemMarcada->second.PegaLista().begin())->fi);

  if(DiaEditarEquipDrenagem.DoModal() == IDOK) 
  {
    const_cast<CEquipDrenagem*>(&ItEquiDrenagemMarcada->second)->PassaDimensoes(&Dimensoes[0]);
    AlterouTerreno = true;

    bool Alterou(false);

    for(int C = 0 ; Alterou == false && C < 7 ; C++) Alterou = Dimensoes[C] != DimensoesRasc[C];
    {
      if(ItEquiDrenagemMarcada->second.PegaTipo() == CAciTopografico::EQUI_BUEIRO || ItEquiDrenagemMarcada->second.PegaTipo() == CAciTopografico::EQUI_BUEIRO_RETANG)
      {
        if(Dimensoes[1] != DimensoesRasc[1])
        {
          ItEquiDrenagemMarcada->second.MudaObjeto(Dimensoes[1] == 1 ? CAciTopografico::EQUI_BUEIRO : CAciTopografico::EQUI_BUEIRO_RETANG,false);
        }
      }
    } 

    if(Alterou) EditarSarjeta();

    if(Dimensoes[7] != 0) forca90GrausBueiro(ItEquiDrenagemMarcada);
  }

  ItEquiDrenagemMarcada = Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end();

  RedrawWindow();
}

void DesPontosGeomView::OnPopupRemoverDrenagem()
{
  POINT PMouse;

  GetCursorPos(&PMouse);
  CPoint PDialogo(PMouse);
  PDialogo.Offset(5,5);

  std::string Mensagem(" Confirma a remoção ");

  Mensagem += CAciTopografico::PegaGeneroNomeEquip(ItEquiDrenagemMarcada->second.PegaTipo()) ? "deste " : "desta ";
  Mensagem += CAciTopografico::Nome(ItEquiDrenagemMarcada->second.PegaTipo());
  Mensagem.erase(Mensagem.find_last_of(' '));
  Mensagem += "? ";

  CDiaRemAciTopog DiaRemAciTopografico(this, PDialogo, Mensagem.c_str());

  if (DiaRemAciTopografico.DoModal() == IDOK)
  {
    //--- Se é bueiro é preciso remover a seção avulsa associada
    
    if (ItEquiDrenagemMarcada->second.Tipo == CAciTopografico::EQUI_BUEIRO || ItEquiDrenagemMarcada->second.Tipo == CAciTopografico::EQUI_BUEIRO_RETANG)
    {
	    SecoesAvulsas.RemoveSecaoAvulsa(CSecaoAvulsa(ItEquiDrenagemMarcada->second.PegaNome(), **ItEquiDrenagemMarcada->second.LstItPontos.begin(), **ItEquiDrenagemMarcada->second.LstItPontos.rbegin(),0.0,0.0));
      AlterouSecoesAvulsas = true;
    }
    
    //--- Sarjeta de offset ou de pista não tem acidente topografico associado

    bool SemAciTtopog(ItEquiDrenagemMarcada->first.find("OFFSE") != std::string::npos || ItEquiDrenagemMarcada->first.find("BORTE") != std::string::npos);
    
    if (!SemAciTtopog)
    {
      //--- Remove o acidente topográfico associado

      Ponto PAtual;

      TransfPixelsXY(PonMousePixelsAtual, PAtual.x, PAtual.y);

      LLDeltaSup& ListasRestricoes(Superficie.PegaListaRestricoes());
      ItCLLDeltaSup ItLRestricaoAtual(ListasRestricoes.begin());

      while (ItLLDeltaSupMarcada == Superficie.PegaListaRestricoes().end() && ItLRestricaoAtual != ListasRestricoes.end())
      {
        if (Superficie.EstaRotacionada()) 
        {
          PAtual.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());
        }

        ItLLDeltaSupMarcada = VerificaNaTangenteRestricoes(ItLRestricaoAtual, **ItEquiDrenagemMarcada->second.LstItPontos.rbegin(), Toleranciax);

        ++ItLRestricaoAtual;
      }

      RemovendoDrenagem = true;

      OnPopupRemover();   //--- Remove o acidente topografico associado

      RemovendoDrenagem = false;
    }

    Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().erase(ItEquiDrenagemMarcada);
    ItEquiDrenagemMarcada = Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end();
  }

  RedrawWindow();
}

void DesPontosGeomView::CalculaPoligonalTraco(citlstitsetPontos it, tylstpontos& LstPontosDesenho, double Afast, citlstitsetPontos itFim,double Escons)
{
  if (fabs(Afast) < 1E-4)
  {
    for (citlstitsetPontos It = it; It != itFim; ++It)
    {
      LstPontosDesenho.emplace_back(**It);
    }

    return;
  }

  double AnguloCompl(Escons != 0.0 ? Matem::M_PI_2 - Escons : 0.0);

  LstPontosDesenho.clear();

  citlstitsetPontos ItPAnterior(it), ItPAtual(it),ItPPosterior;

  double AzAnterior(CCurHorizontais::CalculaAzimuteUTM(**ItPAnterior,**++ItPAtual));

  const_cast<Ponto*>(&(**ItPAnterior))->fi = const_cast<Ponto*>(&(**ItPAtual))->fi = AzAnterior;

  Ponto P1R1, P2R1, P1R2, P2R2;

  P2R1.fi = P1R1.fi = AzAnterior;             //--- Salva o azimute no 1o e no 2o pontos

  AzAnterior += Matem::M_PI_2 - AnguloCompl;
  double Dist(Afast);
  
  if(Escons != 0.0) Dist /= cos(Escons-Matem::M_PI_2);

  IrradiaPonto(AzAnterior,**ItPAnterior, Dist, P1R1,false);
  IrradiaPonto(AzAnterior, **ItPAtual, Dist, P2R1,false);

  P2R2 = P2R1;   //--- No caso de ter só 2 pontos P2R2 é o ponto final

  ItPPosterior = ItPAtual;
  ++ItPPosterior;

  LstPontosDesenho.emplace_back(P1R1);

  while(ItPPosterior != itFim)
  {
    double AZPosterior(CCurHorizontais::CalculaAzimuteUTM(**ItPAtual,**ItPPosterior));

    AZPosterior += Matem::M_PI_2;

    IrradiaPonto(AZPosterior, **ItPAtual, Dist, P1R2,false);
    IrradiaPonto(AZPosterior, **ItPPosterior, Dist, P2R2,false);

    Ponto Intersecao;
  
    //--- Retas Paralelas, neste caso a interseção é o P2R1 (ou P2R2)

    if(CLPontos::CalculaIntersecao(P1R1,P2R1,P1R2,P2R2,Intersecao,false) == true) LstPontosDesenho.emplace_back(Intersecao);
    else LstPontosDesenho.emplace_back(P2R1);

    P1R1 = P1R2;
    P2R1 = P2R2;

    ++ItPAtual;
    ++ItPPosterior;
  }

  LstPontosDesenho.emplace_back(P2R2);
}

void DesPontosGeomView::CalculaPontosBacias(citlstitsetPontos it, tylstpontos& LstPontosDesenho, double Afast, citlstitsetPontos itFim,double AlturaBueiro,double Escon,double AngAla)
{
  double AnguloCompl(Matem::M_PI_2 - Escon);

  LstPontosDesenho.clear();

  citlstitsetPontos ItPAnterior(it), ItProximo(it), ItPPosterior(it),ItPemUso;

  ++ItPPosterior;

  for (int C = 0; C < 2; C++)
  {
    citlstitsetPontos ItPemUso(C == 0 ? ItPAnterior : ItPPosterior);

    double AzAnterior((*it)->fi), Compr(Afast);

    Ponto P1R1, P2R1, P1R2, P2R2;

    AzAnterior += AnguloCompl - Matem::M_PI_2;

    double AngGiro(C == 0 ? Matem::M_PI_2 : -Matem::M_PI_2);

    IrradiaPonto(AzAnterior, **ItPemUso, Afast + 0.60, P1R1, false);
    LstPontosDesenho.emplace_back(P1R1);
    AzAnterior += AngGiro;
    IrradiaPonto(AzAnterior, P1R1, 0.20, P1R2, false);
    LstPontosDesenho.emplace_back(P1R2);
    AzAnterior += AngGiro;
    IrradiaPonto(AzAnterior, P1R2, Afast * 2.0 + 1.20, P2R1, false);
    LstPontosDesenho.emplace_back(P2R1);
    AzAnterior += AngGiro;
    IrradiaPonto(AzAnterior, P2R1, 0.20, P2R2, false);
    LstPontosDesenho.emplace_back(P2R2);

    LstPontosDesenho.emplace_back(P1R1);     //--- Fecha o retangulo

    //--- Calcula os pontos das alas do bueiro

    ittylstpontos itInicioBacia(LstPontosDesenho.insert(LstPontosDesenho.end(), Ponto()));         //--- Marca o inicio da bacia

    AzAnterior = (*it)->fi;
    AzAnterior += AnguloCompl - Matem::M_PI_2;

    Ponto FimBacia1, FimBacia2;

    double Sin2Alfa(sin(AngAla)), Az2(AzAnterior - (C == 0 ? AngAla : -AngAla)),CosAlfa(cos(AngAla/2.0));

    Compr = (AlturaBueiro * 1.5 + 0.30) / Sin2Alfa;  //--- comprimento da ala do bueiro= Altura do bueiro * 1.5, + 0.30 para expessura do bueiro + sobra 
                                                     //--- do aterro na parede da  ala acima da saida do bueiro. Se o bueiro estiver esconso 
                                                     //--- tem q recalcular este 1.5 (rampa do talude de aterro)  #AFAZER #VERIFICAR
                                                     //--- 06-03-2021  -> Não interessa a rampa do talude, vão ser 30cm do mesmo jeito!"

    IrradiaPonto(AzAnterior, **ItPemUso, Afast + 0.50, P1R1, false);
    LstPontosDesenho.emplace_back(P1R1);
    IrradiaPonto(Az2, P1R1, Compr, P2R1, false);
    LstPontosDesenho.emplace_back(P2R1);

    FimBacia1 = P2R1;

    LstPontosDesenho.emplace_back(Ponto());  //--- Separador

    IrradiaPonto(AzAnterior, **ItPemUso, Afast + 0.5 - 0.20 / Sin2Alfa, P1R1, false);

    IrradiaPonto(Az2, P1R1, Compr, P2R1, false);
    LstPontosDesenho.emplace_back(P2R1);

    LstPontosDesenho.emplace_back(P1R1);

    LstPontosDesenho.emplace_back(Ponto());  //--- Separador

    AzAnterior = (*it)->fi;
    AzAnterior -= -AnguloCompl - Matem::M_PI_2;
    Az2 = AzAnterior + (C == 0 ? AngAla : -AngAla);
    
    IrradiaPonto(AzAnterior, **ItPemUso, Afast + 0.50, P1R1, false);
    LstPontosDesenho.emplace_back(P1R1);
    IrradiaPonto(Az2, P1R1, Compr, P2R1, false);
    LstPontosDesenho.emplace_back(P2R1);

    FimBacia2 = P2R1;

    LstPontosDesenho.emplace_back(Ponto()); 

    IrradiaPonto(AzAnterior, **ItPemUso, Afast + 0.50 - 0.20 / Sin2Alfa, P1R1, false);
    LstPontosDesenho.emplace_back(P1R1);
    IrradiaPonto(Az2, P1R1, Compr, P2R1, false);
    LstPontosDesenho.emplace_back(P2R1);

    LstPontosDesenho.emplace_back(Ponto());

    LstPontosDesenho.emplace_back(FimBacia1);
    LstPontosDesenho.emplace_back(FimBacia2);

    if(C == 0) LstPontosDesenho.emplace_back(Ponto());
  }
}

void DesPontosGeomView::CalculaCotasAlinhamento(lstPontos& Alinhamento)
{
  for (auto& RPonto : Alinhamento)
  {
    CalculaCotaPonto(RPonto, Superficie.EstaRotacionada());
  }
}

void DesPontosGeomView::RotacionaLPontos(itlstPontos& itInicio, citlstPontos& itFim,double Angulo, lstPontos& LRotacionado)
{
  LRotacionado.clear();

  for (itlstPontos it = itInicio; it != itFim ; it++)
  {
    if(it->x != INFINITO)
    {
      LRotacionado.emplace_back(it->RotacionaPlano(sin(Angulo),cos(Angulo),Matem::M_PI,true));
    }
  }
}

void DesPontosGeomView::TransladaLPontos(itlstPontos& itInicio, citlstPontos& itFim,const Ponto& De,const Ponto& Para)
{
  double Deltax(Para.x - De.x), Deltay(Para.y-De.y);

  for (itlstPontos it = itInicio; it != itFim; it++)
  {
    if (it->x != INFINITO)
    {
      it->x += Deltax;
      it->y += Deltay;
    }
  }
}

std::pair<double,bool> DesPontosGeomView::VerfEsconsidade(const Ponto& PInicio,const Ponto& PFim, double& EstacaEixo,Ponto& CoordEixo)
{
  if (Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS.size() < 2) return std::pair<double, bool>(Matem::M_PI_2,false);

  const MCurHorizontaisTS& MPIsProjeto(Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS);

  enum {NENHUM_LOCAL,NA_TANGENTE,NA_ESPIRAL_1,NA_CIRCULAR,NA_ESPIRAL_2};

  ItCMCurHorizontaisTS pitMCHAnterior(MPIsProjeto.begin()),pitMCHAtual(pitMCHAnterior);
  bool Achou(false);
  int Local(NENHUM_LOCAL);
  Ponto EixoAnterior,EixoAtual;

  while (!Achou && ++pitMCHAtual != MPIsProjeto.end())
  {
    Local = NA_TANGENTE;

    Achou = CLPontos::CalculaIntersecao(pitMCHAnterior->second.ST, pitMCHAtual->second.TS, PInicio, PFim, CoordEixo, true);

    if (!Achou)   //--- Não achou no trecho reto, procura na curva, (vai pelas cordas a esconsidade não precisa ter muita precisão)
    {
      Local = NA_ESPIRAL_1;

      ittysetSecoesTerrap SecTerrAnterior(SecoesTerraplanagem.find(CSecaoTerrap(pitMCHAtual->second.EstTs.EstVirtual))),
                          SecTerrAtual(SecTerrAnterior);

      if (SecTerrAtual != SecoesTerraplanagem.end())
      {
        EixoAnterior = SecTerrAnterior->PegaCoordEixo();

        if(++SecTerrAtual != SecoesTerraplanagem.end())
        {
          //--- Na verdade não interessa em que parte da curva está. Procura ate o ST.

          for ( ; !Achou && SecTerrAtual->PegaEstaca().EstVirtual <= (pitMCHAtual->second.EstSt.EstVirtual + 0.001) ; ++SecTerrAtual)
          {
            if (SecTerrAtual->PegaEstaca().EstVirtual >= pitMCHAtual->second.EstSc.EstVirtual) Local = NA_CIRCULAR;
            else
            if (SecTerrAtual->PegaEstaca().EstVirtual >= pitMCHAtual->second.EstCs.EstVirtual) Local = NA_ESPIRAL_2;

            EixoAtual = SecTerrAtual->PegaCoordEixo();

            if(EixoAtual.x == INFINITO || EixoAtual.y == INFINITO ||  EixoAtual.z == INFINITO) continue;   //--- Pula os pontos de passagem (não têm secões)
             
            Achou = CLPontos::CalculaIntersecao(EixoAnterior, EixoAtual, PInicio, PFim, CoordEixo, true);

            if(!Achou) EixoAnterior = EixoAtual;
            else EstacaEixo = EixoAnterior.S + hypot(CoordEixo.x - EixoAnterior.x, CoordEixo.y - EixoAnterior.y);
          }
        }
      }
    }

    ++pitMCHAnterior;
  }

  --pitMCHAnterior;

  bool Invertido(false);
  double Esconsidade(INFINITO);

  if (Achou)
  {
    switch (Local)
    {
      case NA_TANGENTE:
      {
        Invertido = Mat.CalculaAreaTriangulo(CoordEixo, pitMCHAnterior->second.ST, PInicio) < 0.0;
        double AzimuteBueiro(Mat.CalculaAzimute(!Invertido ? PInicio : PFim, !Invertido ? PFim : PInicio)),
               DistStAnt(hypot(CoordEixo.x - pitMCHAnterior->second.ST.x, CoordEixo.y - pitMCHAnterior->second.ST.y));

        EstacaEixo = pitMCHAnterior->second.EstSt.EstVirtual + DistStAnt;
        Esconsidade = AzimuteBueiro - pitMCHAnterior->second.ST.fi;

        if (Esconsidade < 0.0) Esconsidade += Matem::M_2_PI;
      }
      break;
      case NA_ESPIRAL_1:
      case NA_CIRCULAR:
      case NA_ESPIRAL_2:
      {
        Invertido = Matem::VerfPontoMaisProximo(PInicio,PFim,pitMCHAtual->second.CC);
        Invertido = !(Invertido ^ (pitMCHAtual->second.Ac > 0.0));
        double AzimuteBueiro(Mat.CalculaAzimute(Invertido ? PFim : PInicio, Invertido ? PInicio : PFim)),   //--- O primeiro ponto tem q estar depois do eixo
               AzimuteEixo(Mat.CalculaAzimute(EixoAnterior,EixoAtual));

        Esconsidade = AzimuteBueiro - AzimuteEixo;
        if (Esconsidade < 0.0) Esconsidade += Matem::M_2_PI;
      }
      break;
    };

    if(Esconsidade> Matem::M_PI)  Esconsidade -= Matem::M_PI;
  }
  else
  {
    //--- Não corta o eixo então o eixo é o ponto inicial

    CoordEixo = PInicio;
  }

  return std::pair<double,bool>(Esconsidade,Invertido);
}

bool DesPontosGeomView::InterRetaEspiralAproxNewton(const CurHorizontal& CurvaAtual,const Ponto& PReta1,const Ponto& PReta2,double LimiteInf, double LimiteSup,Ponto& Inters,int RamoEspiral)
{
  double LimiteInfAtual(LimiteInf),LimiteSupAtual(LimiteSup);
  double DistAtual(LimiteInfAtual + LimiteInfAtual / 2.0);
  int Fim(0);
  Ponto PAtual;

  while (!Fim)
  {
    CalculaPontoEspiral(CurvaAtual,DistAtual,PAtual,RamoEspiral);
    bool LadoAtual(Mat.CalculaAreaTriangulo(PReta1, PReta2, PAtual) > 0.0);

    if (LadoAtual == false) LimiteInfAtual = DistAtual;
    else LimiteSupAtual = DistAtual;

    if(Mat.VerfPontoNaReta(PReta1,PReta2,PAtual,0.01)) 
    {
     Inters = PAtual;
     return true;
    }

    if(DistAtual - LimiteInf < 0.01 || LimiteSup - DistAtual < 0.01) return false;
  }

  return false;
}

void DesPontosGeomView::CalculaPontoEspiral(const CurHorizontal& CurvaAtual, double Dist,Ponto& PCalculado,int RamoEspiral)
{
  double x,y,s;

  CCurHorizontais::CalcxyEsp(CurvaAtual,&x, &y,&s,Dist);

  double Angulo(atan(y/x));

  if(CurvaAtual.Ac < 0.0) Angulo = -Angulo;
  if(RamoEspiral == 1) Angulo = -Angulo;

  if(RamoEspiral == 0) IrradiaPonto(Angulo,CurvaAtual.TS,hypot(x,y),PCalculado);
  else IrradiaPonto(Angulo, CurvaAtual.TS, hypot(x, y), PCalculado);
}

double DesPontosGeomView::CalculaEsconsidadeCurva(Ponto PInicio,Ponto PFim, ItCMCurHorizontaisTS pitMCH)
{
  if (Matem::VerfPontoMaisProximo(PInicio,PFim, pitMCH->second.CC) == false)   // Para calcular no sentido certo o az da reta do bueiro
  {
    Ponto PTemp(PFim);
    PFim = PInicio;
    PInicio = PTemp;
  }

  double AzBueiro(Mat.CalculaAzimute(PInicio,PFim));

  Ponto Intersecao1,Intersecao2;

  Mat.CalcInterRetaCurva(PInicio, PFim, pitMCH->second.CC, pitMCH->second.Raio, Intersecao1, Intersecao2);

  //--- Seleciona a interseção correta, que é a mais proxima
  //--- Na intersecao1 fica a interseção correta

  if(Mat.VerfPontoMaisProximo(Intersecao1,Intersecao2,PInicio) == false) Intersecao1 = Intersecao2;
  
  //if (fabs(AZ1 - AzBueiro) > 0.01) Intersecao1 = Intersecao2;

  double AZCentroInter(CCurHorizontais::CalculaAzimute(Intersecao1, pitMCH->second.CC)),
         Alinhamento(pitMCH->second.Ac < 0.0 ? Matem::M_PI_2 : -Matem::M_PI_2),

         Escons(AZCentroInter - AzBueiro + Alinhamento);

  if(pitMCH->second.Ac > 0.0) Escons -= Matem::M_PI;

  return (Escons); 
}

/* ================ anterior

double DesPontosGeomView::CalculaEsconsidadeCurva(Ponto PInicio,Ponto PFim, ItCMCurHorizontaisTS pitMCH)
{
  if (Matem::VerfPontoMaisProximo(PInicio,PFim, pitMCH->second.CC) == false)   // Para calcular no sentido certo o az da reta do bueiro
  {
    Ponto PTemp(PFim);
    PFim = PInicio;
    PInicio = PTemp;
  }

  double AzBueiro(Mat.CalculaAzimute(PInicio,PFim));

  Ponto Intersecao1,Intersecao2;

  Mat.CalcInterRetaCurva(PInicio, PFim, pitMCH->second.CC, pitMCH->second.Raio, Intersecao1, Intersecao2);

  double AZ1(CCurHorizontais::CalculaAzimute(Intersecao1, PInicio));

  //--- Seleciona a interseção correta
  //--- Na intersecao1 fica a interseção correta

  if (fabs(AZ1 - AzBueiro) > 0.01) Intersecao1 = Intersecao2;

  double AZCentroInter(CCurHorizontais::CalculaAzimute(Intersecao1, pitMCH->second.CC)),
         Alinhamento(pitMCH->second.Ac < 0.0 ? Matem::M_PI_2 : -Matem::M_PI_2);

         double Escons = AZCentroInter - AzBueiro + Alinhamento;

  return (Escons) ; 
}

*/

void DesPontosGeomView::DefineCaixaColetora(std::string& NomeCaixa, ItSSuperficie& ObjetoLigado, ittymapEquipDrenagem ItAgregado, ItLLDeltaSup& pListaAtual)
{
  std::stringstream Temp;
  int Contador(0);

  if (NomeCaixa.size() == 0)
  {
    do
    {
      std::stringstream Temp;

      if (Contador++ > 0) Temp << Contador;

      NomeCaixa = ObjetoLigado->PCentral.Nome;
      NomeCaixa += "-CAIX" + Temp.str();
      Limpa(NomeCaixa);

    } while (Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().find(NomeCaixa) != Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end());
  }

  CEquipDrenagem EquipDrenagem(CAciTopografico::EQUI_CAIXA_COLETORA, NomeCaixa);

  //--- A lista atual é criada com o tipo de objeto ao qual a caixa está agregada, acerta aqui

  Ponto PontoOposto;

  pListaAtual->begin()->TipoObjeto = PontoOposto.AcidenteTopografico = CAciTopografico::EQUI_CAIXA_COLETORA;

  if (pListaAtual->begin()->PCentral == **ItAgregado->second.PegaLista().begin())
  {
    double Temp(ItAgregado->second.Az() + Matem::M_PI_2);   //--- Neste caso o AZ é invertido

    if(Temp > ItAgregado->second.Az() + Matem::M_PI) Temp -= Matem::M_PI;

    pListaAtual->begin()->PCentral.fi = pListaAtual->rbegin()->PCentral.fi = PontoOposto.fi =Temp;
    IrradiaPonto(0.0, **ItAgregado->second.PegaLista().begin(), 0.75, PontoOposto, true);
  }
  else
  {
    pListaAtual->begin()->PCentral.fi = pListaAtual->rbegin()->PCentral.fi = PontoOposto.fi = ItAgregado->second.Az();
    IrradiaPonto(Matem::M_PI, **ItAgregado->second.LstItPontos.rbegin(), 0.75, PontoOposto, true);
  }

  pListaAtual->push_back(PontoOposto);           //--- Insere na lista de acidentes topograficos o segundo ponto da caixa

  EquipDrenagem.InserePonto(ObjetoLigado->PCentral);
  EquipDrenagem.InserePonto(PontoOposto);

  EquipDrenagem.Az(Mat.CalculaAzimute(pListaAtual->begin()->PCentral, pListaAtual->rbegin()->PCentral));

  lstPontos PontosCaixa;

  //--- Define o quadrado da caixa
  //--- distancias que vão definir os 5 pontos da caixa (começa no meio)

  double Distancias[5]{ 0.50,1.0,1.0,1.0,0.50 };

  Ponto PCaixa[5] = { ObjetoLigado->PCentral };

  itlstPontos itPAnterior(PontosCaixa.insert(PontosCaixa.end(), ObjetoLigado->PCentral)), itX[4];
  itPAnterior->fi = ItAgregado->second.Az();

  if (*itPAnterior == **ItAgregado->second.LstItPontos.rbegin()) itPAnterior->fi += Matem::M_PI;   //--- Verifica o início ou o fim do onjeto agregado

  for (int C = 0; C < 5; C++)
  {
    itlstPontos itPAtual(PontosCaixa.insert(PontosCaixa.end(), Ponto()));

    IrradiaPonto(Matem::M_PI_2, *itPAnterior, Distancias[C], *itPAtual, true);

    itPAtual->fi = itPAnterior->fi + Matem::M_PI_2;

    if (C < 4) itX[C] = itPAtual;   //--- Pontos do X da caixa

    itPAnterior++;
  }

  PontosCaixa.emplace(PontosCaixa.end(), Ponto());  //--- Separador ( levanta a pena)

  //--- Desenha o X da caixa

  PontosCaixa.insert(PontosCaixa.end(), *itX[0]);
  PontosCaixa.insert(PontosCaixa.end(), *itX[2]);

  PontosCaixa.insert(PontosCaixa.end(), Ponto());

  PontosCaixa.insert(PontosCaixa.end(), *itX[1]);
  PontosCaixa.insert(PontosCaixa.end(), *itX[3]);

  EquipDrenagem.PegaAlinhamentos().emplace_back(PontosCaixa);

  Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().emplace(EquipDrenagem.PegaNome(), EquipDrenagem);

  NomeCaixa.clear();        //--- Limpa o nome para as próximas iterações com os próximos equipamentos

  AlterouTerreno = true;

  RedrawWindow();
}

void DesPontosGeomView::DefineBaciaAmortecimento(std::string& NomeBacia, ItSSuperficie& ObjetoLigado, ittymapEquipDrenagem ItAgregado, ItLLDeltaSup& pListaAtual)
{
  std::stringstream Temp;
  int Contador(0);

  if (NomeBacia.size() == 0)
  {
    do
    {
      std::stringstream Temp;

      if (Contador++ > 0) Temp << Contador;

      NomeBacia = ObjetoLigado->PCentral.Nome;
      NomeBacia += "-BACIA" + Temp.str();
      Limpa(NomeBacia);

    } while (Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().find(NomeBacia) != Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end());
  }

  CEquipDrenagem EquipDrenagem(CAciTopografico::EQUI_BACIA_DISSIPACAO, NomeBacia);

  Ponto PontoOposto;

  pListaAtual->begin()->TipoObjeto = PontoOposto.AcidenteTopografico = CAciTopografico::EQUI_BACIA_DISSIPACAO;
  pListaAtual->begin()->PCentral.fi = pListaAtual->rbegin()->PCentral.fi = PontoOposto.fi = ItAgregado->second.Az();

  if (pListaAtual->begin()->PCentral == **ItAgregado->second.PegaLista().rbegin())
  {
    IrradiaPonto(Matem::M_PI, **ItAgregado->second.PegaLista().rbegin(), 0.75, PontoOposto, true);
  }
  else
  {
    IrradiaPonto(0.0, **ItAgregado->second.LstItPontos.begin(), 0.75, PontoOposto, true);
  }

  pListaAtual->push_back(PontoOposto);           //--- Insere na lista de acidentes topograficos o segundo ponto da bacia

  EquipDrenagem.InserePonto(ObjetoLigado->PCentral);
  EquipDrenagem.InserePonto(PontoOposto);

  EquipDrenagem.Az(Mat.CalculaAzimute(pListaAtual->begin()->PCentral, pListaAtual->rbegin()->PCentral));
  EquipDrenagem.InserePonto(ObjetoLigado->PCentral);

  lstPontos PontosBacia;

  //--- Define o quadrado da bacia

  enum { LARGURA, COMPRIMENTO };

  double Dimensoes[5] = { 0.0 };

  EquipDrenagem.PegaDimensoes(&Dimensoes[0]);

  double Distancias[5] = { Dimensoes[LARGURA] / 2.0,Dimensoes[COMPRIMENTO],Dimensoes[LARGURA],Dimensoes[COMPRIMENTO],Dimensoes[LARGURA] };
  Ponto PBacia[5] = { ObjetoLigado->PCentral };

  itlstPontos itPAnterior(PontosBacia.insert(PontosBacia.end(), ObjetoLigado->PCentral));
  //itPAnterior->fi = ItAgregado->second.Az();

  //--- As bacias de amortecimento podem ser colocadas em vários equipamentos de drenagem.
  //--- Nos equipamentos compostos por apenas 2 pontos pega o azimute do rbegin do acidente topográfico agregado
  //--- Mas nos equipamentos compostos por mais de 2 pontos é preciso verificar em q ponto a bacia esta sendo colocada, 
  //--- no início ou no fim do equipamento.

  if(ItAgregado->second.PegaLista().size() == 2) 
  {
    itPAnterior->fi = (**ItAgregado->second.PegaLista().rbegin()).fi;
  } 
  else 
  {
    itPAnterior->fi = (pListaAtual->begin()->PCentral ==  **ItAgregado->second.PegaLista().begin()) ? (**ItAgregado->second.PegaLista().begin()).fi : (**ItAgregado->second.PegaLista().rbegin()).fi;
  }

  itPAnterior->fi += Matem::M_PI;

  if (*itPAnterior == **ItAgregado->second.LstItPontos.rbegin()) itPAnterior->fi += Matem::M_PI;   //--- Verifica o início ou o fim do onjeto agregado

  for (int C = 0; C < 5; C++)
  {
    itlstPontos itPAtual(PontosBacia.insert(PontosBacia.end(), Ponto()));

    IrradiaPonto(Matem::M_PI_2, *itPAnterior, Distancias[C], *itPAtual, true);

    itPAtual->fi = itPAnterior->fi + Matem::M_PI_2;

    itPAnterior++;
  }

  EquipDrenagem.PegaAlinhamentos().emplace_back(PontosBacia);
  EquipDrenagem.DefineQuadriculado();
  Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().emplace(EquipDrenagem.PegaNome(), EquipDrenagem);

  NomeBacia.clear();   //--- Limpa o nome para as proximas iterações com os proximos equipamentos

  AlterouTerreno = true;

  RedrawWindow();
}

bool DesPontosGeomView::PegaPontoMaisProximoTangente(Ponto& P1,Ponto* P2)
{
  if(Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS.size() < 2) return false;

  ItCMCurHorizontaisTS ItCHAnte(Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS.begin()), ItCHPost(ItCHAnte);
  bool Achou = false;

  ++ItCHPost;

  while(!Achou && ItCHPost != Trechos.PegaTrechoAtual()->second.mapCurvasHorizontaisTS.end())
  {
    if (Mat.CalcDistPontoReta(ItCHAnte->second.PI, ItCHPost->second.PI, P1, P1.i))
    {
      if(P2)
      {
        Mat.CalcDistPontoReta(ItCHAnte->second.PI, ItCHPost->second.PI, *P2, P2->i);
      }

      return (P1.i < P2->i);
    }
    ++ItCHAnte;
    ++ItCHPost;
  }

  return false;    //--- Ponto fora do projeto
}

void DesPontosGeomView::forca90GrausBueiro(ittymapEquipDrenagem ItBueiro)
{
  //--- Gira o bueiro no eixo o angulo que falta(ou que sobra) para 90 graus

  double AngGirar(Matem::M_PI_2 - ItBueiro->second.Esconsidade());

  Ponto PEixo(ItBueiro->second.CoordEixo()),
        RP1(Mat.R_P(PEixo,**ItBueiro->second.PegaLista().begin())),
        RP2(Mat.R_P(PEixo,**ItBueiro->second.PegaLista().rbegin())),
        NovoLocal;

  //--- Altera o primeiro ponto do bueiro

  ittymapEquipDrenagem itDrenagemMarcada(ItEquiDrenagemMarcada);

  SecoesAvulsas.RemoveSecaoAvulsa(CSecaoAvulsa(ItEquiDrenagemMarcada->second.PegaNome(), **ItEquiDrenagemMarcada->second.LstItPontos.begin(), **ItEquiDrenagemMarcada->second.LstItPontos.rbegin(), 0.0, 0.0));
  Mat.IrradiaPonto(AngGirar, PEixo.x, PEixo.y, RP1.x, RP1.y, NovoLocal);
  ItBueiro->second.ItPontoPego = *ItBueiro->second.PegaLista().begin();
  NovoLocal.AcidenteTopografico = ItBueiro->second.PegaTipo();
  RedefineEquipDrenagem(ItBueiro, NovoLocal);

  //--- Altera o segundo ponto do bueiro

  ItEquiDrenagemMarcada = itDrenagemMarcada;

  SecoesAvulsas.RemoveSecaoAvulsa(CSecaoAvulsa(ItEquiDrenagemMarcada->second.PegaNome(), **ItEquiDrenagemMarcada->second.LstItPontos.begin(), **ItEquiDrenagemMarcada->second.LstItPontos.rbegin(), 0.0, 0.0));
  Mat.IrradiaPonto(AngGirar, PEixo.x, PEixo.y, RP2.x, RP2.y, NovoLocal);
  ItBueiro->second.ItPontoPego = *ItBueiro->second.PegaLista().rbegin();
  RedefineEquipDrenagem(ItBueiro, NovoLocal);
  Ponto P1(*(*ItBueiro->second.PegaLista().begin())),P2((**ItBueiro->second.PegaLista().rbegin()));
  CalculaCotaPonto(P1,Superficie.EstaRotacionada());
  CalculaCotaPonto(P2,Superficie.EstaRotacionada());
  ItBueiro->second.AlteraPontoMantemPosicao(ItBueiro->second.PegaLista().begin(),P1);
  ItBueiro->second.AlteraPontoMantemPosicao(++ItBueiro->second.PegaLista().begin(),P2);
}

void DesPontosGeomView::RedefineEquipDrenagem(ittymapEquipDrenagem ItEquiDrenagem,Ponto& NovoLocal)
{
  Ponto PontoPego(*ItEquiDrenagem->second.ItPontoPego);

  tymapEquipDrenagem& EquipamentosDrenagem(Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem());

  ittymapEquipDrenagem ItEquipAtual(EquipamentosDrenagem.find(ItEquiDrenagem->first));

  if(ItEquipAtual != EquipamentosDrenagem.end())
  {
    Ponto PAMudar(*ItEquipAtual->second.ItPontoPego);
    ItCLLDeltaSup ItLRestricaoAtual(VerfClickRestricao(PAMudar));

    if (ItLRestricaoAtual != Superficie.PegaListaRestricoes().end())
    {
      //--- Muda o ponto no acidente topográfico da superfície

      if (ItEquipAtual->second.PegaTipo() == CAciTopografico::EQUI_BACIA_DISSIPACAO || ItEquipAtual->second.PegaTipo() == CAciTopografico::EQUI_CAIXA_COLETORA)
      {
        //--- Nestes equipamentos tem que mover todos os pontos

        double Dx(NovoLocal.x - PAMudar.x),Dy(NovoLocal.y - PAMudar.y);

        Superficie.DeslocaXYAciTopog(ItLRestricaoAtual, Dx, Dy);
      }
      else
      {
        if (ItEquipAtual->second.PegaTipo() == CAciTopografico::EQUI_BUEIRO || ItEquipAtual->second.PegaTipo() == CAciTopografico::EQUI_BUEIRO_RETANG)
        {
          SecoesAvulsas.RemoveSecaoAvulsa(CSecaoAvulsa(ItEquiDrenagemMarcada->second.PegaNome(), **ItEquiDrenagemMarcada->second.LstItPontos.begin(), **ItEquiDrenagemMarcada->second.LstItPontos.rbegin(), 0.0, 0.0));
        }

        Superficie.MudaXYZPontoAciTopog(ItLRestricaoAtual,NovoLocal, PAMudar);

        ItSSuperficie DeltaSupRemover(PegaPontoMaisProximo(PAMudar, nullptr, nullptr));

        //--- Tem que achar
        //--- Tem que achar, se não achar é erro
        //--- Nos testes nao achou alguns com bacia de dissipaçao
        // --- o if!= end esta aí poe segurança, para estes casos
        //--- VERIFICAR!!!!


        if(DeltaSupRemover != Superficie.pSuperficieAtual->end() && DeltaSupRemover->PCentral == PAMudar) Superficie.RemovePontoInseridoUsuario(DeltaSupRemover);   
      }
    }

    Superficie.InsereDeltaSuperficie(CDeltaSuperficie(NovoLocal,NovoLocal.AcidenteTopografico));
    TrocaPontoEquipDrenagem(ItEquipAtual, NovoLocal);

    //--- Verifica se existe algum equipamento agregado, se existir altera-o tb

    ittymapEquipDrenagem ItAgregado;

    if(0) //--- Não esta dando muito certo
    //if (VerificaPontoDrenagem(PAMudar,&ItAgregado))
    {
      if(ItAgregado->second.PegaTipoEquip() == CEquipDrenagem::EQUIP_AGREGADO)
      {
        ItEquiDrenagemMarcada = ItAgregado;
        ItLLDeltaSupMarcada = VerificaNaTangenteRestricoes(Superficie.PegaListaRestricoes().begin(), **ItEquiDrenagemMarcada->second.PegaLista().begin(), Toleranciax);

       // OnPopupRemoverDrenagem();   não esta dando muito certo

        Superficie.PegaListaRestricoes().push_back(LDeltaSup(1, NovoLocal));
        ItLLDeltaSup ListaAtual = Superficie.PegaListaRestricoes().end();
        ListaAtual--;

       const CDeltaSuperficie CDS(NovoLocal);
       ItSSuperficie ItDeltaSupSobMouse = PegaPontoMaisProximo(NovoLocal, nullptr, nullptr);

        //DefineBaciaAmortecimento(NomeEquiAtual, ItDeltaSupSobMouse, ItEquiDrenagem, ListaAtual);
      }
    }

    ArrastandoEquipDrenagem = false;
    ItEquiDrenagemMarcada = EquipamentosDrenagem.end();

    Superficie.RemovePontoInseridoUsuario(PegaPontoMaisProximo(PontoPego));

    RedrawWindow();
  }
}

void DesPontosGeomView::TrocaPontoEquipDrenagem(ittymapEquipDrenagem ItEquipAtual, const Ponto& NovoLocal)
{
  //--- Muda o ponto no acidente topográfico da superfície

  if (0)
    //if (ItEquipAtual->second.PegaTipo() == CAciTopografico::EQUI_BACIA_DISSIPACAO || ItEquipAtual->second.PegaTipo() == CAciTopografico::EQUI_CAIXA_COLETORA)
  {
    //--- Nestes equipamentos tem que mover todos os pontos

    double Dx(ItEquipAtual->second.ItPontoPego->x - NovoLocal.x), Dy(ItEquipAtual->second.ItPontoPego->y - NovoLocal.y);

    //Superficie.DeslocaXYAciTopog(*ItEquipAtual->second, Dx, Dx);
  }
  else
  {
    citlstitsetPontos It(ItEquipAtual->second.PegaLista().begin());

    //--- Na bacia de dissipação tem que deslocar os 2 pontos
    //--- como todos os pontos serão deslocados não corromperá o set

    if (ItEquipAtual->second.Tipo == CAciTopografico::EQUI_BACIA_DISSIPACAO || ItEquipAtual->second.Tipo == CAciTopografico::EQUI_CAIXA_COLETORA)
    {
      double Dx(NovoLocal.x - ItEquipAtual->second.ItPontoPego->x), Dy(NovoLocal.y - ItEquipAtual->second.ItPontoPego->y);

      while (It != ItEquipAtual->second.PegaLista().end())
      {
        Ponto& PAtual = *(const_cast<Ponto*>(&**It));

        PAtual.x += Dx;
        PAtual.y += Dy;

        ++It;
      }

      DefineAlinEquipDrenagem(ItEquipAtual->second);

      AlterouTerreno = true;

      return;
    }

    while (It != ItEquipAtual->second.PegaLista().end())
    {
      if (**It == *ItEquipAtual->second.ItPontoPego)
      {
        //--- Remove o ponto antigo e insere no novo no acidente topográfico da superfície

        bool Achou(false);

        LLDeltaSup& Restricoes(Superficie.PegaListaRestricoes());

        for (ItLLDeltaSup ItRestAtual = Restricoes.begin(); !Achou && ItRestAtual != Restricoes.end(); ItRestAtual++)
        {
          if (ItRestAtual->size() > 0 && ItRestAtual->begin()->TipoObjeto == ItEquipAtual->second.Tipo)
          {
            for (ItLDeltaSup ItPontoRestAtual = ItRestAtual->begin(); !Achou && ItPontoRestAtual != ItRestAtual->end(); ItPontoRestAtual++)
            {
              if (*ItPontoRestAtual == **It)     //--- este foi o ponto alterado
              {
                ItPontoRestAtual->PCentral.x = NovoLocal.x;
                ItPontoRestAtual->PCentral.y = NovoLocal.y;
                ItPontoRestAtual->PCentral.z = NovoLocal.z;

                Achou = true;
              }
            }
          }
        }
	     //--- Aqui o set pode se corrompido, porisso o AlteraPontoMantemPosicao
	  
        ItEquipAtual->second.AlteraPontoMantemPosicao(It, NovoLocal);

        DefineAlinEquipDrenagem(ItEquipAtual->second);
        ItEquipAtual->second.Az(Mat.CalculaAzimute(**(ItEquipAtual->second.LstItPontos.rbegin()),**(ItEquipAtual->second.LstItPontos.begin())));
                
        //--- Acerta o ponto no acidente topográfico da superficie,  a lista tem apenas 2 pontos, e só um precisa ser alterado
        //--- pra não complicar limpa a lista e insere os 2
        /*
        ListaAtual->clear();
       
        for (itlstitsetPontos it = ItEquipAtual->second.LstItPontos.begin() ; it != ItEquipAtual->second.LstItPontos.end(); ++it)
        {
          ListaAtual->emplace_back(**it);
          ListaAtual->rbegin()->TipoObjeto = (*it)->AcidenteTopografico;

          //--- VERIFICAR!!! e mudar assim esta mt complicado
          //--- Nos equipamentos de drenagem não precisa inserir os pontos na superficie nem nos acidentes topográficos
          //--- Basta que no procurar procure os pontos nos equipamentos de drenagem tb  

          Superficie.RemovePontoInseridoUsuario(PegaPontoMaisProximo(**It));
        }
        */

        AlterouTerreno = true;

        return;
        
      }
      ++It;
    }  
  }
}

bool DesPontosGeomView::VerificaPontoDrenagem(const Ponto& Pon,ittymapEquipDrenagem* ItAgregado)
{
  tymapEquipDrenagem& MapEquipamentos(Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem());

 for (ittymapEquipDrenagem rEquipAtual = MapEquipamentos.begin() ; rEquipAtual != MapEquipamentos.end() ; ++rEquipAtual)
  {
    for (auto& ItLPonto : rEquipAtual->second.PegaLista())
    {
      if(*ItLPonto == Pon) 
      {
        *ItAgregado = rEquipAtual;
        return true;
      }
    }
  }
  return false;
}

//---bool DesPontosGeomView::VerfPontoOutroEquipamento(const Ponto& Pon,const CEquipDrenagem& EquiAtual)
bool DesPontosGeomView::VerfPontoOutroEquipamento(const Ponto& Pon)
{ 
  for (auto& rEquipAtual : Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem())
  {
    if(ItEquiDrenagemMarcada->second == rEquipAtual.second) continue;

    for (auto&  ItPonto : rEquipAtual.second.PegaLista())
    {
      if(Pon == *ItPonto) return true;
    }
  }
  
  return false;
}

ittylstBacias DesPontosGeomView::VerfClicouBaciaHidrografica(ItCLLDeltaSup ItEspigao)
{
/*
  for (ittylstBacias it = Superficie.PegaBacias().begin(); it != Superficie.PegaBacias().end(); it++)
  {
     if(**it->Espigao.LstItPontos.begin() == ItEspigao->begin()->PCentral)    //--- Nos acidentes circulares o promeiro ponto é = ao último
     {                                                                         //--- Não sei pq na superfície não esta assim ---> VERIFICAR
       return it;
     }
  }

  return Superficie.PegaBacias().end();
  */

  if (ItLLDeltaSupMarcada != Superficie.PegaListaRestricoes().end())
  {
    ittylstBacias ItBaciaAtual(Superficie.ProcuraBacia(ItLLDeltaSupMarcada));

    if (ItBaciaAtual != Superficie.PegaBacias().end())
    {
      return ItBaciaAtual;
    }
  }

  return Superficie.PegaBacias().end();
}

ittylstBacias DesPontosGeomView::VerfClicouTalvegueBacia(ItCLLDeltaSup ItEspigao)
{
  for (ittylstBacias it = Superficie.PegaBacias().begin(); it != Superficie.PegaBacias().end(); it++)
  {
    if(it->PegaItTalveguePrincipal() != it->Talvegues.end())
    {
      if (*it->PegaItTalveguePrincipal()->begin() == *ItEspigao->begin())
      {
        return it;
      }
    }

    for (auto& ItTalvegueAtual : it->Talvegues)
    {
      if (*ItTalvegueAtual.begin() == *ItEspigao->begin())
      {
        return it;
      }
    }
  }

  return Superficie.PegaBacias().end();
}

void DesPontosGeomView::DesenhaAciDrenagemMarcada(CDC* pDC)
{
  CPen PenaRemover;
  PenaRemover.CreatePen(PS_SOLID, 4, RGB(190, 190, 190));

  citlstitsetPontos ItPAnterior(ItEquiDrenagemMarcada->second.PegaLista().begin()), ItPAtual(ItPAnterior);

  pDC->SelectObject(PenaRemover);

  pDC->MoveTo(((*ItPAnterior)->x - Deltas[X]) * Escala[X], (AlturaVideo - ((*ItPAnterior)->y - Deltas[Y])) * Escala[Y]);

  while (++ItPAtual != ItEquiDrenagemMarcada->second.PegaLista().end())
  {
    Ponto Ppixels;

    Ppixels.x = (((*ItPAtual)->x - Deltas[X]) * Escala[X]);
    Ppixels.y = (AlturaVideo - ((*ItPAtual)->y - Deltas[Y])) * Escala[Y];

    pDC->LineTo(Ppixels.x, Ppixels.y);
  }
}

void DesPontosGeomView::DefineSecAvulsaBueiro(CEquipDrenagem& Bueiro,bool Invertido)
{
  lstitsetPontos LstItPontos(Bueiro.LstItPontos);

  //--- Verifica se os pontos do bueiro estão invertidos em relação ao estaqueamento

    Ponto P1(Invertido ? **LstItPontos.rbegin() : **LstItPontos.begin()),
          P2(Invertido ? **LstItPontos.begin() : **LstItPontos.rbegin());

  Ponto CoordPolares(Mat.R_P(P1,P2));

  Ponto DeltasMetro((P2.x-P1.x)/ CoordPolares.y, (P2.y - P1.y) / CoordPolares.y),

  //--- Extende na seção avulsa 10 metros para cada lado, para completar o perfil do bueiro

  Deltas10m(DeltasMetro.x * 10.0,DeltasMetro.y * 10.0);

  P1.x -= Deltas10m.x;
  P1.y -= Deltas10m.y;
  P2.x += Deltas10m.x;
  P2.y += Deltas10m.y;

  CoordPolares.y += 10.0;

  //--- Se não cortou o eixo da pista, para todos os efeitos o eixo da pista é o ponto inicial

  if (Bueiro.CoordEixo().x == INFINITO) Bueiro.CoordEixo() = P1;

  ///--- Insere os pontos do eixo na seção avulsa  (A seção avulsa é um objeto a parte do equipamento de drenagem)

  P1.i = Bueiro.CoordEixo().x;
  P2.i = Bueiro.CoordEixo().y;

  CalculaCotaPonto(P1,Superficie.EstaRotacionada());
  CalculaCotaPonto(P2,Superficie.EstaRotacionada());

  CSecaoAvulsa SecAvTemp(Bueiro.PegaNome(), P1, P2, 0.0, 0.0, CSecaoAvulsa::PERFIL_BUEIRO);

  SecAvTemp.Azimute = CoordPolares.x;
  SecAvTemp.Distancia = CoordPolares.y;
  SecoesAvulsas.InsereOrdenada(SecAvTemp, true);
  AlterouSecoesAvulsas = true;

  //--- 1) Acerta os pontos originais do bueiro (os pontos da seção avulsa estão > 10m para cada lado)
  //--- 2) Coloca as coordenadas do eixo nos is de P1 e de P2

  setPontos* pSetPontDefBueiro(const_cast<setPontos*> (&Bueiro.SetPontosAciTopog));
  Ponto P1Bueiro(*pSetPontDefBueiro->begin()),P2Bueiro(*pSetPontDefBueiro->rbegin()),PTemp;

  if (Invertido)
  {
    PTemp = P1Bueiro;
    P1Bueiro = P2Bueiro;
    P2Bueiro = PTemp;
  }

  //--- Insere as coordenadas do eixo no equipamento de drenagem
  //--- Esta sem espaço no objeto, porisso estão indo no i, no futuro mudar para um ponto específico

  P1Bueiro.i = Bueiro.CoordEixo().x;
  P2Bueiro.i = Bueiro.CoordEixo().y;

  //--- Acerta no bueiro as coordenadas dos pontos inicial e final; 

  Bueiro.LimpaPontos();

  Bueiro.InserePonto(P1Bueiro);
  Bueiro.InserePonto(P2Bueiro);
}

bool DesPontosGeomView::LeArqSimbologia(lstPontos& SimbologiaTerrap,const std::string& NomeProjeto)
{
  CFile ArqSimbologia;
  CFileException e;

  std::string NomeCompArquivo(NomeProjeto + ".sbt");

  if (ArqSimbologia.Open(NomeCompArquivo.data(), CFile::modeRead, &e) == 0)
  {
    return false;
  }
  else
  {
    SimbologiaTerrap.clear();

    CArchive ar(&ArqSimbologia, CArchive::load);

    size_t QuanReg;
    Ponto PAtual;

    for (ar >> QuanReg ; QuanReg > 0 ; --QuanReg)
    {
      PAtual << ar;

      SimbologiaTerrap.emplace_back(PAtual);
    }
  }

  return true;
}

bool DesPontosGeomView::CalculaCotaPontoCN(Ponto& pPonto)
{
  //--- Calcula a cota do ponto pela CN
  //--- Esta função é usada quando não tem CN e não tem triangulação (drones)
  //--- Faz um pequena seção que passa pelo ponto se interpola a cota nesta seção
  //--- Tenta uma seção de 50 m em x, se não achar tenta em y, se não achar
  //--- aumenta a distancia em 50m e tenta de novo até 1000m, se não achar retorna false;

  double DistanciaTentar(0.0);
  bool OkX(false),OkY(false);
  int Tentativa(1);
  Ponto PAtualX,PAtualY;

  std::ofstream ArqLog("Teste_SeccionaCN.log", std::ofstream::out | std::ofstream::trunc);
  ArqLog << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

  do
  {
    Ponto PIni(pPonto), PFim(pPonto);

    PAtualX = pPonto;

    //--- Tenta uma seção em x

    DistanciaTentar = 25.0 * Tentativa;

    PIni.x -= DistanciaTentar;
    PFim.x += DistanciaTentar;

    ArqLog << "Tentando em x: dist: " << DistanciaTentar << std::endl;
    
    setPontos SetSecao;

    //--- A rotina de interseção esta com erro quando DY = 0.0,

    PIni.y += DistanciaTentar / 1.0E5;

    Superficie.SeccionaPelaCN(PIni, PAtualX, PFim, SetSecao,true,ArqLog);

    //--- Pega o ponto mais próximo à esquerda e à direita

    Ponto PMaisProxEsq, PMaisProxDir;
    itsetPontos ItProx;

    if (SetSecao.size() > 1)
    {
      if (SetSecao.rbegin()->x > 0.0)
      {
        for (ItProx = SetSecao.begin(); ItProx->x < 0.0; ++ItProx);

        if (ItProx->x >= 0.0 && ItProx != SetSecao.begin())  //--- Tem pontos do lado direito &&  tem pontos do lado esquerdo
        {
          //--- Interpola a cota

          Ponto P1, P2;

          P2 = *ItProx--;
          P1 = *ItProx;

          double DeltaZ(P2.z - P1.z),i(DeltaZ / (P2.x - P1.x));

          PAtualX.z = i * (-P1.x) + P1.z;
          if (fabs(DeltaZ) < 1E-3) PAtualX.Baliza = true;         //--- sinaliza q a cota foi calculada no plano

          OkX = true;
        }
        else ++Tentativa;
      }
      else ++Tentativa;
    }
    else ++Tentativa;

  } while (!OkX && DistanciaTentar < 500.0);

  //--- Faz no eixo Y

  Tentativa = 1;
  PAtualY = pPonto;

  do
  {
    Ponto PIni(pPonto), PFim(pPonto);

    //--- Tenta uma seção em y

    DistanciaTentar = 25.0 * Tentativa;

    PIni.y -= DistanciaTentar;
    PFim.y += DistanciaTentar;

    ArqLog << "Tentando em y: dist: " << DistanciaTentar << std::endl;

    setPontos SetSecao;

    //--- A rotina de interseção esta com erro quando DY = 0.0, DX tb???

    PIni.x += DistanciaTentar / 1.0E5;

    Superficie.SeccionaPelaCN(PIni, PAtualY, PFim, SetSecao,false,ArqLog);

    //--- Pega o ponto mais próximo à esquerda e à direita

    Ponto PMaisProxEsq, PMaisProxDir;
    itsetPontos ItProx;

    if (SetSecao.size() > 1)
    {
      if (SetSecao.rbegin()->x > 0.0)
      {
        for (ItProx = SetSecao.begin(); ItProx->x < 0.0; ++ItProx);    //--- Posiciona no 1o ponto a direita (> 0.0)

        if (ItProx->x >= 0.0 && ItProx != SetSecao.begin())  //--- Tem pontos do lado direito &&  tem pontos do lado esquerdo
        {
          //--- Interpola a cota

          Ponto P1, P2;

          P2 = *ItProx--;
          P1 = *ItProx;

          double DeltaZ(P2.z - P1.z),i(DeltaZ / (P2.x - P1.x));

          PAtualY.z = i * (-P1.x) + P1.z;
          if (fabs(DeltaZ < 1E-3)) PAtualY.Baliza = true;     //--- sinaliza q a cota foi calculada no plano

          OkY = true;
        }
        else ++Tentativa;
      }
      else ++Tentativa;
    }
    else ++Tentativa;

  } while (!OkY && DistanciaTentar < 500.0);

  //--- Assume preferencialmente a cota no eixo que tem cotas diferentes

  pPonto.z = INFINITO;

  if (OkX && OkY)
  {
    if(!PAtualX.Baliza) pPonto.z = PAtualX.z;
    else  pPonto.z = PAtualY.z;
  }
  else
  {
    if(OkX) pPonto.z = PAtualX.z;
    else if (OkY) pPonto.z = PAtualY.z;
  }

  return OkX || OkY;
}
/*
bool DesPontosGeomView::GravaArquivoCN(tylstlstUtm_Coord_3d& LstLstCoorCN)
{
  CFile Arq;
  CFileException e;

  std::string NomeProjeto;

  std::string NomeArq(NomeProjeto + ".cnd");

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
      tylstlstUtm_Coord_3d* pLstLstCoorCN(&LstLstCoorCN);

      //--- não precisa do desembaracaCN o simplifica aceita curvas embaraçadas

           //--- DesembaracaCNs(&pLstLstCoorCN);  //--- dando pb VERIFICAR

           //--- alguma coisa no simplifica esta estragando a triangulação, muitos pontos não estao sendo
           //--- incluidos na triangulação ===>> Verificar

      //CSimpPL::Simplifica(*pLstLstCoorCN);  //--- Dando pb VERIFICAR

      CArcCurvasNivel << 0.0 << 1.0;                 //--- Angulo rotacao e equidistancia das CNs
      CArcCurvasNivel << pLstLstCoorCN->size();

      for (ittylstlstUtm_Coord_3d& itLstCN = pLstLstCoorCN->begin(); itLstCN != pLstLstCoorCN->end(); ++itLstCN)
      {
        CArcCurvasNivel << itLstCN->size();

        for (ittylstUtm_Coord_3d itPontoUTM = itLstCN->begin(); itPontoUTM != itLstCN->end(); ++itPontoUTM)
        {
          Ponto PAtual(itPontoUTM->x, itPontoUTM->y, itPontoUTM->z);

          PAtual >> CArcCurvasNivel;
        }
      }
    }
  }
  return true;
}
*/
void DesPontosGeomView::Rotaciona(double pNovoAngulo,bool pAbsoluto_Relativo)
{
 SetCursor(LoadCursor(NULL, IDC_APPSTARTING));

 Ponto PCentral(Deltas[X] + LarguraVideo / 2.0, Deltas[Y] + AlturaVideo / 2.0);

 double AngComplementar(Mat.M_2_PI - Superficie.PegaAnguloRotacao()),AnguloArotacionar(0.0);

 PCentral.RotacionaPlano(sin(AngComplementar), cos(AngComplementar), -AngComplementar);

 AlterouProjeto = AlterouTerreno = AlterouAngulo = true;
 GetDocument()->SetModifiedFlag(true);

 if (DefinindoLinhaBase != 3)
 {
   if (!ZerarAzimute && AnguloRotacaoAnterior == 0.0)       //--- se true && true o ângulo veio do diálogo
   {
     if (pNovoAngulo < 0.0) pNovoAngulo += 360.0;
     pNovoAngulo = 360.0 - pNovoAngulo;
   }
   else                    //--- Ou é para zerar o azimute ou é para rotacionar o angulo de rotaçao anterior (Está lendo a superfície rotacionada)
   {
     if (ZerarAzimute)
     {
     pNovoAngulo = 0.0;
     pAbsoluto_Relativo = true;
     }
     else  pNovoAngulo = AnguloRotacaoAnterior * 180.0 / Mat.M_PI;
   }

   AnguloArotacionar = (pAbsoluto_Relativo ? (pNovoAngulo / 180.0 * Mat.M_PI) - Superficie.PegaAnguloRotacao() : (pNovoAngulo / 180.0 * Mat.M_PI));

   if (fabs(AnguloArotacionar) < Mat.UmGrau_Rad) return;
 }
 else
 {
 Ponto RP(Mat.R_P(PontosLinhaBase[1], PontosLinhaBase[0]));
 RP.x -= fmod(RP.x, Mat.UmGrau_Rad);
 AnguloArotacionar = RP.x + Mat.M_PI_2;
 if (AnguloArotacionar > Mat.M_2_PI) AnguloArotacionar -= Mat.M_2_PI;
 else  if (AnguloArotacionar < 0.0) AnguloArotacionar += Mat.M_2_PI;
 }

 if (Superficie.PegaAnguloRotacao() + AnguloArotacionar < 0.0)  AnguloArotacionar += Mat.M_2_PI;
 if (Superficie.PegaAnguloRotacao() + AnguloArotacionar > Mat.M_2_PI)  AnguloArotacionar -= Mat.M_2_PI;

 double Sin(sin(AnguloArotacionar)), Cos(cos(AnguloArotacionar));
 Ponto PontoSobMouseAnterior;

 if (DeltaSupSobMouseAnterior != Superficie.pSuperficieAtual->end()) PontoSobMouseAnterior = DeltaSupSobMouseAnterior->PegaPCentral();

 Superficie.RotacionaPlano(Sin, Cos, AnguloArotacionar);
 RotacionaBordoseRestricoes(Sin, Cos, AnguloArotacionar);
 RotacionaSecoes(Sin, Cos, AnguloArotacionar);
 RotacionaSecoesAvulsas(Sin, Cos, AnguloArotacionar);
 RotacionaSecoesTerrap(Sin, Cos, AnguloArotacionar);
 RotacionaSimbologia(Sin, Cos, AnguloArotacionar);

 for (ittymapCTrechos it = Trechos.MapaTrechos.begin(); it != Trechos.MapaTrechos.end(); it++)
 {
   it->second.SuperficieTerr.RotacionaPlano(Sin, Cos, AnguloArotacionar);
   it->second.CurvasHorizontais.RotacionaPlano(Sin, Cos, AnguloArotacionar);
 }

 DeltaSupSobMouseAnterior = DeltaSupAnterior = Superficie.pSuperficieAtual->end();

 SetPontosNoRadar.clear();
 SetPontosRadarInteresse.clear();

 ItInicioSarjeta = ItFimSarjeta = Trechos.PegaTrechoAtual()->second.SuperficieTerr.pSuperficieAtual->end();

 AnguloArotacionar = AnguloRotacao = Superficie.PegaAnguloRotacao();
 Sin = Seno = sin(AnguloRotacao);
 Cos = Coseno = cos(AnguloRotacao);
 Quadrante = floor(AnguloRotacao / Mat.M_PI_2);
 if (Quadrante == 4) Quadrante = 3;

 CriaFontes(true);

 SetCursor(LoadCursor(NULL, IDC_ARROW));

 CalculaNovasDimensoes();
 PCentral.RotacionaPlano(Sin, Cos, AnguloArotacionar);
 CentralizarTela(PCentral);

 DeltaSupSobMouseAnterior = DeltaSupAnterior = Superficie.pSuperficieAtual->end();

 AlterouTerreno = true;
}

int DesPontosGeomView::AjustaDeclinacao()
{
  std::stringstream strMensagem;

  strMensagem << " Agora é preciso ajustar a declinação. Para ajustar a declinação é preciso rotacionar as"
              << " curvas de nível o  ângulo de declinação no sentido contrário, corrigindo-o em relação a"
              << " mapa. Esta correção provoca uma pequena mudança na posição das curvas de nível em "
              << " que também precisa ser corrigida. Precione OK para iniciar";

  monolog.mensagem(-1, strMensagem.str().c_str());

  double NovoAngulo(0.0);
  int Absoluto_Relativo(100);

  CDRotacionar DRotacionar(this, 0.0, &NovoAngulo, &Absoluto_Relativo);

  bool OKRot(false), Cancelou(false), PrimeiraVez(true);

  while (!OKRot && !Cancelou)
  {
    if (DRotacionar.DoModal() == IDOK)
    {
      DPopUpPonto.ShowWindow(SW_SHOW);

      if (PrimeiraVez)
      {
        //--- O Ultimo parametro indica com a barra de progress
        //--- Não está funcionando - @Verificar

        //--- DPopUpPonto.MostraDescricao(700, 200, std::string(" \r\n  Calculando a Superficie...  \r\n "),true);
        DPopUpPonto.MostraDescricao(300, 200, std::string(" \r\n  Calculando a Superficie...  \r\n "));
        DPopUpPonto.RedrawWindow();

        CalculaSuperficie(DPopUpPonto.Progresso());   //--- Esperimentar calcular depois q rotacionar pode dar um efeito final menhor
        Superficie.SetItPontosCNs().clear();          //--- ´pós o cálculo ficou com lixo, limpa, será atualizado quando ler a superfície novamente

        Superficie.Declinacao(NovoAngulo);

        DPopUpPonto.ShowWindow(SW_HIDE);

        PrimeiraVez = false;
      }

      if (fabs(NovoAngulo) > 0.9)
      {
      Rotaciona(NovoAngulo, 1);

      RedrawWindow();

      if (AfxMessageBox("Esta rotação esta ok?", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
      {
        OKRot = true;
        IniciouTranslado = true;
      }
    }
    else
    {
        return 2;
      }
    }
    else
    {
      Cancelou = true;
    }
  }

  return !Cancelou;
}

void DesPontosGeomView::AjustaTranslacao()
{
  std::stringstream strMensagem;

  strMensagem << " Agora faça a translação arrastando as curvas de nível para o lugar correto sobre o mapa. "
              << " Você pode fazer este ajuste usando o mouse ou as teclas de navegação. As teclas de navegação "
              << " proporcionam um ajuste mais preciso pois movem o mapa pixel por pixel. Pressionando <shift> com as "
              << " teclas de navegação você obtem um passo maior." << std::endl 
              << "Pressione o botão esquerdo do mouse para fazer o ajuste movendo o mouse ou <Enter> para fazer o "
              << " ajuste através das teclas de navegação.";

  monolog.mensagem(-1,strMensagem.str().c_str());

  //--- Para ajustar os valor de m_nX n_nY e poder ampliar o mapa durante a transladação
  //--- Mapas em escalas muito grandes precisam ser ampliados durante a transladação
  //--- Para minimizar os erros no deltax/y/Escala

  MudarEscala(EscalaDefault[X] * 100.0, 120);
  IniciouTranslado = 1;                         //-- afxmessagebox nao consome a tecla Enter, entao tem  ter mais uma tecla no keyup

  DeltasIniTranslacao[X] = Deltas[X];
  DeltasIniTranslacao[Y] = Deltas[Y];
  
  //RedrawWindow();
}

void DesPontosGeomView::FinalizaTranslacao(bool ViaTeclaEnter)
{
  double DeltasAjusteUsuario[2] = {0.0,0.0};

  //--- Acerta os novos deltas do mapa (m_nX,m_nY) em pixels, el trlaçãoao inicio das CNs (carta)
  //--- Quando o ajuste é feito elas teclas de navegação o deltax ja vem pronto pois o que muda 
  //--- neste caso  não é o delta x e sim o n_nX

  if(!ViaTeclaEnter)
  {
    DeltasAjusteUsuario[X] = Deltas[X] - DeltasIniTranslacao[X];
    DeltasAjusteUsuario[Y] = Deltas[Y] - DeltasIniTranslacao[Y];
  }

  //-- Carlos R tem a ajustar aqui

  DeltasCarta[X] += DeltasAjusteUsuario[X];
  DeltasCarta[Y] -= DeltasAjusteUsuario[Y];

  AlterouTerreno = AlterouProjeto = true;
  Transladando = TransladandoPorTeclas = FaltaGeoReferenciamento = false;
}
void DesPontosGeomView::OnCotarCNs()
{
  CotandoCNDGN = !CotandoCNDGN;
  RedrawWindow();
}
void DesPontosGeomView::OnUpdateCotarCNs(CCmdUI* pCmdUI)
{
 pCmdUI->Enable(ProjetoGeoreferenciado == SUPERF_GERENMAPAS);
}

void DesPontosGeomView::OnUpdateImportarAciAutoCad(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(((CMainFrame*)AfxGetMainWnd())->TipoSuperficie == SUPERF_TOPOG_ACAD && !ImportouAciTopogAcadOK);
}

#include "CDCotarCNDGN.h"

void DesPontosGeomView::OnUpdateMenucotarcn(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(CotandoCNDGN);
}

void DesPontosGeomView::OnPopupCotarcn()
  {
    int TipoSuperf(((CMainFrame*)AfxGetMainWnd())->TipoSuperficie);

    if (TipoSuperf == SUPERF_GERENMAPAS && itPonMaisProximo->PCentral.Baliza2 & Ponto::CURVA_NIVEL)
    {
      LPontos LTemp;
      LTemp.emplace_back(itPonMaisProximo->PCentral);
      ItCSetItLPontosBegin ItCN(Superficie.SetPontosCurvasNivel.find(CItLPontosBegin(LTemp.begin(), &LTemp)));
      if (ItCN != Superficie.SetPontosCurvasNivel.end())
      {
        double ValorCota(ItCN->pCN->begin()->z);
        if (ValorCota != INFINITO) monolog.mensagem(SEMMENSPADRAO, "Atenção, esta curva de nível já esta cotada!");

        CDCotarCNDGN DCotarCNDGN(&ValorCota);
        if (DCotarCNDGN.DoModal() == IDOK)
        {
          if (ValorCota == 0.0)
          {
            ItCN->pCN->begin()->z = INFINITO;
            RedrawWindow();
            return;
          }
          ItCN->pCN->begin()->z = ValorCota;      //--- cota a CN

          //--- Cota os pontos da CN que estão na superfície 

          for (ItLPontos it = ItCN->pCN->begin(); it != ItCN->pCN->end(); it++)
          {
            it->z = ValorCota;

            ItSSuperficie ItPCN(Superficie.PegaSuperficie().find(*it));

            if (ItPCN != Superficie.PegaSuperficie().end())
            {
              //--- Z não corrompe o set

              const_cast<Ponto*>(&ItPCN->PCentral)->z = ValorCota;

              AlterouTerreno = true;
            }
          }
        }
      }
    }
  RedrawWindow();
}

void DesPontosGeomView::OnImportarAciAutoCad()
{
  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("0"), 0);
  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->ShowWindow(SW_SHOW);

  if (LeRelCamAcadVUE(Superficie.PegaNomeProjeto()))
  {
    Superficie.Serialize(Superficie.PegaNomeProjeto().c_str(), LER, true, false, false, false, false, true);

    RedrawWindow();

    ImportouAciTopogAcadOK = true;
  }
  else
  {
    monolog.mensagem(-1,"Houve erro na importação dos acidentes topográficos do AutoCad.");
  }

  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->ShowWindow(SW_HIDE);

  return;
}

void DesPontosGeomView::OnPopupSaircn()
{
  RedrawWindow();
}

void DesPontosGeomView::OnPopupPontoSair()
{
  RedrawWindow();
}

#include "CDAlterDescPonto.h"

void DesPontosGeomView::OnPopupDescPonto()
{
 CDeltaSuperficie* pDeltaSuperf(const_cast<CDeltaSuperficie*> ((&*itPonMaisProximo)));

 CString* pDescricao(&pDeltaSuperf->PCentral.Descricao);

  CDAlterDescPonto DiaAlteraDescPonto(&pDeltaSuperf->PCentral.Descricao, this);

  DiaAlteraDescPonto.DoModal();

  CString DescPonto;

  int PosAterior(0);
  int PosAtual(pDescricao->Find(';'));
  bool Alterou(false);

  while (PosAtual > -1)
  {
    DescPonto += pDescricao->Mid(PosAterior, PosAtual - PosAterior);
    DescPonto += " \r\n";
    Alterou = true;
    PosAterior = ++PosAtual;

    PosAtual = pDescricao->Find(';', PosAtual);
  }

  if (Alterou) 
  {
    DescPonto += pDescricao->Mid(PosAterior,pDescricao->GetLength()- PosAterior);
    *pDescricao = DescPonto;
  }

  AlterouTerreno = true;

  ClientToScreen(&UltimoPontoPego);

  SetCursorPos(UltimoPontoPego.x, UltimoPontoPego.y);

  RedrawWindow();
}

void DesPontosGeomView::MudouRua(const std::string& NovaRua)
{
  Trechos.TrocaTrecho(NovaRua);

  //--- Se estiver mostrando a terraplanagem e trocar a rua e a rua não tiver terraplanagem não desenha as CN
  //--- Verifica aqui e  se este caso acontecer muda a variável MostrarTerrap para false para mostrar o terreno natural

  if (MostrarTerrap)
  {
    for (ittymapCTrechos it = Trechos.MapaTrechos.begin(); it != Trechos.MapaTrechos.end(); it++)
    {
      if (it->first.compare((((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual())) == 0)
      {
        MostrarTerrap = it->second.SuperficieTerr.PegaSuperficie().size() != 0;
      }
    }
  }

  RedrawWindow();
}

int DesPontosGeomView::LeRelCamAcadVUE(const std::string& NomeProjeto)
{
  CFile Arq;
  CFileException e;

  std::string NomeCompArquivo(NomeProjeto + ".rav");

  if (Arq.Open(NomeCompArquivo.c_str(), CFile::modeRead, &e) == 0)
  {
    monolog.mensagem(13, " O Arquivo de camadas do Autocad->VUE (.rav) não foi encontrado ou não pode ser aberto. "
    "Este arquivo é necessário para a importação dos acidentes topograficos do Autocad. Você terá que importá-lo "
    "de um outro projeto ou criá-lo através do menu <Arquivo/Criar/Camadas Autocad>. Caso você opte por importá-lo "
    "de outro projeto os nomes dos layers dos projetos terão de ser os mesmos. Depois de criar este arquivo você "
    "terá de importar os acidentes topográficos no desenho da superfície através da botão <Importar acidentes Autocad>.");

    monolog.mensagem(-1, " Caso deseja você pode usar o aquivo .rav que se encontra no diretório ...EstWin\\ArquivosBasicos"
      "como um arquivo .rav padrão");     

    return false;
  }
  else
  {
    CArchive arRelCamAcadVUE(&Arq, CArchive::load);
    unsigned int QuanReg(0);
    std::string Camada;
    for (arRelCamAcadVUE >> QuanReg; QuanReg; QuanReg--)
    {
      size_t QuanCampos(0);
      std::string Campo;
      tlstwstr LstCampos;

      for (arRelCamAcadVUE >> QuanCampos; QuanCampos; QuanCampos--)
      {
        arRelCamAcadVUE >> Campo;
        LstCampos.emplace_back(Campo);
      }

      SetRelCamACADVUE.emplace(LstCampos);
    }
  }

  std::string NomeArqTriang, NomeArqAciTopog;

  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("Importando os acidentes topográficos..."), 20);
  LeAciTopogAcad(NomeProjeto, NomeArqTriang, NomeArqAciTopog);
  ImportaAciTopogAcad(NomeArqAciTopog,((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->Progresso);
  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("Gravando os arquivos..."), 80);
  //CotaPontosSemCota();//  A superficie não foi calculada, não da para calcular a cota
  PassaDescAciTopogAcadSuperf();
  GravaArqResAcad();

  ((CDiaThread*)((CEstApp*)AfxGetApp())->pDiaProgresso)->AtualizaDialogo(std::string("0"), 100);

  return AlterouTerreno = AlterouSuperficie = true;
}

int DesPontosGeomView::LeAciTopogAcad(const std::string& NomeArquivo, std::string& NomeArqTRian,std::string& NomeArqAciTopog)
{
  char Buffer[2048] = { 0 };

  std::string Projeto(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase());
  std::string NomeArqIni(Projeto + std::string(".ini"));

  ::GetPrivateProfileString(_T("DadosDesenhos"), _T("ArquivosTopografia"), _T(""), Buffer, 2048, NomeArqIni.c_str());

  std::stringstream StrstrBuffer(Buffer);

  //--- Espaços em branco no string , tem q usar getline

  std::getline(StrstrBuffer, NomeArqTRian,',');
  std::getline(StrstrBuffer, NomeArqAciTopog, ',');

  return true;
}

int DesPontosGeomView::ImportaAciTopogAcad(const std::string& NomeArquivo, CProgressCtrl& Progresso)
{
  tysetPonto SetPontosSuperfAcad;

  //--- Importa as entidades que estão incluidas no arquivo de camadas autocad (cam)

  if (NomeArquivo.size() != 0)
  {
    std::ifstream ArqDXFTop(NomeArquivo);
    std::stringstream strstrMsg;

    if (!ArqDXFTop.is_open() || !ArqDXFTop.good())
    {
      monolog.mensagem(13, CString(NomeArquivo.c_str()) + " (lendo os dados)");
      return false;
    }

    int C(0);
    size_t ContaPontos(0);
    std::string Entitiy;
    int FimEntity(false);

    do
    {
      C++;
      std::getline(ArqDXFTop, Entitiy);
      Limpa(Entitiy, "\t ");
    } while (Entitiy != "ENTITIES");

    std::string Label, Valor;

    AciTopogAcad.clear();

    do
    {
      C++;

      std::getline(ArqDXFTop, Label);
      Limpa(Label, "\t ");

      C++;
      std::getline(ArqDXFTop, Valor);
      Limpa(Valor, "\t ");

      tlstwstr Camada;
      Camada.push_back(Valor);
      auto ItCamAcad(SetRelCamACADVUE.find(Camada));

      if (ItCamAcad != SetRelCamACADVUE.end())
      {
        auto ItCampo(ItCamAcad->begin());

        ItCampo++;
        ItCampo++;

        int AciTopogAtual(atoi(ItCampo->c_str()));
        int GeraArestaforcada(atoi(ItCamAcad->rbegin()->c_str()));

        AciTopogAtual++;   //--- Na classe CAciTopog está com +1 (0 = NENHUMACITOPOG)

        AciTopogAcad.push_back(LPontos());
        auto ItAciAtual(AciTopogAcad.rbegin());
        Ponto P1;
        bool PegandoPonto(false);

        ItCampo++;
        bool Descartar(false);
        do
        {
          C++;
          std::getline(ArqDXFTop, Label);
          Limpa(Label, "\t ");

          C++;
          std::getline(ArqDXFTop, Valor);
          Limpa(Valor, "\t ");

          if (Label == "0")
          {
            PegandoPonto = false;
            break;
          }

          if (Label == "10")
          {
            if (PegandoPonto)   //--- O ponto não tem cota (30) 
            {
              auto ItCampo(ItCamAcad->begin());

              P1.AcidenteTopografico = AciTopogAtual;

              //--- Acontece pontos q deveria ser iguais e são milesimos diferentes
              //--- Isso provova uma série de problemas no VU&E, pontos que deveria ser encontrados no find 
              //--- não são (e vice versa). Estes pontos precisam ser igualados ( Porisso o Find em lugar do find)

              ItSSuperficie ItSuperf(Superficie.Find(P1, 1E-4));
              if (ItSuperf != Superficie.pSuperficieAtual->end())
              {
                Ponto* pPonto(const_cast<Ponto*>(&ItSuperf->PCentral));
                pPonto->Descricao = P1.Descricao = ItCampo->c_str();
                pPonto->AcidenteTopografico = AciTopogAtual;

                P1.x = ItSuperf->PCentral.x;
                P1.y = ItSuperf->PCentral.y;

                if (pPonto->z != INFINITO) P1.z = ItSuperf->PCentral.z;
                else pPonto->z = P1.z;

                ItAciAtual->push_back(P1);

                if (ContaPontos++ % 50 == 0) Progresso.StepIt();
              }
            }
            P1.x = atof(Valor.c_str());
            P1.y = P1.z = INFINITO;
           
            PegandoPonto = true;
          }
          else
          {
            if (Label == "20") P1.y = atof(Valor.c_str());
            else
            {
              if (Label == "30")    //--- O ponto tem conta (pode ser 0)
              {
                P1.z = atof(Valor.c_str());
                P1.AcidenteTopografico = AciTopogAtual;
                std::string Descricao(*(++(ItCamAcad->begin())));
                int Virgula(Descricao.find(','));
                if (Virgula != std::string::npos)   Descricao = Descricao.substr(Virgula + 1);

                P1.Descricao = ItCamAcad->begin()->c_str();
                P1.Descricao += ';';
                P1.Descricao += Descricao.substr(Descricao.find(' ') + 1).c_str();
                ItAciAtual->push_back(P1);

                //--- Acontece pontos q deveriam ser iguais e são milesimos diferentes
                //--- Isso provova uma série de problemas no VU&E, pontos que deveria ser encontrados no find 
                //--- não são (e vice versa). Estes pontos precisam ser igualados ( Porisso o Find em lugar do find)

                ItSSuperficie ItSuperf(Superficie.Find(P1, 1E-4));

                if (ItSuperf != Superficie.pSuperficieAtual->end())
                {
                  Ponto* pPonto(const_cast<Ponto*>(&ItSuperf->PCentral));
                  pPonto->Descricao = P1.Descricao = ItCampo->c_str();
                  pPonto->AcidenteTopografico = AciTopogAtual;

                  P1.x = ItSuperf->PCentral.x;
                  P1.y = ItSuperf->PCentral.y;

                  if (pPonto->z != INFINITO) P1.z = ItSuperf->PCentral.z;
                  else pPonto->z = P1.z;
                }

				
                PegandoPonto = false;

                if (ContaPontos++ % 50 == 0) Progresso.StepIt();
              }
            }
          }
          if (P1.x == 0.0 || P1.y == 0.0)
          {
            Descartar = true;
            AciTopogAcad.pop_back();
          }
        } while (true && !Descartar);

        if (ItAciAtual->size() == 0 || (ItAciAtual->size() == 1 &&  VerfAciUmPonto(AciTopogAtual) == false))
        {
          AciTopogAcad.pop_back();
        }
      }
    } while (ArqDXFTop.good());
  }
  AlterouSuperficie = true;

  return true;
}

int DesPontosGeomView::GravaArqResAcad()
{
  std::string NomeBase(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase());
  CFile ArqResAcad;
  CFileException e;
  std::string NomeArquivo(NomeBase + ".rea");
  if (ArqResAcad.Open(NomeArquivo.c_str(), CFile::modeWrite | CFile::modeCreate, &e) == 0)
  {
    return 0;
  }
  else
  {
    //--- Remove os acidentes com tamanho 0  (não pode acontecer)

    for (auto ItAT = AciTopogAcad.begin(); ItAT != AciTopogAcad.end(); )
    {
      if (ItAT->size() == 0) AciTopogAcad.erase(ItAT++);
      else ItAT++;
    }

    //--- Grava o arquivo de pontos, agora com a descrição

    std::string NomeArqSaidaTopog(NomeBase + "AciTopog.xyz");
    std::ofstream ofs(NomeArqSaidaTopog, std::ofstream::out | std::ofstream::trunc);

    ofs << std::right << std::fixed << std::setfill(' ') << std::setprecision(6);

    //--- Arquivo de acidentes topográficos importados do autocad

    CArchive ar(&ArqResAcad, CArchive::store);
    ar << (double)0.0 << AciTopogAcad.size();      //--- 0.0 angulo de rotação 

    int C(1);

    for (auto ItAT = AciTopogAcad.begin(); ItAT != AciTopogAcad.end(); ItAT++)
    {
      //if(ItAT->size() > 0)
      {
        ar << ItAT->begin()->AcidenteTopografico << ItAT->size();

        for (auto ItPonto = ItAT->begin(); ItPonto != ItAT->end(); ItPonto++)
        {
          *ItPonto >> ar;
           ofs << C++ << " " << ItPonto->y << " " << ItPonto->x << " " << ItPonto->z << " " << ItPonto->Descricao << std::endl;
		}
      }
    }
  }

  return true;
}
//--- Quando o ponto da superficie é obtido de origens diferentes ocorrem erros ´de precisão 
//--- na casa dos milímetros e o comparador do ponto esta na casa dos centésimos de milímetros
//--- Para não precisar de mudar o comparador do ponto o que poderia causar trágicas consequências
//--- Aconteceu quando o programa começou a aceitar o  levantamento do autocad (.dxf) 

ItLDeltaSup DesPontosGeomView::FindListas(LDeltaSup& ListaProcurar, const Ponto& pPonto, double Precisao)
{
  bool Achou(false);

  ItLDeltaSup It(ListaProcurar.begin());

  while (!Achou &&  It != ListaProcurar.end())
  {
    Achou = (fabs(pPonto.x - It->PCentral.x) < Precisao && fabs(pPonto.y - It->PCentral.y) < Precisao);
    if(!Achou) It++;
  }

  return It;
}

/*
void DesPontosGeomView::LeSetForcarAresta()
{
  std::string NomeArq(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase());
  NomeArq += ".rav";

  CFile Arq;
  CFileException e;

  if (Arq.Open(NomeArq.c_str(), CFile::modeRead, &e) == 0)
  {
    std::string NomeArq(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase());

    NomeArq += ".ato";

    if (Arq.Open(NomeArq.c_str(), CFile::modeRead, &e) == 0)
    {
      //--- Arquivo de acidentes topog não existe
      return;
    }
  }

  CArchive CArcAciTopog(&Arq, CArchive::load);
  {
    unsigned int QuanReg;
    SetForcarAresta.emplace(0,0);

    for (CArcAciTopog >> QuanReg; QuanReg; QuanReg--)
    {
      std::string Campo, CampoAnterior;
      size_t QuanCampos;
      int C(0);

      for (CArcAciTopog >> QuanCampos; QuanCampos; QuanCampos--, C++)
      {
        if (C == 0) CArcAciTopog >> CampoAnterior;
        else CArcAciTopog >> Campo;
      }

      int AciTopog(atoi(CampoAnterior.c_str())),
        ForcaAresta(atoi(Campo.c_str()));

      SetForcarAresta.emplace(AciTopog, ForcaAresta);
    }
  }
}
*/
void DesPontosGeomView::LeSetForcarAresta()
{
  std::string NomeArq(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase());
  NomeArq += ".rav";

//void DesPontosGeomView::LeSetForcaAresta(const std::string& NomeProjeto)
//{
  bool ArqAto(false);

  //std::string NomeArq(NomeProjeto);
  //NomeArq += ".rav";

  CFile Arq;
  CFileException e;

  if (Arq.Open(NomeArq.c_str(), CFile::modeRead, &e) == 0)
  {
    ArqAto = true;

    //std::string NomeArq(NomeProjeto);

    std::string NomeArq((((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()));

    NomeArq += ".ato";

    if (Arq.Open(NomeArq.c_str(), CFile::modeRead, &e) == 0)
    {
      //--- Arquivo de acidentes topog não existe
      monolog.mensagem(-1, "Arquivo de acidentes topograficos não encontrado (.ato)");

      return;
    }
  }

  CArchive CArcAciTopog(&Arq, CArchive::load);
  {
    unsigned int QuanReg;

    for (CArcAciTopog >> QuanReg; QuanReg; QuanReg--)
    {
      std::string Campo, CampoAnterior;
      size_t QuanCampos;
      int C(0);

      for (CArcAciTopog >> QuanCampos; QuanCampos; QuanCampos--, C++)
      {
        if (ArqAto)
        {
          if (C == 0) CArcAciTopog >> CampoAnterior;
          else CArcAciTopog >> Campo;
        }
        else
        {
          if (C == 2) CArcAciTopog >> CampoAnterior;
          else CArcAciTopog >> Campo;
        }
      }

      int AciTopog(atoi(CampoAnterior.c_str())),
        ForcaAresta(atoi(Campo.c_str()));

      SetForcarAresta.emplace(AciTopog, ForcaAresta);
    }
  }
}

void DesPontosGeomView::CotaPontosSemCota()
{
  size_t C(0);

  for (itlstlstPontos ItAci = AciTopogAcad.begin(); ItAci != AciTopogAcad.end(); ItAci++)
  {
    C++;

    for (itlstPontos ItPontoAci = ItAci->begin(); ItPontoAci != ItAci->end(); ItPontoAci++)
    {
      if (ItPontoAci->z == 0.0 || ItPontoAci->z == INFINITO )
      {
        CalculaCotaPontoSuperf(*ItPontoAci);
      }
    }
  }
}

void DesPontosGeomView::PassaDescAciTopogAcadSuperf()
{
   //====== carlos ar
   //--- As coordenadas de alguns pontos dos acidentes topográficos do
   //--- Autocad não estão absolutamente iguais as da triangulação, estão 
   //--- com uma diferença a partir da 5 ou 8 ou ate mesmo 12a casa e isto
   //--- causa muitos problemas no vias urbanas & estradas pois estespontos
   //--- deveriam ser iguais e não são encontrados no find, porisso foi criada
   //--- a função Find.
   //--- Para tentar resolver este problema ja na importação dos dados
   //--- Estes pontos são unificados aqui
   //---

  for (itlstlstPontos ItAci = AciTopogAcad.begin(); ItAci != AciTopogAcad.end(); ItAci++)
  {
    for (itlstPontos ItPontoAci = ItAci->begin(); ItPontoAci != ItAci->end(); ItPontoAci++)
    {
      //--- vamos considerar iguais os pontos que estiverem a menos de 1mm
      //--- 

      ItSSuperficie ItS(Superficie.Find(*ItPontoAci,1e-3));

      if (ItS != Superficie.pSuperficieAtual->end())
      {
        //--- Coloca no ponto da superfície a descrição do acidente topografico

       (const_cast<Ponto*>(&ItS->PCentral))->Descricao = ItPontoAci->Descricao;

       //--- Para igualar coloca no acidente topográfico as coordenadas do ponto na superfície

       ItPontoAci->x = ItS->PCentral.x;
       ItPontoAci->y = ItS->PCentral.y;
       ItPontoAci->z = ItS->PCentral.z;
      }
    }
  }
}

bool DesPontosGeomView::CalculaCotaPontoSuperf(Ponto& PontoSemCota)
{
  auto ItPonto(Superficie.Find(PontoSemCota, 1e-4));

  if (ItPonto != Superficie.pSuperficieAtual->end())
  {
    PontoSemCota.z = ItPonto->PCentral.z;

    return true;
  }

  bool Calculou(false);
  Ponto P1, P2, P3;

  if (CCalcSU::PegaTriangulo(P1, P2, P3, PontoSemCota) == 0)  //--- Aqui a superficie não esta rotacionada
  {
    if (Superficie.EstaRotacionada())   //--- se estiver rotaciona os pontos do triangulo
    {
      P1.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());
      P2.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());
      P3.RotacionaPlano(Superficie.PegaSeno(), Superficie.PegaCoseno(), Superficie.PegaAnguloRotacao());
    }

    ItSSuperficie ItSP1(Superficie.Find(P1,1E-4));     //--- Esta superficie ela esta rotacionada. Acertar essa bagunça!!!
    if (ItSP1 != Superficie.pSuperficieAtual->end())
    {
      ItSSuperficie ItSP2(Superficie.Find(P2,1E-4));
      if (ItSP2 != Superficie.pSuperficieAtual->end())
      {
        ItSSuperficie ItSP3(Superficie.Find(P3,1E-4));
        if (ItSP3 != Superficie.pSuperficieAtual->end())
        {
          Matem::CalculaZ(ItSP1->PCentral, ItSP2->PCentral, ItSP3->PCentral, PontoSemCota);
          Calculou = true;
        }
      }
    }
  }
  if (!Calculou)
  {
    CalculaCotaPontoCN(PontoSemCota);    //--- Calcula a cota pelas CNs
  }
  return Calculou;
}

void DesPontosGeomView::DesenhaPontosCotadosSuperf(CDC* ppDC)
{

  //--- Este foi um teste mas não ficou bom
  //--- Pontos sem cota acontecem proncipalmente no AutoCad são pontos de apoio que os projetistas por engano
  //--- colocam na camada de pontos topográficos, deste modo estes pontos são importados como pontos de 
  //--- topografia mas não são. Parece que o melhor mesmo é desprezar estes pontos
  //---
  //--- Fica o código de backup. 
  //--- Estes pontos são gravados no arquivo .psc e lidos no desenho da superfície mas nada
  //--- esta sendo feito com eles, estão apenas sendo informados no log do cálculo da superfície

  return;

  CPen Pena;
  Ponto Ppixels;
 
  Pena.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

  ppDC->SelectObject(&Pena);

  for(auto& PAtual : SetPontoCotadosSuperf)
  {
    Ppixels.x = ((PAtual.x - Deltas[X]) * Escala[X]);
    Ppixels.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y];
   
    //--- circula o ponto em vermelho envolvendo o círculo branco

    ppDC->Arc(Ppixels.x - 5, Ppixels.y - 5, Ppixels.x + 5, Ppixels.y + 5, 0, 0, 0, 0);
  }
}

void DesPontosGeomView::OnUpdateOnDrenagem(CCmdUI* pCmdUI)
{
#ifdef PACOTE_AVANCADO
  pCmdUI->Enable(MostrarTerrap);
#else
  pCmdUI->Enable(false);
#endif
}

void DesPontosGeomView::OnSelecionarCNs()
{
  CDSelecionarCNs DSelecionarCNs(&ProjetoAtual,this,&AcimaDaCota, &AbaixoDaCota);

  DSelecionarCNs.DoModal();

  RedrawWindow();
}

void DesPontosGeomView::DesenhaEspigaoAIncluir(CDC* ppDC)
{
  CPen PenaEspigaoMarcado;
  PenaEspigaoMarcado.CreatePen(PS_SOLID, 4, RGB(255, 32, 32));
  register ItCLDeltaSup ItLEspigaoMarcado(ListaAJuntar.begin());

  ppDC->SelectObject(PenaEspigaoMarcado);

  ppDC->MoveTo((ItLEspigaoMarcado->PCentral.x - Deltas[X]) * Escala[X], (AlturaVideo - (ItLEspigaoMarcado->PCentral.y - Deltas[Y])) * Escala[Y]);

  while (++ItLEspigaoMarcado != ListaAJuntar.end())
  {
    Ponto PAtual = (*ItLEspigaoMarcado).PCentral;   //--- Pega o ponto atual
    Ponto Ppixels;

    Ppixels.x = ((PAtual.x - Deltas[X]) * Escala[X]);
    Ppixels.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y];

    ppDC->LineTo(Ppixels.x, Ppixels.y);
  }
}

void DesPontosGeomView::DesenhaTalvegueMontado(CDC* ppDC)
{
  CPen PenaTalvegueMontado;
  PenaTalvegueMontado.CreatePen(PS_SOLID, 4, RGB(190,190,190));
  register ItCLDeltaSup ItTalvegueMontado(TalvegueMontado.begin());

  ppDC->SelectObject(PenaTalvegueMontado);

  ppDC->MoveTo((ItTalvegueMontado->PCentral.x - Deltas[X]) * Escala[X], (AlturaVideo - (ItTalvegueMontado->PCentral.y - Deltas[Y])) * Escala[Y]);

  while (++ItTalvegueMontado != TalvegueMontado.end())
  {
    Ponto PAtual = (*ItTalvegueMontado).PCentral;   //--- Pega o ponto atual
    Ponto Ppixels;

    Ppixels.x = ((PAtual.x - Deltas[X]) * Escala[X]);
    Ppixels.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y];

    ppDC->LineTo(Ppixels.x, Ppixels.y);
  }
}

void DesPontosGeomView::DesenhaTalveguePrincipal(CDC* ppDC)
{
  CPen PenaTalveguePrincipal;
  PenaTalveguePrincipal.CreatePen(PS_SOLID, 4, RGB(190, 190, 190));
  register ItCLDeltaSup ItTalveguePrincipal(TalveguePrincipal.begin());

  ppDC->SelectObject(PenaTalveguePrincipal);

  ppDC->MoveTo((ItTalveguePrincipal->PCentral.x - Deltas[X]) * Escala[X], (AlturaVideo - (ItTalveguePrincipal->PCentral.y - Deltas[Y])) * Escala[Y]);

  while (++ItTalveguePrincipal != TalveguePrincipal.end())
  {
    Ponto PAtual = (*ItTalveguePrincipal).PCentral;   //--- Pega o ponto atual
    Ponto Ppixels;

    Ppixels.x = ((PAtual.x - Deltas[X]) * Escala[X]);
    Ppixels.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y];

    ppDC->LineTo(Ppixels.x, Ppixels.y);
  }
}

bool DesPontosGeomView::VerfPontoOutroEspigao(const Ponto& Pon)
{
  //  if(ItEquiDrenagemMarcada != Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem().end())
  {
    for (auto& rEquipAtual : Trechos.PegaTrechoAtual()->second.SuperficieTerr.PegaObrasDrenagem())
    {
      if (ItEquiDrenagemMarcada->second == rEquipAtual.second) continue;

      for (auto& ItPonto : rEquipAtual.second.PegaLista())
      {
        if (Pon == *ItPonto) return true;
      }
    }
  }

  return false;
}

void DesPontosGeomView::DesenhaPontosProcurados(CDC* ppDC)
{
  CPen PenaMalha, * pPenaAnterior;
  PenaMalha.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
  pPenaAnterior = ppDC->SelectObject(&PenaMalha);

  std::stringstream strTexto;
  strTexto << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

  int C(0);

  for (auto& PontoAtual : LPontosProcurados)
  {
    C++;

    //--- Desenha a cruz vermelha no ponto

    CPoint PPixels;

    PPixels.x = ((PontoAtual.x - Deltas[X]) * Escala[X]);
    PPixels.y = (AlturaVideo - (PontoAtual.y - Deltas[Y])) * Escala[Y];

    ppDC->MoveTo(PPixels.x, PPixels.y - 20 );

    strTexto.str("");
    strTexto << " " << C;

    ppDC->TextOut(PPixels.x + 15, PPixels.y,strTexto.str().c_str());

    //--- Desenha a cruz, 14 pixels de largura

    ppDC->MoveTo(PPixels.x - 15, PPixels.y);
    ppDC->LineTo(PPixels.x + 15, PPixels.y);
    ppDC->MoveTo(PPixels.x, PPixels.y - 15);
    ppDC->LineTo(PPixels.x, PPixels.y + 15);

    ppDC->Arc(PPixels.x - 4, PPixels.y - 4, PPixels.x + 4, PPixels.y + 4, 0, 0, 0, 0);    //--- Circula o ponto.
  }

  ReleaseDC(ppDC);
}

void DesPontosGeomView::OnMenuremoverbaciahidrol()
{
  ittylstBacias ItBaciaAtual(Superficie.ProcuraBacia(ItLLDeltaSupMarcada));

  if (ItBaciaAtual != Superficie.PegaBacias().end())
  {
    if (AfxMessageBox("Confirma a remoção da bacia hidrográfica associada a este espigão?", MB_OKCANCEL) == IDOK)
    {
      Superficie.PegaBacias().erase(ItBaciaAtual);
      ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();
      AlterouBacias = true;
      RedrawWindow();
    }
  }
}

void DesPontosGeomView::OnMenueditarbaciahidrol()
{
  OnPopupDefinirBacia();
}

void DesPontosGeomView::OnMenuremoverespigao()
{
  RemovendoDrenagem = true;
  OnPopupRemover();
}

void DesPontosGeomView::OnUpdateMenuremoverbaciahidrol(CCmdUI* pCmdUI)
{
  ittylstBacias ItBaciaAtual(Superficie.ProcuraBacia(ItLLDeltaSupMarcada));

  pCmdUI->Enable(ItBaciaAtual != Superficie.PegaBacias().end());
}

void DesPontosGeomView::OnMenubaciasair()
{
  ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();
  RedrawWindow();
}
 
bool DesPontosGeomView::IniciaJuntandoEspigoes_2(ItCLLDeltaSup ItListaProcurar)
{
  ListaAJuntar.clear();
    
  AfxMessageBox("Agora clique no ponto em que deseja a saída deste espigão");
  
  return true;
}

bool DesPontosGeomView::FinalizaJuntandoEspigoes_2(lstItCLLDeltaSup LstItListaProcurar, ItSSuperficie pDeltaSupSobMouse)
{
  bool OK(false), PraFrente(0);
  int Fim(0), ContaPerguntas(0);

  while (!OK && !Fim)
  {
    ItFimJuntandoEspigoes = pDeltaSupSobMouse;

    ItCLLDeltaSup ItLLEspigaoPego(*LstItListaProcurar.begin());

    if (ItLLEspigaoPego != Superficie.PegaListaRestricoes().end())// && ItLLEspigaoMarcado->begin()->TipoObjeto == CAciTopografico::ESPIGAO)
    {
      //--- Posiciona o ponto inicial no espigao

      ItCLDeltaSup ItPInicial(ItLLEspigaoPego->begin()), ItFinal(ItLLEspigaoPego->end());

      ItCLDeltaSup ItPInicialSalvo(ItPInicial);

      while (*ItPInicial != *ItInicioJuntandoEspigoes)
      {
        ItPInicial++;
        if (ItPInicial == ItLLEspigaoPego->end())   //--- A lista é circular...
        {
          ItPInicial = ItLLEspigaoPego->begin();
        }

        if (*ItPInicial == *ItPInicialSalvo) break;   //--- Deu a volta e não achou, o caminho direto não existe
      }
      //--- Posiciona o ponto final no espigao

      if (*ItPInicial == *ItPInicialSalvo)
      {
        if (*ItPInicial != *ItInicioJuntandoEspigoes)   //--- Verifica se não começou exatamente no ponto inicial
        {
          return false;   //--- Deu a volta e não achou
        }
      }

      ItFinal = ItPInicial;

      do
      {
        ListaAJuntar.emplace_back(*ItFinal);

        if (PraFrente == true)
        {
          ItFinal++;

          if (ItFinal == ItLLEspigaoPego->end()) ItFinal = ItLLEspigaoPego->begin();    //--- O Espigão é uma lista circular
        }
        else
        {
          if (ItFinal == ItLLEspigaoPego->begin()) ItFinal = ItLLEspigaoPego->end();

          ItFinal--;
        }
      } while (*ItFinal != *ItFimJuntandoEspigoes);

      ListaAJuntar.emplace_back(*ItFinal);


      RedrawWindow();

      ContaPerguntas++;

      if (ContaPerguntas > 2)
      {
        //AfxMessageBox("Só existem estas 2 possibilidades...");
        ContaPerguntas = 1;

        //return false;
      }

      int Resp(AfxMessageBox("É este o conjunto de pontos que deseja incluir neste espigão?", MB_YESNOCANCEL | MB_ICONEXCLAMATION));

      if (Resp == IDYES)
      {
        ListaAtual->insert(ListaAtual->end(), ListaAJuntar.begin(), ListaAJuntar.end());
        OK = true;
      }
      else
      {
        //--- Lista Atual está o espigão que está sendo criado
        if (Resp == IDNO)
        {
          ListaAJuntar.clear();

          PraFrente = !PraFrente;
        }
        else
        {
          OK = true;
        }
      }
    }
    else
    {
      AfxMessageBox("Este ponto não pertence ao espigão ao qual pertence o ponto inicial. (Antes é preciso clicar nas bifurcações até ele)");
      CriandoRestricao = false;
      Fim = true;
   }
  }

  JuntandoEspigoes = false;
  ItLLEspigaoMarcado = Superficie.PegaListaRestricoes().end();
  ItInicioJuntandoEspigoes = ItFimJuntandoEspigoes = Superficie.pSuperficieAtual->end();
  ListaAJuntar.clear();

  RedrawWindow();

  return true;
}

void DesPontosGeomView::CalculaCotasTalveguesSIG()
{
  for (LDeltaSup& RestAtual : Superficie.PegaListaRestricoes())
  {
    for (CDeltaSuperficie& DeltaSupAtual : RestAtual)
    {
      if (DeltaSupAtual.TipoObjeto == CAciTopografico::TALVEGUE)
      {
        DeltaSupAtual.PCentral.Descricao = "Talvegue";
        DeltaSupAtual.PCentral.Baliza2 = Ponto::ESBOCADO;
        CalculaCotaPonto(DeltaSupAtual.PCentral);
      }
    }
  }
}

void DesPontosGeomView::OnImportarBaciasHidrog()
{
  CString filtro = "Bacias Hidrográficas. (*.dxf) | *.dxf||", NomeProj;

  CFileDialog DiaAbreBacia(TRUE, NULL, ((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase(), OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro, NULL);
  DiaAbreBacia.m_ofn.lpstrTitle = "Vias Urbanas & Estradas - Bacias Hidrográficas - Espigões :";

  if (DiaAbreBacia.DoModal() == IDOK)
  {
    std::string NomeArquivo(DiaAbreBacia.GetPathName().GetBuffer());

    std::ifstream ArqDXFBaciaSIG(NomeArquivo);
    std::stringstream strstrMsg;

    if (!ArqDXFBaciaSIG.is_open() || !ArqDXFBaciaSIG.good())
    {
      monolog.mensagem(13, CString(NomeArquivo.c_str()) + " (lendo os dados)");
      return;
    }

    unsigned int C(0);
    BaciaHidrografica.clear();
    std::string Entitiy;

    do
    {
      C++;
      std::getline(ArqDXFBaciaSIG, Entitiy);
      Limpa(Entitiy, "\t ");
    } while (Entitiy != "ENTITIES");

    std::string Label, Valor;

    do
    {
      C++;

      std::getline(ArqDXFBaciaSIG, Entitiy);
      Limpa(Entitiy, "\t ");

      if (Entitiy == "AcDbEntity")
      {
        Ponto PAtual;

        do
        {
          C++;
          std::getline(ArqDXFBaciaSIG, Label);
          Limpa(Label, "\t ");

          if (Label == "0") break;

          std::getline(ArqDXFBaciaSIG, Valor);
          Limpa(Valor, "\t ");

          if (Valor == "AcDbHatch")
          {
            for (int i = 0; i < 6; i++)
            {
              std::getline(ArqDXFBaciaSIG, Label);
            }

            continue;
          }

          if (Label == "0") break;

          else if (Label == "10") PAtual.x = atof(Valor.c_str());
          else if (Label == "20")
          {
            PAtual.y = atof(Valor.c_str());

            BaciaHidrografica.emplace_back(PAtual);
          }
        } while (true);    //--- Sai no break
      }
    } while (Entitiy != "ENDSEC");

    if (BaciaHidrografica.size() > 2)
    {
      SuavizaEspigao(&BaciaHidrografica);

      CSimpPL::SimplificaXY(BaciaHidrografica);   

      if (*BaciaHidrografica.begin() == *BaciaHidrografica.rbegin())
      {
        Ponto IniTalvegue(*BaciaHidrografica.begin());

        if (Superficie.VerfBaciaImportada(IniTalvegue))
        {
          std::stringstream Msg;

          Msg << "Este espigão já foi importado em ";

          Msg << IniTalvegue.Descricao;
          Msg << " Deseja sobrescrevê-lo?" << std::ends;

          if (dialogar.DoModal(-1, Msg.str().c_str()) != IDOK)
          {
            return;
          }
        }

        lstlstPontos BaciaTemp;
        BaciaTemp.push_back(lstPontos(BaciaHidrografica.begin(), BaciaHidrografica.end()));

        BaciaHidrografica.assign(BaciaTemp.begin()->begin(), BaciaTemp.begin()->end());

        auto ItPAtual(BaciaHidrografica.begin());

        ItPAtual->Nome.Format("PDES-%d", ++UltPontoEsbocado);
        ItPAtual->Descricao = "Espigão";
        ItPAtual->Baliza2 = Ponto::ESBOCADO;   //--- Como se fosse esboçado
        CalculaCotaPonto(*ItPAtual);
        Superficie.InsereDeltaSuperficie(CDeltaSuperficie(*ItPAtual, CAciTopografico::ESPIGAO));
        LLDeltaSup& ListasAtuais(Superficie.PegaListaRestricoes());
        ListasAtuais.push_back(LDeltaSup(1, CDeltaSuperficie(*ItPAtual, CAciTopografico::ESPIGAO)));

        LDeltaSup& EspigaoAtual(*ListasAtuais.rbegin());

        while (++ItPAtual != BaciaHidrografica.end())
        {
          ItPAtual->Nome.Format("PDES-%d", ++UltPontoEsbocado);
          ItPAtual->Descricao = "Espigão";

          ItPAtual->Baliza2 = Ponto::ESBOCADO;   //--- Como se fosse esboçado
          CalculaCotaPonto(*ItPAtual);
          Superficie.InsereDeltaSuperficie(CDeltaSuperficie(*ItPAtual, CAciTopografico::ESPIGAO));
          EspigaoAtual.push_back(*ItPAtual);
        }
        ItLLDeltaSupMarcada = ListasAtuais.end();
        ItLLDeltaSupMarcada--;

        //-- Salva as bacias que ja foram importadas

        Ponto& InicioTalv(*BaciaHidrografica.begin());
        InicioTalv.Descricao = NomeArquivo.c_str();
        Superficie.PegaBaciasImportadas().emplace_back(InicioTalv);

        DefinindoTalveguePrincipal = false;
        ItLLDeltaSup ItTalvegueFoz;

        RedrawWindow();

        if (PegaFozSIG(EspigaoAtual, ItTalvegueFoz, FozSIG))   //--- Estou aqui
        {
          monolog.mensagem(-1, "Click no ponto onde está a nascente do talvegue principal");

          DefinindoTalveguePrincipal = true;

          return;
        }
        else
        {
          monolog.mensagem(-1, "Espigão invalido, a foz não foi encontrada");

          OnMenuremoverespigao();

          Superficie.PegaBaciasImportadas().pop_back();

         // Superficie.RemoveListaDoMapa(ItLLDeltaSupMarcada, ItLLDeltaSupMarcada->begin()->TipoObjeto, ItLLDeltaSupMarcada->begin()->TipoObjeto != CAciTopografico::ESPIGAO);   //--- Não remover os pontos ( Já removeu osque podemser removidos)
         // Superficie.PegaListaRestricoes().erase(ItLLDeltaSupMarcada);

          ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();
        }

        //--- Antigo, pega os talvegues 1 por um (DEIXAR)

        //CDiaIniciaTalvPrincipal DiaIniTalvPrincipal(this);

       // DiaIniTalvPrincipal.DoModal();

       // OnTalveguePrincipal();
      }
      else
      {
        monolog.mensagem(-1, "Espigão invalido.");
      }
    }
  }
}

void DesPontosGeomView::MontaTalveguePrincipal()
{
  setPontos SetPontosTalvegues;

  TalvegueMontado.clear();

  //--- Remove os pontos repetidos do novo talvegue

  for (CDeltaSuperficie& DSAtual : TalveguePrincipal)
  {
    if (SetPontosTalvegues.insert(DSAtual.PCentral).second == true)
    {
      TalvegueMontado.push_back(DSAtual.PCentral);
    }
  }

  RedrawWindow();

  if (dialogar.DoModal(-1,"Confirma este como o talvegue principal? ") == IDOK)
  {
    LLDeltaSup& ListasRestricoes(Superficie.PegaListaRestricoes());

    //--- Remove da superficie os pontos e os talvegues anteriores 

    for(itlstItCLLDeltaSup It = TalveguesDoTalveguePrincipal.begin() ; It != TalveguesDoTalveguePrincipal.end() ;)
    {
      itlstItCLLDeltaSup ItAtual(It),ProxIt(It);
      ProxIt++;
      Superficie.RemoveListaDoMapa(*ItAtual,true,true);
      ListasRestricoes.erase(*ItAtual);
      It = ProxIt;
    }

    //--- Insere na superficie os pontos do novo talvegue passando-os para DeltaSuperficie
    //--- Insere o novo talvegue na lista de restrições
   
    ListasRestricoes.push_back(LDeltaSup());
    LDeltaSup& TalvegueAtual(ListasRestricoes.back());

    for (CDeltaSuperficie& DS : TalvegueMontado)
    {
      Ponto PAtual(DS.PCentral);

      CDeltaSuperficie DSAtual(PAtual, CAciTopografico::TALVEGUE);
      Superficie.InsereDeltaSuperficie(DSAtual);
      TalvegueAtual.emplace_back(DSAtual);
    }

    TalvegueMontado.clear();
    TalveguesDoTalveguePrincipal.clear();
    ItLLDeltaSupMarcada = ListasRestricoes.end();

    AlterouProjeto = AlterouBacias = true;

    RedrawWindow();
  }
}

void DesPontosGeomView::SerializaTalveguePrincipal(int Tipo)
{
  std::string NomeArqTalv(std::string(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()));

  NomeArqTalv += ".tae";     //--- Talvegue sendo editado

  CFile Arq;
  CFileException e;

  
  if (Arq.Open(NomeArqTalv.c_str(),Tipo == LER ? CFile::modeRead : CFile::modeCreate | CFile::modeWrite, &e) == 0)
  {
    return;
  }

  CArchive CArcTalv(&Arq,Tipo == LER ? CArchive::load : CArchive::store);

  if (Tipo == LER)
  {
    TalveguePrincipal.clear();
    size_t QtdPontos(0);
    Ponto PontoAtual;

    for (CArcTalv >> QtdPontos ; QtdPontos ; QtdPontos--)
    {
      PontoAtual << CArcTalv;

      TalveguePrincipal.emplace_back(CDeltaSuperficie(PontoAtual,CAciTopografico::TALVEGUE));
    }
  }
  else
  {
    CArcTalv << TalveguePrincipal.size();

    for (CDeltaSuperficie& DSTalvAtual : TalveguePrincipal)
    {
      DSTalvAtual.PCentral >> CArcTalv;
    }
  }
}

void DesPontosGeomView::DesenhaTalveguePrincipalBacia(CBacia& Bacia, CDC* ppDC)
{
  CPen PenaTalveguePrincipal;
  PenaTalveguePrincipal.CreatePen(PS_SOLID, 4, RGB(0,120,255));

  if (Bacia.PegaItTalveguePrincipal() != Bacia.Talvegues.end() && Bacia.PegaItTalveguePrincipal()->size() > 0)
  {
    ItLDeltaSup ItPontoTalvegue(Bacia.PegaItTalveguePrincipal()->begin());

    ppDC->SelectObject(PenaTalveguePrincipal);

    ppDC->MoveTo((ItPontoTalvegue->PCentral.x - Deltas[X]) * Escala[X], (AlturaVideo - (ItPontoTalvegue->PCentral.y - Deltas[Y])) * Escala[Y]);

    while (++ItPontoTalvegue != Bacia.PegaItTalveguePrincipal()->end())
    {
      Ponto& PAtual(ItPontoTalvegue->PCentral);   //--- Pega o ponto atual
      Ponto Ppixels;

      Ppixels.x = ((PAtual.x - Deltas[X]) * Escala[X]);
      Ppixels.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y];

      ppDC->LineTo(Ppixels.x, Ppixels.y);
    }
  }
}

bool DesPontosGeomView::VerfPontoPertenceLinha(tylstpontos& LinhaPontos, Ponto& Ponto)
{
  ittylstpontos itPAnterior(LinhaPontos.begin()), itPAtual(itPAnterior);
  double Dist(0.0);

  for (itPAtual++; itPAtual != LinhaPontos.end(); itPAtual++, itPAnterior++)
  {
    if (Mat.CalcDistPontoReta(*itPAnterior, *itPAtual, Ponto, Dist))
    {
      if (Dist < 5e-2) return true;
    }
  }

  return false;
}

void DesPontosGeomView::OnUpdateTalveguePrincipal(CCmdUI* pCmdUI)
{
}

afx_msg void DesPontosGeomView::OnPopupCriarTalvPrincipal()
{
  DefinindoTalveguePrincipal = DefinindoTalveguePrincipal == false;
}

afx_msg void DesPontosGeomView::OnUpdatePopupCriarTalvPrincipal(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(VerfClicouBaciaHidrografica(ItLLDeltaSupMarcada) != Superficie.PegaBacias().end());
}

afx_msg void DesPontosGeomView::OnUpdateImportarBaciasHidrog(CCmdUI* pCmdUI)
{
  int TipoSup(((CMainFrame*)AfxGetMainWnd())->TipoSuperficie);
  pCmdUI->Enable(((CMainFrame*)AfxGetMainWnd())->TipoSuperficie == SUPERF_SIG);
}
afx_msg void DesPontosGeomView::OnUpdateImportarTalvegues(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(((CMainFrame*)AfxGetMainWnd())->TipoSuperficie == SUPERF_SIG);
}
void DesPontosGeomView::DesenhaEspigaoMarcado(CDC* ppDC)
{
  if (ItLLEspigaoMarcado != Superficie.PegaListaRestricoes().end() && DefinindoTalveguePrincipal)
  {
    CPen Pena;
    Pena.CreatePen(PS_SOLID, 4, RGB(190, 190, 190));
    register ItCLDeltaSup ItLEspigao(ItLLEspigaoMarcado->begin());

    ppDC->SelectObject(Pena);

    ppDC->MoveTo(((*ItLEspigao).PCentral.x - Deltas[X]) * Escala[X], (AlturaVideo - ((*ItLEspigao).PCentral.y - Deltas[Y])) * Escala[Y]);

    while (++ItLEspigao != ItLLEspigaoMarcado->end())
    {
      Ponto PAtual = (*ItLEspigao).PCentral;   //--- Pega o ponto atual
      Ponto Ppixels;

      Ppixels.x = ((PAtual.x - Deltas[X]) * Escala[X]);
      Ppixels.y = (AlturaVideo - (PAtual.y - Deltas[Y])) * Escala[Y];

      ppDC->LineTo(Ppixels.x, Ppixels.y);
    }
  }
}

void DesPontosGeomView::OnPopupFinTalvPrincipal()
{
  if (DefinindoTalveguePrincipal && TalveguePrincipal.size() > 0)
  {
    MontaTalveguePrincipal();
  }
  
  ItLLDeltaSupMarcada = Superficie.PegaListaRestricoes().end();
  ItBaciaMarcada = Superficie.PegaBacias().end();

  DefinindoTalveguePrincipal = false;

  RedrawWindow();
}

bool DesPontosGeomView::LeArqPCTRL(std::string& NomArq, void* ppLstPCT)
{
  std::string FormatoArqPontos;

  std::string NomeArquivo(NomArq);
  NomeArquivo += ".cfg";

  CConfigProj ConfigProj(NULL, NomeArquivo.c_str());

  ConfigProj.LeArquivo();

  for (int C = 0; C < ConfigProj.FormArqPontos.GetLength(); C++) if (ConfigProj.FormArqPontos[C] != ' ') FormatoArqPontos.push_back(ConfigProj.FormArqPontos[C]);

  std::ifstream ArqCTRL(((CEstApp*)AfxGetApp())->PegaNomeArqPCTRLQGIS(),std::fstream::in);

  tylstUtm_Coord_3d* pLstPCT((tylstUtm_Coord_3d*)ppLstPCT);

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
void DesPontosGeomView::OnImportarTalvegues()
{
  std::string Ext, filtro("Arquivo de Talvegues | *.dxf||");

  CFileDialog DiaAbreArq(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro.c_str(), NULL);
  DiaAbreArq.m_ofn.lpstrTitle = "VU&E- Importar arquivo de Talvegues (SIG) : ";

  std::string NomeArquivo, strEntity("AcDbEntity");

  if (DiaAbreArq.DoModal() == IDOK)
  {
    NomeArquivo = DiaAbreArq.GetPathName();
    std::string NomeReduz(NomeArquivo.substr(NomeArquivo.find_last_of('\\')));

    if(Superficie.VerfArqTalvImportado(NomeReduz))
    {
      monolog.mensagem(-1,"Este arquivo já foi importado");
      return;
    }

    std::ifstream ArqDXFCNSIG(NomeArquivo);
    std::stringstream strstrMsg;

    if (!ArqDXFCNSIG.is_open() || !ArqDXFCNSIG.good())
    {
      monolog.mensagem(13, CString(NomeArquivo.data()) + " (Lendo os dados)");
      return;
    }

    LLDeltaSup& ListasRestricoes(Superficie.PegaListaRestricoes());

    unsigned int C(0);
    std::string Entitiy;
    bool Erro(false);

    do
    {
      C++;

      std::getline(ArqDXFCNSIG, Entitiy);
      Limpa(Entitiy, "\t ");
    } while (Entitiy != "ENTITIES");

    std::string Label, Valor;
    int ContaTalvExistentes(0),ContaTalvegues(0);

    do
    {
      C++;

      std::getline(ArqDXFCNSIG, Entitiy);
      Limpa(Entitiy, "\t ");

      if (Entitiy == strEntity)
      {
        Ponto PAtual;
        double CotaAtual(INFINITO);

        if (ListasRestricoes.size() > 0 && ListasRestricoes.rbegin()->size() < 2)
        {
          ListasRestricoes.pop_back();
        }

      //  if(ListasRestricoes.size() > 100) break;

        ListasRestricoes.emplace_back(LDeltaSup());

        LDeltaSup& TalvegueAtual(*ListasRestricoes.rbegin());

        do
        {
          C++;
          std::getline(ArqDXFCNSIG, Label);
          Limpa(Label, "\t ");

          if (Label == "0") break;

          if (Label == "100")
          {
            /*
              for (int i = 1; i < 7; i++)     //--- Pula o HATCH
              {
                C++;
                std::getline(ArqDXFCNSIG, Label);
              }
              */
          }

          std::getline(ArqDXFCNSIG, Valor);
          Limpa(Valor, "\t ");

          if (Label == "0") break;
          else if (Valor == "AcDbHatch")
               {
                 for(int i = 0 ; i < 6 ; i++)
                 {
                   std::getline(ArqDXFCNSIG, Valor);
                 }
               }
          else if (Label == "10") PAtual.x = atof(Valor.c_str());
          else if (Label == "20")
          {
            PAtual.y = atof(Valor.c_str());

            CalculaCotaPonto(PAtual);
            PAtual.Nome.Format("PDES-%d", ++UltPontoEsbocado);
            PAtual.Descricao = "Talvegue";
            PAtual.Baliza2 = Ponto::ESBOCADO;                           //--- Como se fosse esboçado
            PAtual.AcidenteTopografico = CAciTopografico::TALVEGUE;
            CDeltaSuperficie DSAtual(PAtual, CAciTopografico::TALVEGUE);
            Superficie.InsereDeltaSuperficie(DSAtual);
            TalvegueAtual.emplace_back(DSAtual);
          }
        }while (true);    //--- Sai no break

        if (TalvegueAtual.size() > 1)
        {
          if (*TalvegueAtual.begin() == *TalvegueAtual.rbegin())     //--- Talvegue circular???
          {
            monolog.mensagem(-1,"Talvegues inválidos");
            Superficie.PegaTalveguesImportados().clear();

            Erro = true;
          }

          if (!Erro)
          {
            ContaTalvegues++;

            //-- Para ser igual Verifica os pontos inicial e final

            Ponto IniTalvegue(TalvegueAtual.begin()->PegaPCentral());
            IniTalvegue.i = TalvegueAtual.rbegin()->PCentral.x;
            IniTalvegue.fi = TalvegueAtual.rbegin()->PCentral.y;

            if (Superficie.VerfTalvegueImportado(IniTalvegue, TalvegueAtual.rbegin()->PCentral))
            {

              ContaTalvExistentes++;

              continue;   //--- Vai para o proximo talvegue
            }
            else
            {
              IniTalvegue.Descricao = NomeArquivo.c_str();
              Superficie.PegaTalveguesImportados().emplace_back(IniTalvegue);
            }
          }
        }
      }
    } while (!Erro && Entitiy != "ENDSEC");// && Talvegues.size() < 1000);

    if (!Erro)
    {
      if (ContaTalvExistentes > 0)
      {
        std::stringstream Msg;

        Msg << ContaTalvExistentes << " dos " << ContaTalvegues << " talvegues deste arquivo já existem nesta superficie e não foram importados. ";
        monolog.mensagem(-1, Msg.str().c_str());
      }

      Superficie.NormalizaTalvegues();

      RedrawWindow();

      AlterouBacias = true;
    }
  }
}

bool DesPontosGeomView::PegaFozSIG(LDeltaSup& LstEspigao, ItLLDeltaSup pLTalvegueFoz, Ponto& pFozSIG)
{
  int TotalSaidas(0);
  CBacia BaciaTemp(LstEspigao);

  if (BaciaTemp.MontaBacia(&Superficie))
  {
    TotalSaidas = BaciaTemp.VerificaSaidas(Superficie.pSuperficieAtual);

    if (TotalSaidas == 1)
    {
      pLTalvegueFoz = BaciaTemp.PegaItTalveguePrincipal();
      pFozSIG = BaciaTemp.PegaFoz();

      return true;
    }
  }

  return false;
}

#include <map>
#include "CNoTalvegue.h"
#include "CArvoreN.h"

void DesPontosGeomView::DefineTalveguePrincipalSIG(const Ponto& NascenteFoz)
{
  //--- Controi a árvore de talvegues, a foz tem q ser o nó raiz

  bool Achou(false);
  auto ItTalvegueFozAnt(Superficie.ItTalvFozSIG->begin()), ItTalvegueFozAtual(ItTalvegueFozAnt);

  ItTalvegueFozAtual++;

  do
  {
    Achou = Mat.VerfPontoNaReta(ItTalvegueFozAnt->PCentral, ItTalvegueFozAtual->PCentral, FozSIG, 0.01);
    ItTalvegueFozAnt++;

  } while (!Achou && ++ItTalvegueFozAtual != Superficie.ItTalvFozSIG->end());

  if (Achou)    //--- Ajusta o talvegue inicial ele deve comneçar na foz
  {
    auto ItTalvOriginal(*Superficie.ItTalvFozSIG);

    ObjetoAtual = ItTalvOriginal.begin()->TipoObjeto;    //--- So pode ser talvegue
 
    CArvoreN ArvoreTalvegues(ItTalvOriginal.begin()->PegaPCentral());
    
    int Resposta(IDNO);
    
    int Resultado(ArvoreTalvegues.AchaCaminho(NascenteFoz, Superficie.PegaListaRestricoes(), TalveguePrincipal));

    RedrawWindow();

    if (Resultado == true)
    {
      Resposta = dialogar.DoModal(-1, "É este o talvegue principal desta bacia?");

      if (Resposta == IDOK)
      {
        ArvoreTalvegues.FinalizaAchaCaminho(Superficie.PegaListaRestricoes(), TalveguePrincipal);

        DefinindoTalveguePrincipal = false;

        AlterouBacias = true;

        return;
      }
    }
    else
    {
      monolog.mensagem(-1, "Este Ponto não pertence a um talvegue da bacia. O talvegue principal deve pertencer a bacia.");
    }

    ItListaIniTalvPrincipal = Superficie.PegaListaRestricoes().end();
 
    RedrawWindow();
  }
  else
  {
    monolog.mensagem(-1, "A foz não foi encontrada.");
  }
}

void DesPontosGeomView::SuavizaEspigao(lstPontos* plstEspigao)
{
  if (*plstEspigao->begin() == *plstEspigao->rbegin())
  {
    itlstPontos pAtual(plstEspigao->begin());
    itlstPontos pAnterior(pAtual);

    lstPontos EspigaoSuavizado;

    for (++pAtual; pAtual != plstEspigao->end(); ++pAtual)
    {
      Ponto P((pAnterior->x + pAtual->x) / 2.0, (pAnterior->y + pAtual->y) / 2.0, 0.0);

      EspigaoSuavizado.push_back(P);

      pAnterior = pAtual;
    }

    plstEspigao->clear();

    for(auto& PEspigSuavizadoAtual : EspigaoSuavizado)
    {
      plstEspigao->push_back(PEspigSuavizadoAtual);
    }

    //--- Fecha o poligono

    plstEspigao->push_back(*plstEspigao->begin());
  }
}

LRESULT DesPontosGeomView::MostraListaPontos(WPARAM WP, LPARAM LP)
{
  if (WP != -1)
  {
  std::string Lixo;

  std::stringstream StrStrPontos(BufferPontosProcurados);

  bool PorNome(WP & (1 << CDCoordenadas::TipoBusca::NOME)),PorCoordenadas(WP & (1 << CDCoordenadas::TipoBusca::COORDENADAS)),TrocarXY(WP & (1 << CDCoordenadas::TipoBusca::TROCARXY));

  LPontosProcurados.clear();

  Ponto PAtual;

  if(BufferPontosProcurados.size())
  {
    if(!PorNome)
    {
      do
      {
        if(TrocarXY)
          StrStrPontos >> PAtual.x >> PAtual.y;
        else
          StrStrPontos >> PAtual.y >> PAtual.x;

        if(std::find(LPontosProcurados.begin(), LPontosProcurados.end(),PAtual) == LPontosProcurados.end())
        {
          LPontosProcurados.emplace_back(PAtual);
        }
 
        std::getline(StrStrPontos,Lixo);
        } while (StrStrPontos.good());
    }
    else
    {
      do
      {
        std::string NomePonto("");

        StrStrPontos >> NomePonto;

        if (StrStrPontos.good() && NomePonto.size() > 0)
        {
          ItSSuperficie ItPonto(Superficie.PegaPontoPorNome(NomePonto.c_str()));

          if (ItPonto != Superficie.pSuperficieAtual->end())
          {
            LPontosProcurados.push_back(ItPonto->PegaPCentral());
          }
          else
          {
            monolog.mensagem(-1,"Ponto não encontrado na superfície.");

            return 0;
          }
        }
      }while(StrStrPontos.good());
    }

    Deltas[X] = LPontosProcurados.begin()->x;
    Deltas[Y] = LPontosProcurados.begin()->y;
  
    DeltasReais.x = Deltas[X] -= LarguraVideo / 2.0;
    DeltasReais.y = Deltas[Y] -= AlturaVideo / 2.0;
  }

  OnArrastouDesenho();

  MudarEscala(Escala[X] * 100.0);    //--- Acerta o mapa

  RedrawWindow();
}
  else pDCoordenadas = nullptr;    //--- Chamou Destruindo DCoordenadas

  return 0;
}