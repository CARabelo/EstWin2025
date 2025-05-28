#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqterrpontos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFINITO   1.7976931348623158e+308
#define ANTERIOR   0
#define POSTERIOR  1

extern class monologo monolog;

//----------------------------------------------------------------
//--- Implementação da lista

CArqTerrPontos::CArqTerrPontos(CString& NomeArq) :Atual(NULL)
{
  CFile ArqTerrPontos;
  CFileException e;

  NomeArquivo = NomeArq +".ter";

  if (ArqTerrPontos.Open(NomeArquivo,CFile::modeRead,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {	
    Serialize(ArqTerrPontos,0);
    ArqTerrPontos.Close();
    Atual = GetHeadPosition();
  }
}

void CArqTerrPontos::Serialize(CFile &Arquivo,int Gravar)
{
  struct Trr  //--- Para compatibilidade com a versão anterior.
  {
    char  EstFinal[12], Igu[3], Pontos[6][9];
    unsigned char Erros[2];
  }Dados;

  UINT Tam = sizeof(struct Trr);
  Atual = GetHeadPosition();

  if (Gravar)
  {
    while (Atual)
    {
      try
      {
        ArqTerrPontos NoAtual = GetNext(Atual);
        /*
        strcpy(Dados.EstFinal,NoAtual.EstFinal);
        strcpy(Dados.Igu,NoAtual.Ig);
        strcpy(Dados.Pontos[0],NoAtual.Pontos[0]);
        strcpy(Dados.Pontos[1],NoAtual.Pontos[1]);
        strcpy(Dados.Pontos[2],NoAtual.Pontos[2]);
        strcpy(Dados.Pontos[3],NoAtual.Pontos[3]);
        strcpy(Dados.Pontos[4],NoAtual.Pontos[4]);
        strcpy(Dados.Pontos[5],NoAtual.Pontos[5]);
        memcpy(Dados.Erros,"\x0\x0",2);
        Arquivo.Write(&Dados,Tam);
        */
      }
      catch(CFileException* e)
      {
        monolog.mensagem(14,e->m_cause);
        break;
      }
    }
  }
  else
  {
    try
    {
      ArqTerrPontos NoAtual;

      while (Arquivo.Read(&Dados,Tam) == Tam)
      {
        NoAtual.EstFinal = Dados.EstFinal;
        NoAtual.Ig = Dados.Igu;
        for (int i = 0 ; i < 6 ; i++) NoAtual.Pontos[i] = Dados.Pontos[i];
        //	  NoAtual.Erros[0] = Dados.Erros[0];
        //		NoAtual.Erros[1] = Dados.Erros[1];
        AddTail(NoAtual);
      }
    }
    catch(CFileException* e)
    {
      if (e->m_cause != CFileException::endOfFile)
      {
        //--- Tratar Erro; 
      }
    }
  }
}

void CArqTerrPontos::GravarArquivo()
{
  CFile ArqTerrPontos;
  CFileException e;

  if (ArqTerrPontos.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {
    Serialize(ArqTerrPontos,1);
    ArqTerrPontos.Close();
  }
}

