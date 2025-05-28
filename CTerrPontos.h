//------------------------------------------------------------
class CTerrPontos : public CArqTerrPontos
{
  CString NomeArquivo;           //--- Nome do arquivo.
  CProgressCtrl* ProgCtrl;

public:

  CLPerfis LTerrPontos;

  CTerrPontos(CString &NomeArquivo,CProgressCtrl* Prog = NULL);       //--- Construtor

};
