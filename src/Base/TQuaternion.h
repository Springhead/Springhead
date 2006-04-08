#ifndef TQUATERNION_H
#define TQUATERNION_H

#include <Base/TinyVec.h>
#include <Base/TinyMat.h>
#include <Base/Affine.h>

namespace Spr{;

/**	\addtogroup gpLinearAlgebra ベクトル・行列・座標変換	*/
//@{

/**	Quaternion/4元数．3x3行列の代わりに回転を表すために使える．
	4変数で済む．補間や微分・積分がやりやすい．<br>
	TVec3との掛け算は，回転変換でオーバーロードされている．

	
	そのうち入れたい，オイラー角への変換．
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
	TVec3<ET> RotationHalf() {
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
	TVec3<ET> Rotation() {
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
	TVec3<ET> Axis(){
		TVec3<ET> r;
		r = sub_vector(1, vector_type());
		ET len = r.norm();
		if (len > 1e-20){
			r = r/len;
		}
		return r;
	}

	/// 回転角度 (angleに関数名を変更する予定)
	ET Theta(){
		return (ET)( acos(W()) * 2 );
	}
	//@}

	///@name 初期化・構築
	//@{
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
	void Conjugate() { V() = -V(); }
	///	
	TQuaternion Conjugated() const { TQuaternion rv(*this); rv.Conjugate(); return rv;}
	///逆
	TQuaternion Inv() const { return Conjugated() / this->square(); }

	///回転行列変換
	template<class AM> void FromMatrix(const AM& m)
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
	///	オイラー角へ変換
	template <class VET> void ToEular(TVec3<VET>& v){
		ET poleCheck = X()*Y() + Z()*W();
		VET& heading = v[0];
		VET& attitude = v[1];
		VET& bank = v[2];
		if (poleCheck == 0.5){				//	north pole
			heading = 2 * atan2(X(),W());
			bank = 0;
		}else if(poleCheck == -0.5){		//	south pole
			heading = -2 * atan2(X(),W());
			bank = 0;
		}else{
			heading = atan2(2*Y()*W()-2*X()*Z() , 1 - 2*Y()*Y() - 2*Z()*Z());
			attitude = asin(2*X()*Y() + 2*Z()*W());
			bank = atan2(2*X()*W()-2*Y()*Z() , 1 - 2*X()*X() - 2*Z()*Z());
		}
	}
	///
	template <class VET> void FromEular(const TVec3<VET>& v){
		VET& heading = v[0];
		VET& attitude = v[1];
		VET& bank = v[2];

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
	void RotationArc(const TVec3<ET>& lhs, const TVec3<ET>& rhs)
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
	void Euler(ET yaw, ET pitch, ET roll) {
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

	/** @brief 角速度からquaternionの時間微分を計算
		@param w 角速度

		このquaternionと角速度wから，quaternionの時間微分を計算する．
		ただしwは回転元（ワールド）座標系から見た回転先（ローカル）座標系の角速度を
		回転元座標系で表わしたもの．
	 */
	TQuaternion<ET> Derivative(const TVec3<ET>& w){
		return 0.5 * TQuaternion<ET>(0.0, w.X(), w.Y(), w.Z()) * *this;
	}

	/** @brief クウォータニオンの時間微分から角速度を計算
		@param qd quaternionの時間微分
		@return 角速度

		このquaternionとその時間微分qdから角速度wを計算する．
		ただしwは回転元（ワールド）座標系から見た回転先（ローカル）座標系の角速度を
		回転元座標系で表わしたもの．
	 */
	TVec3<ET> AngularVelocity(const TQuaternion<ET>& qd){
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

///	TQuaternionでベクトルを回転． TQuaternion * vector * TQuaternion.conjugated() と同じ．
template <class ET, class BD>
inline TYPENAME BD::ret_type operator*(const TQuaternion<ET>& q, const PTM::TVectorBase<DIMENC(3), BD>& v){
	TQuaternion<ET> qv(1, ET(v[0]), ET(v[1]), ET(v[2]));
	TYPENAME BD::ret_type r = (q * qv * q.Conjugated()).sub_vector(PTM::TSubVectorDim<1,3>());
	//TYPENAME BD::ret_type tmp = q.V() % v;
	//TYPENAME BD::ret_type r = q.W()*q.W()*v + 2*q.W()*tmp + (q.V()*v)*q.V() + q.V()%tmp;
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


/**	姿勢．位置と向きを表すクラス．
	TQuaternion と TVec3 で向きと位置を表す．
	TAffineの代わりに使える．
*/
template<class ET>
class TPose:public PTM::TVectorBase<DIMENC(7), TVecDesc<TPose<ET>, ET> >{
public:
	typedef TVecDesc<TPose<ET>, ET> desc;
	typedef PTM::TVectorBase<DIMENC(4), desc> base_type;
	///	継承されない基本的なメンバの定義.	@see ::DEF_TVECTOR_BASIC_MEMBER
	DEF_TVECTOR_BASIC_MEMBER(TPose);
	union{
		ET data[7];
//		struct{
//			TQuaternion<ET> Ori();
//			TVec3<ET> Pos();
//		};
		struct{
			ET w,x,y,z;
			ET px, py, pz;
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
	TQuaternion<ET>& Ori() { return *(TQuaternion<ET>*)(data); }
	const TQuaternion<ET>& Ori() const { return *(TQuaternion<ET>*)(data); }
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
	///	単位行列
	static TPose<ET> Unit(){
		TPose<ET> y;
		//PTM::init_unitize(y);
		y.Pos() = TVec3<ET>();
		y.Ori() = TQuaternion<ET>();
		return y;
	}

	/// 与えられた位置でのPoseを初期化して返す
	static TPose<ET> Unit(TVec3<ET> &v){
		TPose<ET> y;
		//PTM::init_unitize(y);
		y.Pos() = v;
		y.Ori() = TQuaternion<ET>();
		return y;
	}

	/// 与えられた回転でのPoseを初期化して返す
	static TPose<ET> Unit(TQuaternion<ET> &q){
		TPose<ET> y;
		//PTM::init_unitize(y);
		y.Pos() = TVec3<ET>();
		y.Ori() = q;
		return y;
	}

	///	平行移動
	static TPose<ET> Trn(element_type px, element_type py, element_type pz){
		return Trn(TVec3<ET>(px, py, pz));
	}
	static TPose<ET> Trn(TVec3<ET> &v){
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

///	float版TPose.
typedef TPose<float> Posef;
///	double版TPose.
typedef TPose<double> Posed;

//@}

}
#endif
