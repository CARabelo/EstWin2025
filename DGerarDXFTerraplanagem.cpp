// DGerarDXF.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "carqterreno.h"
#include <string>
using std::string;
#include "cponto.h"
#include "mat.h"
#include "Estaca.h"
#include "clpontos.h" 
#include "perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include <list>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
#include "cdeltasuperficie.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CAcidentesTopograficos.h"
#include "CSegmento.h"
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "CEquipDrenagem.h"
#include "superficie.h"
#include "supercstring.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "carqcurhorizontais.h" 
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include <map>
#include "ccurhorizontal.h"
#include "Mat.h"
#include "ccurhorizontais.h"
#include "Cadlib\Interface\VC\Cadlib.h"
#include "DGerarDXFTerraplanagem.h"
#include "monologo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LER 1

extern class Matem Mat;
extern class monologo monolog;

/////////////////////////////////////////////////////////////////////////////
// CDGerarDXF dialog


CDGerarDXFTerrapl::CDGerarDXFTerrapl(CWnd* pParent,CString& NomeProj,CString& NomeProjBase)	: CDialog(CDGerarDXFTerrapl::IDD,pParent),Superficie(NomeProj,false/*,true*/),SecoesTipo(NomeProj),
                                                          OK(true),NomeProjeto(NomeProj)
{
  //--- Nos deltas estão as coordenadas do ponto de origem

  Deltas[0] = Deltas[1] = 0.0;

  //{{AFX_DATA_INIT(CDGerarDXF)
  m_CStrMensagem = _T("");
  //}}AFX_DATA_INIT

  Superficie.SerializarEquipDrenagem(LER, NomeProjeto.GetBuffer());
}

void CDGerarDXFTerrapl::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDGerarDXF)
  DDX_Control(pDX, IDC_GERAR, m_BUGerar);
  DDX_Control(pDX, IDC_SAIR, m_BUSair);
  DDX_Control(pDX, IDC_PROGDXFGEO, m_PrgProgresso);
  DDX_Text(pDX, IDC_MENSGERARDXF, m_CStrMensagem);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDGerarDXFTerrapl, CDialog)
  //{{AFX_MSG_MAP(CDGerarDXF)
  ON_BN_CLICKED(IDC_GERAR, OnGerarDXF)
  ON_BN_CLICKED(IDC_SAIR, OnSair)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDGerarDXF message handlers

void CDGerarDXFTerrapl::OnGerarDXF() 
{
  if(Superficie.pSuperficieAtual->size() == 0)
  {
     m_CStrMensagem ="Não há pontos na superfície...";
     UpdateData(FALSE);

     return;
  }

  m_BUGerar.EnableWindow(false);
  m_BUSair.EnableWindow(false);

  m_CStrMensagem.Empty();
  UpdateData(FALSE);

  CString DxfFileName(NomeProjeto.GetBuffer() + CString("ProjTerraplanagem.dxf"));
  CDrawing drw;

  BOOL result=TRUE;

  // Create Drawing
  if(!drawing.Create())
  {
    m_CStrMensagem ="Erro Fatal Criando o arquivo DXF.";
    UpdateData(FALSE);

    return;
  }

  // Tables Section ------------------------------------------
  //  LTYPE table type -------------------------
  LTYPE		ltype;
  OBJHANDLE	objhandle1,objhandle2,objhandle3,objhandle4;

  //  Continuous
  ZeroMemory(&ltype, sizeof(ltype));
  strcpy(ltype.Name, "Continuous");
  strcpy(ltype.DescriptiveText, "Solid line");
  objhandle1 = drawing.AddLinetype(&ltype);
  //  DASHDOT2
  ZeroMemory(&ltype, sizeof(ltype));
  strcpy(ltype.Name, "DASHDOT2");
  strcpy(ltype.DescriptiveText,	"Dash	dot	(.5x)	_._._._._._._._._._._._._._._.");
  ltype.ElementsNumber = 4;
  ltype.PatternLength	=	0.5  * 4.0;
  ltype.Elements[0]	=	0.25 * 4.0;
  ltype.Elements[1]	=	-0.125 * 4.0;
  ltype.Elements[2]	=	0.0 * 4.0;
  ltype.Elements[3]	=	-0.125 * 4.0;
  objhandle2 = drawing.AddLinetype(&ltype);
  //  DASHDOTDOT
  ZeroMemory(&ltype, sizeof(ltype));
  strcpy(ltype.Name, "DASHDOTDOT");
  strcpy(ltype.DescriptiveText, "Dash dotdot (.5x) _.._.._.._.._.._.._.._.._.._.._.._.._.._.._..");
  ltype.ElementsNumber = 6;
  ltype.PatternLength = 0.5 * 4.0;
  ltype.Elements[0] = 0.20 * 4.0;
  ltype.Elements[1] = -0.1 * 4.0;
  ltype.Elements[2] = 0.0 * 4.0;
  ltype.Elements[3] = -0.1 * 4.0;
  ltype.Elements[4] = 0.0 * 4.0;
  ltype.Elements[5] = -0.1 * 4.0;
  objhandle3 = drawing.AddLinetype(&ltype);
  //  DASH
  ZeroMemory(&ltype, sizeof(ltype));
  strcpy(ltype.Name, "DASH");
  strcpy(ltype.DescriptiveText, "Dash (.5x) _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ ");
  ltype.ElementsNumber = 4;
  ltype.PatternLength = 0.75 * 4.0;
  ltype.Elements[0] = 0.25 * 4.0;
  ltype.Elements[1] = -0.125 * 4.0;
  ltype.Elements[2] = 0.25 * 4.0;
   ltype.Elements[3] = -0.125 * 4.0;
  objhandle4 = drawing.AddLinetype(&ltype);
  //  LAYER table type -------------------------
  LAYER	layer;

  // Layer0
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Textos");         //--- textos 
  layer.Color = COLOR_WHITE;
  layer.LineTypeObjhandle = objhandle1; // Continuous
  drawing.AddLayer(&layer);
  // Layer1
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Pontos");         //--- Pontos da superficie 
  layer.Color = 170;
  layer.LineTypeObjhandle = objhandle1; // Continuous
  drawing.AddLayer(&layer);
  // Layer2
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Fronteira");      //--- Fronteira do terreno
  layer.Color = 104;
  layer.LineTypeObjhandle = objhandle1; // Continuous
  drawing.AddLayer(&layer);
  // Layer 3
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Curvas_nivel");   //--- curvas de Nível 
  layer.Color = 30;
  layer.LineTypeObjhandle = objhandle1; // Continuous
  drawing.AddLayer(&layer);
  // Layer 4
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Off-Sets");       // Off-sets
  layer.Color = 30;
  layer.LineTypeObjhandle = objhandle4; // Dash
  drawing.AddLayer(&layer);
  // Layer 5
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Terraplanagem");    // Terraplanagem
  layer.Color = 30;
  layer.LineTypeObjhandle = objhandle2;   // DashDot
  drawing.AddLayer(&layer);
  // Layer6 Drenagem   
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Drenagem");
  layer.Color = COLOR_GRAY;
  layer.LineTypeObjhandle = objhandle1;
  drawing.AddLayer(&layer);
  // Layer7 Drenagem   - Tracejado
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Drenagem_Tracejado");
  layer.Color = COLOR_GRAY;
  layer.LineTypeObjhandle = objhandle4;
  drawing.AddLayer(&layer);
  // Layer8 CN Simbologia
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "CN_Simbologia");
  layer.Color = 30;
  layer.LineTypeObjhandle = objhandle1; // Continuous
  drawing.AddLayer(&layer);
  // Layer9 Simbologia   
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Simbologia");
  layer.Color = 254;                   //--- Perto branco
  layer.LineTypeObjhandle = objhandle1;
  drawing.AddLayer(&layer);

  //  STYLE table type -------------------------

  STYLE	style;
  ZeroMemory(&style, sizeof(style));
  strcpy(style.Name, "Style1");
  strcpy(style.PrimaryFontFilename, "TIMES.TTF");
  style.LastHeightUsed = 0.3;
  style.WidthFactor = 1;
  objhandle1 = drawing.AddTextStyle(&style);

  //  DIMSTYLE table type ----------------------
  DIMSTYLE	dimstyle;

  //  DIM1
  ZeroMemory(&dimstyle, sizeof(dimstyle));
  strcpy(dimstyle.Name, "DIM1");						// DimStyle Name
  dimstyle.dimclrd = 2;									// Dimension line & Arrow heads color
  dimstyle.dimdle = 0.0000;								// Dimension line size after Extensionline
  dimstyle.dimclre = 2;									// Extension line color
  dimstyle.dimexe = 0.1800;								// Extension line size after Dimline
  dimstyle.dimexo = 0.0625;								// Offset from origin
  strcpy(dimstyle.dimblk1, "ClosedFilled");				// 1st Arrow head
  strcpy(dimstyle.dimblk2, "ClosedFilled");				// 2nd Arrow head
  dimstyle.dimasz = 0.1800;								// Arrow size
  dimstyle.dimtxstyObjhandle = objhandle1;				// Text style
  dimstyle.dimclrt = 3;									// Text color
  dimstyle.dimtxt = 0.1800;								// Text height
  dimstyle.dimtad = 1;									// Vertical Text Placement
  dimstyle.dimgap = 0.0900;								// Offset from dimension line
  drawing.AddDimStyle(&dimstyle);

  //--- Entities Section ----------------------------------------
  //---  Desenha os pontos da superfície - Layer 1

  m_CStrMensagem = "Desenhando a superficie...";
  UpdateData(false);

  double Passo(0.0);
  double Razao(Superficie.pSuperficieAtual->size() / 100.0);
  m_PrgProgresso.SetRange(0,100);
  m_PrgProgresso.SetPos(0);

  for(register ItSSuperficie DeltaSupAtual = Superficie.pSuperficieAtual->begin(); DeltaSupAtual !=  Superficie.pSuperficieAtual->end(); DeltaSupAtual++)
  {
    if((*DeltaSupAtual).PegaPCentral().Inserido == false)
    {
      result &= drawing.Point(DeltaSupAtual->PegaPCentral().x -Deltas[0],DeltaSupAtual->PegaPCentral().y - Deltas[1],DeltaSupAtual->PegaPCentral().z,5.0);      

      if(++Passo > Razao) 
      {
        m_PrgProgresso.StepIt();
        Passo = 0.0;
      }
    }
  }
  //---desenha bordos do terreno. Os bordos e as restriçoes são uma lista de listas, nos bordos a lista de listas tem
  //--- na maioria das vezes apenas uma lista.

  result &= drawing.SetLayer("Fronteira");

  for (int i  = BORDOS ; i <= RESTRICOES ; i++)
  {
    const LLDeltaSup& ListaRestAtual = i == BORDOS ? Superficie.PegaListaBordos() : Superficie.PegaListaRestricoes();

    for(ItCLLDeltaSup LDRestricaoAtual = ListaRestAtual.begin() ; LDRestricaoAtual != ListaRestAtual.end() ; LDRestricaoAtual++)
    {
      size_t QtdPontos = LDRestricaoAtual->size();
      ENTVERTEX* pVertexAtual = (struct tag_ENTVERTEX *)new ENTVERTEX[QtdPontos];
      int Indice = 0;

      for(register ItCLDeltaSup ItLRestricao = LDRestricaoAtual->begin() ;  ItLRestricao != LDRestricaoAtual->end() ; Indice++,ItLRestricao++)
      {
        (pVertexAtual+Indice)->Point.x = ItLRestricao->PCentral.x - Deltas[0];
        (pVertexAtual+Indice)->Point.y = ItLRestricao->PCentral.y - Deltas[1];
      }

      unsigned int ObjetoAtual(CAciTopografico::NENHUM_ACITOPOG);

      if(i== RESTRICOES)
      {
        switch (ObjetoAtual = Superficie.VerfAciTopografico(LDRestricaoAtual))
        {
          case CAciTopografico::FRONTEIRA : result &= drawing.SetLayer("Fronteira"); break;
          case CAciTopografico::CRISTA:
          case CAciTopografico::OFF_SETS: result &= drawing.SetLayer("Off-Sets"); break;
          case CAciTopografico::TERRAPLANAGEM: result &= drawing.SetLayer("Terraplanagem"); break;
          default : result &= drawing.SetLayer("Textos"); break;
        }
      }
      
      result &= drawing.PolyLine(pVertexAtual, (int)QtdPontos);

      delete[] pVertexAtual;
    }
  }

  Passo = 0.0;
  Razao = Superficie.pSuperficieAtual->size() / 100;
  m_PrgProgresso.SetPos(0);

  //--- Desenha as CN

  Ponto PAnterior;

  m_CStrMensagem = "Desenhando as curvas  de nível...";
  UpdateData(false);

  Passo = 0.0;
  Razao =  Superficie.LPontosCN.size() / 100.0;
  m_PrgProgresso.SetPos(0);

  double MultCotaMestra(Superficie.PegaEquidisCN() * 5.0);
  double Thickness(0.0);

  result &= drawing.SetLayer("Curvas_nivel");

  for(ItLLPontos itLLCN = Superficie.LPontosCN.begin() ; itLLCN != Superficie.LPontosCN.end() ; itLLCN++) 
  {
    if(itLLCN->size() > 1)
    {
      ENTVERTEX* pEntVertex((struct tag_ENTVERTEX *) new ENTVERTEX[itLLCN->size()]),*ppEntVertex(pEntVertex);

      ZeroMemory(pEntVertex,sizeof(ENTVERTEX)*itLLCN->size());

      for (ItLPontos itLCN = itLLCN->begin(); itLCN != itLLCN->end() ; itLCN++,pEntVertex++) 
      {   
        ConvPRealPVertex(pEntVertex->Point,*itLCN);
      }
    
      if(fabs(fmod(itLLCN->begin()->z,MultCotaMestra)) < 0.0005) Thickness = 0.15;
      else Thickness = 0;
 
      result &= drawing.PolyLine(ppEntVertex,(int)itLLCN->size(),0);   //--- Desenha a CN

      delete[] ppEntVertex;
    }

    if((++Passo) > Razao)
    {
       m_PrgProgresso.StepIt();
       Passo = 0.0;
    }
  }

  //--- Curvas de nível da terraplanagem

  result &= drawing.SetLayer("CN_Simbologia");

  for (ItLLPontos itLLCN = Superficie.LLPontosCNSimbTerrapl.begin(); itLLCN != Superficie.LLPontosCNSimbTerrapl.end(); itLLCN++)
  {
    if (itLLCN->size() > 1)
    {
      ENTVERTEX* pEntVertex((struct tag_ENTVERTEX*) new ENTVERTEX[itLLCN->size()]), * ppEntVertex(pEntVertex);

      ZeroMemory(pEntVertex, sizeof(ENTVERTEX) * itLLCN->size());

      for (ItLPontos itLCN = itLLCN->begin(); itLCN != itLLCN->end(); itLCN++, pEntVertex++)
      {
        ConvPRealPVertex(pEntVertex->Point, *itLCN);
      }

      if (fabs(fmod(itLLCN->begin()->z, MultCotaMestra)) < 0.0005) Thickness = 0.15;
      else Thickness = 0;

      result &= drawing.PolyLine(ppEntVertex, (int)itLLCN->size(), 0);   //--- Desenha a CN

      delete[] ppEntVertex;
    }

    if ((++Passo) > Razao)
    {
      m_PrgProgresso.StepIt();
      Passo = 0.0;
    }
  }

  //--- Simbologia da terraplanagem

  result &= drawing.SetLayer("Simbologia");

  lstPontos SimbologiaTerrap;

  LeArqSimbologia(SimbologiaTerrap, NomeProjeto.GetBuffer());

  bool LevPena(true);
  itlstPontos itPAnterior(SimbologiaTerrap.begin()),itPAtual(itPAnterior);

  ENTVERTEX pEntVertexAnt, pEntVertexAtual;

  ZeroMemory(&pEntVertexAnt, sizeof(ENTVERTEX));
  ZeroMemory(&pEntVertexAtual, sizeof(ENTVERTEX));

  ConvPRealPVertex(pEntVertexAnt.Point, *itPAnterior);

  while (itPAtual != SimbologiaTerrap.end())
  {
    ++itPAtual;

    ConvPRealPVertex(pEntVertexAtual.Point, *itPAtual);

    drawing.Line(pEntVertexAnt.Point.x, pEntVertexAnt.Point.y, pEntVertexAtual.Point.x,pEntVertexAtual.Point.y);

    pEntVertexAnt = pEntVertexAtual;

    ++itPAtual;    //--- Separador

    if (itPAtual != SimbologiaTerrap.end())
    {
      ++itPAtual;

      if (itPAtual != SimbologiaTerrap.end())
      {
        ConvPRealPVertex(pEntVertexAnt.Point, *itPAtual);
      }
    }
  }

  m_PrgProgresso.StepIt();

  DesenhaEquiDrenagem(drawing, result);

  //--- Cabeçalho

  result &= drawing.SetLayer("Textos");
  result &= drawing.Text("Vias Urbanas & Estradas - Versão 3.00 - Boole Software Ltda - Todos os Direitos Autorais Reservados - 2024",Superficie.PegaLimiteEsq(),Superficie.PegaLimiteInf()-5,3.0) > 0;
  
  CString CStrRPontoBase;
  CStrRPontoBase.Format("Ponto Base = {x=%.3lf, y=%.3lf}",Superficie.PegaLimiteEsq(),Superficie.PegaLimiteInf());
  result &= drawing.Text(CStrRPontoBase,Superficie.PegaLimiteEsq(),Superficie.PegaLimiteInf()-9,3.0) > 0;

  //--- Desenha o ponto base

  result &= drawing.Point(Superficie.PegaLimiteEsq(),Superficie.PegaLimiteInf(),0.0,2); 
  
  // Salva o arquivo dxf
 
   m_CStrMensagem ="Salvando o arquivo DXF";
   UpdateData(FALSE);

  if(result || !drawing.SaveDXFFile(DxfFileName))
  {
    result = FALSE;
    m_CStrMensagem ="Erro Salvando o arquivo DXF!";
    UpdateData(FALSE);
  }

    m_PrgProgresso.SetPos(100);

  // Destroi CDrawing e libera a memoria alocada

  result &= drawing.Destroy();

  if(!result)	m_CStrMensagem = "Arquivo DXF Gerado com sucesso!";
  else m_CStrMensagem += " -> Erro Gerando o arquivo DXF!";

  UpdateData(FALSE);

  m_BUSair.EnableWindow(true);   
}

inline void CDGerarDXFTerrapl::IrradiaPonto(double Angulo,double x,double y, double s,double Distancia,Ponto& PCalcular)
{
  double AZ(s + Angulo);

  PCalcular.x =  x + sin(AZ) * Distancia;
  PCalcular.y =  y + cos(AZ) * Distancia;
}

inline void CDGerarDXFTerrapl::IrradiaPonto(double Angulo,double x,double y, double s,double Distancia,REALPOINT& PCalcular)
{
  double AZ = s + Angulo;

  PCalcular.x =  x + sin(AZ) * Distancia;
  PCalcular.y =  y + cos(AZ) * Distancia;
}

void CDGerarDXFTerrapl::OnSair()
{ 
  EndDialog(TRUE);
}

BOOL CDGerarDXFTerrapl::OnInitDialog() 
{
  CDialog::OnInitDialog();

  UpdateData(false);

  if(!OK) m_BUGerar.EnableWindow(false);   //--- houve erro na inicialização

  // TODO: Add extra initialization here

  CString Titulo;

  GetWindowText(Titulo);
  Titulo.Replace("Geométrico", "Terraplanagem");
  SetWindowText(Titulo);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDGerarDXFTerrapl::DesenhaEquiDrenagem(CDrawing& Drawing, BOOL& Res)
{
  tymapEquipDrenagem& EquipamentosDrenagem(Superficie.PegaObrasDrenagem());

  Ponto Ppixels;

  for (auto& rEquipAtual : EquipamentosDrenagem)
  {
    int Tipo(rEquipAtual.second.PegaTipo());

    if (Tipo == CAciTopografico::EQUI_DRENO_PROFUNDO)  Res &= drawing.SetLayer("Drenagem_Tracejado");
    else Res &= drawing.SetLayer("Drenagem");

    for (auto& ItLLPontos : rEquipAtual.second.PegaAlinhamentos())
    {
      if (ItLLPontos.empty()) continue;

      itlstPontos ItPonto(ItLLPontos.begin());

      ENTVERTEX* pEntVertex((struct tag_ENTVERTEX*) new ENTVERTEX[ItLLPontos.size()]), * ppEntVertex(pEntVertex), * pInicio(pEntVertex);

      ppEntVertex = pEntVertex;

      ZeroMemory(pEntVertex, sizeof(ENTVERTEX) * ItLLPontos.size());
      size_t QtdPontos(0);

      for (ItLPontos itLPontos = ItLLPontos.begin(); itLPontos != ItLLPontos.end(); ++itLPontos)
      {
        if (itLPontos->x != INFINITO)
        {
          ConvPRealPVertex(pEntVertex->Point, *itLPontos);
          ++QtdPontos;
          ++pEntVertex;
        }
        else
        {
          Res &= drawing.PolyLine(ppEntVertex, QtdPontos, 0);
          ppEntVertex += QtdPontos;
          QtdPontos = 0;
        }
      }

      if (QtdPontos) Res &= drawing.PolyLine(ppEntVertex, QtdPontos, 0);

      delete[] pInicio;
    }
  }
}

bool CDGerarDXFTerrapl::LeArqSimbologia(lstPontos& SimbologiaTerrap, const std::string& NomeProjeto)
{
  CFile ArqSimbologia;
  CFileException e;

  std::string NomeCompArquivo(NomeProjeto + ".sbt");

  if (ArqSimbologia.Open(NomeCompArquivo.data(), CFile::modeRead, &e) == 0)
  {
    monolog.mensagem(13, CString(NomeCompArquivo.data()) + " (Lendo dados)");

    return false;
  }
  else
  {
    SimbologiaTerrap.clear();

    CArchive ar(&ArqSimbologia, CArchive::load);

    size_t QuanReg;
    Ponto PAtual;

    for (ar >> QuanReg; QuanReg > 0; --QuanReg)
    {
      PAtual << ar;

      SimbologiaTerrap.emplace_back(PAtual);
    }
  }

  return true;
}