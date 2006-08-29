#include "FWAppGL.h"
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

namespace Spr{
	IF_IMP(FWAppGL, Object);
	OBJECT_IMP(FWAppGL, Object);

	UTRef<FWAppGL> FWAppGL::instance = NULL;

	FWAppGLIf* SPR_CDECL CreateFWAppGL(){
		if (!FWAppGL::instance) {
			FWAppGL::instance = DBG_NEW FWAppGL;
		}
		return FWAppGL::instance;
	}

	void FWAppGL::GlutDisplayFunc(){
		FWAppGL::instance->Display();
	}

	void FWAppGL::GlutReshapeFunc(int w, int h){
		FWAppGL::instance->Reshape(w, h);
	}

	void FWAppGL::GlutTimerFunc(int id){
		FWAppGL::instance->Timer();
		glutTimerFunc(FWAppGL::instance->timeStep, GlutTimerFunc, 0);
	}
	void FWAppGL::GlutKeyboardFunc(unsigned char key, int x, int y){
		FWAppGL::instance->Keyboard(key, x, y);
	}

	FWAppGL::FWAppGL(const FWAppGLDesc& d/*=FWAppGLDesc()*/)
	:fwScene(NULL), cycleCount(0), isLoadComplete(false), isSimulating(true)
	{
		timeStep = 20;
		phScene = NULL;
		grScene = NULL;
		fiFileX = NULL;
		grRender = NULL;
		fiSdk = NULL;
	}

	FWAppGL::~FWAppGL(){
		if (fwScene)    { delete fwScene;    }
	}
	void FWAppGL::Load(){
		LoadFile(filename);
		CreateScene();
		CreateRender();
	}
	void FWAppGL::StartApp(std::string f, int lim/*=-1*/){
		instance = this;
		filename = f;
		cycleLimit = lim;
		Load();
		//
		glutDisplayFunc(FWAppGL::GlutDisplayFunc);
		glutReshapeFunc(FWAppGL::GlutReshapeFunc);
		glutKeyboardFunc(FWAppGL::GlutKeyboardFunc);
		glutTimerFunc(timeStep, FWAppGL::GlutTimerFunc, 0);
		glutMainLoop();
	}

	void FWAppGL::LoadFile(std::string filename){
		objs.clear();
		objs.Push(FWSdkIf::CreateSdk());
		fiSdk = FISdk::CreateSdk();
		fiFileX = fiSdk->CreateFileX();
		delete fwScene;
		fwScene = NULL;
		if(phScene) delete phScene->GetSdk();
		phScene = NULL;
		if(grScene) delete grScene->GetSdk();
		grScene = NULL;
		RegisterOldSpringheadNode(fiFileX);

		if (! fiFileX->Load(objs, filename.data()) ) {
			DSTR << "Error: Cannot open load file. " << std::endl;
			exit(EXIT_FAILURE);
		}
		DSTR << "Loaded " << (unsigned)(objs.size()) << "objects." << std::endl;
		DSTR << "LoadFile Complete." << std::endl;
		for(unsigned i=0; i<objs.size(); ++i){
			objs[i]->Print(DSTR);
		}
	}

	void FWAppGL::CreateScene(){
		// Search FWScene
		for (unsigned  i=0; i<objs.size(); i++) {
			if(!fwScene) fwScene = DCAST(FWSceneIf, objs[i]);
		}
		if (!fwScene) {
			DSTR << "FWScene not found." << std::endl;
			exit(EXIT_FAILURE);
		}

		// Get PH and GR Scene
		phScene = fwScene->GetPHScene();
		grScene = fwScene->GetGRScene();

		// Check 
		if (!phScene || !grScene) {
			if (!phScene) { DSTR << "PHScene not found." << std::endl; }
			if (!grScene) { DSTR << "GRScene not found." << std::endl; }
			exit(EXIT_FAILURE);
		}

//		fwScene->Print(DSTR);
		DSTR << "CreateScene Complete." << std::endl;	
	}

	void FWAppGL::CreateRender(){
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		glutInitWindowSize(1000, 700);
		int window = glutCreateWindow("Springhead Application");

		grRender = grScene->GetSdk()->CreateDebugRender();
		grDevice = grScene->GetSdk()->CreateDeviceGL(window);
		grDevice->Init();
		grRender->SetDevice(grDevice);

		Affinef view;
			view.Pos() = Vec3f(0.0, 3.0, 3.0);
			view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));
			view = view.inv();	
		grRender->SetViewMatrix(view);
		isLoadComplete = true;
		DSTR << "CreateRender Complete." << std::endl;	

/*
		GRLightDesc light0;
		light0.ambient			= Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
		light0.diffuse			= Vec4f(0.7f, 0.7f, 0.7f, 1.0f);
		light0.specular			= Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
		light0.position			= Vec4f(0.0f, 10.0f, 0.0f, 1.0f);
		light0.spotDirection	= Vec3f(0.0f, -1.0f, 0.0f);
		light0.spotCutoff		= 145.0f;
		light0.spotFalloff		= 30.0f;
		grRender->PushLight(light0);

		GRLightDesc light1;
		light1.diffuse			= Vec4f(0.8f, 0.8f, 0.8f, 1.0f);
		light1.specular			= Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
		light1.position			= Vec4f(0.0f, 10.0f, 10.0f, 0.0f);
		light1.spotDirection	= Vec3f(0.0f, -1.0f, 0.0f);
		grRender->PushLight(light1);
*/
	}
	void FWAppGL::Step(){
		phScene->Step();
	}

	void FWAppGL::Display(){
		if (!isLoadComplete) return;
		DCAST(FWScene, fwScene)->Sync();

		grRender->ClearBuffer();
		grRender->BeginScene();
		if (debugMode == DM_DEBUG){
			GRCameraIf* cam = NULL;
			if (grScene) cam = grScene->GetCamera();
			if (cam) cam->Render(grRender);
			
			GRLightDesc ld;
			ld.diffuse = Vec4f(1,1,1,1) * 0.8;
			ld.specular = Vec4f(1,1,1,1) * 0.8;
			ld.ambient = Vec4f(1,1,1,1) * 0.4;
			ld.position = Vec4f(1,1,1,0);
			grRender->PushLight(ld);
			grRender->DrawScene(phScene);
			grRender->PopLight();
			if (cam) cam->Rendered(grRender);
		}else{
			grScene->Render(grRender);
		}
		grRender->EndScene();	
	}

	void FWAppGL::Reshape(int w, int h){
		grRender->Reshape(Vec2f(), Vec2f(w,h));
	}

	void FWAppGL::Timer(){
		if (isSimulating){ Step(); }
		glutPostRedisplay();
	}

	void FWAppGL::Keyboard(unsigned char key, int x, int y){
		if (key == 27) {
			exit(0);
		}
		if (key == '.') {
			isSimulating = false;
			Step();
		}
		if (key == ' ') {
			isSimulating = true;
		}
	}
	FWAppGLDesc::DebugMode FWAppGL::GetDebugMode(){
		return debugMode;
	}
	void FWAppGL::SetDebugMode(FWAppGLDesc::DebugMode m){
		debugMode = m;
	}
}
