class CMapaCNIBGE
{
  std::string NomeProjeto;
  tylstCN LCN;
  tylstCTextos LTextos;

  public:

    enum TipoLeitura{LER_NADA,LER_PONTOS_COTA,LER_CNS,LER_COTAS_CN=4,LER_TUDO=7};

    CMapaCNIBGE(const std::string& NomeProj = std::string("")) : NomeProjeto(NomeProj) {}
    int LeArquivoDXF(const std::string& NomeArq);
    int LeArquivoDXF_DGN(const std::string& NomeArq, CProgressCtrl* pProgresso=nullptr);

    tylstCN& CurvasNivel() { return LCN;};
    tylstCTextos& Textos() { return LTextos;};
};

