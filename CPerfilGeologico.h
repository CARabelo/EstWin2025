struct ltEstacaxy 
{ 
  bool operator()(const CEstaca& e1, const CEstaca& e2) const 
  { 
    double Deltax( e1.EstVirtual - e2.EstVirtual), Deltay( e1.Cota - e2.Cota);

    return Deltax < -1e-5 ?  true : Deltax > 1e-5 ? false :
           Deltay < -1e-5 ? true : false;
  };
};

typedef std::set<CEstaca,ltEstacaxy> tyPerfilTipoSolo;
typedef tyPerfilTipoSolo::iterator ittyPerfilTipoSolo;
typedef tyPerfilTipoSolo::const_iterator itctyPerfilTipoSolo;
typedef std::pair<ittyPerfilTipoSolo,ittyPerfilTipoSolo> typairitPerfilTipoSolo;          //--- Cotas calculadas no perfil
typedef std::pair<ittyPerfilTipoSolo,double> typaiitPerfildouble;                         //--- Cotas calculadas no perfil

struct ltPairitPerfildouble 
{ 
  bool operator()(const typaiitPerfildouble& it1, const typaiitPerfildouble& it2) const 
  { 
    return it1.second < it2.second;
  };
};

typedef std::set<typaiitPerfildouble,ltPairitPerfildouble> tysettypaiitPerfildouble;            //--- Set de cotas calculadas no perfil       
typedef tysettypaiitPerfildouble::iterator ittysettypaiitPerfildouble;   

class CPerfilTipoSolo
{
  std::string TipoSoloOriginal;
  tyPerfilTipoSolo PerfilTipoSolo;
  CEstaca PontoInicial;
  int Traco;
  COLORREF Cor;

public:

  double CotaNoPerfil;                          //--- Temporário 
  
  CPerfilTipoSolo(const std::string& TipSolo="Desconhecido",const CEstaca& EstacaIni=CEstaca(0.0),int pTraco=0,COLORREF pCOR=255):TipoSoloOriginal(TipSolo),PontoInicial(EstacaIni),Traco(pTraco),Cor(pCOR)
  {  
    //--- o toupper não capitaliza letras acentuadas...
    /*
    CString Temp(TipoSoloChave.c_str());
    Temp.MakeUpper();
    TipoSoloChave = Temp.GetBuffer();
    */

    //std::transform(TipoSoloChave.begin(),TipoSoloChave.end(),TipoSoloChave.begin(), ::toupper);
  };

  const std::string& PegaTipoSolo() const {return TipoSoloOriginal;};
  COLORREF PegaCor() const {return Cor;}; 
  int PegaTraco() const { return Traco;};
  void TrocaCor(COLORREF NovaCor) { Cor = NovaCor;}; 
  void TrocaTraco(int NovoTraco) { Traco = NovoTraco;};
  void TrocaTipoSolo(const std::string& NovoTipoSolo) { TipoSoloOriginal = NovoTipoSolo;};
  bool IncluiPonto(const CEstaca& Estaca){ return PerfilTipoSolo.insert(Estaca).second;};
  const tyPerfilTipoSolo& PegaPerfil() const {return PerfilTipoSolo;};
  size_t RemovePonto(const CEstaca& Ponto) {return PerfilTipoSolo.erase(Ponto);};
  void RemoveTrecho(ittyPerfilTipoSolo itInicioTrecho,ittyPerfilTipoSolo itFimTrecho) {PerfilTipoSolo.erase(++itInicioTrecho,itFimTrecho);};

  double CalculaCota(CEstaca& Ponto) const 
  {
    if(PerfilTipoSolo.size() < 2) return false;
    ittyPerfilTipoSolo itUB(PerfilTipoSolo.upper_bound(Ponto)),itLB(PerfilTipoSolo.lower_bound(Ponto));
    if (itUB == PerfilTipoSolo.begin()) return INFINITO;

    --itLB;

    return  ((itUB->Cota - itLB->Cota) / (itUB->EstVirtual - itLB->EstVirtual))*(Ponto.EstVirtual - itLB->EstVirtual) + itLB->Cota; 
  };

  bool operator<(const CPerfilTipoSolo& RHS) const 
  { 
    int CompareTipo(TipoSoloOriginal.compare(RHS.TipoSoloOriginal));

    return CompareTipo < 0 ? true : CompareTipo > 0 ? false:
           PontoInicial.EstVirtual < RHS.PontoInicial.EstVirtual ? true : 
           PontoInicial.EstVirtual > RHS.PontoInicial.EstVirtual ? false :
           PontoInicial.Cota < RHS.PontoInicial.Cota ? true : false;
  };

  bool operator==(const CPerfilTipoSolo& RHS) const 
  { 
    double Deltax(fabs(PontoInicial.EstVirtual - RHS.PontoInicial.EstVirtual)),
           DeltaCota(fabs(PontoInicial.Cota - RHS.PontoInicial.Cota)) ;

    return TipoSoloOriginal.compare(RHS.TipoSoloOriginal) == 0 && Deltax < 1e-5 && DeltaCota < 1e-5;
  };

  const CEstaca& PegaPontoInicial() const { return PontoInicial;};

  CArchive& operator>>(CArchive& ar) 
  {
    ar << TipoSoloOriginal;
    PontoInicial.operator>>(ar);
    ar << Traco << Cor;

    ar << PerfilTipoSolo.size();

    for (ittyPerfilTipoSolo it = PerfilTipoSolo.begin() ; it != PerfilTipoSolo.end() ; ++it)
    {
      const_cast<CEstaca*>(&*it)->operator>>(ar);
    }

    return ar;
  };

  CArchive& operator<<(CArchive& ar) 
  {
    ar >> TipoSoloOriginal;
    PontoInicial.operator<<(ar);
    ar >> Traco >> Cor;

    size_t TamPerfil(0);
    CEstaca Ponto;

    for(ar >> TamPerfil ; TamPerfil > 0 ; --TamPerfil)
    {
      Ponto.operator<<(ar);
      PerfilTipoSolo.insert(Ponto);
    }

    return ar;
  };

  void ReleDados();
};

typedef std::set<CPerfilTipoSolo> tysetCPerfilTipoSolo;
typedef tysetCPerfilTipoSolo::iterator ittysetCPerfilTipoSolo;
typedef tysetCPerfilTipoSolo::reverse_iterator rittysetCPerfilTipoSolo;
typedef tysetCPerfilTipoSolo::const_iterator itctysetCPerfilTipoSolo;
typedef std::pair<ittysetCPerfilTipoSolo,bool> pairtysetCPerfilTipoSoloItBool;

typedef std::pair<ittyPerfilTipoSolo*,ittysetCPerfilTipoSolo> typairittyPerfilTSrsetPTS;

typedef std::set<CEstaca> tysetEstaca;
typedef tysetEstaca::iterator ittysetEstaca;
typedef std::pair<ittysetEstaca,ittysetEstaca> typairitittysetEstaca;

class CDadosPerfisGeologicos
{
  std::string TipoSolo;
  double Estaca,Area[2],CV[3],DeltahPrimitivo,Volumes[2];
  Perfil PerfilGeologico,PerfilOffSets;     //-- Os offsets são apenas 2 pontos mas usa o perfil para as funções de uso das listas 

public:

  CDadosPerfisGeologicos(const CDadosPerfisGeologicos& RHS) : TipoSolo(RHS.TipoSolo),Estaca(RHS.Estaca),DeltahPrimitivo(RHS.DeltahPrimitivo),
                        PerfilOffSets(RHS.PerfilOffSets), PerfilGeologico(RHS.PerfilGeologico)
  {
    Area[0] = RHS.Area[0];
    Area[1] = RHS.Area[1];
    CV[0] = RHS.CV[0];
    CV[1] = RHS.CV[1];
    CV[2] = RHS.CV[2];
    Volumes[0] = RHS.Volumes[0];
    Volumes[1] = RHS.Volumes[1];
  };

  CDadosPerfisGeologicos(const std::string& pTipoSolo=std::string(""),double pEstaca=INFINITO,Ponto* pCVs=NULL,Ponto* pOffe=NULL,Ponto* pOffd=NULL,Perfil* PerfilGeol = NULL,double Deltah = -INFINITO) :
  TipoSolo(pTipoSolo),Estaca(pEstaca),DeltahPrimitivo(Deltah)
  {
    Area[0] = Area[1] = Volumes[0] = Volumes[1] = 0.0;

    if(pCVs != NULL)
    {
      CV[0] = pCVs->x;
      CV[1] = pCVs->y;
      CV[2] = pCVs->z;

      if(pOffe != NULL && pOffd !=NULL)
      {
        PerfilOffSets.LimpaTudo();
       // PerfilOffSets.Estaca = Estaca;
        PerfilOffSets.AddTail(*pOffe);
        PerfilOffSets.AddTail(*pOffd);
      }
    }
    if(PerfilGeol != NULL)
    {
      PerfilGeologico.Estaca = PerfilGeol->Estaca;

      for(register POSITION Atual = PerfilGeol->GetHeadPosition() ; Atual != NULL ; PerfilGeol->GetNext(Atual))
      {
        PerfilGeologico.AddTail(PerfilGeol->GetAt(Atual));
      }
    }
    else PerfilGeologico.Estaca = Estaca;
  }; 

  void Serialize(CArchive& ar)  
  {
    if(ar.IsStoring())
    {

      ar << TipoSolo ;
      ar << Estaca << Area[0] << Area[1] << CV[0] << CV[1] << CV[2] << DeltahPrimitivo << Volumes[0] << Volumes[1];
      PerfilOffSets.GetHead() >> ar;
      PerfilOffSets.GetTail() >> ar;

      PerfilGeologico.Serialize(ar);
    }
    else
    {
      ar >> TipoSolo >> Estaca >> Area[0] >> Area[1] >> CV[0] >> CV[1] >> CV[2] >> DeltahPrimitivo >> Volumes[0] >> Volumes[1];

      Ponto PontoOffSet;

      PontoOffSet << ar;
      PerfilOffSets.AddTail(PontoOffSet);

      PontoOffSet << ar;
      PerfilOffSets.AddTail(PontoOffSet);

      PerfilGeologico.Serialize(ar);
    }
  }

  const Perfil& PegaPerfilGeologico() const { return PerfilGeologico;}
  const double PegaEstaca() const {return Estaca;};
  const std::string& PegaNomeTipoSolo() const { return TipoSolo;};
  const Ponto& PegaOffSet(int Num) const { return Num == 0 ? PerfilOffSets.GetHead() : PerfilOffSets.GetTail();};
  double PegaCV(int Lado) const { return CV[Lado];};
  POSITION PosicionaPonto(double x,double y, double Toleranciax,double Toleranciay) {return PerfilGeologico.PosicionaPonto(x,y,Toleranciax,Toleranciay);};
  Ponto PegaPonto(POSITION Pos) { return PerfilGeologico.GetAt(Pos);};
  Ponto PegaPontoOffSet(int Lado) { return Lado == 0 ? PerfilOffSets.GetHead() : PerfilOffSets.GetTail();}
  Ponto PegaPontoOffSet(POSITION Pos) { return PerfilOffSets.GetAt(Pos);}
  void RemovePonto(POSITION Pos) { return PerfilGeologico.RemoveAt(Pos);};
  void RemovePontoOffSet(POSITION Pos) { return PerfilOffSets.RemoveAt(Pos);};
  double PegaDeltahPrimitivo() const { return DeltahPrimitivo;}; 
  double& PegaArea(int Num)  { return Area[Num];};
  double& PegaVolume(int Num) { return Volumes[Num];};
  double PegaValorArea(int Num) const { return Area[Num];};
  double PegaAltura() const {return DeltahPrimitivo;};

  void InserePonto(Ponto& PontoInserir)
  {
    register POSITION Pos(PerfilGeologico.GetHeadPosition());

    while(Pos && PerfilGeologico.GetAt(Pos).x < PontoInserir.x) PerfilGeologico.GetNext(Pos); 

    if(Pos)PerfilGeologico.InsertBefore(Pos,PontoInserir);
    else PerfilGeologico.AddTail(PontoInserir);
  }

  void AlteraPontoPerfil(const Ponto Local, double x,double y)
  {
    int Resp(PerfilGeologico.PosicionaNoMaisProximo(Local.x));

    if(Resp == 0)
    {
      double Dh = Local.y - y;
      Ponto PontoAtual = PerfilGeologico.PegaPontoAtual();
      PontoAtual.x = x;
      PontoAtual.y = PerfilGeologico.PegaPontoAtual().y - Local.y + y;
      PerfilGeologico.AlteraPonto(PontoAtual);
    }
  }

  void RemovePontoPerfil(double x)
  {
    register POSITION Pos(PerfilGeologico.GetHeadPosition());

    while(Pos && PerfilGeologico.GetAt(Pos).x < x) PerfilGeologico.GetNext(Pos); 

    if(Pos && fabs(PerfilGeologico.GetAt(Pos).x - x) < 0.0005)  PerfilGeologico.RemoveAt(Pos);
  }

  bool operator<(const CDadosPerfisGeologicos& RHS) const 
  {
    return Estaca < RHS.Estaca ? true :  
           Estaca > RHS.Estaca ? false : 
           TipoSolo.compare(RHS.TipoSolo) < 0 ? true : 
		       TipoSolo.compare(RHS.TipoSolo) > 0 ? false : 
           DeltahPrimitivo < RHS.DeltahPrimitivo ? true : false;
  }
};

typedef std::set<CDadosPerfisGeologicos> setCDadosPerfGeologicos;
typedef setCDadosPerfGeologicos::iterator itsetCDadosPerfGeologicos;

struct ltitsetCDadosPerfGeoDeltah
{      
  bool operator()(const itsetCDadosPerfGeologicos it1, const itsetCDadosPerfGeologicos it2) const 
  {
    return it1->PegaEstaca() < it2->PegaEstaca() ? true :  
           it1->PegaEstaca() > it2->PegaEstaca() ? false :  
           it1->PegaDeltahPrimitivo() > it2->PegaDeltahPrimitivo() ? true : false;
  }   
};

typedef std::set<itsetCDadosPerfGeologicos,ltitsetCDadosPerfGeoDeltah> setitsetCDadosPerfGeoDeltah;    //--- Por ordem de deltah, udsado na desenho DXF, no gabarito das cotas
typedef setitsetCDadosPerfGeoDeltah::iterator itsetitsetCDadosPerfGeoDeltah;

struct ltDadosPerfGeoTipo 
{ 
  bool operator()(const itsetCDadosPerfGeologicos& it1, const itsetCDadosPerfGeologicos& it2) const 
  { 
    return it1->PegaNomeTipoSolo().compare(it2->PegaNomeTipoSolo()) < 0 ? true :
           it1->PegaNomeTipoSolo().compare(it2->PegaNomeTipoSolo()) > 0 ? false :
           it1->PegaEstaca() < it2->PegaEstaca() ? true : false;
  };
};

typedef std::set<itsetCDadosPerfGeologicos,ltDadosPerfGeoTipo> setitCDadosPerfGeologicos;
typedef setitCDadosPerfGeologicos::iterator itsetitCDadosPerfGeologicos;

class CPerfilGeologico
{
  CFurosSondagem FurosSondagem;
  tysetCPerfilTipoSolo PerfisSolos;
  setCDadosPerfGeologicos SetDadosPerfis;
  setitsetCDadosPerfGeoDeltah SetitDadosPerfisDeltah;

public:

  CPerfilGeologico(const std::string& ProjetoAtual):FurosSondagem(ProjetoAtual){};
  ~CPerfilGeologico(void){};
  const tysetCPerfilTipoSolo& PegaPerfisSolos() const { return PerfisSolos;};
  const setCDadosPerfGeologicos& PegaSetSecaoPerfGeo() const { return SetDadosPerfis;};
  const setitsetCDadosPerfGeoDeltah& PegaSetItSecaoPerfGeoDeltah() const { return SetitDadosPerfisDeltah;};
  void LimpaSetDadosPerfis() {SetDadosPerfis.clear();};
  void LimpaPerfis() {PerfisSolos.clear();};

  pairtysetCPerfilTipoSoloItBool IncluiPerfilSolo(const std::string& TipSolo,CEstaca PontoInicial,int pTipoTraco,COLORREF pCor,bool TerrenoNatual=false) 
  { 
    PontoInicial.Descricao = TipSolo.c_str();   

    pairtysetCPerfilTipoSoloItBool Resp(PerfisSolos.insert(CPerfilTipoSolo(TipSolo,PontoInicial,pTipoTraco,pCor)));

    if(Resp.second == true)
    {
      if((const_cast<CPerfilTipoSolo*>(&*Resp.first))->IncluiPonto(PontoInicial) == false) 
      {
        PerfisSolos.erase(Resp.first);
        Resp.second = false;
      }
    }

    return Resp;
  }

  bool IncluiPontoPerfil(const std::string& TipSolo,const CEstaca& Ponto) 
  { 
    ittysetCPerfilTipoSolo it(PerfisSolos.find(CPerfilTipoSolo(TipSolo,Ponto.EstVirtual)));

    if(it != PerfisSolos.end()) return (const_cast<CPerfilTipoSolo*>(&*it))->IncluiPonto(Ponto);

    return false;
  };

  void CriaSetitDadosPerfisDeltah()
  {
    SetitDadosPerfisDeltah.clear();

     for(register itsetCDadosPerfGeologicos it = SetDadosPerfis.begin() ; it != SetDadosPerfis.end() ; ++it)
    {
      SetitDadosPerfisDeltah.insert(it);
    }
  }

  bool IncluiPontoPerfil(const CEstaca& Ponto,ittysetCPerfilTipoSolo itPerfilsolo) { return (const_cast<CPerfilTipoSolo*>(&*itPerfilsolo))->IncluiPonto(Ponto);};
  size_t RemovePontoPerfil(const CEstaca& Ponto,ittysetCPerfilTipoSolo it){return const_cast<CPerfilTipoSolo*>(&*it)->RemovePonto(Ponto);};
  const CFurosSondagem& PegaFurosSondagem() const { return FurosSondagem;};
  ittysetCPerfilTipoSolo PegaTipoSolo(const std::string& strTipoSolo) const {return PerfisSolos.find(CPerfilTipoSolo(strTipoSolo,INFINITO));};
  ittysetFurosSondagem PegaFuroMaisProximo(double Estaca,double Cota,double ToleranciaEstaca,double ToleranciaCota) { return FurosSondagem.PegaFuroMaisProximo(Estaca,ToleranciaEstaca,Cota,ToleranciaCota);};
  typairittyPerfilTSrsetPTS VerfPontoPerfilDesenho(const CEstaca& pEstaca,double Toleranciaxy_2,ittysetCPerfilTipoSolo* itPTSAtual = NULL,double Escalay=1.0) ;
  bool FundePerfis(ittysetCPerfilTipoSolo itPerfilTipoSolo,ittysetCPerfilTipoSolo PerfilaInserir);
  void ColocaTipoNoPerfil(const std::string& Tipo,ittysetCPerfilTipoSolo& itPerfil,int pTipoTraco,COLORREF pCor);
  void RemovePerfil(ittysetCPerfilTipoSolo itPerfil) { PerfisSolos.erase(itPerfil);};
  void RemovePerfil(CPerfilTipoSolo& PerfilTipoSolo) { PerfisSolos.erase(PerfilTipoSolo);};
  ittysetCPerfilTipoSolo VerfPontoPerfil(const CEstaca& pEstaca,double Toleranciax,double Toleranciay); 
  void PegaTrecho(ittysetCPerfilTipoSolo itPerfil,const CEstaca& Ponto,ittyPerfilTipoSolo* pitLB,ittyPerfilTipoSolo* pitUB) const;
  void CriaPerfisNosTrechos(ittysetEstaca itInicioTrecho,ittysetEstaca itFimTrecho,Ponto Deltas);
  bool Serialize(const std::string& NomeProjeto,int Tipo);
  bool PegaTrechoEntreFurosSondagem(CEstaca& Local,ittysetCPerfilTipoSolo itPerfil,ittyPerfilTipoSolo& itInicioTrecho,ittyPerfilTipoSolo& itFimTrecho);
  static const Ponto& PegaIniFimLimpeza(const Perfil& SecaoTerrenoPrimitivo,Perfil& SecaoPerfilGeologico,double hLimpeza);
  static void AjustaLimpeza(const Perfil& SecaoLimpeza,Perfil& SecaoPerfilGeologico);

  void InsereDadosNotaServico(CDadosPerfisGeologicos& DadosPerfGeol) 
  {
    SetDadosPerfis.erase(DadosPerfGeol);
    SetDadosPerfis.insert(DadosPerfGeol);
  };

  void LimpaDadosNotaServico() { SetDadosPerfis.clear() ;};
  void GravaDadosNotaServico(std::string& NomeProjeto);
  void LeDadosNotaServico(const std::string& NomeProjeto);
  void AlteraPontoPerfis(const Ponto& Local,double x,double y)
  {
    for(itsetCDadosPerfGeologicos it = SetDadosPerfis.begin() ; it != SetDadosPerfis.end() ; ++it)
    {
      const_cast<CDadosPerfisGeologicos*>(&*it)->AlteraPontoPerfil(Local,x,y);
    }
  }
  void ReleDados(const std::string& NomeArquivo);
  void CriaPerfisGeologicosNosTrechos(ittyPerfilTipoSolo itInicioTrecho,ittyPerfilTipoSolo itFimTrecho,Ponto Deltas);

  CArchive& operator>>(CArchive& ar) 
  {
    FurosSondagem >> ar;

    ar << PerfisSolos.size();

    for (ittysetCPerfilTipoSolo it = PerfisSolos.begin() ; it != PerfisSolos.end() ; ++it)
    {
      *(const_cast<CPerfilTipoSolo*>(&*it)) >> ar;
    }

    ar <<  SetDadosPerfis.size();                //--- Secao Transversal

    for(itsetCDadosPerfGeologicos it = SetDadosPerfis.begin() ; it != SetDadosPerfis.end() ; ++it)
    {
      const_cast<CDadosPerfisGeologicos*>(&*it)->Serialize(ar);
    }

    return ar;
  };

  CArchive& operator<<(CArchive& ar) 
  {
    size_t QtdPerfis(0);

    FurosSondagem << ar;

    for (ar >> QtdPerfis ; QtdPerfis > 0 ; --QtdPerfis)
    {
      CPerfilTipoSolo PTS;
      PTS << ar;
      PerfisSolos.insert(PTS);
    }

    for(ar >> QtdPerfis ; QtdPerfis > 0 ; --QtdPerfis)   //--- Seção transversal
    {
      CDadosPerfisGeologicos PerfilAtual;
      PerfilAtual.Serialize(ar);
      SetDadosPerfis.insert(PerfilAtual);
    }

    return ar;
  };

  void CalculaCotasNaEstaca(const CEstaca& Estaca,tysettypaiitPerfildouble& SetCotasPerfil) const
  {
    SetCotasPerfil.clear();

    for (ittysetCPerfilTipoSolo itPerfilSolo = PerfisSolos.begin() ; itPerfilSolo != PerfisSolos.end() ; ++itPerfilSolo)
    {
      double Cota(INFINITO);

      if(itPerfilSolo->PegaPerfil().size() > 2) 
      {
        ittyPerfilTipoSolo itLB(itPerfilSolo->PegaPerfil().lower_bound(Estaca)),itUB(itPerfilSolo->PegaPerfil().upper_bound(Estaca));

        if(itLB != itPerfilSolo->PegaPerfil().end() &&  itUB != itPerfilSolo->PegaPerfil().end())
        {
          if(itLB->EstVirtual > Estaca.EstVirtual) --itLB;

          if(itLB != itPerfilSolo->PegaPerfil().end())
          {
            Cota = ((*itLB).EstReal * (Estaca.EstVirtual - (*itLB).EstVirtual) + (*itLB).Cota);        //--- O i esta em EstReal
            const_cast<CEstaca*>(&*itLB)->Descricao = itPerfilSolo->PegaTipoSolo().c_str();            //--- Temporário
 	          SetCotasPerfil.insert(typaiitPerfildouble(itLB,Cota));
          }
        }
      }
    }
  }

  void Calculais()
  {
    //--- Calcula os is dos segmentos 

    for (ittysetCPerfilTipoSolo itPerfilSolo = PerfisSolos.begin() ; itPerfilSolo != PerfisSolos.end() ; ++itPerfilSolo)
    {
      if(itPerfilSolo->PegaPerfil().size() < 2) continue;

      register ittyPerfilTipoSolo itAnterior(itPerfilSolo->PegaPerfil().begin()),itPosterior(itAnterior);

      for(itPosterior++ ; itPosterior != itPerfilSolo->PegaPerfil().end() ; ++itAnterior)
      {
        const_cast<CEstaca*>(&(*itAnterior))->EstReal = (itPosterior->Cota - itAnterior->Cota) / (itPosterior->EstVirtual - itAnterior->EstVirtual); //--- O i esta em EstReal
				++itPosterior;
      }
    }
  }

  void CalculaVolumes()
  {
    itsetCDadosPerfGeologicos itPerfGeoAtual(PegaSetSecaoPerfGeo().begin());    
       
    if(itPerfGeoAtual != PegaSetSecaoPerfGeo().end())          //-- Se achou pega as secoes pela ordem de tipo de solo
    {
      std::string TipoSoloAtual(itPerfGeoAtual->PegaNomeTipoSolo());
      double AreaAnterior(const_cast<CDadosPerfisGeologicos*>(&*itPerfGeoAtual)->PegaArea(1)),SemiDistancia(0.0), //VolumeSecao(0.0),VolumeAcumulado(0.0),AreaAtual(0.0),
             EstacaAnterior(itPerfGeoAtual->PegaEstaca());

      while(itPerfGeoAtual != PegaSetSecaoPerfGeo().end() &&  itPerfGeoAtual->PegaNomeTipoSolo().compare(TipoSoloAtual) == 0) 
      {
        CDadosPerfisGeologicos* pPerfilAtual((const_cast<CDadosPerfisGeologicos*>(&*itPerfGeoAtual)));

        pPerfilAtual->PegaVolume(0) = (pPerfilAtual->PegaArea(1) +  AreaAnterior) * (EstacaAnterior + pPerfilAtual->PegaEstaca()) / 2.0;

        ++itPerfGeoAtual;
      }
    }
  }
};

