
typedef std::map<const std::string,int> tymapNomesTrechos;
typedef tymapNomesTrechos::iterator ittymapNomesTrechos;
typedef std::pair<tymapNomesTrechos, bool> pairtymapNomesTrechos;

class CArqTrechos
{
public:

  tymapNomesTrechos  MapaNomesTrechos;
  ittymapNomesTrechos itNomeTrechoAtual;

  CArqTrechos(const std::string& pNomeTrecho,bool bTodos);
  bool PegaNomesTrechos(tlstwstr& NomesTrechos, const std::string& pNomeTrecho);

  ~CArqTrechos();
};

