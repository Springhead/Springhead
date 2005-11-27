#ifndef SPR_PHSCENEIF_H
#define SPR_PHSCENEIF_H

namespace Spr{;

struct PHSolidIf;
struct PHSolidDesc;

struct CDShapeIf;
struct CDShapeDesc;

///	シーン
struct PHSceneIf : public ObjectIf{
public:

	///	Solid作成
	virtual PHSolidIf* CreateSolid()=0;
	///	Solid作成
	virtual PHSolidIf* CreateSolid(const PHSolidDesc& desc)=0;
	///	Solidの数
	virtual int GetNSolids()=0;
	///	Solidの取得
	virtual PHSolidIf** GetSolids()=0;


	///	Shape作成
	virtual CDShapeIf* CreateShape(const CDShapeDesc& desc)=0;
	///	Shapeの数
	virtual int GetNShape()=0;
	///	Shapeの取得
	virtual CDShapeIf** GetShapes()=0;


	/// 積分ステップを返す
	virtual double GetTimeStep()const=0;
	/// 積分ステップを設定する
	virtual void SetTimeStep(double dt)=0;
	/// カウント数を返す
	virtual unsigned GetCount()const=0;
	/// カウント数を設定する
	virtual void SetCount(unsigned c)=0;
	///	シーンの時刻を進める ClearForce(); GenerateForce(); Integrate(); と同じ
	virtual void Step()=0;
	///	シーンの時刻を進める（力と速度を積分して，速度と位置を更新）
	virtual void Integrate()=0;
	///	シーンを空にする．
	virtual void Clear()=0;

	///	重力の設定(ここに持たせるべきか要検討だが，Novodexはここ)
	virtual void SetGravity(Vec3f accel)=0;
	virtual Vec3f GetGravity()=0;

};

}	//	namespace Spr
#endif
