// arqnovo.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include "arqnovo.h"
#include "io.h"
#include "monologo.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ARQNOVO  0

/////////////////////////////////////////////////////////////////////////////
// arqnovo dialog

extern class monologo monolog;

arqnovo::arqnovo(CWnd* pParent /*=NULL*/)	: CDialog(arqnovo::IDD, pParent)
{
   NARQ = 19;

  struct arqu rasc[19] =
  {{"VER","CURVAS VERTICAIS",0},
   {"HOR","CURVAS HORIZONTAIS",0},
   {"SEC","SEÇÕES TIPO",0},
   {"ALS","ALARG., SARJETAS E PASSEIOS",0},
   {"OAC","OBRAS DE ARTE CORRENTES",0},
   {"OAE","OBRAS DE ARTE ESPECIAIS",0},
   {"TPA","TALUDES DO PAVIMENTO",0},
   {"TAC","TALUDES DE CORTE",0},
   {"TAA","TALUDES DE ATERRO",0},
   {"CAN","CANTEIROS CENTRAIS",0},
   {"LIM","LIMPEZA DO TERRENO",0},
   {"PAV","EXPESSURAS DO PAVIMENTO",0},
   {"GRA","GRADIENTE",0},
   {"FIX","PISTAS FIXAS",0},
   {"EMP","EMPOLAMENTOS",0},
   {"TPS","TIPOS DE SOLOS",0},
   {"FSO","FUROS DE SONDAGEM",0},
   {"ATO","ACIDENTES TOPOGRÁFICOS",0},
   {"RAV","CAMADAS AUTOCAD",0}
  };

  memcpy(&arquivos,&rasc,sizeof(struct arqu) * NARQ);


  //{{AFX_DATA_INIT(arqnovo)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void arqnovo::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(arqnovo)
  // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(arqnovo, CDialog)
  //{{AFX_MSG_MAP(arqnovo)
  ON_LBN_DBLCLK(IDC_ARQUIVOS, OnDblclkArquivos)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// arqnovo message handlers

int  arqnovo::PreencheLista() 
{
  struct _finddata_t c_file;
  intptr_t hFile;
  CString  nomes[500];
  int i(0), z, C(-1);
  std::string NomeOriginal(Nome);

  //---  Se estiver abrindo um novo arquivo, separa os arquivos que ainda não 
  //---  existem e mostra a lista, caso esteja abrindo um arquivo existemte, mostra
  //---  a lista dos arquivos existentes.

  //--- Acha todos os arquivos existentes do projeto, separa a extensão.

  //--- 14/06/2023 os arquivo ,ato e .rav ficam no diretorio pai 
  //--- então tem q procurar nos 2 diretórios

  for (C = 0; C < 2; C++)
  {
    if(C == 1)
    {
      std::string DirProjeto(Nome);

      for (int i = 0; i < 2; i++)
      {
        DirProjeto = DirProjeto.substr(0,DirProjeto.find_last_of('\\'));
      }

      std::string NomeProj(DirProjeto.substr(DirProjeto.find_last_of('\\')));

      Nome = DirProjeto.c_str();
      Nome += NomeProj.c_str();
    }
  if( (hFile = _findfirst((char *) LPCTSTR (Nome + ".*"), &c_file )) == -1L ) 
  {
      if(C == 0)
      {
        monolog.mensagem(6,Nome);
        return FALSE;
      }
  }
  else
  {
    do
    {
      nomes[i] = c_file.name;
      nomes[i] = nomes[i].Right(nomes[i].GetLength() - nomes[i].Find('.')-1);
      i++;
    }
    while( _findnext( hFile, &c_file ) == 0  && i < 50);

    _findclose(hFile);
    }
  }

  for (z=0 ; z < NARQ ; arquivos[z].achou = FALSE,z++); //--- Limpa as marcas Anteriores.

  //--- Marca os arquivos existentes.

  for (i-- ; i >=0 ; i--)
  {
    for(z=0 ; z < NARQ ;   z++)
    {
      if(!(nomes[i].CompareNoCase(arquivos[z].ext[0]))) break;
    }
    arquivos[z].achou = (z < NARQ);
  }

  //--- Varre a lista,  dependendo do tipo, inclui o arquivo na lista.

  for(z=0 ; z < NARQ ; z++) 
  { 
    if ((Tipo == ARQNOVO && !arquivos[z].achou) || (Tipo != ARQNOVO &&  arquivos[z].achou))
    {
      #if defined (PACOTE_INTERMEDIARIO) || defined (PACOTE_AVANCADO)
        ((CListBox *) GetDlgItem(IDC_ARQUIVOS))->AddString(arquivos[z].ext[1]);    
      #else
        if(arquivos[z].ext[0] != "FSO" && arquivos[z].ext[0] != "TPS") 
          ((CListBox*)GetDlgItem(IDC_ARQUIVOS))->AddString(arquivos[z].ext[1]);
      #endif
    }
  }

  //--- Se a lista é para criar e a lista está vazia, todos os
  //---  arquivos já foram criados.

  if (Tipo == ARQNOVO && ((CListBox *)GetDlgItem(IDC_ARQUIVOS))->GetCount() == 0)
  {
    monolog.mensagem(7,"");
    return FALSE;
  }
  else
  {
    //--- Se a lista é para abrir e a lista está vazia, nenhum arquivo ainda
    //--- foi criado.

    if (Tipo != ARQNOVO && ((CListBox *)GetDlgItem(IDC_ARQUIVOS))->GetCount() == 0)
    {
      monolog.mensagem(8,"");
      return FALSE;
    }
  }
  
  Nome = NomeOriginal.c_str();
  
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

CString arqnovo::Dialoga(int tipo,CString nome)
{
  CString Arq = "";

  if (!nome.IsEmpty())
  {
    Tipo = tipo;
    Nome = nome;

    if (DoModal() == IDOK) 
    {
      //      nome.GetBufferSetLength(Nome.GetLength()+1);
      nome = Nome;
      return Nome;
    }
  }
  else  monolog.mensagem(5,nome);

  return Arq;
}

INT_PTR arqnovo::DoModal() 
{
  return CDialog::DoModal();
}

BOOL arqnovo::OnInitDialog() 
{
  CDialog::OnInitDialog();

  if (Tipo == ARQABRIR)
  {
    SetDlgItemText(IDC_SUBTITULO," Selecione o arquivo que deseja abrir:");
    SetWindowText(" Vias Urbanas & Estradas - Abrir arquivo:");
  }

  if (!PreencheLista()) 
  {
    EndDialog(FALSE);
    return FALSE;
  }
  else return TRUE;

  // return TRUE unlessyou set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void arqnovo::OnOK() 
{
  int Sel(((CListBox*)GetDlgItem(IDC_ARQUIVOS))->GetCurSel());
  CString Escolha;

  if (Sel != LB_ERR) 
  {
    ((CListBox *) GetDlgItem(IDC_ARQUIVOS))->GetText(Sel,Escolha);

    //--- Verifica a extensão selecionada.

    int i(0);
	
    for ( ; i < NARQ && Escolha.Compare(arquivos[i].ext[1]) ; i++);
	
    if (i == 17 || i == 18)    
    {
      std::string NomeTemp(Nome);
      for (int C = 0; C < 2; C++)
      {
        NomeTemp = NomeTemp.substr(0,NomeTemp.find_last_of('\\'));
      }
      std::string NomeProjeto(NomeTemp.substr(NomeTemp.find_last_of('\\')));
      Nome = NomeTemp.c_str();
      Nome += NomeProjeto.c_str();
    }

    Nome += ".";
    Nome += arquivos[i].ext[0];  //--- nome = nome do projeto + extensão do arquivo selecionado.
  }
  else Nome.Empty();  //--- Se não selecionou e pressionou OK, limpa o nome selecionado.

  EndDialog(TRUE);
}

void arqnovo::OnDblclkArquivos() 
{
  OnOK();	
}
