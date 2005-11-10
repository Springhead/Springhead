#pragma once
#ifndef PHCONTACTENGINE_H
#define PHCONTACTENGINE_H

#include "PHSolid.h"
#include <SceneGraph/SGScene.h>
#include <Physics/PHConvexPair.h>
#include <Collision/CDCollisionEngine.h>
#include <Collision/CDPolyhedron.h>
#include <float.h>

namespace Spr{;


/**	接触力計算エンジン．	*/
class PHContactEngine:public SGBehaviorEngine, CDCollisionListener{
public:
	///	判定対象フレームごとのデータ
	class FrameRecord:public CDUserRecord{
	public:
		///@name	UpdateChacheで更新する変数
		//@{
		/**	最後に接触した時刻 = キャッシュパラメータを最後に更新した時刻．
			接触時に，キャッシュを更新するので，count が現在の時刻と等しければ
			衝突が起きたことを意味する．	*/
		unsigned count;
		//	フレーム(剛体)単位のパラメータ
		Vec3f cog, vel, angVel;			///<	重心，速度，角速度
		Vec3f pos, lastPos;				///<	位置，最後の位置
		Quaternionf ori, lastOri;		///<	向き，前回の向き
		//@}
		void UpdateCache(int c);		///<	キャッシュ変数を剛体などから取ってくる．
		PHSolid* solid;					///<	判定対象剛体
		FrameRecord(){ count = -1; }	///<	s
	};
	///	判定対象フレーム対ごとのデータ
	class FramePairRecord:public CDUserRecord{
	public:
		Vec3f cocog;					///<	2剛体の重心の中点(絶対系)
		Vec3f reflexForce;				///<	抗力
		Vec3f reflexTorque;				///<	抗力によるトルク(cocog系)
		Vec3f frictionForce;			///<	摩擦力
		Vec3f frictionTorque;			///<	摩擦力によるトルク(cocog系)
		///@name	絶対系での読み出し
		//@{
		///	抗力
		Vec3f GetReflexForce(){ return reflexForce; }
		///	抗力のトルク
		Vec3f GetReflexTorque(){ return reflexTorque + (cocog^reflexForce); }
		///	摩擦力
		Vec3f GetFrictionForce(){ return frictionForce; }
		///	摩擦力のトルク
		Vec3f GetFrictionTorque(){ return frictionTorque + (cocog^frictionForce); }
		//@}
		
		///	データのクリア．
		void Clear(){
			reflexForce = reflexTorque = frictionForce = frictionTorque = Vec3f();
		}
		///	力の最大値を制約する．
		void LimitForces(){
			bool b = false;
			b |= LimitForce(reflexForce);
			b |= LimitForce(frictionForce);
			b |= LimitForce(reflexTorque);
			b |= LimitForce(frictionTorque);
			if (b){
				reflexForce = frictionForce = reflexTorque = frictionTorque = Vec3f();
			}
		}
	protected:
		bool LimitForce(Vec3f& f){
			float sq = f.square();
			const float MF = 200000;
			if (!(sq < MF*MF)){
				return true;
			}
			return false;
		}
	};
	///	凸形状対ごとのデータ
	class ConvexPairRecord:public CDUserRecord{
	public:
		///@name	抗力の計算
		//@{
		float area;							///<	交差部の面積
		Vec3f reflexSpringForce;			///<	ばねによる抗力
		Vec3f reflexDamperForce;			///<	ダンパーによる抗力
		Vec3f reflexSpringTorque;			///<	ばねによる抗トルク(commonPoint系)
		Vec3f reflexDamperTorque;			///<	ダンパーによる抗トルク(commonPoint系)
		Vec3f reflexForcePoint;				///<	抗力の作用点(commonPoint系)
		//@}

		///@name摩擦の計算
		//@{
		Vec3f transFrictionBase[2];			///<	並進静止摩擦用バネの端点(ローカルフレーム系)
		float rotSpring;					///<	回転ばねの伸び(ラジアン)
		Vec3f dynaFric;						///<	動摩擦力
		Vec3f dynaFricMom;					///<	動摩擦力のモーメント(commonPoint系)
		Vec3f frictionForce;				///<	摩擦力
		Vec3f frictionTorque;				///<	摩擦トルク(摩擦力の作用点 ＝ (reflexForcePoint+commonPoint)系)
		enum FrictionState{ STATIC, DYNAMIC };
		FrictionState frictionState;		///<	摩擦の状態
		//@}
		///@name	絶対座標系での読み出し
		//@{
		///	バネによる抗力の作用点
		Vec3f GetReflexForcePoint(CDConvexPair* cp){ return reflexForcePoint + cp->commonPoint;}
		///	バネによる抗力
		Vec3f GetReflexSpringForce(){ return reflexSpringForce;}
		///	ダンパによる抗力
		Vec3f GetReflexDamperForce(){ return reflexDamperForce;}
		///	抗力
		Vec3f GetReflexForce(){ return reflexSpringForce + reflexDamperForce;}

		///	摩擦力
		Vec3f GetFrictionForce(){ return frictionForce; }
		///	摩擦トルク
		Vec3f GetFrictionTorque(CDConvexPair* cp){ return frictionTorque + (GetReflexForcePoint(cp)^frictionForce); }
		//@}
	};
private:
	bool firstLoadedCall;					///<	Loadedで複数回初期化をしないようにするためのフラグ
	float area;								///<	衝突部分の面積
	float convertedMass;					///<	換算質量
	float reflexSpring;						///<	バネ計数
	float reflexDamper;						///<	ダンパ計数
	float frictionSpring;					///<	摩擦のバネ係数
	float frictionDamper;					///<	摩擦のダンパ係数
	float staticFriction;					///<	静止摩擦係数
	float dynamicFriction;					///<	動摩擦係数
protected:
	int listenerPos;						///<	CDContactEngineの中でのリスナの位置
	int frameRecordPos;						///<	CDContactEngineの中でのフレーム単位のユーザレコードの位置
	int framePairRecordPos;					///<	CDContactEngineの中でのフレーム対単位のユーザレコードの位置
	int convexPairRecordPos;				///<	CDContactEngineの中での凸形状対単位のユーザレコードの位置
	std::vector< std::pair<DWORD, DWORD> > inactiveList;
											///<	非アクティブなペア一覧(ロード時にだけ使用)

	UTRef<CDCollisionEngine> collisionEngine;///<	衝突判定エンジン
public:	
	SGOBJECTDEF(PHContactEngine);
	std::vector< std::vector<DWORD> > inactiveListBackups;
											//<		セーブに備えて覚えておく
	PHSolids solids;						///<	ペナルティ法の対象の剛体
	size_t nSolidsInitilized;				///<	solidsのうちどこまでが初期化済みかを示す．
	///
	PHContactEngine();
	CDCollisionEngine* GetCollisionEngine(){ return collisionEngine; }
	///	子オブジェクトの追加
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	int GetListenerPos(){ return listenerPos; }
	///	FrameRecordのCDCollisionEngineの中での位置
	int GetFrameRecordPos(){ return frameRecordPos; }
	///	FramePairRecordのCDCollisionEngineの中での位置
	int GetFramePairRecordPos(){ return framePairRecordPos; }
	///	ConvexPairRecordのCDCollisionEngineの中での位置
	int GetConvexPairRecordPos(){ return convexPairRecordPos; }
	///	FrameRecordの取得
	FrameRecord* GetFrameRecord(SGFrame* fr){
		return (FrameRecord*)collisionEngine->GetFrameRecord(collisionEngine->GetFrameID(fr), frameRecordPos);
	}
	/**	FramePairRecordの取得
		FramePairRecordはフレームの対ごとに1つ．
		(fr1,fr2)と(fr2,fr1)で読んだ場合，どちらか片方はNULLを返す．
		(fr1,fr2)で値が得られたならば，FramePairRecordはf1に加わった力を持つ．
	*/
	FramePairRecord* GetFramePairRecord(SGFrame* fr1, SGFrame* fr2){
		CDFramePairWithRecord* fp = collisionEngine->GetFramePair(fr1, fr2);
		if (fp) return UTRef<FramePairRecord>(fp->records[framePairRecordPos]);
		return NULL;
	}
	///	アクティブでないフレーム対を追加
	void AddInactive(int f1, int f2);

	///	ロード完了時の処理，CollisionEngine にコールバックを登録する．CollisionEngineがなければ作る．ユーザが呼び出すことはない．
	virtual void Loaded(SGScene* scene);
	///	初期化処理、Loadedが呼ぶ
	virtual void Init(SGScene* scene);
	///	接触力の描画
	void Render(GRRender* r, SGScene* s);
protected:
	///
	int GetPriority() const { return SGBP_CONTACTENGINE; }
	///	時間を dt 進める．(何もしない，Listener の Collide がすべての処理をする．)
	virtual void Step(SGScene* s){}
	///	solidsをクリアする．
	virtual void Clear(SGScene* s);
	///	フレーム同士が衝突している場合，接触解析の前に呼び出される．抗力計算の準備を行う．
	void Before(SGScene* scene, CDFramePairWithRecord* fr);
	/**	フレーム同士が衝突している場合，接触解析の直後に凸形状対ごとに呼び出される．
		接触面積と抗力の計算を行う．*/
	virtual void Analyzed(SGScene* scene, CDFramePairWithRecord* fr, CDGeometryPair* geom, CDConvexPairWithRecord* conv, CDContactAnalysis* analyzer);
	/**	フレーム同士が衝突している場合，衝突解析が終わってからフレーム対につき1度呼び出される．
		抗力の調整，摩擦力計算を行う	*/
	void After(SGScene* scene, CDFramePairWithRecord* fr);
	///	抗力計算
	void CalcReflexForce(PHContactEngine::FrameRecord** fr, CDConvexPairWithRecord* cp,
		PHContactEngine::ConvexPairRecord* rec, CDContactAnalysis* analyzer);
	///	3角形単位の抗力計算
	void CalcTriangleReflexForce(CDConvexPairWithRecord* cp, PHContactEngine::ConvexPairRecord* rec,
		Vec3f p0, Vec3f p1, Vec3f p2, Vec3f cog, Vec3f vel, Vec3f angVel);
	void CalcFriction(PHContactEngine::FrameRecord** fr, PHContactEngine::FramePairRecord* fpr, CDConvexPairWithNormal* cp,
		PHContactEngine::ConvexPairRecord* rec);
	void DrawForce(CDConvexPair& cp, PHContactEngine::ConvexPairRecord& cpr, Affinef afw, GRRender* render, SGScene* scene);

	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;
};


}
#endif

