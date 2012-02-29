#ifndef PH_HAPTICPOINTER_H
#define PH_HAPTICPOINTER_H

#include <Springhead.h>
#include <Physics/PHSolid.h>

namespace Spr{;

//class PHSolidsForHaptic;
class PHSolidPairForHaptic;
//class PHSolidPairsForHaptic;
//class PHShapePairForHaptic;

struct PHIntermediateRepresentation :public UTRefCount{
	int solidID;				///< 剛体のID(PHSolidsForHapitcとPHSolidPairForHaptic)と侵入しているか
	PHSolidPairForHaptic* solidPair;	///< PHSolidPairForHapticへのポインタ
	double depth;				///< 侵入量
	Vec3d normal;				///< 面法線
	Vec3d r;					///< 力覚ポインタの重心からの接触点までのベクトル
	Vec3d pointerPointW;		///< 力覚ポンタの侵入点(world)
	Vec3d contactPointW;		///< 中間表現面にvertexを投影した位置(world)
	Vec3d pointerPointVel;		///< 力覚ポインタ側の接触点の速度(world)
	Vec3d contactPointVel;		///< 剛体側の接触点の速度(world)
	Posed interpolation_pose;	///< 形状の補間姿勢
	double f;					///< 侵入解除計算で接触点が並進移動に作用する大きさ
	float springK;					///< バネ係数
	float damperD;					///< ダンパ係数
	float mu;					///< 動摩擦係数
	float mu0;					///< 最大静止摩擦係数(最大静止摩擦は未実装)
	PHIntermediateRepresentation(){
		solidID = -1;
		f = 0.0;
		solidPair = NULL;
	}
	bool operator < (const PHIntermediateRepresentation& s) const { return depth < s.depth; }
};
typedef PHIntermediateRepresentation PHIr;
typedef std::vector< PHIr* > PHIrs;

//----------------------------------------------------------------------------
// PHHapticPointer
struct PHHapticPointerSt{
	Vec3d last_dr;
	Vec3d last_dtheta;
	Posed proxyPose;	// 摩擦計算用のプロキシ
	Posed lastProxyPose;
	Posed targetProxy;	// 反力計算用のプロキシ
};
class PHHapticPointer : public PHHapticPointerSt, public PHSolid{
	SPR_OBJECTDEF(PHHapticPointer);
protected:
	int pointerID;
	int pointerSolidID;
	float reflexSpring, reflexDamper;
	float reflexSpringOri, reflexDamperOri;
	float localRange;
	double posScale;
	Posed defaultPose;
	double rotaionalWeight;
	SpatialVector hapticForce;

public:
	bool bDebugControl;
	bool bForce;
	bool bFriction;
	bool bVibration;
	std::vector<int> neighborSolidIDs;
	PHSolid hiSolid;
	HIBaseIf* humanInterface;
	PHHapticPointer();
	PHHapticPointer(const PHHapticPointer& p);

	//API
	void	SetHumanInterface(HIBaseIf* hi){ humanInterface = hi; }
	void	EnableForce(bool b){ bForce = b; }
	void	EnableFriction(bool b){ bFriction = b; }
	void	EnableVibration(bool b){ bVibration = b; }
	void	EnableDebugControl(bool b){ bDebugControl = b; }
	void	SetReflexSpring(float s){ reflexSpring = s; }
	float	GetReflexSpring(){ return reflexSpring; }
	void	SetReflexDamper(float d){ reflexDamper = d; }
	float	GetReflexDamper(){ return reflexDamper; }
	void	SetReflexSpringOri(float s){ reflexSpringOri = s; }
	float	GetReflexSpringOri(){ return reflexSpringOri; }
	void	SetReflexDamperOri(float d){ reflexDamperOri = d; }
	float	GetReflexDamperOri(){ return reflexDamperOri; }
	void	SetLocalRange(float r){ localRange = r; } 
	float	GetLocalRange(){ return localRange; }
	void	SetPosScale(double scale){ posScale = scale; }
	double	GetPosScale(){ return posScale; }
	void	SetRotationalWeight(double w){ rotaionalWeight = w; }
	double	GetRotationalWeight(){ return rotaionalWeight ; }
	void	SetDefaultPose(Posed p){ defaultPose = p; }
	Posed	GetDefaultPose(){ return defaultPose; }

	// Implementation
	void	SetPointerID(int id){ pointerID = id; }
	int		GetPointerID(){ return pointerID; }
	void	SetSolidID(int id){ pointerSolidID = id; }
	int		GetSolidID(){ return pointerSolidID; }
	void	UpdateInterface(float dt);
	void	UpdateDirect();
	void	AddHapticForce(SpatialVector f);
	void	DisplayHapticForce();
};
class PHHapticPointers : public std::vector< UTRef< PHHapticPointer > >{};

}
#endif