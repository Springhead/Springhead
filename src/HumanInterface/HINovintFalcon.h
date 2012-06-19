#ifndef HINOVINTFALCON_H
#define HINOVINTFALCON_H

#include <HumanInterface/HIBase.h>

namespace Spr {;

class UTDllLoader; 
/**	@JA	Novint社のFalcon。３自由度力覚インタフェース
	@EN	A Novint company's Falcon interface. A 3 DOF haptic interface. @@*/
class SPR_DLL HINovintFalcon: public HIHaptic{
public:
	static UTDllLoader* sDllLoader;
protected:
	bool good;
	Vec3d pos;
	Vec3d force;
	int button;
	int deviceHandle;
	UTRef<UTDllLoader> dll;
public:
	///	デバイスのタイプ
	SPR_OBJECTDEF(HINovintFalcon);

	HINovintFalcon(const HINovintFalconDesc& desc = HINovintFalconDesc());
	virtual ~HINovintFalcon();
	///	デバイスの初期化(使用前に呼ぶ)
	virtual bool Init(const void* desc);
	///	キャリブレーションの前に行う処理
	virtual bool BeforeCalibration();
	///	キャリブレーション
	virtual bool Calibration();
	///	位置の取得
	Vec3f GetPosition(){ return pos; }
	Quaternionf GetOrientation(){ return Quaternionf(); }
	
	///	力の設定
	void SetForce(const Vec3f& f, const Vec3f& t=Vec3f()){ force = f; }
	///	実際に出した力の取得
	Vec3f GetForce(){ return force; }
	Vec3f GetTorque(){ return Vec3f(); }

	/// ボタン情報の取得
	int GetButton(int ch);
	int GetButtonBit();

	///	デバイスの状態を更新する.
	virtual void Update(float dt);
};

}	//	namespace Spr
#endif
