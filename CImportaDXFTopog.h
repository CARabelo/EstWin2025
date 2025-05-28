class CImportaDXFTopografia
{
  tlstwstr NomesArquivos;
  CTriangulacao Triangulacao;
  LLPontos CurvasNivel;
  LLPontos Talvegues;
  tlstwstr Camadas;

  std::string CaminhoProjeto;

  int LeCamadas(std::ifstream& ArqDXFTopog);
  bool SalvaArquivoCamadas(const std::string& NomeArquivo);

  public:

//    CString NomeArqTriang, NomeArqAciTopog;

   // CDiaImpArqTopografia(std::string& pArqTrian,std::string& pArqAcitopog, CWnd* pParent) : NomeArqTrian(pArqTrian), NomeArqAcitopog(pNomeArqAciTopog), CDialogEx(IDD_DIAIMPTRIATOPOG, pParent){}

    CImportaDXFTopografia(const std::string& pCaminhoProjeto) : CaminhoProjeto(pCaminhoProjeto) {}
    void ImportarArquivo();
    bool ImportaArquivo(const std::string& NomeArquivo, const std::string& NomeArqAciTopog);
    bool ImportaCNs(const std::string& NomeArquivo, const std::string& CNMestra, const std::string& CNInterm);

    //---- ler arquivos SIG

    bool ImportaCNsSIG(const std::string& NomeArquivo, const std::string& strEntity);
    bool ImportaTalveguesSIG(const std::string& NomeArquivo, const std::string& strEntity);

    CTriangulacao& PegaTriangulacao() {return Triangulacao;};
    LLPontos& PegaCurvasNivel() { return CurvasNivel;};
    tlstwstr& PegaCamadas() { return Camadas;};
};

