// Relatorios.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include <string>
#include "afxtempl.h"
#include "cponto.h"
#include "clpontos.h"
#include "estaca.h"
#include "perfil.h"
#include "supercstring.h"
#include "carqtalaterro.h"
#include "carqtalcorte.h"
#include "ctalaterro.h"
#include "ctalcorte.h"
#include "carqsectipo.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "csectipo.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include <list>
#include "csecao.h"
#include "clistaperfis.h"
#include "clistasecoes.h"
#include <string>
#include <set>
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "carqcurhorizontais.h"
#include <map>
#include "ccurhorizontal.h"
#include "Mat.h"
#include "ccurhorizontais.h"
#include "carqcurverticais.h" 
#include "ccurverticais.h"
#include "CRegAreaTaludes.h"
#include "CLimpeza.h"
#include "Relatorios.h"
#include <strstream>
#include <iomanip>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "monologo.h"
#include <sstream> 
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CAcidentesTopograficos.h"
#include "CDeltaSuperficie.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "CEquipDrenagem.h"
#include "EstRel.h"
#include "CDFormatarRelatorio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define CURVA_ESPIRAL 0
extern class Matem Mat;
extern class monologo monolog;

typedef std::map<CEstaca,Ponto> MSecaoGeom;    //--- Secoes geometricas para acesso randomico
typedef MSecaoGeom::iterator itMSecaoGeom;

/////////////////////////////////////////////////////////////////////////////
// CRelatorios

IMPLEMENT_DYNCREATE(CRelatorios, CScrollView)

CRelatorios::CRelatorios():Extremox(0),Extremoy(0),PagAtual(0), NLinhas(0),TamLetra(0,0),LocalAtual(0,0),
                           IDRelatorio(((CMainFrame*)AfxGetMainWnd())->asdfg),CasasDecimais(3)
{
}

CRelatorios::~CRelatorios()
{
}

BEGIN_MESSAGE_MAP(CRelatorios, CScrollView)
  ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
  ON_WM_KEYDOWN()
  ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRelatorios drawing

void CRelatorios::OnDraw(CDC* pDC){DesenhaRelatorio(pDC);}

/////////////////////////////////////////////////////////////////////////////
// CRelatorios diagnostics

#ifdef _DEBUG
void CRelatorios::AssertValid() const
{
  CScrollView::AssertValid();
}

void CRelatorios::Dump(CDumpContext& dc) const
{
  CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRelatorios message handlers

void CRelatorios::OnInitialUpdate() 
{
  SetScrollSizes(MM_TEXT,CSize(200,200));
}

BOOL CRelatorios::PreCreateWindow(CREATESTRUCT& cs) 
{
  // TODO: Add your specialized code here and/or call the base class

  cs.style |=  WS_HSCROLL | WS_VSCROLL;                  
  return CScrollView::PreCreateWindow(cs);
}

BOOL CRelatorios::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  this->GetDocument()->SetModifiedFlag(true);
  pInfo->m_bContinuePrinting = TRUE; 
  return DoPreparePrinting(pInfo);
}

void CRelatorios::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
  // TODO: Add your specialized code here and/or call the base class

  if(pInfo->m_nCurPage > 1) 
    pDC->SetWindowOrg(CPoint(paper_res.x,0));

  CScrollView::OnPrint(pDC, pInfo);

  PagAtual = pInfo->m_nCurPage;
}

void CRelatorios::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
  PagAtual = 0;

  paper_res.x = pDC->GetDeviceCaps(HORZRES);
  paper_res.y = pDC->GetDeviceCaps(VERTRES);

  CScrollView::OnBeginPrinting(pDC, pInfo);
}

void CRelatorios::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
  // TODO: Add your specialized code here and/or call the base class

  CScrollView::OnPrepareDC(pDC, pInfo);
  if(pInfo && pInfo->m_nCurPage > 1) pDC->SetWindowOrg(CPoint(100,100));
}

void CRelatorios::DesenhaRelatorio(CDC* pDC)
{
  TamLetra = pDC->GetTextExtent("Z");
  LocalAtual = CPoint(0,0);
  std::strstream strstreRelatorio;

  ((CEstRel*)GetDocument())->GeraRelatorio(strstreRelatorio,IDRelatorio);
  
  GetDocument()->SetModifiedFlag(true);

  Extremox = Extremoy = NLinhas = 0;
  std::string strLinhaAtual;

  CFont* LF = pDC->GetCurrentFont();
  LOGFONT Log;
  CFont LucidaConsole;

  LucidaConsole.CreateFont(13,0, 0, 0, FW_NORMAL,FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN,_T("Lucida Console"));

  LF->GetLogFont(&Log);

  // strcpy(Log.lfFaceName, "Loucida Console");
  // LucidaConsole.CreateFontIndirect(&Log);

  pDC->SelectObject(&LucidaConsole);

  while((std::getline(strstreRelatorio,strLinhaAtual)))
  {
    pDC->TextOut(LocalAtual.x,LocalAtual.y,strLinhaAtual.c_str());

    LocalAtual.y += TamLetra.cy;
    LocalAtual.x = 0;

    if(Extremox < pDC->GetTextExtent(strLinhaAtual.c_str()).cx)
      Extremox = pDC->GetTextExtent(strLinhaAtual.c_str()).cx;
    NLinhas++;
  }

  Extremoy = ++NLinhas * TamLetra.cy ; 

  pDC->TextOut(0,LocalAtual.y,"FIM DO RELATÓRIO.");

  SetScrollSizes(MM_TEXT,CSize(Extremox,Extremoy));

  RECT Coord; 
  GetClientRect(&Coord);
}

void CRelatorios::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);

  RECT Coord; 
  GetClientRect(&Coord);
  CPoint PAtual(GetDeviceScrollPosition());

  switch (nChar)
  {
  case VK_DOWN:

    if(Coord.right < Extremoy && PAtual.y < Extremoy)
    {
      PAtual.y += TamLetra.cy; 
      ScrollWindow(0,PAtual.y,0,0);
      SetScrollPos(SB_VERT,PAtual.y,false);
      RedrawWindow();
    }
    break;
  case VK_UP: 
    if(Coord.right < Extremoy && PAtual.y > 0)
    {
      PAtual.y -= TamLetra.cy; 
      ScrollWindow(0,PAtual.y,0,0);
      SetScrollPos(SB_VERT,PAtual.y,false);
      RedrawWindow();
    }
    break;
  case VK_LEFT:
    if(Coord.bottom < Extremox && PAtual.x < Extremox)
    {
      PAtual.x += TamLetra.cx; 
      ScrollWindow(PAtual.x,0,0,0);
      SetScrollPos(SB_HORZ,PAtual.x,false);
      RedrawWindow();
    }
    break;
  case VK_RIGHT: 
    if(Coord.bottom < Extremox && PAtual.x > 0)
    {
      PAtual.x -= TamLetra.cx; 
      ScrollWindow(PAtual.x,0,0,0);
      SetScrollPos(SB_HORZ,PAtual.x,false);
      RedrawWindow();
    }
    break;
  case VK_NEXT:
    if(Coord.bottom < Extremoy && PAtual.y < Extremoy - Coord.bottom)
    {
      PAtual.y += (Coord.bottom  / TamLetra.cy) * TamLetra.cy ; 
      ScrollWindow(0,PAtual.y,0,0);
      SetScrollPos(SB_VERT,PAtual.y,false);
      RedrawWindow();
    }
    break;
  case VK_PRIOR: 
    if(Coord.bottom < Extremoy && PAtual.y > 0)
    {
      PAtual.y -= (Coord.right / 2 / TamLetra.cy) *TamLetra.cy; 
      ScrollWindow(0,PAtual.y,0,0);
      SetScrollPos(SB_VERT,PAtual.y,false);
      RedrawWindow();
    }
    break;
  case VK_HOME:
    if(PAtual.y > 0)
    {
      PAtual.y = 0; 
      ScrollWindow(0,PAtual.y,0,0);
      SetScrollPos(SB_VERT,PAtual.y,false);
      RedrawWindow();
    }
    break;
  case VK_END:
    if(PAtual.y < Extremoy - Coord.bottom)
    {
      PAtual.y = Extremoy - Coord.bottom; 
      ScrollWindow(0,PAtual.y,0,0);
      SetScrollPos(SB_VERT,PAtual.y,false);
      RedrawWindow();
    }
    break;
  }
}

void CRelatorios::OnRButtonDown(UINT nFlags, CPoint point)
{
  CMainFrame* MF((CMainFrame*)AfxGetMainWnd());

  CDFormatarRelatorio DFormatarRelatorio(this,&MF->CasasDecimaisRelatorios);
  
  DFormatarRelatorio.DoModal();

  RedrawWindow();
}
