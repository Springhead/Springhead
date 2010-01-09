/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHMOTOR_H
#define PHMOTOR_H

#include <SprPhysics.h>
#include <Physics/PhysicsDecl.hpp>

namespace Spr{;

class PHScene;
class PHConstraintEngine;
class PHJoint1D;

/* 定義不要？
	struct SpatialVectorArray2{
	SpatialVector v[2];
	SpatialVector& operator[](int i){
		return v[i];
	}
};*/
struct PH3ElementSt{
	//SpatialVector xs[2];			///< (3要素モデル専用)ばね部の距離
};

struct PH3ElementCommonData: public PH3ElementSt{
	SpatialVector xd;				///< (3要素モデル専用)ダンパ部の距離
	SpatialVector ws;				///< (3要素モデル専用)ばね部の速さ
	//SpatialVector fs;				///< (3要素モデル専用)ばね部の力
};

/**
	関節コントローラ．インタフェースは持たない．
 */

class PHMotor1D : public PH3ElementCommonData{
public:
	PHJoint1D*		joint;
	bool			yieldFlag;

	virtual void	SetupLCP();
	virtual void	IterateLCP();
	PHMotor1D();
private:
	double  A, Ainv, dA, b, db;
	double  fMaxDt;			///< 関節の出せる力*dtの最大値
	/** to naga > 変数のコメント書けよ．デバッグできないし↓**/
	double	dt, dtinv, K, D, D2;
	
	void ElasticDeformation();	//< 弾性変形
	void PlasticDeformation();	//< 塑性変形
	bool IsYield();				//< 降伏しているか否かのチェック
};

class PHBallJointMotor : public PH3ElementCommonData{
public:
	PHBallJoint*	joint;
	bool			yieldFlag;
	double			fNorm;

	virtual void	SetupLCP();
	virtual void	IterateLCP();

	PHBallJointMotor();

private:
	Vec3d	A, Ainv, dA, b, db;
	//Vec3d	f;
	Quaterniond	propQ;
	Vec3d	 K, D, D2,propV;
	double	fMaxDt;
	double	dt, dtinv;
	void    ElasticDeformation(); //< 弾性変形
	void	PlasticDeformation(); //< 塑性変形
	bool	IsYield(); //< 降伏しているか否かのチェック

};

}

#endif
