// HTTPRequests.h: arquivo de cabeçalho principal para a DLL do HTTPRequests
//

#pragma once

#ifndef __AFXWIN_H__
	#error "inclua 'stdafx.h' antes de incluir este arquivo para PCH"
#endif

#include "resource.h"		// símbolos principais

// CHTTPRequestsApp
// Consulte HTTPRequests.cpp para a implementação desta classe
//

class CHTTPRequestsApp : public CWinApp
{
public:
	CHTTPRequestsApp();

// Substitui
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
