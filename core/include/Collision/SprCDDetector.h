/*
 *  Copyright (c) 2003-2017, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_CDDETECTORIF_H
#define SPR_CDDETECTORIF_H
#include <Foundation/SprObject.h>
#include <Collision/SprCDShape.h>

namespace Spr{;


/**	\defgroup	gpShape	形状・接触判定クラス	*/	
//@{
/**	@file SprCDDetector.h
	接触判定アルゴリズム
 */	


///	形状ペアの基本クラス
struct CDShapePairIf : public ObjectIf {
	SPR_IFDEF(CDShapePair);
	/// 形状を取得する
	CDShapeIf* GetShape(int i);
	///	Shapeの姿勢を取得する
	Posed GetShapePose(int i);
	///	最近傍点を取得する
	Vec3d GetClosestPointOnWorld(int i);
	///	接触法線の取得
	Vec3d GetNormal();
};
/** 直方体のディスクリプタ */
struct CDShapePairState{
	SPR_DESCDEF(CDShapePair);
	CDShapePairState():depth(0), lastContactCount((unsigned)-2){}
	Vec3d normal;				///<	衝突の法線(0から1へ) (Global)
	double depth;				///<	衝突の深さ：最近傍点を求めるために，2物体を動かす距離．
	unsigned lastContactCount;	///<	最後に接触した時刻
	unsigned contactDuration;	///<	接触時間
};


//@}

}

#endif
