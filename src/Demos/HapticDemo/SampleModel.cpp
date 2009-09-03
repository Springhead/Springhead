#include "SampleModel.h"

CDShapeIf* CreateShapeBox(FWSdkIf* fwSdk){
	CDBoxDesc bd;
	bd.boxsize = Vec3f(2, 2, 2);
	CDShapeIf* shapeBox = fwSdk->GetPHSdk()->CreateShape(bd);
	return shapeBox;
}

PHSolidIf* CreateBox(FWSdkIf* fwSdk){
	PHSolidDesc desc;
	desc.mass = 0.05;
	desc.inertia = 0.0333 * Matrix3d::Unit();
	PHSolidIf* soBox = fwSdk->GetScene()->GetPHScene()->CreateSolid(desc);
	CDBoxDesc bd;
	bd.boxsize = Vec3f(2, 2, 2);
	CDShapeIf* shapeBox = fwSdk->GetPHSdk()->CreateShape(bd);
	soBox->AddShape(shapeBox);
	soBox->SetFramePosition(Vec3d(-1, 5, 4));
	//soBox.back()->SetOrientation(
	//	Quaternionf::Rot(Rad(30), 'y') * 
	//	Quaternionf::Rot(Rad(10), 'x'));  
	soBox->GetShape(0)->SetVibration(-200,120,300);
	soBox->GetShape(0)->SetStaticFriction(1);//0.8);
	soBox->GetShape(0)->SetDynamicFriction(1);//0.6);
	return soBox;
}

PHSolidIf* CreateSphere(FWSdkIf* fwSdk){
	PHSolidDesc desc;
	desc.mass = 0.05;
	desc.inertia = 0.0288* Matrix3d::Unit();
	PHSolidIf* phSolid = fwSdk->GetScene()->GetPHScene()->CreateSolid(desc);
	CDSphereDesc sd;
	sd.radius = 1.2;
	CDShapeIf* shape = fwSdk->GetPHSdk()->CreateShape(sd);
	shape->SetElasticity(0.4);
	phSolid->AddShape(shape);
	phSolid->SetFramePosition(Vec3f(0.5, 20,0));
	phSolid->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));  
	return phSolid;
}

PHSolidIf* CreateCapsule(FWSdkIf* fwSdk){
	PHSolidDesc desc;
	desc.mass = 0.05;
	desc.inertia[0][0] = 0.0325;
	desc.inertia[0][1] = 0.0;
	desc.inertia[0][2] = 0.0;
	desc.inertia[1][0] = 0.0;
	desc.inertia[1][1] = 0.02;
	desc.inertia[1][2] = 0.0;
	desc.inertia[2][0] = 0.0;
	desc.inertia[2][1] = 0.0;
	desc.inertia[2][2] = 0.0325;
	PHSolidIf* phSolid = fwSdk->GetScene()->GetPHScene()->CreateSolid(desc);
	CDCapsuleDesc cd;
	cd.radius = 1;
	cd.length = 1;
	CDShapeIf* shape = fwSdk->GetPHSdk()->CreateShape(cd);
	shape->SetVibration(-80,200,150);
	shape->SetElasticity(0.1);
	phSolid->AddShape(shape);
	phSolid->SetAngularVelocity(Vec3f(0,0,0.2));
	phSolid->SetFramePosition(Vec3f(0.5, 20,0));
	phSolid->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));  
	return phSolid;
}

PHSolidIf* CreateRoundCone(FWSdkIf* fwSdk){
	// „‘Ì‚Ìì¬
	PHSolidDesc desc;
	desc.mass = 0.05;
	desc.inertia *= 0.05;
	PHSolidIf*  soRCone = fwSdk->GetScene()->GetPHScene()->CreateSolid(desc);
	// Œ`ó‚Ìì¬
	CDRoundConeDesc rd;
	rd.length = 3.0;
	rd.radius[0] = 1.5;
	rd.radius[1] = 0.5;
	CDShapeIf* shapeRCone = fwSdk->GetPHSdk()->CreateShape(rd);
	// „‘Ì‚ÉŒ`ó‚ð•t‰Á‚·‚é
	soRCone->AddShape(shapeRCone);
	soRCone->SetFramePosition(Vec3d(0, 15, 0));
	return soRCone;
}

PHSolidIf* CreatePolyhedron(FWSdkIf* fwSdk){
	PHSolidDesc desc;
	desc.mass = 0.1;
	desc.inertia = 0.36 * Matrix3d::Unit();
	PHSolidIf* phSolid = fwSdk->GetScene()->GetPHScene()->CreateSolid(desc);
	CDConvexMeshDesc md;
	int nv = rand() % 100 + 50;
	for(int i=0; i < nv; ++i){
		Vec3d v;
		for(int c=0; c<3; ++c){
			v[c] = (rand() % 100 / 100.0 - 0.5) * 4;
//			v[c] = (rand() % 100 / 100.0 - 0.5) * 5 * 1.3;
		}
		md.vertices.push_back(v);
	}
	CDShapeIf* shape = fwSdk->GetPHSdk()->CreateShape(md);
	shape->SetVibration(-100,60,100);
	shape->SetStaticFriction(0.8);
	shape->SetDynamicFriction(0.6);
	phSolid->AddShape(shape);
	phSolid->SetFramePosition(Vec3f(0.5, 20,0));
	phSolid->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));
	return phSolid;
}

PHSolidIf* CreateLumpBox(FWSdkIf* fwSdk){
	PHSolidDesc desc;
	PHSolidIf* phSolid = fwSdk->GetScene()->GetPHScene()->CreateSolid(desc);
	phSolid->AddShape(CreateShapeBox(fwSdk));
	phSolid->AddShape(CreateShapeBox(fwSdk));
	phSolid->AddShape(CreateShapeBox(fwSdk));
	phSolid->AddShape(CreateShapeBox(fwSdk));
	phSolid->AddShape(CreateShapeBox(fwSdk));
	phSolid->AddShape(CreateShapeBox(fwSdk));
	phSolid->AddShape(CreateShapeBox(fwSdk));
	Posed pose;
	pose.Pos() = Vec3d(3, 0, 0);
	phSolid->SetShapePose(1, pose);
	pose.Pos() = Vec3d(-3, 0, 0);
	phSolid->SetShapePose(2, pose);
	pose.Pos() = Vec3d(0, 3, 0);
	phSolid->SetShapePose(3, pose);
	pose.Pos() = Vec3d(0, -3, 0);
	phSolid->SetShapePose(4, pose);
	pose.Pos() = Vec3d(0, 0, 3);
	phSolid->SetShapePose(5, pose);
	pose.Pos() = Vec3d(0, 0, -3);
	phSolid->SetShapePose(6, pose);
	phSolid->SetFramePosition(Vec3f(0.5, 20,0));
	phSolid->SetOrientation(Quaternionf::Rot(Rad(30), 'y'));  
	return phSolid;
}

void CreateWall(FWSdkIf* fwSdk){
	PHSolidDesc desc;
	desc.mass = 0.05;
	desc.inertia = 0.0333 * Matrix3d::Unit();
	int wall_height = 4;
	int numbox = 5;
	for(int i = 0; i < wall_height; i++){
		for(int j = 0; j < numbox; j++){
			PHSolidIf* phSolid = fwSdk->GetScene()->GetPHScene()->CreateSolid(desc);
			CDShapeIf* shape = CreateShapeBox(fwSdk);
			phSolid->AddShape(shape);
			shape->SetVibration(-200,120,300);
			shape->SetStaticFriction(0.8);
			shape->SetDynamicFriction(0.6);
			phSolid->SetFramePosition(Vec3d(-4.0 + (2.0 + 0.1) * j , (2.0 + 0.1) * (double)i, -2.0));  
		}
	}
}

void CreateTower(FWSdkIf* fwSdk){
	PHSolidDesc desc;
	desc.mass = 0.05;
	desc.inertia = 0.0333 * Matrix3d::Unit();
	double tower_radius = 5;
	int tower_height = 5;
	int numbox = 5;
	double theta;
	for(int i = 0; i < tower_height; i++){
		for(int j = 0; j < numbox; j++){
			PHSolidIf* phSolid = fwSdk->GetScene()->GetPHScene()->CreateSolid(desc);
			CDShapeIf* shape = CreateShapeBox(fwSdk);
			phSolid->AddShape(shape);
			shape->SetVibration(-200,120,300);
			shape->SetStaticFriction(0.4);
			shape->SetDynamicFriction(0.3);
			theta = ((double)j + (i % 2 ? 0.0 : 0.5)) * Rad(360) / (double)numbox;
			phSolid->SetFramePosition(Vec3d(tower_radius * cos(theta), 2.0 * ((double)i), tower_radius * sin(theta)));
			phSolid->SetOrientation(Quaterniond::Rot(-theta, 'y'));  
		}
	}
}

void CreateJointBox(FWSdkIf* fwSdk){
	PHBallJointDesc desc;
	{
		desc.poseSocket.Pos()	= Vec3f(0.0f, -1.0f, 0.0f);
		desc.posePlug.Pos()	= Vec3f(0.0f, 1.0f, 0.0f);
		desc.spring			= 3;
		desc.damper		= 2;
	}
	PHSolidIf* rootSolid = CreateBox(fwSdk);
	rootSolid->SetDynamical(false);
	float posy = 15;
	rootSolid->SetFramePosition(Vec3d(-5, posy, 0));
	for(int i = 1; i < 6; i++){
		PHSolidIf* nodeSolid = CreateBox(fwSdk);
		fwSdk->GetScene()->GetPHScene()->CreateJoint(rootSolid, nodeSolid, desc);
		nodeSolid->SetFramePosition(Vec3d(0, posy - 2 * i, 0));
		fwSdk->GetScene()->GetPHScene()->SetContactMode(rootSolid, nodeSolid, PHSceneDesc::MODE_NONE);
		rootSolid = nodeSolid;
	}
}

void Create3ElementJointBox(FWSdkIf* fwSdk){
	PH3ElementBallJointDesc desc;
	{
		desc.poseSocket.Pos()	= Vec3f(0.0f, -1.0f, 0.0f);
		desc.posePlug.Pos()	= Vec3f(0.0f, 1.0f, 0.0f);
		desc.spring			= 1.0;
		desc.damper		= 100;
		desc.secondDamper = 10.0;
		desc.hardnessRate = 5;
		desc.yieldStress =0.2;

	}
	PHSolidIf* rootSolid = CreateBox(fwSdk);
	rootSolid->SetMass(0.1);
	rootSolid->SetDynamical(false);
	double posy = 15;
	Vec3d pos = Vec3d(0, posy, 0);
	rootSolid->SetFramePosition(pos);
	for(int i = 1; i < 8; i++){
		PHSolidIf* nodeSolid = CreateBox(fwSdk);
		nodeSolid->SetMass(0.1);
		fwSdk->GetScene()->GetPHScene()->CreateJoint(rootSolid, nodeSolid, desc);
		nodeSolid->SetFramePosition(Vec3d(0, posy - 2 * i, 0));
		fwSdk->GetScene()->GetPHScene()->SetContactMode(rootSolid, nodeSolid, PHSceneDesc::MODE_NONE);
		rootSolid = nodeSolid;
	}
}