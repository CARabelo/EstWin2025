class CLembrete
{
  unsigned int uiOrigem;
  Ponto PonCoordenadas;
  std::string strTexto,strTitulo;
  COLORREF CorBandeira;

public:

   enum ORIGENS{SEM_ORIGEM,DES_GEOMETRICO,DES_GREIDE,DES_SECOES};

  CLembrete(unsigned int pOrigem=SEM_ORIGEM,const Ponto& pCoord=Ponto(0,0),const std::string& pTexto="",const std::string& pTitulo="",COLORREF pCor=(RGB(255,0,0))):uiOrigem(pOrigem),PonCoordenadas(pCoord),strTexto(pTexto),strTitulo(pTitulo),CorBandeira(pCor) {};
  CLembrete(const CLembrete& pLembrete):uiOrigem(pLembrete.Origem()),PonCoordenadas(pLembrete.PonCoordenadas),strTexto(pLembrete.strTexto),strTitulo(pLembrete.strTitulo),CorBandeira(pLembrete.CorBandeira){};

  ~CLembrete(){};

  unsigned int Origem() const { return uiOrigem;}; 
  const std::string& Texto() const { return strTexto;}; 
  const std::string& Titulo() const { return strTitulo;}; 
  const Ponto& Local() const { return PonCoordenadas;};
  COLORREF Cor() const { return CorBandeira;};
  void Deslocax(double Valor) {PonCoordenadas.x += Valor;};
  void Deslocay(double Valor) {PonCoordenadas.y += Valor;};
  void MudaLocalizacao(double Coorx,double Coory) {PonCoordenadas.x = Coorx;PonCoordenadas.y = Coory;};

  bool operator<(const CLembrete& RHS) const { return uiOrigem < RHS.uiOrigem ? true : uiOrigem > RHS.uiOrigem ? false : PonCoordenadas < RHS.PonCoordenadas;};
  
  CArchive& operator>>(CArchive& ar) 
  {
    PonCoordenadas >> ar;
    ar << uiOrigem <<  CString(strTexto.c_str()) << CString(strTitulo.c_str());

    return ar;
  }

  CArchive& operator<<(CArchive& ar) 
  {
    CString CSTexto,CSTitulo;

    PonCoordenadas << ar;
    ar >> uiOrigem >> CSTexto >> CSTitulo;

    strTexto = CSTexto.GetBuffer();
    strTitulo = CSTitulo.GetBuffer();

    return ar;
  }
};

 

