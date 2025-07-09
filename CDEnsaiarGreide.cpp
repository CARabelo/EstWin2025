// CDEnsaiarGreide.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "afxdialogex.h"
#include "CDEnsaiarGreide.h"
#include "dialogo.h"

extern dialogo dialogar;

extern CDEnsaiarGreide* pDesEnsaiarGreide;

// caixa de diálogo de CDEnsaiarGreide

IMPLEMENT_DYNAMIC(CDEnsaiarGreide, CDialogEx)

static double PadraoParam[8][2] ={{5.0,0.0},
                                  {100.0,0.0},
																	{150.0,0.0},
																	{3.0,0.0},
																	{100.0,5.0},
																	{50.0,10.0},
																	{5.0,0.0},
																	{12.0,0.0}};

CDEnsaiarGreide::CDEnsaiarGreide(CWnd* pParent,std::string& pNomeProj,int& pMetodo,double& pParam1,double& pParam2) : CDialogEx(IDD_DIAENSAIARGREIDE, pParent),NomeProjeto(pNomeProj), Metodo(pMetodo), Parametro1(pParam1),Parametro2(pParam2)
{
	for (int C = 0; C < 8; C++)
	{
		ParamAtuais[C][0] = ParamAtuais[C][1] = INFINITO;
	}
}

CDEnsaiarGreide::~CDEnsaiarGreide()
{
	std::ofstream ArqParamEnsaios(NomeProjeto + "Lst.ens", std::fstream::trunc | std::fstream::out);

	//ArqParamEnsaios.unsetf(std::ios_base::skipws);

	for(int C = 0 ; C < 8 ; C++)
	{
		ArqParamEnsaios << ParamAtuais[C][0] << ',' << ParamAtuais[C][1] << std::endl;
	}

	ArqParamEnsaios.close();

	CString CSDados;
	std::string NomeArquivo(NomeProjeto + std::string(".ini"));

	CSDados.Format("%d,%4.1lf,%4.1lf,", EnsaiarGreide.GetCurSel(), Parametro1, Parametro2);

	::WritePrivateProfileString(_T("DadosDesenhos"), _T("DadosEnsaiarGreide"), CSDados, NomeArquivo.c_str());
}

void CDEnsaiarGreide::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMMETODO, EnsaiarGreide);
	DDX_Text(pDX, IDC_EDIPARAM_1_ENSAIARGREIDE, cstParametro1);
	DDX_Text(pDX, IDC_EDIPARAM_2_ENSAIARGREIDE, cstParametro2);
}

BEGIN_MESSAGE_MAP(CDEnsaiarGreide, CDialogEx)
	ON_BN_CLICKED(IDC_BUTEXECUTAR, &CDEnsaiarGreide::OnBnClickedButexecutar)
	ON_BN_CLICKED(IDCANCEL, &CDEnsaiarGreide::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMMETODO, &CDEnsaiarGreide::OnCbnSelchangeCommetodo)
	ON_BN_CLICKED(IDC_BUTPADRAO, &CDEnsaiarGreide::OnBnClickedButpadrao)
	ON_BN_CLICKED(IDC_BUTADOTAR, &CDEnsaiarGreide::OnBnClickedButadotar)
	ON_BN_CLICKED(IDC_BUTREMOVER, &CDEnsaiarGreide::OnBnClickedButremover)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDEnsaiarGreide

BOOL CDEnsaiarGreide::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	EnsaiarGreide.InsertString(0, "Enésimo Ponto");
	EnsaiarGreide.InsertString(1, "Distância Radial *");
	EnsaiarGreide.InsertString(2, "Distância Perpendicular");
	EnsaiarGreide.InsertString(3, "Reumann Witkam *");
	EnsaiarGreide.InsertString(4, "Opheim");
	EnsaiarGreide.InsertString(5, "Lang *");
	EnsaiarGreide.InsertString(6, "Douglas Peucker*");
	EnsaiarGreide.InsertString(7, "Douglas Peucker(Variante) *");

	char Buffer[64] = { 0 };
	CString Default;
	std::string NomeArquivo(NomeProjeto + ".ini");

	Default.Format("%s,%4.1lf,%4.1lf,"," ",0.0,0.0);
	::GetPrivateProfileString(_T("DadosDesenhos"), _T("DadosEnsaiarGreide"), Default, Buffer, 64, NomeArquivo.c_str());

	std::stringstream strstrBuffer(Buffer);
	char Virgula;
	int Metodo(-1);
	double Param1(0.0), Param2(0.0);

	strstrBuffer >> Metodo >> Virgula >> Param1 >> Virgula >>Param2;

	EnsaiarGreide.SetCurSel(Metodo);
	cstParametro1.Format("% 3.1lf",Param1);
	cstParametro2.Format("% 3.1lf",Param2);

	std::ifstream ArqParamEnsaios(NomeProjeto + "Lst.ens",std::fstream::in);

	//ArqParamEnsaios.unsetf(std::ios_base::skipws);

	for (int C = 0; C < 8; C++)
	{
		ArqParamEnsaios >> ParamAtuais[C][0] >> Virgula >> ParamAtuais[C][1];
	}

	ArqParamEnsaios.close();

	GetDlgItem(IDC_BUTADOTAR)->EnableWindow(false);
	GetDlgItem(IDC_BUTREMOVER)->EnableWindow(false);

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: as páginas de propriedade OCX devem retornar FALSE
}

void CDEnsaiarGreide::OnBnClickedButexecutar()
{
  UpdateData(true);

  Metodo = EnsaiarGreide.GetCurSel();
	Parametro1 = atof(cstParametro1.GetBuffer());
	Parametro2 = atof(cstParametro2.GetBuffer());

	ParamAtuais[Metodo][0] = Parametro1;
	ParamAtuais[Metodo][1] = Parametro2;

	GetParent()->SendMessageToDescendants(WM_ENSAIARGREIDE,0,0);

	GetDlgItem(IDC_BUTADOTAR)->EnableWindow(true);
	GetDlgItem(IDC_BUTREMOVER)->EnableWindow(true);
}

void CDEnsaiarGreide::OnBnClickedCancel()
{
	UpdateData(true);

	Metodo = EnsaiarGreide.GetCurSel();
	Parametro1 = atof(cstParametro1.GetBuffer());
	Parametro2 = atof(cstParametro2.GetBuffer());

	GetParent()->SendMessageToDescendants(WM_ENSAIARGREIDE,TERMINAR_ENSAIOS, 0);

	delete this;
}

void CDEnsaiarGreide::OnCbnSelchangeCommetodo()
{
	int MetodoAtual(EnsaiarGreide.GetCurSel());

	if (ParamAtuais[MetodoAtual][0] != INFINITO)
		cstParametro1.Format("%4.1lf", ParamAtuais[MetodoAtual][0]);
	if (ParamAtuais[MetodoAtual][1] != INFINITO)
		cstParametro2.Format("%4.1lf", ParamAtuais[MetodoAtual][1]);

	UpdateData(false);

  if (MetodoAtual == LANG || MetodoAtual == OPHEIM)
	{
		GetDlgItem(IDC_STATIC_2)->EnableWindow(true);
		GetDlgItem(IDC_EDIPARAM_2_ENSAIARGREIDE)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_STATIC_2)->EnableWindow(false);
		GetDlgItem(IDC_EDIPARAM_2_ENSAIARGREIDE)->EnableWindow(false);
	}
}

void CDEnsaiarGreide::OnBnClickedButpadrao()
{
	int MetodoAtual(EnsaiarGreide.GetCurSel());

	if (MetodoAtual >= 0)
	{
		cstParametro1.Format("%3.1lf", PadraoParam[MetodoAtual][0]);
		cstParametro2.Format("%3.1lf", PadraoParam[MetodoAtual][1]);
	}

	UpdateData(false);
}

void CDEnsaiarGreide::OnBnClickedButadotar()
{
	if (dialogar.DoModal(-1, L"Já existe um greide neste perfil, ao adotar este ensaio\r\n"
	                         L"ele será substituído pelo greide ensaiado, CONFIRMA?") == IDOK)
	{
		GetParent()->SendMessageToDescendants(WM_ENSAIARGREIDE, ADOTAR_ENSAIO_ATUAL, 0);

		GetDlgItem(IDC_BUTADOTAR)->EnableWindow(false);
		GetDlgItem(IDC_BUTREMOVER)->EnableWindow(false);
	}
}

void CDEnsaiarGreide::OnBnClickedButremover()
{
	if (dialogar.DoModal(-1, _T("Confirma a remoção deste ensaio?\r\n")) == IDOK)
	{
		GetParent()->SendMessageToDescendants(WM_ENSAIARGREIDE,REMOVER_ENSAIO_ATUAL, 0);

		GetDlgItem(IDC_BUTADOTAR)->EnableWindow(false);
		GetDlgItem(IDC_BUTREMOVER)->EnableWindow(false);
	}
}
