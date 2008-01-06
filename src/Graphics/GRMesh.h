/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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

class GRMaterial;

/**	@class	GRMesh
    @brief	グラフィックスシーングラフでの座標系を表す． */
class GRMesh: public GRVisual, public GRMeshIfInit, public GRMeshDesc{
	UTRef<GRRenderIf> render;			///< レンダラー
	unsigned int list;					///< ディスプレイリストの識別子

	/// 頂点バッファの作成
	void MakeBuffer();
	/// 頂点バッファの描画
	void DrawBuffer(void* vtx);
public:
	/// ディスプレイリストを作成する．
	void CreateList(GRRenderIf* r);
private:
	///	頂点フォーマットに合った頂点バッファ
	float* vtxs;
	int nVtxs;
	float* blendedVtxs;
	const GRVertexElement* vtxFormat;
	int stride, normalOffset, positionOffset, texOffset;
public:
	OBJECTDEF(GRMesh, GRVisual);
	ACCESS_DESC(GRMesh);

	bool tex3d;												///< 3次元テクスチャの場合 true
	std::vector< UTRef<GRMaterial> > material;				///< マテリアル
	std::vector<int>                 materialList;			///< マテリアルのインデックスリスト

	struct SkinWeight{
		GRFrame* frame;
		Affinef offset;
		std::vector<unsigned> indices;
		std::vector<float> weights;
		SkinWeight(): frame(NULL){}
	};
	std::vector<SkinWeight>	skinWeights;					///< スキンMeshの場合のskinWeights

	float* GetVertex(int i=0){								///< i番目の頂点バッファを返す
		return vtxs + stride * i;
	}
	int GetNVertex(){ return nVtxs; }						///< 頂点数
	const GRVertexElement* GetVertexFormat(){				///< 頂点フォーマットを返す。
		return vtxFormat;
	}
	bool IsTex3d(){ return tex3d; }							///< 3次元テクスチャなら true
	int GetStride(){ return stride; }						///< 1頂点のデータがfloat何個分か
	int GetNormalOffset(){ return normalOffset; }			///< 法線のオフセット(float何個分)
	int GetPositionOffset(){ return positionOffset; }		///< 位置のオフセット(float何個分)
	int GetTexOffset(){ return texOffset; }					///< テクスチャ座標のオフセット(float何個分)

	/**
	   facesと、三角形分割する前のXファイルで指定された面との関連付け \n
	  （Xファイルからロードされた面は、すべて三角形分割されているため、
	   どの面がどのマテリアルを適用するのか判断するためのもの）			*/
	std::vector<size_t> originalFaces;			///< 三角形分割前の面に対応する頂点
	std::vector<unsigned int> originalFaceIds;	///< 三角形に対応する分割前の面の番号
	std::vector<int> faceNormals;				///< 法線インデックス

	GRMesh(const GRMeshDesc& desc=GRMeshDesc());
	~GRMesh();
	void Render(GRRenderIf* r);
	void Rendered(GRRenderIf* r);
	
	virtual bool AddChildObject(ObjectIf* o);
	///	子オブジェクトの数
	virtual size_t NChildObject() const ;
	///	子オブジェクトの取得
	virtual ObjectIf* GetChildObject(size_t pos) ;
	///
	void Print(std::ostream& os) const ;

};

}
#endif
