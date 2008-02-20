#ifndef HIBASE_H
#define HIBASE_H

#include <Base/Affine.h>
#include <Base/BaseUtility.h>
#include <Base/BaseTypeInfo.h>

#include <set>

///	HIBase を継承したクラスはメンバとしてこのマクロを持つ．
#define HIOBJECTDEF(cls)				DEF_UTTYPEINFODEF(cls)
#define HIOBJECTDEFABST(cls)			DEF_UTTYPEINFOABSTDEF(cls)
#define HIOBJECTIMPBASE(cls)			DEF_UTTYPEINFO(cls)
#define HIOBJECTIMPBASEABST(cls)		DEF_UTTYPEINFOABST(cls)
#define HIOBJECTIMP(cls, base)			DEF_UTTYPEINFO1(cls, base)
#define HIOBJECTIMPABST(cls, base)		DEF_UTTYPEINFOABST1(cls, base)


namespace Spr{;
class DRRealDevice;

///	The base class of human interface classes.
class SPR_DLL HIBase: public UTRefCount, UTTypeInfoObjectBase{
protected:
	static std::set<DRRealDevice*> realDevices;
	static int deviceUpdateStep;
	int updateStep;
	bool bGood;
public:
	HIOBJECTDEFABST(HIBase);
	///
	HIBase(){ updateStep = deviceUpdateStep; bGood=false;}
	///
	virtual ~HIBase(){}
	///	Is the device good for use?
	bool IsGood(){ return bGood; }
	///	Update state of the interface; read from/write to real device.
	virtual void Update(float dt);
	/**	Add an real device to the dependency list of real devices. 
		Update() will call real device's Update() function.	*/
	void AddRealDeviceDependency(DRRealDevice* rd);
	///	Clear the dependency list.
	static void ClearRealDeviceDependency();
};
	
	
}


#endif

