#pragma once

// CSimpPL

class CSimpPL : public CWnd
{
	DECLARE_DYNAMIC(CSimpPL)

public:
	CSimpPL();
	virtual ~CSimpPL();
    static __declspec(dllexport) int Simplifica(tylstlstUtm_Coord_3d& pTerreno,int Tipo=0);
	static __declspec(dllexport) int SimplificaXYZ(lstlstPontos& pCNs);
	static __declspec(dllexport) int SimplificaXY(lstPontos& LstPontos);

protected:
	DECLARE_MESSAGE_MAP()
};
