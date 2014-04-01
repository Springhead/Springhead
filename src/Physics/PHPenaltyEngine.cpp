/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHPenaltyEngine.h>
#include <float.h>
#include <Collision/CDDetectorImp.h>
#include <Base/Affine.h>

using namespace PTM;
namespace Spr{;
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
// PHSolidPairForPenalty

void PHSolidPairForPenalty::Setup(unsigned int ct, double dt){
	if(!bEnabled)return;
	//	動力学計算の準備
	reflexForce = reflexTorque = frictionForce = frictionTorque = Vec3f();
	area = 0;

	cocog = ave(solid[0]->GetCenterPosition(), solid[1]->GetCenterPosition());

	//	換算質量の計算
	convertedMass=1.0f;
	if (solid[0]->GetMass() < 1e10f && solid[1]->GetMass() < 1e10f){
		float m0 = (float)solid[0]->GetMass();
		float m1 = (float)solid[1]->GetMass();
		convertedMass = m0 * m1 / (m0+m1);
	}else if (solid[0]->GetMass() < 1e10f){
		convertedMass = (float)solid[0]->GetMass();
	}else if (solid[1]->GetMass() < 1e10f){
		convertedMass = (float)solid[1]->GetMass();
	}
}

void PHSolidPairForPenalty::OnDetect(PHShapePair* _sp, unsigned ct, double dt){
	PHShapePairForPenalty* sp = (PHShapePairForPenalty*)_sp;

	//contacts.push_back(sp);
	static CDContactAnalysis analyzer;
	analyzer.FindIntersection(sp);	//	接触形状の解析
	analyzer.CalcNormal      (sp);	//	法線ベクトルの計算

	//	接触力計算の準備
	float rs[2], rd[2], fs[2], fd[2], sf[2], df[2];
	for(int i=0; i<2; ++i){
		rs[i] = sp->shape[i]->GetMaterial().reflexSpring;
		rd[i] = sp->shape[i]->GetMaterial().reflexDamper;
		fs[i] = sp->shape[i]->GetMaterial().frictionSpring;
		fd[i] = sp->shape[i]->GetMaterial().frictionDamper;
		sf[i] = sp->shape[i]->GetMaterial().mu0;
		df[i] = sp->shape[i]->GetMaterial().mu;
	}

#if 0
	//	2010.07.30 バネ・ダンパを半分にしました。
	reflexSpring    = ave(rs[0], rs[1]) * convertedMass / (float)(4*dt*dt);
	reflexDamper    = ave(rd[0], rd[1]) * convertedMass / (float)(2*dt);
	frictionSpring  = ave(fs[0], fs[1]) * convertedMass / (float)(4*dt*dt);
	frictionDamper  = ave(fd[0], fd[1]) * convertedMass / (float)(2*dt);
#else	
	// 2011.12.12 susa
	// http://springhead.info/control.pdf の記述だと下記のようになる
	// また，ave(rs[0], rs[1])=0.2，ave(rd[0], rd[1])=0.6
	// reflexSpring = 2000, reflexDamper = 3, convertedMass = 5.0e-3
	// となるように設定すると，それっぽい挙動が得られる．
	reflexSpring    = 2 * ave(rs[0], rs[1]) * convertedMass / (float)(dt*dt);
	reflexDamper    = ave(rd[0], rd[1]) * convertedMass / (float)dt;	
	frictionSpring  = 2 * ave(fs[0], fs[1]) * convertedMass / (float)(dt*dt);
	frictionDamper  = ave(fd[0], fd[1]) * convertedMass / (float)dt;
	//DSTR << reflexSpring << reflexDamper << std::endl;
#endif
	// ifに対しての処理が書かれていない．このままだとstaticFrictionが設定されない
	//if (reflexSpring > convertedMass / (float)(2*dt*dt)) 


	staticFriction  = ave(sf[0], sf[1]);
	dynamicFriction = ave(df[0], df[1]);
	
	//	接触力計算	衝突の面積，抗力を求める
	CalcReflexForce(sp, &analyzer);
	area += sp->area;
}

void PHSolidPairForPenalty::GenerateForce(){
	if(!bEnabled)return;
	//	接触判定終了後の処理
	//	抗力とその作用点を求め，摩擦を計算し，抗力と摩擦力を物体に加える．
	int i, j;
	PHShapePairForPenalty* cp;
	for(i = 0; i < shapePairs.height(); i++)for(j = 0; j < shapePairs.width(); j++){
		cp = GetShapePair(i, j);
		if(cp->state == CDShapePair::NONE) continue;
		if (!area) continue;

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
		solid[0]->AddForce(reflexForce + frictionForce, cocog);
		solid[0]->AddTorque(reflexTorque + frictionTorque);
		solid[1]->AddForce(-(reflexForce + frictionForce), cocog);
		solid[1]->AddTorque(-(reflexTorque + frictionTorque));
	}
}

//	凸形状対に発生する反力の計算と最大摩擦力の計算
//	すべて commonPoint を原点とした座標系で計算する．
void PHSolidPairForPenalty::CalcReflexForce(PHShapePairForPenalty* cp, CDContactAnalysis* analyzer){
	//DSTR << "---------------------------------------------------------" << std::endl;
	cp->Clear();
	Vec3f cog[2] = {solid[0]->GetCenterPosition() - cp->commonPoint, solid[1]->GetCenterPosition() - cp->commonPoint};
	/*CDConvexMesh* cmesh[2] = {
		(CDConvexMesh*)cp->shape[0],
		(CDConvexMesh*)cp->shape[1]
	};*/
	if (bUseContactVolume){	//	通常 true
		//for(CDContactAnalysis::Vtxs::iterator it = analyzer->vtxs.begin(); it != analyzer->planes.vtxBegin; ++it){
		for (CDContactAnalysisFace** it = &*analyzer->vtxs.begin(); it != analyzer->planes.vtxBegin; ++it){	// これでいいの？
			CDContactAnalysisFace& qhVtx = **it;
			if (qhVtx.NCommonVtx() < 3) continue;
			Vec3f p0 = qhVtx.CommonVtx(0);
			Vec3f p1;
			Vec3f p2 = qhVtx.CommonVtx(1);
			Vec3f v0 = solid[1]->velocity + (solid[1]->angVelocity^(p0-cog[1]))
					 - solid[0]->velocity - (solid[0]->angVelocity^(p0-cog[0]));
			Vec3f v1;
			Vec3f v2 = solid[1]->velocity + (solid[1]->angVelocity^(p2-cog[1]))
					 - solid[0]->velocity - (solid[0]->angVelocity^(p2-cog[0]));
			for(unsigned i=2; i<qhVtx.NCommonVtx(); ++i){
				p1 = p2;	v1 = v2;
				p2 = qhVtx.CommonVtx(i);
				v2 = solid[1]->velocity + (solid[1]->angVelocity^(p2-cog[1]))
				   - solid[0]->velocity - (solid[0]->angVelocity^(p2-cog[0]));
				//	双対変換の原点＝GJKで見つけた共通点を原点として、三角形の３頂点を渡す
				CalcTriangleReflexForce(cp, p0, p1, p2, v0, v1, v2, qhVtx.id==1);
#if 0				//	hase
				if (cp->reflexSpringForce.norm() > 10000 || !finite(cp->reflexSpringForce.norm()) ){
					DSTR << "CalcTriangleReflexForce returned very large force." << std::endl;
					CalcTriangleReflexForce(cp, p0, p1, p2, sign*v0, sign*v1, sign*v2);
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
			solid[0]->pose * cp->closestPoint[0],
			solid[1]->pose * cp->closestPoint[1]};
		cp->reflexSpringForce = reflexSpring * (closest[1] - closest[0]);
		Vec3f colPos = (closest[0]+closest[1])/2;
		cp->reflexSpringTorque = (colPos-cp->commonPoint) ^ cp->reflexSpringForce;
		Vec3f vel[2];
		vel[0] = solid[0]->velocity + (solid[0]->angVelocity ^ (colPos - solid[0]->GetCenterPosition()));
		vel[1] = solid[1]->velocity + (solid[1]->angVelocity ^ (colPos - solid[1]->GetCenterPosition()));
		Vec3f rvel = vel[1] - vel[0];
		cp->reflexDamperForce = reflexDamper * rvel;
		cp->reflexDamperTorque = (colPos-cp->commonPoint) ^ cp->reflexDamperForce;
		cp->area = 10.0f;
	}
#if 0	//	hase
	if (!finite(cp->reflexForcePoint.norm()) || !_finite(cp->reflexSpringForce.norm()) || cp->reflexForcePoint.norm() > 100){
		DSTR << "reflexForcePoint:" << cp->reflexForcePoint << std::endl;
		DSTR << "reflexSpringForce:"  << cp->reflexSpringForce << std::endl;
		DSTR << "reflexTorque:"  << cp->reflexSpringTorque << std::endl;
		DSTR << "commonPoint:"  << cp->commonPoint << std::endl;
	}
	if (cp->area<=0){
		DSTR << "Area:" << cp->area << std::endl;
	}
	if (cp->reflexSpringForce.norm() > 10000 || !finite(cp->reflexSpringForce.norm()) ){
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

//	この三角形が物体0に与える力を求める。
void PHSolidPairForPenalty::CalcTriangleReflexForce(PHShapePairForPenalty* cp, Vec3f p0, Vec3f p1, Vec3f p2, Vec3f v0, Vec3f v1, Vec3f v2, bool bFront){
	//	p0..p2 三角形の３頂点。原点はGJKの共通点＝双対変換の原点
	//	v0..v2 ３頂点での物体0～1を見たときの相対速度  （絶対速度ではだめ　→　相対速度の向きに動摩擦力が生じるので摩擦で困る）
	//	bFront	物体1の面なら true
	//---------------------------------------------------------------
	//	ばねモデルの計算：各頂点の侵入深さの計算
	float depth0 = p0 * cp->normal;	//	normal は物体0から1の向き。物体0が法線向きにずれるとより侵入して反力が増える。
 	float depth1 = p1 * cp->normal;	//	物体1が法線向きにずれると反力が減るが、a_b_normalの符号も負になるので符号が合う。
 	float depth2 = p2 * cp->normal;
	p0 -= depth0 * cp->normal;
	p1 -= depth1 * cp->normal;
	p2 -= depth2 * cp->normal;
	//	ダンパモデルの計算：各頂点の速度の法線方向成分を求める
	//	物体0が法線向きに速度を持つと反力が増える。
	float vel0_normal = v0 * cp->normal;
	float vel1_normal = v1 * cp->normal;
	float vel2_normal = v2 * cp->normal;

	//	抗力を計算
	float refSp0 = reflexSpring * depth0;
	float refSp1 = reflexSpring * depth1;
	float refSp2 = reflexSpring * depth2;
	
	float refDa0 = reflexDamper * vel0_normal * 0.5;	//	裏表で2回数えてしまうので、半分にしておく
	float refDa1 = reflexDamper * vel1_normal * 0.5;
	float refDa2 = reflexDamper * vel2_normal * 0.5;

	//---------------------------------------------------------------
	//	三角形について積分
	Vec3f a = p1 - p0;
	Vec3f b = p2 - p0;
	Vec3f a_b = a^b;						//	物体1だと裏を向く。
	float a_b_normal = a_b * cp->normal;	//	物体1だと負。
	Vec3f triRefSp = (1.0f/6.0f) * (refSp0 + refSp1 + refSp2) * a_b;
	Vec3f triRefMomSp = (
				((1.0f/12.0f)*refSp0 + (1.0f/24.0f)*refSp1 + (1.0f/24.0f)*refSp2) * p0
			+	((1.0f/24.0f)*refSp0 + (1.0f/12.0f)*refSp1 + (1.0f/24.0f)*refSp2) * p1
			+	((1.0f/24.0f)*refSp0 + (1.0f/24.0f)*refSp1 + (1.0f/12.0f)*refSp2) * p2
		  ) ^ a_b;
	float sign_a_b_normal = a_b_normal > 0 ? 1 : -1;
	Vec3f abs_a_b = sign_a_b_normal * a_b;
	Vec3f triRefDa = (1.0f/6.0f) * (refDa0 + refDa1 + refDa2) * abs_a_b;
	Vec3f triRefMomDa = (
				((1.0f/12.0f)*refDa0 + (1.0f/24.0f)*refDa1 + (1.0f/24.0f)*refDa2) * p0
			+	((1.0f/24.0f)*refDa0 + (1.0f/12.0f)*refDa1 + (1.0f/24.0f)*refDa2) * p1
			+	((1.0f/24.0f)*refDa0 + (1.0f/24.0f)*refDa1 + (1.0f/12.0f)*refDa2) * p2
		  ) ^ abs_a_b;
	//	triXXX は裏表に関係なく + になる
#ifdef _DEBUG
	if (refSp0 > 10000 || refSp1 > 10000 || refSp2 > 10000 || !finite(triRefSp.norm()) ){
		DSTR << "Error: The reflection spring force is too large: " 
			<< refSp0 << " " << refSp1 << " " << refSp2 << " " << triRefSp << std::endl;
	}
#endif
	// 数値演算範囲エラーのチェック
	//  与えられた倍精度の浮動小数点値が有限であるかどうか
	assert(finite(triRefMomSp.norm()));	

	//	3角形の面積の計算
	float triArea = sign_a_b_normal * a_b_normal / 4;		//	裏表で２回数えるから4で割る

	//---------------------------------------------------------------
	//	動摩擦力を求める
	//	動摩擦は、相対速度の方向*抗力。裏表共に数える。抗力が裏の分、表の分だけになっているので、２重に数える心配はない。
	Vec3f velTan0 = v0 - vel0_normal * cp->normal;
	Vec3f velTan1 = v1 - vel1_normal * cp->normal;
	Vec3f velTan2 = v2 - vel2_normal * cp->normal;
	Vec3f fric0, fric1, fric2;
	//	摩擦の計算
	if (velTan0.square() > 1e-8) fric0 = velTan0.unit() * (refSp0+sign_a_b_normal*refDa0);
	if (velTan1.square() > 1e-8) fric1 = velTan1.unit() * (refSp1+sign_a_b_normal*refDa1);
	if (velTan2.square() > 1e-8) fric2 = velTan2.unit() * (refSp2+sign_a_b_normal*refDa2);
	//	符号について：裏面だと、refSpr?+refDa? が - になるが a_b_normalも-になるので打ち消す。

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

	if (abs(a_b_normal) > 1.0){
		//DSTR << "a_b_normal:" << a_b_normal << " f:" << triFric << "  s:" << triRefSp.y << "  d:" << triRefDa.y << std::endl;
	}
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
		cp->transFrictionBase[0] = solid[0]->pose.Inv() * reflexForcePoint;
		cp->transFrictionBase[1] = solid[1]->pose.Inv() * reflexForcePoint;
		cp->rotSpring = 0;
		cp->frictionState = PHShapePairForPenalty::STATIC;
		cp->frictionForce = Vec3f();
		cp->frictionTorque = Vec3f();
		return;
	}

	//	接触が継続している場合．
	
	//	並進バネの計算
	//	グローバル系に変換
	cp->transFrictionBase[0] = solid[0]->pose * cp->transFrictionBase[0];
	cp->transFrictionBase[1] = solid[1]->pose * cp->transFrictionBase[1];
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
	Quaternionf delta[2] = { solid[0]->pose.Ori()*lastOri[0].Inv(), solid[1]->pose.Ori()*lastOri[1].Inv() };
	cp->rotSpring += delta[1].Rotation()*cp->normal - delta[0].Rotation()*cp->normal;
	float frictionSpringTorque = frictionSpring*cp->rotSpring;
	lastOri[0] = solid[0]->pose.Ori();
	lastOri[1] = solid[1]->pose.Ori();


	//	摩擦係数の制約を加える
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
		Vec3f frictionVel = 
			(solid[1]->angVelocity ^ (reflexForcePoint-solid[1]->GetCenterPosition())) + solid[1]->velocity
			- ((solid[0]->angVelocity ^ (reflexForcePoint-solid[0]->GetCenterPosition())) + solid[0]->velocity);
		frictionVel -= frictionVel * cp->normal * cp->normal;
		cp->frictionForce += frictionDamper * frictionVel;
		
		Vec3f frictionAngVel = solid[1]->angVelocity - solid[0]->angVelocity;
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
		frictionSpring = 1e-12f;		//	係数0の場合伸びは無視できるので、伸びを小さな値にしておく。
	}
	cp->transFrictionBase[0] = solid[0]->pose.Inv() * (reflexForcePoint - 0.5f*frictionSpringForce/frictionSpring*frictionForceDicption);
	cp->transFrictionBase[1] = solid[1]->pose.Inv() * (reflexForcePoint + 0.5f*frictionSpringForce/frictionSpring*frictionForceDicption);
	cp->rotSpring = frictionSpringTorque / frictionSpring;
}

//----------------------------------------------------------------------------
void PHPenaltyEngine::Step(){
	PHScene* scene = DCAST(PHScene, GetScene());
	
	if(!scene->IsContactDetectionEnabled())
		return;
	
	unsigned int ct = scene->GetCount();
	double       dt = scene->GetTimeStep();
	int n = (int)solids.size();
	int i, j;
	for(i = 0; i < n; i++)for(j = i+1; j < n; j++)
		GetSolidPair(i,j)->Setup(ct, dt);

	Detect(ct, dt, scene->IsCCDEnabled());

	for(i = 0; i < n; i++)for(j = i+1; j < n; j++)
		GetSolidPair(i,j)->GenerateForce();

}

}
