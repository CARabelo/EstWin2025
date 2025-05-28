#if !defined(AFX_SUPERFICIE_H__F030F6A2_D02F_11D7_BE61_444553540000__INCLUDED_)
#define AFX_SUPERFICIE_H__F030F6A2_D02F_11D7_BE61_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Superficie.h : header file

typedef std::set<CDeltaSuperficie> SSuperficie;       //--- Set de pontos da superficie.
typedef SSuperficie::iterator ItSSuperficie;
typedef SSuperficie::const_iterator ItCSSuperficie;
typedef std::pair<ItCSSuperficie,bool> PaircitbooSSuperficie; 
typedef std::pair<ItSSuperficie,bool> PairitbooSSuperficie; 
typedef std::pair<ItCSSuperficie,ItCSSuperficie> PairitSSuperficie; 

typedef std::set<Ponto> SSetPontos;                   //--- Set  de um conjunto de pontos
typedef SSetPontos::iterator ItSSetPontos; 
typedef SSetPontos::const_iterator ItCSSetPontos;
typedef std::pair< ItSSetPontos,bool> PairItSSetPontosBool;

typedef std::list<Ponto> LPontos;
typedef LPontos::iterator ItLPontos;
typedef LPontos::const_iterator ItCLPontos;
typedef LPontos::reverse_iterator ItRLPontos;

typedef std::list<ItLPontos> LITLPontos;
typedef LITLPontos::iterator ItLITLPontos;

typedef std::list<LPontos> LLPontos;
typedef LLPontos::iterator ItLLPontos;
typedef LLPontos::const_iterator ItCLLPontos;

typedef std::multimap<unsigned int,ItCLLDeltaSup> mmapitLLDeltaSup;
typedef std::pair<unsigned int,ItCLLDeltaSup> pairmmapitLLDeltaSup;
typedef mmapitLLDeltaSup::iterator itmmapitLLDeltaSup;
typedef mmapitLLDeltaSup::const_iterator citmmapitLLDeltaSup;

typedef std::pair<itmmapitLLDeltaSup,itmmapitLLDeltaSup> paitititmmapitLLDeltaSup;
typedef std::pair<ItCLLDeltaSup,ItCLDeltaSup> paitCLLDeltaSupItCLDEltaSup;

typedef std::list< ItCLLDeltaSup> lstItCLLDeltaSup;
typedef lstItCLLDeltaSup::iterator itlstItCLLDeltaSup;

typedef std::list<ItSSuperficie> tylstitSSuperficie; 
typedef tylstitSSuperficie::iterator ittylstitSSuperficie;

struct ltitSSuperficie
{
  bool operator()(const ItSSuperficie itP1, const ItSSuperficie itP2) const 
  { 
    return *itP1 < *itP2;
  }
};

typedef std::set<ItSSuperficie,ltitSSuperficie> tysetitSSuperficie; 
typedef tysetitSSuperficie::iterator ittysetitSSuperficie;

struct ltPontoxyz
{
   bool operator()(const ItLPontos ItP1, const ItLPontos ItP2) const 
   {
    double Deltax = ItP1->x - ItP2->x,Deltay = ItP1->y - ItP2->y;     

    return Deltax  < -1e-4 ? true : Deltax  > 1e-4 ? false : 
           Deltay  < -1e-4 ? true : Deltay  > 1e-4 ? false :
           ItP1->z - ItP2->z < -1e-4 ? true : false;
   }
};

typedef std::set<ItLPontos, ltPontoxyz> SetItLPontos;
typedef SetItLPontos::iterator ItSetItLPontos;
typedef SetItLPontos::const_iterator ItCSetItLPontos;

class CItLPontosBegin
{
  public:
    ItLPontos It;
    LPontos* pCN;
    CItLPontosBegin(ItLPontos pIt, LPontos* ppCN) :  It(pIt), pCN(ppCN) {};
    bool operator<(const CItLPontosBegin& RHS) const
    {
      return *It < *RHS.It;
    };
};

typedef std::set<CItLPontosBegin> SetItLPontosBegin;
typedef SetItLPontosBegin::iterator ItSetItLPontosBegin;
typedef SetItLPontosBegin::const_iterator ItCSetItLPontosBegin;

typedef std::set<std::string> tysetNomeArquivos;
typedef std::set<std::string>::iterator tyditsetNomeArquivos;

typedef std::set<int> tysetint;
typedef tysetint::iterator ittysetint;

struct ltpairint
{
  bool operator()(const std::pair<int,int>& pint1,const std::pair<int,int>& pint2)  const
  {
    return pint1.first < pint2.first;
  }
};

typedef std::set< std::pair<int, int>,ltpairint> tysetpairint;
typedef tysetpairint::iterator ittysetpairint;

typedef std::set<CSegmento> tysetCSegmentos;
typedef tysetCSegmentos::iterator ittysetCSegmentos;

typedef std::list<std::string> tylstNomeArquivos;
typedef tylstNomeArquivos::iterator tyitlstNomeArquivos;

struct ltNomeItSSuperficie
{
  bool operator()(const CString& Nome1,const CString& Nome2)  const
  { 
    return Nome1 < Nome2;
  }
};

typedef std::multimap<CString,ItSSuperficie,ltNomeItSSuperficie> mmapItSSuperficie;
typedef mmapItSSuperficie::iterator itmmapItSSuperficie;
typedef std::pair<CString,ItSSuperficie> pairmmapItSSuperficie;
typedef std::pair<itmmapItSSuperficie,itmmapItSSuperficie> pairititmmapSSuperficie;


struct ltItlDS
{
  bool operator()(const ItLDeltaSup it1, const ItLDeltaSup it2)  const
  {
    return it1->PCentral < it2->PCentral;
  }
};

typedef std::multiset<ItLDeltaSup,ltItlDS> tymsetItLDEltaSup;
typedef tymsetItLDEltaSup::iterator ittymsetItLDEltaSup;

/////////////////////////////////////////////////////////////////////////////
// CSuperficie window

class CSuperficie 
{
  enum{LADOOPOSTO,LADOADJACENTE};
 
  //--- São criadas duas superficies que são alternadas nas rotações:
  //--- ao rotacionar a superficie rotacionada  colocada  no outro set. O ponteiro para a 
  //--- superficie atual é atualizado para esta superficie. Caso a superficie seja rotacionada 
  //--- novamente os pontos rotacionado são colocados na superficie anterior e o ponteiro para 
  //--- a superficie atual é novamente atualizado,

  bool Rotacionada, Alterada, InicioInterpolacao,bInverteuFronteira;
  double AnguloDeRotacao,Seno,Coseno,Area,Perimetro,douDeclinacao;
  int AngRotacaoTopInt;

  SSuperficie SetSuperficie[2];
  mmapitLLDeltaSup MMapItLLDeltaSup;
  mmapItSSuperficie MMapSuperficieNomes;
  SetItLPontos SetItPontosCN;
  LPontos TalveguePrincipal,BaciasImportadas,TalveguesImportados;

  tylstBacias BaciasHidrograficas;

  LLDeltaSup ListaRestricoes,                  //--- Lista com todas as restricoes 
             ListaBordos;                      //--- Lista com os bordos, (apenas uma, mantem o tipo por compatibilidade de iteradores)
  SSetPontos SetPontosFronteira,               //--- Usado para procurar pontos na fronteira
             SetPontosInseridos,
             SetPontosDesabilitados,
             SetPontosInsSecoes,
             SetPontosSoltosDGN;
  tysetCSegmentos SetSegmentosRestricoes;

  std::string NomeProjeto;
  //static tysetint SetNGeraArestaForcada;    
  tysetpairint SetForcarAresta;

  bool inline CalculaCota(const Ponto& P1,const Ponto& P2,Ponto& Intersecao);
  void InterpolaCotas(CDeltaSuperficie& DS1,ittysetArestas itPLigado,double EquiCotas);
  void AtualizaLimites(const CDeltaSuperficie& DeltaSup) 
  {
    const Ponto& PCentAtual = DeltaSup.PegaPCentral();

    if (PCentAtual.x < LimiteEsq) 
    {
      LimiteEsq = PCentAtual.x;
    }
    if (PCentAtual.x > LimiteDir) 
    {
      LimiteDir = PCentAtual.x;
    }
    if (PCentAtual.y > LimiteSup) 
    {
      LimiteSup = PCentAtual.y;
    }
    if (PCentAtual.y < LimiteInf) LimiteInf = PCentAtual.y;
  };

  void RedefineLimites()
  {
    ItSSuperficie ItSSupAtual(pSuperficieAtual->begin());

    LimiteEsq = LimiteInf = INFINITO;
    LimiteSup = LimiteDir = -INFINITO;

    register unsigned int QuanPontos((unsigned int)pSuperficieAtual->size());

  //--- Verificar

    if (QuanPontos > 1)
    {
      for (; QuanPontos; QuanPontos--, ItSSupAtual++)
      {
        AtualizaLimites((*ItSSupAtual));
      }
    }
    if (this->LPontosCN.size() > 0)
    {
      for (ItLLPontos itCN = LPontosCN.begin(); itCN != LPontosCN.end(); itCN++)
      {
        for (ItLPontos itPontoCN = itCN->begin() ; itPontoCN != itCN->end(); itPontoCN++)
        {
          AtualizaLimites(*itPontoCN);
        }
      }
    }
  };

  void PegaNumeroDescricao(const CString& NomeArq);
  int LeArqPontos(const CString& Tipo, const CString& NomeProjeto);
  bool RemoveOutraPonta(const ittysetArestas ItPontoAtual,const Ponto& PCentral);
  void ForcarAresta(const Ponto& ItP1,const Ponto& P2);
  void RetiraArestasInterceptantes(const ItLLDeltaSup itLDeltaSuperficie);
  double DefineAzimute(double Deltax,double* Angulo);
  ittysetArestas AchaSaidaCN(ItSSuperficie itS1,ItSSuperficie itS2,double Cota,int& Achou,int& LadoSaida,itsetPontosS& ArestaSaida);
  void RotacionarCN(double Seno,double Cosseno,double Ang);
  void RotacionaEquipDrenagem(double Seno, double Cosseno, double Ang);
  itsetPontosS PegaMenorAresta(ItSSuperficie itSup,setPontosS& pLstArestas);
  ittysetArestas AchaOutraPonta(const Ponto& PontoAtual,const Ponto& PCentral);
  int LigaCNRestricoesFechadas();

  void LimpaBalizasSuperficie()
  {
    for (ItSSuperficie itSU = pSuperficieAtual->begin() ; itSU != pSuperficieAtual->end() ; itSU++)
	  {
		  for(register ittysetArestas itSA = itSU->PontosLigados.begin() ; itSA !=  itSU->PontosLigados.end() ; itSA++)
		  {
			  for(register itsetPontosS itPI = itSA->PontosInterpolacao.begin(); itPI != itSA->PontosInterpolacao.end() ; itPI++)
			  {
				  (const_cast<Ponto*>(&*itPI))->Baliza = false;   
			  }
		  }
    }
  }

  inline void PegaProximoAlfaNum(std::ifstream& Stream)
  {
    do
    {
      register char Carac(Stream.get());

     // if(iswalnum(Carac) && Carac != ' ')
      if ((iswalnum(Carac) && Carac != ' ') || Carac == '-' || Carac == '+')
      {
        Stream.putback(Carac);
        return;   
      }
    }while (Stream.good() && !Stream.eof());
  }

  void LimpaSupBalizasRestricoes();      //--- Limpa na superficie as balizas das restriçoes interpoladas      
  int PegaNomesArquivosTerreno(tysetNomeArquivos& setNomeArquivos,CString NomeProjeto);                    //--- Obsoleto???
  int PegaNomesArquivosTerreno(tylstNomeArquivos& lstNomeArquivos,const std::string strNomeProj);

  inline void PulaBrancos(std::ifstream& Stream)
  {
    register char Carac(0);
    do
    {
      Carac = Stream.get();
    }while (( Carac == ' '  && Carac != '\x10') && !Stream.eof());

    if(Carac != ' ')   Stream.putback(Carac);
  }

  inline void GetLine(std::ifstream& ifs,std::string& Buffer,const std::string& strDelim)
  {
    Buffer.clear();
    char Carac;

   // if(ifs.eof())
    {
      do
      {
        Carac = ifs.get();
        Buffer.push_back(Carac);
      }while (strDelim.find(*Buffer.rbegin()) == std::string::npos && !ifs.eof() && ifs.good());

      ifs.putback(*Buffer.rbegin());
      Buffer.pop_back();
    }
  }

  void RotacionaPontosInserDesabil(double Sin, double Cos, double AnguloArotacionar)
  {
    SSetPontos* pSetPontosAtual[2] = { &SetPontosInseridos,&SetPontosDesabilitados };

    for (int C = 0; C < 2; C++)
    {
      SSetPontos* pSetSupAtual(pSetPontosAtual[C]), SetRotacionado;

      for (register ItSSetPontos it = pSetSupAtual->begin(); it != pSetSupAtual->end(); ++it)
      {
        Ponto PontoTemp(*it);
        PontoTemp.RotacionaPlano(Sin, Cos, AnguloArotacionar);
        SetRotacionado.insert(PontoTemp);
      }

      *pSetPontosAtual[C] = SetRotacionado;
    }
  }

  int PegaNomesArquivosPIS(tysetNomeArquivos& setNomeArquivos, CString NomeProjeto);
  bool CSuperficie::LeArqPIS(CString& NomeProjBase);                                    //--- Pontos Incluidos seções
  
  int SuperficieAtual,NPontoInsUsuario; 

  double LimiteEsq,LimiteSup,LimiteInf,LimiteDir,AngRotacaoBordos,EquidisCN;

  CAcidentesTopograficos AcidentesTopograficos;
  tymapEquipDrenagem EquipamentosDrenagem;

  void SerializaCNSimbolTerraplanagem(int Tipo, const std::string& NomeProjBase);

  void LimpaTalveguesZerados()
  {
    for (auto it = ListaRestricoes.begin(); it != ListaRestricoes.end(); )
    {
      if (it->size() < 2)
        ListaRestricoes.erase(it++);
      else
        it++;
    }
  }
  // Construction
public:

  const std::string& PegaNomeProjeto() const { return NomeProjeto ; };

  void ZeraAngRot()
  { 
	  Rotacionada = false;  
	  AnguloDeRotacao = 0.0; 
	  Seno = 0.0;
	  Coseno = 1.0;
	  AngRotacaoBordos = 0.0;
	  AngRotacaoTopInt = 0;
	 
  }

  void LimpaBalizasCNs()                  //--- Limpa nas CNs as balizas das CNs desenhadas
  {
    for (ItLLPontos itLLCN = LPontosCN.begin(); itLLCN != LPontosCN.end(); itLLCN++)
      for (register ItLPontos itLCN = itLLCN->begin(); itLCN != itLLCN->end(); itLCN++)
        itLCN->Baliza = itLCN->Baliza2 = false; 
  }
    
  CSuperficie(CString& NomeProjeto, bool SoRestricoes = false);//,bool Terraplanagem = false);
  void CriaSetCurvasNivelNaoCotadasDGN(int SuperfDrones = false);
  bool Descarrega(const std::string& NomeArquivo,bool OutraSuper = false);
  bool VerfGeraArestaForcada(int Tipo) const 
  {
    auto it(SetForcarAresta.find(std::pair<int,int>(Tipo,0)));
    return (it != SetForcarAresta.end() && it->second == 1);
  }
  int VerfPontoForaSuperficie(const Ponto& PontoVerificar,bool MostrarMensagem = true);
  bool VerfPontosForaSuperficie(SSetPontos& PontosProblema,int* pResp);
  bool TemBordos() const {return ListaBordos.size() > 0;}
  int AngRotacaoTopogGraus() const { return AngRotacaoTopInt;};
  bool VerfFronteira(SSetPontos& PontosProblema,bool MostrarMensagem = false,int* pResposta = NULL);
  void Constroi();
  ItCLLDeltaSup VerificaNaTangenteRestricoes(ItCLLDeltaSup ItCLLDeltaSupInicial, const Ponto& PAtual,
                double Toleranciax, ItCLDeltaSup* pItCLDS, double* pRaio, CSuperficie* pSuperfTerrap,
                unsigned int ObjAtual, bool FiltrarAciTopog, bool VerifBordos);
  void LeSetForcarAresta();

  void Clear()
  {
    MMapItLLDeltaSup.clear();
    ListaRestricoes.clear();
    ListaBordos.clear();             
    SetPontosFronteira.clear();      
    SetPontosInseridos.clear();
    SetPontosDesabilitados.clear();
    SetPontosInsSecoes.clear();
    SetSegmentosRestricoes.clear();
    SetItPontosCN.clear();
    LPontosCN.clear();                 
    SetPontosCurvasNivel.clear();
    pSuperficieAtual->clear();
    pOutraSuperficie->clear();
    MMapSuperficieNomes.clear(); 
    //SetPontosCNDGNSemCota.clear();
    ZeraAngRot();
  }
  
  void CriaMSetNomes()
  {
    MMapSuperficieNomes.clear();

    for (register ItSSuperficie itSU = pSuperficieAtual->begin() ; itSU != pSuperficieAtual->end() ; itSU++)
	  {
      MMapSuperficieNomes.insert(pairmmapItSSuperficie(itSU->PCentral.Nome,itSU));
    }
  }

  const CString& ProxNumPonInsUsuario() 
  {
    static CString CStrTemp;
    CStrTemp.Format("%d",++NPontoInsUsuario);
   
    return CStrTemp;
  };

  void InverteCNs()
  {
    for (ItLLPontos itLLCN = LPontosCN.begin(); itLLCN != LPontosCN.end(); itLLCN++)
    {
      std::reverse(itLLCN->begin(), itLLCN->end());
    }
  }

  void LeNumDescPontosInseridos();

  // Attributes
//public:

  enum{TIPOFRONTEIRA,TIPOPOLIGONO,TIPOPROJTERRAPLANAGEM};

  LLPontos LPontosCN,                     //--- Lista com as curvas de nivel
           LLPontosCNSimbTerrapl;         //--- CNs na simbologia de terraplanagem
  SetItLPontosBegin SetPontosCurvasNivel; //--- Pontos CN  

  SSuperficie* pSuperficieAtual,      //--- ponteiro para a superficie que esta sendo usada
             * pOutraSuperficie;      //--- ponteiro para outra superficie ( a q não está sendo usada) 

  lstPontos LPontosRemSecoes;
  ItLLDeltaSup ItTalvFozSIG;

  double MaiorCota;
  int ErroInterpolacao;

  double PegaLimiteEsq() { return LimiteEsq;};
  double PegaLimiteDir() { return LimiteDir;};
  double PegaLimiteInf() { return LimiteInf;};
  double PegaLimiteSup() { return LimiteSup;};

  void PegaLimites(Ponto& SupEsq, Ponto& InfDir)
  {
    SupEsq.x = LimiteEsq;
    SupEsq.y = LimiteSup;
    InfDir.x = LimiteDir;
    InfDir.y = LimiteInf;
  }

  double PegaAnguloRotacao(){ return AnguloDeRotacao;};
  double PegaAngRotacaoBordos() { return AngRotacaoBordos; };
  double PegaSeno() { return Seno;};
  double PegaCoseno() { return Coseno;};
  unsigned long PegaQuantPontos() { return pSuperficieAtual ? (unsigned long) pSuperficieAtual->size() : 0 ;};
  bool EstaRotacionada() { return Rotacionada;};
  const SSuperficie& PegaSuperficie() const {return *pSuperficieAtual;};
  void MudaDeltaSup(ItSSuperficie itDSupAnt,const CDeltaSuperficie& DSupNova) 
  {
    pSuperficieAtual->erase(itDSupAnt);
    pSuperficieAtual->insert(DSupNova);
  };

  ItSSuperficie InsereDeltaSuperficie(const CDeltaSuperficie& CDS)
  {
    PaircitbooSSuperficie pRet( pSuperficieAtual->insert(CDS));
    if(pRet.second == true) 
    {
      (const_cast<CDeltaSuperficie&>(CDS)).Status = CDeltaSuperficie::INSERIDO_USUARIO;
      SetPontosInseridos.insert(CDS.PegaPCentral());

      MMapSuperficieNomes.insert(pairmmapItSSuperficie(pRet.first->PegaPCentral().Nome,pRet.first));

      return pRet.first; 
    }
    return pSuperficieAtual->end(); 
  }

  bool DesabilitaDeltaSuperficie(ItSSuperficie ITSS)
  {
    CDeltaSuperficie* pCDS(const_cast<CDeltaSuperficie*>(&*ITSS));
    pCDS->Status = CDeltaSuperficie::DESABILITADO;
    return SetPontosDesabilitados.insert(ITSS->PegaPCentral()).second;
  }

  bool HabilitaDeltaSuperficie(ItSSetPontos ITSP)
  {
    ItSSuperficie itSSuper(this->pSuperficieAtual->find(*ITSP));
	  if(itSSuper != pSuperficieAtual->end())
	  {
      CDeltaSuperficie* pCDS(const_cast<CDeltaSuperficie*>(&*itSSuper));
      pCDS->Status = CDeltaSuperficie::HABILITADO;
	  }

    return SetPontosDesabilitados.erase(*ITSP) == 1;
  }

  void RemovePontosInseridosUsuario(lstPontos& ItLLstSupRemover)
  {
    ittylstpontos ItUltimoPonto(ItLLstSupRemover.end());  //--- O último Ponto e igual ao primeiro

    ItUltimoPonto--;

    for (ittylstpontos It = ItLLstSupRemover.begin(); It != ItUltimoPonto ; It++)
    {
      RemovePontoInseridoUsuario((PegaPontoMaisProximo(*It, 1e-4)));
    }
  }

  void RemovePontoInseridoUsuario(ItSSuperficie ITSPR)
  {
    if(ITSPR != pSuperficieAtual->end())
    {
      pairititmmapSSuperficie itpairmmapSSuperficie(MMapSuperficieNomes.equal_range(ITSPR->PCentral.Nome));

      itmmapItSSuperficie itMMapAtual(itpairmmapSSuperficie.first);
      
      if(itMMapAtual == MMapSuperficieNomes.end()) exit(0);

      MMapSuperficieNomes.erase(itMMapAtual);
      SetPontosInseridos.erase(ITSPR->PegaPCentral());
      pSuperficieAtual->erase(ITSPR) ;
    }
  }

  void RemovePonto(const Ponto& pPonto)
  {
    ItSSuperficie it(pSuperficieAtual->find(pPonto));

    if (it == pSuperficieAtual->end()) return;

    //--- O Mapa de nomes é usado

    pairititmmapSSuperficie itpairmmapSSuperficie(MMapSuperficieNomes.equal_range(it->PCentral.Nome));

    itmmapItSSuperficie itMMapAtual(itpairmmapSSuperficie.first);

    /*   //--- Dando erro - Remover so o último??? que diferença faz???remove so o primeiro então
    for (itMMapAtual = itpairmmapSSuperficie.first ;  
         itMMapAtual != itpairmmapSSuperficie.second && *itMMapAtual->second == *it ; 
         ++itMMapAtual) ;
    */
  
    if (itMMapAtual != MMapSuperficieNomes.end())
    {
      MMapSuperficieNomes.erase(itMMapAtual);
    }
    else
    {
      // exit(0);    //--- Verificar porque está ocorrendo
      //--- Este mapa serve apenas para facilitar a achar o ponto pelo nome (mais rápido) talvez tirar
      //--- Seja a solução porque ele não ajuda tanto assim
    }

    //------------------------------------------------------------

    //MMapSuperficieNomes.erase(itMMapAtual);
    SetPontosInseridos.erase(pPonto);
    pSuperficieAtual->erase(pPonto) ;
  }

  int LePontos(char* pBuffer);
  int Serialize(const CString& NomeProjeto,int Tipo,bool SoRestricoes=false,bool SoCurvasNivel=false,bool GravarPontosEditados=true,bool SoBordos=false,bool ETrecho=false,bool LerRestricoesAcad=false);
  int Seccionar(const Ponto& P1, const Ponto& P2,const Ponto& CoorEstaca,setPontos& SSetxSecao,double LadoEsq_2,bool bTodas,double Inicial,bool InserirPontas,bool TrimLeft,bool Talvegue);
  int SeccionarTerrap(const Ponto& P1, const Ponto& P2, const Ponto& CoorEstaca, setPontos& SSetxSecao, double LadoEsq_2, SSuperficie* SuperfTerrap);
  ItSSuperficie PegaPontoMaisProximo(CDeltaSuperficie& DSuperficie,double ToleranciaX,double ToleranciaY);
  LLDeltaSup& PegaListaRestricoes() { return ListaRestricoes;};
  LLDeltaSup& PegaListaBordos() { return ListaBordos;};
  int DefineFronteiraOuPoligono(int Tipo = TIPOFRONTEIRA,bool Terraplanagem = false,bool ProjGeoRef=false,bool ProjACAD=false);
  bool DefineBordos();
  bool RotacionaPlano(double VUx,double VUy,double Angulo);
  void ExcluiDeltaSuperficie(ItSSuperficie itDeltaSupAEcluir);
  void MoveDeltaSuperficie(ItSSuperficie itDeltaSupAnterior,const CDeltaSuperficie& DeltaSupAtual);
  ItSSuperficie PegaPontoMaisProximo(Ponto PontoAPegar,double Tolerancia);
  bool InterpolarCotas(ItSSuperficie DeltaSupInicial, ItSSuperficie DeltaSupFinal, CProgressCtrl& Prog, SSetPontos& PontosProblema, double Equidis = 1.0, int TipoSuperf = 0);
  void Ligacn(ItSSuperficie itSupPosterior,ItSSuperficie itSupAtual,itsetPontosS itPonInter,bool Fechada=false);
  bool LigaCurvasFechadas(int TipoSuperf = 0);
  bool LigaCNAbertas(int TipoSuperf = 0);
  void AtribuiEquidisCN( double Valor) {EquidisCN = Valor;};
  double PegaEquidisCN() { return EquidisCN;};
  void AcertaSenoCoseno() { Seno = sin(AnguloDeRotacao) ; Coseno = cos(AnguloDeRotacao);}; 
  unsigned int VerfAciTopografico(ItCLLDeltaSup LDRestricaoAtual,bool Remover = false);
  unsigned int VerfEsboco(ItCLLDeltaSup LDEsbocoAtual, bool Remover);
  void RemoveListaDoMapa(ItCLLDeltaSup itLAtual,int Tipo=CAciTopografico::NENHUM_ACITOPOG,bool RemoverDasuperficie = true,bool Forcar = false);
  void InsereListaNoMapa(ItCLLDeltaSup itLAtual,int Tipo) { MMapItLLDeltaSup.insert(pairmmapitLLDeltaSup(Tipo,itLAtual)); }
  void InsereListaNoMapaEsbocos(ItCLLDeltaSup itLAtual, int Tipo) { MMapItLLDeltaSup.insert(pairmmapitLLDeltaSup(Tipo, itLAtual)); }
  bool VerificaIntersecaoAcidentesTopog(const Ponto& P1,
  const Ponto& P2,
  Ponto& PontoIntersecao,
  int Tipo = CAciTopografico::NENHUM_ACITOPOG,bool Sugerir = false,bool PularInseridos = true,itmmapitLLDeltaSup* ItInicio= nullptr, ItLLDeltaSup* pItLLRes= nullptr, ItLDeltaSup* pItP1=nullptr);
  void RemoveListasDoTipo(unsigned int Tipo) {RemoveRestricoesTipo(Tipo) ; MMapItLLDeltaSup.erase(Tipo);}
  double PegaArea() { return Area;}
  double PegaPerimetro() { return Perimetro; }
  bool VerfAciTopogForaSuperficie(SSetPontos& PontosProblema,ItCLLDeltaSup* ItMarcada=NULL);
  void RemoveRestricoesTipo(unsigned int Tipo) 
  {
    paitititmmapitLLDeltaSup PairItIt(MMapItLLDeltaSup.equal_range(Tipo));

    while(PairItIt.first != PairItIt.second)
    {
      ListaRestricoes.erase(PairItIt.first->second);
      ++PairItIt.first;
    }
  }
  void CriaListaBordos();  //--- Para compatibilidade com versões anteriores
  static double CalcAreaReta(const Ponto& P1,const Ponto& P2) {return ((P1.y + P2.y)/ 2.0 * (P2.x - P1.x));};
  bool VerfArestaForaSuperficie(const Ponto& P1,const Ponto& P2);
  bool VerfAciTopogCruzFronteira(ItCLLDeltaSup& ItLLDeltaSupMarcada,SSetPontos& PontosProblema); 
  const SSetPontos& SetPonInseridos() const { return SetPontosInseridos;}; 
  const SSetPontos& SetPonDesabilitados() const { return SetPontosDesabilitados;}; 
  const SSetPontos& SetPonInsSecoes() const { return SetPontosInsSecoes;};
  //const SSetPontos& SetPonCNDGNSemCota() const { return SetPontosCNDGNSemCota;};
  bool InserePontoInseridoUsuario(const Ponto& PontoAInserir) 
  { 
    return SetPontosInseridos.insert(PontoAInserir).second;
  };
  bool MudaTipoAciTopografico(ItCLLDeltaSup LDRestricaoAtual,unsigned int TipoAtual,unsigned int NovoTipo);  
  void CriaSetSegmentosRestricoes(int Tipo = CAciTopografico::NENHUM_ACITOPOG);
  bool InserirPontoIntersecao(const Ponto& P1,const Ponto& P2,ItCLLDeltaSup ItLDRestricao1,ItCLDeltaSup ItRestricao2P1,ItCLDeltaSup ItRestricao2P2,Ponto& Intersecao,ItLLDeltaSup* pItLDRestricao1=nullptr, ItLDeltaSup* pItP1=nullptr);
  bool VerfPontoRestricao(const Ponto& Ponto,ItLLDeltaSup& pItLLDS,ItLDeltaSup& itPonto);
  ItSSuperficie Find(Ponto PontoProcurar, double MenorDistancia);

  bool VerfPontosAcidentesTopograficos(SSetPontos& PontosProblema) 
  {
    PontosProblema.clear();

    for(ItLLDeltaSup ItLRestricao = ListaRestricoes.begin()	;	ItLRestricao !=	PegaListaRestricoes().end() ; ++ItLRestricao)
    {
      for (register ItLDeltaSup ItLDeltaSupAtual = ItLRestricao->begin() ; ItLDeltaSupAtual != ItLRestricao->end() ; ItLDeltaSupAtual++)
      {
        if(pSuperficieAtual->find(ItLDeltaSupAtual->PegaPCentral()) == pSuperficieAtual->end()) PontosProblema.insert(ItLDeltaSupAtual->PegaPCentral());
      }
    }

    return PontosProblema.size() == 0;
  }

  ItSSuperficie PegaPontoPorNome(const CString& Nome) 
  {
    pairititmmapSSuperficie itpairmmapSSuperficie(MMapSuperficieNomes.equal_range(Nome));
    
    if(itpairmmapSSuperficie.first != MMapSuperficieNomes.end()) return pSuperficieAtual->find(*itpairmmapSSuperficie.first->second);
    
    return pSuperficieAtual->end();
  }

  bool CSuperficie::VerfInsercaoSgmento(Ponto P1,Ponto P2,int Tipo);

  void CriaSetPontosFronteira()
  {
    SetPontosFronteira.clear();
    
    paitititmmapitLLDeltaSup PairItIt(MMapItLLDeltaSup.equal_range(CAciTopografico::FRONTEIRA));

    for(register ItCLDeltaSup itCLDS = PairItIt.first->second->begin() ; itCLDS != PairItIt.first->second->end() ; ++itCLDS)
    {
      SetPontosFronteira.insert(itCLDS->PCentral);
    }
  }

  bool EFronteira(const Ponto& PontoProcurar) const
  {
  //--- 01/05/2022 @VERFIFICAR PRIOR 0
  //--- Ao importar a traingulação da topografia começou não sei pq a dar diferencas dos valores na 9/10a casa decimal
  //--- estas diderenças fazem que o ponto não seja encontrado no set. não se porque isso começou a ocorrer e não ocorre nos 
  //--- outro casos.
  //--- Procedimento para ocasionar o erro:
  //--- Importar a malha triangulada da topografia
  //--- Na superfície definir a fronteira
  //--- Solicitar o cálculo da superfície através da triangulação de Delaunay ou Delaunay-Gabriel
  //--- Vai ocorrer o erro de pontos fora da superficie, nas verdade estes pontos estão na fronteira e pertencem a 
  //--- superficie. Na comparação para verificar se o ponto é da superficie é feita a verificação se o 
  //--- ponto pertence a fronteira, se pertence retorna true ( O ponto pertence a superficie)
  //--- Mas no find do set da fronteira não esta achando o ponto por casa destas diferenças
  //--- A solução por enquando é este if.

    auto IgualouProximo(SetPontosFronteira.lower_bound(PontoProcurar));

    if(IgualouProximo != SetPontosFronteira.end())
    { 
      double Dx(fabs(IgualouProximo->x - PontoProcurar.x)),
             Dy(fabs(IgualouProximo->y - PontoProcurar.y));

      if(Dx < 1e-6 && Dy < 1e-6) return true;

      IgualouProximo--;

      if (IgualouProximo != SetPontosFronteira.end())
      {
        Dx = fabs(IgualouProximo->x - PontoProcurar.x),
        Dy = fabs(IgualouProximo->y - PontoProcurar.y);

        if (Dx < 1e-6 && Dy < 1e-6) return true;
      }

      return false;
    }

    IgualouProximo--;

    if (IgualouProximo != SetPontosFronteira.end())
    {
      double Dx(fabs(IgualouProximo->x - PontoProcurar.x)),
             Dy(fabs(IgualouProximo->y - PontoProcurar.y));

      if (Dx < 1e-6 && Dy < 1e-6) return true;
    }

    return false;


    //--- Era so isso....
    //return SetPontosFronteira.find(PontoProcurar) != SetPontosFronteira.end(); 
  }

  void LimpaSetPontosFronteira() {SetPontosFronteira.clear();}
  void MontaChaves();
  void MudaArea(double NovaArea) { Area = NovaArea; };
  itmmapitLLDeltaSup PegaFronteira() {return MMapItLLDeltaSup.find(CAciTopografico::FRONTEIRA);}
  bool  TemFronteira() {return MMapItLLDeltaSup.find(CAciTopografico::FRONTEIRA) != MMapItLLDeltaSup.end();}
  void PegaListaFronteira(LDeltaSup& LFronteira,int TipoSuperf = 0)
  {
    LFronteira.clear();

    //itmmapitLLDeltaSup itLLDS = MMapItLLDeltaSup.find(TipoSuperf == 0 ? CAciTopografico::FRONTEIRA : CAciTopografico::FRONTEIRA);
   itmmapitLLDeltaSup itLLDS = MMapItLLDeltaSup.find(TipoSuperf == 0 ? CAciTopografico::FRONTEIRA : CAciTopografico::FRONTEIRA_TERRAP);

    if (itLLDS != MMapItLLDeltaSup.end())
    {
      LFronteira = *itLLDS->second;
    }
  }

  void* PegaPontFronteiraSuperf()
  {
    itmmapitLLDeltaSup itLLDS = MMapItLLDeltaSup.find( CAciTopografico::FRONTEIRA);

     return (itLLDS != MMapItLLDeltaSup.end()) ? (void*) &*itLLDS->second : NULL;
  }
  
  static void CSuperficie::AcertaCamposLer(CString& Formato, std::string& Nome, std::string& x, std::string& y, std::string& z, std::string& Desc)
  {
    std::string CopiaNome(Nome), Copiax(x), Copiay(y), Copiaz(z), CopiaDesc(Desc);
    std::string* pCampos[5] = { &CopiaNome,&Copiax,&Copiay,&Copiaz,&CopiaDesc };

    for (int C = 0; C < 9; C++, C++)
    {
      int i(C / 2);

      switch (Formato[C])
      {
      case 'n': Nome = *pCampos[i]; break;
      case 'E': x = *pCampos[i]; break;
      case 'N': y = *pCampos[i]; break;
      case 'C': z = *pCampos[i]; break;
      case 'O': Desc = *pCampos[i]; break;
      default:ASSERT(0);
      }
    }
  }

  void LimpaRestricoeseBordos()
  {
    ListaRestricoes.clear();
    MMapItLLDeltaSup.clear();
    ListaBordos.clear();
  }

  void InverteFronteira(int TipoFronteira=CAciTopografico::FRONTEIRA_TERRAP);
  bool InverteuFronteira() { return bInverteuFronteira; }
  
  tylstBacias& PegaBacias()  { return  BaciasHidrograficas; }

  ittylstBacias ProcuraBacia(ItCLLDeltaSup Espigao)
  {
    for (ittylstBacias it = BaciasHidrograficas.begin(); it != BaciasHidrograficas.end(); it++)
    {
      //--- Procura pelo nome da bacia p primeiro ponto do espigão

      if(Espigao->begin()->PCentral.Descricao.Find(it->PegaDadosBacia().NomeBacia.c_str())!= -1)
      {
        return it;
      }
    }

    return BaciasHidrograficas.end();
  }

  ittylstBacias ProcuraTalvegueBacia(const Ponto& pPonto)
  {
    for (ittylstBacias it = BaciasHidrograficas.begin(); it != BaciasHidrograficas.end(); it++)
    {
      //--- Procura pelo ponto nos talvegues da bacia

      if(it->VerfPontoTalvegue(pPonto))
      {
        return it;
      }
    }

    return BaciasHidrograficas.end();
  }

  CBacia* AchaBaciaTalvPrincipal(const LDeltaSup& TalvPrincipal)
  {
    for (auto& BaciaAtual : BaciasHidrograficas)
    {
      if (BaciaAtual.TemTalveguePrincipal() && *BaciaAtual.PegaItTalveguePrincipal()->begin() == *TalvPrincipal.begin())
      {
        return &BaciaAtual;
      }
    }

    return nullptr;
  }

  tymapEquipDrenagem& PegaObrasDrenagem() { return EquipamentosDrenagem;}

  // Implementation
//public:
  virtual ~CSuperficie();
  double CalculaArea();
  double CalculaPerimetro();
  int SerializarEquipDrenagem(int Tipo, const std::string& NomeProjBase);

  bool MudaXYZPontoAciTopog(ItCLLDeltaSup LAciTopog,Ponto& PAtual,Ponto& PAnterior)
  {
    if (LAciTopog != PegaListaRestricoes().end())
    {
      ItCLDeltaSup ItLPontoAtual(LAciTopog->begin());
      
      while (ItLPontoAtual != LAciTopog->end() && *ItLPontoAtual != PAnterior) 
      {
        ++ItLPontoAtual;
      }

      if (ItLPontoAtual != LAciTopog->end())
      {
        Ponto& pPonto(const_cast<Ponto&>(ItLPontoAtual->PCentral));

        pPonto.x = PAtual.x;
        pPonto.y = PAtual.y;
        pPonto.z = PAtual.z;

        return true;
      }
    }

    return false;
  }
 
  bool DeslocaXYAciTopog(ItCLLDeltaSup LAciTopog, double Dx, double Dy)
  {
    if (LAciTopog != PegaListaRestricoes().end())
    {
      ItCLDeltaSup ItLPontoAtual(LAciTopog->begin());

      while (ItLPontoAtual != LAciTopog->end())
      {
        Ponto& pPonto(const_cast<Ponto&>(ItLPontoAtual->PCentral));

        pPonto.x += Dx;
        pPonto.y += Dy;

        ++ItLPontoAtual;
      }
    
      return true;
    }

    return false;
  }

  void SeccionaPelaCN(const Ponto& PEsq, const Ponto& PCentral, const Ponto& PFim, setPontos& SetSecao,bool EixoX,std::ofstream& ArqLog);
  setPontos& PegaPontosSoltosMapa() { return SetPontosSoltosDGN; };
  double Declinacao(double NovaDeclinacao=INFINITO) 
  { 
    if (NovaDeclinacao != INFINITO) 
    { 
      double DecAnterior(douDeclinacao);
      douDeclinacao = NovaDeclinacao;
      return DecAnterior;
    }
    return douDeclinacao;
  }

  void LimpaAngRotacao() 
  { 
    AngRotacaoBordos =AnguloDeRotacao = 0.0;
    AngRotacaoTopInt = 0;
  }

  void CriaSetCurvasNivel()
  {
    bool Pegar(1);

    SetItPontosCN.clear();
    //--- Este set é usado apenas quando o mouse passa em cima da CN para mostrar o ponto com a cota
    //--- Como a CN tem muitos pontos a metade é o bastante

    for (ItLLPontos itLLCN = LPontosCN.begin(); itLLCN != LPontosCN.end(); itLLCN++)
    {
      for (register ItLPontos itLCN = itLLCN->begin(); itLCN != itLLCN->end(); itLCN++) 
      {
        if(Pegar)  SetItPontosCN.insert(itLCN);
        Pegar = !Pegar;
      }
    }
  }

  bool LeArqPontosSoltosDGN();

  SetItLPontos& SetItPontosCNs() {return SetItPontosCN;};

  void RemoveItSetCN(const ItLPontos ItRemover)
  {
    SetItPontosCN.erase(ItRemover);
  }

  bool PontoXYIgual(const Ponto& P1, const  Ponto& P2, double Precisao = 1e-3)
  {
    return (fabs(P1.x - P2.x) < Precisao && fabs(P1.y - P2.y) < Precisao);
  }
  void AjustaPontosInseridosUsuario();    
  void PegaRestricoesNoPonto(const Ponto& pProcurar, lstItCLLDeltaSup& pLstRestricoesNoPonto);
  int SerializarBacias(int Tipo, const std::string& NomeProjBase);
  void IncluiTalveguesSISSuperficie();
  LPontos& PegaBaciasImportadas() { return BaciasImportadas; };
  LPontos& PegaTalveguesImportados() { return TalveguesImportados; };
  
  bool VerfBaciaImportada(Ponto& PIniBacia)
  {
    for (Ponto& PAtual : BaciasImportadas)
    {
      if (fabs(PAtual.x - PIniBacia.x) < 1e-9 &&
          fabs(PAtual.y - PIniBacia.y) < 1e-9)
      {
        PIniBacia.Descricao = PAtual.Descricao;
        return true;
      }
    }
    return false;
  }

  bool VerfTalvegueImportado(Ponto& PIniTalvegue, const Ponto& PFimTalvegue)
  {
    for (Ponto& PAtual : TalveguesImportados)
    {
      if (fabs(PAtual.x - PIniTalvegue.x) < 1e-9 &&
          fabs(PAtual.y - PIniTalvegue.y) < 1e-9)
      {
        if(fabs(PAtual.i - PFimTalvegue.x) < 1e-9 &&
           fabs(PAtual.fi - PFimTalvegue.y) < 1e-9)
        {
          PIniTalvegue.Descricao = PAtual.Descricao;
          return true;
        }
      }
    }
    return false;
  }

  bool VerfArqTalvImportado(const std::string& NomeArquivo)
  {
    std::string s2(NomeArquivo);
    std::transform(s2.begin(), s2.end(), s2.begin(), [](unsigned char c) { return std::toupper(c); });

    for (Ponto& PAtual : TalveguesImportados)
    {
      std::string s1(PAtual.Descricao);
      std::transform(s1.begin(), s1.end(), s1.begin(),[](unsigned char c) { return std::toupper(c);});

      if(PAtual.Descricao.Find(NomeArquivo.c_str()) != -1) return true;
    }

    return false;
  }

  void SerializaBaciasImportadas(int Tipo, const std::string& NomeProjBase);
  void SerializaTalveguesImportados(int Tipo, const std::string& NomeProjBase);
  void NormalizaTalvegues();
  int LigaTalveguesAlinhados();
  bool DefineTalveguePrincipal(const Ponto& pFoz,const Ponto& pNascente);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPERFICIE_H__F030F6A2_D02F_11D7_BE61_444553540000__INCLUDED_)
