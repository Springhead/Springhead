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

namespace Spr{;

struct PHSdkIf;

struct CDShapeIf;
struct CDShapeDesc;
struct PHEngineIf;
struct PHConstraintEngineIf;
struct PHGravityEngineIf;
struct PHPenaltyEngineIf;

typedef PHConstraintDesc PHJointDesc;

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
	///	接触のモード
	enum ContactMode{
		MODE_NONE,		///< 無視する
		MODE_PENALTY,	///< ペナルティ法で解く
		MODE_LCP		///< LCPで解く
	};
	
	Vec3f	gravity;	///< 重力加速度ベクトル．デフォルト値は(0.0f, -9.8f, 0.0f)．
	int		numIteration;	///< LCPの反復回数

	PHSceneDesc(){Init();}
	void Init(){
		gravity=Vec3f(0,-9.8f,0);
		numIteration = 15;
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

	/** @brief Solidを作成する
		@param desc 作成するSolidのディスクリプタ
		@return Solidのインタフェース
	*/
	virtual PHSolidIf* CreateSolid(const PHSolidDesc& desc = PHSolidDesc())=0;
	
	/** @brief Solidの数を取得する
		@return Solidの数
	 */
	virtual int NSolids()const =0;

	/** @brief Solidを取得する
		@return Solidのインタフェースの配列へのポインタ
	 */
	virtual PHSolidIf** GetSolids()=0;

	/** @brief 指定した剛体同士の接触のモードを設定する
		@param lhs 組の片方の剛体へのポインタ
		@param rhs 組のもう片方の剛体へのポインタ
		@param mode 接触のモード

		剛体lhsと剛体rhsとの接触のモードをmodeに設定する．
	  */
	virtual void SetContactMode(PHSolidIf* lhs,	PHSolidIf* rhs, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP)=0;

	/** @brief 指定した集合に含まれる全ての剛体同士の接触のモードを設定する
		@param group 剛体へのポインタ配列の先頭アドレス
		@param size ポインタ配列の長さ
		@param mode 接触のモード

		group[0]からgroup[size-1]までの全ての剛体の組の接触のモードをmodeに設定する．
	 */
	virtual void SetContactMode(PHSolidIf** group, size_t length, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP)=0;

	/** @brief 指定した剛体と他の剛体との全ての接触のモードを設定する
		@param mode 接触のモード
	 */
	virtual void SetContactMode(PHSolidIf* solid, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP)=0;

	/** @brief シーンが保有する全ての剛体同士の接触のモードを設定する
		@param mode 接触のモード
	 */
	virtual void SetContactMode(PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP)=0;

	///	@brief LCPソルバの計算回数の取得．MODE_LCPの場合の拘束力の繰り返し近似計算の回数．
	virtual int GetNumIteration()=0;
	///	@brief LCPソルバの計算回数の設定．
	virtual void SetNumIteration(int n)=0;
	
	/** @brief 関節を作成する
		@param lhs 関節を取り付ける剛体
		@param rhs 関節を取り付ける剛体
		@param desc 関節のディスクリプタ
		@return 関節のインタフェース
		剛体lhsと剛体rhsを連結する関節を作成する．
		descには作成したい関節の種類に対応するディスクリプタ（PHJointDescから派生する）を渡す．
		lhsにソケットが，rhsにプラグが取り付けられる．
	 */
	virtual PHJointIf* CreateJoint(PHSolidIf* lhs, PHSolidIf* rhs, const IfInfo* ii, const PHJointDesc& desc)=0;
	template <class T> PHJointIf* CreateJoint(PHSolidIf* lhs, PHSolidIf* rhs, const T& desc){
		return CreateJoint(lhs, rhs, T::GetIfInfo(), desc);
	}
	
	/** @brief 関節の数を取得する
	 */
	virtual int NJoints()const=0;

	/** @brief 関節を取得する
	 */
	virtual PHJointIf* GetJoint(int i)=0;

	/** @brief 接触拘束の数を取得する
	 */
	virtual int NContacts()const=0;

	/** @brief 接触拘束を取得する
	 */
	virtual PHContactPointIf* GetContact(int i)=0;

	/** @brief ルートノードを作成する
		@param root ルートノードとなる剛体
		@return ルートノードのインタフェース
		関節ツリーの根となるノードを作成する．
	 */
	virtual PHRootNodeIf* CreateRootNode(PHSolidIf* root, const PHRootNodeDesc& desc = PHRootNodeDesc())=0;

	/** @brief ルートノードの数
	 */
	virtual int NRootNodes()const=0;

	/** @brief ルートノードを取得する
	 */
	virtual PHRootNodeIf* GetRootNode(int i)=0;

	/** @brief ツリーノードを作成する
		@param parent 親ノードのインタフェース
		@param child 子ノードとなる剛体
		@return ツリーノードのインタフェース
		剛体childを参照するツリーノードを作成し，既存のノードparentの子ノードとして追加する．
		parentが指す剛体とchildをつなぐ関節はCreateTreeNodeの呼び出しよりも前に
		CreateJointによって作成されていなければならない．
		さらに，parentがソケット側，childがプラグ側である必要がある．
	 */
	virtual PHTreeNodeIf* CreateTreeNode(PHTreeNodeIf* parent, PHSolidIf* child, const PHTreeNodeDesc& desc = PHTreeNodeDesc())=0;

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
	virtual PHGearIf* CreateGear(PHJoint1DIf* lhs, PHJoint1DIf* rhs, const PHGearDesc& desc = PHGearDesc())=0;
	
	/** @brief ギアの数
	 */
	virtual int NGears()const=0;

	/** @brief ギアを取得する
	 */
	virtual PHGearIf* GetGear(int i)=0;

	/** @brief パスを作成する
		@param desc パスのディスクリプタ
		パスを作成する．
		パスはPHPathJointの軌道を表現するために用いる．
	 */
	virtual PHPathIf* CreatePath(const PHPathDesc& desc = PHPathDesc())=0;

	/** @brief パスの数
	*/
	virtual int NPaths() = 0;

	/** @brief パスを取得する
	*/
	virtual PHPathIf* GetPath(int i)=0;


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
	virtual void ClearForce()=0;
	virtual void GenerateForce()=0;
	virtual void Integrate()=0;

	/** @brief シーンを空にする
	 */
	virtual void Clear()=0;

	/** @brief 重力を設定する
		@param accel 重力加速度ベクトル
	 */
	//ここに持たせるべきか要検討だが，Novodexはここ 
	virtual void SetGravity(const Vec3d& accel)=0;
	
	/** @brief 重力を取得する
		@return 重力加速度ベクトル
	 */
	virtual Vec3d GetGravity()=0;

	virtual int NEngines() = 0;
	/** @brief i番目のエンジンを取得する
		@param i エンジン番号のインデックス
		@return 選択したエンジンへのポインタ
	*/
	virtual PHEngineIf* GetEngine(int i) = 0;

	/** @brief ConstraintEngineを取得する
		@return PHConstraintEngineへのポインタ
	*/
	virtual PHConstraintEngineIf* GetConstraintEngine() = 0;

	/** @brief GravityEnigneを取得する
		@return GravityEngineへのポインタ
	*/
	virtual PHGravityEngineIf*	GetGravityEngine() = 0;

	/** @brief PenaltyEngineを取得する
		@return PenaltyEngineへのポインタ
	*/
	virtual PHPenaltyEngineIf*  GetPenaltyEngine() = 0;

};

//@}

}	//	namespace Spr
#endif
