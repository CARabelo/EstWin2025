typedef std::pair<std::string, std::string> typairstrstr;

class CLerKMLCN
{
  void GravaString(std::ofstream& Arq, std::string& str);

public:
  
   CLerKMLCN(const std::string& NomeArq,tylstGdc_Coord_3d& LstCoordGdc);
   CLerKMLCN(const std::string& NomeArq, tylstGdc_Coord_3d& LstCoordGdc,int Tipo);

  ~CLerKMLCN();

  typairstrstr ExtraiToken(tySetString& Token,std::string& Linha);
};

