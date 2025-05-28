// CProjeto.h : interface da classe Estaca
//
/////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
//--- Classe CProjeto
//--- Esta classe representa um Projeto. a princípio um projeto possui nome e igualdades,. 
//--- É preciso atribuir as igualdades inicialçmente ao projeto porque todas as
//--- estacas dependem delas e cada projeto possui suas igualdades.
//------------------------------------------------------------------------- 

class CProjeto
{
  //--- Construtores

public:

  CProjeto(CString& Nome= CString(""));

  //--- Propriedades

  CIgualdades Igualdades;
  CString Nome_Projeto;

  BOOL operator<(const class CProjeto& P) const { return Nome_Projeto < P.Nome_Projeto; } 
  BOOL operator==(const class CProjeto& P) const { return Nome_Projeto == P.Nome_Projeto; } 

};

