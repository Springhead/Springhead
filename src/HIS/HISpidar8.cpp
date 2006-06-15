#include "HIS.h"
#pragma hdrstop
//#include "HISpidar8.h"

#include <Device/DRContecIsaDa.h>
#include <Device/DRContecIsaCounter.h>

namespace Spr{

void HISpidar8::RegisterDevice(DVDeviceManager& devMan){
	devMan.RPool().Register(new DRContecIsaDa(0x340));
	devMan.RPool().Register(new DRContecIsaDa(0x350));
	devMan.RPool().Register(new DRContecIsaDa(0x360));

	devMan.RPool().Register(new DRContecIsaCounter(0x230));
	devMan.RPool().Register(new DRContecIsaCounter(0x240));
	devMan.RPool().Register(new DRContecIsaCounter(0x250));
	devMan.RPool().Register(new DRContecIsaCounter(0x260));
	devMan.RPool().Register(new DRContecIsaCounter(0x270));
	devMan.RPool().Register(new DRContecIsaCounter(0x280));
}

void HISpidar8::Init(DVDeviceManager& devMan){
	//	１ニュートン出すためのDAの出力電圧[V/N]	{0枚目,1枚目,...}
	//	maxon の場合
	//	0.008[m]    / 0.0438[Nm/A] / 0.5[A/V] =  0.365296803653
	//	千葉精機の場合
	//	0.008[m]    / 0.0277[Nm/A] / 0.5[A/V] =  0.577617328520
	//REAL CSpidar::voltPerNewtonDef[] = {0.5776f, 0.5776f};
	//	１カウントの長さ [m/plus]
	//	2*PI*0.008[m] / 500[plus]*4 = 2.513274122872e-5
	//REAL CSpidar::lenPerPlusDef[] = {2.513274122872e-5f, -2.513274122872e-5f};
	Vec3f motorPos[][3] =	//	モータの取り付け位置(中心を原点とする)
	{
		// left hand side
		{Vec3f(-0.025f, +0.230f, -0.283f), Vec3f(-0.316f, +0.080f, -0.283f), Vec3f(-0.366f, +0.230f, -0.025f)},
		{Vec3f(-0.025f, +0.230f, +0.283f), Vec3f(-0.316f, +0.025f, +0.283f), Vec3f(-0.366f, +0.230f, +0.025f)},
		{Vec3f(-0.025f, -0.230f, +0.283f), Vec3f(-0.316f, -0.025f, +0.283f), Vec3f(-0.366f, -0.230f, +0.025f)},
		{Vec3f(-0.025f, -0.230f, -0.283f), Vec3f(-0.316f, -0.080f, -0.283f), Vec3f(-0.366f, -0.230f, -0.025f)},
		// right hand side
		{Vec3f(+0.025f, +0.230f, -0.283f), Vec3f(+0.316f, +0.080f, -0.283f), Vec3f(+0.366f, +0.230f, -0.025f)},
		{Vec3f(+0.025f, +0.230f, +0.283f), Vec3f(+0.316f, +0.025f, +0.283f), Vec3f(+0.366f, +0.230f, +0.025f)},
		{Vec3f(+0.025f, -0.230f, +0.283f), Vec3f(+0.316f, -0.025f, +0.283f), Vec3f(+0.366f, -0.230f, +0.025f)},
		{Vec3f(+0.025f, -0.230f, -0.283f), Vec3f(+0.316f, -0.080f, -0.283f), Vec3f(+0.366f, -0.230f, -0.025f)}
	};

	for(int i=0; i<8; ++i){
		sp3[i].Init(devMan, motorPos[i], 0.5776f, 2.924062107079e-5f, 0.35f, 30.0f, i<4);
	}
}
void HISpidar8::Update(float dt){
	for(int i=0; i<8; ++i){
		sp3[i].Update(dt);
	}
}

}
