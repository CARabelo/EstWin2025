class CalculaNota
{
	 CAlargamento Alargamentos;
	 CCurVerticais CurvasVerticais;
 	 CSecTipo SecaoTipo;
	 CCurHorizontais CurvasHorizontais;
	 CEstacasACalcular EstacasACalcular;
	 CLPontos SecaoAtual;

	 void MontaSecao();
	 void CotaSecao(Perfil& SecaoAtual);


	 public:

	CalculaNota(CString NomeProjeto);       //--- Construtor
};