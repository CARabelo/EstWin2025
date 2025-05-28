// DesView.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "carqterreno.h"
#include "math.h"
#include "Estaca.h"
#include "cponto.h"
#include "clpontos.h" 
#include "perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "DesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DesView

IMPLEMENT_DYNCREATE(DesView, CView)

DesView::DesView(bool DesCruz,bool CircPonto,CString& NomeProj):Superficie(NomeProj), DesenhaCruz(DesCruz), CirculaPonto(CircPonto)
{
}

DesView::~DesView()
{
}

BEGIN_MESSAGE_MAP(DesView, CView)
	//{{AFX_MSG_MAP(DesView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DesView drawing

void DesView::OnDraw(CDC* pDC)
{
	CPen Pena(PS_SOLID,1,RGB(0,0,0));
	
	CPonto Pxy;
	CPoint Ppixels;
	
	CPoint Ppixels((int) (Deltas[X] + (Pxy.x * Escala[X])),(int) (Deltas[Y] + (Resolucao[Y] - (Pxy.y - CotaInferior) *Escala[Y])));
		
	for(int QuanPontos = Pontos.size(),ItLSupAtual = CLPontos.begin(); QuanPontos ; QuanPontos--,ItLSupAtual++)
	{
		if (DesenhaCruz)
		{
	    //--- Desenha a cruz.
		
	    pDc->MoveTo(Ppixels.x-5,Ppixels.y);
	    pDC->LineTo(Ppixels.x+5.Ppixels.y);		
	    pDc->MoveTo(Ppixels.x,Ppixels.y-5);
	    pDc->LineTo(Ppixels.x,Ppixels.y+5);
	  }
     
     //--- circula o ponto
                 
	  if (CirculaPonto) pDC->Arc(Ppixels.x-3,Ppixels.y-3,Ppixels.x+3,Ppixels.y+3,0,0,0,0);   	
  }
  
  if (CriandoAcidenteTopografico)
	{ 
		//--- Desenha o elástico
		
		pDc->MoveTo(PAtualPixels.x,PAtualPixels.y);
		pDc->LineTo(PontoMousePixelsAtual.x,PontoMousePixelsAtual);   
	}
}

/////////////////////////////////////////////////////////////////////////////
// DesView diagnostics

#ifdef _DEBUG
void DesView::AssertValid() const
{
	CView::AssertValid();
}

void DesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// DesView message handlers

void DesView::OnMouseMove(UINT nFlags, CPoint& point) 
{
  static POSITION PosAnterior(NULL);
  
  PontoMousePixelAtual = point;
	
	DesPerfilView::TransfPixelsXY(point,MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY);  //--- Pega as coordenadas (em metros) 
	                                                                                   //--- do ponto local o mouse está.
	MouseXYPon->UpdateData(FALSE);      //--- Mostra as coordenadas de onde o mouse está.
	
	if (Arrastando)     //--- Está arrastando?
	{
		//--- Esta variavel (ArrastandoPonto) se faz necessária porque quando o mouse der pulos
		//--- Maiores que a tolerância de aproximação do ponto (tolerâncias x e y) o ponto não 
		//--- seria mais encontrado nas chamadas a PosicionaPonto (do perfil), fazendo-o parar 
		//--- de ser arrastado no meio da operação. Esta variável assegura o arrasto do ponto
		//--- Independentemente dos saltos do mouse, ela apenas retorna a FALSE , no final da 
		//--- operação, no método LPbuttonUP.

	  if (ArrastandoPonto)    //--- Esta Arrastando um ponto?
		{
			//--- Sim, Altera o ponto.
			
			PontoAtual.x = MouseXYGre->m_DOCoorX;
		  PontoAtual.y = MouseXYGre->m_DOCoorX;
		  
		  Alterou = true;

      //--- Mostra o ponto alterado.

  	 MostraPonto(PAtual,PROJETO,TipoPonto); 
		}
		else     //--- Não esta arrastando um ponto, então está arrastando o desenho.
		{
		  Deltas[X] += point.x - InicioArrast.x;  
	    Deltas[Y] += point.y - InicioArrast.y;
		  InicioArrast = point;
		  DeltaYMetro = Deltas[Y] / Escala[Y];    //--- Deslocamento do Arrasto.
		}
	  RedrawWindow();   //--- Esta arrastando alguma coisa, então redesenha a janela.
	}
	else   //--- Não está arrastando nada...
	{
    //--- Se o mouse estiver sobre algum ponto mostra as coordenadas do ponto.
	  //--- Caso contrário, limpa o dialogo do ponto , caso ele não esteja limpo.

		if (Pontos.find(MouseXY->m_DOCoorX,MouseXY->m_DOCoorY)) 
		{
			MostraPonto(PosMouse); 

      //---PosAnterior != pos para evitar chamadas desnecessarias ao desenho da seção

			if (PosAnterior != PosMouse)   PosAnterior = PosMouse;  //--- Atualiza a anterior
		}
		else if (!DPontoGreide.Limpo) DPontoGreide.LimpaDialogo();  
	}
}

noclickbotdir()
{
	//--- O Botão direito é reservado para edições de acidentes topográficos
	
	DesPerfilView::TransfPixelsXY(point,MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY);  //--- Pega as coordenadas (em metros) 
	 
	CPonto PontoMouse = Pontos.find(CPonto(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY));
	 
	//--- Se o click foi sobre um ponto e se estiver criando um acidente topográfico, inclui o ponto na lista, selele já não estiver nela 
	//--- Se o click foi sobre um ponto e não estiver criando o acidente topografico inicia uma lista de acidente topografico. 

  if(PontoMouse)
  {	 
	  if (CriandoAcidenteTopografico)
	  {
	    if (!LAcidenteAtual.find(PontoAtual))LAcidenteAtual.push_back(PontoMouse);
	  }
	}
	else Arrastando = !Arrastando;      //--- Botão direito no fundo do desenho, inicio ou fim do arrasto do desenho.
} 

no clickbotaoesq()
{
	//--- O Botão esquerdo é reservado para edições do ponto
	
	DesPerfilView::TransfPixelsXY(point,MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY);  //--- Pega as coordenadas (em metros) 
	 
	CPonto& PontoMouse = Pontos.Find(CPonto(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY));
	
	if (PontoMouse != Pontos.end())
  {
  	if (!ArrastandoPonto)       //--- Se não estiver arrastando um ponto inicia o arrasto
  	{
  		PontoAnterior = PontoMouse;   //--- Salva o ponto para poder desfazer
  		
  		PontoAtual = PontoMouse;
  		Arrastandoponto = true;
  	}
  	else ArrastandoPonto = false;
  }
}

no doubleclickbotaoesq(UINT nFlags, CPoint& point)     //--- Insere um ponto no local do mouse
{                                                      //--- Se o CTRL estiver presionado deleta o ponto.
	if (!(nFlags && CONTROL)) 
	{
		double Cota(0,0);
		CDPedeCota DPedeCota(&Cota);
		
		if (DPedeCota.DoModal() == OK)
		{
		  Pontos.insert(CPonto(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY,Cota));
		  refresh();
		}
	}	
	else 
	{
		CPonto& PRasc(Pontos.find(CPonto(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY));
		
		if (PRasc != Pontos.end()) 	
		{
			Pontos.Remove(PRasc);
		  refresh();
		}
	}
}

no doubleclickbotaodir(UINT nFlags, CPoint& point)    //--- Termina o acidente topografico atual e o 
{                                                     //--- insere na lista de acidentes topográficos 
	if (CriandoAcidenteTopografico)
	{  		  
	  LAcidentesTopograficos.PushBack(LAcidenteAtual);
	  LAcidenteAtual.Limpatudo();
	  CriandoAcidenteTopografico = false;
	}
}