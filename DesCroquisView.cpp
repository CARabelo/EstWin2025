#include "stdafx.h"
#include "CMemDC.h"
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "cponto.h"
#include "estaca.h"
#include "clpontos.h"
#include "perfil.h"
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
#include <list>
#include "resource.h"
#include <sstream>

BEGIN_MESSAGE_MAP(DesCroquisView, CView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_CROQUIS_REMOVERTITULO, &DesCroquisView::OnCroquisRemovertitulo)
	ON_COMMAND(ID_CROQUIS_SAIR, &DesCroquisView::OnCroquisSair)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(DesCroquisView, CView)

DesCroquisView::DesCroquisView() : pFronteira(NULL), AlturaVideoOriginalD2(100.0), LarguraVideoOriginalD2(100.0), Estado(SELECIONANDO), 
                                   Titulo(true), AlturaCroquisPixels(100), pPicture(NULL), pDesenharMapa(NULL),TipoCroquis(NENHUM_TIPO)
{
}

int DesCroquisView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	/*lpCreateStruct->style &= ~WS_VISIBLE;
	lpCreateStruct->dwExStyle |= WS_EX_TOPMOST & WS_EX_TOOLWINDOW;
	lpCreateStruct->dwExStyle |= WS_EX_PALETTEWINDOW;
	lpCreateStruct->dwExStyle &= !(WS_MAXIMIZEBOX | WS_SYSMENU | WS_MINIMIZEBOX);

	CMenu* pTopMenu = GetSystemMenu(FALSE);
	if (pTopMenu != NULL)
	{
		pTopMenu->RemoveMenu(SC_SIZE, MF_BYCOMMAND);      //Size
		pTopMenu->RemoveMenu(SC_MOVE, MF_BYCOMMAND);      //Move
		pTopMenu->RemoveMenu(SC_MAXIMIZE, MF_BYCOMMAND);  //Maximize
		pTopMenu->RemoveMenu(SC_CLOSE, MF_BYCOMMAND);  //Maximize
	}
	*/

	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void DesCroquisView::OnDraw(CDC* ppDC)
{
	CMMemDC pDC(ppDC);

	CPen Pena;
	Pena.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	pDC->SelectObject(&Pena);

	if (pFronteira)
	{
		LDeltaSup* pRestricao((LDeltaSup*)pFronteira);

		ItCLDeltaSup LDRestricaoAtual = pRestricao->begin();

		Ponto PPixels;

		PPixels.x = ((LDRestricaoAtual->PCentral.x - Deltas[0]) * Escala);
		PPixels.y = (AlturaDesenhoMetros - (LDRestricaoAtual->PCentral.y - Deltas[1])) * Escala;

		pDC->MoveTo(PPixels.x, PPixels.y);

		for (LDRestricaoAtual++ ; LDRestricaoAtual != pRestricao->end(); ++LDRestricaoAtual)
		{	
			PPixels.x = ((LDRestricaoAtual->PCentral.x - Deltas[0]) * Escala);
			PPixels.y = (AlturaDesenhoMetros - (LDRestricaoAtual->PCentral.y - Deltas[1])) * Escala;

			pDC->LineTo(PPixels.x, PPixels.y);
		}
	}
	if (pDesenharMapa != nullptr && *pDesenharMapa != 0)
	{
		static CPoint CoordMapa(0,0);

		CDC* Pcd(GetDC());

		CSize Size(pPicture->GetSize(Pcd));

		double EscalaDes(AlturaCroquisPixels / Size.cy);

		if (pPicture)
		{
		  pPicture->Draw(&pDC, CoordMapa, EscalaDes);
		}
	}

	if (0 && LarguraVideoOriginalD2 > 3 && AlturaVideoOriginalD2 > 3)
	{
	   pDC->Rectangle(CPointMouse.x - LarguraVideoOriginalD2, CPointMouse.y + AlturaVideoOriginalD2, CPointMouse.x + LarguraVideoOriginalD2, CPointMouse.y - AlturaVideoOriginalD2);
	}
	else
	{
	  pDC->SelectStockObject(WHITE_PEN);
	  pDC->Ellipse(CPointMouse.x - 3, CPointMouse.y - 3, CPointMouse.x + 3, CPointMouse.y + 3);
	  pDC->SelectObject(Pena);
	  pDC->Arc(CPointMouse.x - 4, CPointMouse.y - 4, CPointMouse.x + 4, CPointMouse.y + 4, 0, 0, 0, 0);
	}
}

void DesCroquisView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	pDC->SetBkColor(RGB(0,0,0));
	CView::OnPrepareDC(pDC, pInfo);
}

void DesCroquisView::OnInitialUpdate()
{
	((CMainFrame*)AfxGetMainWnd())->pDesCroquisView2 = this;

	std::string NomeArquivo(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual() + std::string(".ini"));

	char Buffer[512] = { 0 };
	int Posx(0), Posy(0);

	::GetPrivateProfileString(_T("DesenhoGeo"), _T("Croquis"),_T("1,1,0,0"), Buffer, 511, NomeArquivo.c_str());

	std::stringstream strstrBuffer(Buffer);
	char Virgula;

	strstrBuffer >> Estado >> Virgula >> Titulo >> Virgula >> Posx >>Virgula >> Posy;

	Titulo = !Titulo;
	OnCroquisRemovertitulo();

	CMenu* pTopMenu = GetSystemMenu(FALSE);
	if (pTopMenu != NULL)
	{
		pTopMenu->RemoveMenu(SC_SIZE, MF_BYCOMMAND);      //Size
		pTopMenu->RemoveMenu(SC_MOVE, MF_BYCOMMAND);      //Move
		pTopMenu->RemoveMenu(SC_MAXIMIZE, MF_BYCOMMAND);  //Maximize
		pTopMenu->RemoveMenu(SC_CLOSE, MF_BYCOMMAND);  //Maximize
	}

	UpdateData(FALSE);

	CView::OnInitialUpdate();
}

void DesCroquisView::PegaDados(const void* ppFront, const void* ppProjeto, const void* ppFoto, COLORREF* ppCores,double dEscala)
{
  pFronteira = ppFront;
  ppCores = ppCores;
}

void DesCroquisView::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMenu menu;
	if (menu.LoadMenu(IDR_MENUCROQUIS))
	{
		CMenu* pPopup(menu.GetSubMenu(0));
		if (pPopup != NULL)
		{
			CPoint PAux(point);
			ClientToScreen(&PAux);
			pPopup->TrackPopupMenu(TPM_TOPALIGN | TPM_LEFTALIGN, PAux.x + 5, PAux.y + 5, AfxGetMainWnd());

			return;
		}
	}

	CView::OnRButtonDown(nFlags, point);
}

void DesCroquisView::OnCroquisRemovertitulo()
{
	Titulo = !Titulo;

	Titulo ? ((CMDIChildWnd*)this->GetParentFrame())->ModifyStyle(0,WS_CAPTION, SWP_FRAMECHANGED) :
		      ((CMDIChildWnd*)this->GetParentFrame())->ModifyStyle(WS_CAPTION,0, SWP_FRAMECHANGED);
}

void DesCroquisView::OnCroquisSair()
{
}

void DesCroquisView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (Estado == SELECIONANDO)
	{
		CPointMouse = point;
		RedrawWindow();
	}

	CView::OnMouseMove(nFlags, point);
}

void DesCroquisView::OnLButtonDown(UINT nFlags, CPoint point)
{
	Ponto  PCoordMetros;

	TransfPixelsXY(point, PCoordMetros.x, PCoordMetros.y);  //--- Pega as coordenadas (em metros) 

	Estado = !Estado;

	if (Estado == SELECIONADO) pJanDesGeom->SendMessage(VUE_USRMSG_CLICOU_CROQUIS, PCoordMetros.x, PCoordMetros.y);
	else OnMouseMove(nFlags, point);

	CView::OnLButtonDown(nFlags, point);
}

void DesCroquisView::DefineEscala()
{
	Escala = AlturaCroquisPixels / AlturaDesenhoMetros;
	double Temp = LarguraCroquisPixels / LarguraDesenhoMetros;

	if (Temp < Escala)  Escala = Temp;
}

void DesCroquisView::MudouTamDesenho(double NovaAltura, double NovaLargura)
{
	TamVideoOriginal(NovaAltura, NovaLargura);

	RedrawWindow();
}

void DesCroquisView::MoveuDesenho(double Mousex,double Mousey)
{
	CPointMouse.x = ((Mousex - Deltas[0]) * Escala);
	CPointMouse.y = (AlturaDesenhoMetros - (Mousey - Deltas[1])) * Escala;

	RedrawWindow();
}

void DesCroquisView::OnSize(UINT nType, int cx, int cy)
{
	AlturaCroquisPixels = cy;
	LarguraCroquisPixels = cx;

	if (TipoCroquis == TIPO_PONTOS)
	{
		AlturaCroquisPixels /= 1.15;
		LarguraCroquisPixels /= 1.15;
	}

	DefineEscala();
	
	CView::OnSize(nType, cx, cy);
}

DesCroquisView::~DesCroquisView()
{
	((CMainFrame*)AfxGetMainWnd())->pDesCroquisView2 = NULL;
	std::string NomeArquivo(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual() + std::string(".ini"));

	CString CSDados;

	int Posx = 100, Posy = 50;

	CSDados.Format("%d,%d,%d,%d", Estado, Titulo, Posx, Posy);
	
	::WritePrivateProfileString(_T("DesenhoGeo"), _T("Croquis"), CSDados, NomeArquivo.c_str());
}

void DesCroquisView::Rotacionou(void* pSuperficie)
{
	Ponto PNovoSupEsq, PNovoInfDir;
	((CSuperficie*)pSuperficie)->PegaLimites(PNovoSupEsq, PNovoInfDir);

	AlturaDesenhoMetros = PNovoSupEsq.y - PNovoInfDir.y;
	LarguraDesenhoMetros = PNovoInfDir.x - PNovoSupEsq.x;

	DefineEscala();

	Deltas[0] = PNovoSupEsq.x - LarguraDesenhoMetros * .05;
	Deltas[1] = PNovoInfDir.y + AlturaDesenhoMetros * .05;

	RedrawWindow();
}

inline BOOL DesCroquisView::OnEraseBkgnd(CDC* pDC)
{
	return false;
}

void DesCroquisView::TransfPixelsXY(CPoint& Ponto, double& Coorx, double& Coory)
{
	//--- Mapeia as cordenadas do ponto que estão em pixels para met{ros.

	Coorx = ((double)Ponto.x) / Escala + Deltas[0];
	Coory = AlturaDesenhoMetros - ((double)Ponto.y) / Escala + Deltas[1];

}