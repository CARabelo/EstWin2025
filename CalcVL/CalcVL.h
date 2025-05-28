// CalcVL.h : main header file for the CalcVL DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "..\resource.h"    //--- Precisa do caminho, se refere ao Resource do EstWin, não do CalcVL

// CCalcVLApp
// See CalcVL.cpp for the implementation of this class
//

class CCalcVLApp : public CWinApp
{
public:
	CCalcVLApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
