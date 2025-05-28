#pragma once
#include <afxwin.h>

#define VUE_USRMSG_CLICOU_CROQUIS  WM_APP + 4

class DesCroquisView : public CView
{
protected:

	enum ENUM_ESTADO{ SELECIONANDO,SELECIONADO };

	DECLARE_DYNCREATE(DesCroquisView);
	double AlturaVideoOriginalD2, LarguraVideoOriginalD2;
	CPoint CPointMouse;
	int Estado,Titulo,*pDesenharMapa;
	CPicture* pPicture;
	
public:

	enum TiposCroquis{NENHUM_TIPO,TIPO_PONTOS,TIPO_MAPA};

	TiposCroquis TipoCroquis;

	CView* pJanDesGeom;

	const void* pFronteira;
	COLORREF* pCores;
	double Escala;
	double AlturaCroquisPixels, LarguraCroquisPixels,
		     AlturaDesenhoMetros, LarguraDesenhoMetros,
		Deltas[2];
	int Iniciou;

    DesCroquisView();
	~DesCroquisView();
	
	void PegaDados(const void* ppFront, const void* ppProjeto, const void* ppFoto, COLORREF* ppCores, double dEscala);
	void TamVideoOriginal(double Altura,double Largura) 
	{ 
		AlturaVideoOriginalD2 = Altura / 2.0 * Escala;
		LarguraVideoOriginalD2 = Largura / 2.0 * Escala;
	}

	void JanelaDesGeom(CView* pWnd)	{pJanDesGeom = pWnd;}

	void TransfPixelsXY(CPoint& Ponto, double& Coorx, double& Coory);

	void MudouTamDesenho(double NovaAltura, double NovaLargura);
	void MoveuDesenho(double Mousex, double Mousey);
	void DefineEscala();
	void AtribuiPicture(CPicture* pPic) 
	{ 
		CDC* Pcd = GetDC();
		CSize Size(pPic->GetSize(Pcd));

		double Escala(AlturaCroquisPixels / Size.cy);
		
		pPicture = pPic;
	}
	void DesenharMapa(int* pDesMapa) { pDesenharMapa = pDesMapa; };
	void Tipo(DesCroquisView::TiposCroquis pTipo) { TipoCroquis = pTipo;};
	void Rotacionou(void* pSuperficie);

	double PegaEscala() { return Escala; };

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnDraw(CDC* /*pDC*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnInitialUpdate();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCroquisRemovertitulo();
	afx_msg void OnCroquisSair();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

