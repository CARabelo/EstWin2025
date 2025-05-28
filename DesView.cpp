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
		//--- Desenha o el�stico
		
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
	                                                                                   //--- do ponto local o mouse est�.
	MouseXYPon->UpdateData(FALSE);      //--- Mostra as coordenadas de onde o mouse est�.
	
	if (Arrastando)     //--- Est� arrastando?
	{
		//--- Esta variavel (ArrastandoPonto) se faz necess�ria porque quando o mouse der pulos
		//--- Maiores que a toler�ncia de aproxima��o do ponto (toler�ncias x e y) o ponto n�o 
		//--- seria mais encontrado nas chamadas a PosicionaPonto (do perfil), fazendo-o parar 
		//--- de ser arrastado no meio da opera��o. Esta vari�vel assegura o arrasto do ponto
		//--- Independentemente dos saltos do mouse, ela apenas retorna a FALSE , no final da 
		//--- opera��o, no m�todo LPbuttonUP.

	  if (ArrastandoPonto)    //--- Esta Arrastando um ponto?
		{
			//--- Sim, Altera o ponto.
			
			PontoAtual.x = MouseXYGre->m_DOCoorX;
		  PontoAtual.y = MouseXYGre->m_DOCoorX;
		  
		  Alterou = true;

      //--- Mostra o ponto alterado.

  	 MostraPonto(PAtual,PROJETO,TipoPonto); 
		}
		else     //--- N�o esta arrastando um ponto, ent�o est� arrastando o desenho.
		{
		  Deltas[X] += point.x - InicioArrast.x;  
	    Deltas[Y] += point.y - InicioArrast.y;
		  InicioArrast = point;
		  DeltaYMetro = Deltas[Y] / Escala[Y];    //--- Deslocamento do Arrasto.
		}
	  RedrawWindow();   //--- Esta arrastando alguma coisa, ent�o redesenha a janela.
	}
	else   //--- N�o est� arrastando nada...
	{
    //--- Se o mouse estiver sobre algum ponto mostra as coordenadas do ponto.
	  //--- Caso contr�rio, limpa o dialogo do ponto , caso ele n�o esteja limpo.

		if (Pontos.find(MouseXY->m_DOCoorX,MouseXY->m_DOCoorY)) 
		{
			MostraPonto(PosMouse); 

      //---PosAnterior != pos para evitar chamadas desnecessarias ao desenho da se��o

			if (PosAnterior != PosMouse)   PosAnterior = PosMouse;  //--- Atualiza a anterior
		}
		else if (!DPontoGreide.Limpo) DPontoGreide.LimpaDialogo();  
	}
}

noclickbotdir()
{
	//--- O Bot�o direito � reservado para edi��es de acidentes topogr�ficos
	
	DesPerfilView::TransfPixelsXY(point,MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY);  //--- Pega as coordenadas (em metros) 
	 
	CPonto PontoMouse = Pontos.find(CPonto(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY));
	 
	//--- Se o click foi sobre um ponto e se estiver criando um acidente topogr�fico, inclui o ponto na lista, selele j� n�o estiver nela 
	//--- Se o click foi sobre um ponto e n�o estiver criando o acidente topografico inicia uma lista de acidente topografico. 

  if(PontoMouse)
  {	 
	  if (CriandoAcidenteTopografico)
	  {
	    if (!LAcidenteAtual.find(PontoAtual))LAcidenteAtual.push_back(PontoMouse);
	  }
	}
	else Arrastando = !Arrastando;      //--- Bot�o direito no fundo do desenho, inicio ou fim do arrasto do desenho.
} 

no clickbotaoesq()
{
	//--- O Bot�o esquerdo � reservado para edi��es do ponto
	
	DesPerfilView::TransfPixelsXY(point,MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY);  //--- Pega as coordenadas (em metros) 
	 
	CPonto& PontoMouse = Pontos.Find(CPonto(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY));
	
	if (PontoMouse != Pontos.end())
  {
  	if (!ArrastandoPonto)       //--- Se n�o estiver arrastando um ponto inicia o arrasto
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
{                                                     //--- insere na lista de acidentes topogr�ficos 
	if (CriandoAcidenteTopografico)
	{  		  
	  LAcidentesTopograficos.PushBack(LAcidenteAtual);
	  LAcidenteAtual.Limpatudo();
	  CriandoAcidenteTopografico = false;
	}
}