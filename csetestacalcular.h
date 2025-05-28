typedef  cset<CEstaca,cestaca::compara<cestaca&>::iterator itsetEstACalcular;
//------------------------------------------------------------

class CEstacasACalcular : public cset<CEstaca,cestaca::compara<cestaca&> >
{
        POSITION Atual;      //--- Ponteiro para o Atual da lista. 
        CList <CEstaca, CEstaca&> PontosACalcular;

        protected:

  void InsOrdenada(CEstaca &No); //--- Insersão ordenada na lista.com estaca
        int Compara(CEstaca &No1,CEstaca &No2);  //--- compara dois nós.

    //--- Métodos de procura no set     

   itsetEstACalcular PegaMaiorIgual(CEstaca& Est) { return lower_bound(Est);};
   itsetEstACalcular PegaMenor(CEstaca& Est) { itsetEstACalcular pit(find(Sec)) ; return pit != end() ? pit-- : pit; };
   itsetEstACalcular PegaMaior(CEstaca& Est) { itsetEstACalcular pit(find(Sec)) ; return  pit != end() ? pit++ : pit;};
   itsetEstACalcular PegaMenorIgual(CEstaca& Est) { itsetEstACalcular pit(find(Sec)) ; return pit != end() pit ? pit--;};


  public:

        CEstacasACalcular(CCurHorizontais& CurHorizontais, CCurVerticais& CurVerticais);
};
