// CSStack.cpp : implementation file
//
#pragma warning disable:4786

#include "stdafx.h"
#include "est.h"
#include <set>
#include "cponto.h"
#include "classsetpontos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSetPontos

CSetPontos::CSetPontos() 
{
}

CSetPontos::~CSetPontos()
{
}

/////////////////////////////////////////////////////////////////////////////
// CSetPontos message handlers

class Ponto* CSetPontos::AchaPonto(Ponto& PPonto)
{
  //--- Busca por um ponto no set, retorna um ponteiro para o ponto se achar ou NULL
  //--- caso contrário.

  std::set<Ponto>::iterator Atual = Pontos.find(PPonto);

  return (Atual == Pontos.end() ? NULL : const_cast<Ponto*>(&*Atual));
}

class Ponto* CSetPontos::IncluiPonto(Ponto& PPonto)
{
  //--- Inclui um ponto no set.

  static std::set<Ponto>::iterator UltimoInserido = Pontos.begin(),
    Atual = Pontos.insert(UltimoInserido,PPonto);

  return (Atual == Pontos.end() ? NULL : const_cast<Ponto*>(&*Atual));
}
