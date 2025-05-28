// Aresta.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "cponto.h"
#include <list>
using std::list;
#include <set>
using std::set;
#include "Aresta.h"
#include "cdeltasuperficie.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAresta

CAresta::CAresta() : pDeltaSuperficie(NULL),Feito(false) {};
CAresta::CAresta(CDeltaSuperficie* ppDeltaSuper) : pDeltaSuperficie(ppDeltaSuper),Feito(false) {};


/////////////////////////////////////////////////////////////////////////////
// CAresta message handlers
