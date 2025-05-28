#include "stdafx.h"
#include "afxtempl.h"
#include "supercstring.h"
#include "math.h"
#include "estaca.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjeto
//
// Implementação da classe CProjeto
//////////////////////////////////////////////////////////////////////////////

CProjeto::CProjeto(CString& Nome) : Nome_Projeto(Nome)
{
  Igualdades.Criar(Nome);
};

