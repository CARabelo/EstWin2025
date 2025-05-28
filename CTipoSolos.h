class CTipoSolo
{
public:

  tlstwstr ListaCampos;

  int Traco,TracoGDI;
  COLORREF Cor;

  CTipoSolo() {};
  CTipoSolo(tlstwstr& LstDados) : ListaCampos(LstDados) {};
  CTipoSolo(CString& Cod,CString& Desc=CString(""),CString& pTraco=CString(""),CString& pCor=CString("")):Traco(0),Cor(255),TracoGDI(0)
  {
    ListaCampos.clear();

    ListaCampos.push_back(Cod.GetBuffer());
    ListaCampos.push_back(Desc.GetBuffer());
    ListaCampos.push_back(pTraco.GetBuffer());
    ListaCampos.push_back(pCor.GetBuffer());

    Traco = atoi(pTraco.GetBuffer());
    Cor = atoi(pCor.GetBuffer());
  }
  
  CTipoSolo(const std::string& Cod,const std::string& Desc=std::string(""),const std::string& pTraco=std::string(""),const std::string& pCor=std::string("")):Traco(0),Cor(255),TracoGDI(0)
  {
    ListaCampos.clear();

    ListaCampos.push_back(Cod);
    ListaCampos.push_back(Desc);
    ListaCampos.push_back(pTraco);
    ListaCampos.push_back(pCor);

    Traco = atoi(pTraco.c_str());
    Cor = atoi(pCor.c_str());
  }

  int Consiste() 
  {
    return 0;
  };

  bool operator<(const CTipoSolo& RHS) const 
  {	
    if(ListaCampos.size() > 0 && RHS.ListaCampos.size() > 0)
      return ListaCampos.begin()->compare(*RHS.ListaCampos.begin()) < 0;
    else return true;
  }
};

typedef std::set<CTipoSolo> tysetCTipoSolo;
typedef tysetCTipoSolo::iterator itsetCTipoSolo;
