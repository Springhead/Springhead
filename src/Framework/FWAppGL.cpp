#include <Framework/SprFWAppGL.h>

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

void FWAppGL::CreateRender(){
	grRender = fwSdk->GetGRSdk()->CreateDebugRender();
	grDevice = fwSdk->GetGRSdk()->CreateDeviceGL();
	grDevice->Init();
	grRender->SetDevice(grDevice);

	Affinef view;
		view.Pos() = Vec3f(0.0, 3.0, 3.0);
		view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));
		view = view.inv();	
	grRender->SetViewMatrix(view);

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
}
