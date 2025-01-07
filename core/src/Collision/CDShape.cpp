/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Collision/CDShape.h>

namespace Spr{;

PHMaterial::PHMaterial(){
	e = 0.4f;
	density = 1000.0f;	//	Water's density in MKS
	spring = 0.0f;
	damper = 0.0f;

	reflexSpring = 5.0f;
	reflexDamper = 0.1f;
	frictionSpring = 1500.0f;// 5.0;
	frictionDamper = 0.0f;//0.1f;
	vibA = -20;
	vibB = 90;
	vibW = 200;
	vibT = 0;
	vibContact = false;

	mu = 0.35f;
	mu0 = 0.5f;
	rotationFriction = 0.001f;

	/**	時変摩擦係数のための係数	ms = mu+ A log(1+Bt),  md = mu+ A log (1+B C/v)  [Dieterich 1979]
		Cは動摩擦時のスリップ距離を意味する C/v < dt(時間刻み) のときは、v=C/dt を使う */
	timeVaryFrictionA = 0.1f;
	timeVaryFrictionB = 1800;
	timeVaryFrictionC = 0.0f;
	///	粘性摩擦のための係数	f_t = frictionViscocity * vel * f_N
	frictionViscosity = 0.0f;
	stribeckVelocity = 0.1f;
	stribeckmu = 0.1f;

	///LuGre Modelのための係数
	bristlesSpringK = 4000.0;//100000.0;			///< LuGreモデルにおける剛毛のバネ係数
	bristlesDamperD = 4.0;//sqrt(100000.0);	///< LuGreモデルにおける剛毛のダンパ係数
	bristlesViscosityV = 0.4;//0.4f;			///< LuGreモデルにおける剛毛にはたらく粘性抵抗の係数


//GMS用
	timeVaryFrictionAs = { 0.0001f,0.0003f,0.005f };
	timeVaryFrictionBs = { 1800,1500,2500 };
	timeVaryFrictionCs = { 0.0f,0.0f,0.0f };
	timeVaryFrictionDs = { 0.0f,0.0f,0.0f };
	mus = { 0.02f,0.07f,0.03f };
	mu0s = { 0.5f,0.5f,0.50f };
	stribeckmus = { 0.005f,0.007f,0.009f };
	stribeckVelocitys = { 0.4f,0.5f,0.2f };
	c = { 0.01f,0.03f,0.05f };
	bristleK = { 1000.0f,700.0f,800.0f };

	

	velocityFieldMode      = VelocityField::NONE;
	velocityFieldAxis      = Vec3d();
	velocityFieldMagnitude = 0.0;
}

Vec3d PHMaterial::CalcVelocity(const Vec3d& pos, const Vec3d& normal) const{
	if(velocityFieldMode == VelocityField::NONE){
		return Vec3d();
	}
	if(velocityFieldMode == VelocityField::LINEAR){
		return velocityFieldAxis * velocityFieldMagnitude;
	}
	if(velocityFieldMode == VelocityField::CYLINDER){
		Vec3d  v  = velocityFieldAxis % pos;
		double vn = v * normal;
		Vec3d  vt = v - vn * normal;
		const double eps = 1.0e-10;
		double vtnorm = vt.norm();
		if(vtnorm < eps)
			return Vec3d();
		vt *= (velocityFieldMagnitude/vtnorm);
		return vt;
	}
	return Vec3d();
}

//-------------------------------------------------------------------------------------------------

float CDShape::CalcCylinderVolume(float r, float l){
	return (float)M_PI * r * r * l;
}

Matrix3f CDShape::CalcCylinderInertia(float r, float l){
	float Ix = (1.0f/4.0f) * r*r + (1.0f/12.0f) * l*l;
	float Iz = (1.0f/2.0f) * r*r;
	return CalcCylinderVolume(r, l) * Matrix3f::Diag(Ix, Ix, Iz);
}

float CDShape::CalcHemisphereCoM(float r){
	return (3.0f/8.0f) * r;
}

float CDShape::CalcHemisphereVolume(float r){
	return (4.0f/3.0f) * (float)M_PI * r*r*r;
}

Matrix3f CDShape::CalcHemisphereInertia(float r){
	float I = (2.0f/5.0f) * r*r;
	return CalcHemisphereVolume(r) * Matrix3f::Diag(I, I, I);
}

float CDShape::CalcConeCoM(float l){
	return (1.0f/4.0f) * l;
}

float CDShape::CalcConeVolume(float r, float l){
	return (1.0f/3.0f) * (float)M_PI * r*r * l;
}

Matrix3f CDShape::CalcConeInertia(float r, float l){
	float Ix = (3.0f/20.0f) * r*r + (1.0f/10.0f) * l*l;
	float Iz = (3.0f/10.0f) * r*r;
	return CalcConeVolume(r, l) * Matrix3f::Diag(Ix, Ix, Iz);
}

float CDShape::CalcTetrahedronVolume(const Vec3f& a, const Vec3f& b, const Vec3f& c){
	return (1.0f/6.0f) * std::abs(a * (b % c));
}
Vec3f CDShape::CalcTetrahedronCoM(const Vec3f& a, const Vec3f& b, const Vec3f& c){
	return (1.0f/4.0f) * (a + b + c);
}
Matrix3f CDShape::CalcTetrahedronInertia(const Vec3f& a, const Vec3f& b, const Vec3f& c){
	// 正準四面体からの変換行列
	Matrix3f A;
	A.col(0) = a;
	A.col(1) = b;
	A.col(2) = c;
	// 正準四面体のC行列
	float c0 = (1.0f/60.0f);
	float c1 = (1.0f/120.0f);
	Matrix3f C(
		c0, c1, c1,
		c1, c0, c1,
		c1, c1, c0);
	// C行列の変換
	Matrix3f Cd = std::abs(A.det()) * (A * C * A.trans());
	Matrix3f Id = (Cd[0][0] + Cd[1][1] + Cd[2][2]) * Matrix3f::Unit() - Cd;
	return Id;
}
float CDShape::CalcRoundConeVolume(Vec2f radius,float length)
{
	float sinTheta = (radius[1] - radius[0]) / length;
	float cosTheta = sqrt(1 - pow((radius[1] - radius[0]) / length, 2.0f));
	float zMinus = -(length / 2.0f) - radius[0];
	float zZero = -(length / 2.0f) - radius[0] * sinTheta;
	float zOne = length / 2.0f - radius[1] * sinTheta;
	float zPlus = length / 2.0f + radius[1];

	float volumeR0 = M_PI * radius[0]*radius[0] * (zZero - zMinus) -
		(M_PI / 3.0f)*(pow((zZero + length / 2.0f), 3.0f) - pow(zMinus + length/2.0f, 3.0f));
	float volumeR1 = M_PI * radius[1] * radius[1] * (zPlus - zOne) +
		(M_PI / 3.0f)*(pow(length / 2.0f - zPlus, 3.0f) - pow(length / 2.0f - zOne, 3.0f));
	//float volumeCone = M_PI * pow((radius[0] + radius[1]) * cosTheta,2.0f)/4.0f*(length - radius[0]*sinTheta + radius[1] * sinTheta);
	float volumeCone = (1.0f / 3.0f)*length * M_PI *(pow(radius[0],2.0f) + radius[0]*radius[1] + pow(radius[1],2.0f));
	//DSTR << " length " << length << " r0 " << radius[0] << " r1 " <<radius[1] << 
	//	"sinTheta " << sinTheta << " cosTheta " << cosTheta <<
	//	" zMinus " << zMinus << " zZero " << zZero << " zOne " << zOne <<
	//	" zPlus " << zPlus << std::endl << " volumeR0 " << volumeR0 << " volumeR1 " << volumeR1 << " volumeCone " << volumeCone <<std::endl;
	
	return volumeR0 + volumeR1 + volumeCone;
}
Vec3f CDShape::CalcRoundConeCenterOfMass(Vec2f radius,float length) {
	float sinTheta = (radius[1] - radius[0]) / length;
	float cosTheta = sqrt(1 - pow((radius[1] - radius[0]) / length, 2.0f));
	float tanTheta = sinTheta / cosTheta;
	float zMinus = -(length / 2.0f) - radius[0];
	float zZero = -(length / 2.0f) - radius[0] * sinTheta;
	float zOne = length / 2.0f - radius[1] * sinTheta;
	float zPlus = length / 2.0f + radius[1];

	// 球の中にすべて含まれる場合
	if((radius[0] >= radius[1]) && radius[0] >= length + radius[1]){
		return Vec3f(0, 0, -length / 2);
	}else if ((radius[1] >= radius[0]) && radius[1] >= length + radius[0]) {
		return Vec3f(0, 0, length / 2);
	}
	else {
		float r0 = M_PI * (pow(radius[0], 2.0f) * (pow(zZero, 2.0f) - pow(zMinus, 2.0f)) / 2.0f -
			(zZero*pow(length / 2.0f + zZero, 3.0f) -
				zMinus * pow(length / 2.0f + zMinus, 3.0f)) / 3.0f +
				(pow(length / 2 + zZero, 4.0f) - pow(length / 2 + zMinus, 4.0f)) / 12.0f);
		float r1 = M_PI * (pow(radius[1], 2.0f) * (pow(zPlus, 2.0f) - pow(zOne, 2.0f)) / 2.0f +
			(zPlus*pow(length / 2.0f - zPlus, 3.0f) -
				zOne * pow(length / 2.0f - zOne, 3.0f)) / 3.0f +
				(pow(length / 2 - zPlus, 4.0f) - pow(length / 2 - zOne, 4.0f)) / 12.0f);

		float tempVar = radius[0] * cosTheta + length * tanTheta / 2.0f + radius[0] * sinTheta*tanTheta;
		float cone = 0;
		// tanThetaが0の時0割になってしまうのでその場合は0にする
		if (abs(tanTheta) >= 1.0e-05) {
			cone = M_PI * ((zOne*pow(zOne*tanTheta + tempVar, 3.0f) -
				zZero * pow(zZero*tanTheta + tempVar, 3.0f)) / (3.0f * tanTheta) -
				((pow(zOne*tanTheta + tempVar, 4.0f) -
					pow(zZero*tanTheta + tempVar, 4.0f)) / (12.0f * pow(tanTheta, 2.0f))));
		}
		//float cone = density * M_PI * ((pow(zOne, 2.0f) - pow(zZero, 2.0f))*
		//	(radius[0] * cosTheta + length * tanTheta / 2.0f + radius[0] * sinTheta*tanTheta) / 2.0f +
		//	(pow(zOne, 3.0f) - pow(zZero, 3.0f))*tanTheta / 3.0f);

		//DSTR << " length " << length << " r0 " << radius[0] << " r1 " << radius[1] <<
		//	"sinTheta " << sinTheta << " cosTheta " << cosTheta << " tanTheta " << tanTheta <<
		//	" zMinus " << zMinus << " zZero " << zZero << " zOne " << zOne <<
		//	" zPlus " << zPlus << std::endl << " r0 " << r0 << " r1 " << r1 <<
		//	" cone " << cone << " mu " << density << " volume " << CalcVolume() <<
		//	" M " << CalcVolume()*density << " tasu " << r0 + r1 + cone <<
		//	" ans " << (r0 + r1 + cone) / (CalcVolume()*density) << std::endl;
		//DSTR << pow(radius[0], 2.0f) * (pow(zZero, 2.0f) - pow(zMinus, 2.0f)) / 2.0f << " " <<
		//	pow(radius[1], 2.0f) * (pow(zPlus, 2.0f) - pow(zOne, 2.0f)) / 2.0f << std::endl;
		//DSTR << (zZero*pow(length / 2.0f + zZero, 3.0f) -
		//	zMinus * pow(length / 2.0f + zMinus, 3.0f)) / 3.0f << " " <<
		//	(zPlus*pow(length / 2.0f - zPlus, 3.0f) -
		//		zOne * pow(length / 2.0f - zOne, 3.0f)) / 3.0f << std::endl;
		return Vec3f(0, 0, (r0 + r1 + cone) / (CDShape::CalcRoundConeVolume(radius, length)));
	}
}
Matrix3f CDShape::CalcRoundConeMomentOfInertia(Vec2f radius, float length){
	float sinTheta = (radius[1] - radius[0]) / length;
	float cosTheta = sqrt(1 - pow((radius[1] - radius[0]) / length, 2.0f));
	float tanTheta = sinTheta / cosTheta;
	float zMinus = -(length / 2.0f) - radius[0];
	float zZero = -(length / 2.0f) - radius[0] * sinTheta;
	float zOne = length / 2.0f - radius[1] * sinTheta;
	float zPlus = length / 2.0f + radius[1];

	
	// 球の中にすべて含まれる場合
	if((radius[0] >= radius[1]) && radius[0] >= length + radius[1]){
		return (2.0f/5.0f)*(4.0f/3.0f)*M_PI*pow(radius[0],3.0f)*pow(radius[0],2.0f)* Matrix3f::Unit();
	}else if ((radius[1] >= radius[0]) && radius[1] >= length + radius[0]) {
		return (2.0f/5.0f)*(4.0f/3.0f)*M_PI*pow(radius[1],3.0f)*pow(radius[1],2.0f)* Matrix3f::Unit();
	}
	else {
		float r0 = (M_PI / 2.0f)*(pow(radius[0], 4.0f)*(zZero - zMinus) -
			(2.0f / 3.0f)*pow(radius[0], 2.0f)*(pow(zZero + length / 2, 3.0f) - (pow(zMinus + length / 2, 3.0f))) +
			(1.0f / 5.0f)*(pow(zZero + length / 2.0f, 5.0f) - pow(zMinus + length / 2.0f, 5.0f)));
		float r1 = (M_PI / 2.0f)*(pow(radius[0], 4.0f)*(zPlus - zOne) +
			(2.0f / 3.0f)*pow(radius[0], 2.0f)*(pow(length / 2 - zPlus, 3.0f) - (pow(length / 2 - zOne, 3.0f))) -
			(1.0f / 5.0f)*(pow(length / 2.0f - zPlus, 5.0f) - pow(length / 2.0f - zOne, 5.0f)));
		float tempVar = radius[0] * cosTheta + length * tanTheta / 2.0f + radius[0] * sinTheta*tanTheta;
		float cone = 0;
		if (abs(tanTheta) >= 1.0e-05) {
			cone = (M_PI / (10.0f*tanTheta))*(pow(zOne*tanTheta + tempVar, 5.0f) -
				pow(zZero*tanTheta + tempVar, 5.0f));
		}
		else
		{
			cone = CDShape::CalcCylinderInertia(radius[0], length).zz;
		}

		//DSTR << "RoundCone "<<cone << " r0 "<<r0 << " r1 " << r1  <<" Iz Inertia "<<r0 + r1 + cone << std::endl;
		float Iz = r0 + r1 + cone;
		float g = CDShape::CalcRoundConeCenterOfMass(radius,length).z;
		float r0Ix = M_PI * (1.0f / 60.0f)* (12 * pow(zMinus, 5) + 15 * pow(zMinus, 4)* (length - 2 * g) + 5 * pow(zMinus, 3)*
			(4 * Square(g) - 8 * g* length + Square(length) - 4 * Square(radius[0])) + 15 * Square(zMinus)* g *(2 * g* length - Square(length) +
				4 * pow(radius[0], 2)) + 15 * zMinus *pow(g, 2) *(pow(length, 2) - 4 * pow(radius[0], 2)) -
			zZero * (12 * pow(zZero, 4) + 15 * pow(zZero, 3) *(length - 2 * g) + 5 * pow(zZero, 2) *(4 * pow(g, 2) -
				8 * g* length + pow(length, 2) - 4 * pow(radius[0], 2)) + 15 * zZero* g *(2 * g *length - pow(length, 2) +
					4 * pow(radius[0], 2)) + 15 * pow(g, 2)* (pow(length, 2) - 4 * pow(radius[0], 2))));

		float r1Ix = M_PI * (1.0f / 60.0f)* (12 * pow(zOne, 5) - 15 * pow(zOne, 4) *(2 * g + length) + 5 * pow(zOne, 3) *
			(4 * pow(g, 2) + 8 * g* length + pow(length, 2) - 4 * pow(radius[1], 2)) - 15 * pow(zOne, 2) *g* (2 * g *length + pow(length, 2) -
				4 * pow(radius[1], 2)) + 15 * zOne *pow(g, 2) *(pow(length, 2) - 4 * pow(radius[1], 2)) +
			zPlus * ((-12) *pow(zPlus, 4) + 15 * pow(zPlus, 3) *(2 * g + length) - 5 * pow(zPlus, 2) *(4 * pow(g, 2) +
				8 * g* length + pow(length, 2) - 4 * pow(radius[1], 2)) + 15 * zPlus* g* (2 * g* length + pow(length, 2) -
					4 * pow(radius[1], 2)) - 15 * pow(g, 2) *(pow(length, 2) - 4 * pow(radius[1], 2))));
		float coneIx = 0;
		if (abs(tanTheta) >= 1.0e-05) {
			coneIx = M_PI * (1.0f / 30.0f)* (-6.0f* pow(zZero, 5)* Square(tanTheta) + 15 * pow(zZero, 4)* tanTheta* (g* tanTheta - tempVar) - 10.0f *pow(zZero, 3.0f)* (Square(tempVar) - 4.0f* tempVar* g* tanTheta + Square(g)*Square(tanTheta)) + 30.0f* Square(zZero)* tempVar *g *(tempVar - g * tanTheta) -
				30 * zZero* Square(tempVar)* Square(g) + zOne * (6.0f * pow(zOne, 4)*Square(tanTheta) + 15 * pow(zOne, 3) *tanTheta* (tempVar - g * tanTheta) + 10 * Square(zOne)* (Square(tempVar) - 4 * tempVar* g *tanTheta + Square(g)*Square(tanTheta)) - 30 * zOne* tempVar* g* (tempVar - g * tanTheta) + 30 * Square(tempVar) *Square(g)));
		}
		else
		{
			coneIx = CDShape::CalcCylinderInertia(radius[0], length).xx;
		}
		//DSTR << "RoundCone Ix r0"<<r0Ix<<" r1 " <<r1Ix << " cone " << coneIx << std::endl;
		float Ix = (1.0f / 2.0f)*Iz + r0Ix + r1Ix + coneIx;
		Matrix3d I;
		I.zz = Iz;
		I.xx = Ix;
		I.yy = Ix;
		return I;
	}
}
}
