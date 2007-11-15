//	Do not edit. MakeTypeDesc.bat will update this file.
#define ACCESS_DESC_PHConstraintDesc(cls) \
protected: \
	bool bEnabled ; \
	Posed poseSocket ; \
	Posed posePlug ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		bEnabled = ((PHConstraintDesc*)desc)->bEnabled; \
		poseSocket = ((PHConstraintDesc*)desc)->poseSocket; \
		posePlug = ((PHConstraintDesc*)desc)->posePlug; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((PHConstraintDesc*)desc)->bEnabled = bEnabled; \
		((PHConstraintDesc*)desc)->poseSocket = poseSocket; \
		((PHConstraintDesc*)desc)->posePlug = posePlug; \
		return true; \
	} \

#define ACCESS_DESC_PHConstraintDesc_NOBASE \
protected: \
	bool bEnabled ; \
	Posed poseSocket ; \
	Posed posePlug ; \
public: \
	virtual void SetDesc(const void* desc){ \
		bEnabled = ((PHConstraintDesc*)desc)->bEnabled; \
		poseSocket = ((PHConstraintDesc*)desc)->poseSocket; \
		posePlug = ((PHConstraintDesc*)desc)->posePlug; \
	} \
	virtual bool GetDesc(void* desc){ \
		((PHConstraintDesc*)desc)->bEnabled = bEnabled; \
		((PHConstraintDesc*)desc)->poseSocket = poseSocket; \
		((PHConstraintDesc*)desc)->posePlug = posePlug; \
		return true; \
	} \

#define ACCESS_DESC_PHJoint1DDesc(cls) \
protected: \
	double lower ; \
	double upper ; \
	double spring ; \
	double origin ; \
	double damper ; \
	double desiredVelocity ; \
	double torque ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		lower = ((PHJoint1DDesc*)desc)->lower; \
		upper = ((PHJoint1DDesc*)desc)->upper; \
		spring = ((PHJoint1DDesc*)desc)->spring; \
		origin = ((PHJoint1DDesc*)desc)->origin; \
		damper = ((PHJoint1DDesc*)desc)->damper; \
		desiredVelocity = ((PHJoint1DDesc*)desc)->desiredVelocity; \
		torque = ((PHJoint1DDesc*)desc)->torque; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((PHJoint1DDesc*)desc)->lower = lower; \
		((PHJoint1DDesc*)desc)->upper = upper; \
		((PHJoint1DDesc*)desc)->spring = spring; \
		((PHJoint1DDesc*)desc)->origin = origin; \
		((PHJoint1DDesc*)desc)->damper = damper; \
		((PHJoint1DDesc*)desc)->desiredVelocity = desiredVelocity; \
		((PHJoint1DDesc*)desc)->torque = torque; \
		return true; \
	} \

#define ACCESS_DESC_PHJoint1DDesc_NOBASE \
protected: \
	double lower ; \
	double upper ; \
	double spring ; \
	double origin ; \
	double damper ; \
	double desiredVelocity ; \
	double torque ; \
public: \
	virtual void SetDesc(const void* desc){ \
		lower = ((PHJoint1DDesc*)desc)->lower; \
		upper = ((PHJoint1DDesc*)desc)->upper; \
		spring = ((PHJoint1DDesc*)desc)->spring; \
		origin = ((PHJoint1DDesc*)desc)->origin; \
		damper = ((PHJoint1DDesc*)desc)->damper; \
		desiredVelocity = ((PHJoint1DDesc*)desc)->desiredVelocity; \
		torque = ((PHJoint1DDesc*)desc)->torque; \
	} \
	virtual bool GetDesc(void* desc){ \
		((PHJoint1DDesc*)desc)->lower = lower; \
		((PHJoint1DDesc*)desc)->upper = upper; \
		((PHJoint1DDesc*)desc)->spring = spring; \
		((PHJoint1DDesc*)desc)->origin = origin; \
		((PHJoint1DDesc*)desc)->damper = damper; \
		((PHJoint1DDesc*)desc)->desiredVelocity = desiredVelocity; \
		((PHJoint1DDesc*)desc)->torque = torque; \
		return true; \
	} \

#define ACCESS_DESC_PHTreeNodeDesc(cls) \
protected: \
	bool bEnabled ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		bEnabled = ((PHTreeNodeDesc*)desc)->bEnabled; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((PHTreeNodeDesc*)desc)->bEnabled = bEnabled; \
		return true; \
	} \

#define ACCESS_DESC_PHTreeNodeDesc_NOBASE \
protected: \
	bool bEnabled ; \
public: \
	virtual void SetDesc(const void* desc){ \
		bEnabled = ((PHTreeNodeDesc*)desc)->bEnabled; \
	} \
	virtual bool GetDesc(void* desc){ \
		((PHTreeNodeDesc*)desc)->bEnabled = bEnabled; \
		return true; \
	} \

#define ACCESS_DESC_PHRootNodeDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_PHRootNodeDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_PHTreeNode1DDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_PHTreeNode1DDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_PHHingeJointNodeDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_PHHingeJointNodeDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_PHSliderJointNodeDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_PHSliderJointNodeDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_PHPathJointNodeDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_PHPathJointNodeDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_PHBallJointNodeDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_PHBallJointNodeDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_PHGearDesc(cls) \
protected: \
	double ratio ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		ratio = ((PHGearDesc*)desc)->ratio; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((PHGearDesc*)desc)->ratio = ratio; \
		return true; \
	} \

#define ACCESS_DESC_PHGearDesc_NOBASE \
protected: \
	double ratio ; \
public: \
	virtual void SetDesc(const void* desc){ \
		ratio = ((PHGearDesc*)desc)->ratio; \
	} \
	virtual bool GetDesc(void* desc){ \
		((PHGearDesc*)desc)->ratio = ratio; \
		return true; \
	} \

#define ACCESS_DESC_PHHingeJointDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_PHHingeJointDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_PHSliderJointDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_PHSliderJointDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_PHPathDesc(cls) \
protected: \
	PHPathPoint points ; \
	bool bLoop ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		points = ((PHPathDesc*)desc)->points; \
		bLoop = ((PHPathDesc*)desc)->bLoop; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((PHPathDesc*)desc)->points = points; \
		((PHPathDesc*)desc)->bLoop = bLoop; \
		return true; \
	} \

#define ACCESS_DESC_PHPathDesc_NOBASE \
protected: \
	PHPathPoint points ; \
	bool bLoop ; \
public: \
	virtual void SetDesc(const void* desc){ \
		points = ((PHPathDesc*)desc)->points; \
		bLoop = ((PHPathDesc*)desc)->bLoop; \
	} \
	virtual bool GetDesc(void* desc){ \
		((PHPathDesc*)desc)->points = points; \
		((PHPathDesc*)desc)->bLoop = bLoop; \
		return true; \
	} \

#define ACCESS_DESC_PHPathJointDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_PHPathJointDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_PHBallJointDesc(cls) \
protected: \
	LimitST limit ; \
	Vec3d torque ; \
	SwingTwist goal ; \
	Vec3d spring ; \
	Vec3d damper ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		limit = ((PHBallJointDesc*)desc)->limit; \
		torque = ((PHBallJointDesc*)desc)->torque; \
		goal = ((PHBallJointDesc*)desc)->goal; \
		spring = ((PHBallJointDesc*)desc)->spring; \
		damper = ((PHBallJointDesc*)desc)->damper; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((PHBallJointDesc*)desc)->limit = limit; \
		((PHBallJointDesc*)desc)->torque = torque; \
		((PHBallJointDesc*)desc)->goal = goal; \
		((PHBallJointDesc*)desc)->spring = spring; \
		((PHBallJointDesc*)desc)->damper = damper; \
		return true; \
	} \

#define ACCESS_DESC_PHBallJointDesc_NOBASE \
protected: \
	LimitST limit ; \
	Vec3d torque ; \
	SwingTwist goal ; \
	Vec3d spring ; \
	Vec3d damper ; \
public: \
	virtual void SetDesc(const void* desc){ \
		limit = ((PHBallJointDesc*)desc)->limit; \
		torque = ((PHBallJointDesc*)desc)->torque; \
		goal = ((PHBallJointDesc*)desc)->goal; \
		spring = ((PHBallJointDesc*)desc)->spring; \
		damper = ((PHBallJointDesc*)desc)->damper; \
	} \
	virtual bool GetDesc(void* desc){ \
		((PHBallJointDesc*)desc)->limit = limit; \
		((PHBallJointDesc*)desc)->torque = torque; \
		((PHBallJointDesc*)desc)->goal = goal; \
		((PHBallJointDesc*)desc)->spring = spring; \
		((PHBallJointDesc*)desc)->damper = damper; \
		return true; \
	} \

#define ACCESS_DESC_PHSpringDesc(cls) \
protected: \
	Vec3d spring ; \
	Vec3d damper ; \
	double springOri ; \
	double damperOri ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		spring = ((PHSpringDesc*)desc)->spring; \
		damper = ((PHSpringDesc*)desc)->damper; \
		springOri = ((PHSpringDesc*)desc)->springOri; \
		damperOri = ((PHSpringDesc*)desc)->damperOri; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((PHSpringDesc*)desc)->spring = spring; \
		((PHSpringDesc*)desc)->damper = damper; \
		((PHSpringDesc*)desc)->springOri = springOri; \
		((PHSpringDesc*)desc)->damperOri = damperOri; \
		return true; \
	} \

#define ACCESS_DESC_PHSpringDesc_NOBASE \
protected: \
	Vec3d spring ; \
	Vec3d damper ; \
	double springOri ; \
	double damperOri ; \
public: \
	virtual void SetDesc(const void* desc){ \
		spring = ((PHSpringDesc*)desc)->spring; \
		damper = ((PHSpringDesc*)desc)->damper; \
		springOri = ((PHSpringDesc*)desc)->springOri; \
		damperOri = ((PHSpringDesc*)desc)->damperOri; \
	} \
	virtual bool GetDesc(void* desc){ \
		((PHSpringDesc*)desc)->spring = spring; \
		((PHSpringDesc*)desc)->damper = damper; \
		((PHSpringDesc*)desc)->springOri = springOri; \
		((PHSpringDesc*)desc)->damperOri = damperOri; \
		return true; \
	} \

#define ACCESS_DESC_PHSceneDesc(cls) \
protected: \
	Vec3f gravity ; \
	int numIteration ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		gravity = ((PHSceneDesc*)desc)->gravity; \
		numIteration = ((PHSceneDesc*)desc)->numIteration; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((PHSceneDesc*)desc)->gravity = gravity; \
		((PHSceneDesc*)desc)->numIteration = numIteration; \
		return true; \
	} \

#define ACCESS_DESC_PHSceneDesc_NOBASE \
protected: \
	Vec3f gravity ; \
	int numIteration ; \
public: \
	virtual void SetDesc(const void* desc){ \
		gravity = ((PHSceneDesc*)desc)->gravity; \
		numIteration = ((PHSceneDesc*)desc)->numIteration; \
	} \
	virtual bool GetDesc(void* desc){ \
		((PHSceneDesc*)desc)->gravity = gravity; \
		((PHSceneDesc*)desc)->numIteration = numIteration; \
		return true; \
	} \

#define ACCESS_DESC_PHSdkDesc(cls) \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		return true; \
	} \

#define ACCESS_DESC_PHSdkDesc_NOBASE \
protected: \
public: \
	virtual void SetDesc(const void* desc){ \
	} \
	virtual bool GetDesc(void* desc){ \
		return true; \
	} \

#define ACCESS_DESC_PHSolidDesc(cls) \
protected: \
	double mass ; \
	Matrix3d inertia ; \
	Vec3d center ; \
	bool dynamical ; \
public: \
	virtual void SetDesc(const void* desc){ \
		cls##::SetDesc(desc); \
		mass = ((PHSolidDesc*)desc)->mass; \
		inertia = ((PHSolidDesc*)desc)->inertia; \
		center = ((PHSolidDesc*)desc)->center; \
		dynamical = ((PHSolidDesc*)desc)->dynamical; \
	} \
	virtual bool GetDesc(void* desc){ \
		cls##::GetDesc(desc); \
		((PHSolidDesc*)desc)->mass = mass; \
		((PHSolidDesc*)desc)->inertia = inertia; \
		((PHSolidDesc*)desc)->center = center; \
		((PHSolidDesc*)desc)->dynamical = dynamical; \
		return true; \
	} \

#define ACCESS_DESC_PHSolidDesc_NOBASE \
protected: \
	double mass ; \
	Matrix3d inertia ; \
	Vec3d center ; \
	bool dynamical ; \
public: \
	virtual void SetDesc(const void* desc){ \
		mass = ((PHSolidDesc*)desc)->mass; \
		inertia = ((PHSolidDesc*)desc)->inertia; \
		center = ((PHSolidDesc*)desc)->center; \
		dynamical = ((PHSolidDesc*)desc)->dynamical; \
	} \
	virtual bool GetDesc(void* desc){ \
		((PHSolidDesc*)desc)->mass = mass; \
		((PHSolidDesc*)desc)->inertia = inertia; \
		((PHSolidDesc*)desc)->center = center; \
		((PHSolidDesc*)desc)->dynamical = dynamical; \
		return true; \
	} \

