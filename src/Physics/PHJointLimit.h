/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHJOINTLIMIT_H
#define PHJOINTLIMIT_H

#include <SprPhysics.h>
#include <Physics/PhysicsDecl.hpp>

namespace Spr{;

class PHScene;
class PHConstraintEngine;
class PHJoint1D;
class PHBallJoint;
struct PH3ElementCommonData;

/**
	関節の可動範囲拘束．インタフェースは持たない．
 */

class PHJointLimit1D{
public:
	PHJoint1D*	joint;
	double  A, Ainv, dA, b, db;
	double	f, F;
	bool	onLower, onUpper;			///< 可動範囲の下限、上限に達している場合にtrue
	
	virtual void	SetupLCP();
	virtual void	IterateLCP();
	virtual void	SetupCorrectionLCP();
	virtual void	IterateCorrectionLCP();

	PHJointLimit1D();
};

struct OnLimit{
	bool onLower;
	bool onUpper;
	bool& operator [] (int i){
		return i==0 ? onLower : onUpper;
	}
};

struct LimitLine{
	double SwingLow[15][5];
	double SwingUp[15][5];
};

class PHBallJointLimit{
public:
	PHBallJoint*	joint;
	Vec3d	A, Ainv, dA, b, db;
	Vec3d	f, F;
	bool	onLower, onUpper;

	/*********************************************************
	Socket座標系：親剛体についている関節の座標系
	Plug座標系：子剛体についている関節の座標系
	拘束座標系：毎ステップ更新する，拘束の条件に使う座標系
	(x軸：拘束円の接線方向，
	 y軸：limitDirの延長線上の一点と交わる直線，
	 z軸：Socket座標系から見たPlug座標系のz軸の方向)
	**********************************************************/

	Vec3d			nowTheta;				///< 現在SocketからPlugに伸びているベクトル(Jc.ez())と稼動域制限の中心ベクトルとのなす角度(.x:swing, .y:twist, .z:swingDir)
	bool			anyLimit;				///< どこかのリミットにかかっているかどうかを調べるフラグ == (onLimit.onUpper || onLimit.onLower)
	OnLimit			onLimit[2];				///< 可動域制限にかかっているとtrue ([0]:swing, [1]:twist)	
	int				limitCount[2];			///< 上の配列のいくつまで入っているのか数える
	double			Irrupt;					///< 侵入量
	Vec3d			tanLine;
	int				FunNum;
	Matrix3d		limDir;					///< 初期の拘束座標系 (x軸,y軸,z軸( = limitDir))
	Vec3d			BefPos;					///< 前回の位置
	LimitLine		limitLine;				///< 拘束範囲の指定

	void	CheckLimit();
	bool	SetConstLine(char* fileName , int i);
	double	GetConstLine(int num, int way){ return limitLine.SwingUp[num][way]; }

	void	SetupLCP();
	void	IterateLCP();
	void	Projection(double& f, int k);

	PHBallJointLimit();
};

}

#endif
