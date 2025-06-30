// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

#pragma warning(disable : 4786) //--- Nomes muito longos nos templates 
#pragma warning(disable : 4244)  //conversion from 'double' to 'long', possible loss of data

#define	WM_CLOSE_TOOLBAR (WM_APP+100)
#define	WM_AMPLIAR_MAPA (WM_APP+101)
#define WM_BUTUP_LISTBOX_PROJETOS (WM_APP+102)
#define WM_MOSTRAR_PONTOS_USUARIO (WM_APP+103)

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows 95 Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxmt.h>              //--- Para CEvent
//--- Para debugar vazamento de memoria
//#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <afxcontrolbars.h>
//#define _CRT_SECURE_NO_WARNINGS  //Sem warnins para funções deprecated
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <locale>
#include <algorithm> 
#include <afxcontrolbars.h>
#include <set>
#include <afxcontrolbars.h>

//--- Inclusões genéricas

#include <sstream> 
#include <iomanip>

#define INFINITO   1.7976931348623158e+308
#define EVENT_CHAVE 1001

typedef std::list<std::string> tlstwstr;
typedef tlstwstr::iterator ittlstwstr;
typedef tlstwstr::const_iterator cittlstwstr;
typedef tlstwstr::reverse_iterator rittlstwstr;
typedef std::list<ittlstwstr> tlstittlstwstr;
typedef tlstittlstwstr::iterator ittlstittlstwstr;

typedef std::list<tlstwstr> tlstlstwstr;
typedef tlstlstwstr::iterator ittlstlstwstr;

typedef std::set<std::string> tySetString;
typedef tySetString::iterator tyitSetString;

typedef std::set<std::string> tysetNomeArquivos;
typedef std::set<std::string>::iterator tyditsetNomeArquivos;

typedef tysetNomeArquivos tysetNomes;
typedef tyditsetNomeArquivos ittysetNomes;

static std::ifstream& operator>>(std::ifstream& ifs,std::string& str)  
{
  register size_t Size(-1);
  char Espaco;

  str.clear();

  ifs >> Size >> Espaco;

  str.reserve(Size);

  register char Carac;

  while(Size > 0)
  {
    ifs >> Carac;
    str.push_back(Carac);
    --Size;
  }

  return ifs;
}

static std::ofstream& operator<<(std::ofstream& ofs,std::string& str)  
{
  ofs << str.size() << ' ';

  for (register std::string::iterator it = str.begin() ; it != str.end() ; ++it) 
  {
    ofs << *it;
  }

  return ofs;
}

static std::ifstream& operator>>(std::ifstream& ifs,CString& CStr)  
{
  register int Size(-1);

  CStr.Empty();

  ifs >> Size;

  CStr.GetBufferSetLength(Size);

  while(Size > 0)
  {
    register char Carac;

    ifs >> Carac;
    CStr += Carac;
    --Size;
  }

  return ifs;
};

static std::ofstream& operator<<(std::ofstream& ofs, CString& CStr)  
{
  int Tam(CStr.GetLength());
  
  ofs << CStr.GetLength();

  for (register int i = 0 ; i < Tam  ; ++i) 
  {
    ofs << CStr[i];
  }

  return ofs;
};

static CArchive& operator>>(CArchive& ar,std::string& str)  
{
  register size_t Size(-1);

  str.clear();

  ar >> Size;

  str.reserve(Size);

  while(Size > 0)
  {
    register char Carac;

    ar >> Carac;
    str.push_back(Carac);
    --Size;
  }

  return ar;
}

static CArchive& operator<<(CArchive& ar,const std::string& str) 
{
  ar << str.size();

  for (register std::string::const_iterator it = str.begin() ; it != str.end() ; ++it) 
  {
    ar << *it;
  }

  return ar;
}

static bool FindAnyCharS1S2(const std::string& S1,const std::string& S2) 
{
  for(register std::string::const_iterator it=S1.begin() ; it != S1.end() ; ++it) 
  {
    if(S2.find(*it) != std::string::npos) return true;
  }

  return false;
}

static std::string& ToUpper(std::string& Dado,int Modo = 0)
{
  static std::string Resposta;
  std::locale Locale;

  if(Modo == 1)
  {
    Resposta = Dado;

    for (std::string::iterator It = Resposta.begin() ; It != Resposta.end() ; It++)
    *It = std::toupper(*It,Locale);
  }
  else
  {
     for (std::string::iterator It = Dado.begin() ; It != Dado.end() ; It++)
    *It = std::toupper(*It,Locale);

    Resposta = Dado;
  }

  return Resposta;
}

static std::string Versao(std::string& Texto)
{
  //return "5.40.00 30_09_2020"; 
  //std::string coment("Inclusão das bacias Hidrograficas (dados geométricos) e dos equipamentos de drenagem");

  //return "5.41.00 20_11_2020"; 
  //std::string coment("Inclusão da dll vcruntime140.dll (Windows 10)");
  
  //  return "5.42.00 17_02_2021"; 
  //std::string coment("Inclusão das OACs");

 // return "5.43.00 29_01_2022";
 // std::string coment("Inclusão da Declinação");
  
 // return "3.00.00 01_05_2024";
 // std::string coment("achar o caminho do talvegue principal");


  return "3.01.00 10_08_2024";
  Texto = "Inclusão cálculo TCs DENIT";
}

static bool EEspaco(char Carac)
{
  static std::string Conjunto(" ");
  for(register std::string::const_iterator it = Conjunto.begin() ; it != Conjunto.end() ; it++) 
  {
    if(std::find(Conjunto.begin(),Conjunto.end(),Carac) != Conjunto.end()) return true;
  }

  return false;
}

static bool Limpa(std::string& Dado, const std::string& Lixo = _T(" "))
{
  std::string Limpo;

  if (Dado.size() > 0)
  {
    for (register auto it = Dado.begin(); it != Dado.end(); it++)
    {
      if (Lixo.find(*it) == std::string::npos)  Limpo.push_back(*it);
    }
  }

  bool Retorno(Limpo.size() != Dado.size());

  Dado = Limpo;

  return Retorno;
}

/*


BOOL FileExiste(LPCTSTR path)
{
  DWORD dwAttrib(GetFileAttributes(path));

  return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_NORMAL));
}

*/

//#define DEMOVER
//#define PACOTE_BASICO
//#define PACOTE_INTERMEDIARIO 
#define PACOTE_AVANCADO 

