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
#include "DGerarDXF.h"
#include "monologo.h"
#include "CArqTrechos.h"
#include <sstream>  

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class Matem Mat;
extern class monologo monolog;

/////////////////////////////////////////////////////////////////////////////
// CDGerarDXF dialog

CDGerarDXF::CDGerarDXF(CWnd* pParent,CString& NomeProj,CString& pNomeProjBase,double pCx,double pCy,double pEscala) : CDialog(CDGerarDXF::IDD,pParent),Superficie(pNomeProjBase),//,SecoesTipo(NomeProj),CurvasHorizontais(NomeProj,SecoesTipo),
                                                          OK(true),NomeProjeto(NomeProj),NomeProjetoBase(pNomeProjBase),PLocal(pCx,pCy),Escala(pEscala)
{
}

void CDGerarDXF::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDGerarDXF)
  DDX_Control(pDX, IDC_GERAR, m_BUGerar);
  DDX_Control(pDX, IDC_SAIR, m_BUSair);
  DDX_Control(pDX, IDC_PROGDXFGEO, m_PrgProgresso);
  DDX_Text(pDX, IDC_MENSGERARDXF, m_CStrMensagem);
  DDX_Control(pDX, IDC_CHETODOSTRECHOS, CheTodosTrechos);
  DDX_Control(pDX, IDC_CHETRECHOATUAL, CheTrechoAtual);

  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDGerarDXF, CDialog)
  //{{AFX_MSG_MAP(CDGerarDXF)
  ON_BN_CLICKED(IDC_GERAR, OnGerarDXF)
  ON_BN_CLICKED(IDC_SAIR, OnSair)
  //}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_CHETODOSTRECHOS, &CDGerarDXF::OnBnClickedChetodostrechos)
  ON_BN_CLICKED(IDC_CHETRECHOATUAL, &CDGerarDXF::OnBnClickedChectrechoatual)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDGerarDXF message handlers

void CDGerarDXF::OnGerarDXF()
{
  if (Superficie.pSuperficieAtual->size() == 0)
  {
    m_CStrMensagem = "Não há pontos na superfície...";
    UpdateData(FALSE);

    return;
  }

  m_BUGerar.EnableWindow(false);
  m_BUSair.EnableWindow(false);

  m_CStrMensagem.Empty();
  UpdateData(FALSE);

  CString DxfFileName(NomeProjeto.GetBuffer() + CString("Geometrico.dxf"));
  CDrawing drw;

  BOOL result = TRUE;

  // Create Drawing
  if (!drawing.Create())
  {
    m_CStrMensagem = "Erro Fatal Criando o arquivo DXF.";
    UpdateData(FALSE);

    return;
  }

  // Tables Section ------------------------------------------
  //  LTYPE table type -------------------------
  LTYPE		ltype;
  OBJHANDLE	objhandle1, objhandle2, objhandle3, objhandle4;

  //  Continuous
  ZeroMemory(&ltype, sizeof(ltype));
  strcpy(ltype.Name, "Continuous");
  strcpy(ltype.DescriptiveText, "Solid line");
  objhandle1 = drawing.AddLinetype(&ltype);
  //  DASHDOT2
  ZeroMemory(&ltype, sizeof(ltype));
  strcpy(ltype.Name, "DASHDOT2");
  strcpy(ltype.DescriptiveText, "Dash	dot	(.5x)	_._._._._._._._._._._._._._._.");
  ltype.ElementsNumber = 4;
  ltype.PatternLength = 0.5  * 4.0;
  ltype.Elements[0] = 0.25 * 4.0;
  ltype.Elements[1] = -0.125 * 4.0;
  ltype.Elements[2] = 0.0 * 4.0;
  ltype.Elements[3] = -0.125 * 4.0;
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
  strcpy(layer.Name, "Projeto_Geometrico");         //--- Projeto Geometrico
  layer.Color = COLOR_RED;
  layer.LineWeight = 4;
  layer.LineTypeObjhandle = objhandle1; // Continuous
  drawing.AddLayer(&layer);
  // Layer3
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Fronteira");         //--- Fronteira do terreno
  layer.Color = 104;
  layer.LineTypeObjhandle = objhandle1; // Continuous
  drawing.AddLayer(&layer);
  // Layer4
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Cristas_pes");         //--- Cristas e pés Não esta sendo usado
  layer.Color = COLOR_CYAN;
  layer.LineTypeObjhandle = objhandle1; // Continuous
  drawing.AddLayer(&layer);
  // Layer5
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Secoes");         //--- Secoes 
  layer.Color = COLOR_YELLOW;
  layer.LineTypeObjhandle = objhandle1; // Continuous
  drawing.AddLayer(&layer);
  // Layer6
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Triangulacao");         //--- Triangulação 
  layer.Color = COLOR_MAGENTA;
  layer.LineTypeObjhandle = objhandle1; // Continuous
  drawing.AddLayer(&layer);
  // Layer7
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Curvas_nivel");         //--- curvas de Nível 
  layer.Color = 30;
  layer.LineTypeObjhandle = objhandle1; // Continuous
  drawing.AddLayer(&layer);
  // Layer8
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Muro");         //--- Muro 
  layer.Color = COLOR_CYAN;
  layer.LineTypeObjhandle = objhandle1; // Continuous
  drawing.AddLayer(&layer);
  // Layer9
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Cerca");         //--- Cerca 
  layer.Color = COLOR_RED;
  layer.LineTypeObjhandle = objhandle2; // DashDot
  drawing.AddLayer(&layer);
  // Layer10
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Bordo");         //--- Bordo 
  layer.Color = 33;
  layer.LineTypeObjhandle = objhandle4; // Dashs
  drawing.AddLayer(&layer);
  // Layer11
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Meio_Fio");         //--- Meio-Fio 
  layer.Color = COLOR_CYAN;
  layer.LineTypeObjhandle = objhandle4; // DashDot
  drawing.AddLayer(&layer);
  // Layer12
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Edificacao");         //--- Casa 
  layer.Color = COLOR_YELLOW;
  layer.LineTypeObjhandle = objhandle1; // Continuous
  drawing.AddLayer(&layer);
  // Layer13
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "N_A");         //--- N.A. 
  layer.Color = COLOR_WHITE_BLUE;
  layer.LineTypeObjhandle = objhandle1; //Continuous
  drawing.AddLayer(&layer);
  // Layer14
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Rocha");         //--- Rocha 
  layer.Color = COLOR_MAGENTA;
  layer.LineTypeObjhandle = objhandle3; // DashDotDot
  drawing.AddLayer(&layer);
  // Layer15
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Ponte");         //--- Ponte 
  layer.Color = COLOR_BLUE;
  layer.LineTypeObjhandle = objhandle1; // Continuous
  drawing.AddLayer(&layer);
  // Layer16
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Poste");         //--- Poste. 
  layer.Color = COLOR_BLUE;
  layer.LineTypeObjhandle = objhandle1; //Continuous
  drawing.AddLayer(&layer);
  // Layer17
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Bueiro");         //--- Bueiro 
  layer.Color = COLOR_MAGENTA;
  layer.LineTypeObjhandle = objhandle4; // DashDot
  drawing.AddLayer(&layer);
  // Layer18
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Rede_Elet_BT");         //--- Rede elétrica BT 
  layer.Color = COLOR_RED;
  layer.LineTypeObjhandle = objhandle3; // DashDotDot
  drawing.AddLayer(&layer);
  // Layer19
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Rede_Elet_AT");         //--- Rede Elétrica AT 
  layer.Color = COLOR_RED;
  layer.LineTypeObjhandle = objhandle4; // DashDot
  drawing.AddLayer(&layer);
  // Layer20
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Pontos_Inseridos");         //--- Pontos Inseridos 
  layer.Color = COLOR_GRAY;
  layer.LineTypeObjhandle = objhandle4; // DashDot
  drawing.AddLayer(&layer);
  // Layer21
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Cristas");         //--- Cristas
  layer.Color = 31;
  layer.LineTypeObjhandle = objhandle1; // Continuous
  drawing.AddLayer(&layer);
  // Layer22
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Pes");         //--- Pes 
  layer.Color = 31;
  layer.LineTypeObjhandle = objhandle4; // Dash
  drawing.AddLayer(&layer);
  // Layer23
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Ferrovia");         //--- Ferrovia 
  layer.Color = 160;
  layer.LineTypeObjhandle = objhandle4; // Dash
  drawing.AddLayer(&layer);
  // Layer24
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Mapa");         //--- Mapa 
  layer.Color = 160;
  layer.LineTypeObjhandle = objhandle4; // Dash
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
  dimstyle.dimclrd = 2;											// Dimension line & Arrow heads color
  dimstyle.dimdle = 0.0000;									// Dimension line size after Extensionline
  dimstyle.dimclre = 2;											// Extension line color
  dimstyle.dimexe = 0.1800;									// Extension line size after Dimline
  dimstyle.dimexo = 0.0625;									// Offset from origin
  strcpy(dimstyle.dimblk1, "ClosedFilled");	// 1st Arrow head
  strcpy(dimstyle.dimblk2, "ClosedFilled");	// 2nd Arrow head
  dimstyle.dimasz = 0.1800;									// Arrow size
  dimstyle.dimtxstyObjhandle = objhandle1;	// Text style
  dimstyle.dimclrt = 3;											// Text color
  dimstyle.dimtxt = 0.1800;									// Text height
  dimstyle.dimtad = 1;											// Vertical Text Placement
  dimstyle.dimgap = 0.0900;									// Offset from dimension line
  drawing.AddDimStyle(&dimstyle);

  //--- Entities Section ----------------------------------------
  //---  Desenha os pontos da superfície - Layer 1

  CArqTrechos NomesTrechos(NomeProjetoBase.GetBuffer(), true);

  for (ittymapNomesTrechos it = NomesTrechos.MapaNomesTrechos.begin(); it != NomesTrechos.MapaNomesTrechos.end(); it++)
  {
    if (CheTrechoAtual.GetCheck() == BST_CHECKED && it->first.find(NomeProjeto) == std::string::npos) continue;

    CSecTipo SecoesTipo(CString(it->first.c_str()));
    CCurHorizontais CurvasHorizontais(CString(it->first.c_str()), SecoesTipo);
    MSecaoGeom MapaSecoesGeom;           //--- Mapa de secoes geometricas.
    std::list<Ponto> LPSecoes;
    MCurHorizontaisTS mapCurvasHorizontaisTSDXF;

    Deltas[0] = Deltas[1] = 0.0;

    mapCurvasHorizontaisTSDXF = CurvasHorizontais.PegaMapCurHorizontaisTS();

    CFile ArqCalcSec;
    CFileException e;
    unsigned int QSecoes(0);

     //{{AFX_DATA_INIT(CDGerarDXF)
    m_CStrMensagem = _T("");
    //}}AFX_DATA_INIT

    if (ArqCalcSec.Open(CString(it->first.c_str()) + CString(".pes"), CFile::modeRead | CFile::shareDenyNone, &e) != 0)
    {
      CArchive CArqCalcSec(&ArqCalcSec, CArchive::load);
      Ponto PEsq, PDir, PEixo;
      CEstaca EstacaAtual;
      for (CArqCalcSec >> QSecoes; QSecoes; QSecoes--)  //--- Busca coordenadas de todas as seções.
      {
        EstacaAtual << CArqCalcSec;

        PDir << CArqCalcSec;
        PEixo << CArqCalcSec;
        PEsq << CArqCalcSec;

        LPSecoes.push_back(PDir);
        LPSecoes.push_back(PEixo);
        LPSecoes.push_back(PEsq);

        MapaSecoesGeom.insert(MSecaoGeom::value_type(EstacaAtual, PEixo));
      }

      CArqCalcSec.Close();
      ArqCalcSec.Close();
    }

    m_CStrMensagem = "Desenhando a superficie...";
    UpdateData(false);
    //m_PrgProgresso.SetRange(0,((short)(Superficie.PegaSuperficie()).size()));

     /*    Teste SPLINE
    result &= drawing.SetLayer("Layer1");

     ENTVERTEX  AVertexRasc[4];

     AVertexRasc[0].Point.x = 0.0;
     AVertexRasc[0].Point.y = 0.0;
     AVertexRasc[1].Point.x = 1.0;
     AVertexRasc[1].Point.y = 1.0;
     AVertexRasc[2].Point.x = 2.0;
     AVertexRasc[2].Point.y = 1.0;
     AVertexRasc[3].Point.x = 3.0;
     AVertexRasc[3].Point.y = 0.0;

     drawing.SpLine(&AVertexRasc[0],4);

     if(!drawing.SaveDXFFile(DxfFileName))
    {
      result = FALSE;
      m_CStrMensagem ="Erro Salvando o arquivo DXF!";
      UpdateData(FALSE);
    }

    // Destroi CDrawing e libera a memoria alocada

    result &= drawing.Destroy();

    if(!result)	m_CStrMensagem = "Arquivo DXF Gerado com sucesso!";
    else m_CStrMensagem += " -> Erro Gerando o arquivo DXF!";

    UpdateData(FALSE);

    m_BUSair.EnableWindow(true);

        if(1) return;
     */

    //--- Mapa ==============================================================================

    std::string NomeArquivo(NomeProjetoBase + ".ini");

    char Buffer[1024] = { 0 };
    CString Default;

    Default.Format("%lf,%lf,%lf,%i,%lf,%lf,%lf,%s", 0.0, 0.0, 1.0, 0, 1.0, 0.0, 0.0, "");
    ::GetPrivateProfileString(_T("DadosDesenhos"),  _T("DadosMapa"), Default, Buffer, 511, NomeArquivo.c_str());

    std::stringstream strstrBuffer(Buffer);
    char Virgula;
    double DeltasMapa[2], m_nRatio, m_nX, m_nY, RazaoEscalasJPGDes;
    int ProjetoGeoreferenciado;

    strstrBuffer >> DeltasMapa[0] >> Virgula >> DeltasMapa[1] >> Virgula >> RazaoEscalasJPGDes >> Virgula >> ProjetoGeoreferenciado >> Virgula
                 >> m_nRatio >> Virgula >> m_nX >> Virgula >> m_nY >> Virgula;

    char BufferNome[1024] = { 0 };

    strstrBuffer.getline(BufferNome, 1023);

    if (strlen(BufferNome) > 3)
    {
   //   result &= drawing.SetLayer("Mapa");
   //   drawing.Image(BufferNome, m_nX, m_nY, m_nRatio, m_nRatio);
    }

    //===================================================================================

    double Passo(0.0);
    double Razao(Superficie.pSuperficieAtual->size() / 100.0);
    m_PrgProgresso.SetRange(0, 100);
    m_PrgProgresso.SetPos(0);

    result = 0;

    result &= drawing.SetLayer("Pontos");

    for (register ItSSuperficie DeltaSupAtual = Superficie.pSuperficieAtual->begin(); DeltaSupAtual != Superficie.pSuperficieAtual->end(); DeltaSupAtual++)
    {
      if ((*DeltaSupAtual).PegaPCentral().Inserido == false)
      {
        result &= drawing.Point(DeltaSupAtual->PegaPCentral().x - Deltas[0], DeltaSupAtual->PegaPCentral().y - Deltas[1], DeltaSupAtual->PegaPCentral().z, 5.0);

        if (++Passo > Razao)
        {
          m_PrgProgresso.StepIt();
          Passo = 0.0;
        }
      }
    }

    result &= drawing.SetLayer("Pontos_Inseridos");

    for (register ItSSuperficie DeltaSupAtual = Superficie.pSuperficieAtual->begin(); DeltaSupAtual != Superficie.pSuperficieAtual->end(); DeltaSupAtual++)
    {
      if (DeltaSupAtual->PegaPCentral().Inserido == true)
      {
        result &= drawing.Point(DeltaSupAtual->PegaPCentral().x - Deltas[0], DeltaSupAtual->PegaPCentral().y - Deltas[1], DeltaSupAtual->PegaPCentral().z, 5.0);

        if (++Passo > Razao)
        {
          m_PrgProgresso.StepIt();
          Passo = 0.0;
        }
      }
    }

    size_t Tam(mapCurvasHorizontaisTSDXF.size());

    ENTVERTEX* pVertexEixo((struct tag_ENTVERTEX *)new ENTVERTEX[Tam]),
             * pVertexBEsq((struct tag_ENTVERTEX *)new ENTVERTEX[Tam * 2]),
             * pVertexBDir((struct tag_ENTVERTEX *)new ENTVERTEX[Tam * 2]);

    ZeroMemory(pVertexEixo, sizeof(ENTVERTEX)*Tam);
    ZeroMemory(pVertexBEsq, sizeof(ENTVERTEX)*Tam * 2);
    ZeroMemory(pVertexBDir, sizeof(ENTVERTEX)*Tam * 2);

    double Larguras[2];
    Ponto PLateral;

    //--- Desenha o eixo e os bordos - Layer 2

    result &= drawing.SetLayer("Projeto_Geometrico");
    m_CStrMensagem = "Desenhando o projeto geométrico...";
    UpdateData(false);

    Passo = 0.0;
    Razao = mapCurvasHorizontaisTSDXF.size() / 100.0;
    m_PrgProgresso.SetPos(0);

    int i(0);

    for (ItMCurHorizontaisTS itMCH = mapCurvasHorizontaisTSDXF.begin(); itMCH != mapCurvasHorizontaisTSDXF.end(); itMCH++)
    {
      //--- Acumula os vertices para desenhar a polyline

#undef PI   // ohhh my God!!!!!!!

    //--- Marca o PI TS SC CS ST

      ConvPRealPVertex((pVertexEixo + i)->Point, itMCH->second.PI);
      drawing.Point((pVertexEixo + i)->Point.x, (pVertexEixo + i)->Point.y, 0.0, 5.0);

      //--- desenha a curva, eixo e bordos 

      if (itMCH->second.Lc > 0.0) DesenhaEspiral((itMCH->second), &result,SecoesTipo,CurvasHorizontais);
      if (itMCH != --mapCurvasHorizontaisTSDXF.end()) DesenhaCircular(itMCH->second, &result, SecoesTipo, CurvasHorizontais);

      //--- Calcula os pontos nos bordos, no TS e no ST, já no sistema do AUTOCAD

      SecoesTipo.BuscaLargurasPistas(itMCH->second.EstTs, Larguras[ESQUERDO], Larguras[DIREITO]);

      //--- TS
      IrradiaPonto(Matem::M_PI_2, itMCH->second.TS.x - Deltas[0], itMCH->second.TS.y - Deltas[1], itMCH->second.TS.fi, Larguras[0], ((pVertexBEsq + i * 2)->Point));

      //--- Bordo Direito
      IrradiaPonto(Matem::M_PI_2, itMCH->second.TS.x - Deltas[0], itMCH->second.TS.y - Deltas[1], itMCH->second.TS.fi, Larguras[1], (pVertexBDir + i * 2)->Point);

      //--- ST
      SecoesTipo.BuscaLargurasPistas(itMCH->second.EstSt, Larguras[ESQUERDO], Larguras[DIREITO]);

      IrradiaPonto(Matem::M_PI_2, itMCH->second.ST.x - Deltas[0], itMCH->second.ST.y - Deltas[1], itMCH->second.ST.fi, Larguras[0], (pVertexBEsq + i * 2 + 1)->Point);

      //--- Bordo Direito
      IrradiaPonto(Matem::M_PI_2, itMCH->second.ST.x - Deltas[0], itMCH->second.ST.y - Deltas[1], itMCH->second.ST.fi, Larguras[1], (pVertexBDir + i * 2 + 1)->Point);

      i++;

      if (++Passo > Razao)
      {
        m_PrgProgresso.StepIt();
        Passo = 0.0;
      }
    }

    result &= drawing.PolyLine(pVertexEixo,(int)mapCurvasHorizontaisTSDXF.size());   //--- eixo

    //--- Desenha um ponto nas estacas

    if (LPSecoes.size())
    {
      Ponto PAtual, Ppixels;

      register std::list<Ponto>::iterator itlPontos = LPSecoes.begin(), itPAtual;

      //--- Desenha um ponto no eixo

      while (itlPontos != LPSecoes.end())
      {
        // m_PrgProgresso.StepIt();

        itPAtual = itlPontos++;
        result &= drawing.Point(itlPontos->x - Deltas[0], itlPontos->y - Deltas[1], itlPontos->z != INFINITO ? itlPontos->z : 0.0, 5.0);
        itlPontos++;
        itlPontos++;
      }
    }

    Passo = 0.0;
    Razao = ((i - 1) * 2) / 100.0;
    m_PrgProgresso.SetPos(0);

    //--- Desenha os Bordos da pista

    for (int i2 = 1; i2 < (i - 1) * 2; i2++, i2++)
    {
      result &= drawing.Line((pVertexBEsq + i2)->Point.x, (pVertexBEsq + i2)->Point.y, (pVertexBEsq + i2 + 1)->Point.x, (pVertexBEsq + i2 + 1)->Point.y) > 0;
      result &= drawing.Line((pVertexBDir + i2)->Point.x, (pVertexBDir + i2)->Point.y, (pVertexBDir + i2 + 1)->Point.x, (pVertexBDir + i2 + 1)->Point.y) > 0;

      if (++Passo > Razao)
      {
        m_PrgProgresso.StepIt();
        Passo = 0.0;
      }
    }

    delete[] pVertexEixo;
    delete[] pVertexBEsq;
    delete[] pVertexBDir;

    //---desenha bordos do terreno. Os bordos e as restriçoes são uma lista de listas, nos bordos a lista de listas tem
    //--- na maioria das vezes apenas uma lista.

    result &= drawing.SetLayer("Fronteira");

    for (i = BORDOS; i <= RESTRICOES; i++)
    {
      const LLDeltaSup& ListaRestAtual = i == BORDOS ? Superficie.PegaListaBordos() : Superficie.PegaListaRestricoes();

      for (ItCLLDeltaSup LDRestricaoAtual = ListaRestAtual.begin(); LDRestricaoAtual != ListaRestAtual.end(); LDRestricaoAtual++)
      {
        size_t QtdPontos = LDRestricaoAtual->size();
        ENTVERTEX* pVertexAtual = (struct tag_ENTVERTEX *)new ENTVERTEX[QtdPontos];
        int Indice = 0;

        //  m_PrgProgresso.StepIt();

        for (register ItCLDeltaSup ItLRestricao = LDRestricaoAtual->begin(); ItLRestricao != LDRestricaoAtual->end(); Indice++, ItLRestricao++)
        {
          (pVertexAtual + Indice)->Point.x = ItLRestricao->PCentral.x - Deltas[0];
          (pVertexAtual + Indice)->Point.y = ItLRestricao->PCentral.y - Deltas[1];
          (pVertexAtual + Indice)->Point.z = ItLRestricao->PCentral.z;
        }

        unsigned int ObjetoAtual(CAciTopografico::NENHUM_ACITOPOG);

        if (i == RESTRICOES)
        {
          switch (ObjetoAtual = Superficie.VerfAciTopografico(LDRestricaoAtual))
          {
          case CAciTopografico::BORDO: result &= drawing.SetLayer("Bordo"); break;
          case CAciTopografico::N_A: result &= drawing.SetLayer("N_A"); break;
          case CAciTopografico::ROCHA:result &= drawing.SetLayer("Rocha"); break;
          case CAciTopografico::MEIO_FIO: result &= drawing.SetLayer("Meio_Fio"); break;
          case CAciTopografico::PONTE: result &= drawing.SetLayer("Ponte"); break;
          case CAciTopografico::BUEIRO: result &= drawing.SetLayer("Bueiro"); break;
          case CAciTopografico::POSTE: result &= drawing.SetLayer("Poste"); break;
          case CAciTopografico::REDE_ELETRICA_BT:result &= drawing.SetLayer("Rede_Elet_BT"); break;
          case CAciTopografico::REDE_ELETRICA_AT:result &= drawing.SetLayer("Rede_Elet_AT"); break;
          case CAciTopografico::FRONTEIRA: result &= drawing.SetLayer("Fronteira"); break;
          case CAciTopografico::CANTO_CASA:
          case CAciTopografico::CASA:  result &= drawing.SetLayer("Edificacao"); break;
          case CAciTopografico::CANTO_MURO:
          case CAciTopografico::MURO:  result &= drawing.SetLayer("Muro"); break;
          case CAciTopografico::CANTO_CERCA:
          case CAciTopografico::CERCA:result &= drawing.SetLayer("Cerca"); break;
          case CAciTopografico::TAMPA_BUEIRO:result &= drawing.SetLayer("Tampa_Bueiro"); break;
          case CAciTopografico::CRISTA:result &= drawing.SetLayer("Cristas"); break;
          case CAciTopografico::PE:result &= drawing.SetLayer("Pes"); break;
          case CAciTopografico::LINHA_FERREA:result &= drawing.SetLayer("Ferrovia"); break;
          default: result &= drawing.SetLayer("Textos"); break;
          }
        }

        if (ObjetoAtual == CAciTopografico::POSTE || ObjetoAtual == CAciTopografico::TAMPA_BUEIRO)
        {
          for (int C = 0; C < QtdPontos; C++)
          {
            if (ObjetoAtual == CAciTopografico::TAMPA_BUEIRO) result &= drawing.Circle(pVertexAtual[C].Point.x, pVertexAtual[C].Point.y, 0.6);
            else result &= drawing.Circle(pVertexAtual[C].Point.x, pVertexAtual[C].Point.y, 0.4);
          }
        }
        else result &= drawing.PolyLine(pVertexAtual, (int)QtdPontos, 8);

        delete[] pVertexAtual;
      }

      result &= drawing.SetLayer("Cristas_pes");
    }

    result &= drawing.SetLayer("Secoes");

    if (LPSecoes.size())
    {
      Ponto PAtual, Ppixels;

      register std::list<Ponto>::iterator itlPontos = LPSecoes.begin(), itPAtual;

      //--- Desenha os segmentos de retas das seções e desenha um ponto no eixo

      while (itlPontos != LPSecoes.end())
      {
        //   m_PrgProgresso.StepIt();

        itPAtual = itlPontos++;

        result &= drawing.Point(itlPontos->x - Deltas[0],itlPontos->y - Deltas[1],itlPontos->z != INFINITO ? itlPontos->z : 0.0, 5.0);

        itlPontos++;
        result &= drawing.Line(itPAtual->x - Deltas[0], itPAtual->y - Deltas[1], itlPontos->x - Deltas[0], itlPontos->y - Deltas[1]);
        itlPontos++;
      }
    }

    Passo = 0.0;
    Razao = Superficie.pSuperficieAtual->size() / 100;
    m_PrgProgresso.SetPos(0);

    //--- Desenha as Arestas

    if (false)
    {
      result &= drawing.SetLayer("Triangulacao");

      for (ItSSuperficie DeltaSupAtual = Superficie.pSuperficieAtual->begin(); DeltaSupAtual != Superficie.pSuperficieAtual->end(); DeltaSupAtual++)
      {
        const Ponto& PCentral = DeltaSupAtual->PegaPCentral();   //--- Pega o ponto atual
        Ponto PPixelsCentral(PCentral.x - Deltas[0], PCentral.y - Deltas[1]);
        const Ponto* pPontoAtual;

        for (register ittysetArestas PLigAtual = DeltaSupAtual->PontosLigados.begin(); PLigAtual != DeltaSupAtual->PontosLigados.end(); PLigAtual++)
        {
          pPontoAtual = &PLigAtual->PontoLigado;

          if (!pPontoAtual->Baliza)             //--- Se este ponto ainda não foi desenhado
          {
            Ppixels.x = pPontoAtual->x - Deltas[0];
            Ppixels.y = pPontoAtual->y - Deltas[1];

            result &= drawing.Line(PPixelsCentral.x, PPixelsCentral.y, Ppixels.x, Ppixels.y);
          }
        }

        (const_cast<CDeltaSuperficie*>(&*DeltaSupAtual))->PegaPCentralNC().Baliza = false;            //--- sinaliza q já foi desenhado

        if (++Passo > Razao)
        {
          m_PrgProgresso.StepIt();
          Passo = 0.0;
        }
      }
    }

    //--- Desenha as CN

    Ponto PAnterior;

    m_CStrMensagem = "Desenhando as curvas  de nível...";
    UpdateData(false);
    // m_PrgProgresso.SetRange(0,(short)Superficie.LPontosCN.size() * 700);
    // m_PrgProgresso.SetStep(1);
   //  m_PrgProgresso.SetPos(0);

    Passo = 0.0;
    Razao = Superficie.LPontosCN.size() / 100.0;
    m_PrgProgresso.SetPos(0);

    double MultCotaMestra(Superficie.PegaEquidisCN() * 5.0);
    double Thickness(0.0);

    result &= drawing.SetLayer("Curvas_nivel");

    for (ItLLPontos itLLCN = Superficie.LPontosCN.begin(); itLLCN != Superficie.LPontosCN.end(); itLLCN++)
    {
      // m_PrgProgresso.StepIt();

      if (itLLCN->size() > 1)
      {
        ENTVERTEX* pEntVertex((struct tag_ENTVERTEX *) new ENTVERTEX[itLLCN->size()]), *ppEntVertex(pEntVertex);

        ZeroMemory(pEntVertex, sizeof(ENTVERTEX)*itLLCN->size());

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

    m_PrgProgresso.StepIt();

    //--- Cabeçalho

    result &= drawing.SetLayer("Textos");
    result &= drawing.Text("Vias Urbanas & Estradas - Versão 4.40 - Boole Software Ltda - Todos os Direitos Autorais Reservados - 2017", Superficie.PegaLimiteEsq(), Superficie.PegaLimiteInf(), 3.0) > 0;

    CString CStrRPontoBase;
    CStrRPontoBase.Format("Ponto Base = {x=%.3lf, y=%.3lf}", Superficie.PegaLimiteEsq(), Superficie.PegaLimiteInf());
    result &= drawing.Text(CStrRPontoBase, Superficie.PegaLimiteEsq(), Superficie.PegaLimiteInf() - 4, 3.0) > 0;

    m_PrgProgresso.SetPos(100);
  }

  //--- Desenha o ponto base

 // result &= drawing.Point(Superficie.PegaLimiteEsq(), Superficie.PegaLimiteInf(), 0.0, 2);

  // Salva o arquivo dxf

  m_CStrMensagem = "Salvando o arquivo DXF";
  UpdateData(FALSE);

  if (result || !drawing.SaveDXFFile(DxfFileName))
  {
    result = FALSE;
    m_CStrMensagem = "Erro Salvando o arquivo DXF!";
    UpdateData(FALSE);
  }

  // Destroi CDrawing e libera a memoria alocada

  result &= drawing.Destroy();

  if(!result)	m_CStrMensagem = "Arquivo DXF Gerado com sucesso!";
  else m_CStrMensagem += " -> Erro Gerando o arquivo DXF!";

  UpdateData(FALSE);

  m_BUSair.EnableWindow(true);   
}

inline void CDGerarDXF::IrradiaPonto(double Angulo,double x,double y, double s,double Distancia,Ponto& PCalcular)
{
  double AZ(s + Angulo);

  PCalcular.x =  x + sin(AZ) * Distancia;
  PCalcular.y =  y + cos(AZ) * Distancia;
}

inline void CDGerarDXF::IrradiaPonto(double Angulo,double x,double y, double s,double Distancia,REALPOINT& PCalcular)
{
  double AZ = s + Angulo;

  PCalcular.x =  x + sin(AZ) * Distancia;
  PCalcular.y =  y + cos(AZ) * Distancia;
}

void CDGerarDXF::DesenhaCircular(const CurHorizontal& Curva,int* result,CSecTipo& SecoesTipo,CCurHorizontais& CurvasHorizontais)
{
  double PI_2(Mat.M_PI_2),Largura[2],Angulos[2]= {Curva.SC.fi,Curva.CS.fi};

  //--- Calcula os azimutes para o inicio e o fim da curva

  for (int i = 0 ; i < 2 ; i++)
  {
    if (Curva.Ac <  0.0) Angulos[i] += Mat.M_PI_2;
    else Angulos[i] -=PI_2;
    if (Angulos[i] < 0.0) Angulos[i] += Mat.M_2_PI;

    Angulos[i] = Angulos[i] / Mat.M_PI * 180.0;

    //--- Converte o angulo para o sistema cartesiano do AUTOCAD (zero grau ao leste, antihorário)

  //  Angulos[i] = (360 - Angulos[i]);

    Angulos[i] = 450.0 - Angulos[i];

    //--- No primeiro quadrante o angulo fica > 360 então reduz ao primeiro ciclo.

    if (Angulos[i] > 360.0) Angulos[i] -= 360.0;
  }

  //--- Dependendo do lado da curva o angulo final pode ser menor que o inicial, mas no AUTOCAD o inicial tem q ser menor.

  if (Angulos[0]  > Angulos[1])
  {
    double temp;

    temp = Angulos[0];
    Angulos[0] = Angulos[1];
    Angulos[1] = temp;
  }

  SecoesTipo.BuscaLargurasPistas(Curva.EstSc,Largura[ESQUERDO],Largura[DIREITO]);
 
  //--- Soma a SuperLargura

  double SL(CurvasHorizontais.BuscaSuperLargura(Curva.EstSc));
  if(SL > 0.0)
  {
    Largura[ESQUERDO] -= SL;
    Largura[DIREITO] += SL;
  }

  //--- Desenha a curva, bordos e eixo

  Ponto CC(Curva.CC.x - Deltas[0],Curva.CC.y -  Deltas[1]);

  *result &= drawing.Arc(CC.x,CC.y,Curva.Raio+Largura[0],Angulos[0],Angulos[1]);
  *result &= drawing.Arc(CC.x,CC.y,Curva.Raio,Angulos[0],Angulos[1]);
  *result &= drawing.Arc(CC.x,CC.y,Curva.Raio+Largura[1],Angulos[0],Angulos[1]);

  //--- Marca o centro da curva

  *result &= drawing.Point(CC.x,CC.y,0.0,5.0);  
}
/*
void CDGerarDXF::DesenhaEspiral(const CurHorizontal& Curva,int* result)
{
  static int DELTA(5);                            //--- Fator de cresimento do array        
  MSecaoGeom* pMsecoesGeom = &MapaSecoesGeom;     //--- Ponteiro que apontontará para o mapa a ser usado o definitivo ou o temporário (default = definitivo)

  register itMSecaoGeom IMSecaoGeomAtual = MapaSecoesGeom.find(Curva.EstTs);     //--- Procura pelo TS no mapa de seções

  for(int Ramo = ESQUERDO ; Ramo <= DIREITO ; Ramo++)
  {
    //--- Desenha uma parabola cúbica de 3 em 3 pontos. A parabola cubica é MUITO proxima a espiral para os raios envolvidos.

    if (Ramo == ESQUERDO) IMSecaoGeomAtual = pMsecoesGeom->find(Curva.EstTs);
    else IMSecaoGeomAtual = pMsecoesGeom->find(Curva.EstCs);    

    VENTVERTEX VPontosCurva;        //--- Pontos que serão desenhados
    int nPontos(0);

    //--- Desenha o eixo

    while(IMSecaoGeomAtual != pMsecoesGeom->end() && (*IMSecaoGeomAtual).first <= (Ramo == ESQUERDO ? Curva.EstSc : Curva.EstSt))
    { 
      if(div(nPontos,DELTA).rem == 0) VPontosCurva.resize(VPontosCurva.size() + DELTA);       //--- aloca de 5 em 5 pontos

      ConvPRealPVertex(VPontosCurva[nPontos].Point,(*IMSecaoGeomAtual).second);

      nPontos++;
      IMSecaoGeomAtual++;
    }

    *result &= drawing.PolyLine(&(*VPontosCurva.begin()),nPontos);

    //--- Desenha os bordos direito e esquerdo
    //--- Calcula os pontos no bordo esquerdo e desenha a parabola cubica de 3 em 3 pontos

    for(int Lado = ESQUERDO ; Lado <= DIREITO; Lado++)
    {
      IMSecaoGeomAtual = pMsecoesGeom->find(Ramo == ESQUERDO ? Curva.EstTs : Curva.EstCs);

      nPontos = 0;
      VPontosCurva.clear();

      Ponto PLateral;
      double Largura[2];

      while(IMSecaoGeomAtual != pMsecoesGeom->end()&&(*IMSecaoGeomAtual).first <= (Ramo == ESQUERDO ? Curva.EstSc : Curva.EstSt))
      { 
        if(div(nPontos,DELTA).rem == 0) VPontosCurva.resize(VPontosCurva.size() + DELTA);       //--- aloca de 5 em 5 pontos

        SecoesTipo.BuscaLargurasPistas((*IMSecaoGeomAtual).first,Largura[ESQUERDO],Largura[DIREITO]);

        IrradiaPonto(Mat.M_PI_2,(*IMSecaoGeomAtual).second.x,(*IMSecaoGeomAtual).second.y,(*IMSecaoGeomAtual).second.fi,Largura[Lado],PLateral);

        ConvPRealPVertex(VPontosCurva[nPontos].Point,PLateral);

        nPontos++;
        IMSecaoGeomAtual++;
      }

      *result &= drawing.PolyLine(&(*VPontosCurva.begin()),nPontos);
    }
  }
}
*/
void CDGerarDXF::DesenhaEspiral(const CurHorizontal& Curva,int* result,CSecTipo& SecoesTipo,CCurHorizontais& CurvasHorizontais)
{
  for(int Ramo = CCurHorizontais::PRIMEIRO_RAMO ; Ramo <= CCurHorizontais::SEGUNDO_RAMO ; Ramo++)
  {
    //--- Desenha uma parabola cúbica de 3 em 3 pontos. A parabola cubica é MUITO proxima a espiral para os raios envolvidos.

    VENTVERTEX VPontosCurva;        //--- Pontos que serão desenhados
    int nPAtual(0);
    Ponto PontoAtual(Ramo == CCurHorizontais::PRIMEIRO_RAMO ? Curva.TS : Curva.CS);
    CEstaca EstacaAtual(Ramo == CCurHorizontais::PRIMEIRO_RAMO ? Curva.EstTs : Curva.EstCs); 
    vecPontos VecPontosEixo;

    //--- Calcula o primeiro ponto.

    PontoAtual.S = EstacaAtual.EstReal;       //--- O valor da estaca vai em S
    VecPontosEixo.push_back(PontoAtual);
    VPontosCurva.resize(5);
    ConvPRealPVertex(VPontosCurva[nPAtual].Point,PontoAtual);
    ++nPAtual;

    EstacaAtual += CalculaDeltaDes(Curva,Ramo == CCurHorizontais::PRIMEIRO_RAMO ? 0.0 : Curva.Lc);
 
    //--- Desenha o eixo

    while(EstacaAtual < (Ramo == CCurHorizontais::PRIMEIRO_RAMO ? Curva.EstSc : Curva.EstSt))
    { 
      PontoAtual.S = EstacaAtual.EstReal;       //--- O valor da estaca vai em S

      if(div(nPAtual,5).rem == 0) VPontosCurva.resize(VPontosCurva.size() + 5);       //--- aloca de 5 em 5 pontos

      if(CCurHorizontais::CalculaEspiral(Ramo,PontoAtual,EstacaAtual,Curva) == true)
      {
        VecPontosEixo.push_back(PontoAtual);
        ConvPRealPVertex(VPontosCurva[nPAtual].Point,PontoAtual);
        ++nPAtual;
      }

      CEstaca Temp = Curva.EstSt - EstacaAtual;

      EstacaAtual += CalculaDeltaDes(Curva,Ramo == CCurHorizontais::PRIMEIRO_RAMO ? (EstacaAtual.EstVirtual - Curva.EstTs.EstVirtual) : (Curva.EstSt.EstVirtual -EstacaAtual.EstVirtual)); 
    }

    //--- Calcula o último ponto do ramo;

    if(EstacaAtual >= (Ramo == CCurHorizontais::PRIMEIRO_RAMO ? Curva.EstSc : Curva.EstSt))
    {
      PontoAtual = Ramo == CCurHorizontais::PRIMEIRO_RAMO ? Curva.SC : Curva.ST;
      PontoAtual.S = Ramo == CCurHorizontais::PRIMEIRO_RAMO ? Curva.EstSc.EstReal : Curva.EstSt.EstReal;      //--- O valor da estaca vai em S
      VecPontosEixo.push_back(PontoAtual);
      if(div(nPAtual,5).rem == 0) VPontosCurva.resize(VPontosCurva.size() + 5);                       //--- aloca de 5 em 5 pontos
      ConvPRealPVertex(VPontosCurva[nPAtual].Point,PontoAtual);
      ++nPAtual;
    }
     
    *result &= drawing.PolyLine(&(*VPontosCurva.begin()),nPAtual);

    //--- Desenha os bordos direito e esquerdo
    //--- Calcula os pontos no bordo esquerdo e desenha a parabola cubica de 3 em 3 pontos

     for(int Lado = ESQUERDO ; Lado <= DIREITO; Lado++)
    {
      Ponto PLateral;
      double Largura[2];

      VPontosCurva.clear();

    //  EstacaAtual = Ramo == CCurHorizontais::PRIMEIRO_RAMO ? Curva.EstTs : Curva.EstCs; 
      PontoAtual = Ramo == CCurHorizontais::PRIMEIRO_RAMO ? Curva.TS : Curva.CS;

      for (int C = 0 ; C < nPAtual ; C++)
      {
        EstacaAtual = VecPontosEixo[C].S;                                         // O Valor da estaca ficou guardado em S
        if(div(C,5).rem == 0) VPontosCurva.resize(VPontosCurva.size() + 5);       //--- aloca de 5 em 5 pontos

        SecoesTipo.BuscaLargurasPistas(EstacaAtual,Largura[ESQUERDO],Largura[DIREITO]);
        double SL(CurvasHorizontais.BuscaSuperLargura(EstacaAtual));

        if(SL != 0.0)
        {
          double ms(Curva.Sl / Curva.Lc),SLEstaca(0.0);

          if(Ramo == CCurHorizontais::PRIMEIRO_RAMO) SLEstaca =  ms * (EstacaAtual.EstReal - Curva.EstTs.EstReal);
          else SLEstaca = ms * (Curva.EstSt.EstReal - EstacaAtual.EstReal);

          Largura[ESQUERDO] -= SLEstaca;
          Largura[DIREITO] += SLEstaca;
        }

        IrradiaPonto(Mat.M_PI_2,VecPontosEixo[C].x,VecPontosEixo[C].y,VecPontosEixo[C].fi,Largura[Lado],PLateral);
        ConvPRealPVertex(VPontosCurva[C].Point,PLateral);
      }

      *result &= drawing.PolyLine(&(*VPontosCurva.begin()),nPAtual);
    }
  }
}


void CDGerarDXF::OnSair()
{ 
  EndDialog(TRUE);
}

BOOL CDGerarDXF::OnInitDialog() 
{
  CDialog::OnInitDialog();

  UpdateData(false);

  if(!OK) m_BUGerar.EnableWindow(false);   //--- houve erro na inicialização

  CheTodosTrechos.SetCheck(BST_UNCHECKED);
  CheTrechoAtual.SetCheck(BST_CHECKED);
  
  // TODO: Add extra initialization here

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

 double CDGerarDXF::CalculaDeltaDes(const CurHorizontal& Curva,double l)
 {
   if(l < 5.0) l = 5.0;

   double Raio(Curva. Lc > 0.0 ? Curva.K / l : Curva.Raio);

 //  return Raio > 1599.9 ? 8.0 : Raio > 799.9 ? 4.0 : Raio > 399.9 ? 2.0 : Raio > 199.9 ? 1.0 : Raio > 99.9 ? 0.5 : 0.250 ;   
    return Raio > 1599.9 ? 20.0 : Raio > 799.9 ? 15.0 : Raio > 399.9 ? 10.0 : Raio > 199.9 ? 5.0 : 2.5 ;   
 }


 void CDGerarDXF::OnBnClickedChetodostrechos()
 {
   CheTodosTrechos.SetCheck(BST_CHECKED);
   CheTrechoAtual.SetCheck(BST_UNCHECKED);
 }

 void CDGerarDXF::OnBnClickedChectrechoatual()
 {
   CheTodosTrechos.SetCheck(BST_CHECKED);
   CheTrechoAtual.SetCheck(BST_UNCHECKED);
 }

