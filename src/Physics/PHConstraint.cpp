#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
using namespace std;
namespace Spr{;

#define SUBMAT(r, c, h, w) sub_matrix(TSubMatrixDim<r, c, h, w>())
#define SUBVEC(o, l) sub_vector(TSubVectorDim<o, l>())

inline double rowtimes(const Matrix3d& M, int k, const Vec3d& v){
	return M.item(k, 0) * v[0] + M.item(k, 1) * v[1] + M.item(k, 2) * v[2];
}

//----------------------------------------------------------------------------
// PHSolidAux
void PHSolidAux::SetupDynamics(double dt){
	minv = solid->GetMassInv();
	Iinv = solid->GetInertiaInv();
	Quaterniond q = solid->GetOrientation();
	v = q.Conjugated() * solid->GetVelocity();
	w = q.Conjugated() * solid->GetAngularVelocity();
	if(solid->IsDynamical()){
		f = q.Conjugated() * solid->nextForce;
		t = q.Conjugated() * solid->nextTorque;
		dv0 = minv * f * dt;
		dw0 = Iinv * (t - w % (solid->GetInertia() * w)) * dt;
	}
	else{
		dv0.clear();
		dw0.clear();
	}
	dv.clear();
	dw.clear();
}
void PHSolidAux::SetupCorrection(){
	dV.clear();
	dW.clear();
}

//----------------------------------------------------------------------------
// PHConstraint
//OBJECTIMP(PHConstraint, SceneObject);
IF_IMP(PHConstraint, SceneObject);
PHConstraint::PHConstraint(){
	fv.clear();
	fw.clear();
	Fv.clear();
	Fq.clear();
	bEnabled = true;
}
void PHConstraint::Init(PHSolidAux* lhs, PHSolidAux* rhs, const PHJointDesc& desc){
	solid[0] = lhs, solid[1] = rhs;
	for(int i = 0; i < 2; i++){
		desc.poseJoint[i].Ori().ToMatrix(Rj[i]);
		rj[i] = desc.poseJoint[i].Pos();
	}
	bEnabled = desc.bEnabled;
}
void PHConstraint::CompJacobian(bool bCompAngular){
	Matrix3d	R[2], Rjabs[2];
	Vec3d		r[2];
	R[0] = solid[0]->solid->GetRotation();
	R[1] = solid[1]->solid->GetRotation();
	r[0] = solid[0]->solid->GetCenterPosition();
	r[1] = solid[1]->solid->GetCenterPosition();
	Rjabs[0] = R[0] * Rj[0];
	Rjabs[1] = R[1] * Rj[1];
	Rjrel = Rjabs[0].trans() * Rjabs[1];
	qjrel.FromMatrix(Rjrel);
	rjrel = Rjabs[0].trans() * ((R[1] * rj[1] + r[1]) - (R[0] * rj[0] + r[0]));
	Jvv[0] = -Rj[0].trans();
	Jvw[0] = -Rj[0].trans() * (-Matrix3d::Cross(rj[0]));
	Jvv[1] =  Rjabs[0].trans() * R[1];
	Jvw[1] =  Jvv[1] * (-Matrix3d::Cross(rj[1]));
	vjrel = Jvv[0] * solid[0]->v + Jvw[0] * solid[0]->w + Jvv[1] * solid[1]->v + Jvw[1] * solid[1]->w;
	
	if(bCompAngular){
		Jwv[0].clear();
		Jww[0] = Jvv[0];
		Jwv[1].clear();
		Jww[1] = Jvv[1];
		wjrel = Jwv[0] * solid[0]->v + Jww[0] * solid[0]->w + Jwv[1] * solid[1]->v + Jww[1] * solid[1]->w;
		
		Matrix3d E(
			 qjrel.W(),  qjrel.Z(), -qjrel.Y(),
			-qjrel.Z(),  qjrel.W(),  qjrel.X(),
			 qjrel.Y(), -qjrel.X(),  qjrel.W());
		E *= 0.5;
		Jqv[0].clear();
		Jqw[0] = E * Jww[0];
		Jqv[1].clear();
		Jqw[1] = E * Jww[1];
	}
}

void PHConstraint::SetupDynamics(double dt){
	bFeasible = solid[0]->solid->IsDynamical() || solid[1]->solid->IsDynamical();
	if(!bEnabled || !bFeasible)return;

	CompJacobian(GetJointType() != PHJointDesc::JOINT_CONTACT);		//ê⁄êGçSë©ÇÃèÍçáÇÕâÒì]ä÷åWÇÃÉÑÉRÉrçsóÒÇÕïKóvÇ»Ç¢
	CompDof();
	CompMotorForce();

	int i, j;
	Av.clear();
	Aw.clear();
	Aq.clear();
	for(i = 0; i < 2; i++){
		if(solid[i]->solid->IsDynamical()){
			for(j = 0; j < dim_v; j++){
				Tvv[i].row(j) = Jvv[i].row(j) * solid[i]->minv;
				Tvw[i].row(j) = Jvw[i].row(j) * solid[i]->Iinv;
				Av[j] += Jvv[i].row(j) * Tvv[i].row(j) + Jvw[i].row(j) * Tvw[i].row(j);
			}
			if(mode == MODE_TORQUE){
				for(; j < 3; j++){
					solid[i]->dv += Tvv[i].row(j) * fv[j];
					solid[i]->dw += Tvw[i].row(j) * fv[j];
				}
			}
			for(j = 0; j < dim_w; j++){
				Twv[i].row(j) = Jwv[i].row(j) * solid[i]->minv;
				Tww[i].row(j) = Jww[i].row(j) * solid[i]->Iinv;
				Aw[j] += Jwv[i].row(j) * Twv[i].row(j) + Jww[i].row(j) * Tww[i].row(j);
			}
			if(mode == MODE_TORQUE){
				for(; j < 3; j++){
					solid[i]->dv += Twv[i].row(j) * fw[j];
					solid[i]->dw += Tww[i].row(j) * fw[j];
				}
			}
			for(j = 0; j < dim_q; j++){
				Tqv[i].row(j) = Jqv[i].row(j) * solid[i]->minv;
				Tqw[i].row(j) = Jqw[i].row(j) * solid[i]->Iinv;
				Aq[j] += Jqv[i].row(j) * Tqv[i].row(j) + Jqw[i].row(j) * Tqw[i].row(j);
			}
		}
	}
	
	for(j = 0; j < dim_v; j++){
		bv[j] = vjrel[j] +
			rowtimes(Jvv[0], j, solid[0]->dv0) + rowtimes(Jvw[0], j, solid[0]->dw0) +
			rowtimes(Jvv[1], j, solid[1]->dv0) + rowtimes(Jvw[1], j, solid[1]->dw0);
	}
	for(int j = 0; j < dim_w; j++){
		bw[j] = wjrel[j] + 
			rowtimes(Jwv[0], j, solid[0]->dv0) + rowtimes(Jww[0], j, solid[0]->dw0) +
			rowtimes(Jwv[1], j, solid[1]->dv0) + rowtimes(Jww[1], j, solid[1]->dw0);
	}

	CompBias(dt);

	if(dim_v)for(i = 0; i < 3; i++)
		Av[i] = 1.0 / Av[i];
	if(dim_w)for(i = 0; i < 3; i++)
		Aw[i] = 1.0 / Aw[i];
	if(dim_q)for(i = 0; i < 3; i++)
		Aq[i] = 1.0 / Aq[i];

	for(j = 0; j < dim_v; j++){
		bv[j] *= Av[j];
		Jvv[0].row(j) *= Av[j];
		Jvw[0].row(j) *= Av[j];
		Jvv[1].row(j) *= Av[j];
		Jvw[1].row(j) *= Av[j];
	}
	for(int j = 0; j < dim_w; j++){
		bw[j] *= Aw[j];
		Jwv[0].row(j) *= Aw[j];
		Jww[0].row(j) *= Aw[j];
		Jwv[1].row(j) *= Aw[j];
		Jww[1].row(j) *= Aw[j];
	}
	//fv.clear();
	//fw.clear();
}
void PHConstraint::CompError(){
	Bv = rjrel;
	Quaterniond qrel;
	qrel.FromMatrix(Rjrel);
	Bq = qrel.V();
}
void PHConstraint::SetupCorrection(double dt, double max_error){
	if(!bEnabled || !bFeasible)return;

	int j;
	Vec3d v[2], w[2];
	for(int i = 0; i < 2; i++){
		v[i] = solid[i]->v + solid[i]->dv0 + solid[i]->dv;
		w[i] = solid[i]->w + solid[i]->dw0 + solid[i]->dw;
	}
	CompError();
	// velocity updateÇ…ÇÊÇÈâeãøÇâ¡éZ
	double tmp;
	for(j = 0; j < dim_v; j++){
		Bv[j] *= Av[j];
		Bv[j] += (rowtimes(Jvv[0], j, v[0]) + rowtimes(Jvw[0], j, w[0]) +
				  rowtimes(Jvv[1], j, v[1]) + rowtimes(Jvw[1], j, w[1])) * dt;
	}
	Bv *= 0.1;	//àÍìxÇ…åÎç∑Ç0Ç…ÇµÇÊÇ§Ç∆Ç∑ÇÈÇ∆Ç‚Ç‚êUìÆìIÇ…Ç»ÇÈÇÃÇ≈ìKìñÇ…åÎç∑Çè¨Ç≥Ç≠å©ÇπÇÈ
	tmp = max(max(Bv[0], Bv[1]), Bv[2]);
	if(tmp > max_error)
		Bv *= (max_error / tmp);
	for(j = 0; j < dim_q; j++){
		Bq[j] *= Aq[j];
		Bq[j] += (rowtimes(Jqv[0], j, v[0]) + rowtimes(Jqw[0], j, w[0]) +
				  rowtimes(Jqv[1], j, v[1]) + rowtimes(Jqw[1], j, w[1])) * dt;
	}
	Bq *= 0.1;
	tmp = max(max(Bq[0], Bq[1]), Bq[2]);
	if(tmp > max_error)
		Bq *= (max_error / tmp);
	//DSTR << B.norm() << endl;
	//Fv.clear();
	//Fq.clear();
}
void PHConstraint::IterateDynamics(){
	if(!bEnabled || !bFeasible)return;

	Vec3d fvnew, fwnew, dfv, dfw;
	int i, j;
	for(j = 0; j < dim_v; j++){
		fvnew[j] = fv[j] - (bv[j] + 
			rowtimes(Jvv[0], j, solid[0]->dv) + rowtimes(Jvw[0], j, solid[0]->dw) +
			rowtimes(Jvv[1], j, solid[1]->dv) + rowtimes(Jvw[1], j, solid[1]->dw));
		Projectionfv(fvnew[j], j);
		dfv[j] = fvnew[j] - fv[j];
		for(i = 0; i < 2; i++){
			if(solid[i]->solid->IsDynamical()){
				solid[i]->dv += Tvv[i].row(j) * dfv[j];
				solid[i]->dw += Tvw[i].row(j) * dfv[j];
			}
		}
		fv[j] = fvnew[j];
	}
	for(j = 0; j < dim_w; j++){
		fwnew[j] = fw[j] - (bw[j] + 
			rowtimes(Jwv[0], j, solid[0]->dv) + rowtimes(Jww[0], j, solid[0]->dw) +
			rowtimes(Jwv[1], j, solid[1]->dv) + rowtimes(Jww[1], j, solid[1]->dw));
		Projectionfw(fwnew[j], j);
		dfw[j] = fwnew[j] - fw[j];
		for(i = 0; i < 2; i++){
			if(solid[i]->solid->IsDynamical()){
				solid[i]->dv += Twv[i].row(j) * dfw[j];
				solid[i]->dw += Tww[i].row(j) * dfw[j];
			}
		}
		fw[j] = fwnew[j];
	}
}
void PHConstraint::IterateCorrection(){
	if(!bEnabled || !bFeasible)return;

	Vec3d Fvnew, Fqnew, dFv, dFq;
	int i, j;
	for(j = 0; j < dim_v; j++){
		Fvnew[j] = Fv[j] - (Bv[j] + 
			rowtimes(Jvv[0], j, solid[0]->dV) + rowtimes(Jvw[0], j, solid[0]->dW) +
			rowtimes(Jvv[1], j, solid[1]->dV) + rowtimes(Jvw[1], j, solid[1]->dW));
		ProjectionFv(Fvnew[j], j);
		dFv[j] = Fvnew[j] - Fv[j];
		for(i = 0; i < 2; i++){
			if(solid[i]->solid->IsDynamical()){
				solid[i]->dV += Tvv[i].row(j) * dFv[j];
				solid[i]->dW += Tvw[i].row(j) * dFv[j];
			}
		}
		Fv[j] = Fvnew[j];
	}
	for(j = 0; j < dim_q; j++){
		Fqnew[j] = Fq[j] - (Bq[j] + 
			rowtimes(Jqv[0], j, solid[0]->dV) + rowtimes(Jqw[0], j, solid[0]->dW) +
			rowtimes(Jqv[1], j, solid[1]->dV) + rowtimes(Jqw[1], j, solid[1]->dW));
		ProjectionFq(Fqnew[j], j);
		dFq[j] = Fqnew[j] - Fq[j];
		for(i = 0; i < 2; i++){
			if(solid[i]->solid->IsDynamical()){
				solid[i]->dV += Tqv[i].row(j) * dFq[j];
				solid[i]->dW += Tqw[i].row(j) * dFq[j];
			}
		}
		Fq[j] = Fqnew[j];
	}
}

}