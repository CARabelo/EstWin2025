class DParabola
{
  const CurVertical &PIV;
	int               EquiMin,NPontos;

	CArray <Point,Point&> Pontos;
  
	DParabola(CurVertical &RPIV);
	Desenha(CDC *pDC,int FatorEscala);
}

DParabola(CurVertical &RPIV):EquiMin(5),PIV(RPIV)
{
  Dist = (int)((PIV.Y1 + PIV.Y2) * 100.0);  //--- distancia do inicio ao fim da parabola.
  NPontos(dist / EquiMin);    //--- Numero de pontos a serem calculados.

	//--- Serão desenhadas circulares a cada 3 pontos, então calcula o núnero de pontos a 
	//--- serem calculados.

	if (NPontos < 3) NPontos = 3;
	else
	{
	  I = NPontos - 3;
		I = if ((I mod 2)) I--;;
		NPontos += I;
	} 

  int EquiDist = Dist / NPontos;   //--- Equidistância entre os pontos.
	Pontos.SetSize(NPontos);         //--- Tamanho do array qua acomodará os pontos
	
	for (int x = 0,int i = 0 ; x < Dist ; x += EquiDist,i++) 
	{
	  int Y =    //=== Calcular o valor de y;
	  Pontos[i].OffSet(x,y);    //--- Armazena o ponto.
  }
}

DParabola::Desenha(CDC *pDC,double FatorEscala)
{
  //--- Desenha uma circular em cada 3 pontos da parabola, consederando o fator escala
	//--- Requerido.

  for(int i = 0 ; i < NPontos ; )
	  pDC->Arc(Ponto[i].x * FatorEscala,Ponto[i].y * FatorEscala,
		         Ponto[i++].x * FatorEscala,Ponto[i++].y * FatorEscala,
						 Ponto[i++].x * FatorEscala,Ponto[i++].y * FatorEscala)
}


