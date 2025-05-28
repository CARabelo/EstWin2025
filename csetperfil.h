//------------------------------------------------------------

class Perfil : public CSetPontos
{
        public:
        
        CEstaca Estaca;

  //--- Construção

  Perfil(CEstaca Est);
  Perfil(double Estaca = INFINITO,double cota = INFINITO,int Ig = 0);  //--- Construtor default
  Perfil(class Perfil &Alarg);  //--- Construtor de cópia;
        void Constroi(double Estaca = INFINITO,double CotaEixo = INFINITO); //--- Construtor atrasado

  //--- Operações

        Perfil& operator=(Perfil &No);
        CSetPontos& operator=(CSetPontos& L);
        void Serialize(CArchive& ar);
};

