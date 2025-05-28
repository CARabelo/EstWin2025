// GeraSuperficie.h: interface for the CGeraSuperficie class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GERASUPERFICIE_H__17674AC1_C816_11D7_BE61_8A216B9EE51B__INCLUDED_)
#define AFX_GERASUPERFICIE_H__17674AC1_C816_11D7_BE61_8A216B9EE51B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGeraSuperficie
{
  std::string strNomeProj;
  CProgressCtrl* pProgresso;
  CStatic* pCSTexto;
  int ConfigSuper;
  std::string FormatoArqPontos;

public:

  bool Gerar(int TipoSuperf,CProgressCtrl* pProgress);
  CGeraSuperficie(CString& NomeProjeto,CProgressCtrl* pProgress,CStatic* pCSText,int pConfigSuper,const CString& FormArqPontos);
  ~CGeraSuperficie();
};

#endif // !defined(AFX_GERASUPERFICIE_H__17674AC1_C816_11D7_BE61_8A216B9EE51B__INCLUDED_)
