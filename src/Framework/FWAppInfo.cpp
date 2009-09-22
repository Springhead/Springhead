#include <Framework/FWAppInfo.h>
#include <Framework/SprFWApp.h>
#include <Springhead.h>

namespace Spr{;

enum TimerType;

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

void FWTimer::GLUTTimerFunc(int id){
	FWApp* app = FWApp::instance;
	if(!app)return;
	
	app->ReleaseAllTimer();		//マルチメディアタイマーを解放
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
	//app->GetGRAdaptee()->SetTimer(id, ms);
	glutTimerFunc(ms, GLUTTimerFunc, id);

	// 再描画要求はTimerFuncで記述するようにする？
	//app->GetGRAdaptee()->PostRedisplay();

	app->CreateAllTimer();		//マルチメディアタイマーを再構成
}

void FWTimer::MultiMediaTimerFunc(int id){
	FWApp* app = FWApp::instance;
	if(!app)return;
	
	app->TimerFunc(id);

	// 再描画要求はTimerFuncで記述するようにする？
	//app->GetGRAdaptee()->PostRedisplay();
}

FWTimer::FWTimer(int _id ,TimerType t){
	id			=	_id;
	interval	=	0;
	resolution	=	1;
	timerType	=   t;
	mtimer		=	NULL;
}
void FWTimer::SetInterval(unsigned ms){
	interval	=	ms;
	if(timerType==MMTimer && mtimer!=NULL) {
		mtimer->Interval(interval);
	}
}
void FWTimer::SetResolution(unsigned r){
	resolution	=	r;
	if(timerType==MMTimer && mtimer!=NULL) {
		mtimer->Resolution(resolution);
	}
}
void FWTimer::Create(){
	//FWGraphicsAdaptee* adaptee = (FWApp::instance)->GetGRAdaptee();
	//adaptee->SetTimer(id, interval);
	switch(timerType){
		case GLUTTimer:
			glutTimerFunc(interval, GLUTTimerFunc, id);
			break;
		case MMTimer:
			mtimer = new UTMMTimer;
			mtimer->Resolution(resolution);							// 分解能[ms]
			if(interval!=0) mtimer->Interval(interval);				// 呼びだし感覚[ms]
			mtimer->Set(MultiMediaTimerFunc, id);					// コールバックする関数
			mtimer->Create();										// コールバック開始
			break;
	}
}

void FWTimer::Recreate(){
	//GLUTTimerに関しては未実装
	if(timerType==MMTimer){
		if(mtimer!=NULL){
			mtimer->Create();
		}
	}
}

void FWTimer::Release(){
	//GLUTTimerに関しては未実装
	if(timerType==MMTimer){
		if(mtimer!=NULL){
			mtimer->Release();
		}
	}
}

}