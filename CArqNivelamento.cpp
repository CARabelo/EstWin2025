#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "cvisada.h"
#include "carqnivelamento.h"

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

void CArqNivelamento::Serialize(CArchive &ar)
{/*
 struct niv  //--- Para compatibilidade com a versão anterior.
 {
 char  Visada[12], Igu[3], Plref[9], Leitura[6], Cota[9];
 unsigned char Erros[2];
 }Dados;

 UINT Tam = sizeof(struct niv);
 */
  Atual = GetHeadPosition();

  if (ar.IsStoring())
  {
    ArqNivelamento NoAtual;
    while (Atual)
    {
      try
      {
        NoAtual = GetNext(Atual);
      
        /*
        strcpy(Dados.Visada,NoAtual.Visada);
        strcpy(Dados.Igu,NoAtual.Ig);
        strcpy(Dados.Plref,NoAtual.PlRef);
        strcpy(Dados.Leitura,NoAtual.Leitura);
        strcpy(Dados.Cota,NoAtual.Cota);
        memcpy(Dados.Erros,"\x0\x0",2);
        Arquivo.Write(&Dados,Tam);
        */
        ar << NoAtual.Visada << NoAtual.Ig << NoAtual.PlRef 
          << NoAtual.Leitura << NoAtual.Cota ;
      }
      catch(CArchiveException* e)
      {
        if (e->m_cause != CArchiveException::endOfFile)   //--- fim do arquivo?, ok, sai do bloco..
          monolog.mensagem(16,CString(" Nivelamento : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
  else
  {
    try
    {
      ArqNivelamento NoAtual;

      /*			while (Arquivo.Read(&Dados,Tam) == Tam)
      {
      NoAtual.Visada = (CString) Dados.Visada;
      NoAtual.Ig = Dados.Igu;
      NoAtual.PlRef = Dados.Plref;
      NoAtual.Leitura = Dados.Leitura;
      NoAtual.Cota = Dados.Cota;
      NoAtual.Erros[0] = Dados.Erros[0];
      NoAtual.Erros[1] = Dados.Erros[1];
      */
      do
      {
        ar >> NoAtual.Visada >> NoAtual.Ig  >> NoAtual.PlRef >> NoAtual.Leitura
          >> NoAtual.Cota;

        AddTail(NoAtual);
      } while(TRUE);
    }
    catch(CArchiveException *e)
    {
      if (e->m_cause != CFileException::endOfFile)
      {
        if (e->m_cause != CArchiveException::endOfFile)   //--- fim do arquivo?, ok, sai do bloco..
          monolog.mensagem(16,CString(" Nivelamento : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

      }
    }
  }
}

CArqNivelamento::CArqNivelamento(CString& NomeArq)
{
  CFile ArqNivelamento;
  CFileException e;

  NomeArquivo = NomeArq + ".niv";

  if (ArqNivelamento.Open(NomeArquivo,CFile::modeRead,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {	
    CArchive ar(&ArqNivelamento,CArchive::load); 
    Serialize(ar);
    ar.Close();
    ArqNivelamento.Close();
    Atual = GetHeadPosition();
  }
}

void CArqNivelamento::GravarArquivo()
{
  CFile ArqNivelamento;
  CFileException e;

  if (ArqNivelamento.Open(NomeArquivo,CFile::modeCreate | CFile::modeWrite,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
  }
  else
  {
    CArchive ar(&ArqNivelamento,CArchive::store);
    Serialize(ar);
    ar.Close();
    ArqNivelamento.Close();
  }
}

