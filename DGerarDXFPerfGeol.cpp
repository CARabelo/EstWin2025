// DGerarDXFPerfGeol.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
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
#include "Igualdades.h"
#include "projeto.h"
#include "Cadlib\Interface\VC\Cadlib.h"
#include "CxCotas.h"
#include "CTipoSolos.h"
#include "CArqTiposSolos.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include "CArqFurosSondagem.h"
#include <algorithm>
#include "CPerfilGeologico.h"
#include "DGerarDXFPerfGeol.h"
#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ALTURALETRA  1.5
#define DISTMINCOTAS 2.0
#define LIMESQ -INFINITO
#define LIMDIR INFINITO

#define LER 0

/////////////////////////////////////////////////////////////////////////////
// CDGerarDXFPerfGeol dialog

CDGerarDXFPerfGeol::CDGerarDXFPerfGeol(CWnd* pParent,CString& NomeProj)	: CDialog(CDGerarDXFPerfGeol::IDD,pParent),OK(true), DxfFileName(NomeProj + "PerfGeol.dxf"),
  NomeProjeto(NomeProj),AlturaCotas(3),DistMinCotas(3.7),DistEntreCotas(0.7)
{
  Deltas[0] = 0.0;
  Deltas[1] = 0.0;
  Escala[0] = 1.0;
  Escala[1] = 1.0;

  //{{AFX_DATA_INIT(CDGerarDXFPerfGeol)
  m_CStrMensagem = _T("");
  //}}AFX_DATA_INIT
}

void CDGerarDXFPerfGeol::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDGerarDXFPerfGeol)
  DDX_Control(pDX, IDC_GERAR, m_BUGerar);
  DDX_Control(pDX, IDC_SAIR, m_BUSair);
  DDX_Control(pDX, IDC_PROGDXFGEO, m_Progresso);
  DDX_Text(pDX, IDC_MENSGERARDXF, m_CStrMensagem);
  //}}AFX_DATA_MAP
  DDX_Text(pDX, IDC_EDIALTCOTAS, AlturaCotas);
  DDV_MinMaxDouble(pDX, AlturaCotas, 1, 20);
}

BEGIN_MESSAGE_MAP(CDGerarDXFPerfGeol, CDialog)
  //{{AFX_MSG_MAP(CDGerarDXFPerfGeol)
  ON_BN_CLICKED(IDC_GERAR, OnGerar)
  ON_BN_CLICKED(IDC_SAIR, OnSair)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDGerarDXFPerfGeol message handlers

void CDGerarDXFPerfGeol::OnGerar() 
{
  UpdateData(true);

  m_BUGerar.EnableWindow(false);
  m_BUSair.EnableWindow(false);

  BOOL Erro= false;

  // Create	Drawing
  if(!drawing.Create())
  {
    m_CStrMensagem ="Erro	Fatal	Criando	o	arquivo	DXF.";
    UpdateData(FALSE);

    return;
  }

  //  Tables Section ------------------------------------------
  //  LTYPE table type ----------------------------------------

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

  //----  As Penas e os traços são definidos em função do tipo de solo 

  CArqTiposSolos ArqTiposSolos(NomeProjeto.c_str());
  tysetCTipoSolo SetTiposSolos;
  ArqTiposSolos.CriaSet(SetTiposSolos);
  ArqTiposSolos.ConverteGDI(SetTiposSolos);

  CPerfilGeologico PerfilGeologico(NomeProjeto + ".prg");
  PerfilGeologico.Serialize(NomeProjeto,LER);

  if(PerfilGeologico.PegaPerfisSolos().size() == 0)
  {
    m_CStrMensagem ="Não há perfís geológicos...";
    UpdateData(FALSE);
    return;
  }

  CalculaCotasFurosSondagem(&PerfilGeologico);

  //--- Conta os perfis com nomes diferentes que será o numero de linhas no gabarito de cotas, como também o numero de layers de perfís

  tysetstring SetNomesTiposSolosUsados;

  for(rittysetCPerfilTipoSolo itPerfilSolo=PerfilGeologico.PegaPerfisSolos().rbegin() ; itPerfilSolo != PerfilGeologico.PegaPerfisSolos().rend() && !Erro; ++itPerfilSolo)
  {
    std::string strTemp(RemoveBrancos(itPerfilSolo->PegaTipoSolo()));
    SetNomesTiposSolosUsados.insert(strTemp);
  }

  size_t QtdPerfis(SetNomesTiposSolosUsados.size());

  LAYER* LayersPerfis(new LAYER[QtdPerfis+1]);
  ZeroMemory(LayersPerfis,sizeof(LAYER) * QtdPerfis);

  int C(0);
  unsigned int TabCores[10]={1,2,3,4,5,6,30,50,70,90}; 

  for(ittysetstring itTipoSoloAtual = SetNomesTiposSolosUsados.begin() ; itTipoSoloAtual != SetNomesTiposSolosUsados.end() ; ++itTipoSoloAtual,C++)
  {
    strcpy((LayersPerfis+C)->Name,itTipoSoloAtual->c_str());     //--- Nome do tipo de solo
    (LayersPerfis+C)->Color = TabCores[C % 11] ;                 //--- itTipoSoloAtual->Cor : autocad Não tem RGB
    (LayersPerfis+C)->LineTypeObjhandle = objhandle1;            //--- Continuous
    drawing.AddLayer(LayersPerfis+C);
  }

  strcpy((LayersPerfis+C)->Name,"Indefinido");              
  (LayersPerfis+C)->Color = 1;
  (LayersPerfis+C)->LineTypeObjhandle = objhandle1;          
  drawing.AddLayer(LayersPerfis+C);

  ///  LAYER table type -------------------------

  LAYER layer;

  // Layer - Texto
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name,"Texto");              
  layer.Color = 255;
  layer.LineTypeObjhandle = objhandle1;    
  drawing.AddLayer(&layer);
  //Layer - Malha
  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "Malha");           
  layer.Color = 84;
  layer.LineTypeObjhandle = objhandle1;    
  drawing.AddLayer(&layer);

  ZeroMemory(&layer, sizeof(layer));
  strcpy(layer.Name, "FurosSondagem");    
  layer.Color = 253;
  layer.LineTypeObjhandle = objhandle1;    
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
  style.LastHeightUsed = 0.3;
  style.WidthFactor = 1;
  objhandle1 = drawing.AddTextStyle(&style);

  drawing.SetTextStyle("Style1");  

  //--- Entities Section ----------------------------------------
  //--- Desenha os Perfís --------------------------------------

  double MenorCota(INFINITO),MenorEstaca(INFINITO),MaiorEstaca(-INFINITO);

  for(rittysetCPerfilTipoSolo itPerfilSolo=PerfilGeologico.PegaPerfisSolos().rbegin() ; itPerfilSolo != PerfilGeologico.PegaPerfisSolos().rend() && !Erro; ++itPerfilSolo)
  {
    ENTVERTEX* VertexPerfil((struct tag_ENTVERTEX *) new ENTVERTEX[itPerfilSolo->PegaPerfil().size()]);
    ZeroMemory(VertexPerfil,sizeof(ENTVERTEX)*itPerfilSolo->PegaPerfil().size());

    C = 0;

    for(ittyPerfilTipoSolo& itPerfilTipoSolo = itPerfilSolo->PegaPerfil().begin() ; itPerfilTipoSolo != itPerfilSolo->PegaPerfil().end() ;++itPerfilTipoSolo)
    {
      if(itPerfilTipoSolo->EstVirtual != INFINITO && itPerfilTipoSolo->Cota != INFINITO)
      {
        ConvEstRealPVertex((VertexPerfil+C)->Point,*itPerfilTipoSolo);

        if(MenorCota > itPerfilTipoSolo->Cota) MenorCota = itPerfilTipoSolo->Cota;
        if(MenorEstaca > itPerfilTipoSolo->EstVirtual) MenorEstaca = itPerfilTipoSolo->EstVirtual;
        if(MaiorEstaca < itPerfilTipoSolo->EstVirtual) MaiorEstaca = itPerfilTipoSolo->EstVirtual;

        C++;
      }
    }

    LAYER* LayerAtual(PegaLayer(LayersPerfis,RemoveBrancos(itPerfilSolo->PegaTipoSolo()),(unsigned int)QtdPerfis));

    if(LayerAtual != NULL) Erro &= drawing.SetLayer(LayerAtual->Name);
    else Erro &= drawing.SetLayer("Indefinido");

    Erro &= drawing.PolyLine(VertexPerfil,C);    //--- Desenha o perfil,tem q usar o C porque ele conta as seções válidas

    for (register int i = 0 ; i < C && !Erro ; ++i) 
    {
      Erro &= drawing.Point((VertexPerfil+i)->Point.x,(VertexPerfil+i)->Point.y,0.0,5.0);  
    }

    delete[] VertexPerfil;
  }

  //--- Desenha os furos de sondagem

  drawing.SetLayer("FurosSondagem");

  const tysetFurosSondagem SetFurosSondagem(PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem());

  for(itctysetFurosSondagem itFuroSondagem = SetFurosSondagem.begin() ; itFuroSondagem != SetFurosSondagem.end() ; ++itFuroSondagem)
  {
    double CotaAtual(itFuroSondagem->PegaEstaca().Cota);

    for(itctysetCTipoSoloSondagem itTipoSolo = itFuroSondagem->PegaSetSolos().begin()  ; itTipoSolo != itFuroSondagem->PegaSetSolos().end() ; itTipoSolo++)
    {
      REALPOINT P3;

      CotaAtual-=itTipoSolo->PegaProfundidade();

      ConvEstRealPVertex(P3,CEstaca(itFuroSondagem->PegaEstaca().EstVirtual,CotaAtual));

      Erro &= drawing.Circle(itFuroSondagem->PegaEstaca().EstVirtual,CotaAtual*10.0,4);

      if(MenorCota > CotaAtual) MenorCota = CotaAtual;
    }

    REALPOINT P1,P2;

    ConvEstRealPVertex(P1,itFuroSondagem->PegaEstaca());
    ConvEstRealPVertex(P2,CEstaca(itFuroSondagem->PegaEstaca().EstVirtual,CotaAtual));

    Erro &= drawing.Line(P1.x,P1.y,P2.x,P2.y);
  }

  if(!Erro)
  {
    //--- Cota o desenho

    m_CStrMensagem = "cotando o desenho...";
    UpdateData(false);    
    m_Progresso.SetPos(0);
    m_Progresso.SetRange(0,(short)PerfilGeologico.PegaPerfisSolos().size());

    //--- Prepara os locais onde as cotas serão impressas deslocando-as caso elas se sobreponham

    DirecaoAtual = VU_E_LADOESQUERDO;
    double DeslocRefx(0.0),DeslocAtualx(DeslocRefx),
      DeslocAtualy(95.0),DeslocyCotas(0.0);

    int Count(0);
    std::string StrNomePerfAnterior(PerfilGeologico.PegaPerfisSolos().size() > 0 ? PerfilGeologico.PegaPerfisSolos().rbegin()->PegaTipoSolo():"");

    //--- Distribui as cotas no gabarito de cotas

    for(ittysetCPerfilTipoSolo ritPerfilSolo=PerfilGeologico.PegaPerfisSolos().begin() ; ritPerfilSolo != PerfilGeologico.PegaPerfisSolos().end() && !Erro; ++ritPerfilSolo)
    {
      if(StrNomePerfAnterior.compare(ritPerfilSolo->PegaTipoSolo()) == 0)     //--- Se o nome for o mesmo, ou seja se for o mesmo perfil, mantem o alinhamento das cotas
      {
        DeslocyCotas += 75.0;
        DeslocAtualy -= 75.0;
      }
      else 
      {
        StrNomePerfAnterior = ritPerfilSolo->PegaTipoSolo();
        LstNomesTiposSolos.push_back(ritPerfilSolo->PegaTipoSolo());
      }

      SetxCotas.clear();
      SetxCotas.insert(CxCotas(0.0,0.0));   //--- Insere um ponto inicial

      for(ittyPerfilTipoSolo& itPerfilTipoSolo = ritPerfilSolo->PegaPerfil().begin() ; itPerfilTipoSolo != ritPerfilSolo->PegaPerfil().end() ;++itPerfilTipoSolo)
      {
        if(itPerfilTipoSolo->EstVirtual != INFINITO) 
        {
          double EstacaAtual(itPerfilTipoSolo->EstVirtual);

          if (DirecaoAtual != VU_E_ACABOUESPACO) 
          {
            if (itPerfilTipoSolo->EstVirtual  < ((*SetxCotas.rbegin()).PegaxCota() + ALTURALETRA + DISTMINCOTAS))
            {
              DistribuiCotas(EstacaAtual,SetxCotas);   //--- cota sobrepos, distribui a posição
            }
            else  SetxCotas.insert(CxCotas(EstacaAtual,EstacaAtual));       //--- Não Sobrepos, insere normalmente
          }
          else SetxCotas.insert(CxCotas(EstacaAtual,EstacaAtual));          //--- Acabou o espaço, insere assim mesmo
        }
      }

      for(ittyPerfilTipoSolo& itPerfilTipoSolo = ritPerfilSolo->PegaPerfil().begin() ; itPerfilTipoSolo != ritPerfilSolo->PegaPerfil().end() ;++itPerfilTipoSolo)
      {
        CString CStrRPontoBase;
        Erro &= drawing.SetLayer("Texto");

        m_Progresso.StepIt();
        Ponto PontoAtual;

        double CotaTerreno(itPerfilTipoSolo->Cota), PosxAtual(-INFINITO);

        ittysetxCotas itXCotaAtual(SetxCotas.end());

        if(itPerfilTipoSolo->EstVirtual != INFINITO) 
        {
          itXCotaAtual = SetxCotas.find(CxCotas(itPerfilTipoSolo->EstVirtual,0.0));
          if(itXCotaAtual == SetxCotas.end()) itXCotaAtual = SetxCotas.find(CxCotas(itPerfilTipoSolo->EstVirtual,0.0));

          if(itXCotaAtual != SetxCotas.end()) 
          {
            double Local(itXCotaAtual->PegaxCota()+1.5);  

            CStrRPontoBase.Format("%12.12s",const_cast<CEstaca*>(&*itPerfilTipoSolo)->GetBuffer());
            Erro &= drawing.Text(CStrRPontoBase,itXCotaAtual->PegaxCota()+1.5+DeslocAtualx,(MenorCota)*10.0-DeslocAtualy+20.0,3.0,90.0) > 0;

            CStrRPontoBase.Format("%9.3lf",itPerfilTipoSolo->Cota);
            Erro &= drawing.Text(CStrRPontoBase,itXCotaAtual->PegaxCota()+1.5+DeslocAtualx,(MenorCota)*10.0-DeslocAtualy+50.0,3.0,90.0) > 0;

          }

          if(itXCotaAtual == SetxCotas.end()) itXCotaAtual = SetxCotas.find(CxCotas(itPerfilTipoSolo->EstVirtual,0.0));

          //--- Desenha o traço que marca a estaca

          Erro &= drawing.SetLayer("Texto");

          if(itXCotaAtual != SetxCotas.end()) 
          {
            double xDeslocado(itXCotaAtual->PegaxCota());

            Erro &= drawing.Line(itPerfilTipoSolo->EstVirtual,(MenorCota-1.75)*10.0-DeslocAtualy+20,itPerfilTipoSolo->EstVirtual,(MenorCota-1.22)*10.0-DeslocAtualy+20);
            Erro &= drawing.Line(itPerfilTipoSolo->EstVirtual,(MenorCota-1.22)*10.0-DeslocAtualy+20, xDeslocado,(MenorCota-1.06)*10.0-DeslocAtualy+20);
            Erro &= drawing.Line(xDeslocado,(MenorCota-1.06)*10.0+-DeslocAtualy+20,xDeslocado,(MenorCota-0.60)*10.0+-DeslocAtualy+20);
          }
        }
      }

      DeslocyCotas -= 75.0;
      DeslocAtualy += 75.0;
    }

    //--- Desenha a malha das cotas

    double DeslocInicial(-20.0),Desloc(DeslocInicial);

    Erro &= drawing.SetLayer("Malha");

    drawing.Line(MenorEstaca-15.0,(MenorCota)*10.0-20.0,MaiorEstaca+6.0,(MenorCota)*10.0-20.0);
    Desloc = 75.0+13.5;

    int C(0);

    for( ;  C < QtdPerfis ;  C++)
    {
      Erro &= drawing.Line(MenorEstaca-15.0,(MenorCota)*10.0-Desloc,MaiorEstaca+6.0,(MenorCota)*10.0-Desloc);
      Desloc += 75.0;
    }

    Desloc -= 75.0;

    //--- Verticais

    Erro &= drawing.Line(MenorEstaca-15.0,(MenorCota)*10.0+DeslocInicial,MenorEstaca-15.0,(MenorCota)*10.0-Desloc);
    Erro &= drawing.Line(MenorEstaca-8.0,(MenorCota)*10.0+DeslocInicial,MenorEstaca-8.0,(MenorCota)*10.0-Desloc);
    Erro &= drawing.Line(MaiorEstaca+6.0,(MenorCota)*10.0+DeslocInicial,MaiorEstaca+6.0,(MenorCota)*10.0-Desloc);

    C = 0;

    //--- Nomes das colunas

    for(ittylststring it = LstNomesTiposSolos.begin() ;  it != LstNomesTiposSolos.end() && ! Erro; ++it,C++)
    {
      Erro &= drawing.Text(it->c_str(),(MenorEstaca-10.0),(MenorCota-1)*10.0-(C+1)*75.0,3.0,90.0) > 0;
    }

    Erro &= drawing.SetLayer("Texto");
    Erro &= drawing.Text("Vias Urbanas & Estradas - Versão 3.00 - Boole Software Ltda - Todos os Direitos Autorais Reservados - 2024",MenorEstaca-10.0,(MenorCota)*10.0-C*75.0-30.0, 4.0) > 0;

    m_Progresso.SetPos((int)PerfilGeologico.PegaPerfisSolos().size());

    //--- Verifica se houve erro

    if(Erro)
    {
      m_CStrMensagem ="Erro gerando o arquivo DXF!";
      UpdateData(FALSE);
    }

    if(!Erro && !drawing.SaveDXFFile(DxfFileName.c_str()))
    {
      m_CStrMensagem ="Erro Salvando o arquivo DXF!";
      UpdateData(FALSE);
    }
    // Destroi CDrawing e libera a memoria alocada

    Erro &= drawing.Destroy();
    delete[] LayersPerfis;

    if(!Erro) m_CStrMensagem = "Arquivo DXF Gerado com sucesso!";
    else m_CStrMensagem += " -> Erro Gerando o arquivo DXF!";
    m_BUGerar.EnableWindow(false);
    m_BUSair.EnableWindow(true);

    UpdateData(FALSE);

  }
}

void CDGerarDXFPerfGeol::OnSair() 
{
  EndDialog(TRUE);
}

BOOL CDGerarDXFPerfGeol::OnInitDialog() 
{
  CDialog::OnInitDialog();

  if(!OK)
  {
    m_BUGerar.EnableWindow(false);
    m_CStrMensagem = "Não há curvas perfís geológicos, o arquivo não foi gerado.";

    UpdateData(false);
  } 

  CString Titulo;

  GetWindowText(Titulo);
  Titulo.Replace("Geométrico", "Perfís Geológicos");
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

void CDGerarDXFPerfGeol::DistribuiCotas(double& XCota,tysetxCotas& SetxCotas)
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

CDGerarDXFPerfGeol::~CDGerarDXFPerfGeol()
{
  std::string NomeArquivo(NomeProjeto + ".ini");

  CString CSDados;

  CSDados.Format("%lf,%lf",AlturaCotas,DistMinCotas);

  ::WritePrivateProfileString(_T("DadosDesDXF"),_T("Greide"),CSDados,NomeArquivo.c_str());	
} 

void CDGerarDXFPerfGeol::CalculaCotasFurosSondagem(const void* vpPerfilGeologico)
{
  const CPerfilGeologico* pPerfilGeologico((CPerfilGeologico*) vpPerfilGeologico);

  double ProfFuro(0.0);

  const tysetFurosSondagem& SetFurosSondagem(pPerfilGeologico->PegaFurosSondagem().PegaSetFurosSondagem());

  for(ittysetFurosSondagem itFuroSondagem = SetFurosSondagem.begin() ; itFuroSondagem != SetFurosSondagem.end() ; ++itFuroSondagem)
  {
    (const_cast<CFuroSondagem*>(&*itFuroSondagem))->Alteracota(PegaCotaTerreno(itFuroSondagem->PegaEstaca(),vpPerfilGeologico));

    if(itFuroSondagem->PegaEstaca().Cota == INFINITO)
    {
      AfxMessageBox("Nao há terreno ne estaca " + itFuroSondagem->PegaEstaca() + ". As cotas das profundidades do furo de sondagem nesta estaca não puderam ser calculadas corretamente.");
    }

    double ProfFuro(0.0);

    for(ittysetCTipoSoloSondagem itTipoSolo = itFuroSondagem->PegaSetSolos().begin()  ; itTipoSolo != itFuroSondagem->PegaSetSolos().end() ; itTipoSolo++)
    {
      ProfFuro += itTipoSolo->PegaProfundidade();
      (const_cast<CTipoSoloSondagem*>(&*itTipoSolo))->AlteraCota(itFuroSondagem->PegaEstaca().Cota - ProfFuro);  
    }
    (const_cast<CFuroSondagem*>(&*itFuroSondagem))->AlteraCotaFundo(ProfFuro);
  }
}

double CDGerarDXFPerfGeol::PegaCotaTerreno(const CEstaca& Estaca,const void* vpPerfilGeologico)
{
  const CPerfilGeologico* pPerfilGeologico((CPerfilGeologico*) vpPerfilGeologico);

  ittysetCPerfilTipoSolo itPerfilSolo(pPerfilGeologico->PegaPerfisSolos().begin());
  ittyPerfilTipoSolo& itAtual(itPerfilSolo->PegaPerfil().begin()),
    itAnterior(itPerfilSolo->PegaPerfil().begin());

  while(itAtual != itPerfilSolo->PegaPerfil().end() && itAtual->EstVirtual - Estaca.EstVirtual < 0.005)
  {
    itAnterior = itAtual++;
  }

  if(fabs(itAtual->EstVirtual - Estaca.EstVirtual) < 0.005) return  itAtual->Cota;

  if(itAnterior != itAtual)
  {
    double i((itAtual->Cota - itAnterior->Cota) / (itAtual->EstVirtual - itAnterior->EstVirtual));

    return (Estaca.EstVirtual - itAnterior->EstVirtual) * i +  itAnterior->Cota;
  }

  return 0.0;
}
