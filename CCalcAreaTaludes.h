#pragma once


// CCalcAreaTaludes

class CCalcAreaTaludes 
{
  std::string NomeProjeto; 

public:
	CCalcAreaTaludes();
	~CCalcAreaTaludes();

  bool CalculaAreas(CProgressCtrl& CProgBar,CWnd* Edit);

protected:
};
