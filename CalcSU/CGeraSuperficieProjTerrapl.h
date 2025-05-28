// GeraSuperficie.h: interface for the CGeraSuperficie class.
//
//////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifndef VU_E_CPOINTD
#define VU_E_CPOINTD

typedef std::list<Ponto> tylstcponto;
typedef tylstcponto::iterator ittylstcponto;
typedef std::set<Ponto> SSetPontos;                 
typedef SSetPontos::iterator ItSSetPontos;

#endif

class CGeraSuperficieProjTerrpl
{
  std::string strNomeProj;
  CProgressCtrl* pProgresso;
  CStatic* pCSTexto;
  int ConfigSuper;

public:

  bool Gerar(tylstcponto& LstPontosRemovidos,SSetPontos& SetPontosProblema,double AreaTerrap,tylstcponto& LstFronteira,const std::string& FormatoArqPontos,int GeoRef,int TipoProj = -1);
  CGeraSuperficieProjTerrpl(CString& NomeProjeto, CProgressCtrl* pProgress, CStatic* pCSText, int pConfigSuper);
  ~CGeraSuperficieProjTerrpl();
};
