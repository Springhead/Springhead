/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef GRMesh_H
#define GRMesh_H

#include <SprGraphics.h>
#include "GRFrame.h"

namespace Spr{;

/**	@class	GRMesh
    @brief	グラフィックスシーングラフでの座標系を表す． */
class GRMesh: public InheritGRVisual<GRMeshIf, GRVisual>, public GRMeshDesc{
	std::vector<unsigned int> list;		///< ディスプレイリストの識別子
	GRRenderIf* render;
	/// 頂点フォーマットGRVertexElement に合わせ、ディスプレイリストを作成する
	void CreateList(GRRenderIf* r);
	/// Xファイルから MeshMaterialList設定された場合、マテリアル毎にsub mesh単位でディスプレイリストを作成する
	void CreateSubList(void* vtx);
public:
	OBJECT_DEF(GRMesh);
	ACCESS_DESC(GRMesh);
	GRMesh(const GRMeshDesc& desc=GRMeshDesc());
	void Render(GRRenderIf* r);
	void Rendered(GRRenderIf* r);
};
}
#endif
