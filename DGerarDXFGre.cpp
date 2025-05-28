// DGerarDXF.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "stdafx.h"
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
#include "carqcurverticais.h"
#include "ccurverticais.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "csecao.h"
#include "clistasecoes.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
//#include "CadLib.h"

#include "DGerarDXFgre.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDGerarDXFGre dialog

CDGerarDXFGre::CDGerarDXFGre(CWnd* pParent,CString& NomeProj)	: CDialog(CDGerarDXFGre::IDD,pParent),
CurvasVerticais(NomeProj),Secoes(NomeProj),HPavimento(NomeProj),OK(false)
{
  Deltas[0] = 0.0;
  Deltas[1] = 0.0;
  Escala[0] = 1.0;
  Escala[1] = 1.0;


  //{{AFX_DATA_INIT(CDGerarDXFGre)
  m_CStrMensagem = _T("");
  //}}AFX_DATA_INIT
}

void CDGerarDXFGre::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDGerarDXFGre)
  DDX_Control(pDX, IDC_GERAR, m_BUGerar);
  DDX_Control(pDX, IDC_PROGDXFGEO, m_PrgProgresso);
  DDX_Text(pDX, IDC_MENSGERARDXF, m_CStrMensagem);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDGerarDXFGre, CDialog)
  //{{AFX_MSG_MAP(CDGerarDXFGre)
  ON_BN_CLICKED(IDC_GERAR, OnGerarDXF)
  ON_BN_CLICKED(IDC_SAIR, OnSair)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDGerarDXFGre message handlers

void CDGerarDXFGre::OnGerarDXF() 
{
  static char szFilter[] = "DXF file format(*.dxf) |*.dxf|All Files (*.*)|*.*||";

  CFileDialog SaveDlg( FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_EXPLORER, szFilter, NULL );
  SaveDlg.m_ofn.lpstrTitle = "Save DXF File As";

  if(SaveDlg.DoModal()==IDOK)
  {	
    // Saving sample dxf file data

    CString DxfFileName(SaveDlg.GetPathName() );
    CDrawing drw;

    if(SaveDlg.GetFileExt().IsEmpty()) DxfFileName += ".dxf";

    BOOL result= false;

    // Create Drawing
    if(!drawing.Create())
    {
      m_CStrMensagem ="Erro Fatal Criando o arquivo DXF.";
      this->UpdateData(FALSE);
      return;
    }

    // Tables Section ------------------------------------------
    //  LTYPE table type -------------------------
    LTYPE		ltype;
    OBJHANDLE	objhandle1;

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
    drawing.AddLinetype(&ltype);

    //  LAYER table type -------------------------
    LAYER	layer;

    // Layer1
    ZeroMemory(&layer, sizeof(layer));
    strcpy(layer.Name, "Layer1");         //--- Pontos Terreno
    layer.Color = 2;
    layer.LineTypeObjhandle = objhandle1; // Continuous
    drawing.AddLayer(&layer);

    // Layer2
    ZeroMemory(&layer, sizeof(layer));
    strcpy(layer.Name, "Layer2");         //--- Projeto Greide
    layer.Color = 1;
    layer.LineTypeObjhandle = objhandle1; // Continuous
    drawing.AddLayer(&layer);

    // Layer3
    ZeroMemory(&layer, sizeof(layer));
    strcpy(layer.Name, "Layer3");         //--- Greide Terraplanagem
    layer.Color = 5;
    layer.LineTypeObjhandle = objhandle1; // Continuous
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
    //---  Desenha os pontos do terreno - Layer 1

    result &= drawing.SetLayer("Layer1");

    int Tam(Secoes.GetCount()),i(0);

    m_PrgProgresso.SetRange(0,Secoes.GetCount());

    if (Tam)
    {
      ENTVERTEX* VertexTerreno = (struct tag_ENTVERTEX *) new ENTVERTEX[Tam];
      ZeroMemory(VertexTerreno,sizeof(ENTVERTEX)*Tam);
      POSITION SecAtual(Secoes.GetHeadPosition());

      m_CStrMensagem = "Desenhando o terreno...";
      UpdateData(false);

      while(SecAtual)
      {
        const CEstaca& EstacaAtual(Secoes.GetNext(SecAtual).Terreno.Estaca);

        m_PrgProgresso.StepIt();

        //--- A estaca pode estar sem dados ou sem terreno.       

        if(EstacaAtual.EstVirtual == INFINITO || EstacaAtual.Cota == INFINITO) continue;  

        ConvEstRealPVertex((VertexTerreno+i)->Point,EstacaAtual);

        i++;
      }  

      result &= drawing.PolyLine(VertexTerreno,i-1);    //--- tem q usar o i porque ele conta as seções válidas

      delete[] VertexTerreno;
    }

    // Salva o arquivo dxf

    if(result || !drawing.SaveDXFFile(DxfFileName))
    {
      result = FALSE;
      m_CStrMensagem ="Erro Salvando o arquivo DXF!";
      UpdateData(FALSE);
      return;
    }

    // Destroi CDrawing e libera a memoria alocada

    result &= drawing.Destroy();

    if(!result)	m_CStrMensagem = "Arquivo DXF Gerado com sucesso!";
    else m_CStrMensagem = "Erro Gerando o arquivo DXF!";

    UpdateData(FALSE);

    m_BUGerar.EnableWindow(false);   //--- Desabilita o botão "Gerar DXF"
  }
}

void CDGerarDXFGre::OnSair(){ EndDialog(TRUE);}
