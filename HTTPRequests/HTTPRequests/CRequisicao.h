class CRequisicao
{
public:
  CRequisicao();
  ~CRequisicao();

  std::string EnviaRequisicao(std::string& strRequisicao)
  {
    return get_response(strRequisicao);
  }

  static std::string get_response(std::string url);

  static __declspec(dllexport) std::string EnviaRequisicaoPegaResp(const std::string& Requisicao);
};

