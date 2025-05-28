#include "stdafx.h"
#include "afxtempl.h"
#include "carqterreno.h"
#include "math.h"
#include "Estaca.h"
#include "cponto.h"
#include "clpontos.h" 
#include "perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include <sstream>
#include <iomanip>
#include "estdoc.h"
#include "resource.h"
#include <string>
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "monologo.h"
#include "io.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define SECOES_POR_TRIANGULACAO 17
#define CURVAS_VERTICAIS 2
#define CAMADAS_AUTOCAD 23

static char* Numeros = {"01234567890.-+"};
extern class monologo monolog;

/////////////////////////////////////////////////////////////////////////////
// CMyStruct

void CMyStruct::FormatMyStruct(tlstwstr pListaCampos,CString& str, int Template) 
{
  std::stringstream LinhaFormatada;

  switch (Template)
  {
    case IDD_TIPOSSOLOS:
    {
      int C(0);

      for (ittlstwstr It = pListaCampos.begin(); It != pListaCampos.end() && C++ < 2 ; It++)
      {
        std::stringstream Temp;

        Temp << std::right << std::fixed << std::setfill(' ') << std::setprecision(2);

        std::operator<<(LinhaFormatada, _T('\t'));
        std::operator<<(LinhaFormatada,*It);
        std::operator<<(LinhaFormatada,Temp.str());
      }
    }
    break;
    case IDD_CADASTROACITOPOG:
    {
      if(pListaCampos.size() > 1)
      {
        int C(0);
        ittlstwstr It = pListaCampos.begin();

        for (It++; It != pListaCampos.end() && C++ < 3 ; It++)
        {
          std::operator<<(LinhaFormatada, _T('\t'));

          if(C == 1)
          {
            std::string CampoTemp(It->substr(0,8));
            std::operator<<(LinhaFormatada,CampoTemp);
          }
          else  std::operator<<(LinhaFormatada,*It);
        }
      }
    }
    break;
    case IDD_RELACADACITOPOG:
    {
      if (pListaCampos.size() > 1)
      {
        int C(0);
        ittlstwstr It(pListaCampos.begin());;

        It++;
        It++;
		    It++;

        std::operator<<(LinhaFormatada, *It);
      }
    }
    break;
    default:
    {
      for (ittlstwstr It = pListaCampos.begin(); It != pListaCampos.end(); It++)
      {
        std::operator<<(LinhaFormatada, _T('\t'));
        std::operator<<(LinhaFormatada,*It);

      }
    }
  }

  LinhaFormatada << std::ends;

  str = LinhaFormatada.str().c_str();
}

/////////////////////////////////////////////////////////////////////////////
// CColleDoc

IMPLEMENT_DYNCREATE(CEstDoc, CDocument)

  BEGIN_MESSAGE_MAP(CEstDoc, CDocument)
    //{{AFX_MSG_MAP(CEstDoc)
    //}}AFX_MSG_MAP
    //  ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
  END_MESSAGE_MAP()

  /////////////////////////////////////////////////////////////////////////////
  // CColleDoc construction/destruction

  CEstDoc::CEstDoc() {}

  CEstDoc::~CEstDoc()
  {
	  if (NomeCompleto.size() > 0 && strstr(NomeCompleto.c_str(), ".des") == 0)
	  {
		  CFile File(NomeCompleto.c_str(), CFile::modeRead);

		  if (File.GetLength() == 0)
		  {
			  File.Close();
			  CFile::Remove(NomeCompleto.c_str());
		  }
	  }
  }

  BOOL CEstDoc::OnNewDocument()
  {
    if (!CDocument::OnNewDocument())return FALSE;
    return TRUE;
  }

  void CEstDoc::DeleteContents()
  {
    POSITION pos = m_mystructList.GetHeadPosition();
    while (pos != NULL)	delete m_mystructList.GetNext(pos);

    m_mystructList.RemoveAll();
  }

  void CEstDoc::SetTitle(LPCTSTR lpszTitle)
  {
    int     i;
    CString nome(lpszTitle),ext,rasc;
    LPCTSTR tipos[28][2] =
    {
      {"IGU","IGUALDADES"},
      {"VER","CURVAS VERTICAIS"},
      {"HOR","CURVAS HORIZONTAIS"},
      {"LIM","LIMPEZA DO TERRENO"},
      {"PAV","EXPESSURAS DO PAVIMENTO"},
      {"TAC","TALUDES DE CORTE"},
      {"TAA","TALUDES DE ATERRO"},
      {"TPA","TALUDES DE PAVIMENTAÇÃO"},
      {"SEC","SEÇÕES-TIPO  DE PAVIMENTAÇÃO"},
      {"OAE","OBRAS DE ARTE ESPECIAIS"},
      {"OAC","OBRAS DE ARTE CORRENTES"},
      {"CAN","CANTEIROS CENTRAIS"},
      {"TER","TERRENO POR PONTOS"},
      {"FIX","PISTAS FIXAS"},
      {"ALS","ALARGAMENTOS, SARGETAS E PASSEIOS"},
      {"NIV","NIVELAMENTO DAS SEÇÕES"},
      {"GRA","GRADIENTES"},
      {"SET","SEÇÕES POR TRIANGULAÇÃO"},
      {"SAV","SECÕES AVULSAS"},
      {"EMP","EMPOLAMENTOS"},
      {"TPS","TIPOS DE SOLOS"},
      {"FSO","FUROS DE SONDAGEM"},
      {"ATO","ACIDENTES TOPOGRÁFICOS"}, 
      {"RAV","CAMADAS AUTOCAD"},
      {"   ","ARQUIVO DESCONHECIDO"}
    };

    ext = nome.Right(3);
    nome = nome.Left(nome.GetLength() - 4) + " - ";
    nome.MakeUpper();

    for (i=0 ; ext.CompareNoCase(tipos[i][0]) != 0 && i < 22 ; i++);

    if (i < 22)    //--- nome de tarefas de digitação
    {
      nome += tipos[i][1];
      CDocument::SetTitle(nome);
    }
    else   //--- Desenhos.
    {
      CMainFrame *MainFrame((CMainFrame*)AfxGetMainWnd());

      switch (MainFrame->asdfg)
      {
        case IDD_DESSECOES: nome = " Desenho das Seções "; break;
        case IDD_DESENHARPERFIL: nome = " Desenho do Greide "; break;
        case IDD_DESPONTOSGEO: nome = " Desenho da Superfície ";break;
        case IDD_REL_NOTA_SERVICO_TERRAPLANAGEM: nome = " Relatório da Nota de Serviço de Terraplanagem";break;
        case IDD_REL_PROJ_GEOMETRICO: nome = " Relatório do Projeto Geométrico ";break;
        case IDD_REL_PROJ_GEOMETRICO_DETALHADO: nome = " Relatório do Projeto Geométrico Detalhado ";break;
        case IDD_REL_PROJ_GREIDE: nome = " Relatório Projeto do Greide ";break;
        case IDD_REL_NOTA_SERVICO_PAVIM: nome = " Relatório da Nota de Serviço de Pavimentação ";break;
        case IDD_REL_TERRENO_PRIMITIVO: nome = " Relatório do Terreno Primitivo ";break;
        case IDD_REL_PROJ_TERRAPLANAGEM: nome = " Relatório do Projeto de Terraplanagem ";break;
        case IDD_DESPERFILGEOLOGICO: nome = " Desenho do Perfil Geológico "; break;
	    case IDD_DESPERFISAVULSOS: nome = " Desenho dos Perfís Avulsos"; break;
	    case IDD_TAREFACROQUISGEO: nome = "Croquis"; break;
        case IDD_DIAINICIAL: nome = "Vias Urbanas & Estradas - Página Inicial ";break;
        
        default : nome = "ERRO";
      }

      if(MainFrame->asdfg != IDD_DIAINICIAL) nome += " - " + CString(MainFrame->PegaRuaAtual());

      CDocument::SetTitle(nome);
    }
  }

  int CEstDoc::PegaNumArquivo(CArchive &ar)
  {
    CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();

    CString Extensao = ar.m_strFileName.Right(3),
      EXT = "IGUNIVVERHORLIMPAVTACTAATPASECOACOAECANTERFIXALSGRASETSAVEMPTPSFSOATORAV";
    Extensao.MakeUpper();
    if (EXT.Find(Extensao) == -1) return - 1 ; //-- Mensagem de erro, tem que ter;
    return EXT.Find(Extensao) / 3;
  }
  /////////////////////////////////////////////////////////////////////////////
  // CColleDoc serialization

  void CEstDoc::Serialize(CArchive& ar)
  {
    //--- Pela extensão pega o Numero do arquivo e pelo numero do arquivo 
    //--- o número de campos dele.
    if (((CMainFrame*)AfxGetMainWnd())->asdfg==601) return;

    int TabCorr[23] = {2,5,5,11,4,4,9,9,6,8,3,5,8,1,8,9,3,12,11,4,4,10,5},  //-- Tabela de correspondencia do numero do arquivo com a quantidade de campos
        NumArquivo = PegaNumArquivo(ar);

    ASSERT(NumArquivo != -1); //-- tem que ser <>;
    POSITION Atual;

    //--- Nas seções por triangulação de pontos o arquivo não é texto é de pontos calculado, então requer um tratamento diferente.

    if (ar.IsStoring())
    {
      ar << (unsigned long)m_mystructList.GetCount();

      int Icone(0);

      Atual = m_mystructList.GetHeadPosition();

      while (Atual)
      {
        try
        {
          CMyStruct* pMyStruct(m_mystructList.GetNext(Atual));

          ar << pMyStruct->lstRegAtual.size();

          for (ittlstwstr It = pMyStruct->lstRegAtual.begin(); It != pMyStruct->lstRegAtual.end(); It++)
          {
            ar << *It;
          }
        }
        catch (CArchiveException* e)
        {
          monolog.mensagem(16, ar.m_pDocument != NULL ? ar.m_pDocument->GetTitle() : CString(""), -1, e->m_cause);

          e->Delete();

          break;
        }
      }
    }
    else
    {
      unsigned long QuanReg(0);

      try
      {
        for (ar >> QuanReg ; QuanReg > 0 ; QuanReg--)
        {
          CMyStruct* pMyStruct(new CMyStruct);

          size_t QuanCampos(0);

          std::string CampoAtual;

          for(ar >> QuanCampos ; QuanCampos > 0 ; QuanCampos--) 
          {
            ar >> CampoAtual;
            pMyStruct->lstRegAtual.push_back(CampoAtual);
          }

          m_mystructList.AddTail(pMyStruct);
        }
      }
      catch (CArchiveException* e)
      {
        monolog.mensagem(15, ar.m_pDocument != NULL ? ar.m_pDocument->GetTitle() : CString(""), -1, e->m_cause);

        e->Delete();
      }
    }
  }

  void CEstDoc::GravaSecaoTriangulada(CArchive& ar, const void* EstacaAtual, const void* ListaAtualPontos)
  {
    if (((CLPontos*)ListaAtualPontos)->GetCount() == 0) return;

    ar << (*((CEstaca*)EstacaAtual)) << ((CLPontos*)ListaAtualPontos)->GetCount();

    POSITION Atual(((CLPontos*)ListaAtualPontos)->GetHeadPosition());

    while (Atual) ((CLPontos*)ListaAtualPontos)->GetNext(Atual) >> ar;
  }

  /////////////////////////////////////////////////////////////////////////////
// CColleDoc diagnostics

#ifdef _DEBUG
void CEstDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CEstDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CColleDoc commands

BOOL CEstDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
  std::string NomeArq(lpszPathName), Ext(NomeArq.substr(NomeArq.size() - 3, 3));

  std::transform(Ext.begin(), Ext.end(), Ext.begin(), ::toupper);

  if (Ext.compare("RAV") == 0)
  {
    Ext = "ATO";
    NomeArq = NomeArq.substr(0, NomeArq.find_last_of('.')+1);
    NomeArq += Ext;
  }
  //--- Os arquivos de acidentes topográficos estão no nível de projeto ( 1 diretório acima)

  if (Ext.compare("RAV") == 0 || Ext.compare("ATO") == 0)
  {
  /*
    size_t PosFimProj(NomeArq.rfind('\\', NomeArq.rfind('\\') - 1)),
      PosIniProj(NomeArq.rfind('\\', PosFimProj - 1));

    std::string NomeProj(NomeArq.substr(PosIniProj + 1, PosFimProj - PosIniProj - 1));

    NomeArq = NomeArq.substr(0, PosIniProj + 1);
    NomeArq += NomeProj + '\\' + NomeProj + '.' + Ext;
    */
        
    CFileException e;
    CFile ArqAT;

    if (ArqAT.Open(NomeArq.c_str(), CFile::modeRead | CFile::shareDenyNone, &e) == 0)   //--- Pontos Terraplanagem Projeto
    {
      monolog.mensagem(1, NomeArq.c_str());
      return false;
    }

    CArchive CArqAcitopog(&ArqAT, CArchive::load);

    tlstwstr LstCampos;
    unsigned int QuanReg(0);

    LstRegAciTopog.clear();

    for (CArqAcitopog >> QuanReg; QuanReg; QuanReg--)
    {
      size_t QuanCampos(0);
      std::string Campo;
      tlstwstr LstCampos;

      for (CArqAcitopog >> QuanCampos; QuanCampos; QuanCampos--)
      {
        CArqAcitopog >> Campo;
        LstCampos.emplace_back(Campo);
      }
      if (0) LstCampos.emplace_back("");    //--- Acrescentar 1 campo
      LstRegAciTopog.emplace_back(LstCampos);
    }

    if (0)
    {
      int Relacao2[] = {
      -1,1,2,3,12,17,18,19,5,4,6,21,13,7,8,9,10,11,1,14,15,16,20,22,24,25,23,27,26,39,38,37,-1,41,42,40,43,44,45,46
      };

      int C = 2;

      for (auto it = LstRegAciTopog.begin(); it != LstRegAciTopog.end(); it++, C++)
      {
        std::stringstream strNum;

        strNum << Relacao2[C];

        *it->rbegin() = strNum.str();
      }

      CFileException e;
      CFile ArqAT2;

      if (ArqAT2.Open(NomeArq.c_str(), CFile::modeCreate | CFile::modeWrite, &e) == 0)   //--- Pontos Terraplanagem Projeto
      {
        monolog.mensagem(1, NomeArq.c_str());
        return false;
      }

      CArchive CArqAcitopog(&ArqAT2, CArchive::store);

      CArqAcitopog << (unsigned int)LstRegAciTopog.size();

      for (auto it = LstRegAciTopog.begin(); it != LstRegAciTopog.end(); it++)
      {
        CArqAcitopog << it->size();

        for (auto itc = it->begin(); itc != it->end(); itc++)
        {
          CArqAcitopog << *itc;
        }
      }
    }
  }

  /*--- Se for desenho ou pagina inicial não tem q abrir arquivo ---*/

  NomeCompleto.clear();
  
  //--- Desenhos e dialogo inicial não tem docs

  if (strstr(lpszPathName, ".des") == 0 && strstr(lpszPathName, ".pin") == 0)
  {
    CDocument::OnOpenDocument(lpszPathName);
    NomeCompleto = lpszPathName;
  }

  return TRUE;
}

  unsigned int CEstDoc::PegaQuantidadeSecoes() 
  {
    register POSITION Atual = m_mystructList.GetHeadPosition();
    CEstaca EstacaAtual;
    unsigned int Contador(0);

    while(Atual)
    {
      CMyStruct* pMyStruct = m_mystructList.GetNext(Atual);

      pMyStruct->Campos[0].SpanExcluding(" ");

      if(!pMyStruct->Campos[0].IsEmpty())
      {
        CEstaca EstacaLida(pMyStruct->Campos[0]);

        if (EstacaAtual != EstacaLida)
        {
          EstacaAtual = EstacaLida; 
          Contador++;
        }
      }
    }

    return Contador;
  }

  BOOL CEstDoc::SaveModified()
  {
    // TODO: Add your specialized code here and/or call the base class

    POSITION pos = GetFirstViewPosition();
    CView* pFirstView = GetNextView( pos );

    if(GetTitle().Find(" Desenho da Superfície ") == 0)
    {
      POSITION pos = GetFirstViewPosition();
      CView* pFirstView = GetNextView( pos );

      UpdateAllViews(NULL,100,pFirstView);

      return true;
    }
    else  
    {
      //--- Se o arquivo não existe ainda cria um arquivo so para não chamar o dialogo padrão, porque não é necessário

      struct _finddata_t c_file;

      if(NomeCompleto.size() > 0)
      {
        if(_findfirst((char *) LPCTSTR (NomeCompleto.c_str()), &c_file ) == -1L) 
        {
          if(1 || m_mystructList.GetCount() > 0)  
          {
            CFile File(NomeCompleto.c_str(),CFile::modeCreate);
            File.Close();
          //  m_bAutoDelete = false;
            return CDocument::SaveModified();
          }
        }
        else  return CDocument::SaveModified();
      }
      else return CDocument::SaveModified();
    }

    return TRUE;
  }

POSITION CEstDoc::PegaPosOrdenada(std::string EstacaInserir,unsigned long PosArqRav) 
{
  POSITION pos(m_mystructList.GetHeadPosition()); 

  if(NomeCompleto.find("TIPO") != std::string::npos)
  {
    CEstaca EstInserir(EstacaInserir.c_str());
    bool Achou(false);

    while (pos && !Achou) 
    {
      CEstaca EstacaAtual(m_mystructList.GetAt(pos)->lstRegAtual.begin()->c_str()); 

      if(EstacaAtual > EstInserir) Achou = true;
      else m_mystructList.GetNext(pos);
    }
  }
  else
  {
    bool Achou(false);
    CEstaca EstInserir(EstacaInserir.c_str());

    if(EstInserir.EstReal != INFINITO)
    {
      while (pos && !Achou) 
      {
        CEstaca EstAtual(m_mystructList.GetAt(pos)->lstRegAtual.begin()->c_str());

        if(EstAtual > EstInserir) Achou = true;
        else m_mystructList.GetNext(pos);
       }
    }
    else   //--- Não é estaca, ordena pelo texto
    {
      if (NomeCompleto.find("RAV") != std::string::npos)   //--- Se for arquiv .RAV tem  pb do sort do listbox, tem q buscar pela posicção do listbox da camada autocad
      {
        while (pos && PosArqRav)
        {
          m_mystructList.GetNext(pos);
          PosArqRav--;
        }
      }
      else
      {
      while (pos && !Achou)
      {
        if(*m_mystructList.GetAt(pos)->lstRegAtual.begin() > EstacaInserir) Achou = true;
        else m_mystructList.GetNext(pos);
        }
      }
    }
  }

  return pos;
}
   
