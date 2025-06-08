//--- classe que guarda os dados das interseções entre as CN e os talvegues

#define LADO_DIREITO 1
#define LADO_ESQUERDO 0
#define CENTRO 2

#define PARA_FRENTE 1
#define PARA_TRAS 0

class CIntersCNEspigao
{
public:
  Ponto  PontoIntersecao;
  ItLPontos ItPontoAntCN;
  citlstitsetPontos itPEspigAnterior;
  ItLLPontos ItCN;
  void* pSuperficie;

  CIntersCNEspigao(const Ponto& pInter, ItLPontos pItPontoAntCN, ItLLPontos pItCN, citlstitsetPontos pItEspigAnterior) : PontoIntersecao(pInter), ItPontoAntCN(pItPontoAntCN), ItCN(pItCN), itPEspigAnterior(pItEspigAnterior),pSuperficie(nullptr)
  {
  }

  inline bool operator<(const CIntersCNEspigao& RHS) const
  {
    return PontoIntersecao < RHS.PontoIntersecao;
  }

  inline bool operator==(const CIntersCNEspigao& RHS) const
  {
    return PontoIntersecao == RHS.PontoIntersecao;
  }
};

typedef std::list<CIntersCNEspigao> tylstIntersecoes;
typedef tylstIntersecoes::iterator ittylstIntersecoes;
typedef tylstIntersecoes::reverse_iterator rittylstIntersecoes;

class CBacia  
{
  CDadosBacia DadosBacia;
  bool pCalculada;
  tysetCotasArea SetCotasAreas;
  unsigned int EquidisCNHistog;
  double ExtTotalCN;

  Ponto Foz,CentroGravidade;

  int CalculaComprimentoDiagonal();
  int CalculaComprimentoTalvegues();
  bool DefineTalveguePrincipal();
  void PerfilTalvPrincipal(void* pSuperfAtual);
  void LimpaSecao(setPontos& SetXPontos, double DistMinima, double IncMinima);
  int CalculaCotasParticoes();
  int CalculaInterCNEspigao(void* pSuperfAtual);
  int CalculaAreasCN(void* psuperf,double& pExtTotalCN,std::ofstream& ArqLog);
  ittylstIntersecoes PegaIntersEspigao(const Ponto& PontoInters);
  ittylstIntersecoes PegaIntersCN(const Ponto& PontoInters);
  ItLPontos PegaPontoCN(ItLLPontos ItCN, const Ponto& PontoInters);  
  int VerfLado(const Ponto& PEspigAnt,const Ponto& PontoEspigAnt,ItLPontos itCNAtual);
  citlstitsetPontos PegaPontoEspigao(const Ponto& PontoCN);
  int VerfLadoaSeguir(ittylstIntersecoes IntersEspigao);
  void VerfCircular(citlstitsetPontos& ItEspigCalcAtual, citlstitsetPontos& ItEspigCalcAnterior);
  int CalculaDadosHipsometricos(void* pSuperf);
  bool VerificaBacia(tylstIntersecoes& InterErro, std::ofstream& pArqLog);
  void GravaLog(const std::string& strMsg, std::ostream& Stream, bool ForcarHora=false);
  int CalculaAreaPelaCN(ItLPontos& ItCNAtual, ItLPontos ItCNAnterior,double& pAreaCNAtual,double& pExtensaoAtual,ittylstIntersecoes itInterEspigao,std::ofstream& pArqLog,int Sentido);
  int CalculaAreaPelaCNVoltando(ItLPontos& itCNAtual, ItLPontos itCNAnterior, double& pAreaCNAtual, double& pExtensaoAtual, ittylstIntersecoes itInterEspigao, std::ofstream& pArqLog);
  int CalculaAreaPeloEspigao(double& pAreaCNAtual,double& pExtensaoAtual,ItLPontos& ItCNAtual,const Ponto& PontoInterInicial,const Ponto& PontoInterFinal,std::ofstream& pArqLog,int Direcao);
  bool CalculaCotaFoz(ItLDeltaSup itFoz, ItLLDeltaSup itIalvegue);
  int InsereFozNoEspigao(CAciTopografico& pEspigaoOrdenado, const Ponto& pFoz);

  ItLLDeltaSup ItTalveguePrincipal;
  setPontos PerfilTalveguePrincipal;
  setPontos ParticoesTalveguePrincipal;

  public:

 // enum {PONTO_CN, PONTO_INTERS_CN_ESPIGAO,PONTO_CALCULADO,CN_GIRA_DIREITA=4};
    enum {SEM_DEFINICAO, PONTO_INTERS_CN_ESPIGAO, PONTO_CALCULADO, CN_GIRA_DIREITA=4,PONTO_CN=8,FOZ=16};   //--- PONTO_CN tem q ser igual ao Ponto::CURVA_NIVEL = 8

  tylstIntersecoes LstIntersecoesCN, LstIntersecoesEspigao;

  CAciTopografico EspigaoBacia;
  lstPontos LstPontosBacia,
            TalvPrincSendoEditado;
  LLDeltaSup Talvegues;

  int Erro,TotalSaidas;

  int VerificaSaidas(void* pSuperficieAtual);
  int MontaBacia(void* pSuperficieAtual);
  int VerfTalvegue(ItLLDeltaSup AciTopog);
  int CalculaCurvaIpsometrica();
  CDadosBacia& PegaDadosBacia() { return DadosBacia; }
  const CDadosBacia& PegaDadosBacia() const { return DadosBacia; }
  void MudaNomeBacia(const std::string& NovoNome) 
  {
    DadosBacia.NomeBacia = NovoNome;

    //--- Coloca o nome no primeiro ponto do espigão (para saber o nome da bacia ao clicar no espigão)

    std::string Temp((*EspigaoBacia.LstItPontos.begin())->Descricao); 
    Temp = Temp.substr(0,Temp.find_last_of(' '));
    Temp += " " + NovoNome;

    *(const_cast<CString*>(&(*EspigaoBacia.LstItPontos.begin())->Descricao)) = Temp.c_str();
  };
  Ponto& PegaFoz() { return Foz;};

  setPontos& PegaPerfTalvPrincipal() {return PerfilTalveguePrincipal;};
  setPontos& PegaParticoesTalveguePrincipal() { return ParticoesTalveguePrincipal;};
  ItLLDeltaSup PegaItTalveguePrincipal() { return ItTalveguePrincipal;};
  bool TemTalveguePrincipal() {return PerfilTalveguePrincipal.size() > 2;};   //--- Mínimo 2 pontos + foz
  void RemoveTalveguePrincipal()
  {
    ItTalveguePrincipal = Talvegues.end();
    PerfilTalveguePrincipal.clear();
    ParticoesTalveguePrincipal.clear();
  }

  //Não está sendo usada...
  void IncluiTalveguePrincipal(ItLLDeltaSup ItTalveguePrincipal)
  {
    RemoveTalveguePrincipal();
    Talvegues.push_back(*ItTalveguePrincipal);
    Foz.x = Foz.y = Foz.z = INFINITO;
    pCalculada = false;
  }

  int CalculaCurvaHipsometrica(void* pSuperfAtual,double& pExtTotalCN);
  void InverteDirecaoGiro() { EspigaoBacia.InverteDirecaoGiro();};
  bool VerfDescidaTalveguePrincipal();
  unsigned int& EquidistCNHistograma() { return EquidisCNHistog; };

  enum{BACIA_OK, BACIA_ABERTA,FALTA_ESPIGAO,MUITAS_SAIDAS,SEM_SAIDA,FALTA_TALVEGUE_PRINCIPAL,TALVEGUE_PRINCIPAL_SEM_FOZ,
       ERRO_CN_ESPIGAO,TALVEGUE_PRINC_ONDULADO };

  CBacia():pCalculada(false),Erro(0),TotalSaidas(0), EquidisCNHistog(5), ExtTotalCN(INFINITO){};

  CBacia(const CAciTopografico& TalvegueBacia);
  CBacia(const LDeltaSup& DeltaSupTalvegueBacia);
  int DefineEspigao(ItCLLDeltaSup DeltaSupTalvegueBacia);
  int Calcular(void* pSuperf, CString& StrObservacoes, CDialogEx* Dialogo);
  void Calculada(bool Estado) { pCalculada = Estado; };
  bool EstaCalculada() { return pCalculada; };
  bool CalciMedioTalvPrincipal();
  void LimpaIntersEspigao();
  void LimpaIntersCN(void* pSuperf);
  bool VerfCNCortandoVariasVezesEspigao(tylstIntersecoes& pInterErro,std::ofstream& pArqLog);
  tysetCotasArea& PegaSetCotasAreas() { return SetCotasAreas; };

  inline void RemovePontoHistograma(double x)
  {
    CCotaArea CotaArea(x,0.0,0.0,0.0);
    SetCotasAreas.erase(CotaArea);
  }

  inline void IncrementaItCN(ItLPontos& itCNAtu, ItLPontos& itCNAnt,LPontos& LCN,int Dir)
  {
    if (Dir == PARA_TRAS)
    {
      //--- Verifica listas circulares ( O último ponto é repetido)
      //--- Se itCNAtual ficar end no meio é porque a CN é circular e chegou no fim da CN
      //--- Neste caso o proximo ponto é o 1o antes do fim da CN e o ponto anterior é o 
      //--- fim da CN (Lista  circular)

      if (itCNAtu == LCN.begin())  //--- Se chegou no começo vai pro ultimo
      {
        itCNAtu = LCN.end();
        --itCNAtu;
        --itCNAtu;     //--- O último ponto é repetido
        --itCNAnt;
      }
      else 
      {
        if (itCNAnt == LCN.begin())
        {
          itCNAnt = LCN.end();
          --itCNAnt;
          --itCNAnt;
          --itCNAtu;
        }
        else
        {
          --itCNAtu;
          --itCNAnt;
        }
      }
    }
    else
    {
      ++itCNAtu;
      ++itCNAnt;

      //--- A lista é circular

      if (itCNAtu == LCN.end())   //--- Se chegou no último vai pro começo
      {
        itCNAnt = itCNAtu = LCN.begin();
        ++itCNAtu;                //--- O último ponto é repetido
      }
      else
      {
        if (itCNAnt == LCN.end())
        {
          itCNAnt = LCN.begin();
          ++itCNAnt;
          ++itCNAtu;
        }
      }
    }
  }

  inline void IncrementaItEspigao(citlstitsetPontos& ItEspigAnt, citlstitsetPontos& ItEspigAtu,int Dir)
  {
    if (Dir == LADO_ESQUERDO)
    {
      //--- Verifica listas circulares ( No espigão O último ponto NÃO é repetido)

      if (ItEspigAtu == EspigaoBacia.PegaLista().begin())  //--- Se chegou no começo vai pro ultimo
      {
        ItEspigAtu = EspigaoBacia.PegaLista().end();
        --ItEspigAtu;
      }
      else
      {
        if (ItEspigAnt == EspigaoBacia.PegaLista().begin())
        {
          ItEspigAnt = EspigaoBacia.PegaLista().end();
          --ItEspigAnt;
        }
      }

      --ItEspigAtu;
      --ItEspigAnt;
    }
    else
    {
      ItEspigAtu++;
      ItEspigAnt++;

      //--- A lista é circular

      if (ItEspigAtu == EspigaoBacia.PegaLista().end())   //--- Se chegou no último dá a volta
      {
        ItEspigAtu = EspigaoBacia.PegaLista().begin();
      }
      else
      {
        if (ItEspigAnt == EspigaoBacia.PegaLista().end())
        {
          ItEspigAnt = EspigaoBacia.PegaLista().begin();
        }
      }
    }
  }

  inline void CalculaAreaFaixaCNs()
  {
    ittysetCotasArea ItCA(SetCotasAreas.begin()), ItCAAnterior(SetCotasAreas.end());
    ExtTotalCN = 0.0;
   
    while (ItCA != SetCotasAreas.end())
    {
      if (ItCA->AreaTotal >= 0.0)
      {
        CCotaArea* pCA(const_cast<CCotaArea*>(&*ItCA));

        pCA->AreaCN = 100.0 - ItCA->AreaTotal / EspigaoBacia.PegaArea() * 100.0;
        ExtTotalCN += ItCA->Extensao;

        if (ItCAAnterior != SetCotasAreas.end())
        {
          pCA->Inclinacao = (ItCA->Cota - ItCAAnterior->Cota) / (ItCA->AreaCN - ItCAAnterior->AreaCN);
        }
        ItCAAnterior = ItCA;
      }
      ItCA++;
    }
  }

  inline bool CBacia::VerfPontoTalvegue(const Ponto& Ponto)
  {
    for (ItLLDeltaSup itTalvegueAtual = Talvegues.begin(); itTalvegueAtual != Talvegues.end(); ++itTalvegueAtual)
    {
      for (ItLDeltaSup itPAtual = itTalvegueAtual->begin(); itPAtual != itTalvegueAtual->end(); ++itPAtual)
      {
        if (*itPAtual == Ponto) return true;
      }
    }

    return false;
  }

  bool& Calculada() { return pCalculada; };
  bool VerificaCnsCortandoTalveguePrincipal(void* pSuperficie);
  Ponto& CG() { return CentroGravidade;};
  double PegaEquidisCN(void* pSuperf);
  
  void LimpaBacia(void* pSuperf)
  {
    EspigaoBacia.Limpa();
    LimpaIntersCN(pSuperf);
    LimpaIntersEspigao();
    LstPontosBacia.clear();
  }

  void RemoveFoz();

  CArchive& operator<<(CArchive& ar);
  CArchive& operator>>(CArchive& ar);

  bool operator<(const CBacia& RHS) const
  {
    return Foz < RHS.Foz;
  }
};

typedef std::list<CBacia> tylstBacias;
typedef tylstBacias::iterator ittylstBacias;
typedef tylstBacias::const_iterator cittylstBacias;
