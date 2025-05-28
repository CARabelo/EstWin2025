#include "stdafx.h"
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "Mainfrm.h"
#include "CPonto.h"
#include "CLPontos.h"
#include "CSecaoAvulsa.h"
#include "SuperCString.h"
#include "CArqSecoesAvulsas.h"
#include "CSecoesAvulsas.h"
//#include "cdpopuppontos.h"
#include "DesPerfilAvulsoView.h"
//#include "CPonto.h"

#define X        0
#define Y        1

IMPLEMENT_DYNCREATE(DesPerfilAvulsoView, CView)
BEGIN_MESSAGE_MAP(DesPerfilAvulsoView, CView)
END_MESSAGE_MAP()

DesPerfilAvulsoView::DesPerfilAvulsoView() :
	PerfisAvulsos(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual())),
	ArrastandoPonto(FALSE),
	Arrastando(FALSE), DeltaYMetro(0.0),
	Alterou(FALSE),
	TerminarArrasto(FALSE),
	PermitirArrastar(true),
	CircularPontos(true),
//	DPopUpPontos(this, RGB(255, 255, 166)),
	EZoom(false),
	LimiteEsq(INFINITO),
	LimiteDir(-INFINITO),
	LimiteSup(-INFINITO),
	LimiteInf(INFINITO),
	PonMousePixelAtual(0, 0),
	CursorAtual(IDC_HAND),
	PAtual(NULL),
	Projeto(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual())
{
	Resolucao[X] = 1024.0; Resolucao[Y] = 768.0;
	Escala[X] = Escala[Y] = 100.0;
	Deltas[X] = Deltas[Y] = 0.0;

	const CList<CSecaoAvulsa, CSecaoAvulsa&>&	PerfisAvulsos(PerfisAvulsos.PegaLSecoesAvulsas());

	if (PerfisAvulsos.GetSize() > 0)
	{
		//--- Atualiza os limites

		for (POSITION Atual = PerfisAvulsos.GetHeadPosition(); Atual != NULL; PerfisAvulsos.GetNext(Atual))
		{
      CSecaoAvulsa SecAvAtual(PerfisAvulsos.GetAt(Atual));

      /*
      citlstPontos itPontoAtual(PerfisAvulsos.GetAt(Atual).GetHead());

			if(itPontoAtual != PerfisAvulsos.GetAt(Atual).PegaListaPontos().end())
			{
				if (itPontoAtual->x != INFINITO && itPontoAtual->y != INFINITO)
				{
					if (itPontoAtual->y > LimiteSup) LimiteSup = itPontoAtual->y;
					if (itPontoAtual->y < LimiteInf) LimiteInf = itPontoAtual->y;
					if (itPontoAtual->x < LimiteEsq) LimiteEsq = itPontoAtual->x;
					if (itPontoAtual->x > LimiteDir) LimiteDir = itPontoAtual->x;
				}
			}
      */
		}
	}
}

DesPerfilAvulsoView::~DesPerfilAvulsoView()
{
}


void DesPerfilAvulsoView::OnDraw(CDC*)
{
}
