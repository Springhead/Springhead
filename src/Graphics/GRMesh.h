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
	std::vector<unsigned int> list;				///< ディスプレイリストの識別子
	GRRenderIf* render;
	
	/// 頂点フォーマット GRVertexElement に合わせ、ディスプレイリストを作成する
	void CreateList(GRRenderIf* r);
	/// ディスプレイリストの管理を行う（1meshに対し、materialを複数持つ場合は、material毎にListを作成する）
	void ListManage(void* vtx);
	void LoadTexture();
public:
	OBJECT_DEF(GRMesh);
	ACCESS_DESC(GRMesh);

	std::vector<GRMaterialDesc> material;		///< マテリアル
	std::vector<int> materialList;				///< マテリアルのインデックスリスト
	std::vector<GRTextureDesc>  texture;		///< テクスチャ
	/**
	   facesと、三角形分割する前のXファイルで指定された面との関連付け \n
	  （Xファイルからロードされた面は、すべて三角形分割されているため、
	   どの面がどのマテリアルを適用するのか判断するためのもの）			*/
	std::vector<unsigned int> elementIndex;
	std::vector<int> originalFaces;			///< 面を構成するための頂点インデックス（三角形分割前の面に対するインデックス）
	std::vector<int> faceNormals;			///< 法線インデックス

	GRMesh(const GRMeshDesc& desc=GRMeshDesc());
	void Render(GRRenderIf* r);
	void Rendered(GRRenderIf* r);
};
}
#endif
