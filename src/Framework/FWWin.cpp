/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/FWWin.h>
#include <Framework/FWScene.h>
#include <Graphics/SprGRScene.h>
#include <Graphics/SprGRRender.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

FWWin::FWWin(int wid, const FWWinDesc& desc):FWWinDesc(desc), id(wid){
	render		= 0;
	scene		= 0;
	keyMouse	= 0;
	joyStick	= 0;
	trackball	= 0;
	dragger		= 0;
	debugMode	= true;
}
	
void FWWin::Display(){
	// GRSceneにカメラフレームが無い場合はトラックボールを直接ビュー変換に反映する
	if(!scene->GetGRScene() || !scene->GetGRScene()->GetCamera() || !scene->GetGRScene()->GetCamera()->GetFrame()){
		render->SetViewMatrix(trackball->GetAffine().inv());
	}
	scene->Draw(render, debugMode);
	render->SwapBuffers();
}
void FWWin::Reshape(int w, int h){
	render->Reshape(Vec2f(), Vec2f(w,h));
}

void FWWin::OnUpdatePose(HITrackballIf*, bool dir){
	// トラックボールとGRSceneのカメラフレームの同期
	GRCameraIf* cam = GetScene()->GetGRScene()->GetCamera();
	if(!cam)
		return;
	GRFrameIf* camFrame = cam->GetFrame();
	if(!camFrame)
		return;

	if(dir){
		camFrame->SetTransform(trackball->GetAffine());
	}
	else{
		trackball->SetPosition(camFrame->GetTransform().Pos());
	}
}


}