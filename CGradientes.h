//------------------------------------------------------------

class Gradiente
{
public:

  CEstaca EstFinal;
  double x; 

  Gradiente(const CEstaca& Est=CEstaca("INFINITO"),double Val = 0.0) : EstFinal(Est),x(Val) {};

  const Gradiente &operator=(const Gradiente &No);
  bool operator<(const Gradiente& RHS) const { return EstFinal < RHS.EstFinal;};
};

typedef std::set<Gradiente> setGradientes;
typedef setGradientes::iterator itsetGradientes;
typedef const setGradientes::iterator citsetGradientes;

//------------------------------------------------------------

class CGradiente : public CArqGradiente
{
  setGradientes SetGradientes;

public:

  CGradiente(CString& NomeArquivo);       //--- Construtor
  double BuscaGradiente(const CEstaca& Estaca);
  bool Consiste();

protected:

  std::string NomeArquivo;           //--- Nome do arquivo.

};
