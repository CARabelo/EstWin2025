// GeraSuperficie.h: interface for the CGeraSuperficie class.
//
//////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGeraAlphaShape
{
  std::string strNomeProj;
  CProgressCtrl* pProgresso;
  CStatic* pCSTexto;
  unsigned long DMP;
  std::string FormatoArqPontos;

public:

  bool Gerar(int Tipo);

  CGeraAlphaShape(CString& pNomeProj,CProgressCtrl* pProgress,CStatic* pCSText,unsigned long pDMP,const CString& FormatoArqPontos,int Tipo); 
  ~CGeraAlphaShape();
};


