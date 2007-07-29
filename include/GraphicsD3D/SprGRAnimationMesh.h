/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprGRAnimationMesh.h
 *	@brief キャラクタアニメーション用メッシュ
*/

#ifndef SPR_GRAnimationMesh_H
#define SPR_GRAnimationMesh_H

#include <Graphics/SprGRFrame.h>

namespace Spr{;

///	@brief GRAnimationMeshのデスクリプタ．
struct GRAnimationMeshDesc /*: GRVisualDesc*/{
	std::string filename;		///< 読み込むファイル名(.x)
};


/**	@brief	グラフィックスで使うキャラクタアニメーション表示用のMesh */
struct GRAnimationMeshIf: public GRVisualIf{
	IF_DEF(GRAnimationMesh);
	virtual void SetMotion(const std::string& name)=0;		///< モーションを選択
	virtual void SetTime(double time)=0;					///< モーションの時刻を設定(単位は秒)
};


}
#endif
