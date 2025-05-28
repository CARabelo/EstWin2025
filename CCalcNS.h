#pragma once
// CCalcNS

class CCalcNS 
{
public:
	CCalcNS();
	virtual ~CCalcNS();

  static __declspec(dllexport) int CalculaNS(const CString& NomeProj,CProjeto* pProj);

protected:
};


