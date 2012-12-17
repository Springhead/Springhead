/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprPHFemMeshNew.h
 *	@brief 剛体
*/
#ifndef SPR_PH_FEMMESH_NEW_IF_H
#define SPR_PH_FEMMESH_NEW_IF_H

#include <Foundation/SprObject.h>

/**	\addtogroup gpPhysics	*/
//@{
namespace Spr{;

/// PHFemMeshNewへのインタフェース
struct PHFemMeshNewIf : public SceneObjectIf{
	SPR_IFDEF(PHFemMeshNew);
};

///	FemMeshのステート
struct PHFemMeshNewState{};

///	FemMeshのディスクリプタ
struct PHFemMeshNewDesc: public PHFemMeshNewState{
	SPR_DESCDEF(PHFemMeshNew);
	std::vector<Vec3d> vertices;
	std::vector<int> tets;
	std::vector<int> faces;
};

/// Femの共通計算部分
struct PHFemIf : public SceneObjectIf{
	SPR_IFDEF(PHFem);
};

/// Femのデスクリプタ
struct PHFemDesc{
};

/// 振動計算
struct PHFemVibrationIf : public PHFemIf{
	SPR_IFDEF(PHFemVibration);
};

/// 振動計算のデスクリプタ
struct PHFemVibrationDesc : public PHFemDesc{
	SPR_DESCDEF(PHFemVibration);
	double young;		///< ヤング率(Pa, N/m2)
	double poisson;		///< ポアソン比(*一様な立方体の場合、-1 <= v <= 0.5)
	double density;		///< 密度(kg/m3)
	double alpha;		///< 粘性減衰率
	double beta;		///< 構造減衰率
	PHFemVibrationDesc(){
		young = 1.0;
		poisson = 0.5;
		density = 1.0;
		alpha = 0.0;
		beta = 0.0;
	}
};


//@}

}	//	namespace Spr
#endif