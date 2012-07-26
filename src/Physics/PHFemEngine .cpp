/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHFemEngine.h>
#include <Physics/PHFemMesh.h>

using namespace std;
namespace Spr{

//static std::ofstream *dlog;

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// FEMEngine
PHFemEngine::PHFemEngine(){ 
	freq	=	0;
	freqN	=	1000;
}

void PHFemEngine::Step(){
	double dt = GetScene()->GetTimeStep();
	// Step  (freqN)回に一回だけ呼び出す
	if(freq > freqN){
		for(size_t i=0; i<meshes.size(); ++i){
			meshes[i]->Step(dt * freqN);
		}
		freq = 0;
	}
	freq +=1;
}

void PHFemEngine::Clear(){
}

bool PHFemEngine::AddChildObject(ObjectIf* o){
	// --- --- --- --- --- --- ---
	// アクチュエータの場合
	PHFemMesh* mesh = o->Cast();
	if(mesh){
		meshes.push_back(mesh);
		return true;
	}
	return false;
}

}
