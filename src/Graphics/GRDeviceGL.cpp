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

	size = Vec2f(300, 300);
	center = Vec2f(0.0, 0.0);
	front = 1.0;
	back = 500.0;

	// 視点行列の設定
	viewMatrix.Pos() = Vec3f(0.0, 5.0, 10.0);	                        // eye
	viewMatrix.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));	// center, up 
	// ビュー行列の逆行列をワールド行列とする
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
/// Viewportと射影行列を設定
void GRDeviceGL::Resize(Vec2f screen){
	glViewport(0, 0, static_cast<GLsizei>(screen.x), static_cast<GLsizei>(screen.y));
	/*Affinef afProj = Affinef::ProjectionGL(Vec3f(center.x, center.y, screen.x/2.0),	// 視野角90度
											Vec2f(size.x, size.x*screen.y/screen.x),
											front, 
											back);*/

	Affinef afProj = Affinef::ProjectionGL(Vec3f(center.x, center.y, screen.x/2.0),	// 視野角90度
											Vec2f(screen.x, screen.x*screen.y/screen.x),
											front, 
											back);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(afProj);
	glMatrixMode(GL_MODELVIEW);
}
/// モデル行列をかける
void GRDeviceGL::MultModelMatrix(const Affinef& afw){	
	glMatrixMode(GL_MODELVIEW);
	modelMatrix = modelMatrix * afw;
	glLoadMatrixf(viewMatrix * modelMatrix);
	//glMultMatrixf(afw);
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
void GRDeviceGL::SetViewMatrix(const Affinef& afv){     // これでいいの？違う気がする。。
	viewMatrix  = afv;		// 視点行列の保存
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(viewMatrix * modelMatrix);
}
/// 投影行列を設定
void GRDeviceGL::SetProjectionMatrix(const Affinef& afp){    // <-----------使って確認まだ
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(afp);
	glMatrixMode(GL_MODELVIEW);
}
void GRDeviceGL::DrawDirect(TPrimitiveType ty, Vec3f* begin, Vec3f* end){}
void GRDeviceGL::DrawIndexed(TPrimitiveType ty, size_t* begin, size_t* end, Vec3f* vtx){}
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

