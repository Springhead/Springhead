#ifndef PH_JOINTMULTI_H
#define PH_JOINTMULTI_H

#include <Physics/PHJoint.h>

namespace Spr{;

///	他自由度の関節の基本クラス
template <int NDOF>
class PHJointMulti:public PHJointBase{
public:
	typedef PTM::TVector<NDOF, double> AVec;
	AVec torque;							///<	トルク
	AVec accel;								///<	関節加速度
	AVec delta_position;					///<	変位の変分
	AVec velocity;							///<	速度
	void AddTorque(AVec t){ torque+=t; }	///<	トルクを追加
	void SetTorque(AVec t){ torque=t; }		///<	トルクを設定
	AVec GetTorque(){ return torque; }		///<	トルクを取得
	AVec GetVelocity(){ return velocity; }	///<	関節速度を取得
	///	関節の自由度
	virtual int GetJointDof(){ return NDOF; }
	///	関節速度の取得
	virtual double GetJointVelocity(int i){ return velocity[i]; }
	///	関節加速度の取得
	virtual double GetJointAccel(int i){ return accel[i]; }
	///	関節トルクの取得
	virtual double GetJointTorque(int i){ return torque[i]; }
	///	関節トルクの設定
	virtual void SetJointTorque(double v, int i){ torque[i] = v; }
	///	関節トルクを加える
	virtual void AddJointTorque(double v, int i){ torque[i] += v; }
	///	articulated inertia & ZA-force
	void CompArticulatedInertia(double dt){
		//	子ジョイントのIa,Zaの計算．
		PHJointBase::CompArticulatedInertia(dt);
		Iss = (S_tr * Ia * S).inv();
		//親ノードのZa,Iaに自分のZa,Iaを積み上げる
		//Ia
		OfParent(&PHJointMulti::Ia) += 
			pXc_Mat_cXp( Ia - (Ia * S*Iss*S_tr * Ia) );
		//Za
		OfParent(&PHJointMulti::Za) += 
			pXc_Vec( Z_plus_Ia_c + Ia*S*Iss*(torque - S_tr*Z_plus_Ia_c) );
	}
	/**	派生クラスのIntegrate() が呼び出す関数．
		派生クラスのIntegrate()の例：
		void Integrate(double dt){
			PreIntegrate(dt);
			//	ここで，delta_position から，関節の姿勢を計算．
			PropagateState();
			PHJointBase::Integrate(dt);
		}
	*/
	void PreIntegrate(SGScene* scene){
		double dt = scene->GetTimeStep();
		a_p = cXp_Vec(OfParent(&PHJointMulti::a));

		if (intType == SYMPLETIC){
			//	x(dt) = x(0) + dt*v(0)/m
			//	v(dt) = v(0) + dt*f(dt)
			//加速度を計算
			accel = Iss*(torque - S_tr*(Z_plus_Ia_c + Ia*a_p));
			//速度を積分
			velocity += accel * scene->GetTimeStep();
			//位置を積分する準備
			delta_position = velocity * scene->GetTimeStep();
		}else{
			//加速度を計算
			accel = Iss*(torque - S_tr*(Z_plus_Ia_c + Ia*a_p));
			//位置を積分する準備
			delta_position = (velocity + 0.5 * accel * dt) * dt;
			//速度を積分
			velocity += accel * dt;
		}
		velocity *= scene->GetVelocityLossPerStep();
		//重心周りの加速度(子ノードの積分で使用する)
		a = a_p + c + S*accel;
		if (velocity.norm() > PHJOINT_MAX_VELOCITY){
			DSTR << "The velocity of " << GetName() << ":" << velocity << "was limited" << std::endl;
			if (_finite(velocity[0]) ){
				velocity = PHJOINT_MAX_VELOCITY * velocity.unit();
			}else{
				velocity.clear();
			}
		}
	}
	void CalcAccel(double dt){				///<	このジョイントの加速度計算．詳細は基本クラスのコメントを参照．
		GetParent()->CalcAccel(dt);	//	親の加速度を計算
		a_p = cXp_Vec(OfParent(&PHJointMulti::a));
		//加速度を計算
		accel = Iss*(torque - S_tr*(Z_plus_Ia_c + Ia*a_p));
		//重心周りの加速度(子ノードの積分で使用する)
		a = a_p + c + S*accel;
	}
protected:
	typedef PTM::TMatrixCol<6, NDOF, double>	SMat;
	typedef PTM::TMatrixCol<NDOF, 6, double>	SMatTr;
	SMat S;									///<	spatial joint axis in Fc coord.
	SMatTr S_tr;							///<	S' Sの独自の転地.
	PTM::TMatrixCol<NDOF, NDOF, double> Iss;///<	S'IaS のキャッシュ
	virtual void Reset(){
		accel.clear();
		torque.clear();
		PHJointBase::Reset();
	}
/*
	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;
*/	
	virtual void ClearTorqueRecursive(){
		PHJointBase::ClearTorqueRecursive();
		torque.clear();
	}
};


/**	ボールジョイント，軸は子剛体側に固定される．*/
class PHJointBall:public PHJointMulti<3>{
	double massFactor;
public:
	SGOBJECTDEF(PHJointBall);
	Quaterniond position;					///<	関節の向き
	Vec3d center;			//	可動域の中心
	double minDot;			//	可動範囲
	double minTwist;			//	ひねりの可動範囲
	double maxTwist;			//	ひねりの可動範囲

	PHJointBall();
	///	関節位置の取得
	virtual double GetJointPosition(int i){ return position.rotation()[i]; }

protected:
	virtual void Integrate(SGScene* scene);
	virtual void CompJointAxis();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	virtual void CompCoriolisAccel();
	virtual void Loaded(SGScene* s);
	virtual double MassFactor();
	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;
};

/**	ユニバーサルジョイント，軸は子剛体側に固定される x軸と y軸が動く．*/
class PHJointUniversal:public PHJointMulti<2>{
public:
	SGOBJECTDEF(PHJointUniversal);
	Vec2d position;					///<	関節の向き
	///	関節位置の取得
	virtual double GetJointPosition(int i){ return position[i]; }

	virtual void Integrate(SGScene* scene);
	virtual void CompJointAxis();
	virtual void CompRelativePosition();
	virtual void CompRelativeVelocity();
	virtual void CompCoriolisAccel();
	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;
};

}
#endif
