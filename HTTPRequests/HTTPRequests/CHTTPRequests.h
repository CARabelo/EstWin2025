class CHTTPRequests
{
public:
  CHTTPRequests();
  ~CHTTPRequests();

  static __declspec(dllexport) std::string EnviaRequisicaoPegaResposta(std::string& Requisicao);
};

