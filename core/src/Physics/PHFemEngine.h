/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_FEMENGINE_H
#define PH_FEMENGINE_H

#include "PHEngine.h"
#include "PHFemMesh.h"
#include "PHFemMeshNew.h"

//For the FEM interface implementation
#include "PHFemCollision.h"
#include <Physics/PHContactPoint.h>
#include <Physics/PHConstraintEngine.h>

#define THERMO_MODE 0
#define VIBRATION_MODE 1

namespace Spr{

// 現状はメッシュのステップを呼ぶのみ
// 複数のメッシュでのやりとりをおこなう場合は
// PHFemEngineに新しいエンジンを持たせることで対応する予定


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
// FEMエンジン
// 

class PHFemMeshPair {
public:
	PHFemMeshNew* mesh[2];
	
	PHFemMeshPair();
	
};

class PHFemEngine : public PHEngine{
	bool bVibrationTransfer;
	bool bThermalTransfer;
public:
	SPR_OBJECTDEF(PHFemEngine);
	double fdt;
	std::vector<PHFemMesh*> meshes;
	std::vector<PHFemMeshNew*> meshes_n;
	std::vector<PHFemMeshPair*> meshPairs;
	double heatTransferRatio;
	PHFemEngine();
	void setheatTransferRatio(double setheatTransferRatio);
	void SetVibrationTransfer(bool bEnable);	///<	For haptic vibration simulation
	void SetThermalTransfer(bool bEnable);		///<	For thermal simulation
	int  GetPriority() const {return SGBP_NONE; }	//	Must call this engine explicitly
	void Step();
	
	bool AddMeshPair(PHFemMeshNewIf* m0, PHFemMeshNewIf* m1);
	bool RemoveMeshPair(PHFemMeshNewIf* m0, PHFemMeshNewIf* m1);
	void ThermalTransfer();
	void HeatTrans(PHFemMeshPair* mp);

	void Clear();
	virtual bool AddChildObject(ObjectIf* o);
	void SetTimeStep(double dt);
	double GetTimeStep();

	//For the multiple object implemenation VARIABLES
	bool **contactMatrix;    //contact matrix to surf throug the object's contact
	PHFemCollision ***femColArray;  //saves the objects to control their FEM collision interaction

	//For the multiple object implemenation FUNCTIONS
	PHFemMeshNew* GetMeshByName(std::string name);
	void	FEMSolidMatchRefresh();   
	void	VibrationTransfer();
	void	clearContacts();  
	void	InitContacts();
	void	ClearContactVectors();

	int NMesh() { return (int)meshes.size();  }
	PHFemMeshIf* GetMesh(int i) { return meshes[i]->Cast(); }
	int NMeshNew() { return (int)meshes_n.size(); }
	PHFemMeshNewIf* GetMeshNew(int i) { return meshes_n[i]->Cast(); }
};

}

#endif
