class CListaSecoes : public  CList<CSecao,CSecao&> 
{
  enum{PROJETO,TERRENO,PROJETO_TERRENO,NENHUM};

  POSITION Atual;              //--- Ponteiro para o Atual da lista. 
  //	void Serialize(CArchive &arp,CArchive& art,int Tipo);
  CString Projeto;              //--- Nome do projeto em uso.
  CLPerfis TerrenoOrfao;        //--- Lista de terreno sem projeto
  double VolumeTotalCorte,VolumeTotalAterro,VolumeTotalEmpola;
  int EquidisLateral;

public:

  enum{LER,GRAVAR,GRAVAR_SO_PROJETO};

  CTaludesCorte TaludesCorte;
  CTaludesAterro TaludesAterro;

  void Constroi(CString& NomeProjeto) 
  {
    Projeto = NomeProjeto;
    Serialize(LER);
  };

  CString PegaProjeto() { return Projeto;};

  CListaSecoes(CString &NomeProjeto = (CString) "");
  void InsereOrdenada(CSecao &No,int INSERIRIGUAL = FALSE); //--- Insersão ordenada na lista.
  void TaludesDefault();
  void DefRetangDes();        //--- Define as areas de desenhos (Usada nos desenhos).
  void CalculaIs();
  void CalculaIsLongitudinais();       //--- Calcula is longitudinais.
  void Serialize(const int Tipo);
  void LimpaTudo();                    //--- Remove todos os elemntos da lista
  //--- Posiciona o ponteiro da lista na seção igual ou mais 
  //--- proxima no sentido indicado
  POSITION PosicionaNoMaisProximo(CEstaca Estaca,int Direcao = FALSE); 
  double PegaEquidisLateral() { return EquidisLateral;}; 

  void TotalizaVolumes() 
  {
    POSITION Atual = GetTailPosition();

    if(!Atual)
    {
      VolumeTotalCorte = VolumeTotalAterro = VolumeTotalEmpola = 0.0;
      return;
    }

    while (Atual != GetHeadPosition() && GetAt(Atual).Volume[2] == 0.0 && GetPrev(Atual).Volume[3] == 0);

    VolumeTotalCorte = GetAt(Atual).Volume[2];
    VolumeTotalAterro = GetAt(Atual).Volume[3];
    VolumeTotalEmpola = GetAt(Atual).Volume[9];
  };

  double PegaVolTotCorte() { return VolumeTotalCorte ; };
  double PegaVolTotAterro() { return VolumeTotalAterro ; };
  double PegaVolTotEmpola() { return VolumeTotalEmpola ; };

  //--- Retorna um ponteiro para a estaca que estiver dentro da tolerância x e y.
  POSITION PosicionaEstaca(double x,double y,double Toleranciax,double Toleranciay,int Tipo);  

  //--- Posicionadores da lista.

  POSITION PosicionaPrimeira();
  POSITION PosicionaUltima();
  POSITION Posiciona(CEstaca& Estaca);

  //--- Insertores

  //--- Extratores

  POSITION PegaAtual(){return Atual;};
  void LimpaAtual() {Atual = NULL;};

  //--- removedores

  void Remove(POSITION& POSecaoARemover) { RemoveAt(POSecaoARemover);};

  //--- Operadores

  CListaSecoes &operator=(CListaSecoes &No);    
  BOOL operator++(int i);      //--- Acança na lista
  BOOL operator--(int i);      //--- retrocede na lista. 

protected:

  int Compara(CSecao &No1,CSecao &No2) ;  //--- compara dois nós.

};
