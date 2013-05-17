#ifndef PH_HAPTICPOINTER_H
#define PH_HAPTICPOINTER_H

//#include <Springhead.h>
#include <Physics/PHSolid.h>

namespace Spr{;

//----------------------------------------------------------------------------
// PHHapticPointer
struct PHHapticPointerSt : public PHHapticPointerDesc{
	Vec3d last_dr;
	Vec3d last_dtheta;
	Posed proxyPose;	// 摩擦計算用のプロキシ
	Posed lastProxyPose;
	Posed targetProxy;	// 反力計算用のプロキシ
	SpatialVector vcForce;
	SpatialVector hapticForce;
};
class PHHapticPointer : public PHHapticPointerSt, public PHSolid{
	SPR_OBJECTDEF(PHHapticPointer);
	ACCESS_DESC(PHHapticPointer);
protected:
	int pointerID;
	int pointerSolidID;
	float reflexSpring, reflexDamper;
	float reflexSpringOri, reflexDamperOri;
	float localRange;
	double posScale;
	Posed defaultPose;
	double rotaionalWeight;


public:
	bool bDebugControl;		// physicsLoopから力覚ポインタを操作するためのフラグ
	bool bForce;
	bool bFriction;
	bool bVibration;
	bool bMultiPoints;
	HapticRenderMode hapticRenderMode;
	std::vector<int> neighborSolidIDs;
	PHSolid hiSolid;
	PHSolidIf* vcSolid;
	PHSolid vcSolidCopied;
	PHHapticPointer();
	PHHapticPointer(const PHHapticPointer& p);

	//API
	void	SetHapticRenderMode(HapticRenderMode m){ hapticRenderMode = m; }
	void	EnableForce(bool b){ bForce = b; }
	void	EnableFriction(bool b){ bFriction = b; }
	void	EnableMultiPoints(bool b){ bMultiPoints = b; }
	bool	IsMultiPoints(){ return bMultiPoints; }
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
	void	SetDefaultPose(Posed p){ 
		SetPose(p);
		defaultPose = p; 
	}
	Posed	GetDefaultPose(){ return defaultPose; }

	int     NNeighborSolids() { return (int)neighborSolidIDs.size(); }
	PHSolidIf*   GetNeighborSolid(int i) {
		int j = neighborSolidIDs[i];
		return DCAST(PHSceneIf,GetScene())->GetSolids()[j];
	}
	float   GetContactForce(int i);

	// Implementation
	void	SetPointerID(int id){ pointerID = id; }
	int		GetPointerID(){ return pointerID; }
	void	SetSolidID(int id){ pointerSolidID = id; }
	int		GetSolidID(){ return pointerSolidID; }
	void	UpdateHumanInterface(Posed pose, SpatialVector vel);
	void	UpdateDirect();
	void	AddHapticForce(SpatialVector f);
	SpatialVector	GetHapticForce();

	virtual void UpdateVelocity(double dt){}
	virtual void UpdatePosition(double dt){}
};
class PHHapticPointers : public std::vector< UTRef< PHHapticPointer > >{};

}
#endif