#include "stdafx.h"
#include "supercstring.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString SuperCString::Algarismos = "0123456789";
CString SuperCString::Sinais = ".+-"; 

SuperCString::SuperCString(const CString& String)
{
  CString::operator=(String);
}

int SuperCString::ENumero()
{
  SuperCString Dado(*this);
  Dado.Limpa(" ");

  if (Dado.IsEmpty()) return 2;   //--- Vazio  

  //-- se o primeiro algarismo não for algarismo nem sinal, retorna FALSE.
  int i = Algarismos.Find(Dado.GetAt(0));
  int f = Sinais.Find(Dado.GetAt(0));

  if (Algarismos.Find(Dado.GetAt(0)) == std::string::npos && Sinais.Find(Dado.GetAt(0)) == std::string::npos) return FALSE; 

  //--- remove o ponto e os sinais, tem q ter só um de cada

  if(Dado.Remove('.') > 1) return false;
  if(Dado.Remove('+') > 1) return false;
  if(Dado.Remove('-') > 1) return false;

  //--- o q sobrou tem q ser algarismo

  for (register int i = 0 ; i < Dado.GetLength(); i++) if (Algarismos.Find(Dado.GetAt(i)) ==  -1) return false;

  return true;
}

BOOL SuperCString::EEstaca()
{  
  SuperCString Dado(*this);
  Dado.Limpa(" ");

  if(Dado.GetLength() == 0) return false;

  CString Temp(Dado);  //--- Move o string para buffer temporário.
  Temp.TrimRight();

  int Mais(Temp.Find('+'));

  SuperCString Int,Frac;

  if (Mais != -1)
  {
    Int = Temp.Left(Mais).GetBuffer();	
    Frac = Temp.Right(Mais).GetBuffer();
  }
  else Int = Dado;

  return (Int.ENumero() && Frac.ENumero()); //--- Se as duas partes forem numero, é uma estaca
}

const SuperCString &SuperCString::operator=(const char *Dado)
{
  CString::operator=(Dado);
  return *this;
}
const SuperCString &SuperCString::operator=(const CString *Dado)
{
  CString::operator=(*Dado);
  return *this;
}

bool SuperCString::Limpa(const CString& Lixo,bool Inverter)
{
  CString Limpo("");

  if(this->GetLength() > 0)
  {
    if(!Inverter)
    {
      for(register int i = 0 ; i < GetLength() ; i++)
        if(Lixo.Find(this->operator[](i)) == -1)  Limpo += this->operator[](i); 
    }
    else 
    {
      for(register int i = 0 ; i < this->GetLength() ; i++)
        if(Lixo.Find(this->operator[](i)) != -1)  Limpo += this->operator[](i); 
    }
  }

  bool Retorno = Limpo.GetLength() != this->GetLength();

  CString::operator=(Limpo);

  return Retorno;
}

bool SuperCString::EEstaca(const char * pDados)
{  
  SuperCString Dado(pDados);
  Dado.Limpa(" ");

  if(Dado.GetLength() == 0) return false;

  CString Temp(Dado);  //--- Move o string para buffer temporário.
  Temp.TrimRight();

  int Mais(Temp.Find('+'));

  SuperCString Int,Frac;

  if (Mais != -1)
  {
    Int = Temp.Left(Mais).GetBuffer();	
    Frac = Temp.Right(Mais).GetBuffer();
  }
  else Int = Dado;

  return (Int.ENumero() && Frac.ENumero()); //--- Se as duas partes forem numero, é uma estaca
}

int SuperCString::ENumero(const char* pDados)
{
  SuperCString Dado(pDados);
  Dado.Limpa(" ");

  if (Dado.IsEmpty()) return 2;   //--- Vazio  

  //-- se o primeiro algarismo não for algarismo nem sinal, retorna FALSE.
  int i = Algarismos.Find(Dado.GetAt(0));
  int f = Sinais.Find(Dado.GetAt(0));

  if (Algarismos.Find(Dado.GetAt(0)) == std::string::npos && Sinais.Find(Dado.GetAt(0)) == std::string::npos -1) return FALSE; 

  //--- remove o ponto e os sinais, tem q ter só um de cada

  if(Dado.Remove('.') > 1) return false;
  if(Dado.Remove('+') > 1) return false;
  if(Dado.Remove('-') > 1) return false;

  //--- o q sobrou tem q ser algarismo

  for (register int i = 0 ; i < Dado.GetLength(); i++) if (Algarismos.Find(Dado.GetAt(i)) ==  -1) return false;

  return true;
}
