#pragma once

// CCalcVL

class CCalcVL : public CWnd
{
	DECLARE_DYNAMIC(CCalcVL)

public:
	CCalcVL();
	virtual ~CCalcVL();
  static __declspec(dllexport) int CalculaVL(CString& NomeProj,CProjeto* pProj);

protected:
	DECLARE_MESSAGE_MAP()
};
