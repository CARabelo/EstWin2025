#pragma once

#ifndef VU_E_CPOINTD
#define VU_E_CPOINTD

typedef std::list<Ponto> tylstcponto;
typedef tylstcponto::iterator ittylstcponto;
typedef tylstcponto::const_iterator cittylstcponto;

#endif

// CalcSU

typedef std::set<Ponto> SSetPontos;                   //--- Set  de um conjunto de pontos
typedef SSetPontos::iterator ItSSetPontos;

class CCalcSU 
{
public:

  enum { TRIANG_DELAUNAY, TRIANG_DELAUNAY_FORCADA, TRIANG_DELAUNAY_GABRIEL };
  enum { TOPOG_CONVENCIONAL, TOPOG_DRONES, PONTOSARQDGN, PONTOSCNDGN};

	CCalcSU();
	virtual ~CCalcSU();

  static __declspec(dllexport) int CalculaSU(CString& NomeProj,CStatic* pStatic,CProgressCtrl* pProgresso,int ConfigSuper,const std::string& formArqPontos,int TipoSuperficie);
  static __declspec(dllexport) int CalculaAS(CString& NomeProj,CProgressCtrl* pProgresso,CStatic* pCSTexto,unsigned long DMP,const CString& FormatoArqPontos,int tipo);
  static __declspec(dllexport) int CalculaSUProjTerrpl(CString& NomeProj, CStatic* pStatic, CProgressCtrl* pProgresso, int ConfigSuper,tylstcponto& LstPontosRemovidos,SSetPontos& SetPontosProblema,double AreaTerrap,tylstcponto& LstFronteira,const std::string& FormatoArqPontos,int ProjGeoReferenciado,int TipoProjeto);
  static __declspec(dllexport) int IniciaCalculaCotas(const std::string& strNomeProj);
  static __declspec(dllexport) int PegaTriangulo(Ponto& P1, Ponto& P2, Ponto& P3, Ponto& PCalc);

protected:
};




