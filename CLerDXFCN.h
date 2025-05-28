class CLerDXFCN
{
  CProgressCtrl* pProgresso;
  CStatic* pObservacoes;
  void* pDialogo;
  
  bool BuscaLabelCN(const std::string& pLabel, std::ifstream& pArqDXFCN, std::string& pstrBufferLinha,int Tipo = CLerDXFCN::IGUAL)
  {
    int AchouCN(false);
    unsigned long contalinhas = 0;

    do
    {
      std::getline(pArqDXFCN, pstrBufferLinha);
      contalinhas++;
      Limpa(pstrBufferLinha);
      if(pArqDXFCN.good() &&  pstrBufferLinha == pLabel)
      {
        std::getline(pArqDXFCN, pstrBufferLinha);
        contalinhas++;
        Limpa(pstrBufferLinha);
        if(pstrBufferLinha.compare("6") == 0) AchouCN = true;
      }
    }
    while ((pArqDXFCN.good() && !AchouCN));

    return pArqDXFCN.good();
  }

  bool BuscaLabel(const std::string& pLabel, std::ifstream& pArqDXFCN, std::string& pstrBufferLinha, int Tipo = CLerDXFCN::IGUAL)
  {
    do
    {
      std::getline(pArqDXFCN, pstrBufferLinha);
      Limpa(pstrBufferLinha);
    } while (pArqDXFCN.good() && ((Tipo == IGUAL && pstrBufferLinha != pLabel) || (Tipo != IGUAL && pstrBufferLinha.find(pLabel) == std::string::npos)));

    return pArqDXFCN.good();
  }

  int LePontosCN(tylstGdc_Coord_3d& pLstCoordGdc, std::ifstream& pArqDXFCN, std::string& pBufferLinha, std::ofstream& ArqTXT);
  int LePontosSoltos(lstPontos& PontosSoltos, std::ifstream& pArqDXFCN, std::string& pBufferLinha, std::ofstream& pArqTXT);

  //int LePontosCN(lstPontos& LPontosCN, std::ifstream& pArqDXFCN, std::string& pBufferLinha);

  public:

  CLerDXFCN(const std::string& NomeArq, lstlstPontos& CNs, setPontos& PontosSoltos, CProgressCtrl* pProgresso=nullptr,void* pDialog=nullptr, CStatic* pStaObserv=nullptr);

  ~CLerDXFCN();

  enum ETipos{NENHUM_TIPO,IGUAL,CONTEM};

};

