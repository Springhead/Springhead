#ifndef PHCHANGEOBJECT_H
#define PHCHANGEOBJECT_H

#include <Collision/CDFramePair.h>
#include <Collision/CDCollisionEngine.h>
#include "PHSolid.h"

namespace Spr{;

/** Solidの入れ替えを行うクラスの基本クラス．PHChangeObjectContainerが持つ */
class PHChangeObject:public SGObject{
	SGOBJECTDEFABST(PHChangeObject);
protected:

public:
	UTRef<PHSolid> solid[2];		///<	入れ替え前[0]と入れ替え後[1]の剛体

	/// IsChange()の結果に応じてChange()を実行する．PHChangeObjectContainer::Step()から呼ばれる
	void Step();
	/// Solidの入れ替えを行う
	void Change();
	/// 入れ替え処理を行うかどうか判断する
	virtual bool IsChange()=0;
	/// クリア
	virtual void Clear()=0;
};

///	PHChangeObjectの配列
class PHChangeObjects:public std::vector< UTRef<PHChangeObject> >{
};

/** チェンジオブジェクトコンテナ．BehaviorEngineの一種 */
class PHChangeObjectContainer:public SGBehaviorEngine{
	SGOBJECTDEF(PHChangeObjectContainer);
	public:
		PHChangeObjects cos;
		/// 
		bool AddChildObject(SGObject* o, SGScene* s);
		///
		int GetPriority() const {return SGBP_CHANGEOBJECT;}
		virtual void Step(SGScene* s);
		virtual void Loaded(SGScene* scene);
		virtual void Clear(SGScene* s){ cos.clear(); }
		virtual size_t NChildObjects(){ return cos.size(); }
		virtual SGObject* ChildObject(size_t i){ return cos[i]; }
};

/** 指定フレームへの衝突の有無で，入れ替え処理を行うか判断 */
class PHChangeObjectCollision:public PHChangeObject, CDCollisionListener {
	SGOBJECTDEF(PHChangeObjectCollision);

public:
	UTRef<SGFrame> detectorFrame;	///<	検出用フレーム
	bool bCollision;				///<	接触フラグ
protected:
	int listenerPos;				///<	リスナの位置

public:
	///
	bool AddChildObject(SGObject* o, SGScene* s);
	/// 接触時にCDCollisionEngine::Step()から呼ばれるコールバック．接触フラグをTRUEにする
	void After(SGScene* scene, CDFramePairWithRecord* fr);
	/// クリア
	virtual void Clear();
	/// 接触フラグの結果を返し，次のステップに備えてフラグをFALSEに初期化する
	virtual bool IsChange();
	/// ロード完了時の処理，CDCollisionEngine にコールバックを登録する．CDCollisionEngine がなければ作る．
	virtual void Loaded(SGScene* scene);
};

/** Solidの姿勢と比較用姿勢行列を比べて，入れ替え処理を行うか判断 */
class PHChangeObjectOrientation:public PHChangeObject {
	SGOBJECTDEF(PHChangeObjectOrientation);

public:
	Matrix3d comparativeOrientation;	///<	比較用姿勢行列
	UTRef<SGFrame> comparativeFrame;	///<	比較用フレーム(データファイルに記述されているときだけ使う)
	Vec3d selectedAxis;					///<	比較する軸の選択(x,y,z)　この軸はSolidの座標系
	Vec3d targetedInnerProduct;			///<	目標内積値(xの内積,yの内積,内積)
	Vec3d accuracy;						///<	目標内積値の精度
	public:
	///
	bool AddChildObject(SGObject* o, SGScene* s);
	/// クリア
	virtual void Clear();
	/// 入れ替えを行うかどうかの判定
	virtual bool IsChange();
};

/** データのロード・セーブ用の構造体 */
struct PHChangeObjectOrientationInfo{
	Vec3f selectedAxis;
	Matrix3f comparativeOrientation;
	Vec3f targetedInnerProduct;
	Vec3f accuracy;
};

};
#endif
