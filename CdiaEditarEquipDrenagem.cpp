// CdiaEditarEquipDrenagem.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "CDiaEditarEquipDrenagem.h"
#include "afxdialogex.h"
#include "CPonto.h"
#include <set>
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "Mat.h"

extern class Matem Mat;

// caixa de diálogo de CdiaEditarEquipDrenagem

IMPLEMENT_DYNAMIC(CdiaEditarEquipDrenagem, CDialogEx)

CdiaEditarEquipDrenagem::CdiaEditarEquipDrenagem(double *pDim,int pQtdDim,const std::string& PNome,int pEquip,CWnd* pParent,double pEsconsidade,double pAzimute):
                         CDialogEx("IDD_DIAEDITAREQUIPDRENAGEM", pParent),pDimensoes(pDim),Nome(PNome),QtdDimensoes(pQtdDim),
                         Equipamento(pEquip),dEsconsidade(pEsconsidade),Azimute(pAzimute),bForcar90Graus(false)
{
  for(int C = 0 ; C < pQtdDim ; C++)
	{
	  if(pDimensoes[C] == INFINITO) pDimensoes[C] = 0.0;
	}

	Dim_A = pDimensoes[0]; 
	Dim_B = pDimensoes[1]; 
	Dim_C = pDimensoes[2]; 
	Dim_D = pDimensoes[3]; 
	Dim_E = pDimensoes[4];
	Dim_F = pDimensoes[5]; 
	Dim_G = pDimensoes[6];

	BueiroBMP.LoadBitmap(IDB_BITBUEIRO);
	TubulacaoBMP.LoadBitmap(IDB_BITTUBULACAO);
	SarjetaBMP.LoadBitmap(IDB_BITSARJETA);
	DescidaDaguaBMP.LoadBitmap(IDB_BITDESCIDADAGUA);
	CaixaColetoraBMP.LoadBitmap(IDB_BITCAIXACOLETORA);
	BaciaDissipacaoBMP.LoadBitmap(IDB_BITBACIADISSIPACAO);
	ValetaTerraplBMP.LoadBitmap(IDB_BITVALETAOFFSET);
	ValetaTNBMP.LoadBitmap(IDB_BITVALETA);
	CanaletaBMP.LoadBitmap(IDB_BITCANALETATN);
	CanaletaTerrapBMP.LoadBitmap(IDB_BITCANALETAOFFSET);
}

CdiaEditarEquipDrenagem::~CdiaEditarEquipDrenagem()
{
}

void CdiaEditarEquipDrenagem::DoDataExchange(CDataExchange* pDX)
{
	if (Equipamento == CAciTopografico::EQUI_BUEIRO) Dim_D = Dim_C;

	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_RADFORCAR90GRAUS, bForcar90Graus);
	DDX_Text(pDX, IDC_EDIDIM_A, Dim_A);
	DDX_Text(pDX, IDC_EDIDIM_B, Dim_B);
	DDX_Text(pDX, IDC_EDIDIM_C, Dim_C);
	DDX_Text(pDX, IDC_EDIDIM_D, Dim_D);
	DDX_Text(pDX, IDC_EDIDIM_E, Dim_E);
	DDX_Text(pDX, IDC_EDIDIM_F, Dim_F);
	DDX_Text(pDX, IDC_EDIDIM_G, Dim_G);
	DDX_Text(pDX, IDC_EDIESCONS, Esconsidade);
	DDX_Text(pDX, IDC_EDIAZIMUTE, AzGraus);
	DDX_Control(pDX, IDC_BMPCROQUIS, CroquisBMP);
}

BEGIN_MESSAGE_MAP(CdiaEditarEquipDrenagem, CDialogEx)
	ON_BN_CLICKED(IDOK, &CdiaEditarEquipDrenagem::OnBnClickedOk)
END_MESSAGE_MAP()

// manipuladores de mensagens de CdiaEditarEquipDrenagem

void CdiaEditarEquipDrenagem::OnBnClickedOk()
{
	CDialogEx::OnOK();

	UpdateData(true);

	if (Equipamento == CAciTopografico::EQUI_BUEIRO)  Dim_D = Dim_C;     //--- Altura = Largura

	pDimensoes[0] = Dim_A;
	pDimensoes[1] = Dim_B;
	pDimensoes[2] = Dim_C;
	pDimensoes[3] = Dim_D;
	pDimensoes[4] = Dim_E;
	pDimensoes[5] = Dim_F;
	pDimensoes[6] = Dim_G;

	pDimensoes[7] = bForcar90Graus ? 1.0 : 0.0;
}

BOOL CdiaEditarEquipDrenagem::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(FALSE);

	GetDlgItem(IDC_EDINOME)->SetWindowText(Nome.c_str());

	if(QtdDimensoes < 7) GetDlgItem(IDC_EDIDIM_G)->EnableWindow(false);
	if(QtdDimensoes < 6) GetDlgItem(IDC_EDIDIM_F)->EnableWindow(false);
	if(QtdDimensoes < 5) GetDlgItem(IDC_EDIDIM_E)->EnableWindow(false);
	if(QtdDimensoes < 4) GetDlgItem(IDC_EDIDIM_D)->EnableWindow(false);
	if(QtdDimensoes < 3) GetDlgItem(IDC_EDIDIM_C)->EnableWindow(false);
	if(QtdDimensoes < 2) GetDlgItem(IDC_EDIDIM_B)->EnableWindow(false);

	if(Equipamento == CAciTopografico::EQUI_BUEIRO) GetDlgItem(IDC_EDIDIM_D)->EnableWindow(false);

	switch (Equipamento)
	{
		case CAciTopografico::EQUI_BUEIRO:
		case CAciTopografico::EQUI_BUEIRO_RETANG: CroquisBMP.SetBitmap(BueiroBMP); break;
		case CAciTopografico::EQUI_CANALETA_TN: CroquisBMP.SetBitmap(CanaletaBMP); break;
		case CAciTopografico::EQUI_CANALETA_TERRAPL: CroquisBMP.SetBitmap(CanaletaTerrapBMP);break;
		case CAciTopografico::EQUI_VALETA_TN: CroquisBMP.SetBitmap(ValetaTNBMP); break;
		case CAciTopografico::EQUI_VALETA_TERRAPL: CroquisBMP.SetBitmap(ValetaTerraplBMP); break;
		case CAciTopografico::EQUI_DRENO_PROFUNDO:
		case CAciTopografico::EQUI_TUBULACAO: CroquisBMP.SetBitmap(TubulacaoBMP); break;
		case CAciTopografico::EQUI_DESCIDA_DAGUA: CroquisBMP.SetBitmap(DescidaDaguaBMP); break;
		case CAciTopografico::EQUI_CAIXA_COLETORA: CroquisBMP.SetBitmap(CaixaColetoraBMP); break;
		case CAciTopografico::EQUI_BACIA_DISSIPACAO: CroquisBMP.SetBitmap(BaciaDissipacaoBMP); break;
		case CAciTopografico::EQUI_SARJETA: CroquisBMP.SetBitmap(SarjetaBMP); break;
		default: ASSERT(0);
	}

	if (Azimute != INFINITO)
	{
		AzGraus = Mat.strg_gms(Azimute * 180.0 / Matem::M_PI).c_str();
	}

	if (dEsconsidade != INFINITO)
	{
		Esconsidade = Mat.strg_gms(dEsconsidade * 180.0 / Matem::M_PI).c_str();

		double DifEsc(fabs(dEsconsidade - Matem::M_PI_2));

		if (DifEsc < Matem::M_PI_DIV_180 * 10.0 && DifEsc > Matem::UM_SEGUNDO)
		{
			GetDlgItem(IDC_RADFORCAR90GRAUS)->EnableWindow(true);
		}
	}

	UpdateData(FALSE);

	return TRUE;
}

