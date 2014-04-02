#include "Handler.h"

class GearHandler : public Handler{
public:
	enum{
		ID_VEL_PLUS,
		ID_VEL_ZERO,
		ID_VEL_MINUS,
	};
	struct Gear{
		PHSolidIf*		solid;
		PH1DJointIf*	joint;
		PHGearIf*		gear;
		double			radius;
	};

	vector<Gear>	gears;
	double			damper;

public:
	void AddGear(const Posed& pose, double r, double l, Gear* par = 0){
		PHSdkIf*   phSdk   = GetPHSdk();
		PHSceneIf* phScene = GetPHScene();

		Gear g;
		g.radius = r;

		g.solid = phScene->CreateSolid();
		g.solid->SetPose(pose);
		
		CDConvexMeshDesc md;
		int div = 30;
		double dtheta = 2.0 * M_PI / (double)div;
		double theta = 0.0;
		for(int i = 0; i < div; i++){
			md.vertices.push_back(Vec3d(r * cos(theta), r * sin(theta), -l/2.0));
			md.vertices.push_back(Vec3d(r * cos(theta), r * sin(theta),  l/2.0));
			theta += dtheta;
		}
		g.solid->AddShape(phSdk->CreateShape(md));
		
		CDCapsuleDesc cd;
		cd.radius = 0.1;
		cd.length = l;
		g.solid->AddShape(phSdk->CreateShape(cd));
		g.solid->SetShapePose(1, Posed(Vec3d(r-0.1, 0.0, 0.0), Quaterniond()));

		PHHingeJointDesc hd;
		hd.poseSocket = pose;
		g.joint = phScene->CreateJoint(soFloor, g.solid, hd)->Cast();

		if(par){
			PHGearDesc gd;
			gd.ratio = -par->radius / g.radius;
			g.gear = phScene->CreateGear(par->joint, g.joint, gd);
		}

		gears.push_back(g);
	}

	GearHandler(SampleApp* a):Handler(a){
		app->AddAction(MENU_GEAR, ID_VEL_PLUS, "positive velocity");
		app->AddHotKey(MENU_GEAR, ID_VEL_PLUS, 'f');
		app->AddAction(MENU_GEAR, ID_VEL_ZERO, "zero velocity");
		app->AddHotKey(MENU_GEAR, ID_VEL_ZERO, 'g');
		app->AddAction(MENU_GEAR, ID_VEL_MINUS, "negative velocity");
		app->AddHotKey(MENU_GEAR, ID_VEL_MINUS, 'y');
	}
	~GearHandler(){}

	virtual void BuildScene(){
		soFloor = app->CreateFloor();

		AddGear(Posed(Vec3d( 0.0, 10.0, 0.0), Quaterniond()), 1.0, 0.5);
		AddGear(Posed(Vec3d( 2.0, 10.0, 0.0), Quaterniond()), 1.0, 0.5, &gears[0]);
		AddGear(Posed(Vec3d( 6.0, 10.0, 0.0), Quaterniond()), 3.0, 0.5, &gears[1]);
		AddGear(Posed(Vec3d(10.0, 10.0, 0.0), Quaterniond()), 1.0, 0.5, &gears[2]);
		AddGear(Posed(Vec3d(12.0, 10.0, 0.0), Quaterniond()), 1.0, 0.5, &gears[3]);
		AddGear(Posed(Vec3d( 6.0,  6.0, 0.0), Quaterniond::Rot(Rad(90.0), 'y')), 1.0, 0.5, &gears[2]);
		AddGear(Posed(Vec3d( 6.0,  4.0, 0.0), Quaterniond::Rot(Rad(90.0), 'y')), 1.0, 0.5, &gears[5]);
		
		GetPHScene()->CreateTreeNodes(soFloor);
		GetPHScene()->SetContactMode(PHSceneDesc::MODE_NONE);

		damper = 100.0;
	}
	virtual void OnAction(int id){
		if(id == ID_VEL_PLUS){
			gears[0].joint->SetDamper(damper);
			gears[0].joint->SetTargetVelocity(Rad(180));
		}
		if(id == ID_VEL_ZERO){
			gears[0].joint->SetDamper(0.0);
			gears[0].joint->SetTargetVelocity(Rad(0.0));
		}
		if(id == ID_VEL_MINUS){
			gears[0].joint->SetDamper(damper);
			gears[0].joint->SetTargetVelocity(Rad(-180.0));
		}
	}
};

