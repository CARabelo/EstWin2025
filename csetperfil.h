//------------------------------------------------------------

class Perfil : public CSetPontos
{
        public:
        
        CEstaca Estaca;

  //--- Constru��o

  Perfil(CEstaca Est);
  Perfil(double Estaca = INFINITO,double cota = INFINITO,int Ig = 0);  //--- Construtor default
  Perfil(class Perfil &Alarg);  //--- Construtor de c�pia;
        void Constroi(double Estaca = INFINITO,double CotaEixo = INFINITO); //--- Construtor atrasado

  //--- Opera��es

        Perfil& operator=(Perfil &No);
        CSetPontos& operator=(CSetPontos& L);
        void Serialize(CArchive& ar);
};

