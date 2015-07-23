/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprPHSolid.h
 *	@brief 剛体
*/
#ifndef SPR_PHOPOBJ_H
#define SPR_PHOPOBJ_H

#include <Foundation/SprObject.h>



/**	\addtogroup gpPhysics	*/
//@{
namespace Spr{;


///	剛体のステート
struct PHOpObjState{
	Vec3d		velocity;		///<	質量中心の速度		(World系)
	Vec3d		angVelocity;	///<	角速度				(World系)
//	Vec3d		lastVelocity;	///<	前回の速度			(World系)
//	Vec3d		lastAngVelocity;///<	前回の角速度			(World系)
	Posed		pose;			///<	座標原点の位置と向き	(World系)
//	Posed		lastPose;		///<	前回の位置と向き		(World系)
};


struct PHOpObjDesc : public PHOpObjState{
	double		mass;			///<	質量
	Matrix3d	inertia;		///<	慣性テンソル	(Local系)
	Vec3d		center;			///<	質量中心の位置	(Local系)
	bool		dynamical;		///<	物理法則に従うか(速度は積分される)

	PHOpObjDesc(){ Init(); }
	void Init(){
		mass = 1.0f;
		inertia = Matrix3d::Unit();
		dynamical = true;
	}
};

struct CDShapeIf; 
struct PHTreeNodeIf;

///	剛体のインタフェース
struct PHOpObjIf : public SceneObjectIf{
	SPR_IFDEF(PHOpObj);
	
	
	bool initialDeformObject(Vec3f *vts, int vtsNum, float pSize);
	void		SetGravity(bool bOn);
	void SimpleSimulationStep();

	void AddVertextoLocalBuffer(Vec3f v);
	bool InitialObjUsingLocalBuffer(float pSize);

	//隠したほうがいい関数たち
	void positionPredict();
	void groupStep();
	void integrationStep();
	void ReducedPositionProject();
	void positionProject();

	void BuildBlendWeight();
	

};

//@}

}	//	namespace Spr
#endif
