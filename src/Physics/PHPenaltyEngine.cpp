/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <float.h>
#include <Collision/CDDetectorImp.h>
#include <Base/Affine.h>

using namespace PTM;
namespace Spr{;
const float SPRING = 0.2f;
const float DAMPER = 0.3f;
extern bool bUseContactVolume;
static float reflexSpring;
static float reflexDamper;
static float frictionSpring;
static float frictionDamper;
static float staticFriction;
static float dynamicFriction;

//----------------------------------------------------------------------------
// PHShapePairForPenalty
void PHShapePairForPenalty::Clear(){
	area = 0;
	reflexSpringForce.clear();
	reflexDamperForce.clear();
	reflexSpringTorque.clear();
	reflexDamperTorque.clear();
	dynaFric.clear();
	dynaFricMom.clear();
}

//----------------------------------------------------------------------------
// PHSolidInfoForPenalty
void PHSolidInfoForPenalty::UpdateCache(int c){
	if ((unsigned)c == count) return;
	assert(solid);
	count = c;
	vel = solid->GetVelocity();
	angVel = solid->GetAngularVelocity();
	lastPos = pos;
	pos = solid->GetFramePosition();
	lastOri = ori;
	ori = solid->GetOrientation();
	cog = ori * solid->GetCenter() + pos;
}

//----------------------------------------------------------------------------
// PHSolidPairForPenalty

void PHSolidPairForPenalty::Setup(unsigned int ct, double dt){
	//	動力学計算の準備
	reflexForce = reflexTorque = frictionForce = frictionTorque = Vec3f();
	area = 0;

	solid[0]->UpdateCache(ct);
	solid[1]->UpdateCache(ct);
	cocog = ave(solid[0]->cog, solid[1]->cog);

	PHSolid* s[2] = {solid[0]->solid, solid[1]->solid};

	//	換算質量の計算
	convertedMass=1.0f;
	if (s[0]->GetMass() < 1e10f && s[1]->GetMass() < 1e10f){
		float m0 = (float)s[0]->GetMass();
		float m1 = (float)s[1]->GetMass();
		convertedMass = m0 * m1 / (m0+m1);
	}else if (s[0]->GetMass() < 1e10f){
		convertedMass = (float)s[0]->GetMass();
	}else if (s[1]->GetMass() < 1e10f){
		convertedMass = (float)s[1]->GetMass();
	}
}

void PHSolidPairForPenalty::OnDetect(PHShapePairForPenalty* sp, PHPenaltyEngine* engine, unsigned ct, double dt){
	//contacts.push_back(sp);
	static CDContactAnalysis analyzer;
	analyzer.FindIntersection(sp);	//	接触形状の解析
	analyzer.CalcNormal(sp);		//	法線ベクトルの計算

	//	接触力計算の準備
	float rs[2], rd[2], fs[2], fd[2], sf[2], df[2];
	for(int i=0; i<2; ++i){
		rs[i] = fs[i] = SPRING;
		rd[i] = fd[i] = DAMPER;
		sf[i] = sp->shape[i]->material.mu0;
		df[i] = sp->shape[i]->material.mu;
	}
	reflexSpring    = ave(rs[0], rs[1]) * convertedMass / (float)(2*dt*dt);
	reflexDamper    = ave(rd[0], rd[1]) * convertedMass / (float)(dt);
	frictionSpring  = ave(fs[0], fs[1]) * convertedMass / (float)(2*dt*dt);
	frictionDamper  = ave(fd[0], fd[1]) * convertedMass / (float)(dt);
	staticFriction  = ave(sf[0], sf[1]);
	dynamicFriction = ave(df[0], df[1]);
	
	//	接触力計算	衝突の面積，抗力を求める
	CalcReflexForce(sp, &analyzer);
	area += sp->area;
}

void PHSolidPairForPenalty::GenerateForce(){
	//	接触判定終了後の処理
	//	抗力とその作用点を求め，摩擦を計算し，抗力と摩擦力を物体に加える．
	int i, j;
	PHShapePairForPenalty* cp;
	for(i = 0; i < shapePairs.height(); i++)for(j = 0; j < shapePairs.width(); j++){
		cp = shapePairs.item(i, j);
		if(!cp->bActive)continue;

		//	積分したペナルティと速度を面積で割る
		cp->reflexSpringForce /= area;
		cp->reflexDamperForce /= area;
		cp->reflexSpringTorque /= area;
		cp->reflexDamperTorque /= area;
		cp->dynaFric /= area;
		cp->dynaFricMom /= area;
				
		DEBUG_EVAL(
			if ( !finite( cp->reflexSpringForce.norm() )
				|| !finite( cp->frictionForce.norm() )
				|| !finite( cp->frictionTorque.norm() ) ){
				DSTR << "Error: forces: " << cp->reflexSpringForce << cp->frictionForce << cp->frictionTorque << std::endl;
			}
		)
		//	摩擦力を計算する
		CalcFriction(cp);
		//	力を加える．
		Vec3f refF = cp->reflexSpringForce + cp->reflexDamperForce;
		Vec3f refT = cp->reflexSpringTorque + cp->reflexDamperTorque
			+ ((cp->commonPoint - cocog)^refF);

		reflexForce += refF;
		reflexTorque += refT;
		frictionForce += cp->frictionForce;
		frictionTorque += cp->frictionTorque
			+ (((cp->reflexForcePoint+cp->commonPoint) - cocog) ^ cp->frictionForce);

		//	力を制限する．
		//	LimitForces();
		// 力を加える．
		solid[0]->solid->AddForce(reflexForce + frictionForce, cocog);
		solid[0]->solid->AddTorque(reflexTorque + frictionTorque);
		solid[1]->solid->AddForce(-(reflexForce + frictionForce), cocog);
		solid[1]->solid->AddTorque(-(reflexTorque + frictionTorque));
	}
}

//	凸形状対に発生する反力の計算と最大摩擦力の計算
//	すべて commonPoint を原点とした座標系で計算する．
void PHSolidPairForPenalty::CalcReflexForce(PHShapePairForPenalty* cp, CDContactAnalysis* analyzer){
	cp->Clear();
	Vec3f cog[2] = {solid[0]->cog - cp->commonPoint, solid[1]->cog - cp->commonPoint};
	CDConvexMesh* cmesh[2] = {
		(CDConvexMesh*)cp->shape[0],
		(CDConvexMesh*)cp->shape[1]
	};
	if (bUseContactVolume){	//	通常 true
		//for(CDContactAnalysis::Vtxs::iterator it = analyzer->vtxs.begin(); it != analyzer->planes.vtxBegin; ++it){
		for (CDContactAnalysisFace** it = &*analyzer->vtxs.begin(); it != analyzer->planes.vtxBegin; ++it){	// これでいいの？
			CDContactAnalysisFace& qhVtx = **it;
			if (qhVtx.NCommonVtx() < 3) continue;
			int curID = qhVtx.id;
			Vec3f p0 = qhVtx.CommonVtx(0);
			Vec3f p1;
			Vec3f p2 = qhVtx.CommonVtx(1);
			for(unsigned i=2; i<qhVtx.NCommonVtx(); ++i){
				p1 = p2;
				p2 = qhVtx.CommonVtx(i);
				CalcTriangleReflexForce(cp, p0, p1, p2, cog[curID], solid[curID]->vel, solid[curID]->angVel);
#if 0				//	hase
				if (cp->reflexSpringForce.norm() > 10000 || !_finite(cp->reflexSpringForce.norm()) ){
					DSTR << "CalcTriangleReflexForce returned very large force." << std::endl;
					CalcTriangleReflexForce(cp, cp, p0, p1, p2, cog[curID], fr[curID]->vel, fr[curID]->angVel);
				}
#endif
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
		if (cp->reflexSpringForce.square() > 1e-8){
			cp->reflexForcePoint = (cp->reflexSpringForce ^ cp->reflexSpringTorque) / (cp->reflexSpringForce*cp->reflexSpringForce);
			cp->reflexForcePoint -= cp->reflexForcePoint * cp->normal * cp->normal;
		}else{
			cp->reflexForcePoint = cp->center - cp->commonPoint;
		}
	}else{	//	bUseContactVolume
		Vec3d closest[2] = {
			solid[0]->ori * cp->closestPoint[0] + solid[0]->pos,
			solid[1]->ori * cp->closestPoint[1] + solid[1]->pos};
		cp->reflexSpringForce = reflexSpring * (closest[1] - closest[0]);
		Vec3f colPos = (closest[0]+closest[1])/2;
		cp->reflexSpringTorque = (colPos-cp->commonPoint) ^ cp->reflexSpringForce;
		Vec3f vel[2];
		vel[0] = solid[0]->vel + (solid[0]->angVel ^ (colPos - solid[0]->cog));
		vel[1] = solid[1]->vel + (solid[1]->angVel ^ (colPos - solid[1]->cog));
		Vec3f rvel = vel[1] - vel[0];
		cp->reflexDamperForce = reflexDamper * rvel;
		cp->reflexDamperTorque = (colPos-cp->commonPoint) ^ cp->reflexDamperForce;
		cp->area = 10.0f;
	}
#if 0	//	hase
	if (!_finite(cp->reflexForcePoint.norm()) || !_finite(cp->reflexSpringForce.norm()) || cp->reflexForcePoint.norm() > 100){
		DSTR << "reflexForcePoint:" << cp->reflexForcePoint << std::endl;
		DSTR << "reflexSpringForce:"  << cp->reflexSpringForce << std::endl;
		DSTR << "reflexTorque:"  << cp->reflexSpringTorque << std::endl;
		DSTR << "commonPoint:"  << cp->commonPoint << std::endl;
	}
	if (cp->area<=0){
		DSTR << "Area:" << cp->area << std::endl;
	}
	if (cp->reflexSpringForce.norm() > 10000 || !_finite(cp->reflexSpringForce.norm()) ){
		int i=0;
		DSTR << "Error: Too large reflexForce: " << cp->reflexSpringForce << std::endl;
		DSTR << "solidName:" << solid[0].solid->GetName() << " - " << solid[1].solid->GetName() << "  ";
		DSTR << "commonPoint:" << cp->commonPoint << std::endl;
		for(CDContactAnalysis::Vtxs::iterator it = analyzer->vtxs.begin(); it != analyzer->planes.vtxBegin; ++it){
			CDFace& face = **it;
			DSTR << "p" << i++ << "\t d:" << face.dist << " n:" << face.normal << std::endl;
		}
		i = 1;
		for(CDQHPlane<CDFace>* it = analyzer->planes.begin; it != analyzer->planes.end; ++it){
			if (it->deleted) continue;
			DSTR << "v" << i++ << "\t " << it->normal/it->dist << std::endl;
		}
	}
#endif
}

void PHSolidPairForPenalty::CalcTriangleReflexForce(PHShapePairForPenalty* cp, Vec3f p0, Vec3f p1, Vec3f p2, Vec3f cog, Vec3f vel, Vec3f angVel){
	//---------------------------------------------------------------
	//	ばねモデルの計算：各頂点の侵入深さの計算
	float depth0 = p0 * cp->normal;
 	float depth1 = p1 * cp->normal;
 	float depth2 = p2 * cp->normal;
	//	ダンパモデルの計算：各頂点の速度とその法線方向成分を求める
	angVel *= 1;	/*	角速度をダンパの計算の前に増やす．
						ここで角速度のダンパを強くすると安定するけど，
						転がらなくなる．
						抗力は出しても，引力は出さないような工夫をすれば
						安定でかつ転がるようにできるのではないかと思う．
						by hase
					*/
	p0 -= depth0 * cp->normal;
	p1 -= depth1 * cp->normal;
	p2 -= depth2 * cp->normal;
	Vec3f vel0 = (angVel^(p0-cog)) + vel;
	Vec3f vel1 = (angVel^(p1-cog)) + vel;
	Vec3f vel2 = (angVel^(p2-cog)) + vel;
	float vel0_normal = vel0 * cp->normal;
	float vel1_normal = vel1 * cp->normal;
	float vel2_normal = vel2 * cp->normal;

	//	抗力を計算
	float refSp0 = reflexSpring * depth0;
	float refSp1 = reflexSpring * depth1;
	float refSp2 = reflexSpring * depth2;

	float refDa0 = reflexDamper * vel0_normal;
	float refDa1 = reflexDamper * vel1_normal;
	float refDa2 = reflexDamper * vel2_normal;

	//---------------------------------------------------------------
	//	三角形について積分
	Vec3f a = p1 - p0;
	Vec3f b = p2 - p0;
	Vec3f a_b = a^b;
	float a_b_normal = a_b * cp->normal;
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
	if (refSp0 > 10000 || refSp1 > 10000 || refSp2 > 10000 || !finite(triRefSp.norm()) ){
		DSTR << "Error: The reflection spring force is too large: " << triRefSp << std::endl;
	}
#endif
	// 数値演算範囲エラーのチェック
	//  与えられた倍精度の浮動小数点値が有限であるかどうか
	assert(finite(triRefMomSp.norm()));	

	//	3角形の面積の計算
	float triArea = a_b_normal / 4;
	if (triArea < 0) triArea = -triArea;

	//---------------------------------------------------------------
	//	動摩擦力を求める
	Vec3f velTan0 = vel0 - vel0_normal * cp->normal;
	Vec3f velTan1 = vel1 - vel1_normal * cp->normal;
	Vec3f velTan2 = vel2 - vel2_normal * cp->normal;
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
	assert(finite(triFric.norm()));
	assert(finite(triFricMom.norm()));
	//---------------------------------------------------------------
	//	結果を加える
	cp->area += triArea;
	cp->reflexSpringForce += triRefSp;
	cp->reflexSpringTorque += triRefMomSp;
	cp->reflexDamperForce += triRefDa;
	cp->reflexDamperTorque += triRefMomDa;
	cp->dynaFric += triFric;
	cp->dynaFricMom += triFricMom;
}

//	凸形状対に発生する摩擦力の計算
//	力の作用点を原点とした座標系で計算する．
void PHSolidPairForPenalty::CalcFriction(PHShapePairForPenalty* cp){
	//	初めての接触の時
	Vec3f reflexForcePoint = cp->reflexForcePoint + cp->commonPoint;	//	力の作用点(絶対系)
	if (cp->state == PHShapePairForPenalty::NEW){
		//	バネモデルの始点を設定する．
		cp->transFrictionBase[0] = solid[0]->ori.Inv() * (reflexForcePoint - solid[0]->pos);
		cp->transFrictionBase[1] = solid[1]->ori.Inv() * (reflexForcePoint - solid[1]->pos);
		cp->rotSpring = 0;
		cp->frictionState = PHShapePairForPenalty::STATIC;
		cp->frictionForce = Vec3f();
		cp->frictionTorque = Vec3f();
		return;
	}

	//	接触が継続している場合．
	
	//	並進バネの計算
	//	グローバル系に変換
	cp->transFrictionBase[0] = solid[0]->ori * cp->transFrictionBase[0] + solid[0]->pos;
	cp->transFrictionBase[1] = solid[1]->ori * cp->transFrictionBase[1] + solid[1]->pos;
	//	平面上に落とす
	cp->transFrictionBase[0] -= (cp->transFrictionBase[0]-cp->center) * cp->normal * cp->normal;
	cp->transFrictionBase[1] -= (cp->transFrictionBase[1]-cp->center) * cp->normal * cp->normal;
	//	差がバネの力
	Vec3f transSpring = cp->transFrictionBase[1] - cp->transFrictionBase[0];
	float transSpringNorm = transSpring.norm();
	float frictionSpringForce = frictionSpring * transSpringNorm;
	Vec3f frictionForceDicption;
	if (transSpringNorm>1e-10f) frictionForceDicption = transSpring / transSpringNorm;

	//	回転バネの計算
	Quaternionf delta[2] = { solid[0]->ori*solid[0]->lastOri.Inv(), solid[1]->ori*solid[1]->lastOri.Inv() };
	cp->rotSpring += delta[1].Rotation()*cp->normal - delta[0].Rotation()*cp->normal;
	float frictionSpringTorque = frictionSpring*cp->rotSpring;


	//	動摩擦の制約を加える
	float fricCoeff = (cp->frictionState == PHShapePairForPenalty::STATIC) ? staticFriction : dynamicFriction;
	float maxFric = fricCoeff * cp->dynaFric.norm();

	//hase	摩擦のテスト中
//	float reflexForce = (cp->reflexSpringForce+cp->reflexDamperForce).norm();
//	if (maxFric < fricCoeff * reflexForce) maxFric = fricCoeff * reflexForce;
	//	ここまで
		
		
	float maxRotFric = fricCoeff * (cp->dynaFricMom - (cp->reflexForcePoint^cp->dynaFric)).norm();
	assert(finite(maxFric));
	DEBUG_EVAL( if(!finite(maxRotFric)){ DSTR << "FricMax:" << maxFric << "  " << maxRotFric << std::endl; } )

	cp->frictionState = PHShapePairForPenalty::STATIC;
	if (frictionSpringForce > maxFric){
		frictionSpringForce = maxFric;
		cp->frictionState = PHShapePairForPenalty::DYNAMIC;
	}
	if (abs(frictionSpringTorque) > maxRotFric){
		frictionSpringTorque = sign(frictionSpringTorque) * maxRotFric;
		cp->frictionState = PHShapePairForPenalty::DYNAMIC;
	}
	cp->frictionForce = frictionSpringForce * frictionForceDicption;
	cp->frictionTorque = frictionSpringTorque * cp->normal;

	if (cp->frictionState == PHShapePairForPenalty::STATIC){
		///	ダンパによる静止摩擦力の計算
		Vec3f frictionVel = ((solid[1]->angVel ^ (reflexForcePoint-solid[1]->cog)) + solid[1]->vel) - ((solid[0]->angVel ^ (reflexForcePoint-solid[0]->cog)) + solid[0]->vel);
		frictionVel -= frictionVel * cp->normal * cp->normal;
		cp->frictionForce += frictionDamper * frictionVel;
		
		Vec3f frictionAngVel = solid[1]->angVel - solid[0]->angVel;
		DEBUG_EVAL( if (!finite(frictionAngVel.norm())){ DSTR << "frictionAngVel: " << frictionAngVel << std::endl; } )
		

		frictionAngVel = (frictionAngVel * cp->normal) * cp->normal;
		cp->frictionTorque += frictionDamper * frictionAngVel;

		if (cp->frictionForce.norm() > maxFric){
			if (cp->frictionForce.norm() > 1e-10f) cp->frictionForce = cp->frictionForce.unit() * maxFric;
			else cp->frictionForce = Vec3f();
		}

		if (cp->frictionTorque.norm() > maxRotFric){
			if(cp->frictionTorque.norm() > 1e-10f) cp->frictionTorque = cp->frictionTorque.unit() * maxRotFric;
			else cp->frictionTorque = Vec3f();
		}
	}
#if 0
	if (cp->frictionTorque.norm() > 100){
		DSTR << "FT:" << cp->frictionTorque << area << std::endl;
		DSTR << "rotFricSpring:" << cp->rotSpring;
		DSTR << std::endl;
	}
#endif

	//	ローカル系に変換して保存
	if (frictionSpring < 1e-12f){	//	摩擦のばね係数が0だと、伸びが計算できなくなる。
		frictionSpring = 1e12f;		//	係数0の場合伸びは無視できるので、伸びを小さな値にしておく。
	}
	cp->transFrictionBase[0] = solid[0]->ori.Inv() * (reflexForcePoint - 0.5f*frictionSpringForce/frictionSpring*frictionForceDicption - solid[0]->pos);
	cp->transFrictionBase[1] = solid[1]->ori.Inv() * (reflexForcePoint + 0.5f*frictionSpringForce/frictionSpring*frictionForceDicption - solid[1]->pos);
	cp->rotSpring = frictionSpringTorque / frictionSpring;
}

//----------------------------------------------------------------------------
OBJECT_IMP(PHPenaltyEngine, PHEngine);

void PHPenaltyEngine::Step(){
	//if(!ready)
	//	Init();
	
	PHScene* scene = DCAST(PHScene, GetScene());
	unsigned int ct = scene->GetCount();
	double dt = scene->GetTimeStep();
	int n = solids.size();
	int i, j;
	for(i = 0; i < n; i++)for(j = i+1; j < n; j++)
		solidPairs.item(i,j)->Setup(ct, dt);

	Detect(ct, dt);

	for(i = 0; i < n; i++)for(j = i+1; j < n; j++)
		solidPairs.item(i,j)->GenerateForce();

}

}
