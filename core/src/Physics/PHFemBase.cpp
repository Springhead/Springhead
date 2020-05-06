#include "PHFemMeshNew.h"
#include "PHFemBase.h"

namespace Spr{;

///////////////////////////////////////////////////////////////////
/* 計算モジュールの共通部分 */
void PHFemBase::SetPHFemMesh(PHFemMeshNew* m){ phFemMesh = m; }

PHFemMeshNewIf* PHFemBase::GetPHFemMesh(){ return phFemMesh->Cast(); }

int PHFemBase::NVertices(){ return GetPHFemMesh()->NVertices(); }


//* 積分関数
/////////////////////////////////////////////////////////////////////////////////////////
//行列版
void PHFemBase::CompInitialCondition(const VMatrixRd& _M, const VMatrixRd& _K, const VMatrixRd& _C,
		const VVectord& _f, VVectord& _x, VVectord& _v, VVectord& _a){
	_a = _M.inv() * (_C * (-1.0 * _v) + _K * (-1.0 * _x) + _f);
}

void PHFemBase::InitExplicitEuler(const VMatrixRd& _M, VMatrixRd& _MInv){
	_MInv.assign(_M.inv());
}

void PHFemBase::ExplicitEuler(const VMatrixRd& _MInv, const VMatrixRd& _K, const VMatrixRd& _C, 
		const VVectord& _f, const double& _dt, VVectord& _xd, VVectord& _v){
	VVectord tmp;
	tmp.assign(_f - (_K *_xd) - ( _C * _v));
	_v += _MInv * tmp * _dt;
	_xd += _v * _dt;
}

// 定式化しなおさないといけない.2013.1.3
void PHFemBase::ImplicitEuler(const VMatrixRd& _MInv, const VMatrixRd& _K, const VMatrixRd& _C, 
		const VVectord& _f, const double& _dt, VVectord& _xd, VVectord& _v){
#if 0
	// 外力をまだ組み込んでない
	int NDof = GetPHFemMesh()->vertices.size() * 3;
	VMatrixRd E;	// 単位行列
	E.resize(NDof, NDof);
	E.clear(0.0);
	for(int i = 0; i < NDof; i++){
		E[i][i] = 1.0;
	}
	VMatrixRd _Kt;
	_Kt.resize(NDof, NDof);
	_Kt.clear(0.0);	
	VMatrixRd _CtInv;
	_CtInv.resize(NDof, NDof);
	_CtInv.clear(0.0);
	VMatrixRd _DInv;
	_DInv.resize(NDof, NDof);
	_DInv.clear(0.0);

	_Kt = _MInv * _K * _dt;
	_CtInv = (E + _MInv * _C * _dt).inv();
	_DInv = (E + _CtInv * _Kt * vdt).inv();

	xdl = _DInv * (xdl + _CtInv * (vl + _Kt * xdlInit) * vdt);
	vl = _CtInv * (vl - _Kt * (xdl - xdlInit));
	//DSTR << "Integrate" << std::endl;
	//DSTR << "_K" << std::endl;
	//DSTR << _K << std::endl;
	//DSTR << "_CInv" << std::endl;
	//DSTR << _CInv << std::endl;
	//DSTR << "_DInv" << std::endl;
	//DSTR << _DInv << std::endl;
#endif
}

void PHFemBase::InitSimplectic(const VMatrixRd& _M, VMatrixRd& _MInv){
	_MInv.assign(_M.inv());
}

void PHFemBase::Simplectic(const VMatrixRd& _MInv, const VMatrixRd& _K, const VMatrixRd& _C, 
	const VVectord& _f, const double& _dt, VVectord& _xd, VVectord& _v){
	VVectord tmp;
	tmp.assign(_f - (_K *_xd) - ( _C* _v));
	_xd += _v * _dt;
	_v += _MInv * tmp * _dt;
}

void PHFemBase::InitNewmarkBeta(const VMatrixRd& _M, const VMatrixRd& _K, const VMatrixRd& _C, 
	const double& _dt, VMatrixRd& _SInv, const double b){
	double dt2 = pow(_dt, 2);
	_SInv.assign((_M + (0.5 *_dt * _C) + (b * dt2 * _K)).inv());	// _M, _K, Cが変化しない限り定数
}

void PHFemBase::NewmarkBeta(const VMatrixRd& _SInv, const VMatrixRd& _K, const VMatrixRd& _C, 
		const VVectord& _f, const double& _dt, VVectord& _x, VVectord& _v, VVectord& _a, const double b){
	double dt2 = pow(_dt, 2);
	VVectord _Ct;
	_Ct.assign(_C * (_v + (0.5 * _dt * _a)));
	VVectord _Kt;
	_Kt.assign(_K * (_x + (_dt * _v) + ((0.5 - b)* dt2 * _a)));
	VVectord _al;	// 前回の加速度
	_al.assign(_a);

	_a = _SInv * (_f - _Ct - _Kt);
	_x += (_dt * _v) + ((0.5 - b) * dt2 * _al) + (b * dt2 * _a);	// xの更新が先
	_v += 0.5 * _dt * (_al + _a);
}

// 1自由度版
void PHFemBase::InitExplicitEuler(const double& _m, double& _sInv){
	_sInv = 1.0 / _m;		// _mが変化しない限り定数
}

void PHFemBase::ExplicitEuler(const double& _sInv, const double& _k, const double& _c, 
		const double& _f, const double& _dt, double& _x, double& _v){
	double tmp = _f - (_k * _x) - (_c * _v);
	_v += _dt * _sInv * tmp;
	_x += _dt * _v;			// 速度更新が後
}

void PHFemBase::InitSimplectic(const double& _m, double& _sInv){
	_sInv = 1.0 / _m;		// _mが変化しない限り定数
}

void PHFemBase::Simplectic(const double& _sInv, const double& _k, const double& _c, 
		const double& _f, const double& _dt, double& _x, double& _v){
	double tmp = _f - (_k * _x) - (_c * _v);
	_v += _dt * _sInv * tmp;	// 速度更新が先
	_x += _dt * _v;
}

void PHFemBase::InitNewmarkBeta(const double& _m, const double& _k , const double& _c, 
	const double & _dt, double& _sInv, const double b){
	double dt2 = pow(_dt, 2);
	_sInv = 1.0 / (_m + (0.5 * _dt * _c) + (b * dt2 * _k));		// _m, _k, _cが変化しない場合は定数
}

void PHFemBase::NewmarkBeta(const double& _sInv, const double& _k, const double& _c,
		const double& _f, const double& _dt, double& _x, double& _v, double& _a, const double b){
	double dt2 = pow(_dt, 2);
	double _al = _a;	// 前回の加速度
	double _ct = _c * (_v + (0.5 * _dt * _a));
	double _kt = _k * (_x + (_dt * _v) + ((0.5 - b) * dt2 * _a));
	_a = _sInv * (_f - _ct - _kt);
	_x += (_dt * _v) + ((0.5 - b) * dt2 * _al) + (b * dt2 * _a);	// xの更新が先
	_v += 0.5 * _dt * (_al + _a);
}
}
