// CDiaDadosHidrologicos.cpp : arquivo de implementação
//
#include "stdafx.h"
#include "Est.h"
#include "CPonto.h"
#include "cdeltasuperficie.h"
#include "CLPontos.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CDadosBacias.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "ImageListBox.h"
#include "CDiaDadosHidrologicos.h"
#include "afxdialogex.h"
#include "Mat.h"
#include "Monologo.h"
#include "Dialogo.h"

//--- Métodos de cálculo do tempo de concentração

#define CALIFORNIA_CULVERTS_PRACTICE 1
#define KIRPICH 2 
#define PICKING 3
#define KERBY_HATHAWAY 4
#define DNOS 5
#define KERBY 6
#define KIRPICH_CALIFORNIA_CULVERT_PRACTICE 7
#define US_CORPS_OF_ENGINEERS 8
#define KIRPICH_MODIFICADA 9
#define GEORGE_RIBEIRO 10
#define PASINI 11
#define VENTURA 12
#define ROSSI 13
#define GIANDOTTI 14
#define JOHN_COLLINS 15
#define VEN_TE_CHOW 16


//--- Métodos de cálculo da vazão

#define RACIONAL 1
#define RACIONAL_CORRIGIDO 2
#define I_PAI_WU 3
#define QC_VEN_TE_CHOW 4
#define MCMATH 5
#define KOKEI_UEHARA 6
#define HIDROG_TRIANG_SINTETICO 7

#define VUE_USRMSG_ROLOU_IMAGE_LIST  WM_APP + 5

extern Matem Mat;
extern monologo monolog;
extern dialogo dialogar;

// caixa de diálogo de CDiaDadosHidrologicos

IMPLEMENT_DYNAMIC(CDiaDadosHidrologicos, CDialogEx)

CDiaDadosHidrologicos::CDiaDadosHidrologicos(CBacia& pBacia, CWnd* pParent,int pCalculou): CDialogEx(IDD_DIADADOSHIDROL, pParent),
 douDeclivEquiv(0.0),douTempoConc_Tc(0.0), douCoefForma(0.0), douCoeVolEscoam(0.0),
 douCoeEscSuperf(0.0), douVolEscoHidrog(0.0), douVazaoCheia(0.0), douVazaoMaxProj(0.0), douVelocMediaMann(0.0),
 douRaioHidraulico(0.0), douVazaoContMann(0.0), douVazaoMannContinuidade(0.0), douIntensChuva_i(0.0), douDuracaoChuva_t(0.0),
 douRetornoChuva_T(0.0), douItenschuva_it(0.0),Calcular(false),Calculado(false),pDadosBacia(&pBacia.PegaDadosBacia()), douCoefRugosidadeCanal(0.0),
 douCoefDistrEspa_K(0.0), douDeclividadeCanal(0.0),douAreaMolhadaCanal(0.0),Bacia(pBacia),FatorLinear(1.0),FatorArea(1.0),Calculou(pCalculou),
 douFatorDefluvio_X(0.0), douFatorClimatico_Y(0.0), douFatorRedTempo_Z(0.0), douFatorRetardancia_N(0.0), douFatorCaracteristicas_K(0.0),
 douCoefRetardo_Teta(0.0), douCoefArmazenamento_Ct(0.0), douInsensidadeMaxima_I(0.0),douTempoRetardamento_Tr(0.0),douRaioHidraulicoCanal(0.0),
 douDeclivMediaBacia_I(0.0),douqm(0.0),douVegetacao(0.0), pSuperficie(nullptr),LisMetodosTc(this), LisMetodosQc(this),intQtdBueiros(0),douDiaMinimoBueiro(0.0)
{
}

void CDiaDadosHidrologicos::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIDECEQUIV_S, douDeclivEquiv);
	DDX_Text(pDX, IDC_EDITEMPOCONC_Tc, douTempoConc_Tc);
	DDX_Text(pDX, IDC_EDICOEFICIENTEFORMA_C1, douCoefForma);
	DDX_Text(pDX, IDC_EDICOEFIVOLUMESCOAM_C2, douCoeVolEscoam);
	DDX_Text(pDX, IDC_EDICOEFESCOAMSUPERFICIAL_C, douCoeEscSuperf);
	DDX_Text(pDX, IDC_EDIVOLESCOAMHROGRAMA_V, douVolEscoHidrog);
	DDX_Text(pDX, IDC_EDIVAZAODECHEIA_Q, douVazaoCheia);
	DDX_Text(pDX, IDC_EDIVAZAODECHEIA_Q2, douVazaoMaxProj);
	DDX_Text(pDX, IDC_EDIVELMEDIAMANN, douVelocMediaMann);
	DDX_Text(pDX, IDC_EDIRAIOHIDRAULICOCANAL, douRaioHidraulicoCanal);
	DDX_Text(pDX, IDC_EDIVELMEDIAMANN3, douVazaoContMann);
	DDX_Text(pDX, IDC_EDIVELMEDIAMANN4, douVazaoMannContinuidade);
	DDX_Text(pDX, IDC_EDITEMPODURACAOCHUVA_t, douDuracaoChuva_t);
	DDX_Text(pDX, IDC_EDITEMPORETORNOCHUVA_t, douRetornoChuva_T);
	DDX_Text(pDX, IDC_EDIINTENSCHUVA_it, douItenschuva_it);
	DDX_Text(pDX, IDC_EDCOEFDISTRESPACIAL_K, douCoefDistrEspa_K);
	DDX_Text(pDX, IDC_EDICOEFRUGOSIDADE, douCoefRugosidadeCanal);
	DDX_Text(pDX, IDC_EDIDECLIVCANAL, douDeclividadeCanal);
	DDX_Text(pDX, IDC_EDIAREAMOLHADACANAL, douAreaMolhadaCanal);
	DDX_Text(pDX, IDC_EDIDEFLUVIO_X, douFatorDefluvio_X);
	DDX_Text(pDX, IDC_EDIFATORCLIMATICO_Y, douFatorClimatico_Y);
	DDX_Text(pDX, IDC_EDIFATORREDTEMPO_Z, douFatorRedTempo_Z);
	DDX_Text(pDX, IDC_EDIFATORRETARDANCIA_N, douFatorRetardancia_N);
	DDX_Text(pDX, IDC_EDIFATORCARACBACIA_K, douFatorCaracteristicas_K);
	DDX_Text(pDX, IDC_EDICOEFRETARDO_TETA, douCoefRetardo_Teta);
	DDX_Text(pDX, IDC_EDICOEFARMAZ_CT, douCoefArmazenamento_Ct);
	DDX_Text(pDX, IDC_EDIINTENSMAX_I, douInsensidadeMaxima_I);
	DDX_Text(pDX, IDC_EDI_TR, douTempoRetardamento_Tr);
	DDX_Text(pDX, IDC_EDIDECLIVMEDIABACIA, douDeclivMediaBacia_I);
	DDX_Text(pDX, IDC_EDIRAIOHIDRAULICO, douRaioHidraulico);
	DDX_Text(pDX, IDC_EDI_QM, douqm);
	DDX_Text(pDX, IDC_EDIFATORVEGETACAO_Pr, douVegetacao);
	DDX_Text(pDX, IDC_EDIDIAMINBUEIRO, douDiaMinimoBueiro);
	DDX_Text(pDX, IDC_EDIQTDBUEIROS, intQtdBueiros);
	DDX_Control(pDX, IDC_BUTCALCULAR, ButCalcular);
	DDX_Control(pDX, IDC_BUTCALCULARCANAL, ButCalcularCanal);
	DDX_Control(pDX, IDC_LISTC, LisMetodosTc);
	DDX_Control(pDX, IDC_LISQC, LisMetodosQc);
	DDX_Control(pDX, IDC_RADPONTE, RadPonte);
	DDX_Control(pDX, IDC_RADBSTC, RadBSTC);
}

BEGIN_MESSAGE_MAP(CDiaDadosHidrologicos, CDialogEx)
   ON_BN_CLICKED(IDC_BUTCALCULAR, &CDiaDadosHidrologicos::OnBnClickedButcalcular)
   ON_BN_CLICKED(IDOK, &CDiaDadosHidrologicos::OnBnClickedOk)
   ON_EN_CHANGE(IDC_EDIINTENSCHUVA_it, &CDiaDadosHidrologicos::MudouParametros)
   ON_EN_CHANGE(IDC_EDITEMPODURACAOCHUVA_t, &CDiaDadosHidrologicos::MudouParametros)
   ON_EN_CHANGE(IDC_EDITEMPORETORNOCHUVA_t, &CDiaDadosHidrologicos::MudouParametros)
   ON_EN_CHANGE(IDC_EDCOEFDISTRESPACIAL_K, &CDiaDadosHidrologicos::MudouParametros)
   ON_EN_CHANGE(IDC_EDICOEFIVOLUMESCOAM_C2, &CDiaDadosHidrologicos::MudouParametros)
   ON_EN_CHANGE(IDC_EDICOEFRUGOSIDADE, &CDiaDadosHidrologicos::MudouParametrosCanal)
   ON_EN_CHANGE(IDC_EDIRAIOHIDRAULICOCANAL, &CDiaDadosHidrologicos::MudouParametrosCanal)
   ON_EN_CHANGE(IDC_EDIDECLIVCANAL, &CDiaDadosHidrologicos::MudouParametrosCanal)
   ON_EN_CHANGE(IDC_EDIAREAMOLHADACANAL, &CDiaDadosHidrologicos::MudouParametrosCanal)
   ON_EN_CHANGE(IDC_EDIFATORRETARDANCIA_N, &CDiaDadosHidrologicos::MudouParametros)
   ON_EN_CHANGE(IDC_EDIFATORCARACBACIA_K, &CDiaDadosHidrologicos::MudouParametros)
   ON_EN_CHANGE(IDC_EDIDEFLUVIO_X, &CDiaDadosHidrologicos::MudouParametros)
   ON_EN_CHANGE(IDC_EDIFATORCLIMATICO_Y, &CDiaDadosHidrologicos::MudouParametros)
   ON_EN_CHANGE(IDC_EDIFATORREDTEMPO_Z, &CDiaDadosHidrologicos::MudouParametros)
   ON_EN_CHANGE(IDC_EDICOEFRETARDO_TETA, &CDiaDadosHidrologicos::MudouParametros)
   ON_EN_CHANGE(IDC_EDICOEFARMAZ_CT, &CDiaDadosHidrologicos::MudouParametros)
   ON_EN_CHANGE(IDC_EDIINTENSMAX_I, &CDiaDadosHidrologicos::MudouParametros)
   ON_EN_CHANGE(IDC_EDI_QM, &CDiaDadosHidrologicos::MudouParametros)
	 ON_EN_CHANGE(IDC_EDIFATORVEGETACAO_Pr, &CDiaDadosHidrologicos::MudouParametros)
   ON_LBN_SELCHANGE(IDC_LISQC, &CDiaDadosHidrologicos::OnLbnSelchangeLisqc)
   ON_LBN_SELCHANGE(IDC_LISTC, &CDiaDadosHidrologicos::OnLbnSelchangeListc)
   ON_BN_CLICKED(IDC_BUTCALCULARCANAL, &CDiaDadosHidrologicos::OnBnClickedButcalcularcanal)
   ON_MESSAGE(VUE_USRMSG_ROLOU_IMAGE_LIST, OnRolouImageList)
   ON_UPDATE_COMMAND_UI(IDC_BUTCALCULAR, OnUpdateButCalcular)
END_MESSAGE_MAP()

BOOL CDiaDadosHidrologicos::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ImagensTc.Create(IDB_BITFORMULAS_TC,190,3040, RGB(127, 127, 127));
	LisMetodosTc.SetImageList(&ImagensTc);

	for (int i = 0; i < 16; i++)
	{
		LisMetodosTc.AddItem(i);
	}

	ImagensQc.Create(IDB_BITFORMULAS_QC, 160, 1900, RGB(127, 127, 127));
	LisMetodosQc.SetImageList(&ImagensQc);

	for (int i = 0; i < 7 ; i++)
	{
		LisMetodosQc.AddItem(i);
	}

	ButCalcular.EnableWindow(Calculou);
	ButCalcularCanal.EnableWindow(Calculou);

	RadPonte.SetCheck(true);

	if (pDadosBacia != nullptr)
	{
		douDeclivEquiv = pDadosBacia->S;
		douTempoConc_Tc = pDadosBacia->Tc; 
		douCoefForma = pDadosBacia->C1; 
		douCoeVolEscoam = pDadosBacia->C2;
		douCoeEscSuperf = pDadosBacia->C; 
		douVolEscoHidrog = pDadosBacia->V; 
		douVazaoCheia = pDadosBacia->Q; 
		douVazaoMaxProj = pDadosBacia->Qp; 
		douVelocMediaMann = pDadosBacia->VM;
		douRaioHidraulico = pDadosBacia->RH; 
		douVazaoMannContinuidade = pDadosBacia->QMc;
		douItenschuva_it = pDadosBacia->i;
		douDuracaoChuva_t = pDadosBacia->t;
		douRetornoChuva_T = pDadosBacia->T;
		douCoefRugosidadeCanal = pDadosBacia->n;
		douCoefDistrEspa_K = pDadosBacia->K;
		douVazaoContMann = pDadosBacia->Qc;
		douDeclivMediaBacia_I = pDadosBacia->I; 
		douAreaMolhadaCanal = pDadosBacia->Am;
		douFatorDefluvio_X = pDadosBacia->Fator_X;
		douFatorClimatico_Y = pDadosBacia->Fator_Y;
		douFatorRedTempo_Z = pDadosBacia->Fator_Z;
		douFatorRetardancia_N = pDadosBacia->Fator_N;
		douFatorCaracteristicas_K = pDadosBacia->Fator_K;
		douCoefRetardo_Teta = pDadosBacia->CoefRetardo_Teta;
		douCoefArmazenamento_Ct = pDadosBacia->CT;
		douInsensidadeMaxima_I = pDadosBacia->Im;
		douTempoRetardamento_Tr = pDadosBacia->Tr;
		douRaioHidraulicoCanal = pDadosBacia->rh;
		douDeclividadeCanal = pDadosBacia->I_Canal;
		douDiaMinimoBueiro = pDadosBacia->D_Bueiro;
		intQtdBueiros = pDadosBacia->QtdBueiros;
		douqm = pDadosBacia->qm;
		douVegetacao = pDadosBacia->Pr;
		douDeclivEquiv = Mat.Arredonda(douDeclivEquiv, 6);
		douTempoConc_Tc = Mat.Arredonda(douTempoConc_Tc, 1);
		douCoeEscSuperf = Mat.Arredonda(douCoeEscSuperf, 2);
		douCoefForma = Mat.Arredonda(douCoefForma, 2);
		douVolEscoHidrog = Mat.Arredonda(douVolEscoHidrog, 1);
		douVazaoCheia = Mat.Arredonda(douVazaoCheia, 1);
		douVazaoMaxProj = Mat.Arredonda(douVazaoMaxProj, 1);
		douRaioHidraulico = Mat.Arredonda(douRaioHidraulico, 1);
		douVazaoContMann = Mat.Arredonda(douVazaoContMann, 1);
		douDeclivMediaBacia_I = Mat.Arredonda(douDeclivMediaBacia_I, 6);
		douAreaMolhadaCanal = Mat.Arredonda(douAreaMolhadaCanal, 2);
		douFatorDefluvio_X = Mat.Arredonda(douFatorDefluvio_X,4);
		douFatorClimatico_Y = Mat.Arredonda(douFatorClimatico_Y,4);
		douFatorRedTempo_Z = Mat.Arredonda(douFatorRedTempo_Z,4);
		douFatorRetardancia_N = Mat.Arredonda(douFatorRetardancia_N,4);
		douFatorCaracteristicas_K = Mat.Arredonda(douFatorCaracteristicas_K,4);
		douCoefRetardo_Teta = Mat.Arredonda(douCoefRetardo_Teta,4);
		douRaioHidraulicoCanal = Mat.Arredonda(douRaioHidraulicoCanal,3);
		douDiaMinimoBueiro = Mat.Arredonda(douDiaMinimoBueiro,1);
		douqm = Mat.Arredonda(douqm,1);
		douVegetacao = Mat.Arredonda(douVegetacao,2);
		LisMetodosQc.SetCurSel(pDadosBacia->MetodoCalculoQ-1);
		LisMetodosTc.SetCurSel(pDadosBacia->MetodoCalculoTc-1);

		LisMetodosQc.SetCurSel(-1);
		LisMetodosTc.SetCurSel(-1);

		UpdateData(FALSE);
	}

	OnRolouImageList(0,0);

	ButCalcular.EnableWindow(Calculou);
	ButCalcularCanal.EnableWindow(Calculou);

	FatorLinear = 1.0E3;
	FatorArea = 1.0E6;

	if (Calculou)
	{
		if(douItenschuva_it != 0.0 || douDuracaoChuva_t != 0.0)
		{
		  monolog.mensagem(-1,_T("Os dados hidrologicos desta bacia estao obsoletos, precisam ser recalculados."));
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
								// EXCEPTION: as páginas de propriedade OCX devem retornar FALSE
}

void CDiaDadosHidrologicos::OnBnClickedButcalcular()
{
  UpdateData(TRUE);

	if (douItenschuva_it > 0.0 && douDuracaoChuva_t > 0.0 && douRetornoChuva_T > 0.0 && douCoeVolEscoam > 0.0 && douCoefDistrEspa_K > 0.0)
	{
	  if(Bacia.PegaParticoesTalveguePrincipal().size() == 1)
		{ 
			if (dialogar.DoModal(-1,"O Talvegue principal desta bacia nao esta particionado, deseja calcular os dados hidrologicos mesmo assim ? ") != IDOK)
			{
				monolog.mensagem(-1, "Para Particioná-lo vá em Desenhos - Seções - e clique no botão <talvegues>)", MB_OK | MB_ICONEXCLAMATION);

				return;
			}
		}

		Bacia.CalciMedioTalvPrincipal();

		if (fabs(pDadosBacia->S) < 1e-5)
		{
			monolog.mensagem(-1, "Erro: O talvegue principal desta bacia esta plano, precisa ser revisado!! ");

			return;
		}

		//---Está calculando 2x (na 1a fase tb) e esta dando diferença, verificar 18/10/2023
	  ///CalculaDeclividadeEquivalente();

		Bacia.CG() = CalculaCG();

		pDadosBacia->S = douDeclivEquiv;

		pDadosBacia->MetodoCalculoQ = LisMetodosQc.GetTopIndex()+1;//ComMetodoCalculo.GetCurSel();

		if(pDadosBacia->MetodoCalculoQ !=  -1)
		{ 
		  //--- Passa os parametros

			pDadosBacia->t = douDuracaoChuva_t;
			pDadosBacia->T = douRetornoChuva_T;
			pDadosBacia->i = douItenschuva_it;
			pDadosBacia->K = douCoefDistrEspa_K;
			pDadosBacia->C2 = douCoeVolEscoam;
			pDadosBacia->n = douCoefRugosidadeCanal;

			Bacia.PegaDadosBacia().LCG = Bacia.CG().S = pDadosBacia->LCG = hypot(Bacia.PegaFoz().x - Bacia.CG().x,Bacia.PegaFoz().y - Bacia.CG().y);

			douTempoConc_Tc = CalculaTc(LisMetodosTc.GetTopIndex()+1);
			douCoefForma = 4.0 / (2.0 + pDadosBacia->Kf);
			douCoeEscSuperf = 2.0 / (1.0 + pDadosBacia->Kf) * pDadosBacia->C2 / pDadosBacia->C1;
		  douVolEscoHidrog = 0.278 * pDadosBacia->C2 * pDadosBacia->i * (douTempoConc_Tc / 60.0) * 3.6 * pow(pDadosBacia->Area/FatorArea, 0.9) * pDadosBacia->K  * 1.5 * 10.0;
			douVazaoCheia = CalculaQ(LisMetodosQc.GetTopIndex()+1);
			douVazaoMaxProj = douVazaoCheia * 1.1;
						
			pDadosBacia->RH = douRaioHidraulico ;
			pDadosBacia->V = douVolEscoHidrog;
			pDadosBacia->Q = douVazaoCheia;
			pDadosBacia->C = douCoeEscSuperf;
			pDadosBacia->Tc = douTempoConc_Tc;

	    Calculado = true;
		  Calcular = false;

			//--- Arredondamentos

			douDeclivEquiv = Mat.Arredonda(douDeclivEquiv,6);
			douTempoConc_Tc = Mat.Arredonda(douTempoConc_Tc, 1);
			douCoeEscSuperf = Mat.Arredonda(douCoeEscSuperf, 2);
			douCoefForma = Mat.Arredonda(douCoefForma, 2);
			douVolEscoHidrog = Mat.Arredonda(douVolEscoHidrog, 1);
			douVazaoCheia = Mat.Arredonda(douVazaoCheia, 1);
			douVazaoMaxProj = Mat.Arredonda(douVazaoMaxProj, 1);
			douRaioHidraulico = Mat.Arredonda(douRaioHidraulico, 1);
			douVazaoContMann = Mat.Arredonda(douVazaoContMann, 1);
			douVelocMediaMann = Mat.Arredonda(douVelocMediaMann, 1);
			douVazaoMannContinuidade = Mat.Arredonda(douVazaoMannContinuidade, 1);
			douVelocMediaMann = Mat.Arredonda(douVelocMediaMann, 1);
			douTempoRetardamento_Tr = Mat.Arredonda(douTempoRetardamento_Tr,1);

			UpdateData(FALSE);
		}
  }

	Calculou = false;
	ButCalcular.EnableWindow(false);
}

CDiaDadosHidrologicos::~CDiaDadosHidrologicos()
{
}

void CDiaDadosHidrologicos::OnBnClickedOk()
{
	if (pDadosBacia != nullptr)
	{
	  UpdateData(TRUE);

		pDadosBacia->S = douDeclivEquiv;
		pDadosBacia->Tc = douTempoConc_Tc;
		pDadosBacia->C1 = douCoefForma;
		pDadosBacia->C = douCoeEscSuperf;
		pDadosBacia->V = douVolEscoHidrog;
		pDadosBacia->Q = douVazaoCheia;
		pDadosBacia->Qp = douVazaoMaxProj;
		pDadosBacia->VM = douVelocMediaMann;
		pDadosBacia->RH = douRaioHidraulico;
		pDadosBacia->QMc = douVazaoMannContinuidade;
		pDadosBacia->MetodoCalculoQ = LisMetodosQc.GetTopIndex()+1;
		pDadosBacia->MetodoCalculoTc = LisMetodosTc.GetTopIndex()+1;
		pDadosBacia->Qc = douVazaoContMann;
		pDadosBacia->I = douDeclivMediaBacia_I;
		pDadosBacia->Am = douAreaMolhadaCanal;
		pDadosBacia->Fator_X = douFatorDefluvio_X;
		pDadosBacia->Fator_Y = douFatorClimatico_Y;
		pDadosBacia->Fator_Z = douFatorRedTempo_Z;
		pDadosBacia->Fator_N = douFatorRetardancia_N;
		pDadosBacia->Fator_K = douFatorCaracteristicas_K;
		pDadosBacia->CoefRetardo_Teta = douCoefRetardo_Teta;
		pDadosBacia->CT = douCoefArmazenamento_Ct;
		pDadosBacia->Im = douInsensidadeMaxima_I;
		pDadosBacia->Tr = douTempoRetardamento_Tr;
		pDadosBacia->rh = douRaioHidraulicoCanal;
		pDadosBacia->D_Bueiro = douDiaMinimoBueiro;
		pDadosBacia->QtdBueiros = intQtdBueiros;
		pDadosBacia->qm = douqm;
		pDadosBacia->Pr = douVegetacao;
	}
	CDialogEx::OnOK();
}

void CDiaDadosHidrologicos::OnBnClickedButcalcfinais()
{
	UpdateData(TRUE);

	douVelocMediaMann = 1.0 / pDadosBacia->n * pow(pDadosBacia->RH, 2.0 / 3.0) * pow(pDadosBacia->i, 0.5);
	douVazaoMannContinuidade = 1.0 / pDadosBacia->n * pow(pDadosBacia->RH, 2.0 / 3.0) * pDadosBacia->Area / FatorArea  * sqrt(pDadosBacia->i);

	douVelocMediaMann = Mat.Arredonda(douVelocMediaMann,1);
	douVazaoMannContinuidade = Mat.Arredonda(douVazaoMannContinuidade, 1);

	UpdateData(FALSE);
}

void CDiaDadosHidrologicos::CalculaDeclividadeEquivalente()
{
  double L(0.0), xAnterior(Bacia.PegaPerfTalvPrincipal().begin()->x),yAnterior(Bacia.PegaPerfTalvPrincipal().begin()->y),
	       DxAtual(0.0),Denominador(0.0);

	//--- Inclui a ultima particao, para fechar o while

	Bacia.PegaParticoesTalveguePrincipal().insert(Ponto(Bacia.PegaPerfTalvPrincipal().rbegin()->x, Bacia.PegaPerfTalvPrincipal().rbegin()->y));

	itsetPontos itPartAtual(Bacia.PegaParticoesTalveguePrincipal().begin());

  do
	{
		DxAtual = itPartAtual->x - xAnterior;

		double IMedioAtual(DxAtual / sqrt(fabs(yAnterior - itPartAtual->y) / (DxAtual)));   //--- VERIFICAR!!! o perfil tem q estar descendo

		Denominador += IMedioAtual;

		xAnterior = itPartAtual->x;
		yAnterior = itPartAtual->y;

	}while(++itPartAtual != Bacia.PegaParticoesTalveguePrincipal().end());

	L = Bacia.PegaPerfTalvPrincipal().rbegin()->x - Bacia.PegaPerfTalvPrincipal().begin()->x;

	douDeclivEquiv = pow(L / Denominador,2.0);
}

void CDiaDadosHidrologicos::MudouParametros()
{
	UpdateData(TRUE);

	int MetodoTc(LisMetodosTc.GetTopIndex() + 1),
		  MetodoQ(LisMetodosQc.GetTopIndex() + 1);
	bool DadosGerais(douItenschuva_it > 0.0 && douDuracaoChuva_t > 0.0 && douRetornoChuva_T > 0.0 && douCoeVolEscoam > 0.0 && douCoefDistrEspa_K > 0.0),
		   ResQ(DadosGerais), ResTc(DadosGerais);

	switch (MetodoQ)
	{
	  case RACIONAL_CORRIGIDO: ResQ &= douCoefRetardo_Teta > 0.0; break;
	  case QC_VEN_TE_CHOW: ResQ &= douFatorDefluvio_X > 0.0 && douFatorClimatico_Y > 0.0 && douFatorRedTempo_Z > 0.0; break;
	  case KOKEI_UEHARA: ResQ &=  douCoefArmazenamento_Ct > 0.0 && douInsensidadeMaxima_I > 0.0; break;
	  case HIDROG_TRIANG_SINTETICO: ResQ &= douqm > 0.0;
	}

	switch (MetodoTc)
	{
	  case KERBY_HATHAWAY: ResTc &= douFatorRetardancia_N > 0.0; break;
	  case DNOS: ResTc &= douFatorCaracteristicas_K > 0.0; break;
	  case GEORGE_RIBEIRO: ResTc &= douVegetacao > 0.0;break;
	}

	ButCalcular.EnableWindow(ResQ & ResTc);
	
	Calcular = ResQ && ResTc;
	Calculado = false;
}

void CDiaDadosHidrologicos::MudouParametrosCanal()
{
	UpdateData(TRUE);

	if (douCoefRugosidadeCanal > 0.0 && douRaioHidraulicoCanal > 0.0 && douDeclividadeCanal >= 0.0 && douAreaMolhadaCanal > 0.0)
	{
		Calcular = true;
		Calculado = false;
		ButCalcularCanal.EnableWindow(true);
	}
	else
	{
		ButCalcularCanal.EnableWindow(false);
	}
}

void CDiaDadosHidrologicos::OnBnClickedButcalcularcanal()
{
  UpdateData(true);

	pDadosBacia->n = douCoefRugosidadeCanal;
	pDadosBacia->rh = douRaioHidraulicoCanal;
	pDadosBacia->I_Canal = douDeclividadeCanal;
	pDadosBacia->Am = douAreaMolhadaCanal;
	pDadosBacia->Fator_X = douFatorDefluvio_X;
	pDadosBacia->Fator_Y = douFatorClimatico_Y;
	pDadosBacia->Fator_Z = douFatorRedTempo_Z;
	pDadosBacia->Fator_N = douFatorRetardancia_N;
	pDadosBacia->Fator_K = douFatorCaracteristicas_K;
	pDadosBacia->CoefRetardo_Teta = douCoefRetardo_Teta;
	pDadosBacia->qm = douqm;
	pDadosBacia->Pr = douVegetacao;

	douVelocMediaMann = 1.0 / pDadosBacia->n * pow(pDadosBacia->rh, 2.0 / 3.0) * pow(douDeclividadeCanal, 0.5);
	douVazaoContMann = douVelocMediaMann * douAreaMolhadaCanal;
	douVazaoMannContinuidade = 1.0 / pDadosBacia->n * pow(pDadosBacia->rh, 2.0 / 3.0) * pDadosBacia->Area * sqrt(douDeclividadeCanal) / FatorArea;

	douVazaoContMann = Mat.Arredonda(douVazaoContMann, 1);
	douVelocMediaMann = Mat.Arredonda(douVelocMediaMann, 1);
	douVazaoMannContinuidade = Mat.Arredonda(douVazaoMannContinuidade, 1);

	douDiaMinimoBueiro = 0.0;

  if(1) //if (RadPonte.GetCheck() == false)
  {
		double  Diam1(0.80);
		bool Ok(false);
		int QtdBueiros(1);

		while (!Ok && Diam1 < 10.00)
		{
			double cos_teta_div2((1.0 - ((2.0 * Diam1 * 0.80) / Diam1))),
				     teta(acos(cos_teta_div2) * 2.0),
				     Ac((teta - sin(teta) / 8.0) * Diam1 * Diam1),
				     hc(Diam1 * 0.8),
				     Qc(Ac * sqrt(9.81 * hc));

			if (Qc > douVazaoMannContinuidade)
			{
			  Ok = true;
			}
			else 
			{
			  Diam1 += 0.2;
			}
		}

		double AreaNecessaria((pow((Diam1 / 2), 2) * Matem::M_PI)),
			   Diam2(0.6),Diam3(0.6);

		if (Diam1 > 2.5)
		{
		  QtdBueiros = 2;
			Ok = false;

			while (!Ok && Diam2 < 2.5)
			{
			  double AreaAtual(pow((Diam2 /2),2) * Matem::M_PI * 2.0); 

			  if (AreaAtual > AreaNecessaria)
			  {
				  Ok = true;
			  }
			  else
			  {
				  Diam2 += 0.2;
			  }
			}
		}

    if (Diam2 > 2.5)
    {
		  QtdBueiros = 3;
      Ok = false;

      while (!Ok && Diam3 < 2.5)
      {
        double AreaAtual(pow((Diam3 / 2), 2) * Matem::M_PI * 2.0);

        if (AreaAtual > AreaNecessaria)
        {
          Ok = true;
        }
        else
        {
          Diam3 += 0.2;
        }
      }
    }

		switch (QtdBueiros)
		{
			case 1: douDiaMinimoBueiro = Diam1; break;
			case 2: douDiaMinimoBueiro = Diam2; break;
			case 3: douDiaMinimoBueiro = Diam3; break;
		}

		intQtdBueiros = QtdBueiros;
	}

	ButCalcularCanal.EnableWindow(false);

	UpdateData(false);
}

double CDiaDadosHidrologicos::CalculaTc(int Metodo)
{
	double Resultado(INFINITO);
	int i(0);

	switch(Metodo)
	{
	  case CALIFORNIA_CULVERTS_PRACTICE: Resultado = 57.0 * pow((pow(pDadosBacia->Lt, 2.0) / pDadosBacia->h), 0.385 / FatorLinear); break;
  	  case KIRPICH: Resultado = 0.95 * pow(pow(pDadosBacia->Lt,3.0) / pDadosBacia->h,0.385) / FatorLinear; break;
	  case PICKING: Resultado = 5.32 * pow(pow(pDadosBacia->Lt,2.0) / douDeclivEquiv,0.33) / 100.0; break;
	  case KERBY_HATHAWAY: Resultado = 37.1 * pow(douFatorRetardancia_N,0.47) * pow(pDadosBacia->Lt,0.47) * pow(douDeclivEquiv,-0.235);break;
	  case DNOS: Resultado = 10.0 * pow(pDadosBacia->Area,0.3) * pow(pDadosBacia->Lt,0.2) / (douFatorCaracteristicas_K * pow(douDeclivEquiv,0.4)) ; break;
	  case KERBY: Resultado = 37.0 * pow(((pDadosBacia->Lt * 0.50) / douDeclivEquiv),0.47) / FatorLinear; break;
	  case KIRPICH_CALIFORNIA_CULVERT_PRACTICE: Resultado = 0.95 * pow((pow(pDadosBacia->Lt,3.0) / pDadosBacia->h),0.385) / FatorLinear; break;
	  case US_CORPS_OF_ENGINEERS: Resultado = 0.30 * pow((pDadosBacia->Lt / pow(douDeclivEquiv, 0.25)),0.76) / 100.0; break;
	  case KIRPICH_MODIFICADA: Resultado = 1.42 * pow((pow(pDadosBacia->Lt,3.0) / pDadosBacia->h),0.385) / FatorLinear; break;
	  case GEORGE_RIBEIRO: Resultado = (16.0 * pDadosBacia->Lt) / ((1.05 - 0.2 * douVegetacao) * pow((100.0 * douDeclivEquiv),0.04)) / (FatorLinear *10.0); break;
	  case PASINI: Resultado = (0.107 * pow(pDadosBacia->Area * pDadosBacia->Lt,1.0 /3.0)) / sqrt(douDeclivEquiv) / 100.0; break;
	  case VENTURA: Resultado = 0.127 * sqrt(pDadosBacia->Area * douDeclivEquiv); break;
	  case ROSSI: Resultado = 0.77 * pow(pDadosBacia->Lt / sqrt(douDeclivEquiv),0.295); break;
	  case GIANDOTTI: Resultado = (4.0 * sqrt(pDadosBacia->Area) + 1.50 * pDadosBacia->Lt) / (0.80 * pDadosBacia->h); break;
	  case JOHN_COLLINS: Resultado = pow((pow(pDadosBacia->Area,2.0) / douDeclivEquiv),0.20) * (pDadosBacia->Lt / (4.0 * pDadosBacia->Area / Matem::M_PI) * 44.0); break;
	  case VEN_TE_CHOW:  Resultado = 25.2 * pow((pDadosBacia->Lt / sqrt(douDeclivEquiv)), 0.64) / FatorLinear; break;
	}

	return Resultado;
}

double CDiaDadosHidrologicos::CalculaQ(int Metodo)
{
	double Resultado(INFINITO);

	switch (Metodo)
	{
	  case RACIONAL: Resultado = douCoeEscSuperf * pDadosBacia->i * (pDadosBacia->Area / FatorArea) / 3.6 ; break;
	  case RACIONAL_CORRIGIDO:  Resultado = douCoeEscSuperf * pDadosBacia->i * (pDadosBacia->Area / FatorArea) / 3.6 * douCoefRetardo_Teta ; break;
	  case I_PAI_WU: Resultado = 0.278 * douCoeEscSuperf * pDadosBacia->i * pow(pDadosBacia->Area / FatorArea , 0.9) * pDadosBacia->K / 100.0;	break;
	  case QC_VEN_TE_CHOW: Resultado = (pDadosBacia->Area / FatorArea) * douFatorDefluvio_X * douFatorClimatico_Y * douFatorRedTempo_Z / 3.6 ;break;
	  case MCMATH: Resultado = 91.0 * pDadosBacia->i * (pow(pDadosBacia->Area / FatorArea,0.8)) * (pow(pDadosBacia->S,.8)); break;
	  case KOKEI_UEHARA : Resultado = CalculaKokeiUehara();
  	case HIDROG_TRIANG_SINTETICO : Resultado = (0.20836 * (pDadosBacia->Area / FatorArea) * douqm) / (sqrt(douTempoConc_Tc) + douTempoConc_Tc * 0.6);
	}

	return Resultado;
}

void CDiaDadosHidrologicos::OnLbnSelchangeLisqc()
{
	LisMetodosQc.SetCurSel(-1);

	MudouParametros();
}

void CDiaDadosHidrologicos::OnLbnSelchangeListc()
{
	LisMetodosTc.SetCurSel(-1);

	GetDlgItem(IDC_EDIFATORRETARDANCIA_N)->EnableWindow(false);
	GetDlgItem(IDC_STA_N)->EnableWindow(false);
	GetDlgItem(IDC_EDIFATORCARACBACIA_K)->EnableWindow(false);
	GetDlgItem(IDC_STA_K)->EnableWindow(false);
	GetDlgItem(IDC_STA_SETA_TC)->EnableWindow(false);

	GetDlgItem(IDC_EDIDEFLUVIO_X)->EnableWindow(false);
	GetDlgItem(IDC_EDIFATORCLIMATICO_Y)->EnableWindow(false);
	GetDlgItem(IDC_EDIFATORREDTEMPO_Z)->EnableWindow(false);

	GetDlgItem(IDC_STA_X)->EnableWindow(false);
	GetDlgItem(IDC_STA_Y)->EnableWindow(false);
	GetDlgItem(IDC_STA_Z)->EnableWindow(false);
	GetDlgItem(IDC_STA_SETA_Q)->EnableWindow(false);

	GetDlgItem(IDC_STA_CT)->EnableWindow(false);
	GetDlgItem(IDC_EDICOEFARMAZ_CT)->EnableWindow(false);
	GetDlgItem(IDC_STA_I)->EnableWindow(false);
	GetDlgItem(IDC_EDIINTENSMAX_I)->EnableWindow(false);
	GetDlgItem(IDC_STA_I)->EnableWindow(false);
	GetDlgItem(IDC_STA_TR)->EnableWindow(false);
	GetDlgItem(IDC_STAMIN_TR)->EnableWindow(false);
	GetDlgItem(IDC_EDI_TR)->EnableWindow(false);
	GetDlgItem(IDC_STA_MM_H)->EnableWindow(false);

	GetDlgItem(IDC_EDI_QM)->EnableWindow(false);
	GetDlgItem(IDC_STA_QM)->EnableWindow(false);

	GetDlgItem(IDC_EDICOEFRETARDO_TETA)->EnableWindow(false);
	GetDlgItem(IDC_STA_TETA)->EnableWindow(false);

	GetDlgItem(IDC_EDIFATORVEGETACAO_Pr)->EnableWindow(false);
	GetDlgItem(IDC_STA_FATOR_VEGETACAO)->EnableWindow(false);

	if(LisMetodosTc.GetTopIndex()+1 == KERBY_HATHAWAY)
	{
	  GetDlgItem(IDC_EDIFATORRETARDANCIA_N)->EnableWindow(true);
	  GetDlgItem(IDC_STA_N)->EnableWindow(true);
	  GetDlgItem(IDC_EDIFATORCARACBACIA_K)->EnableWindow(false);
	  GetDlgItem(IDC_STA_SETA_TC)->EnableWindow(true);
	}
	else
	{
	  if (LisMetodosTc.GetTopIndex() + 1 == DNOS)
	  {
	    GetDlgItem(IDC_EDIFATORCARACBACIA_K)->EnableWindow(true);
	  	GetDlgItem(IDC_STA_K)->EnableWindow(true);
	    GetDlgItem(IDC_STA_SETA_TC)->EnableWindow(true);
	  }
		else
		{
			if (LisMetodosTc.GetTopIndex() + 1 == GEORGE_RIBEIRO)
			{
				GetDlgItem(IDC_EDIFATORVEGETACAO_Pr)->EnableWindow(true);
				GetDlgItem(IDC_STA_FATOR_VEGETACAO)->EnableWindow(true);
			}
		}
	}

	if (LisMetodosQc.GetTopIndex() + 1 == QC_VEN_TE_CHOW)
	{
	  GetDlgItem(IDC_EDIDEFLUVIO_X)->EnableWindow(true);
	  GetDlgItem(IDC_EDIFATORCLIMATICO_Y)->EnableWindow(true);
	  GetDlgItem(IDC_EDIFATORREDTEMPO_Z)->EnableWindow(true);

	  GetDlgItem(IDC_STA_X)->EnableWindow(true);
	  GetDlgItem(IDC_STA_Y)->EnableWindow(true);
	  GetDlgItem(IDC_STA_Z)->EnableWindow(true);
	  GetDlgItem(IDC_STA_SETA_Q)->EnableWindow(true);
	}
	else
	{
  	if (LisMetodosQc.GetTopIndex() + 1 == RACIONAL_CORRIGIDO)
	  {
	    GetDlgItem(IDC_EDICOEFRETARDO_TETA)->EnableWindow(true);
	 	  GetDlgItem(IDC_STA_TETA)->EnableWindow(true);
		  GetDlgItem(IDC_STA_SETA_Q)->EnableWindow(true);
	  }
	  else
	  {
			if (LisMetodosQc.GetTopIndex() + 1 == KOKEI_UEHARA)
			{
				GetDlgItem(IDC_STA_CT)->EnableWindow(true);
				GetDlgItem(IDC_EDICOEFARMAZ_CT)->EnableWindow(true);
				GetDlgItem(IDC_STA_I)->EnableWindow(true);
				GetDlgItem(IDC_EDIINTENSMAX_I)->EnableWindow(true);
				GetDlgItem(IDC_STA_I)->EnableWindow(true);
				GetDlgItem(IDC_EDIINTENSMAX_I)->EnableWindow(true);
				GetDlgItem(IDC_STA_TR)->EnableWindow(true);
				GetDlgItem(IDC_STAMIN_TR)->EnableWindow(true);
				GetDlgItem(IDC_EDI_TR)->EnableWindow(true);
				GetDlgItem(IDC_STA_MM_H)->EnableWindow(true);
			}
			else
			{
				if (LisMetodosQc.GetTopIndex() + 1 == HIDROG_TRIANG_SINTETICO)
				{
					GetDlgItem(IDC_EDI_QM)->EnableWindow(true);
					GetDlgItem(IDC_STA_QM)->EnableWindow(true);
				}
			}
	  }
	}

	MudouParametros();
}

LRESULT CDiaDadosHidrologicos::OnRolouImageList(WPARAM WP, LPARAM LP)  //--- Rolou a lista de imagens das formulas
{
	OnLbnSelchangeListc();

	return true;
}

Ponto CDiaDadosHidrologicos::CalculaCG()  
{
// http://dan-scientia.blogspot.com/2009/10/centroide-de-um-poligono.html  

	double Numeradorx(0.0), Denominadorx(0.0), Numeradory(0.0), Denominadory(0.0), Somax(0.0), Somay(0.0);
	itlstitsetPontos ItEspigaoAnterior(Bacia.EspigaoBacia.LstItPontos.begin()), ItEspigaoAtual(ItEspigaoAnterior);

	Bacia.EspigaoBacia.LstItPontos.push_back(*Bacia.EspigaoBacia.LstItPontos.begin());   //--- Insere O ultimo ponto. (Tem q ser = ao primeiro, poligono fechado)
	double AreaS(0.0);

	for (ItEspigaoAtual++; ItEspigaoAtual != Bacia.EspigaoBacia.LstItPontos.end(); ItEspigaoAtual++, ItEspigaoAnterior++)
	{
	  double Multiplicador((((*ItEspigaoAnterior)->x * (*ItEspigaoAtual)->y) - ((*ItEspigaoAtual)->x * (*ItEspigaoAnterior)->y)));
	  double Deltax(((*ItEspigaoAnterior)->x + (*ItEspigaoAtual)->x) * Multiplicador),
		       Deltay(((*ItEspigaoAnterior)->y + (*ItEspigaoAtual)->y) * Multiplicador);

	  Somax += Deltax;
	  Somay += Deltay;
		AreaS += ((*ItEspigaoAnterior)->x * (*ItEspigaoAtual)->y) - ((*ItEspigaoAtual)->x * (*ItEspigaoAnterior)->y);   
	}

	AreaS = fabs(AreaS) / 2.0;
	//--- ===> VERIFICAR 16/09/2023
	//--- As 2 areas estão ligeiramente diferentes mas o bastante para produzir deslocar bastante o CG em pequenas bacias.
	//-- Parece que o cálculo do site não é bem a área
	//Ponto CG(fabs(Somax) / Bacia.PegaDadosBacia().Area / 6.0,fabs(Somay) / Bacia.PegaDadosBacia().Area / 6.0);
	Ponto CG(fabs(Somax) / AreaS / 6.0,fabs(Somay) / AreaS / 6.0);

	Bacia.EspigaoBacia.LstItPontos.pop_back();    //--- Deixa como estava.

	return CG;
}

double CDiaDadosHidrologicos::CalculaKokeiUehara()
{
	CDadosBacia& DB(Bacia.PegaDadosBacia());

	DB.Tr = douTempoRetardamento_Tr = 0.75 * douCoefArmazenamento_Ct * pow((DB.Lt * DB.LCG), 0.3);
  DB.Td = douTempoRetardamento_Tr /4.0;
	DB.h1 = DB.Td * douInsensidadeMaxima_I;
	DB.h1c = DB.K * DB.h1;
	DB.Hexc = DB.C * DB.h1c;	
	DB.Vesc = DB.Area /1.0E3 * DB.Hexc,
	DB.Tb = 3.0 * DB.Tc;

	double Q(2.0 * DB.Vesc / DB.Tb * 3.6E2);

	return Q;
}

void CDiaDadosHidrologicos::OnUpdateButCalcular(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(Calculou);
}

