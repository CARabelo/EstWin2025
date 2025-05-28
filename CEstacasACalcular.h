//------------------------------------------------------------

class CEstacasACalcular : public CList<CEstaca,CEstaca&>
{
  POSITION Atual;      //--- Ponteiro para o Atual da lista. 
  CProjeto* Projeto;

protected:

  void InsOrdenada(CEstaca &No,int INSEREIGUAL = FALSE); //--- Insers�o ordenada na lista.com estaca
  int Compara(CEstaca &No1,CEstaca &No2);  //--- compara dois n�s.

  //--- Posiciona o ponteiro da lista na se��o igual ou mais 
  //--- proxima no sentido indicado

  int PosicionaNoMaisProximo(CEstaca& Estaca,int Direcao = FALSE); 

public:

  CEstacasACalcular(CCurHorizontais* CurHorizontais,CCurVerticais* CurVerticais,const CString& NomeProj="",CProjeto* CPproj = NULL);
  void RemoveEstacasForaGeom(CCurHorizontais* pCCurHoriz, CList <CEstaca, CEstaca&>& ListaEstacasACalcular);    //--- Se o projeto mudou remove as estacas que sobraram do estaqueamento anterior
};