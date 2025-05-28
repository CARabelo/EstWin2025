// monologo.h : header file
//
#define QTDMENSAGENS 41
#define SEMMENSPADRAO 40   
/////////////////////////////////////////////////////////////////////////////
// monologo dialog

class monologo
{
  // Construction
public:

  monologo();   // standard constructor
  int mensagem(int num,CString complem="",int MotivFile=-1,int MotivArch=-1, bool Cancelar=false); 
  int mensagem(int num,int Motivo=-1,CString complem="",bool Cancelar = false)
  {  
    return mensagem(num,complem,Motivo,Cancelar);
  }; 

  int mensagem(int num,CString& Mensagem,CString& Complemento ){ return mensagem(num,Mensagem);}; 

  // Implementation

protected:
  CString mensagens[QTDMENSAGENS], 
          MotivosFile[20],
          MotivosArch[8];
public:
  CString ErrosCurVer[9],
          ErrosCurHor[13],
          ErrosAlarg[9],
          ErrosGradi[6],
          ErrosCante[11],
          ErrosSecTipo[9],
          ErrosExpess[4],
          ErrosIgual[2],
          ErrosOAE[6],
          ErrosOAC[2],
          ErrosSecTri[12],
          ErrosHPavim[3],
          ErrosTiposSolos[5],
          ErrosFurosSondagem[2],
          ErrosLimpeza[7],
          ErrosEmpola[4],
          ErrosPistasFixas[4];
};
