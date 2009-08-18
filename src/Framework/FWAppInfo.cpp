#include <Framework/FWAppInfo.h>
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

GTimer::GTimer(){
	interval	=	0;
	func		=	NULL;			
	adaptee		=	NULL;
	adapteeNo	=	NULL;
}
void GTimer::Interval(unsigned i){
	interval	=	i;
}
void GTimer::Set(GTimerFunc* f){
	func = f;
}

void GTimer::Create(FWGraphicsAdaptee* a){
	adaptee=a;
	adaptee->AddTimer();
	adapteeNo=adaptee->GetTimerNo();
	adaptee->SetTimerFunc(func,adapteeNo);
	adaptee->Timer(adapteeNo);
}

void GTimer::Loop(){
	adaptee->Loop(adapteeNo,interval);
}

}