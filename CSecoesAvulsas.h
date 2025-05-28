typedef std::map<Ponto,POSITION> mapSecoesAvulsas;
typedef mapSecoesAvulsas::iterator itmapSecoesAvulsas;
typedef std::pair<Ponto,POSITION> pairPontoSecAvulsa;

#define IGUAL 0

class CSecoesAvulsas ///: public CArqSecoesAvulsas
{
	POSITION Atual;                //--- Ponteiro para o Atual da lista por lado. 
	CString NomeArquivo;           //--- Nome do arquivo.
	CList<CSecaoAvulsa, CSecaoAvulsa&>  LSecoesAvulsas;
  mapSecoesAvulsas MapSecoesAvulsasP1;    

protected:

	int Compara(class CSecaoAvulsa &No1, class CSecaoAvulsa &No2);  //--- compara dois nós.
	//--- Posiciona o ponteiro da lista na seção igual ou mais 
	//--- proxima no sentido indicado
	int PosicionaNoMaisProximo(std::string& Nome, int Direcao = FALSE);

public:

	CList<CSecaoAvulsa, CSecaoAvulsa&>& PegaLSecoesAvulsas() { return LSecoesAvulsas; };  //--- tem q ser const acertar

	CSecoesAvulsas(CString& NomeArquivo);       //--- Construtor
	void InsereOrdenada(CSecaoAvulsa &No, int INSERIRIGUAL = FALSE); //--- Insersão ordenada na lista.
	bool Consiste();
	void RotacionaPlano(double Sin, double Cos, double Angulo);
	bool SerializeSecoesCalculadas(const std::string& NomeArquivo, int Operacao);
//	void InsereOrdenadaArq(class CSecaoAvulsa &No, int INSERIRIGUAL);
	void Serialize(const std::string& NomeProjeto, int Operacao);
	// void PassaSecoesParaMap();
	const itmapSecoesAvulsas End() { return MapSecoesAvulsasP1.end(); };

	const itmapSecoesAvulsas PegaSecaoNoMapa(const Ponto& PInicial)
	{
		return MapSecoesAvulsasP1.find(PInicial);
	};

	const itmapSecoesAvulsas UpperBound(const Ponto& PInicial)
	{
		return MapSecoesAvulsasP1.upper_bound(PInicial);
	}

	const itmapSecoesAvulsas LowerBound(const Ponto& PInicial)
	{
		return MapSecoesAvulsasP1.lower_bound(PInicial);
	}

	void RemoveSecaoAvulsa(const CSecaoAvulsa& SecaoAvulsa)
	{
		if (PosicionaNoMaisProximo(std::string(SecaoAvulsa.Nome)) == IGUAL)
		{
			LSecoesAvulsas.RemoveAt(Atual);
		}

		itmapSecoesAvulsas itSecAvulsa(MapSecoesAvulsasP1.find(SecaoAvulsa.PInicial));

		if(itSecAvulsa != MapSecoesAvulsasP1.end()) MapSecoesAvulsasP1.erase(MapSecoesAvulsasP1.find(SecaoAvulsa.PInicial));
	}

  void RemoveSecaoAvulsa(POSITION Pos)
	{
      Ponto PInicial(LSecoesAvulsas.GetAt(Pos).PInicial);

      MapSecoesAvulsasP1.erase(PInicial);
      LSecoesAvulsas.RemoveAt(Pos);
	}

	bool VerfPontoSecaoAvulsa(const Ponto& PSobMouse, double Toleranciax, itmapSecoesAvulsas& pItPerfMaisPerto)
	{
		bool PegouPonto(false);
		Ponto PAnterior(PSobMouse), PPosterior(PSobMouse);

		PAnterior.x -= Toleranciax;
		PPosterior.x += Toleranciax;

		itmapSecoesAvulsas itMSecaoAvAnterior(UpperBound(&PAnterior));
		itmapSecoesAvulsas itMSecaoAvPosterior(UpperBound(&PPosterior));
		itmapSecoesAvulsas itSecAvMaisPerto(MapSecoesAvulsasP1.end());
		itmapSecoesAvulsas itPonSecAvAnterior(MapSecoesAvulsasP1.end());    //---- Static não esta funcionando quando abre o desenho de novo     

		double MenorDistancia(INFINITO), DistanciaAtual;

    pItPerfMaisPerto = MapSecoesAvulsasP1.end();

		for (itmapSecoesAvulsas ItMapSecAvAtual = itMSecaoAvAnterior; ItMapSecAvAtual != itMSecaoAvPosterior; ItMapSecAvAtual++)
		{
			DistanciaAtual = pow((*ItMapSecAvAtual).first.x - PSobMouse.x, 2) + pow((*ItMapSecAvAtual).first.y - PSobMouse.y, 2);

			if (DistanciaAtual < MenorDistancia)
			{
				MenorDistancia = DistanciaAtual;
				itSecAvMaisPerto = pItPerfMaisPerto = ItMapSecAvAtual;
				PegouPonto = true;
			}
		}

		return PegouPonto;
	}

  void RemoveSecaoAvulsaMapa(itmapSecoesAvulsas ItSecRemover)
  {
    //---Remove Da lista

   CSecaoAvulsa& SecAvAtual(LSecoesAvulsas.GetAt(ItSecRemover->second));

    if (PosicionaNoMaisProximo(SecAvAtual.Nome) == IGUAL)
    {
      LSecoesAvulsas.RemoveAt(ItSecRemover->second);
    }

    MapSecoesAvulsasP1.erase(ItSecRemover);
  }
};