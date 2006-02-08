#include "Graphics.h"
#pragma hdrstop
#include <gl/glut.h>

namespace Spr {;
//----------------------------------------------------------------------------
//	GRDeviceGL
OBJECTIMP(GRDeviceGL, GRDevice);
IF_IMP(GRDeviceGL, GRDevice);
/// 初期設定
void GRDeviceGL::Init(){
	glDrawBuffer(GL_BACK);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	// 視点行列の設定
	viewMatrix.Pos() = Vec3f(0.0, 5.0, 10.0);	                        // eye
	viewMatrix.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));	// center, up 
	viewMatrix = viewMatrix.inv();			
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(viewMatrix);
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
/// モデル行列をかける
void GRDeviceGL::MultModelMatrix(const Affinef& afw){	
	glMatrixMode(GL_MODELVIEW);
	glMultMatrixf(afw);
}
/// モデル行列の行列スタックをPush
void GRDeviceGL::PushModelMatrix(){
	glPushMatrix();
}
/// モデル行列の行列スタックをPop
void GRDeviceGL::PopModelMatrix(){
	glPopMatrix();
}
/// モデル行列を設定
void GRDeviceGL::SetModelMatrix(const Affinef& afw){
	modelMatrix = afw;		// モデル行列の保存
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(viewMatrix * modelMatrix);
}
///	視点行列を設定
void GRDeviceGL::SetViewMatrix(const Affinef& afv){   
	viewMatrix  = afv;		// 視点行列の保存
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(viewMatrix);
}
/// 投影行列を設定
void GRDeviceGL::SetProjectionMatrix(const Affinef& afp){  
	projectionMatrix = afp;
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projectionMatrix);
	glMatrixMode(GL_MODELVIEW);
}
/// 頂点座標を指定してプリミティブを描画
void GRDeviceGL::DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end){
	GLenum mode;
	switch(ty) {
		case POINTS: 
			mode = GL_POINTS;
			break;
		case LINES:
			mode = GL_LINES;
			break;
		case LINE_STRIP:
			mode = GL_LINE_STRIP;
			break;
		case TRIANGLES:
			mode = GL_TRIANGLES;
			break;
		case TRIANGLE_STRIP:
			mode = GL_TRIANGLE_STRIP;
			break;
		case TRIANGLE_FAN:
			mode = GL_TRIANGLE_FAN;
			break;
		default:
			/* DO NOTHING */
			break;
	}
	glBegin(mode);
	for(;begin!=end; ++begin)
		glVertex3fv(*begin);
	glEnd();
}
/// 頂点座標とインデックスを指定してプリミティブを描画
void GRDeviceGL::DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx){
	GLenum mode;
	switch(ty) {
		case POINTS: 
			mode = GL_POINTS;
			break;
		case LINES:
			mode = GL_LINES;
			break;
		case LINE_STRIP:
			mode = GL_LINE_STRIP;
			break;
		case TRIANGLES:
			mode = GL_TRIANGLES;
			break;
		case TRIANGLE_STRIP:
			mode = GL_TRIANGLE_STRIP;
			break;
		case TRIANGLE_FAN:
			mode = GL_TRIANGLE_FAN;
			break;
		default:
			/* DO NOTHING */
			break;
	}
	glBegin(mode);
	for(;begin!=end; ++begin) glVertex3fv(vtx[*begin]);
	glEnd();
}


void GRDeviceGL::DrawText(Vec2f pos, std::string str, const GRFont& font){}
void GRDeviceGL::SetMaterial(const GRMaterial& mat){}
void GRDeviceGL::SetLineWidth(float w){}
void GRDeviceGL::PushLight(const GRLight& m){}
void GRDeviceGL::PopLight(){}
void GRDeviceGL::SetDepthWrite(bool b){}
void GRDeviceGL::SetDepthTest(bool b){}
void GRDeviceGL::SetDepthFunc(TDepthFunc f){}
void GRDeviceGL::SetAlphaMode(TBlendFunc src, TBlendFunc dest){}

}	//	Spr

