
typedef CList<CSecaoAvulsa, CSecaoAvulsa&> tyCLPerfisAvulsos;

class DesPerfilAvulsoView : public CView
{
	CSecoesAvulsas PerfisAvulsos;
//	CDPopUpPontos DPopUpPontos;

	bool ArrastandoPonto,
		Arrastando,
		TerminarArrasto,
		Alterou,
		EZoom,
		PermitirArrastar,
		CircularPontos;
	double DeltaYMetro,
		LimiteEsq,
		LimiteDir,
		LimiteSup,
		LimiteInf,
		Escala[2],          //--- Escala em pixels por metro; 
		EscalaDefault[2],   //--- Escalas padrão que permitem a visualização de todo o desenho.
		Deltas[2],          //--- Deslocamento nos eixos.
		AlturaVideo,        //--- Altura do video em metros.
		LarguraVideo,       //--- Largura do vídeo em metros. (Eixo x)
		Resolucao[2],       //--- Resolucao do vídeo em pixels.
		Toleranciax,        //--- Tolerancias para procurar o ponto na posição 
		Toleranciay,        //--- Atual do mouse.
		Toleranciaxy_2,     //--- tolerancias em perfis geologicos (não ficou bom, remover)
		RazaoHV;            //--- Razão entre as escalas H e V.
 CPoint InicioZoom,
		FimZoom,
		PonMousePixelAtual,   //--- Ponto atual do mouse
		FimArrast;            //--- Idem.
 LPCTSTR CursorAtual;
 std::string Projeto;
 POSITION PAtual;

protected:
	DECLARE_DYNCREATE(DesPerfilAvulsoView)
	DECLARE_MESSAGE_MAP()
public:
	DesPerfilAvulsoView();
	~DesPerfilAvulsoView();
	virtual void OnDraw(CDC* /*pDC*/);
};

