#include "DemoModel.h"

CDShapeIf* CreateShapeBox(PHSceneIf* phscene){
	CDBoxDesc cd;
	cd.boxsize = Vec3f(1.0, 1.0, 1.0) * modelScale;
	cd.material.mu0 = 0.5;
	cd.material.mu = 0.3;
	CDShapeIf* shape = phscene->GetSdk()->CreateShape(cd);
	shape->SetVibration(-20, 120, 300);
	return shape;
}

CDShapeIf* CreateShapeSphere(PHSceneIf* phscene){
	CDSphereDesc cd;
	cd.radius = 0.5 * modelScale;
	cd.material.mu0 = 0.5;
	cd.material.mu = 0.3;
	CDShapeIf* shape = phscene->GetSdk()->CreateShape(cd);
	shape->SetVibration(-20, 120, 300);
	return shape;
}
CDShapeIf* CreateShapeCapsule(PHSceneIf* phscene){
	CDCapsuleDesc cd;
	cd.radius = 0.5 * modelScale;
	cd.length = 1.0;
	cd.material.mu0 = 0.5;
	cd.material.mu = 0.3;
	CDShapeIf* shape = phscene->GetSdk()->CreateShape(cd);
	shape->SetVibration(-30,200,150);
	return shape;
}
CDShapeIf* CreateShapeCone(PHSceneIf* phscene){
	CDRoundConeDesc cd;
	cd.radius = Vec2f(0.8, 0.3) * modelScale;
	cd.length = 2.0;
	cd.material.mu0 = 0.8;
	cd.material.mu = 0.6;
	CDShapeIf* shape = phscene->GetSdk()->CreateShape(cd);
	shape->SetVibration(-30,200,150);
	return shape;
}

PHSolidIf* CreateBox(PHSceneIf* phscene){
	PHSolidDesc desc;
	desc.mass = 0.05;
	PHSolidIf* solid = phscene->CreateSolid(desc);
	solid->AddShape(CreateShapeBox(phscene));
	solid->SetInertia(solid->GetMass() * solid->GetShape(0)->CalcMomentOfInertia() * (1/solid->GetShape(0)->CalcMomentOfInertia()));
	solid->SetFramePosition(Vec3d(-1, 5, 4));
	return solid;
}

PHSolidIf* CreateSphere(PHSceneIf* phscene){
	PHSolidDesc desc;
	desc.mass = 0.05;
	PHSolidIf* solid = phscene->CreateSolid(desc);
	solid->AddShape(CreateShapeSphere(phscene));
	solid->SetInertia(solid->GetMass() * solid->GetShape(0)->CalcMomentOfInertia() * (1/solid->GetShape(0)->CalcMomentOfInertia()));
	solid->SetFramePosition(Vec3f(0.5, 1,0));
//	solid->SetDynamical(false);
	return solid;
}

PHSolidIf* CreateCapsule(PHSceneIf* phscene){
	PHSolidDesc desc;
	desc.mass = 0.05;
	PHSolidIf* solid = phscene->CreateSolid(desc);
	solid->AddShape(CreateShapeCapsule(phscene));
	solid->SetInertia(solid->GetMass() * solid->GetShape(0)->CalcMomentOfInertia() * (1/solid->GetShape(0)->CalcMomentOfInertia()));
	solid->SetFramePosition(Vec3f(0.5, 20,0));
	return solid;
}

PHSolidIf* CreateRoundCone(PHSceneIf* phscene){
	PHSolidDesc desc;
	desc.mass = 0.05;
	PHSolidIf*  solid = phscene->CreateSolid(desc);
	solid->AddShape(CreateShapeCone(phscene));
	solid->SetInertia(solid->GetMass() * solid->GetShape(0)->CalcMomentOfInertia() * (1/solid->GetShape(0)->CalcMomentOfInertia()));
	solid->SetFramePosition(Vec3d(-0, 0, 0));
	solid->SetOrientation(Quaterniond::Rot(Rad(90), 'x'));
	solid->SetShapePose(0,Posed(1,0,0,0,0,0,0));
	return solid;
}

PHSolidIf* CreatePolyhedron(PHSceneIf* phscene){
	PHSolidDesc desc;
	desc.mass = 0.1;
	PHSolidIf* solid = phscene->CreateSolid(desc);
	CDConvexMeshDesc cd;
	int nv = rand() % 100 + 50;
	for(int i=0; i < nv; ++i){
		Vec3d v;
		for(int c=0; c<3; ++c){
			v[c] = (rand() % 100 / 100.0 - 0.5) * 4;
//			v[c] = (rand() % 100 / 100.0 - 0.5) * 5 * 1.3;
		}
		cd.vertices.push_back(v);
	}
	CDShapeIf* shape = phscene->GetSdk()->CreateShape(cd);
	shape->SetVibration(-10,60,100);
	shape->SetStaticFriction(0.5);
	shape->SetDynamicFriction(0.3);
	solid->AddShape(shape);
	solid->SetInertia(solid->GetMass() * solid->GetShape(0)->CalcMomentOfInertia() * (1/solid->GetShape(0)->CalcMomentOfInertia()));
	solid->SetFramePosition(Vec3f(0.5, 20,0));
	solid->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));
	return solid;
}

PHSolidIf* CreateLumpBox(PHSceneIf* phscene){
	PHSolidDesc desc;
	desc.mass = 0.4;
	PHSolidIf* solid = phscene->CreateSolid(desc);
	for(int i = 0; i < 7; i++){
		solid->AddShape(CreateShapeBox(phscene));
	}
	Posed pose;
	pose.Pos() = Vec3d(3, 0, 0);
	solid->SetShapePose(1, pose);
	pose.Pos() = Vec3d(-3, 0, 0);
	solid->SetShapePose(2, pose);
	pose.Pos() = Vec3d(0, 3, 0);
	solid->SetShapePose(3, pose);
	pose.Pos() = Vec3d(0, -3, 0);
	solid->SetShapePose(4, pose);
	pose.Pos() = Vec3d(0, 0, 3);
	solid->SetShapePose(5, pose);
	pose.Pos() = Vec3d(0, 0, -3);
	solid->SetShapePose(6, pose);
	Matrix3f inertia;
	for(int i = 0; i < 7; i++){
		inertia += solid->GetShape(i)->CalcMomentOfInertia();
	}
	solid->SetInertia(solid->GetMass() * inertia);
	solid->SetFramePosition(Vec3f(0.5, 20,0));
	solid->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));  
	return solid;
}

void CreateWall(PHSceneIf* phscene){
	int wall_height = 4;
	int numbox = 5;
	for(int i = 0; i < wall_height; i++){
		for(int j = 0; j < numbox; j++){
			PHSolidIf* solid = CreateBox(phscene);
			solid->SetFramePosition(Vec3d(-4.0 + (2.0 + 0.1) * j , (2.0 + 0.1) * (double)i, -2.0));  
		}
	}
}

void CreateTower(PHSceneIf* phscene){
	double tower_radius = 5;
	int tower_height = 5;
	int numbox = 5;
	double theta;
	for(int i = 0; i < tower_height; i++){
		for(int j = 0; j < numbox; j++){
			PHSolidIf* solid = CreateBox(phscene);
			theta = ((double)j + (i % 2 ? 0.0 : 0.5)) * Rad(360) / (double)numbox;
			solid->SetFramePosition(Vec3d(tower_radius * cos(theta), 2.0 * ((double)i), tower_radius * sin(theta)));
			solid->SetOrientation(Quaterniond::Rot(-theta, 'y'));  
		}
	}
}

void CreateJointBox(PHSceneIf* phscene){
	PHBallJointDesc desc;
	{
		desc.poseSocket.Pos()	= Vec3f(0.0f, -1.0f, 0.0f);
		desc.posePlug.Pos()	= Vec3f(0.0f, 1.0f, 0.0f);
		desc.spring			= 3;
		desc.damper		= 2;
	}
	PHSolidIf* rootSolid = CreateBox(phscene);
	rootSolid->SetDynamical(false);
	float posy = 15;
	rootSolid->SetFramePosition(Vec3d(-5, posy, 0));
	for(int i = 1; i < 6; i++){
		PHSolidIf* nodeSolid = CreateBox(phscene);
		phscene->CreateJoint(rootSolid, nodeSolid, desc);
		nodeSolid->SetFramePosition(Vec3d(0, posy - 2 * i, 0));
		phscene->SetContactMode(rootSolid, nodeSolid, PHSceneDesc::MODE_NONE);
		rootSolid = nodeSolid;
	}
}
