class CConvUTMGDC
{
public:
  CConvUTMGDC();
  ~CConvUTMGDC();

  static __declspec(dllexport) int ConverteGDCUTM(const tylstGdc_Coord_3d& CoordGDC,tylstUtm_Coord_3d& CoordUTM,int NumDatum);
};

