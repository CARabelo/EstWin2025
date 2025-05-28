// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
#pragma warning(disable : 4786) //--- Nomes muito longos nos templates 
#pragma warning(disable : 4244)  //conversion from 'double' to 'long', possible loss of data

#define WINVER 0x0A00
#define _WIN32_WINNT 0x0A00

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

//--- Inclusões genéricas

#include <sstream> 
#include <iomanip>

typedef std::list<std::string> tlstwstr;

static std::ifstream& operator>>(std::ifstream& ifs,std::string& str)  
{
  register size_t Size(-1);

  str.clear();

  ifs >> Size;

  str.reserve(Size);

  while(Size > 0)
  {
    register char Carac;

    ifs >> Carac;
    str.push_back(Carac);
    --Size;
  }

  return ifs;
}

static std::ofstream& operator<<(std::ofstream& ofs,std::string& str)  
{
  ofs << str.size();

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


static CArchive& operator>>(CArchive& ar, std::string& str)
{
  register size_t Size(-1);

  str.clear();

  ar >> Size;

  str.reserve(Size);

  while (Size > 0)
  {
    register char Carac;

    ar >> Carac;
    str.push_back(Carac);
    --Size;
  }

  return ar;
}

static CArchive& operator<<(CArchive& ar, std::string& str)
{
  ar << str.size();

  for (register std::string::iterator it = str.begin(); it != str.end(); ++it)
  {
    ar << *it;
  }

  return ar;
}

