/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
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
#include <Springhead.h>

namespace Spr{;

///	@brief 表示用のMesh(GRMesh)のデスクリプタ．
struct GRMeshDesc{
	std::vector<Vec3f> positions;				///< 頂点の座標
	std::vector<Vec3f> normals;					///< 頂点の法線
	std::vector<Vec4f> colors;					///< 頂点の色
	std::vector<Vec2f> texCoords;				///< テクスチャUV
	std::vector<size_t> faces;					///< 面を構成する頂点の番号（Xファイルからのロードの場合、四角形は三角形に分割）

	std::vector<GRMaterialDesc> material;		///< マテリアル
	std::vector<int> materialList;				///< マテリアルのインデックスリスト（XファイルのMeshMaterialListの番号をそのままロード）
	/**
	   facesと、三角形分割する前のXファイルで指定された面との関連付け		\n
	  （1 mesh をマテリアル毎にさらに mesh 分割し、各 sub mesh に対し、  　	\n
　　　　　　　 マテリアル設定；												\n
　　　　　　　 DisplayListでレンダリング；									\n
		を繰り返す。Xファイルからロードされた面は、すべて三角形分割されるため、
		どの面がどのマテリアルを適用するのか判断するために、elementIndex が必要。） */
	std::vector<size_t> elementIndex;	
};


/**	@brief	グラフィックスで使う表示用のMesh */
struct GRMeshIf: public GRVisualIf{
	IF_DEF(GRMesh);
};


//@}
}
#endif
