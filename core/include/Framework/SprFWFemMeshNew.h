/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FW_FEMMESH_NEW_IF_H
#define SPR_FW_FEMMESH_NEW_IF_H

#include <Framework/SprFWObject.h>
#include <Physics/SprPHFemMeshNew.h>

namespace Spr{;
/** \addtogroup gpFramework */
//@{

struct FWFemMeshNewDesc: public FWObjectDesc{
	unsigned kogetex; //焦げテクスチャの枚数
	std::string meshRoughness;			///< tetgenで生成するメッシュの粗さ
	FWFemMeshNewDesc();
	bool spheric;
	void Init();
	unsigned texture_mode;
};

/** 
 */
struct FWFemMeshNewIf : FWObjectIf{
	SPR_IFDEF(FWFemMeshNew);
	PHFemMeshNewIf* GetPHFemMesh();
	void SetTexmode(unsigned mode);
	void EnableDrawEdgeCW(bool flag);
	void Settexmode1Map(float temp);
};

//@}
}

#endif
