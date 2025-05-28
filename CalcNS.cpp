// CalcNS.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "..\resource.h"  //--- Precisa do caminho, se refere ao resource do EstWin não do CalcNS 
#include "afxtempl.h"
#include <winnetwk.h>
#include <set>
#include <map>
#include "supercstring.h"
#include "math.h"
#include "estaca.h"
#include "cponto.h"
#include "clpontos.h"
#include "carqgradientes.h"
#include "cgradientes.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include "carqcurverticais.h"
#include "carqIgualdades.h" 
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "ccurverticais.h"
#include "carqcurhorizontais.h"
#include "perfil.h"
#include "carqcanteiros.h"
#include "CCanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "ccurhorizontal.h"
#include "..\mat.h"  
#include "ccurhorizontais.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "cestacasacalcular.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include "csecao.h"
#include "clistasecoes.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
#include <afxmt.h>              //--- Para CEvent
#include "carqpistasfixas.h"
#include "cpistasfixas.h"
#include "calculanota.h"
#include "calcnota.h"
#include "CalcNS.h"
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

// CCalcNSApp

BEGIN_MESSAGE_MAP(CCalcNSApp, CWinApp)
END_MESSAGE_MAP()

class Matem Mat;
class monologo monolog;
extern char* PegaProjetoAtual();
extern class CProjeto* AchaProjetoNoVetor();

// CCalcNSApp construction

CCalcNSApp::CCalcNSApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CCalcNSApp object

CCalcNSApp theApp;

void CCalcNSApp::CalcularNS(const CString& CSProj,CProjeto* pProj)
{
  CalcNota* pCalculaNota;

  pCalculaNota = (CalcNota*) new CalcNota(CSProj,NULL,pProj);

  pCalculaNota->DoModal();

  delete pCalculaNota; 

}; 

// CCalcNSApp initialization

BOOL CCalcNSApp::InitInstance()
{
	CWinApp::InitInstance();

  return TRUE;
}
