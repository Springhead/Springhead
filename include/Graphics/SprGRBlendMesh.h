/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprGRBlendMesh.h
 *	@brief スキンメッシュなどでベースとなる頂点ブレンディング用メッシュ
*/

/**	\addtogroup	gpGraphics	*/
//@{

#ifndef SPR_GRBlendMesh_H
#define SPR_GRBlendMesh_H

#include <Graphics/SprGRFrame.h>

namespace Spr{;

///	@brief 表示用のBlendMesh(GRBlendMesh)のデスクリプタ．
struct GRBlendMeshDesc{
	std::vector<Vec3f> 	positions;				///< 頂点の座標
	std::vector<Vec3f> 	normals;				///< 頂点の法線
	std::vector<Vec4f> 	colors;					///< 頂点の色
	std::vector<Vec2f> 	texCoords;				///< テクスチャUV
	std::vector<size_t> faces;					///< 面を構成する頂点インデックス（四角形は三角形に分割）
	std::vector<float> 	blend;					///< 各頂点の重み
};


/**	@brief	グラフィックスで使う表示用のBlendMesh */
struct GRBlendMeshIf: public GRVisualIf{
	IF_DEF(GRBlendMesh);
};


//@}
}
#endif
