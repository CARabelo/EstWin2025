// ConvUTMGDC.h: arquivo de cabeçalho principal para a DLL do ConvUTMGDC
//

#pragma once

#ifndef __AFXWIN_H__
	#error "inclua 'stdafx.h' antes de incluir este arquivo para PCH"
#endif

#include "resource.h"		// símbolos principais


// CConvUTMGDCApp
// Consulte ConvUTMGDC.cpp para a implementação desta classe
//

class CConvUTMGDCApp : public CWinApp
{
public:
	CConvUTMGDCApp();

// Substitui
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
