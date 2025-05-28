typedef std::pair<std::string, std::string> typairstrstr;

class CLerKML
{
  void GravaString(std::ofstream& Arq, std::string& str);

  //tylstGdc_Coord_3d LstPontosGdc;

public:
  
  CLerKML(const std::string& NomeArq,tylstGdc_Coord_3d& LstCoordGdc);
  ~CLerKML();

  typairstrstr ExtraiToken(tySetString& Token,std::string& Linha);
};

