#ifndef SPR_GRMATERIAL_H
#define SPR_GRMATERIAL_H
#include <Springhead.h>

namespace Spr {;

///	グラフィックスの材質
struct GRMaterial{
	Vec4f diffuse;					///<	
	Vec4f ambient;					///<	
	Vec4f specular;					///<	
	Vec4f emissive;					///<	
	float power;					///<	
	UTString texture;				///<	テクスチャファイル名
	GRMaterial(){ power = 0.0f; }
	GRMaterial(Vec4f d, Vec4f a, Vec4f s, Vec4f e, float p):
		diffuse(d), ambient(a), specular(s), emissive(e), power(p){}
	GRMaterial(Vec4f c, float p):
		diffuse(c), ambient(c), specular(c), emissive(c), power(p){}
	GRMaterial(Vec4f c):
		diffuse(c), ambient(c), specular(c), emissive(c), power(0.0f){}
	bool IsOpaque(){
		return diffuse.W() >= 1.0 && ambient.W() >= 1.0 && specular.W() >= 1.0 && emissive.W() >= 1.0;
	}
};

}	//	Spr
#endif
