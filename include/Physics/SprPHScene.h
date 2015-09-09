/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/** \addtogroup gpPhysics 	*/
//@{

/**
 *	@file SprPHScene.h
 *	@brief シーン
*/
#ifndef SPR_PHSCENEIF_H
#define SPR_PHSCENEIF_H
#include <Foundation/SprScene.h>
#include <Physics/SprPHSolid.h>
#include <Physics/SprPHJoint.h>
#include <Physics/SprPHIK.h>
#include <Physics/SprPHEngine.h>
#include <Physics/SprPHFemMesh.h>
#include <Physics/SprPHOpObj.h>
#include <Physics/SprPHFemMeshNew.h>
#include <Physics/SprPHHaptic.h>
#include <Physics/SprPHSkeleton.h>
#include <Physics/SprPHOpSpHashAgent.h>

namespace Spr{;

struct PHSdkIf;
struct CDShapeIf;
struct CDShapeDesc;
struct PHEngineIf;
struct PHConstraintEngineIf;
struct PHGravityEngineIf;
struct PHPenaltyEngineIf;

/// Ray castの結果保存用
struct PHRaycastHit{
	PHSolidIf* solid;
	CDShapeIf* shape;
	Vec3f	 point;
	float	 distance;
};

struct PHRayDesc{
	Vec3d origin;
	Vec3d direction;
};

struct PHRayIf : SceneObjectIf{
public:
	SPR_IFDEF(PHRay);
	Vec3d	GetOrigin();
	void	SetOrigin(const Vec3d& ori);
	Vec3d	GetDirection();
	void	SetDirection(const Vec3d& dir);
	void	Apply();
	int		NHits();
	PHRaycastHit* GetHits();
	PHRaycastHit* GetNearest();
	PHRaycastHit* GetDynamicalNearest();
};

/// 物理エンジンのシーンの状態
struct PHSceneState{
	/// 積分ステップ[s]
	double timeStep;
	double haptictimeStep;
	/// 積分した回数
	unsigned count;

	PHSceneState(){Init();}
	void Init(){
		timeStep       = 0.005;
		haptictimeStep = 0.001;
		count          = 0;
	}
};
/// 物理エンジンのシーンのデスクリプタ
struct PHSceneDesc: public PHSceneState{
	/// Broad phaseのモード
	enum BroadPhaseMode{
		MODE_SORT_AND_SWEEP_X,	///< AABBを一方向にソートして走査
		MODE_SORT_AND_SWEEP_Y,
		MODE_SORT_AND_SWEEP_Z,
		MODE_PARTITION,			///< 空間を分割
	};

	///	接触のモード
	enum ContactMode{
		MODE_NONE,		///< 無視する
		MODE_PENALTY,	///< ペナルティ法で解く
		MODE_LCP		///< LCPで解く
	};

	/// 物性（摩擦係数、跳ね返り係数）の合成則
	enum MatrialBlending{
		BLEND_MIN,		///< 最小値
		BLEND_MAX,		///< 最大値
		BLEND_AVE_ADD,	///< 加法平均
		BLEND_AVE_MUL,	///< 乗法平均
	};

	/// LCPの解法
	enum LCPSolver{
		SOLVER_GS,		///< ガウスーザイデル法
		SOLVER_CG,		///< 共役勾配法 (未実装：試したが遅かったので未コミットtazz)
	};
	
	/** 設定パラメータ．
		各EngineのAPIを介して全パラメータが取得・設定可能だが，以下のパラメータは
		頻繁に用いるのでPHSceneIfからもアクセスできるようにしてある．（要検討）
	 */
	Vec3d	gravity;			///< 重力加速度ベクトル
	double	airResistanceRate;	///< 毎ステップ剛体の速度に掛けられる倍率
	double	contactTolerance;	///< 接触の許容交差量
	double	impactThreshold;	///< 衝突速度．これよりも大きな速度で接触したら衝突として扱う
	double	frictionThreshold;	///< 動摩擦速度．接触面に変更な速度成分がこれよりも大きい場合に動摩擦として扱う
	double  maxDeltaPosition;	///< 1ステップあたりの移動量の上限
	double  maxDeltaOrientation;///< 1ステップあたりの回転量の上限
	double	maxVelocity;		///< 剛体および関節の最大速度．これをこえたらクリッピングされる
	double	maxAngularVelocity;	///< 剛体および関節の最大角速度
	double  maxForce;			///< 剛体に作用する外力，関節の拘束力，接触力の最大値
	double  maxMoment;			///< 同モーメントの最大値
	int		numIteration;		///< LCPの反復回数
	int		method;
	bool	bContactDetectionEnabled;	///< 接触判定が有効か．これがfalseだと接触判定自体を行わない
	bool	bCCDEnabled;				///< Continuous Collision Detectionの有効化
	int     broadPhaseMode;
	int     blendMode;
	
	PHSceneDesc(){Init();}
	void Init();	
};

///	シーン
struct PHSceneIf : public SceneIf{
public:
	SPR_IFDEF(PHScene);

	/** @brief このSceneをもつSDKを返す
		@return SDKのインタフェース
	 */
	PHSdkIf* GetSdk();

	/** @brief Solidを作成する
		@param desc 作成するSolidのディスクリプタ
		@return Solidのインタフェース
	*/
	PHSolidIf* CreateSolid(const PHSolidDesc& desc = PHSolidDesc());
	
	/** @brief Solidの数を取得する
		@return Solidの数
	 */
	int NSolids()const;

	/** @brief Solidを取得する
		@return Solidのインタフェースの配列へのポインタ
	 */
	PHSolidIf** GetSolids();

	/** @brief SolidPairの数を取得する
		@return SolidPairの数
	*/
	int NSolidPairs()const;

	/** @brief SolidPairを取得する
		@return 
	*/
	PHSolidPairForLCPIf* GetSolidPair(int i, int j);

	/** @brief SolidPairを取得する
		@param lhs 組みの片方の剛体へのポインタ
		@param rhs 組みのもう片方の剛体へのポインタ
		@param bSwap スワップされた場合はsoiid[0] = lhs, solid[1] = rhsとなる
		@return 剛体のペア
	*/
	PHSolidPairForLCPIf* GetSolidPair(PHSolidIf* lhs, PHSolidIf* rhs, bool& bSwap);


	/** @brief 指定した剛体同士の接触のモードを設定する
		@param lhs 組の片方の剛体へのポインタ
		@param rhs 組のもう片方の剛体へのポインタ
		@param mode 接触のモード

		剛体lhsと剛体rhsとの接触のモードをmodeに設定する．
	  */
	void SetContactMode(PHSolidIf* lhs,	PHSolidIf* rhs, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP);

	/** @brief 指定した集合に含まれる全ての剛体同士の接触のモードを設定する
		@param group 剛体へのポインタ配列の先頭アドレス
		@param size ポインタ配列の長さ
		@param mode 接触のモード

		group[0]からgroup[size-1]までの全ての剛体の組の接触のモードをmodeに設定する．
	 */
	void SetContactMode(PHSolidIf** group, size_t length, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP);

	/** @brief 指定した剛体と他の剛体との全ての接触のモードを設定する
		@param mode 接触のモード
	 */
	void SetContactMode(PHSolidIf* solid, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP);

	/** @brief シーンが保有する全ての剛体同士の接触のモードを設定する
		@param mode 接触のモード
	 */
	void SetContactMode(PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP);

	/** @brief 関節を作成する
		@param lhs 関節を取り付ける剛体
		@param rhs 関節を取り付ける剛体
		@param desc 関節のディスクリプタ
		@return 関節のインタフェース
		剛体lhsと剛体rhsを連結する関節を作成する．
		descには作成したい関節の種類に対応するディスクリプタ（PHJointDescから派生する）を渡す．
		lhsにソケットが，rhsにプラグが取り付けられる．
	 */
	PHJointIf* CreateJoint(PHSolidIf* lhs, PHSolidIf* rhs, const IfInfo* ii, const PHJointDesc& desc);
	template <class T> PHJointIf* CreateJoint(PHSolidIf* lhs, PHSolidIf* rhs, const T& desc){
		return CreateJoint(lhs, rhs, T::GetIfInfo(), desc);
	}
	
	/** @brief 関節の数を取得する
	 */
	int NJoints()const;

	/** @brief 関節を取得する
	 */
	PHJointIf* GetJoint(int i);

	/** @brief 接触拘束の数を取得する
	 */
	int NContacts()const;

	/** @brief 接触拘束を取得する
	 */
	PHContactPointIf* GetContact(int i);

	/** @brief FEM Meshの数を取得する
	 */
	int NFemMeshes()const;

	/** @brief FEM Meshを取得する
	 */
	PHFemMeshIf* GetFemMesh(int i);

	/** @brief FEM MeshNewの数を取得する
	 */
	int NFemMeshesNew()const;

	/** @brief FEM MeshNewを取得する
	 */
	PHFemMeshNewIf* GetFemMeshNew(int i);

	/** @brief ルートノードを作成する
		@param root ルートノードとなる剛体
		@return ルートノードのインタフェース
		関節ツリーの根となるノードを作成する．
	 */
	PHRootNodeIf* CreateRootNode(PHSolidIf* root, const PHRootNodeDesc& desc = PHRootNodeDesc());

	/** @brief ルートノードの数
	 */
	int NRootNodes()const;

	/** @brief ルートノードを取得する
	 */
	PHRootNodeIf* GetRootNode(int i);

	/** @brief ツリーノードを作成する
		@param parent 親ノード
		@param child  子ノードとなる剛体
		@return       ツリーノード
		剛体childを参照するツリーノードを作成し，既存のノードparentの子ノードとして追加する．
		parentが指す剛体とchildをつなぐ関節はCreateTreeNodeの呼び出しよりも前に
		CreateJointによって作成されていなければならない．
		さらに，parentがソケット側，childがプラグ側である必要がある．
	 */
	PHTreeNodeIf* CreateTreeNode(PHTreeNodeIf* parent, PHSolidIf* child, const PHTreeNodeDesc& desc = PHTreeNodeDesc());

	/** @brief 関節の接続関係を抽出してツリー構造を自動生成する
		@param solid 根となる剛体
	 */
	PHRootNodeIf* CreateTreeNodes(PHSolidIf* solid);

	/** @brief ギアを作成する
		@param lhs ギアで連動させる関節
		@param rhs ギアで連動させる関節
		@param desc ギアのディスクリプタ
		@return ギアのインタフェース
		1自由度関節lhsとrhsを連動させるギアを作成する．
		連動された関節は，
		rhs->GetPosition() = desc.ratio * lhs->GetPosition()
		を満たすように拘束される．
	 */
	PHGearIf* CreateGear(PH1DJointIf* lhs, PH1DJointIf* rhs, const PHGearDesc& desc = PHGearDesc());
	
	/** @brief ギアの数
	 */
	int NGears()const;

	/** @brief ギアを取得する
	 */
	PHGearIf* GetGear(int i);

	/** @brief パスを作成する
		@param desc パスのディスクリプタ
		パスを作成する．
		パスはPHPathJointの軌道を表現するために用いる．
	 */
	PHPathIf* CreatePath(const PHPathDesc& desc = PHPathDesc());

	/** @brief パスの数
	*/
	int NPaths();

	/** @brief パスを取得する
	*/
	PHPathIf* GetPath(int i);

	/** @brief raycastの光線を作成する
	 */
	PHRayIf* CreateRay(const PHRayDesc& desc = PHRayDesc());

	/** @brief 光線の数
	 */
	int NRays();

	/** @brief 光線を取得する
	 */
	PHRayIf* GetRay(int i);

	/** @brief IKのノード（IKに用いることのできる可動物(=関節など)）を作成する．
	 */
	PHIKActuatorIf* CreateIKActuator(const IfInfo* ii, const PHIKActuatorDesc& desc = PHIKActuatorDesc());
	template <class T> PHIKActuatorIf* CreateIKActuator(const T& desc){
		return CreateIKActuator(T::GetIfInfo(), desc);
	}

	/** @brief IKアクチュエータの数
	 */
	int NIKActuators();

	/** @brief IKアクチュエータを取得する
	 */
	PHIKActuatorIf* GetIKActuator(int i);

	/** @brief IKのエンドエフェクタを作成する．
	 */
	PHIKEndEffectorIf*	 CreateIKEndEffector(const PHIKEndEffectorDesc& desc = PHIKEndEffectorDesc());

	/** @brief IKエンドエフェクタの数
	 */
	int NIKEndEffectors();

	/** @brief IKエンドエフェクタを取得する
	 */
	PHIKEndEffectorIf* GetIKEndEffector(int i);


	/** @brief スケルトン（ボーンの集合）の数
	 */
	int NSkeletons();

	/** @brief スケルトン（ボーンの集合）を取得する
	 */
	PHSkeletonIf* GetSkeleton(int i);

	/** @brief スケルトン（ボーンの集合）を作成する
	 */
	PHSkeletonIf* CreateSkeleton(const PHSkeletonDesc& desc = PHSkeletonDesc());


	/** @brief 積分ステップを取得する
		@return 積分ステップ
	 */
	double GetTimeStep()const;

	/** @brief 積分ステップの逆数を取得する
		@return 積分ステップの逆数
	 */
	double GetTimeStepInv()const;

	/** @brief 積分ステップを設定する
		@param dt 積分ステップ
	 */
	void SetTimeStep(double dt);

	/** @brief 積分ステップを取得する
		@return 積分ステップ
	 */
	double GetHapticTimeStep()const;

	/** @brief 積分ステップを設定する
		@param dt 積分ステップ
	 */
	void SetHapticTimeStep(double dt);

	/** @brief カウント数を取得する
		@return カウント数
		シーンが作成されてから現在までにStep()を実行した回数を返す．
	 */
	unsigned GetCount()const;

	/** @brief カウント数を設定する
		@param count カウント数
	 */
	void SetCount(unsigned count);

	/** @brief 重力を設定する
		@param accel 重力加速度ベクトル
	 */
	//ここに持たせるべきか要検討だが，Novodexはここ 
	void SetGravity(const Vec3d& accel);
	
	/** @brief 重力を取得する
		@return 重力加速度ベクトル
	 */
	Vec3d GetGravity();

	/** @brief 回転に対する空気抵抗の割合を設定する
		@param rate 回転に対する空気抵抗の割合
		標準は1.0 比率を下げるとシミュレーションが安定する(PHSolid::UpdateVelocity()内で呼ばれる）
	 */
	void    SetAirResistanceRate(double rate);
	
	/** @brief 回転に対する空気抵抗の割合を取得する
		@return 回転に対する空気抵抗の割合
	 */
	double  GetAirResistanceRate();

	/// @brief 接触の許容交差量を設定する
	void    SetContactTolerance(double tol);
	/// @brief 接触の許容交差量を取得する
	double  GetContactTolerance();

	/// 衝突と静接触の閾値
	void    SetImpactThreshold(double vth);
	double  GetImpactThreshold();

	/// 静摩擦と動摩擦の閾値
	void    SetFrictionThreshold(double vth);
	double  GetFrictionThreshold();

	/// 許容最大速度．超えたら飽和
	void    SetMaxVelocity        (double vmax);
	double  GetMaxVelocity        ();
	/// 許容最大角速度．超えたら飽和
	void    SetMaxAngularVelocity (double wmax);
	double  GetMaxAngularVelocity ();
	/// 許容最大力．超えたら飽和
	void    SetMaxForce           (double fmax);
	double  GetMaxForce           ();
	/// 許容最大モーメント．超えたら飽和
	void    SetMaxMoment          (double tmax);
	double  GetMaxMoment          ();
	/// 許容最大移動量．超えないようにdtを調整
	void    SetMaxDeltaPosition   (double dpmax);
	double  GetMaxDeltaPosition   ();
	/// 許容最大回転量．超えないようにdtを調整
	void    SetMaxDeltaOrientation(double dqmax);
	double  GetMaxDeltaOrientation();
	
	/// @brief LCPソルバの選択
	int  GetLCPSolver();
	void SetLCPSolver(int method);

	///	@brief LCPソルバの計算回数の取得．MODE_LCPの場合の拘束力の繰り返し近似計算の回数．
	int GetNumIteration();
	///	@brief LCPソルバの計算回数の設定．
	void SetNumIteration(int n);
	
	/** @brief 状態の保存 (ObjectStates の保存）に，
		Constraints が持つ拘束力を含めるかどうか設定する．
	*/
	void SetStateMode(bool bConstraints);

	/** @brief 接触判定エンジンの有効・無効を設定する．SetContactModeより優先する．
	*/
	void EnableContactDetection(bool enable);
	bool IsContactDetectionEnabled();

	void EnableCCD(bool enable);
	bool IsCCDEnabled();

	/// 衝突判定のbroad phase手法を設定
	void SetBroadPhaseMode(int mode);
	int  GetBroadPhaseMode();

	/** 衝突判定処理の対象範囲を設定 (MODE_PARTITION) 
		@param center	領域の中心
		@param extent   領域のサイズ
		@param nx       x方向分割数
		@param ny       y方向分割数
		@param nz       z方向分割数
	 */
	void SetContactDetectionRange(Vec3f center, Vec3f extent, int nx, int ny, int nz);

	/** 物性（摩擦係数、跳ね返り係数）の合成ルールを設定
	 */
	void SetMaterialBlending(int mode);

	/** @brief シーンの時刻を進める
	 */
	void Step();
	
	void ClearForce();
	void GenerateForce();
	void Integrate();
	void IntegratePart1();
	void IntegratePart2();

	/** @brief シーンを空にする
	 */
	void Clear();

	/// エンジンアクセス用API	(公開の是非については要検討)

	int NEngines();
	/** @brief i番目のエンジンを取得する
		@param i エンジン番号のインデックス
		@return 選択したエンジンへのポインタ
	*/
	PHEngineIf* GetEngine(int i);

	/** @brief ConstraintEngineを取得する
		@return PHConstraintEngineへのポインタ
	*/
	PHConstraintEngineIf* GetConstraintEngine();

	/** @brief GravityEnigneを取得する
		@return GravityEngineへのポインタ
	*/
	PHGravityEngineIf*	GetGravityEngine();

	/** @brief PenaltyEngineを取得する
		@return PenaltyEngineへのポインタ
	*/
	PHPenaltyEngineIf*  GetPenaltyEngine();

	/** @brief IKEngineを取得する
		@return IKEngineへのポインタ
	*/
	PHIKEngineIf*  GetIKEngine();

	/** @brief FemEngineを取得する
		@return FemEngineへのポインタ
	*/
	PHFemEngineIf*  GetFemEngine();

	/** @brief HapticEngineを取得する
		@return HapticEngineへのポインタ
	*/
	PHHapticEngineIf* GetHapticEngine();

	/** @brief OrientedParticleEngineを取得する
	@return OrientedParticleEngineへのポインタ
	*/
	PHOpEngineIf* GetOpEngine();

	/** @brief Op衝突判定のagentを取得する
	@return agentへのポインタ
	*/
	PHOpSpHashColliAgentIf* GetOpColliAgent();

	/** @breif HapticPointerを作成する
		@retrun HapticPointerへのポインタ
	*/
	PHHapticPointerIf* CreateHapticPointer();
	
	/** @breif HapticLoopを更新する
	*/
	void StepHapticLoop();
};

//@}

}	//	namespace Spr
#endif
