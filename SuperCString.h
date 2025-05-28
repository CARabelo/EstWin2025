
class SuperCString : public CString
{
 static CString Algarismos,  //--- Caracteres válidos no string
                Sinais;      //--- Podem ocorrer no string apenas no primeiro caractere.

public:

  bool Limpa(const CString& Lixo,bool Inverter = false);

  SuperCString(){};
  SuperCString(const CString& String);
  int ENumero() ;
  BOOL EEstaca();
  const SuperCString &operator=(const char *Dado);
  const SuperCString &operator=(const CString* Dado);

  static int ENumero(const char* pDados);
  static bool EEstaca(const char* pDados);
};

