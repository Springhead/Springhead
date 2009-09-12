#include <Framework/FWAppInfo.h>
#include <Framework/SprFWApp.h>
#include <Springhead.h>

namespace Spr{;

CameraInfo::CameraInfo():
	rot(Rad(0.0), Rad(80.0)), zoom(1.0f),
	rotRangeY(Rad(-180.0), Rad(180.0)), rotRangeX(Rad(-80.0), Rad(80.0)), zoomRange(0.01f, 100.0f){
	UpdateView();
}

void CameraInfo::UpdateView(){
	view  = Affinef();
	view.Pos() = target + zoom * Vec3f(
		cos(rot.x) * cos(rot.y),
		sin(rot.x),
		cos(rot.x) * sin(rot.y));
	view.LookAtGL(target);
}

void GTimer::GTimerFunc(int id){
	FWApp* app = FWApp::instance;
	if(!app)return;
	
	app->TimerFunc(id);
	
	// タイマーの再始動
	// 周期が指定されていればその値を採用
	int ms = app->GetTimer(id)->GetInterval();
	// それ以外はアクティブシーンのtime stepから決定
	if(!ms){
		FWSceneIf* scene = app->GetSdk()->GetScene();
		if(scene && scene->GetPHScene()){
			ms = (int)(scene->GetPHScene()->GetTimeStep() * 1000.0);
			if(ms < 1) ms = 1;
		}
	}
	app->GetGRAdaptee()->SetTimer(id, ms);

	// 再描画要求
	app->GetGRAdaptee()->PostRedisplay();
}

GTimer::GTimer(int _id){
	id			=	_id;
	interval	=	0;
}
void GTimer::SetInterval(unsigned ms){
	interval	=	ms;
}
void GTimer::Create(){
	FWGraphicsAdaptee* adaptee = (FWApp::instance)->GetGRAdaptee();
	adaptee->SetTimer(id, interval);
}

}