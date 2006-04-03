
/** \addtogroup gpPhysics 	*/
//@{

/**
 *	@file SprPHScene.h
 *	@brief シーン
*/
#ifndef SPR_PHSCENEIF_H
#define SPR_PHSCENEIF_H

namespace Spr{;

struct PHSolidIf;
struct PHSolidDesc;

struct CDShapeIf;
struct CDShapeDesc;

struct PHJointIf;
struct PHJointDesc;

/// 物理エンジンのシーンの状態
struct PHSceneState{
	/// 積分ステップ
	double timeStep;
	/// 積分した回数
	unsigned count;
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
		//contactSolver = SOLVER_PENALTY;
		contactSolver = SOLVER_CONSTRAINT;
	}
};

///	シーン
struct PHSceneIf : public SceneIf{
public:
	IF_DEF(PHScene);

	/** @brief このSceneをもつSDKを返す
		@return SDKのインタフェース
	 */
	virtual PHSdkIf* GetSdk()=0;

	/** @brief Shapeを作成する
		@param desc 作成するShapeのディスクリプタ
		@return Shapeのインタフェース
	 */
	virtual CDShapeIf* CreateShape(const CDShapeDesc& desc)=0;
	
	/** @brief Solidを作成する
		@return Solidのインタフェース

		PHSolidDescのデフォルト値を用いてSolidを作成する．
	 */
	virtual PHSolidIf* CreateSolid()=0;
	
	/** @brief Solidを作成する
		@param desc 作成するSolidのディスクリプタ
		@return Solidのインタフェース
	*/
	virtual PHSolidIf* CreateSolid(const PHSolidDesc& desc)=0;
	
	/** @brief Solidの数を取得する
		@return Solidの数
	 */
	virtual int NSolids()=0;

	/** @brief Solidを取得する
		@return Solidのインタフェースの配列へのポインタ
	 */
	virtual PHSolidIf** GetSolids()=0;

	/** @brief 特定のSolidの組について接触を有効化/無効化する
		@param bEnable trueならば有効化する．falseならば無効化する．

		Solid lhsとSolid rhsとの接触を有効化/無効化する．
		無効化された場合，衝突判定や接触力の計算は行われない．
	  */
	virtual void EnableContact(PHSolidIf* lhs,	PHSolidIf* rhs, bool bEnable = true)=0;

	/** @brief 関節を作成する
		@param desc 関節のディスクリプタ
		@return 関節のインタフェース
		Solid lhsとSolid rhsを連結する関節を作成する．
	 */
	virtual PHJointIf* CreateJoint(PHSolidIf* lhs, PHSolidIf* rhs, const PHJointDesc& desc)=0;

	/** @brief 積分ステップを取得する
		@return 積分ステップ
	 */
	virtual double GetTimeStep()const=0;
	
	/** @brief 積分ステップを設定する
		@param dt 積分ステップ
	 */
	virtual void SetTimeStep(double dt)=0;
	
	/** @brief カウント数を取得する
		@return カウント数
		シーンが作成されてから現在までにStep()を実行した回数を返す．
	 */
	virtual unsigned GetCount()const=0;

	/** @brief カウント数を設定する
		@param count カウント数
	 */
	virtual void SetCount(unsigned count)=0;
	
	/** @brief シーンの時刻を進める
	 */
	virtual void Step()=0;

	/** @brief シーンを空にする
	 */
	virtual void Clear()=0;

	/** @brief 重力を設定する
		@param accel 重力加速度ベクトル
	 */
	//ここに持たせるべきか要検討だが，Novodexはここ 
	virtual void SetGravity(Vec3f accel)=0;
	
	/** @brief 重力を取得する
		@return 重力加速度ベクトル
	 */
	virtual Vec3f GetGravity()=0;

};

//@}

}	//	namespace Spr
#endif
