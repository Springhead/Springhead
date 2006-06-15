#include "HIS.h"
#pragma hdrstop
//#include "HISpidarCanoe.h"

namespace Spr {;
//----------------------------------------------------------------------------
HIOBJECTIMP(HISpidarCanoe, HISpidarG6);

HISpidarCanoe::HISpidarCanoe() : HISpidarG6() {
}

/*
#define PX 0.20f/2
#define PY 0.20f/2
#define PZ 0.20f/2
#define GX 0.065f/2
#define GY 0.065f/2
*/
void HISpidarCanoe::MakeWireVec(){
#if 1
	//	θを[-π,π]に強制
	if (ori.W() < 0) ori *= -1;
	if (ori.Theta() > M_PI*0.7){
//		ori.x *= -1;
/*		Matrix3f mat;
		ori.to_matrix(mat);
		mat.Ex() *= -1;
		mat.Ey() *= -1;
		ori.from_matrix(mat);
*/
		ori = Quaternionf::Rot(Rad(180), 'z') * ori;
		pos.x *= -1;
		DSTR << "HISpdiarCanoe: reversed " << ori << std::endl;
	}
#endif
	for(unsigned int i=0; i<motor.size(); ++i){
		wireDirection[i] = motor[i].pos - (ori*motor[i].jointPos + pos);
		calculatedLength[i] = wireDirection[i].norm();
		wireDirection[i] /= calculatedLength[i];
		wireMoment[i] = (ori * motor[i].jointPos) ^ wireDirection[i];
	}
}
void HISpidarCanoe::Update(float dt){
	HISpidarG6::Update(dt);
#if 0
	for(int i=0; i<8; ++i){
		DSTR << motor[i].GetLength() << " ";
	}
	DSTR << std::endl;
#endif
}
bool HISpidarCanoe::Init(DVDeviceManager& dev){
	//	糸のグリップへの取り付け位置
	const float GX = 1.27f/2.0f;		//	x方向の辺の長さ/2
	const float GY = 0.0f;				//	y方向の辺の長さ/2
	const float GZ = 0.40f/2.0f;		//	z方向の辺の長さ/2

#if 0	//	ミーティングルーム
	//	モータの取り付け位置. モータが直方体に取り付けられている場合は，
	const float PX = 2.76f/2;		//	x方向の辺の長さ/2
	const float PY = 2.50f/2;		//	y方向の辺の長さ/2
	const float PZ = 2.085f/2;		//	z方向の辺の長さ/2
	Vec3f motorPos[8][2] = {		//	モータの取り付け位置(中心を原点とするDirectX座標系（右がX,上がY,奥がZ）)
		{Vec3f(-PX, PY, PZ), Vec3f( -GX, GY,  GZ)},//左上前
		{Vec3f( PX, PY,-PZ), Vec3f(  GX, GY, -GZ)},//右上奥
		{Vec3f(-PX,-PY,-PZ), Vec3f( -GX, GY, -GZ)},//左下奥
		{Vec3f( PX,-PY, PZ), Vec3f(  GX, GY,  GZ)},//右下前
		{Vec3f( PX, PY, PZ), Vec3f(  GX, GY,  GZ)},//右上前
		{Vec3f(-PX, PY,-PZ), Vec3f( -GX, GY, -GZ)},//左上奥
		{Vec3f( PX,-PY,-PZ), Vec3f(  GX, GY, -GZ)},//右下奥
		{Vec3f(-PX,-PY, PZ), Vec3f( -GX, GY,  GZ)} //左下前
	};
#endif
#if 1	//	工作室
	const float PX = 0.061f/2;		//	x方向の辺の長さ/2
	const float PY = 2.35f/2;		//	y方向の辺の長さ/2
	const float PZ = 1.49f/2;		//	z方向の辺の長さ/2
	Vec3f motorPos[8][2] = {		//	モータの取り付け位置(中心を原点とするDirectX座標系（右がX,上がY,奥がZ）)
		{Vec3f( PX,-PY+PY,-PZ), Vec3f(  GX, GY, -GZ)},
		{Vec3f(-PX,-PY+PY,-PZ), Vec3f( -GX, GY, -GZ)},
		{Vec3f(-PX,-PY+PY, PZ), Vec3f( -GX, GY,  GZ)},
		{Vec3f( PX,-PY+PY, PZ), Vec3f(  GX, GY,  GZ)},
		{Vec3f( PX, PY+PY,-PZ), Vec3f(  GX, GY, -GZ)},
		{Vec3f(-PX, PY+PY,-PZ), Vec3f( -GX, GY, -GZ)},
		{Vec3f(-PX, PY+PY, PZ), Vec3f( -GX, GY,  GZ)},
		{Vec3f( PX, PY+PY, PZ), Vec3f(  GX, GY,  GZ)},
	};
#endif
#if 0	//	itokyo
	const float PX = 0.061f/2;		//	x方向の辺の長さ/2
	const float PY = 1.85f/2;		//	y方向の辺の長さ/2
	const float PZU = 2.62f/2;
	const float PZLF = 0.62f;
	const float PZLR = 0.17f;
	Vec3f motorPos[8][2] = {		//	モータの取り付け位置(中心を原点とするDirectX座標系（右がX,上がY,奥がZ）)
		{Vec3f( PX,-PY,-PZU+PZLF), Vec3f(  GX, -GZ,  0)},
		{Vec3f(-PX,-PY,-PZU+PZLF), Vec3f( -GX, -GZ,  0)},
		{Vec3f(-PX,-PY, PZU+PZLR), Vec3f( -GX, -GZ,  0)},
		{Vec3f( PX,-PY, PZU+PZLR), Vec3f(  GX, -GZ,  0)},
		{Vec3f( PX, PY,-PZU), Vec3f(  GX,  GZ,   0)},
		{Vec3f(-PX, PY,-PZU), Vec3f( -GX,  GZ,   0)},
		{Vec3f(-PX, PY, PZU), Vec3f( -GX,  GZ,   0)},
		{Vec3f( PX, PY, PZU), Vec3f(  GX,  GZ,   0)},
	};
#endif
#if 0	//	Etech
	const float In = 0.0254f;
	const float PX = 2.5	*In/2;		//	x方向の辺の長さ/2
	const float PY = 101	*In/2;		//	y方向の辺の長さ/2
	const float PZ = 109	*In/2;		//	z方向の辺の長さ/2
	const float PZ2 = 85*In	-PZ;		//	
	Vec3f motorPos[8][2] = {		//	モータの取り付け位置(中心を原点とするDirectX座標系（右がX,上がY,奥がZ）)
		{Vec3f(-PX,-PY, PZ), Vec3f( -GX, GY,  GZ)},
		{Vec3f( PX,-PY, PZ), Vec3f(  GX, GY,  GZ)},
		{Vec3f( PX,-PY,-PZ2), Vec3f(  GX, GY, -GZ)},
		{Vec3f(-PX,-PY,-PZ2), Vec3f( -GX, GY, -GZ)},
		{Vec3f(-PX, PY, PZ), Vec3f( -GX, GY,  GZ)},
		{Vec3f( PX, PY, PZ), Vec3f(  GX, GY,  GZ)},
		{Vec3f( PX, PY,-PZ), Vec3f(  GX, GY, -GZ)},
		{Vec3f(-PX, PY,-PZ), Vec3f( -GX, GY, -GZ)},
	};
#endif
/*
	//キャリブレーション位置を中心からずらす
	for( int i=0; i<8; i++ ) motorPos[i][0] -= Vec3f( 0,-0.0675,0.095 );
*/
	//	86120-15000=71120
//	double lpp = 3.0824008138351983723296032553408e-5 * 500 /1024;
	double lpp = 1.0 / 71120.0;
	if( HISpidarG6::Init(dev, 8, motorPos, 0.3f, (float)lpp, 0.8f, 8.0f) == false ){
		return false;
	}
	motor[1].lengthPerPulse *= -1;
	motor[3].lengthPerPulse *= -1;
	motor[4].lengthPerPulse *= -1;
	motor[6].lengthPerPulse *= -1;

	Calib();
	return true;
}

}//	namespace Spr
