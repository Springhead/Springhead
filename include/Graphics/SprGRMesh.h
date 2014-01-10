/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprGRMesh.h
 *	@brief メッシュ
*/

/**	\addtogroup	gpGraphics	*/
//@{

#ifndef SPR_GRMesh_H
#define SPR_GRMesh_H

#include <Graphics/SprGRFrame.h>

namespace Spr{;

struct GRMeshFace{
	int	nVertices;	///< 3 or 4
	int	indices[4];
};

struct GRSkinWeightIf: public GRVisualIf{
	SPR_IFDEF(GRSkinWeight);
};

struct GRSkinWeightDesc{
	SPR_DESCDEF(GRSkinWeight);
	Affinef offset;
	std::vector<unsigned> indices;
	std::vector<float> weights;
};

/**	@brief	グラフィックスで使う表示用のMesh */
struct GRMeshIf: public GRVisualIf{
	SPR_IFDEF(GRMesh);

	/// 頂点の個数
	int		NVertex();
	/// 面の個数(3角形分割後)
	int		NTriangle();
	/// 面の個数(3角形分割前)
	int		NFace();
	
	/// 頂点配列
	Vec3f*	GetVertices();
	/// 法線配列
	Vec3f*	GetNormals();
	/// 頂点色配列
	Vec4f*	GetColors();
	/// テクスチャ座標配列
	Vec2f*	GetTexCoords();
	/// 頂点インデックス配列
	GRMeshFace*	GetFaces();
	/// 面法線
	GRMeshFace* GetFaceNormals();
	/// マテリアルインデックス配列
	int*	GetMaterialIndices();

	/** @brief 右手/左手座標系の切り換え
		頂点座標のZ成分の符号を反転する．
	 */
	void SwitchCoordinate();

	/// 3DテクスチャのOn/Off
	void EnableTex3D(bool on = true);
	bool IsTex3D();

	/// スキンウェイトの作成
	GRSkinWeightIf* CreateSkinWeight(const GRSkinWeightDesc& desc);

	/** bounding boxを計算
		@param bbmin
		@param bbmax
		@param aff		AABBを定義する座標系との変換
	 */
};

///	@brief 表示用のMesh(GRMesh)のデスクリプタ．
struct GRMeshDesc : GRVisualDesc{
	SPR_DESCDEF(GRMesh);
	std::vector<Vec3f>		vertices;			///< 頂点の座標
	std::vector<GRMeshFace>	faces;				///< 面. 面を構成する頂点数と頂点インデックス配列
	std::vector<Vec3f>		normals;			///< 法線
	std::vector<GRMeshFace>	faceNormals;		///< 面の法線
	std::vector<Vec4f>		colors;				///< 頂点の色
	std::vector<Vec2f>		texCoords;			///< テクスチャUV	
	std::vector<int>		materialList;		///< マテリアルリスト
};

//@}
}
#endif
