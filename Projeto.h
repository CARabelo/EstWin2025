// CProjeto.h : interface da classe Estaca
//
/////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
//--- Classe CProjeto
//--- Esta classe representa um Projeto. a princ�pio um projeto possui nome e igualdades,. 
//--- � preciso atribuir as igualdades inicial�mente ao projeto porque todas as
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

