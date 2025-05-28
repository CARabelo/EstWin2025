// CalculaTerreno.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include "math.h"
#include "estaca.h"
#include "afxtempl.h"
#include "cponto.h"
#include "clPontos.h"
#include "perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "cvisada.h"
#include "carqnivelamento.h"
#include "supercstring.h"
#include "carqterrpontos.h"
#include "cterrpontos.h"
#include "CalculaTerreno.h"
#include <string>
#include <map> 
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "io.h"
#include "monologo.h"

#define INFINITO   1.7976931348623158e+308

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class monologo monolog;  

/////////////////////////////////////////////////////////////////////////////

void DadDouble::Converte(CVisada Visada, CString IgStr, CString PlRefStr, CString LeituraStr, CString CotaStr)
{
  //Passa a estrutura niv de strings para nivd de doubles 
  CString Igualdade;
  Igualdade.Format("%6i",Ig);
  if (Visada.EEstaca()) 
  {
    PontoLido.Constroi(Visada,Igualdade);
  }
  else 
  {
    Afastamento = atof(Visada);
  }
  PlRef = 0.0;
  if (!Visada.EMais()) Cota = atof(CotaStr);
  Leitura = atof(LeituraStr) / 1000.0;
  Ig = atoi(IgStr);
}

// CalculaTerreno dialog

CalculaTerreno::CalculaTerreno(CString NomeProj,CWnd* pParent) : CDialog(CalculaTerreno::IDD, pParent),NomeProjeto(NomeProj)
{
  //{{AFX_DATA_INIT(CalculaTerreno)
  m_IgFim = 0;
  m_IgIni = 0;
  m_EstFim = _T("");
  m_EstIni = _T("");
  //}}AFX_DATA_INIT
}

void CalculaTerreno::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CalculaTerreno)
  DDX_Text(pDX, IDC_EDIIGFIM, m_IgFim);
  DDV_MinMaxInt(pDX, m_IgFim, 0, 99);
  DDX_Text(pDX, IDC_EDIIGINI, m_IgIni);
  DDV_MinMaxInt(pDX, m_IgIni, 0, 99);
  DDX_Text(pDX, IDC_EDIESTFIM, m_EstFim);
  DDV_MaxChars(pDX, m_EstFim, 14);
  DDX_Text(pDX, IDC_EDIESTINI, m_EstIni);
  DDV_MaxChars(pDX, m_EstIni, 14);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CalculaTerreno, CDialog)
  //{{AFX_MSG_MAP(CalculaTerreno)
  ON_BN_CLICKED(IDC_COMPLETO, OnCompleto)
  ON_BN_CLICKED(IDC_CHENIV, OnCheniv)
  ON_BN_CLICKED(IDC_CHETER, OnCheter)
  ON_WM_CREATE()
  ON_WM_KILLFOCUS()
  ON_BN_CLICKED(IDC_INICIAR, OnIniciar)
  ON_BN_CLICKED(IDC_OK, OnOk)
  ON_BN_CLICKED(IDC_CANCELAR, OnCancelar)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CalculaTerreno message handlers

BOOL CalculaTerreno::OnInitDialog() 
{
  CDialog::OnInitDialog();

  //--- Inicializa os dados do di�logo.

  CheckDlgButton(IDC_RADATUAIS,TRUE);  //--- Usar cotas atuais.
  CheckDlgButton(IDC_COMPLETO,TRUE);   //--- C�lculo completo.
  CheckDlgButton(IDC_CHETER,FALSE);     //--- Usar arquivo de terreno. por pontos.
  CheckDlgButton(IDC_RADPRINIV,TRUE);  //--- Prioridade de c�lculo-nivelamento.

  GetDlgItem(IDC_OK)->EnableWindow(FALSE); //--- Desabilita o bot�o OK.

  struct _finddata_t c_file;
  if( (_findfirst((char *) LPCTSTR (NomeProjeto + ".NIV"), &c_file )) != -1L ) 
    CheckDlgButton(IDC_CHENIV,TRUE);     //--- Usar arquivo de nivelamento.
  else
    GetDlgItem(IDC_CHENIV)->EnableWindow(FALSE); //--- N�o existe arq. de niv.

  if( (_findfirst((char *) LPCTSTR (NomeProjeto + ".TER"), &c_file )) != -1L ) 
    CheckDlgButton(IDC_CHETER,TRUE);     //--- Usar arquivo de nivelamento.
  else
    GetDlgItem(IDC_CHETER)->EnableWindow(FALSE); //--- N�o existe arq. de Terr. po pontos

  //--- Se o nivelamento e o terr. por pontos n�o est�o habilitados � porque
  //--- eles n�o existem, se n�o existem n�o h� o que calcular.

  if (!GetDlgItem(IDC_CHENIV)->IsWindowEnabled() && !GetDlgItem(IDC_CHETER)->IsWindowEnabled())
  {
    monolog.mensagem(9,"Crie primeiro o nivelamento das se��es e/ou terreno por pontos"); 
    EndDialog(FALSE);
    return FALSE;
  }

  //--- Se o arquivo de niv. ou o de terreno n�o existem, desabilita-os como prioridade
  //--- Uum dos dois est� habilitado, conforme if anterior.

  if (!GetDlgItem(IDC_CHENIV)->IsWindowEnabled()) 
  {
    CheckDlgButton(IDC_RADPRINIV,FALSE);             //--- Tira a marca.
    CheckDlgButton(IDC_RADPRITER,TRUE);              //--- Marca o terr. por pontos.
    GetDlgItem(IDC_RADPRINIV)->EnableWindow(FALSE);  //-- Desabilita o Niv.. 
  }
  else
  {
    if (!GetDlgItem(IDC_CHETER)->IsWindowEnabled()) 
    {
      CheckDlgButton(IDC_RADPRITER,FALSE);             //--- Tira a marca.
      CheckDlgButton(IDC_RADPRINIV,TRUE);              //--- Marca o nivelamento.
      GetDlgItem(IDC_RADPRITER)->EnableWindow(FALSE);  //--- Desabilita.
    }
  }

  GetDlgItem(IDC_PAUSA)->EnableWindow(FALSE);  //--- Desabilita o pausa.
  GetDlgItem(IDC_OK)->EnableWindow(FALSE);  //--- Desabilita o ok.

  CString Titulo;
  GetWindowText(Titulo);
  SetWindowText(Titulo + NomeProjeto);  //--- Acrescenta o nome do projeto ao t�tulo.

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CalculaTerreno::OnCompleto() 
{
  UINT DESMARCADO = 8;
  struct _finddata_t c_file;

  //--- Se desmarcar o trecho completo, habilita a entrada de dados do trecho
  //--- caso contr�rio desabilita.

  if (((CButton *) GetDlgItem(IDC_COMPLETO))->GetState() == DESMARCADO)
  {
    //GetDlgItem(IDC_INICIO)->EnableWindow(TRUE);
    //GetDlgItem(IDC_FIM)->EnableWindow(TRUE);
    GetDlgItem(IDC_STAESTINI)->EnableWindow(TRUE);
    GetDlgItem(IDC_STAESTFIM)->EnableWindow(TRUE);
    GetDlgItem(IDC_STAIGINI)->EnableWindow(TRUE);
    GetDlgItem(IDC_STAIGFIM)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDIIGINI)->EnableWindow(TRUE);
    GetDlgItem(IDC_EDIIGFIM)->EnableWindow(TRUE);

    //--- Se existe o arquivo.tca habilita inclus�o de se��es existentes.

    if( (_findfirst((char *) LPCTSTR (NomeProjeto + ".TCA"), &c_file )) != -1L ) //--- Se existe terreno calculado...
    {
      CheckDlgButton(IDC_CHEINCLUIR,TRUE);     //--- Incluir secoes existentes.
      GetDlgItem(IDC_CHEINCLUIR)->EnableWindow(TRUE);
      GetDlgItem(IDC_STAINCLUIR)->EnableWindow(TRUE);
    }
  }
  else
  {
    //    GetDlgItem(IDC_INICIO)->EnableWindow(FALSE);
    //--    GetDlgItem(IDC_FIM)->EnableWindow(FALSE);
    GetDlgItem(IDC_STAESTINI)->EnableWindow(FALSE);
    GetDlgItem(IDC_STAESTFIM)->EnableWindow(FALSE);
    GetDlgItem(IDC_STAIGINI)->EnableWindow(FALSE);
    GetDlgItem(IDC_STAIGFIM)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIIGINI)->EnableWindow(FALSE);   
    GetDlgItem(IDC_EDIIGFIM)->EnableWindow(FALSE);

    //--- Se existe o arquivo.tca habilita inclus�o de se��es existentes.

    if( (_findfirst((char *) LPCTSTR (NomeProjeto + ".TCA"), &c_file )) != -1L ) //--- Se existe terreno calculado...
    {
      CheckDlgButton(IDC_CHEINCLUIR,FALSE);     //--- Incluir secoes existentes.
      GetDlgItem(IDC_CHEINCLUIR)->EnableWindow(FALSE);
      GetDlgItem(IDC_STAINCLUIR)->EnableWindow(FALSE);
    }
  }
}

void CalculaTerreno::OnCheniv() 
{
  if (!IsDlgButtonChecked(IDC_CHENIV))   //--- Se n�o usar o arquivo de nivelamento...
  {
    if (IsDlgButtonChecked(IDC_CHETER))  //--- Se o terreno estiver marcado...
    {
      CheckDlgButton(IDC_RADPRINIV,FALSE);  //--- Retira a marca.
      CheckDlgButton(IDC_RADPRITER,TRUE);   //--- Marca o terreno.
      GetDlgItem(IDC_RADPRINIV)->EnableWindow(FALSE); //--- Desabilita a prioridade para. niv.
    }
    else                                //--- se o terreno tamb�m n�o estiver marcado... 
    {
      monolog.mensagem(10,"");             //--- deve ser usado no m�nimo um dos arquivos.
      CheckDlgButton(IDC_CHENIV,TRUE);  //--- Mantem a marca.
    }
  }
  else GetDlgItem(IDC_RADPRINIV)->EnableWindow(TRUE); //--- Se usar habilita a prioridade para ele.
}

void CalculaTerreno::OnCheter() 
{
  if (!IsDlgButtonChecked(IDC_CHETER))   //--- Se n�o usar o arquivo de terr. por pontos...
  {
    if (IsDlgButtonChecked(IDC_CHENIV))  //--- Se o nivelamanto estiver marcado...
    { 
      CheckDlgButton(IDC_RADPRITER,FALSE);  //--- Retira a marca.
      CheckDlgButton(IDC_RADPRINIV,TRUE);   //--- Marca o projeto.
      GetDlgItem(IDC_RADPRITER)->EnableWindow(FALSE); //--- Desabilita a prioridade para terr.
    }
    else                                //--- Se o nivelamento tamb�m n�o estiver marcado
    {
      monolog.mensagem(10,"");          //--- deve ser usado no m�nimo um dos arquivos.
      CheckDlgButton(IDC_CHETER,TRUE);  //--- Mantem a marca.
    }
  }
  else GetDlgItem(IDC_RADPRITER)->EnableWindow(TRUE);  //--- Se usar habilita a prioridade para ele.
}

int CalculaTerreno::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CDialog::OnCreate(lpCreateStruct) == -1) return -1;

  CenterWindow();

  return 0;
}

void CalculaTerreno::OnKillFocus(CWnd* pNewWnd) 
{
  CDialog::OnKillFocus(pNewWnd);
  /*	
  WINDOWPLACEMENT lpwndpl;
  GetWindowPlacement(&lpwndpl); 

  lpwndpl.showCmd |= SW_MINIMIZE;

  SetWindowPlacement(&lpwndpl);
  */
}

void CalculaTerreno::OnIniciar() 
{
  GetDlgItem(IDC_INICIAR)->EnableWindow(FALSE); //--- Desabilita o bot�o iniciar.
  GetDlgItem(IDC_PAUSA)->EnableWindow(TRUE);    //--- Habilita o bot�o pausa.

  //--- Seta as variaveis booleanas para posterior referencia no c�lculo.

  TIPO = (((CButton *) GetDlgItem(IDC_RADATUAIS))->GetCheck() == TRUE);
  PRIORIDADE = (((CButton *) GetDlgItem(IDC_RADPRINIV))->GetCheck() == TRUE);
  USARNIV = (((CButton *) GetDlgItem(IDC_CHENIV))->GetCheck() == TRUE);
  USARTER = (((CButton *) GetDlgItem(IDC_CHETER))->GetCheck() == TRUE);

  GeraListaNivelamento();
  GravaTerreno();

  SetDlgItemText(IDC_EXECUTANDO,"O.K., os c�lculos foram executados com �xito.");
  GetDlgItem(IDC_CANCELAR)->EnableWindow(FALSE);  //--- Desabilita o cancelar.
  GetDlgItem(IDC_OK)->EnableWindow(TRUE);  //--- Habilita o ok.
}

void CalculaTerreno::GravaTerreno()
{
  //--- Se for para usar o terreno por pontos L� o arquivo de terreno por pontos e 
  //--- funde as duas listas em uma s�, resolvendo as ambiguidades.

  if (USARTER) 
  {
    SetDlgItemText(IDC_EXECUTANDO,"Abrindo o arquivo de terreno por pontos...");
    CTerrPontos TerrenoPorPontos(NomeProjeto,Prog);   //--- Abre o arquivo e cria a lista de terreno por pontos

    //--- Funde as tres listas em uma s� resolvendo as ambiguidades de se��es iguais.
    //--- A lista final retorna na lista de nivalamento, as se��es da lista de terreno 
    //--- por pontos s�o incluidas na lista de nivelamento e excluidas da lista de terreno
    //--- por pontos, no final a lista de terreno por pontos estar� vazia e a lista
    //--- de nivelamento estar� completa.

    SetDlgItemText(IDC_EXECUTANDO,"Gerando o terreno por pontos...");

    POSITION TerrAtual = TerrenoPorPontos.LTerrPontos.GetHeadPosition(),
      NivAtual = TerrenoNivelado.GetHeadPosition();

    Perfil NoTerrAtual,NoNivAtual;

    if (TerrAtual) NoTerrAtual = TerrenoPorPontos.LTerrPontos.GetAt(TerrAtual);
    if (NivAtual) NoNivAtual = TerrenoNivelado.GetAt(NivAtual);

    while (TerrAtual)
    {
      if (NivAtual)
      {
        if (NoTerrAtual.Estaca.EstVirtual < NoNivAtual.Estaca.EstVirtual) 
          TerrenoNivelado.InsertBefore(NivAtual,NoTerrAtual);	
        else 
        {
          if (NoTerrAtual.Estaca.EstVirtual == NoNivAtual.Estaca.EstVirtual) //--- Grava nivelamento
            if (PRIORIDADE == TERRPONTOS) TerrenoNivelado.InsertBefore(NivAtual,NoTerrAtual);	  
        }

        //--- Se a se��o de terreno por pontos � menor ou igual a de nivelamento ela
        //--- n�o ser� mais necess�ria, se menor ela foi inclu�da na lista de nivelamento, 
        //--- se igual a ambiguidade foi resolvida, ou ela foi inclu�da ou n�o na lista de
        //--- nivelamento (dependendo de PRIORIDADE). Exclui ent�o a se��o.

        if (NoTerrAtual.Estaca.EstVirtual <= NoNivAtual.Estaca.EstVirtual)  //--- Grava nivelamento
        {
          POSITION Temp = TerrAtual;
          TerrenoPorPontos.LTerrPontos.GetNext(TerrAtual);
          if (TerrAtual) NoTerrAtual = TerrenoPorPontos.LTerrPontos.GetAt(TerrAtual);
          TerrenoPorPontos.LTerrPontos.RemoveAt(Temp);	  
        }
        TerrenoNivelado.GetNext(NivAtual);
        if (NivAtual) NoNivAtual = TerrenoNivelado.GetNext(NivAtual);
      }
      else 
      {
        TerrenoNivelado.AddTail(NoTerrAtual);
        if (TerrAtual)
        {
          POSITION Temp = TerrAtual;
          TerrenoPorPontos.LTerrPontos.GetNext(TerrAtual);
          if (TerrAtual) NoTerrAtual = TerrenoPorPontos.LTerrPontos.GetAt(TerrAtual);
          TerrenoPorPontos.LTerrPontos.RemoveAt(Temp);	  
        }
      }
    }
  }
  SetDlgItemText(IDC_EXECUTANDO,"Gravando o arquivo do terreno calculado...");
  if (/*!EMedicao*/ 1) TerrenoNivelado.GravarArquivo(NomeProjeto); //--- Grava o Arquivo do terreno calculado (.tca)
  //else GravaMedicao();      //--- Grava como medicao
}

void CalculaTerreno::GeraListaNivelamento()
{
  int NumIg = 0;
  double PlanoRef = 0.0;
  SetDlgItemText(IDC_EXECUTANDO,"Abrindo o arquivo de nivelamento.");
  CArqNivelamento ArqNivel(NomeProjeto);
  SetDlgItemText(IDC_EXECUTANDO,"Calculando o nivelamento.");
  Prog = (CProgressCtrl *) GetDlgItem(IDC_PROGRESSO);
  Prog->SetRange(0,ArqNivel.GetCount()+1);
  Prog->SetStep(1);

  POSITION Atual(ArqNivel.GetHeadPosition());
  if (!Atual) return;

  //--- Gera a lista do arquivo de nivelamento na memoria e 
  //--- calcula a caderneta de nivelamento, gravando o arquivo da caderneta
  //--- calculada 

  ArqNivelamento NoAtual = ArqNivel.GetAt(Atual);
  Prog->StepIt();

  while(Atual)  //--- Enquanto houverem registros...
  {
    Perfil  SecaoAtual;

    DadosDouble.Converte(NoAtual.Visada,NoAtual.Ig,NoAtual.PlRef,NoAtual.Leitura,NoAtual.Cota);       
    if (DadosDouble.Ig < NumIg) DadosDouble.Ig = NumIg;  //--- Mant�m a �ltima igualdade.
    else NumIg = DadosDouble.Ig;

    if (TIPO == DIGITADA || Atual == ArqNivel.GetHeadPosition())     //--- Se tipo estiver para digitada...
    {
      PlanoRef = DadosDouble.Cota + DadosDouble.Leitura;
      NoAtual.PlRef.Format("%8.3f",PlanoRef);
      NoAtual.Cota.Format("%8.3f",atof(NoAtual.Cota));   //--- formata o que foi digitado.
    }
    else 
    {													 
      //--- formata os campos para estaca com visada menos.

      DadosDouble.Cota = PlanoRef - DadosDouble.Leitura; 
      NoAtual.Cota.Format("%8.3f",DadosDouble.Cota);  
      NoAtual.PlRef.Empty();
    }

    if (NoAtual.Visada.EEstaca()) 
    {
      SecaoAtual.Estaca.Constroi(NoAtual.Visada,NoAtual.Cota,NoAtual.Ig);
      SecaoAtual.InsereOrdenada(Ponto(0.0,DadosDouble.Cota,INFINITO,INFINITO,INFINITO,INFINITO,CString("TERREN")));
    }

    ArqNivel.SetAt(Atual,NoAtual);
    ArqNivel.GetNext(Atual);
    Prog->StepIt();
    if (!Atual) break;   //--- Se u �ltimo registro for uma estaca...;
    NoAtual = ArqNivel.GetAt(Atual);

    while (Atual && !NoAtual.Visada.EEstaca())
    {
      DadosDouble.Converte(NoAtual.Visada,NoAtual.Ig,NoAtual.PlRef,NoAtual.Leitura,NoAtual.Cota);       

      if (NoAtual.Visada.EMais()) 
      {
        DadosDouble.PlRef = PlanoRef = DadosDouble.Cota + DadosDouble.Leitura;
        NoAtual.PlRef.Format("%8.3f",DadosDouble.PlRef);
        NoAtual.Cota.Empty();
      }
      else
      {
        DadosDouble.Cota = PlanoRef - DadosDouble.Leitura;   //--- Calcula menos

        //--- Formata o campo para visada menos. 

        DadosDouble.Cota = PlanoRef - DadosDouble.Leitura;
        NoAtual.Cota.Format("%8.3f",DadosDouble.Cota);  
        NoAtual.PlRef.Empty();
      }

      if (NoAtual.Visada.ENumero())
        SecaoAtual.InsereOrdenada(Ponto(DadosDouble.Afastamento,DadosDouble.Cota,INFINITO,INFINITO,INFINITO,INFINITO,CString("TERREN")));

      ArqNivel.SetAt(Atual,NoAtual);
      ArqNivel.GetNext(Atual);
      Prog->StepIt();
      if (Atual) NoAtual = ArqNivel.GetAt(Atual);
    }
    //--- Nos nivelamentos do eixo feito junto com o das se�oes o inicio do nivelamento
    //--- (do ps at� a primeira secao, gera uma se��o com estaca == INFINITO e ela 
    //--- n�o deve ser gravada.

    if (SecaoAtual.Estaca.EstVirtual != INFINITO) 
      TerrenoNivelado.InsereOrdenada(SecaoAtual);
  }
  SetDlgItemText(IDC_EXECUTANDO,"Gravando o arquivo calculado.");
  ArqNivel.GravarArquivo();
}

/*
void CalculaTerreno::GravaSec(class perfil *perfil,CFile *ArqTerreno)
{
if (!perfil->Pontos) return;
perfil->Pontos->Ordena();  //--- Ordena a lista de pontos.
CFile ArqTerCalc;
struct ter Terreno;
int Tam = sizeof(struct ter);

Terreno.tipo = 'e';
Terreno.dados.estaca.valor = (float) perfil->EstVirtual;
Terreno.dados.estaca.cota = (float) perfil->Pontos->CalculaY(0.00);
ArqTerreno->Write(&Terreno,Tam);

for(perfil->Pontos->Atual = perfil->Pontos->Primeiro ; perfil->Pontos->Atual ; perfil->Pontos->Atual  = perfil->Pontos->Atual->Proximo)
{
Terreno.tipo = 'p';
Terreno.dados.ponto.x = (float) perfil->Pontos->Atual->Ponto[X];
Terreno.dados.ponto.y = (float) perfil->Pontos->Atual->Ponto[Y];
ArqTerreno->Write(&Terreno,Tam);
}
}

void CalculaTerreno::TrataExecao(CFileException *e)
{

}
/*
void CalculaTerreno::PegaTrecho()
{

CString EstacaIni,EstacaFim,IgIni,IgFim;

//--- Pega os dados do di�logo

GetDlgItemText(IDC_EDIESTFIM,EstacaIni);
GetDlgItemText(IDC_EDIESTINI,EstacaFim);
GetDlgItemText(IDC_EDIIGINI,IgIni);
GetDlgItemText(IDC_EDIIGFIM,IgFim);

//--- Cria as estacas. Se a caixa inicial estiver vazia o inicio = -infinito,
//--- Se a caixa final estiver vazia o fim = +infinito.

if (EstacaIni.IsEmpty()) EstacaInicial.Constroi(-INFINITO,0);
else EstacaInicial.Constroi(EstacaIni,IgIni);
if (EstacaFinal.IsEmpty()) EstacaFinal.Constroi(INFINITO,0);
else EstacaFinal.Constroi(EstacaFinal,IgFim);
}


void CalculaTerreno::CalculaTerr()
{
//--- Inicializa o ponteiro para barra de progresso.

Prog = (CProgressCtrl *) GetDlgItem(IDC_PROGRESSO);

//--- Busca o inicio e o fim do trecho, caso as caixas n�o tenham sido preenchidas
//--- o in�cio ficar� em -infinito e o fim em +infinito.

PegaTrecho();

//--- Posiciona a Secao de nivelamento e do terreno por pontos na primeira secao 
//--- >= � estaca inicial do trecho.

Nivelamento.Posiciona(MENOR,EstacaInicial.EstVirtual);
TerrPontos.Posiciona(MENOR,EstacaInicial.EstVirtual);

//--- Cria e configura a faixa de valores da barra de progresso, o valor m�ximo 
//--- depende da existencia dos arquivos e do arquivo que contem a maior estaca
//--- Neste ponto as listas est�o com a �ltima estaca lida.

if (Nivelamento.Atual)
{
if (TerrPontos.Atual)
{
if (Nivelamento.Atual->EstVirtual > TerrPontos.Atual->EstVirtual) Prog->SetRange(Inicio_Faixa,(int) Nivelamento.Atual->EstVirtual);
else Prog->SetRange(Inicio_Faixa,(int) TerrPontos.Atual->EstVirtual);
}
else Prog->SetRange(Inicio_Faixa,(int) Nivelamento.Atual->EstVirtual);
}
else if (TerrPontos.Atual) Prog->SetRange(Inicio_Faixa,(int) TerrPontos.Atual->EstVirtual);

SetDlgItemText(IDC_EXECUTANDO," Gravando o arquivo calculado..."); //--- Texto da barra de progresso.

while ((Nivelamento.Atual && Nivelamento.Atual->EstVirtual <= EstacaFinal.EstVirtual) ||
(TerrPontos.Atual && TerrPontos.Atual->EstVirtual <= EstacaFinal.EstVirtual))  //--- Enquanto houverem se��es...
{
if (Nivelamento.Atual)  //--- Se existe estaca atual de nivelamento...
{
if (TerrPontos.Atual)  //--- Se existe estaca atual de terreno por pontos
{
//--- Se a estaca de nivelamento � anterior � do terreno por pontos... 

if (Nivelamento.Atual->EstVirtual < TerrPontos.Atual->EstVirtual) 
{
GravaSec(Nivelamento.Atual,&ArqTerr);   //--- Passa a secao do nivelamento.
Prog->SetPos((int) Nivelamento.Atual->EstVirtual);
Nivelamento++;   //--- passa para a proxima sec�o de nivelamento.
}
else 
{
//--- Se � estaca do terreno por pontos for anterior � de nivelamento..
/*
if (TerrPontos.Atual->EstVirtual < Nivelamento.Atual->EstVirtual) 
{
GravaSec(TerrPontos.Atual,&ArqTerr);  //--- Passa a se��o de terreno por pontos.
Prog->SetPos((int) TerrPontos.Atual->EstVirtual);
TerrPontos++;
}
else //---sen�o as se��es s�o iguais (mesma estaca) 
{
//--- Se a prioridade � para o nivelamento passa a se��o de nivelamento,
//--- sen�o passa a de terreno por pontos

if (PRIORIDADE == NIVELAMENTO) GravaSec(Nivelamento.Atual,&ArqTerr);
else GravaSec(TerrPontos.Atual,&ArqTerr);

Prog->SetPos((int) Nivelamento.Atual->EstVirtual);
Nivelamento++;  //--- Passa para a proxima se��o de nivelamento.
TerrPontos++;   //--- Passa para a pr�xima se��o de terreno por pontos.
}
}
}
else  //--- Se tem nivelamento e n�o tem terreno por pontos..
{
GravaSec(Nivelamento.Atual,&ArqTerr);  //--- Passa a se��o de nivelamento.
Prog->SetPos((int) Nivelamento.Atual->EstVirtual);
Nivelamento++;    //--- Passa para a proxima se��o de nivelamento.   
}
}
else       //-- Se n�o tem nivelamento.
{
GravaSec(TerrPontos.Atual,&ArqTerr);   //--- Passa a se��o de terreno por pontos.
Prog->SetPos((int) TerrPontos.Atual->EstVirtual);
TerrPontos++;     //--- Passa para a pr�xima se��o de terreno por pontos.
}
}
//--- Fecha o arquivo

ArqTerr.Close();

//--- O programa chegou ao fim ent�o remove os arquivos anteriores e 
//--- renomeia os arquivos para o nome original.

try
{
CFile::Remove(NomeProjeto + ".TCA");
}
catch (CFileException* e)
{
TrataExecao(e);
}

CFile::Rename(NomeProjeto + ".TC@",NomeProjeto + ".TCA");

if (USARNIV) 
{
try
{
CFile::Remove(NomeProjeto + ".NIB");
}
catch(CFileException* e)
{
TrataExecao(e);
}

CFile::Rename(NomeProjeto + ".NIV",NomeProjeto + ".NIB");
CFile::Rename(NomeProjeto + ".NI@",NomeProjeto + ".NIV");
}
SetDlgItemText(IDC_EXECUTANDO," Os c�lculos foram executados com �xito.");
GetDlgItem(IDC_OK)->EnableWindow(TRUE); //--- Habilita o bot�o OK.
GetDlgItem(IDC_CANCELAR)->EnableWindow(FALSE); //--- Desabilita o bot�o cancelar.
GetDlgItem(IDC_PAUSA)->EnableWindow(FALSE); //--- Desabilita o bot�o pausa.
}

void CalculaTerreno::OnOk() 
{
DestroyWindow();	//--- Finaliza o di�logo
}

/*
atual = GetHeadPosition();

No = GetAt(Atual);

while (atual)
{
processa a estaca;

GetNext(Atual);
if (!Atual) return;
NoAtual = GetAt(Atual);

while (atual && !NoAtual.EEstVirtual)
{
processa os pontos;
GetNext(Atual);
if (!Atual) return;
NoAtual = GetAt(Atual);
}
}  


*/
/*
void GravaMedicao()
{

POSITION Atual(TerrenoNivelado.GetHeadPosition());

if (ar.IsStoring())
{ 
ar << GetCount();

while (Atual)
{

for (int i = 0 ; i < 6 ; i++) ar << (double) 0.0;   //--- Na meddi 
Perfil& PerfilAtual = GetNext(Atual);
PerfilAtual.Serialize(ar);
}
}

while(NivAtual)
{


}



}
*/
void CalculaTerreno::OnOk() {EndDialog(TRUE);}	//--- Finaliza o di�logo

void CalculaTerreno::OnCancelar() {EndDialog(TRUE);} //---Finaliza o Dialogo

