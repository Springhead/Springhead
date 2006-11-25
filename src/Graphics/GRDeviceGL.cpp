/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#define USE_GREW
#define GLEWMX

#if defined(USE_GREW)
# include <GL/glew.h>
#endif

#include "Graphics.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#ifdef _MSC_VER
# include <windows.h>
#endif
#include <stack>


#include <GL/glut.h>
#include "GRLoadBmp.h"




namespace Spr {;
//----------------------------------------------------------------------------
//	GRDeviceGL
IF_OBJECT_IMP(GRDeviceGL, GRDevice);
/// 初期設定
void GRDeviceGL::Init(){
	nLights = 0;
	glDrawBuffer(GL_BACK);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
//	glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
	SetVertexFormat(GRVertexElement::vfP3f);
	
	// 視点行列の設定
	viewMatrix.Pos() = Vec3f(0.0, 0.0, 1.0);	                        // eye
	viewMatrix.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));	// center, up 
	viewMatrix = viewMatrix.inv();			
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(viewMatrix);
}
///	Viewport設定
void GRDeviceGL::SetViewport(Vec2f pos, Vec2f sz){
	glViewport((int)pos.x , (int)pos.y, (int)sz.x, (int)sz.y);
}
/// バッファクリア
void GRDeviceGL::ClearBuffer(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
/// レンダリングの開始前に呼ぶ関数
void GRDeviceGL::BeginScene(){
  /// GLでレンダリングする場合、特に処理する必要なし
}
/// レンダリングの終了後に呼ぶ関数
void GRDeviceGL::EndScene(){
  /// ダブルバッファモード時、カレントウィンドウのバッファ交換を行う
	glutSwapBuffers();
}
///	カレントの視点行列をafvで置き換える
void GRDeviceGL::SetViewMatrix(const Affinef& afv){   
	viewMatrix  = afv;		// 視点行列の保存
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(viewMatrix);
}
/// カレントの投影行列をafpで置き換える
void GRDeviceGL::SetProjectionMatrix(const Affinef& afp){  
	projectionMatrix = afp;
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projectionMatrix);
	glMatrixMode(GL_MODELVIEW);
}
/// カレントのモデル行列をafwで置き換える
void GRDeviceGL::SetModelMatrix(const Affinef& afw){
	modelMatrix = afw;		// モデル行列の保存
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(viewMatrix * modelMatrix);
}
/// カレントのモデル行列に対してafwを掛ける
void GRDeviceGL::MultModelMatrix(const Affinef& afw){	
	modelMatrix = modelMatrix * afw;		
	glMatrixMode(GL_MODELVIEW);
	glMultMatrixf(afw);
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
}
/// 複数モデル行列の modelMatrices[matrixId][0] をカレントのモデル行列として置き換える
bool GRDeviceGL::SetModelMatrices(const Affinef& afw, unsigned int matrixId, unsigned int elementId){
	if (matrixId < modelMatrices.size()){
		if (elementId < modelMatrices[matrixId].size()){
			modelMatrices[matrixId][elementId] = afw;
			return true;
		}
	}
	return false;
}
/// 複数モデル行列において、モデル行列afwを掛ける（modelMatrices[matrixId][0] *= afw;）
bool GRDeviceGL::MultModelMatrices(const Affinef& afw, unsigned int matrixId){
	if (matrixId < modelMatrices.size()){
		modelMatrices[matrixId][0] = modelMatrices[matrixId][0] * afw;
		return true;
	}
	return false;
}	
/// 複数モデル行列の modelMatrices[matrixId] にafwを追加する
bool GRDeviceGL::PushModelMatrices(const Affinef& afw, unsigned int matrixId){
	if (matrixId < modelMatrices.size()){
		modelMatrices[matrixId].push_back(afw);
	} else if( matrixId == modelMatrices.size()){
		std::vector<Affinef> tmpafw;
		tmpafw.push_back(afw);
		modelMatrices.push_back(tmpafw);
	} else{
		return false;
	}
	return true;
}	
/// 複数モデル行列の modelMatrices[matrixId] の最後尾の要素を削除する
bool GRDeviceGL::PopModelMatrices(unsigned int matrixId){
	if (matrixId < modelMatrices.size()){
		modelMatrices[matrixId].pop_back();
		return true;
	} else{
		return false;
	}
}
/// 頂点座標を指定してプリミティブを描画
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
	}else if (e == GRVertexElement::vfT2fC4bP3f){
		vertexFormatGl = GL_T2F_C4UB_V3F;
		vertexSize = sizeof(float)*5 + sizeof(char)*4;
		vertexColor = true;
	}else if (e == GRVertexElement::vfT2fN3fP3f){
		vertexFormatGl = GL_T2F_N3F_V3F;
		vertexSize = sizeof(float)*8;
		vertexColor = false;
	}else if (e == GRVertexElement::vfT2fC4fN3fP3f){
		vertexFormatGl = GL_T2F_C4F_N3F_V3F;
		vertexSize = sizeof(float)*12;
		vertexColor = true;
	}else if (e == GRVertexElement::vfP3fB1f) {
		vertexFormatGl = GL_V3F; 
		vertexSize = sizeof(float)*4;
		vertexColor = false;
	}else if (e == GRVertexElement::vfC4bP3fB1f){
		vertexFormatGl = GL_C4UB_V3F;
		vertexSize = sizeof(float)*4+sizeof(char)*4;
		vertexColor = true;
	}else if (e == GRVertexElement::vfN3fP3fB1f){
		vertexFormatGl = GL_N3F_V3F;
		vertexSize = sizeof(float)*7;
		vertexColor = false;
	}else if (e == GRVertexElement::vfC4fN3fP3fB1f){
		vertexFormatGl = GL_C4F_N3F_V3F;
		vertexSize = sizeof(float)*11;
		vertexColor = true;
	}else if (e == GRVertexElement::vfT2fP3fB1f){
		vertexFormatGl = GL_T2F_V3F;
		vertexSize = sizeof(float)*6;
		vertexColor = false;
	}else if (e == GRVertexElement::vfT2fC4bP3fB1f){
		vertexFormatGl = GL_T2F_C4UB_V3F;
		vertexSize = sizeof(float)*6 + sizeof(char)*4;
		vertexColor = true;
	}else if (e == GRVertexElement::vfT2fN3fP3fB1f){
		vertexFormatGl = GL_T2F_N3F_V3F;
		vertexSize = sizeof(float)*9;
		vertexColor = false;
	}else if (e == GRVertexElement::vfT2fC4fN3fP3fB1f){
		vertexFormatGl = GL_T2F_C4F_N3F_V3F;
		vertexSize = sizeof(float)*13;
		vertexColor = true;
	}else {
		vertexFormatGl = 0;
		vertexSize = 0;
		vertexColor = false;
		assert(0);
	}
}
/// 頂点座標を指定してプリミティブを描画
void GRDeviceGL::SetVertexShader(void* s){
	assert(0);	//	To Be Implemented
}

/// 頂点座標を指定してプリミティブを描画
void GRDeviceGL::DrawDirect(GRRenderBaseIf::TPrimitiveType ty, void* vtx, size_t count, size_t stride){
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
	if (!stride) stride = vertexSize;
	if (vertexColor) glEnable(GL_COLOR_MATERIAL);
	else glDisable(GL_COLOR_MATERIAL);
	SetMaterial(currentMaterial);
	glInterleavedArrays(vertexFormatGl, stride, vtx);
	glDrawArrays(mode, 0, count);
}
/// 頂点座標とインデックスを指定してプリミティブを描画
void GRDeviceGL::DrawIndexed(GRRenderBaseIf::TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride){
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
	if (!stride) stride = vertexSize;
	if (vertexColor) glEnable(GL_COLOR_MATERIAL);	
	else glDisable(GL_COLOR_MATERIAL);
	SetMaterial(currentMaterial);
	glInterleavedArrays(vertexFormatGl, stride, vtx);
	glDrawElements(mode, count, GL_UNSIGNED_INT, idx);
}
int GRDeviceGL::CreateList(GRRenderBaseIf::TPrimitiveType ty, void* vtx, size_t count, size_t stride){
	int list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	DrawDirect(ty, vtx, count, stride);
	glEndList();
	return list;
}
int GRDeviceGL::CreateList(GRMaterialIf* mat, unsigned int texid, 
						   GRRenderBaseIf::TPrimitiveType ty, void* vtx, size_t count, size_t stride){
	int list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	SetMaterial(*DCAST(GRMaterial, mat));
	if (texid){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texid);
	}else{
		glDisable(GL_TEXTURE_2D);
	}
	DrawDirect(ty, vtx, count, stride);
	glEndList();
	return list;
}
int GRDeviceGL::CreateList(float radius, int slices, int stacks){
	int list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	glutSolidSphere(radius, slices, stacks);
	glEndList();
	return list;
}
int GRDeviceGL::CreateList(GRMaterialIf* mat, float radius, int slices, int stacks){
	int list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	GRMaterialDesc* desc = DCAST(GRMaterial, mat);
	SetMaterial(*desc);
	unsigned int texid = LoadTexture(desc->texname);
	if (texid){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texid);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}else{
		glDisable(GL_TEXTURE_2D);
	}		
	glutSolidSphere(radius, slices, stacks);
	glEndList();
	return list;	
}		
int GRDeviceGL::CreateIndexedList(GRRenderBaseIf::TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride){
	int list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	DrawIndexed(ty, idx, vtx, count, stride);
	glEndList();
	return list;
}
int GRDeviceGL::CreateIndexedList(GRMaterialIf* mat,  
								  GRRenderBaseIf::TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride){
	int list = glGenLists(1);						  
	glNewList(list, GL_COMPILE);
	GRMaterialDesc* desc = DCAST(GRMaterial, mat);
	SetMaterial(*desc);								  	
	unsigned int texid = LoadTexture(desc->texname);
	if (texid){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texid);
	}else{
		glDisable(GL_TEXTURE_2D);
	}
	DrawIndexed(ty, idx, vtx, count, stride);
	glEndList();
	return list;
}
void GRDeviceGL::DrawList(int list){
	glCallList(list);
}
void GRDeviceGL::ReleaseList(int list){
	glDeleteLists(list, 1);
}
/// 3次元テキストの描画（GLオンリー版でfontは指定なし） .. Vec2f pos
void GRDeviceGL::DrawFont(Vec2f pos, const std::string str){
	glDisable(GL_LIGHTING);
	glRasterPos2fv(pos);
	std::string::const_iterator iter;
	for (iter = str.begin(); iter != str.end();	++iter) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *iter);
	}
	glEnable(GL_LIGHTING);
}
/// 3次元テキストの描画（GLオンリー版でfontは指定なし）.. Vec3f pos
void GRDeviceGL::DrawFont(Vec3f pos, const std::string str){
	glDisable(GL_LIGHTING);
	glRasterPos3fv(pos);
	std::string::const_iterator iter;
	for (iter = str.begin(); iter != str.end();	++iter) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *iter);
	}
	glEnable(GL_LIGHTING);
}
/// 3次元テキストの描画 ... Vec2f pos
void GRDeviceGL::DrawFont(Vec2f pos, const std::string str, const GRFont& font){
/// VC版のみfontをサポートする。
#ifdef _MSC_VER	
	GLuint	base = 0;						// DisplayList開始指標番号
	bool	findFont = false;				// fontListの検索結果
	GLsizei	range	 = 256;					//	生成するディスプレイリストの数
	unsigned long color=font.color;			//	フォントの色

	if (fontList.size() > 0) {		// fontListが空でない場合は、fontListの検索が必要
		// ストアサイズ10以上の場合、先頭を削除	
		if (fontList.size() > 10) {		
			fontList.erase(fontList.begin());	
		}
		// fontListを検索
		std::map<unsigned int, GRFont>::iterator itr = fontList.begin();
		while((itr != fontList.end()) && (findFont == false)) {
			if (((*itr).second.height == font.height)
				&& ((*itr).second.width == font.width)
				&& ((*itr).second.weight == font.weight)
				&& ((*itr).second.bItalic == font.bItalic)
				&& ((strcmp((*itr).second.face.c_str(),font.face.c_str()))==0)
				&& ((*itr).second.color == font.color)){
					findFont = true;
					base = (*itr).first;
					color = (*itr).second.color;
			} 
			++itr;
		}
	}
	if (findFont == false ) {		// 新規登録の場合
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
		HDC hDC = GetDC(0);
		// 0から256のコードの文字を、DisplayListのbase番目から登録.
		// wglUseFontBitmaps()関数、使用して、生成したフォントをディスプレイリストに割当てる.
		hOldFont = (HFONT)SelectObject(hDC, hFont);			
		wglUseFontBitmaps(hDC, 0, range, fontBase);		
		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);		
		// fontListへ登録
		fontList.insert(std::map<unsigned int, GRFont>::value_type(fontBase, font));	
		base = fontBase;	
	}

	glDisable(GL_LIGHTING);
	glColor3f(((font.color >> 16) & 0xFF) / 255.0,
			  ((font.color >>  8) & 0xFF) / 255.0,
			  ((font.color >>  0) & 0xFF) / 255.0);
	glRasterPos2fv(pos);
	glPushAttrib(GL_LIST_BIT);
	glListBase(base);											// 	ディスプレイリストを渡す.	
	glCallLists(str.size(), GL_UNSIGNED_BYTE, str.c_str());		// 文字列を渡す.
	glPopAttrib();
	glEnable(GL_LIGHTING);
#else	// _MSC_VER	
	// VC以外の環境では、fontをサポートしない。
	DrawFont(pos, str);
#endif	
}
/// 3次元テキストの描画  ... Vec3f pos
void GRDeviceGL::DrawFont(Vec3f pos, const std::string str, const GRFont& font){
/// VC版のみfontをサポートする。
#ifdef _MSC_VER		
	GLuint	base = 0;						// DisplayList開始指標番号
	bool	findFont = false;				// fontListの検索結果
	GLsizei	range	 = 256;					//	生成するディスプレイリストの数
	unsigned long color=font.color;			//	フォントの色

	if (fontList.size() > 0) {		// fontListが空でない場合は、fontListの検索が必要
		// ストアサイズ10以上の場合、先頭を削除	
		if (fontList.size() > 10) {		
			fontList.erase(fontList.begin());	
		}
		// fontListを検索
		std::map<unsigned int, GRFont>::iterator itr = fontList.begin();
		while((itr != fontList.end()) && (findFont == false)) {
			if (((*itr).second.height == font.height)
				&& ((*itr).second.width == font.width)
				&& ((*itr).second.weight == font.weight)
				&& ((*itr).second.bItalic == font.bItalic)
				&& ((strcmp((*itr).second.face.c_str(),font.face.c_str()))==0)
				&& ((*itr).second.color == font.color)){
					findFont = true;
					base = (*itr).first;
					color = (*itr).second.color;
			} 
			++itr;
		}
	}
	if (findFont == false ) {		// 新規登録の場合
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
		HDC hDC = GetDC(0);
		// 0から256のコードの文字を、DisplayListのbase番目から登録.
		// wglUseFontBitmaps()関数、使用して、生成したフォントをディスプレイリストに割当てる.
		hOldFont = (HFONT)SelectObject(hDC, hFont);			
		wglUseFontBitmaps(hDC, 0, range, fontBase);		
		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);		
		// fontListへ登録
		fontList.insert(std::map<unsigned int, GRFont>::value_type(fontBase, font));	
		base = fontBase;	
	}

	glDisable(GL_LIGHTING);
	glColor3f(((font.color >> 16) & 0xFF) / 255.0,
			  ((font.color >>  8) & 0xFF) / 255.0,
			  ((font.color >>  0) & 0xFF) / 255.0);
	glRasterPos3fv(pos);
	glPushAttrib(GL_LIST_BIT);
	glListBase(base);											// 	ディスプレイリストを渡す.	
	glCallLists(str.size(), GL_UNSIGNED_BYTE, str.c_str());		// 文字列を渡す.
	glPopAttrib();
	glEnable(GL_LIGHTING);
#else	// _MSC_VER	
	// VC以外の環境では、fontをサポートしない。
	DrawFont(pos, str);
#endif	
}
/// 描画の材質の設定
void GRDeviceGL::SetMaterial(const GRMaterialDesc& mat){
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat.ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat.diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat.specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,  mat.emissive);
	glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, mat.power);
	currentMaterial = mat;
}
/// 描画する点・線の太さの設定
void GRDeviceGL::SetLineWidth(float w){
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
/// デプスバッファへの書き込みを許可/禁止する
void GRDeviceGL::SetDepthWrite(bool b){
	glDepthMask(b);
}
/// デプステストを有効/無効にする
void GRDeviceGL::SetDepthTest(bool b){
if (b) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);
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
if (b) glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
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
/// テクスチャのロード（戻り値：テクスチャID）	
static const GLenum	pxfm[] = {GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_BGR_EXT, GL_BGRA_EXT};
unsigned int GRDeviceGL::LoadTexture(const std::string filename){
	char *texbuf = NULL;
	int tx=0, ty=0, nc=0;
	unsigned int texId=0;

	// ファイル名が空なら return 0;
	if (filename.empty()) return 0;

	// paintLib でファイルをロード．
	int h = LoadBmpCreate(filename.c_str());
	tx = LoadBmpGetWidth(h);
	ty = LoadBmpGetHeight(h);
	nc = LoadBmpGetBytePerPixel(h);
	texbuf = new char[tx*ty*nc];
	LoadBmpGetBmp(h, texbuf);
	LoadBmpRelease(h);

	// テクスチャの生成．
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	int rv = gluBuild2DMipmaps(GL_TEXTURE_2D, nc, tx, ty, pxfm[nc - 1], GL_UNSIGNED_BYTE, texbuf);
	if (rv){
		DSTR << gluErrorString(rv) << std::endl;
	}
	delete texbuf;

	return texId;
}
/// シェーダの初期化	
void GRDeviceGL::InitShader(){
#if defined(USE_GREW)
    glewInit();
    if (GLEW_ARB_vertex_program && GLEW_ARB_fragment_program)
		DSTR << "Ready for GLSL" << std::endl;
    else {
		DSTR << "No GLSL support." << std::endl;
		assert(0);
    }
#elif defined(GL_VERSION_2_0)    
	if(glslInit())	assert(0);
 	DSTR << "Ready for OpenGL 2.0" << std::endl;
#else
	DSTR << "No GLSL support." << std::endl;
	assert(0);
#endif	
}	
/// シェーダオブジェクトの作成	
bool GRDeviceGL::CreateShader(std::string vShaderFile, std::string fShaderFile, GRHandler& shaderProgram){
	GRHandler vertexShader;
	GRHandler fragmentShader;
#if defined(USE_GREW)
    vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB); 
    fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
    if (ReadShaderSource(vertexShader, vShaderFile)==false)   return false;
	if (ReadShaderSource(fragmentShader, fShaderFile)==false) return false;
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
    if (ReadShaderSource(vertexShader, vShaderFile)==false)   return false;
    if (ReadShaderSource(fragmentShader, fShaderFile)==false) return false;
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
	return true;
}	
/// シェーダオブジェクトの作成	
bool GRDeviceGL::CreateShader(std::string vShaderFile, GRHandler& shaderProgram){
	GRHandler vertexShader;
#if defined(USE_GREW)
    vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB); 
    if (ReadShaderSource(vertexShader, vShaderFile)==false)   return false;
    glCompileShaderARB(vertexShader);
    shaderProgram = glCreateProgramObjectARB();
    glAttachObjectARB(shaderProgram, vertexShader);
	glDeleteObjectARB(vertexShader);
    glLinkProgramARB(shaderProgram);
	glUseProgramObjectARB(shaderProgram);
#elif defined(GL_VERSION_2_0)    
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
    if (ReadShaderSource(vertexShader, vShaderFile)==false)   return false;
    glCompileShader(vertexShader);
    shaderProgram = glCreateProgram();    
    glAttachObject(shaderProgram, vertexShader);
	glDeleteShader(vertexShader);
	glLinkProgram(shaderProgram);	
	glUseProgram(shaderProgram);	    
#endif
	return true;
}		
/// シェーダのソースプログラムをメモリに読み込み、シェーダオブジェクトと関連付ける	
bool GRDeviceGL::ReadShaderSource(GRHandler shader, std::string file){
	FILE *fp;
	char *source = NULL;
	int length=0;

	if (file.empty()) 	
		return false;

	fp = fopen(file.c_str(),"rb");
	if (fp != NULL) {      
      	fseek(fp, 0, SEEK_END);
      	length = ftell(fp);
      	rewind(fp);					
		if (length > 0) {
			source = (char *)malloc(sizeof(char) * (length+1));
			if (source == NULL) {
				DSTR << "Could not allocate read buffer." << std::endl;
				return false;
			}
			length = fread((void *)source,sizeof(char),length,fp);
			source[length] = '\0';
		}
		fclose(fp);
	}	
	const char* sourceStrings =  (char*)source;
#if defined(USE_GREW)	
	glShaderSourceARB(shader, 1, &sourceStrings, &length);
	//glShaderSourceARB(shader, 1, &source, NULL);		
#elif defined(GL_VERSION_2_0) 
	glShaderSource(shader, 1, &sourceStrings, &length);
#else
	return false;
#endif;														
	free((void*)source);
	return true;
}		
	
}	//	Spr

