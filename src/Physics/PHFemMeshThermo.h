/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHFEMMESHTHERMO_H
#define PHFEMMESHTHERMO_H

#include <Physics/PHFemMesh.h>

namespace Spr{;

///	熱のFEM用のメッシュ
class PHFemMeshThermo: public PHFemMesh{
public:
	SPR_OBJECTDEF(PHFemMeshThermo);

	//	頂点
	struct StateVar{
		double temperature;
	};
	struct Coeff{
	};
	std::vector<StateVar> vertexVars;
	std::vector<Coeff> edgeCoeffs;
	
	PHFemMeshThermo(const PHFemMeshThermoDesc& desc=PHFemMeshThermoDesc(), SceneIf* s=NULL);
	///	デスクリプタのサイズ
	virtual size_t GetDescSize() const { return sizeof(PHFemMeshThermoDesc); };
	///	デスクリプタの読み出し(コピー版)
	virtual bool GetDesc(void* desc) const;
	///デスクリプタの設定。
	virtual void SetDesc(const void* desc);
	//熱伝導計算本体
	
};




}	//	namespace Spr
#endif
