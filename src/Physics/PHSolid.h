#ifndef PHSOLID_H
#define PHSOLID_H

#include <Springhead.h>
#include <Foundation/Object.h>
#include <Physics/PHScene.h>
#include <Physics/PHEngine.h>

namespace Spr{;

enum PHIntegrationMode{
		PHINT_NONE,			//積分しない
		PHINT_ARISTOTELIAN,	//f = mv
		PHINT_EULER,		//オイラー法
		PHINT_SIMPLETIC,
		PHINT_ANALYTIC,
		PHINT_RUNGEKUTTA2,	//２次ルンゲクッタ法
		PHINT_RUNGEKUTTA4	//４次ルンゲクッタ法
};

///	剛体
class PHSolid : public Object, public PHSolidIf{
	Vec3d	_angvel[4];			///<	数値積分係数
	Vec3d	_angacc[4];
protected:
	double		mass;			///<	質量
	Matrix3d	inertia;		///<	慣性テンソル
	Matrix3d	inertia_inv;
	Vec3d		force;			///<	力				(World)
	Vec3d		torque;			///<	トルク			(World)
	Vec3d		velocity;		///<	速度			(World)
	Vec3d		angVelocity;	///<	角速度			(World)
	Vec3d		center;			///<	質量中心の位置	(Local..frameのposture系)
	Vec3d		pos;			///<	位置			(World)
	Quaterniond quat;			///<	向き			(World)

	///	積分方式
	PHIntegrationMode integrationMode;

	//オイラーの運動方程式
	//慣性行列は対角行列を前提．
	Vec3d	Euler(const Matrix3d& I, const Vec3d& t, const Vec3d& w){
		return Vec3d(
			(t[0] - (I[2][2] - I[1][1]) * w.Y() * w.Z()) / I[0][0],
			(t[1] - (I[0][0] - I[2][2]) * w.Z() * w.X()) / I[1][1],
			(t[2] - (I[1][1] - I[0][0]) * w.X() * w.Y()) / I[2][2]);
	}
public:
	OBJECTDEF(PHSolid);
	BASEIMP_OBJECT(Object);
	void Print(std::ostream& os)const{Object::Print(os);}
	PHSolid();											///< 構築
	
	bool		AddChildObject(Object* o, PHScene* s);///< ロード時に使用．
	size_t		NReferenceObjects();					///< 1
	Object*		ReferenceObject(size_t i);				///< フレームを返す．
	void		Step();									///< 時刻を進める．
	
	void		AddForce(Vec3d f);						///< 力を質量中心に加える
	void		AddTorque(Vec3d t){ torque += t; }		///< トルクを加える
	void		AddForce(Vec3d f, Vec3d r);				///< 力を 位置r(World系) に加える
	//混乱するしGravityEngineしか使ってなかったので廃棄候補
	//void		AddForceLocal(Vec3d f, Vec3d r);		///< 力を 位置r(Local系) に加える
	void		ClearForce();							///< 力とトルクをクリア
	Vec3d		GetForce() const {return force;}		///< 加えられた力
	Vec3d		GetTorque() const {return torque;}		///< 加えられたトルク
	void		SetForce(Vec3d f){force = f;}			///< 力を設定する
	void		SetTorque(Vec3d t){torque = t;}			///< トルクをセットする

	double		GetMass(){return mass;}					///< 質量
	double		GetMassInv(){return 1.0 / mass;}		///< 質量の逆数
	void		SetMass(double m){mass = m;}			///< 質量の設定
	void		SetMassInv(double minv){mass = 1.0 / minv;}
														///< 質量の逆数の設定
	Matrix3d	GetInertia(){return inertia;}			///< 慣性テンソル
	Matrix3d	GetInertiaInv(){return inertia_inv;}	///< 慣性テンソルの逆数
	void		SetInertia(const Matrix3d& I){			///< 慣性テンソルの設定
		inertia = I;
		inertia_inv = I.inv();
	}
	void		SetInertiaInv(const Matrix3d& Iinv){	///< 慣性テンソルを逆数で設定
		inertia_inv = Iinv;
		//inertia = Iinv.inv();
	}

	///	積分方式の取得
	PHIntegrationMode GetIntegrationMode(){ return integrationMode; }
	///	積分方式の設定
	void SetIntegrationMode(PHIntegrationMode m){ integrationMode=m; }

	Vec3d		GetFramePosition() const {return pos;}
	void		SetFramePosition(const Vec3d& p){pos = p;}
	Vec3d		GetCenterPosition() const {return quat*center + pos;}
														///< 重心位置の取得
	void		SetCenterPosition(const Vec3d& p){		///< 重心位置の設定
		pos = p - quat*center;
	}

	///	向きの取得
	Matrix3d	GetRotation() const { Matrix3d rv; quat.to_matrix(rv); return rv; }
	///	向きの設定
	void		SetRotation(const Matrix3d& r){
		quat.from_matrix(r);
	}

	///	向きの取得
	Quaterniond GetOrientation() const {return quat;}
	///	向きの設定
	void		SetOrientation(const Quaterniond& q){
		quat = q;
		Matrix3f m;
		quat.to_matrix(m);
	}

	///	質量中心の速度の取得
	Vec3d		GetVelocity() const {return velocity;}
	///	質量中心の速度の設定
	void		SetVelocity(const Vec3d& v){velocity = v;}

	///	角速度の取得
	Vec3d		GetAngularVelocity() const {return angVelocity;}
	///	角速度の設定
	void		SetAngularVelocity(const Vec3d& av){angVelocity = av;}

	///	ローカルフレームから見た，剛体の質量中心位置の設定
	Vec3d		GetCenter() const {return center;}
	///	ローカルフレームから見た，剛体の質量中心位置の取得
	void		SetCenter(const Vec3d& c){center = c;}		

};

class PHSolids:public std::vector< UTRef<PHSolid> >{
public:
	UTRef<PHSolid> Erase(const UTRef<PHSolid>& s){
		iterator it = std::find(begin(), end(), s);
		if (it == end()) return NULL;
		UTRef<PHSolid> rv = *it;
		erase(it);
		return *it;
	}
	UTRef<PHSolid>* Find(const UTRef<PHSolid>& s){
		iterator it = std::find(begin(), end(), s);
		if (it == end()) return NULL;
		else return &*it;
	}
	UTRef<PHSolid>* Find(const UTRef<PHSolid>& s) const {
		return ((PHSolids*)this)->Find(s);
	}
};

///	Solidの積分を行うクラスのベース
class PHSolverBase:public PHEngine{
	OBJECTDEFABST(PHSolverBase);
public:
	virtual void ClearForce()=0;
};

/**	Solidを保持するクラス．Solidの更新も行う．	*/
class PHSolidContainer:public PHSolverBase{
	OBJECTDEF(PHSolidContainer);
public:
	PHSolids solids;
	bool AddChildObject(Object* o, PHScene* s);
	bool DelChildObject(Object* o, PHScene* s);
	///
	int GetPriority() const {return SGBP_SOLIDCONTAINER;}
	///	速度→位置、加速度→速度の積分
	virtual void Step();
	///	剛体にかかった力のクリア
	virtual void ClearForce();
	
	virtual void Clear(PHScene* s){ solids.clear(); }
	///	所有しているsolidの数
	virtual size_t NChildObjects(){ return solids.size(); }
	///	所有しているsolid
	virtual Object* ChildObject(size_t i){ return solids[i]; }

};

/**	Solidの力をクリアするクラス	*/
class PHSolidClearForce:public PHEngine{
	OBJECTDEF(PHSolidClearForce);
public:
	typedef std::vector< UTRef<PHSolverBase> > PHSolvers;
	PHSolvers solvers;
	///	クリアする
	virtual void Step();
	virtual int GetPriority() const { return SGBP_CLEARFORCE; }
};

struct SolidInfo{
	float mass;				///<	質量
	Matrix3f inertia;		///<	慣性モーメント
	Vec3f velocity;			///<	速度
	Vec3f angularVelocity;	///<	角速度
	Vec3f center;			///<	中心位置
};

}	//	namespace Spr
#endif
