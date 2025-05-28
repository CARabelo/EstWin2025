class CLimpeza
{
  public:

   double Estaca,
          h,
          SemiDistancia, 
          AreaAtual,
          VolumeTotal,
          VolumeAtual;

  CLimpeza(void);
  ~CLimpeza(void);

 void serialize(CArchive& ar);
};

typedef std::list<CLimpeza> tlstCLimpeza;
typedef tlstCLimpeza::iterator ittlstCLimpeza;