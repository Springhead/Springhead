#include <Framework/SprFWAppGLUT.h>
#include "FWScene.h"
#include "FWSdk.h"
#include "FWOldSpringheadNode.h"
#include <FileIO/FIFileX.h>
#include <FileIO/FISdk.h>
#include <Physics/PHSdk.h>
#include <Graphics/GRSdk.h>

#include <GL/glut.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

FWAppGLUT* FWAppGLUT::instance;
	
void FWAppGLUT::GlutDisplayFunc(){
	FWAppGLUT::instance->CallDisplay();
}

void FWAppGLUT::GlutReshapeFunc(int w, int h){
	FWAppGLUT::instance->CallReshape(w, h);
}

void FWAppGLUT::GlutTimerFunc(int id){
	FWAppGLUT::instance->OnTimer();
	FWSceneIf* fs = FWAppGLUT::instance->GetFWScene();
	PHSceneIf* ps = NULL;
	if (fs) ps = fs->GetPHScene();
	int timeStep = 1000;
	if (ps) timeStep = ps->GetTimeStep() * 1000.0;
	if (timeStep<1) timeStep = 1;
	glutTimerFunc(timeStep, GlutTimerFunc, 0);
}
void FWAppGLUT::GlutKeyboardFunc(unsigned char key, int x, int y){
	FWAppGLUT::instance->CallKeyboard(key, x, y);
}

void FWAppGLUT::AtExit(){
	if(FWAppGLUT::instance->vfBridge)
		FWAppGLUT::instance->vfBridge->AtExit();
}

//-----------------------------------------------------------------------

void FWAppGLUT::Init(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	FWAppGL::Init(argc, argv);
	atexit(FWAppGLUT::AtExit);
}

void FWAppGLUT::Start(){
	instance = this;
	windowID = CreateWindow();
	CreateRender();
	glutDisplayFunc(FWAppGLUT::GlutDisplayFunc);
	glutReshapeFunc(FWAppGLUT::GlutReshapeFunc);
	glutKeyboardFunc(FWAppGLUT::GlutKeyboardFunc);
	glutTimerFunc(1, FWAppGLUT::GlutTimerFunc, 0);
	glutMainLoop();
}

void FWAppGLUT::Display(){
	FWAppGL::Display();
	/// ダブルバッファモード時、カレントウィンドウのバッファ交換を行う
	glutSwapBuffers();
}

void FWAppGLUT::OnTimer(){
	CallStep();
	glutPostRedisplay();
}

int FWAppGLUT::CreateWindow(const FWWindowDesc d){
	glutInitWindowSize(d.width, d.height);
	glutInitWindowPosition(d.top, d.left);
	int window = glutCreateWindow("Springhead Application");
	return window;
}


}
