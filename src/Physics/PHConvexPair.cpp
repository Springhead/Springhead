#include "Physics.h"
#pragma hdrstop

namespace Spr{

void PHConvexPair::CalcFriction(Quaternionf* ori, Vec3f* pos, Vec3f* cog, Vec3f* vel, Vec3f* angVel, float k, float b){
	//	初めての接触の時
	if (state == NEW){
		//	バネモデルの始点を設定する．
		for(int i=0; i<2; ++i){
			transFrictionBase[i] = ori[i].inv() * (reflexForcePoint - pos[i]);
			rotFricSpring = Vec3f();
			lastOri01 = ori[0].inv() * ori[1];
		}
		return;
	}

	//	接触が継続している場合．
	for(int i=0; i<2; ++i){
		//	グローバル系に変換
		transFrictionBase[i] = ori[i] * transFrictionBase[i] + pos[i];
		//	平面上に落とす
		transFrictionBase[i] -= (transFrictionBase[i]-commonPoint) * normal * normal;
	}
	Quaternionf ori01 = ori[0].inv() * ori[1];
	Quaternionf deltaRot = lastOri01.inv() * ori01;
	DEBUG_EVAL( if (!_finite(deltaRot.rotation().norm())){ DSTR << "deltaRot.rotation():" << deltaRot << std::endl; } )
	lastOri01 = ori01;
	rotFricSpring += deltaRot.rotation();
	rotFricSpring = rotFricSpring*normal*normal;
	DEBUG_EVAL( if (!_finite(rotFricSpring.norm())){ DSTR << "rotFricSpring:" << rotFricSpring << std::endl; } )
	
	//	並進ばねの力
	Vec3f frictionSpringForce = k * (transFrictionBase[1] - transFrictionBase[0]);
	//	回転バネの力
	Vec3f frictionSpringTorque = k*rotFricSpring;

	//	動摩擦の制約を加える
	enum FricState{ STOP, DYNA } state = STOP;

	float staticFric = 0.5f;		//	μ0/μ
	float staticFricMax = staticFric * dynaFric.norm();
	float staticRotFricMax = staticFric * (dynaFricMom - (reflexForcePoint^dynaFric)).norm();
	assert(_finite(staticFricMax));
	DEBUG_EVAL( if(!_finite(staticRotFricMax)){ DSTR << "FricMax:" << staticFricMax << "  " << staticRotFricMax << std::endl; } )

	if (frictionSpringForce.norm() > staticFricMax){
		if (frictionSpringForce.norm() > 1e-10f) frictionSpringForce = frictionSpringForce.unit() * staticFricMax;
		else frictionSpringForce = Vec3f();
		state = DYNA;
	}
	if (frictionSpringTorque.norm() > staticRotFricMax){
		if (frictionSpringTorque.norm() > 1e-10f) frictionSpringTorque = staticRotFricMax * frictionSpringTorque.unit();
		else frictionSpringTorque = Vec3f();
		state = DYNA;
	}
	frictionForce = frictionSpringForce;
	frictionTorque = frictionSpringTorque;

	if (state == STOP){
		///	ダンパによる静止摩擦力の計算
		Vec3f frictionVel = ((angVel[1] ^ (reflexForcePoint-cog[1])) + vel[1]) - ((angVel[0] ^ (reflexForcePoint-cog[0])) + vel[0]);
		frictionVel -= frictionVel * normal * normal;
		frictionForce += b * frictionVel;
		
		Vec3f frictionAngVel = angVel[1] - angVel[0];
		DEBUG_EVAL( if (!_finite(frictionAngVel.norm())){ DSTR << "frictionAngVel: " << frictionAngVel << std::endl; } )

		
		frictionAngVel = (frictionAngVel * normal) * normal;
		frictionTorque += b * frictionAngVel;

		if (frictionForce.norm() > staticFricMax){
			if (frictionForce.norm() > 1e-10f) frictionForce = frictionForce.unit() * staticFricMax;
			else frictionForce = Vec3f();
		}

		if (frictionTorque.norm() > staticRotFricMax){
			if(frictionTorque.norm() > 1e-10f) frictionTorque = frictionTorque.unit() * staticRotFricMax;
			else frictionTorque = Vec3f();
		}
	}
	DEBUG_EVAL( 
	   if (frictionTorque.norm() > 100){
		   DSTR << "FT:" << frictionTorque << area << std::endl;
		   DSTR << "rotFricSpring:" << rotFricSpring;
		   DSTR << "  deltaRot:" << deltaRot;
		   DSTR << std::endl;
	   }
	)

	//	ローカル系に変換して保存
	transFrictionBase[0] = ori[0].inv() * (reflexForcePoint - 0.5f*frictionSpringForce/k - pos[0]);
	transFrictionBase[1] = ori[1].inv() * (reflexForcePoint + 0.5f*frictionSpringForce/k - pos[1]);
	rotFricSpring = frictionSpringTorque / k;
}

void PHConvexPair::CalcReflexForce(Vec3f* cog, Vec3f* vel, Vec3f* angVel, float k, float b){
	area = 0;
	reflexSpringForce.clear();
	reflexDamperForce.clear();
	reflexSpringTorque.clear();
	reflexDamperTorque.clear();
	dynaFric.clear();
	dynaFricMom.clear();

	for(Vtxs::iterator it = vtxs.begin(); it != planes.vtxBegin; ++it){
		CDFace& face = **it;
		if (face.NCommonVtx() < 3) continue;
		int cvxId = ((CDPolyhedron*)&*convex[0])->HasFace(&face) ? 0 : 1;
		Vec3f p0 = face.CommonVtx(0) + commonPoint;
		Vec3f p1;
		Vec3f p2 = face.CommonVtx(1) + commonPoint;
		for(unsigned i=2; i<face.NCommonVtx(); ++i){
			p1 = p2;
			p2 = face.CommonVtx(i) + commonPoint;
			CalcTriangleReflexForce(p0, p1, p2, cog[cvxId], vel[cvxId], angVel[cvxId], k, b);
		}
	}
	/*	ap: 作用点とすると
		torque = ap ^ force,
		ap * force = 0 とすると ap ⊥ torque,  force ⊥ torque, ap ⊥ force 
		となり，3つのベクトルは直行する．
		そのため，
		|torque| = |ap|*|force| となる．

		ap = ((force^torque) / (|force|*|torque|)) * (|torque|/|force|)
		   = (force^torque) / force^2
		
		(ap + k*normal) * normal = interior * normal
		k = (interior-ap) * normal 
	*/
	if (reflexSpringForce.square() > 1e-8){
		reflexForcePoint = (reflexSpringForce ^ reflexSpringTorque) / (reflexSpringForce*reflexSpringForce);
		reflexForcePoint -= (reflexForcePoint-commonPoint) * normal * normal;
	}else{
		reflexForcePoint = commonPoint;
	}
#ifdef _DEBUG
	if (!_finite(reflexForcePoint.norm()) || !_finite(reflexSpringForce.norm()) || reflexForcePoint.norm() > 100){
		DSTR << "reflexForcePoint:" << reflexForcePoint << std::endl;
		DSTR << "reflexSpringForce:"  << reflexSpringForce << std::endl;
		DSTR << "reflexTorque:"  << reflexSpringTorque << std::endl;
		DSTR << "commonPoint:"  << commonPoint << std::endl;
	}
	if (area<=0){
		DSTR << "Area0:" << std::endl;
	}
	if (reflexSpringForce.norm() > 10000 || !_finite(reflexSpringForce.norm()) ){
		int i=0;
		DSTR << "Error: Too large reflexForce: " << reflexSpringForce << std::endl;
		DSTR << "commonPoint:" << commonPoint << std::endl;
		for(Vtxs::iterator it = vtxs.begin(); it != planes.vtxBegin; ++it){
			CDFace& face = **it;
			DSTR << "p" << i++ << "\t d:" << face.dist << " n:" << face.normal << std::endl;
		}
		i = 1;
		for(CDQHPlane<CDFace>* it = planes.begin; it != planes.end; ++it){
			if (it->deleted) continue;
			DSTR << "v" << i++ << "\t " << it->normal/it->dist << std::endl;
		}
	}
#endif
}

void PHConvexPair::CalcTriangleReflexForce(Vec3f p0, Vec3f p1, Vec3f p2, Vec3f cog, Vec3f vel, Vec3f angVel, float spring, float damper){
	//---------------------------------------------------------------
	//	ばねモデルの計算：各頂点の侵入深さの計算
	float depth0 = (p0 - commonPoint) * normal;
 	float depth1 = (p1 - commonPoint) * normal;
 	float depth2 = (p2 - commonPoint) * normal;
	//	ダンパモデルの計算：各頂点の速度とその法線方向成分を求める
	angVel *= 1;	/*	角速度をダンパの計算の前に増やす．
						ここで角速度のダンパを強くすると安定するけど，
						転がらなくなる．
						抗力は出しても，引力は出さないような工夫をすれば
						安定でかつ転がるようにできるのではないかと思う．
						by hase
					*/
	p0 -= (p0 - commonPoint) * normal * normal;
	p1 -= (p1 - commonPoint) * normal * normal;
	p2 -= (p2 - commonPoint) * normal * normal;
	Vec3f vel0 = (angVel^(p0-cog)) + vel;
	Vec3f vel1 = (angVel^(p1-cog)) + vel;
	Vec3f vel2 = (angVel^(p2-cog)) + vel;
	float vel0_normal = vel0 * normal;
	float vel1_normal = vel1 * normal;
	float vel2_normal = vel2 * normal;

	//	抗力を計算
	float refSp0 = spring * depth0;
	float refSp1 = spring * depth1;
	float refSp2 = spring * depth2;

	float refDa0 = damper * vel0_normal;
	float refDa1 = damper * vel1_normal;
	float refDa2 = damper * vel2_normal;

	//---------------------------------------------------------------
	//	三角形について積分
	Vec3f a = p1 - p0;
	Vec3f b = p2 - p0;
	Vec3f a_b = a^b;
	float a_b_normal = a_b * normal;
	Vec3f triRefSp = (1.0f/6.0f) * (refSp0 + refSp1 + refSp2) * a_b;
	Vec3f triRefMomSp = (
				((1.0f/12.0f)*refSp0 + (1.0f/24.0f)*refSp1 + (1.0f/24.0f)*refSp2) * p0
			+	((1.0f/24.0f)*refSp0 + (1.0f/12.0f)*refSp1 + (1.0f/24.0f)*refSp2) * p1
			+	((1.0f/24.0f)*refSp0 + (1.0f/24.0f)*refSp1 + (1.0f/12.0f)*refSp2) * p2
		  ) ^ a_b;
	Vec3f triRefDa = (1.0f/6.0f) * (refDa0 + refDa1 + refDa2) * a_b;
	Vec3f triRefMomDa = (
				((1.0f/12.0f)*refDa0 + (1.0f/24.0f)*refDa1 + (1.0f/24.0f)*refDa2) * p0
			+	((1.0f/24.0f)*refDa0 + (1.0f/12.0f)*refDa1 + (1.0f/24.0f)*refDa2) * p1
			+	((1.0f/24.0f)*refDa0 + (1.0f/24.0f)*refDa1 + (1.0f/12.0f)*refDa2) * p2
		  ) ^ a_b;
#ifdef _DEBUG
	if (refSp0 > 1000 || refSp1 > 1000 || refSp2 > 1000 || !_finite(triRefSp.norm()) ){
		DSTR << "Error: The reflection spring force is too large: " << triRefSp << std::endl;
	}
#endif
	assert(_finite(triRefMomSp.norm()));

	//	3角形の面積の計算
	float triArea = a_b_normal / 4;
	if (triArea < 0) triArea = -triArea;

	//---------------------------------------------------------------
	//	動摩擦力を求める
	Vec3f velTan0 = vel0 - vel0_normal * normal;
	Vec3f velTan1 = vel1 - vel1_normal * normal;
	Vec3f velTan2 = vel2 - vel2_normal * normal;
	Vec3f fric0, fric1, fric2;
	if (velTan0.square() > 1e-8) fric0 = velTan0.unit() * (refSp0+refDa0);
	if (velTan1.square() > 1e-8) fric1 = velTan1.unit() * (refSp1+refDa1);
	if (velTan2.square() > 1e-8) fric2 = velTan2.unit() * (refSp2+refDa2);

	Vec3f triFric = (1.0f/6.0f) * (fric0 + fric1 + fric2) * a_b_normal;
	Vec3f triFricMom = (
				(p0 ^ ((1.0f/12.0f)*fric0 + (1.0f/24.0f)*fric1 + (1.0f/24.0f)*fric2))
			+	(p1 ^ ((1.0f/24.0f)*fric0 + (1.0f/12.0f)*fric1 + (1.0f/24.0f)*fric2))
			+	(p2 ^ ((1.0f/24.0f)*fric0 + (1.0f/24.0f)*fric1 + (1.0f/12.0f)*fric2))
		  ) * a_b_normal;
	assert(_finite(triFric.norm()));
	assert(_finite(triFricMom.norm()));
	//---------------------------------------------------------------
	//	結果を加える
	area += triArea;
	reflexSpringForce += triRefSp;
	reflexSpringTorque += triRefMomSp;
	reflexDamperForce += triRefDa;
	reflexDamperTorque += triRefMomDa;
	dynaFric += triFric;
	dynaFricMom += triFricMom;
}

//----------反力と摩擦力のばね，ダンパ係数を別々に分けた---------------
void PHConvexPair::CalcReflexForce(Vec3f* cog, Vec3f* vel, Vec3f* angVel, float rk, float rb, float fk, float fb){
	area = 0;
	reflexSpringForce.clear();
	reflexDamperForce.clear();
	reflexSpringTorque.clear();
	reflexDamperTorque.clear();
	dynaFric.clear();
	dynaFricMom.clear();

	for(Vtxs::iterator it = vtxs.begin(); it != planes.vtxBegin; ++it){
		CDFace& face = **it;
		if (face.NCommonVtx() < 3) continue;
		int cvxId = ((CDPolyhedron*)&*convex[0])->HasFace(&face) ? 0 : 1;
		Vec3f p0 = face.CommonVtx(0) + commonPoint;
		Vec3f p1;
		Vec3f p2 = face.CommonVtx(1) + commonPoint;
		for(unsigned i=2; i<face.NCommonVtx(); ++i){
			p1 = p2;
			p2 = face.CommonVtx(i) + commonPoint;
			CalcTriangleReflexForce(p0, p1, p2, cog[cvxId], vel[cvxId], angVel[cvxId], rk, rb, fk, fb);
		}
	}
	/*	ap: 作用点とすると
		torque = ap ^ force,
		ap * force = 0 とすると ap ⊥ torque,  force ⊥ torque, ap ⊥ force 
		となり，3つのベクトルは直行する．
		そのため，
		|torque| = |ap|*|force| となる．

		ap = ((force^torque) / (|force|*|torque|)) * (|torque|/|force|)
		   = (force^torque) / force^2
		
		(ap + k*normal) * normal = interior * normal
		k = (interior-ap) * normal 
	*/
	if (reflexSpringForce.square() > 1e-8){
		reflexForcePoint = (reflexSpringForce ^ reflexSpringTorque) / (reflexSpringForce*reflexSpringForce);
		reflexForcePoint -= (reflexForcePoint-commonPoint) * normal * normal;
	}else{
		reflexForcePoint = commonPoint;
	}
#ifdef _DEBUG
	if (!_finite(reflexForcePoint.norm()) || !_finite(reflexSpringForce.norm()) || reflexForcePoint.norm() > 100){
		DSTR << "reflexForcePoint:" << reflexForcePoint << std::endl;
		DSTR << "reflexSpringForce:"  << reflexSpringForce << std::endl;
		DSTR << "reflexTorque:"  << reflexSpringTorque << std::endl;
		DSTR << "commonPoint:"  << commonPoint << std::endl;
	}
	if (area<=0){
		DSTR << "Area0:" << std::endl;
	}
	if (reflexSpringForce.norm() > 10000 || !_finite(reflexSpringForce.norm()) ){
		int i=0;
		DSTR << "Error: Too large reflexForce: " << reflexSpringForce << std::endl;
		DSTR << "commonPoint:" << commonPoint << std::endl;
		for(Vtxs::iterator it = vtxs.begin(); it != planes.vtxBegin; ++it){
			CDFace& face = **it;
			DSTR << "p" << i++ << "\t d:" << face.dist << " n:" << face.normal << std::endl;
		}
		i = 1;
		for(CDQHPlane<CDFace>* it = planes.begin; it != planes.end; ++it){
			if (it->deleted) continue;
			DSTR << "v" << i++ << "\t " << it->normal/it->dist << std::endl;
		}
	}
#endif
}

//----------反力と摩擦力のばね，ダンパ係数を別々に分けた---------------
void PHConvexPair::CalcTriangleReflexForce(Vec3f p0, Vec3f p1, Vec3f p2, Vec3f cog, Vec3f vel, Vec3f angVel, float rk, float rb, float fk, float fb){
	//---------------------------------------------------------------
	//	ばねモデルの計算：各頂点の侵入深さの計算
	float depth0 = (p0 - commonPoint) * normal;
 	float depth1 = (p1 - commonPoint) * normal;
 	float depth2 = (p2 - commonPoint) * normal;
	//	ダンパモデルの計算：各頂点の速度とその法線方向成分を求める
	angVel *= 1;	/*	角速度をダンパの計算の前に増やす．
						ここで角速度のダンパを強くすると安定するけど，
						転がらなくなる．
						抗力は出しても，引力は出さないような工夫をすれば
						安定でかつ転がるようにできるのではないかと思う．
						by hase
					*/
	p0 -= (p0 - commonPoint) * normal * normal;
	p1 -= (p1 - commonPoint) * normal * normal;
	p2 -= (p2 - commonPoint) * normal * normal;
	Vec3f vel0 = (angVel^(p0-cog)) + vel;
	Vec3f vel1 = (angVel^(p1-cog)) + vel;
	Vec3f vel2 = (angVel^(p2-cog)) + vel;
	float vel0_normal = vel0 * normal;
	float vel1_normal = vel1 * normal;
	float vel2_normal = vel2 * normal;

	//	抗力を計算
	float refSp0 = rk * depth0;
	float refSp1 = rk * depth1;
	float refSp2 = rk * depth2;

	float refDa0 = rb * vel0_normal;
	float refDa1 = rb * vel1_normal;
	float refDa2 = rb * vel2_normal;

	//---------------------------------------------------------------
	//	三角形について積分
	Vec3f a = p1 - p0;
	Vec3f b = p2 - p0;
	Vec3f a_b = a^b;
	float a_b_normal = a_b * normal;
	Vec3f triRefSp = (1.0f/6.0f) * (refSp0 + refSp1 + refSp2) * a_b;
	Vec3f triRefMomSp = (
				((1.0f/12.0f)*refSp0 + (1.0f/24.0f)*refSp1 + (1.0f/24.0f)*refSp2) * p0
			+	((1.0f/24.0f)*refSp0 + (1.0f/12.0f)*refSp1 + (1.0f/24.0f)*refSp2) * p1
			+	((1.0f/24.0f)*refSp0 + (1.0f/24.0f)*refSp1 + (1.0f/12.0f)*refSp2) * p2
		  ) ^ a_b;
	Vec3f triRefDa = (1.0f/6.0f) * (refDa0 + refDa1 + refDa2) * a_b;
	Vec3f triRefMomDa = (
				((1.0f/12.0f)*refDa0 + (1.0f/24.0f)*refDa1 + (1.0f/24.0f)*refDa2) * p0
			+	((1.0f/24.0f)*refDa0 + (1.0f/12.0f)*refDa1 + (1.0f/24.0f)*refDa2) * p1
			+	((1.0f/24.0f)*refDa0 + (1.0f/24.0f)*refDa1 + (1.0f/12.0f)*refDa2) * p2
		  ) ^ a_b;
#ifdef _DEBUG
	if (refSp0 > 1000 || refSp1 > 1000 || refSp2 > 1000 || !_finite(triRefSp.norm()) ){
		DSTR << "Error: The reflection spring force is too large: " << triRefSp << std::endl;
	}
#endif
	assert(_finite(triRefMomSp.norm()));

	//	3角形の面積の計算
	float triArea = a_b_normal / 4;
	if (triArea < 0) triArea = -triArea;

	//---------------------------------------------------------------
	//	動摩擦力を求める
	float friSp0 = fk * depth0;
	float friSp1 = fk * depth1;
	float friSp2 = fk * depth2;
	float friDa0 = fb * vel0_normal;
	float friDa1 = fb * vel1_normal;
	float friDa2 = fb * vel2_normal;
	Vec3f velTan0 = vel0 - vel0_normal * normal;
	Vec3f velTan1 = vel1 - vel1_normal * normal;
	Vec3f velTan2 = vel2 - vel2_normal * normal;
	Vec3f fric0, fric1, fric2;
	if (velTan0.square() > 1e-8) fric0 = velTan0.unit() * (friSp0+friDa0);
	if (velTan1.square() > 1e-8) fric1 = velTan1.unit() * (friSp1+friDa1);
	if (velTan2.square() > 1e-8) fric2 = velTan2.unit() * (friSp2+friDa2);

	Vec3f triFric = (1.0f/6.0f) * (fric0 + fric1 + fric2) * a_b_normal;
	Vec3f triFricMom = (
				(p0 ^ ((1.0f/12.0f)*fric0 + (1.0f/24.0f)*fric1 + (1.0f/24.0f)*fric2))
			+	(p1 ^ ((1.0f/24.0f)*fric0 + (1.0f/12.0f)*fric1 + (1.0f/24.0f)*fric2))
			+	(p2 ^ ((1.0f/24.0f)*fric0 + (1.0f/24.0f)*fric1 + (1.0f/12.0f)*fric2))
		  ) * a_b_normal;
	assert(_finite(triFric.norm()));
	assert(_finite(triFricMom.norm()));
	//---------------------------------------------------------------
	//	結果を加える
	area += triArea;
	reflexSpringForce += triRefSp;
	reflexSpringTorque += triRefMomSp;
	reflexDamperForce += triRefDa;
	reflexDamperTorque += triRefMomDa;
	dynaFric += triFric;
	dynaFricMom += triFricMom;
}

};
