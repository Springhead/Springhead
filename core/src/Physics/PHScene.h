/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PHSCENE_H
#define PHSCENE_H
#include <Physics/SprPHScene.h>
#include "../Foundation/Scene.h"
#include "PHEngine.h"
#include "PHSolid.h"
#include "PHIKEngine.h"
#include "PHIKActuator.h"
#include "PHIKEndEffector.h"
#include "PHSkeleton.h"
#include "PHOpEngine.h"
#include <Physics/PHOpSpHashColliAgent.h>

namespace Spr {;

class CDShape;
struct CDShapeDesc;
class PHSolidContainer;
class PHPenaltyEngine;
class PHConstraintEngine;
class PHGravityEngine;
class PHFemEngine;
class PHHapticEngine;
struct PHConstraintDesc;
struct PHJointDesc;
class PHScene;

class SPR_DLL PHRay : public SceneObject, public PHRayDesc{
	SPR_OBJECTDEF(PHRay);
protected:
	std::vector<PHRaycastHit>	hits;
public:
	Vec3d	GetOrigin(){ return origin; }
	void	SetOrigin(const Vec3d& ori){ origin = ori; }
	Vec3d	GetDirection(){ return direction; }
	void	SetDirection(const Vec3d& dir){ direction = dir; }
	void	Apply();
	int		NHits(){ return (int)hits.size(); }
	PHRaycastHit* GetHits(){ return &hits[0]; }
	PHRaycastHit* GetNearest();
	PHRaycastHit* GetDynamicalNearest();
};
typedef std::vector< UTRef<PHRay> > PHRays;

class SPR_DLL PHScene : public Scene, public PHSceneDesc{
	SPR_OBJECTDEF(PHScene);
	friend class PHConstraint;
public:
	PHEngines				engines;
protected:
	/// 各種エンジン
	PHSolidContainer*		solids;
	PHPenaltyEngine*		penaltyEngine;
	PHConstraintEngine*		constraintEngine;
	PHGravityEngine*		gravityEngine;
	PHIKEngine*				ikEngine;
	PHRays					rays;
	PHFemEngine*			femEngine;
	PHHapticEngine*			hapticEngine;
	PHOpEngine*				opEngine;
	
	UTRef<PHOpSpHashColliAgent>	opSpAgent;

	std::vector< UTRef<PHSkeleton> > skeletons;

	double					timeStepInv;	///< timeStepの逆数．高速化用
public:
	
	friend class			PHSolid;
	friend class			PHFrame;
	friend class			Object;

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	///	コンストラクタ
	PHScene(const PHSceneDesc& desc = PHSceneDesc());
	void Init();
	///	デストラクタ
	~PHScene();

	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//このクラス内での機能.
	PHEngines*				GetEngines();
	CDShapeIf*				CreateShape(const IfInfo* ii, const CDShapeDesc& desc);	
	double					GetTimeStepInv(){ return timeStepInv; }
	
	//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//インタフェース(PHSceneIf)の実装
	PHSdkIf*				GetSdk();
	PHSolidIf*				CreateSolid(const PHSolidDesc& desc = PHSolidDesc());
	int						NSolids() const;
	PHSolidIf**				GetSolids();
	void					SetContactMode(PHSolidIf* lhs, PHSolidIf* rhs, PHSceneDesc::ContactMode = PHSceneDesc::MODE_LCP);
	void					SetContactMode(PHSolidIf** group ,size_t length, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP);
	void					SetContactMode(PHSolidIf* solid, PHSceneDesc::ContactMode = PHSceneDesc::MODE_LCP);
	void					SetContactMode(PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP);
	int                     GetLCPSolver();
	void                    SetLCPSolver(int method);
	int	                    GetNumIteration();
	void                    SetNumIteration(int n);
	int	                    GetNumIterationCorrection();
	void                    SetNumIterationCorrection(int n);
	PHJointIf*				CreateJoint(PHSolidIf* lhs, PHSolidIf* rhs, const IfInfo* ii, const PHJointDesc& desc);
	int						NJoints() const;
	PHJointIf*				GetJoint(int i);
	int						NContacts() const;
	PHContactPointIf*		GetContact(int i);
	int						NSolidPairs() const;
	PHSolidPairForLCPIf*	GetSolidPair(int i, int j);
	PHSolidPairForLCPIf*	GetSolidPair(PHSolidIf* lhs, PHSolidIf* rhs, bool& bSwap);
//	UTCombination<UTRef<PHSolidPairForLCPIf>>	GetSolidPair();
	PHRootNodeIf*			CreateRootNode(PHSolidIf* root, const PHRootNodeDesc& desc = PHRootNodeDesc());
	int						NRootNodes() const;
	PHRootNodeIf*			GetRootNode(int i);
	PHTreeNodeIf*			CreateTreeNode(PHTreeNodeIf* parent, PHSolidIf* child, const PHTreeNodeDesc& desc = PHTreeNodeDesc());
	void					CreateTreeNodesRecurs(PHTreeNodeIf* node, PHSolidIf* solid);
	PHRootNodeIf*			CreateTreeNodes      (PHSolidIf* solid);
	PHGearIf*				CreateGear(PH1DJointIf* lhs, PH1DJointIf* rhs, const PHGearDesc& desc = PHGearDesc());
	int						NGears() const;
	PHGearIf*				GetGear(int i);
	PHPathIf*				CreatePath(const PHPathDesc& desc = PHPathDesc());
	int						NPaths() const;
	PHPathIf*				GetPath(int i);
	PHRayIf*				CreateRay(const PHRayDesc& desc = PHRayDesc());
	int						NRays() const;
	PHRayIf*				GetRay(int i);

	PHIKActuatorIf*			CreateIKActuator(const IfInfo* ii, const PHIKActuatorDesc& desc = PHIKActuatorDesc());
	int						NIKActuators() const;
	PHIKActuatorIf*			GetIKActuator(int i);
	PHIKEndEffectorIf*		CreateIKEndEffector(const PHIKEndEffectorDesc& desc = PHIKEndEffectorDesc());
	int						NIKEndEffectors() const;
	PHIKEndEffectorIf*		GetIKEndEffector(int i);

	int						NSkeletons() { return (int)(skeletons.size()); }
	PHSkeletonIf*			GetSkeleton(int i) { return skeletons[i]->Cast(); }
	PHSkeletonIf*			CreateSkeleton(const PHSkeletonDesc& desc = PHSkeletonDesc()) {
		PHSkeleton* skeleton = DBG_NEW PHSkeleton();
		skeleton->SetDesc(&desc);
		skeletons.push_back(skeleton);
		skeleton->SetScene(this->Cast());
		return skeleton->Cast();
	}
	


	int						NFemMeshes()const;
	PHFemMeshIf*			GetFemMesh(int i);
	int						NFemMeshesNew() const;
	PHFemMeshNewIf*			GetFemMeshNew(int i);
	int						NOpObjs() const;
	PHOpObj*			GetOpObj(int i);

	//void 					FindNeighboringSolids(PHSolidIf* solid, double range, PHSolidIfs& nsolids);

	/// 積分ステップを返す
	double		GetTimeStep()const{return timeStep;}
	/// 積分ステップを設定する
	void		SetTimeStep(double dt);
	/// 力覚積分ステップを返す
	double		GetHapticTimeStep()const{ return haptictimeStep; }
	/// 力覚積分ステップを設定する
	void		SetHapticTimeStep(double dt){ haptictimeStep = dt; }
	/// カウント数を返す
	unsigned	GetCount()const{return count;}
	/// カウント数を設定する
	void		SetCount(unsigned c){count = c;}
	
	void	SetGravity(const Vec3d& accel);
	Vec3d	GetGravity();
	
	double	GetAirResistanceRate(){ return airResistanceRate; }
	void	SetAirResistanceRate(double rate){ airResistanceRate = rate; }

	double  GetContactTolerance(){ return contactTolerance; }
	void    SetContactTolerance(double tol){ contactTolerance = tol; }

	double	GetImpactThreshold(){ return impactThreshold; }
	void	SetImpactThreshold(double vth){ impactThreshold = vth; }
	
	double	GetFrictionThreshold(){ return frictionThreshold; }
	void	SetFrictionThreshold(double vth){ frictionThreshold = vth; }
	
	double  GetMaxVelocity           ()            { return maxVelocity; }
	void    SetMaxVelocity           (double vmax) { maxVelocity = vmax; }	
	double  GetMaxAngularVelocity    ()            { return maxAngularVelocity; }
	void    SetMaxAngularVelocity    (double wmax) { maxAngularVelocity = wmax; }
	double  GetMaxForce              ()            { return maxForce; }
	void    SetMaxForce              (double fmax) { maxForce = fmax; }
	double  GetMaxMoment             ()            { return maxMoment; }
	void    SetMaxMoment             (double tmax) { maxMoment = tmax; }
	double  GetMaxDeltaPosition      ()            { return maxDeltaPosition; }
	void    SetMaxDeltaPosition      (double dpmax){ maxDeltaPosition = dpmax; }
	double  GetMaxDeltaOrientation   ()            { return maxDeltaOrientation; }
	void    SetMaxDeltaOrientation   (double dqmax){ maxDeltaOrientation = dqmax; }
	void	EnableContactDetection   (bool enable) { bContactDetectionEnabled = enable; }
	bool    IsContactDetectionEnabled()            { return bContactDetectionEnabled; }
	void    EnableCCD                (bool enable) { bCCDEnabled = enable; }
	bool    IsCCDEnabled             ()            { return bCCDEnabled; }
	void    SetBroadPhaseMode        (int mode)    { broadPhaseMode = mode; }
	int     GetBroadPhaseMode        ()            { return broadPhaseMode; } 
	void    SetContactDetectionRange (Vec3f center, Vec3f extent, int nx, int ny, int nz);
	void    SetMaterialBlending      (int mode);
	
	///	シーンの時刻を進める ClearForce(); GenerateForce(); Integrate(); と同じ
	void					Step();
	///	シーンの時刻を進める（力のクリア）
	void					ClearForce();
	///	シーンの時刻を進める（力の生成）
	void					GenerateForce();
	///	シーンの時刻を進める（力と速度を積分して，速度と位置を更新）
	void					Integrate();
	void					IntegratePart1();
	void					IntegratePart2();
	
	void					StepHapticLoop();
	void					StepHapticSync();

	int						NEngines();
	PHEngineIf*				GetEngine(int i);
	PHConstraintEngineIf*	GetConstraintEngine();
	PHGravityEngineIf*		GetGravityEngine();
	PHPenaltyEngineIf*		GetPenaltyEngine();
	PHIKEngineIf*			GetIKEngine();
	PHFemEngineIf*			GetFemEngine();
	PHHapticEngineIf*		GetHapticEngine();
	PHOpEngineIf*				GetOpEngine();
	PHOpSpHashColliAgentIf* GetOpColliAgent();
	PHHapticPointerIf*		CreateHapticPointer();
	void					SetStateMode(bool bConstraints);
	
	virtual void			Clear();
	virtual ObjectIf*		CreateObject(const IfInfo* info, const void* desc);
	virtual size_t			NChildObject() const;
	virtual ObjectIf*		GetChildObject(size_t pos);
	virtual bool			AddChildObject(ObjectIf* o);
	virtual bool			DelChildObject(ObjectIf* o);	
	
	ACCESS_DESC(PHScene);
	virtual size_t      GetStateSize   () const;
	virtual void        ConstructState (void* m) const;
	virtual void        DestructState  (void* m) const;
	virtual const void* GetStateAddress() const { return NULL; } // not supported.
	virtual bool        GetState       (void* s) const;
	virtual void        SetState       (const void* s);
	virtual void        GetStateR      (char*& s);
	virtual void        SetStateR      (const char*& state);
	virtual bool        WriteStateR    (std::ostream& fout);
	virtual bool        ReadStateR     (std::istream& fin);
	virtual void        DumpObjectR    (std::ostream& os, int level=0) const;
protected:
	virtual void AfterSetDesc();
	virtual void BeforeGetDesc() const;
};

}
#endif
