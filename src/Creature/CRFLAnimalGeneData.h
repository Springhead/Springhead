/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef CRFLANIMALGENEDATA_H
#define CRFLANIMALGENEDATA_H

#include <Springhead.h>
#include <Creature/CRFourLegsAnimalBody.h>

using namespace Spr;

struct CRFLAnimalGeneData{

public:
	Quaterniond goalDir;
	enum {
		GEN_QUATERNIOND=0,
		GEN_DOUBLE
	} geneType;

};
/*
bool equal(const std::vector<CRFLAnimalGeneData>& a, const std::vector<CRFLAnimalGeneData>& b){
	if (a.size() != b.size()) return false;
	for(size_t i=0; i<a.size(); ++i)
		if (a[i].goalDir != b[i].goalDir || a[i].geneType != b[i].geneType) return false;
	return true;
}
///	ƒNƒ‰ƒX‚Ì”äŠr
bool operator == (const std::vector<CRFLAnimalGeneData>& a, const std::vector<CRFLAnimalGeneData>& b){
	return equal(a,b);
}
///	ƒNƒ‰ƒX‚Ì”äŠr
bool operator != (const std::vector<CRFLAnimalGeneData>& a, const std::vector<CRFLAnimalGeneData>& b){
	return !equal(a,b);
}
*/
#endif