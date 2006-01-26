#include "Graphics.h"
#pragma hdrstop
#include <gl/glut.h>

namespace Spr {;
//----------------------------------------------------------------------------
//	GRDeviceGL
OBJECTIMP(GRDeviceGL, GRDevice);
IF_IMP(GRDeviceGL, GRDevice);
// 初期設定
void GRDeviceGL::Init(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0); 
	glDrawBuffer(GL_BACK);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 5.0, 10.0, 
		      0.0, 0.0, 0.0,
		 	  0.0, 1.0, 0.0);
}
// 画面をクリアする
void GRDeviceGL::ClearBuffer(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void GRDeviceGL::BeginScene(){




}
void GRDeviceGL::EndScene(){




}
// glutReshapeFunc()で指定したコールバック関数
// ウィンドウ生成時とリサイズ時に実行する関数
void GRDeviceGL::Resize(Vec2f screen){
	int w = static_cast<int>(screen.data[0]);
	int h = static_cast<int>(screen.data[1]);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}
void GRDeviceGL::MultModelMatrix(const Affinef& afw){}
void GRDeviceGL::PushModelMatrix(){}
void GRDeviceGL::PopModelMatrix(){}
void GRDeviceGL::SetModelMatrix(const Affinef& afw){}
void GRDeviceGL::SetViewMatrix(const Affinef& afv){}
void GRDeviceGL::SetProjectionMatrix(const Affinef& afp){}
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

