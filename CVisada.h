// Cvisada.h : interface da classe CVisada
//
/////////////////////////////////////////////////////////////////////////////

class CVisada : public CString 
{
public:

  //--- Construtor

  CVisada(CString Visada = "");

  BOOL EEstaca();
  BOOL EDistancia();
  BOOL EMais();
  BOOL ENumero();
};

