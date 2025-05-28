//-----------------------------------------------------------------
//--- classe lista dos pontos de terreno

class CTerreno : public CArqTerreno
{
  POSITION PosProximaSecao(POSITION &Sec);

protected:

public:

  CTerreno(CString& NomeArq);
  ~CTerreno();
};


