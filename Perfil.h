//------------------------------------------------------------

class Perfil : public CLPontos
{
public:

  CEstaca Estaca;

  //--- Construção

  Perfil(CEstaca Est);
  Perfil(double Estaca = INFINITO,double Cota = INFINITO,int Ig = 0) : Estaca(Estaca,Cota,Ig) {};  //--- Construtor default
  Perfil(const Perfil &Alarg);  //--- Construtor de cópia;
  void Constroi(double Estaca = INFINITO,double CotaEixo = INFINITO); //--- Construtor atrasado

  //--- Operações

  Perfil& operator=(const Perfil &No);
  CLPontos& operator=(const CLPontos& L);
  void Serialize(CArchive& ar);
};
