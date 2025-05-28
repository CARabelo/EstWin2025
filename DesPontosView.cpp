// DesSecaoView.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include "xymouse.h"
#include "afxtempl.h"
#include "carqterreno.h"
#include "math.h"
#include "Estaca.h"
#include "cponto.h"
#include "clpontos.h" 
#include "perfil.h"
#include "afxtempl.h"
#include "supercstring.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtalcorte.h"
#include "carqtalaterro.h"
#include "ctalcorte.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "clistaperfis.h"
#include "csecao.h"
#include "clistasecoes.h"
#include "dponto.h"
#include "carqgradientes.h"
#include "cgradientes.h" 
#include "dselsecao.h"
#include "comboprocdessecao.h"
#include "listamedicoes.h"
#include "dvolumes.h"
#include "calcnota.h"

#include <string>

#include "DesSecaoView.h"
#include "mainfrm.h"
#include "dentraponto.h" 
#include "math.h"
#include "ddeslizantes.h"
#include "xygreide.h"
#include "palette.h"
#include "mytoolbar.h"
#include "dtipodespongeo.h"
#include "childfrm.h"
#include "monologo.h"
#include "igualdades.h" 
#include "projeto.h"
#include "string.h"
#include "CalcVolumes.h"
#include "carqcurverticais.h"
#include "ccurverticais.h"
#include "dpontogreide.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
#include "desperfilview.h"
#include<iostream.h>
#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define X        0
#define Y        1
#define ESQUERDO 0
#define DIREITO  1
#define CORTE    0
#define ATERRO   1
#define IGUAL    0
#define GRAVAR   1
#define LER      0  

extern class monologo monolog;     
int Semaforo(100);


/////////////////////////////////////////////////////////////////////////////
// DesSecaoView
IMPLEMENT_DYNCREATE(DesSecaoView, CView)
//
DesSecaoView::DesSecaoView() : 
															 
															 ArrastandoPonto(FALSE),
															 Arrastando(FALSE),
															 DeltaYMetro(0.0),
															 PAtual(NULL),
                               NomeProjeto(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()),
															 Alterou(FALSE),
															 PrimeiraAlteracao(FALSE),
															 TerminarArrasto(FALSE)
{
}

DesSecaoView::~DesSecaoView()
{
	CMainFrame* MainFrame = (CMainFrame*) AfxGetMainWnd();

//	MainFrame->DVolumes->
  //--- Pega os dados iniciais e salva-os

  CString CSDados;

	double Estaca(SecaoAtual ? Secoes.GetAt(SecaoAtual).Estaca.EstVirtual : INFINITO);

	CSDados.Format("%lf,%lf,%lf,%lf,%lf,%lf",Escala[X],Escala[Y],RazaoHV,Estaca,Deltas[X],Deltas[Y]);

	AfxGetApp()->WriteProfileString(_T("DadosDesenhos"),MainFrame->GreideAtivo ? _T("DadosSecComPerfil") : !EMedicaoD? _T("DadosSecSemPerfil") : _T("DadosSecMedicao") ,CSDados);	

  if (AlterouSecao) Secoes.Serialize(GRAVAR);

	MainFrame->DesSecao = NULL;    
}

BEGIN_MESSAGE_MAP(DesSecaoView, CView)
	//{{AFX_MSG_MAP(DesSecaoView)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_BUTPROXIMA, OnButproxima)
	ON_COMMAND(ID_BUTANTERIOR, OnButanterior)
	ON_COMMAND(ID_PRIMEIRASECAO, OnPrimeirasecao)
	ON_COMMAND(ID_ULTIMASECAO, OnUltimasecao)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_PRIMEIRASECAO, OnUpdatePrimeirasecao)
	ON_UPDATE_COMMAND_UI(ID_ULTIMASECAO, OnUpdateUltimasecao)
	ON_WM_CREATE()
	ON_COMMAND(ID_CONFIGTOOLSEC, OnConfigtoolsec)
	ON_WM_SIZE()
	ON_COMMAND(ID_CALCOFFSETS, OnCalcoffsets)
	ON_COMMAND(ID_RESTAURARSECAO, OnRestaurarsecao)
	ON_UPDATE_COMMAND_UI(ID_RESTAURARSECAO, OnUpdateRestaurarsecao)
	ON_UPDATE_COMMAND_UI(ID_CALCOFFSETS, OnUpdateCalcoffsets)
	ON_COMMAND(ID_GRAVAR, OnGravar)
	ON_COMMAND(ID_CALCULO_VOLUMES, OnCalculoVolumes)
	ON_COMMAND(ID_CALCVOLUMES, OnCalcvolumes)
	ON_UPDATE_COMMAND_UI(ID_CALCVOLUMES, OnUpdateCalcvolumes)
	ON_UPDATE_COMMAND_UI(ID_GRAVAR, OnUpdateGravar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DesSecaoView drawing

void DesSecaoView::OnDraw(CDC* pDC)
{
  CPen Pena(PS_SOLID,1,RGB(0,0,0));
	
	for(int QuanPontos = Pontos.size(); QuanPontos ; QuanPontos--,ItLSupAtual++)
	{
		CPonto Pxy((*ItLSupAtual)->PegaPCentral());
		CPoint Ppixels((int) (Deltas[X] + (Pxy.x * Escala[X])),(int) (Deltas[Y] + (Resolucao[Y] - (Pxy.y - CotaInferior) *Escala[Y])));
		
	  //--- Desenha a cruz.
		
	  pDc->MoveTo(Ppixels.x-5,Ppixels.y);
	  pDC->LineTo(Ppixels.x+5.Ppixels.y);
		
	  pDc->MoveTo(Ppixels.x,Ppixels.y-5);
	  pDc->LineTo(Ppixels.x,Ppixels.y+5);
                 
    //--- circula o ponto
                 
	  pDC->Arc(Ppixels.x-3,Ppixels.y-3,Ppixels.x+3,Ppixels.y+3,0,0,0,0);   	
  }
}

/////////////////////////////////////////////////////////////////////////////
// DesSecaoView diagnostics

#ifdef _DEBUG
void DesSecaoView::AssertValid() const
{
	CView::AssertValid();
}

void DesSecaoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// DesSecaoView message handlers

void DesSecaoView::OnMouseMove(UINT nFlags, CPoint point) 
{
 static POSITION PosAnterior(NULL);

  //--- Mostra as coordenadas de onde o mouse est�, Estaca e cota.
	
	DesPerfilView::TransfPixelsXY(point,MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY);  //--- Pega as coordenadas (em metros) 
	                                                                                   //--- do ponto local o mouse est�.
	MouseXYGre->UpdateData(FALSE);

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

    int Tipo;
		CString NumMed;

		POSITION PosMouse(VerfPonto(MouseXYGre->m_DOCoorX,MouseXYGre->m_DOCoorY,Tipo,TipoPonto,NumMed));

		if (PosMouse) 
		{
			MostraPonto(PosMouse,Tipo,TipoPonto,NumMed); 

      //---PosAnterior != pos para evitar chamadas desnecessarias ao desenho da se��o

			if (PosAnterior != PosMouse)   PosAnterior = PosMouse;  //--- Atualiza a anterior
		}
		else if (!DPontoGreide.Limpo) DPontoGreide.LimpaDialogo();  
	}															 
}

void DesSecaoView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

  CChildFrame* ChildFrame =  (CChildFrame*) GetParentFrame();
  CMainFrame* MainFrame = (CMainFrame *) AfxGetMainWnd();

  MouseXY = (xyMouse *) MainFrame->MouseXY;

	//--- Se for medi��o, o projeto e o terreno n�o podem ser alterados, ent�o desabilita os bot�es

	 //--- Pega os dados iniciais e atualiza o desenho

  CString CSStatus(MainFrame->GreideAtivo ? (AfxGetApp()->GetProfileString(_T("DadosDesenhos"),_T("DadosSecComPerfil"),_T("30,30,1.0,0.0,0.0,0.0,5,2"))) :
	                              !EMedicaoD ? (AfxGetApp()->GetProfileString(_T("DadosDesenhos"),_T("DadosSecSemPerfil"),_T("30,30,1.0,0.0,0.0,0.0,5,2"))) : 
                                            (AfxGetApp()->GetProfileString(_T("DadosDesenhos"),_T("DadosSecMedicao"),_T("30,30,1.0,0.0,0.0,0.0,5,2")))); 
	double Estaca;

  _stscanf(CSStatus,"%lf,%lf,%lf,%lf,%lf,%lf.%i,%i",&Escala[X],&Escala[Y],&RazaoHV,&Estaca,&Deltas[X],&Deltas[Y],&Dados[0],&Dados[1],&Dados[2]);

  //--- posiciona a se��o

	SecaoAtual = Secoes.Posiciona(CEstaca(Estaca));   
	if (!SecaoAtual) SecaoAtual = Secoes.PosicionaPrimeira();

	if (SecaoAtual) SecaoAnterior = Secoes.GetAt(SecaoAtual);  //--- Salva a secao sem altera��es, para poder restaur�-la

	//--- Inicializa os valores da escala, da raz�o e da toler�ncia.

	ChildFrame->Deslizantes.MudarEscala((int) Escala[Y]);
	ChildFrame->Deslizantes.MudarRazao(RazaoHV);

	CalculaParametros();

  DPonto.Create(this); 
 	DPonto.ShowWindow(Ponto);

	//--- dialogo dos volumes 

	MainFrame->DPontoSecoes = &DPonto;    //--- Guarda a referencia para uso futuro (Mostrar / esconder o dialogo
	MainFrame->DVolumes= &DVolumes;       //--- Idem

	//--- Preenche o box de procurar
	//--- � preciso limpar os espa�os � esquerda, sen�o as fun�oes do combo box ficam prejudicadas 

	
}

void DesSecaoView::CalculaParametros()
{
  //--- Calcula a Largura e a Altura do video, que depende da resolucao e da escala
	//--- em pixels por metro.
	//--- Dependendo da resolu��o do video a escala deve variar, os valores default s�o:
  //---
  //--- 1024 x 768 -> 20 pixels por metro.
	//--- 640 X 480  -> 12,5 pixels por metro (proporcionalmente), todas as escalas
	//--- tem seu velor default proporcional ao de 1024 x 768, Esta escala varia
	//--- quando o usu�rio pressiona o bot�o + ou o bot�o - (Alargar ou encolher o desenho).

  LarguraVideo = Resolucao[X] / Escala[X];
  AlturaVideo = Resolucao[Y] / Escala[Y];
  
  //--- A escala mudou, acerta as toler�ncias (5 pixels em qualquer eixo);

	Toleranciax = 5.0 / Escala[X];
	Toleranciay = 5.0 / Escala[Y];

  //--- Deslocamento no eixo y / metro

  DeltaYMetro = Deltas[Y] / Escala[Y];    //--- Deslocamento do Arrasto.
}


POSITION DesSecaoView::VerfPonto(double x,double y,CString& NumMed)
{
  //--- Verifica Se o mouse esta sobre algum ponto de terreno, de projeto ou do
	//--- pavimento, caso esteja posiciona o ponteiro do perfil para o ponto em quest�o,
	//--- atrav�s da mensagem " PosicionaPonto";

	PAtual = NULL;
  CSecao& SecAtual = Secoes.GetAt(SecaoAtual);  //--- Pega a se��o atual.

  Tipo = TERRENO;

	PAtual = SecAtual.Terreno.PosicionaPonto(x,y,Toleranciax,Toleranciay);

	if (!PAtual)
	{
	  Tipo = PROJETO;
		PAtual = SecAtual.Projeto.PosicionaPonto(x,y,Toleranciax,Toleranciay);
	}

	if (!PAtual)
	{
	  Tipo = PAVIMENTO;
		PAtual = SecAtual.Pavimento.PosicionaPonto(x,y,Toleranciax,Toleranciay);
	}

	//--- Se n�o achou nada e h� medi��es desenhadas, verifica nas medi��es

	if (!PAtual)
	{
		POSITION MedicaoAtual = ListaMedicoes.GetHeadPosition();

		while (MedicaoAtual && !PAtual)
		{
			CListaSecoes& LMedAtual = *ListaMedicoes.GetNext(MedicaoAtual); 

			POSITION PMedAtual = LMedAtual.PosicionaNoMaisProximo(SecAtual.Projeto.Estaca); 

			if (PMedAtual)
			{
        Tipo = MEDICAO;

				CSecao& SecMedAtual = LMedAtual.GetAt(PMedAtual);
				if (fabs(SecAtual.Projeto.Estaca - SecMedAtual.Projeto.Estaca) < 0.0005)
					PAtual = SecMedAtual.Projeto.PosicionaPonto(x,y,Toleranciax,Toleranciay);
				if (PAtual)NumMed = LMedAtual.PegaProjeto().Right(3);
			}
		}
	}

	return PAtual;
}

void DesSecaoView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//--- O primeiro click em cima de um ponto inicia o arrasto, no segundo click
 	//--- finaliza o arrasto, no primeiro click arrastando ponto fica true at� que o usu�rio click novamente
  //--- quando Terminararrasto fica true, o arrasto termina efetivamente quando o usu�rio tira o dedo do 
  //--- bot�o do mouse no m�todp onlbuttonup. 

	//--- Se j� estiver arrastando coloca true que o arrasto deve ser terminado (no m�todo onlbuttonup) e retorna
	//--- n�o h� nada mais a fazer. 

  if (ArrastandoPonto) 
	{
	  TerminarArrasto = TRUE;
		return;
	}

  Arrastando = TRUE;     //--- Est� arrastando alguma coisa....
	InicioArrast = point;	

	if (SecaoAtual) //-- Se tem	secao desenhada...
  {
    double x,y; 
		POSITION PAtual;
		   
  	TransfPixelsXY(point,x,y);  //--- Pega as coordenadas (em metros) do ponto local o mouse est�.

    if ((PAtual = VerfPonto(x,y)))   		  //--- Se o  mouse estiver sobre algum ponto, inicia o arrasto 
		{
			ArrastandoPonto = !EMedicaoD || Tipo == MEDICAO; //--- Medicao so pode alterar a medicao...	

		  if (!EMedicaoD)        
			{
			  //--- Salva o ponto sendo alterado para poder restaur�-lo posteriormente.

        switch(Tipo)
				{
          case PROJETO: PontoAnterior  = Secoes.GetAt(SecaoAtual).Projeto.GetAt(PAtual); break;
				  case TERRENO: PontoAnterior  = Secoes.GetAt(SecaoAtual).Terreno.GetAt(PAtual); break;
				  case PAVIMENTO: PontoAnterior  = Secoes.GetAt(SecaoAtual).Pavimento.GetAt(PAtual); break;
				}
			}
		}
  }

	CView::OnLButtonDown(nFlags, point);
}

void DesSecaoView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//--- Se estiver arrastando um ponto e for o segundo click (Terminar arrasto = TRUE)
	//--- finaliza o arrasto

	if (TerminarArrasto) 
	{
		Arrastando = ArrastandoPonto = TerminarArrasto = FALSE;
     
    Alterou = AlterouSecao = PrimeiraAlteracao = TRUE;    //--- houve Altera��o na secao

    //--- Se o ponto alterior era o do gradiente e o valor de x foi mudado, precisamos inserir outro ponto no gradiente.

	  CSecao& SecaoAtu = Secoes.GetAt(SecaoAtual);

    if (Tipo == TERRENO && Gradientes.BuscaGradiente(SecaoAtu.Estaca) == PontoAnterior.x)
		{
		  Ponto& PontoAtual = SecaoAtu.Terreno.GetAt(PAtual);     //--- Pega o ponto atual (Alterado)

      //--- Se a diferen�a entre o x do ponto atual e o ponto anterior for pequena, apenas ajusta o x do ponto atual
			//--- para o x do gradiente, caso contrario interpola (ou extrapola) outro ponto no gradiente

		  if (fabs(PontoAnterior.x  - PontoAtual.x) < 0.0005)  PontoAtual.x = PontoAnterior.x;
			else 
			SecaoAtu.Terreno.InsereOrdenada(Ponto(PontoAnterior.x,SecaoAtu.Terreno.Calculay(PontoAnterior.x)));

      //--- o gradiente foi alterado, ent�o acerta a cota na estaca do terreno pois esta � que � usada no desenho do perfil

  		SecaoAtu.Terreno.Estaca.Cota = SecaoAtu.Terreno.PegaPontoAtual().y;   //--- O atual � o gradiente que acabou de ser calculado

			 CMainFrame* MainFrame = (CMainFrame *) AfxGetMainWnd();

      if (MainFrame->GreideAtivo) ((CWnd*)(MainFrame->DesPerfil))->RedrawWindow();    //--- O ponto de terreno do gradiente foi alterado, mostra a altera��o no perfil.
	  }
	}
	else if (!ArrastandoPonto) Arrastando = FALSE;	    //--- Se n�o estiver arrastando um ponto  p�ra de arrastar 
	                                                    //--- o desenho (se estiver arrastando)
	RedrawWindow();
}


void DesSecaoView::TransfPixelsXY(CPoint& Ponto,double& Coorx,double& Coory)
{
  //--- Mapeia as cordenadas do ponto que est�o em pixels para metros.
  //---  
  //--- O c�lculo � feito pela f�rmula.
  //--- Sendo.
  //---  px           -> Valor da coordenada x em pixels.
  //---  p(x)         -> Valor do afastamento (x) do ponto.
  //---  Escala(x)    -> Valor da escala(Pixels/metro) no eixo x.
  //---  Resolu��o(x) -> Valor da resolu��o no eixo x (em pixels).
  //---  D(x)         -> Valor do Deslocamento (tecla seta para a direita ou para a esquerda)
  //---  C(x)         -> Valor da coordenada x do ponto em metros.
  //---
  //---  C(x) = (p(x) - Resolucao(x) / 2 - D(x)) / Esc(x).
  //---

  Coorx = (Ponto.x - ResolucaoXDiv2 - Deltas[X]) / Escala[X];

  //---  E para y:   (O sentido � invertido...)
  //---
  //---  C(y) = (Resolucao(y) - p(y) + D(y)) / Esc[Y] + CotaInferior;
  //---

  Coory = (Resolucao[Y] - Ponto.y + Deltas[Y]) / Escala[Y] + CotaInferior;
}

void DesSecaoView::MostraPonto(int Tipo,CSecao& SecaoAtual,CString& NumMed)
{
  //--- mostra o ponto que est� sob o mouse.

  Perfil& CPerfil = Tipo == TERRENO ? SecaoAtual.Terreno : SecaoAtual.Projeto; 

  POSITION p(PAtual);        //--- PAtual n�o pode ser alterado!!, copia-o em p
  Ponto PontoAtual(CPerfil.GetPrev(p));

	if (EMedicaoD)
	{
		int i = 0;
	}

  double i1(p ? CPerfil.GetAt(p).i : INFINITO);
  DPonto.Atualiza(PontoAtual.x,PontoAtual.y,i1,PontoAtual.i,Tipo,NumMed);

  //--- Atualiza os bot�es com o tipo de perfil, terreno ou projeto.

	if (!EMedicaoD)
	{
    MouseXY->m_RATerreno.SetCheck(Tipo == TERRENO);
    MouseXY->m_RAProjeto.SetCheck(Tipo == PROJETO);
		if (Tipo == TERRENO) MouseXY->m_RATerreno.SetFocus();
	  else MouseXY->m_RAProjeto.SetFocus();

	}
	else MouseXY->m_RAMedicao.SetCheck(Tipo == MEDICAO);
}

void DesSecaoView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  bool AlterouSecao(TRUE);

  if (!SecaoAtual) return;  //--- N�o tem se��o? retorna.

  CSecao& SecAtual = Secoes.GetAt(SecaoAtual);  

	double x,y;
	TransfPixelsXY(point,x,y);  //--- Transforma as coordenadas de pixels para metros.

  //--- Inclui o ponto no perfil.

  if (MouseXY->m_RATerreno.GetCheck()) 
	{
    //--- Se caso estiver criando uma nova se��o de terreno � preciso acertar o valor da estaca e da cota da estaca
		//--- pois ambos est�o com INFINITO para indicar que esta se��o n�o tem terreno.nha terreno.

		if (!SecAtual.Terreno.GetCount())   //--- Se n�o tem terreno, est� inserindo uma nova se��o de terreno...
		{
		  x = Gradientes.BuscaGradiente(SecAtual.Estaca);     //--- O primeiro ponto tem que ser o gradiente.

      //--- Atualiza os valores da estaca do terreno (est�o com infinito pois s se��o n�o tem terreno(agora passa a ter))

			SecAtual.Terreno.Estaca = SecAtual.Projeto.Estaca;  
			SecAtual.Terreno.Estaca.Cota = y;

			//--- O ponto de terreno do gradiente foi incluido, mostra a altera��o no perfil.

      ((CWnd*)((CMainFrame*)AfxGetMainWnd())->DesPerfil)->RedrawWindow();    
		}
		
		SecAtual.Terreno.InsereOrdenada(Ponto(x,y),FALSE,TRUE);
	}
	else  
	{
		if (!EMedicaoD) SecAtual.Projeto.InsereOrdenada(Ponto(x,y,INFINITO,CString("PONINS")),FALSE,TRUE);
		else 
		{
      CSecao* SecMedAtual = PegaSecMedicao(SecAtual.Projeto.Estaca);

			if(SecMedAtual) SecMedAtual->Projeto.InsereOrdenada(Ponto(x,y),FALSE,TRUE);
			else AlterouSecao = FALSE;
		}
	}

  if(AlterouSecao)  
	{
    RedrawWindow();
		Alterou = PrimeiraAlteracao =  TRUE;
	}
}

void DesSecaoView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  if (!SecaoAtual) return;   //--- n�o tem se��o retorna.

  double x,y;
	TransfPixelsXY(point,x,y);  //--- Pega as coordenadas (em metros) do ponto local o mouse est�.

  if (VerfPonto(x,y))  //--- Se o mouse estiver sobre um ponto...
	{                    //--- Deleta-o, mas verifica se � o gradiente pois o gradiente n�o pode ser excluido

    CSecao& SecAtual = Secoes.GetAt(SecaoAtual);  

	  if (Tipo == TERRENO)  
		{
      //--- Se estiver tentando deletar o gradiente avisa que ele n�o pode ser deletado 

			if (Gradientes.BuscaGradiente(SecAtual.Estaca) != SecAtual.Terreno.PegaPontoAtual().x) SecAtual.Terreno.ExcluiPonto();
			else 
			{
				monolog.mensagem(11);   //--- O gradiente n�o pode ser deletado!!!
			  return;
			}
		}
		else if (Tipo == PROJETO) SecAtual.Projeto.ExcluiPonto();
		     else SecAtual.Pavimento.ExcluiPonto();

    Alterou = AlterouSecao = Tipo == TERRENO || Tipo == PROJETO;

		RedrawWindow();   //--- Redesenha a secao, sem o ponto
	} 
}

void DesSecaoView::OnRButtonUp(UINT nFlags, CPoint point) 
{
  static BOOL EstouAqui(FALSE);

  if (!(nFlags & MK_CONTROL)) 
  {		
    if (ArrastandoPonto)    //--- Esta Arrastando um ponto? ent�o o operador desistiu do arrasto
		{
		  CSecao& SecAtual = Secoes.GetAt(SecaoAtual);  //--- Pega a se��o atual.

			//--- Sim, Volta o ponto ao que era no in�cio do arrasto.

		  if (Tipo == PROJETO) SecAtual.Projeto.AlteraPonto(PontoAnterior,PAtual);
			else SecAtual.Terreno.AlteraPonto(PontoAnterior,PAtual);

			Arrastando = ArrastandoPonto = FALSE;

      //--- Mostra o ponto alterado no dialogo.

			MostraPonto(Tipo,SecAtual); 

			RedrawWindow();
		}
		//else OnConfigtoolsec();  //--- Mostra a caixa de configura��o da janela.

		return;  //--- Se a tecla CTRL n�o est� pressionada retorna nada mais a fazer....
  }

	if (EstouAqui) return;   //--- Se j� estiver entrando com um ponto ponto, volta, evita reentradas.

  double x,y;
	TransfPixelsXY(point,x,y);  //--- Pega as coordenadas (em metros) do ponto local o mouse est�.
	POSITION Atual(VerfPonto(x,y));  

  if (Atual)  //--- Se o mouse est� sobre um ponto...
	{           //--- Mostra o di�logo para alterar o ponto.
	  //--- Salva os dados anteriores.

	  CString i1Antes(DPonto.m_STi1), i2Antes(DPonto.m_STi2), xAntes(DPonto.m_STx), yAntes(DPonto.m_STy);

    EstouAqui = TRUE;  //--- Evita reentradas.

    //--- Pede o novo ponto.
   
    CDEntraPonto DEntraPonto(DPonto.m_STi1,DPonto.m_STi2,DPonto.m_STx,DPonto.m_STy,point,Tipo,this);

    //--- Executa o di�logo.

		if (DEntraPonto.DoModal() == IDOK)
		{
			CSecao& SecAtual = Secoes.GetAt(SecaoAtual);  
			Perfil& PerfilAtual = Tipo == PROJETO ? SecAtual.Projeto : SecAtual.Terreno; 

			//--- Se Alterou o x ou o y do ponto, procede a altera��o.

			if (xAntes.Compare(DPonto.m_STx) != 0 || yAntes.Compare(DPonto.m_STy))
				PerfilAtual.AlteraPonto(Ponto(atof(DPonto.m_STx),atof(DPonto.m_STy)));

	  	//--- Se alterou o i1 e/ou o i2, altera o ponto anterior e/ou o ponto posterior.

			if (i1Antes.Compare(DPonto.m_STi1) != 0) PerfilAtual.AlteraRampas(atof(DPonto.m_STi1),ESQUERDO);   
			if (i2Antes.Compare(DPonto.m_STi2) != 0) PerfilAtual.AlteraRampas(atof(DPonto.m_STi2),DIREITO);   
		}

		RedrawWindow(); 
	}
	EstouAqui = FALSE;	
}

void DesSecaoView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CView::OnRButtonDown(nFlags, point);
}

void DesSecaoView::OnCalcoffsets() 
{	  
  Posiciona(INFINITO,-INFINITO);  //--- Mudar o nome para redesenha

  Alterou = FALSE;
}

void DesSecaoView::OnUpdatePrimeirasecao(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(SecaoAtual != Secoes.GetHeadPosition());
}

void DesSecaoView::OnUpdateUltimasecao(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(SecaoAtual != Secoes.GetTailPosition());
}

int DesSecaoView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
 // lpCreateStruct->dwExStyle |= WS_EX_PALETTEWINDOW;
	
	lpCreateStruct->dwExStyle &= !(WS_MAXIMIZEBOX | WS_SYSMENU |WS_MINIMIZEBOX);

	lpCreateStruct->style |= WS_DLGFRAME; 

	if (CView::OnCreate(lpCreateStruct) == -1)	return -1;

	return 0;
}

void DesSecaoView::MudarEscala(int Fator)
{
	Escala[X] = Escala[Y] = Fator;
	Escala[X] *= RazaoHV;
	CalculaParametros();
	RedrawWindow();
}

void DesSecaoView::MudarRazao(double Fator)
{
	RazaoHV = Fator;
	Escala[X] = Escala[Y] * Fator;
	CalculaParametros();
  RedrawWindow();
}

void DesSecaoView::Posiciona(double x,double Cota)
{
  //--- Procura e desenha uma determinada se��o.

	//--- Esse neg�cio de mandar os parametros com INFINITO e -INFINITO acabou que ficou confuso,
	//--- alterar, criando outro parametro indicando o q alterou, o greide, terreno ou a se��o...

	//SecaoAtual =  x == INFINITO ? SecaoAtual : Secoes.Posiciona(CEstaca(x));

	//--- Se x = INFINITO vai iniciar uma nova se��o;

	if (x != INFINITO)
	{
    SecaoAtual = Secoes.Posiciona(CEstaca(x));
		if (SecaoAtual) 
		{
      CEstaca& EstacaAnterior = SecaoAnterior.Estaca;

			SecaoAnterior = Secoes.GetAt(SecaoAtual);  //--- Salva a secao sem altera��es, para poder restaur�-la

      //--- Atualiza o di�logo dos volumes

			DVolumes.Atualiza(SecaoAnterior);
		}

		PrimeiraAlteracao = FALSE;   
	}

  if (SecaoAtual)
	{
		CSecao &SecAtual = Secoes.GetAt(SecaoAtual);

		//--- Se o a cota for = INFINITO a altera��o no greide excluiu o projeto nesta se��o, (o promeiro ou o �ltimo) PIV foi alterdo e posicionado
		//--- antes (ou depois) desta se��o, tirando o projeto desta se��o, So resta ent�o retirar i projeto desta se��o
		
		if (Cota == INFINITO)
		{
			if (!EMedicaoD)
			{
		    SecAtual.Projeto.Destroi();
 		    SecAtual.Pavimento.Destroi();
			}
   	}                                   
		else
		{
      //--- Se cota veio com -infinito o terreno foi alterado

			//-- Houve Altera��o de cota?, ou seja o greide foi alterado? 

			//--- � preciso verificar tamb�m se a se��o esta sem projeto, caso esteja o projeto tem que ser contruido
			//--- pois o usu�rio pode ter retirado o greide nesta se��o e esta recolocando agora

      //--- Nestes casos a se��o tem que ser remontada pois a altera��o pode te-la alterado, de corte 
			//--- para aterro ou o offset no pavimento. 
			//--- Concluindo, Qualquer altera��o implica na remontagem da se��o de projeto.

			if (Cota == -INFINITO || x == INFINITO || (fabs(SecAtual.Estaca.Cota - Cota) > 0.0005) || !SecAtual.Projeto.GetCount()) 
			{ 


        if (Alterou == 0)
				{
          if (fabs(SecAtual.Estaca.Cota - Cota) > 0.0005)
					{
					  int i = 0;
					}

					int i = 0;
				}

        Alterou = TRUE;

				//--- Se houve altera�ao de cota ou recria��o do projeto, a se��o precisa de ser remontada...

        if (!EMedicaoD)   //--- Medi��o n�o se��o tipo, taludes de pavimento etc...
				{	
          //--- Busca os pontos que foram inseridos via desenho e os mantem na se��o de projeto

          CLPontos PontosInseridos;
					PegaPontosInseridos(Secoes.GetAt(SecaoAtual),PontosInseridos); 

					SecTipo SecaoTipo;    //--- lista com os pontos da secao tipo.
					SecoesTipo.BuscaSecTipo(SecAtual.Estaca,SecaoTipo); //--- Insere a se�ao tipo na lista
 
					TaludesPavim TalPavimento;		//-- Cria  o talude de pavimento
					TaludesPavimento.BuscaTaludesPavim(SecAtual.Estaca,TalPavimento); //--- busca os taludes do pavimento
					SecAtual.SecaoTipo = SecaoTipo;   
					SecAtual.Projeto = (CLPontos&) SecaoTipo;
					SecAtual.Pavimento = SecAtual.Projeto;

					//--- Se foram inseridos pontos com o mouse os mant�m na se��o

					if (PontosInseridos.GetCount()) SecAtual.Projeto.InsereLista(PontosInseridos);    
 
					//--- Se alterou o x a cota no eixo tem q ser recalculada

					if (x != INFINITO) SecAtual.Estaca.Cota = SecAtual.Projeto.Estaca.Cota = Cota;    //--- atribui a nova cota da se��o

					SecAtual.AlteraYPontos(Cota);   //--- retira os taludes e os bordos
					SecAtual.CotaSecao(Gradientes.BuscaGradiente(SecAtual.Estaca),0.5/*HPavimento.BuscaH(EstacaAtual)*/);  //--- Cota a se��o de projeto.
					SecAtual.CalculaBordos(TalPavimento,-0.5/*HPavimento.BuscaH(EstacaAtual)*/,Canteiros); //-- Calcula os bordos de pavimenta�ao.

					SecAtual.CalculaOffSets(TaludesCorte,TaludesAterro,FALSE); //--- finalmente, calcula os offsets.
				}
				else 
				{
          //--- Se for medi��o procura pela medi��o, se achar procura pela se��o, se achar
					//--- basta excluir os taludes e recalcular os off-sets, j� que se��o n�o tem se��o tipo alargamentos etc

	        POSITION MedicaoAtual = ListaMedicoes.GetHeadPosition();

				  if (MedicaoAtual)
					{
						CListaSecoes& LMedAtual = *ListaMedicoes.GetAt(MedicaoAtual);
 				    POSITION PMedAtual = LMedAtual.PosicionaNoMaisProximo(SecAtual.Projeto.Estaca); 

				    if(PMedAtual)
						{
					     CSecao& SecMedAtual = LMedAtual.GetAt(PMedAtual);
					    if (fabs(SecMedAtual.Projeto.Estaca - SecAtual.Projeto.Estaca) < 0.0005)
							{
				        SecMedAtual.ExcluiPonTaludes();  
				        SecMedAtual.CalculaOffSets(TaludesCorte,TaludesAterro,TRUE);
							}
						}
					}
				}
			}
		}
	  RedrawWindow();
	 }
}

void DesSecaoView::OnConfigtoolsec() 
{
  CRect Rect;

  GetClientRect(Rect) ;
	ClientToScreen(Rect);

  CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();

	//---Mostra o dialogo, passa os ponteiros para poder alterar os toolbars  in line;

	CDSelSecao SelSecao(this,MainFrame->mpCDesTool1,MainFrame->mpCDesTool2,
		                  MainFrame->mpCDesTool3,MainFrame->mpCDesTool4,MainFrame->DVolumes,Rect,TRUE);

	SelSecao.DoModal();
}

BOOL DesSecaoView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
//  cs.dwExStyle |= WS_EX_PALETTEWINDOW;   //--- Nao funciona!!!!!!!

	 // lpCreateStruct->dwExStyle |= WS_EX_PALETTEWINDOW;
	
	cs.dwExStyle &= !(WS_MAXIMIZEBOX | WS_SYSMENU | WS_MINIMIZEBOX);

	cs.style |= WS_DLGFRAME; 

	return CView::PreCreateWindow(cs);
}

void DesSecaoView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
  //--- Cenmtraliza o desenho dentro do novo tamanho da janela

	RECT Coord; 
  GetClientRect(&Coord);

  Resolucao[X] = Coord.right; 
  Resolucao[Y] = Coord.bottom;

	ResolucaoXDiv2 = Resolucao[X] / 2.0;     //--- Usada nas transforma�es de coordenadas.
	ResolucaoYDiv2 = Resolucao[Y] / 2.0;     //--- Usada nos desenhos dos pontos.   

	CalculaParametros();
}

CSecao* DesSecaoView::PegaSecMedicao(CEstaca& Estaca) 
{
  if (!ListaMedicoes.GetHeadPosition()) return NULL;

	CListaSecoes& LMedAtual = *ListaMedicoes.GetHead();
	POSITION PMedAtual = LMedAtual.PosicionaNoMaisProximo(Estaca); 

	if (PMedAtual)
	{
		CSecao* SecMedAtual = &LMedAtual.GetAt(PMedAtual);

		return (fabs(SecMedAtual->Projeto.Estaca - Estaca) < 0.0005) ? SecMedAtual : NULL;
	}

	return NULL;
}

int DesSecaoView::PegaPontosInseridos(const CSecao& SecaoAtual,CLPontos& PontosInseridos)
{
  if (!SecaoAtual.Projeto.GetCount()) return 0;

	Ponto& PAtual = SecaoAtual.Projeto.GetHead();
	POSITION Atual = SecaoAtual.Projeto.GetHeadPosition();

	while(Atual) 
	{
		 PAtual = SecaoAtual.Projeto.GetNext(Atual);
    if (!PAtual.Descricao.Find("PONINS")) PontosInseridos.AddHead(PAtual);
	}

	return PontosInseridos.GetCount();

}

void DesSecaoView::OnRestaurarsecao() 
{
	Secoes.GetAt(SecaoAtual) = SecaoAnterior;
	RedrawWindow();
  Alterou = PrimeiraAlteracao = FALSE;
}

void DesSecaoView::OnUpdateRestaurarsecao(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(PrimeiraAlteracao);	
}

void DesSecaoView::OnUpdateCalcoffsets(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(Alterou);	
}

void DesSecaoView::OnGravar() 
{
  if (AlterouSecao) Secoes.Serialize(GRAVAR);
}

void DesSecaoView::Gravar() 
{
  if (AlterouSecao) Secoes.Serialize(GRAVAR);
}

void DesSecaoView::OnCalculoVolumes() 
{
	// TODO: Add your command handler code here
	
  int i = 0;

}

void DesSecaoView::OnCalcvolumes() 
{
		CString ppp;

  //--- Grava as altera��es do greide

  if (((CMainFrame*) AfxGetMainWnd())->DesPerfil) 
		  ((DesPerfilView*)((CMainFrame*)AfxGetMainWnd())->DesPerfil)->Gravar();

	Gravar();    //--- Grava as altera��es da se��o
		
  CalcNota DCalcNota(this);
	if (!DCalcNota.DoModal() == IDOK) return;
	 /*

   O semaforo n�o funciona nem a pau, a thread do c�lculo so come�a quando o outro dialogo inicia...??? 

		DCalcNota.Create(&Semaforo,this);
		DCalcNota.ShowWindow(TRUE);
		DCalcNota.CalcularDireto();
		pThread->ResumeThread();

		ppp.Format("%i",Semaforo);
		DCalcNota.SetDlgItemText(1038,ppp);

    int d = Semaforo;

	 		for( ; Semaforo != 1 ; )
		{
				d = Semaforo;

			ppp.Format("%i",d);
		DCalcNota.SetDlgItemText(1038,ppp);

		}
*/
  CalcVolumes DCalcVolumes(NomeProjeto,this);

	if (DCalcVolumes.DoModal() == IDOK)   //--- dialogo do c�lculo dos volumes retornou OK? 
	{
		Alterou = FALSE;

		if (SecaoAtual)
		{
      //--- Salva a estaca atual para poder mostrar os novos volumes dela

		  CEstaca EstacaAtual((Secoes.GetAt(SecaoAtual)).Estaca);
		
		  Secoes.LimpaTudo();            //--- Remove as se�oes atuais
		  Secoes.Serialize(LER);         //--- Le a novas se��es

      SecaoAtual = Secoes.Posiciona(EstacaAtual);

	    Reinicializa();

			RedrawWindow();
		}
	}
}

void DesSecaoView::Reinicializa()
{
  Secoes.DefRetangDes();                   //--- Define os ret�ngulos m�nimos para desenho das sec�es.
	Secoes.CalculaIs();                      //--- Calcula os is das retas das se��es.

  //--- Calcula os is das medi��es

  POSITION MedicaoAtual = ListaMedicoes.GetHeadPosition();

	if(MedicaoAtual) UltMedicao = ListaMedicoes.GetHead();   //-- Salva a �ltima medicao (para altera��es)
	
	while (MedicaoAtual)
	{
	  CListaSecoes& LMedAtual = *ListaMedicoes.GetNext(MedicaoAtual); 
	  LMedAtual.CalculaIs();
	}

	if (Secoes.GetCount() > 0) 
	{
		DVolumes.AtualizaVolumeTotal(Secoes.GetTail());
    DVolumes.Atualiza(Secoes.GetAt(SecaoAtual));
  } 
}

void DesSecaoView::OnUpdateGravar(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(Alterou);	
}

void DesSecaoView::OnUpdateCalcvolumes(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(Alterou);	
}

