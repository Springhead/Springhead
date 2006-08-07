/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHSCENE_H
#define PHSCENE_H
#include <Physics/SprPHScene.h>
#include <Foundation/Scene.h>
#include <Physics/PHEngine.h>

namespace Spr {;

class CDShape;
struct CDShapeDesc;
class PHSolidContainer;
class PHPenaltyEngine;
class PHConstraintEngine;
struct PHConstraintDesc;
typedef PHConstraintDesc PHJointDesc;

/**	シーングラフのトップノード．光源・視点を持つ．
	レンダラとシーングラフの関係が深いため，
	レンダラが違うとシーングラフはそのまま使用できない．
	シーングラフは，たとえばレンダラがOpenGLなら，displayList
	を持っているし，D3Dならば ID3DXMeshを持っている．
	OpenGLのシーングラフをD3Dに変換するためには，一度Documentに
	セーブして，D3D形式でロードしなければならない．	*/
class SPR_DLL PHScene:public InheritScene<PHSceneIf, Scene>, public PHSceneDesc{
	OBJECT_DEF(PHScene);
public:
	PHEngines engines;
protected:
	PHSolidContainer*	solids;
	PHPenaltyEngine*	penaltyEngine;
	PHConstraintEngine* constraintEngine;
public:
	///	コンストラクタ
	PHScene(const PHSceneDesc& desc=PHSceneDesc());
	void Init();
	///	デストラクタ
	~PHScene(){}

	PHSolidIf* CreateSolid();
	PHSolidIf* CreateSolid(const PHSolidDesc& desc);
	CDShapeIf* CreateShape(const CDShapeDesc& desc);
	PHJointIf* CreateJoint(const PHJointDesc& desc);	///< ファイルローダ用
	PHJointIf* CreateJoint(PHSolidIf* lhs, PHSolidIf* rhs, const PHJointDesc& desc);
	PHPathIf*  CreatePath(const PHPathDesc& desc);
	void SetContactMode(PHSolidIf* lhs, PHSolidIf* rhs, PHSceneDesc::ContactMode = PHSceneDesc::MODE_LCP);
	void SetContactMode(PHSolidIf** group ,size_t length, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP);
	void SetContactMode(PHSolidIf* solid, PHSceneDesc::ContactMode = PHSceneDesc::MODE_LCP);
	void SetContactMode(PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP);
	virtual int GetNumIteration();
	virtual void SetNumIteration(int n);
	void SetGravity(const Vec3d& g);
	PHSdkIf* GetSdk();

	/// 積分ステップを返す
	double GetTimeStep()const{return timeStep;}
	/// 積分ステップを設定する
	void SetTimeStep(double dt);
	/// カウント数を返す
	unsigned GetCount()const{return count;}
	/// カウント数を設定する
	void SetCount(unsigned c){count = c;}
	///	シーンの時刻を進める ClearForce(); GenerateForce(); Integrate(); と同じ
	void Step();
	///	シーンの時刻を進める（力のクリア）
	void ClearForce();
	///	シーンの時刻を進める（力の生成）
	void GenerateForce();
	///	シーンの時刻を進める（力と速度を積分して，速度と位置を更新）
	void Integrate();
	///	シーンを空にする．
	void Clear();

	virtual void SetGravity(Vec3f accel);
	virtual Vec3f GetGravity();

	virtual int NSolids();
	virtual PHSolidIf** GetSolids();
	ObjectIf* CreateObject(const IfInfo* info, const void* desc);
	virtual size_t NChildObject() const;
	virtual ObjectIf* GetChildObject(size_t pos);
	virtual bool AddChildObject(ObjectIf* o);
	virtual bool DelChildObject(ObjectIf* o);
protected:
	friend class PHSolid;
	friend class Object;
	ACCESS_DESC_STATE(PHScene);
};

}
#endif
