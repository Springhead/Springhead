/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef GRBlendMesh_H
#define GRBlendMesh_H

#include <Graphics/SprGRBlendMesh.h>
#include <Graphics/GRFrame.h>

namespace Spr{;

class GRMaterial;

/** @class GRBlendMesh
	@brief グラフィックスシーングラフでの座標系を表す．	*/
class GRBlendMesh: public GRVisual, public GRBlendMeshDesc{
	GRRenderIf* render;							///< レンダラー				
	std::vector<unsigned int> list;				///< ディスプレイリストの識別子
			
	/// 頂点フォーマット、シェーダフォーマット に合わせ、ディスプレイリストを作成する．
	void CreateList(GRRenderIf* r);
public:
	SPR_OBJECTDEF(GRBlendMesh);
	ACCESS_DESC(GRBlendMesh);

	std::vector< UTRef<GRMaterial> > material;				///< マテリアル
	std::vector<int>                 materialList;			///< マテリアルのインデックスリスト
	/**
	   facesと、三角形分割する前のXファイルで指定された面との関連付け \n
	  （Xファイルからロードされた面は、すべて三角形分割されているため、
	   どの面がどのマテリアルを適用するのか判断するためのもの）			*/
	std::vector<int> originalFaces;				///< 三角形分割前の面に対応する頂点
	std::vector<unsigned int> originalFaceIds;	///< 三角形に対応する分割前の面の番号
	std::vector<int> faceNormals;				///< 法線インデックス
	
	std::vector<Affinef>				blendMatrix;			///< ブレンド変換行列

	
	GRBlendMesh(const GRBlendMeshDesc& desc=GRBlendMeshDesc());
	~GRBlendMesh();
	void Render(GRRenderIf* r);
	void Rendered(GRRenderIf* r);
};
}
#endif
