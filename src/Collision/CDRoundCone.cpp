/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Springhead.h>
#include <Foundation/Object.h>
#include "Collision.h"
#include <set>
#pragma hdrstop
#include "CDQuickHull3DImp.h"

namespace Spr{;

//----------------------------------------------------------------------------
//	CDRoundCone
CDRoundCone::CDRoundCone() {
}

CDRoundCone::CDRoundCone(const CDRoundConeDesc& desc){
	radius = desc.radius;
	length = desc.length;
	material = desc.material;
}

bool CDRoundCone::IsInside(const Vec3f& p){
	return (p - Vec3f(0.0f, 0.0f, 0.5f * -length)).square() < radius[0] * radius[0] ||
		   (p - Vec3f(0.0f, 0.0f, 0.5f * +length)).square() < radius[1] * radius[1] ||
		   (p.x*p.x + p.y*p.y < radius * radius && -0.5f*length < p.z && p.z < 0.5f*length);
}
float CDRoundCone::CalcVolume(){
	return  2.0f/3.0f * (float)M_PI * radius[0] * radius[0] * radius[0] +
			2.0f/3.0f * (float)M_PI * radius[1] * radius[1] * radius[1] +
			(float)M_PI * (radius[0]*radius[0] + radius[0]*radius[1] + radius[1]*radius[1]) * length / 3.0f;
}
	
// ƒTƒ|[ƒgƒ|ƒCƒ“ƒg‚ğ‹‚ß‚é
Vec3f CDRoundCone::Support(const Vec3f& p) const {
	float normal_Z = (radius[0] - radius[1]) / length;
	float n = p.norm();
	Vec3f dir;
	if(n < 1.0e-10f){
		dir = Vec3f();
	}else{
		dir = p / n;
	}

	if (-1 < normal_Z && normal_Z < 1) {
		if (normal_Z < dir.Z()) {
			// p‚Ì•û‚ªZ²‘O•û ¨ radius[1]‚ğg—p
			return dir*radius[1] + Vec3f(0,0, length/2.0);
		} else {
			// p‚Ì•û‚ªZ²Œã•û ¨ radius[0]‚ğg—p
			return dir*radius[0] + Vec3f(0,0,-length/2.0);
		}
	} else {
		// ‚Ç‚¿‚ç‚©‚Ì‹…‚É•ïŠÜ‚³‚ê‚Ä‚¢‚é
		if (radius[0] < radius[1]) {
			return dir*radius[1] + Vec3f(0,0, length/2.0);
		} else {
			return dir*radius[0] + Vec3f(0,0,-length/2.0);
		}
	}
}

// Ø‚èŒû‚ğ‹‚ß‚é. ÚG‰ğÍ‚ğs‚¤.
bool CDRoundCone::FindCutRing(CDCutRing& ring, const Posed& toW) {
	//	Ø‚èŒû(ring.local)Œn‚Å‚Ì ƒJƒvƒZƒ‹‚ÌŒü‚«
	Vec3f dir = ring.localInv.Ori() * toW.Ori() * Vec3f(0,0,1);
	Vec3f center = ring.localInv * toW.Pos();
	float sign = center.X() > 0.0f ? 1.0f : -1.0f;
	//	sinA : (r1-r0)/length ‚É‚È‚éB
	//	sinB : Cutring–Ê‚Æ‰~“›–Ê‚Ìü‚Æ‚Ì‚È‚·Šp‚ª B
	//	sinA+B = dir.X() ‚É‚È‚éB
	float sinA = (radius[1]-radius[0]) / length;
	float sinB = dir.X()*sign * sqrt(1-sinA*sinA)  -  sqrt(1-dir.X()*dir.X()) * sinA;

	float r = radius[0];
	if (sinB < 0){
		dir = -dir;
		sinA *= -1;
		sinB *= -1;
		r = radius[1];
	}
	center = center - (length/2) * dir;
	if (sinB < 0.3f) { // ‘¤–Ê‚ªÚG–Ê‚É‚Ù‚Ú•½s‚Èê‡
		float shrink = sqrt(1-dir.X()*dir.X());	//	ŒX‚¢‚Ä‚¢‚é‚½‚ß‚É‹——£‚ªk‚ŞŠ„‡
		float start = -0.0f*length*shrink;
		float end = 1.0f*length*shrink;

		if (sinB > 1e-4){	//	Š®‘S‚É•½s‚Å‚È‚¢ê‡
			float depth = r/shrink - sign*center.X();
			float cosB = sqrt(1-sinB*sinB);
			float is = depth / sinB * cosB;	//	ÚG–Ê‚Æ’†Sü‚ğ”¼Œa‚¸‚ç‚µ‚½ü‚Æ‚ÌŒğ“_
			if (is < end) end = is;
			if (end+1e-4 < start){//0.001 < start){
				DSTR << "CDRoundCone::FindCutRing() may have a problem" << std::endl;
			}
			if (end <= start) return false;
		}
		//	ring‚Éü•ª‚ğ’Ç‰Á
		float lenInv = 1/sqrt(dir.Y()*dir.Y() + dir.Z()*dir.Z());
		ring.lines.push_back(CDCutLine(Vec2f(-dir.Y(), -dir.Z())*lenInv, -start));
		ring.lines.push_back(CDCutLine(Vec2f(dir.Y(), dir.Z())*lenInv, end));
		ring.lines.push_back(CDCutLine(Vec2f(dir.Z(), -dir.Y())*lenInv, 0));
		ring.lines.push_back(CDCutLine(Vec2f(-dir.Z(), dir.Y())*lenInv, 0));
		return true;
	}
	return false;
}

Vec3d CDRoundCone::Normal(Vec3d p){
	Vec2d	r = radius;
	double	l = length;

	// RoundCone‚Ì‘¤–Ê‚ÌŠp“xi‘¤–Ê‚ªZ²‚É‚’¼‚È‚Æ‚«0‹A•½s(‚Â‚Ü‚èƒJƒvƒZƒ‹Œ^)‚Ì‚Æ‚«90‹j
	double theta = acos((r[1]-r[0])/l);

	if ( (p[2] > (r[0]*cos(theta) + l/2.0)) || (p[2] < (r[1]*cos(theta) - l/2.0)) ) {
		// ÚG“_‚ª‚Ç‚¿‚ç‚©‚Ì‹…‘Ì‚É‚ ‚éê‡F
		return p.unit();
	} else {
		// ÚG“_‚ª‹…‘Ì‚Æ‹…‘Ì‚ÌŠÔ‚É‚ ‚éê‡F
		Vec3d pNormal = p;
		pNormal[2] = 0; pNormal = pNormal.unit() * sin(theta);
		pNormal[2] = cos(theta);
		return pNormal;
	}
}

double CDRoundCone::CurvatureRadius(Vec3d p){
	/// ‰º‹L‚ÌŒvZ‚ÍŒ»“_‚ÅƒoƒO‚à‚¿B
	/// RoundCone‚Ì—¼’[‚Ì”¼Œa‚ª“¯‚¶‚Å‚ ‚é‚Æ‚«‚ÉIND‚É‚È‚é
	/// ‚¢‚¸‚êC³‚·‚é‚±‚ÆI ('09/02/17, mitake)

	Vec2d	r = radius;
	double	l = length;

	// RoundCone‚Ì‘¤–Ê‚ÌŠp“xi‘¤–Ê‚ªZ²‚É‚’¼‚È‚Æ‚«0‹A•½s(‚Â‚Ü‚èƒJƒvƒZƒ‹Œ^)‚Ì‚Æ‚«90‹j
	double theta = acos((r[1]-r[0])/l);
	// ÚG“_‚ÌZÀ•W
	double Zc = p[2];

	/*
	ÚG“_‚ª‚Ç‚¿‚ç‚©‚Ì‹…‘Ì‚É‚ ‚éê‡F
	*/
	if (Zc > (r[0]*cos(theta) + l/2.0)) {
		return r[0];
	} else if (Zc < (r[1]*cos(theta) - l/2.0)) {
		return r[1];
	}

	/*
	ÚG“_‚ª‹…‘Ì‚Æ‹…‘Ì‚ÌŠÔ‚É‚ ‚éê‡F
	ÚG“_‚ğ’Ê‚èAÚG“_‚ÌˆÊ’u‚Å‘¤–Ê‚Æ’¼Œğ‚·‚é’f–Ê‚É‚¨‚¯‚éRoundCone‚ÌØ’f‚ğl‚¦‚éB
	‚»‚Ì’f–Ê‚Í‘È‰~‚Æ‚È‚èA‚»‚Ì‘È‰~‚ÌÅ¬‹È—¦”¼Œai’·²‚Ì’[‚É‚¨‚¯‚é‹È—¦”¼Œaj‚ª‹‚ß‚é‚à‚Ì‚Æ‚È‚éB
	*/

	/// -- RoundCone‚ğ‰„’·‚µ‚½‰~‚Ì’¸“_‚ÌZÀ•W
	double Z0 = l/2*(r[1]+r[0])/(r[1]-r[0]);

	/// -- Z-RÀ•WŒn‚ÅŒ©‚½‚Æ‚«‚Ì‘¤–Ê‚ÌŒX‚«
	double M1 = tan(Rad(90) - theta);
	/// -- Z-RÀ•WŒn‚ÅŒ©‚½‚Æ‚«‚Ì’f–Ê‚ÌŒX‚«
	double M2 = tan(theta);

	/// -- Ø’f–Ê‚ÌŒü‚±‚¤‘¤‚Ì“_
	double Za = (M1*Z0-M2*Zc)/(M1-M2);
	Vec2d pA = Vec2d(Za, -M1*Za + M1*Z0);
	/// -- Ø’f–Ê‚Ì‚±‚Á‚¿‘¤‚Ì“_
	double Zb = (M1*Z0+M2*Zc)/(M1+M2);
	Vec2d pB = Vec2d(Zb,  M1*Zb - M1*Z0);
	/// -- RoundCone‚ğ‰„’·‚µ‚½‰~‚Ì’¸“_
	Vec2d p0 = Vec2d(Z0, 0);

	/// -- Ø’f‘È‰~‚Ì‘å‚«‚³‚ğl‚¦‚é‚½‚ß‚ÌOŠpŒ`‚ÌO•Ó
	double Ta = (pB - p0).norm(), Tb = (pA - p0).norm(), Tc = (pA - pB).norm();
	/// -- Ø’f‘È‰~‚Ì’·²”¼Œa
	double eL = Tc/2;
	/// -- Ø’f‘È‰~‚Ì’†S‚©‚çÅ“_‚Ü‚Å‚Ì‹——£i’·²”¼Œa‚©‚çTa,Tb,Tc‚©‚ç‚È‚éOŠpŒ`‚Ì“àÚ‰~”¼Œa‚ğˆø‚¢‚½‚à‚Ì‚É‚È‚éj
	double f = eL - (Ta*Tc)/(Ta+Tb+Tc);
	/// -- Ø’f‘È‰~‚Ì’Z²”¼Œa
	double eS = sqrt(eL*eL - f*f);
	/// -- Ø’f‘È‰~‚ÌÅ¬‹È—¦”¼Œa
	double Rmin = eS*eS / eL;

	return Rmin;
}

Vec2f CDRoundCone::GetRadius() {
	return radius;
}
float CDRoundCone::GetLength() {
	return length;
}
void CDRoundCone::SetRadius(Vec2f r) {
	radius=r;
}
void CDRoundCone::SetLength(float l) {
	length=l;
}
void CDRoundCone::SetWidth(Vec2f r) {
	Vec2f radiusMargin=r-radius;
	
	length-=(radiusMargin.x+radiusMargin.y);
	radius=r;
}

}	//	namespace Spr
