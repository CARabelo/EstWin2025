#include "stdafx.h"
#include "CPonto.h"
#include "CRetangulo.h"

CDRetangulo::~CDRetangulo(void)
{
}

void CDRetangulo::AjustaMaior(const Ponto& pPonto)
{
    if(pPonto.x < x1) x1 = pPonto.x;
    if(pPonto.x > x2) x2 = pPonto.x;
    if(pPonto.y < y1) y1 = pPonto.y;
    if(pPonto.y > y2) y2 = pPonto.y;
}
