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
  mensagens[0] = "O diretório não pôde ser criado.";
  mensagens[1] = "O projeto não pôde ser criado.";
  mensagens[2] = "Este projeto já existe, não pode ser criado.";
  mensagens[3] = "Este valor está fora da faixa permitida, que é:";
  mensagens[4] = "Este arquivo não pode ser editado pelo Vias Urbanas & Estradas";
  mensagens[5] = "Primeiro abra um projeto para depois criar um arquivo.";
  mensagens[6] = "Os arquivos sumiram!!!";
  mensagens[7] = "Todos os arquivos deste projeto já foram criados.";
  mensagens[8] = "Nenhum arquivo foi criado ainda.";
  mensagens[9] = "Não há dados de terreno para serem calculados."; 
  mensagens[10]= "Deve ser usado no mínimo um arquivo.";
  mensagens[11] = "O ponto no gradiente não pode ser deletado";
  mensagens[12] = "Há um erro nos taludes de pavimento, abortando...";
  mensagens[13] = "Erro na abertura do arquivo: ";
  mensagens[14] = "Erro ao abrir o arquivo: ";
  mensagens[15] = "Erro ao ler o arquivo: ";
  mensagens[16] = "Erro ao gravar o arquivo: ";
  mensagens[17] = "Dados incompletos para a execução dos cálculos.";
  mensagens[18] = "Erro na curva, desenvolvimento da circular não confere com o AC.";  
  mensagens[19] = "Os ramos da espiral estão reentrantes.";
  mensagens[20] = "AC muito pequeno (< 30""), (Se for esta a intenção, zere o raio)";
  mensagens[21] = "Curvas transpassando";
  mensagens[22] = "Houve erro ao inserir a curva neste ponto.";
  mensagens[23] = "Arquivo não encontrado.";
  mensagens[24] = "Registro inconsistente - ";
  mensagens[25] = "Erro inesperado, contacte o suporte.";
  mensagens[26] = "Curvas estão fora de ordem. ";
  //mensagens[27] = "Ponto de canteiro não encontrado na seção tipo, será desconsiderado no cálculo. Est: ";  Aguardar
  mensagens[27] = "Ponto de canteiro não encontrado na seção tipo. Est: ";
  mensagens[28] = "O arquivo de curva verticais  está inconsistente.";
  mensagens[29] = "Erro ao criar o arquivo: ";
  mensagens[30] = "A lista de bordos esta vazia.";
  mensagens[31] = "A superficie não está fechada, verifique bordos proximo ao ponto:";
  mensagens[32] = "O primeiro e o último PIV devem ter (Y1 e Y2)=0, zerando-os na gravação." ;
  mensagens[33] = "A alteração maxima permitida no I é de 0.03, o I anterior será mantido.";
  mensagens[34] = "PIVs transpassando, os valores anteriores serão mantidos ";
  mensagens[35] = "Esta alteração resulta em rampas maiores que 1 ou -1, os valores anteriores serão mantidos.";
  mensagens[36] = "Encontrado ponto de bordo que não pertençe a superficie, abortando...";
  mensagens[37] = "Erro ao inserir curva, programa abortado. Contacte o suporte ERR-00033";
  mensagens[38] = "Erro na abertura do arquivo da superficie(.sup). ";
  mensagens[39] = "Erro de alocação de memória, não há mais espaço na memoria...";
  mensagens[40] = "";

  MotivosFile[CFileException::none] = "\r\nNenhum erro.";
  MotivosFile[CFileException::genericException] = "\r\nErro genérico";
  MotivosFile[CFileException::fileNotFound] = "\r\nArquivo não encontrado.";
  MotivosFile[CFileException::badPath] = "\r\nO caminho especificado é invalido.";
  MotivosFile[CFileException::tooManyOpenFiles] = "\r\nO número permitido de arquivos abertos foi ultrapassado";
  MotivosFile[CFileException::accessDenied] = "\r\nO acesso ao arquivo foi negado.";
  MotivosFile[CFileException::invalidFile] = "\r\nO manipulador do arquivo está inválido.";
  MotivosFile[CFileException::removeCurrentDir] = "\r\nO diretório corrente não pode ser deletado.";
  MotivosFile[CFileException::directoryFull] = "\r\nO número máximo de arquivos no diretório foi alcançado.";
  MotivosFile[CFileException::badSeek] = "\r\nHouve um erro ao usar o ponteiro do arquivo.";
  MotivosFile[CFileException::hardIO ] = "\r\nErro no hardware.";
  MotivosFile[CFileException::sharingViolation] = "\r\nShare.exe não esta carregado ou houve um erro de compartilhamento.";
  MotivosFile[CFileException::lockViolation] = "\r\nHouve uma tentativa de trancar uma região do disco que já esta trancada.";
  MotivosFile[CFileException::diskFull] = "\r\nDisco cheio, não há espaço para mais arquivos neste disco.";

  MotivosArch[CArchiveException::none] = "\r\nNenhum Erro.";
  MotivosArch[CArchiveException::genericException] = "\r\nErro não especificado.";
  MotivosArch[CArchiveException::readOnly] = "\r\nTentativa de escrever em um  arquivo aberto somente para leitura.";
  MotivosArch[CArchiveException::endOfFile] = "\r\nEncontrado o fim do arquivo inesperadamente.";
  MotivosArch[CArchiveException::writeOnly] = "\r\nTentativa de ler um  arquivo aberto somente para gravação.";
  MotivosArch[CArchiveException::badIndex] = "\r\nO fomato do arquivo está inválido.";
  MotivosArch[CArchiveException::badClass] = "\r\nTentativa de ler um objeto do tipo incorreto.";
  MotivosArch[CArchiveException::badSchema] = "\r\nTentativa de ler um objeto com uma versão diferente da classe.";

  ErrosCurVer[0] = " - Estaca PIV invalida.";
  ErrosCurVer[1] = " - Igualdade inválida.";
  ErrosCurVer[2] = " - Cota inválida";
  ErrosCurVer[3] = " - Y1 inválido.";
  ErrosCurVer[4] = " - Y2 inválido.";

  ErrosCurHor[0] = " - Estaca do TS inválida.";
  ErrosCurHor[1] = " - Igualdade do TS inválida.";
  ErrosCurHor[2] = " - Estaca do ST inválida.";
  ErrosCurHor[3] = " - Igualdade do ST inválida.";
  ErrosCurHor[4] = " - Lado Inválido.";
  ErrosCurHor[5] = " - Tipo inválido.";
  ErrosCurHor[6] = " - AC inválido.";
  ErrosCurHor[7] = " - Raio inválido.";
  ErrosCurHor[8] = " - LC inválido.";
  ErrosCurHor[9] = " - SE inválido.";
  ErrosCurHor[10] = " - SL inválido.";
  ErrosCurHor[11] = " - Houve um erro na abertura do arquivo de curvas Horizontais.";
  ErrosCurHor[12] = " - O Número de campos está incorreto (<> 9)"; 

  ErrosAlarg[0] = "Estaca final inválida.";
  ErrosAlarg[1] = "Igualdade inválida."; 
  ErrosAlarg[2] = "Lado inválido.";
  ErrosAlarg[3] = "Dx inválido."; 
  ErrosAlarg[4] = "Dy inválido.";
  ErrosAlarg[5] = "O par xy de algum ponto não esta completo";
  ErrosAlarg[6] = "O deslocamento deve ser negativo para a esquerda e positivo para a direita e seu valor minimo absoluto é 0.001m";
  ErrosAlarg[7] = "Deve haver no mínimo 2 pontos. (O ponto inicial é sempre (0,0))";
  ErrosAlarg[8] = "O ponto inicial deve ser sempre (0,0))";

  ErrosGradi[0] = "Estaca final inválida.";
  ErrosGradi[1] = "Igualdade inválida."; 
  ErrosGradi[2] = "Afastamento inválido.";
  ErrosGradi[3] = "Quantidade de campos inválida.";
  ErrosGradi[4] = "O Valor de x está fora da seção Tipo.";
  ErrosGradi[5] = "Atenção: Não há secão tipo com esta estaca final.";

  ErrosExpess[0] = "Estaca final inválida.";
  ErrosExpess[1] = "Igualdade inválida."; 
  ErrosExpess[2] = "Altura inválida.";
  ErrosExpess[3] = "Altura do pavimento fora dos limites (> 5.00m  ou < 0.10m).";

  ErrosEmpola[0] = "Estaca final inválida.";
  ErrosEmpola[1] = "Igualdade inválida."; 
  ErrosEmpola[2] = "% invalido";
  ErrosEmpola[3] = "% fora dos limites (< 1% ou > 90%)";

  ErrosCante[0] = "Estaca final inválida.";
  ErrosCante[1] = "Igualdade inválida."; 
  ErrosCante[2] = "INI 1 inválido."; 
  ErrosCante[3] = "FIM 1 inválido.";
  ErrosCante[4] = "INI 2 inválido."; 
  ErrosCante[5] = "FIM 2 inválido.";
  ErrosCante[6] = "INI 3 inválido."; 
  ErrosCante[7] = "FIM 3 inválido.";
  ErrosCante[8] = "Algum par INI/FIM está incompleto.";
  ErrosCante[9] = "O valor de deltax entre o início e o fim do  não pode ser menor que 0.001m.";
  ErrosCante[10] = "Faltam pontos para determinar o início ou o fim do canteiro.";

  ErrosSecTipo[0] = "Estaca final inválida.";
  ErrosSecTipo[1] = "Igualdade inválida."; 
  ErrosSecTipo[2] = "x inválido."; 
  ErrosSecTipo[3] = "y inválido.";
  ErrosSecTipo[4] = "Não há pontos suficientes para formar uma seção tipo, minimo 3 pontos";
  ErrosSecTipo[5] = "Algum par xy está incompleto.";
  ErrosSecTipo[6] = "O valor absoluto do deltax entre os pontos não pode ser menor que 0.001m.";
  ErrosSecTipo[7] = "É Obrigatorio existir um ponto (0,0) na seção tipo.";
  
  ErrosIgual[0] = "Estaca 1 inválida.";
  ErrosIgual[1] = "Estaca 2 inválida.";

  ErrosOAE[0] = "Estaca inicial inválida.";
  ErrosOAE[1] = "Igualdade inicial inválida.";
  ErrosOAE[2] = "Estaca Final inválida.";
  ErrosOAE[3] = "Igualdade final inválida.";
  ErrosOAE[4] = "Dados incompletos.É preciso definir as estacas iniciais e finais.";
  ErrosOAE[5] = "A estaca final deve estar depois da estaca inicial.";

  ErrosOAC[0] = "Estaca inválida.";
  ErrosOAC[1] = "Igualdade inválida";

  ErrosSecTri[0] = "Estaca Inválida.";
  ErrosSecTri[1] = "Igualdade Inválida.";
  ErrosSecTri[2] = "Afastamento 1 Inválido.";
  ErrosSecTri[3] = "Cota 1 Inválida.";
  ErrosSecTri[4] = "Afastamento 2 Inválido.";
  ErrosSecTri[5] = "Cota 2 Inválida.";
  ErrosSecTri[6] = "Afastamento 3 Inválido.";
  ErrosSecTri[7] = "Cota 3 Inválida.";
  ErrosSecTri[8] = "Afastamento 4 Inválido.";
  ErrosSecTri[9] = "Cota 4 Inválida.";
  ErrosSecTri[10] = "Afastamento 5 Inválido.";
  ErrosSecTri[11] = "Cota 5 Inválida.";

  ErrosHPavim[0] = "Estaca Inválida.";
  ErrosHPavim[1] = "Altura de pavimento Inválida.";
  ErrosHPavim[2] = "Faltam Campos.";

  ErrosTiposSolos[0] = "Código de solo inválido";
  ErrosTiposSolos[1] = "Profundidade tem que ser um número";
  ErrosTiposSolos[2] = "Profundidade fora dos limites (<0.00 ou > 50.0m)";
  ErrosTiposSolos[3] = "Estaca Inválida";
  ErrosTiposSolos[4] = "Cota do furo inválida";

  ErrosFurosSondagem[0] = "Tipo de solo inexistente";

  ErrosLimpeza[0] = "Estaca inválida";
  ErrosLimpeza[1] = "Igualdade inválida";
  ErrosLimpeza[2] = "Altura inválida";
  ErrosLimpeza[3] = "Exesso lateral inválido";
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
