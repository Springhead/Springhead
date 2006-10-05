/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef GRSphere_H
#define GRSphere_H

#include <SprGraphics.h>
#include "GRFrame.h"

namespace Spr{;

class GRMaterial;
/**	@class	GRSphere
    @brief	グラフィックスシーングラフでの座標系を表す． */
class GRSphere: public InheritGRVisual<GRSphereIf, GRVisual>, public GRSphereDesc{
	unsigned int list;				///< ディスプレイリストの識別子
	GRRenderIf* render;
	
	/// 頂点フォーマット GRVertexElement に合わせ、ディスプレイリストを作成する
	void CreateList(GRRenderIf* r);
public:
	OBJECT_DEF(GRSphere);
	ACCESS_DESC(GRSphere);

	std::vector< UTRef<GRMaterial> > material;	///< マテリアル
	std::vector<int> materialList;				///< マテリアルのインデックスリスト
	std::vector<GRTextureDesc>  texture;		///< テクスチャ
	/**
	   facesと、三角形分割する前のXファイルで指定された面との関連付け \n
	  （Xファイルからロードされた面は、すべて三角形分割されているため、
	   どの面がどのマテリアルを適用するのか判断するためのもの）			*/
	std::vector<unsigned int> elementIndex;
	std::vector<int> originalFaces;			///< 面を構成するための頂点インデックス（三角形分割前の面に対するインデックス）
	std::vector<int> faceNormals;			///< 法線インデックス

	GRSphere(const GRSphereDesc& desc=GRSphereDesc());
	~GRSphere();
	void Render(GRRenderIf* r);
	void Rendered(GRRenderIf* r);
	virtual bool AddChildObject(ObjectIf* o);
};
}
#endif
