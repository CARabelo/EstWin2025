// ConvUTMGDC.cpp: define as rotinas de inicialização para a DLL.
//

#include "stdafx.h"
#include <list>
#include "ConvUTMGDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: Se esta DLL é conectada dinamicamente contra as MFC DLLs,
//		qualquer função exportada desta DLL que chama a
//		MFC deve possuir o macro AFX_MANAGE_STATE adicionado no
//		começo desta função.
//
//		Por exemplo:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			//corpo normal da função aqui
//		}
//
//		É muito importante que essa macro apareça em todas
//		as funções, antes de qualquer chamada a MFC.  Isto significa que
//		it deve aparecer como a primeira instrução dentro de
//		função, antes mesmo da declaração de qualquer objeto variável
//		uma vez que seus construtores podem gerar chamadas ao MFC
//		DLL.
//
//		Consulte Notas Técnicas MFC 33 e 58 para informações
//		adicionais.
//

// CConvUTMGDCApp

BEGIN_MESSAGE_MAP(CConvUTMGDCApp, CWinApp)
END_MESSAGE_MAP()


// construção de CConvUTMGDCApp

CConvUTMGDCApp::CConvUTMGDCApp()
{
	// TODO: adicione código de construção aqui,
	// Coloque todas as inicializações significativas em InitInstance
}


// O único objeto de CConvUTMGDCApp

CConvUTMGDCApp theApp;


// inicialização de CConvUTMGDCApp

BOOL CConvUTMGDCApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
