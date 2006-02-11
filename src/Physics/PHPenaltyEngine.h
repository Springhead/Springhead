#ifndef PHPENALTYENGINE_H
#define PHPENALTYENGINE_H

#include <SprPhysics.h>
#include <Foundation/Object.h>
#include <Physics/PHScene.h>
#include <Physics/PHEngine.h>
#include <Base/Combination.h>
#include <Collision/CDDetectorImp.h>

namespace Spr{;


class PHPenaltyEngine: public PHEngine{
	OBJECTDEF(PHPenaltyEngine);

	
	class PHShapePair:public CDShapePair{
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

		PHShapePair(CDShape* s1, CDShape* s2):CDShapePair(s1, s2){
		}
		void Clear();

		///@name	絶対座標系での読み出し
		//@{
		///	バネによる抗力の作用点
		Vec3f GetReflexForcePoint(){ return reflexForcePoint + commonPoint;}
		///	バネによる抗力
		Vec3f GetReflexSpringForce(){ return reflexSpringForce;}
		///	ダンパによる抗力
		Vec3f GetReflexDamperForce(){ return reflexDamperForce;}
		///	抗力
		Vec3f GetReflexForce(){ return reflexSpringForce + reflexDamperForce;}

		///	摩擦力
		Vec3f GetFrictionForce(){ return frictionForce; }
		///	摩擦トルク
		Vec3f GetFrictionTorque(){ return frictionTorque + (GetReflexForcePoint()^frictionForce); }
		//@}
	};


	class PHSolidInfo{
	public:
		PHSolid* solid;
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
	};


	class PHSolidPair: public UTRefCount{
	public:
		PHSolidInfo solid[2];
		UTCombination<UTRef< PHShapePair> > shapePairs;

		Vec3f cocog;					///<	2剛体の重心の中点(絶対系)
		Vec3f reflexForce;				///<	抗力
		Vec3f reflexTorque;				///<	抗力によるトルク(cocog系)
		Vec3f frictionForce;			///<	摩擦力
		Vec3f frictionTorque;			///<	摩擦力によるトルク(cocog系)
		float area;

		PHSolidPair(PHSolid* s0, PHSolid* s1){
			solid[0].solid = s0;
			solid[1].solid = s1;
		}
		void Init();
		void Clear();
		bool Detect(PHPenaltyEngine* engine);

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
		void CalcReflexForce(PHShapePair* sp, CDContactAnalysis* analyzer);
		void CalcTriangleReflexForce(PHShapePair* cp, Vec3f p0, Vec3f p1, Vec3f p2, Vec3f cog, Vec3f vel, Vec3f angVel);
		void CalcFriction(PHShapePair* cp);
	};


protected:
	bool	 ready;
	PHSolids solids;
	UTCombination<UTRef<PHSolidPair> > solidPairs;
public:
	void Add(PHSolid* s);	/// Solidを登録する
	void Invalidate(){ready = false;}		/// readyフラグをリセットする
	void Init();			/// 初期化し，readyフラグをセットする
	///
	int GetPriority() const {return SGBP_PENALTYENGINE;}
	///	速度→位置、加速度→速度の積分
	virtual void Step();
	virtual void Clear(PHScene* s){ solids.clear(); }

	PHPenaltyEngine():ready(false){}
};

}	//	namespace Spr
#endif
