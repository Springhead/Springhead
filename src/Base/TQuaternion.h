#ifndef TQUATERNION_H
#define TQUATERNION_H

#include <Base/TinyVec.h>
#include <Base/TinyMat.h>

namespace Spr{;

/**	そのうち入れたい，オイラー角への変換．
heading = atan2(2*qy*qw-2*qx*qz , 1 - 2*qy2 - 2*qz2)
attitude = asin(2*qx*qy + 2*qz*qw) 
bank = atan2(2*qx*qw-2*qy*qz , 1 - 2*qx2 - 2*qz2)

except when qx*qy + qz*qw = 0.5 (north pole)
which gives:
heading = 2 * atan2(x,w)
bank = 0
and when qx*qy + qz*qw = -0.5 (south pole)
which gives:
heading = -2 * atan2(x,w)
bank = 0 
*/

//-----------------------------------------------------------------------------
//		TQuaternion
///		4元数クラス.
template<class ET>
class TQuaternion:public PTM::TVectorBase<DIMENC(4), TVecDesc<TQuaternion<ET>, ET> >{
public:
	typedef TVecDesc<TQuaternion<ET>, ET> desc;
	typedef PTM::TVectorBase<DIMENC(4), desc> base_type;
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
	const vector_type& V() const {return sub_vector(vector_type());}

	///	z成分
	element_type& W(){ return w; }
	///	x成分
	element_type& X(){ return x; }
	///	y成分
	element_type& Y(){ return y; }
	///	z成分
	element_type& Z(){ return z; }
	/// 
	vector_type& V() {return sub_vector(1,vector_type());}
	/// 回転ベクトル．0..PIの範囲で回転ベクトルを返す．
	TVec3<ET> rotation_half() {
		TQuaternion<ET> tmp;
		if (tmp.W() < 0) tmp = -*this;
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
	TVec3<ET> rotation() {
		//	W() = cos(theta/2) なので
		TVec3<ET> r;
		if (W() < -1) W() = -1;
		if (W() > 1) W() = 1;
		ET theta = (ET)( acos(W()) * 2 );
		r = sub_vector(1, vector_type());
		ET len = r.norm();
		if (len > 1e-20){
			r = r/len;
		}
		r *= theta;
		return r;
	}

	/// 回転軸
	TVec3<ET> axis(){
		TVec3<ET> r;
		r = sub_vector(1, vector_type());
		ET len = r.norm();
		if (len > 1e-20){
			r = r/len;
		}
		return r;
	}

	/// 回転角度 (angleに関数名を変更する予定)
	ET theta(){
		return (ET)( acos(W()) * 2 );
	}
	//@}

	///@name 初期化・構築
	//@{
	///	コンストラクタ
	void set_default(){ W() = 1; X() = 0; Y() = 0; Z() = 0;}

	///	コンストラクタ
	TQuaternion(element_type wi, element_type xi, element_type yi, element_type zi){ W() = wi; X() = xi; Y() = yi; Z() = zi;}
	template <class B>
	void InitDirect(element_type a, const PTM::TVectorBase<DIMENC(3), B> v){
		W() = a; V() = v;
	}
	template <class B>
	void InitDirect(element_type a, const PTM::TVectorBase<DIMENC(4), B> v){
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
	void conjugate() { V() = -V(); }
	///	
	TQuaternion conjugated() const { TQuaternion rv(*this); rv.conjugate(); return rv;}
	///逆
	TQuaternion inv() const { return conjugated() / square(); }

	///回転行列変換
	template<class AM> void from_matrix(const AM& m)
	{
		ET tr = m[0][0] + m[1][1] + m[2][2] + 1;
		if (tr > 1e-6f){
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
			W() = (m[0][1] - m[1][0] ) / s;
		}
		unitize();
	}
	template<class AM> void to_matrix(AM& mat) const
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
	///	オイラー角へ変換
	template <class ET> void to_eular(TVec3<ET>& v){
		ET poleCheck = X()*Y() + Z()*W();
		ET& heading = v[0];
		ET& attitude = v[1];
		ET& bank = v[2];
		if (poleCheck == 0.5){				//	north pole
			heading = 2 * atan2(X(),W());
			bank = 0;
		}else if(poleCheck == -0.5){		//	south pole
			heading = -2 * atan2(X(),W());
			bank = 0;
		}else{
			heading = atan2(2*Y()*W()-2*X()*Z() , 1 - 2*Y()2 - 2*Z()2);
			attitude = asin(2*X()*Y() + 2*Z()*W());
			bank = atan2(2*X()*W()-2*Y()*Z() , 1 - 2*X()2 - 2*Z()2);
		}
	}
	///
	template <class ET> void from_eular(const TVec3<ET>& v){
		ET& heading = v[0];
		ET& attitude = v[1];
		ET& bank = v[2];

		ET c1 = cos(heading / 2);
		ET c2 = cos(attitude / 2);
		ET c3 = cos(bank / 2);
		ET s1 = sin(heading / 2);
		ET s2 = sin(attitude / 2);
		ET s3 = sin(bank / 2);
		
		W() = c1 c2 c3 - s1 s2 s3;
		X() = s1 s2 c3 + c1 c2 s3;
		Y() = s1 c2 c3 + c1 s2 s3;
		Z() = c1 s2 c3 - s1 c2 s3;
	}
	///lhsを回転してrhsに一致させるクウォータニオン
	void rotation_arc(const TVec3<ET>& lhs, const TVec3<ET>& rhs)
	{
		TVec3<ET> v0, v1, c;
		ET d, s;
		v0 = lhs.unit();
		v1 = rhs.unit();
		c = PTM::cross(v0, v1);
		d = PTM::dot(v0, v1);
		s = sqrt((1.0 + d) * 2.0);
		W() = s / 2.0;
		V() = c / s;
	}

	///オイラー角で指定
	void euler(ET yaw, ET pitch, ET roll) {
		ET cosYaw	= cos(yaw / 2);
		ET sinYaw	= sin(yaw / 2);
		ET cosPitch	= cos(pitch / 2);
		ET sinPitch	= sin(pitch / 2);
		ET cosRoll	= cos(roll / 2);
		ET sinRoll	= sin(roll / 2);
		W() = cosRoll * cosPitch * cosYaw - sinRoll * sinPitch * sinYaw;
		X() = cosRoll * sinPitch * sinYaw + sinRoll * cosPitch * cosYaw;
		Y() = cosRoll * cosPitch * sinYaw + sinRoll * sinPitch * cosYaw;
		Z() = cosRoll * sinPitch * cosYaw - sinRoll * cosPitch * sinYaw;
	}

	//角速度からクウォータニオンの時間微分を計算
	//w : 角速度（ワールド）
	//q : クウォータニオン
	TQuaternion<ET> derivative(const TVec3<ET>& w){
		return 0.5 * TQuaternion<ET>(0.0, w.X(), w.Y(), w.Z()) * *this;
	}
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

///	TQuaternionでベクトルを回転． TQuaternion * vector * TQuaternion.conjugated() と同じ．
template <class ET, class BD>
inline TYPENAME BD::ret_type operator*(const TQuaternion<ET>& q, const PTM::TVectorBase<DIMENC(3), BD>& v){
	TQuaternion<ET> qv(1, ET(v[0]), ET(v[1]), ET(v[2]));
	TYPENAME BD::ret_type r = (q * qv * q.conjugated()).sub_vector(PTM::TSubVectorDim<1,3>());
	return r;
}
///	TQuaternion の内積．
template <class T1, class T2>
inline T1 dot(const TQuaternion<T1>& q1, const TQuaternion<T2>& q2) {
	return q1.X() * q2.X() + q1.Y() * q2.Y() + q1.Z() * q2.Z() + q1.W() * q2.W();
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

}

#endif
