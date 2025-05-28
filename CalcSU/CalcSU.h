// CalcSU.h : main header file for the CalcSU DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

// CCalcSUApp
// See CalcSU.cpp for the implementation of this class
//

class CCalcSUApp : public CWinApp
{
public:
	CCalcSUApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
