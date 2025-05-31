// DesSecaoView.cpp : implementation file
//
#pragma warning(disable: C4244)  //conversion from 'double' to 'long', possible loss of data
#include "stdafx.h"
#include "est.h"
#include "xymouse.h"
#include "afxtempl.h"
#include "carqterreno.h"
#include "math.h"
#include "Estaca.h"
#include "cponto.h"
#include "clpontos.h" 
#include "perfil.h"
#include "supercstring.h"
#include "carqsecoesavulsas.h"
#include <map>
#include "csecaoavulsa.h"
#include "csecoesavulsas.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtalcorte.h"
#include "carqtalaterro.h"
#include "ctalcorte.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include "clistaperfis.h"
#include <list>
#include "csecao.h"
#include "clistasecoes.h"
#include "carqgradientes.h"
#include <set>
#include <string>
#include "cgradientes.h" 
#include "dselsecao.h"
#include "comboprocdessecao.h"
#include "listamedicoes.h"
#include "dvolumes.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "Projeto.h"
#include "calcnota.h"
#include <string>
#include "meleditbox.h"
#include "cdpopuppontos.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
#include "carqcurhorizontais.h"
#include "ccurhorizontal.h"
#include "Mat.h"
#include "ccurhorizontais.h"
#include "carqpistasfixas.h"
#include "cpistasfixas.h"
#include "CTipoSolos.h"
#include "CArqTiposSolos.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include "CArqFurosSondagem.h"
#include <algorithm>
#include "CPerfilGeologico.h"
#include "CDadosBacias.h"
#include "CDeltaSuperficie.h"
#include "SetxPontos.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CAcidentesTopograficos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "DesSecaoView.h"
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "dentraponto.h" 
#include "math.h"
#include "ddeslizantes.h"
#include "deslizantesgeom.h"
#include "xygreide.h"
#include "palette.h"
#include "mytoolbar.h"
#include "diapontosgeom.h"
#include "childfrm.h"
#include "monologo.h"
#include "string.h"
#include "CalcVolumes.h"
#include "carqcurverticais.h"
#include "ccurverticais.h"
#include "CArqOAC.h"
#include "COAC.h"
#include "CArqOAE.h"
#include "COAE.h"
#include "CMemDC.h"
#include "desperfilview.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <iomanip> 
#include <strstream>   
#include "dialogo.h"
#include "DCalcNotaVolumesDireto.h"
#include "DSecionar.h"
#include <afxmt.h>
#include "mat.h"
#include "Mat.h"
#include "CDiaGravarPonInsSecoes.h"
#include <sstream>
#include <iomanip>
#include "ImageListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define X        0
#define Y        1
#define ESQUERDO 0
#define DIREITO  1
#define CORTE    0
#define ATERRO   1
#define IGUAL    0
#define GRAVAR   1
#define LER      0  
#define NENHUM  -1

#define	WM_CLOSE_TOOLBAR (WM_APP+100)

extern Matem Mat;

void inline RotacionaPontoPlano(double Sin,double Cos,double Angulo,double* x,double* y)
{
  //                   Matriz de rotação

  /*
  x’ = x . cos (fi)  -  y . sin(fi)

  y’ = x . sin (fi)  +  y . cos(fi)
  */

  double xTemp(*x * Cos - *y * Sin),yTemp(*x * Sin + *y * Cos);

  *x = xTemp;
  *y = yTemp;
}

extern class monologo monolog;    
extern class dialogo dialogar;
extern class Matem Mat;

CEvent* Evento = NULL;         //--- Evento que sinalizará o fim dos calculo da nota de serviço para poder iniciar o cálculo dos volumes

/////////////////////////////////////////////////////////////////////////////
// DesSecaoView
IMPLEMENT_DYNCREATE(DesSecaoView, CView)

  DesSecaoView::DesSecaoView() : EMedicaoD(((CMainFrame*) AfxGetMainWnd())->EMedicao),Secoes((CString)((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()),
  ArrastandoPonto(FALSE),Arrastando(FALSE),DeltaYMetro(0.0),PAtual(NULL),NomeProjeto(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()),
  TaludesCorte(NomeProjeto),TaludesPavimento(NomeProjeto),TaludesAterro(NomeProjeto),AlterouSecao(FALSE),PrimeiraAlteracao(FALSE),
  Gradientes(NomeProjeto),Alargamentos(NomeProjeto),CurvasHorizontais(NomeProjeto, SecoesTipo),
  ComPerfil(((CMainFrame*)AfxGetMainWnd())->GreideAtivo),SecoesTipo(NomeProjeto),AlterouSecoes(FALSE),TerminarArrasto(FALSE),
  ListaMedicoes(NomeProjeto,EMedicaoD), DVolumes(this,((CMainFrame*)AfxGetMainWnd())->GreideAtivo ? COM_DES_PERFIL : ((CMainFrame*)AfxGetMainWnd())->DesPontosGeo ? COM_DES_PONTOS : COM_DES_SECOES,NomeProjeto.GetBuffer()),
  DPopUpPonto(NULL,RGB(255,255,166)),SecAnterior(NULL),POSPonAnterior(NULL), AltPavimento(NomeProjeto,CString(".pav")),PistasFixas(NomeProjeto),
  CanteirosCentrais(NomeProjeto),DesenharSecAvulsas(false),AltLimpeza(NomeProjeto,CString(".lim")),CotaInferior(0.0),CursorAtual(IDC_HAND),SecaoAtual(NULL),
  PerfilGeologico(NomeProjeto.GetBuffer()),DesenharPerfisGeologicos(true),Trecho(((CMainFrame*) AfxGetMainWnd())->PegaRuaAtual()),DesenharAciTopog(true),
  PAciTopogAtual(NULL),SecoesAvulsas(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual())),InseriuPontos(false), DesenharTalvegues(false),
  DesenharHistogramas(false),itBaciaAtual(BaciasHidrograficas.end()), xParticaoAtual(INFINITO), Particionando(false),AlterouBacias(false),
  AlterouPerfilTalvegue(false),
  EscalaX10(false)
{
  EscalaSecoes[X] = EscalaSecoes[Y] = EscalaRazaoTalvegue[X] = EscalaRazaoTalvegue[Y] = EscalaRazaoHpsometrica[X] = EscalaRazaoHpsometrica[Y] = INFINITO;
  
  TaludesPavimento.TaludesDefault();
  TaludesCorte.TaludesDefault();
  TaludesAterro.TaludesDefault();

  Secoes.DefRetangDes();                   //--- Define os retângulos mínimos para desenho das secões.
  Secoes.CalculaIs();                      //--- Calcula os is das retas das seções.

  //--- Calcula os is das medições

  POSITION MedicaoAtual = ListaMedicoes.GetHeadPosition();

  if(MedicaoAtual) UltMedicao = ListaMedicoes.GetHead();   //-- Salva a última medicao (para alterações)

  while (MedicaoAtual)
  {
    CListaSecoes& LMedAtual = *ListaMedicoes.GetNext(MedicaoAtual); 
    LMedAtual.CalculaIs();
  }

  CMainFrame* MainFrame((CMainFrame*) AfxGetMainWnd());

  MainFrame->InsereSecoes(&Secoes,this);                  //--- Ponteiro para ser usado no desenho do perfil, que 
                                                          //--- precisa envias mensagems para as seções,como mudar a seçao atual e 
                                                          //--- informar sobre alterações no greide.
  DVolumes.AtualizaVolumeTotal(Secoes.PegaVolTotCorte(),Secoes.PegaVolTotAterro(),Secoes.PegaVolTotEmpola());

  PerfilGeologico.Serialize(NomeProjeto.GetBuffer(),LER);

  DPopUpPonto.Create(); 

  LeBaciasHidrograficas(MainFrame->m_sNomeComp.GetBuffer());
 // ConverteSecoesAvulsasSecoesTrans();  complicou... aguardar
}

DesSecaoView::~DesSecaoView()
{
  CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());

  if(AlterouSecoes) GravarSecoes(true);
  if(AlterouBacias) GravaBaciasHidrograficas(MainFrame->m_sNomeComp.GetBuffer(),true);

  std::string NomeArquivo(MainFrame->PegaProjetoAtual() + std::string(".ini"));

  //--- Salva o estado da janela, tamanho, combos toolbars etc.

  CString CSDados;

  std::stringstream StrStrDados;

  StrStrDados << DesenharPavimento << ',' << DesenharTerraplanagem << ',' << DesenharTerreno << ',' << DesenharMalha << ',' << CotarMalha << ','
              << EquiMalha << ',' << EquiCotas << ',' << Ferramentas << ',' << Escalas << ',' << Procurar << ',' << Coordenadas << ',' << Volumes << ',' << Mostrar << ','
              << PermitirAlterar << ',' << TerrenoProjeto << ',' << CircularPontos << ',' << DesenharPerfisGeologicos << ',' << DesenharAciTopog << ','
              << DesenharSecAvulsas << ',' << DesenharTalvegues << ',' << DesenharHistogramas << ',' << (int) DVolumes.IsWindowVisible() << ','
              << EquiMalhaTalv << ',' << EquiCotasTalv << ',' << EquiMalhaHipso << ',' << EquiCotasHipso;

  std::string Secao(std::string("DadosDesenhos_") + MainFrame->PegaRuaAtual());

  ::WritePrivateProfileString(Secao.c_str(),MainFrame->GreideAtivo  ? _T("configSecComPerfil") : 
                                            MainFrame->DesPontosGeo ? _T("configSecComGeom") :
                                            !EMedicaoD ? _T("ConfigSecSemPerfil") :_T("ConfigSecMedicao"),
                                            StrStrDados.str().c_str(),NomeArquivo.c_str());	

  double Estaca(SecaoAtual ? Secoes.GetAt(SecaoAtual).Estaca.EstVirtual : INFINITO);

  StrStrDados.str(std::string());
  StrStrDados.clear();

  StrStrDados << std::setprecision(17);  //--- Pra pegar o infinito

  StrStrDados << Escala[X] << ',' << Escala[Y] << ',' << RazaoHV << ',' << Estaca << ',' << Deltas[X] << ',' << Deltas[Y] << ',' << EquiMalha << ',' << EquiCotas << ',' << ComTitulo << ','
              << EscalaSecoes[X] << ',' << EscalaSecoes[Y] << ',' <<EscalaRazaoTalvegue[X] << ',' << EscalaRazaoTalvegue[Y] << ',' << EscalaRazaoHpsometrica[X] << ',' << EscalaRazaoHpsometrica[Y] << ','
              << EscalaTalv[X] << ',' << EscalaTalv[Y] << ',' << DeltasTalvegues[X] << ',' << DeltasTalvegues[Y] << ',' << RazaoTalvHV << ','
              << EscalaHipso[X] << ',' << EscalaHipso[Y] << ',' << DeltasHipso[X] << ',' << DeltasHipso[Y] << ',' << RazaoHipsoHV;

   ::WritePrivateProfileString(Secao.c_str(),_T("DadosSecoes") ,StrStrDados.str().c_str(),NomeArquivo.c_str());	

  //--- le o estado anterior das toolbars e dos dialogos e salva-os

  CSDados.Format("%i,%i,%i,%i,%i",Ferramentas,Escalas,Procurar,Coordenadas,Volumes);

  ::WritePrivateProfileString(Secao.c_str(),MainFrame->GreideAtivo  ? _T("DadosSecComPerfilDialogos") : 
                                            MainFrame->DesPontosGeo ? _T("DadosSecComGeomDialogos") :
                                                         !EMedicaoD ? _T("DadosSecSemPerfilDialogos") : _T("DadosSecMedicaodialogos") ,CSDados,NomeArquivo.c_str());	

  ::WritePrivateProfileString(_T("SettingsChild"), _T("UltSecAvulsa"),UltimaSecaoAvulsa,NomeArquivo.c_str());
  ::WritePrivateProfileString(_T("SettingsChild"), _T("UltTalvegue"), UltimoTalvegue, NomeArquivo.c_str());
  ::WritePrivateProfileString(_T("SettingsChild"), _T("UltHistograma"), UltimoHipsograma, NomeArquivo.c_str());
}

BEGIN_MESSAGE_MAP(DesSecaoView, CView)
  ON_WM_MOUSEMOVE()
  ON_COMMAND(ID_BUTPROXIMA, OnButproxima)
  ON_COMMAND(ID_BUTANTERIOR, OnButanterior)
  ON_COMMAND(ID_PRIMEIRASECAO, OnPrimeirasecao)
  ON_COMMAND(ID_ULTIMASECAO, OnUltimasecao)
  ON_COMMAND(ID_BUTFECHar, OnButFechar)
  ON_UPDATE_COMMAND_UI(ID_BUTFECHar,OnUpdateFechar)
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_RBUTTONDBLCLK()
  ON_WM_RBUTTONUP()
  ON_UPDATE_COMMAND_UI(ID_PRIMEIRASECAO, OnUpdatePrimeirasecao)
  ON_UPDATE_COMMAND_UI(ID_ULTIMASECAO, OnUpdateUltimasecao)
  ON_UPDATE_COMMAND_UI(ID_BUTPROXIMA, OnUpdateUltimasecao)
  ON_UPDATE_COMMAND_UI(ID_BUTANTERIOR, OnUpdatePrimeirasecao)
  ON_COMMAND(ID_BUTPROPAGARALTERACOES, OnPropagar)
  ON_UPDATE_COMMAND_UI(ID_BUTPROPAGARALTERACOES, OnUpdatePropagar)
  ON_WM_CREATE()
  ON_COMMAND(ID_CONFIGTOOLSEC, OnConfigtoolsec)
  ON_WM_SIZE()
  ON_COMMAND(ID_CALCOFFSETS, OnCalcoffsets)
  ON_UPDATE_COMMAND_UI(ID_CALCOFFSETS, OnUpdateCalcOffSets)
  ON_COMMAND(ID_RESTAURARSECAO, OnRestaurarsecao)
  ON_COMMAND(ID_CALCVOLUMES, OnCalcvolumes)
  ON_UPDATE_COMMAND_UI(ID_CALCVOLUMES, OnUpdateCalcvolumes)
  ON_COMMAND(ID_GRAVAR, OnGravarSecoes)
  ON_UPDATE_COMMAND_UI(ID_GRAVAR, OnUpdateGravar)
  ON_WM_ERASEBKGND()
  ON_COMMAND(ID_ALTERNARTITULO, OnAlternartitulo)
  ON_COMMAND(ID_BUTVISUALIZARTODASECAO, OnButvisualizartodasecao)
  ON_COMMAND(ID_BUTMOSTSECAVULSAS,OnButmostsecavulsas)
  ON_UPDATE_COMMAND_UI(ID_RESTAURARSECAO, OnUpdateRestaurarsecao)
  ON_COMMAND(ID_TRANCARDESSECOES, OnButTrancarSecao)
  ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
  ON_WM_KEYUP()
  ON_BN_CLICKED(IDC_RATERRENO, OnBnClickedRaterreno) // Não funciona
  ON_BN_CLICKED(IDC_RAPROJETO, OnBnClickedRaprojeto) // não funciona
  ON_WM_MOUSEWHEEL()
  ON_WM_CLOSE()
  ON_MESSAGE(WM_CLOSE_TOOLBAR, DesSecaoView::FechouToolBar)
  ON_WM_SHOWWINDOW()
//  ON_UPDATE_COMMAND_UI(ID_BUTDESACITOPOG, &DesSecaoView::OnUpdateButdesacitopog)
  ON_COMMAND(ID_BUTACITOPOG, &DesSecaoView::OnButacitopog)
  ON_COMMAND(ID_BUTSECAVULSASECOES, &DesSecaoView::OnButSecAvulsas)
  ON_UPDATE_COMMAND_UI(ID_BUTSECAVULSASECOES, &DesSecaoView::OnUpdateButSecAvulsas)
  ON_COMMAND(ID_BUTTALVEGUES, &DesSecaoView::OnButTalvegues)
  ON_UPDATE_COMMAND_UI(ID_BUTTALVEGUES, &DesSecaoView::OnUpdateButTalvegues)
  ON_COMMAND(ID_BUTPARTICIONAR, &DesSecaoView::OnButParticionar)
  ON_COMMAND(ID_BUTHIPSOGRAMA, &DesSecaoView::OnButHistograma)
  ON_UPDATE_COMMAND_UI(ID_BUTHIPSOGRAMA, &DesSecaoView::OnUpdateButTalvegues)
  ON_COMMAND(ID_BUTX10, &DesSecaoView::OnButX10)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DesSecaoView drawing

void DesSecaoView::OnDraw(CDC* ppDC)
{
  CSecao* SecAtual(NULL);

  if(DesenharSecAvulsas) 
  {
    SecAtual = &SecaoAvulsaAtual;
    SecAtual->DefRetangDes();       
  }
  else 
  {
    if (DesenharTalvegues)
    {
      SecAtual = &TalvegueAtual;
      SecAtual->DefRetangDes();
    }
    else
    {
      if(DesenharHistogramas)
      {
        SecAtual = &HipsogramaAtual;
        SecAtual->DefRetangDes();
      }
      else if(SecaoAtual) SecAtual = &Secoes.GetAt(SecaoAtual);  //--- Vai desenhar  a secao atual.
    }
  }

  CMMemDC pDC(ppDC);

  if (!SecAtual)
  {
    //--- So pra apagar o fundo

    CPen Pena;
    Pena.CreatePen(PS_SOLID, 1, Cores[CORFUNDO]);
    pDC->SelectObject(&Pena);
    pDC->MoveTo(0,0);
    pDC->LineTo(10,10);

    DeleteObject(Pena);

    return;
  }

  //--- Se alterou a secão, acerta o dialogo de volumes

  if (SecAtual && SecAnterior && (!SecAnterior || SecAnterior->Estaca != SecAtual->Estaca))
  {
    DVolumes.Atualiza(*SecAtual);
    SecAnterior = SecAtual;
  }

  CotaInferior = (SecAtual->Maiores[Y] + SecAtual->Menores[Y] - AlturaVideo) / 2.0 ;

  //--- Desenha a malha

  CPen PenaMalha,*pPenaAnterior;                     
  PenaMalha.CreatePen(PS_SOLID,1,Cores[CORMALHA]);
  pPenaAnterior = pDC->SelectObject(&PenaMalha);    

  if (DesenharMalha)
  {
    long int DELTAXMTS(EquiMalha * Escala[X]),DELTAYMTS(EquiMalha * Escala[Y]),
             LARGVIDEOMTS(LarguraVideo * Escala[X]) , ALTVIDEOMTS(AlturaVideo * Escala[Y]),
             CotaInicial(floor(((-Deltas[X] - Resolucao[X] / 2.0) / Escala[X] + EquiMalha) / EquiMalha) * EquiMalha),
             XInicial(Deltas[X] + (CotaInicial*Escala[X]+Resolucao[X] / 2.0)),
             CotaYInicial(floor(CotaInferior + (ALTVIDEOMTS + Deltas[Y]) / Escala[Y])), 
             YInicial(Resolucao[Y] - ((CotaYInicial - CotaInferior - DeltaYMetro)*Escala[Y]));

    if(DELTAXMTS < 2) DELTAXMTS = 2;
    if(DELTAYMTS < 2) DELTAYMTS = 2;

    for(register long int XAtual = XInicial ; XAtual < LARGVIDEOMTS ; XAtual += DELTAXMTS)
    {
      pDC->MoveTo(XAtual,0);
      pDC->LineTo(XAtual,ALTVIDEOMTS); 
    }

    for(register long int YAtual = YInicial; YAtual < ALTVIDEOMTS; YAtual += DELTAYMTS)
    {
      pDC->MoveTo(0,YAtual);														//--- ALTVIDEOMTS - YAtual porque y cresce para baixo
      pDC->LineTo( LARGVIDEOMTS,YAtual);
    }

    if(CotarMalha)
    {
      CFont* FonteAnterior = pDC->SelectObject(&CurrierNewVertical);
      pDC->SetTextAlign(TA_BOTTOM | TA_LEFT);
      pDC->SetTextColor(Cores[CORTEXTO]);
      pDC->SetBkColor(Cores[CORFUNDO]);

      CString Coord;
      const unsigned long EquiCota(EquiMalha * EquiCotas);

      unsigned int QuantX(floor((double)ALTVIDEOMTS / DELTAYMTS) / EquiCotas), 				        //--- cota 3 linhas da malha x
                   QuantY(floor((double)LARGVIDEOMTS / DELTAXMTS) / EquiCotas);			        	//--- cota 4 linhas da malha Y

      DELTAXMTS *= EquiCotas;

      for(long int XAtual = XInicial ; XAtual < LARGVIDEOMTS ; XAtual += DELTAXMTS)
      {
        if(!DesenharHistogramas || (CotaInicial >= 0 && CotaInicial <= 100))
        {
          Coord.Format("%li",CotaInicial);
          pDC->TextOut(XAtual-1,ALTVIDEOMTS - 20,Coord);
        }
        CotaInicial += EquiCota;
      }

      pDC->SelectObject(&CurrierNewHorizontal);

      DELTAYMTS *= EquiCotas;
      int DeltaCota = EquiCotas * EquiMalha;     //--- n malhas x o valor de cada malha q dá n * n; 

      for(long int YAtual = YInicial; YAtual < ALTVIDEOMTS; YAtual += DELTAYMTS)
      {
        Coord.Format("%-ld",CotaYInicial);
        pDC->TextOut(0,YAtual-1,Coord);
        CotaYInicial -= DeltaCota;
      }

      pDC->SelectObject(FonteAnterior);  
    }
  }

  CFont* FonteAnterior(pDC->SelectObject(&CurrierNewHorizontal));
  pDC->SetTextAlign(TA_BOTTOM | TA_LEFT);
  pDC->SetTextColor(Cores[CORTEXTO]);
  pDC->SetBkColor(Cores[CORFUNDO]);
  CString CSArea;

  itsetCDadosPerfGeologicos itPerfGeo(PerfilGeologico.PegaSetSecaoPerfGeo().upper_bound(CDadosPerfisGeologicos("",SecAtual->Estaca.EstVirtual)));

   if(itPerfGeo != PerfilGeologico.PegaSetSecaoPerfGeo().end())
  {
    int n(1);

    while(itPerfGeo->PegaEstaca() == SecAtual->Estaca.EstVirtual)
    {
      CSArea.Format("Area %i = %-5.2lf",n,const_cast<CDadosPerfisGeologicos*>(&*itPerfGeo)->PegaArea(0));

      pDC->TextOut(40,40 + 15*(n-1),CSArea);

      ++itPerfGeo;
      ++n;
    }
  }

  CPen Pena[7];                                 //--- Penas do projeto e do terreno.

  bool GabaritarProjeto(EMedicaoD && ((CMainFrame*)AfxGetMainWnd())->GabaritarProjeto);   //-- Gabaritar projeto o quando for medição

  Pena[PROJETO].CreatePen(EMedicaoD && GabaritarProjeto ? PS_DASH :PS_SOLID,1,Cores[CORTERRAPLANAGEM]);
  Pena[TERRENO].CreatePen(EMedicaoD ? PS_DASH : PS_SOLID,1,Cores[CORTERRENO]);
  Pena[PAVIMENTO].CreatePen(EMedicaoD  && GabaritarProjeto ? PS_DASH :PS_SOLID,1,Cores[CORPAVIMENTO]);
  Pena[MEDICAO].CreatePen(PS_SOLID,1,Cores[CORTERRAPLANAGEM]);
  Pena[TERRPRIMEDICAO].CreatePen(PS_SOLID,1,Cores[CORTERRENO]);
  Pena[LIMPEZA].CreatePen(PS_DASH,1,Cores[CORTERRENO]);

  POSITION MedicaoAtual(ListaMedicoes.GetHeadPosition());
  BOOL DesenhouTerrMed(!EMedicaoD);

  Perfil PerfilPrimMed,PerfilAtual;

  for (int i = PROJETO ; i <= LIMPEZA || ((EMedicaoD && MedicaoAtual) || !DesenhouTerrMed) ; i++)   
  {
    if (i == TERRENO && !DesenharTerreno || (!DesenharSecAvulsas && (i == TERRENO && DesenharPerfisGeologicos)) || 
        i == LIMPEZA && !DesenharTerreno || i == PAVIMENTO && !DesenharPavimento || i == PROJETO && !DesenharTerraplanagem) continue;

    //--- Perfil que será desenhado

    //--- Só desenho o projeto e o pavimento se não for medicao ou for medição e for para gabaritar o projeto

    if((i == PROJETO || i == PAVIMENTO) && (EMedicaoD && !GabaritarProjeto)) continue;
    if(i == LIMPEZA && DesenharPerfisGeologicos) continue;

    // Seleciona a respectiva pena no device context
    // e salva a pena antiga ao mesmo tempo.

    switch (i)
    {
    case PROJETO : 
      {
        PerfilAtual = SecAtual->Projeto;
        pDC->SelectObject(&Pena[PROJETO]);
        break;
      }
    case TERRENO : 
      {
        pDC->SelectObject(&Pena[TERRENO]);    
        PerfilAtual = SecAtual->Terreno;
        break;
      }
    case PAVIMENTO : 
      {
        pDC->SelectObject(&Pena[PAVIMENTO]);    
        PerfilAtual = SecAtual->Pavimento;
        break;
      }
    case LIMPEZA : 
      {
        pDC->SelectObject(&Pena[LIMPEZA]);    
        PerfilAtual = SecAtual->Limpeza;
        break;
      }
      /*
      case TERRPRIMEDICAO : 
      {
      pDC->SelectObject(&Pena[PAVIMENTO]);    
      ListaMedicoes.TerrPrimMedicao.BuscaPerfil(SecAtual.Estaca,PerfilAtual);
      PerfilAtual.RemoveTudo();
      break;
      }
      */
    default://--- Default é medicao
      {
        if(MedicaoAtual)
        {
          pDC->SelectObject(&Pena[MEDICAO]);    

          CListaSecoes& LMedAtual(*ListaMedicoes.GetNext(MedicaoAtual)); 
          POSITION PMedAtual = LMedAtual.PosicionaNoMaisProximo(SecAtual->Projeto.Estaca); 
          if(PMedAtual)
          {
            CSecao& SecMedAtual = LMedAtual.GetAt(PMedAtual);
            if (fabs(SecMedAtual.Projeto.Estaca - SecAtual->Projeto.Estaca) < 0.0005)
              PerfilAtual = SecMedAtual.Projeto;

            else 	continue;     //--- Não há secao de medicao correspondente
          }
          else continue;     
          break;
        }
        else    //--- Senao é pra desenhar o terreno da primeira medicao
        {
          pDC->SelectObject(&Pena[TERRPRIMEDICAO]);  

          //      PerfilAtual =  ListaMedicoes.TerrPrimMedicao.BuscaPerfil(SecAtual.Estaca) ;
          ListaMedicoes.TerrPrimMedicao.BuscaPerfil(SecAtual->Estaca,PerfilAtual);

          DesenhouTerrMed = true;
        }
      }
    }

    POSITION PonAtual = PerfilAtual.GetHeadPosition();
    if (!PonAtual) continue;   //--- Se o perfil não tem pontos , não há o que desenhar

    //--- Os valores dos pixels são calculados na chamada das funções <MoveTo> e <LineTo>
    //--- A coordenada x em pixels é calculada pela fórmula:
    //--- Sendo:
    //---   px           -> Valor da coordenada x em pixels.
    //---   p(x)         -> Valor do afastamento x do ponto.
    //---   Escala(x)    -> Valor da escala(Pixels/metro) no eixo x.
    //---   Resolução(x) -> Valor da resolução no eixo x (em pixels).
    //---   D(x)         -> Valor do Deslocamento (tecla seta para a direita ou esquerda)
    //---
    //--- px = D(x) + (p(x) * escala(x) + Resolução(x) / 2).
    //---
    //---  "Resolução(x) / 2"  porque considera-se o x = 0 no meio do eixo x.
    //---
    //--- A Coordenada y em pixels é calculada pela fórmula:
    //---
    //--- py = D(y) + (Resolução y - ((p(y) - cotainferior) * escala(y)))
    //---
    //--- " Resolucao y -"  porque as coordenadas do video são espelhadas no eixo x
    //--- ou seja:
    //---
    //--- são assim:       O------> (x)
    //---                  |
    //--- 			           |
    //--- 				         |
    //---	         			   V (y)
    //---
    //--- E queremos o desenho assim:
    //---
    //---                  ^ (y)
    //---                  |
    //---                  |
    //---                  |
    //---                  O------> (x)
    //---

    //--- Desenha o primeiro ponto.

    Ponto PontoAtual(PerfilAtual.GetAt(PonAtual));
    CPoint P((int) (Deltas[X] + (PontoAtual.x*Escala[X]+Resolucao[X] / 2.0)),
             (int) ((Resolucao[Y] - ((PontoAtual.y-CotaInferior-DeltaYMetro)*Escala[Y]))));
    pDC->MoveTo(P);                               //--- Inicializa a linha;
    if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0);    //--- Circula o ponto.

    PerfilAtual.GetNext(PonAtual);

    //--- Desenha os pontos restantes.

    while(PonAtual)
    {
      PontoAtual = PerfilAtual.GetNext(PonAtual);
      P.x = (int) (Deltas[X] + (PontoAtual.x*Escala[X]+Resolucao[X] / 2.0));
      P.y = (int) (Resolucao[Y] - ((PontoAtual.y-CotaInferior-DeltaYMetro)*Escala[Y]));
      pDC->LineTo(P);                              //--- Desenha a linha.
      if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0);   //--- circula o ponto
    }  
  }

  //--- Perfís Geológicos

  if(DesenharPerfisGeologicos)
  {
    itsetCDadosPerfGeologicos itPerfGeo(PerfilGeologico.PegaSetSecaoPerfGeo().upper_bound(CDadosPerfisGeologicos("",SecAtual->Estaca.EstVirtual)));

    if(itPerfGeo != PerfilGeologico.PegaSetSecaoPerfGeo().end())
    {
      while(itPerfGeo->PegaEstaca() == SecAtual->Estaca.EstVirtual)
      {
        CTipoSolo TipoSoloAtual(itPerfGeo->PegaNomeTipoSolo().c_str());
        itsetCTipoSolo itTipoSolo(SetTiposSolos.find(TipoSoloAtual));

        CPen PenaPerfil,PenaSemTipo,*pPenaAUsar(NULL);

        if(itTipoSolo == SetTiposSolos.end() || itTipoSolo->Traco < 0 || itTipoSolo->Traco > 6)  
        {
          PenaSemTipo.CreatePen(PS_DOT,1,Cores[CORTERRENO]);
          pDC->SelectObject(PenaSemTipo);
        }
        else
        {
          PenaPerfil.CreatePen(itTipoSolo->TracoGDI,(itTipoSolo->Traco == 0) ? 2 : 1,itTipoSolo->Cor);
          pDC->SelectObject(PenaPerfil);
        }

        PerfilAtual = itPerfGeo->PegaPerfilGeologico();
        POSITION PonAtual(PerfilAtual.GetHeadPosition());

        if (PonAtual) 
        {
          Ponto PontoAtual(PerfilAtual.GetAt(PonAtual));
          CPoint P((int) (Deltas[X] + (PontoAtual.x*Escala[X]+Resolucao[X] / 2.0)),
                   (int) ((Resolucao[Y] - ((PontoAtual.y-CotaInferior-DeltaYMetro)*Escala[Y]))));
          pDC->MoveTo(P);    
          if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0);   

          //--- Desenha o primeiro ponto.

          PontoAtual = PerfilAtual.GetAt(PonAtual);
          P.x = (int) (Deltas[X] + (PontoAtual.x*Escala[X]+Resolucao[X] / 2.0));
          P.y = (int) ((Resolucao[Y] - ((PontoAtual.y-CotaInferior-DeltaYMetro)*Escala[Y])));
          pDC->MoveTo(P);                              
          if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0);   

          PerfilAtual.GetNext(PonAtual);

          //--- Desenha os pontos restantes.

          while(PonAtual)
          {
            PontoAtual = PerfilAtual.GetNext(PonAtual);
            P.x = (int) (Deltas[X] + (PontoAtual.x*Escala[X]+Resolucao[X] / 2.0));
            P.y = (int) (Resolucao[Y] - ((PontoAtual.y-CotaInferior-DeltaYMetro)*Escala[Y]));
            pDC->LineTo(P);                             
            if(CircularPontos) pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0);  
          }  
          //--- Desenha os off-sets

          if(CircularPontos)
          {
            for (register int C = 0; C < 2 ; C++)
            {
              if(itPerfGeo->PegaOffSet(C).x != INFINITO && itPerfGeo->PegaCV(C) <= 0.0)
              {
                CPoint P((int) (Deltas[X] + (itPerfGeo->PegaOffSet(C).x*Escala[X]+Resolucao[X] / 2.0)),
                         (int) ((Resolucao[Y] - (itPerfGeo->PegaOffSet(C).y-CotaInferior-DeltaYMetro)*Escala[Y])));
                pDC->MoveTo(P);  
                pPenaAnterior = pDC->SelectObject(&Pena[PROJETO]);
                pDC->Arc(P.x-3,P.y-3,P.x+3,P.y+3,0,0,0,0); 
                pDC->SelectObject(pPenaAnterior);
              }
            }
          }
        }
        ++itPerfGeo;
      }
    }
  }

  //if(DesenharRestricoes == true)
  if (DesenharAciTopog)
  {
    CPen PenaRestricoes;

    for (POSITION RestAtual = SecAtual->AcidentesTopograficos.GetHeadPosition(); RestAtual != NULL; SecAtual->AcidentesTopograficos.GetNext(RestAtual))
    {
      Ponto& PontoRestricaoAtual(SecAtual->AcidentesTopograficos.GetAt(RestAtual));

      unsigned int TipoRestricao(PontoRestricaoAtual.AcidenteTopografico);

      if (TipoRestricao != CAciTopografico::NENHUM_ACITOPOG) CriaPenaRestriçao(&PenaRestricoes, TipoRestricao);

      pDC->SelectObject(PenaRestricoes);

      if (TipoRestricao != CAciTopografico::POSTE && TipoRestricao != CAciTopografico::TAMPA_BUEIRO && TipoRestricao != CAciTopografico::ARVORE)
      {
        if (TipoRestricao != CAciTopografico::CERCA && TipoRestricao != CAciTopografico::CANTO_CERCA && TipoRestricao != CAciTopografico::MURO &&
          TipoRestricao != CAciTopografico::CANTO_MURO && TipoRestricao != CAciTopografico::LINHA_FERREA)
        {
          CPoint P((int)(Deltas[X] + (PontoRestricaoAtual.x * Escala[X] + Resolucao[X] / 2.0)),
            (int)((Resolucao[Y] - ((PontoRestricaoAtual.y - CotaInferior - DeltaYMetro) * Escala[Y]))));

          pDC->Arc(P.x - 3, P.y - 3, P.x + 3, P.y + 3, 0, 0, 0, 0);

          pDC->MoveTo(P);
          P.y -= Escala[Y] * 3.0;
          pDC->LineTo(P);
        }
        else
        {
          Ponto PInicial(Deltas[X] + (PontoRestricaoAtual.x * Escala[X] + Resolucao[X] / 2.0), (Resolucao[Y] - ((PontoRestricaoAtual.y - CotaInferior - DeltaYMetro) * Escala[Y])));
          Ponto PFinal(PInicial.x, PInicial.y - 3.0 * Escala[Y]);

          switch (TipoRestricao)
          {
            case CAciTopografico::CERCA: DesenhaCerca(PInicial, PFinal, pDC); break;
            case CAciTopografico::LINHA_FERREA: DesenhaLinhaFerrea(PInicial, PFinal, pDC); break;
            case CAciTopografico::MURO: DesenhaMuro(PInicial, PFinal, pDC); break;

            default:
            {
              CPoint P((int)(Deltas[X] + (PontoRestricaoAtual.x * Escala[X] + Resolucao[X] / 2.0)),
                       (int)((Resolucao[Y] - ((PontoRestricaoAtual.y - CotaInferior - DeltaYMetro) * Escala[Y]))));

              pDC->Arc(P.x - 3, P.y - 3, P.x + 3, P.y + 3, 0, 0, 0, 0);

              pDC->MoveTo(P);
              P.y -= Escala[Y] * 3.0;
              pDC->LineTo(P);
            }
          }
        }
      }
    }
  }

  if (DesenharTalvegues)
  {
    //--- Desenha as Partiçoes do talvegue.

    if (itBaciaAtual != BaciasHidrograficas.end())
    {
      if (itBaciaAtual->TemTalveguePrincipal())
      {
        ItLLDeltaSup itTalvPrincipal(itBaciaAtual->PegaItTalveguePrincipal());

        for (itsetPontos it = itBaciaAtual->PegaParticoesTalveguePrincipal().begin(); it != itBaciaAtual->PegaParticoesTalveguePrincipal().end(); ++it)
        {
          int Px(Deltas[X] + (it->x * Escala[X] + Resolucao[X] / 2.0));

          pDC->MoveTo(Px, 0);
          pDC->LineTo(Px, Resolucao[Y]);
        }

        if (xParticaoAtual != INFINITO)
        {
          pDC->MoveTo(xParticaoAtual, 0);
          pDC->LineTo(xParticaoAtual, Resolucao[Y]);
        }
      }
    }
  }

  pDC->SelectObject(pPenaAnterior);   //--- Recoloca a pena Anterior.
}

/////////////////////////////////////////////////////////////////////////////
// DesSecaoView diagnostics

#ifdef _DEBUG
void DesSecaoView::AssertValid() const
{
  CView::AssertValid();
}

void DesSecaoView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// DesSecaoView message handlers

void DesSecaoView::OnMouseMove(UINT nFlags, CPoint point) 
{
  SetCursor(LoadCursor(NULL,CursorAtual));

  CMainFrame* MainFrame((CMainFrame *) AfxGetMainWnd());

  PonMousePixelsAtual = point;

  TransfPixelsXY(point,MouseXY->m_DOCoorX,MouseXY->m_DOCoorY);  //--- Pega as coordenadas (em metros) do ponto local o mouse está.

  MouseXY->UpdateData(FALSE);    //--- mostra as coordenadas de onde o mouse está.

  if (Particionando)
  {
    if(itBaciaAtual != BaciasHidrograficas.end())
    {
      if(MouseXY->m_DOCoorX > itBaciaAtual->PegaPerfTalvPrincipal().begin()->x &&
         MouseXY->m_DOCoorX < itBaciaAtual->PegaPerfTalvPrincipal().rbegin()->x)
      { 
        xParticaoAtual = point.x;

        RedrawWindow();
      }

      return;
    }
  }

  if (!SecaoAtual && !DesenharSecAvulsas && !DesenharTalvegues && !DesenharHistogramas) return;  //--- não tem secao? retorna...

  CSecao& SecAtual(DesenharSecAvulsas ? SecaoAvulsaAtual : DesenharTalvegues ? TalvegueAtual : DesenharHistogramas? HipsogramaAtual : Secoes.GetAt(SecaoAtual));  //--- Pega a seção atual.

  if (Arrastando)     //--- Está arrastando?
  {
    //--- Esta variavel (ArrastandoPonto) se faz necessária porque quando o mouse der pulos
    //--- Maiores que a tolerância de aproximação do ponto (tolerâncias x e y) o ponto não 
    //--- seria mais encontrado nas chamadas a PosicionaPonto (do perfil), fazendo-o parar 
    //--  de ser arrastado no meio da operação. Esta variável assegura o arrasto do ponto
    //--- Independentemente dos saltos do mouse, ela apenas retorna a FALSE , no final da 
    //--- operação, no método LPbuttonUP, ou seja, quando o usuário tirar o dedo do botão
    //--- do mouse.

    if (PermitirAlterar && ArrastandoPonto)    //--- Esta Arrastando um ponto?
    {
      //--- Sim, Altera o ponto.
      if (Tipo == PROJETO) SecAtual.Projeto.AlteraPonto(Ponto(MouseXY->m_DOCoorX,MouseXY->m_DOCoorY,INFINITO,INFINITO,INFINITO,INFINITO,CString("PONINS")),PAtual);
      else
      {
        if(Tipo == TERRENO)
        {
          POSITION PPrevTerr(PAtual),PProxTerr(PAtual);
          SecAtual.Terreno.GetPrev(PPrevTerr);
          SecAtual.Terreno.GetNext(PProxTerr);

          //--- Verifica se a alteração  está entre os pontos anterior e posterior

          if((!PPrevTerr || SecAtual.Terreno.GetAt(PPrevTerr).x < MouseXY->m_DOCoorX) && 
             (!PProxTerr || SecAtual.Terreno.GetAt(PProxTerr).x > MouseXY->m_DOCoorX))
          {
            PerfilGeologico.AlteraPontoPerfis(SecAtual.Terreno.GetAt(PAtual),MouseXY->m_DOCoorX,MouseXY->m_DOCoorY);
			
            AlteraLimpeza(SecAtual,Ponto(MouseXY->m_DOCoorX,MouseXY->m_DOCoorY,INFINITO),PAtual);
            SecAtual.Terreno.AlteraPonto(Ponto(MouseXY->m_DOCoorX,MouseXY->m_DOCoorY,INFINITO),PAtual);
          }
        }
        else 
        {
          if(Tipo == PERFILGEOLOGICO)
            const_cast<Perfil*>(&(itPerfGeo->PegaPerfilGeologico()))->AlteraPonto(Ponto(MouseXY->m_DOCoorX,MouseXY->m_DOCoorY,INFINITO),PAtual);
        }
      }

      //--- Mostra o ponto alterado no dialogo.

      CRect Rect;
      GetWindowRect(&Rect);
      DPopUpPonto.MostraDescricao(point.x+20+Rect.left,point.y+20+Rect.top,PreparaDescricao(Tipo,SecAtual));
    }
    else     //--- Não esta arrastando um ponto?...então só pode estar está arrastando o desenho.
    {
      Deltas[X] += point.x - InicioArrast.x;  
      Deltas[Y] += point.y - InicioArrast.y;
      InicioArrast = point;
      DeltaYMetro = Deltas[Y] / Escala[Y];    //--- Deslocamento do Arrasto.
    }

    RedrawWindow();   //--- Esta arrastando alguma coisa, então redesenha a janela.
  }
  else   //--- Não está arrastando nada...
  {
    //--- Se o mouse estiver sobre algum ponto mostra as coordenadas do ponto.
    //--- Caso contrário, limpa o dialogo do ponto , caso ele não esteja limpo.

    CString NumMedicao;

    CRect Rect;
    this->GetWindowRect(&Rect);
    double xParticaoTalvegue(INFINITO);

    POSITION POSPAtual(VerfPonto(MouseXY->m_DOCoorX,MouseXY->m_DOCoorY,&xParticaoTalvegue, NumMedicao));

    if (Tipo == PARTICAOTALVEGUE)
    {
      DPopUpPonto.MostraDescricao(point.x + 20 + Rect.left, point.y + 20 + Rect.top, PreparaDescricao(Tipo, SecAtual, &xParticaoTalvegue));
    }
    else
    {
      if (Tipo == HIPSOGRAMA)
      {
        DPopUpPonto.MostraDescricao(point.x + 20 + Rect.left, point.y + 20 + Rect.top, PreparaDescricao(Tipo, SecAtual, &xParticaoTalvegue));
      }
      if (POSPAtual) 
      {
        //--- Se estiver no mesmo ponto mantém o diálogo (Para evitar muitos popups)

        if (POSPonAnterior != POSPAtual) 
        {
          DPopUpPonto.MostraDescricao(point.x+20+Rect.left,point.y+20+Rect.top,PreparaDescricao(Tipo,SecAtual));
          POSPonAnterior = POSPAtual;
        }
      }
      else 
      {
        POSPonAnterior = NULL;
        DPopUpPonto.ShowWindow(false); 
      }
    }
  }	    											
}

void DesSecaoView::OnInitialUpdate() 
{
  CView::OnInitialUpdate();

  CChildFrame* ChildFrame((CChildFrame*) GetParentFrame());

  pComboFind = &ChildFrame->PegaComboProcurarDesSecao();

  CMainFrame* MainFrame((CMainFrame *) AfxGetMainWnd());

  MouseXY = (xyMouse *) MainFrame->MouseXY;

  //--- Se for medição, o projeto e o terreno não podem ser alterados, então desabilita os botões

  if (EMedicaoD)
  {
    MouseXY->m_RATerreno.EnableWindow(FALSE);
    MouseXY->m_RAProjeto.EnableWindow(FALSE);
  }
  else	MouseXY->m_RATerreno.SetCheck(TRUE);  //--- Se não é medicao Inicializa como terreno.

  //--- Pega os dados iniciais e atualiza o desenho

  char Buffer[512]={0};
  std::string NomeArquivo(MainFrame->PegaProjetoAtual() + std::string(".ini"));
  int DVolumesVisivel(0);

  //--- Pega a configuração

  double Estaca(0.0);
  std::string Secao(std::string("DadosDesenhos_") + MainFrame->PegaRuaAtual());

  MainFrame->GreideAtivo  ? (::GetPrivateProfileString(Secao.c_str(),_T("ConfigSecComPerfil"),_T("1,1,1,1,1,10,2,1,1,1,1,1,16,1,1,1,1,1,0,0,1,1,500,5,50,2"),Buffer,511,NomeArquivo.c_str())) :
  MainFrame->DesPontosGeo ? (::GetPrivateProfileString(Secao.c_str(),_T("ConfigSecComGeom"),_T("1,1,1,1,1,10,2,1,1,1,1,1,16,1,1,1,1,1,0,0,1,1,500,5,50,2"),Buffer,511,NomeArquivo.c_str())) :
               !EMedicaoD ? (::GetPrivateProfileString(Secao.c_str(),_T("ConfigSecSemPerfil"),_T("1,1,1,1,1,10,2,1,1,1,1,1,16,1,1,1,1,1,0,0,1,1,500,5,50,2"),Buffer,511,NomeArquivo.c_str())) : 
                            (::GetPrivateProfileString(Secao.c_str(),_T("ConfigSecMedicao"),_T("1,1,1,1,1,10,2,1,1,1,1,1,16,1,1,1,1,1,0,0,1,1,500,5,50,2"),Buffer,511,NomeArquivo.c_str())); 

  _stscanf(Buffer,"%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i,%i",&DesenharPavimento,&DesenharTerraplanagem,&DesenharTerreno,&DesenharMalha,&CotarMalha,&EquiMalha,&EquiCotas,&Ferramentas,
    &Escalas,&Procurar,&Coordenadas,&Volumes,&Mostrar,&PermitirAlterar,&TerrenoProjeto,&CircularPontos,&DesenharPerfisGeologicos,&DesenharAciTopog,&DesenharSecAvulsas,&DesenharTalvegues,&DesenharHistogramas,&DVolumesVisivel,
    &EquiMalhaTalv,&EquiCotasTalv,&EquiMalhaHipso,&EquiCotasHipso);

  //--- Paga as escalas anteriores
     
  ::GetPrivateProfileString(Secao.c_str(), _T("DadosSecoes"),_T("30.0,30.0,1.0,0.0,0.0,0.0,5,2,1,0.1,1.0,10.0,1.0,1.0,10.0,0.1,1.0,0,0,10.0,10,1,0,0,10"), Buffer, 511, NomeArquivo.c_str());

  _stscanf(Buffer,"%lf,%lf,%lf,%lf,%lf,%lf,%i,%i,%i,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf",
           &Escala[X],&Escala[Y],&RazaoHV,&Estaca,&Deltas[X],&Deltas[Y],&EquiMalha,&EquiCotas,&ComTitulo,
           &EscalaSecoes[X],&EscalaSecoes[Y],&EscalaRazaoTalvegue[X],&EscalaRazaoTalvegue[Y],&EscalaRazaoHpsometrica[X],&EscalaRazaoHpsometrica[Y],
           &EscalaTalv[X],&EscalaTalv[Y],&DeltasTalvegues[X],&DeltasTalvegues[Y],&RazaoTalvHV,
           &EscalaHipso[X],&EscalaHipso[Y],&DeltasHipso[X],&DeltasHipso[Y],&RazaoHipsoHV);

  //--- Pega os dialogos ativos
  //--- le o estado anterior das toolbars e atualiza-os

  MainFrame->GreideAtivo ? ::GetPrivateProfileString(_T("SettingsChild"),_T("StatusVisualizarSecComPerfil"),_T("1,1,1,1,1,1"),Buffer,511,NomeArquivo.c_str())	:
                           ::GetPrivateProfileString(_T("SettingsChild"),_T("StatusVisualizarSecSemPerfil"),_T("1,1,1,1,1,1"),Buffer,511,NomeArquivo.c_str());

 ::GetPrivateProfileString(Secao.c_str(),MainFrame->GreideAtivo ? _T("DadosSecComPerfilDialogos") : 
                                         MainFrame->DesPontosGeo ? _T("DadosSecComGeomDialogos") : 
                                                      !EMedicaoD ? _T("DadosSecSemPerfilDialogos") : 
                                                                  _T("DadosSecMedicaodialogos"),"1,1,1,1,1",Buffer,511,NomeArquivo.c_str());

  _stscanf(Buffer,"%i,%i,%i,%i,%i",&Ferramentas,&Escalas,&Procurar,&Coordenadas,&Volumes);

  ::GetPrivateProfileString(_T("SettingsChild"), _T("UltSecAvulsa"), _T(""), Buffer,511, NomeArquivo.c_str());
  UltimaSecaoAvulsa = Buffer;
  ::GetPrivateProfileString(_T("SettingsChild"), _T("UltTalvegue"), _T(""), Buffer, 511, NomeArquivo.c_str());
  UltimoTalvegue = Buffer;
  ::GetPrivateProfileString(_T("SettingsChild"), _T("UltHistograma"), _T(""), Buffer, 511, NomeArquivo.c_str());
  UltimoHipsograma = Buffer;

  MainFrame->ShowControlBar(MainFrame->PegaBarra(1),Coordenadas, FALSE);	
  MainFrame->ShowControlBar(MainFrame->PegaBarra(2),Escalas, FALSE);	
  MainFrame->ShowControlBar(MainFrame->PegaBarra(3),Procurar, FALSE);	
  MainFrame->ShowControlBar(MainFrame->PegaBarra(4),Ferramentas, FALSE);	

  //--- dialogo dos volumes 

  DVolumes.Create(this);
  if (!ComTitulo) OnAlternartitulo();   //--- O diálogo dos volumes tem q estar criado
  DVolumes.ShowWindow(DVolumesVisivel);

  MainFrame->DVolumes= &DVolumes;       //--- Idem

  //--- posiciona a seção

  SecaoAtual = Secoes.Posiciona(CEstaca(Estaca));   
  if (!SecaoAtual) SecaoAtual = Secoes.PosicionaPrimeira();

  if (SecaoAtual) 
  {
    MostraSecao(SecaoAtual);
    SecaoAnterior = Secoes.GetAt(SecaoAtual);  //--- Salva a secao sem alterações, para poder restaurá-la
    DVolumes.Atualiza(SecaoAnterior);
  }
  
  if(1)
  {
    //  else AfxMessageBox("Não há seções transversais", MB_ICONASTERISK);

      //--- Pega a última seção avulsa desenhada

    if (DesenharSecAvulsas && UltimaSecaoAvulsa.GetLength())
    {
      POSITION PSecAvAtual(PegaSecaoAvulsa(UltimaSecaoAvulsa));

      if (PSecAvAtual)
      {
        SecaoAvulsaAtual.Terreno = SecoesAvulsas.PegaLSecoesAvulsas().GetAt(PSecAvAtual);
        SecaoAvulsaAtual.Projeto = SecoesAvulsas.PegaLSecoesAvulsas().GetAt(PSecAvAtual).LstPontosProjeto;
        SecaoAvulsaAtual.Terreno.Estaca.Descricao = SecoesAvulsas.PegaLSecoesAvulsas().GetAt(PSecAvAtual).Nome.c_str();
        SecaoAvulsaAtual.AcidentesTopograficos = SecoesAvulsas.PegaLSecoesAvulsas().GetAt(PSecAvAtual).AcidentesTopograficos;
        SecaoAvulsaAtual.AlinhamentosTerrap = SecoesAvulsas.PegaLSecoesAvulsas().GetAt(PSecAvAtual).AlinhamentosTerrap;

        Escala[X] = EscalaSecoes[X];
        Escala[Y] = EscalaSecoes[Y];
        RazaoHV = EscalaSecoes[Y];
      }
      else
      {
        UltimaSecaoAvulsa = "";
        DesenharSecAvulsas = false;
      }
    }
    else
    {
      //--- Pega o último talvegue desenhado

      if (DesenharTalvegues && UltimoTalvegue.GetLength())
      {
        EscalaTalv[X] = Escala[X];
        EscalaTalv[Y] = Escala[Y];

        EquiMalhaTalv = EquiMalha;
        EquiCotasTalv = EquiCotas;

        DesenharTalvegues = false;  //--- O ButTalvegues inverte o sentido

        OnButTalvegues();
      }
      else
      {
        UltimoTalvegue = "";
        DesenharTalvegues = false;

        if (DesenharHistogramas && UltimoHipsograma.GetLength())
        {
          EscalaHipso[X] = Escala[X];
          EscalaHipso[Y] = Escala[Y];

          EquiMalhaHipso = EquiMalha;
          EquiCotasHipso = EquiCotas;

          DesenharHistogramas = false;

          OnButHistograma();
        }
        else
        {
          UltimoHipsograma = "";
          DesenharHistogramas = false;
        }
      }

      if (DesenharSecAvulsas)
      {
        PreencheComboProcurarSecAvulsas(ChildFrame);
        ((CComboProcurarDesSecao*)pComboFind)->PosicionaTexto(UltimaSecaoAvulsa);
      }
      else
      {
        if(DesenharTalvegues)
        {
          PreencheComboProcurarTalvegues(ChildFrame);
          ((CComboProcurarDesSecao*)pComboFind)->PosicionaTexto(UltimoTalvegue);
          MainFrame->ShowControlBar(MainFrame->PegaBarra(BARRA_PARTICIONAR),true,true);
        }
        else
        {
          if (DesenharHistogramas)
          {
            PreencheComboProcurarTalvegues(ChildFrame);
            ((CComboProcurarDesSecao*)pComboFind)->PosicionaTexto(UltimoHipsograma);
          }
          else
          {
            if(Secoes.GetSize() > 0)
            { 
              PreencheComboProcurar(ChildFrame);
              ((CComboProcurarDesSecao*)pComboFind)->Posiciona(Secoes.GetAt(SecaoAtual).Estaca);
            }
            else
            {
               //--- Não há nada pré-desenhado verifica se tem algum talvegue ou curva hipsometrica

              if (BaciasHidrograficas.size())
              {
                 auto BaciaAtual(BaciasHidrograficas.begin());
                 auto PerfTalveguePrincipal(BaciaAtual->PegaPerfTalvPrincipal());

                 if (BaciaAtual->PegaPerfTalvPrincipal().size())
                 {
                   PreencheComboProcurarTalvegues(ChildFrame);
                   ((CComboProcurarDesSecao*)pComboFind)->PosicionaTexto(UltimoTalvegue);
                   MainFrame->ShowControlBar(MainFrame->PegaBarra(BARRA_PARTICIONAR), true, true);

                   ((CComboProcurarDesSecao*)pComboFind)->SetCurSel(0);

                   DesenharHistogramas = DesenharTalvegues = false;
                 }
                 else
                 {
                   // verifica o hipsograma

                   PreencheComboProcurarTalvegues(ChildFrame);
                   ((CComboProcurarDesSecao*)pComboFind)->PosicionaTexto(UltimoHipsograma);
                   MainFrame->ShowControlBar(MainFrame->PegaBarra(BARRA_PARTICIONAR),false,false);

                   ((CComboProcurarDesSecao*)pComboFind)->SetCurSel(0);

                   DesenharHistogramas = DesenharTalvegues = false;

                   OnButHistograma();
                 }
              }
              AfxMessageBox("Não há seções para serem desenhadas");
            }
          }
        }
      }
    }
  }

    //--- Inicializa os valores da escala, da razão e da tolerância.

    if(!DesenharTalvegues)
    {
      ChildFrame->Deslizantes.MudarEscala(Escala[X]);
      ChildFrame->Deslizantes.MudarRazao(RazaoHV);
    }

    MainFrame->mpCDesTool1->GetToolBarCtrl().SetState(ID_TRANCARDESSECOES, !PermitirAlterar);
    MainFrame->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTPROPAGARALTERACOES, DesenharPerfisGeologicos);
    MainFrame->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTACITOPOG, DesenharAciTopog);
    MainFrame->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTSECAVULSASECOES, DesenharSecAvulsas);
    MainFrame->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTTALVEGUES, DesenharTalvegues);
    MainFrame->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTHIPSOGRAMA, DesenharHistogramas);

    MouseXY->m_RATerreno.SetCheck(TerrenoProjeto == 1);
    MouseXY->m_RAProjeto.SetCheck(TerrenoProjeto == 0);

    CalculaParametros();     //--- Calcula os parametros iniciais

    //--- Pega as cores

    ::GetPrivateProfileString(Secao.c_str(), _T("DadosDesPerfilCores"), _T("0,4227072,12632256,255,33023,16711680"), Buffer, 511, NomeArquivo.c_str());
    _stscanf(Buffer, "%lu,%lu,%lu,%lu,%lu,%lu", &Cores[0], &Cores[1], &Cores[2], &Cores[3], &Cores[4], &Cores[5]);

    //--- Fontes para escrita horizontal e vertical

    pDC = GetDC();                          
    ASSERT_VALID(pDC);

    CFont* LF = pDC->GetCurrentFont();
    LOGFONT Log;

    LF->GetLogFont(&Log);
    Log.lfEscapement = 900;

    strcpy(Log.lfFaceName, "Courier New");
    CurrierNewVertical.CreateFontIndirect(&Log);

    Log.lfEscapement = 0;
    CurrierNewHorizontal.CreateFontIndirect(&Log);

    CArqTiposSolos ArqTiposSolos(MainFrame->PegaProjetoAtual());
    ArqTiposSolos.CriaSet(SetTiposSolos);
    ArqTiposSolos.ConverteGDI(SetTiposSolos);

    if (DesenharPerfisGeologicos && PerfilGeologico.PegaSetSecaoPerfGeo().size() == 0) DesenharPerfisGeologicos = false;

    RedrawWindow();
}

void DesSecaoView::OnButproxima() 
{
  if (!DesenharSecAvulsas && SecaoAtual && SecaoAtual != Secoes.PosicionaUltima())
  {
    Secoes.GetNext(SecaoAtual);

    //--- Pula as seções que não podem ser exibidas

    while(SecaoAtual && FiltraMostrar(SecaoAtual)) Secoes.GetNext(SecaoAtual);

    if (SecaoAtual)
    {
      MostraSecao(SecaoAtual); 
      ((CComboProcurarDesSecao *)pComboFind)->Posiciona(Secoes.GetAt(SecaoAtual).Estaca);
      RedrawWindow();    
      AlterouSecao = false;
    }
  }
}

void DesSecaoView::OnButanterior() 
{
  if (!DesenharSecAvulsas && SecaoAtual && SecaoAtual != Secoes.PosicionaPrimeira())
  {
    Secoes.GetPrev(SecaoAtual);

    //--- Pula as seções que não podem ser exibidas

    while(SecaoAtual && FiltraMostrar(SecaoAtual)) Secoes.GetPrev(SecaoAtual);

    if (SecaoAtual)
    {
      MostraSecao(SecaoAtual); 
      ((CComboProcurarDesSecao *)pComboFind)->Posiciona(Secoes.GetAt(SecaoAtual).Estaca);
      AlterouSecao = false;
      RedrawWindow();   
    }
  }
}

void DesSecaoView::OnPrimeirasecao() 
{
  SecaoAtual = Secoes.PosicionaPrimeira();

  if (SecaoAtual) 
  {
    while(SecaoAtual && FiltraMostrar(SecaoAtual)) Secoes.GetNext(SecaoAtual);

    if(SecaoAtual)
    {
      MostraSecao(SecaoAtual); 
      ((CComboProcurarDesSecao *)pComboFind)->Posiciona(Secoes.GetAt(SecaoAtual).Estaca);
      AlterouSecao = false;
      RedrawWindow();  
    }
  }
}

void DesSecaoView::OnUltimasecao() 
{
  SecaoAtual = Secoes.PosicionaUltima();

  if (SecaoAtual) 
  {
    while(SecaoAtual && FiltraMostrar(SecaoAtual)) Secoes.GetPrev(SecaoAtual);

    if(SecaoAtual)
    {
      MostraSecao(SecaoAtual); 
      ((CComboProcurarDesSecao *)pComboFind)->Posiciona(Secoes.GetAt(SecaoAtual).Estaca);
      RedrawWindow();  
    }
  }
}

void DesSecaoView::OnButFechar() 
{
  ((CChildFrame*) GetParentFrame())->Fechar();
}

void DesSecaoView::OnUpdateFechar(CCmdUI* pCmdUI) 
{
  //--- Com o desenho do perfil a janela tem que ficar aberta porcausa da lista de secoes

  if(((CMainFrame*)AfxGetMainWnd())->GreideAtivo)  pCmdUI->Enable(false);
  else pCmdUI->Enable(!ComTitulo);	
}

void DesSecaoView::CalculaParametros()
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

  //--- A escala mudou, acerta as tolerâncias (5 pixels em qualquer eixo);

  Toleranciax = 4.0 / Escala[X];
  Toleranciay = 4.0 / Escala[Y];

  //--- Deslocamento no eixo y / metro

  DeltaYMetro = Deltas[Y] / Escala[Y];    //--- Deslocamento do Arrasto.
}

POSITION DesSecaoView::VerfPonto(double x,double y, double* pxParticao,CString& NumMed)
{
  //--- Verifica Se o mouse esta sobre algum ponto de terreno, de projeto ou do
  //--- pavimento, caso esteja posiciona o ponteiro do perfil para o ponto em questão,
  //--- através da mensagem " PosicionaPonto";

  PAtual = PAciTopogAtual = NULL;

  CSecao& SecAtual(DesenharSecAvulsas ? SecaoAvulsaAtual : DesenharTalvegues ? TalvegueAtual : DesenharHistogramas? HipsogramaAtual : Secoes.GetAt(SecaoAtual));  //--- Pega a seção atual.
  
  Tipo = ALINHAMENTOSTERRAPL;                   //--- Tem q ser verificado antes de  todos paramostrar os offsets

  PAtual = SecAtual.AlinhamentosTerrap.PosicionaPonto(x, y, Toleranciax, Toleranciay);

  if (PAtual)
  {
    PAlinTerrapAtual = SecAtual.AlinhamentosTerrap.PosicionaPontoX(SecAtual.AlinhamentosTerrap.GetAt(PAtual).x, 1e-3);
  }

  if (!DesenharPerfisGeologicos && !DesenharHistogramas)     //--- Se estiver desenhando os perfis geológicos não procura no terreno, procura nos perfís geológicos
  {
    if (!PAtual)
    {
      Tipo = TERRENO;

      PAtual = SecAtual.Terreno.PosicionaPonto(x, y, Toleranciax, Toleranciay);

      if (PAtual)
      {
        //--- Sincroniza o atual na limpeza e dos perfis geologicos, necessário nas movimentações sincronizadas dos pontos

        SecAtual.Limpeza.PosicionaPonto(x, y, Toleranciax, Toleranciay);

        //--- Verifica se há algum acidente topográfico no ponto

        Ponto& PontoAtual(SecAtual.Terreno.GetAt(PAtual));


        PAciTopogAtual = SecAtual.AcidentesTopograficos.PosicionaPontoX(PontoAtual.x, 1e-3);

        if(DesenharTalvegues) Tipo = PERFILTALVEGUE;     //--- Os perfís dos talvegues estão no terreno da seção
      }
    }
  }

  if (!PAtual)
  {
    Tipo = PROJETO;
    PAtual = SecAtual.Projeto.PosicionaPonto(x,y,Toleranciax,Toleranciay);
  }

  if (!PAtual)
  {
    Tipo = PAVIMENTO;
    PAtual = SecAtual.Pavimento.PosicionaPonto(x,y,Toleranciax,Toleranciay);
  }

  if (!PAtual)
  {
    Tipo = LIMPEZA;
    PAtual = SecAtual.Limpeza.PosicionaPonto(x,y,Toleranciax,Toleranciay);
  }
  
  if (!PAtual)
  {
    Tipo = ACITOPOGFORASUPERF;
    PAtual = SecAtual.AcidentesTopograficos.PosicionaPonto(x,y,Toleranciax,Toleranciay);
    if(PAtual)
    {
      PAciTopogAtual = SecAtual.AcidentesTopograficos.PosicionaPontoX(SecAtual.AcidentesTopograficos.GetAt(PAtual).x,1e-3);
    }
  }

  if (!PAtual /*|| (Tipo == TERRENO && DesenharPerfisGeologicos)*/)   //--- Se achou um ponto de terreno e houver perfil geológico é preciso posicionar o itPerfGeo
  {                                                               //--- Para mostrar a descrição do perfil geológico não a do terreno  
    //--- Procura nos perfis geologicos

    itPerfGeo = PerfilGeologico.PegaSetSecaoPerfGeo().upper_bound(CDadosPerfisGeologicos("",SecAtual.Estaca.EstVirtual));  

    while(itPerfGeo != PerfilGeologico.PegaSetSecaoPerfGeo().end() && itPerfGeo->PegaEstaca() == SecAtual.Estaca.EstVirtual && PAtual == NULL)
    {
      Tipo = PERFILGEOLOGICO;      

      PAtual = const_cast<CDadosPerfisGeologicos*>(&*itPerfGeo)->PosicionaPonto(x,y,Toleranciax,Toleranciay);   

      if(!PAtual)
      {
        Tipo = PERFILGEOLOGICOOFFSETS;             //--- Procura nos off-sets dos perfís geológicos

        PerfOffSetsPerfGeologico.LimpaTudo(); //--- Perfil temporário apenas para mostrar os offsets dos perfis geologicos

        for(int C = 0 ; C < 2 ; C++)
        {
          if(itPerfGeo->PegaOffSet(C).x != INFINITO && itPerfGeo->PegaCV(C) < 0.0) PerfOffSetsPerfGeologico.AddTail(Ponto(itPerfGeo->PegaOffSet(C)));
        }

        if(PerfOffSetsPerfGeologico.GetSize())
        {
          PAtual = PerfOffSetsPerfGeologico.PosicionaPonto(x,y,Toleranciax,Toleranciay);   
        }
      }

      if(!PAtual) ++itPerfGeo;
    }
	
    if (!PAtual)
    {
      Tipo = PERFILGEOLOGICOOFFSETS;

      PerfOffSetsPerfGeologico.LimpaTudo(); //--- Perfil temporário apenas para mostrar os offsets dos perfis geologicos

      //--- Procura nos off-sets dos perfis geológicos 

      itPerfGeo = PerfilGeologico.PegaSetSecaoPerfGeo().upper_bound(CDadosPerfisGeologicos("",SecAtual.Estaca.EstVirtual));

      for( ; itPerfGeo != PerfilGeologico.PegaSetSecaoPerfGeo().end() && itPerfGeo->PegaEstaca() == SecAtual.Estaca.EstVirtual && PAtual == NULL ;  ++itPerfGeo)
      {
        for(int C = 0 ; C < 2 ; C++)
        {
          if(itPerfGeo->PegaOffSet(C).x != INFINITO && itPerfGeo->PegaCV(C) < 0.0) PerfOffSetsPerfGeologico.AddTail(Ponto(itPerfGeo->PegaOffSet(C)));
        }
        if(PerfOffSetsPerfGeologico.GetSize())
        {
          PAtual = PerfOffSetsPerfGeologico.PosicionaPonto(x,y,Toleranciax,Toleranciay);   
        }
      }
    }

    if (!PAtual)
    {
      Tipo = HIPSOGRAMA;

      if (DesenharHistogramas)
      {
        PAtual = HipsogramaAtual.Terreno.GetHeadPosition();

        for (; PAtual != NULL; HipsogramaAtual.Terreno.GetNext(PAtual))
        {
          double xAtual(HipsogramaAtual.Terreno.GetAt(PAtual).x),
                 yAtual(HipsogramaAtual.Terreno.GetAt(PAtual).y);

          if (x > (xAtual - Toleranciax) && x < (xAtual + Toleranciax) &&
              y > (yAtual - Toleranciay) && y < (yAtual + Toleranciay)) 
            break;
        }

        if (PAtual != NULL) return PAtual;
      }
    }

    if (!PAtual)
    {
      Tipo = PARTICAOTALVEGUE;

      if (DesenharTalvegues)
      {
        //--- Desenha as Partiçoes do talvegue.

        if (itBaciaAtual != BaciasHidrograficas.end())
        {
          ItLLDeltaSup itTalvPrincipal(itBaciaAtual->PegaItTalveguePrincipal());

          itsetPontos it(itBaciaAtual->PegaParticoesTalveguePrincipal().begin());

          for (; it != itBaciaAtual->PegaParticoesTalveguePrincipal().end(); ++it)
          {
            if (x > (it->x - Toleranciax) && x < (it->x + Toleranciax)) break;
          }

          if (it != itBaciaAtual->PegaParticoesTalveguePrincipal().end())
          {
           // if(pxParticao)
            if(1)
            {
              *pxParticao = it->x;

              Tipo = PARTICAOTALVEGUE;

              PAtual = SecAtual.Terreno.GetHeadPosition();  //---- Só pra constar
            }
            else Tipo = ERROPARTICAO;

            return PAtual;
          }
        }
      }
    }
  }

  //--- Se não achou nada e há medições desenhadas, verifica nas medições

  if (!PAtual)
  {
    POSITION MedicaoAtual = ListaMedicoes.GetHeadPosition();

    while (MedicaoAtual && !PAtual)
    {
      CListaSecoes& LMedAtual = *ListaMedicoes.GetNext(MedicaoAtual); 

      POSITION PMedAtual = LMedAtual.PosicionaNoMaisProximo(SecAtual.Projeto.Estaca); 

      if (PMedAtual)
      {
        Tipo = MEDICAO;

        CSecao& SecMedAtual = LMedAtual.GetAt(PMedAtual);
        if (fabs(SecAtual.Projeto.Estaca - SecMedAtual.Projeto.Estaca) < 0.0005)
          PAtual = SecMedAtual.Projeto.PosicionaPonto(x,y,Toleranciax,Toleranciay);
        if (PAtual)NumMed = LMedAtual.PegaProjeto().Right(3);
      }
    }
  }

  if(!PAtual) Tipo = NENHUMTIPO; 

  return PAtual;
}

void DesSecaoView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  if (Particionando)
  {
    xParticaoAtual = point.x;

    RedrawWindow();

    return;
  }
  
  //--- O primeiro click em cima de um ponto inicia o arrasto, no segundo click
  //--- finaliza o arrasto, no primeiro click arrastando ponto fica true até que o usuário click novamente
  //--- quando Terminararrasto fica true, o arrasto termina efetivamente quando o usuário tira o dedo do 
  //--- botão do mouse no método onlbuttonup. 

  //--- Se já estiver arrastando coloca true que o arrasto deve ser terminado (no método onlbuttonup) e retorna
  //--- não há nada mais a fazer. 

  if (ArrastandoPonto || Arrastando) 
  {
    TerminarArrasto = TRUE;
    if(ArrastandoPonto) 
    {
      if(!AlterouSecao || !AlterouSecoes)
      {
        AlterouSecao = AlterouSecoes =true;    //--- houve Alteração na secao
        ((CMainFrame*) AfxGetMainWnd())->InsereDesenhoAlterado(Trecho,std::string("DesSecao"),this);
      }
    }

    return;
  }

  Arrastando = TRUE;     //--- Está arrastando alguma coisa....
  InicioArrast = point;	

  if (SecaoAtual) //-- Se tem	secao desenhada...
  {
    double x,y; 
    POSITION PAtual;

    TransfPixelsXY(point,x,y);  //--- Pega as coordenadas (em metros) do ponto local o mouse está.

    PAtual = VerfPonto(x,y);

    //--- so podem ser alterados pontos de terreno ou os de projeto que estiverem fora da seção tipo

    const CSecao& SecAtual(Secoes.GetAt(SecaoAtual));

    if(Tipo == TERRENO || Tipo == PERFILGEOLOGICO || Tipo == PERFILGEOLOGICOOFFSETS || (Tipo == PROJETO &&  SecAtual.PodeAlterar(SecAtual.Projeto.GetAt(PAtual).x)))
    {
      ArrastandoPonto = !EMedicaoD || Tipo == MEDICAO; //--- Medicao so pode alterar a medicao...	

      if (!EMedicaoD)        
      {
        //--- Salva o ponto sendo alterado para poder restaurá-lo posteriormente.

        switch(Tipo)
        {
        case PROJETO: PontoAnterior  = Secoes.GetAt(SecaoAtual).Projeto.GetAt(PAtual); break;
        case TERRENO: PontoAnterior = Secoes.GetAt(SecaoAtual).Terreno.GetAt(PAtual); break;
        case PERFILGEOLOGICO: PontoAnterior =  const_cast<CDadosPerfisGeologicos*>(&*itPerfGeo)->PegaPonto(PAtual); break; 
        case PERFILGEOLOGICOOFFSETS: PontoAnterior = const_cast<CDadosPerfisGeologicos*>(&*itPerfGeo)->PegaPontoOffSet(PAtual); break; 
        }
      }
    }
  }

  //CView::OnLButtonDown(nFlags, point);
}

void DesSecaoView::OnLButtonUp(UINT nFlags, CPoint point) 
{
  //--- Se estiver arrastando um ponto e for o segundo click (Terminar arrasto = TRUE)
  //--- finaliza o arrasto

  if (!SecaoAtual && !DesenharSecAvulsas && !DesenharTalvegues && !DesenharHistogramas) return;  //--- não tem secao? retorna...

  if (TerminarArrasto)
  {
    Arrastando = ArrastandoPonto = TerminarArrasto = FALSE;

    //--- Se o ponto alterior era o do gradiente e o valor de x foi mudado, precisamos inserir outro ponto no gradiente.

    if (SecaoAtual)
    {
      CSecao& SecaoAtu(Secoes.GetAt(SecaoAtual));

      if (PAtual != NULL)
      {
        Ponto& PontoAtual(SecaoAtu.Terreno.GetAt(PAtual));     //--- Pega o ponto atual (Alterado)
        PontoAtual.Descricao = PontoAnterior.Descricao;
        PontoAtual.Inserido = PontoAnterior.Inserido;
      }

      if (Tipo == TERRENO && Gradientes.BuscaGradiente(SecaoAtu.Estaca) == PontoAnterior.x)
      {
        Ponto& PontoAtual = SecaoAtu.Terreno.GetAt(PAtual);     //--- Pega o ponto atual (Alterado)

        //--- Se a diferença entre o x do ponto atual e o ponto anterior for pequena, apenas ajusta o x do ponto atual
        //--- para o x do gradiente, caso contrario interpola (ou extrapola) outro ponto no gradiente

        if (fabs(PontoAnterior.x - PontoAtual.x) < 0.0005)  PontoAtual.x = PontoAnterior.x;
        else
          SecaoAtu.Terreno.InsereOrdenada(Ponto(PontoAnterior.x, SecaoAtu.Terreno.Calculay(PontoAnterior.x)));

        //--- o gradiente foi alterado, então acerta a cota na estaca do terreno pois esta é que é usada no desenho do perfil

        SecaoAtu.Terreno.Estaca.Cota = SecaoAtu.Terreno.PegaPontoAtual().y;   //--- O atual é o gradiente que acabou de ser calculado

        CMainFrame* MainFrame((CMainFrame*)AfxGetMainWnd());

        if (MainFrame->GreideAtivo) ((CWnd*)(MainFrame->DesPerfil))->RedrawWindow();    //--- O ponto de terreno do gradiente foi alterado, mostra a alteração no perfil.
      }
    }
    RedrawWindow();  //--- O redesenho é obrigatório no final do arrasto, independentemente dos ticks
  }
  
  if(Particionando)
  {
    if (itBaciaAtual != BaciasHidrograficas.end())
    {
      setPontos& itParticoesTalvPrincipal(itBaciaAtual->PegaParticoesTalveguePrincipal());

      itParticoesTalvPrincipal.insert(MouseXY->m_DOCoorX);

      AlterouBacias = true;

      BaciasAlteradas.insert(itBaciaAtual);
    }

    xParticaoAtual = INFINITO;
  }

  CursorAtual = Arrastando ? ArrastandoPonto ? PermitirAlterar ? IDC_CROSS : IDC_NO : IDC_SIZEALL : IDC_HAND;
  SetCursor(LoadCursor(NULL,CursorAtual));
}

void DesSecaoView::TransfPixelsXY(CPoint& Ponto,double& Coorx,double& Coory)
{
  //--- Mapeia as cordenadas do ponto que estão em pixels para metros.
  //---  
  //--- O cálculo é feito pela fórmula.
  //--- Sendo.
  //---  px           -> Valor da coordenada x em pixels.
  //---  p(x)         -> Valor do afastamento (x) do ponto.
  //---  Escala(x)    -> Valor da escala(Pixels/metro) no eixo x.
  //---  Resolução(x) -> Valor da resolução no eixo x (em pixels).
  //---  D(x)         -> Valor do Deslocamento (tecla seta para a direita ou para a esquerda)
  //---  C(x)         -> Valor da coordenada x do ponto em metros.
  //---
  //---  C(x) = (p(x) - Resolucao(x) / 2 - D(x)) / Esc(x).
  //---

  Coorx = (Ponto.x - ResolucaoXDiv2 - Deltas[X]) / Escala[X];

  //---  E para y:   (O sentido é invertido...)
  //---
  //---  C(y) = (Resolucao(y) - p(y) + D(y)) / Esc[Y] + CotaInferior;
  //---

  Coory = (Resolucao[Y] - Ponto.y + Deltas[Y]) / Escala[Y] + CotaInferior;
}

void DesSecaoView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  if (!PermitirAlterar || !SecaoAtual) return;  //--- Não tem seção? retorna.

  if(DesenharPerfisGeologicos)      //--- So pode incluir pontos no terreno primitivo
  {
    AfxMessageBox("So podem ser incluidos pontos no terreno primitivo, caso seja esta sua intenção, desligue o botão de desenho dos perfís geológicos.",MB_ICONASTERISK);

    return;
  }

  Arrastando = false;    //--- Arrastando ficou true no buttondown, neste caso é incluir ponto

  CSecao& SecAtual = Secoes.GetAt(SecaoAtual);  

  double x,y;
  TransfPixelsXY(point,x,y);  //--- Transforma as coordenadas de pixels para metros.

  //--- Inclui o ponto no perfil.

  if (MouseXY->m_RATerreno.GetCheck()) 
  {
    //--- Se caso estiver criando uma nova seção de terreno é preciso acertar o valor da estaca e da cota da estaca
    //--- pois ambos estão com INFINITO para indicar que esta seção não tem terreno.nha terreno.

    if (!SecAtual.Terreno.GetCount())   //--- Se não tem terreno, está inserindo uma nova seção de terreno...
    {
      //  x = Gradientes.BuscaGradiente(SecAtual.Estaca);     //--- O primeiro ponto tem que ser o gradiente.

      //--- Atualiza os valores da estaca do terreno (estão com infinito pois s seção não tem terreno(agora passa a ter))

      SecAtual.Terreno.Estaca = SecAtual.Projeto.Estaca;  
      SecAtual.Terreno.Estaca.Cota = y;
      
      if(!AlterouSecao || !AlterouSecoes)
      {
        AlterouSecao = true;    //--- houve Alteração na secao
        ((CMainFrame*) AfxGetMainWnd())->InsereDesenhoAlterado(Trecho,std::string("DesSecao"),this);
      }

      //--- O ponto de terreno do gradiente foi incluido, mostra a alteração no perfil.

      if(((CWnd*)((CMainFrame*)AfxGetMainWnd())->DesPerfil)) ((CWnd*)((CMainFrame*)AfxGetMainWnd())->DesPerfil)->RedrawWindow();    
    }

    Ponto PontoInserido(Ponto(x, y, INFINITO, INFINITO, INFINITO, INFINITO, CString("Ponto inserido - Seções"), false, true, CString(""), -1));
    SecAtual.Terreno.InsereOrdenada(PontoInserido,FALSE,TRUE);
    PropagaInsercaoPerfGeologico(PontoInserido);
    InseriuPontos = true;

    if(!AlterouSecao)
    {
      AlterouSecao = AlterouSecoes =true;    //--- houve Alteração na secao
      ((CMainFrame*) AfxGetMainWnd())->InsereDesenhoAlterado(Trecho,std::string("DesSecao"),this);
    }

    AlterouSecao = true;
  }
  else  
  {
    if(SecAtual.PodeAlterar(x))     //--- se não estiver na seção tipo
    {
      if (!EMedicaoD) 
      {
        SecAtual.Projeto.InsereOrdenada(Ponto(x,y,INFINITO,INFINITO,INFINITO,INFINITO,CString("PONINS")),FALSE,TRUE);

        if(!AlterouSecao)
        {
          AlterouSecao = true;    //--- houve Alteração na secao
          ((CMainFrame*) AfxGetMainWnd())->InsereDesenhoAlterado(Trecho,std::string("DesSecao"),this);
        }
      }
      else 
      {
        CSecao* SecMedAtual(PegaSecMedicao(SecAtual.Projeto.Estaca));

        if(SecMedAtual) 
        {
          SecMedAtual->Projeto.InsereOrdenada(Ponto(x,y),FALSE,TRUE);

          if(!AlterouSecao)
          {
            AlterouSecao = AlterouSecoes =true;    //--- houve Alteração na secao
            ((CMainFrame*) AfxGetMainWnd())->InsereDesenhoAlterado(Trecho,std::string("DesSecao"),this);
          }
        }
      }
    }
  }

  if(AlterouSecao)  
  {
    RedrawWindow();
    AlterouSecoes = true;
  }
}

void DesSecaoView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  if (!SecaoAtual) return;   //--- não tem seção retorna.

  double x,y;
  TransfPixelsXY(point,x,y);  //--- Pega as coordenadas (em metros) do ponto local o mouse está.

  if (VerfPonto(x,y))  //--- Se o mouse estiver sobre um ponto...
  {                    //--- Deleta-o, mas verifica se é o gradiente pois o gradiente não pode ser excluido
    CSecao& SecAtual = Secoes.GetAt(SecaoAtual);  

    if (Tipo == TERRENO)  
    {
      //--- Se estiver tentando deletar o gradiente avisa que ele não pode ser deletado 

      if (Gradientes.BuscaGradiente(SecAtual.Estaca) != SecAtual.Terreno.PegaPontoAtual().x) SecAtual.Terreno.ExcluiPonto();
      else 
      {
        monolog.mensagem(11,-1);   //--- O gradiente não pode ser deletado!!!
        return;
      }
    }
    else if (Tipo == PROJETO) SecAtual.Projeto.ExcluiPonto();
    else SecAtual.Pavimento.ExcluiPonto();

    AlterouSecao = AlterouSecoes = Tipo == TERRENO || Tipo == PROJETO;

     if(AlterouSecao)
     {
        ((CMainFrame*) AfxGetMainWnd())->InsereDesenhoAlterado(Trecho,std::string("DesSecao"),this);
     }

    RedrawWindow();   //--- Redesenha a secao, sem o ponto
  } 
}

void DesSecaoView::OnRButtonUp(UINT nFlags, CPoint point) 
{
  static BOOL EstouAqui(FALSE);

  if (!(nFlags & MK_CONTROL)) 
  {		
    if (ArrastandoPonto)    //--- Esta Arrastando um ponto? então o operador desistiu do arrasto
    {
      CSecao& SecAtual = Secoes.GetAt(SecaoAtual);  //--- Pega a seção atual.

      //--- Sim, Volta o ponto ao que era no início do arrasto.

      if (Tipo == PROJETO) SecAtual.Projeto.AlteraPonto(PontoAnterior,PAtual);
      else SecAtual.Terreno.AlteraPonto(PontoAnterior,PAtual);

      Arrastando = ArrastandoPonto = FALSE;

      //--- Mostra o ponto alterado no dialogo.

      CRect Rect;
      this->GetWindowRect(&Rect);
      DPopUpPonto.MostraDescricao(point.x+20+Rect.left,point.y+20+Rect.top,PreparaDescricao(Tipo,SecAtual));

      RedrawWindow();
    }
    else OnConfigtoolsec();  //--- Mostra a caixa de configuração da janela.

    return;  //--- Se a tecla CTRL não está pressionada retorna nada mais a fazer....
  }

  if (EstouAqui) 
  {
    ASSERT(0);
    return;   //--- Se já estiver entrando com um ponto ponto, volta, evita reentradas.
  }

  double x,y,xParticao(INFINITO);
  TransfPixelsXY(point,x,y);  //--- Pega as coordenadas (em metros) do ponto local o mouse está.

  POSITION Atual(VerfPonto(x,y,&xParticao)); 

  CSecao& SecAtual(Secoes.GetAt(SecaoAtual));  //--- Pega a seção atual.

  //--- Se o mouse estiver sobre um ponto e pode alterar, mostra o dialogo de alterar ponto

  if (Atual && (Tipo == TERRENO || (Tipo == PROJETO &&  SecAtual.PodeAlterar(SecAtual.Projeto.GetAt(PAtual).x))))   
  { 
    Perfil& CPerfil(Tipo == TERRENO ? SecAtual.Terreno : Tipo == PROJETO ? SecAtual.Projeto : SecAtual.Pavimento); 

    Ponto& PontoAtual(CPerfil.GetPrev(Atual));
    double iAnterior(Atual ? CPerfil.GetAt(Atual).i : INFINITO),iAtual(PontoAtual.i),x(PontoAtual.x),y(PontoAtual.y),
           iAnteriorAntes(iAnterior),iAtualAntes(iAtual),xAntes(x),yAntes(y);

    //--- Salva os dados anteriores.

    EstouAqui = TRUE;  //--- Evita reentradas.

    //--- Pede o novo ponto.

    CDEntraPonto DEntraPonto(&iAnterior,&iAtual,&x,&y,point,Tipo,this);

    //--- Executa o diálogo.

    if (DEntraPonto.DoModal() == IDOK)
    {
      //--- Se Alterou o x ou o y do ponto, procede a alteração.

      if (xAntes != x || yAntes != y)	
      {
        PontoAtual.x = x;
        PontoAtual.y = y;
      }
    }

    RedrawWindow(); 
  }
  EstouAqui = FALSE;	
}

void DesSecaoView::OnCalcoffsets() 
{	  
  Posiciona(INFINITO,-INFINITO);  //--- Mudar o nome para redesenha

  AlterouSecoes = true;
}

void DesSecaoView::OnUpdatePrimeirasecao(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!DesenharSecAvulsas && SecaoAtual != Secoes.GetHeadPosition());
}

void DesSecaoView::OnUpdateUltimasecao(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!DesenharSecAvulsas && SecaoAtual != Secoes.GetTailPosition());
}

void DesSecaoView::OnUpdateCalcOffSets(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!DesenharSecAvulsas);
}

int DesSecaoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  lpCreateStruct->dwExStyle |= WS_EX_PALETTEWINDOW;
  lpCreateStruct->dwExStyle &= !(WS_MAXIMIZEBOX | WS_SYSMENU |WS_MINIMIZEBOX);
  lpCreateStruct->dwExStyle |=	WS_EX_TOPMOST ; 

  if (CView::OnCreate(lpCreateStruct) == -1)	return -1;

  return 0;
}

void DesSecaoView::MudarEscala(int Fator)
{
  if(Fator <= 0) Fator = 1;

  if (DesenharTalvegues)
  {
    Escala[X] = EscalaTalv[X] = Fator / 1000.0;
    Escala[Y] = EscalaTalv[Y] = EscalaTalv[X] * RazaoHV;
  }
  else
  {
    if (DesenharHistogramas)
    {
      Escala[X] = EscalaHipso[X] = Fator ;
      Escala[Y] = EscalaHipso[Y] = EscalaHipso[X] / RazaoHV;  //10
    }
    else
    {
      Escala[X]= Escala[Y] = EscalaSecoes[Y] = Fator;
      Escala[X] *= RazaoHV;
    }
  }
  
  CalculaParametros();
  RedrawWindow();
}

void DesSecaoView::MudarRazao(double Fator)
{
  if (DesenharTalvegues)
  {
    Escala[Y] = Escala[X] * Fator;
    RazaoHV = Fator;
  }
  else
  {
    if (DesenharHistogramas)
    {
      Escala[Y] = Escala[X] / Fator;  //10
    }
    else
    {
      RazaoHV = Fator;
      EscalaSecoes[X] = EscalaSecoes[Y] * Fator;

      Escala[X] = EscalaSecoes[X];
      Escala[Y] = EscalaSecoes[Y];
    }
  }

  CalculaParametros();
  RedrawWindow();
}

void DesSecaoView::Posiciona(double x, double Cota, bool VeioDoComboProcurar, int Tipo, std::string* pNome)
{
  if (x != INFINITO)
  {
    if (DesenharSecAvulsas)              //--- Estava desenhando seções avulsas acerta o combo procurar para secoes tranversais
    {
      PreencheComboProcurar((CChildFrame*)GetParentFrame());
      DesenharSecAvulsas = false;
      ((CMainFrame*)AfxGetMainWnd())->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTSECAVULSASECOES, false);
    }

    POSITION SecaoAtualRasc(Secoes.Posiciona(CEstaca(x)));

    if (!SecaoAtualRasc) return;

    while (SecaoAtualRasc && FiltraMostrar(SecaoAtualRasc)) Secoes.GetNext(SecaoAtualRasc);

    if (!SecaoAtualRasc)
    {
      if (VeioDoComboProcurar)
      {
        ((CComboProcurarDesSecao*)pComboFind)->SetCurSel(-1);
        AfxMessageBox("Seção filtrada, não atende aos requisitos do filtro atual (Ver configuração)");
      }
      return;
    }

    SecaoAtual = SecaoAtualRasc;

    if (!VeioDoComboProcurar) pComboFind->SetCurSel(-1); //--- Limpa o combo procurar, se não veio dele
  }

  if (DesenharSecAvulsas || Tipo == 1)   //--- 1 == Secao Avulsa
  {
    if (VeioDoComboProcurar || pNome != NULL) //---- Se Tem nome da seção Avulsa veio da sincronização com o desenho geométrico
    {
      if (pNome != NULL) UltimaSecaoAvulsa = pNome->c_str();

      const CList<CSecaoAvulsa, CSecaoAvulsa&>& LSecoesAvulas(SecoesAvulsas.PegaLSecoesAvulsas());

      POSITION PSecAvAtual(LSecoesAvulas.GetHeadPosition());

      for (; PSecAvAtual && LSecoesAvulas.GetAt(PSecAvAtual).Nome.compare(UltimaSecaoAvulsa) != 0; LSecoesAvulas.GetNext(PSecAvAtual));

      if (PSecAvAtual != NULL)
      {
        SecaoAvulsaAtual.Terreno = LSecoesAvulas.GetAt(PSecAvAtual);
        SecaoAvulsaAtual.AcidentesTopograficos = SecoesAvulsas.PegaLSecoesAvulsas().GetAt(PSecAvAtual).AcidentesTopograficos;
        SecaoAvulsaAtual.AlinhamentosTerrap = SecoesAvulsas.PegaLSecoesAvulsas().GetAt(PSecAvAtual).AlinhamentosTerrap;
        SecaoAvulsaAtual.Projeto = SecoesAvulsas.PegaLSecoesAvulsas().GetAt(PSecAvAtual).LstPontosProjeto;
        SecaoAvulsaAtual.AcidentesTopograficos = SecoesAvulsas.PegaLSecoesAvulsas().GetAt(PSecAvAtual).AcidentesTopograficos;
      }
      else
      {
        SecaoAvulsaAtual.Terreno.RemoveAll();
        SecaoAvulsaAtual.AcidentesTopograficos.RemoveAll();
        SecaoAvulsaAtual.AlinhamentosTerrap.RemoveAll();
        SecaoAvulsaAtual.Projeto.RemoveAll();
        SecaoAvulsaAtual.AcidentesTopograficos.RemoveAll();
      }

      if (pNome != NULL)
      {
        DesenharSecAvulsas = false;        //--- Vai ficar true no OnButSecAvulsas
        OnButSecAvulsas();                 //--- Para preencher o combo com seções avulsas
        DesenharPerfisGeologicos = false;

        if (DesenharSecAvulsas)              //--- Estava desenhando seções tranversais acerta o combo procurar para secoes tranversais
        {
          ((CMainFrame*)AfxGetMainWnd())->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTSECAVULSASECOES, true);
        }
      }

      RedrawWindow();

      return;
    }
  }

  if (DesenharTalvegues || Tipo == 2)   //--- 2 == Talvegues
  {
    if (VeioDoComboProcurar || pNome != NULL) //---- Se Tem nome do talvegue veio da sincronização com o desenho geométrico
    {
      if (pNome != NULL) UltimoTalvegue = pNome->c_str();

      itBaciaAtual = BaciasHidrograficas.begin();

      for (; itBaciaAtual != BaciasHidrograficas.end(); ++itBaciaAtual)
      {
        if(itBaciaAtual->PegaDadosBacia().NomeBacia == *pNome) break;
      }

      if (itBaciaAtual != BaciasHidrograficas.end())
      {
        setPontos& PerfilTalvegue(itBaciaAtual->PegaPerfTalvPrincipal());

        TalvegueAtual.Terreno.LimpaTudo();

        for (itsetPontos it = PerfilTalvegue.begin(); it != PerfilTalvegue.end(); ++it)
        {
          TalvegueAtual.Terreno.AddTail(*(const_cast<Ponto*>(&*it)));
        }
      }

      RedrawWindow();

      return;
    }
  }

  if (DesenharHistogramas || Tipo == 3)              //--- 3 == Histogramas
  {
    if (VeioDoComboProcurar || pNome != NULL)        //---- Se Tem nome do talvegue veio da sincronização com o desenho geométrico
    {
      if (pNome != NULL) UltimoHipsograma = pNome->c_str();

      ittylstBacias itBacia(BaciasHidrograficas.begin());

      for (; itBacia != BaciasHidrograficas.end(); ++itBacia)
      {
        if (itBacia->PegaDadosBacia().NomeBacia == *pNome) break;
      }

      if (itBacia != BaciasHidrograficas.end())
      {
        const tysetCotasArea& SetCotasAreas(itBacia->PegaSetCotasAreas());

        HipsogramaAtual.Terreno.LimpaTudo();

        for (ittysetCotasArea it = SetCotasAreas.begin(); it != SetCotasAreas.end(); ++it)
        {
          HipsogramaAtual.Terreno.AddTail(Ponto(it->AreaCN, it->Cota, it->Inclinacao));
        }

        HipsogramaAtual.Terreno.Estaca.Descricao = UltimoHipsograma;     //--- Guarda o nome da bacia (usado na deleção de pontos do hipsograma)

      }

      RedrawWindow();

      return;
    }
  }
  else                //--- Else é seção transversal               
  {
    //--- Procura e desenha uma determinada seção.

    //--- Esse negócio de mandar os parametros com INFINITO e -INFINITO acabou que ficou confuso,
    //--- alterar, criando outro parametro indicando o q alterou, o greide, terreno ou a seção...

    //SecaoAtual =  x == INFINITO ? SecaoAtual : Secoes.Posiciona(CEstaca(x));

    //--- Se x = INFINITO vai iniciar uma nova seção;

    if (x != INFINITO)
    {
      SecaoAtual = Secoes.Posiciona(CEstaca(x));

      if (SecaoAtual)
      {
        CEstaca& EstacaAnterior = SecaoAnterior.Estaca;

        SecaoAnterior = Secoes.GetAt(SecaoAtual);  //--- Salva a secao sem alterações, para poder restaurá-la
      }
    }

    if (SecaoAtual)
    {
      //--- Atualiza o diálogo dos volumes

      DVolumes.Atualiza(SecaoAnterior);

      CSecao& SecAtual(Secoes.GetAt(SecaoAtual));
      Perfil ProjetoEditado(SecAtual.Projeto);

      MostraSecao(SecaoAtual);

      ((CComboProcurarDesSecao*)pComboFind)->Posiciona(SecAtual.Estaca);

      //if (VeioDoComboProcurar && SecaoAtual)
       // pComboFind->SetCurSel(pComboFind->FindString(pComboFind->GetCurSel(), SecAtual.Estaca));   //--- Atualiza a estaca no diálogo de procurar

      //--- Se o a cota for = INFINITO a alteração no greide excluiu o projeto nesta seção, (o promeiro ou o último) PIV foi alterdo e posicionado
      //--- antes (ou depois) desta seção, tirando o projeto desta seção, So resta então retirar i projeto desta seção

      if (Cota != INFINITO)     //--- Tem q haver pelo menos 2 pontos de projeto
      {
        //--- Se cota veio com -infinito o terreno foi alterado

        //-- Houve Alteração de cota?, ou seja o greide foi alterado? 

        //--- É preciso verificar também se a seção esta sem projeto, caso esteja o projeto tem que ser contruido
        //--- pois o usuário pode ter retirado o greide nesta seção e esta recolocando agora

        //--- Nestes casos a seção tem que ser remontada pois a alteração pode te-la alterado, de corte 
        //--- para aterro ou o offset no pavimento. 
        //--- Concluindo, Qualquer alteração implica na remontagem da seção de projeto.

        if (Cota == -INFINITO || x == INFINITO || (fabs(SecAtual.Estaca.Cota - Cota) > 0.0005) || !SecAtual.Projeto.GetCount())
        {
          //--- Se houve alteraçao de cota ou recriação do projeto, a seção precisa de ser remontada...

          if (!EMedicaoD)   //--- Medição não seção tipo, taludes de pavimento etc...
          {
            Perfil ProjetoAtual(Secoes.GetAt(SecaoAtual).Estaca);

            //--- Busca os pontos que foram inseridos via desenho e os mantem na seção de projeto

            CLPontos PontosInseridos;
            PegaPontosInseridos(Secoes.GetAt(SecaoAtual), PontosInseridos, Gradientes.BuscaGradiente(Secoes.GetAt(SecaoAtual).Estaca));

            SecTipo SecaoTipo;    //--- lista com os pontos da secao tipo.
            if (SecoesTipo.BuscaSecTipo(SecAtual.Estaca, SecaoTipo))      //--- Insere a seçao tipo na lista, se não tem seção tipo não faz nada
            {

              TaludesPavim TalPavimento;		//-- Cria  o talude de pavimento
              TaludesPavimento.BuscaTaludesPavim(SecAtual.Estaca, TalPavimento); //--- busca os taludes do pavimento

              ProjetoAtual.InsereLista(SecaoTipo);
              AplicaSuperElevacao(ProjetoAtual, Gradientes.BuscaGradiente(SecAtual.Estaca)); //--- Aplica a superelevaçao a seção tipo.
              AplicaSuperLargura(ProjetoAtual);     //--- Aplica a superlargura aos bordos          

              SecAtual.SecaoTipo.IgualPerfil(ProjetoAtual);
              SecAtual.Projeto = ProjetoAtual;
              SecAtual.Pavimento = SecAtual.Projeto;

              //--- insere os alargamentos

              Perfil PontosAInserir; //-- lista Temporária, Como os perfís a serem buscados podem ou não existir, não pode ser chamada por referência
              Alargamentos.BuscaAlargamento(SecAtual.Estaca, ESQUERDO, PontosAInserir); //--- Pega os pontos de alarg Esq.
              SecAtual.Projeto.InsereListaLado(PontosAInserir, ESQUERDO, false);        //--- Insere-os na secao de projeto.
              PontosAInserir.RemoveTudo();                                              //--- Limpa os pontos de alargamento esquerdo.
              Alargamentos.BuscaAlargamento(SecAtual.Estaca, DIREITO, PontosAInserir);  //--- Pega os pontos de alarg. Dir.
              SecAtual.Projeto.InsereListaLado(PontosAInserir, DIREITO, false);         //--- Insere-os na secao de projeto.

              //--- Se foram inseridos pontos com o mouse os mantém na seção

              if (PontosInseridos.GetCount()) SecAtual.Projeto.InsereLista(PontosInseridos);

              //--- Se alterou o x a cota no eixo tem q ser recalculada

              if (x != INFINITO) SecAtual.Estaca.Cota = SecAtual.Projeto.Estaca.Cota = Cota;    //--- atribui a nova cota da seção

              double ExessoLateral(0.0);
              double HLimpeza(AltLimpeza.BuscaH(SecAtual.Estaca, &ExessoLateral));

              SecAtual.AlteraYPontos(Cota);   //--- retira os taludes e os bordos
              SecAtual.CotaSecao(Gradientes.BuscaGradiente(SecAtual.Estaca), AltPavimento.BuscaH(SecAtual.Estaca), SecaoTipo);    //--- Cota a seção de projeto.
              Perfil TerrenoAtual(SecAtual.Terreno);                                                           //--- busca o perfil de terreno.
              if (fabs(HLimpeza) > 0.005) SecAtual.MoveTerrenoY(-HLimpeza);                                    //--- Limpa todo o terreno
              SecAtual.CalculaBordos(TalPavimento, -AltPavimento.BuscaH(SecAtual.Estaca), CanteirosCentrais);  //--- Calcula os bordos de pavimentaçao. e os ofsets em seção mixta
              RemovePontosEditadosProjeto(SecAtual, ProjetoEditado);                                           //--- A seção esta sendo remontada, os pontos movidos estão no lugar de origem, são removidos aqui
              Perfil ProjetoOriginalSemOffSets(SecAtual.Projeto);                                              //--- Salva a seção de projeto original para restaurar depois ds perfís geológicos   
              SecAtual.CalculaOffSets(TaludesCorte, TaludesAterro, CSecao::VUE_TERRENO_PRIMITIVO);             //--- finalmente, calcula os offsets., se os 2 foram calculados efetiva a alteração
              Perfil ProjetoOriginalComOffSets(SecAtual.Projeto);
              if (fabs(HLimpeza) > 0.005) SecAtual.MoveTerrenoY(HLimpeza);                     //--- volta ao terreno primitivo

              SecAtual.IncluiListaLimpeza(HLimpeza, ExessoLateral, SecAtual.Terreno);          //--- Aplica a limpeza

              //--- Calcula dos dados do perfil geológico

              tysettypaiitPerfildouble itCotasGeologicas;
              PerfilGeologico.CalculaCotasNaEstaca(SecAtual.Estaca, itCotasGeologicas);
              ittysettypaiitPerfildouble ItCotaGeologicaAtual(itCotasGeologicas.begin());
              Perfil TerrenoPrimitivo(TerrenoAtual);
              double DeltahPerfil(0.0);
              Ponto pCVs, OffSetEsq, OffSetDir;

              while (ItCotaGeologicaAtual != itCotasGeologicas.end())
              {
                TerrenoAtual = TerrenoPrimitivo;

                if (TerrenoAtual.Estaca.Cota != INFINITO)
                {
                  DeltahPerfil = ItCotaGeologicaAtual->second - TerrenoAtual.Estaca.Cota;

                  if (DeltahPerfil > -0.2)   //--- É o terreno natural?
                  {
                    CPerfilGeologico::AjustaLimpeza(SecAtual.Limpeza, TerrenoAtual);
                  }
                  else
                  {
                    TerrenoAtual.AlteraFaixaY(Ponto(-INFINITO, INFINITO, DeltahPerfil));                                  //---  define o perfil da camada
                  }

                  SecAtual.Terreno = TerrenoAtual;                                                                    //--- Perfil do terreno no perfil geológico 
                  SecAtual.Projeto = ProjetoOriginalSemOffSets;                                                       //--- Projeto sem os pontos de taludesm calculados anteriormente 
                  if (DeltahPerfil < -0.20) SecAtual.CalculaOffSets(TaludesCorte, TaludesAterro, CSecao::VUE_PERFIL_GEOLOGICO, &pCVs, &OffSetEsq, &OffSetDir);     //--- finalmente, calcula os offsets.
                  CDadosPerfisGeologicos DadosPerfGeologicoAtual(std::string(ItCotaGeologicaAtual->first->Descricao), SecAtual.Estaca.EstVirtual,
                    &pCVs, &OffSetEsq, &OffSetDir, &TerrenoAtual, DeltahPerfil);
                  PerfilGeologico.InsereDadosNotaServico(DadosPerfGeologicoAtual);
                }
                ++ItCotaGeologicaAtual;
              }

              SecAtual.Terreno = TerrenoPrimitivo;
              SecAtual.Projeto = ProjetoOriginalComOffSets;


              AlterouSecao = true;
            }
          }
          else
          {
            //--- Se for medição procura pela medição, se achar procura pela seção, se achar
            //--- basta excluir os taludes e recalcular os off-sets, já que seção não tem seção tipo alargamentos etc

            POSITION MedicaoAtual = ListaMedicoes.GetHeadPosition();

            if (MedicaoAtual)
            {
              CListaSecoes& LMedAtual = *ListaMedicoes.GetAt(MedicaoAtual);
              POSITION PMedAtual = LMedAtual.PosicionaNoMaisProximo(SecAtual.Projeto.Estaca);

              if (PMedAtual)
              {
                CSecao& SecMedAtual = LMedAtual.GetAt(PMedAtual);
                if (fabs(SecMedAtual.Projeto.Estaca - SecAtual.Projeto.Estaca) < 0.0005)
                {
                  SecMedAtual.ExcluiPonTaludes();
                  SecMedAtual.CalculaOffSets(TaludesCorte, TaludesAterro, TRUE);
                }
              }
            }
          }
        }
      }
      RedrawWindow();
    }
  }
}

void DesSecaoView::OnConfigtoolsec() 
{
  CRect Rect;

  GetClientRect(Rect) ;
  ClientToScreen(Rect);

  CMainFrame *MainFrame((CMainFrame*)AfxGetMainWnd());

  //---Mostra o dialogo, passa os ponteiros para poder alterar os toolbars  in line;

  int* Dados[15] = {&DesenharPavimento,&DesenharTerraplanagem,&DesenharTerreno,&DesenharMalha,&CotarMalha,&EquiMalha,&EquiCotas,&Ferramentas,&Escalas,&Procurar,
                    &Coordenadas,&Volumes,&Mostrar,&CircularPontos,&DesenharPerfisGeologicos};

  CDSelSecao SelSecao(this,MainFrame->mpCDesTool1,MainFrame->mpCDesTool2,MainFrame->mpCDesTool3,MainFrame->mpCDesTool4,MainFrame->DVolumes,Rect,true,&Dados[0],&Cores[0]);

  SelSecao.DoModal();

  DesenharPavimento = *Dados[0];
  DesenharTerraplanagem = *Dados[1];
  DesenharTerreno = *Dados[2];
  DesenharMalha = *Dados[3];
  CotarMalha = *Dados[4];
  EquiMalha = *Dados[5];
  EquiCotas = *Dados[6];
  Ferramentas = *Dados[7];
  Escalas = *Dados[8];
  Procurar = *Dados[9];
  Coordenadas = *Dados[10];
  Volumes = *Dados[11];
  Mostrar = *Dados[12];
  CircularPontos = *Dados[13];
  DesenharPerfisGeologicos = *Dados[14];


  RedrawWindow();
}

BOOL DesSecaoView::PreCreateWindow(CREATESTRUCT& cs) 
{
  cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,                // use any window styles
                                     AfxGetApp()->LoadStandardCursor(IDC_HAND),
                                     (HBRUSH) (COLOR_WINDOW + 1));                        // background brush

  return CView::PreCreateWindow(cs);
}

void DesSecaoView::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);

  //--- Centraliza o desenho dentro do novo tamanho da janela
  
  RECT Coord; 
  GetClientRect(&Coord);

  Resolucao[X] = Coord.right; 
  Resolucao[Y] = Coord.bottom;

  ResolucaoXDiv2 = Resolucao[X] / 2.0;     //--- Usada nas transformaões de coordenadas.
  ResolucaoYDiv2 = Resolucao[Y] / 2.0;     //--- Usada nos desenhos dos pontos.   



  CalculaParametros();
}

CSecao* DesSecaoView::PegaSecMedicao(CEstaca& Estaca) 
{
  if (!ListaMedicoes.GetHeadPosition()) return NULL;

  CListaSecoes& LMedAtual = *ListaMedicoes.GetHead();
  POSITION PMedAtual = LMedAtual.PosicionaNoMaisProximo(Estaca); 

  if (PMedAtual)
  {
    CSecao* SecMedAtual = &LMedAtual.GetAt(PMedAtual);

    return (fabs(SecMedAtual->Projeto.Estaca - Estaca) < 0.0005) ? SecMedAtual : NULL;
  }

  return NULL;
}

int DesSecaoView::PegaPontosInseridos(const CSecao& SecaoAtual,CLPontos& PontosInseridos,double Gradiente)
{
  if (!SecaoAtual.Projeto.GetCount()) return 0;

  Ponto PAtual = SecaoAtual.Projeto.GetHead();
  POSITION Atual = SecaoAtual.Projeto.GetHeadPosition();

  while(Atual) 
  {
    PAtual = SecaoAtual.Projeto.GetNext(Atual);
    if(PAtual.Descricao == "PONINS")
    {
      PAtual.y = PAtual.y - SecaoAtual.Projeto.Estaca.Cota; 

      /*
      if (PAtual.x < Gradiente)
      {
      if (Atual) PAtual.y = PAtual.y - SecaoAtual.Projeto.GetAt(Atual).y;
      else PAtual.y = 0.0;
      }
      else
      {
      SecaoAtual.Projeto.GetPrev(Atual);
      if (Atual) SecaoAtual.Projeto.GetPrev(Atual);

      if (Atual) PAtual.y = SecaoAtual.Projeto.GetAt(Atual).y - PAtual.y;
      else PAtual.y = 0.0;    
      }
      */
      PontosInseridos.InsereOrdenada(PAtual); 
    }
  }

  return PontosInseridos.GetCount();
}

void DesSecaoView::OnRestaurarsecao() 
{
  Secoes.GetAt(SecaoAtual) = SecaoAnterior;
  RedrawWindow();
  AlterouSecao = false;
}

void DesSecaoView::OnUpdateRestaurarsecao(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(AlterouSecao);	
}

void DesSecaoView::OnCalcvolumes() 
{
  //--- Grava as alterações do greide

  if( dialogar.DoModal(10) == IDOK)
  {
    CMainFrame* pMainFrame =((CMainFrame*)((CMainFrame*)AfxGetMainWnd()));

    if(AlterouSecoes) GravarSecoes(false);

    ///if (pMainFrame->DesPerfil && ((DesPerfilView*)((CMainFrame*)pMainFrame)->DesPerfil)->AlterouProjeto())   pode ter que calcular a nota tambem: podem ter sido inseridos pontos de terreno e os offsets tem q ser calculadoa
    {  
      //    ((DesPerfilView*)((CMainFrame*)pMainFrame)->DesPerfil)->GravarProjeto(false,false,false);

      // CString NomeProjeto;

      CalcNota DCalcNota(NomeProjeto,this,pMainFrame->AchaProjetoNoVetor());
      DCalcNota.DoModal();

      /*
      CDSecionar DSecionar(NomeProjeto,this);
      DSecionar.DoModal();
      */

    }

    //--- Calcula a nota de serviço
    //--- O dialogo chamado é o DCalcnota mas ele é modal então não o mostra e usa o 
    //--- dcalcnotavolumesdireto para mostrar o progress e as mensagens

    //--- THREADS COM DIALOGO NÃO MODAL NÃO ESTA FUNCIONANDO... VERIFICAR.

    //  CDCalcNotaVolumesDireto DCalcNotaVolumesDireto(this);
    //  DCalcNotaVolumesDireto.Create(this);                      //--- Direto, não modal
    //  DCalcNotaVolumesDireto.ShowWindow(true);

    //  CalcNota DCalcNota(&DCalcNotaVolumesDireto);

    //  DCalcNota.Create(NULL,NULL,&DCalcNotaVolumesDireto);      //--- Direto, não modal
    //  DCalcNotaVolumesDireto.SetWindowText(" Vias Urbanas e Estradas - Cálculo da nota de serviço: ");

    //  if (pMainFrame->DesPerfil)  DCalcNota.CalcularDireto();   //--- Se esta com o perfil calcula a nota
    //    DCalcNota.CalcularDireto();   //--- Se esta com o perfil calcula a nota

    /*   if (pMainFrame->DesPerfil)
    {
    CalcNota DCalcNota(this);
    DCalcNota.DoModal();
    }
    */
  }

  //--- Agora os volumes
  //--- Idem Idem

  //  CDCalcNotaVolumesDireto DCalcNotaVolumesDireto(this);
  //  DCalcNotaVolumesDireto.Create(this);                      //--- Direto, não modal
  //  DCalcNotaVolumesDireto.ShowWindow(true);

  //  CalcVolumes DCalcVolumes(NomeProjeto,&DCalcNotaVolumesDireto);

  //  DCalcVolumes.Create(this,&DCalcNotaVolumesDireto);                      //--- Direto, não modal
  //  DCalcNotaVolumesDireto.SetWindowText(" Vias Urbanas e Estradas - Cálculo dos Volumes: ");
  //  DCalcVolumes.CalcularDireto();

  CalcVolumes DCalcVolumes(NomeProjeto,this);
  DCalcVolumes.DoModal();

  ReleSecoes();                                          //--- Lê novamente as seções
  PerfilGeologico.ReleDados(NomeProjeto.GetBuffer());    //--- e os perfis geológicos

  RedrawWindow();
}

void DesSecaoView::ReleSecoes()
{
  AlterouSecao = AlterouSecoes = false;

  //--- Lê novamente as seções para atualizar os volumes

  if (SecaoAtual)
  {
    //--- Salva a estaca atual para poder mostrar os novos volumes dela

    SecAnterior = NULL;     //--- as seções serão relidas, limpa a anterior

    CEstaca EstacaAtual((Secoes.GetAt(SecaoAtual)).Terreno.Estaca);

    Secoes.LimpaTudo();            //--- Remove as seçoes atuais
    Secoes.Serialize(LER);         //--- Le a novas seções

    SecaoAtual = Secoes.Posiciona(EstacaAtual);

    Reinicializa();

    RedrawWindow();
  }
}

void DesSecaoView::Reinicializa()
{
  Secoes.DefRetangDes();                   //--- Define os retângulos mínimos para desenho das secões.
  Secoes.CalculaIs();                      //--- Calcula os is das retas das seções.

  //--- Calcula os is das medições

  POSITION MedicaoAtual = ListaMedicoes.GetHeadPosition();

  if(MedicaoAtual) UltMedicao = ListaMedicoes.GetHead();   //-- Salva a última medicao (para alterações)

  while (MedicaoAtual)
  {
    CListaSecoes& LMedAtual = *ListaMedicoes.GetNext(MedicaoAtual); 
    LMedAtual.CalculaIs();
  }

  if (Secoes.GetCount() > 0) 
  {
    DVolumes.AtualizaVolumeTotal(Secoes.PegaVolTotCorte(),Secoes.PegaVolTotAterro(),Secoes.PegaVolTotEmpola());
    DVolumes.Atualiza(Secoes.GetAt(SecaoAtual));
  } 
}

void DesSecaoView::OnUpdateGravar(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(AlterouSecoes || AlterouBacias || AlterouPerfilTalvegue);
}

void DesSecaoView::OnUpdateCalcvolumes(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(AlterouSecoes);	
}

void DesSecaoView::OnUpdateButSecAvulsas(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(SecoesAvulsas.PegaLSecoesAvulsas().GetCount() > 0);
}

BOOL DesSecaoView::OnEraseBkgnd(CDC* pDC) 
{
  return false;
}

std::string DesSecaoView::PreparaDescricao(int Tipo,const CSecao& SecaoAtual,double* pxParticaoTalvegue) 
{
  if(Tipo == TERRENO && DesenharPerfisGeologicos) Tipo = PERFILGEOLOGICO;    //--- Mostra a descrição do perfil geológico não a do terreno

  const Perfil& CPerfil = Tipo == TERRENO || Tipo == PERFILTALVEGUE ? SecaoAtual.Terreno :
                          Tipo == PROJETO ? SecaoAtual.Projeto : 
                          Tipo == PAVIMENTO ? SecaoAtual.Pavimento :
                          Tipo == ACITOPOGFORASUPERF ? SecaoAtual.AcidentesTopograficos :
                          Tipo == PERFILGEOLOGICO ? itPerfGeo->PegaPerfilGeologico() : 
                          Tipo == ALINHAMENTOSTERRAPL ? SecaoAtual.AlinhamentosTerrap : 
                          Tipo == PARTICAOTALVEGUE ? SecaoAtual.Terreno :
                          Tipo == HIPSOGRAMA ? SecaoAtual.Terreno : PerfOffSetsPerfGeologico;

  POSITION p(PAtual);                                //--- PAtual não pode ser alterado!!, copia-o em p
  Ponto PontoAtual(CPerfil.GetPrev(p));

  CString CSTipo[13] = {" Projeto"," Terreno"," Pavimento"," Limpeza"," Medição"," Medição Terraplanagem","     Perfil Geológico","Perfil Geológico","  Acidente Topográfico"," Terraplanagem", " Particao Talvegue", " Hipsograma","Perfil Talvegue"};

  double i1(p ? CPerfil.GetAt(p).i : INFINITO);

  std::strstream strstreamTemp;

  strstreamTemp << std::right << std::fixed << std::setfill(' ') << std::setw(14) << std::setprecision(3)
                << CSTipo[Tipo].GetBuffer();
	
  if(Tipo == PERFILGEOLOGICO || Tipo == PERFILGEOLOGICOOFFSETS) strstreamTemp << "\r\n " <<   itPerfGeo->PegaNomeTipoSolo();
//  else if(Tipo == ACITOPOGFORASUPERF) strstreamTemp << "\r\n " <<   itPerfGeo
  
  strstreamTemp << " \r\n Afast.= " << PontoAtual.x 
                << " \r\n Cota= " << PontoAtual.y;
				
  if(PontoAtual.Descricao.GetLength() && PontoAtual.Descricao.Find("Ala") == -1) strstreamTemp << " \r\n Tipo= " << PontoAtual.Descricao.GetBuffer(1);

  strstreamTemp << std::setprecision(4);

  if(i1!= INFINITO)
  {
    if(fabs(i1) >= 5000.0) strstreamTemp << " \r\n i1" << (i1 > 0 ? ">+5E3" : "<-5E3");
    else strstreamTemp << " \r\n i1 (%)= " << i1 * 100.0;
  }

  if(PontoAtual.Descricao.Find("Ala") == -1 && PontoAtual.i != INFINITO)
  {
    if(fabs(PontoAtual.i) > 5E3) strstreamTemp << " \r\n i2" << (PontoAtual.i > 0 ? ">+5E3" : "<-5E3");
    else strstreamTemp << " \r\n i2 (%)= " << PontoAtual.i * 100.0;
  }

  //--- Inclui o acidente topográfico

  if(PAciTopogAtual != NULL)
  {
    int AcidentetopogAtual = SecaoAtual.AcidentesTopograficos.GetAt(PAciTopogAtual).AcidenteTopografico;

    strstreamTemp << " \r\n" << CAciTopografico::Nome(AcidentetopogAtual);  
  }
  else
  {
    if (Tipo == ALINHAMENTOSTERRAPL)
    {
      strstreamTemp << " \r\n" << CAciTopografico::Nome(PontoAtual.AcidenteTopografico);
    }
    else
    {
      if (Tipo == PARTICAOTALVEGUE)
      {
        //--- Não vai usar o strstrstream

        strstreamTemp << std::ends;
        const std::string Ret(strstreamTemp.str());
        strstreamTemp.freeze(false);
       
        std::stringstream strstrTemp;

        strstrTemp << std::right << std::fixed << std::setfill(' ') << std::setprecision(0);

        strstrTemp << " Partição do Talvegue:"
                   << " \r\n" << "x = "
                   << std::setprecision(2) << *pxParticaoTalvegue << std::ends;

        return strstrTemp.str();
      }    
      else
      {
        if (Tipo == HIPSOGRAMA)
        {
          //--- Não vai usar o strstrstream

          strstreamTemp << std::ends;
          const std::string Ret(strstreamTemp.str());
          strstreamTemp.freeze(false);

          std::stringstream strstrTemp;

          strstrTemp << std::right << std::fixed << std::setfill(' ') << std::setprecision(0);

          strstrTemp << " Hipsograma:"
                     << " \r\n" << "% Área Total = "
                     << std::setprecision(2) << PontoAtual.x 
                     << " \r\n" << "Cota = "
                     << std::setprecision(3) << PontoAtual.y; 

         if(PontoAtual.z != INFINITO) strstrTemp << " \r\n" << "i = " << PontoAtual.z << " m/%";
                     
         strstrTemp << std::ends;

          return strstrTemp.str();
        }
      }
    }
  }

  strstreamTemp << std::ends;
  const std::string Ret(strstreamTemp.str()); 
  strstreamTemp.freeze(false);

  return Ret;
}

void DesSecaoView::OnNcLButtonDown2(UINT nHitTest, CPoint point) 
{
  // TODO: Add your message handler code here and/or call default

  GetParentFrame()->ShowWindow(SW_SHOWNOACTIVATE);   
  MostrarVolumes(Volumes > 0);
}

void DesSecaoView::OnButTrancarSecao()
{
  PermitirAlterar = PermitirAlterar == false;
}

void DesSecaoView::OnAlternartitulo()
{
  CChildFrame* CCFrm((CChildFrame*)GetParentFrame());

  if (CCFrm->GetStyle() & WS_CAPTION)
  {
    CCFrm->ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);
    DVolumes.ModifyStyle(WS_CAPTION, 0, SWP_FRAMECHANGED);
    CCFrm->m_wndStatusBar.ShowWindow(SW_HIDE);
    CCFrm->ShowControlBar(&CCFrm->m_wndStatusBar, false, true);

    ComTitulo = false;
  }
  else
  {
    CCFrm->ModifyStyle(0, WS_CAPTION, SWP_FRAMECHANGED);
    DVolumes.ModifyStyle(0, WS_CAPTION, SWP_FRAMECHANGED);
    CCFrm->m_wndStatusBar.ShowWindow(SW_SHOW);
    CCFrm->ShowControlBar(&CCFrm->m_wndStatusBar, true, true);

    ComTitulo = true;
  }
}

void DesSecaoView::Fechar()
{
  ((CChildFrame*)GetParentFrame())->Fechar();
}

void DesSecaoView::CalculaVolumes()
{
  CDCalcNotaVolumesDireto DCalcNotaVolumesDireto(this);
  DCalcNotaVolumesDireto.Create(this);

  CalcVolumes DCalcVolumes(NomeProjeto, this);
  DCalcVolumes.Create(this, &DCalcNotaVolumesDireto);                      //--- Direto, não modal

  DCalcNotaVolumesDireto.SetWindowText(" Vias Urbanas & Estradas - Cálculo da nota de serviço: ");
  DCalcNotaVolumesDireto.ShowWindow(true);

  DCalcVolumes.CalcularDireto();

  WaitForSingleObject(CEvent(), 700);      //--- Aguarda 0,7 segundos para sair

  AlterouSecoes = false;
}

void DesSecaoView::OnButvisualizartodasecao()
{
  CSecao* pSecAtual(nullptr);

  if (DesenharSecAvulsas)
  {
    pSecAtual = &SecaoAvulsaAtual;
    pSecAtual->DefRetangDes();
  }
  else
  {
    if (DesenharTalvegues)
    {
      pSecAtual = &TalvegueAtual;
      pSecAtual->DefRetangDes();
    }
    else
    {
      if (DesenharHistogramas)
      {
        pSecAtual = &HipsogramaAtual;
        pSecAtual->DefRetangDes();
      }
      else if (SecaoAtual) pSecAtual = &Secoes.GetAt(SecaoAtual);  //--- Vai desenhar  a secao atual.
    }
  }

  if (pSecAtual)
  {
    if (DesenharTalvegues)
    {
      double Deltax(pSecAtual->Maiores[X] - pSecAtual->Menores[X]),
             Deltay(pSecAtual->Maiores[Y] - pSecAtual->Menores[Y]);

      EscalaTalv[X] = Resolucao[X] / Deltax * 1000.0;
      EscalaTalv[Y] = Resolucao[Y] / Deltay;
      RazaoTalvHV = 10.0;

     // if (EscalaTalv[X] > EscalaTalv[Y]) EscalaTalv[X] = EscalaTalv[Y];

      ((CChildFrame*)GetParentFrame())->Deslizantes.MudarEscala(ceil(EscalaTalv[X]));
      ((CChildFrame*)GetParentFrame())->Deslizantes.MudarRazao(RazaoTalvHV);

      DeltasTalvegues[X] = -Resolucao[X] / 2.0;
      DeltasTalvegues[Y] = 0.0;

      Deltas[X] = DeltasTalvegues[X];
      Deltas[Y] = DeltasTalvegues[Y];

      Escala[X] = EscalaTalv[X];
      Escala[Y] = EscalaTalv[Y];

      MudarEscala(Escala[X]);

      RedrawWindow();
    }
    else
    {
      if (DesenharHistogramas)
      {
        double Deltax(pSecAtual->Maiores[X] - pSecAtual->Menores[X]),
               Deltay(pSecAtual->Maiores[Y] - pSecAtual->Menores[Y]);

        EscalaHipso[X] = Resolucao[X] / Deltax * 10.0;
        EscalaHipso[Y] = Resolucao[Y] / Deltay * 10.0;
        RazaoHipsoHV = 1.0;

        if (EscalaHipso[X] > EscalaHipso[Y]) EscalaHipso[X] = EscalaHipso[Y];

        ((CChildFrame*)GetParentFrame())->Deslizantes.MudarEscala(int(EscalaHipso[X]));
        ((CChildFrame*)GetParentFrame())->Deslizantes.MudarRazao(RazaoHipsoHV);

        DeltasHipso[X] = -Resolucao[X] / 2.0;
        DeltasHipso[Y] = 0.0;

        Deltas[X] = DeltasHipso[X];
        Deltas[Y] = DeltasHipso[Y];

        Escala[X] = EscalaHipso[X];
        Escala[Y] = EscalaHipso[Y];

        MudarEscala(Escala[X]);

        RedrawWindow();
      }
      else
      {
        Deltas[X] = (pSecAtual->Maiores[X] - pSecAtual->Menores[X]) / 2.0 * Escala[X];
        Deltas[X] -= Resolucao[X] / 2.0;
        Deltas[Y] = (pSecAtual->Maiores[Y] - pSecAtual->Menores[Y]) / 2.0 * Escala[Y];

        MudarEscala(Escala[X]);

        RedrawWindow();
      }
    }
  }
}

void DesSecaoView::OnButmostsecavulsas() 
{
}

BOOL DesSecaoView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  pInfo->m_bContinuePrinting = TRUE; 
  return DoPreparePrinting(pInfo);
}

void DesSecaoView::OnGravarSecoes()
{
//--- Aguardar
/*
  bool IncluirPontosSuperf(false);
  CDiaGravarAltSecoes DiaGravarAltSecoes(this,&IncluirPontosSuperf);

  if (DiaGravarAltSecoes.DoModal() == IDC_OK)
  {
    GravarSecoes(true);
    if(IncluirPontosSuperf) IncluiPontosIseridosNaSuperficie();
    ((CMainFrame*)AfxGetMainWnd())->RemoveDesenhoAlterado(Trecho, std::string("DesSecao"));
  }
  */
  if(AlterouSecoes) GravarSecoes(true);
  if (AlterouBacias) GravaBaciasHidrograficas((((CMainFrame*)AfxGetMainWnd())->m_sNomeComp.GetBuffer()));
 }

void DesSecaoView::GravarSecoes(bool Perguntar) 
{ 
  int Resposta(IDOK), RespostaInserir(IDOK);

  bool IncluirPontosSuperf(false);

  if (!Perguntar || dialogar.DoModal(4, CString(" Secões - " + CString(Trecho.c_str()))) == IDOK)
  {
    Secoes.Serialize(OPGRAVAR);
    PerfilGeologico.Serialize(std::string(NomeProjeto), GRAVAR);
    SecoesAvulsas.Serialize(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual(),1);

    if (AlterouPerfilTalvegue) SalvaPerfilTalvegue();

    if (AlterouBacias || AlterouPerfilTalvegue) GravaBaciasHidrograficas(((CMainFrame*)AfxGetMainWnd())->m_sNomeComp.GetBuffer());

    AlterouSecao = AlterouSecoes = AlterouPerfilTalvegue = false;
  }

  if (InseriuPontos)   
  {
    if (((CMainFrame*)AfxGetMainWnd())->TipoSuperficie != 1)       //--- Tipo superficie == 1 ==> rones ou Gerenciadores de mapas (não gera a superficie)
    {
      if (!DesenharSecAvulsas)
      {

        CDiaGravarPonInsSecoes DiaGravarPonInsSecoes(this);          //--- Incluir pontos inseridos na superficie?  

        if (DiaGravarPonInsSecoes.DoModal() == IDOK)
        {
          IncluiPontosInsRemNaSuperficie(1);  //--- Gera arquivo de pontos .xyz
        }
        else
        {
          IncluiPontosInsRemNaSuperficie(0); //--- Gera arquivo .rel
        }

        ((CMainFrame*)AfxGetMainWnd())->RemoveDesenhoAlterado(Trecho, std::string("DesSecao"));
        ((CMainFrame*)AfxGetMainWnd())->SecaoAlterada = true;

        InseriuPontos = false;
      }
    }
    else
    {
	  AfxMessageBox("Os pontos inseridos ou removidos nesta edição não farão parte da superfície já que esta superfície "
                    "foi importada de drones ou de gerenciadores de mapas. A ausência de off-sets em alguma "
                    "seção poderá impedir o cálculo da terraplanagem. Neste caso, se desejar calcular o "
                    "projeto de terraplanagem será preciso recessionar todo o terreno aumentando a equidistância "
                    "lateral da seção tal que o off-set nestas seções possa ser encontrado. Caso faça esta opção, "
                    "todos os cálculos, nota de serviço, volumes e todos os restantes serão invalidados e terão "
                    "que ser solicitados novamente.Neste caso, os pontos removidos nesta edição tabém serão "
                    "removidos da superfície.", MB_ICONASTERISK);
	
      IncluiPontosInsRemNaSuperficie(0); //--- Gera arquivo .rel
    }
  }
  ((CMainFrame*)AfxGetMainWnd())->RemoveDesenhoAlterado(Trecho, std::string("DesSecao"));

  AlterouSecoes = true;
};

void DesSecaoView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  if (GetKeyState(VK_SHIFT) < 0)
  {
    nChar = toupper(nChar);

    switch (nChar)
    {
      case 'N': DesenharTerraplanagem = DesenharTerraplanagem == 0; break;
      case 'P': DesenharPavimento = DesenharPavimento == 0; break;
      case 'M': DesenharMalha = DesenharMalha == 0; break;
      case 'T': DesenharTerreno = DesenharTerreno == 0; break;
      case 'O': CotarMalha = CotarMalha == 0; break;
      case 'C': CircularPontos = CircularPontos == 0; break;
      case 'G': DesenharPerfisGeologicos = DesenharPerfisGeologicos == 0; break;
      case '2': Mostrar & 0x1 ? Mostrar &= 0x11110 : Mostrar |= 0x1; break;
      case '1': Mostrar & 0x10 ? Mostrar &= 0x11101 : Mostrar |= 0x10; break;
      case 'R': Mostrar & 0x100 ? Mostrar &= 0x11011 : Mostrar |= 0x100; break;
      case 'E': Mostrar & 0x1000 ? Mostrar &= 0x10111 : Mostrar |= 0x1000; break;
      case 'S': Mostrar & 0x10000 ? Mostrar &= 0x01111 : Mostrar |= 0x10000; break;
    }

    RedrawWindow();

    if (((CMainFrame*)AfxGetMainWnd())->GreideAtivo) ((DesPerfilView*)((CMainFrame*)AfxGetMainWnd())->DesPerfil)->OnKeyUp(nChar, nRepCnt, nFlags);

    return;
  }

  if (!DesenharTalvegues && !SecaoAtual) return;    //--- Só os talveguesa não tem secao atual

  CSecao& SecAtual = DesenharTalvegues ? TalvegueAtual :
    DesenharSecAvulsas ? SecaoAvulsaAtual : Secoes.GetAt(SecaoAtual);

  switch (nChar)
  {
    case VK_PRIOR: for (int i = 0; i < 10; i++) OnButanterior(); break;
    case VK_NEXT: for (int i = 0; i < 10; i++) OnButproxima(); break;
    case VK_UP: for (unsigned int C = 0; C < nRepCnt; C++); OnButanterior(); break;
    case VK_SPACE:
    case VK_DOWN: for (unsigned int C = 0; C < nRepCnt; C++) OnButproxima(); break;
    case  VK_DELETE:                            //--- deleta um PIV sobre o mouse
    {
      RedrawWindow();      //--- Necessário para atualizar a tela senão a tecla del não pega a 1a vez (?????)

      if (Tipo == PARTICAOTALVEGUE)
      {
        if (itBaciaAtual != BaciasHidrograficas.end())
        {
          if (itBaciaAtual->TemTalveguePrincipal())
          {
            setPontos& Particoes(itBaciaAtual->PegaParticoesTalveguePrincipal());
            itsetPontos itParticao(Particoes.upper_bound(MouseXY->m_DOCoorX - Toleranciax));

            if (itParticao != Particoes.end())
            {
              if (fabs(itParticao->x - MouseXY->m_DOCoorX) < Toleranciax)
              {
                Particoes.erase(itParticao);
                AlterouBacias = true;

                RedrawWindow();

                return;
              }
            }
          }
        }
      }
      else
      {
        if (Tipo == HIPSOGRAMA)
        {
          if (SecaoAtual != NULL)
          {
            itBaciaAtual = BaciasHidrograficas.begin();

            for (; itBaciaAtual != BaciasHidrograficas.end(); ++itBaciaAtual)
            {
              if (itBaciaAtual->PegaDadosBacia().NomeBacia == HipsogramaAtual.Terreno.Estaca.Descricao.GetBuffer()) break;
            }

            if (itBaciaAtual != BaciasHidrograficas.end())
            {
              if (AfxMessageBox("Confirma a remoção deste ponto do hipsograma?", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
              {
                itBaciaAtual->RemovePontoHistograma(HipsogramaAtual.Terreno.GetAt(PAtual).y);
                HipsogramaAtual.Terreno.ExcluiPonto(PAtual);

                AlterouBacias = true;

                RedrawWindow();
              }
            }
          }
        }
        else
        {
          //--- Tipo = PERFILTALVEGUE
        }
      }

      if (!SecaoAtual) return;   //--- não tem seção retorna.

      if (POSPonAnterior)  //--- Se o mouse estiver sobre um ponto...
      {                    //--- Deleta-o, mas verifica se é o gradiente pois o gradiente não pode ser excluido
        if (Tipo == TERRENO || Tipo == PERFILTALVEGUE)      //-- O perfil do talvegue fica no terreno da seçaao
        {
          if (DesenharSecAvulsas)
          {

          }

          //--- Se estiver tentando deletar o gradiente avisa que ele não pode ser deletado 
          //--- Agora pode 11052020 

          if (1 || Gradientes.BuscaGradiente(SecAtual.Estaca) != SecAtual.Terreno.PegaPontoAtual().x)
          {
            Ponto PontoAtual(SecAtual.Terreno.PegaPontoAtual());

            SecAtual.PontosRemovidosDesenho.AddTail(PontoAtual);

            PropagaRemoçãoPerfGeologico(SecAtual.Terreno.PegaPontoAtual().x);
            SecAtual.Terreno.ExcluiPonto();

            AlterouSecao = InseriuPontos = true;

            //--- Se foi o gradiente Reinsere-o

            if (Gradientes.BuscaGradiente(SecAtual.Estaca) == PontoAtual.x)
            {
              PontoAtual = SecAtual.Terreno.Interpola(PontoAtual.x, CString("GRADIE"));
              AfxMessageBox("O gradiente foi removido e reinserido no terreno", IDOK);
              SecAtual.Terreno.Estaca.Cota = PontoAtual.y;

              if (((CMainFrame*)AfxGetMainWnd())->GreideAtivo) ((CWnd*)(((CMainFrame*)AfxGetMainWnd())->DesPerfil))->RedrawWindow();    //--- O ponto de terreno do gradiente foi alterado, mostra a alteração no perfil.
            }

            if (Tipo == PERFILTALVEGUE) AlterouPerfilTalvegue = true;
          }
          else
          {
            monolog.mensagem(11, -1);   //--- O gradiente não pode ser deletado!!!  Agora pode 11052020
            return;
          }
        }
        else
        {
          if (Tipo == PROJETO)
          {
            if (SecAtual.PodeAlterar(SecAtual.Projeto.GetAt(PAtual).x)) SecAtual.Projeto.ExcluiPonto();
          }
          else
          {
            if (Tipo == PERFILGEOLOGICO)
            {
              const_cast<CDadosPerfisGeologicos*>(&*itPerfGeo)->RemovePonto(PAtual);
            }
            else
            {
              if (Tipo == PERFILGEOLOGICOOFFSETS)
              {
                const_cast<CDadosPerfisGeologicos*>(&*itPerfGeo)->RemovePontoOffSet(PAtual);
              }
            }
          }
        }

        AlterouSecao = AlterouSecoes = Tipo == TERRENO || Tipo == PROJETO || Tipo == PERFILGEOLOGICO || Tipo == PERFILGEOLOGICOOFFSETS;

        RedrawWindow();   //--- Redesenha a secao, sem o ponto
      }
    }
    break;
    case VK_ESCAPE:
    {
      xParticaoAtual = INFINITO;
      Particionando = false;

      ((CMainFrame*)AfxGetMainWnd())->mpCToolBarTalvegues->GetToolBarCtrl().SetState(ID_BUTPARTICIONAR, false);

      if (ArrastandoPonto)
      {
        if (Tipo == PROJETO) SecAtual.Projeto.AlteraPonto(PontoAnterior, PAtual);
        else
        {
          RestauraPontosPerfGeo(PontoAnterior, SecAtual.Terreno.GetAt(PAtual));
          AlteraLimpeza(SecAtual, PontoAnterior, PAtual);
          SecAtual.Terreno.AlteraPonto(PontoAnterior, PAtual);
        }

        ArrastandoPonto = Arrastando = false;
      }
      RedrawWindow();
    }
    break;
    case VK_F5: OnCalcoffsets(); break;
  }
}

void DesSecaoView::AplicaSuperElevacao(Perfil& SecaoAtual,double Gradiente)   //--- Está igual ao calculanota, UNIFICAR
{
  int LadoCurva(NENHUM);
  Perfil SecaoRasc = SecaoAtual;
  double SE(CurvasHorizontais.BuscaSuperElevacao(SecaoRasc.Estaca,LadoCurva));
  if(SE == INFINITO) return;  //--- busca a super-elevaçao, se na tangente não faz nada
  if( LadoCurva == DIREITO) SE *= -1.0;
  POSITION PGradiente(SecaoRasc.GetHeadPosition());  //--- Inicializa o atual

  //--- Marca o gradiente.

  //--- O gradiente não pode ser aplicado fora do eixo de giro da superelevação porque altera o greide sensivelmente
  //--- Por enquando fica forçado em (0,0)
  //--- Reanalisar 17/06/2016
  //--- Todos fazem assim 07/12/2018

  while(PGradiente && SecaoRasc.GetAt(PGradiente).x < Gradiente)  SecaoRasc.GetNext(PGradiente);

  Ponto PonGradiente = SecaoRasc.GetAt(PGradiente);

  //--- O cálculo é feito por lado a partir do gradiente.

  for (int LadoPista = ESQUERDO ; LadoPista <= DIREITO ; LadoPista++)
  {
    POSITION Anterior(PGradiente),Ultimo = LadoPista == ESQUERDO ? SecaoRasc.GetHeadPosition() : SecaoRasc.GetTailPosition();
    double DeltaYanterior(0.0);

    while(Anterior != Ultimo)
    {
      Ponto& PAnterior(LadoPista == ESQUERDO ? SecaoRasc.GetPrev(Anterior) : SecaoRasc.GetNext(Anterior));
      Ponto& PAtual(SecaoRasc.GetAt(Anterior)); 

      double DeltaX(PAtual.x - PAnterior .x),DeltaY(PAnterior.y - PAtual.y),i(LadoPista == ESQUERDO ? PAtual.i : PAnterior.i);

      //--- Verifica a necessidade de se aplicar a super elevação à pista.
      //--- Caso positivo aplica-a.
      //--- Se for true tem q aplicar a SE .... Exceto se for pista fixa ou canteiro
      if (LadoCurva != LadoPista || fabs(SE) >= fabs(i))           //--- Teve que incluir o = na comparação do SE para entrar no if certo no caso de canteiro central
      {
        int Ini_Fim_Fixas(-1),Ini_Fim_Canteiros(-5); 

        //--- Verifica se é canteiro ou pista fixa

        bool EFixa(PistasFixas.VerfPistaFixas(SecaoAtual.Estaca,PAtual.x,Ini_Fim_Fixas)),
          ECant(CanteirosCentrais.ECanteiro(SecaoAtual.Estaca,PAtual.x,Ini_Fim_Fixas));

        if(!EFixa && !ECant)         //--- Se não for pista fixa ou canteiro (para efeito de SE os 2 são iguais) , aplica a SE      
        {
          Ponto& PontoOriginal = SecaoAtual.GetAt(SecaoAtual.PosicionaPontoX(PAtual.x));
          PontoOriginal.y  = DeltaYanterior = SE * DeltaX + DeltaYanterior;
          PontoOriginal.i = SE;
        }
        else
        {
          //--- É pista fixa:
          //--- Se for pista fixa e o lado for esquerdo e não for fim de pista fixa ou o lado for direito e não for inicio de pista fixa ou
          //--- Se for canteiro e o lado for esquerdo e não for início de pista fixa ou o lado for direito e não for fim de pista fixa ou
          //--- aplica a SE

          Ponto& PontoOriginal(SecaoAtual.GetAt(SecaoAtual.PosicionaPontoX(PAtual.x)));

          if(!ECant || (EFixa && (LadoPista == ESQUERDO && Ini_Fim_Fixas == CPistasFixas::FIM) || (LadoPista == DIREITO && Ini_Fim_Fixas == CPistasFixas::INICIO))) 
          {
            PontoOriginal.y = DeltaYanterior = SE * DeltaX + DeltaYanterior;
            PontoOriginal.i = SE;
          }
          else      //--- Senão é ponto de pista fixa ou canteiro: Não aplica a SE, Mantem o delta y atual
          {
            DeltaYanterior += PontoOriginal.y - DeltaYanterior;
          }
        }
      }
      else 
      {
        if (LadoCurva == ESQUERDO) DeltaY *= -1.0;
        DeltaYanterior += i > 0.0 ? DeltaY : -DeltaY;
        Ponto& PontoOriginal = SecaoAtual.GetAt(SecaoAtual.PosicionaPontoX(PAtual.x));
        PontoOriginal.y = DeltaYanterior;
      }
    }
  }
}

void DesSecaoView::AplicaSuperLargura(Perfil& ProjetoAtual)
{
  double SL(CurvasHorizontais.BuscaSuperLargura(ProjetoAtual.Estaca));
  if(SL == 0.0) return;  //--- busca a superlargura, se na tangente não faz nada

  ProjetoAtual.CalculaRampa(ProjetoAtual.GetHeadPosition());
  ProjetoAtual.CalculaRampa(ProjetoAtual.GetTailPosition());

  Ponto& BordoEsquerdo = ProjetoAtual.GetHead(),&BordoDireito = ProjetoAtual.GetTail();

  BordoEsquerdo.y -= SL * BordoEsquerdo.i;
  BordoEsquerdo.x -= SL;

  BordoDireito.y += SL * BordoDireito.i;
  BordoDireito.x += SL;
}

void DesSecaoView::AlteraLimpeza(CSecao& SecAtual,Ponto NovoPonto,POSITION PTerrAtual)
{
  Ponto PTerr(SecAtual.Terreno.GetAt(PTerrAtual));

  SecAtual.Limpeza.PosicionaNoMaisProximo(PTerr.x);

  if(SecAtual.Limpeza.PegaAtual() != NULL)
  {
    Ponto PLImpezaAtual(SecAtual.Limpeza.PegaPontoAtual());

    if(fabs(PLImpezaAtual.x - PTerr.x) < 0.001)
    {
      NovoPonto.y -= AltLimpeza.BuscaH(SecAtual.Projeto.Estaca);
      SecAtual.Limpeza.AlteraPonto(NovoPonto);
    }
    else
    {
      POSITION PProx(SecAtual.Limpeza.PegaAtual());
      SecAtual.Limpeza.GetNext(PProx);

      if(SecAtual.Limpeza.PegaAtual())
      {
        PLImpezaAtual = SecAtual.Limpeza.GetAt(PProx);

        if(fabs(PLImpezaAtual.x - PTerr.x) < 0.001)
        { 
          NovoPonto.y -= AltLimpeza.BuscaH(SecAtual.Projeto.Estaca);
          SecAtual.Limpeza.AlteraPonto(NovoPonto);
        }
      }  
    }
  }
}

//--- Estas duas mensagens não estão sendo chamadas, verificar

void DesSecaoView::OnBnClickedRaterreno()
{
  TerrenoProjeto = MouseXY->m_RATerreno.GetCheck();
}

void DesSecaoView::OnBnClickedRaprojeto()
{
  TerrenoProjeto = !MouseXY->m_RAProjeto.GetCheck();
}

BOOL DesSecaoView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
  ((CChildFrame*) GetParentFrame())->Deslizantes.MouseWheelOn(nFlags, zDelta, pt);

  return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void DesSecaoView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
  pDC->SetBkColor(Cores[CORFUNDO]);
  CView::OnPrepareDC(pDC, pInfo);
}

void DesSecaoView::OnClose()
{
  CView::OnClose();
}

LRESULT DesSecaoView::FechouToolBar(WPARAM wParam, LPARAM lParam)
{
  switch(lParam)
  {
  case IDR_DESSECAO:  Coordenadas = (wParam > 0) ; break;
  case IDR_DESSECAO2: Escalas = (wParam > 0); break;
  case IDR_DESSECAO3: Procurar = (wParam > 0); break;
  case IDR_DESSECAO4: Ferramentas = (wParam > 0) ; break;
  }
  return 0;
}

void DesSecaoView::OnShowWindow(BOOL bShow, UINT nStatus)
{
  CView::OnShowWindow(bShow, nStatus);

  if(((CMainFrame*) AfxGetMainWnd())->GreideAlterado == true)
    SetWindowText(_T("Desenho das Seções - Seções Inválidas - O Greide Foi Alterado "));
}

void DesSecaoView::RemovePontosEditadosProjeto(CSecao &SecAtual,const Perfil& ProjetoEditado)           //--- A seção esta sendo remontada, os pontos movidos estão no lugar de origem, são removidos aqui
{
  if(SecAtual.SecaoTipo.GetCount() > 2)
  {
    POSITION Atual(SecAtual.Projeto.GetHeadPosition());

    while(Atual != NULL)
    {
      Ponto PontoAtual(SecAtual.Projeto.GetAt(Atual));

      if(PontoAtual.Descricao.Find("PONINS") == -1 && (SecAtual.SecaoTipo.GetHead().x > PontoAtual.x || SecAtual.SecaoTipo.GetTail().x < PontoAtual.x)) //-- Não pode estar na secao tipo
      {
        if(ProjetoEditado.Find(PontoAtual) == NULL) 
        {
          if(PontoAtual.Descricao.Find("BORTE") == -1)   //--- bordos de terraplanagem qdo cv < hpavim  (estão fora da seção tipo mas não podem ser excluidos) 
          {
            if(PontoAtual.Descricao.Find("ALARG") == -1) //--- Idem
            {
              POSITION PARemover(Atual);
              SecAtual.Projeto.GetNext(Atual);
              SecAtual.Projeto.RemoveAt(PARemover);
            }
            else  SecAtual.Projeto.GetNext(Atual);
          }
          else  SecAtual.Projeto.GetNext(Atual);
        }
        else  SecAtual.Projeto.GetNext(Atual);
      }
      else SecAtual.Projeto.GetNext(Atual);
    }
  }
}

void DesSecaoView::RestauraPontosPerfGeo(const Ponto& PontoAnterior,const Ponto& PontoAtual)
{
  const CSecao& SecAtual(Secoes.GetAt(SecaoAtual));

  itsetCDadosPerfGeologicos itPerfGeo(PerfilGeologico.PegaSetSecaoPerfGeo().upper_bound(CDadosPerfisGeologicos("",SecAtual.Estaca.EstVirtual)));

  if(itPerfGeo != PerfilGeologico.PegaSetSecaoPerfGeo().end())
  {
    while(itPerfGeo->PegaEstaca() == SecAtual.Estaca.EstVirtual)
    {
      PerfilGeologico.AlteraPontoPerfis(PontoAtual,PontoAnterior.x,PontoAnterior.y);

      ++itPerfGeo;
    }
  }
}

void DesSecaoView::PropagaRemoçãoPerfGeologico(double x)
{
  const CSecao& SecAtual(Secoes.GetAt(SecaoAtual));

  itsetCDadosPerfGeologicos itPerfGeo(PerfilGeologico.PegaSetSecaoPerfGeo().upper_bound(CDadosPerfisGeologicos("",SecAtual.Estaca.EstVirtual)));

  if(itPerfGeo != PerfilGeologico.PegaSetSecaoPerfGeo().end())
  {
    while(itPerfGeo->PegaEstaca() == SecAtual.Estaca.EstVirtual)
    {
      const_cast<CDadosPerfisGeologicos*>(&*itPerfGeo)->RemovePontoPerfil(x);

      ++itPerfGeo;
    }
  }
}

void DesSecaoView::PropagaInsercaoPerfGeologico(Ponto& PontoAtual)
{
  const CSecao& SecAtual(Secoes.GetAt(SecaoAtual));

  itsetCDadosPerfGeologicos itPerfGeo(PerfilGeologico.PegaSetSecaoPerfGeo().upper_bound(CDadosPerfisGeologicos("",SecAtual.Estaca.EstVirtual)));

  while(itPerfGeo != PerfilGeologico.PegaSetSecaoPerfGeo().end() &&  itPerfGeo->PegaEstaca() == SecAtual.Estaca.EstVirtual)
  {
    const_cast<CDadosPerfisGeologicos*>(&*itPerfGeo)->InserePonto(Ponto(PontoAtual.x,PontoAtual.y+itPerfGeo->PegaDeltahPrimitivo()));
    ++itPerfGeo;
  }
}

void DesSecaoView::OnPropagar() 
{
  DesenharPerfisGeologicos = DesenharPerfisGeologicos == false;

  RedrawWindow();
}

void DesSecaoView::OnUpdatePropagar(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!DesenharSecAvulsas);
}

void DesSecaoView::OnSetFocus(CWnd* pOldWnd)
{
  CView::OnSetFocus(pOldWnd);

  CMainFrame* MF(((CMainFrame*) AfxGetMainWnd()));

  if(MF->SelecionaRua(Trecho) == CB_ERR) 
  {
   std::string Projeto(NomeProjeto.GetBuffer()); 
  
    std::string NomeProjetoBase(Projeto.substr(0,Projeto.find_last_of('\\')));
    NomeProjetoBase = NomeProjetoBase.substr(0,NomeProjetoBase.find_last_of('\\'));

    int Sel(MF->m_ProjetosBox.FindString(0,NomeProjetoBase.c_str()));

    if (Sel == LB_ERR) 
    {
      MF->m_ProjetosBox.InsertString(0,NomeProjetoBase.c_str());
      MF->m_ProjetosBox.SetCurSel(0);

    }
    else MF->m_ProjetosBox.SetCurSel(Sel);

    MF->m_sNomeComp = NomeProjetoBase.c_str();

    ((CEstApp*) AfxGetApp())->MudouProjetoSelecionado(NomeProjetoBase);
  }
  /*
  MF->ShowControlBar(MF->PegaBarra(1),true,false);
  MF->ShowControlBar(MF->PegaBarra(2),true,false);
  MF->ShowControlBar(MF->PegaBarra(3),true,false);
  MF->ShowControlBar(MF->PegaBarra(4),true,false);
  */
  /*
  MF->PegaBarra(1)->SetWindowPos(&CWnd::wndTopMost ,0,0,0,0,SWP_NOMOVE  | SWP_NOSIZE | SWP_SHOWWINDOW);
  MF->PegaBarra(2)->SetWindowPos(&CWnd::wndTopMost ,0,0,0,0,SWP_NOMOVE  | SWP_NOSIZE | SWP_SHOWWINDOW);
  MF->PegaBarra(3)->SetWindowPos(&CWnd::wndTopMost ,0,0,0,0,SWP_NOMOVE  | SWP_NOSIZE | SWP_SHOWWINDOW);
  MF->PegaBarra(4)->SetWindowPos(&CWnd::wndTopMost ,0,0,0,0,SWP_NOMOVE  | SWP_NOSIZE | SWP_SHOWWINDOW);
  */

/*
  MF->PegaBarra(1)->GetParentFrame()->BringWindowToTop();
  MF->PegaBarra(2)->GetParentFrame()->BringWindowToTop();
  MF->PegaBarra(3)->GetParentFrame()->BringWindowToTop();
  MF->PegaBarra(4)->GetParentFrame()->BringWindowToTop();
  */
  }

void DesSecaoView::DesenhaCerca(const Ponto& PInicial,const Ponto& PFinal,CDC* ppDC)
{
  //--- Determina pontos do X 
  //--- Rotaciona x
  //--- DesenhaLinha
  //--- Desenha o x
  //--- Repete atá a altura chagar a 0.75m

  Ponto PPixelInicial(PInicial),
        Pontosx[5] = {Ponto(-2.0,4.0),Ponto(2.0,4.0),Ponto(-2.0,0.0),Ponto(2.0,0.0),Ponto(0.0,4.0)},       //--- Coordenadas do
        PontoAnterior(PInicial),PontoAtual,   
        DeltasAci(0.0,-.75 * Escala[Y]);
  
  bool Acabou(false);
  CPen PenaRestricao;
  int TipoRestricao(CAciTopografico::CERCA);

  CriaPenaRestriçao(&PenaRestricao,TipoRestricao);
  ppDC->SelectObject(PenaRestricao); 

  do
  {
    PontoAtual.x =  PontoAnterior.x + DeltasAci.x;
    PontoAtual.y =  PontoAnterior.y + DeltasAci.y;

    //--- Acabou??? acerta o final

    if((PontoAtual.x > PFinal.x  || PontoAtual.y < PFinal.y))
    {
      PontoAtual.x = PFinal.x;
      PontoAtual.y = PFinal.y;
      Acabou = true;
    }

    ppDC->MoveTo(PontoAnterior.x,PontoAnterior.y);
    ppDC->LineTo(PontoAtual.x,PontoAtual.y);

    PontoAnterior.x = PontoAtual.x;// + Pontosx[4].x;
    PontoAnterior.y = PontoAtual.y;// + Pontosx[4].y;

    //--- Desenha o x

    if (!Acabou)
    {
      ppDC->MoveTo(PontoAtual.x + Pontosx[0].x, PontoAtual.y + Pontosx[0].y);
      ppDC->LineTo(PontoAtual.x + Pontosx[3].x, PontoAtual.y + Pontosx[3].y);
      ppDC->MoveTo(PontoAtual.x + Pontosx[1].x, PontoAtual.y + Pontosx[1].y);
      ppDC->LineTo(PontoAtual.x + Pontosx[2].x, PontoAtual.y + Pontosx[2].y);
    }
  }while (!Acabou);

  ppDC->Arc(PInicial.x - 3, PInicial.y - 3, PInicial.x + 3, PInicial.y + 3, 0, 0, 0, 0);
}

void DesSecaoView::CriaPenaRestriçao(CPen *PenaObjeto, unsigned int Objeto)
{
  PenaObjeto->DeleteObject();

  switch (Objeto)
  {
  case CAciTopografico::BORDO:  PenaObjeto->CreatePen(PS_DASH, 1, RGB(189, 141, 0)); break;
  case CAciTopografico::N_A: PenaObjeto->CreatePen(PS_SOLID, 1, RGB(128, 128, 255)); break;
  case CAciTopografico::N_A_PER: PenaObjeto->CreatePen(PS_DOT, 1, RGB(128, 128, 255)); break;
  case CAciTopografico::ROCHA: PenaObjeto->CreatePen(PS_DASHDOTDOT, 1, RGB(160, 160, 160)); break;
  case CAciTopografico::CANTO_CERCA:
  case CAciTopografico::CERCA: PenaObjeto->CreatePen(PS_SOLID, 1, RGB(127, 127, 127)); break;
  case CAciTopografico::CANTO_MURO:
  case CAciTopografico::MURO:  PenaObjeto->CreatePen(PS_SOLID, 1, RGB(127, 127, 127)); break;
  case CAciTopografico::CANTO_CASA:
  case CAciTopografico::CASA:  PenaObjeto->CreatePen(PS_SOLID, 2, RGB(192, 192, 192)); break;
  case CAciTopografico::MEIO_FIO: PenaObjeto->CreatePen(PS_DASH, 1, RGB(160, 160, 160)); break;
  case CAciTopografico::PONTE: PenaObjeto->CreatePen(PS_SOLID, 2, RGB(0, 0, 192)); break;
  case CAciTopografico::TAMPA_BUEIRO: PenaObjeto->CreatePen(PS_SOLID, 1, RGB(192, 192, 192)); break;
  case CAciTopografico::POSTE: PenaObjeto->CreatePen(PS_DASHDOTDOT, 1, RGB(192, 0, 0)); break;
  case CAciTopografico::REDE_ELETRICA_BT: PenaObjeto->CreatePen(PS_DASHDOTDOT, 1, RGB(192, 0, 0)); break;
  case CAciTopografico::REDE_ELETRICA_AT: PenaObjeto->CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0)); break;
  case CAciTopografico::FRONTEIRA: PenaObjeto->CreatePen(PS_SOLID, 2, RGB(64, 64, 192)); break;
  case CAciTopografico::EIXO:  PenaObjeto->CreatePen(PS_DASHDOT, 1, RGB(223, 223, 223)); break;
  case CAciTopografico::CRISTA: PenaObjeto->CreatePen(PS_SOLID, 1, RGB(189, 157, 126)); break;
  case CAciTopografico::PE: PenaObjeto->CreatePen(PS_DOT, 1, RGB(189, 157, 126)); break;
  case CAciTopografico::BUEIRO: PenaObjeto->CreatePen(PS_DASHDOT, 1, RGB(0, 0, 223)); break;
  case CAciTopografico::LINHA_FERREA: PenaObjeto->CreatePen(PS_SOLID, 1, RGB(127, 127, 127)); break;
  case CAciTopografico::ALAGADO:  PenaObjeto->CreatePen(PS_DASHDOTDOT, 1, RGB(128, 128, 255)); break;
  case CAciTopografico::ALINHAMENTO: PenaObjeto->CreatePen(PS_DASHDOT, 1, RGB(0, 0, 255)); break;
  case CAciTopografico::ARVORE: PenaObjeto->CreatePen(PS_SOLID, 1, RGB(0, 127, 0)); break;
  case CAciTopografico::FLORESTA:PenaObjeto->CreatePen(PS_SOLID, 1, RGB(0, 127, 0)); break;
  case CAciTopografico::MATA:PenaObjeto->CreatePen(PS_DASHDOT, 1, RGB(0, 127, 0)); break;
  case CAciTopografico::PLANTACAO:PenaObjeto->CreatePen(PS_DASHDOTDOT, 1, RGB(0, 127, 0)); break;
  case CAciTopografico::EQUI_BUEIRO:
  case CAciTopografico::EQUI_BUEIRO_RETANG: PenaObjeto->CreatePen(PS_DASHDOTDOT, 1, RGB(200, 20, 20)); break;
  default: PenaObjeto->CreatePen(PS_DASHDOT, 1, RGB(255, 255, 255)); break;
  }
}

void DesSecaoView::OnButacitopog()
{
  DesenharAciTopog = DesenharAciTopog == 0;
  RedrawWindow();
}

void DesSecaoView::OnButSecAvulsas()
{
  DesenharSecAvulsas = DesenharSecAvulsas == 0;

  CMainFrame* MF((CMainFrame*) AfxGetMainWnd());

  MF->ShowControlBar(MF->PegaBarra(BARRA_PARTICIONAR), false, false);

  if (DesenharSecAvulsas)
  {
    MF->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTTALVEGUES, false);
    MF->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTHIPSOGRAMA, false);
    MF->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTSECAVULSASECOES, true);

    DesenharHistogramas = DesenharTalvegues = false;

    PreencheComboProcurarSecAvulsas(GetParentFrame());

    if (SecoesAvulsas.PegaLSecoesAvulsas().GetCount())
    {
      if (SecaoAvulsaAtual.Terreno.GetSize() < 2)
      {
        SecaoAvulsaAtual.Terreno = SecoesAvulsas.PegaLSecoesAvulsas().GetHead();
        SecaoAvulsaAtual.AcidentesTopograficos = SecoesAvulsas.PegaLSecoesAvulsas().GetHead().AcidentesTopograficos;
        SecaoAvulsaAtual.AlinhamentosTerrap = SecoesAvulsas.PegaLSecoesAvulsas().GetHead().AlinhamentosTerrap;
        SecaoAvulsaAtual.Projeto = SecoesAvulsas.PegaLSecoesAvulsas().GetHead().LstPontosProjeto;
        UltimaSecaoAvulsa = SecoesAvulsas.PegaLSecoesAvulsas().GetHead().Nome.c_str();
      }

      ((CComboProcurarDesSecao*)pComboFind)->PosicionaTexto(UltimaSecaoAvulsa);

      DVolumes.Atualiza(SecaoAvulsaAtual, true);

      RedrawWindow();
    }
  }
  else
  {
    CChildFrame* pChildFrame((CChildFrame*)GetParentFrame());

    ((CComboProcurarDesSecao*)pComboFind)->GetLBText(((CComboProcurarDesSecao*)pComboFind)->GetCurSel(), UltimaSecaoAvulsa);

    PreencheComboProcurar(pChildFrame);

    ((CComboProcurarDesSecao*)pComboFind)->Posiciona(Secoes.GetAt(SecaoAtual).Estaca);

    RedrawWindow();
  }
}

void DesSecaoView::OnButTalvegues()
{
  CMainFrame* MF((CMainFrame*)AfxGetMainWnd());

  DesenharTalvegues = DesenharTalvegues == 0;

  if (DesenharTalvegues)
  {
    Tipo = PERFILTALVEGUE;

    MF->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTHIPSOGRAMA,false);
    MF->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTSECAVULSA, false);
    MF->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTTALVEGUES, true);

    MF->ShowControlBar(MF->PegaBarra(BARRA_PARTICIONAR), true, false);

    DesenharSecAvulsas = DesenharHistogramas = false;

    PreencheComboProcurarTalvegues(GetParentFrame());

    if (BaciasHidrograficas.size())
    {
      int CurSel(0);

      if(UltimoTalvegue.GetLength()) ((CComboProcurarDesSecao*)pComboFind)->PosicionaTexto(UltimoTalvegue);

      (((CComboProcurarDesSecao*)pComboFind)->GetLBText(CurSel,UltimoTalvegue));

      DVolumes.Atualiza(TalvegueAtual, true);

      std::string UltTalvegue(UltimoTalvegue.GetBuffer());

      Posiciona(INFINITO, INFINITO, true, Tipo,&UltTalvegue);

      EquiMalha = EquiMalhaTalv;
      EquiCotas = EquiCotasTalv;

      Escala[X] = EscalaTalv[X];
      Escala[Y] = EscalaTalv[Y];

      ((CChildFrame*)GetParentFrame())->Deslizantes.MudarEscala(Escala[X] * 1000.0);
      ((CChildFrame*)GetParentFrame())->Deslizantes.MudarRazao(RazaoHV);
      CalculaParametros();

      RedrawWindow();
    }
  }
  else
  {
    EscalaTalv[X] = Escala[X];
    EscalaTalv[Y] = Escala[Y];

    EquiMalhaTalv = EquiMalha;
    EquiCotasTalv = EquiCotas;

    MF->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTTALVEGUES, false);

    if(AlterouPerfilTalvegue)
    {
      //--- Salva o perfil do talvegue alterado. 

      SalvaPerfilTalvegue();
      AlterouPerfilTalvegue = false;
    }
   
    CChildFrame* pChildFrame((CChildFrame*)GetParentFrame());

    PreencheComboProcurar(pChildFrame);

    MF->ShowControlBar(MF->PegaBarra(7), false, false);
  }

  RedrawWindow();
}

void DesSecaoView::DesenhaLinhaFerrea(const Ponto& PInicial, const Ponto & PFinal, CDC* ppDC)
{
  Ponto Pontosx[5] = { Ponto(-1.0,0.0),Ponto(1.0,0.0),Ponto(-6.0,6.0),Ponto(7.0,6.0),Ponto(0.0,9.0) },
    PontoAtual(PInicial);

  //--- Desenha as 2 linhas

  int Quadrante(0);

  for (register int C = 0; C < 2; C++)
  {
    ppDC->MoveTo(PInicial.x + Pontosx[C].x, PInicial.y + Pontosx[C].y);
    ppDC->LineTo(PFinal.x + Pontosx[C].x, PFinal.y + Pontosx[C].y);
  }

  //--- Desenha os traços

  do
  {
    ppDC->MoveTo(PontoAtual.x + Pontosx[2].x, PontoAtual.y - Pontosx[2].y);
    ppDC->LineTo(PontoAtual.x + Pontosx[3].x, PontoAtual.y - Pontosx[3].y);

    PontoAtual.x += Pontosx[4].x;
    PontoAtual.y -= Pontosx[4].y;

  } while (PontoAtual.y > PFinal.y);

  ppDC->Arc(PInicial.x - 3, PInicial.y - 3, PInicial.x + 3, PInicial.y + 3, 0, 0, 0, 0);
}

void DesSecaoView::DesenhaMuro(const Ponto& PInicial, const Ponto & PFinal, CDC* ppDC)
{
  ppDC->MoveTo(PInicial.x - 1, PInicial.y);
  ppDC->LineTo(PInicial.x - 1, PFinal.y);
  ppDC->LineTo(PInicial.x + 1, PFinal.y);
  ppDC->LineTo(PInicial.x + 1, PInicial.y);

  ppDC->Arc(PInicial.x - 3, PInicial.y - 3, PInicial.x + 3, PInicial.y + 3, 0, 0, 0, 0);
}

bool DesSecaoView::IncluiPontosInsRemNaSuperficie(int tipo)
{
  typlstPontos ListaPontosInseridos,ListaPontosRemovidos;

  for (POSITION Atual = Secoes.GetHeadPosition(); Atual != NULL; Secoes.GetNext(Atual))
  {
    CSecao& SecaoAtual = Secoes.GetAt(Atual);

    if (SecaoAtual.Estaca.EstVirtual != INFINITO)
    {
      Ponto CoordSecao, PontoaCalcular;

      CurvasHorizontais.PegaDadosGeometricos(CoordSecao, SecaoAtual.Estaca);

      for (POSITION AtualTerr = SecaoAtual.Terreno.GetHeadPosition(); AtualTerr != NULL; SecaoAtual.Terreno.GetNext(AtualTerr))
      {
        Ponto PontoTerrAtual(SecaoAtual.Terreno.GetAt(AtualTerr));

        if (PontoTerrAtual.Inserido == true)
        {
          Mat.IrradiaPonto(CoordSecao.fi, CoordSecao.x, CoordSecao.y, Matem::M_PI_2, PontoTerrAtual.x, PontoaCalcular);
          PontoaCalcular.z = PontoTerrAtual.y;
          PontoaCalcular.Descricao = PontoTerrAtual.Descricao;
          ListaPontosInseridos.push_back(PontoaCalcular);
        }
      }

      if (SecaoAtual.PontosRemovidosDesenho.GetCount() > 0)
      {
        for(POSITION AtualRem = SecaoAtual.PontosRemovidosDesenho.GetHeadPosition() ; AtualRem ; SecaoAtual.PontosRemovidosDesenho.GetNext(AtualRem))
        {
          Ponto PontoTerrAtualRem(SecaoAtual.PontosRemovidosDesenho.GetAt(AtualRem));

          Mat.IrradiaPonto(CoordSecao.fi, CoordSecao.x, CoordSecao.y, Matem::M_PI_2, PontoTerrAtualRem.x, PontoaCalcular);
          PontoaCalcular.z = PontoTerrAtualRem.y;
          PontoaCalcular.Descricao = PontoTerrAtualRem.Descricao;
          ListaPontosRemovidos.push_back(PontoaCalcular);
        }
      }
    }
  }

  std::string NomeProjetoBase = ((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase();
  NomeProjetoBase += "_PonInsSec_";
  NomeProjetoBase += ((CMainFrame*)AfxGetMainWnd())->PegaRuaAtual();
  NomeProjetoBase +=  Tipo == 0 ? ".rel" : ".xyz";

  std::ofstream ArqPontosInsRemovidos(NomeProjetoBase, std::fstream::out | std::fstream::app);
  if (!ArqPontosInsRemovidos.is_open())
  {
    std::string strMsg(std::string("Erro ao abrir o arquivo de saida:"));
    strMsg += NomeProjeto.GetBuffer();

    AfxMessageBox(strMsg.c_str(),MB_ICONERROR);

    return false;
  }

  ArqPontosInsRemovidos << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

  unsigned int Contador(1);
  CTime Agora(CTime::GetCurrentTime());
  std::stringstream strstreamTemp;
  strstreamTemp << std::right << std::fixed << std::setfill('0') << std::setprecision(2);
  unsigned int UltimaEdicao(atoi(PegaUltimaEdicao(NomeProjetoBase).c_str()));
  UltimaEdicao++;
  if(UltimaEdicao == 1)
  {
    ArqPontosInsRemovidos << "//--- Este arquivo contem os pontos inseridos/removidos na edição das seções." << std::endl
                          << "//--- ===> NÃO EDITAR <=== A edição deste arquivo provocará erros irrecuperáveis no projeto."  << std::endl;
  }
  ArqPontosInsRemovidos <<   "//--- " << Agora.GetDay() << '/' << Agora.GetMonth() << '/' << Agora.GetYear() << ' '
                        <<   "//--- " << Agora.GetHour() << ':' << Agora.GetMinute() << ':' << Agora.GetSecond() << ' ' << std::endl
						<<   "//--- " << "Edição " 
                        << (UltimaEdicao < 10 ? "000" : UltimaEdicao < 100 ? "00" : UltimaEdicao < 1000 ? "0" : "")
                        << UltimaEdicao << std::endl;

  for (ittyplstPontos it = ListaPontosInseridos.begin() ; it != ListaPontosInseridos.end() ; it++)
  {
    ArqPontosInsRemovidos << Contador++ << ' ' << it->y << ' ' << it->x << ' ' << it->z << ' ' << it->Descricao << std::endl;
  }

  ArqPontosInsRemovidos << "//--- Pontos Removidos:" << std::endl;

  Contador = 0;

  for (ittyplstPontos it = ListaPontosRemovidos.begin(); it != ListaPontosRemovidos.end(); it++)
  {
    ArqPontosInsRemovidos << Contador++ << ' ' << it->y << ' ' << it->x << ' ' << it->z << ' ' << it->Descricao << std::endl;
  }
  return true;
}

#include "io.h"
#include <sstream>
#include <iomanip>

std::string DesSecaoView::PegaUltimoArquivoSecoes(std::string& NomeAProcurar)   //--- não está sendo usado
{
  struct _finddata_t c_file;
  intptr_t hFile;
  int i(0);
  tysetNomeArquivos SetNomesArquivos;
  std::stringstream strNumProxArq;
  std::string Rasc(NomeAProcurar);

  std::string strNomeProj(NomeAProcurar.substr(0, NomeAProcurar.find_last_of('\\')+1));

  Rasc += '*';
        
  if ((hFile = _findfirst((char *)LPCTSTR(Rasc.c_str()), &c_file)) == -1L)
  {
    return "001";
  }
  else
  {
    do
    {
      SetNomesArquivos.insert(strNomeProj + c_file.name);

    } while (++i < 1000 && _findnext(hFile, &c_file) == 0);

    _findclose(hFile);

    std::string NumUltimoArq(*SetNomesArquivos.rbegin());
    NumUltimoArq = NumUltimoArq.substr(0, NumUltimoArq.find_last_of('.'));
    NumUltimoArq = NumUltimoArq.substr(NumUltimoArq.size() - 3, 3);

    int NumProxArq(atoi(NumUltimoArq.c_str()));
    NumProxArq++;

    strNumProxArq << std::right << std::fixed << std::setfill('0');
    strNumProxArq.width(3);

    strNumProxArq <<  NumProxArq;
  }
  
  return   strNumProxArq.str();
}

void  DesSecaoView::PreencheComboProcurar(void* pChildFrame)
{
  ((CChildFrame*)pChildFrame)->PegaComboProcurarDesSecao().ResetContent();

  //--- Preenche o box de procurar
  //--- É preciso limpar os espaços à esquerda, senão as funçoes do combo box ficam prejudicadas 

  POSITION PSecao(Secoes.GetHeadPosition());

  while (PSecao)
  {
    const CSecao& rSecAtual(Secoes.GetNext(PSecao));

    CEstaca* EstacaAtual(const_cast<CEstaca*> (&rSecAtual.Projeto.Estaca));

    if (EstacaAtual->Find("INF") != -1) EstacaAtual = (const_cast<CEstaca*> (&rSecAtual.Terreno.Estaca));

    if (EstacaAtual->Find("INF") == -1)
    {
      EstacaAtual->TrimLeft();
      ((CChildFrame*)pChildFrame)->PegaComboProcurarDesSecao().AddString(CString((*EstacaAtual)) + CString(" ") + EstacaAtual->Descricao);
    }
  }
}

void  DesSecaoView::PreencheComboProcurarSecAvulsas(void* pChildFrame)
{
  //--- Preenche o box de procurar com as seções avulsas

  POSITION PSecao(SecoesAvulsas.PegaLSecoesAvulsas().GetHeadPosition());

  ((CChildFrame*)pChildFrame)->PegaComboProcurarDesSecao().ResetContent();

  while (PSecao)
  {
    const CSecaoAvulsa& rSecAvAtual(SecoesAvulsas.PegaLSecoesAvulsas().GetNext(PSecao));
   
    ((CChildFrame*)pChildFrame)->PegaComboProcurarDesSecao().AddString(rSecAvAtual.Nome.c_str());
  }
}

void  DesSecaoView::PreencheComboProcurarTalvegues(void* pChildFrame)
{
  ((CChildFrame*)pChildFrame)->PegaComboProcurarDesSecao().ResetContent();

  //--- Preenche o box de procurar com as seções avulsas

  tySetString  SetNomes;

  for (ittylstBacias itBacia = BaciasHidrograficas.begin(); itBacia != BaciasHidrograficas.end(); ++itBacia)
  {
    SetNomes.insert(itBacia->PegaDadosBacia().NomeBacia.c_str());
  }

  for (const std::string& NomeAtual : SetNomes)
  {
    ((CChildFrame*)pChildFrame)->PegaComboProcurarDesSecao().AddString(NomeAtual.c_str());
  }
}

void DesSecaoView::LeBaciasHidrograficas(const std::string& NomeProjBase)
{
  BaciasHidrograficas.clear();

  CString Nom(NomeProjBase.c_str() + CString(".bah"));
  CFile Arq;
  CFileException e;

  if (Arq.Open(Nom, CFile::modeRead , &e) == TRUE)
  {
    size_t QuanBacias(0);
    CArchive CArcBacias(&Arq, CArchive::load);
    double AnguloDeRotacao;

    CArcBacias >> AnguloDeRotacao;

    for (CArcBacias >> QuanBacias ; QuanBacias > 0; --QuanBacias)
    {
      BaciasHidrograficas.push_back(CBacia());
      CBacia& BaciaAtual(*BaciasHidrograficas.rbegin());

      BaciaAtual << CArcBacias;
    }
  }
}

void DesSecaoView::GravaBaciasHidrograficas(const std::string& NomeProjBase,int Perguntar)
{
  if (!Perguntar || dialogar.DoModal(4, CString(" Bacias Hidrográficas - " + CString(Trecho.c_str()))) == IDOK)
  {
    CString Nom(NomeProjBase.c_str() + CString(".bah"));
    CFile Arq;
    CFileException e;

    if (Arq.Open(Nom, CFile::modeCreate | CFile::modeWrite, &e) == TRUE)
    {
      size_t QuanBacias(0);
      CArchive CArcBacias(&Arq, CArchive::store);
      double AnguloDeRotacao(0.0);

      CArcBacias << AnguloDeRotacao;

      CArcBacias << BaciasHidrograficas.size();

      for (ittylstBacias it = BaciasHidrograficas.begin() ; it != BaciasHidrograficas.end() ; ++it)
      {
        if(BaciasAlteradas.find(it) != BaciasAlteradas.end()) it->Calculada() = false;
        *it >> CArcBacias;
      }
    }
    AlterouBacias = false;
  }
}

void DesSecaoView::OnButParticionar()
{
  Particionando = !Particionando;

  CMainFrame* MF((CMainFrame*)AfxGetMainWnd());

  MF->mpCToolBarTalvegues->GetToolBarCtrl().SetState(ID_BUTPARTICIONAR, Particionando);

  xParticaoAtual = INFINITO;

  RedrawWindow();
}

void DesSecaoView::OnButHistograma()
{
  CMainFrame* MF((CMainFrame*)AfxGetMainWnd());

  DesenharHistogramas = DesenharHistogramas == 0;

  if(DesenharHistogramas)
  {
    MF->ShowControlBar(MF->PegaBarra(BARRA_PARTICIONAR), false, false);
    MF->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTTALVEGUES, false);
    MF->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTSECAVULSA, false);
    MF->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTHIPSOGRAMA, true);

    DesenharSecAvulsas = DesenharTalvegues = false;

    PreencheComboProcurarTalvegues(GetParentFrame());

    if (BaciasHidrograficas.size())
    {
      ((CComboProcurarDesSecao*)pComboFind)->PosicionaTexto(UltimoHipsograma);

      int CurSel(((CComboProcurarDesSecao*)pComboFind)->GetCurSel());

      ((CComboProcurarDesSecao*)pComboFind)->GetLBText(CurSel, UltimoHipsograma);

      std::string UltHistograma(UltimoHipsograma.GetBuffer());

      Posiciona(INFINITO, INFINITO, true, Tipo, &UltHistograma);

      RazaoHV = 10.0;
      Escala[X] = EscalaHipso[X];
      Escala[Y] = EscalaHipso[Y];

      EquiMalha = EquiMalhaHipso;
      EquiCotas = EquiCotasHipso;

      ((CChildFrame*)GetParentFrame())->Deslizantes.MudarEscala(Escala[X]);
      ((CChildFrame*)GetParentFrame())->Deslizantes.MudarRazao(RazaoHV);
      CalculaParametros();

      RedrawWindow();
    }
  }
  else
  {
    EscalaHipso[X] = Escala[X];
    EscalaHipso[Y] = Escala[Y];

    EquiMalhaHipso = EquiMalha;
    EquiCotasHipso = EquiCotas;

    MF->mpCDesTool1->GetToolBarCtrl().SetState(ID_BUTHIPSOGRAMA,false);

    CChildFrame* pChildFrame((CChildFrame*)GetParentFrame());

    ((CComboProcurarDesSecao*)pComboFind)->GetLBText(((CComboProcurarDesSecao*)pComboFind)->GetCurSel(), UltimoTalvegue);

    PreencheComboProcurar(pChildFrame);

    if (SecaoAtual) ((CComboProcurarDesSecao*)pComboFind)->Posiciona(Secoes.GetAt(SecaoAtual).Estaca);
    else AfxMessageBox("Não há seções transversais", MB_ICONASTERISK);

    MF->ShowControlBar(MF->PegaBarra(7), false, false);
  }

  RedrawWindow();
}

void DesSecaoView::OnUpdateButTalvegues(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(BaciasHidrograficas.size() > 0);
}

void DesSecaoView::OnButX10()
{
  EscalaX10 = !EscalaX10;
  double Fator(0.0);

  if(EscalaX10) 
    ((CChildFrame*)GetParentFrame())->Deslizantes.MudarRazao(Fator=0.1);
  else
    ((CChildFrame*)GetParentFrame())->Deslizantes.MudarRazao(Fator=1.0);
  
  RazaoHV = Fator;
  Escala[X] = Escala[Y] * Fator;

  CalculaParametros();

  RedrawWindow();
}

void DesSecaoView::SalvaPerfilTalvegue()
{
  ittylstBacias itBacia(BaciasHidrograficas.begin());

  for (; itBacia != BaciasHidrograficas.end(); ++itBacia)
  {
    if (itBacia->PegaDadosBacia().NomeBacia == UltimoTalvegue.GetBuffer()) break;
  }

  if (itBacia != BaciasHidrograficas.end())
  {
    setPontos& PerfilTalvegue(itBacia->PegaPerfTalvPrincipal());

    PerfilTalvegue.clear();

    for (POSITION Atual = TalvegueAtual.Terreno.GetHeadPosition(); Atual != NULL; TalvegueAtual.Terreno.GetNext(Atual))
    {
      PerfilTalvegue.emplace(TalvegueAtual.Terreno.GetAt(Atual));
    }
  }
}
std::string DesSecaoView::PegaUltimaEdicao(std::string& NomeArquivo)
{
  std::string strBuffer;
  std::stringstream Formatado;
  unsigned int ContaEdicao(0);
  std::ifstream ArqPontosInsRem(NomeArquivo, std::fstream::in);
  if (ArqPontosInsRem.is_open())
  {
    while(ArqPontosInsRem.good())
    {
      std::getline(ArqPontosInsRem, strBuffer);
      if (strBuffer.find("Edição") != std::string::npos)
      {
        ContaEdicao++;
      }
    }
  }
  Formatado << ContaEdicao;
  return Formatado.str();
}
void DesSecaoView::ConverteSecoesAvulsasSecoesTrans()  //--- AGUARDAR
{
  //--- Para possibilitar a edição das seções avulsas as seções avulsas são transformadas em seções transversais
  //--- porque apenas as seçoes transverais podem ser desenhadas e editadas 
  //--- Nesta conversão o perfil da seção avulsae passado para o perfil do terreno da seção transversal
  //--- Convertendo uma avulsa para uma seção  transversal (para possibilitar o desenho e a edição)
  //--- Esta rotina passa a seção avulsa para o terreno  da seção transversal), possibilitando 
  //--- o desenho e a edição
  /*
  SecoesAv.RemoveAll();  //--- Nesta lista ficam as secoes avulsas tranformadas em seções tranversais

  CList<CSecaoAvulsa, CSecaoAvulsa&>& LSecoesAvulsas(SecoesAv.PegaLSecoesAvulsas());

  for(POSITION PSecAv = LSecoesAvulsas.GetHeadPosition() ; PSecAv ; LSecoesAvulsas.GetNext(PSecAv))
  {
    auto& SecaoAvulsa(LSecoesAvulsas.GetAt(PSecAv));

    CSecao SecaoTransAtual;

    for (auto PosPontoSecAvulsaAtual = SecaoAvulsa.GetHeadPosition(); PosPontoSecAvulsaAtual; SecaoAvulsa.GetNext(PosPontoSecAvulsaAtual))
    {
      auto& PontoAtual(SecaoAvulsa.GetAt(PosPontoSecAvulsaAtual));

      SecaoTransAtual.Terreno.AddTail(PontoAtual);
    }
    SecaoTransAtual.Terreno.Estaca = SecaoAvulsa.Nome.c_str();

    SecoesAv.AddTail(SecaoTransAtual);
  }
  */
}

void DesSecaoView::ConverteSecoesTransSecoesAvulsas()  //--- AGUARDAR
{
  //--- Para possibilitar a edição das seções avulsasas seções avulsas são transformadas em seções transversais
  //--- porque apenas as seçoes transverais são desenhadas e editadas
  //--- Nesta conversão o perfilda seção avulsae passado para o perfil do terreno da seção transversal
  //--- Converte uma secao transversal em uma secao avulsa.
  //--- Esta Rotina volta a seção avulsa editada (que está no terreno da seção transversal) 
  //--- para a seção avulsa

  //--- Varre as seções avulsas que estão na lista de seçoes avulsas mas no formato de seções transversais
  //--- E as retrna para as lista de seções avulsas, apenas o terreno é alterado
  /*
  bool Achou(false);
  CList<CSecaoAvulsa, CSecaoAvulsa&>& LSecoesAvulsas(SecoesAv.PegaLSecoesAvulsas());

  for (auto PSecAvAtual = LSecoesAvulsas.GetHeadPosition(); ! Achou && PSecAvAtual; LSecoesAvulsas.GetNext(PSecAvAtual))
  {
    auto SecaoAvulsaAtual(LSecoesAvulsas.GetAt(PSecAvAtual));
    auto PosSecaoTransversal(SecoesAv.GetHeadPosition());

    auto& SecaoTransversaoAv(SecoesAv.GetAt(PosSecaoTransversal));

    std::string NomeSecAv(SecaoTransversaoAv.Terreno.Estaca.GetBuffer());
    std::string NomeSecaoAvulsa(SecaoAvulsaAtual.Nome);

    if(NomeSecaoAvulsa.compare(NomeSecAv) == 0)
    {
      SecaoAvulsaAtual.RemoveAll();

      for (POSITION Pos = SecaoTransversaoAv.Terreno.GetHeadPosition(); Pos; SecaoTransversaoAv.Terreno.GetNext(Pos))
      {
        SecaoAvulsaAtual.AddTail(SecaoTransversaoAv.Terreno.GetAt(Pos));
      }

      Achou = true;
    }
  }
  */
}