// CDSelecionarCNs.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "afxdialogex.h"
#include "CDSelecionarCNs.h"

// caixa de diálogo de CDSelecionarCNs

IMPLEMENT_DYNAMIC(CDSelecionarCNs, CDialogEx)

CDSelecionarCNs::CDSelecionarCNs(const std::string* pProj,CWnd* pParent,double* pAcimaDaCota,double* pAbaixoDaCota) : CDialogEx(IDD_DIASELECIONARCNS, pParent),
    pProjeto(pProj),pdouLimiteSuperior(pAcimaDaCota), pdouLimiteInferior(pAbaixoDaCota),
	  dNaCota(0.0),dAcimaDaCota(0.0),dAbaixoDaCota(0.0),
	  dCotaInicial(0.0),dCotaFinal(0.0),bTodas(TRUE),bNaCota(FALSE),bAcimaCota(FALSE), 
		bAbaixoCota(FALSE),bNaFaixa(FALSE)
{
}

CDSelecionarCNs::~CDSelecionarCNs()
{
  CString CSDados;

  std::string NomeArquivo;

	NomeArquivo = *pProjeto + std::string(".ini");

	CSDados.Format("%d,%d,%d,%d,%d,%lf,%lf,%lf,%lf,%lf",bTodas,bNaCota,bAcimaCota,bAbaixoCota,bNaFaixa,
		              dNaCota,dAcimaDaCota,dAbaixoDaCota,dCotaInicial,dCotaFinal);

	::WritePrivateProfileString(_T("DadosDesenhos"),_T("DadosSelCNs"), CSDados, NomeArquivo.c_str());
}

void CDSelecionarCNs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDINACOTA, dNaCota);
	//DDV_MinMaxDouble(pDX, dNaCota, -100.0, 10000.0);
	DDX_Text(pDX, IDC_EDIACIMADACOTA, dAcimaDaCota);
	//DDV_MinMaxDouble(pDX, dNaCota, -100.0, 10000.0);
	DDX_Text(pDX, IDC_EDIABAIXODACOTA, dAbaixoDaCota);
	//DDV_MinMaxDouble(pDX, dNaCota, -100.0, 10000.0);
	DDX_Text(pDX, IDC_EDICOTAINICIAL, dCotaInicial);
	//DDV_MinMaxDouble(pDX, dNaCota, -100.0, 10000.0);
	DDX_Text(pDX, IDC_EDICOTAFINAL, dCotaFinal);
	//DDV_MinMaxDouble(pDX, dNaCota, -100.0, 10000.0);
	DDX_Check(pDX, IDC_CHETODASCN, bTodas);
	DDX_Check(pDX, IDC_CHENACOTA, bNaCota);
	DDX_Check(pDX, IDC_CHEACIMADACOTA, bAcimaCota);
	DDX_Check(pDX, IDC_CHEABAIXODACOTA, bAbaixoCota);
	DDX_Check(pDX, IDC_CHENAFAIXA, bNaFaixa);
}

BEGIN_MESSAGE_MAP(CDSelecionarCNs, CDialogEx)
	ON_BN_CLICKED(IDC_CHETODASCN, &CDSelecionarCNs::OnBnClickedChetodascn)
	ON_BN_CLICKED(IDC_CHENACOTA, &CDSelecionarCNs::OnBnClickedChecnnacota)
	ON_BN_CLICKED(IDC_CHEACIMADACOTA, &CDSelecionarCNs::OnBnClickedChecnacimadacota)
	ON_BN_CLICKED(IDC_CHEABAIXODACOTA, &CDSelecionarCNs::OnBnClickedChecnabaixodacota)
	ON_BN_CLICKED(IDC_CHENAFAIXA, &CDSelecionarCNs::OnBnClickedChecnnafaixa)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDSelecionarCNs

void CDSelecionarCNs::OnBnClickedChetodascn()
{
	UpdateData(true);

	bNaCota = bAcimaCota = bAbaixoCota = bNaFaixa = false;

	UpdateData(false);
}

void CDSelecionarCNs::OnBnClickedChecnnacota()
{
  UpdateData(true);

	bTodas = bAcimaCota = bAbaixoCota = bNaFaixa = false;

	LimpaOutrosEdits(IDC_CHENACOTA);
		
	UpdateData(false);
}

void CDSelecionarCNs::OnBnClickedChecnacimadacota()
{
	UpdateData(true);

	bTodas = bNaCota = bAbaixoCota = bNaFaixa = false;

	LimpaOutrosEdits(IDC_CHEACIMADACOTA);

	UpdateData(false);
}

void CDSelecionarCNs::OnBnClickedChecnabaixodacota()
{
	UpdateData(true);

	bTodas = bNaCota = bAcimaCota = bNaFaixa = false;

	LimpaOutrosEdits(IDC_CHEABAIXODACOTA);

	UpdateData(false);
}

void CDSelecionarCNs::OnBnClickedChecnnafaixa()
{
	UpdateData(true);

	bTodas = bNaCota = bAcimaCota = bAbaixoCota = false;

	LimpaOutrosEdits(IDC_CHENAFAIXA);

	UpdateData(false);
}

void CDSelecionarCNs::LimpaOutrosEdits(unsigned int Check)
{
  if(Check != IDC_CHENACOTA) SetDlgItemText(IDC_EDINACOTA,"");
	if(Check != IDC_CHEACIMADACOTA) SetDlgItemText(IDC_EDIACIMADACOTA, "");
	if(Check != IDC_CHEABAIXODACOTA) SetDlgItemText(IDC_EDIABAIXODACOTA, "");
	if(Check != IDC_CHENAFAIXA)
	{
	  SetDlgItemText(IDC_EDICOTAINICIAL, "");
	  SetDlgItemText(IDC_EDICOTAFINAL, "");
	}
}

BOOL CDSelecionarCNs::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	char Buffer[1024] = { 0 };
	CString Default;

	std::string NomeArquivo(*pProjeto + std::string(".ini"));

	Default.Format("%d,%d,%d,%d,%d,%lf,%lf,%lf,%lf,%lf",1, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0,0.0);
	::GetPrivateProfileString(_T("DadosDesenhos"), _T("DadosSelCNs"), Default, Buffer, 511, NomeArquivo.c_str());

	std::stringstream strstrBuffer(Buffer);
	char Virgula;

	strstrBuffer >> bTodas >> Virgula >> bNaCota >> Virgula >> bAcimaCota >> Virgula >> bAbaixoCota >> Virgula >> bNaFaixa >> Virgula
	             >> dNaCota >> Virgula >> dAcimaDaCota >> Virgula >> dAbaixoDaCota >> Virgula >> dCotaInicial >> Virgula >> dCotaFinal;
								
  UpdateData(FALSE);
	
	//LimpaOutrosEdits(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: as páginas de propriedade OCX devem retornar FALSE
}

void CDSelecionarCNs::OnOK()
{
	UpdateData(true);

  if (((CButton*)GetDlgItem(IDC_CHETODASCN))->GetCheck() == TRUE)
  {
    *pdouLimiteSuperior = INFINITO;
    *pdouLimiteInferior = -INFINITO;
  }
  else
  {
    if (((CButton*)GetDlgItem(IDC_CHENACOTA))->GetCheck() == TRUE)
    {
      *pdouLimiteSuperior = *pdouLimiteInferior = dNaCota;
    }
    else
    {
      if (((CButton*)GetDlgItem(IDC_CHEACIMADACOTA))->GetCheck() == TRUE)
      {
        *pdouLimiteSuperior = INFINITO;
        *pdouLimiteInferior = dAcimaDaCota;
      }
      else
      {
        if (((CButton*)GetDlgItem(IDC_CHEABAIXODACOTA))->GetCheck() == TRUE)
        {
          *pdouLimiteSuperior = dAbaixoDaCota;
          *pdouLimiteInferior = -INFINITO;
        }
        else
        {
          if (((CButton*)GetDlgItem(IDC_CHENAFAIXA))->GetCheck() == TRUE)
          {
            *pdouLimiteSuperior = dCotaFinal;
            *pdouLimiteInferior = dCotaInicial;
          }
        }
      }
    }
  }

	CDialogEx::OnOK();
}
