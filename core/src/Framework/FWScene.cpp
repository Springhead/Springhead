/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Framework/FWScene.h>
#include <Framework/FWObject.h>
#include <Framework/FWSdk.h>
#include <Framework/FWHapticPointer.h>
#include <Physics/PHScene.h>
#include <Physics/PHSdk.h>
#include <Physics/PHContactPoint.h>
#include <Physics/PHContactEngine.h>
#include <Physics/PHConstraintEngine.h>
#include <Physics/PHHapticEngine.h>
#include <Physics/PHFemEngine.h>
#include <Physics/PHBallJoint.h>
#include <Physics/PHHingeJoint.h>
#include <Physics/PHSliderJoint.h>
#include <Graphics/GRScene.h>
#include <Graphics/GRSdk.h>
#include <Graphics/GRDrawEllipsoid.h>

#ifdef USE_HDRSTOP

#pragma hdrstop
#endif

namespace Spr{;

FWScene::FWScene(const FWSceneDesc& d) : phScene(NULL), grScene(NULL){
	// デフォルト描画設定
	renderPHScene = true;
	renderGRScene = false;
	// ソリッド描画のみ
	renderSolid = true;
	renderWire  = false;
	// 付加情報の描画はなし
	renderAxisWorld = renderAxisSolid = renderAxisConst = false;
	renderForceSolid = renderForceConst	= false;
	renderGridX = renderGridY = renderGridZ = false;
	renderLimit     = false;
	renderContact	= false;
	renderBBox      = false;
	renderIK		= false;
	renderFEM		= false;
	renderSkeletonSensor = false;
	renderOp		= false;
	// 倍率は等倍
	scaleAxisWorld = scaleAxisSolid = scaleAxisConst = 1.0f;
	scaleForce = 1.0f;
	scaleMoment = 1.0f;
	scaleIK = 1.0f;
	// マテリアル
	matAxis.x	= GRRenderIf::RED;
	matAxis.y	= GRRenderIf::GREEN;
	matAxis.z	= GRRenderIf::BLUE;
	matContact	= GRRenderIf::YELLOW;
	matBBoxLocal = GRRenderIf::LIGHTGRAY;
	matBBoxWorld = GRRenderIf::WHITE;
	matForce	= GRRenderIf::ORANGE;
	matMoment	= GRRenderIf::CYAN;
	matCoP[0] = GRRenderIf::BLUE;
	matCoP[1] = GRRenderIf::GREEN;
	matCoP[2] = GRRenderIf::RED;
	matGrid.x = matGrid.y = matGrid.z = GRRenderIf::GRAY;
	// 座標軸
	axisStyle = FWSceneIf::AXIS_LINES;
	// グリッド
	gridOffset.clear();
	gridSize = Vec3f(100.0f, 100.0f, 100.0f);
	gridSlice = Vec3i(100, 100, 100);
}

FWSdkIf* FWScene::GetSdk(){
	NameManagerIf* nm = GetNameManager();
	FWSdkIf* sdk = DCAST(FWSdkIf, nm);
	return sdk;
}

NamedObjectIf* FWScene::FindObject(UTString name, UTString cls){
	//	余分にphScene,grSceneの2つのシーンを検索するので，NameManager::FindObjectとちょっと違う．
	//	2つのシーンは，Sdkに所有されているので，普通にNameManagerとして，FWSceneを指定することはできない．

	//	まず自分と子孫を検索
	NamedObjectIf* rv = FindObjectFromDescendant(name, cls);
	if (rv) return rv;
	//	先祖を検索
	rv = FindObjectFromAncestor(name, cls);
	if (rv) return rv;

	//	なければ，phSceneとgrSceneについて自分と子孫を探す。
	if (phScene){
		rv = DCAST(PHScene, phScene)->FindObjectFromDescendant(name, cls);
		if (rv) return rv;
	}
	if (grScene){
		rv = DCAST(GRScene, grScene)->FindObjectFromDescendant(name, cls);
		if (rv) return rv;
	}

	//	それでもないならば、namespaceを削って、もう一度検索
	size_t pos = name.find('/');
	if (pos != UTString::npos){	//	 名前空間の指定がある場合
		UTString n = name.substr(pos+1);
		rv = FindObject(n, cls);
	}
	return rv;
}

ObjectIf* FWScene::CreateObject(const IfInfo* info, const void* desc){
	ObjectIf* rv = Scene::CreateObject(info, desc);
	if (!rv && phScene) rv = phScene->CreateObject(info, desc);
	if (!rv && grScene) rv = grScene->CreateObject(info, desc);
	return rv;
}

FWObjectIf* FWScene::CreateFWObject(){
	FWObjectDesc desc;
	FWObjectIf* obj = DCAST(FWObjectIf, CreateObject(FWObjectIf::GetIfInfoStatic(), &desc));
	obj->SetPHSolid(NULL);
	obj->SetGRFrame(NULL);
	AddChildObject(obj);
	return obj;
}

bool FWScene::AddChildObject(ObjectIf* o){
	FWSdkIf* sdk = DCAST(FWSdkIf, GetNameManager());
	bool ok = false;
	if (!ok) {
		FWObject* obj = DCAST(FWObject, o);
		if (obj) {
			fwObjects.push_back(obj->Cast());
			obj->SetScene(Cast());
			// デフォルトネーム
			if(strcmp(obj->GetName(), "") == 0){
				char name[256];
				sprintf(name, "object%d", NObject()-1);
				obj->SetName(name);
			}
			ok = true;
		}
	}
	if (!ok) {
		PHScene* obj = DCAST(PHScene, o);
		if (obj) {
			phScene = obj->Cast();
			sdk->GetPHSdk()->AddChildObject(obj->Cast());
			ok = true;
		}
	}
	if (!ok) {
		GRScene* obj = DCAST(GRScene, o);
		if (obj) {
			grScene = obj->Cast();
			sdk->GetGRSdk()->AddChildObject(obj->Cast());
			ok = true;
		}
	}
	if (!ok && phScene) {
		ok = phScene->AddChildObject(o);
	}
	if (!ok && grScene) {
		ok = grScene->AddChildObject(o);
	}
	return ok;
}

bool FWScene::DelChildObject(ObjectIf* o){
	FWObjectIf* obj = DCAST(FWObjectIf, o);
	if(obj){
		FWObjects::iterator it = find(fwObjects.begin(), fwObjects.end(), obj);
		if(it != fwObjects.end()){
			fwObjects.erase(it);
			return true;
		}
	}
	return false;
}

HIForceDevice6D* FWScene::GetHumanInterface(size_t pos){
/*	hase:	TBW
	if (pos < humanInterfaces.size()) return humanInterfaces[pos];
*/	return NULL;
}

size_t FWScene::NChildObject() const{
	return fwObjects.size() + (grScene?1:0) + (phScene?1:0);
}

ObjectIf* FWScene::GetChildObject(size_t pos){
	if (phScene){
		if (pos == 0) return phScene;
		pos--;
	}

	if (grScene){
		if (pos == 0) return grScene;
		pos--;
	}	

	if (pos < fwObjects.size()) return fwObjects[pos];

	return NULL;
}

FWSceneIf* SPR_CDECL CreateFWScene(){
	FWScene* rv = DBG_NEW FWScene;
	return rv->Cast();
}

FWSceneIf* SPR_CDECL CreateFWScene(const void* desc){
	FWScene* rv = DBG_NEW FWScene(*(FWSceneDesc*)desc);
	return rv->Cast();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void FWScene::Sync(){
	//	オブジェクト位置・姿勢の同期
	for(FWObjects::iterator it = fwObjects.begin(); it!=fwObjects.end(); ++it){
		DCAST(FWObject, *it)->Sync();
	}
	
	//	カメラの同期 (未実装？)
	if(grScene){
		HIForceDevice6D* device = GetHumanInterface(HI_CAMERACONTROLLER);
		GRCameraIf* camera = grScene->GetCamera();
		if(camera && device){
			Posed pose;
	/*		hase:	TBW	
			pose.Pos() = device->GetPos();	
			pose.Ori() = device->GetOri();
	*/		if (!camera->GetFrame()){
				GRSceneIf* scene = DCAST(GRSceneIf, camera->GetNameManager());
				if (scene) camera->SetFrame(DCAST(GRFrameIf, 
					scene->CreateVisual(GRFrameIf::GetIfInfoStatic(), GRFrameDesc())));
			}
			if (camera->GetFrame()){
				Affinef af;
				pose.ToAffine(af);
				camera->GetFrame()->SetTransform(af);
			}
		}
	}

}

void FWScene::Step(){
	if (phScene) phScene->Step();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// 描画系

void FWScene::Draw(GRRenderIf* render, bool debug){
	if (debug){
		renderPHScene = true;
		renderGRScene = false;
	}else{
		renderPHScene = false;
		renderGRScene = true;
	}
	Draw(render);
}
void FWScene::Draw(GRRenderIf* render){
	if (renderPHScene) DrawPHScene(render);
	if (renderGRScene && GetGRScene()){
		Sync();
		GetGRScene()->Render(render);
	}
}

/// シーン内の全てのオブジェクトをレンダリングする
void FWScene::DrawPHScene(GRRenderIf* render){
	if (!phScene) return;

	// 光源が1つもセットされない場合、適当な光源を自動で追加
	bool defLight = false;
	if(render->NLights() == 0){
		defLight = true;
		GRLightDesc ld;
		ld.diffuse = Vec4f(1,1,1,1) * 0.8f;
		ld.specular = Vec4f(1,1,1,1) * 0.8f;
		ld.ambient = Vec4f(1,1,1,1) * 0.4f;
		ld.position = Vec4f(1,1,1,0);
		render->PushLight(ld);
	}

	// ワールド座標軸
	if(renderAxisWorld){
		if(renderSolid)
			DrawCoordinateAxis(render, scaleAxisWorld, true);
		if(renderWire){
			render->SetLighting(false);
			DrawCoordinateAxis(render, scaleAxisWorld, false);
			render->SetLighting(true);
		}
	}

	// グリッド
	render->SetLighting(false);
	if(renderGridX){
		render->PushModelMatrix();
		render->MultModelMatrix(Affinef::Trn(gridOffset.x, 0.0f, 0.0f));
		render->MultModelMatrix(Affinef::Rot(Rad(90.0f), 'y'));
		render->SetMaterial(matGrid.x);
		render->DrawGrid(gridSize.x, gridSlice.x);
		render->PopModelMatrix();
	}
	if(renderGridY){
		render->PushModelMatrix();
		render->MultModelMatrix(Affinef::Trn(0.0f, gridOffset.y, 0.0f));
		render->MultModelMatrix(Affinef::Rot(Rad(90.0f), 'x'));
		render->SetMaterial(matGrid.y);
		render->DrawGrid(gridSize.y, gridSlice.y);
		render->PopModelMatrix();
	}
	if(renderGridZ){
		render->PushModelMatrix();
		render->MultModelMatrix(Affinef::Trn(0.0f, 0.0f, gridOffset.z));
		render->SetMaterial(matGrid.z);
		render->DrawGrid(gridSize.z, gridSlice.z);
		render->PopModelMatrix();
	}
	render->SetLighting(true);

	// 接触判定
	if(renderBBox){
		PHConstraintEngine* engine = phScene->GetConstraintEngine()->Cast();
		//DrawDetectorRegion(render, &engine->rootRegion);
		for(int c = 0; c < (int)engine->cells.size(); c++)
			DrawBBox(render, Posed(), &engine->cells[c].bbox);
	}

	// 剛体
	PHSolidIf **solids = phScene->GetSolids();
	for(int i = 0; i < phScene->NSolids(); ++i){
		if(!IsRenderEnabled(solids[i]))
			continue;

		// BBox
		if(renderBBox){
			PHSolid* so = solids[i]->Cast();
			if (so->bboxReady) {
				render->SetMaterial(matBBoxLocal);
				DrawBBox(render, so->GetPose(), &so->bbLocal);
			}
			else if (so->aabbReady) {
				render->SetMaterial(matBBoxWorld);
				DrawBBox(render, Posed(), &so->bbWorld);
			}
		}

		// 形状を描画
		if (renderSolid) {
			Mat matSolid = GetSolidMaterial(solids[i]);
			if (matSolid.mat == -1)
				matSolid = GetAutoMaterial(i);
			render->SetMaterial(matSolid.mat, matSolid.alpha);
			if (matSolid.alpha < 1.0f) {
				render->SetAlphaTest(true);
				render->SetAlphaMode(GRRenderBaseIf::BF_SRCALPHA, GRRenderBaseIf::BF_INVSRCALPHA);
			}
			DrawSolid(render, solids[i], true);
			if (matSolid.alpha < 1.0f) {
				render->SetAlphaTest(false);
			}
		}
		if(renderWire){
			Mat matWire  = GetWireMaterial(solids[i]);
			if(matWire.mat == -1)
				matWire = GetAutoMaterial(i);
			render->SetMaterial(matWire.mat, matWire.alpha);
			render->SetLighting(false);
			DrawSolid(render, solids[i], false);
			render->SetLighting(true);
		}
	}

	// 拘束
	for(int i = 0; i < phScene->NJoints(); ++i){
		PHConstraintIf* con = phScene->GetJoint(i);
		if(IsRenderEnabled(con))
			DrawConstraint(render, con);
	}
	
	// 接触
	if(renderContact){
#if 0
		for(int i = 0; i < phScene->NContacts(); ++i){
			PHContactPointIf* con = phScene->GetContact(i);
			if(IsRenderEnabled(con)){
				DrawConstraint(render, con);
				DrawContact(render, con);
			}
		}
#else
		//	DrawContactはsectionが変化するので、裏でシミュレーションが走ると落ちる。
		//	pointsも同様
		DrawContactSafe(render, phScene->GetConstraintEngine());
#endif
	}

	// 関節可動域
	if(renderLimit){
		for(int i = 0; i < phScene->NJoints(); ++i){
			PHConstraintIf* con = phScene->GetJoint(i);
			if(IsRenderEnabled(con))
				DrawLimit(render, con);
		}
	}
	
	// Inverse Kinematics
	if(renderIK){
		//SetMaterialSample((GRRenderIf::TMaterialSample)2);
		PHIKEngineIf* ikEngine = phScene->GetIKEngine();
		if (ikEngine) {
			DrawIK(render, ikEngine);
		}
	}

	if(renderHaptic){
		PHHapticEngineIf* hapticEngine = phScene->GetHapticEngine();
		if(hapticEngine){
			DrawHaptic(render, hapticEngine);
		}
	}

	if(renderSkeletonSensor){
		DrawSkeletonSensor(render);
	}

	if(renderFEM){
		PHFemEngineIf* femEngine = phScene->GetFemEngine();
		if(femEngine){
			DrawFem(render, femEngine);
		}
	}

	if (renderOp){
		PHOpEngineIf* opEngine = phScene->GetOpEngine();
		if (opEngine){
			DrawOp(render, opEngine);
		}
	}
	
	/*
	if(defLight) render->PopLight();
	if (cam) cam->Rendered(render);
	*/
}

/// 剛体をレンダリングする
void FWScene::DrawSolid(GRRenderIf* render, PHSolidIf* solid, bool solid_or_wire){
	Affinef aff;
	solid->GetPose().ToAffine(aff);
	render->PushModelMatrix();
	render->MultModelMatrix(aff);
	
	for(int s = 0; s < solid->NShape(); ++s){
		CDShapeIf* shape = solid->GetShape(s);
		if(IsRenderEnabled(shape)){
			solid->GetShapePose(s).ToAffine(aff);
			render->PushModelMatrix();
			render->MultModelMatrix(aff);
			DrawShape(render, shape, solid_or_wire);
			render->PopModelMatrix();
		}
	}

	// 座標軸
	if(renderAxisSolid)
		DrawCoordinateAxis(render, scaleAxisSolid, solid_or_wire);

	// 力
	if(renderForceSolid){
		DrawForce(render, solid->GetForce(), solid->GetTorque());
	}


	render->PopModelMatrix();
}

void FWScene::DrawBBox(GRRenderIf* render, Posed pose, PHBBox* bbox){
	Vec3d bbcenter = bbox->GetBBoxCenter();
	Vec3d bbextent = bbox->GetBBoxExtent();
	render->PushModelMatrix();
	Affinef af;
	pose.ToAffine(af);
	render->MultModelMatrix(af);
	render->MultModelMatrix(Affinef::Trn(bbcenter.x, bbcenter.y, bbcenter.z));
	render->SetLighting(false);
	render->DrawBox(2.0*bbextent.x, 2.0*bbextent.y, 2.0*bbextent.z, false);
	render->SetLighting(true);
	render->PopModelMatrix();
}

void FWScene::DrawShape(GRRenderIf* render, CDShapeIf* shape, bool solid_or_wire){
	CDBoxIf*		box		= DCAST(CDBoxIf, shape);
	CDSphereIf*		sphere	= DCAST(CDSphereIf, shape);
	CDEllipsoidIf*	ell = shape->Cast();
	CDCapsuleIf*	cap = DCAST(CDCapsuleIf, shape);
	CDRoundConeIf* rc = DCAST(CDRoundConeIf, shape);
	CDCylinderIf * cy = DCAST(CDCylinderIf, shape);
	CDConvexMeshIf* mesh	= DCAST(CDConvexMeshIf, shape);

	// solid, wireframeの順に描画
	const int slice = 16;
	if(box){
		Vec3f sz = box->GetBoxSize();
		render->DrawBox(sz.x, sz.y, sz.z, solid_or_wire);
	}
	if(sphere)	render->DrawSphere(sphere->GetRadius(), slice, slice, solid_or_wire);
	if (ell)	render->DrawEllipsoid(ell->GetRadius(), slice, slice, solid_or_wire);
	if(cap)		render->DrawCapsule(cap->GetRadius(), cap->GetLength(), slice, solid_or_wire);
	if (rc) {
		Vec2f r = rc->GetRadius();
		render->DrawRoundCone(r[0], r[1], rc->GetLength(), slice, solid_or_wire);
	}
	if (cy) {
		render->DrawCylinder(cy->GetRadius(), cy->GetLength(), slice, solid_or_wire);
	}
	if(mesh)	DrawMesh(render, mesh, solid_or_wire);
}

void FWScene::DrawConstraint(GRRenderIf* render, PHConstraintIf* con){
	Affinef aff;
	Vec3d f, t;

	Posed sock, plug;
	con->GetSocketPose(sock);
	con->GetPlugPose(plug);

	if(renderAxisConst || renderForceConst){
		// socket
		(con->GetSocketSolid()->GetPose() * sock).ToAffine(aff);
		render->PushModelMatrix();
		render->MultModelMatrix(aff);

		if(renderAxisConst){
			if(renderSolid)
				DrawCoordinateAxis(render, scaleAxisConst, true);
			if(renderWire){
				render->SetLighting(false);
				DrawCoordinateAxis(render, scaleAxisConst, false);
				render->SetLighting(true);
			}
		}
	
		if(renderForceConst){
			con->GetConstraintForce(f, t);
			DrawForce(render, f, t);
			PHContactEngineIf* ce =  DCAST(PHContactEngineIf, con);
			if (ce){
				DrawContactEngine(render, ce);
			}
		}
		render->PopModelMatrix();
	}
	
	// plug
	if(renderAxisConst){
		(con->GetPlugSolid()->GetPose() * plug).ToAffine(aff);
		render->PushModelMatrix();
		render->MultModelMatrix(aff);
		if(renderSolid)
			DrawCoordinateAxis(render, scaleAxisConst, true);
		if(renderWire){
			render->SetLighting(false);
			DrawCoordinateAxis(render, scaleAxisConst, false);
			render->SetLighting(true);
		}
		render->PopModelMatrix();
	}

	// 追加拘束座標系（一部のJointLimitが関節本体とは別に持っている）
	if(renderAxisConst){
		PHBallJoint* bj = con->Cast();
		if (bj) {
			PHBallJointLimit* limit = bj->GetLimit()->Cast();
			if (limit) {
				con->GetSocketSolid()->GetPose().ToAffine(aff);
				Matrix3d Jcinv = bj->limit->Jcinv;
				aff.Rot() = Jcinv.trans() * aff.Rot();
				aff.Trn() = con->GetPlugSolid()->GetPose().Pos();

				render->PushModelMatrix();
				render->MultModelMatrix(aff);
				render->SetLighting(false);
				DrawCoordinateAxis(render, scaleAxisConst*0.5, false);
				render->SetLighting(true);
				render->PopModelMatrix();
			}
		}
	}
}

void FWScene::DrawLimit(GRRenderIf* render, PHConstraintIf* con){
	Affinef aff;

	Posed sock, plug;
	con->GetSocketPose(sock);
	con->GetPlugPose(plug);

	if (renderLimit) {
		render->SetDepthTest(false);
		render->SetLineWidth(2);
		render->SetLighting(false);

		// ボールジョイントのLimit
		PHBallJoint* bj = con->Cast();
		if (bj) {
			// 現在の角度
			(con->GetPlugSolid()->GetPose() * plug).ToAffine(aff);
			render->PushModelMatrix();
			render->MultModelMatrix(aff);
			render->SetMaterial(matAxis.y);
			render->DrawLine(Vec3f(0,0,0), Vec3f(0,0,1));
			render->PopModelMatrix();

			// 基準方向（角度０）
			(con->GetSocketSolid()->GetPose() * sock).ToAffine(aff);
			render->PushModelMatrix();
			render->MultModelMatrix(aff);
			render->SetMaterial(matAxis.z);
			render->DrawLine(Vec3f(0,0,0), Vec3f(0,0,1));

			// Spline可動域曲線の表示
			PHBallJointSplineLimit* spL = bj->GetLimit()->Cast();
			if (spL && spL->IsEnabled()) {
				for (int i=0; i<spL->limitCurve.NEdges(); ++i) {
					SplinePoint pt = spL->limitCurve.GetPointOnEdge(i, 0);
					if (i==0) {
						render->SetMaterial(matAxis.x);
					} else if (i%2==0) {
						render->SetMaterial(matAxis.y);
					} else {
						render->SetMaterial(matAxis.z);
					}
					for (double t=0; t<1.0; t+=0.1) {
						pt.t = t; pt.Update();
						Vec3d p0 = Vec3d(sin(pt.pos[0])*cos(pt.pos[1]), sin(pt.pos[0])*sin(pt.pos[1]), cos(pt.pos[0]));

						pt.t = (t+0.1 > 1.0) ? 1.0 : (t+0.1); pt.Update();
						Vec3d p1 = Vec3d(sin(pt.pos[0])*cos(pt.pos[1]), sin(pt.pos[0])*sin(pt.pos[1]), cos(pt.pos[0]));

						render->DrawLine(p0, p1);
					}
				}
			}

			// 円形可動域曲線の表示
			PHBallJointConeLimit* coL = bj->GetLimit()->Cast();
			if (coL && coL->IsEnabled()) {
				// （注）現状limitDirには非対応<!!>
				Vec2d lim; coL->GetSwingRange(lim);
				for (double t=0; t<2*M_PI; t+=Rad(10)) {
					double z = cos(lim[1]);
					double r = z * tan(lim[1]);
					render->SetMaterial(matAxis.x);
					render->DrawLine(Vec3f(r*cos(t), r*sin(t), z), Vec3f(r*cos(t+Rad(10)), r*sin(t+Rad(10)), z));
				}
			}

			render->PopModelMatrix();
		}

		// ヒンジジョイントのLimit
		PHHingeJoint* hj = con->Cast();
		if (hj) {
			PH1DJointLimit* limit = hj->GetLimit()->Cast();
			if (limit && limit->IsEnabled()) {
				// 現在の角度
				(con->GetPlugSolid()->GetPose() * plug).ToAffine(aff);
				render->PushModelMatrix();
				render->MultModelMatrix(aff);
				render->SetMaterial(matAxis.y);
				render->DrawLine(Vec3f(0,0,0), Vec3f(1,0,0));
				render->PopModelMatrix();

				// 基準方向（角度０）
				(con->GetSocketSolid()->GetPose() * sock).ToAffine(aff);
				render->PushModelMatrix();
				render->MultModelMatrix(aff);
				render->SetMaterial(matAxis.z);
				render->DrawLine(Vec3f(0,0,0), Vec3f(1,0,0));

				// 可動範囲
				Vec2d lim; limit->GetRange(lim);
				if (lim[0] < lim[1]) {
					double l0=std::max(lim[0],-2*M_PI), l1=std::min(lim[1],2*M_PI);
					for (double t=l0; t<l1; t+=Rad(5)) {
						double r  = 1.0;
						double t2 = std::min(lim[1], t+Rad(5));
						render->SetMaterial(matAxis.x);
						render->DrawLine(Vec3f(r*cos(t), r*sin(t), 0.0), Vec3f(r*cos(t2), r*sin(t2), 0.0));
					}
				}
				render->PopModelMatrix();
			}
		}

		// スライダジョイントののLimit
		PHSliderJoint* sj = con->Cast();
		if (sj) {
			PH1DJointLimit* limit = sj->GetLimit()->Cast();
			if (limit && limit->IsEnabled()) {
				con->GetSocketSolid()->GetPose().ToAffine(aff);
				render->PushModelMatrix();
				render->MultModelMatrix(aff);
				Vec2d lim; limit->GetRange(lim);
				if (lim[0] < lim[1]) {
					float vtx[2][3] = {{(float)lim[0], 0.0f, 0.0f}, {(float)lim[1], 0.0f, 0.0f}};
					GLuint idx[2] = {0, 1};
					render->SetMaterial(matAxis.z);
					render->DrawIndexed(GRRenderBaseIf::LINES, idx, (void*)vtx, 2);
				}
				render->PopModelMatrix();
			}
		}

		render->SetLineWidth(1);
		render->SetLighting(true);
		render->SetDepthTest(true);
	}
}

void FWScene::DrawContactSafe(GRRenderIf* render, PHConstraintEngineIf* cei){
	if (!cei) return;
	PHConstraintEngine* ce = cei->Cast();
	PHConstraintEngine::ContactInfo& infos = ce->contactInfoQueue.queue[ce->contactInfoQueue.reading];

	for(unsigned i=0; i<infos.points.size(); ++i){
		PHContactPointIf* con = (PHContactPointIf*)&*infos.points[i];
		if(IsRenderEnabled(con)) DrawConstraint(render, con);
	}

	render->SetMaterial(matContact);
	render->SetLighting(false);
	render->SetDepthTest(false);
	render->SetVertexFormat(GRVertexElement::vfP3f);
	for(unsigned i=0; i<infos.sections.size(); ++i){
		if(infos.sections[i].size() < 3) continue;	
		render->DrawDirect(GRRenderBaseIf::LINE_LOOP, &infos.sections[i][0], infos.sections[i].size());
	}
	if (ce->contactInfoQueue.reading != ce->contactInfoQueue.wrote){
		ce->contactInfoQueue.reading = 
			ce->contactInfoQueue.reading < 2 ? ce->contactInfoQueue.reading+1 : 0;
	}
	render->SetDepthTest(true);
	render->SetLighting(true);
}
void FWScene::DrawContact(GRRenderIf* render, PHContactPointIf* con){
	render->SetMaterial(matContact);	
	render->SetLighting(false);
	render->SetDepthTest(false);	
	PHContactPoint* c = con->Cast();
	if (c->shapePair->section.size() > 1) {
		std::vector<Vec3f> vtx;
		vtx.resize(c->shapePair->section.size());
		copy(c->shapePair->section.begin(), c->shapePair->section.end(), vtx.begin());
		render->SetVertexFormat(GRVertexElement::vfP3f);
		render->DrawDirect(GRRenderBaseIf::LINE_LOOP, &vtx[0], vtx.size());
	}
	render->SetPointSize(10, true);
	render->SetMaterial(GRRenderBaseIf::GREEN);
	render->DrawPoint(c->shapePair->center);
/*	render->SetMaterial(GRRenderBaseIf::GREEN);
	render->DrawPoint(c->shapePair->shapePoseW[0] * c->shapePair->closestPoint[0]);
	render->SetMaterial(GRRenderBaseIf::YELLOWGREEN);
	render->DrawPoint(c->shapePair->shapePoseW[1] * c->shapePair->closestPoint[1]);
*/

	render->SetDepthTest(true);
	render->SetLighting(true);
}

/*void FWScene::DrawDetectorRegion(GRRenderIf* render, PHContactDetector::Region* region){
	int n = (int)region->regions.size();
	// 最下層のみBBoxを描画
	if(n == 0)
		DrawBBox(render, Posed(), &region->bbox);
	for(int i = 0; i < n; i++)
		DrawDetectorRegion(render, region->regions[i]);
}*/

/// IKの計算結果をレンダリングする
void FWScene::DrawIK(GRRenderIf* render, PHIKEngineIf* ikEngine) {
	render->SetLighting(false);
	render->SetDepthTest(false);
	render->SetLineWidth(5);
	render->SetMaterial(matAxis.x);

	for (size_t i=0; i < DCAST(PHIKEngine,ikEngine)->actuators.size(); ++i) {
		PHIKActuator* ikA = DCAST(PHIKEngine,ikEngine)->actuators[i];
		if(!ikA) continue;

		PHIKActuator* ikP = ikA->GetParent()->Cast();
		if (ikP) {
			Posed ikAp; ikA->joint->GetPlugPose(ikAp);
			Posed ikPp; ikP->joint->GetPlugPose(ikPp);
			Vec3d p0 = ikA->GetSolidTempPose() * ikAp.Pos();
			Vec3d p1 = ikP->GetSolidTempPose() * ikPp.Pos();
			render->DrawLine(p0, p1);
		} else {
			Posed ikAp; ikA->joint->GetPlugPose(ikAp);
			Vec3d p0 = ikA->GetSolidTempPose() * ikAp.Pos();
			Vec3d p1 = ikA->joint->GetSocketSolid()->GetPose().Pos();
			render->DrawLine(p0, p1);
		}

		PHIKEndEffector* ikE = ikA->eef;
		if (ikE) {
			Posed ikAp; ikA->joint->GetPlugPose(ikAp);
			Vec3d p0 = ikA->GetSolidTempPose() * ikAp.Pos();
			Vec3d p1 = ikE->GetSolidTempPose() * ikE->GetTargetLocalPosition();
			render->DrawLine(p0, p1);
		}

		// -----

		if (i % 3 == 0) {
			render->SetMaterial(matAxis.x);
		}
		else if (i % 3 == 1) {
			render->SetMaterial(matAxis.y);
		}
		else {
			render->SetMaterial(matAxis.z);
		}

		Posed pose = ikA->GetSolidTempPose();
		render->DrawLine(pose.Pos(), pose * Vec3d(2, 0, 0));
		render->DrawLine(pose.Pos(), pose * Vec3d(0, 2, 0));
		render->DrawLine(pose.Pos(), pose * Vec3d(0, 0, 2));

		render->SetMaterial(matAxis.x);

		// -----
	}

	render->SetLineWidth(1);
	render->SetLighting(true);
	render->SetDepthTest(true);
}

void FWScene::DrawHaptic(GRRenderIf* render, PHHapticEngineIf* hapticEngine) {
	PHHapticEngine* he = DCAST(PHHapticEngine, hapticEngine);
	int Npointers = he->NPointers();
	if(Npointers == 0) return;

	// プロキシの描画
	for(int i = 0; i< he->NPointers(); i++){
		PHHapticPointer* pointer = he->GetPointer(i)->Cast();
		Posed proxyPose = pointer->proxyPose;
		Affinef aff;
		proxyPose.ToAffine(aff);
		render->PushModelMatrix();
		render->MultModelMatrix(aff);
	
		render->SetMaterial(render->GOLD);
		for(int j = 0; j < pointer->NShape(); ++j){
			CDShapeIf* shape = pointer->GetShape(j);
			if(IsRenderEnabled(shape)){
				pointer->GetShapePose(j).ToAffine(aff);
				render->PushModelMatrix();
				render->MultModelMatrix(aff);
				DrawShape(render, shape, false);
				render->PopModelMatrix();
			}
		}
		render->PopModelMatrix();

#if 0	//	lastProxy
		render->SetMaterial(render->CORAL);
		proxyPose = pointer->lastProxyPose;
		proxyPose.ToAffine(aff);
		render->PushModelMatrix();
		render->MultModelMatrix(aff);

		for (int j = 0; j < pointer->NShape(); ++j) {
			CDShapeIf* shape = pointer->GetShape(j);
			if (IsRenderEnabled(shape)) {
				pointer->GetShapePose(j).ToAffine(aff);
				render->PushModelMatrix();
				render->MultModelMatrix(aff);
				DrawShape(render, shape, false);
				render->PopModelMatrix();
			}
		}
		render->PopModelMatrix();
#endif
	}

	if (he->GetHapticStepMode() != PHHapticEngine::SINGLE_THREAD) {
		render->SetLighting(false);
		render->SetDepthTest(false);

		render->PushModelMatrix();
		render->SetModelMatrix(Affinef());
		for (int i = 0; i < Npointers; i++) {
			UTRef<PHHapticPointer> pointer = DBG_NEW PHHapticPointer(*(PHHapticPointer*)&*he->GetPointer(i));
			float range = pointer->GetLocalRange();
			float radius = 0.02f;
			int nNeighbors = (int)pointer->neighborSolidIDs.size();
			for (int j = 0; j < nNeighbors; j++) {
				int solidID = pointer->neighborSolidIDs[j];
				PHSolidPairForHaptic* solidPair = (PHSolidPairForHaptic*)he->GetSolidPairTemp(solidID, i);
				for (int k = 0; k < solidPair->body[0]->NShape(); k++) {
					for (int l = 0; l < solidPair->body[1]->NShape(); l++) {
						PHShapePairForHaptic* sp = (PHShapePairForHaptic*)solidPair->GetShapePair(k, l);
						for (int m = 0; m < 2; m++) {
							// 近傍点対		・白点
							Posed p;
							p.Pos() = sp->shapePoseW[m] * sp->closestPoint[m];
							Affinef aff;
							p.ToAffine(aff);
							render->PushModelMatrix();
							render->MultModelMatrix(aff);
							render->SetMaterial(GRRenderIf::WHITE);
							render->DrawSphere(range * radius, 10, 10, true);
							render->PopModelMatrix();
						}
						// 接触点		：黄色
						for (int m = 0; m < (int)sp->intersectionVertices.size(); m++) {
							Posed p;
							p.Pos() = sp->shapePoseW[1] * sp->intersectionVertices[m];
							Affinef aff;
							p.ToAffine(aff);
							render->PushModelMatrix();
							render->MultModelMatrix(aff);
							render->SetMaterial(GRRenderIf::YELLOW);
							render->DrawSphere(range * radius, 10, 10, true);
							render->PopModelMatrix();
						}
#if 0
						// 面
						Posed p;	// 面の位置姿勢
						p.Pos() = sp->shapePoseW[0] * sp->closestPoint[0];
						Vec3d vec = Vec3d(0.0, 1.0, 0.0);
						double angle = acos(vec * sp->normal);
						Vec3d axis = vec % sp->normal;
						if(axis.norm() < 1e-5) axis = vec;
						p.Ori() = Quaterniond::Rot(angle, axis);
						
						Affinef aff;
						p.ToAffine(aff);
						Vec4f moon(1.0, 1.0, 0.8, 0.3);
						render->PushModelMatrix();
						render->MultModelMatrix(aff);
						render->SetMaterial( GRMaterialDesc(moon) );
						render->SetAlphaTest(true);
						render->SetAlphaMode(render->BF_SRCALPHA, render->BF_ONE);
						render->DrawBox(range * 0.5f, 0.005f, range * 0.5f, true);
						render->PopModelMatrix();
						render->SetAlphaTest(false);
#endif
					}
				}
			}
		}
		render->PopModelMatrix();
	}
	render->SetLighting(true);
	render->SetDepthTest(true);
}

void FWScene::DrawSkeletonSensor(GRRenderIf* render) {
	render->SetDepthTest(true);

	for (size_t i=0; i<skeletonSensors.size(); ++i) {
		FWSkeletonSensor* ss = skeletonSensors[i]->Cast();
		for (size_t j=0; j<ss->skeletons.size(); ++j) {
			for (size_t k=0; (int)k<ss->skeletons[j].phSkeleton->NBones(); ++k) {
				PHSolidIf* so = ss->skeletons[j].phSkeleton->GetBone((int) k)->GetProxySolid();
				if (!so) { so = ss->skeletons[j].phSkeleton->GetBone((int) k)->GetSolid(); }

				DrawSolid(render, so, true);
			}
		}
	}
}

void FWScene::DrawFem(GRRenderIf* render, PHFemEngineIf* femEngine){
	PHFemEngine* fe = DCAST(PHFemEngine, femEngine);

	//render->SetDepthTest(false);
	render->SetLighting(false);
	for(int i = 0; i< (int)fe->meshes_n.size(); i++){
		PHFemMeshNew* mesh = fe->meshes_n[i];
		Posed solidPose = mesh->GetPHSolid()->GetPose();


		// 頂点の描画
		int nv = (int)mesh->vertices.size();
		for(int j = 0; j < nv; j++){
			Posed p;
			p.Pos() = solidPose * mesh->vertices[j].pos;
			//p.Pos().y -= 0.01;
			//Affinef aff;
			//p.ToAffine(aff);
			//render->PushModelMatrix();
			//render->MultModelMatrix(aff);
			//render->SetMaterial(GRRenderIf::YELLOW);
			//render->DrawSphere(0.01f, 10, 10, true);
			//render->PopModelMatrix();
			// 頂点番号の描画
			std::stringstream str;
			str << j;
			render->DrawFont((Vec3f)p.Pos(), str.str());
		}
		// 辺の描画
		int ne = (int)mesh->edges.size();
		ne = (int)mesh->nSurfaceEdge;
		for(int j = 0; j < ne; j++){
			Vec3d p[2];
			p[0] = solidPose * mesh->vertices[mesh->edges[j].vertexIDs[0]].pos;
			p[1] = solidPose * mesh->vertices[mesh->edges[j].vertexIDs[1]].pos;
			render->SetMaterial(GRRenderIf::YELLOW);
			render->DrawLine(p[0], p[1]);
		}
		// 各種情報を2次元座標で描画
		// テクスチャ、温度、振動変位の描画などはFWFemMeshでおこなう
		render->EnterScreenCoordinate();
		float pos = 20.0f;
		float margin = 20.0f;
		std::stringstream str;
		{
			// 頂点数
			str << "Vertices : " << mesh->vertices.size() << std::endl;
			render->DrawFont(Vec2f(20, pos), str.str());
			str.str("");
			str.clear(std::stringstream::goodbit);
			pos += margin;		
		}
		{
			// 辺数
			str << "Edges : " << mesh->edges.size() << std::endl;
			render->DrawFont(Vec2f(20, pos), str.str());
			str.str("");
			str.clear(std::stringstream::goodbit);
			pos += margin;
		}
		{
			// 四面体数
			str << "Tetrahedrons : " << (int)mesh->tets.size() << std::endl;
			render->DrawFont(Vec2f(20, pos), str.str());			
			str.str("");
			str.clear(std::stringstream::goodbit);
			pos += margin;
		}


		
		/// FemVibration関係の情報描画
		if(mesh->GetPHFemVibration()){
			PHFemVibrationIf* vib = mesh->GetPHFemVibration()->Cast();
			//{
			//	// 積分モード
			//	str << "Integration Mode : " << vib- << std::endl;
			//	render->DrawFont(Vec2f(20, pos), str.str());			
			//	str.str("");
			//	str.clear(std::stringstream::goodbit);
			//	pos += margin;
			//}
			{
				// 刻み
				str << "dt : " << vib->GetTimeStep() << std::endl;
				render->DrawFont(Vec2f(20, pos), str.str());			
				str.str("");
				str.clear(std::stringstream::goodbit);
				pos += margin;
			}

		}
		render->LeaveScreenCoordinate();
	}
	render->SetLighting(true);
	//render->SetDepthTest(true);
}

void FWScene::DrawOp(GRRenderIf* render, PHOpEngineIf* opEngineif)
{
	PHOpEngine* opEngine = DCAST(PHOpEngine, opEngineif);
	for (int obji = 0; obji < (int) opEngine->opObjs.size(); obji++)
	{
		PHOpObj &drawObj = *opEngine->opObjs[obji];
//			//DrawAddInternalP
//			if (beginAddITP)
//			{
//				if (tempAddItP->hitedByMouse)
//					render->SetMaterial(GRRenderIf::YELLOW);
//				else render->SetMaterial(GRRenderIf::DARKBLUE);
//
//				Affinef affpos;
//				affpos.Pos() = tempAddItP->pCurrCtr;
//				render->PushModelMatrix();//相対座標で使う
//				render->MultModelMatrix(affpos);
//#ifdef USE_AVG_RADIUS
//				float ra = drawObj.averRadius, rb = ra, rc = ra / 2;
//#endif
//#ifdef USE_DYN_RADIUS
//				float ra = drawObj.averRadius, rb = ra, rc = ra / 2;//internal p
//#endif
//				Spr::TQuaternion<float> elliRotQ; elliRotQ.FromMatrix(tempAddItP->pCurrOrint.Inv() * tempAddItP->ellipRotMatrix);
//
//				drawEll.drawOval(ra * radiusCoe, rb * radiusCoe, rc* radiusCoe, 8, elliRotQ);//dp.pCurrOrint.Inv());
//
//				render->PopModelMatrix();
//			}

			
			for (int i = 0; i < drawObj.assPsNum; i++)
			{
				PHOpParticle &dp = drawObj.objPArr[i];
				
				Vec3f pos1 = dp.pNewCtr;
				Affinef affpos;
				affpos.Pos() = pos1;

				if (dp.hitedByMouse)
					render->SetMaterial(GRRenderIf::YELLOW);
				else if (dp.isColliedSphashSolved)
					render->SetMaterial(GRRenderIf::RED);
				else if (dp.isColliedSphashSolvedReady)
					render->SetMaterial(GRRenderIf::DEEPPINK);
				else if (dp.isColliedbySphash)
					render->SetMaterial(GRRenderIf::GREEN);
				else if (dp.isColliedbyColliCube)
					;
				else render->SetMaterial(GRRenderIf::CADETBLUE);

				render->PushModelMatrix();//相対座標で使う
				render->MultModelMatrix(affpos);
				
				
#ifdef USE_AVG_RADIUS
				float ra = drawObj.objAverRadius, rb = ra, rc = ra / 2;
#else
				float ra = dp.pMainRadius, rb = dp.pSecRadius, rc = dp.pThrRadius;
#endif
#ifdef USE_DYN_RADIUS
				float ra = dp.pDynamicRadius, rb = ra, rc = ra / 2;//internal p
#endif

				Spr::TQuaternion<float> elliRotQ; elliRotQ.FromMatrix(dp.pCurrOrint.Inv() * dp.ellipRotMatrix);

				DrawEllipsoid drawEll;
				drawEll.drawOval(ra * opEngine->radiusCoe, rb * opEngine->radiusCoe, rc* opEngine->radiusCoe, 18, elliRotQ);//dp.pCurrOrint.Inv());

				render->PopModelMatrix();


			}

	//if (drawOrien)
		
			for (int i = 0; i<drawObj.assGrpNum; i++)
			{
			
				Vec3f &sttP = drawObj.objPArr[i].pNewCtr;

				////DrawPindex
				//if (drawObj.objId == 0)
				//	sstr << "PA " << i;
				//else if (drawObj.objId == 1)
				//	sstr << "PB " << i;
				//else sstr << "PX " << i;
				////if(i==0||i==18||i==22)
				//render->DrawFont(sttP, sstr.str());
				//sstr.str("");


				render->SetMaterial(GRRenderIf::WHITE);
				Vec3f addVec = Vec3f(0.6, 0.6, 0.6);
		
				Spr::TPose<float> ptclPos;
			
				ptclPos = TPose<float>(sttP, drawObj.objPArr[i].pNewOrint);
		
				ptclPos.x = -ptclPos.x;
				ptclPos.y = -ptclPos.y;
				ptclPos.z = -ptclPos.z;
		
				addVec = ptclPos * addVec;


				render->DrawLine(sttP, addVec);
	
			}
		
		}
}
void FWScene::DrawMesh(GRRenderIf* render, CDConvexMeshIf* mesh, bool solid){
	Vec3f* base = mesh->GetVertices();
	if(solid){
		for (int f=0; f<mesh->NFace(); ++f) {	
			CDFaceIf* face = mesh->GetFace(f);
			this->DrawFaceSolid(render, face, base);
		}
	}
	else{
		for (int f=0; f<mesh->NFace(); ++f) {	
			CDFaceIf* face = mesh->GetFace(f);
			this->DrawFaceWire(render, face, base);
		}
	}
}

void FWScene::DrawFaceSolid(GRRenderIf* render, CDFaceIf* face, Vec3f * base){
	int numIndices = face->NIndex();			// (=3 :三角形なので3頂点)
	struct Vtx{
		Vec3f n;
		Vec3f p;
	} vtxs[10];
	assert((size_t)numIndices <= sizeof(vtxs)/sizeof(vtxs[0]));
	for(int v = 0; v < numIndices; ++v)
		vtxs[v].p = base[face->GetIndices()[v]].data;
	Vec3f edge0, edge1;
	edge0 = vtxs[1].p - vtxs[0].p;
	edge1 = vtxs[2].p - vtxs[0].p;
	Vec3f n = (edge0^edge1).unit();
	for(int v = 0; v < numIndices; ++v)
		vtxs[v].n = n;
	render->SetVertexFormat(GRVertexElement::vfN3fP3f);
	render->DrawDirect(GRRenderBaseIf::TRIANGLE_FAN, vtxs, numIndices);
}

void FWScene::DrawFaceWire(GRRenderIf* render, CDFaceIf* face, Vec3f * base){
	int numIndices = face->NIndex();
	Vec3f vtxs[10];
	assert((size_t)numIndices+1 <= sizeof(vtxs)/sizeof(vtxs[0]));
	int v;
	for(v = 0; v < numIndices; ++v)
		vtxs[v] = base[face->GetIndices()[v]].data;
	vtxs[v] = base[face->GetIndices()[0]].data;
	render->SetVertexFormat(GRVertexElement::vfP3f);
	render->DrawDirect(GRRenderBaseIf::LINES, vtxs, numIndices+1);
}

void FWScene::DrawCoordinateAxis(GRRenderIf* render, float scale, bool solid_or_wire){
	if(axisStyle == FWSceneIf::AXIS_LINES){
		render->SetVertexFormat(GRVertexElement::vfP3f);
		render->PushModelMatrix();
		render->MultModelMatrix(Affinef::Scale(scale, scale, scale));

		float vtx[4][3] = {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};
		GLuint idx_x[2] = {0, 1};
		GLuint idx_y[2] = {0, 2};
		GLuint idx_z[2] = {0, 3};
		render->SetMaterial(matAxis.x);
		render->DrawIndexed(GRRenderBaseIf::LINES, idx_x, (void*)vtx, 2);
		render->SetMaterial(matAxis.y);
		render->DrawIndexed(GRRenderBaseIf::LINES, idx_y, (void*)vtx, 2);
		render->SetMaterial(matAxis.z);
		render->DrawIndexed(GRRenderBaseIf::LINES, idx_z, (void*)vtx, 2);
	
		render->PopModelMatrix();
	}
	if(axisStyle == FWSceneIf::AXIS_ARROWS){
		float rbar  = 0.05f * scale;
		float rhead = 0.10f * scale;
		float lhead = 0.20f * scale;
		// x
		render->SetMaterial(matAxis.x);
		render->DrawArrow(Vec3f(), scale * Vec3f(1.0f, 0.0f, 0.0f), rbar, rhead, lhead, 16, solid_or_wire);
		// y
		render->SetMaterial(matAxis.y);
		render->DrawArrow(Vec3f(), scale * Vec3f(0.0f, 1.0f, 0.0f), rbar, rhead, lhead, 16, solid_or_wire);
		// z
		render->SetMaterial(matAxis.z);
		render->DrawArrow(Vec3f(), scale * Vec3f(0.0f, 0.0f, 1.0f), rbar, rhead, lhead, 16, solid_or_wire);
	}
}

void FWScene::DrawForce(GRRenderIf* render, const Vec3d& f, const Vec3d& t){
	render->SetLighting(false);
	render->SetDepthTest(false);
	// constraint force
	render->SetMaterial(matForce);
	render->DrawLine(Vec3f(), scaleForce * f);
	// constraint moment
	render->SetMaterial(matMoment);
	render->DrawLine(Vec3f(), scaleMoment * t);
			
	render->SetDepthTest(true);
	render->SetLighting(true);
}
void FWScene::DrawContactEngine(GRRenderIf* render, PHContactEngineIf* ceIf) {
	PHContactEngine* ce = ceIf->Cast();
	render->SetLighting(false);
	render->SetDepthTest(false);

	Vec3f newCop = ce->phceInfo.necessaryInfo.ContLocal.inv() * (ce->phceInfo.necessaryInfo.NewCoP - ce->phceInfo.necessaryInfo.ContPoint);
	Vec3f oldCop = ce->phceInfo.necessaryInfo.ContLocal.inv() * (ce->phceInfo.necessaryInfo.OldCoP - ce->phceInfo.necessaryInfo.ContPoint);
	// CoP
	render->SetMaterial(matCoP[1]);
	render->DrawLine(newCop, oldCop);
	render->SetMaterial(matCoP[0]);
	render->SetPointSize(5);
	render->DrawPoint(oldCop);
	render->SetMaterial(matCoP[2]);
	render->DrawPoint(newCop);
	render->SetPointSize(1);

	render->SetDepthTest(true);
	render->SetLighting(true);
}

void FWScene::EnableRenderPHScene(bool enable){
	renderPHScene = enable;
}
void FWScene::EnableRenderGRScene(bool enable){
	renderGRScene = enable;
}
void FWScene::SetRenderMode(bool solid, bool wire){
	renderSolid = solid;
	renderWire  = wire;
}
void FWScene::EnableRender(ObjectIf* obj, bool enable){
	renderObject[obj] = enable;
}
void FWScene::SetSolidMaterial(int mat, PHSolidIf* solid) {
	matSolid[solid] = Mat(mat, 1);
}
void FWScene::SetSolidMaterial(int mat, float alpha, PHSolidIf* solid) {
	matSolid[solid] = Mat(mat, alpha);
}

void FWScene::SetWireMaterial(int mat, PHSolidIf* solid) {
	matWire[solid] = Mat(mat, 1);
}
void FWScene::SetWireMaterial(int mat, float alpha, PHSolidIf* solid) {
	matWire[solid] = Mat(mat, alpha);
}
void FWScene::EnableRenderAxis(bool world, bool solid, bool con){
	renderAxisWorld = world;
	renderAxisSolid = solid;
	renderAxisConst = con;
}
void FWScene::SetAxisMaterial(int matX, int matY, int matZ){
	matAxis = Vec3i(matX, matY, matZ);
}
void FWScene::SetAxisScale(float scaleWorld, float scaleSolid, float scaleConst){
	scaleAxisWorld = scaleWorld;
	scaleAxisSolid = scaleSolid;
	scaleAxisConst = scaleConst;
}
void FWScene::SetAxisStyle(int style){
	axisStyle = style;
}
void FWScene::EnableRenderForce(bool solid, bool con){
	renderForceSolid = solid;
	renderForceConst = con;
}
void FWScene::SetForceMaterial(int matf, int matm){
	matForce	= matf;
	matMoment	= matm;	
}
void FWScene::SetForceScale(float scalef, float scalem){
	scaleForce	= scalef;
	scaleMoment = scalem;
}
void FWScene::EnableRenderContact(bool enable){
	renderContact = enable;
	if (phScene){
		PHConstraintEngineIf* c = phScene->GetConstraintEngine();
		if (c) c->EnableRenderContact(enable);
	}
}
void FWScene::SetContactMaterial(int mat){
	matContact = mat;
}
void FWScene::EnableRenderBBox(bool enable){
	renderBBox = enable;
}
void FWScene::SetLocalBBoxMaterial(int mat) {
	matBBoxWorld = mat;
}
void FWScene::SetWorldBBoxMaterial(int mat) {
	matBBoxLocal = mat;
}
void FWScene::EnableRenderGrid(bool x, bool y, bool z){
	renderGridX = x;
	renderGridY = y;
	renderGridZ = z;
}
void FWScene::SetGridOption(char axis, float offset, float size, int slice){
	int i = tolower(axis) - 'x';
	gridOffset[i] = offset;
	gridSize[i]   = size;
	gridSlice[i]  = slice;
}
void FWScene::SetGridMaterial(int matX, int matY, int matZ){
	matGrid = Vec3i(matX, matY, matZ);
}
void FWScene::EnableRenderIK(bool enable){
	renderIK = enable;
}
void FWScene::SetIKMaterial(int mat){
	matIK = mat;
}
void FWScene::SetIKScale(float scale){
	scaleIK = scale;
}
void FWScene::EnableRenderLimit(bool enable){
	renderLimit = enable;
}
void FWScene::EnableRenderHaptic(bool enable){
	renderHaptic = enable;
}
void FWScene::EnableRenderFem(bool enable){
	renderFEM = enable;
}
void FWScene::EnableRenderSkeletonSensor(bool enable){
	renderSkeletonSensor = enable;
}
void FWScene::EnableRenderOp(bool enable){
	renderOp = enable;
}
bool FWScene::IsRenderEnabled(ObjectIf* obj){
	std::map<ObjectIf*, bool>::iterator it = renderObject.find(obj);
	if(it != renderObject.end())
		return it->second;
	return true;
}
FWScene::Mat FWScene::GetSolidMaterial(PHSolidIf* solid){
	// 最初に特定のsolidにあてられたマテリアルがあるか調べ，
	// なければ次に0 (全剛体)のマテリアルを調べ，
	// どちらもなければ-1を返す
	std::map<PHSolidIf*, Mat>::iterator it;
	it = matSolid.find(solid);
	if(it != matSolid.end())
		return it->second;
	it = matSolid.find(0);
	if(it != matSolid.end())
		return it->second;
	return Mat(-1, 1);
}
FWScene::Mat FWScene::GetWireMaterial(PHSolidIf* solid){
	std::map<PHSolidIf*, FWScene::Mat>::iterator it;
	it = matWire.find(solid);
	if(it != matWire.end())
		return it->second;
	it = matWire.find(0);
	if(it != matWire.end())
		return it->second;
	return Mat(-1, 1);
}
FWScene::Mat FWScene::GetAutoMaterial(int i){
	/// iがひとつ増えるたびに色系統が変わるように色を選択する
	const int colorGroups		= 8;  // 系統の数　　　白、灰色系は避けたので８系統
	const int colorsPerGroup	= 5;  // 系統内の色数　オレンジ系が５色しかないのであわせる
	GRRenderIf::TMaterialSample groupTop[] = {
		GRRenderIf::INDIANRED,
		GRRenderIf::PINK,
		GRRenderIf::CORAL,
		GRRenderIf::GOLD,
		GRRenderIf::LAVENDER,
		GRRenderIf::GREENYELLOW,
		GRRenderIf::AQUA,
		GRRenderIf::CORNSILK,
	};

	int cycle  = (i % (colorGroups * colorsPerGroup));
	int group  = cycle / colorsPerGroup;
	int offset = cycle % colorsPerGroup;
	int color  = groupTop[i] + offset;

	return Mat(color, 1);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// HumanInterface系

void FWScene::AddHumanInterface(HIForceDevice6D* d){
	//	hase	TBW
	//	humanInterfaces.push_back(d);
}

FWHapticPointerIf* FWScene::CreateHapticPointer(){
	UTRef< FWHapticPointer > fwHapticPointer = DBG_NEW FWHapticPointer;
	fwHapticPointers.push_back(fwHapticPointer->Cast());
	return fwHapticPointer->Cast();
}
FWHapticPointerIf*	FWScene::GetHapticPointer(int i){
	return fwHapticPointers[i];
}
int FWScene::NHapticPointers(){
	return (int)fwHapticPointers.size();
}
void FWScene::UpdateHapticPointers(){
	assert(this);
	PHHapticEngine* he = GetPHScene()->GetHapticEngine()->Cast();
	if(he->GetHapticStepMode() == PHHapticEngineDesc::SINGLE_THREAD){	// single thread
		for(int i = 0; i < NHapticPointers(); i++){
			FWHapticPointer* fp = GetHapticPointer(i)->Cast();
			fp->UpdateHumanInterface(fp->GetPHHapticPointer()->Cast(), GetPHScene()->GetHapticTimeStep());
		}
	}else{	// multi thread
		if(he->NPointersInHaptic()==0) return;
		for(int i = 0; i < NHapticPointers(); i++){
			FWHapticPointer* fp = GetHapticPointer(i)->Cast();
			PHHapticPointer* plp = (PHHapticPointer*)he->GetPointerInHaptic(i);
			fp->UpdateHumanInterface(plp, GetPHScene()->GetHapticTimeStep());
		}
	}
}

FWOpHapticHandlerIf* FWScene::CreateOpHapticHandler(){
	fwOpHapticHandler = DBG_NEW FWOpHapticHandler();
	
	return XCAST(fwOpHapticHandler);
}

FWOpHapticHandlerIf* FWScene::GetOpHapticHandler(){
	return XCAST(fwOpHapticHandler);
}

void FWScene::UpdateOpHapticHandler()
{
	PHOpEngineIf* opEngine = phScene->GetOpEngine();
	fwOpHapticHandler->UpdateHumanInterface(DCAST(PHOpHapticController, opEngine->GetOpHapticController()), fwOpHapticHandler->GetHapticTimeInterval());

}

}
