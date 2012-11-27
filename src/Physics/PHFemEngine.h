/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_FEMENGINE_H
#define PH_FEMENGINE_H

#include <Physics/PHEngine.h>
#include <Physics/PHFemMesh.h>
#include <Physics/PHFemMeshNew.h>

namespace Spr{;

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// FEMエンジン
// 

class PHFemEngine : public PHEngine{
public:
	SPR_OBJECTDEF(PHFemEngine);
	double fdt;
	std::vector<PHFemMesh*> meshes;
	std::vector<PHFemMeshNew*> meshes_n;

	PHFemEngine();

	int  GetPriority() const {return SGBP_NONE; }//SGBP_DYNAMICALSYSTEM;}
	void Step();
	void Clear();
	virtual bool AddChildObject(ObjectIf* o);
	void SetTimeStep(double dt);
	double GetTimeStep();
};

}

#endif
