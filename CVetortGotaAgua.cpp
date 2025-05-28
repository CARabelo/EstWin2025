#include "stdafx.h"
#include "CMemDC.h"
#include "CPonto.h"
#include "Mat.h"
#include "CVetortGotaAgua.h"

extern class Matem Mat;

COLORREF CVetortGotaAgua::Tons[5] = {
                           RGB(255,255,255),
                           RGB(204,204,204),
                           RGB(153,153,153),
                           RGB(102,102,102),
                           RGB(51,51,51)
                          };

CVetortGotaAgua::CVetortGotaAgua(double pVel, lstPontos& pTraj, CMMemDC* ppDC) : Velocidade(pVel), Trajetoria(pTraj), TamanhoTotal(0),Fim(false),
                                             TamanhoAtual(0), LocalAtual(*Trajetoria.begin()),itp1RetaTraj(Trajetoria.begin()), itp2RetaTraj(itp1RetaTraj),
                                             pDC(ppDC),DistAtual(0.0)
{
  itp2RetaTraj++;

  //Ponto Resp = Mat.R_P(100.0,200.0, 110.0, 210.0);


  auto Resp(Mat.R_P(*itp1RetaTraj,*itp2RetaTraj));


  //Ponto Resp(Mat.R_P(itp1RetaTraj->x, itp1RetaTraj->y, itp2RetaTraj->x, itp2RetaTraj->y));
  //Ponto Resp(100,100);


  itp1RetaTraj->fi = Resp.x;
  DistTrajetoria = Resp.y;

  TamanhoTotal = Velocidade * 10.0;

  TamBlocoTom = TamanhoTotal / 5;
}

void CVetortGotaAgua::Inicia()
{
}

void CVetortGotaAgua::Move()
{
  if (TamanhoAtual < TamanhoTotal)
  {
    TamanhoAtual += TamBlocoTom;

    VetorAtual.push_back(CalcCoordxyVetor());
    DistribuiTons();
  }
  else
  {
    MoveVetor();
  }
 
   Desenha();
}

void CVetortGotaAgua::MoveVetor()
{
 PegaProxPontoVetor();
}

void CVetortGotaAgua::DistribuiTons()
{
}

void CVetortGotaAgua::Desenha()
{
  itlstPontos itPAnterior(VetorAtual.begin()),itPAtual(itPAnterior);

 int C(0);

 for (itPAtual++; itPAtual != VetorAtual.end(); itPAnterior++, itPAtual++)
 {
   DesenhaLinha(*itPAnterior, *itPAtual,Tons[C++]);
 }
}

Ponto CVetortGotaAgua::CalcCoordxyVetor()
{
  return Ponto(TamanhoAtual * cos(itp1RetaTraj->fi),TamanhoAtual * sin(itp1RetaTraj->fi));
}

void CVetortGotaAgua::PegaProxPontoVetor()
{
  DistAtual += TamBlocoTom;

  if (DistAtual < DistTrajetoria)
  {
    if(VetorAtual.size() == 5)
    {
      for(auto& VetAtual : VetorAtual)
      {
        VetAtual.x += TamBlocoTomX;
        VetAtual.y += TamBlocoTomY;
      }
    }
    else
    {
      size_t Tam(VetorAtual.size());

      Tam++;

      VetorAtual.emplace_back((Tam * TamBlocoTomX),(Tam * TamBlocoTomY));
    }
  }
  else
  {
    itp2RetaTraj++;

    if(itp2RetaTraj != Trajetoria.end()) 
    {
      itp1RetaTraj++;
      Inicia();
    }
    else Fim = true;
  }
}

void CVetortGotaAgua::DesenhaLinha(Ponto& PAnt, Ponto& PPost, COLORREF& Tom)
{
  CPen Pen;
  Pen.CreatePen(PS_SOLID, 1,Tom);
  pDC->SelectObject(&Pen);

  pDC->MoveTo(PAnt.x, PAnt.y);
  pDC->LineTo(PPost.x, PPost.y);

  DeleteObject(Pen);
}