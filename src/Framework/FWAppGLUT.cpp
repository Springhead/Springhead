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
	FWAppGLUT::instance->Display();
}

void FWAppGLUT::GlutReshapeFunc(int w, int h){
	FWAppGLUT::instance->Reshape(w, h);
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
	FWAppGLUT::instance->Keyboard(key, x, y);
}
//-----------------------------------------------------------------------

void FWAppGLUT::ProcessArguments(int argc, char* argv[]){
	glutInit(&argc, argv);
	FWApp::ProcessArguments(argc, argv);
}
void FWAppGLUT::Start(){
	FWAppGL::Start();
	instance = this;
	int w = CreateWindow();
	CreateRender(w);
	glutDisplayFunc(FWAppGLUT::GlutDisplayFunc);
	glutReshapeFunc(FWAppGLUT::GlutReshapeFunc);
	glutKeyboardFunc(FWAppGLUT::GlutKeyboardFunc);
	glutTimerFunc(1, FWAppGLUT::GlutTimerFunc, 0);
	glutMainLoop();
}
void FWAppGLUT::OnTimer(){
	Step();
	glutPostRedisplay();
}

int FWAppGLUT::CreateWindow(int arg){
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1000, 700);
	int window = glutCreateWindow("Springhead Application");
	return window;
}


}
