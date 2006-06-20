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
	std::vector<Vec3f> positions;		///<	頂点の座標
	std::vector<Vec3f> normals;			///<	頂点の法線
	std::vector<Vec4f> colors;			///<	頂点の色
	std::vector<Vec2f> texCoords;		///<	テクスチャUV
	std::vector<size_t> faces;		///<	面を構成する頂点の番号．
};


/**	@brief	グラフィックスで使う表示用のMesh */
struct GRMeshIf: public GRVisualIf{
	IF_DEF(GRMesh);
};


//@}
}
#endif
