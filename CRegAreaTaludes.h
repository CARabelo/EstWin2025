class CRegAreaTaludes
{
  public:

   double Estaca;
   double SemiDistancia, 
          AreaAtual,
          AreaTotal;

  CRegAreaTaludes(void);
  ~CRegAreaTaludes(void);

 void serialize(CArchive& ar);
};

typedef std::list<CRegAreaTaludes> tlstCRegAreaTaludes;
typedef tlstCRegAreaTaludes::iterator ittlstCRegAreaTaludes;