#ifndef SPR_PHSCENEIF_H
#define SPR_PHSCENEIF_H

namespace Spr{;

struct PHSolidIf;
struct PHSolidDesc;

struct CDShapeIf;
struct CDShapeDesc;

/// 物理エンジンのシーンの状態
struct PHSceneState{
	/// 積分ステップ
	double timeStep;
	/// 積分した回数
	unsigned int count;
	PHSceneState(){Init();}
	void Init(){
		timeStep = 0.005;
		count = 0;
	}
};
/// 物理エンジンのシーンのデスクリプタ
struct PHSceneDesc: public PHSceneState{
	///	接触・拘束解決エンジンの種類
	enum SolverType{
		SOLVER_PENALTY,
		SOLVER_CONSTRAINT
	} contactSolver;
	PHSceneDesc(){Init();}
	void Init(){
		contactSolver = SOLVER_PENALTY;
		//contactSolver = SOLVER_CONSTRAINT;
	}
};

///	シーン
struct PHSceneIf : public SceneIf{
public:
	IF_DEF(PHScene);

	///	このSceneをもつSDKを返す
	virtual PHSdkIf* GetSdk()=0;

	///	Shapeの作成
	virtual CDShapeIf* CreateShape(const CDShapeDesc& desc)=0;
	///	Solid作成
	virtual PHSolidIf* CreateSolid()=0;
	///	Solid作成
	virtual PHSolidIf* CreateSolid(const PHSolidDesc& desc)=0;
	///	Solidの数
	virtual int NSolids()=0;
	///	Solidの取得
	virtual PHSolidIf** GetSolids()=0;

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
	///	重力の取得
	virtual Vec3f GetGravity()=0;

};

}	//	namespace Spr
#endif
