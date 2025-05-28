#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "carqterreno.h"
#include "cponto.h"
#include <string>
#include "mat.h"
#include "estaca.h"
#include "clpontos.h"
#include "perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include <list>
#include <set>
#include <vector>
#include <algorithm>
#include "cdeltasuperficie.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include <map>
#include "CAcidentesTopograficos.h"
#include "CSegmento.h"
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "CEquipDrenagem.h"
#include "superficie.h"
#include "xygreide.h"
#include "meleditbox.h"
#include "cdpopuppontos.h"
#include "supercstring.h"
#include "carqcurhorizontais.h" 
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "ccurhorizontal.h"
#include "ccurhorizontais.h"
#include "carqsecoesavulsas.h"
#include "csecaoavulsa.h"
#include "csecoesavulsas.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include "CLembrete.h"
#include "CRadar.h"
#include "CArqAciTopograficos.h"
#include "CTrecho.h"

CTrecho::CTrecho(const std::string& pNomeTrecho) : NomeTrecho(pNomeTrecho), 
                                                   SecoesTipo(CString(pNomeTrecho.c_str())),
                                                   CurvasHorizontais(CString(pNomeTrecho.c_str()), SecoesTipo),
                                                   SecoesAvulsas(CString(pNomeTrecho.c_str())),
                                                   Alargamentos(CString(pNomeTrecho.c_str())),
                                                   ArquivoAciTopograficos(CString(pNomeTrecho.c_str())),
                                                   mapCurvasHorizontaisTS(CurvasHorizontais.PegaMapCurHorizontaisTS()),
                                                   mapCurvasHorizontaisPI(CurvasHorizontais.PegaMapCurHorizontaisPI()),
                                                   mapCurvasHorizontaisCC(CurvasHorizontais.PegaMapCurHorizontaisCC()),
                                                   SuperficieTerr(CString(pNomeTrecho.c_str()), false)
{
}

CTrecho::~CTrecho()
{
}
