typedef std::map<const CDRetangulo,CAciTopografico> mapAciTipog;
typedef mapAciTipog::iterator itmapAciTipog;
typedef mapAciTipog::const_iterator citmapAciTipog;
typedef std::pair<const CDRetangulo,CAciTopografico> pairmapAciTipog;

class CAcidentesTopograficos
{
  mapAciTipog MapAcidentesTopograficos;

  public:
    CAcidentesTopograficos(void) {};
    ~CAcidentesTopograficos(void) {};

    bool CAcidentesTopograficos::Insere(const CAciTopografico& pAciTopog)
    {
      return MapAcidentesTopograficos.insert(pairmapAciTipog(pAciTopog.BuscaRetanInscrito(),pAciTopog)).second;
    }

    bool CAcidentesTopograficos::Remove(const CAciTopografico& pAciTopog)
    {
      return MapAcidentesTopograficos.erase(pAciTopog.BuscaRetanInscrito()) > 0;
    }

    const mapAciTipog& PegaMapAcidentesTopograficos() const { return MapAcidentesTopograficos;}; 

    CArchive& operator<<(CArchive& ar) 
    {
      unsigned long QuanAcidTopog(0);
    
      for(ar >> QuanAcidTopog ; QuanAcidTopog > 0 ; --QuanAcidTopog) 
      {
        CAciTopografico AciTopogAtual;
        AciTopogAtual << ar; 
        pairmapAciTipog pmap(AciTopogAtual.BuscaRetanInscrito(),AciTopogAtual);
        MapAcidentesTopograficos.insert(pairmapAciTipog(AciTopogAtual.BuscaRetanInscrito(),AciTopogAtual));
      }
      return ar;
   }

   CArchive& operator>>(CArchive& ar) 
   {
     ar << (unsigned long) MapAcidentesTopograficos.size();
     
     for (register itmapAciTipog it = MapAcidentesTopograficos.begin() ; it != MapAcidentesTopograficos.end()  ; ++it)
     {
       it->second >> ar;
     }
     
     return ar;
   }
};


