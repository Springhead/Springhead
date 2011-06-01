/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHFEMMESH_H
#define PHFEMMESH_H

#include <SprPhysics.h>
#include <Foundation/Object.h>
#include <Physics/PHScene.h>

namespace Spr{;

///	有限要素法シミュレーションのためのメッシュ(4面体メッシュ)

class PHFemMesh: public SceneObject{
public:
	struct Integral{
		double volume;		//	\int {N} {N}^T dV
		double gradient;	//	\int d{N}/dx d{N}^T/dx + d{N}/dy d{N}^T/dy + d{N}/dz d{N}^T/dz  dV
		double surface;		//	\int {N} {N}^T dS	表面の頂点のみが値を持つ
	};
	struct FemVertex{
		Vec3d pos;
		std::vector<int> tets;
		std::vector<int> neighbors;
		std::vector<Integral> integralNeighbors;
		Integral integralSelf;
		FemVertex();
	};
	struct Tet{
		int vertices[4];
	};
	class Face{
		int sorted[3];
	public:
		int vertices[3];
		void Update();
		bool operator < (const Face& f2);
		bool operator == (const Face& f2);
	};
	std::vector<FemVertex> vertices;
	std::vector<int> surfaceVertices;
	std::vector<Face> surfaceFaces;
	std::vector<Tet> tets;
	SPR_OBJECTDEF(PHFemMesh);
	PHFemMesh(const PHFemMeshDesc& desc=PHFemMeshDesc(), SceneIf* s=NULL);
	//	デスクリプタのサイズ
	virtual size_t GetDescSize() const { return sizeof(PHFemMeshDesc); };
	//	デスクリプタの読み出し(コピー版)
	virtual bool GetDesc(void* desc) const ;
	// デスクリプタの設定
	virtual void SetDesc(const void* desc);
	

};




}	//	namespace Spr
#endif
