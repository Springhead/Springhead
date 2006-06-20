/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <FileIO/FIDocScene.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;
void SPR_DLL RegisterLoaderForCollision(FILoadScene* l){
	REGISTER_LOADER(l, CDMesh);
	REGISTER_LOADER(l, CDSphere);
	REGISTER_LOADER(l, CDPhysicalMaterial);
}

//-------------------------------------------------------------------
//	Saver
//-------------------------------------------------------------------

void SPR_DLL RegisterSaverForCollision(FISaveScene* l){
	REGISTER_SAVER(l, CDPhysicalMaterial);
}
}

void RegisterLoaderForCollision(Spr::FILoadScene* l){
	Spr::RegisterLoaderForCollision(l);
}
void SPR_DLL RegisterSaverForCollision(Spr::FISaveScene* s){
	Spr::RegisterSaverForCollision(s);
}



