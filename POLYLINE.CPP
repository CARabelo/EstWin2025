// PolyLine.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "cponto.h"
#include "clpontos.h"
#include "PolyLine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define   X   0
#define   Y   1

/////////////////////////////////////////////////////////////////////////////
// PolyLine

//IMPLEMENT_DYNCREATE(PolyLine, CFormView)

PolyLine::PolyLine(int Estilo,int Expessura,COLORREF Cor) : Pena(Estilo,Expessura,Cor)
{

}

void PolyLine::DoDataExchange(CDataExchange* pDX)
{
/*
	CView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PolyLine)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	*/
}
/*
BEGIN_MESSAGE_MAP(PolyLine)
	//{{AFX_MSG_MAP(PolyLine)
	ON_COMMAND(ID_PROC_ANTE, OnProcAnte)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// PolyLine diagnostics

#ifdef _DEBUG
void PolyLine::AssertValid() const
{
//	CView::AssertValid();
}
           
void PolyLine::Dump(CDumpContext& dc) const
{
//	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// PolyLine message handlers

void PolyLine::OnDraw(CDC* pDC) 
{
  CPen penBlack,newPen;  // Construct it, then initialize
  if( newPen.CreatePen( PS_SOLID,1, RGB(255,0,0)))
  {
        // Seleciona a pena no device context
        // e salva a pena antiga ao mesmo tempo

    CPen* pOldPen = pDC->SelectObject( &newPen );

	  LPontos.InsereOrdenada(Ponto(0.0,0.0));
	  LPontos.InsereOrdenada(Ponto(500.0,100.0));
	  LPontos.InsereOrdenada(Ponto(600.0,0.0));
	  LPontos.InsereOrdenada(Ponto(700.0,100.0));
 	  LPontos.InsereOrdenada(Ponto(800.0,0.0));

    POSITION Atual(LPontos.GetHeadPosition());
    pDC->MoveTo((int)LPontos.GetAt(Atual).Ordenada[0],(int)LPontos.GetAt(Atual).Ordenada[1]);
		LPontos.GetNext(Atual);

    while(Atual) 
		{
      pDC->LineTo((int)LPontos.GetAt(Atual).Ordenada[0],(int)LPontos.GetAt(Atual).Ordenada[1]);
		  LPontos.GetNext(Atual);
		}
    pDC->SelectObject( pOldPen );
	}
}

void PolyLine::OnProcAnte() 
{
  //static int a(23),b(123);
  //LPontos.InsereOrdenada(Ponto(a+=10,b+=20));	
	//RedrawWindow();
}

