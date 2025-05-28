
class CCotaArea
{
  public:
    double Cota, AreaTotal, Extensao, AreaCN,Inclinacao;

    CCotaArea(double pCota, double pArea, double pAreaCN, double pExtensao,double pInclinacao=INFINITO) : Cota(pCota), AreaTotal(pArea), Extensao(pExtensao), AreaCN(pAreaCN),Inclinacao(pInclinacao) {};

    bool operator<(const CCotaArea& RHS) const
    {
      return Cota < RHS.Cota ? true : false;
    }
};

typedef std::set<CCotaArea> tysetCotasArea;
typedef tysetCotasArea::iterator ittysetCotasArea;
typedef tysetCotasArea::reverse_iterator rittysetCotasArea;
