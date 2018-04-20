/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef TQUATERNION_H
#define TQUATERNION_H

#include <Base/TinyVec.h>
#include <Base/TinyMat.h>
#include <Base/Affine.h>

namespace Spr{;

/**	\addtogroup gpLinearAlgebra	*/
//@{
/**	@file TQuaternion.h
	クォータニオンクラスの定義
 */

/**	Quaternion/4元数．3x3行列の代わりに回転を表すために使える．
	4変数で済む．補間や微分・積分がやりやすい．<br>
	TVec3との掛け算は，回転変換でオーバーロードされている．
*/
template<class ET>
class TQuaternion:public PTM::TVectorBase<4, TVecDesc<TQuaternion<ET>, ET> >{
public:
	typedef TVecDesc<TQuaternion<ET>, ET> desc;
	typedef PTM::TVectorBase<4, desc> base_type;
	///	継承されない基本的なメンバの定義.	@see ::DEF_TVECTOR_BASIC_MEMBER
	DEF_TVECTOR_BASIC_MEMBER(TQuaternion);
	union{
		ET data[4];
		struct{
			ET w,x,y,z;
		};
	};
	///	要素のアクセス
	ET& item_impl(size_t i){ return data[i]; }
	///	要素のアクセス
	const ET& item_impl(size_t i) const { return data[i]; }
	///	ストライド
	size_t stride_impl() const { return 1; }

	///	3次元の部分ベクトル
	typedef PTM::TSubVector<3, desc> vector_type;
	///@name 変数アクセス
	//@{
	///	w成分
	const element_type& W() const { return w; }
	///	x成分
	const element_type& X() const { return x; }
	///	y成分
	const element_type& Y() const { return y; }
	///	z成分
	const element_type& Z() const { return z; }
	///
	const vector_type& V() const {return this->sub_vector(1,vector_type());}

	///	z成分
	element_type& W(){ return w; }
	///	x成分
	element_type& X(){ return x; }
	///	y成分
	element_type& Y(){ return y; }
	///	z成分
	element_type& Z(){ return z; }
	/// 
	vector_type& V() {return this->sub_vector(1,vector_type());}

	/// 回転ベクトル．0..PIの範囲で回転ベクトルを返す．
	TVec3<ET> RotationHalf() {
		TQuaternion<ET> tmp;
		if (W() < 0) tmp = -*this;
		else tmp = *this;
		TVec3<ET> r;
		if (tmp.W() > 1) tmp.W() = 1;
		ET theta = (ET)( acos(tmp.W()) * 2 );
		r = tmp.sub_vector(1, vector_type());
		ET len = r.norm();
		if (len > 1e-20){
			r = r/len;
		}
		r *= theta;
		return r;
	}
	///	回転ベクトル2． 0..2PIの範囲で回転ベクトルを返す．	angle から関数名変更
	TVec3<ET> Rotation(){
		//	W() = cos(theta/2) なので
		TVec3<ET> r;
		if (W() < -1) W() = -1;
		if (W() > 1) W() = 1;
		ET theta = (ET)( acos(W()) * 2 );
		r = this->sub_vector(1, vector_type());
		ET len = r.norm();
		if (len > 1e-20){
			r = r/len;
		}
		r *= theta;
		return r;
	}

	/// 回転軸
	TVec3<ET> Axis()const{
		TVec3<ET> r;
#ifdef	_MSC_VER
		r = sub_vector(1, vector_type());
#else
		r = this->sub_vector(1, vector_type());
#endif
		ET len = r.norm();
		if (len > 1e-20){
			r = r/len;
		}
		else{	
			// rのノルムが0ということは回転角度が0ということなので回転軸は任意．
			// 0ベクトルを返すわけにはいかないので[1 0 0]を返す
			return TVec3<ET>(1, 0, 0);
		}
		return r;
	}

	/// 回転角度 (angleに関数名を変更する予定)
	ET Theta()const{
		/// 数値誤差でWが1.0を上回るとエラーする場合がある
		double w = W();
		w = std::min(w,  1.0);
		w = std::max(w, -1.0);
		return (ET)( acos(w) * 2 );
	}
	//@}

	///@name 初期化・構築
	//@{
	///	コンストラクタ
	TQuaternion(element_type wi, element_type xi, element_type yi, element_type zi){ W() = wi; X() = xi; Y() = yi; Z() = zi;}
	template <class B>
	void InitDirect(element_type a, const PTM::TVectorBase<3, B> v){
		W() = a; V() = v;
	}
	template <class B>
	void InitDirect(element_type a, const PTM::TVectorBase<4, B> v){
		W() = v[0]; X() = v[1]; Y() = v[2]; Z() = v[3];
	}
	static TQuaternion<ET> Rot(element_type angle, const TVec3<element_type>& axis){
		TQuaternion<ET> quat;
		PTM::init_quaternion(quat, angle, axis);
		return quat;
	}
	static TQuaternion<ET> Rot(element_type angle, char axis){
		TQuaternion<ET> quat;
		PTM::init_quaternion(quat, angle, axis);
		return quat;
	}
	static TQuaternion<ET> Rot(const TVec3<element_type>& rot){
		TQuaternion<ET> quat;
		PTM::init_quaternion(quat, rot);
		return quat;
	}

	//@}
	///共役
	void Conjugate() { V() = -V(); }
	///	
	TQuaternion Conjugated() const { TQuaternion rv(*this); rv.Conjugate(); return rv;}
	///逆
	TQuaternion Inv() const { return Conjugated() / this->square(); }

	///回転行列変換
	template<class AM> void FromMatrix(const AM& m)
	{
		ET tr = m[0][0] + m[1][1] + m[2][2] + (ET)1;
		if (tr > (0.1f)){
			ET s = ET( 0.5/sqrt(tr) );
			W() = ET( 0.25 / s );
			X() = ET( (m[2][1] - m[1][2]) * s );
			Y() = ET( (m[0][2] - m[2][0]) * s );
			Z() = ET( (m[1][0] - m[0][1]) * s );
		}else if (m[0][0] > m[1][1] && m[0][0] > m[2][2]) { 
			ET s = ET(sqrt( 1.0 + m[0][0] - m[1][1] - m[2][2] ) * 2);
			X() = ET(0.25) * s;
			Y() = (m[0][1] + m[1][0]) / s; 
			Z() = (m[0][2] + m[2][0]) / s; 
			W() = (m[1][2] - m[2][1]) / s;
		} else if (m[1][1] > m[2][2]) {
			ET s = ET( sqrt(1.0 + m[1][1] - m[0][0] - m[2][2] ) * 2);
			X() = (m[0][1] + m[1][0] ) / s;
			Y() = ET(0.25) * s;
			Z() = (m[1][2] + m[2][1] ) / s; 
			W() = (m[0][2] - m[2][0] ) / s;
		} else { 
			ET s = ET( sqrt( 1.0 + m[2][2] - m[0][0] - m[1][1] ) * 2); 
			X() = (m[0][2] + m[2][0] ) / s;
			Y() = (m[1][2] + m[2][1] ) / s;
			Z() = ET(0.25) * s;
			W() = (m[1][0] - m[0][1] ) / s;
		}
		this->unitize();
	}
	template<class AM> void ToMatrix(AM& mat) const
	{
		typedef TYPENAME AM::element_type AMET;
		mat[0][0] = AMET( 1.0 - 2.0 * (Y()*Y() + Z()*Z()) );
		mat[1][0] = AMET( 2.0 * (X()*Y() + Z()*W()) );
		mat[2][0] = AMET( 2.0 * (X()*Z() - Y()*W()) );
		mat[0][1] = AMET( 2.0 * (Y()*X() - Z()*W()) );
		mat[1][1] = AMET( 1.0 - 2.0 * (X()*X() + Z()*Z()) );
		mat[2][1] = AMET( 2.0 * (Y()*Z() + X()*W()) );
		mat[0][2] = AMET( 2.0 * (X()*Z() + Y()*W()) );
		mat[1][2] = AMET( 2.0 * (Y()*Z() - X()*W()) );
		mat[2][2] = AMET( 1.0 - 2.0 * (X()*X() + Y()*Y()) );
	}
	///	オイラー角(Y軸(heading), Z軸(attitude), X軸(bank)の順のオイラー角)へ変換
	/*	オイラー角は関節の順番によっていろいろな定義がある。
		ここでは、Y軸→Z軸→X軸の順を考えている。
		heading, attitude, bankの呼び名は、車が右を向いている場合を考えると、
		納得いくと思います。
	*/
	template <class VET> void ToEuler(TVec3<VET>& v)const{
		ET poleCheck = X()*Y() + Z()*W();
		VET& heading = v[0];
		VET& attitude = v[1];
		VET& bank = v[2];
		if (poleCheck > 0.499){				//	north pole
			heading = 2 * atan2(X(),W());
			bank = 0;
		}else if(poleCheck < -0.499){		//	south pole
			heading = -2 * atan2(X(),W());
			bank = 0;
		}else{
			heading = atan2(2*Y()*W()-2*X()*Z() , 1 - 2*Y()*Y() - 2*Z()*Z());
			attitude = asin(2*X()*Y() + 2*Z()*W());
			bank = atan2(2*X()*W()-2*Y()*Z() , 1 - 2*X()*X() - 2*Z()*Z());
		}
	}
	///	オイラー角(Y軸(heading), Z軸(attitude), X軸(bank)の順のオイラー角)から変換
	template <class VET> void FromEuler(const TVec3<VET>& v){
		VET heading = v[0];
		VET attitude = v[1];
		VET bank = v[2];

		ET c1 = cos(heading / 2);
		ET c2 = cos(attitude / 2);
		ET c3 = cos(bank / 2);
		ET s1 = sin(heading / 2);
		ET s2 = sin(attitude / 2);
		ET s3 = sin(bank / 2);
		
		W() = c1*c2*c3 - s1*s2*s3;
		X() = s1*s2*c3 + c1*c2*s3;
		Y() = s1*c2*c3 + c1*s2*s3;
		Z() = c1*s2*c3 - s1*c2*s3;
	}
	///lhsを回転してrhsに一致させるクウォータニオン
	void RotationArc(const TVec3<ET>& lhs, const TVec3<ET>& rhs){
		TVec3<ET> v0, v1, c;
		ET d, s;
		v0 = lhs.unit();
		v1 = rhs.unit();
		c = PTM::cross(v0, v1);
		d = PTM::dot(v0, v1);
		if(d==-1){
			// lhsとrhsが正反対の向きのときにゼロ割りを起こしていたのでikkoが勝手に書き加えた (07/08/27)
			W() = 0;
			V() = PTM::cross(v0, (v0==TVec3<ET>(1,0,0) ? TVec3<ET>(0,1,0) : TVec3<ET>(1,0,0))).unit();
		}
		else{
			s = sqrt(((ET)1.0 + d) * (ET)2.0);
			W() = s / (ET)2.0;
			V() = c / s;
		}
	}

	///オイラー角で指定 (FromEulerとまったく同じ)
	void Euler(ET heading, ET attitude, ET bank) {
		ET c1 = cos(heading / 2);
		ET s1 = sin(heading / 2);
		ET c2 = cos(attitude / 2);
		ET s2 = sin(attitude / 2);
		ET c3 = cos(bank / 2);
		ET s3 = sin(bank / 2);
		
		W() = c1*c2*c3 - s1*s2*s3;
		X() = s1*s2*c3 + c1*c2*s3;
		Y() = s1*c2*c3 + c1*s2*s3;
		Z() = c1*s2*c3 - s1*c2*s3;
	}

	TVec3<ET> ToEuler() const{
		ET poleCheck = X()*Y() + Z()*W();
		ET heading;
		ET attitude;
		ET bank;
		if (poleCheck > 0.499) {				//	north pole
			heading = 2 * atan2(X(), W());
			attitude = 0;
			bank = 0;
		}
		else if (poleCheck < -0.499) {		//	south pole
			heading = -2 * atan2(X(), W());
			attitude = 0;
			bank = 0;
		}
		else {
			heading = atan2(2 * Y()*W() - 2 * X()*Z(), 1 - 2 * Y()*Y() - 2 * Z()*Z());
			attitude = asin(2 * X()*Y() + 2 * Z()*W());
			bank = atan2(2 * X()*W() - 2 * Y()*Z(), 1 - 2 * X()*X() - 2 * Z()*Z());
		}
		return TVec3<ET>(heading, attitude, bank);
	}

	/** @brief 角速度からquaternionの時間微分を計算
		@param w 角速度

		このquaternionと角速度wから，quaternionの時間微分を計算する．
		ただしwは回転元（ワールド）座標系から見た回転先（ローカル）座標系の角速度を
		回転元座標系で表わしたもの．
	 */
	TQuaternion<ET> Derivative(const TVec3<ET>& w)const{
		return 0.5 * TQuaternion<ET>(0.0, w.X(), w.Y(), w.Z()) * *this;
	}

	/** @brief クウォータニオンの時間微分から角速度を計算
		@param qd quaternionの時間微分
		@return 角速度

		このquaternionとその時間微分qdから角速度wを計算する．
		ただしwは回転元（ワールド）座標系から見た回転先（ローカル）座標系の角速度を
		回転元座標系で表わしたもの．
	 */
	TVec3<ET> AngularVelocity(const TQuaternion<ET>& qd)const{
		return 2.0 * (qd * Conjugated()).V();
	}

protected:
	///	コンストラクタ
	void set_default(){ W() = 1; X() = 0; Y() = 0; Z() = 0;}
};

///	TQuaternion 同士の掛け算．回転変換としては，合成になる．
template <class A, class B>
TQuaternion<A> operator*(const TQuaternion<A>& q1, const TQuaternion<B>& q2){
    TQuaternion<A> rv;
	rv.W() = q1.W() * q2.W() - q1.X() * q2.X() - q1.Y() * q2.Y() - q1.Z() * q2.Z();
	rv.X() = q1.W() * q2.X() + q1.X() * q2.W() + q1.Y() * q2.Z() - q1.Z() * q2.Y();
	rv.Y() = q1.W() * q2.Y() + q1.Y() * q2.W() + q1.Z() * q2.X() - q1.X() * q2.Z();
	rv.Z() = q1.W() * q2.Z() + q1.Z() * q2.W() + q1.X() * q2.Y() - q1.Y() * q2.X();
	return rv;
}

///	TQuaternionでベクトルを回転． Quaternion * vector * Quaternion^{*} と同じ．
template <class ET, class BD>
inline TYPENAME BD::ret_type operator*(const TQuaternion<ET>& q, const PTM::TVectorBase<3, BD>& v){
	// ↓第1成分は0ではないか？？　tazz
	//TQuaternion<ET> qv(1, ET(v[0]), ET(v[1]), ET(v[2]));
	//TYPENAME BD::ret_type r = (q * qv * q.Conjugated()).sub_vector(PTM::TSubVectorDim<1,3>());
	// ↓は第1成分を0として展開したコード
	TYPENAME BD::ret_type tmp = q.V() % v;
	TYPENAME BD::ret_type r = (TYPENAME BD::element_type)(q.W()*q.W())*v
                                + (TYPENAME BD::element_type)(2*q.W())*tmp
                                + (TYPENAME BD::element_type)(q.V()*v)*q.V() + q.V()%tmp;
	return r;
}

///	TQuaternionで行列を回転． TQuaternion * (ex,ey,ez) * TQuaternion.conjugated() と同じ．
template <class ET, class BD>
inline TYPENAME BD::ret_type operator*(const TQuaternion<ET>& q, const PTM::TMatrixBase<3, 3, BD>& m){
	TYPENAME BD::ret_type r;
	for(int i=0; i<3; ++i){
		// 上と同じく0と思われる
		TQuaternion<ET> qv(0, ET(m.col(i)[0]), ET(m.col(i)[1]), ET(m.col(i)[2]));
		r.col(i) = (q * qv * q.Conjugated()).sub_vector(PTM::TSubVectorDim<1,3>());
	}
	return r;
}

///	TQuaternion の内積．
template <class T1, class T2>
inline T1 dot(const TQuaternion<T1>& q1, const TQuaternion<T2>& q2) {
	return q1.X() * q2.X() + q1.Y() * q2.Y() + q1.Z() * q2.Z() + q1.W() * q2.W();
}

/// 回転操作をせず，普通にQuaternionとベクトルの積を求める
template<class ET, class BD>
inline TQuaternion<ET> mult(const TQuaternion<ET>& q, const PTM::TVectorBase<3, BD>& v){
	// 普通の計算は回転操作とは違うので，ベクトルをクォータニオンに直す時にw成分を0にしないと演算がおかしくなる
	TQuaternion<ET> qv(0, ET(v[0]), ET(v[1]), ET (v[2]));
	return q*qv;
}

/// 回転操作をせず，普通にベクトルとQuaternionの積を求める
template<class BD, class ET>
inline TQuaternion<ET> mult(const PTM::TVectorBase<3, BD>& v, const TQuaternion<ET>& q){
	TQuaternion<ET> qv(0, ET(v[0]), ET(v[1]), ET (v[2]));
	return qv*q;
}

template <class T1, class T2>
TQuaternion<T1> interpolate(T1 t, const TQuaternion<T1>& q1, const TQuaternion<T2>& q2){
	TQuaternion<T1> ret;
	TQuaternion<T1> q3 ;
    float      dot;

    dot = q1.X() * q2.X() + q1.Y() * q2.Y() + q1.Z() * q2.Z() + q1.W() * q2.W();
    
    // dot < 0.0fの時、反転する
    if (dot < 0.0f){
        dot = -dot;
        q3 = TQuaternion<T1>(-q2.W(), -q2.X(), -q2.Y(), -q2.Z());
    }
    else q3 = q2;
    
    if (dot >  1.0f) dot = 1.0f;
	if (dot < -1.0f) dot = -1.0f;
	
	float angle = acos(dot);
	float sina, sinat, sinaomt;
	
	sina    = sin(angle          );
	sinat   = sin(angle *      t );
	sinaomt = sin(angle * (1 - t));
	
	if (sina){
		ret.X() = (q1.X() * sinaomt + q3.X() * sinat) / sina;
		ret.Y() = (q1.Y() * sinaomt + q3.Y() * sinat) / sina;
		ret.Z() = (q1.Z() * sinaomt + q3.Z() * sinat) / sina;
		ret.W() = (q1.W() * sinaomt + q3.W() * sinat) / sina;
	}else{
		ret = q1;
	}
	return ret;
}

///	float版TQuaternion.
typedef TQuaternion<float> Quaternionf;
///	double版TQuaternion.
typedef TQuaternion<double> Quaterniond;


/**	姿勢．位置と向きを表すクラス．
	TQuaternion と TVec3 で向きと位置を表す．
	TAffineの代わりに使える．
*/
template<class ET>
class TPose:public PTM::TVectorBase<7, TVecDesc<TPose<ET>, ET> >{
public:
	typedef TVecDesc<TPose<ET>, ET> desc;
	typedef PTM::TVectorBase<4, desc> base_type;
	///	継承されない基本的なメンバの定義.	@see ::DEF_TVECTOR_BASIC_MEMBER
	DEF_TVECTOR_BASIC_MEMBER(TPose);
	union{
		ET data[7];
		struct{
			ET w,x,y,z;		//< 回転
			ET px, py, pz;	//< 位置
		};
	};

	///	要素のアクセス
	ET& item_impl(size_t i){ return data[i]; }
	///	要素のアクセス
	const ET& item_impl(size_t i) const { return data[i]; }
	///	ストライド
	size_t stride_impl() const { return 1; }

	///	要素のアクセス
	TVec3<ET>& Pos() { return *(TVec3<ET>*)(data+4); }
	const TVec3<ET>& Pos() const { return *(TVec3<ET>*)(data+4); }
	TQuaternion<ET>& Ori() { return *(TQuaternion<ET>*)(void*)(data); }
	const TQuaternion<ET>& Ori() const { return *(TQuaternion<ET>*)(void*)(data); }
	ET& W(){return w;}
	const ET& W() const{return w;}
	ET& X(){return x;}
	const ET& X() const{return x;}
	ET& Y(){return y;}
	const ET& Y() const{return y;}
	ET& Z(){return z;}
	const ET& Z() const{return z;}
	ET& Px(){return px;}
	const ET& Px() const{return px;}
	ET& Py(){return py;}
	const ET& Py() const{return py;}
	ET& Pz(){return pz;}
	const ET& Pz() const{return pz;}

	/**@name	要素へのアクセス	*/
	//@{
	/// 
	ET& PosX() {return px;}
	const ET& PosX() const {return px;}
	/// 
	ET& PosY() {return py;}
	const ET& PosY() const {return py;}
	/// 
	ET& PosZ() {return pz;}
	const ET& PosZ() const {return pz;}
	/// 
	ET& OriX() {return x;}
	const ET& OriX() const {return x;}
	/// 
	ET& OriY() {return y;}
	const ET& OriY() const {return y;}
	/// 
	ET& OriZ() {return z;}
	const ET& OriZ() const {return z;}
	/// 
	ET& OriW() {return w;}
	const ET& OriW() const {return w;}
	
	TPose<ET> Inv() const { 
		TPose<ET> rv;
		rv.Ori() = Ori().Inv();
		rv.Pos() = -(rv.Ori()*Pos());
		return rv;
	}

	/**@name	初期化と構築	*/
	TPose(const TVec3<ET>& p, const TQuaternion<ET>& q = TQuaternion<ET>()){
		Pos() = p;
		Ori() = q;
	}
	///	単位行列
	static TPose<ET> Unit(){
		TPose<ET> y;
		//PTM::init_unitize(y);
		y.Pos() = TVec3<ET>();
		y.Ori() = TQuaternion<ET>();
		return y;
	}

	///	平行移動
	static TPose<ET> Trn(element_type px, element_type py, element_type pz){
		return Trn(TVec3<ET>(px, py, pz));
	}
	static TPose<ET> Trn(const TVec3<ET> &v){
		TPose<ET> y;
		y.Pos() = v;
		y.Ori() = TQuaternion<ET>();
		return y;
	}

	/// 回転
	static TPose<ET> Rot(ET wi, ET xi, ET yi, ET zi){ 
		TPose<ET> y;
		y.Pos() = TVec3<ET>();
		y.Ori() = TQuaternion<ET>(wi, xi, yi, zi);
		return y;
	}
	static TPose<ET> Rot(ET angle, const TVec3<element_type>& axis){ 
		TPose<ET> y;
		y.Pos() = TVec3<ET>();
		y.Ori() = TQuaternion<ET>::Rot(angle, axis);
		return y;
	}
	static TPose<ET> Rot(ET angle, char axis){
		TPose<ET> y;
		y.Pos() = TVec3<ET>();
		y.Ori() = TQuaternion<ET>::Rot(angle, axis);
		return y;
	}
	static TPose<ET> Rot(const TVec3<ET>& rot){
		TPose<ET> y;
		y.Pos() = TVec3<ET>();
		y.Ori() = TQuaternion<ET>::Rot(rot);
		return y;
	}
	static TPose<ET> Rot(const TQuaternion<ET> &q){
		TPose<ET> y;
		y.Pos() = TVec3<ET>();
		y.Ori() = q;
		return y;
	}

	/// Affine変換の行列から読み出します．
	template <class AT>
	void FromAffine(const TAffine<AT>& f){
		/// Affine行列の位置ベクトルにposeの位置をコピーする
		Pos() = f.Trn();
		/// Oriからできる行列をAffine行列の対応する場所に上書き
		Ori().FromMatrix(f.Rot());
	}

	/// Affine変換の行列に変換し返す
	template <class B>
	void ToAffine(TAffine<B>& af) const {
		/// Affine行列の位置ベクトルにposeの位置をコピーする
		af.Trn() = Pos();
		/// Oriからできる行列をAffine行列の対応する場所に上書き
		Ori().ToMatrix(af.Rot());
	}

	operator TAffine<ET>(){ 
		TAffine<ET> rv;
		ToAffine(rv);
		return rv;
	}

	///@name 初期化・構築
	//@{
	///	コンストラクタ
	template <class TW,class TX, class TY, class TZ,class TPX, class TPY, class TPZ> TPose (TW wi,TX xi, TY yi, TZ zi,TPX pxi, TPY pyi, TPZ pzi){
		W() = element_type(wi);
		X() = element_type(xi);
		Y() = element_type(yi);
		Z() = element_type(zi);
		Px() = element_type(pxi);
		Py() = element_type(pyi);
		Pz() = element_type(pzi);
	}
	template <class AT> TPose (const TAffine<AT>& af){
		FromAffine(af);
	}
	//@}

protected:
	///	コンストラクタ
	void set_default(){ 
		W() = 1; X() = 0; Y() = 0; Z() = 0;
		Pos().clear();
	}
};
template <class EP, class EV>
TVec3<EV> operator * (const TPose<EP>& p, const TVec3<EV>& v){
	return p.Ori()*v + p.Pos();
}

template <class EA, class EB>
TPose<EA> operator * (const TPose<EA>& a, const TPose<EB>& b){
	TPose<EA> rv;
	rv.Pos() = a.Pos() + a.Ori() * b.Pos();
	rv.Ori() = a.Ori() * b.Ori();
	return rv;
}

/// Poseの補間
template <class EA, class EB>
TPose<EA> interpolate(EA t, const TPose<EA>& p0, const TPose<EB>& p1){
	TQuaternion<EA> quat = interpolate(t, p0.Ori(), p1.Ori());
	TVec3<EA> pos = interpolate(t, p0.Pos(), p1.Pos());
	TPose<EA> pose;
	pose.Pos() = pos;
	pose.Ori() = quat;
	return pose;
}

///	float版TPose.
typedef TPose<float> Posef;
///	double版TPose.
typedef TPose<double> Posed;

//@}

//----------------------------------------------------------------------------
// SwingTwist
/** 回転のスイング・ツイスト角表現 */
struct SwingTwist : public Vec3d{
	double& SwingDir(){return item(0);}
	double& Swing(){return item(1);}
	double& Twist(){return item(2);}
	
	void ToQuaternion(Quaterniond& q){
		// tazzさんのメモの(11)式、軸[cos(psi), sin(psi), 0]^Tまわりにthetaだけ回転した後Z軸まわりにphi回転するquaternion
		double psi = SwingDir(), the = Swing(), phi = Twist();
		double cos_the = cos(the / 2), sin_the = sin(the / 2);
		double cos_phi = cos(phi / 2), sin_phi = sin(phi / 2);
		double cos_psiphi = cos(psi - phi / 2), sin_psiphi = sin(psi - phi / 2);
		q.w = cos_the * cos_phi;
		q.x = sin_the * cos_psiphi;
		q.y = sin_the * sin_psiphi;
		q.z = cos_the * sin_phi;
	}
	
	void FromQuaternion(const Quaterniond& q){
		// tazzさんのメモの(12)式、item[0]:psi, item[1]:theta, item[2]:phi
		item(0) = atan2(q.w * q.y + q.x * q.z, q.w * q.x - q.y * q.z);
		item(1) = 2 * atan2(sqrt(q.x * q.x + q.y * q.y), sqrt(q.w * q.w + q.z * q.z));
		item(2) = 2 * atan2(q.z, q.w);
	}
	
	void Jacobian(Matrix3d& J){
		// tazzさんのメモの(13)式、st=[psi, theta, phi]^Tの時間微分から角速度ωを与えるヤコビアンJ (ω = J * (d/dt)st)
		double psi = SwingDir();
		double the = std::max(Rad(1.0), Swing());	// スイング角0でランク落ちするので無理やり回避
		double cos_psi = cos(psi), sin_psi = sin(psi);
		double cos_the = cos(the), sin_the = sin(the);
		J[0][0] = -sin_the * sin_psi;
		J[0][1] =  cos_psi;
		J[0][2] =  sin_the * sin_psi;
		J[1][0] =  sin_the * cos_psi;
		J[1][1] =  sin_psi;
		J[1][2] = -sin_the * cos_psi;
		J[2][0] =  1.0 - cos_the;
		J[2][1] =  0.0;
		J[2][2] =  cos_the;
	}
	
	void JacobianInverse(Matrix3d& J, const Quaterniond& q){
		// tazzさんのメモの(14)式、角速度ωからst=[psi, theta, phi]^Tの時間微分を求めるヤコビアンJInv ((d/dt)st = JInv * ω)
		const double eps = 1.0e-2;									// (14)式の分母が0になることを防ぐためのeps。小さくし過ぎると（1.0e-12とかすると）0付近で横にしたときに物体が外れてしまう
		double w2z2 = std::max(eps, q.w * q.w + q.z * q.z);
		double w2z2inv = 1.0 / w2z2;
		double x2y2 = std::max(eps, 1.0 - w2z2);
		double x2y2inv = 1.0 / x2y2;
		double tmp = sqrt(w2z2inv * x2y2inv);
		double wy_xz = q.w * q.y + q.x * q.z;
		double wx_yz = q.w * q.x - q.y * q.z;
		J[0][0] =  0.5 * (w2z2inv - x2y2inv) * wy_xz;
		J[0][1] = -0.5 * (w2z2inv - x2y2inv) * wx_yz;
		J[0][2] =  1.0;
		J[1][0] =  tmp * wx_yz;
		J[1][1] =  tmp * wy_xz;
		J[1][2] =  0.0;
		J[2][0] =  w2z2inv * wy_xz;
		J[2][1] = -w2z2inv * wx_yz;
		J[2][2] =  1.0;
	}
	
	void Coriolis(Vec3d& c, const Vec3d& sd){
		double cos_psi = cos(SwingDir()), sin_psi = sin(SwingDir());
		double cos_the = cos(Swing()), sin_the = sin(Swing());
		double tmp1 = sd[1] * (sd[0] - sd[2]);
		double tmp2 = sd[0] * (sd[0] - sd[2]);
		double tmp3 = sd[0] * sd[1];
		c[0] = -cos_the * sin_psi * tmp1 - sin_the * cos_psi * tmp2 - sin_psi * tmp3;
		c[1] =  cos_the * cos_psi * tmp1 - sin_the * sin_psi * tmp2 + cos_psi * tmp3;
		c[2] =  sin_the * tmp1;	
	}
};

}
#endif
