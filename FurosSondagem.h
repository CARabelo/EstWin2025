class CFurosSondagem 
{
  tysetFurosSondagem SetFurosSondagem;
  std::string strNomeArquivo;

public:

  CFurosSondagem(const std::string& NomArq="");
  ~CFurosSondagem(){};
  const tysetFurosSondagem& PegaSetFurosSondagem() const { return SetFurosSondagem;};
  ittysetFurosSondagem PegaFuroMaisProximo(double Estaca,double ToleranciaEstaca,double Cota, double ToleranciaCota) const;

  CArchive& operator>>(CArchive& ar) 
    {
      ar << SetFurosSondagem.size();

      for (ittysetFurosSondagem it = SetFurosSondagem.begin() ; it != SetFurosSondagem.end() ; ++it)
      {
        *it >> ar;
      }
    
      return ar;
    };

  CArchive& operator<<(CArchive& ar) 
  {
    CFuroSondagem FS;
    size_t TamSet(0);
    
    for(ar >> TamSet ; TamSet > 0 ; --TamSet)
    {
      FS << ar;
      SetFurosSondagem.insert(FS);
    }
    return ar;
  };
};