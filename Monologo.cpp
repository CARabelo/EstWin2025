// monologo.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "monologo.h"
#include <string>
#include "DiaMonologo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ERROINESPERADO 25

/////////////////////////////////////////////////////////////////////////////
// monologo dialog

monologo::monologo() 
{
  mensagens[0] = "O diret�rio n�o p�de ser criado.";
  mensagens[1] = "O projeto n�o p�de ser criado.";
  mensagens[2] = "Este projeto j� existe, n�o pode ser criado.";
  mensagens[3] = "Este valor est� fora da faixa permitida, que �:";
  mensagens[4] = "Este arquivo n�o pode ser editado pelo Vias Urbanas & Estradas";
  mensagens[5] = "Primeiro abra um projeto para depois criar um arquivo.";
  mensagens[6] = "Os arquivos sumiram!!!";
  mensagens[7] = "Todos os arquivos deste projeto j� foram criados.";
  mensagens[8] = "Nenhum arquivo foi criado ainda.";
  mensagens[9] = "N�o h� dados de terreno para serem calculados."; 
  mensagens[10]= "Deve ser usado no m�nimo um arquivo.";
  mensagens[11] = "O ponto no gradiente n�o pode ser deletado";
  mensagens[12] = "H� um erro nos taludes de pavimento, abortando...";
  mensagens[13] = "Erro na abertura do arquivo: ";
  mensagens[14] = "Erro ao abrir o arquivo: ";
  mensagens[15] = "Erro ao ler o arquivo: ";
  mensagens[16] = "Erro ao gravar o arquivo: ";
  mensagens[17] = "Dados incompletos para a execu��o dos c�lculos.";
  mensagens[18] = "Erro na curva, desenvolvimento da circular n�o confere com o AC.";  
  mensagens[19] = "Os ramos da espiral est�o reentrantes.";
  mensagens[20] = "AC muito pequeno (< 30""), (Se for esta a inten��o, zere o raio)";
  mensagens[21] = "Curvas transpassando";
  mensagens[22] = "Houve erro ao inserir a curva neste ponto.";
  mensagens[23] = "Arquivo n�o encontrado.";
  mensagens[24] = "Registro inconsistente - ";
  mensagens[25] = "Erro inesperado, contacte o suporte.";
  mensagens[26] = "Curvas est�o fora de ordem. ";
  //mensagens[27] = "Ponto de canteiro n�o encontrado na se��o tipo, ser� desconsiderado no c�lculo. Est: ";  Aguardar
  mensagens[27] = "Ponto de canteiro n�o encontrado na se��o tipo. Est: ";
  mensagens[28] = "O arquivo de curva verticais  est� inconsistente.";
  mensagens[29] = "Erro ao criar o arquivo: ";
  mensagens[30] = "A lista de bordos esta vazia.";
  mensagens[31] = "A superficie n�o est� fechada, verifique bordos proximo ao ponto:";
  mensagens[32] = "O primeiro e o �ltimo PIV devem ter (Y1 e Y2)=0, zerando-os na grava��o." ;
  mensagens[33] = "A altera��o maxima permitida no I � de 0.03, o I anterior ser� mantido.";
  mensagens[34] = "PIVs transpassando, os valores anteriores ser�o mantidos ";
  mensagens[35] = "Esta altera��o resulta em rampas maiores que 1 ou -1, os valores anteriores ser�o mantidos.";
  mensagens[36] = "Encontrado ponto de bordo que n�o perten�e a superficie, abortando...";
  mensagens[37] = "Erro ao inserir curva, programa abortado. Contacte o suporte ERR-00033";
  mensagens[38] = "Erro na abertura do arquivo da superficie(.sup). ";
  mensagens[39] = "Erro de aloca��o de mem�ria, n�o h� mais espa�o na memoria...";
  mensagens[40] = "";

  MotivosFile[CFileException::none] = "\r\nNenhum erro.";
  MotivosFile[CFileException::genericException] = "\r\nErro gen�rico";
  MotivosFile[CFileException::fileNotFound] = "\r\nArquivo n�o encontrado.";
  MotivosFile[CFileException::badPath] = "\r\nO caminho especificado � invalido.";
  MotivosFile[CFileException::tooManyOpenFiles] = "\r\nO n�mero permitido de arquivos abertos foi ultrapassado";
  MotivosFile[CFileException::accessDenied] = "\r\nO acesso ao arquivo foi negado.";
  MotivosFile[CFileException::invalidFile] = "\r\nO manipulador do arquivo est� inv�lido.";
  MotivosFile[CFileException::removeCurrentDir] = "\r\nO diret�rio corrente n�o pode ser deletado.";
  MotivosFile[CFileException::directoryFull] = "\r\nO n�mero m�ximo de arquivos no diret�rio foi alcan�ado.";
  MotivosFile[CFileException::badSeek] = "\r\nHouve um erro ao usar o ponteiro do arquivo.";
  MotivosFile[CFileException::hardIO ] = "\r\nErro no hardware.";
  MotivosFile[CFileException::sharingViolation] = "\r\nShare.exe n�o esta carregado ou houve um erro de compartilhamento.";
  MotivosFile[CFileException::lockViolation] = "\r\nHouve uma tentativa de trancar uma regi�o do disco que j� esta trancada.";
  MotivosFile[CFileException::diskFull] = "\r\nDisco cheio, n�o h� espa�o para mais arquivos neste disco.";

  MotivosArch[CArchiveException::none] = "\r\nNenhum Erro.";
  MotivosArch[CArchiveException::genericException] = "\r\nErro n�o especificado.";
  MotivosArch[CArchiveException::readOnly] = "\r\nTentativa de escrever em um  arquivo aberto somente para leitura.";
  MotivosArch[CArchiveException::endOfFile] = "\r\nEncontrado o fim do arquivo inesperadamente.";
  MotivosArch[CArchiveException::writeOnly] = "\r\nTentativa de ler um  arquivo aberto somente para grava��o.";
  MotivosArch[CArchiveException::badIndex] = "\r\nO fomato do arquivo est� inv�lido.";
  MotivosArch[CArchiveException::badClass] = "\r\nTentativa de ler um objeto do tipo incorreto.";
  MotivosArch[CArchiveException::badSchema] = "\r\nTentativa de ler um objeto com uma vers�o diferente da classe.";

  ErrosCurVer[0] = " - Estaca PIV invalida.";
  ErrosCurVer[1] = " - Igualdade inv�lida.";
  ErrosCurVer[2] = " - Cota inv�lida";
  ErrosCurVer[3] = " - Y1 inv�lido.";
  ErrosCurVer[4] = " - Y2 inv�lido.";

  ErrosCurHor[0] = " - Estaca do TS inv�lida.";
  ErrosCurHor[1] = " - Igualdade do TS inv�lida.";
  ErrosCurHor[2] = " - Estaca do ST inv�lida.";
  ErrosCurHor[3] = " - Igualdade do ST inv�lida.";
  ErrosCurHor[4] = " - Lado Inv�lido.";
  ErrosCurHor[5] = " - Tipo inv�lido.";
  ErrosCurHor[6] = " - AC inv�lido.";
  ErrosCurHor[7] = " - Raio inv�lido.";
  ErrosCurHor[8] = " - LC inv�lido.";
  ErrosCurHor[9] = " - SE inv�lido.";
  ErrosCurHor[10] = " - SL inv�lido.";
  ErrosCurHor[11] = " - Houve um erro na abertura do arquivo de curvas Horizontais.";
  ErrosCurHor[12] = " - O N�mero de campos est� incorreto (<> 9)"; 

  ErrosAlarg[0] = "Estaca final inv�lida.";
  ErrosAlarg[1] = "Igualdade inv�lida."; 
  ErrosAlarg[2] = "Lado inv�lido.";
  ErrosAlarg[3] = "Dx inv�lido."; 
  ErrosAlarg[4] = "Dy inv�lido.";
  ErrosAlarg[5] = "O par xy de algum ponto n�o esta completo";
  ErrosAlarg[6] = "O deslocamento deve ser negativo para a esquerda e positivo para a direita e seu valor minimo absoluto � 0.001m";
  ErrosAlarg[7] = "Deve haver no m�nimo 2 pontos. (O ponto inicial � sempre (0,0))";
  ErrosAlarg[8] = "O ponto inicial deve ser sempre (0,0))";

  ErrosGradi[0] = "Estaca final inv�lida.";
  ErrosGradi[1] = "Igualdade inv�lida."; 
  ErrosGradi[2] = "Afastamento inv�lido.";
  ErrosGradi[3] = "Quantidade de campos inv�lida.";
  ErrosGradi[4] = "O Valor de x est� fora da se��o Tipo.";
  ErrosGradi[5] = "Aten��o: N�o h� sec�o tipo com esta estaca final.";

  ErrosExpess[0] = "Estaca final inv�lida.";
  ErrosExpess[1] = "Igualdade inv�lida."; 
  ErrosExpess[2] = "Altura inv�lida.";
  ErrosExpess[3] = "Altura do pavimento fora dos limites (> 5.00m  ou < 0.10m).";

  ErrosEmpola[0] = "Estaca final inv�lida.";
  ErrosEmpola[1] = "Igualdade inv�lida."; 
  ErrosEmpola[2] = "% invalido";
  ErrosEmpola[3] = "% fora dos limites (< 1% ou > 90%)";

  ErrosCante[0] = "Estaca final inv�lida.";
  ErrosCante[1] = "Igualdade inv�lida."; 
  ErrosCante[2] = "INI 1 inv�lido."; 
  ErrosCante[3] = "FIM 1 inv�lido.";
  ErrosCante[4] = "INI 2 inv�lido."; 
  ErrosCante[5] = "FIM 2 inv�lido.";
  ErrosCante[6] = "INI 3 inv�lido."; 
  ErrosCante[7] = "FIM 3 inv�lido.";
  ErrosCante[8] = "Algum par INI/FIM est� incompleto.";
  ErrosCante[9] = "O valor de deltax entre o in�cio e o fim do  n�o pode ser menor que 0.001m.";
  ErrosCante[10] = "Faltam pontos para determinar o in�cio ou o fim do canteiro.";

  ErrosSecTipo[0] = "Estaca final inv�lida.";
  ErrosSecTipo[1] = "Igualdade inv�lida."; 
  ErrosSecTipo[2] = "x inv�lido."; 
  ErrosSecTipo[3] = "y inv�lido.";
  ErrosSecTipo[4] = "N�o h� pontos suficientes para formar uma se��o tipo, minimo 3 pontos";
  ErrosSecTipo[5] = "Algum par xy est� incompleto.";
  ErrosSecTipo[6] = "O valor absoluto do deltax entre os pontos n�o pode ser menor que 0.001m.";
  ErrosSecTipo[7] = "� Obrigatorio existir um ponto (0,0) na se��o tipo.";
  
  ErrosIgual[0] = "Estaca 1 inv�lida.";
  ErrosIgual[1] = "Estaca 2 inv�lida.";

  ErrosOAE[0] = "Estaca inicial inv�lida.";
  ErrosOAE[1] = "Igualdade inicial inv�lida.";
  ErrosOAE[2] = "Estaca Final inv�lida.";
  ErrosOAE[3] = "Igualdade final inv�lida.";
  ErrosOAE[4] = "Dados incompletos.� preciso definir as estacas iniciais e finais.";
  ErrosOAE[5] = "A estaca final deve estar depois da estaca inicial.";

  ErrosOAC[0] = "Estaca inv�lida.";
  ErrosOAC[1] = "Igualdade inv�lida";

  ErrosSecTri[0] = "Estaca Inv�lida.";
  ErrosSecTri[1] = "Igualdade Inv�lida.";
  ErrosSecTri[2] = "Afastamento 1 Inv�lido.";
  ErrosSecTri[3] = "Cota 1 Inv�lida.";
  ErrosSecTri[4] = "Afastamento 2 Inv�lido.";
  ErrosSecTri[5] = "Cota 2 Inv�lida.";
  ErrosSecTri[6] = "Afastamento 3 Inv�lido.";
  ErrosSecTri[7] = "Cota 3 Inv�lida.";
  ErrosSecTri[8] = "Afastamento 4 Inv�lido.";
  ErrosSecTri[9] = "Cota 4 Inv�lida.";
  ErrosSecTri[10] = "Afastamento 5 Inv�lido.";
  ErrosSecTri[11] = "Cota 5 Inv�lida.";

  ErrosHPavim[0] = "Estaca Inv�lida.";
  ErrosHPavim[1] = "Altura de pavimento Inv�lida.";
  ErrosHPavim[2] = "Faltam Campos.";

  ErrosTiposSolos[0] = "C�digo de solo inv�lido";
  ErrosTiposSolos[1] = "Profundidade tem que ser um n�mero";
  ErrosTiposSolos[2] = "Profundidade fora dos limites (<0.00 ou > 50.0m)";
  ErrosTiposSolos[3] = "Estaca Inv�lida";
  ErrosTiposSolos[4] = "Cota do furo inv�lida";

  ErrosFurosSondagem[0] = "Tipo de solo inexistente";

  ErrosLimpeza[0] = "Estaca inv�lida";
  ErrosLimpeza[1] = "Igualdade inv�lida";
  ErrosLimpeza[2] = "Altura inv�lida";
  ErrosLimpeza[3] = "Exesso lateral inv�lido";
  ErrosLimpeza[4] = "Faltam campos";
  ErrosLimpeza[5] = "Valor da altura fora dos limites (< 0.00m ou > 3.00m).";
  ErrosLimpeza[6] = "Valor do excesso lateral fora dos limites (< 0.30m ou > 30.00m).";
}

/////////////////////////////////////////////////////////////////////////////
// monologo message handlers

int monologo::mensagem(int num,CString complem,int MotivoFile,int MotivoArch,bool Cancelar) 
{
  if (num == SEMMENSPADRAO)
  {
    CDiaMonologo DiaMonologo(complem.GetBuffer());
    return DiaMonologo.DoModal();
  }

  if (num < -1 || num > QTDMENSAGENS  || MotivoFile < -1 ||  MotivoFile > CFileException::diskFull || MotivoArch < -1 ||  MotivoArch > CArchiveException::badSchema) 
  {

    CDiaMonologo DiaMonologo(mensagens[ERROINESPERADO].GetBuffer());
    return DiaMonologo.DoModal();
  }

  CString Mensagem((num >=0 ? mensagens[num] : ""));
  
  Mensagem += complem;
  Mensagem += CString((MotivoFile == -1 && (MotivoArch == -1 || MotivoArch == 0)) ? CString("") :
                       MotivoFile != -1 ? MotivosFile[MotivoFile] : MotivosArch[MotivoArch]);

   CDiaMonologo DiaMonologo(Mensagem.GetBuffer());
   return DiaMonologo.DoModal();
}
