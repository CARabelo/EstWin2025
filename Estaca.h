// Cestaca.h : interface da classe Estaca
//
/////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
//--- Classe Cestaca.
//--- Esta classe representa uma estaca que pode tanto ser um string como
//--- um double, os construtores sobrecarregados definem qual o tipo usado.
//--- Uma propriedade cria a outra, são complementares, ou seja, caso venha
//--- um string o double é criado a partir dele, caso venha um double o string 
//--- é criado a partir dele. as igualdades são tratadas automáticamente,
//--- na construção já são consideradas e o objeto possui as duas propriedades,
//--- a estaca real e a virtual. 
//------------------------------------------------------------------------- 

#define INFINITO   1.7976931348623158e+308

class CEstaca : public CString 
{
  void ParaDouble(const CString& Ig = CString(""));
  void ParaString();

  bool CEstaca::e_numeroInt(const CString& Estaca)
  {
    int Sinalizada(Estaca[0] == '-' || Estaca[0] == '+');  

    register int i;

    for (i =  Sinalizada ? 1 : 0; i < Estaca.GetLength() && (Estaca[i] == ' ') ; i++) ;  // pula os espaços em branco

    for (; i < Estaca.GetLength() && isdigit(Estaca[i]) ; i++) ;      //--- Verifica se todos são digitos

    return  ((Sinalizada && i > 1 && i == Estaca.GetLength()) || i == Estaca.GetLength());
  }

public:

  enum SINAIS{NENHUM_SINAL,MAO_LIVRE,FIM_SINAIS};

  double EstReal,EstVirtual,Cota;
  CString Descricao,Igualdade;
  unsigned long Sinalizadores;

  static double ERRO_MINIMO;
 
  //--- Métodos publicos

  void AlteraValor(double Valor,double PCota = 0.0,int Ig = 0);

  //--- construtores

  CEstaca(const double Estaca,const  double Cota = INFINITO,const int Ig=0,const CString& Tipo=CString(""),unsigned long Sinais=NENHUM_SINAL);
  CEstaca(const CString& Estaca,const CString& Ig = CString("") ,const CString& Descr=CString(""),double PCota=INFINITO);
  CEstaca(const CEstaca& Estaca,const CString& Desc);
  CEstaca(const char *Estaca,const CString& Ig=CString(""));
  CEstaca();

  //--- construtores "Atrasados"

  void Constroi(const double Estaca,const double Cota = INFINITO,const int Ig=0,const CString& Tipo=CString(""));
  void Constroi(const char *Estaca,const CString& Ig = CString(""));
  void Constroi(const CString& Estaca,const CString& PCota,const CString& Ig=CString(""),const CString& Descr=CString(""));

  //--- Operacões

  void AtualizaValores() { ParaDouble();};

  int Compara(const CEstaca& Est);

  //--- Operadores
  //--- InLine

  //--- Usando 5e-3 como a distancia elementar. As estacas são iguais se a distancia entre elas for menor que 5e-3 
  //--- as outras definições <, >, <= e >= são feitas a partir desta.

  bool operator==(const CEstaca& EstacaAComparar) const {return fabs(EstacaAComparar.EstVirtual - EstVirtual) < ERRO_MINIMO;}
  bool operator!=(const CEstaca& EstacaAComparar) const {return fabs(EstacaAComparar.EstVirtual - EstVirtual) > ERRO_MINIMO;}
  bool operator>(const CEstaca& EstacaAComparar) const {return EstVirtual - EstacaAComparar.EstVirtual > ERRO_MINIMO;}
  bool operator<(const CEstaca& EstacaAComparar) const {return (EstVirtual - EstacaAComparar.EstVirtual) < -ERRO_MINIMO;} 
  bool operator>=(const CEstaca& EstacaAComparar) const  {return EstVirtual - EstacaAComparar.EstVirtual > -ERRO_MINIMO;}
  bool operator<=(const CEstaca& EstacaAComparar) const {return  EstVirtual - EstacaAComparar.EstVirtual < ERRO_MINIMO;}
  double operator-(const CEstaca& EstASubtrair) { return EstVirtual - EstASubtrair.EstVirtual; }
  double operator+(double ValorASomar) { return EstVirtual + ValorASomar;}

  CEstaca& operator+(const CEstaca& EstacaAsomar)
  {
    static CEstaca EstTemp(EstVirtual + EstacaAsomar.EstVirtual);
    ParaString();

    return EstTemp;
  }
  CEstaca& operator+=(double  Valor) 
  { 
    EstVirtual += Valor ; 
    EstReal += Valor;
    ParaString() ; 
    return *this ; 
  }

  //--- Operadores (Not in line)

  const double &operator=(const double& Valor);
  const CEstaca &operator=(const CEstaca& Estaca);

  CArchive& operator>>(CArchive& ar);
  CArchive& operator<<(CArchive& ar);

 std::ifstream& operator<<(std::ifstream& ifs) 
 {
   ifs >> *this;
   ifs >> EstReal >> EstVirtual >> Cota;
   ifs >> Descricao >> Igualdade;
   ifs >> Sinalizadores;
 }
 
 std::ofstream& operator>>(std::ofstream& ofs) 
 {
   ofs << *this;
   ofs << EstReal << EstVirtual << Cota;
   ofs << Descricao << Igualdade;
   ofs << Sinalizadores;
 }
};

