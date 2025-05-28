// CalcSU.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "..\resource.h"    //--- Precisa do caminho se refere au resource do setWin, não do CalcSU
#include "resource.h"
#include "afxtempl.h"
#include <winnetwk.h>
#include <set>
#include "supercstring.h"
#include "math.h"
#include "Estaca.h"
#include "carqIgualdades.h" 
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "CalcSU.h"
#include "CPonto.h"
#include "mat.h"    // Atenção -> não é Math.h
#include "monologo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CCalcSUApp

BEGIN_MESSAGE_MAP(CCalcSUApp, CWinApp)
END_MESSAGE_MAP()

class Matem Mat;
class monologo monolog;
//extern char* PegaProjetoAtual();
//extern class CProjeto* AchaProjetoNoVetor();

// CCalcSUApp construction

CCalcSUApp::CCalcSUApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CCalcSUApp object

CCalcSUApp theApp;

// CCalcSUApp initialization

BOOL CCalcSUApp::InitInstance()
{
	CWinApp::InitInstance();

  return TRUE;
}
