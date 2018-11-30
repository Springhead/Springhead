/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHBody.h>
#include <Physics/PHScene.h>
#include <Physics/PHPenaltyEngine.h>
#include <Physics/PHConstraintEngine.h>
#include <Physics/PHHapticEngine.h>
#include <Collision/CDShape.h>

#include <float.h>

using namespace PTM;
namespace Spr{;


//----------------------------------------------------------------------------
//	PHBBox
void PHBBox::GetSupport(const Vec3f& dir, float& minS, float& maxS) {
	Vec3f ext0(bboxExtent.X(), bboxExtent.Y(), bboxExtent.Z());
	Vec3f ext1(-bboxExtent.X(), bboxExtent.Y(), bboxExtent.Z());
	Vec3f ext2(bboxExtent.X(), -bboxExtent.Y(), bboxExtent.Z());
	Vec3f ext3(bboxExtent.X(), bboxExtent.Y(), -bboxExtent.Z());
	float d = abs(dir*ext0);
	float d1 = abs(dir*ext1);
	if (d < d1) d = d1;
	float d2 = abs(dir*ext2);
	if (d < d2) d = d2;
	float d3 = abs(dir*ext3);
	if (d < d3) d = d3;
	float c = dir * bboxCenter;
	minS = c - d;
	maxS = c + d;
}

void PHBBox::GetBBoxWorldMinMax(Posed& pose, Vec3d& _min, Vec3d& _max) {
	Vec3d ext[8];
	Vec3d bb = bboxExtent;
	ext[0] = Vec3f(bb.x, bb.y, bb.z);
	ext[1] = Vec3f(-bb.x, bb.y, bb.z);
	ext[2] = Vec3f(bb.x, -bb.y, bb.z);
	ext[3] = Vec3f(bb.x, bb.y, -bb.z);
	ext[4] = Vec3f(-bb.x, -bb.y, bb.z);
	ext[5] = Vec3f(bb.x, -bb.y, -bb.z);
	ext[6] = Vec3f(-bb.x, bb.y, -bb.z);
	ext[7] = Vec3f(-bb.x, -bb.y, -bb.z);

	for (int i = 0; i < 8; i++)
		ext[i] = pose * (bboxCenter + ext[i]);

	_min = ext[0];
	_max = ext[0];

	for (int i = 1; i < 8; i++) {
		for (int k = 0; k < 3; k++) {
			_min[k] = std::min(_min[k], ext[i][k]);
			_max[k] = std::max(_max[k], ext[i][k]);
		}
	}
}
bool PHBBox::Intersect(PHBBox& lhs, PHBBox& rhs) {
	Vec3f bbmin[2];
	Vec3f bbmax[2];
	bbmin[0] = lhs.GetBBoxMin();
	bbmax[0] = lhs.GetBBoxMax();
	bbmin[1] = rhs.GetBBoxMin();
	bbmax[1] = rhs.GetBBoxMax();
	for (int k = 0; k < 3; k++) {
		if (bbmax[0][k] < bbmin[1][k])
			return false;
		if (bbmax[1][k] < bbmin[0][k])
			return false;
	}
	return true;
}



//----------------------------------------------------------------------------
//	PHFrame
PHFrame::PHFrame() :body(NULL), shape(NULL) {
	bboxReady = false;
}
PHFrame::PHFrame(PHBody* so, CDShape* sh) : body(so), shape(sh) {
	bboxReady = false;
}
PHFrame::PHFrame(const PHFrameDesc& desc) : PHFrameDesc(desc), body(NULL), shape(NULL) {
	bboxReady = false;
}

ObjectIf* PHFrame::GetChildObject(size_t pos) {
	if (pos == 0) return (ObjectIf*)shape;
	return NULL;
}
bool PHFrame::AddChildObject(ObjectIf * o) {
	CDShape* s = o->Cast();
	if (s) {
		if (!shape) {
			shape = s;
		}
		else {
			assert(body);
			PHFrame* f = DBG_NEW PHFrame;
			f->shape = s;
			body->AddFrame(f->Cast());
		}
		if (body) {
			body->bboxReady = false;
			body->aabbReady = false;
			//接触エンジンのshapePairsを更新する
			PHScene* scene = DCAST(PHScene, body->GetScene());
			scene->penaltyEngine->UpdateShapePairs(body);
			scene->constraintEngine->UpdateShapePairs(body);
			scene->hapticEngine->UpdateShapePairs(body);
		}
		return true;
	}
	return false;
}
bool PHFrame::DelChildObject(ObjectIf* obj) {
	CDShape* s = obj->Cast();
	if (s == shape) {
		shape = NULL;
		if (body) body->DelChildObject(Cast());
		return true;
	}
	return false;
}
size_t PHFrame::NChildObject() const {
	if (shape) return 1;
	return 0;
}
Posed PHFrame::GetPose() {
	return pose;
}
void PHFrame::SetPose(Posed p) {
	pose = p;
	pose_abs = body->GetPose() * p;
	bboxReady = false;
}
bool PHFrame::CalcBBox() {
	CDConvex* convex = shape->Cast();
	if (!convex)
		return false;
	if (bboxReady && convex->bboxReady)
		return false;

	Vec3f _min, _max;
	float inf = std::numeric_limits<float>::max();
	_min = Vec3f(inf, inf, inf);
	_max = Vec3f(-inf, -inf, -inf);
	shape->CalcBBox(_min, _max, Posed());
	bbShape.SetBBoxMinMax(_min, _max);

	_min = Vec3f(inf, inf, inf);
	_max = Vec3f(-inf, -inf, -inf);
	shape->CalcBBox(_min, _max, pose);
	bbBody.SetBBoxMinMax(_min, _max);

	convex->bboxReady = true;
	bboxReady = true;
	return true;
}
void PHFrame::CalcAABB() {
	// 形状ローカルのBBoxを囲うAABBを作る
	// 形状自体からAABBを作った方が無駄は無いが計算がかさむ
	pose_abs = body->GetPose() * pose;

	Vec3d bbmin, bbmax;
	bbShape.GetBBoxWorldMinMax(pose_abs, bbmin, bbmax);
	bbWorld[0].SetBBoxMinMax(bbmin, bbmax);

	// CCD用は前時刻からの並進移動量を考慮してAABBを膨らませる
	for (int k = 0; k < 3; k++) {
		Vec3f dir;
		if (delta[k] < 0)
			bbmin[k] += delta[k];
		else bbmax[k] += delta[k];
	}
	bbWorld[1].SetBBoxMinMax(bbmin, bbmax);

}

void PHFrame::CompInertia() {
	mass = shape->GetMaterial().density * shape->CalcVolume();
	center = shape->CalcCenterOfMass();
	inertia = shape->GetMaterial().density * shape->CalcMomentOfInertia();
}

//----------------------------------------------------------------------------
//	PHBody
//

int PHBody::NFrame() { return (int)frames.size(); }
PHFrameIf* PHBody::GetFrame(int i) {
	if (i >= (int)frames.size()) return NULL;
	return frames[i]->Cast();
}
void PHBody::AddFrame(PHFrameIf* fi) {
	PHFrame* f = fi->Cast();
	assert(f);
	frames.push_back(f->Cast());
	frames.back()->body = this;
	if (frames.back()->shape) {
		bboxReady = false;
		aabbReady = false;
		//接触エンジンのshapePairsを更新する
		PHScene* scene = DCAST(PHScene, GetScene());
		scene->penaltyEngine->UpdateShapePairs(this);
		scene->constraintEngine->UpdateShapePairs(this);
		scene->hapticEngine->UpdateShapePairs(this);
	}
}
/*void PHBody::DelFrame(int i){
frames.erase(frames.begin()+i);
CalcBBox();
//接触エンジンのshapePairsを更新する
PHScene* scene = DCAST(PHScene, GetScene());
scene->penaltyEngine   ->DelShapePairs(this, i);
scene->constraintEngine->DelShapePairs(this, i);
}*/
void PHBody::AddShape(CDShapeIf* shape) {
	frames.push_back(DBG_NEW PHFrame(this, shape->Cast()));
	bboxReady = false;
	aabbReady = false;
	PHScene* scene = DCAST(PHScene, GetScene());
	scene->penaltyEngine->UpdateShapePairs(this);
	scene->constraintEngine->UpdateShapePairs(this);
	scene->hapticEngine->UpdateShapePairs(this);
}
void PHBody::AddShapes(CDShapeIf** shBegin, CDShapeIf** shEnd) {
	for (CDShapeIf** sh = shBegin; sh != shEnd; sh++) {
		CDShape* shape = DCAST(CDShape, *sh);
		frames.push_back(DBG_NEW PHFrame());
		frames.back()->shape = shape;
	}
	bboxReady = false;
	aabbReady = false;
	PHScene* scene = DCAST(PHScene, GetScene());
	scene->penaltyEngine->UpdateShapePairs(this);
	scene->constraintEngine->UpdateShapePairs(this);
	scene->hapticEngine->UpdateShapePairs(this);
}
void PHBody::RemoveShape(int i) {
	RemoveShapes(i, i + 1);
}
void PHBody::RemoveShapes(int iBegin, int iEnd) {
	if (iBegin < 0 || iBegin >= (int)frames.size())
		return;
	if (iEnd   < 1 || iEnd   >(int)frames.size())
		return;
	if (iEnd <= iBegin)
		return;
	int n = iEnd - iBegin;
	for (int i = 0; i < n; i++)
		frames.erase(frames.begin() + iBegin);
	bboxReady = false;
	aabbReady = false;
	//接触エンジンのshapePairsを更新する
	PHScene* scene = DCAST(PHScene, GetScene());
	scene->penaltyEngine->DelShapePairs(this, iBegin, iEnd);
	scene->constraintEngine->DelShapePairs(this, iBegin, iEnd);
}
void PHBody::RemoveShape(CDShapeIf* shape) {
	CDShape* sh = DCAST(CDShape, shape);
	for (unsigned i = 0; i<frames.size(); ++i) {
		if (frames[i]->shape == sh) {
			RemoveShape(i);
			i--;
		}
	}
}
Posed	PHBody::GetShapePose(int i) {
	if (0 <= i && i < (int)frames.size())
		return frames[i]->pose;
	return Posed();
}
void PHBody::ClearShape() {
	frames.clear();
}

void PHBody::SetShapePose(int i, const Posed& pose) {
	if (0 <= i && i < (int)frames.size()) {
		frames[i]->SetPose(pose);
		bboxReady = false;
		aabbReady = false;
	}
}
int PHBody::NShape() {
	return (int)frames.size();
}
CDShapeIf* PHBody::GetShape(int i) {
	return frames[i]->shape->Cast();
}


}
