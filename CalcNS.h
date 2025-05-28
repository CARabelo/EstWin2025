// CalcNS.h : main header file for the CalcNS DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "..\..\EstWin\resource.h"

// CCalcNSApp
// See CalcNS.cpp for the implementation of this class
//

class CCalcNSApp : public CWinApp
{
public:
	CCalcNSApp();

// Overrides
public:
	virtual BOOL InitInstance();

  static __declspec(dllexport) void CalcularNS(const CString& CSProj,CProjeto* pProj);
  
	DECLARE_MESSAGE_MAP()
};
