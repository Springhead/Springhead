/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHFemEngine.h>
//#include <Physics/PHFemMesh.h>
//#include <Physics/PHFemMeshNew.h>

using namespace std;
namespace Spr{
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
// FEMEngine
PHFemEngine::PHFemEngine(){
	fdt = 0.02;
}

void PHFemEngine::Step(){
	/// 旧メッシュの更新
	for(size_t i = 0; i < meshes.size(); ++i){
		meshes[i]->Step(GetTimeStep());
	}

	/// 新メッシュの更新
	for(size_t i = 0; i < meshes_n.size(); ++i){
		meshes_n[i]->Step(GetTimeStep());
	}
}

void PHFemEngine::Clear(){}

void PHFemEngine::SetTimeStep(double dt){
	fdt = dt;
}

double PHFemEngine::GetTimeStep(){
	return fdt;
}

bool PHFemEngine::AddChildObject(ObjectIf* o){
	PHFemMesh* mesh = o->Cast();
	if(mesh){
		meshes.push_back(mesh);
		return true;
	}
	PHFemMeshNew* mesh_n = o->Cast();
	if(mesh_n){
		meshes_n.push_back(mesh_n);
		return true;
	}
	return false;
}

}
