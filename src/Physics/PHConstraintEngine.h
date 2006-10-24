/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHCONSTRAINTENGINE_H
#define PHCONSTRAINTENGINE_H

#include <Physics/PHContactDetector.h>
#include <Physics/PHConstraint.h>

namespace Spr{;

class PHSolid;
class PHJoint;
class PHConstraintEngine;

///	形状の組
class PHShapePairForLCP : public CDShapePair{
public:
	void CalcNormal(PHSolid* solid0, PHSolid* solid1);
	///	接触解析．接触部分の切り口を求めて，切り口を構成する凸多角形の頂点をengineに拘束として追加する．
	void EnumVertex(PHConstraintEngine* engine, unsigned ct, PHSolidInfoForLCP* solid0, PHSolidInfoForLCP* solid1);
	///	連続接触判定
	bool ContDetect(unsigned ct, CDConvex* s0, CDConvex* s1, const Posed& pose0, const Vec3d& delta0, const Posed& pose1, const Vec3d& delta1);
};

/// Solidの組
class PHConstraintEngine;
class PHSolidPairForLCP : public PHSolidPair<PHSolidInfoForLCP, PHShapePairForLCP, PHConstraintEngine>{
public:
	virtual void OnDetect(PHShapePairForLCP* cp, PHConstraintEngine* engine, unsigned ct, double dt);
	virtual void OnContDetect(PHShapePairForLCP* cp, PHConstraintEngine* engine, unsigned ct, double dt);
};

class PHConstraintEngine : public PHContactDetector<PHSolidInfoForLCP, PHShapePairForLCP, PHSolidPairForLCP, PHConstraintEngine>{
	typedef PHContactDetector<PHSolidInfoForLCP, PHShapePairForLCP, PHSolidPairForLCP, PHConstraintEngine> base_type;
	friend class PHConstraint;

	OBJECT_DEF_NOIF(PHConstraintEngine);

	PHJoint* CreateJoint(const PHJointDesc& desc);
	
public:
	int		numIteration;			///< 速度更新LCPの反復回数
	double	correctionRate;			///< 誤差修正比率
	double	shrinkRate;				///< LCP初期値を前回の解に対して縮小させる比率
	//int max_iter_correction;		///< 誤差修正LCPの反復回数
	//double max_error;

	PHConstraintEngine();
	~PHConstraintEngine();
	PHJoint* AddJoint(const PHJointDesc& desc);	///< 関節を追加する（ファイルローダ用）
	PHJoint* AddJoint(PHSolid* lhs, PHSolid* rhs, const PHJointDesc& desc);	///< 関節の追加する
	bool AddJoint(PHSolidIf* lhs, PHSolidIf* rhs, PHJointIf* j);
	
	virtual int GetPriority() const {return SGBP_CONSTRAINTENGINE;}
	virtual void Step();			///< 
	virtual void Dynamics(double dt, int ct);		///< 
	//virtual void Correction(double dt, int ct);		///< 
	void UpdateSolids(double dt);	///< 結果をSolidに反映する
	PHConstraints GetContactPoints();
	void Clear();

protected:
	PHConstraints	points;			///< 接触点の配列
	PHConstraints	joints;			///< 関節の配列
	
	void SetupDynamics(double dt);	///< 速度更新LCPの準備
	void IterateDynamics();			///< 速度更新LCPの一度の反復
	
	//void SetupCorrection(double dt);///< 誤差修正LCPの準備
	//void IterateCorrection();		///< 誤差修正LCPの一度の反復
	
	friend class PHShapePairForLCP;
};

}	//	namespace Spr
#endif
