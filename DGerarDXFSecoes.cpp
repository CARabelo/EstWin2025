// DGerarDXFSecoes.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "cponto.h"
#include "clpontos.h"
#include "estaca.h"
#include "perfil.h"
#include "supercstring.h"
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
#include <list>
#include "csecao.h"
#include "clistaperfis.h"
#include "clistasecoes.h"
#include "Cadlib\Interface\VC\Cadlib.h"
#include <string>
#include <set>
#include "CTipoSolos.h"
#include "CArqTiposSolos.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include "CArqFurosSondagem.h"
#include <algorithm>
#include "CPerfilGeologico.h"
#include "CxCotas.h"
#include "CSecaoAvulsa.h"
#include <map>
#include "CSecoesAvulsas.h"
#include "DGerarDXFSecoes.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ESPACOX  10.0
#define ALTURA   50.0
#define LIMESQ -INFINITO
#define LIMDIR INFINITO
#define LER 0

/////////////////////////////////////////////////////////////////////////////
// CDGerarDXFSecoes dialog

CDGerarDXFSecoes::CDGerarDXFSecoes(CWnd* pParent,CString& NomeProj)	: CDialog(CDGerarDXFSecoes::IDD,pParent),Secoes(NomeProj),NomeProjeto(NomeProj),
  MenorCota(INFINITO),MaiorCota(-INFINITO),MenorAfast(INFINITO),MaiorAfast(-INFINITO),m_ChePerfisGeologicos(true), AlturaCotas(3.0),DistMinCotas(0.15),DistEntreCotas(0.37),
  SecoesAvulsasOrig(NomeProj)
{
}

void CDGerarDXFSecoes::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GERAR, m_BUGerar);
  DDX_Control(pDX, IDC_SAIR, m_BUSair);
  DDX_Control(pDX, IDC_PROGDXFGEO, m_ProgProgresso);
  DDX_Text(pDX, IDC_MENSGERARDXF, m_CStrMensagem);
  DDX_Check(pDX, IDC_CHEPERFGEOLOGICO, m_ChePerfisGeologicos);
  DDX_Text(pDX, IDC_EDIALTCOTAS, AlturaCotas);
	DDV_MinMaxDouble(pDX, AlturaCotas, 1, 30);
}

BEGIN_MESSAGE_MAP(CDGerarDXFSecoes, CDialog)
  ON_BN_CLICKED(IDC_GERAR, OnGerarSecoes)
  ON_BN_CLICKED(IDC_SAIR, OnSair)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDGerarDXFSecoes message handlers

void CDGerarDXFSecoes::OnGerarSecoes()
{
  CString DxfFileName(NomeProjeto.c_str() + CString("Secoes.dxf"));

  BOOL Erro(false);

  // Create Drawing
  if (!drawing.Create())
  {
    m_CStrMensagem = "Erro Fatal Criando o arquivo DXF.";
    this->UpdateData(FALSE);
    return;
  }

  m_BUGerar.EnableWindow(false);
  m_BUSair.EnableWindow(false);

  UpdateData(true);

  AltCotas = AlturaCotas / 10.0;

  DistMinCotas = AltCotas / 15.0;
  if (DistMinCotas < 0.15) DistMinCotas = 0.15;
  DistEntreCotas = AltCotas + DistMinCotas;

  // Tables Section ------------------------------------------
  //  LTYPE table type -------------------------
  LTYPE		ltype;
  OBJHANDLE	objhandle1, objhandle2, objhandle3;

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
  ltype.PatternLength = 0.5;
  ltype.Elements[0] = 0.25;
  ltype.Elements[1] = -0.125;
  ltype.Elements[2] = 0.0;
  ltype.Elements[3] = -0.125;
  objhandle2 = drawing.AddLinetype(&ltype);
  //  DASHDOTDOT
  ZeroMemory(&ltype, sizeof(ltype));
  strcpy(ltype.Name, "DASHDOTDOT");
  strcpy(ltype.DescriptiveText, "Dash dotdot (.5x) _.._.._.._.._.._.._.._.._.._.._.._.._.._.._..");
  ltype.ElementsNumber = 6;
  ltype.PatternLength = 0.5;
  ltype.Elements[0] = 0.20;
  ltype.Elements[1] = -0.1;
  ltype.Elements[2] = 0.0;
  ltype.Elements[3] = -0.1;
  ltype.Elements[4] = 0.0;
  ltype.Elements[5] = -0.1;
  objhandle3 = drawing.AddLinetype(&ltype);

  //  LAYER table type -------------------------
  LAYER	layer;
  // Texto
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Texto");
  layer.Color = 255;
  layer.LineTypeObjhandle = objhandle1;
  drawing.AddLayer(&layer);
  // Terreno
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Terreno");
  layer.Color = 30;
  layer.LineTypeObjhandle = objhandle1;
  drawing.AddLayer(&layer);
  // Pontos Terreno
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "PontosTerreno");
  layer.Color = 30;
  layer.LineTypeObjhandle = objhandle1;
  drawing.AddLayer(&layer);
  // Projeto
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Projeto");
  layer.Color = 10;
  layer.LineTypeObjhandle = objhandle1;
  drawing.AddLayer(&layer);
  // Limpeza
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Limpeza");
  layer.Color = 30;
  layer.LineTypeObjhandle = objhandle2;
  drawing.AddLayer(&layer);
  // Pavimento
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Pavimento");
  layer.Color = 160;
  layer.LineTypeObjhandle = objhandle1;
  drawing.AddLayer(&layer);
  // Pontos Pavimento
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "PontosPavimento");
  layer.Color = 160;
  layer.LineTypeObjhandle = objhandle1;
  drawing.AddLayer(&layer);

  //--- Layers dos perfís ecologicos
  {
    CArqTiposSolos ArqTiposSolos(NomeProjeto.c_str());
    tysetCTipoSolo SetTiposSolos;
    ArqTiposSolos.CriaSet(SetTiposSolos);
    ArqTiposSolos.ConverteGDI(SetTiposSolos);

    LAYER* LayersPerfis(new LAYER[SetTiposSolos.size() + 1]);
    ZeroMemory(LayersPerfis, sizeof(LAYER) * SetTiposSolos.size());

    size_t C(0), QtdPerfis(SetTiposSolos.size());
    unsigned int TabCores[10] = { 1,2,3,4,5,6,30,50,70,90 };

    for (itsetCTipoSolo itTipoSoloAtual = SetTiposSolos.begin(); itTipoSoloAtual != SetTiposSolos.end(); ++itTipoSoloAtual, C++)
    {
      strcpy((LayersPerfis + C)->Name, RemoveBrancos(const_cast<CTipoSolo*>(&*itTipoSoloAtual)->ListaCampos.begin()->c_str()).c_str());     //--- Nome do tipo de solo
      (LayersPerfis + C)->Color = TabCores[C % 10];                //--- itTipoSoloAtual->Cor : autocad Não tem RGB
      (LayersPerfis + C)->LineTypeObjhandle = objhandle1;           //--- Continuous
      drawing.AddLayer(LayersPerfis + C);
    }

    strcpy((LayersPerfis + C)->Name, "Indefinido");
    (LayersPerfis + C)->Color = 1;
    (LayersPerfis + C)->LineTypeObjhandle = objhandle1;
    drawing.AddLayer(LayersPerfis + C);
  }

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
  //  STYLE table type -------------------------
  STYLE	style;
  ZeroMemory(&style, sizeof(style));
  strcpy(style.Name, "Style1");
  strcpy(style.PrimaryFontFilename, "TIMES.TTF");
  style.LastHeightUsed = 0.3;
  style.WidthFactor = 1;
  objhandle1 = drawing.AddTextStyle(&style);
  drawing.SetTextStyle("Style1");

  //--- Desenha as secoes 

  m_CStrMensagem = "Desenhando as Seções...";
  UpdateData(false);
  int ContaSecoes(0);
  int LadoDesenho((sqrt((double)Secoes.GetCount()) + 0.5));
  Ponto OffSetXY(0.0, 0, 0);

  CPerfilGeologico PerfilGeologico(NomeProjeto);            //--- Perfís geológicos do terreno
  PerfilGeologico.Serialize(NomeProjeto, LER);
  PerfilGeologico.CriaSetitDadosPerfisDeltah();            //--- Para pegar os perfis geológicos na ordem certa, usado na impressão das cotas

  POSITION SecAtual(Secoes.GetHeadPosition());

  PassaSecoesAvulsasOriSecoes();

  for (int C = 0; C < 2; C++)
  {
    m_ProgProgresso.SetRange(0, C == 0 ? Secoes.GetCount() : SecoesAvulsas.GetCount());
    m_ProgProgresso.SetStep(1);

    POSITION SecAtual(C == 0 ? Secoes.GetHeadPosition() : SecoesAvulsas.GetHeadPosition());

    while (SecAtual && !Erro)
    {
      CSecao& SecaoAtual(C == 0 ? Secoes.GetNext(SecAtual) : SecoesAvulsas.GetNext(SecAtual));
      CEstaca& EstacaAtual(SecaoAtual.Terreno.Estaca);

      m_ProgProgresso.StepIt();

      //--- A estaca pode estar sem dados ou sem terreno.       

      if (C == 0 && (EstacaAtual.EstVirtual == INFINITO || EstacaAtual.Cota == INFINITO)) continue;

      //--- PAjuste contém os dados para ajuste do desenho.
      //--- em x ficara a cota máxima em t a cota minima e em z a cota média

      Ponto PAjuste(CentralizaSecao(SecaoAtual));

      //--- Desenha o ponto base.

      //Erro &= drawing.Point(OffSetXY.x, OffSetXY.y + PAjuste.y, 0.0, 5.0);

      //--- Projeto

      {
        int Tam = SecaoAtual.Projeto.GetCount();
        ENTVERTEX* VertexProjeto(new ENTVERTEX[Tam]);
        ZeroMemory(VertexProjeto, sizeof(ENTVERTEX)*Tam);
        int C(0);

        Erro &= drawing.SetLayer("Projeto");

        for (POSITION PontoAtual = SecaoAtual.Projeto.GetHeadPosition(); PontoAtual; SecaoAtual.Projeto.GetNext(PontoAtual), C++)
        {
          SeparaMenores(SecaoAtual.Projeto.GetAt(PontoAtual));
          ConvPontoPVertex((VertexProjeto + C)->Point, SecaoAtual.Projeto.GetAt(PontoAtual), OffSetXY, 0.0);
        }

        Erro &= drawing.PolyLine(VertexProjeto, C);

        //--- Desenha os pontos de Projeto

        for (int z = 0; z < C && !Erro; z++)  Erro &= drawing.Point((VertexProjeto + z)->Point.x, (VertexProjeto + z)->Point.y, 0.0, 5.0);

        delete[] VertexProjeto;
      }

      //--- Pavimento

      {
        int Tam(SecaoAtual.Pavimento.GetCount());
        ENTVERTEX* VertexPavimento(new ENTVERTEX[Tam]);
        ZeroMemory(VertexPavimento, sizeof(ENTVERTEX)*Tam);
        int C(0);

        Erro &= drawing.SetLayer("Pavimento");

        for (POSITION PontoAtual = SecaoAtual.Pavimento.GetHeadPosition(); PontoAtual; SecaoAtual.Pavimento.GetNext(PontoAtual), C++)
        {
          SeparaMenores(SecaoAtual.Projeto.GetAt(PontoAtual));
          ConvPontoPVertex((VertexPavimento + C)->Point, SecaoAtual.Pavimento.GetAt(PontoAtual), OffSetXY, 0.0);
        }

        Erro &= drawing.PolyLine(VertexPavimento, C);

        //--- Desenha os pontos de Pavimento

        for (int z = 0; z < C && !Erro; z++) Erro &= drawing.Point((VertexPavimento + z)->Point.x, (VertexPavimento + z)->Point.y, 0.0, 5.0);

        delete[] VertexPavimento;
      }

      //--- Terreno

      {
        int Tam(SecaoAtual.Terreno.GetCount());
        ENTVERTEX* VertexTerreno(new ENTVERTEX[Tam]);
        ZeroMemory(VertexTerreno, sizeof(ENTVERTEX)*Tam);
        int C(0);

        Erro &= drawing.SetLayer("Terreno");

        for (POSITION PontoAtual = SecaoAtual.Terreno.GetHeadPosition(); PontoAtual; SecaoAtual.Terreno.GetNext(PontoAtual), C++)
        {
          SeparaMenores(SecaoAtual.Terreno.GetAt(PontoAtual));
          ConvPontoPVertex((VertexTerreno + C)->Point, SecaoAtual.Terreno.GetAt(PontoAtual), OffSetXY, 0.0);
        }

        Erro &= drawing.PolyLine(VertexTerreno, C);

        //--- Desenha os pontos de terreno

        for (int z = 0; z < C && !Erro; z++) Erro &= drawing.Point((VertexTerreno + z)->Point.x, (VertexTerreno + z)->Point.y, 0.0, 5.0);

        delete[] VertexTerreno;
      }

      //--- Limpeza

      {
        int Tam(SecaoAtual.Limpeza.GetCount());
        ENTVERTEX* VertexTerreno(new ENTVERTEX[Tam]);
        ZeroMemory(VertexTerreno, sizeof(ENTVERTEX)*Tam);
        int C(0);

        Erro &= drawing.SetLayer("Limpeza");

        for (POSITION PontoAtual = SecaoAtual.Limpeza.GetHeadPosition(); PontoAtual; SecaoAtual.Limpeza.GetNext(PontoAtual), C++)
        {
          SeparaMenores(SecaoAtual.Projeto.GetAt(PontoAtual));
          ConvPontoPVertex((VertexTerreno + C)->Point, SecaoAtual.Limpeza.GetAt(PontoAtual), OffSetXY, 0.0);
        }

        Erro &= drawing.PolyLine(VertexTerreno, C);

        //--- Desenha os pontos de limpeza

        for (int z = 0; z < C && !Erro; z++) Erro &= drawing.Point((VertexTerreno + z)->Point.x, (VertexTerreno + z)->Point.y, 0.0, 5.0);

        delete[] VertexTerreno;
      }

      //--- Perfís Geológicos

      if (m_ChePerfisGeologicos)
      {
        itsetCDadosPerfGeologicos itPerfGeo(PerfilGeologico.PegaSetSecaoPerfGeo().upper_bound(CDadosPerfisGeologicos("", EstacaAtual.EstVirtual)));

    
          while (itPerfGeo != PerfilGeologico.PegaSetSecaoPerfGeo().end() && itPerfGeo->PegaEstaca() == EstacaAtual.EstVirtual)
          {
            const Perfil& PerfGeolAtual(itPerfGeo->PegaPerfilGeologico());
            int Tam(PerfGeolAtual.GetCount());
            ENTVERTEX* VertexPerfGeologico(new ENTVERTEX[Tam]);
            ZeroMemory(VertexPerfGeologico, sizeof(ENTVERTEX)*Tam);
            int C(0);

            if (drawing.SetLayer(RemoveBrancos(itPerfGeo->PegaNomeTipoSolo().c_str()).c_str()) == FALSE)
              Erro &= drawing.SetLayer("Indefinido");

            for (POSITION PontoAtual = PerfGeolAtual.GetHeadPosition(); PontoAtual; PerfGeolAtual.GetNext(PontoAtual), C++)
            {
              SeparaMenores(PerfGeolAtual.GetAt(PontoAtual));
              ConvPontoPVertex((VertexPerfGeologico + C)->Point, PerfGeolAtual.GetAt(PontoAtual), OffSetXY, 0.0);
            }

            Erro &= drawing.PolyLine(VertexPerfGeologico, C);

            //--- Desenha os pontos 

            for (int z = 0; z < C && !Erro; z++) Erro &= drawing.Point((VertexPerfGeologico + z)->Point.x, (VertexPerfGeologico + z)->Point.y, 0.0, 5.0);

            delete[] VertexPerfGeologico;

            ++itPerfGeo;
          }
      }

      MenorCota -= 3.0;

      //--- Cota a seção 

      Erro &= drawing.SetLayer("Texto");

      std::string Titulo(C == 0 ? EstacaAtual.GetBuffer() : "                                                        ");
      Titulo += "  " + SecaoAtual.Estaca.Descricao;

    Erro &= drawing.Text(Titulo.c_str(),OffSetXY.x,OffSetXY.y+(ALTURA/2.0)+PAjuste.y-9.0,.50) > 0;
    CString CStrRPontoBase;
   //  CStrRPontoBase.Format("OffSet = {x=%.3lf, y=%.3lf}",OffSetXY.x,OffSetXY.y);
   // Erro &= drawing.Text(CStrRPontoBase,OffSetXY.x,OffSetXY.y+(ALTURA/2.0)+PAjuste.y-9.7,.30) > 0;

      ENTVERTEX P1, P2;
      double Desloc(1.0);
      Perfil PerfilCVS;

      //--- Calcula as CVs

      if (SecaoAtual.Projeto.GetHeadPosition() != NULL)
      {
        for (POSITION Atual = SecaoAtual.Projeto.GetHeadPosition(); !Erro && Atual; SecaoAtual.Projeto.GetNext(Atual))
        {
          double px(SecaoAtual.Projeto.GetAt(Atual).x);
          double py(SecaoAtual.Projeto.GetAt(Atual).y);
          double CotaTerreno(CalcCotaTerreno(px, SecaoAtual.Limpeza.GetCount() == 0 ? &SecaoAtual.Terreno : &SecaoAtual.Limpeza));

          if (py != INFINITO && CotaTerreno != INFINITO)
          {
            PerfilCVS.AddTail(Ponto(px, -(py - CotaTerreno), 0.0));
          }
        }
      }

      enum Tipos { TIPOTERRENO, TIPOLIMPEZA, TIPOCV, TIPOPROJETO, TIPOPAVIMENTACAO, FIMTIPO };
      POSITION Atual[FIMTIPO] = { SecaoAtual.Terreno.GetHeadPosition(),SecaoAtual.Limpeza.GetHeadPosition(),PerfilCVS.GetHeadPosition(),
                                  SecaoAtual.Projeto.GetHeadPosition(),SecaoAtual.Pavimento.GetHeadPosition() };
      Perfil* Perfis[FIMTIPO] = { &SecaoAtual.Terreno,&SecaoAtual.Limpeza,&PerfilCVS,&SecaoAtual.Projeto,&SecaoAtual.Pavimento };
      CString NomesStr[FIMTIPO] = { "      TERRENO","     LIMPEZA","         CV","      PROJETO","   PAVIMENTO" };

      tylstPOSITION LstPOSITION(Atual, Atual + FIMTIPO);
      tylstpPerfil LstPerfis(Perfis, Perfis + FIMTIPO);
      tylststdcstr LstNomes(NomesStr, NomesStr + FIMTIPO);

      int QtdPerfis(FIMTIPO);

      if (m_ChePerfisGeologicos)
      {
        itsetCDadosPerfGeologicos itPerfGeoPerfil(PerfilGeologico.PegaSetSecaoPerfGeo().upper_bound(CDadosPerfisGeologicos("", EstacaAtual.EstVirtual)));

        if (itPerfGeoPerfil != PerfilGeologico.PegaSetSecaoPerfGeo().end())
        {
          setitsetCDadosPerfGeoDeltah sit(PerfilGeologico.PegaSetItSecaoPerfGeoDeltah());

          itsetitsetCDadosPerfGeoDeltah itPerfGeo(sit.find(itPerfGeoPerfil));

          //--- Um set esta ordenado por nome do perfil, o outro por deltah.
          //--- A ordem nos sets é diferente, tem q posicionar no primeiro perfil desta estaca, cujo deltah = 0.0;
          //--- Retrocede até o primeiro perfil desta estaca.

          while (itPerfGeo != sit.end() && (*itPerfGeo)->PegaEstaca() == EstacaAtual.EstVirtual) --itPerfGeo;

          if (itPerfGeo != sit.end()) ++itPerfGeo;
          else itPerfGeo = sit.begin();

          while (itPerfGeo != sit.end() && (*itPerfGeo)->PegaEstaca() == EstacaAtual.EstVirtual)
          {
            if ((*itPerfGeo)->PegaPerfilGeologico().GetCount() > 2)
            {
              LstPOSITION.push_back((*itPerfGeo)->PegaPerfilGeologico().GetHeadPosition());
              LstPerfis.push_back(&(*itPerfGeo)->PegaPerfilGeologico());
              LstNomes.push_back((*itPerfGeo)->PegaNomeTipoSolo().substr(0, 19).c_str());
            }

            ++QtdPerfis;
            ++itPerfGeo;
          }
        }
      }

      ittylstpPerfil ItLstPerfilAtual(LstPerfis.begin());

      //--- cota a secao

      for (ittylstPOSITION itPosAtual = LstPOSITION.begin(); itPosAtual != LstPOSITION.end(); ++itPosAtual)
      {
        if ((*ItLstPerfilAtual)->GetSize() > 2)
        {
          //--- Prepara os locais onde as cotas serão impressas deslocando-as caso elas se sobreponham

          POSITION AtualRasc(*itPosAtual);
          SetxCotas.clear();
          SetxCotas.insert(CxCotas((*ItLstPerfilAtual)->GetAt(AtualRasc).x, (*ItLstPerfilAtual)->GetAt(AtualRasc).x));

          DirecaoAtual = VU_E_LADOESQUERDO;

          //--- Distribui as cotas na seção

          for (Ponto PontoAtual = (*ItLstPerfilAtual)->GetNext(AtualRasc); !Erro && AtualRasc != NULL; (*ItLstPerfilAtual)->GetNext(AtualRasc))
          {
            PontoAtual = (*ItLstPerfilAtual)->GetAt(AtualRasc);

            if (DirecaoAtual != VU_E_ACABOUESPACO)
            {
              if (PontoAtual.x < ((*SetxCotas.rbegin()).PegaxCota() + AltCotas + DistMinCotas))
              {
                DistribuiCotas(PontoAtual.x, SetxCotas);   //--- cota sobrepos, distribui a posição
              }
              else  SetxCotas.insert(CxCotas(PontoAtual.x, PontoAtual.x));       //--- Não Sobrepos, insere normalmente
            }
            else SetxCotas.insert(CxCotas(PontoAtual.x, PontoAtual.x));          //--- Acabou o espaço, insere assim mesmo
          }

          const double MeiaAlturaLetra(AltCotas / 2.0);

          for (; !Erro && *itPosAtual; (*ItLstPerfilAtual)->GetNext(*itPosAtual))
          {
            Ponto PontoAtual2((*ItLstPerfilAtual)->GetAt(*itPosAtual));
            ittysetxCotas itXCotaAtual(SetxCotas.find(CxCotas(PontoAtual2.x, 0.0)));

            if (itXCotaAtual != SetxCotas.end())
            {
              CStrRPontoBase.Format("% 3.2lf    % 4.3lf", PontoAtual2.x, PontoAtual2.y);
              ConvPontoPVertex(P1.Point, Ponto(itXCotaAtual->PegaxCota(), MenorCota - Desloc), OffSetXY, 0.0);

              Erro &= drawing.Text(CStrRPontoBase, P1.Point.x + MeiaAlturaLetra, P1.Point.y - 3.2, .30, 90.0) > 0;

              if (itXCotaAtual != SetxCotas.end()) Erro &= DesenhaMarca(P1, itXCotaAtual->PegaEstaca() + OffSetXY.x, 0.0);

              if (itXCotaAtual->PegaxCota() < MenorAfast)  MenorAfast = itXCotaAtual->PegaxCota() - AltCotas * 2.0;
              if (itXCotaAtual->PegaxCota() > MaiorAfast) MaiorAfast = itXCotaAtual->PegaxCota() - AltCotas * 2.0;
            }
          }
        }
        //--- Desenha o gabarito da cotas

        ConvPontoPVertex(P1.Point, Ponto(MenorAfast - 2.0, MenorCota - Desloc + .50), OffSetXY, 0.0);
        ConvPontoPVertex(P2.Point, Ponto(MaiorAfast + 1.0, MenorCota - Desloc + .50), OffSetXY, 0.0);

        drawing.Line(P1.Point.x, P1.Point.y, P2.Point.x, P1.Point.y);

        Desloc += 4.0;

        ++ItLstPerfilAtual;
      }

      //--- Escreve o título da faixa

      double Dh(LstNomes.size() * -4.0);

      for (ittylststdcstr ItLstNomeAtual = LstNomes.begin(); ItLstNomeAtual != LstNomes.end(); ++ItLstNomeAtual)
      {
        CStrRPontoBase = *ItLstNomeAtual;
        ConvPontoPVertex(P1.Point, Ponto(MenorAfast - 1.25, MenorCota - Desloc - Dh), OffSetXY, 0.0);
        Erro &= drawing.Text(CStrRPontoBase, P1.Point.x, P1.Point.y - 3.3, .30, 90.0) > 0;

        Dh += 4.00;
      }

      //--- Fecha o gabarito das cotas

      ConvPontoPVertex(P1.Point, Ponto(MenorAfast - 2.0, MenorCota - Desloc + .50), OffSetXY, 0.0);
      ConvPontoPVertex(P2.Point, Ponto(MaiorAfast + 1.0, MenorCota - Desloc + .50), OffSetXY, 0.0);

      drawing.Line(P1.Point.x, P1.Point.y, P2.Point.x, P2.Point.y);

      ConvPontoPVertex(P1.Point, Ponto(MenorAfast - 2.0, MenorCota - 1.0 + .50), OffSetXY, 0.0);
      ConvPontoPVertex(P2.Point, Ponto(MenorAfast - 2.0, MenorCota - Desloc + .50), OffSetXY, 0.0);

      drawing.Line(P1.Point.x, P1.Point.y, P2.Point.x, P2.Point.y);

      ConvPontoPVertex(P1.Point, Ponto(MaiorAfast + 1.0, MenorCota - 1.0 + .50), OffSetXY, 0.0);
      ConvPontoPVertex(P2.Point, Ponto(MaiorAfast + 1.0, MenorCota - Desloc + .50), OffSetXY, 0.0);

      drawing.Line(P1.Point.x, P1.Point.y, P2.Point.x, P2.Point.y);

      ConvPontoPVertex(P1.Point, Ponto(MenorAfast - 1.0, MenorCota - 1.0 + .50), OffSetXY, 0.0);
      ConvPontoPVertex(P2.Point, Ponto(MenorAfast - 1.0, MenorCota - Desloc + .50), OffSetXY, 0.0);

      drawing.Line(P1.Point.x, P1.Point.y, P2.Point.x, P2.Point.y);

      //--- Passa para a próxima seção

      //   if (++ContaSecoes % LadoDesenho == 0)      //--- passa a linha de seções - para desenhar as seções em linhas
      if (0)
      {
        OffSetXY.y += ALTURA;
        OffSetXY.x = 0.0;
      }
      else
        //  OffSetXY.x += Secoes.PegaEquidisLateral() * 2.0 + ESPACOX;
        OffSetXY.x += (MaiorAfast - MenorAfast) + ESPACOX;

      MenorCota = MenorAfast = INFINITO;
      MaiorCota = MaiorAfast = -INFINITO;
    }
}
  //--- Verifica se houve erro

  if(Erro)
  {
    m_CStrMensagem ="Erro gerando o arquivo DXF!";
    UpdateData(FALSE);
  }

  //--- Cabeçalho

  Erro &= drawing.Text("Vias Urbanas & Estradas - Versão 4.40 - Boole Software Ltda - Todos os Direitos Autorais Reservados - 2017",0.0,160.0, 4.0) > 0;

  // Salva o arquivo dxf

  if(!Erro && !drawing.SaveDXFFile(DxfFileName))
  {
    m_CStrMensagem ="Erro Salvando o arquivo DXF!";
    UpdateData(FALSE);

    m_BUSair.EnableWindow(true);
  }

  // Destroi CDrawing e libera a memoria alocada

  Erro &= drawing.Destroy();

  if(!Erro)	m_CStrMensagem = "Arquivo DXF Gerado com sucesso!";
  else m_CStrMensagem += " -> Erro Gerando o arquivo DXF!";

  m_BUGerar.EnableWindow(false);
  m_BUSair.EnableWindow(true);

  UpdateData(FALSE);
}

void CDGerarDXFSecoes::OnSair() 
{
  EndDialog(TRUE);	
}

Ponto CDGerarDXFSecoes::CentralizaSecao(CSecao& SecaoAtual) 
{
  Ponto PResposta(-INFINITO,INFINITO,INFINITO);  //---x-> Y superior,y->Y inferior e  Z-> cota Média 

  if(SecaoAtual.Projeto.GetCount())
  {
    //--- A seção é centralizada em relação ao projeto ou ao terreno, projeto tem preferência
    //--- Separa maior e a menor cota de projeto. o deslocamento é essa diferença / 2  

    for (POSITION PAtual=SecaoAtual.Projeto.GetHeadPosition() ; PAtual ; SecaoAtual.Projeto.GetNext(PAtual))
    {
      Ponto& PontoAtual = SecaoAtual.Projeto.GetAt(PAtual);

      if (PontoAtual.y > PResposta.x) PResposta.x = PontoAtual.y;
      if (PontoAtual.y < PResposta.y) PResposta.y = PontoAtual.y;
    }

    PResposta.z = (PResposta.x + PResposta.y) / 2.0;
  }
  else
  {
    if(SecaoAtual.Terreno.GetCount())
    {
      for (POSITION PAtual=SecaoAtual.Terreno.GetHeadPosition() ; PAtual ; SecaoAtual.Terreno.GetNext(PAtual))
      {
        Ponto& PontoAtual = SecaoAtual.Terreno.GetAt(PAtual);

        if (PontoAtual.y > PResposta.x) PResposta.x = PontoAtual.y;
        if (PontoAtual.y < PResposta.y) PResposta.y = PontoAtual.y;
      }
    } 
    else PResposta.x = PResposta.y = 0.0;

    PResposta.z = (PResposta.x + PResposta.y) / 2.0;
  }

  return PResposta;
}

BOOL CDGerarDXFSecoes::OnInitDialog() 
{
  CDialog::OnInitDialog();

  if(Secoes.GetCount() == 0) 
  {
    m_CStrMensagem = "Não há seções, o arquivo não foi gerado.";
    UpdateData(false);

    m_BUGerar.EnableWindow(false);
  }

  CString Titulo;

  GetWindowText(Titulo);
  Titulo.Replace("Geométrico", "Seções");
  SetWindowText(Titulo);

  UpdateData(true);

  char Buffer[512]={0};
 std::string NomeArquivo(NomeProjeto + ".ini");

  //--- Pega a configuração

  ::GetPrivateProfileString(_T("DadosDesDXF"),_T("Secoes"),_T("3.0,3.7"),Buffer,511,NomeArquivo.c_str());
  _stscanf(Buffer,"%lf,%lf",&AlturaCotas,&DistMinCotas);
   
  UpdateData(FALSE);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

double CDGerarDXFSecoes::CalcCotaTerreno(double x, Perfil* PerfTerreno)
{
  PerfTerreno->PosicionaNoMaisProximo(x);

  if (!PerfTerreno->PegaAtual()) return INFINITO;

  const Ponto& P(PerfTerreno->GetAt(PerfTerreno->PegaAtual()));

  if ((PerfTerreno->PegaAtual() == PerfTerreno->GetHeadPosition() && x < P.x) || (PerfTerreno->PegaAtual() == PerfTerreno->GetTailPosition() && x > P.x)) return INFINITO;

  POSITION Atual(PerfTerreno->PegaAtual());

  if (!Atual) return INFINITO;

  Ponto PCalc;

  if (x > P.x)
  {
    PerfTerreno->GetNext(Atual);
    const Ponto& PProx = PerfTerreno->GetAt(Atual);
    PCalc.i = (PProx.y - P.y) / (PProx.x - P.x);
  }
  else
  {
    PerfTerreno->GetPrev(Atual);
    const Ponto& PAnte = PerfTerreno->GetAt(Atual);
    PCalc.i = (PAnte.y - P.y) / (PAnte.x - P.x);
  }

  return PCalc.i * ( x - P.x) + P.y;  
}

void CDGerarDXFSecoes::DistribuiCotas(double XCota,tysetxCotas& SetxCotas)
{
  bool Acabou(false);
  double XCotaRasc(XCota);

  //--- Se Acabou espaço lado esquerdo, passa a direção pro lado direito

  if (DirecaoAtual == VU_E_LADOESQUERDO && (SetxCotas.rbegin())->PegaxCota() < (LIMESQ + AltCotas + DistMinCotas)) DirecaoAtual = VU_E_LADODIREITO; 

  if (DirecaoAtual == VU_E_LADOESQUERDO)
  {
    //--- Arreda todas as cotas para esquerda

    ittysetxCotas it(SetxCotas.end()); 

    if (SetxCotas.size() > 0)
    {
      it--;

      do
      {
        if ((it->PegaxCota() + AltCotas + DistMinCotas) < XCotaRasc) Acabou = true;
        else
        {
          const_cast<CxCotas*>(&*it)->AlteraxCota(XCotaRasc - AltCotas - DistMinCotas);//difDist;
          XCotaRasc = it->PegaxCota();
          it--;
        }
      } while (!Acabou &&  it != SetxCotas.end() && it != SetxCotas.begin());
    }

    //--- insere a cota

    SetxCotas.insert(CxCotas(XCota,XCota));
  }
  else
  {
    double Novox(((SetxCotas.rbegin())->PegaxCota() + AltCotas + DistMinCotas));

    if (Novox < LIMDIR)  XCotaRasc = Novox;
    else DirecaoAtual = VU_E_ACABOUESPACO;  //---Acabou espaço lado direito

    SetxCotas.insert(CxCotas(XCota,Novox));
  }
}

int CDGerarDXFSecoes::DesenhaMarca(ENTVERTEX P1,double XCota,double MeioTamLetra)
{
  int Erro(0);

  Erro &= drawing.Line(P1.Point.x-MeioTamLetra,P1.Point.y, P1.Point.x-MeioTamLetra,P1.Point.y+0.1);
  Erro &= drawing.Line(P1.Point.x-MeioTamLetra,P1.Point.y+0.1, XCota,P1.Point.y+0.4);
  Erro &= drawing.Line(XCota,P1.Point.y+0.4,XCota,P1.Point.y+0.7);

  return Erro;
}
CDGerarDXFSecoes::~CDGerarDXFSecoes()
{
   std::string NomeArquivo(NomeProjeto + ".ini");

   CString CSDados;

   CSDados.Format("%lf,%lf",AlturaCotas,DistMinCotas);

   ::WritePrivateProfileString(_T("DadosDesDXF"),_T("Secoes"),CSDados,NomeArquivo.c_str());	
}

void CDGerarDXFSecoes::PassaSecoesAvulsasOriSecoes()
{
  CSecao SecaoAtual;
  
  for (POSITION Atual = SecoesAvulsasOrig.PegaLSecoesAvulsas().GetHeadPosition(); Atual; SecoesAvulsasOrig.PegaLSecoesAvulsas().GetNext(Atual))
  {
    POSITION Pos(SecoesAvulsas.AddTail(SecaoAtual));

    if (Pos)
    {
      CSecao& RSecAtual(SecoesAvulsas.GetAt(Pos));

      RSecAtual.Terreno = SecoesAvulsasOrig.PegaLSecoesAvulsas().GetAt(Atual);
      RSecAtual.Projeto = SecoesAvulsasOrig.PegaLSecoesAvulsas().GetAt(Atual).LstPontosProjeto;
      RSecAtual.Estaca.Descricao = SecoesAvulsasOrig.PegaLSecoesAvulsas().GetAt(Atual).Nome.c_str();
    }
  }
}