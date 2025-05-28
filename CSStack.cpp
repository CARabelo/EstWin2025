// CSStack.cpp : implementation file
//
#pragma warning(disable:4786)

#include "stdafx.h"
#include "est.h"
#include <vector>
#include <iostream>
#include "afxtempl.h"
#include "math.h"
#include "supercstring.h"
#include "estaca.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "csstack.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCSStack

CCSStack::CCSStack() 
{
  Projetos.reserve(10);  //--- Tamanho reservado ao vetor;
}

CCSStack::~CCSStack()
{
}

/////////////////////////////////////////////////////////////////////////////
// CCSStack message handlers

class CProjeto* CCSStack::AchaProjeto(CString& Projeto)
{
  //--- Procura por um projeto no vetor, retorna um ponteiro para o projeto se achar ou NULL
  //--- caso contrário.

  if (Projetos.empty()) return NULL;

  std::vector<CProjeto>::iterator it(Projetos.begin()); 
  for(; 
    it != Projetos.end() && it->Nome_Projeto.CompareNoCase(Projeto) != 0; 
    it++);

  return (it == Projetos.end() ? NULL : &*it);
}

void CCSStack::ExcluiProjeto(CString& Projeto)
{
  //--- Exclui um projeto do vetor.

  if (Projetos.empty()) return;

  std::vector<CProjeto>::iterator it(Projetos.begin());
  for( ; it != Projetos.end() && it->Nome_Projeto.CompareNoCase(Projeto) != 0; it++);

  if (it != Projetos.end()) Projetos.erase(it);
}

void CCSStack::IncluiProjeto(CString& Nome)
{
  //--- Inclui um projeto no vetor, inclui no final do vetor

  Projetos.push_back(CProjeto(Nome));
}


