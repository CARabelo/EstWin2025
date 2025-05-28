#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqcurhorizontais.h"
#include <math.h>
#include <string>
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include <sstream>
#include <iomanip>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFINITO   1.7976931348623158e+308
#define ANTERIOR   0
#define POSTERIOR  1
#define ESQUERDO   0
#define ESPIRAL    0

extern const double M_PI= 3.14159265358979323846;

extern class monologo monolog;

//----------------------------------------------------------------
//--- Implementação do nó da lista.

ArqCurHorizontais::ArqCurHorizontais(tlstwstr& pListaDados ) : ListaCampos(pListaDados)
{
}

ArqCurHorizontais::ArqCurHorizontais(CString& PEstTs,int PIgTs,CString& PEstSt,int PIgSt,int PLado,int PTipo,double PAc,double PRaio,double PLc,double PSe, double PSl) : Erro(0)
{
  std::stringstream strstrTemp;

 strstrTemp << std::right << std::fixed << std::setfill(' ') << std::setprecision(0);

 strstrTemp << PEstTs << ';' << PIgTs << ';' << PEstSt << ';' << PIgSt << ';' << (PLado == ESQUERDO ? 'E' : 'D') << ';' <<  (PTipo == ESPIRAL ? 'E' : 'C') << ';'; 

  double ACGraus(fabs(PAc * 180.0 / M_PI));
  int Graus((int) floor(fabs(ACGraus)+ 0.0000000001));
  double GrausFrac(ACGraus - Graus), Minutos(GrausFrac * 60.0),Segundos(fmod(Minutos,1.0) * 60.0);

 strstrTemp << std::setprecision(0);
 
 strstrTemp << ACGraus << "° " << Minutos << "' " ;
 
 strstrTemp << std::setprecision(2);

 strstrTemp << Segundos << '"' <<';';

 strstrTemp << PRaio << ';' << PLc << ';' << PSe * 100.0 << ';' << PSl << std::ends;

 char Buffer[256]={0};
 int C(0);

 while(strstrTemp.good() == true && C++ < 11)
 {
   strstrTemp.getline(&Buffer[0],255,';');

   ListaCampos.push_back(std::string(Buffer));
 }
}

CArchive& ArqCurHorizontais::operator<<(CArchive& ar)
{
  ar << ListaCampos.size();

  for(ittlstwstr It = ListaCampos.begin() ; It != ListaCampos.end() ; It++)
  {
    ar << *It;
  }

  return ar;
}

CArchive& ArqCurHorizontais::operator>>(CArchive& ar)
{
  size_t QuanCampos(0);
  std::string CampoAtual;

  for(ar >> QuanCampos ; QuanCampos > 0 ; QuanCampos--)
  {
    ar >> CampoAtual;
    ListaCampos.push_back(CampoAtual);
  }

  return ar;
}

int ArqCurHorizontais::Consiste()
{
  Erro = 0;
  bool CurvaEspiral(false);

  if(ListaCampos.size() == 11)
  {
    ittlstwstr It(ListaCampos.begin());

    if(!SuperCString::EEstaca(It->c_str())) Erro = 1; 
    It++;
    if(!SuperCString::ENumero(It->c_str())) Erro = 2; 
    It++;

    if(!SuperCString::EEstaca(It->c_str())) Erro = 3; 
    It++;
    if(!SuperCString::ENumero(It->c_str())) Erro = 4; 
    It++;
  
    ToUpper(*It);
    if(*It != "E" && *It != "D") Erro = 5;
    It++;

    ToUpper(*It);
    if(*It != "E" && *It != "C") Erro = 6;
    if(*It == "E") CurvaEspiral = true;

    It++;

    SuperCString AnguloTemp(It->c_str());

    AnguloTemp.Limpa(" °'\"");

    if(!SuperCString::ENumero(AnguloTemp)) Erro = 7; 
    It++;

    if(!SuperCString::ENumero(It->c_str())) Erro = 8; 
    It++;

    if(CurvaEspiral)
    {
      if(!SuperCString::ENumero(It->c_str())) Erro = 9; 
      It++;

      if(!SuperCString::ENumero(It->c_str())) Erro = 10; 
      It++;

      if(!SuperCString::ENumero(It->c_str())) Erro = 11; 
      It++;
    }
  }
  else Erro = 12;

  return Erro;
}

//ArqCurHorizontais::ArqCurHorizontais

const ArqCurHorizontais& ArqCurHorizontais::operator=(const ArqCurHorizontais& RHS)
{
  ListaCampos = RHS.ListaCampos;

  return *this;
}


//----------------------------------------------------------------
//--- Implementação da lista

CArqCurHorizontais::CArqCurHorizontais(CString& NomeArq) : Atual(NULL),NomeArquivo(NomeArq + ".hor")
{
  CFile ArqHorizontais;
  CFileException e;

  if (ArqHorizontais.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    if (((CMainFrame*)AfxGetMainWnd())->PegaSilencioso() == false) monolog.mensagem(23,NomeArquivo.GetBuffer(1));

    return;
  }
  else
  {	
    CArchive ar(&ArqHorizontais,CArchive::load);
    Serialize(ar);
    ArqHorizontais.Close();
    ar.Close();
    Atual = GetHeadPosition();
  }
}

void CArqCurHorizontais::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    ar << (unsigned long) GetCount();

    Atual = GetHeadPosition();

    while (Atual)
    {
      try
      {
        GetNext(Atual) << ar;
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(15,CString(" CurvasHorizontais : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
  else
  {
    unsigned long QuanReg(0);

    for(ar >> QuanReg ; QuanReg > 0 ; QuanReg--)
    {
      ArqCurHorizontais NoAtual;

      try
      {
        NoAtual >> ar;

        AddTail(NoAtual);
      }
      catch(CArchiveException* e)
      {
          monolog.mensagem(16,CString(" Gradientes : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();
      }
    }
  }
}

void CArqCurHorizontais::GravarArquivo()
{
  CFile ArqCurHorizontais;
  CFileException e;

  if (ArqCurHorizontais.Open(NomeArquivo,CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {
    CArchive ar(&ArqCurHorizontais,CArchive::store);
    Serialize(ar);
  }
}

