// CRadar
class CRadar 
{
  unsigned int Raio,RaioPixels;
  COLORREF CorTela;

  public:
	  CRadar(unsigned int pRaio,COLORREF pCorTela=RGB(40,58,42)):Raio(pRaio),CorTela(pCorTela) {};
	  virtual ~CRadar(){};

    inline void CRadar::Draw(CDC* pDC,int x,int y)
    {
      if(x == 0 || y == 0) return;

      CBrush Brush(CorTela);

      pDC->SelectObject(&Brush);

      static CPen shapePen(NULL,2,CorTela / 2);
      CPen* oldPen(pDC->SelectObject(&shapePen));
      pDC->MoveTo(x,y);
      pDC->Ellipse(x-RaioPixels,y+RaioPixels,x+RaioPixels,y-RaioPixels);

      pDC->SelectObject(oldPen);

      return;
    }

    COLORREF PegaCorTela() const { return CorTela;}
    unsigned int PegaRaio() const { return Raio;};
    unsigned int PegaRaioPixels() const { return RaioPixels;};
    void MudaRaio(unsigned int NovoRaio,double Escala){Raio = NovoRaio; RaioPixels = Raio*Escala;}
    void MudaCorTela(COLORREF* NovaCor) 
    { 
      CorTela = *NovaCor;
    }
    void RecalculaRaioPixels(double NovaEscala){ RaioPixels = Raio*NovaEscala;}
};


