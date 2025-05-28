typedef std::map<CChaveAlinhamento,CAlinhamento> tymapCAlinhamentos;
typedef tymapCAlinhamentos::iterator ittymapCAlinhamentos;
typedef tymapCAlinhamentos::const_iterator cittymapCAlinhamentos;
typedef std::pair <CChaveAlinhamento,CAlinhamento> typairchavealinCAlinhamentos;
typedef std::pair <ittymapCAlinhamentos, bool> typairtymapCAlinhamentosBool;

typedef std::map<double,ittymapCAlinhamentos> tymapAlinhamentosAtivos;
typedef tymapAlinhamentosAtivos::iterator ittymapAlinhamentosAtivos;
typedef std::pair <const double, ittymapCAlinhamentos> typairdoubleittymapAlinhamentos;
typedef std::pair <ittymapAlinhamentosAtivos, bool> typairittymapAlinhamentosAtivosbool;

typedef std::list<Ponto> tylstpontos;
typedef tylstpontos::iterator ittylstpontos;

class CAlinhamentos
{
  public:

  CAlinhamentos();
  ~CAlinhamentos();

  tymapCAlinhamentos MapAlinhamentos;
  tymapAlinhamentosAtivos MapAlinhamentosAtivos;
  tylstpontos LstPontosBordo;
  
  void Serialize(CArchive& ar);
};

