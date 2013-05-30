/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <SprDefs.h>
#include <Graphics/GRDeviceGL.h>
#include "GRLoadBmp.h"

// OpenGL
#if defined(USE_GLEW)
#ifndef _MSC_VER
# define GLEW_STATIC		// 定義するとAPI宣言にdeclspecキーワードが付かない
#endif
# include <GL/glew.h>
#endif

#include <GL/glut.h>

#ifdef _MSC_VER
# include <windows.h>
#endif

#include <iomanip>
#include <sstream>
#ifdef _MSC_VER
#include <io.h>
#endif

// intrin.hとinterlocked.hppとの呼び出し規約の不整合を回避するためにwindows.hをインクルードするように指示
#if defined _MSC_VER
# define BOOST_USE_WINDOWS_H
#endif
#include <boost/regex.hpp>

static bool enableDebugMessage = false;

typedef unsigned int uint;

namespace Spr {;
//----------------------------------------------------------------------------
//	GRDeviceGL
/// 初期設定
void GRDeviceGL::Init(){
	// バージョン取得 (glGetIntegerによるバージョン取得は3.0以降が必要)
	majorVersion = 1;
	minorVersion = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

	nLights      = 0;
	fontBase     = -1;
	bPointSmooth = false;
	bLineSmooth  = false;

	glDrawBuffer(GL_BACK);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	SetVertexFormat(GRVertexElement::vfP3f);
	
	// 視点行列の設定
	viewMatrix.Pos() = Vec3f(0.0, 0.0, 1.0);	                        // eye
	viewMatrix.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));	// center, up 
	viewMatrix = viewMatrix.inv();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(viewMatrix);

	// デフォルトでシェーダ不使用
	//SetShader(0);
	curShader = 0;

	// シャドウ関係
	shadowTexId     = 0;
	shadowBufferId  = 0;
}

///	Viewport設定
void GRDeviceGL::SetViewport(Vec2f pos, Vec2f sz){
	glViewport((int)pos.x , (int)pos.y, (int)sz.x, (int)sz.y);
}
Vec2f GRDeviceGL::GetViewportPos(){
	int tmp[4];
	glGetIntegerv(GL_VIEWPORT, tmp);
	return Vec2f(tmp[0], tmp[1]);
}
Vec2f GRDeviceGL::GetViewportSize(){
	int tmp[4];
	glGetIntegerv(GL_VIEWPORT, tmp);
	return Vec2f(tmp[2], tmp[3]);
}
/// バッファクリア
void GRDeviceGL::ClearBuffer(bool color, bool depth){
	if(color){
		glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	if(depth){
		glClearDepth(1.0); 
		glClear(GL_DEPTH_BUFFER_BIT);
	}
}
void GRDeviceGL::SwapBuffers(){
	glutSwapBuffers();
}

/// レンダリングの開始前に呼ぶ関数
void GRDeviceGL::BeginScene(){		
  /// GLでレンダリングする場合、特に処理する必要なし
	
}
/// レンダリングの終了後に呼ぶ関数
void GRDeviceGL::EndScene(){
	glFinish();
}
///	カレントの視点行列をafvで置き換える
void GRDeviceGL::SetViewMatrix(const Affinef& afv){   
	viewMatrix  = afv;		// 視点行列の保存
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(viewMatrix);
	//glLoadMatrixf(viewMatrix * modelMatrix);
}
void GRDeviceGL::GetViewMatrix(Affinef& afv){   
	afv = viewMatrix;
}
/// カレントの投影行列をafpで置き換える
void GRDeviceGL::SetProjectionMatrix(const Affinef& afp){  
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(afp);
	glMatrixMode(GL_MODELVIEW);
}
/// カレントの投影行列を取得する
void GRDeviceGL::GetProjectionMatrix(Affinef& afp){  
	glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat *)&afp);
}	
/// カレントのモデル行列をafwで置き換える
void GRDeviceGL::SetModelMatrix(const Affinef& afw){
	modelMatrix = afw;		// モデル行列の保存
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(viewMatrix * modelMatrix);

	CalcShadowMatrix();
}
void GRDeviceGL::GetModelMatrix(Affinef& afw){
	afw = modelMatrix;
}
/// カレントのモデル行列に対してafwを掛ける
void GRDeviceGL::MultModelMatrix(const Affinef& afw){	
	modelMatrix = modelMatrix * afw;		
	glMatrixMode(GL_MODELVIEW);
	glMultMatrixf(afw);

	CalcShadowMatrix();
}
/// カレントのモデル行列をモデル行列スタックへ保存する
void GRDeviceGL::PushModelMatrix(){
	modelMatrixStack.push(modelMatrix);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}
/// モデル行列スタックから取り出し、カレントのモデル行列とする
void GRDeviceGL::PopModelMatrix(){
	modelMatrix = modelMatrixStack.top();
	modelMatrixStack.pop();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	CalcShadowMatrix();
}
/// ブレンド変換行列の全要素を削除する
void GRDeviceGL::ClearBlendMatrix(){
	blendMatrix.clear();
}
/// ブレンド変換行列を設定する
bool GRDeviceGL::SetBlendMatrix(const Affinef& afb){
	blendMatrix.push_back(afb);
	return true;
}
/// ブレンド変換行列を設定する
bool GRDeviceGL::SetBlendMatrix(const Affinef& afb, unsigned int id){
	if (id == 0){
		SetBlendMatrix(afb);
		return true;
	}
	if (blendMatrix.size() > id) {
		blendMatrix[id] = afb;
		return true;
	}
	return false;
}		
/// 頂点フォーマットの指定
void GRDeviceGL::SetVertexFormat(const GRVertexElement* e){	
	if (e == GRVertexElement::vfP3f) {
		vertexFormatGl = GL_V3F; 
		vertexSize = sizeof(float)*3;
		vertexColor = false;
	}else if (e == GRVertexElement::vfC4bP3f){
		vertexFormatGl = GL_C4UB_V3F;
		vertexSize = sizeof(float)*3+sizeof(char)*4;
		vertexColor = true;
	}else if (e == GRVertexElement::vfN3fP3f){
		vertexFormatGl = GL_N3F_V3F;
		vertexSize = sizeof(float)*6;
		vertexColor = false;
	}else if (e == GRVertexElement::vfC4fN3fP3f){
		vertexFormatGl = GL_C4F_N3F_V3F;
		vertexSize = sizeof(float)*10;
		vertexColor = true;
	}else if (e == GRVertexElement::vfT2fP3f){
		vertexFormatGl = GL_T2F_V3F;
		vertexSize = sizeof(float)*5;
		vertexColor = false;
	}else if (e == GRVertexElement::vfT4fP4f){
		vertexFormatGl = GL_T4F_V4F;
		vertexSize = sizeof(float)*8;
		vertexColor = false;
	}else if (e == GRVertexElement::vfT2fC4bP3f){
		vertexFormatGl = GL_T2F_C4UB_V3F;
		vertexSize = sizeof(float)*5+sizeof(char)*4;
		vertexColor = true;
	}else if (e == GRVertexElement::vfT2fN3fP3f){
		vertexFormatGl = GL_T2F_N3F_V3F;
		vertexSize = sizeof(float)*8;
		vertexColor = false;
	}else if (e == GRVertexElement::vfT2fC4fN3fP3f){
		vertexFormatGl = GL_T2F_C4F_N3F_V3F;
		vertexSize = sizeof(float)*12;
		vertexColor = true;
	}else if (e == GRVertexElement::vfT4fC4fN3fP4f){
		vertexFormatGl = GL_T4F_C4F_N3F_V4F;
		vertexSize = sizeof(float)*15;
		vertexColor = false;
	}else if (e == GRVertexElement::vfP3fB4f) {
		vertexFormatGl = GL_V3F; 
		vertexSize = sizeof(float)*15;
		vertexColor = false;
	}else if (e == GRVertexElement::vfC4bP3fB4f){
		vertexFormatGl = GL_C4UB_V3F;
		vertexSize = sizeof(float)*15+sizeof(char)*4;
		vertexColor = true;
	}else if (e == GRVertexElement::vfC3fP3fB4f){
		vertexFormatGl = GL_C3F_V3F;
		vertexSize = sizeof(float)*18;
		vertexColor = true;
	}else if (e == GRVertexElement::vfN3fP3fB4f){
		vertexFormatGl = GL_N3F_V3F;
		vertexSize = sizeof(float)*18;
		vertexColor = false;
	}else if (e == GRVertexElement::vfC4fN3fP3fB4f){
		vertexFormatGl = GL_C4F_N3F_V3F;
		vertexSize = sizeof(float)*22;
		vertexColor = true;
	}else if (e == GRVertexElement::vfT2fP3fB4f){
		vertexFormatGl = GL_T2F_V3F;
		vertexSize = sizeof(float)*17;
		vertexColor = false;
	}else if (e == GRVertexElement::vfT2fC4bP3fB4f){
		vertexFormatGl = GL_T2F_C4UB_V3F;
		vertexSize = sizeof(float)*17+sizeof(char)*4;
		vertexColor = true;
	}else if (e == GRVertexElement::vfT2fN3fP3fB4f){
		vertexFormatGl = GL_T2F_N3F_V3F;
		vertexSize = sizeof(float)*20;
		vertexColor = false;
	}else if (e == GRVertexElement::vfT2fC4fN3fP3fB4f){
		vertexFormatGl = GL_T2F_C4F_N3F_V3F;
		vertexSize = sizeof(float)*24;
		vertexColor = true;
	}else {
		for(int i=0; GRVertexElement::typicalFormats[i]; ++i){
			if (CompareVertexFormat(e, GRVertexElement::typicalFormats[i])){
				SetVertexFormat(GRVertexElement::typicalFormats[i]);
				return;
			}
		}
		for(int i=0; GRVertexElement::typicalBlendFormats[i]; ++i){
			if (CompareVertexFormat(e, GRVertexElement::typicalBlendFormats[i])){
				SetVertexFormat(GRVertexElement::typicalBlendFormats[i]);
				return;
			}
		}
		vertexFormatGl = 0;
		vertexSize = 0;
		vertexColor = false;
		assert(0);
	}
}
/*void GRDeviceGL::SetVertexShader(void* s){
	assert(0);	//	To Be Implemented
}*/
/// 頂点座標を指定してプリミティブを描画
void GRDeviceGL::DrawDirect(GRRenderBaseIf::TPrimitiveType ty, void* vtx, size_t count, size_t stride){
	GLenum mode = GL_TRIANGLES;
	switch(ty) {
		case GRRenderBaseIf::POINTS:			mode = GL_POINTS;			break;
		case GRRenderBaseIf::LINES:				mode = GL_LINES;			break;
		case GRRenderBaseIf::LINE_STRIP:		mode = GL_LINE_STRIP;		break;
		case GRRenderBaseIf::LINE_LOOP:			mode = GL_LINE_LOOP;		break;
		case GRRenderBaseIf::TRIANGLES:			mode = GL_TRIANGLES;		break;
		case GRRenderBaseIf::TRIANGLE_STRIP:	mode = GL_TRIANGLE_STRIP;	break;
		case GRRenderBaseIf::TRIANGLE_FAN:		mode = GL_TRIANGLE_FAN;		break;
		case GRRenderBaseIf::QUADS:				mode = GL_QUADS;			break;
		default:				/* DO NOTHING */			break;
	}
	if (!stride) stride = vertexSize;
	if (vertexColor) glEnable(GL_COLOR_MATERIAL);
	else glDisable(GL_COLOR_MATERIAL);
	SetMaterial(currentMaterial);
	glInterleavedArrays(vertexFormatGl, (GLsizei)stride, vtx);
	glDrawArrays(mode, 0, (GLsizei)count);
}
/// 頂点座標とインデックスを指定してプリミティブを描画
void GRDeviceGL::DrawIndexed(GRRenderBaseIf::TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride){
	GLenum mode = GL_TRIANGLES;
	switch(ty) {
		case GRRenderBaseIf::POINTS:			mode = GL_POINTS;			break;
		case GRRenderBaseIf::LINES:				mode = GL_LINES;			break;
		case GRRenderBaseIf::LINE_STRIP:		mode = GL_LINE_STRIP;		break;
		case GRRenderBaseIf::LINE_LOOP:			mode = GL_LINE_LOOP;		break;
		case GRRenderBaseIf::TRIANGLES:			mode = GL_TRIANGLES;		break;
		case GRRenderBaseIf::TRIANGLE_STRIP:	mode = GL_TRIANGLE_STRIP;	break;
		case GRRenderBaseIf::TRIANGLE_FAN:		mode = GL_TRIANGLE_FAN;		break;
		case GRRenderBaseIf::QUADS:				mode = GL_QUADS;			break;
		default:				/* DO NOTHING */			break;
	}
	if (!stride) stride = vertexSize;
	if (vertexColor) glEnable(GL_COLOR_MATERIAL);
	else glDisable(GL_COLOR_MATERIAL);

	glInterleavedArrays(vertexFormatGl, (GLsizei)stride, vtx);
	glDrawElements(mode, (GLsizei)count, GL_UNSIGNED_INT, idx);
	//naga
	//glFinish()はスキンウェイトのある頂点の場合，途中で書き換えがあるので，データ書き換え中に描画されないようにする．
	//しかし，著しく処理が重くなる可能性がある．画面がちらつくのを我慢するか，処理が重くなるのを我慢するか注意する必要がある．
	glFinish();	
}

void GRDeviceGL::DrawPoint(Vec3f p){
	glBegin(GL_POINTS);
	glVertex3f(p.x, p.y, p.z);
	glEnd();
}

void GRDeviceGL::DrawLine(Vec3f p0, Vec3f p1){
	glBegin(GL_LINES);
	glVertex3fv((const float*)&p0);
	glVertex3fv((const float*)&p1);
	glEnd();
}

inline Vec3f Spline(float s, const Vec3f& p0, const Vec3f& v0, const Vec3f& p1, const Vec3f& v1){
	float s2 = s*s, s3 = s2 * s;	
	return (1.0 - 3.0*s2 + 2.0*s3) * p0 + (s - 2.0*s2 + s3) * v0 + (3.0*s2 - 2.0*s3) * p1 + (-s2 + s3) * v1;	
}

void GRDeviceGL::DrawSpline(Vec3f p0, Vec3f v0, Vec3f p1, Vec3f v1, int ndiv){
	float dt = 1.0f / (float)ndiv;
	float t0 = 0.0f, t1 = dt;
	Vec3f a, b;
	a = Spline(t0, p0, v0, p1, v1);
	for(int i = 0; i < ndiv; i++){
		b = Spline(t1, p0, v0, p1, v1);
		DrawLine(a, b);
		a = b;
		t0 = t1;
		t1 += dt;
	}
}

void GRDeviceGL::DrawArrow(Vec3f p0, Vec3f p1, float rbar, float rhead, float lhead, int slice, bool solid){
	Vec3f d = p1 - p0;
	float l = d.norm();
	if(l == 0.0f)
		return;

	// 矢印をz軸に合わせる回転
	Quaternionf q;
	Affinef aff;
	q.RotationArc(Vec3f(0.0f, 0.0f, 1.0f), d);
	q.ToMatrix(aff.Rot());

	this->PushModelMatrix();
	this->MultModelMatrix(aff);
	this->MultModelMatrix(Affinef::Trn(0.0f, 0.0f, 0.5f * l));
	DrawCylinder(rbar, l, slice, solid, false);
	this->MultModelMatrix(Affinef::Trn(0.0f, 0.0f, 0.5f * l));
	DrawCone(rhead, lhead, slice, solid);
	this->PopModelMatrix();
}

void GRDeviceGL::DrawBox(float sx, float sy, float sz, bool solid){
	this->PushModelMatrix();
	this->MultModelMatrix(Affinef::Scale(sx, sy, sz));
	solid ? glutSolidCube(1.0) : glutWireCube(1.0);
	this->PopModelMatrix();
}

void GRDeviceGL::DrawSphere(float radius, int slices, int stacks, bool solid){
	if(solid)
		 glutSolidSphere(radius, slices, stacks);
	else glutWireSphere(radius, slices, stacks);
}

void GRDeviceGL::DrawCone(float radius, float height, int slice, bool solid){
	if(solid)
		 glutSolidCone(radius, height, slice, 1);
	else glutWireCone(radius, height, slice, 1);
}

void GRDeviceGL::DrawCylinder(float radius, float height, int slice, bool solid, bool cap){
	float hhalf = 0.5f * height;
	
	// 側面
	glBegin(solid ? GL_QUAD_STRIP : GL_LINES);
	float x,y;
	for (int i=0; i<=slice; i++) {
		x = GetSin(i, slice);
		y = GetCos(i, slice);
		glNormal3f(x, y, 0.0);
		glVertex3f(radius * x, radius * y, -hhalf);
		glVertex3f(radius * x, radius * y,  hhalf);
	}
	glEnd();

	// 上面・下面
	if(cap){
		PushModelMatrix();
		MultModelMatrix(Affinef::Trn(0.0f, 0.0f,  hhalf));
		DrawDisk(radius, slice, solid);
		MultModelMatrix(Affinef::Trn(0.0f, 0.0f, -height));
		MultModelMatrix(Affinef::Rot((float)Rad(180.0), 'x'));
		DrawDisk(radius, slice, solid);
		PopModelMatrix();
	}	
}

void GRDeviceGL::DrawDisk(float radius, int slice, bool solid){
	float x, y;
	if(solid){
		glBegin(GL_TRIANGLE_FAN);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		for(int i = slice; i >= 0; i--){
			x = GetSin(i, slice);
			y = GetCos(i, slice);
			glVertex3f(radius * x, radius * y, 0.0f);
		}
		glEnd();
	}
	else{
		glBegin(GL_LINES);
		for(int i = slice; i >= 0; i--){
			x = GetSin(i, slice);
			y = GetCos(i, slice);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(radius * x, radius * y, 0.0f);
		}
		glEnd();
	}
}

void GRDeviceGL::DrawCapsule(float radius, float height, int slice, bool solid){
	DrawCylinder(radius, height, slice, solid, false);

	this->PushModelMatrix();
	this->MultModelMatrix(Affinef::Trn(0.0f, 0.0f, -height/2.0f));
	solid ? glutSolidSphere(radius, slice, slice) : glutWireSphere(radius, slice, slice);
	this->MultModelMatrix(Affinef::Trn(0.0f, 0.0f, height));
	solid ? glutSolidSphere(radius, slice, slice) : glutWireSphere(radius, slice, slice);
	this->PopModelMatrix();
}

void GRDeviceGL::DrawRoundCone(float rbottom, float rtop, float height, int slice, bool solid){
	
	float normal_Z = (rbottom - rtop) / height;
	if (-M_PI/2.0 < normal_Z && normal_Z < M_PI/2.0) {
		float theta = acos(normal_Z);
		float R0 =  rbottom * sin(theta);
		float Z0 = -height / 2.0f + rbottom * cos(theta);
		float R1 =  rtop * sin(theta);
		float Z1 =  height / 2.0f + rtop * cos(theta);

		// 側面を描画
		glBegin(solid ? GL_QUAD_STRIP : GL_LINES);
		float x,y,st;
		for (int i=0; i<=slice; i++) {
			x = GetSin(i, slice);
			y = GetCos(i, slice);
			st = sin(theta);
			glNormal3f(x*st, y*st, cos(theta));
			glVertex3f(R0 * x, R0 * y, Z0);
			glVertex3f(R1 * x, R1 * y, Z1);
		}
		glEnd();
	}

	this->PushModelMatrix();
	this->MultModelMatrix(Affinef::Trn(0.0f, 0.0f, -height/2.0f));
	solid ? glutSolidSphere(rbottom, 20, 20) : glutWireSphere(rbottom, 20, 20);
	this->MultModelMatrix(Affinef::Trn(0.0f, 0.0f, height));
	solid ? glutSolidSphere(rtop, 20, 20) : glutWireSphere(rtop, 20, 20);
	this->PopModelMatrix();

}

void GRDeviceGL::DrawCurve(const Curve3f& curve){
	size_t N = curve.NPoints();

	// 点を描画
	for(uint i = 0; i < N; i++)
		DrawPoint(curve.GetPos(i));
	
	if(N < 2)
		return;

	int type = curve.GetType();
	if(type == Interpolate::LinearDiff){
		for(uint i = 0; i < N-1; i++)
			DrawLine(curve.GetPos(i), curve.GetPos(i+1));		
	}
	else if(type == Interpolate::LinearInt){
		float h;
		Vec3f p;
		Vec3f v;
		for(uint i = 0; i < N-1; i++){
			h = curve.GetTime(i+1) - curve.GetTime(i);
			p = curve.GetPos(i);
			v = curve.GetVel(i);
			DrawLine(p, p + v * h);
		}
	}
	else{
		// それ以外の曲線は折れ線近似で描画
		float t0 = curve.GetTime(0);
		float t1 = curve.GetTime((int)N-1);
		const float ndiv = 100.0f;
		float dt = (t1 - t0) / ndiv;
		Vec3f p0 = curve.GetPos(t0);
		Vec3f p1;
		for(float t = t0 + dt; t < t1; t += dt){
			p1 = curve.GetPos(t);
			DrawLine(p0, p1);
			p0 = p1;
		}
		DrawLine(p0, curve.GetPos(t1));
	}
}

void GRDeviceGL::DrawGrid(float size, int slice, float lineWidth){
	/*double range = 5000;
	GRMaterialDesc mat;
	mat.ambient	 = Vec4f();
	mat.emissive = Vec4f(0.6, 0.6, 0.6, 1.0);
	mat.diffuse	 = Vec4f();
	mat.specular = Vec4f();
	mat.power	 = 0.0;
	this->SetMaterial(mat);*/
	if(slice <= 0)
		return;

	float halfsize = 0.5f * size;
	float div = size / (float)slice;
	float pos = -halfsize;
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
	for(int i = 0; i <= slice; i++){
		glVertex3f(-halfsize, pos, 0.0f);
		glVertex3f( halfsize, pos, 0.0f);
		glVertex3f(pos, -halfsize, 0.0f);
		glVertex3f(pos,  halfsize, 0.0f);
		pos += div;
	}
	glEnd();
}

///	DiplayList の作成
int GRDeviceGL::StartList(){
	int list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	return list;
}
void GRDeviceGL::EndList(){
	glEndList();
}
///	DisplayListの表示	
void GRDeviceGL::DrawList(int list){
	glCallList(list);
}
///	DisplayListの解放	
void GRDeviceGL::ReleaseList(int list){
	glDeleteLists(list, 1);
}
/// 3次元テキストの描画（GLオンリー版でfontは指定なし） .. Vec2f pos
void GRDeviceGL::DrawFont(Vec2f pos, const std::string str){
	DrawFont(Vec3f(pos.x, pos.y, 0.0f), str);
}

/// 3次元テキストの描画（GLオンリー版でfontは指定なし）.. Vec3f pos
void GRDeviceGL::DrawFont(Vec3f pos, const std::string str){
	// シェーダが有効だとビットマップ関係がうまく動かないので一時無効化して対処
	glUseProgram(0);

	bool lighting = !!glIsEnabled(GL_LIGHTING);
	SetLighting(false);
	if(fontBase != -1){
		glBindTexture(GL_TEXTURE_3D,0);								//直前に使用した3Dテクスチャを文字色に反映させない.
		glColor3f(((font.color >> 16) & 0xFF) / 255.0,
				  ((font.color >>  8) & 0xFF) / 255.0,
				  ((font.color >>  0) & 0xFF) / 255.0);
		glRasterPos3fv(pos);
		glPushAttrib(GL_LIST_BIT);
		glListBase(fontBase);											// 	ディスプレイリストを渡す.	
		glCallLists((GLsizei)str.size(), GL_UNSIGNED_BYTE, str.c_str());		// 文字列を渡す.
		glPopAttrib();
	}
	else{
		glColor3f(1.0f, 1.0f, 1.0f);		// 白限定（暫定）
		//glColor3f(0.0f, 0.0f, 0.0f);		// 黒限定（暫定）
		glRasterPos3fv(pos);
		std::string::const_iterator iter;
		for (iter = str.begin(); iter != str.end();	++iter) {
			glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *iter);
		}
	}
	if(lighting)
		SetLighting(true);

	// シェーダを再度有効化
	if(curShader != 0)
		glUseProgram(curShader->programId);
}

void GRDeviceGL::SetFont(const GRFont& font){
	/// VC版のみfontをサポートする。
#ifdef _MSC_VER
	bool	fontFound = false;
	GLsizei	range	 = 256;					//	生成するディスプレイリストの数

	if (fontList.size() > 0) {		// fontListが空でない場合は、fontListの検索が必要
		// ストアサイズ10以上の場合、先頭を削除	
		if (fontList.size() > 10) {		
			fontList.erase(fontList.begin());	
		}
		// fontListを検索
		std::map<unsigned int, GRFont>::iterator itr = fontList.begin();
		while((itr != fontList.end()) && (fontFound == false)) {
			if (((*itr).second.height == font.height)
				&& ((*itr).second.width == font.width)
				&& ((*itr).second.weight == font.weight)
				&& ((*itr).second.bItalic == font.bItalic)
				&& ((strcmp((*itr).second.face.c_str(),font.face.c_str()))==0)
				&& ((*itr).second.color == font.color)){
					fontFound = true;
					fontBase = (*itr).first;
			} 
			++itr;
		}
	}
	if (fontFound == false ) {		// 新規登録の場合
		// 引数で指定された特性を持つ論理フォントを作成する
		HFONT		hFont;			// フォントハンドル
		HFONT		hOldFont;	
		fontBase = glGenLists(range);
		// フォントの作成
		hFont = CreateFont(font.height,						//	フォントの高さ
							font.width,						//	平均文字幅
							0,								//	文字送り方向のX軸の角度
							0,								//	各文字とX軸の角度
							font.weight,					//	フォントの太さ
							font.bItalic,					//	イタリック体
							FALSE,							//	アンダーライン
							FALSE,							//	打ち消し線付
							ANSI_CHARSET,					//	フォント文字セット
							OUT_DEFAULT_PRECIS,				//	出力精度
							CLIP_DEFAULT_PRECIS,			//	クリッピング精度
							ANTIALIASED_QUALITY,			//	出力品質
							FF_DONTCARE | DEFAULT_PITCH,	//	ピッチとファミリ(文字間隔と書体)
							font.face.c_str());				//	タイプフェイス
		
		assert(hFont);
		HDC hDC = wglGetCurrentDC();
		// 0から256のコードの文字を、DisplayListのbase番目から登録.
		// wglUseFontBitmaps()関数、使用して、生成したフォントをディスプレイリストに割当てる.
		hOldFont = (HFONT)SelectObject(hDC, hFont);			
		BOOL b;
		// wglUseFontBitmapsは一度目のコールでエラーし二度目に成功するケースがある
		b = wglUseFontBitmaps(hDC, 0, range, fontBase);
		b = wglUseFontBitmaps(hDC, 0, range, fontBase);
		if (!b){
			DWORD e = GetLastError();
			char* lpMsgBuf;
			FormatMessage(
		        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		        FORMAT_MESSAGE_FROM_SYSTEM,
		        NULL,
				e,
		        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		        (LPSTR)&lpMsgBuf,
		        0, NULL );
			DSTR << "wglUseFontBitmaps() failed with error:" << e << std::endl;
			DSTR << lpMsgBuf << std::endl;
			LocalFree(lpMsgBuf);
		}
		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);		
		// fontListへ登録
		fontList.insert(std::map<unsigned int, GRFont>::value_type(fontBase, font));	
	}
	this->font = font;
#endif
}

void GRDeviceGL::SetMaterial(const GRMaterialIf* mi){
	GRMaterial* mat = mi->Cast();
	currentMaterial = * mat;
	if (mat->texnameAbs.length()) currentMaterial.texname = mat->texnameAbs;
	SetMaterial(currentMaterial);
}
/// 描画の材質の設定
void GRDeviceGL::SetMaterial(const GRMaterialDesc& mat){
	glMaterialfv(GL_FRONT, GL_AMBIENT,   mat.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat.diffuse);
	glColor4fv(mat.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  mat.specular);
	glMaterialfv(GL_FRONT, GL_EMISSION,  mat.emissive);
	glMaterialf (GL_FRONT, GL_SHININESS, mat.power);
	if (mat.texname.length()){
		int texId = LoadTexture(mat.texname);
		if (mat.Is3D()){
			SetTexture2D(false);
			SetTexture3D(true);
			glBindTexture(GL_TEXTURE_3D, texId);
			if (enableDebugMessage){ std::cout << "1: glBindTexture(GL_TEXTURE_3D, " << texId << ");" << std::endl; }
		}else{
			// std::cout << " SetMaterial : id : " << texId << std::endl;
			SetTexture3D(false);
			SetTexture2D(true);
			glBindTexture(GL_TEXTURE_2D, texId);
			if (enableDebugMessage){ std::cout << "2: glBindTexture(GL_TEXTURE_2D, " << texId << ");" << std::endl; }
		}
	}else{
		SetTexture2D(false);
		SetTexture3D(false);
		glBindTexture(GL_TEXTURE_3D, 0);
		if (enableDebugMessage){ std::cout << "3: glBindTexture(GL_TEXTURE_3D, " << 0 << ");" << std::endl; }
		glBindTexture(GL_TEXTURE_2D, 0);
		if (enableDebugMessage){ std::cout << "4: glBindTexture(GL_TEXTURE_2D, " << 0 << ");" << std::endl; }
	}
	currentMaterial = mat;
}

void GRDeviceGL::SetPointSize(float sz, bool smooth){
	if(bPointSmooth && !smooth)
		glDisable(GL_POINT_SMOOTH);
	if(!bPointSmooth && smooth)
		glEnable(GL_POINT_SMOOTH);
	bPointSmooth = smooth;
	glPointSize(sz);
}

void GRDeviceGL::SetLineWidth(float w, bool smooth){
	if(bLineSmooth && !smooth)
		glDisable(GL_LINE_SMOOTH);
	if(!bLineSmooth && smooth)
		glEnable(GL_LINE_SMOOTH);
	bLineSmooth = smooth;
	glLineWidth(w);
}

/// 光源スタックをPush
void GRDeviceGL::PushLight(const GRLightDesc& light){
	if (nLights < GL_MAX_LIGHTS) {
		glLightfv(GL_LIGHT0+nLights, GL_AMBIENT, light.ambient);
		glLightfv(GL_LIGHT0+nLights, GL_DIFFUSE, light.diffuse);
		glLightfv(GL_LIGHT0+nLights, GL_SPECULAR, light.specular);
		glLightfv(GL_LIGHT0+nLights, GL_POSITION, light.position);
		if (light.position.W()){
			// attenuation factor = 1/(GL_CONSTANT_ATTENUATION + GL_LINEAR_ATTENUATION * d + GL_QUADRATIC_ATTENUATION * d^2)
			glLightf(GL_LIGHT0+nLights, GL_CONSTANT_ATTENUATION, light.attenuation0);		// 	一定減衰率
			glLightf(GL_LIGHT0+nLights, GL_LINEAR_ATTENUATION, light.attenuation1);			//	線形減衰率
			glLightf(GL_LIGHT0+nLights, GL_QUADRATIC_ATTENUATION, light.attenuation2);		//	2次減衰率
			glLightfv(GL_LIGHT0+nLights, GL_SPOT_DIRECTION, light.spotDirection);
			glLightf(GL_LIGHT0+nLights, GL_SPOT_EXPONENT, light.spotFalloff);
			glLightf(GL_LIGHT0+nLights, GL_SPOT_CUTOFF, light.spotCutoff);
		}
		glEnable(GL_LIGHT0+nLights);
	}
	nLights ++;
}
/// 光源スタックをPop
void GRDeviceGL::PopLight(){
	nLights--;
	if (nLights < GL_MAX_LIGHTS) glDisable(GL_LIGHT0+nLights);
}
int GRDeviceGL::NLights(){
	return nLights;
}
/// デプスバッファへの書き込みを許可/禁止する
void GRDeviceGL::SetDepthWrite(bool b){
	glDepthMask(b);
}
/// デプステストを有効/無効にする
void GRDeviceGL::SetDepthTest(bool b){
	if(b) glEnable(GL_DEPTH_TEST);
	else  glDisable(GL_DEPTH_TEST);
}
/// デプスバッファ法に用いる判定条件を指定する
void GRDeviceGL::SetDepthFunc(GRRenderBaseIf::TDepthFunc f){
	GLenum dfunc = GL_LESS;
	switch(f){
		case GRRenderBaseIf::DF_NEVER:		dfunc = GL_NEVER;		break;
		case GRRenderBaseIf::DF_LESS:		dfunc = GL_LESS;		break;
		case GRRenderBaseIf::DF_EQUAL:		dfunc = GL_EQUAL;		break;
		case GRRenderBaseIf::DF_LEQUAL:		dfunc = GL_LEQUAL;		break;
		case GRRenderBaseIf::DF_GREATER:	dfunc = GL_GREATER;		break;
		case GRRenderBaseIf::DF_NOTEQUAL:	dfunc = GL_NOTEQUAL;	break;
		case GRRenderBaseIf::DF_GEQUAL:		dfunc = GL_GEQUAL;		break;
		case GRRenderBaseIf::DF_ALWAYS:		dfunc = GL_ALWAYS;		break;
		default:			/* DO NOTHING */		break;

	}
	glDepthFunc(dfunc);
}
/// アルファブレンディングを有効/無効にする
void GRDeviceGL::SetAlphaTest(bool b){
	if(b) glEnable(GL_BLEND);
	else  glDisable(GL_BLEND);
}
/// アルファブレンディングのモード設定(SRCの混合係数, DEST混合係数)
void GRDeviceGL::SetAlphaMode(GRRenderBaseIf::TBlendFunc src, GRRenderBaseIf::TBlendFunc dest){
	GRRenderBaseIf::TBlendFunc  bffac[2] = { src, dest };
	GLenum glfac[2];

	for (int iCnt=0; iCnt<2; ++iCnt){
		switch(bffac[iCnt]) {
			case GRRenderBaseIf::BF_ZERO:				glfac[iCnt] = GL_ZERO;					break;
			case GRRenderBaseIf::BF_ONE:				glfac[iCnt] = GL_ONE;					break;
			case GRRenderBaseIf::BF_SRCCOLOR:			glfac[iCnt] = GL_SRC_COLOR;				break;
			case GRRenderBaseIf::BF_INVSRCCOLOR:		glfac[iCnt] = GL_ONE_MINUS_SRC_COLOR;	break;
			case GRRenderBaseIf::BF_SRCALPHA:			glfac[iCnt] = GL_SRC_ALPHA;				break;
			case GRRenderBaseIf::BF_INVSRCALPHA:		glfac[iCnt] = GL_ONE_MINUS_SRC_ALPHA;	break;
			case GRRenderBaseIf::BF_DESTALPHA:			glfac[iCnt] = GL_DST_ALPHA;				break;
			case GRRenderBaseIf::BF_INVDESTALPHA:		glfac[iCnt] = GL_ONE_MINUS_DST_ALPHA;	break;
			case GRRenderBaseIf::BF_DESTCOLOR:			glfac[iCnt] = GL_DST_COLOR;				break;
			case GRRenderBaseIf::BF_INVDESTCOLOR:		glfac[iCnt] = GL_ONE_MINUS_DST_COLOR;	break;
			case GRRenderBaseIf::BF_SRCALPHASAT:		glfac[iCnt] = GL_SRC_ALPHA_SATURATE;	break;
			case GRRenderBaseIf::BF_BOTHINVSRCALPHA:	glfac[iCnt] = 0;						break;
			case GRRenderBaseIf::BF_BLENDFACTOR:		glfac[iCnt] = 0;						break;
			case GRRenderBaseIf::BF_INVBLENDFACTOR:		glfac[iCnt] = 0;						break;
			default:					/* DO NOTHING */						break;
		}
	}
	glBlendFunc(glfac[0], glfac[1]);
}
void GRDeviceGL::SetLighting(bool on){
	if(on) glEnable (GL_LIGHTING);
	else   glDisable(GL_LIGHTING);

	glUniform1i(enableLightingLoc, (int)on);
}

void GRDeviceGL::SetTexture2D(bool on){
	if(on) glEnable (GL_TEXTURE_2D);
	else   glDisable(GL_TEXTURE_2D);
	glUniform1f(enableTex2DLoc, (float)on);
}

void GRDeviceGL::SetTexture3D(bool on){
	if(on) glEnable (GL_TEXTURE_3D);
	else   glDisable(GL_TEXTURE_3D);
	glUniform1f(enableTex3DLoc, (float)on);
}

void GRDeviceGL::SetTextureImage(const std::string id, int components, int xsize, int ysize, int format, const char* tb){
	unsigned int texId=0;

	char *texbuf = NULL;
	int tx=0, ty=0, nc=0;

	GRTexnameMap::iterator it = texnameMap.find(id);
	if (it != texnameMap.end()) {
		texId = it->second;

		float bx = log((float)xsize)/log(2.0f);
		bx = (int)bx + ((bx - (int)bx) > 0.5);
		float by = log((float)ysize)/log(2.0f);
		by = (int)by + ((by - (int)by) > 0.5);

		tx = (int)(pow(2.0f, bx)) ; ty = (int)(pow(2.0f, by)); nc = 4;
		texbuf  = DBG_NEW char[tx*ty*nc];

		glDisable(GL_TEXTURE_3D);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texId);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		gluScaleImage((GLenum)format, xsize, ysize, GL_UNSIGNED_BYTE, tb, tx, ty, GL_UNSIGNED_BYTE, texbuf);

		int rv = gluBuild2DMipmaps(GL_TEXTURE_2D, nc, tx, ty, (GLenum)format, GL_UNSIGNED_BYTE, texbuf);
		if (rv){DSTR << "SetTextureImage :" << gluErrorString(rv) << std::endl;}
		glBindTexture(GL_TEXTURE_2D, 0);

		delete texbuf;
	}
}

/// テクスチャのロード（戻り値：テクスチャID）
static const GLenum	pxfm[] = {GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_BGR_EXT, GL_BGRA_EXT};
//static const GLenum	pxfm[] = {GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA};
static boost::regex Tex3DRegex("^(.*_tex3d_)([0-9]+)(\\Q.\\E[^\\Q.\\E]+)$");
unsigned int GRDeviceGL::LoadTexture(const std::string filename){
	GRTexnameMap::iterator it = texnameMap.find(filename);
	if (it != texnameMap.end()) return it->second;
	if (enableDebugMessage) { std::cout << "GRDeviceGL::LoadTexture(" << filename.c_str() << ");" << std::endl; }

	// ファイル名が空なら return 0;
	if (filename.empty()) return 0;

	char *texbuf = NULL;
	int tx=0, ty=0, tz=0, nc=0;
	unsigned int texId=0;

	boost::smatch results;
	if (boost::regex_search(filename, results, Tex3DRegex)) {
		//	3D textureの場合
		//	ファイルの数を調べる
		for(tz=0; ; ++tz){
			std::ostringstream fnStr;
			fnStr << results.str(1)
				<< std::setfill('0') << std::setw(results.str(2).length()) << tz
				<< results.str(3);
#ifdef _MSC_VER
			if (_access(fnStr.str().c_str(), 0) != 0) break;
#else
#warning not supported for linux
#endif
		}
		//	画像サイズを調べる
		int pictureSize;
		{
			std::ostringstream fnStr;
			fnStr << results.str(1)
				<< std::setfill('0') << std::setw(results.str(2).length()) << 0
				<< results.str(3);

			int h = LoadBmpCreate(fnStr.str().c_str());
			tx = LoadBmpGetWidth(h);
			ty = LoadBmpGetHeight(h);
			nc = LoadBmpGetBytePerPixel(h);
			pictureSize = tx*ty*nc;
			texbuf = DBG_NEW char[pictureSize * tz];
			LoadBmpRelease(h);
		}
		//	ファイルのロード
		for(int i=0; i<tz; ++i){
			std::ostringstream fnStr;
			fnStr << results.str(1)
				<< std::setfill('0') << std::setw(results.str(2).length()) << i
				<< results.str(3);
			int h = LoadBmpCreate(fnStr.str().c_str());
			int x = LoadBmpGetWidth(h);
			int y = LoadBmpGetHeight(h);
			int c = LoadBmpGetBytePerPixel(h);
			if (x!=tx || y!=ty || c!=nc){
				DSTR << "Error: Texture file '" << fnStr.str() << "' has an illegal format." << std::endl;
				delete texbuf;
				return 0;
			}
			LoadBmpGetBmp(h, texbuf+pictureSize*i);
			LoadBmpRelease(h);	
		}
		// テクスチャの生成．
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_3D);
		glGenTextures(1, (GLuint *)&texId);
		glBindTexture(GL_TEXTURE_3D, texId);
		if (enableDebugMessage){ std::cout << "7: glBindTexture(GL_TEXTURE_3D, " << texId << ");" << std::endl; }
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage3D(GL_TEXTURE_3D, 0, nc, tx, ty, tz, 0, pxfm[nc - 1], GL_UNSIGNED_BYTE, texbuf);
		GLenum err = glGetError();	
		while (err == 0x501 && tx > 2 && ty > 2){
			//	テクスチャのサイズを半分にしてみる
			pictureSize /= 4;
			tx /= 2;
			ty /= 2;
			char* newTexbuf = DBG_NEW char[pictureSize * tz];
			for(int z=0; z<tz; ++z){
				for(int y=0; y<ty; ++y){
					for(int x=0; x<tx; ++x){
						for(int c=0; c<nc; ++c){
							newTexbuf[z*pictureSize + (y*tx + x)*nc+c] 
								= texbuf[z*pictureSize*4 + (2*y*2*tx + 2*x)*nc+c];
						}
					}					
				}					
			}
			delete texbuf;
			texbuf = newTexbuf;
			glTexImage3D(GL_TEXTURE_3D, 0, nc, tx, ty, tz, 0, pxfm[nc - 1], GL_UNSIGNED_BYTE, texbuf);
			err = glGetError();
		}
		if (err){
			DSTR << "glTexImage3D glGetError:" << std::setbase(16) << err << std::endl;
		}
		glBindTexture(GL_TEXTURE_3D, 0);
		if (enableDebugMessage){ std::cout << "8: glBindTexture(GL_TEXTURE_3D, " << 0 << ");" << std::endl; }
		delete texbuf;
	}else{	
		//	2D textureの場合
		
		bool loadFromFile = (filename.c_str()[0]!=':');
		
		if (loadFromFile) {
			// paintLib でファイルをロード．
			int h = LoadBmpCreate(filename.c_str());
			if (!h) {
				DSTR << "GRDeviceGL::LoadTexture() fail to load \'" << filename << "\'." << std::endl;
			}
			tx = LoadBmpGetWidth(h);
			ty = LoadBmpGetHeight(h);
			nc = LoadBmpGetBytePerPixel(h);
			texbuf = DBG_NEW char[tx*ty*nc];
			LoadBmpGetBmp(h, texbuf);
			LoadBmpRelease(h);
		}

		// テクスチャの生成．
		glDisable(GL_TEXTURE_3D);
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, (GLuint *)&texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		if (loadFromFile) {
			int rv = gluBuild2DMipmaps(GL_TEXTURE_2D, nc, tx, ty, pxfm[nc - 1], GL_UNSIGNED_BYTE, texbuf);
			delete texbuf;
			if (rv){
				DSTR << gluErrorString(rv) << std::endl;
				return 0;
			}
		}
		glBindTexture(GL_TEXTURE_2D, 0);

	}
	texnameMap[filename] = texId;
	return texId;
}

#if 0		//	頂点シェーダーを使って頂点ブレンドをする時の参考用
/// インデックス形式によるシェーダを適用した DisplayList の作成（SetVertexFormat() および SetShaderFormat() の後に呼ぶ）
int GRDeviceGL::CreateShaderIndexedList(GRHandler shader, void* location, 
										GRRenderBaseIf::TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride){
	int list = glGenLists(1);
	glNewList(list, GL_COMPILE);

	if (!stride) stride = vertexSize;	
	GLenum mode = GL_TRIANGLES;
	switch(ty) {
		case GRRenderBaseIf::POINTS:			mode = GL_POINTS;			break;
		case GRRenderBaseIf::LINES:				mode = GL_LINES;			break;
		case GRRenderBaseIf::LINE_STRIP:		mode = GL_LINE_STRIP;		break;
		case GRRenderBaseIf::TRIANGLES:			mode = GL_TRIANGLES;		break;
		case GRRenderBaseIf::TRIANGLE_STRIP:	mode = GL_TRIANGLE_STRIP;	break;
		case GRRenderBaseIf::TRIANGLE_FAN:		mode = GL_TRIANGLE_FAN;		break;
		case GRRenderBaseIf::QUADS:				mode = GL_QUADS;			break;
		default:				/* DO NOTHING */			break;
	}											
	
	if ((shaderType == GRShaderFormat::shP3fB4f)		// 他のGRShaderFormatは未対応	
			|| (shaderType == GRShaderFormat::shC4bP3fB4f)
			|| (shaderType == GRShaderFormat::shC3fP3fB4f))			
	{
		glUseProgram(shader);

		// ロケーション型へのキャスト
		GRShaderFormat::SFBlendLocation *loc = (GRShaderFormat::SFBlendLocation *)location;
		if (loc) {
			glUniformMatrix4fv(loc->uBlendMatrix, 4, false, (GLfloat *)&*blendMatrix.begin());
			
			glEnableVertexAttribArray(loc->aWeight); 
			glEnableVertexAttribArray(loc->aMatrixIndices); 
			glEnableVertexAttribArray(loc->aNumMatrix); 
			
			// vtxを頂点フォーマット型へキャスト
			if (shaderType == GRShaderFormat::shP3fB4f) {
				GRVertexElement::VFP3fB4f* basePointer = (GRVertexElement::VFP3fB4f *)vtx;
				glVertexAttribPointer(loc->aWeight, 4, GL_FLOAT, GL_FALSE, vertexSize, &(basePointer->b.data[0]));		
				glVertexAttribPointer(loc->aMatrixIndices, 4, GL_FLOAT, GL_FALSE, vertexSize, &(basePointer->mi.data[0]));
				glVertexAttribPointer(loc->aNumMatrix, 4, GL_FLOAT, GL_FALSE, vertexSize, &(basePointer->nb.data[0]));
			} else if(shaderType == GRShaderFormat::shC4bP3fB4f){
				GRVertexElement::VFC4bP3fB4f* basePointer = (GRVertexElement::VFC4bP3fB4f *)vtx;
				glVertexAttribPointer(loc->aWeight, 4, GL_FLOAT, GL_FALSE, vertexSize, &(basePointer->b.data[0]));		
				glVertexAttribPointer(loc->aMatrixIndices, 4, GL_FLOAT, GL_FALSE, vertexSize, &(basePointer->mi.data[0]));
				glVertexAttribPointer(loc->aNumMatrix, 4, GL_FLOAT, GL_FALSE, vertexSize, &(basePointer->nb.data[0]));
			} else if(shaderType == GRShaderFormat::shC3fP3fB4f){
				GRVertexElement::VFC3fP3fB4f* basePointer = (GRVertexElement::VFC3fP3fB4f *)vtx;
				glVertexAttribPointer(loc->aWeight, 4, GL_FLOAT, GL_FALSE, vertexSize, &(basePointer->b.data[0]));		
				glVertexAttribPointer(loc->aMatrixIndices, 4, GL_FLOAT, GL_FALSE, vertexSize, &(basePointer->mi.data[0]));
				glVertexAttribPointer(loc->aNumMatrix, 4, GL_FLOAT, GL_FALSE, vertexSize, &(basePointer->nb.data[0]));
			}

			glInterleavedArrays(vertexFormatGl, stride, vtx);
			glDrawElements(mode, count, GL_UNSIGNED_INT, idx);	
	
			glDisableVertexAttribArray(loc->aWeight);
			glDisableVertexAttribArray(loc->aMatrixIndices);
			glDisableVertexAttribArray(loc->aNumMatrix);

		} else {
			DSTR << "To Be Implemented. " << std::endl;		
			assert(0);
		}
	} else {
		DSTR << "To Be Implemented. " << std::endl;
		assert(0);
	}									
										
	glEndList();


	return list;
}	
#endif

/// シェーダの初期化	
/*void GRDeviceGL::InitShader(){
#if defined(USE_GLEW)
    glewInit();
	if ( GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader ) {
		DSTR << "GLSL ready" << std::endl;
	} else {
		DSTR << "GLSL not ready" << std::endl;
		assert(0);
	}
#elif defined(GL_VERSION_2_0)    
	if(glslInit())	assert(0);
 	DSTR << "Ready for OpenGL 2.0" << std::endl;
#else
	DSTR << "No GLSL support." << std::endl;
	assert(0);
#endif	
}*/	
/// シェーダフォーマットの設定
/*void GRDeviceGL::SetShaderFormat(GRShaderFormat::ShaderType type){
	shaderType = type;
	 if (type == GRShaderFormat::shP3fB4f) {
		vertexShaderFile = "shP3fB4f.vert";
		fragmentShaderFile = "PassThrough.frag";
	}else if (type == GRShaderFormat::shC4bP3fB4f){
		vertexShaderFile = "shC4bP3fB4f.vert";
		fragmentShaderFile = "PassThrough.frag";
	}else if (type == GRShaderFormat::shC3fP3fB4f){
		vertexShaderFile = "shC3fP3fB4f.vert";
		fragmentShaderFile = "PassThrough.frag";
	}else if (type == GRShaderFormat::shN3fP3fB4f){
		vertexShaderFile = "shN3fP3fB4f.vert";
		fragmentShaderFile = "PassThrough.frag";
	}else if (type == GRShaderFormat::shC4fN3fP3fB4f){
		vertexShaderFile = "shC4fN3fP3fB4f.vert";
		fragmentShaderFile = "PassThrough.frag";
	}else if (type == GRShaderFormat::shT2fP3fB4f){
		vertexShaderFile = "shT2fP3fB4f.vert";
		fragmentShaderFile = "PassThrough.frag";
	}else if (type == GRShaderFormat::shT2fC4bP3fB4f){
		vertexShaderFile = "shT2fC4bP3fB4f.vert";
		fragmentShaderFile = "PassThrough.frag";
	}else if (type == GRShaderFormat::shT2fN3fP3fB4f){
		vertexShaderFile = "shT2fN3fP3fB4f.vert";
		fragmentShaderFile = "PassThrough.frag";
	}else if (type == GRShaderFormat::shT2fC4fN3fP3fB4f){
		vertexShaderFile = "shT2fC4fN3fP3fB4f.vert";
		fragmentShaderFile = "PassThrough.frag";
	}else {
		vertexShaderFile = "";
		fragmentShaderFile = "";
		assert(0);
	}
}*/	
/// シェーダオブジェクトの作成	
/*bool GRDeviceGL::CreateShader(std::string vShaderFile, std::string fShaderFile, GRHandler& shader){
	GRHandler vertexShader;
	GRHandler fragmentShader;
#if defined(USE_GLEW)
    vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB); 
    fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    if (ReadShaderSource(vertexShader, vShaderFile)==false)   return false;
	if (ReadShaderSource(fragmentShader, fShaderFile)==false) return false;
    glCompileShaderARB(vertexShader);
    glCompileShaderARB(fragmentShader);
    shader = glCreateProgramObjectARB();
    glAttachObjectARB(shader, vertexShader);
    glAttachObjectARB(shader, fragmentShader);
	glDeleteObjectARB(vertexShader);
	glDeleteObjectARB(fragmentShader);	
    glLinkProgramARB(shader);
	glUseProgramObjectARB(shader);
#elif defined(GL_VERSION_2_0)    
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);	
    if (ReadShaderSource(vertexShader, vShaderFile)==false)   return false;
    if (ReadShaderSource(fragmentShader, fShaderFile)==false) return false;
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);    
    shader = glCreateProgram();    
    glAttachObject(shader, vertexShader);
    glAttachObject(shader, fragmentShader);	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);	
	glLinkProgram(shader);	
	glUseProgram(shader);	    
#endif
	return true;
}*/		
/// シェーダオブジェクトの作成、GRDeviceGL::shaderへの登録（あらかじめShaderFile名を登録しておく必要がある）	
/*GRHandler GRDeviceGL::CreateShader(){
	GRHandler vertexShader;
	GRHandler fragmentShader;
	GRHandler shaderProgram;
#if defined(USE_GLEW)
//	GLEWContext* ctx = NULL;
//	ctx = glewGetContext();
	vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB); 
    fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    if (ReadShaderSource(vertexShader, vertexShaderFile)==false)   return 0;
	if (ReadShaderSource(fragmentShader, fragmentShaderFile)==false) return 0;
    glCompileShaderARB(vertexShader);
    glCompileShaderARB(fragmentShader);
    shaderProgram = glCreateProgramObjectARB();
    glAttachObjectARB(shaderProgram, vertexShader);
    glAttachObjectARB(shaderProgram, fragmentShader);
	glDeleteObjectARB(vertexShader);
	glDeleteObjectARB(fragmentShader);	
    glLinkProgramARB(shaderProgram);
	glUseProgramObjectARB(shaderProgram);
#elif defined(GL_VERSION_2_0)    
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);	
    if (ReadShaderSource(vertexShader, vertexShaderFile)==false)   return 0;
    if (ReadShaderSource(fragmentShader, fragmentShaderFile)==false) return 0;
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);    
    shaderProgram = glCreateProgram();    
    glAttachObject(shaderProgram, vertexShader);
    glAttachObject(shaderProgram, fragmentShader);	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);	
	glLinkProgram(shaderProgram);	
	glUseProgram(shaderProgram);	    
#endif
	return shaderProgram;
}*/	

bool GRDeviceGL::CheckGLVersion(int major, int minor){
	if(majorVersion < major || (majorVersion == major && minorVersion < minor)){
		DSTR << "this function requires OpenGL " << major << "." << minor << "or higher" << std::endl;
		return false;
	}
	return true;
}

/// シェーダのソースプログラムをメモリに読み込み、シェーダオブジェクトと関連付ける	
bool GRDeviceGL::ReadShaderSource(const char* filename, std::string& src){
	std::ifstream ifs;
	ifs.open(filename, std::ios_base::in);
	if(!ifs.is_open())
		return false;

	char c;
	src.clear();
	while(c = ifs.get(), ifs.good())
		src.push_back(c);
	
	ifs.close();
	return true;
}

GRShaderIf* GRDeviceGL::CreateShader(const GRShaderDesc& sd){
	UTRef<GRShader> shader = DBG_NEW GRShader();
	
	std::string vssrc, fssrc;
	if(!ReadShaderSource(sd.vsname.c_str(), vssrc)){
		DSTR << "failed to read vertex shader source " << sd.vsname << std::endl;
		return 0;
	}
	if(!ReadShaderSource(sd.fsname.c_str(), fssrc)){
		DSTR << "failed to read fragment shader source " << sd.fsname << std::endl;
		return 0;
	}

	// #defineを付加
	std::string defines;
	if(sd.bEnableLighting)
		defines += "#define ENABLE_LIGHTING\n";
	if(sd.bEnableTexture)
		defines += "#define ENABLE_TEXTURE\n";
	if(sd.bShadowCreate)
		defines += "#define SHADOW_CREATE\n";
	if(sd.bShadowRender)
		defines += "#define SHADOW_RENDER\n";
	vssrc = defines + vssrc;
	fssrc = defines + fssrc;

	shader->vertShaderId = glCreateShader(GL_VERTEX_SHADER);
	shader->fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vsSourceArray[] = {vssrc.c_str()};
	const char* fsSourceArray[] = {fssrc.c_str()};

	glShaderSource(shader->vertShaderId, 1, vsSourceArray, NULL);
	glShaderSource(shader->fragShaderId, 1, fsSourceArray, NULL);

	int result;
	
	glCompileShader(shader->vertShaderId);
	glGetShaderiv  (shader->vertShaderId, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE){
		DSTR << "vertex shader compilation failed" << std::endl;
		PrintShaderInfoLog(shader->vertShaderId, false);
		return 0;
	}

	glCompileShader(shader->fragShaderId);
	glGetShaderiv  (shader->fragShaderId, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE){
		DSTR << "fragment shader compilation failed" << std::endl;
		PrintShaderInfoLog(shader->fragShaderId, false);
		return 0;
	}

	shader->programId = glCreateProgram();
	glAttachShader(shader->programId, shader->vertShaderId);
	glAttachShader(shader->programId, shader->fragShaderId);

	glLinkProgram (shader->programId);
	glGetProgramiv(shader->programId, GL_LINK_STATUS, &result);
	if(result == GL_FALSE){
		DSTR << "shader program link failed" << std::endl;
		PrintShaderInfoLog(shader->programId, true);
		return 0;
	}

	shaders.push_back(shader);

	return shader->Cast();
}

bool GRDeviceGL::SetShader(GRShaderIf* sh){
	if(!sh){
		glUseProgram(0);
		curShader = 0;
		return false;
	}

	GRShader* shader = sh->Cast();

	// シェーダを選択
	glUseProgram(shader->programId);
	GLenum rv = glGetError();
	if (rv != GL_NO_ERROR){
		//const GLubyte * str = gluErrorString(rv);
		//DSTR << "Error: glUseProgram returns 0x" << std::setbase(16) << rv << " " << (str ? str : (const GLubyte*)"") << std::endl;
		glUseProgram(0);
		curShader = 0;
		return false;
	}

	// ユニフォーム変数のロケーション取得
	enableLightingLoc = glGetUniformLocation(shader->programId, "enableLighting");
	enableTex2DLoc    = glGetUniformLocation(shader->programId, "enableTex2D"   );
	enableTex3DLoc    = glGetUniformLocation(shader->programId, "enableTex3D"   );
	tex2DLoc          = glGetUniformLocation(shader->programId, "tex2D"         );
	tex3DLoc          = glGetUniformLocation(shader->programId, "tex3D"         );
	shadowTexLoc      = glGetUniformLocation(shader->programId, "shadowTex"     );
	shadowMatrixLoc   = glGetUniformLocation(shader->programId, "shadowMatrix"  );
	shadowColorLoc    = glGetUniformLocation(shader->programId, "shadowColor"   );
	
	// テクスチャサンプラとテクスチャユニット番号の対応
	if(tex2DLoc != -1)
		glUniform1i(tex2DLoc, 0);
	if(tex3DLoc != -1)
		glUniform1i(tex3DLoc, 0);
	if(shadowTexLoc != -1)
		glUniform1i(shadowTexLoc, 1);

	curShader = shader;
	return true;
}

/*
void GRDeviceGL::DeleteShader(){
	if(vertShaderId != -1)
		glDeleteShader(vertShaderId);
	if(fragShaderId != -1)
		glDeleteShader(fragShaderId);
	if(programId != -1)
		glDeleteProgram(programId);
	vertShaderId = fragShaderId = programId = -1;
}
*/

void GRDeviceGL::PrintShaderInfoLog(int id, bool prog_or_shader){
	int loglen;
	if(prog_or_shader)
		 glGetProgramiv(id, GL_INFO_LOG_LENGTH, &loglen);
	else glGetShaderiv (id, GL_INFO_LOG_LENGTH, &loglen);

	if(loglen > 0){
		std::string logstr;
		logstr.resize(loglen);
		GLsizei nwritten;
		if(prog_or_shader)
			 glGetProgramInfoLog(id, loglen, &nwritten, &logstr[0]);
		else glGetShaderInfoLog (id, loglen, &nwritten, &logstr[0]);

		DSTR << logstr << std::endl;
	}
}

/// ロケーション情報の取得（SetShaderFormat()でシェーダフォーマットを設定しておく必要あり）
/*void GRDeviceGL::GetShaderLocation(GRHandler shader, void* location){
	if ((shaderType == GRShaderFormat::shP3fB4f)
			|| (shaderType == GRShaderFormat::shC4bP3fB4f)
			|| (shaderType == GRShaderFormat::shC3fP3fB4f))
	{
		// shaderType が、shP3fB4f または shC4bP3fB4f または shC3fP3fB4f で、
		// ロケーション情報 に SFBlendLocation の型が指定された場合
		SFBlendLocation *loc = (SFBlendLocation *)location;
		if (loc) {
			loc->uBlendMatrix	= glGetUniformLocationARB(shader, "blendMatrix" );
			loc->aWeight		= glGetAttribLocationARB(shader, "weights");;
			loc->aMatrixIndices	= glGetAttribLocationARB(shader, "matrixIndices");
			loc->aNumMatrix		= glGetAttribLocationARB(shader, "numMatrix");
		} else {
			DSTR << "To Be Implemented. " << std::endl;
			assert(0);
		}
	} else {
		DSTR << "To Be Implemented. " << std::endl;
		assert(0);
	}
}*/	
	
void GRDeviceGL::SetShadowLight(const GRShadowLightDesc& sld){
	if(!CheckGLVersion(3, 0))
		return;

	shadowView.Pos() = sld.position;
	shadowView.LookAtGL(sld.lookat, sld.up);
	shadowView = shadowView.inv();

	// スクリーンの幅
	shadowProj.clear();
	if(sld.directional){
		shadowProj[0][0] =  2.0f / sld.size.x;
		shadowProj[1][1] =  2.0f / sld.size.y;
		shadowProj[2][2] = -2.0f / (sld.back - sld.front);
		shadowProj[2][3] = -(sld.back + sld.front) / (sld.back - sld.front);
		shadowProj[3][3] =  1.0f;
	}
	else{
		float f = 1.0f / tanf(sld.fov / 2.0f);
		shadowProj[0][0] = f;
		shadowProj[1][1] = f;
		shadowProj[2][2] = - (sld.back + sld.front) / (sld.back - sld.front);
		shadowProj[2][3] = -2.0f * sld.front * sld.back / (sld.back - sld.front);
		shadowProj[3][2] = -1.0f;
	}

	CalcShadowMatrix();

	if(shadowTexId == 0 || shadowDesc.texWidth != sld.texWidth || shadowDesc.texHeight != sld.texHeight){
		float border[] = {1.0f, 0.0f, 0.0f, 0.0};
		if(shadowTexId != 0)
			glDeleteTextures(1, &shadowTexId);

		glGenTextures(1, &shadowTexId);
		glBindTexture(GL_TEXTURE_2D, shadowTexId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, sld.texWidth, sld.texHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR/*GL_NEAREST*/);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*GL_NEAREST*/);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
	}

	// フレームバッファ作成
	if(shadowBufferId == 0){
		glGenFramebuffers(1, &shadowBufferId);
	}

	// フレームバッファにシャドウテクスチャをバインド
	glBindFramebuffer(GL_FRAMEBUFFER, shadowBufferId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTexId, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shadowDesc = sld;
}

void GRDeviceGL::CalcShadowMatrix(){
	const float bias[] = {
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0};
	shadowMatrix = (*(Affinef*)bias) * shadowProj * shadowView * modelMatrix;
	glUniformMatrix4fv(shadowMatrixLoc, 1, false, (float*)&shadowMatrix);
}

void GRDeviceGL::EnterShadowMapGeneration(){
	if(!CheckGLVersion(3, 0))
		return;

	if(shadowTexId == 0 || shadowBufferId == 0)
		SetShadowLight(shadowDesc);

	// シャドウテクスチャ用フレームバッファをバインド
	glBindFramebuffer(GL_FRAMEBUFFER, shadowBufferId);

	// カラーバッファへの書き込みを禁止
	glDrawBuffer(GL_NONE);

	// 現在の始点・投影変換を退避
	GetViewMatrix      (shadowViewTmp);
	GetProjectionMatrix(shadowProjTmp);

	// 光源に合わせた視点・投影変換
	SetViewMatrix      (shadowView);
	SetProjectionMatrix(shadowProj);

	// ビューポート退避
	shadowVpPosTmp  = GetViewportPos();
	shadowVpSizeTmp = GetViewportSize();

	// テクスチャ用ビューポート
	SetViewport(Vec2f(), Vec2f((float)shadowDesc.texWidth, (float)shadowDesc.texHeight));

	// デプスオフセット
	// - GL_LINESなどの線描画にはオフセットが効かないので不要な影がついてしまう
	// - 線描画時にglPolygonModeでGL_LINEに切り替えれば良さそうだが未対応
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, shadowDesc.offset);

	// 前面カリング
	// - 両面描画してるケースが割りと多いので却下
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	//＊デプスバッファをクリアはユーザが行う

}

void GRDeviceGL::LeaveShadowMapGeneration(){
	if(!CheckGLVersion(3, 0))
		return;

	// デフォルトフレームバッファをバインド
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 視点・投影変換を復帰
	SetViewMatrix      (shadowViewTmp);
	SetProjectionMatrix(shadowProjTmp);

	// ビューポートを復帰
	SetViewport(shadowVpPosTmp, shadowVpSizeTmp);

	// シャドウテクスチャをユニット1にバインド
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowTexId);
	glActiveTexture(GL_TEXTURE0);

	// デプスオフセット
	glPolygonOffset(0.0f, 0.0f);

	// 影の色と透明度
	glUniform4fv(shadowColorLoc, 1, (float*)&shadowDesc.color);

	// 背面カリング有効化
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
}

}	//	Spr

