#if !defined(AFX_LISTAMEDICOES_H__2E384721_30AB_11D7_BE60_9EE8363BD708__INCLUDED_)
#define AFX_LISTAMEDICOES_H__2E384721_30AB_11D7_BE60_9EE8363BD708__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListamEDICOES.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListaProjetoos window

class CListaMedicoes : public  CList<CListaSecoes*,CListaSecoes*> 
{
  CString NomeProjeto;
  int QuantMedicoes;

  // Construction
public:
  CListaMedicoes(CString& NomeProj,int EMedicao);

  CLPerfis TerrPrimMedicao;

  // Attributes
public:

  // Operations
public:

  // Implementation
public:
  virtual ~CListaMedicoes();


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTAMEDICOES_H__2E384721_30AB_11D7_BE60_9EE8363BD708__INCLUDED_)
