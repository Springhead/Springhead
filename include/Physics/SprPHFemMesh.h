/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprPHFemMesh.h
 *	@brief FEMシミュレーションのための四面体メッシュ
*/
#ifndef SPR_PHFemMeshIf_H
#define SPR_PHFemMeshIf_H


/**	\addtogroup gpPhysics	*/
//@{
namespace Spr{;

///	FemMeshのステート
struct PHFemMeshState{
};
///	FemMeshのディスクリプタ
struct PHFemMeshDesc: public PHFemMeshState{
	std::vector<Vec3d> vertices;
	std::vector<int> tets;
	PHFemMeshDesc();
	void Init();
};

///	FemMeshのインタフェース
struct PHFemMeshIf : public SceneObjectIf{
	SPR_IFDEF(PHFemMesh);
};

//@}

}	//	namespace Spr
#endif
