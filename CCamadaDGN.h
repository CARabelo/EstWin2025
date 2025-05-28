typedef std::set<ItRLPontos> tysetRItLPontos;
typedef tysetRItLPontos::iterator ittysetRItLPontos;

class CCamadaDGN
{
  CAcidentesTopograficos AciTopograficos;   //--- CNs Talvegues e cia
  tysetRItLPontos SetFimCN;                 //--- Usado nas liga��es das CNs

  tylstlstUtm_Coord_3d LstLstCNUTM;

  public:

  CCamadaDGN();
  int LerKML();

  tylstlstUtm_Coord_3d& PegaLstLstCNUTM() { return LstLstCNUTM;};
};

