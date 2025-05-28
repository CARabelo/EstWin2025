void CSuperficie::LigaCurvasFechadas()
{
  //---  Procura em todos os pontos da superfice por pontos de cota q ainda não foram ligados ( so restaram CNs fechadas)

  for (register ItSSuperficie itSupAtual = pSuperficieAtual->begin() ; itSupAtual != pSuperficieAtual->end() ; itSupAtual++)
  {
    //--- Procura em todos os pontos ligados

    for(register ittysetArestas itSupLigadaAtual = itSupAtual->PontosLigados.begin() ; itSupLigadaAtual != itSupAtual->PontosLigados.end() ; itSupLigadaAtual++)
    {
       //--- Procura em todas as cotas

       for (register itsetPontosS itPontoCota = (*itSupLigadaAtual).PontosInterpolacao.begin() ; itPontoCota != (*itSupLigadaAtual).PontosInterpolacao.end() ; itPontoCota++)
       {
         //--- Se não foi interpolada interpola-a
          
         if(itPontoCota->Baliza == false) 
         {
           ItSSuperficie ItSupLigada = pSuperficieAtual->find(itSupLigadaAtual->PontoLigado);

           Ligacn(ItSupLigada,itSupAtual,itPontoCota);
         }
       }
    }
  }
}