/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Graphics/GRRender.h>

// intrin.hとinterlocked.hppとの呼び出し規約の不整合を回避するためにwindows.hをインクルードするように指示
#if defined _MSC_VER
# define BOOST_USE_WINDOWS_H
#endif
#include <boost/regex.hpp>

#include <GL/glew.h>

namespace Spr {;

static boost::regex Tex3DRegex("^(.*_tex3d_)([0-9]+)(\\Q.\\E[^\\Q.\\E]+)$");
bool GRMaterialDesc::Is3D() const{
	return boost::regex_match(texname, Tex3DRegex);
}

//----------------------------------------------------------------------------
//	GRShader

void GRShader::GetLocations(){
	enableLightingLoc = glGetUniformLocation(programId, "enableLighting");
	enableTex2DLoc    = glGetUniformLocation(programId, "enableTex2D"   );
	enableTex3DLoc    = glGetUniformLocation(programId, "enableTex3D"   );
	tex2DLoc          = glGetUniformLocation(programId, "tex2D"         );
	tex3DLoc          = glGetUniformLocation(programId, "tex3D"         );
	shadowTexLoc      = glGetUniformLocation(programId, "shadowTex"     );
	shadowMatrixLoc   = glGetUniformLocation(programId, "shadowMatrix"  );
	shadowColorLoc    = glGetUniformLocation(programId, "shadowColor"   );
	enableBlendingLoc = glGetUniformLocation(programId, "enableBlending");
	blendMatricesLoc  = glGetUniformLocation(programId, "blendMatrices" );

}

//----------------------------------------------------------------------------
//	GRRender
#undef  RGB
#define RGB(name, x, y, z) {matSample[GRRenderBaseIf::##name].diffuse = Vec4f((float)x/255.0f, (float)y/255.0f, (float)z/255.0f, 1.0f);}

GRRender::GRRender(){
	matSample.resize(GRRenderBaseIf::MATERIAL_SAMPLE_END);
	RGB(INDIANRED,				205,  92,  92);
	RGB(LIGHTCORAL,				240, 128, 128);
	RGB(SALMON,					250	, 128, 114);
	RGB(DARKSALMON,				233, 150, 122);
	RGB(LIGHTSALMON,			255, 160, 122);
	RGB(RED,					255,   0,   0);
	RGB(CRIMSON,				220,  20,  60);
	RGB(FIREBRICK,				178,  34,  34);
	RGB(DARKRED,				139,   0,   0);
	RGB(PINK,					255, 192, 203);
	RGB(LIGHTPINK,				255, 182, 193);
	RGB(HOTPINK,				255, 105, 180);
	RGB(DEEPPINK,				255,  20, 147);
	RGB(MEDIUMVIOLETRED,		199,  21, 133);
	RGB(PALEVIOLETRED,			219, 112, 147);
	RGB(CORAL,					255, 127,  80);
	RGB(TOMATO,					255,  99,  71);
	RGB(ORANGERED,				255,  69,   0);
	RGB(DARKORANGE,				255, 140,   0);
	RGB(ORANGE,					255, 165,   0);
	RGB(GOLD,					255, 215,   0);
	RGB(YELLOW,					255, 255,   0);
	RGB(LIGHTYELLOW,			255, 255, 224);
	RGB(LEMONCHIFFON,			255, 250, 205);
	RGB(LIGHTGOLDENRODYELLOW,	250, 250, 210);
	RGB(PAPAYAWHIP,				255, 239, 213);
	RGB(MOCCASIN,				255, 228, 181);
	RGB(PEACHPUFF,				255, 218, 185);
	RGB(PALEGOLDENROD,			238, 232, 170);
	RGB(KHAKI,					240, 230, 140);
	RGB(DARKKHAKI,				189, 183, 107);
	RGB(LAVENDER,				230, 230, 250);
	RGB(THISTLE,				216, 191, 216);
	RGB(PLUM,					221, 160, 221);
	RGB(VIOLET,					238, 130, 238);
	RGB(ORCHILD,				218, 112, 214);
	RGB(FUCHSIA,				255,   0, 255);
	RGB(MAGENTA,				255,   0, 255);
	RGB(MEDIUMORCHILD,			186,  85, 211);
	RGB(MEDIUMPURPLE,			147, 112, 219);
	RGB(BLUEVIOLET,				138,  43, 226);
	RGB(DARKVIOLET,				148,   0, 211);
	RGB(DARKORCHILD,			153,  50, 204);
	RGB(DARKMAGENTA,			139,   0, 139);
	RGB(PURPLE,					128,   0, 128);
	RGB(INDIGO,					 75,   0, 130);
	RGB(DARKSLATEBLUE,			 72,  61, 139);
	RGB(SLATEBLUE,				106,  90, 205);
	RGB(MEDIUMSLATEBLUE,		123, 104, 238);
	RGB(GREENYELLOW,			173, 255,  47);
	RGB(CHARTREUSE,				127, 255,   0);
	RGB(LAWNGREEN,				124, 252,   0);
	RGB(LIME,					  0, 252,   0);
	RGB(LIMEGREEN,				 50, 205,  50);
	RGB(PALEGREEN,				152, 251, 152);
	RGB(LIGHTGREEN,				144, 238, 144);
	RGB(MEDIUMSPRINGGREEN,		  0, 250, 154);
	RGB(SPRINGGREEN,			  0, 255, 127);
	RGB(MEDIUMSEAGREEN,			 60, 179, 113);
	RGB(SEAGREEN,				 46, 139,  87);
	RGB(FORESTGREEN,			 34, 139,  34);
	RGB(GREEN,					  0, 128,   0);
	RGB(DARKGREEN,				  0, 100,   0);
	RGB(YELLOWGREEN,			154, 205,  50);
	RGB(OLIVEDRAB,				107, 142,  35);
	RGB(OLIVE,					128, 128,   0);
	RGB(DARKOLIVEGREEN,			 85, 107,  47);
	RGB(MEDIUMAQUAMARINE,		102, 205, 170);
	RGB(DARKSEAGREEN,			143, 188, 143);
	RGB(LIGHTSEAGREEN,			 32, 178, 170);
	RGB(DARKCYAN,				  0, 139, 139);
	RGB(TEAL,					  0, 128, 128);
	RGB(AQUA,					  0, 255, 255);
	RGB(CYAN,					  0, 255, 255);
	RGB(LIGHTCYAN,				224, 255, 255);
	RGB(PALETURQUOISE,			175, 238, 238);
	RGB(AQUAMARINE,				127, 255, 212);
	RGB(TURQUOISE,				 64, 224, 208);
	RGB(MEDIUMTURQUOISE,		 72, 209, 204);
	RGB(DARKTURQUOISE,			  0, 206, 209);
	RGB(CADETBLUE,				 95, 158, 160);
	RGB(STEELBLUE,				 70, 130, 180);
	RGB(LIGHTSTEELBLUE,			176, 196, 222);
	RGB(POWDERBLUE,				176, 224, 230);
	RGB(LIGHTBLUE,				173, 216, 230);
	RGB(SKYBLUE,				135, 206, 235);
	RGB(LIGHTSKYBLUE,			135, 206, 250);
	RGB(DEEPSKYBLUE,			  0, 191, 255);
	RGB(DODGERBLUE,				 30, 144, 255);
	RGB(CORNFLOWERBLUE,			100, 149, 237);
	RGB(ROYALBLUE,				 65, 105, 225);
	RGB(BLUE,					  0,   0, 255);
	RGB(MEDIUMBLUE,				  0,   0, 205);
	RGB(DARKBLUE,				  0,   0, 139);
	RGB(NAVY,					  0,   0, 128);
	RGB(MIDNIGHTBLUE,			 25,  25, 112);
	RGB(CORNSILK,				255, 248, 220);
	RGB(BLANCHEDALMOND,			255, 235, 205);
	RGB(BISQUE,					255, 228, 196);
	RGB(NAVAJOWHITE,			255, 222, 173);
	RGB(WHEAT,					245, 222, 179);
	RGB(BURLYWOOD,				222, 184, 135);
	RGB(TAN,					210, 180, 140);
	RGB(ROSYBROWN,				188, 143, 143);
	RGB(SANDYBROWN,				244, 164,  96);
	RGB(GOLDENROD,				218, 165,  32);
	RGB(DARKGOLDENROD,			184, 134,  11);
	RGB(PERU,					205, 133,  63);
	RGB(CHOCOLATE,				210, 105,  30);
	RGB(SADDLEBROWN,			139,  69,  19);
	RGB(SIENNA,					160,  82,  45);
	RGB(BROWN,					165,  42,  42);
	RGB(MAROON,					128,   0,   0);
	RGB(WHITE,					255, 255, 255);
	RGB(SNOW,					255, 250, 250);
	RGB(HONEYDEW,				240, 255, 240);
	RGB(MINTCREAM,				245, 255, 250);
	RGB(AZURE,					240, 255, 255);
	RGB(ALICEBLUE,				240, 248, 255);
	RGB(GHOSTWHITE,				248, 248, 255);
	RGB(WHITESMOKE,				245, 245, 245);
	RGB(SEASHELL,				255, 245, 238);
	RGB(BEIGE,					245, 245, 220);
	RGB(OLDLACE,				253, 245, 230);
	RGB(FLORALWHITE,			255, 250, 240);
	RGB(IVORY,					255, 255, 240);
	RGB(ANTIQUEWHITE,			250, 235, 215);
	RGB(LINEN,					250, 240, 230);
	RGB(LAVENDERBLUSH,			255, 240, 245);
	RGB(MISTYROSE,				255, 228, 225);
	RGB(GAINSBORO,				220, 220, 220);
	RGB(LIGHTGRAY,				211, 211, 211);
	RGB(SILVER,					192, 192, 192);
	RGB(DARKGRAY,				169, 169, 169);
	RGB(GRAY,					128, 128, 128);
	RGB(DIMGRAY,				105, 105, 105);
	RGB(LIGHTSLATEGRAY,			119, 136, 153);
	RGB(SLATEGRAY,				112, 128, 144);
	RGB(DARKSLATEGRAY,			 47,  79,  79);
	RGB(BLACK,					  0,   0,   0);

	// �����͊g�U���Ɠ����ɐݒ�
	for(int i = 0; i < (int)matSample.size(); i++)
		matSample[i].ambient = matSample[i].diffuse;

	/*matSample.resize(24);
	std::vector<GRMaterialDesc>::iterator itr;
	itr = matSample.begin();
	(itr++)->diffuse = Vec4f(1.0, 0.0, 0.0, 1.0);			// red
	(itr++)->diffuse = Vec4f(0.0, 1.0, 0.0, 1.0);			// green
	(itr++)->diffuse = Vec4f(0.0, 0.0, 1.0, 1.0);			// blue
	(itr++)->diffuse = Vec4f(1.0, 1.0, 0.0, 1.0);			// yellow
	(itr++)->diffuse = Vec4f(0.0, 1.0, 1.0, 1.0);			// cyan
	(itr++)->diffuse = Vec4f(1.0, 0.0, 1.0, 1.0);			// magenta
	(itr++)->diffuse = Vec4f(1.0, 1.0, 1.0, 1.0);			// white
	(itr++)->diffuse = Vec4f(0.5, 0.5, 0.5, 1.0);			// gray
	(itr++)->diffuse = Vec4f(1.0, 0.27, 0.0, 1.0);			// orange
	(itr++)->diffuse = Vec4f(0.198, 0.0, 0.0, 1.0);			// brown
	(itr++)->diffuse = Vec4f(0.676, 0.844, 0.898, 1.0);		// light blue
	(itr++)->diffuse = Vec4f(0.574, 0.438, 0.855, 1.0);		// medium purple
	(itr++)->diffuse = Vec4f(0.0, 0.391, 0.0, 1.0);			// dark green
	(itr++)->diffuse = Vec4f(0.578, 0.0, 0.824, 1.0);		// dark violet
	(itr++)->diffuse = Vec4f(0.0, 0.543, 0.543, 1.0);		// dark cyan
	(itr++)->diffuse = Vec4f(0.676, 1.0, 0.184, 1.0);		// green yellow
	(itr++)->diffuse = Vec4f(0.195, 0.801, 0.195, 1.0);		// lime green
	(itr++)->diffuse = Vec4f(0.801, 0.359, 0.359, 1.0);		// indian red
	(itr++)->diffuse = Vec4f(0.293, 0.0, 0.508, 1.0);		// indigo
	(itr++)->diffuse = Vec4f(0.0, 0.198, 0.198, 1.0);		// green indigo
	(itr++)->diffuse = Vec4f(0.198, 0.398, 0.0, 1.0);		// olive green
	(itr++)->diffuse = Vec4f(0.198, 0.398, 0.797, 1.0);		// navy blue
	(itr++)->diffuse = Vec4f(0.398, 1.0, 0.797, 1.0);		// turquoise blue
	(itr++)->diffuse = Vec4f(0.598, 1.0, 0.398, 1.0);		// emerald green
	for(std::vector<GRMaterialDesc>::iterator it=matSample.begin(); it!= matSample.end(); ++it){
		it->diffuse += Vec4f(0.5,0.5,0.5,1);
		it->diffuse /= 2;
	}*/

	screenCoord = false;
}

#undef RGB

void GRRender::Print(std::ostream& os) const{
	device->Print(os);
}
void GRRender::Reshape(Vec2f pos, Vec2f screen){
	SetViewport(pos, screen);
	bool yIsZero = false, xIsZero = false;
	if (camera.size.y<=0){
		yIsZero = true;
		camera.size.y = camera.size.x*(screen.y/screen.x);
	}
	if (camera.size.x<=0){
		xIsZero = true;
		camera.size.x = camera.size.y*(screen.x/screen.y);
	}
	Affinef afProj = Affinef::ProjectionGL(Vec3f(camera.center.x, camera.center.y, camera.front), 
		camera.size, camera.front, camera.back);
	SetProjectionMatrix(afProj);
	if (yIsZero) { camera.size.y = 0; }
	if (xIsZero) { camera.size.x = 0; }
}
void GRRender::SetMaterial(int matname){
	if(0 <= matname && matname < GRRenderIf::MATERIAL_SAMPLE_END)
		SetMaterial(matSample[matname]);
}
void GRRender::SetCamera(const GRCameraDesc& c){
	Vec2f vpsize = GetViewportSize();
	// メモリ比較でフィルタすると外部からSetProjectionMatrixした際にカメラが反映されないので無効化 tazz
	//if (memcmp(&camera,&c, sizeof(c)) != 0){
		camera = c;
		if (camera.size.y==0) camera.size.y = camera.size.x*(vpsize.y/vpsize.x);
		if (camera.size.x==0) camera.size.x = camera.size.y*(vpsize.x/vpsize.y);
		Affinef afProj = Affinef::ProjectionGL(Vec3f(camera.center.x, camera.center.y, camera.front), 
			camera.size, camera.front, camera.back);
		SetProjectionMatrix(afProj);
	//}
}
Vec2f GRRender::GetPixelSize(){
	// ピクセルサイズ = スクリーンサイズ / ビューポートサイズ
	Vec2f vpsize = GetViewportSize();
	Vec2f px(camera.size.x / vpsize.x, camera.size.y / vpsize.y);
	if(px.x == 0.0f)
		px.x = px.y;
	if(px.y == 0.0f)
		px.y = px.x;
	return px;
}	
Vec3f GRRender::ScreenToCamera(int x, int y, float depth, bool LorR){
	//アスペクト比
	Vec2f vpsize = GetViewportSize();
	double r = (vpsize.x / vpsize.y);

	//スクリーンサイズ
	Vec2f camSize = camera.size;
	if(camSize.x ==0) camSize.x = camSize.y * r ;	//片方を0に設定してある場合，内部で自動的に比率が計算されているので，再度計算し代入する
	if(camSize.y ==0) camSize.y = camSize.x / r ;

	Vec3f pos(
		 camera.center.x + ((float)x - vpsize.x / 2.0f) * (camSize.x / vpsize.x),
		 camera.center.y + (vpsize.y / 2.0f - (float)y) * (camSize.y / vpsize.y),
		 (LorR ? camera.front : -camera.front));
	pos *= (depth / camera.front);
	return pos;
}

void GRRender::EnterScreenCoordinate(){
	if(screenCoord)
		return;
	GetViewMatrix(affViewTmp);
	GetModelMatrix(affModelTmp);
	GetProjectionMatrix(affProjTmp);

	SetModelMatrix(Affinef());
	SetViewMatrix(Affinef::Trn(0.0f, 0.0f, -1.001f * GetCamera().front));
	Affinef affOrtho = Affinef::OrthoGL(GetViewportSize());
	SetProjectionMatrix(affOrtho);
	screenCoord = true;
}

void GRRender::LeaveScreenCoordinate(){
	if(!screenCoord)
		return;
	SetProjectionMatrix(affProjTmp);
	SetViewMatrix(affViewTmp);
	SetModelMatrix(affModelTmp);
	screenCoord = false;
}
	

//----------------------------------------------------------------------------
//	GRMaterial
void GRMaterial::Render(GRRenderIf* render){
	if(!enabled)
		return;
	render->SetMaterial(this->Cast());
}

//----------------------------------------------------------------------------
//	GRLight
void GRLight::Render(GRRenderIf* render){
	if(!enabled)
		return;
	render->PushLight(*this);
}
void GRLight::Rendered(GRRenderIf* render){
	if(!enabled)
		return;
	render->PopLight();
}

//----------------------------------------------------------------------------
//	GRCamera
size_t GRCamera::NChildObject() const {
	return frame ? 1 : 0;
}
ObjectIf* GRCamera::GetChildObject(size_t pos){
	if (pos == 0) return frame ? frame : NULL;
	return NULL;
}
bool GRCamera::AddChildObject(ObjectIf* o){
	GRFrame* f = DCAST(GRFrame, o);
	if (f){
		frame = f->Cast();
		return true;
	}
	return false;
}
void GRCamera::Render(GRRenderIf* r){
	if(!enabled)
		return;

	if (frame){
		Affinef af = frame->GetWorldTransform();
		r->SetViewMatrix(af.inv());
	}
	r->SetCamera(*this);
}

}	//	Spr
