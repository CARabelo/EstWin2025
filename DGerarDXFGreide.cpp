// DGerarDXFGreide.cpp : implementation file
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
#include "supercstring.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "carqcurverticais.h"
#include "ccurverticais.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include "csecao.h"
#include "clistasecoes.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
#include "Cadlib\Interface\VC\Cadlib.h"
#include "CxCotas.h"
#include "DGerarDXFgreide.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DES_PAVIMENTO 0
#define DES_TERRAPLANAGEM 1

#define LIMESQ -INFINITO
#define LIMDIR INFINITO

/////////////////////////////////////////////////////////////////////////////
// CDGerarDXFGreide dialog

CDGerarDXFGreide::CDGerarDXFGreide(CWnd* pParent,CString& NomeProj)	: CDialog(CDGerarDXFGreide::IDD,pParent),
  CurvasVerticais(NomeProj),Secoes(NomeProj),HPavimento(NomeProj),HLimpeza(NomeProj,CString(".lim")),OK(true),NomeProjeto(NomeProj)
  , AlturaCotas(3.0),DistMinCotas(0.7),DistEntreCotas(3.7)
{
  Deltas[0] = 0.0;
  Deltas[1] = 0.0;
  Escala[0] = 1.0;
  Escala[1] = 1.0;

  if(CurvasVerticais.LCurvasVerticais.GetCount() < 2) OK = false;

  //{{AFX_DATA_INIT(CDGerarDXFGreide)
  m_CStrRazaoHV = 0.0;
  m_CStrMensagem = _T("");
  //}}AFX_DATA_INIT
}

void CDGerarDXFGreide::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDGerarDXFGreide)
  DDX_Control(pDX, IDC_GERAR, m_BUGerar);
  DDX_Control(pDX, IDC_SAIR, m_BUSair);
  DDX_Control(pDX, IDC_PROGDXFGEO, m_Progresso);
  DDX_Text(pDX, IDC_MENSGERARDXF, m_CStrMensagem);
  //}}AFX_DATA_MAP
  DDX_Text(pDX, IDC_EDIALTCOTAS, AlturaCotas);
  DDV_MinMaxDouble(pDX, AlturaCotas, 1, 20);
}

BEGIN_MESSAGE_MAP(CDGerarDXFGreide, CDialog)
  //{{AFX_MSG_MAP(CDGerarDXFGreide)
  ON_BN_CLICKED(IDC_GERAR, OnGerar)
  ON_BN_CLICKED(IDC_SAIR, OnSair)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDGerarDXFGreide message handlers

void CDGerarDXFGreide::OnGerar() 
{
  UpdateData(true);

  DistMinCotas = AlturaCotas / 15.0;
  if(DistMinCotas < 0.7) DistMinCotas = 0.7;
  DistEntreCotas = AlturaCotas + DistMinCotas;

  CString DxfFileName(NomeProjeto.GetBuffer() + CString("Greide.dxf"));

  m_BUGerar.EnableWindow(false);
  m_BUSair.EnableWindow(false);

  if(!OK)
  {
    m_CStrMensagem ="Não há curvas verticais para gerar o greide.";
    this->UpdateData(FALSE);
    return;
  }

  BOOL Erro= false;

  // Create	Drawing
  if(!drawing.Create())
  {
    m_CStrMensagem ="Erro	Fatal	Criando	o	arquivo	DXF.";
    UpdateData(FALSE);

    return;
  }

  // Tables Section ------------------------------------------
  //  LTYPE table type -------------------------
  LTYPE   ltype;
  OBJHANDLE objhandle1,objhandle2;

  //  Continuous
  ZeroMemory(&ltype, sizeof(ltype));
  strcpy(ltype.Name, "Continuous");
  strcpy(ltype.DescriptiveText, "Solid line");
  objhandle1 = drawing.AddLinetype(&ltype);

  //  DASHDOT2
  ZeroMemory(&ltype, sizeof(ltype));
  strcpy(ltype.Name, "DASHDOT2");
  strcpy(ltype.DescriptiveText, "Dash dot (.5x) _._._._._._._._._._._._._._._.");
  ltype.ElementsNumber = 4;
  ltype.PatternLength = 10.0;
  ltype.Elements[0] = 5.0;
  ltype.Elements[1] = -2.5; 
  ltype.Elements[2] = 0.0;
  ltype.Elements[3] = -2.5;
  objhandle2 = drawing.AddLinetype(&ltype);

  //  LAYER table type -------------------------
  LAYER layer;

  // Layer0 - Texto
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name,"Texto");               //--- Texto
  layer.Color = 255;
  layer.LineTypeObjhandle = objhandle1;     // Continuous
  drawing.AddLayer(&layer);
  // Layer1- Terreno
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Terreno");            //--- Terreno
  layer.Color = 30;
  layer.LineTypeObjhandle = objhandle1;     // Continuous
  drawing.AddLayer(&layer);
  // Layer2 - PontosTerreno
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "PontosTerreno");      //--- Pontos Terreno
  layer.Color = 30;
  layer.LineTypeObjhandle = objhandle1;     // Continuous
  drawing.AddLayer(&layer);
  // Layer3 - Greide
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Greide");             //--- Projeto Greide
  layer.Color = 170;
  layer.LineTypeObjhandle = objhandle1;     // Continuous
  layer.LineWeight = 50;
  drawing.AddLayer(&layer);
  //Layer4 - Malha
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Malha");             //--- Malha
  layer.Color = 84;
  layer.LineTypeObjhandle = objhandle1;     // Continuous
  drawing.AddLayer(&layer);
  // Layer5- GreideTerraplanagem
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "GreideTerraplanagem");//--- Greide Terraplanagem
  layer.Color = 170;
  layer.LineTypeObjhandle = objhandle2;     //--- Dash-Dot 
  drawing.AddLayer(&layer);
  // Layer6- Limpeza
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Limpeza");             //--- Limpeza
  layer.Color = 30;
  layer.LineTypeObjhandle = objhandle2;      // Dash-dot
  drawing.AddLayer(&layer);

  //  DIMSTYLE table type ----------------------
  DIMSTYLE  dimstyle;
  //  DIM1
  ZeroMemory(&dimstyle, sizeof(dimstyle));
  strcpy(dimstyle.Name, "DIM1");            // DimStyle Name
  dimstyle.dimclrd = 2;                     // Dimension line & Arrow heads color
  dimstyle.dimdle = 0.0000;                 // Dimension line size after Extensionline
  dimstyle.dimclre = 2;                     // Extension line color
  dimstyle.dimexe = 0.1800;                 // Extension line size after Dimline
  dimstyle.dimexo = 0.0625;                 // Offset from origin
  strcpy(dimstyle.dimblk1, "ClosedFilled"); // 1st Arrow head
  strcpy(dimstyle.dimblk2, "ClosedFilled"); // 2nd Arrow head
  dimstyle.dimasz = 0.1800;                 // Arrow size
  dimstyle.dimtxstyObjhandle = objhandle1;  // Text style
  dimstyle.dimclrt = 3;                     // Text color
  dimstyle.dimtxt = .1800;                  // Text height
  dimstyle.dimtad = 1;                      // Vertical Text Placement
  dimstyle.dimgap = 0.0900;                 // Offset from dimension line
  drawing.AddDimStyle(&dimstyle);

  //  STYLE table type -------------------------
  STYLE style;

  ZeroMemory(&style, sizeof(style));
  strcpy(style.Name, "Style1");
  strcpy(style.PrimaryFontFilename, "TIMES.TTF");
  style.LastHeightUsed = 0.2;
  style.WidthFactor = 1;
  objhandle1 = drawing.AddTextStyle(&style);

  drawing.SetTextStyle("Style1");  

  //--- Entities Section ----------------------------------------
  //---  Desenha os pontos do terreno - Layer 1

  double MenorCota(INFINITO),MenorEstaca(INFINITO),MaiorEstaca(-INFINITO);

  int Tam(Secoes.GetCount()),i(0);

  if (Tam)
  {
    m_CStrMensagem = "Desenhando o terreno...";
    UpdateData(false);
    m_Progresso.SetRange(0,Secoes.GetCount()-1);
    m_Progresso.SetStep(1);

    ENTVERTEX* VertexTerreno = (struct tag_ENTVERTEX *) new ENTVERTEX[Tam],* VertexLimpeza = (struct tag_ENTVERTEX *) new ENTVERTEX[Tam];
    ZeroMemory(VertexTerreno,sizeof(ENTVERTEX)*Tam);
    ZeroMemory(VertexLimpeza,sizeof(ENTVERTEX)*Tam);

    POSITION SecAtual(Secoes.GetHeadPosition());

    while(SecAtual)
    {
      CEstaca& EstacaAtual(Secoes.GetNext(SecAtual).Terreno.Estaca);
      m_Progresso.StepIt();

      //--- A estaca pode estar sem dados ou sem terreno.       

      if(EstacaAtual.EstVirtual == INFINITO || EstacaAtual.Cota == INFINITO) continue;  

      ConvEstRealPVertex((VertexTerreno+i)->Point,EstacaAtual);

      EstacaAtual.Cota -=  HLimpeza.BuscaH(EstacaAtual);
      ConvEstRealPVertex((VertexLimpeza+i)->Point,EstacaAtual);

      if(MenorCota > EstacaAtual.Cota) MenorCota = EstacaAtual.Cota;
      if(MenorEstaca > EstacaAtual.EstVirtual) MenorEstaca = EstacaAtual.EstVirtual;
      if(MaiorEstaca < EstacaAtual.EstVirtual) MaiorEstaca = EstacaAtual.EstVirtual;

      i++;
    }  

//    Erro &= drawing.SetLayer("Textos");
//    Erro &= drawing.Text("Vias Urbanas & Estradas - Versão 4.40 - Boole Software Ltda - Todos os Direitos Autorais Reservados - 2017", 0, 0, 3.0) > 0;

    // Erro &= drawing.PolyLine(VertexTerreno,i-1,0,1.0,1.0);    //--- tem q usar o i porque ele conta as seções válidas
    Erro &= drawing.SetLayer("Terreno");
    Erro &= drawing.PolyLine(VertexTerreno,i-1);    //--- tem q usar o i porque ele conta as seções válidas
    Erro &= drawing.SetLayer("Limpeza");
    Erro &= drawing.PolyLine(VertexLimpeza,i-1);    

    //--- Desenha os pontos de terreno e da limpeza

    Erro &= drawing.SetLayer("Limpeza");
    for (register int z = 0 ; z < i && !Erro ; z++) Erro &= drawing.Point((VertexLimpeza+z)->Point.x,(VertexLimpeza+z)->Point.y,0.0,5.0);  
    Erro &= drawing.SetLayer("PontosTerreno");
    for (register int z = 0 ; z < i && !Erro ; z++) Erro &= drawing.Point((VertexTerreno+z)->Point.x,(VertexTerreno+z)->Point.y,0.0,5.0);  

    delete[] VertexTerreno;
    delete[] VertexLimpeza;
  }
  //--- desenha o projeto - Layer 3 
  //--- Desenha as tangentes
  //****************************************
  double Desloc(0.0);
  POSITION Atual(CurvasVerticais.LCurvasVerticais.GetHeadPosition());     //--- Aponta para o PIV atual.

  if (Atual && !Erro)     //--- Se Tem Projeto e não houve Erro
  {
    Erro &= drawing.SetLayer("Greide");

    CurVertical* PIVAtual;  //--- PIV que abrigará os PIVs do projeto.

    int Tam(CurvasVerticais.LCurvasVerticais.GetCount());

    ENTVERTEX* VertexProjeto = (struct tag_ENTVERTEX *) new ENTVERTEX[Tam];
    ZeroMemory(VertexProjeto,sizeof(ENTVERTEX)*Tam);

    //--- Desenha o greide Pavimentação e terraplanagem 

    m_CStrMensagem = "Desenhando o greide...";
    UpdateData(false);    
    m_Progresso.SetRange(0,CurvasVerticais.LCurvasVerticais.GetCount()-2);       

    for (int C = 0 ; C < 2 ; C++ )
    {
      Atual = CurvasVerticais.LCurvasVerticais.GetHeadPosition();  

      Erro &= C == 1 ? drawing.SetLayer("GreideTerraplanagem") : drawing.SetLayer("Greide");

      double HPavim(C == 1 ? HPavimento.BuscaH(PIVAtual->PTV) : 0.0);  

      //--- Desenha as tangentes.

      while(Atual != CurvasVerticais.LCurvasVerticais.GetTailPosition() && !Erro)
      {
        PIVAtual = &CurvasVerticais.LCurvasVerticais.GetNext(Atual);
        CurVertical& ProximoPIV(CurvasVerticais.LCurvasVerticais.GetAt(Atual));

        Erro &= drawing.Line(PIVAtual->PTV.EstVirtual,(PIVAtual->PTV.Cota - HPavim) * 10.0, 
          ProximoPIV.PCV.EstVirtual,(ProximoPIV.PCV.Cota - HPavim) * 10.0);

        //--- Desenha as parábolas

        if(ProximoPIV.Y1 > 0.0 || ProximoPIV.Y2 > 0.0)
        {
          ENTVERTEX PontosPIV[7];
          ZeroMemory(&PontosPIV,sizeof(ENTVERTEX)*7);

          HPavim = C == 1 ? HPavimento.BuscaH(ProximoPIV.PCV) : 0.0;

          PontosPIV[0].Point.x = ProximoPIV.PCV.EstVirtual;
          PontosPIV[0].Point.y = (ProximoPIV.PCV.Cota - HPavim) * 10.0;
          PontosPIV[1].Point.x = ProximoPIV.PIV.EstVirtual;
          PontosPIV[1].Point.y = (ProximoPIV.PIV.Cota - HPavim) * 10.0;
          PontosPIV[2].Point.x = ProximoPIV.PTV.EstVirtual;
          PontosPIV[2].Point.y = (ProximoPIV.PTV.Cota - HPavim) * 10.0;

          //--- Pontos auxiliares para desenho das parabolas no DXF

          for(int z = 0 ; z < 2 ; z++)        //--- Y1
          {
            PontosPIV[3+z].Point.x = ProximoPIV.PCV.EstVirtual + (ProximoPIV.Y1 / 3.0 * (z+1));
            PontosPIV[3+z].Point.y = CurvasVerticais.CalculaCota(CEstaca(PontosPIV[3+z].Point.x));
          }

          for(int z = 0 ; z < 2 ; z++)        //--- Y2
          {
            PontosPIV[5+z].Point.x = ProximoPIV.PIV.EstVirtual + (ProximoPIV.Y2 / 3.0 * (z+1));
            PontosPIV[5+z].Point.y = CurvasVerticais.CalculaCota(CEstaca(PontosPIV[5+z].Point.x));
          }

          DesenhaParabola(PontosPIV[0],PontosPIV[1],PontosPIV[2],&PontosPIV[3]);

          drawing.Point(PontosPIV[1].Point.x,PontosPIV[1].Point.y,0.0,5.0);
        }

        m_Progresso.StepIt();
      }
    }
  }

  Atual = NULL;
  Desloc = 0.0;

  if(!Erro)
  {
    //--- Cota o desenho

    m_CStrMensagem = "cotando o desenho...";
    UpdateData(false);    
    m_Progresso.SetPos(0);
    m_Progresso.SetRange(0,Secoes.GetCount());

    //--- Prepara os locais onde as cotas serão impressas deslocando-as caso elas se sobreponham

    POSITION PrimSec(Secoes.GetHeadPosition());
    CSecao& PrimSecao(Secoes.GetNext(PrimSec)); 
    SetxCotas.insert(CxCotas(PrimSecao.Estaca.EstVirtual,PrimSecao.Estaca.EstVirtual));

    //--- Distribui as cotas nas seçoes existentes

    for(POSITION SecAtual = PrimSec ; SecAtual ; Secoes.GetNext(SecAtual))
    {    
      CSecao& SecaoAtual(Secoes.GetAt(SecAtual)); 

      double EstacaAtual(SecaoAtual.Projeto.Estaca.EstVirtual); 
      if(EstacaAtual == INFINITO) EstacaAtual = SecaoAtual.Terreno.Estaca.EstVirtual; 

      if(EstacaAtual != INFINITO) 
      {
        if (DirecaoAtual != VU_E_ACABOUESPACO) 
        {
          if (EstacaAtual < ((*SetxCotas.rbegin()).PegaxCota() + AlturaCotas + DistMinCotas))
          {
            DistribuiCotas(EstacaAtual,SetxCotas);   //--- cota sobrepos, distribui a posição
          }
          else  SetxCotas.insert(CxCotas(EstacaAtual,EstacaAtual));   //--- Não Sobrepos, insere normalmente
        }
        else SetxCotas.insert(CxCotas(EstacaAtual,EstacaAtual));   //--- Acabou o espaço, insere assim mesmo
      }
    }

    //----------------------------------------------------------------------------------------

    CString CStrRPontoBase;
    Erro &= drawing.SetLayer("Texto");
    double DeslocRef(20.0), DeslocAtual(DeslocRef);

    for(POSITION SecAtual(Secoes.GetHeadPosition()); SecAtual ; Secoes.GetNext(SecAtual))
    {    
      m_Progresso.StepIt();
      Ponto PontoAtual;
      DeslocAtual = DeslocRef;

      CSecao& SecaoAtual(Secoes.GetAt(SecAtual)); 

      double CotaTerreno(SecaoAtual.Terreno.Estaca.Cota),CotaProjeto(SecaoAtual.Projeto.Estaca.Cota), 
             hLimp(HLimpeza.BuscaH(SecaoAtual.Estaca)),CotaLimpeza(CotaTerreno-hLimp),
             PosxAtual(-INFINITO);

      ittysetxCotas itXCotaAtual(SetxCotas.end());

      if(SecaoAtual.Projeto.Estaca.EstVirtual != INFINITO || (SecaoAtual.Terreno.Estaca.EstVirtual != INFINITO)) 
      {
        itXCotaAtual = SetxCotas.find(CxCotas(SecaoAtual.Projeto.Estaca.EstVirtual,0.0));
        if(itXCotaAtual == SetxCotas.end()) itXCotaAtual = SetxCotas.find(CxCotas(SecaoAtual.Terreno.Estaca.EstVirtual,0.0));

        if(itXCotaAtual != SetxCotas.end()) 
        {
          double Local((SecaoAtual.Projeto.Estaca.Descricao != "PCV" && SecaoAtual.Projeto.Estaca.Descricao != "PTV") ?  itXCotaAtual->PegaxCota()+1.5 :   itXCotaAtual->PegaxCota()+1.5);  

          CStrRPontoBase.Format("%-5.5s",SecaoAtual.Estaca.Descricao);
          Erro &= drawing.Text(CStrRPontoBase,itXCotaAtual->PegaxCota()+1.5,(MenorCota-DeslocAtual+0.3)*10.0,AlturaCotas,90.0) > 0;
     
          CStrRPontoBase.Format("%-12.12s",SecaoAtual.Estaca.GetBuffer());
          Erro &= drawing.Text(CStrRPontoBase,itXCotaAtual->PegaxCota()+1.5,(MenorCota-DeslocAtual+2.5)*10.0,AlturaCotas,90.0) > 0;
        }

        if(itXCotaAtual == SetxCotas.end()) itXCotaAtual = SetxCotas.find(CxCotas(SecaoAtual.Terreno.Estaca.EstVirtual,0.0));

        //--- Desenha o traço (ou a chamada) que marca a estaca

        Erro &= drawing.SetLayer("Texto");

        if(itXCotaAtual != SetxCotas.end()) 
        {
          double xDeslocado(itXCotaAtual->PegaxCota());

          Erro &= drawing.Line(SecaoAtual.Estaca.EstVirtual,(MenorCota-18.38)*10.0, SecaoAtual.Estaca.EstVirtual,(MenorCota-18.22)*10.0);
          Erro &= drawing.Line(SecaoAtual.Estaca.EstVirtual,(MenorCota-18.22)*10.0, xDeslocado,(MenorCota-18.06)*10.0);
          Erro &= drawing.Line(xDeslocado,(MenorCota-18.06)*10.0,xDeslocado,(MenorCota-17.89)*10.0);

          if(!SecaoAtual.Estaca.Descricao.IsEmpty())
          {
            Erro &= drawing.Line(SecaoAtual.Estaca.EstVirtual,(MenorCota-18.38)*10.0,SecaoAtual.Estaca.EstVirtual,(MenorCota-18.55)*10.0);
            Erro &= drawing.Line(SecaoAtual.Estaca.EstVirtual,(MenorCota-18.55)*10.0, xDeslocado,(MenorCota-18.72)*10.0);
            Erro &= drawing.Line(xDeslocado,(MenorCota-18.72)*10.0, xDeslocado,(MenorCota-18.83)*10.0);
          }
        }
      }

      DeslocAtual -= 6.0;

      if(itXCotaAtual != SetxCotas.end() && CotaProjeto != INFINITO)
      {
        CStrRPontoBase.Format("%4.3lf",CotaProjeto);
        Erro &= drawing.Text(CStrRPontoBase,itXCotaAtual->PegaxCota()+1.5,(MenorCota-DeslocAtual)*10.0,AlturaCotas,90.0) > 0;
      }

      DeslocAtual -= 2.25;

      if(CotaTerreno != INFINITO)
      {
        if(itXCotaAtual == SetxCotas.end()) itXCotaAtual = SetxCotas.find(CxCotas(SecaoAtual.Terreno.Estaca.EstVirtual,0.0));
        if(itXCotaAtual != SetxCotas.end())
        {
          CStrRPontoBase.Format("% 4.3lf",CotaTerreno);
          Erro &= drawing.Text(CStrRPontoBase,itXCotaAtual->PegaxCota()+1.5,(MenorCota-DeslocAtual)*10.0,AlturaCotas,90.0) > 0;
        }
      }

      if(itXCotaAtual != SetxCotas.end())
      {

        DeslocAtual -= 2.5;

        if(CotaTerreno != INFINITO && hLimp > 0.0)
        {
          CStrRPontoBase.Format("% 4.3lf",CotaTerreno-hLimp);
          Erro &= drawing.Text(CStrRPontoBase,itXCotaAtual->PegaxCota()+1.5,(MenorCota-DeslocAtual)*10.0,AlturaCotas,90.0) > 0;
        }

        DeslocAtual -= 2.75;

        if(CotaProjeto != INFINITO && CotaTerreno != INFINITO)
        {
          CStrRPontoBase.Format("% 3.2lf",CotaProjeto-CotaTerreno-hLimp);
          Erro &= drawing.Text(CStrRPontoBase,itXCotaAtual->PegaxCota()+1.5,(MenorCota-DeslocAtual)*10.0,AlturaCotas,90.0) > 0;
        }
      }
    }

    //--- Cotas os PIVs.
    //--- Os PIVS são cotados numa faixa diferente então é preciso verificar a sobreposição nesta faixa

    POSITION PrimPIV(CurvasVerticais.LCurvasVerticais.GetHeadPosition());
    CurVertical& PrimCurva(CurvasVerticais.LCurvasVerticais.GetNext(PrimPIV)); 
    SetxCotas.insert(CxCotas(PrimCurva.PIV.EstVirtual,PrimCurva.PIV.EstVirtual));

    //--------------------------------------------------------------------------------------------------------- 
    //--- Cota  os PIVS pegando os espaços distribuidos do set

    Erro &= drawing.SetLayer("Texto");

    Atual = CurvasVerticais.LCurvasVerticais.GetHeadPosition();

    Desloc = 4.0;

    double Desloc2(25.0);

    while(Atual != NULL && !Erro)
    {
      CurVertical& PIVAtual(CurvasVerticais.LCurvasVerticais.GetNext(Atual));
      ittysetxCotas itXCotaAtualPCV(SetxCotas.end()),itXCotaAtualPIV(SetxCotas.find(CxCotas(PIVAtual.PIV.EstVirtual,0.0)));

      if(PIVAtual.Y1 > 0.0 && PIVAtual.PCV.EstVirtual != INFINITO)
      {
        itXCotaAtualPCV = SetxCotas.find(CxCotas(PIVAtual.PCV.EstVirtual,0.0));

        if(itXCotaAtualPCV != SetxCotas.end()) 
        {
          CStrRPontoBase.Format("I=% 3.4lf%%",PIVAtual.I1 * 100.0);
          Erro &= drawing.Text(CStrRPontoBase,/*PIVAtual.PCV.EstVirtual*/itXCotaAtualPCV->PegaxCota()+1.5,(MenorCota-19.0-Desloc)*10.0,AlturaCotas,90.0) > 0;

          if(itXCotaAtualPIV != SetxCotas.end())
          {
            CStrRPontoBase.Format("Y1=% 4.2lf",PIVAtual.Y1);
            Erro &= drawing.Text(CStrRPontoBase,/*PIVAtual.PIV.EstVirtual*/itXCotaAtualPIV->PegaxCota()-5.0+1.5,(MenorCota-19.0-Desloc)*10.0,AlturaCotas,90.0) > 0;
          }
        }
      }

      if(PIVAtual.PIV.Cota != INFINITO && PIVAtual.PIV.EstVirtual != INFINITO)
      { 
        if(itXCotaAtualPIV != SetxCotas.end()) 
        {
          CStrRPontoBase.Format("% 4.3lf",PIVAtual.PIV.Cota);
          Erro &= drawing.Text(CStrRPontoBase,/*PIVAtual.PIV.EstVirtual*/itXCotaAtualPIV->PegaxCota()+1.5,(MenorCota-Desloc-0.3)*10.0,AlturaCotas,90.0) > 0;
        }
      }

      if(PIVAtual.Y2 > 0.0 && PIVAtual.PIV.EstVirtual != INFINITO)  
      {
        if(itXCotaAtualPIV != SetxCotas.end()) 
        {
          CStrRPontoBase.Format("Y2=% 4.2lf",PIVAtual.Y2);
          Erro &= drawing.Text(CStrRPontoBase,itXCotaAtualPIV->PegaxCota()+5.0+1.5,(MenorCota-19.0-Desloc)*10.0,AlturaCotas,90.0) > 0;
        }
      }

      ittysetxCotas itXCotaAtualPTV(SetxCotas.find(CxCotas(PIVAtual.PTV.EstVirtual,0.0)));
      if(itXCotaAtualPTV != SetxCotas.end()) 
      {
        CStrRPontoBase.Format("I=% 3.4lf%%",PIVAtual.I2 * 100.0);
        Erro &= drawing.Text(CStrRPontoBase,itXCotaAtualPTV->PegaxCota()+1.5,(MenorCota-19.0-Desloc)*10.0,AlturaCotas,90.0) > 0;
      }    
    }

    //--- Desenha a malha das cotas

    Desloc = 20.5;
    Erro &= drawing.SetLayer("Malha");
    drawing.Line(MenorEstaca-20.0,(MenorCota-Desloc-3.0)*10.0,MaiorEstaca+1.0,(MenorCota-Desloc-3.0) * 10.0);
    Desloc -= 3.20;
    Erro &= drawing.Line(MenorEstaca-20.0,(MenorCota-Desloc-1.0)*10.0,MaiorEstaca+1.0,(MenorCota-Desloc-1.0) * 10.0);
    Desloc -= 2.75;
    Erro &= drawing.Line(MenorEstaca-20.0,(MenorCota-Desloc)*10.0,MaiorEstaca+1.0,(MenorCota-Desloc) * 10.0);
    Desloc -= 2.5;
    Erro &= drawing.Line(MenorEstaca-20.0,(MenorCota-Desloc)*10.0,MaiorEstaca+1.0,(MenorCota-Desloc) * 10.0);
    Desloc -= 2.5;
    Erro &= drawing.Line(MenorEstaca-20.0,(MenorCota-Desloc)*10.0,MaiorEstaca+1.0,(MenorCota-Desloc) * 10.0);
    Desloc -= 2.5;
    Erro &= drawing.Line(MenorEstaca-20.0,(MenorCota-Desloc)*10.0,MaiorEstaca+1.0,(MenorCota-Desloc) * 10.0);
    Desloc -= 2.5;
    Erro &= drawing.Line(MenorEstaca-20.0,(MenorCota-Desloc)*10.0,MaiorEstaca+1.0,(MenorCota-Desloc) * 10.0);
    Desloc -= 2.5;
    Erro &= drawing.Line(MenorEstaca-20.0,(MenorCota-Desloc)*10.0,MaiorEstaca+1.0,(MenorCota-Desloc) * 10.0);

    //--- Verticais

    Erro &= drawing.Line(MenorEstaca-20.0,(MenorCota-Desloc)*10.0,MenorEstaca-20.0,(MenorCota-23.5)*10.0);
    Erro &= drawing.Line(MenorEstaca-13.0,(MenorCota-Desloc)*10.0,MenorEstaca-13.0,(MenorCota-23.5)*10.0);
    Erro &= drawing.Line(MaiorEstaca-4.0,(MenorCota-Desloc)*10.0,MaiorEstaca-4.0,(MenorCota-23.5)*10.0);
  }

  std::string Nome[7] = {"GREIDE","ESTACA","PROJETO","TERRENO","LIMPEZA","C.V.","PIV"};
  double Desloc3[7] = {-2.5,-0.7,-0.3,-0.3,-0.3,0.0,0.0}; 

  for(int C= 0 ;!Erro && C < 7; C++)
  {
    Erro &= drawing.Text(Nome[C].c_str(),(MenorEstaca-15.0),(MenorCota-19.0+2.55*C+(Desloc3[C]))*10.0,AlturaCotas,90.0) > 0;
  }
  //--- Verifica se houve erro

  if(Erro)
  {
    m_CStrMensagem ="Erro gerando o arquivo DXF!";
    UpdateData(FALSE);
  }

  //--- Cabeçalho

  Erro &= drawing.SetLayer("Texto");
  Erro &= drawing.Text("Vias Urbanas e Estradas - Versão 3.00 - Boole Software Ltda - Todos os Direitos Autorais Reservados - 2024",0.0,(MenorCota-Desloc-23.0)*10.0, 5.0) > 0;

  // Salva o arquivo dxf

  if(!Erro && !drawing.SaveDXFFile(DxfFileName))
  {
    m_CStrMensagem ="Erro Salvando o arquivo DXF!";
    UpdateData(FALSE);
  }
  // Destroi CDrawing e libera a memoria alocada

  Erro &= drawing.Destroy();

  if(!Erro) m_CStrMensagem = "Arquivo DXF Gerado com sucesso!";
  else m_CStrMensagem += " -> Erro Gerando o arquivo DXF!";
  m_BUGerar.EnableWindow(false);
  m_BUSair.EnableWindow(true);

  UpdateData(FALSE);
}

void CDGerarDXFGreide::OnSair() 
{
  EndDialog(TRUE);
}

BOOL CDGerarDXFGreide::OnInitDialog() 
{
  CDialog::OnInitDialog();

  if(!OK)
  {
    m_BUGerar.EnableWindow(false);
    m_CStrMensagem = "Não há curvas verticais, o arquivo não foi gerado.";

    UpdateData(false);
  } 

  CString Titulo;

  GetWindowText(Titulo);
  Titulo.Replace("Geométrico", "Greide");
  SetWindowText(Titulo);

 char Buffer[512]={0};
 std::string NomeArquivo(NomeProjeto + ".ini");

  //--- Pega a configuração

  ::GetPrivateProfileString(_T("DadosDesDXF"),_T("Greide"),_T("3.0,3.7"),Buffer,511,NomeArquivo.c_str());
  _stscanf(Buffer,"%lf,%lf",&AlturaCotas,&DistMinCotas);

  UpdateData(FALSE);

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}

bool CDGerarDXFGreide::DesenhaParabola(ENTVERTEX PCV,ENTVERTEX PIV,ENTVERTEX PTV,PENTVERTEX Auxiliares)
{
  ENTVERTEX Pontos[9];
  PCV.Flag = 4;

  Pontos[0] = Pontos[1] = Pontos[2] =Pontos[3] = Pontos[4] = PCV;

  Pontos[1].Point.x = (PCV.Point.x + PIV.Point.x) / 2.0;
  Pontos[1].Point.y = (PCV.Point.y + PIV.Point.y) / 2.0;

  Pontos[3].Point.x = (PIV.Point.x + PTV.Point.x) / 2.0;
  Pontos[3].Point.y = (PIV.Point.y + PTV.Point.y) / 2.0;

  Pontos[2].Point.x = (Pontos[1].Point.x + Pontos[3].Point.x) / 2.0;
  Pontos[2].Point.y = (Pontos[1].Point.y + Pontos[3].Point.y) / 2.0;

  Pontos[4] = PTV;

  int Erro(drawing.Point(PCV.Point.x,PCV.Point.y,0.0,5.0));
  Erro |= drawing.Point(PTV.Point.x,PTV.Point.y,0.0,5.0);
  Erro = drawing.PolyLine((PENTVERTEX)&Pontos,5,4); 

  return Erro > 0;
}

enum { VU_E_LADOESQUERDO, VU_E_LADODIREITO, VU_E_ACABOUESPACO };

void CDGerarDXFGreide::DistribuiCotas(double XCota,tysetxCotas& SetxCotas)
{
  if(SetxCotas.find(CxCotas(XCota,0.0)) != SetxCotas.end()) return;

  bool Acabou(false);
  double XCotaRasc(XCota);

  //--- Se Acabou espaço lado esquerdo, passa a direção pro lado direito

  if (DirecaoAtual == VU_E_LADOESQUERDO && (SetxCotas.rbegin())->PegaxCota() < (LIMESQ + AlturaCotas + DistMinCotas)) DirecaoAtual = VU_E_LADODIREITO; 

  if (DirecaoAtual == VU_E_LADOESQUERDO)
  {
    //--- Arreda todas as cotas para esquerda

    ittysetxCotas it(SetxCotas.end()); 

    it--;

    do
    {
      if ((it->PegaxCota() + AlturaCotas + DistMinCotas) < XCotaRasc) Acabou = true; 
      else 
      {
        const_cast< CxCotas*>(&*it)->AlteraxCota(XCotaRasc - (AlturaCotas + DistMinCotas));//difDist;
        XCotaRasc = it->PegaxCota();
        it--;
      }
    }while(! Acabou && it != SetxCotas.begin()); 

    //--- insere a cota

    SetxCotas.insert(CxCotas(XCota,XCota));
  }
  else
  {
    double Novox(((SetxCotas.rbegin())->PegaxCota() + AlturaCotas + DistMinCotas));

    if (Novox < LIMDIR)  XCotaRasc = Novox;
    else DirecaoAtual = VU_E_ACABOUESPACO;  //---Acabou espaço lado direito

    SetxCotas.insert(CxCotas(XCota,Novox));
  }
}
 CDGerarDXFGreide::~CDGerarDXFGreide()
 {
   std::string NomeArquivo(NomeProjeto + ".ini");

   CString CSDados;

   CSDados.Format("%lf,%lf",AlturaCotas,DistMinCotas);

   ::WritePrivateProfileString(_T("DadosDesDXF"),_T("Greide"),CSDados,NomeArquivo.c_str());	
 }