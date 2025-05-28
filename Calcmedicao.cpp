// CalcMedicao.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "estaca.h"
#include "afxtempl.h"
#include "cponto.h"
#include "clpontos.h"
#include "perfil.h"
#include "clistaperfis.h"
//#include "listapon.h"
#include "supercstring.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "csecao.h"
#include "clistasecoes.h"
#include "CalcMedicao.h"
#include "mainfrm.h"
#include "perfilant.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FINFINITO  (float) 3.39999995214436425e38   //--- Infinito para float;

/////////////////////////////////////////////////////////////////////////////
// CalcMedicao dialog

CalcMedicao::CalcMedicao(CWnd* pParent /*=NULL*/)
	: CDialog(CalcMedicao::IDD, pParent)
{
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

  //-- Taludes Padrao -1 1.5 1 -1.5.

  double PontosTaludes[2][2][4] = {{{0.0,0.0,-2000.0, 3000.0},
																		{0.0,0.0,-1500.0,-1000.0}},
	                                 {{0.0,0.0, 2000.0, 3000.0},
	                                  {0.0,0.0, 1500.0,-1000.0}}};

	for(int lado=0 ; lado < 2 ; lado++)
	{
	  for(int tipo=0 ; tipo < 2 ; tipo++)
		{
		  Taludes[lado][tipo].Inclui(99999,0);

		  for(int Ordenada = 0 ; Ordenada < 4 ; Ordenada+=2)
				Taludes[lado][tipo].Atual->IncluiPonto(PontosTaludes[lado][tipo][Ordenada],PontosTaludes[lado][tipo][Ordenada+1]);
		}
	}
  
	CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();
  NomeProjeto = MainFrame->m_sNomeComp;

  //--- Pega a configuracao do calculo.

 UsarEditadas = IsDlgButtonChecked(IDC_RADUSARSECEDI);
 Acumulada = IsDlgButtonChecked(IDC_RADACUMULADA);

  //--- Se ler os dois perfis , o do projeto e o do terreno, calcula a medicào 

  if (LeMedicaoAnterior() && LeMedicaoAtual()) CalculaMedicao();
}

int CalcMedicao::LeMedicaoAnterior()
{
	CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();
	CString NomeArquivo = MainFrame->m_sNomeComp;

  //--- Se não for a primeira medição ou se não for acumulada coloca o numero da medicao anterior no nome
	//--- Caso contrario o arquivo anterior não tem o "MED???", retira-o

	if (!MainFrame->m_sMedicaoAnterior.IsEmpty() && !Acumulada)
	{
	  NomeArquivo = NomeArquivo.Left(NomeArquivo.GetLength() - 3);
		NomeArquivo += MainFrame->m_sMedicaoAnterior;
	}
	else NomeArquivo = NomeArquivo.Left(NomeArquivo.GetLength() - 6);

	NomeArquivo += ".tca";   //--- coloca a extensão para arquivos de terreno (".tca")

  CFile ArqMedAnt;
  CFileException CFe;
	
	ArqMedAnt.Open(NomeArquivo,CFile::modeRead | CFile::typeBinary,&CFe);
  if (CFe.m_cause != CFileException::none) 
	{
	  MessageBox("Erro na abertura do arquivo : " + NomeArquivo);
		return FALSE;
	}

  int Lidos,Tam = sizeof(struct ter);

  //--- Prepara a barra de progresso.

  SetDlgItemText(IDC_EXECUTANDO,"Lendo a medição anterior...");
	Prog = (CProgressCtrl *) GetDlgItem(IDC_PROGRESSOMED);  //--- Inicializa o ponteiro para barra de progresso.

	//--- Inicializa faixa de valores da barra de progresso

	Prog->SetRange(0,ArqMedAnt.GetLength() / Tam);  
	Prog->SetStep(0);   //--- Valor do incremento = 1;
	Prog->SetPos(1);    //-- Posicao Inicial da barra de progresso.

  struct ter MedAnt;

  Lidos = ArqMedAnt.Read(&MedAnt,Tam);

  do
  {       
		Prog->StepIt();

    MedicaoAnterior.Inclui(MedAnt.dados.estaca.valor,0);
		MedicaoAnterior.Atual->IncluiPonto(0.0,MedAnt.dados.ponto.y); //--- Deve ser o x da linha base!!!!

    while((Lidos = ArqMedAnt.Read(&MedAnt,Tam)) == Tam && MedAnt.tipo != 'e')
		{
      MedicaoAnterior.Atual->IncluiPonto(MedAnt.dados.ponto.x,MedAnt.dados.ponto.y);
			Prog->StepIt();
		}

		MedicaoAnterior.Atual->Pontos->Ordena();

  }while (Lidos == Tam);

  ArqMedAnt.Close();

  return TRUE;
}

int CalcMedicao::LeMedicaoAtualNota()
{
	CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();

	CString NomeArquivo = MainFrame->m_sNomeComp + ".not";

  CFile ArqMedAtuNot;
  CFileException CFe;

  ArqMedAtuNot.Open(NomeArquivo,CFile::modeRead,&CFe);
  if (CFe.m_cause != CFileException::none)
	{

	  MessageBox("Erro na abertura do arquivo : " + NomeArquivo);
		return FALSE;
	}

  struct not MedAtuNot;

  unsigned Tam = sizeof(struct not);

  SetDlgItemText(IDC_EXECUTANDO,"Lendo a medição atual - Editada");
  Prog = (CProgressCtrl *) GetDlgItem(IDC_PROGRESSOMED);  //--- Inicializa o ponteiro para barra de progresso.

 //--- Inicializa faixa de valores da barra de progresso

 Prog->SetRange(0,ArqMedAtuNot.GetLength() / Tam);  
 Prog->SetStep(1);   //--- Valor do incremento = 1 porque le um registro anter do wwile;
 Prog->SetPos(1);    //-- Posicao Inicial da barra de progresso.

 unsigned Lidos = ArqMedAtuNot.Read(&MedAtuNot,Tam);

 while(Lidos == Tam) 
 {
    MedicaoAtual.Inclui(MedAtuNot.ponto.x,0);
	  Prog->StepIt();

    for (int i = 0 ; i < 8 ; i++) 
		{
			ArqMedAtuNot.Read(&MedAtuNot,Tam);
			Prog->StepIt();

			//-- Le o div a mais no caso de PIV

			if (!strcmp(MedAtuNot.tiponto,"PIV"))
			{
			  ArqMedAtuNot.Read(&MedAtuNot,Tam);
			  Prog->StepIt();
		  }
		}

	  while((Lidos = ArqMedAtuNot.Read(&MedAtuNot,Tam)) && strcmp(MedAtuNot.tiponto,"EST"))
		{
		  if (strcmp(MedAtuNot.tiponto,"OFE") && strcmp(MedAtuNot.tiponto,"OFD")) 
         MedicaoAtual.Atual->IncluiPonto(MedAtuNot.ponto.x,MedAtuNot.ponto.y);
	
		  Prog->StepIt();
	  }
  }

  ArqMedAtuNot.Close();
	
  return TRUE;
}

int CalcMedicao::LeMedicaoAtual()
{
	CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();
	CString NomeArquivo = MainFrame->m_sNomeComp + ".tca";

  if (!UsarEditadas) 
  {
    CFileException CFe;
    int Lidos,Tam = sizeof(struct ter);
    CFile ArqMedAtu;
    ArqMedAtu.Open(NomeArquivo,CFile::modeRead | CFile::typeBinary,&CFe);
    if (CFe.m_cause != CFileException::none)
		{
		  MessageBox("Erro na abertura do arquivo : " + NomeArquivo);
		  return FALSE;
		}

    SetDlgItemText(IDC_EXECUTANDO,"Lendo a medição atual (Levantada)...");

    //--- Inicializa faixa de valores da barra de progresso

	  Prog->SetRange(0,ArqMedAtu.GetLength() / Tam);  
	  Prog->SetStep(1);   //--- Valor do incremento = 1;
	  Prog->SetPos(0);    //-- Posicao Inicial da barra de progresso.

    struct ter MedAtu;
    Lidos = ArqMedAtu.Read(&MedAtu,Tam);

    do
	  {  
			Prog->StepIt();   //--- Incrementa a barra de progresso.

      MedicaoAtual.Inclui(MedAtu.dados.estaca.valor,0);
      MedicaoAtual.Atual->IncluiPonto(0.0,MedAtu.dados.ponto.y);  //-- Deve ser a x da linha base!!!!!

      while((Lidos = ArqMedAtu.Read(&MedAtu,Tam)) && MedAtu.tipo != 'e')
		  {
        MedicaoAtual.Atual->IncluiPonto(MedAtu.dados.ponto.x,MedAtu.dados.ponto.y);
			  Prog->StepIt();
			}
		
		  MedicaoAtual.Atual->Pontos->Ordena();

		}while (Lidos == Tam);

    ArqMedAtu.Close();
	}
  else LeMedicaoAtualNota();
  
  return TRUE;
}

void CalcMedicao::MontaSecoes()
{
  //--- Prepara a barra de progresso.
  
	SetDlgItemText(IDC_EXECUTANDO,"Preparando as medições para cálculo...");
	Prog = (CProgressCtrl *) GetDlgItem(IDC_PROGRESSOMED);  //--- Inicializa o ponteiro para barra de progresso.

	//--- Inicializa faixa de valores da barra de progresso

	Prog->SetRange(0,MedicaoAnterior.Nitens < MedicaoAtual.Nitens ? MedicaoAnterior.Nitens : MedicaoAtual.Nitens);  

	Prog->SetStep(1);   //--- Valor do incremento = 1;
	Prog->SetPos(0);    //-- Posicao Inicial da barra de progresso.

  //--- Monta as seções para cálculo incluindo os perfís de projeto e de terreno.
  //--- Caso haja apenas um perfil na secão ele é deletado porque não será usado.

  for(MedicaoAnterior.Atual = MedicaoAnterior.Primeira,MedicaoAtual.Atual = MedicaoAtual.Primeira ;
      MedicaoAnterior.Atual && MedicaoAtual.Atual ; )
  {
    Prog->StepIt();   //--- Incrementa a barra de progresso

    //--- Se os perfís são da mesma estaca inclui uma secao na lista de secoes

    if (MedicaoAnterior.Atual->EstVirtual == MedicaoAtual.Atual->EstVirtual)
    {
      Secoes.Inclui(MedicaoAnterior.Atual,MedicaoAtual.Atual);
      MedicaoAnterior++;
      MedicaoAtual++;
    }
    else  //--- Senão exclui o perfil da secao anterior.
    {
      if (MedicaoAnterior.Atual->EstVirtual > MedicaoAtual.Atual->EstVirtual) 
        MedicaoAtual.Exclui(MedicaoAtual.Atual);
      else 
			  MedicaoAnterior.Exclui(MedicaoAnterior.Atual);
    }
  }

  //--- Inclui um ponteiro para os taludes nas secoes.
	//--- Implementar para ler o arquivo;

	for(Secoes.Atual = Secoes.Primeira ; Secoes.Atual ; Secoes++)
		for (int Lado = ESQUERDO ; Lado <= DIREITO ; Lado++)
			for (int Tipo = CORTE ; Tipo <= ATERRO ; Tipo++)
        Secoes.Atual->Taludes[Lado][Tipo] = Taludes[Lado][Tipo].Primeira;
}

void CalcMedicao::CalculaOffSets()
{
  SetDlgItemText(IDC_EXECUTANDO,"Calculando os off-sets...");
	Prog->SetRange(0,Secoes.Nitens);  //--- O nuneto de itens pode ter mudado, se secoes sem ter ou proj.
	Prog->SetPos(0);    //-- Posicao Inicial da barra de progresso.
  
	//--- Para todas as secoes calcule os offsets.

  for(Secoes.Atual = Secoes.Primeira ; Secoes.Atual ;)  
	{
	  //--- Calcula os Offsets da seção, se não achar um dos dois, exclui a seção, ela não interessa,
		//--- No excluir a lista é automaticamente incrementada, caso contrario incrementa a lista.

    if (!Secoes.Atual->CalculaOffSets()) Secoes.Exclui(Secoes.Atual);
		else Secoes++;														 
		                          
		Prog->StepIt();                   //--- Incrementa a barra de prograsso 
	}
}

void CalcMedicao::CalculaAreas()
{
  SetDlgItemText(IDC_EXECUTANDO,"Calculando as áreas...");
	Prog->SetRange(0,Secoes.Nitens);   //--- O número de itens pode ter mudado, se nãa achou off-sets.  
	Prog->SetPos(0);    //-- Posicao Inicial da barra de progresso.

	//--- Para todas as secoes calcule os volumes. 

  for(Secoes.Atual = Secoes.Primeira ; Secoes.Atual ; Secoes.Atual = Secoes.Atual->Proxima)  
	{
    Secoes.Atual->CalculaArea();
		Prog->StepIt();
	}
}

void CalcMedicao::CalculaVolumes()
{
	Prog->SetRange(0,Secoes.Nitens); //--- Inicializa faixa de valores da barra de progresso,
	                                 //--- O número de secoes pode ser diferente.....
  SetDlgItemText(IDC_EXECUTANDO,"Calculando os Volumes...");
	Prog->SetPos(0);    //-- Posicao Inicial da barra de progresso.

	//--- Para todas as secoes calcule os volumes. 

  for(Secoes.Atual = Secoes.Primeira ; Secoes.Atual ; Secoes.Atual = Secoes.Atual->Proxima)  
	{
    Secoes.Atual->CalculaVolumes();
		Prog->StepIt();
	}
}

void CalcMedicao::GravaArqVolumes()
{
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
				strcpy(Volumes.tipo,"INT");  //--- Interseções.
        ArqVolumes.Write(&Volumes,Tam);
			}

      strcpy(Volumes.tipo,"EST");
	    Volumes.dados.vol.estaca = (float) Secoes.Atual->Terreno->EstVirtual;  //--- Estaca.
  		Volumes.dados.vol.difigual = (float) (Secoes.Atual->Terreno->EstVirtual - Secoes.Atual->Terreno->EstReal);
      Volumes.dados.vol.mdist = (float) Secoes.Atual->SemiDistancia;    //--- Semi distância.

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
	  SetDlgItemText(IDC_EXECUTANDO,"Ok, os cálculos foram executados com exito.");
	}
}

void CalcMedicao::GravaArqNota()
{
  SetDlgItemText(IDC_EXECUTANDO,"Gravando o arquivo da nota de serviço...");
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
}

void CalcMedicao::CalculaMedicao()
{
  MontaSecoes();     //--- Montas as secoes com os perfis das med, anteriores e atuais.
  CalculaOffSets();  //--- Calcula os offsets de todas as seçoes.
	CalculaAreas();    //--- Calcula as áreas das seções.
  CalculaVolumes();  //--- Calcula os volumes de todas as seções.
  GravaArqNota();    //--- Temporario, apenas para conpatibilidade com a versao anterior.
  GravaArqVolumes(); //--- Grava o arquivo com os calculos de volumes
}

BOOL CalcMedicao::OnInitDialog() 
{
  CDialog::OnInitDialog();

  //--- prepara a caixa de dialogo
  
	CheckDlgButton(IDC_RADUSARSECEDI,TRUE);
  CheckDlgButton(IDC_RADACUMULADA,TRUE);
  CheckDlgButton(IDC_COMPLETO,TRUE);

  //--- Pega o numero da medicao.

	CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();
	SetDlgItemText(IDC_NUMMEDICAO,MainFrame->m_sMedicao);

 // UsarEditadas = IsDlgButtonChecked(IDC_RADUSARSECEDI);
//	UsarEditadas = FALSE;

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}

void CalcMedicao::OnOk() 
{
  EndDialog(TRUE);
}
