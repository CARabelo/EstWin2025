//------------------------------------------------------------

class Perfil : public CLPontos
{
public:

  CEstaca Estaca;

  //--- Constru��o

  Perfil(CEstaca Est);
  Perfil(double Estaca = INFINITO,double Cota = INFINITO,int Ig = 0) : Estaca(Estaca,Cota,Ig) {};  //--- Construtor default
  Perfil(const Perfil &Alarg);  //--- Construtor de c�pia;
  void Constroi(double Estaca = INFINITO,double CotaEixo = INFINITO); //--- Construtor atrasado

  //--- Opera��es

  Perfil& operator=(const Perfil &No);
  CLPontos& operator=(const CLPontos& L);
  void Serialize(CArchive& ar);
};
