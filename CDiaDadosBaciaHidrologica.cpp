// CDiaDadosBaciaHidrologica.cpp : arquivo de implementação
//
#include "stdafx.h"
#include "Est.h"
#include <set>
#include "CPonto.h"
#include "cdeltasuperficie.h"
#include "CDadosBacias.h"
#include "CLPontos.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "CDiaDadosBaciaHidrologica.h"
#include "afxdialogex.h"
#include "ImageListBox.h"
#include "CDiaDadosHidrologicos.h"
#include "Mat.h"
#include "Monologo.h"

extern Matem Mat;
extern monologo monolog;

IMPLEMENT_DYNAMIC(CDiaDadosBaciaHidrologica, CDialogEx)

CDiaDadosBaciaHidrologica::CDiaDadosBaciaHidrologica(CWnd* pParent, ittylstBacias BaciaHidrol,const CString& pNomeProjeto,std::string& pNomeBacia,tylstBacias& pLstBacias,void* pSuperf)
                           : CDialogEx(IDD_DIADADOSBACIAHIDROLOGICA, pParent),
                             DadosBaciaOriginal(BaciaHidrol->PegaDadosBacia()),
                             DadosBacia(BaciaHidrol->PegaDadosBacia()),
                             CSNomeBacia(BaciaHidrol->PegaDadosBacia().NomeBacia.c_str()),
                             BOMetros(FALSE),
                             BOKilometros(TRUE),
                             NomeProjeto(pNomeProjeto),
                             NovoNomeBacia(pNomeBacia),
                             Bacia(*BaciaHidrol),
                             LstBacias(pLstBacias),
                             ItBacia(BaciaHidrol),
                             pSuperficie(pSuperf),
                             EquiCNHipso(_T("1")),
                             StrObservacoes(_T("")),
                             Calculou(false)
{
}

CDiaDadosBaciaHidrologica::~CDiaDadosBaciaHidrologica()
{
  WriteWindowPlacement();
}

void CDiaDadosBaciaHidrologica::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIAREA, CSArea);
  DDX_Text(pDX, IDC_EDICOMPRIMENTO, CSComprimento);
  DDX_Text(pDX, IDC_EDIKC, CSKc);
  DDX_Text(pDX, IDC_EDIKF, CSKf);
  DDX_Text(pDX, IDC_EDILT, CSLt);
  DDX_Text(pDX, IDC_EDIDD, CSDd);
  DDX_Text(pDX, IDC_EDIL, CSl);
  DDX_Text(pDX, IDC_EDINOMEBACIA, CSNomeBacia);
  DDX_Control(pDX, IDC_EDINOMEBACIA, CEdiNomeBacia);
  DDX_Text(pDX, IDC_EDIEQUICNHIPSO, EquiCNHipso);
  DDX_Control(pDX, IDC_BUTCALC, ButCalcular);
  DDX_Control(pDX, IDC_BUTDADOSHIDROL,ButCalcularHidrol);
  DDX_Text(pDX, IDC_STAOBSERVACOES, StrObservacoes);
  DDX_Text(pDX, IDC_EDH, CSH);
  DDX_Text(pDX, IDC_EDINCG, CSNCG);
  DDX_Text(pDX, IDC_EDIECG, CSECG);
  DDX_Text(pDX, IDC_EDICOTACG, CSCotaCG);
  DDX_Text(pDX, IDC_EDIDISCGFOZ,CSDistCGFoz);
}

BEGIN_MESSAGE_MAP(CDiaDadosBaciaHidrologica, CDialogEx)
  ON_BN_CLICKED(IDOK, &CDiaDadosBaciaHidrologica::OnBnClickedOk)
  ON_BN_CLICKED(IDC_RADMETROS, &CDiaDadosBaciaHidrologica::OnBnClickedRadmetros)
  ON_BN_CLICKED(IDC_RADKILOMETROS, &CDiaDadosBaciaHidrologica::OnBnClickedRadkilometros)
  ON_BN_CLICKED(IDC_BUTDADOSHIDROL, &CDiaDadosBaciaHidrologica::OnBnClickedButDadosHidrol)
  ON_BN_CLICKED(IDC_BUTCALC, &CDiaDadosBaciaHidrologica::OnBnClickedButcalc)
  ON_EN_CHANGE(IDC_EDIEQUICNHIPSO, &CDiaDadosBaciaHidrologica::OnEnChangeEdiequicnhipso)
  ON_EN_CHANGE(IDC_EDINOMEBACIA, &CDiaDadosBaciaHidrologica::OnEnChangeEdinomebacia)
  ON_EN_SETFOCUS(IDC_EDINOMEBACIA, &CDiaDadosBaciaHidrologica::OnEnSetfocusEdinomebacia)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDiaDadosBaciaHidrologica

void CDiaDadosBaciaHidrologica::OnBnClickedRadmetros()
{
  UpdateData(true); //-- so pra pegar o nome

  //if(BOMetros == TRUE) return;

  BOMetros = false;
  BOKilometros = true;

  DadosBacia.Balizas &= !CDadosBacia::UNI_KM;

  SetDlgItemText(IDC_STAAREA, "m2");
  SetDlgItemText(IDC_STACOMPRIMENTO, "m");
  SetDlgItemText(IDC_STACOMPTALVEGUES, "m");
  SetDlgItemText(IDC_STADENSDRENAGEM, "Km/Km2");
  SetDlgItemText(IDC_STAPERESCOAMENTO, "m");

  CSKc.Format("% 12.2lf", DadosBacia.Kc);
  CSKf.Format("% 12.2lf", DadosBacia.Kf);
  CSLt.Format("% 12.2lf", DadosBacia.Lt);
  CSDd.Format("% 12.2lf", DadosBacia.Dd);
  CSl.Format("% 12.2lf", DadosBacia.l);
  CSComprimento.Format("% 12.1lf", DadosBacia.Diagonal);
  CSArea.Format("% 12.2lf", DadosBacia.Area);
  CSH.Format("% 4.2lf", DadosBacia.h);
  CSNCG.Format("%12.3lf",ItBacia->CG().y);
  CSECG.Format("%12.3lf", ItBacia->CG().x);
  CSCotaCG.Format("%12.3lf", ItBacia->CG().z);
  CSDistCGFoz.Format("%12.1lf", ItBacia->PegaDadosBacia().LCG);

  ((CButton*)GetDlgItem(IDC_RADMETROS))->SetCheck(BOMetros);
  ((CButton*)GetDlgItem(IDC_RADKILOMETROS))->SetCheck(BOKilometros);

  UpdateData(false);
}

void CDiaDadosBaciaHidrologica::OnBnClickedRadkilometros()
{
  UpdateData(true); 

  if(BOKilometros == TRUE) return;

  BOKilometros = TRUE;
  BOMetros = false;

  DadosBacia.Balizas |= CDadosBacia::UNI_KM;

  SetDlgItemText(IDC_STAAREA,"km2");
  SetDlgItemText(IDC_STACOMPRIMENTO,"km");
  SetDlgItemText(IDC_STACOMPTALVEGUES,"km");
  SetDlgItemText(IDC_STADENSDRENAGEM,"km/Km2");
  SetDlgItemText(IDC_STAPERESCOAMENTO,"km");
   
  CSl.Format("% 12.2lf", DadosBacia.l / 1.0E3);
  CSComprimento.Format("% 12.2lf", DadosBacia.Diagonal / 1.0E3);
  CSArea.Format("% 12.2lf", DadosBacia.Area / 1.0E6);

  ((CButton*)GetDlgItem(IDC_RADMETROS))->SetCheck(BOMetros);
  ((CButton*)GetDlgItem(IDC_RADKILOMETROS))->SetCheck(BOKilometros);

  UpdateData(false);
}

BOOL CDiaDadosBaciaHidrologica::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  ReadWindowPlacement();

  MostraDadosGeometricos();

  ((CButton*)GetDlgItem(IDC_RADMETROS))->SetCheck(BOMetros);
  ((CButton*)GetDlgItem(IDC_RADKILOMETROS))->SetCheck(BOKilometros);

  if (CSNomeBacia.IsEmpty())
  {
    CSNomeBacia = NovoNomeBacia.c_str();
  }

  UpdateData(false);

  ButCalcularHidrol.EnableWindow(ItBacia->Calculada() == true);
  ButCalcular.EnableWindow(ItBacia->Calculada() == false);
  ButCalcular.SetFocus();

  return TRUE;  
}

void CDiaDadosBaciaHidrologica::OnBnClickedOk()
{
  UpdateData(true);

  if(CSNomeBacia.IsEmpty())
  {
    monolog.mensagem(-1,"A bacia deve ter um nome.");
    return;
  }

  NovoNomeBacia = CSNomeBacia;

  BOKilometros = ((CButton*)GetDlgItem(IDC_RADKILOMETROS))->GetCheck();

  //--- Verifica o nome da bacia

  cittylstBacias& ItBaciaAtual(LstBacias.begin());

  while (ItBaciaAtual != LstBacias.end() && (ItBaciaAtual == ItBacia || ItBaciaAtual->PegaDadosBacia().NomeBacia.find(CSNomeBacia.GetBuffer()) == std::string::npos))
  {
    ItBaciaAtual++;
  }

  if (ItBaciaAtual != LstBacias.end())
  {
    monolog.mensagem(-1,"Já existe uma bacia com este nome.");
    return;
  }

  CDialogEx::OnOK();
}

bool CDiaDadosBaciaHidrologica::ReadWindowPlacement()
{
  CString Bloco("DiaBaciaHidro");

  TCHAR szFormat[] = _T("%d");
  char Buffer[127] = { 0 };

  ::GetPrivateProfileString(Bloco, "Valor", "0", Buffer, 127, (NomeProjeto + ".ini"));
  if (strlen(Buffer) == 0) return FALSE;

  int Valor(0);

  if (_stscanf(Buffer, szFormat, &Valor) != 1) return false;

  BOKilometros = Valor > 0;
  BOMetros = false;
  BOKilometros = true;

  return TRUE;
}

void CDiaDadosBaciaHidrologica::WriteWindowPlacement()
{
  CString Bloco("DiaBaciaHidro");
  TCHAR szFormat[] = _T("%d"), szBuffer[100];

  wsprintf(szBuffer, szFormat, BOKilometros);

  ::WritePrivateProfileString(Bloco, "Valor", szBuffer, NomeProjeto + ".ini");
}

void CDiaDadosBaciaHidrologica::OnBnClickedButDadosHidrol()
{
  CDiaDadosHidrologicos DiaDadosHidrologicos(Bacia, this,Calculou);

  DiaDadosHidrologicos.DoModal();
}

void CDiaDadosBaciaHidrologica::OnBnClickedButcalc()
{
  UpdateData(true);  

  auto It(LstBacias.begin());

  if (CSNomeBacia.GetLength() > 0)
  {
    while(It != LstBacias.end() && (It == ItBacia || (It->PegaDadosBacia().NomeBacia.find(CSNomeBacia.GetBuffer()) == std::string::npos)))
    {
      It++;
    }
  }

  if (It != LstBacias.end())
  {
    monolog.mensagem(-1,"Já existe uma bacia com este nome.");

    CEdiNomeBacia.SetFocus();

    return;
  }

  ItBacia->Erro = CBacia::BACIA_OK;
  ItBacia->PegaDadosBacia().NomeBacia = CSNomeBacia;
  ItBacia->EquidistCNHistograma() = atof(EquiCNHipso.GetBuffer());
  ItBacia->Calcular(pSuperficie,StrObservacoes,(CDialogEx*)this);

  ItBacia->CG() = CalculaCG();
  ItBacia->CG().S = hypot(Bacia.PegaFoz().x - Bacia.CG().x, Bacia.PegaFoz().y - Bacia.CG().y);

  ItBacia->PegaDadosBacia().LCG = ItBacia->CG().S;

  std::string Mensagem;

  if (ItBacia->Erro != CBacia::BACIA_OK)
  {
    //--- Erros de cálculo

    switch (ItBacia->Erro)
    {
      case CBacia::MUITAS_SAIDAS: Mensagem = "Existem muitas saídas de talvegues nesta bacia (Cada bacia deve possuir apenas uma saida (Foz))."; break;
      case CBacia::SEM_SAIDA: Mensagem = "Esta bacia não tem saída (foz), toda bacia deve possuir uma e apenas uma foz."; break;
      case CBacia::ERRO_CN_ESPIGAO:  Mensagem = "Há curvas de nível cortando o espigão várias vezes, verifique o arquivo de log."; break;
      default: Mensagem = "Houve um erro no cálculo da bacia hidrográfica";
    }

    monolog.mensagem(-1,Mensagem.c_str());

    return;
  }
  else
  {
    MostraDadosGeometricos();
   
    ButCalcularHidrol.EnableWindow(true);
    ItBacia->Calculada() = Calculou = true;
    ButCalcular.EnableWindow(Calculou == false);
  }
}

void CDiaDadosBaciaHidrologica::OnEnChangeEdiequicnhipso()
{
  UpdateData(TRUE);
}

void CDiaDadosBaciaHidrologica::OnEnChangeEdinomebacia()
{
  UpdateData(TRUE);

  ItBacia->Calculada() = Calculou = false;
  ButCalcular.EnableWindow(Calculou == false);
  ButCalcularHidrol.EnableWindow(false);
}

void CDiaDadosBaciaHidrologica::MostraDadosGeometricos()
{
  CDadosBacia& DB(ItBacia->PegaDadosBacia());

  if (BOKilometros == TRUE)
  {
    SetDlgItemText(IDC_STAAREA, "km2");
    SetDlgItemText(IDC_STACOMPRIMENTO, "km");
    SetDlgItemText(IDC_STACOMPTALVEGUES, "km");
    SetDlgItemText(IDC_STADENSDRENAGEM, "km/Km2");
    SetDlgItemText(IDC_STAPERESCOAMENTO, "km");
  }
  
  CSKc.Format("% 12.2lf", DB.Kc);
  CSKf.Format("% 12.2lf", DB.Kf);
  CSLt.Format("% 12.2lf", DB.Lt / (BOKilometros ? 1.0E3 : 1.0));
  CSDd.Format("% 12.2lf", DB.Dd);
  CSl.Format("% 12.2lf", DB.l / (BOKilometros ? 1.0E3 : 1.0));
  CSH.Format("% 4.2lf",DB.h);
  CSComprimento.Format("% 12.2lf", DB.Diagonal / (BOKilometros ?  1.0E3 : 1.0));
  CSArea.Format("% 12.2lf", DB.Area / (BOKilometros ? 1.0E6 : 1.0));
  CSNomeBacia = DB.NomeBacia.c_str();
  CSNCG.Format("% 12.3lf", ItBacia->CG().x);
  CSECG.Format("% 12.3lf", ItBacia->CG().y);
  CSCotaCG.Format("% 12.3lf", ItBacia->CG().z);
  CSDistCGFoz.Format("% 12.2lf", ItBacia->PegaDadosBacia().LCG);
  CSNCG.Format("%12.3lf", ItBacia->CG().y);
  CSECG.Format("%12.3lf", ItBacia->CG().x);
  CSCotaCG.Format("%12.3lf", ItBacia->CG().z);
  CSDistCGFoz.Format("%12.1lf", ItBacia->PegaDadosBacia().LCG);

  ((CButton*)GetDlgItem(IDC_RADMETROS))->SetCheck(BOMetros);
  ((CButton*)GetDlgItem(IDC_RADKILOMETROS))->SetCheck(BOKilometros);

  UpdateData(false);
}
void CDiaDadosBaciaHidrologica::OnEnSetfocusEdinomebacia()
{
  if(ItBacia->Calculada()) ((CButton*)GetDlgItem(IDC_STAAREA))->SetFocus();
}
void CDiaDadosBaciaHidrologica::OnEnSetfocusEdiequicnhipso()
{
  if (ItBacia->Calculada()) ((CButton*)GetDlgItem(IDC_STAAREA))->SetFocus();
}

Ponto CDiaDadosBaciaHidrologica::CalculaCG()
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
