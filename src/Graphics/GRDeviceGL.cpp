#include "Graphics.h"
#pragma hdrstop
#include <gl/glut.h>

namespace Spr {;
//----------------------------------------------------------------------------
//	GRDeviceGL
OBJECTIMP(GRDeviceGL, GRDevice);
IF_IMP(GRDeviceGL, GRDevice);

void GRDeviceGL::Init(){
	glDrawBuffer(GL_BACK);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 3.0, 9.0, 
		      0.0, 0.0, 0.0,
		 	  0.0, 1.0, 0.0);

}

void GRDeviceGL::ClearBuffer(){}
void GRDeviceGL::BeginScene(){}
void GRDeviceGL::EndScene(){}
void GRDeviceGL::Resize(Vec2f screen){}
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

