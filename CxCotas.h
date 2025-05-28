//---  Classe que armazena a posição das contas no DXF

class CxCotas
{
  double Estaca,xCota;   //--- Estaca e x onde a cota será escrita

  public:
  
    CxCotas(const double& pEstaca,double pxCota) : Estaca(pEstaca) , xCota(pxCota) {};
    double PegaxCota() const { return xCota;};
    double PegaEstaca() const { return Estaca;};
    void AlteraxCota(double NovoxCota) { xCota = NovoxCota;};
    double operator-=(double Valor) { xCota -= Valor; return xCota;};
    bool operator<(const CxCotas& RHS) const { return Estaca < RHS.Estaca;};
    const CxCotas& operator=(const CxCotas& RHS) 
    {
      Estaca = RHS.Estaca;
      xCota = RHS.xCota;

      return *this;
    }
};

typedef std::set<CxCotas> tysetxCotas;
typedef tysetxCotas::iterator ittysetxCotas;
typedef tysetxCotas::reverse_iterator rittysetxCotas;
