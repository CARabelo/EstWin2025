#include "stdafx.h"
#include <list>
#include <set>
#include "Utm_Coord_3d.h"
#include <stdio.h>
/*
Utm_Coord_3d::Utm_Coord_3d(double X, double Y, double Z, short Zone, bool hemi_n,int pNumPon,const std::string& pObs) :
	x(X), y(Y), z(Z), zone(Zone), hemisphere_north(hemi_n),NumPonto(pNumPon),Observacoes(pObs)  {}

std::string Utm_Coord_3d::toString() {
	char buff[100];
	if( hemisphere_north ) {
	    sprintf_s( buff, " n: %g e: %g h: %g zone: %d north", y, x, z, zone );
	} else {
	    sprintf_s( buff, " n: %g e: %g h: %g zone: %d south", y, x, z, zone );
	}
	return std::string( buff );
}
*/