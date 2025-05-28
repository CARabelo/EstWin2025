// CalcMedicao.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "math.h"
#include "estaca.h"
#include "afxtempl.h"
#include "cponto.h"
#include "clpontos.h"
#include "perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "supercstring.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include <list>
#include "csecao.h"
#include "clistasecoes.h"
#include "CalculaMedicao.h"
#include <string>
#include <map>
#include "CExcomboBox.h" 
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "monologo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MENOR     -1
#define MAIOR      1
#define IGUAL      0

/////////////////////////////////////////////////////////////////////////////
// CalcMedicao dialog

CalcMedicao::CalcMedicao(CString& NomeProjeto,CWnd* pParent) : CDialog(CalcMedicao::IDD, pParent),Secoes(NomeProjeto)
{
  Secoes.TaludesDefault();  //--- Se os arquivos de taludes n�o foram encontrados assume valores default

  //{{AFX_DATA_INIT(CalcMedicao)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CalcMedicao::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CalcMedicao)
  // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CalcMedicao, CDialog)
  //{{AFX_MSG_MAP(CalcMedicao)
  ON_BN_CLICKED(IDC_INICIARCALCMED, OnIniciarcalcmed)
  ON_BN_CLICKED(IDC_OK, OnOk)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CalcMedicao message handlers

void CalcMedicao::OnIniciarcalcmed() 
{
  CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();
  NomeProjeto = MainFrame->m_sNomeComp;

  //--- Pega a configuracao do calculo.

  // UsarEditadas = IsDlgButtonChecked(IDC_RADUSARSECEDI);
  Acumulada = IsDlgButtonChecked(IDC_RADACUMULADA);

  //--- Se ler os dois perfis , o do projeto e o do terreno, calcula a medic�o 

  if (LeMedicaoAnterior() && LeMedicaoAtual()) CalculaMedicao();
}

int CalcMedicao::LeMedicaoAnterior()
{
  //--- Pega o numero da medicao anterior.

  CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();
  CString NomeArquivo = MainFrame->m_sNomeComp;

  //--- Se n�o for a primeira medi��o ou se n�o for acumulada coloca o numero da medicao anterior no nome
  //--- Caso contrarioo o c�lculo ser� feito com o terreno primitivo
  if (!MainFrame->m_sMedicaoAnterior.IsEmpty() && !Acumulada)
  {
    NomeArquivo = NomeArquivo.Left(NomeArquivo.GetLength() - 3);
    NomeArquivo += MainFrame->m_sMedicaoAnterior + ".tme";

  }
  else NomeArquivo = NomeArquivo.Left(NomeArquivo.GetLength() - 6) + ".tca";

  MedicaoAnterior.Constroi(NomeArquivo);  

  return TRUE;
}

int CalcMedicao::LeMedicaoAtual()
{
  CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();
  CString NomeArquivo = MainFrame->m_sNomeComp + ".tca"; 
  SetDlgItemText(IDC_EXECUTANDO,"Abrindo o arquivo do terreno atual.");
  MedicaoAtual.Constroi(NomeArquivo);

  return TRUE;
}

void CalcMedicao::MontaSecoes()
{
  //--- Prepara a barra de progresso.

  SetDlgItemText(IDC_EXECUTANDO,"Preparando a medi��o para c�lculo...");

  //--- Monta as se��es para c�lculo incluindo os perf�s de projeto e de terreno.
  //--- Caso haja apenas um perfil na sec�o ele � deletado porque n�o ser� usado.

  POSITION AtualMedAnt = MedicaoAnterior.GetHeadPosition(),
    AtualMedAtual = MedicaoAtual.GetHeadPosition();

  if (!AtualMedAnt || !AtualMedAtual) return;

  while(AtualMedAnt && AtualMedAtual)
  {
    Perfil& SecaoAnterior = MedicaoAnterior.GetAt(AtualMedAnt),
      & SecaoAtual = MedicaoAtual.GetAt(AtualMedAtual);

    //--- Se os perf�s s�o da mesma estaca inclui uma secao na lista de secoes

    if (SecaoAnterior.Estaca.EstVirtual == SecaoAtual.Estaca.EstVirtual)
    { 
      CSecao Temp(SecaoAnterior,SecaoAtual);
      Secoes.AddTail(Temp);
      SecaoAnterior.RemoveAll();
      SecaoAtual.RemoveAll();
      MedicaoAnterior.GetNext(AtualMedAnt);
      MedicaoAtual.GetNext(AtualMedAtual);
    }
    else
    {
      if (SecaoAnterior.Estaca.EstVirtual > SecaoAtual.Estaca.EstVirtual)
      {
        SecaoAtual.RemoveAll();
        MedicaoAtual.GetNext(AtualMedAtual);
      }
      else
      {
        SecaoAnterior.RemoveAll();
        MedicaoAnterior.GetNext(AtualMedAnt);
      }
    }
  }

  //--- Remove o que sobrou das se��es lidas, as secoes a calcular est�o em Secoes.

  MedicaoAnterior.RemoveAll();
  MedicaoAtual.RemoveAll();

  //	if (Secoes.GetCount() < 2) ; //-- abortar explicando . tem que haver no m�nimo 2 secoes.
}

void CalcMedicao::CalculaOffSets()
{
  SetDlgItemText(IDC_EXECUTANDO,"Calculando os off-sets...");
  Prog = (CProgressCtrl *) GetDlgItem(IDC_PROGRESSO);
  Prog->SetRange(0,Secoes.GetCount()+1);
  Prog->SetStep(1);

  POSITION Atual(Secoes.GetHeadPosition());

  while(Atual) 
  {
    CSecao &SecaoAtual = Secoes.GetAt(Atual); 
    Prog->StepIt();

    //--- Calcula os Offsets da se��o, se n�o achar um dos dois, exclui a se��o, ela n�o interessa,
    //--- No excluir a lista � automaticamente incrementada, caso contrario incrementa a lista.

    if (!SecaoAtual.CalculaOffSets(Secoes.TaludesCorte,Secoes.TaludesAterro,TRUE)) 
    {
      POSITION Temp(Atual);
      Secoes.GetNext(Atual);
      Secoes.RemoveAt(Temp);
    }
    else Secoes.GetNext(Atual);
  }	
}

void CalcMedicao::CalculaAreas()
{
  Prog->SetPos(0);
  SetDlgItemText(IDC_EXECUTANDO,"Calculando as �reas...");

  //--- Para todas as secoes calcule os volumes. 

  POSITION Atual(Secoes.GetHeadPosition());
  if (!Atual) return;

  Prog->SetRange(0,Secoes.GetCount()+1);
  Prog->SetStep(1);

  while(Atual) 
  {
    Prog->StepIt();
    CSecao &SecaoAtual = Secoes.GetAt(Atual); 
    SecaoAtual.CalculaArea();
    Secoes.GetNext(Atual);
  }						
}							

void CalcMedicao::CalculaVolumes()
{
  Prog->SetPos(0);
  SetDlgItemText(IDC_EXECUTANDO,"Calculando os Volumes...");

  //--- Para todas as secoes calcule os volumes. 

  POSITION Atual(Secoes.GetHeadPosition());
  if (!Atual) return;

  Prog->SetRange(0,Secoes.GetCount());
  Prog->SetStep(1);

  CSecao* SecaoAnterior = &Secoes.GetAt(Atual);
  Secoes.GetNext(Atual);
  Prog->StepIt();

  while(Atual) 
  {
    Prog->StepIt();
    CSecao &SecaoAtual = Secoes.GetAt(Atual);
    SecaoAtual.CalculaVolumes(*SecaoAnterior,0.0);
    SecaoAnterior = &SecaoAtual;
    Secoes.GetNext(Atual);
  }					
}

void CalcMedicao::GravaArqVolumes()
{
}

void CalcMedicao::GravaArqNota()
{

  SetDlgItemText(IDC_EXECUTANDO,"Gravando o arquivo da nota de servi�o...");
  Prog->SetPos(0);    //-- Posicao Inicial da barra de progresso.

  CFile ArqNota;
  CFileException e;

  CString NomeArquivo = NomeProjeto + ".not";

  if (ArqNota.Open(NomeArquivo,CFile::modeCreate | CFile::modeWrite,&e) == 0)
  {
    return;
  }
  else
  {	
    CArchive ar(&ArqNota,CArchive::store);

    POSITION Atual = Secoes.GetHeadPosition();

    ar << Secoes.GetCount();  //--- N�mero de secoes que ser�o gravadas.

    while(Atual)
    {
      CSecao& Secao = Secoes.GetNext(Atual);
      Secao.Serialize(ar,TRUE);
    }

    ar.Close();
    ArqNota.Close();
  }
}

void CalcMedicao::GravaArqTerrProxMed()
{
  //--- Esta se��o calculada passa a ser o terreno da medi��o anterior.
  //--- Remove do terreno os pontos do off-set esquerdo ao off-set direito e insere nele
  //--- os pontos do projeto da medi��o, este ser� o terreno da pr�xima medi��o

  CFile ArqTerrMedicao;
  CFileException e;

  if (ArqTerrMedicao.Open(NomeProjeto + ".tme",CFile::modeWrite |CFile::modeCreate | CFile::shareDenyNone,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) 
    {
      //monolog.mensagem(1,NomeArquivo.GetBuffer(1));
      return;
    }
  }

  CArchive arp(&ArqTerrMedicao,CArchive::store);

  POSITION PSecaoAtual = Secoes.GetHeadPosition();

  arp << Secoes.GetCount(); 

  while(PSecaoAtual)
  {
    CSecao& SecaoAtual = Secoes.GetNext(PSecaoAtual);

    Perfil PerfilTerrAtual(SecaoAtual.Terreno),
      & PerfilProjAtual= SecaoAtual.Projeto;

    //--- remove do terreno a faixa do off-set esquerdo ao off-set direito 

    PerfilTerrAtual.RemoveFaixa(PerfilProjAtual.GetHead().x,PerfilProjAtual.GetTail().x);

    //--- insere os pontos de projeto na lista de terreno

    PerfilTerrAtual.InsereLista(PerfilProjAtual);

    //--- Serializa o perfil

    PerfilTerrAtual.Serialize(arp);
  }
}

/*
void CalcMedicao::MergeTerrenoNatural(const CString& NomeArquivo)
{
//--- Inclui os pontos do terreno primitivo anteriores ao offset esquerdo da medi��o anterior e
//--- Posteriores ao offset direito na se��o da medi��o anterior.

CString NomeArqTerrPrimitivo(NomeArquivo.Left(NomeArquivo.GetLength() - 6) + ".tca");

CLPerfis TerrenoPrimitivo(NomeArqTerrPrimitivo);

POSITION Atual = TerrenoPrimitivo.GetHeadPosition();

while (Atual)       //--- Varre todo o terreno primitivo
{
Perfil& PerfilTerrPriAtual = TerrenoPrimitivo.GetNext(Atual);   //--- Pega o perfil to Terreno Primitivo

if (PerfilTerrPriAtual.GetCount == 0) continue;

POSITION PPontoTerrNat = PerfilTerrPriAtual.GetHeadPosition();

//--- Procura na medi��o anterior uma se��o igual ao do terreno primitivo, se existir inclui os pontos 

if (MedicaoAnterior.PosicionaNoMaisProximo(PerfilTerrPriAtual.Estaca) == IGUAL)
{
Perfil& PerfilMedAnterior = MedicaoAnterior.GetAt(MedicaoAnterior.PegaAtual());

if (PerfilMedAnterior.GetHeadPosition())   ///--- Se tem pontos na medicacao
{
//--- Pega os primeiros pontos de cada perfil

Ponto* PontoMedAnt = &PerfilMedAnterior.GetHead();

//--- Inclui os os pontos � esquerda 

while (PPontoTerrNat && 
PontoMedAnt->x > PerfilTerrPriAtual.GetAt(PPontoTerrNat).x) 
PerfilMedAnterior.InsereOrdenada(PerfilTerrPriAtual.GetNext(PPontoTerrNat));  

//--- Passa os pontos da medi��o

PontoMedAnt = &PerfilMedAnterior.GetTail();

while (PPontoTerrNat && PontoMedAnt->x > PerfilTerrPriAtual.GetAt(PPontoTerrNat).x) 
PerfilTerrPriAtual.GetNext(PPontoTerrNat);
}
//--- Inclui os pontos da direita, no caso de n�o haver esta estaca na medi��o, a inclui

while (PPontoTerrNat) PerfilMedAnterior.InsereOrdenada(PerfilTerrPriAtual.GetNext(PPontoTerrNat)) ;
}
else MedicaoAnterior.InsereOrdenada(PerfilTerrPriAtual);  //--- Se a secao nao existe na medi��o anterior insere-a
}
}
*/

void CalcMedicao::CalculaMedicao()
{
  MontaSecoes();     //--- Montas as secoes com os perfis das med, anteriores e atuais.
  CalculaOffSets();  //--- Calcula os offsets de todas as se�oes.
  CalculaAreas();    //--- Calcula as �reas das se��es.
  CalculaVolumes();  //--- Calcula os volumes de todas as se��es.
  GravaArqNota();    //--- Temporario, apenas para conpatibilidade com a versao anterior.
  GravaArqVolumes(); //--- Grava o arquivo com os calculos de volumes
  GravaArqTerrProxMed();   //--- Grava o Arquivo com  o perfil de terreno transformado desta medi��o   

  SetDlgItemText(IDC_EXECUTANDO,"O.K., Os c�lculos foram executados com �xito.");

}

BOOL CalcMedicao::OnInitDialog() 
{
  CDialog::OnInitDialog();

  //--- prepara a caixa de dialogo

  CheckDlgButton(IDC_RADPARCIAL,TRUE);
  CheckDlgButton(IDC_COMPLETO,TRUE);

  //--- Pega o numero da medicao.

  CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();
  SetDlgItemText(IDC_NUMMEDICAO,MainFrame->m_sMedicao);

  //--- Muda o tamanho da fonte do edit que cont�m o n�mero da medi�ao.

  LOGFONT Log;

  GetDlgItem(IDC_NUMMEDICAO)->GetFont()->GetLogFont(&Log);
  Log.lfHeight = 55;
  CFont NovaFonte;
  NovaFonte.CreateFontIndirect(&Log);
  GetDlgItem(IDC_NUMMEDICAO)->SetFont(&NovaFonte,TRUE);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CalcMedicao::OnOk() 
{
  EndDialog(TRUE);
}


/* Rotina compat�vel com a vers�o anterior do arquivo de nota de servi�o *****
**************************************************************

	SetDlgItemText(IDC_EXECUTANDO,"Gravando o arquivo da nota de servi�o...");
	Prog->SetPos(0);    //-- Posicao Inicial da barra de progresso.

	CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();
	CString NomeArquivo = MainFrame->m_sNomeComp + ".not";
	
	CFile ArqNota;
	CFileException CFe;
	int Tam = sizeof(struct not);

	ArqNota.Open(NomeArquivo,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary,&CFe);
	if (CFe.m_cause == CFileException::none)
	{
		struct not RegNota;

		for(MedicaoAtual.Atual = MedicaoAtual.Primeira; MedicaoAtual.Atual ; MedicaoAtual++)
		{
			strcpy(RegNota.tiponto,"EST");
			RegNota.ponto.x = (float) MedicaoAtual.Atual->EstVirtual;
			RegNota.ponto.y = (float) MedicaoAtual.Atual->Pontos->CalculaY(0.00);
			ArqNota.Write(&RegNota,Tam);

			for (int i = 0 ; i < 9 ; i++) 
			{
				strcpy(RegNota.tiponto,"DIV");
				RegNota.ponto.x = RegNota.ponto.y = (float) 0.0;

				switch (i)
				{
					case 0: strcpy(RegNota.tiponto,"PIV");break;
					case 6:
					case 7: RegNota.ponto.x = FINFINITO; break;
				}

				ArqNota.Write(&RegNota,Tam);
			}

			for (MedicaoAtual.Atual->Pontos->Atual = MedicaoAtual.Atual->Pontos->Primeiro ; 
					 MedicaoAtual.Atual->Pontos->Atual ; 
					 (*(MedicaoAtual.Atual->Pontos))++)
			{
				strcpy(RegNota.tiponto,"   ");
				RegNota.ponto.x = (float) MedicaoAtual.Atual->Pontos->Atual->Ponto[0];
				RegNota.ponto.y = (float) MedicaoAtual.Atual->Pontos->Atual->Ponto[1];
				ArqNota.Write(&RegNota,Tam);
			}

			Prog->StepIt();
		}
		ArqNota.Close();
	}
	*/
//-----------------------------------------------------------------


	/* Rotina vompat�vel com a vers�o anterior do arquivo de volumes *********
	**************************************************************************

	SetDlgItemText(IDC_EXECUTANDO,"Gravando os volumes...");
	Prog->SetPos(0);    //-- Posicao Inicial da barra de progresso.

	CFile ArqVolumes;
	CFileException CFe;
	struct vol Volumes;
	int Tam = sizeof(struct vol);
	CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();
	CString NomeArquivo = MainFrame->m_sNomeComp + ".vol"; 

	ArqVolumes.Open(NomeArquivo,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary,&CFe);
	if (CFe.m_cause == CFileException::none)
	{
		for(Secoes.Atual = Secoes.Primeira ; Secoes.Atual ; Secoes++)  
		{
			Secoes.Atual->Intersecoes.Atual = Secoes.Atual->Intersecoes.Primeiro;

			while(Secoes.Atual->Intersecoes.Atual)
			{
				for(int i = 0 ; Secoes.Atual->Intersecoes.Atual && i < 3 ; Secoes.Atual->Intersecoes++,i++) 
				{
					Volumes.dados.pontos[i].x = (float) Secoes.Atual->Intersecoes.Atual->Ponto[X];
					Volumes.dados.pontos[i].y = (float) Secoes.Atual->Intersecoes.Atual->Ponto[Y];
				}
				strcpy(Volumes.tipo,"INT");  //--- Interse��es.
				ArqVolumes.Write(&Volumes,Tam);
			}

			strcpy(Volumes.tipo,"EST");
			Volumes.dados.vol.estaca = (float) Secoes.Atual->Terreno->EstVirtual;  //--- Estaca.
			Volumes.dados.vol.difigual = (float) (Secoes.Atual->Terreno->EstVirtual - Secoes.Atual->Terreno->EstReal);
			Volumes.dados.vol.mdist = (float) Secoes.Atual->SemiDistancia;    //--- Semi dist�ncia.

			for (int Tipo = CORTE ; Tipo <= ATERRO ; Tipo++)
			{
				Volumes.dados.vol.cg[Tipo] = (float) 0.0;              //---Centros de gravidade (implementar);
				Volumes.dados.vol.areas[Tipo] = (float) Secoes.Atual->AreaTotal[Tipo];     //-- Areas.
				Volumes.dados.vol.volumes[Tipo] = (float) Secoes.Atual->Volume[Tipo];   //--- volumes atuais.
				Volumes.dados.vol.volumes[Tipo+2] = (float) Secoes.Atual->Volume[Tipo+2]; //--- Volumes totais.
			}
			ArqVolumes.Write(&Volumes,Tam);   //--- Grava o registro de volumes.
			Prog->StepIt();
		}
		ArqVolumes.Close();
		SetDlgItemText(IDC_EXECUTANDO,"Ok, os c�lculos foram executados com exito.");
	}
	*/
