/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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

/**	
	@brief 表示用のBlendMesh(GRBlendMesh)のデスクリプタ．
	
　　 １頂点に適用できるブレンド変換行列は、4個まで．
　　 そのため、blends、matrixIndices は、Vec4f型としてある．
　　 ただし、インデックス付きブレンド変換行列であるため、あらかじめ登録できるブレンド変換行列に制限はなし．
	
 */
struct GRBlendMeshDesc{	
	std::vector<Vec3f> 	positions;			///< 頂点の座標
	std::vector<Vec3f> 	normals;			///< 頂点の法線
	std::vector<Vec4f> 	colors;				///< 頂点の色
	std::vector<Vec2f> 	texCoords;			///< テクスチャUV
	std::vector<size_t> faces;				///< 面を構成する頂点インデックス（四角形は三角形に分割）
	std::vector<Vec4f>  blends;				///< 各頂点の重み（1：N = 頂点：blend）
    std::vector<Vec4f>  matrixIndices;		///< 変換行列インデックス（1:N = 頂点：matrix）
	std::vector<Vec4f>  numMatrix;			///< 各頂点に対するブレンド変換行列の数
};


/**	@brief	グラフィックスで使う表示用のBlendMesh */
struct GRBlendMeshIf: public GRVisualIf{
	SPR_IFDEF(GRBlendMesh);
};


//@}
}
#endif
