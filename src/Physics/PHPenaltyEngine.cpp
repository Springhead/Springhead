#include "Physics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <float.h>
#include <Collision/CDDetectorImp.h>


using namespace PTM;
namespace Spr{;
const float SPRING = 0.2f;
const float DAMPER = 0.3f;
extern bool bUseContactVolume;

void PHPenaltyEngine::PHShapePair::Clear(){
	area = 0;
	reflexSpringForce.clear();
	reflexDamperForce.clear();
	reflexSpringTorque.clear();
	reflexDamperTorque.clear();
	dynaFric.clear();
	dynaFricMom.clear();
}

//----------------------------------------------------------------------------
void PHPenaltyEngine::PHSolidInfo::UpdateCache(int c){
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

inline float Ave(float a, float b){
//	float c = 1/(1/a + 1/b) ;
//	return c*2;
	return (a+b)/2.0f;
}

void PHPenaltyEngine::PHSolidPair::Clear(){
	reflexForce = reflexTorque = frictionForce = frictionTorque = Vec3f();
	area = 0;
}

static float reflexSpring;
static float reflexDamper;
static float frictionSpring;
static float frictionDamper;
static float staticFriction;
static float dynamicFriction;

//	衝突検出と接触力の計算
bool PHPenaltyEngine::PHSolidPair::Detect(PHPenaltyEngine* engine){
	bool rv = false;
	int ct = DCAST(PHScene,engine->GetScene())->GetCount();
	double dt = DCAST(PHScene,engine->GetScene())->GetTimeStep();

	//	動力学計算の準備
	Clear();
	solid[0].UpdateCache(ct);
	solid[1].UpdateCache(ct);
	cocog = 0.5f * (solid[0].cog+solid[1].cog);

	//	換算質量の計算
	float convertedMass=1.0f;
	if (solid[0].solid->GetMass() < 1e10f && solid[1].solid->GetMass() < 1e10f){
		float m0 = (float)solid[0].solid->GetMass();
		float m1 = (float)solid[1].solid->GetMass();
		convertedMass = m0 * m1 / (m0+m1);
	}else if (solid[0].solid->GetMass() < 1e10f){
		convertedMass = (float)solid[0].solid->GetMass();
	}else if (solid[1].solid->GetMass() < 1e10f){
		convertedMass = (float)solid[1].solid->GetMass();
	}

	//	Shape同士の接触判定
	typedef std::vector<PHShapePair*> Contacts;
	Contacts contacts;
	for(int i=0; i<shapePairs.height(); ++i){
		for(int j=0; j<shapePairs.width(); ++j){
			PHShapePair* sp = shapePairs.item(i,j);
			//shapePair->UpdateShapePose(solid[0].solid->GetPose(), solid[1].solid->GetPose());
			if ( sp->Detect(ct) ){
				rv = true;

				contacts.push_back(sp);
				static CDContactAnalysis analyzer;
				analyzer.FindIntersection(sp);	//	接触形状の解析
				analyzer.CalcNormal(sp);			//	法線ベクトルの計算

				//	接触力計算の準備
				float rs[2], rd[2], fs[2], fd[2], sf[2], df[2];
				for(int i=0; i<2; ++i){
					rs[i] = fs[i] = SPRING;
					rd[i] = fd[i] = DAMPER;
					sf[i] = sp->shape[i]->shape->material.mu0;
					df[i] = sp->shape[i]->shape->material.mu;
				}
				reflexSpring = Ave(rs[0], rs[1]) * convertedMass / (float)(2*dt*dt);
				reflexDamper = Ave(rd[0], rd[1]) * convertedMass / (float)(dt);
				frictionSpring = Ave(fs[0], fs[1]) * convertedMass / (float)(2*dt*dt);
				frictionDamper = Ave(fd[0], fd[1]) * convertedMass / (float)(dt);
				staticFriction = (sf[0]+sf[1])/2;
				dynamicFriction = (df[0]+df[1])/2;
				
				//	接触力計算	衝突の面積，抗力を求める
				CalcReflexForce(sp, &analyzer);
				area += sp->area;
			}
		}
	}



	//	接触判定終了後の処理
	//	抗力とその作用点を求め，摩擦を計算し，抗力と摩擦力を物体に加える．
	if (rv){
		for(Contacts::iterator it = contacts.begin(); it != contacts.end(); ++it){
			PHShapePair* cp = *it;
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
	//		DSTR << "ref:" << cp->reflexSpringForce << cp->reflexDamperForce << std::endl;
			Vec3f refF = cp->reflexSpringForce + cp->reflexDamperForce;
			Vec3f refT = cp->reflexSpringTorque + cp->reflexDamperTorque
				+ ((cp->commonPoint - cocog)^refF);

			reflexForce += refF;
			reflexTorque += refT;
			frictionForce += cp->frictionForce;
			frictionTorque += cp->frictionTorque
				+ (((cp->reflexForcePoint+cp->commonPoint) - cocog) ^ cp->frictionForce);
		}
		//	DSTR << std::endl;
		//	力を制限する．
		//	LimitForces();
		// 力を加える．
		solid[0].solid->AddForce(reflexForce + frictionForce, cocog);
		solid[0].solid->AddTorque(reflexTorque + frictionTorque);
		solid[1].solid->AddForce(-(reflexForce + frictionForce), cocog);
		solid[1].solid->AddTorque(-(reflexTorque + frictionTorque));
	}
	return rv;
}


//	凸形状対に発生する反力の計算と最大摩擦力の計算
//	すべて commonPoint を原点とした座標系で計算する．
void PHPenaltyEngine::PHSolidPair::CalcReflexForce(PHShapePair* cp, CDContactAnalysis* analyzer){
	cp->Clear();
	Vec3f cog[2] = {solid[0].cog - cp->commonPoint, solid[1].cog - cp->commonPoint};
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
				CalcTriangleReflexForce(cp, p0, p1, p2, cog[curID], solid[curID].vel, solid[curID].angVel);
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
			solid[0].ori * cp->closestPoint[0] + solid[0].pos,
			solid[1].ori * cp->closestPoint[1] + solid[1].pos};
		cp->reflexSpringForce = reflexSpring * (closest[1] - closest[0]);
		Vec3f colPos = (closest[0]+closest[1])/2;
		cp->reflexSpringTorque = (colPos-cp->commonPoint) ^ cp->reflexSpringForce;
		Vec3f vel[2];
		vel[0] = solid[0].vel + (solid[0].angVel ^ (colPos - solid[0].cog));
		vel[1] = solid[1].vel + (solid[1].angVel ^ (colPos - solid[1].cog));
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
void PHPenaltyEngine::PHSolidPair::CalcTriangleReflexForce(PHShapePair* cp, Vec3f p0, Vec3f p1, Vec3f p2, Vec3f cog, Vec3f vel, Vec3f angVel){
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
void PHPenaltyEngine::PHSolidPair::CalcFriction(PHShapePair* cp){
	//	初めての接触の時
	Vec3f reflexForcePoint = cp->reflexForcePoint + cp->commonPoint;	//	力の作用点(絶対系)
	if (cp->state == PHShapePair::NEW){
		//	バネモデルの始点を設定する．
		cp->transFrictionBase[0] = solid[0].ori.Inv() * (reflexForcePoint - solid[0].pos);
		cp->transFrictionBase[1] = solid[1].ori.Inv() * (reflexForcePoint - solid[1].pos);
		cp->rotSpring = 0;
		cp->frictionState = PHShapePair::STATIC;
		cp->frictionForce = Vec3f();
		cp->frictionTorque = Vec3f();
		return;
	}

	//	接触が継続している場合．
	
	//	並進バネの計算
	//	グローバル系に変換
	cp->transFrictionBase[0] = solid[0].ori * cp->transFrictionBase[0] + solid[0].pos;
	cp->transFrictionBase[1] = solid[1].ori * cp->transFrictionBase[1] + solid[1].pos;
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
	Quaternionf delta[2] = { solid[0].ori*solid[0].lastOri.Inv(), solid[1].ori*solid[1].lastOri.Inv() };
	cp->rotSpring += delta[1].Rotation()*cp->normal - delta[0].Rotation()*cp->normal;
	float frictionSpringTorque = frictionSpring*cp->rotSpring;


	//	動摩擦の制約を加える
	float fricCoeff = (cp->frictionState == PHShapePair::STATIC) ? staticFriction : dynamicFriction;
	float maxFric = fricCoeff * cp->dynaFric.norm();

	//hase	摩擦のテスト中
//	float reflexForce = (cp->reflexSpringForce+cp->reflexDamperForce).norm();
//	if (maxFric < fricCoeff * reflexForce) maxFric = fricCoeff * reflexForce;
	//	ここまで
		
		
	float maxRotFric = fricCoeff * (cp->dynaFricMom - (cp->reflexForcePoint^cp->dynaFric)).norm();
	assert(finite(maxFric));
	DEBUG_EVAL( if(!finite(maxRotFric)){ DSTR << "FricMax:" << maxFric << "  " << maxRotFric << std::endl; } )

	cp->frictionState = PHShapePair::STATIC;
	if (frictionSpringForce > maxFric){
		frictionSpringForce = maxFric;
		cp->frictionState = PHShapePair::DYNAMIC;
	}
	if (abs(frictionSpringTorque) > maxRotFric){
		frictionSpringTorque = sign(frictionSpringTorque) * maxRotFric;
		cp->frictionState = PHShapePair::DYNAMIC;
	}
	cp->frictionForce = frictionSpringForce * frictionForceDicption;
	cp->frictionTorque = frictionSpringTorque * cp->normal;

	if (cp->frictionState == PHShapePair::STATIC){
		///	ダンパによる静止摩擦力の計算
		Vec3f frictionVel = ((solid[1].angVel ^ (reflexForcePoint-solid[1].cog)) + solid[1].vel) - ((solid[0].angVel ^ (reflexForcePoint-solid[0].cog)) + solid[0].vel);
		frictionVel -= frictionVel * cp->normal * cp->normal;
		cp->frictionForce += frictionDamper * frictionVel;
		
		Vec3f frictionAngVel = solid[1].angVel - solid[0].angVel;
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
	cp->transFrictionBase[0] = solid[0].ori.Inv() * (reflexForcePoint - 0.5f*frictionSpringForce/frictionSpring*frictionForceDicption - solid[0].pos);
	cp->transFrictionBase[1] = solid[1].ori.Inv() * (reflexForcePoint + 0.5f*frictionSpringForce/frictionSpring*frictionForceDicption - solid[1].pos);
	cp->rotSpring = frictionSpringTorque / frictionSpring;
}


void PHPenaltyEngine::PHSolidPair::Init(){
	shapePairs.resize(solid[0].solid->shapes.size(), solid[1].solid->shapes.size());
	for(unsigned i=0; i<solid[0].solid->shapes.size(); ++i){
		for(unsigned j=0; j<solid[1].solid->shapes.size(); ++j){
			shapePairs.item(i, j) = DBG_NEW PHShapePair(&solid[0].solid->shapes[i], &solid[1].solid->shapes[j]);
		}
	}
}


//----------------------------------------------------------------------------
OBJECT_IMP(PHPenaltyEngine, PHEngine);

void PHPenaltyEngine::Add(PHSolid* s){
	solids.push_back(s);
	Invalidate();
}

void PHPenaltyEngine::Init(){
	solidPairs.resize(solids.size(), solids.size());
	for(unsigned i=0; i<solids.size(); ++i){
		for(unsigned j=i+1; j<solids.size(); ++j){
			solidPairs.item(i, j) = DBG_NEW PHSolidPair(solids[i], solids[j]);
			solidPairs.item(i, j)->Init();
		}
	}
	ready = true;
}


/**	Solidの端の位置をもち，ソートされるもの	*/
struct PHSolidEdge{
	float edge;				///<	端の位置(グローバル系)
	int solid;				///<	元の solidの位置
	bool bMin;				///<	初端ならtrue
	bool operator < (const PHSolidEdge& s) const { return edge < s.edge; }
};

void PHPenaltyEngine::Step(){
	if(!ready)
		Init();
	
	Vec3f dir(0,0,1);
	typedef std::vector<PHSolidEdge> Edges;
	Edges edges;
	edges.resize(solids.size()*2);
	Edges::iterator eit = edges.begin();
	for(unsigned int i = 0; i < solids.size(); ++i){
		solids[i]->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
		if (!finite(eit[0].edge) || !finite(eit[1].edge)){
			DSTR << solids[i]->GetName() << " min:" << eit[0].edge << " max:" << eit[1].edge << std::endl;
			solids[i]->GetBBoxSupport(dir, eit[0].edge, eit[1].edge);
		}
		eit[0].solid = i; eit[0].bMin = true;
		eit[1].solid = i; eit[1].bMin = false;
		eit += 2;
	}
	if (edges.size() > 200){
		DSTR << "strange edges" << std::endl;
		DSTR << (unsigned)&*edges.begin() << (unsigned)&*edges.end();
	}
	std::sort(edges.begin(), edges.end());
	//	端から見ていって，接触の可能性があるノードの判定をする．
	typedef std::set<int> SolidSet;
	SolidSet cur;							//	現在のSolidのセット
	for(Edges::iterator it = edges.begin(); it != edges.end(); ++it){
		if (it->bMin){						//	初端だったら，リスト内の物体と判定
//			DSTR << "Col " << it->solid << " - ";
			for(SolidSet::iterator itf=cur.begin(); itf != cur.end(); ++itf){
				int f1 = it->solid;
				int f2 = *itf;
				if (f1 > f2) std::swap(f1, f2);
				PHSolidPair* pair =  solidPairs.item(f1, f2);
				//	SolidとSolidの衝突判定
				pair->Detect(this);	//	再帰的に衝突判定(GJKを使用)
//				DSTR << " " << *itf;
			}
//			DSTR << std::endl;
			cur.insert(it->solid);
		}else{
			cur.erase(it->solid);			//	終端なので当該フレームを削除．
		}
	}
	
}


}
