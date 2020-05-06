#include "BoxStack.h"
//#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <ctime>
#include <string>
#include <GL/glut.h>

#define ESC 27

BoxStack bstack;

BoxStack::BoxStack(){
	dt = 0.02;//0.05;
	gravity =  Vec3d(0, -9.8f , 0);
	nIter = 5;
	bGravity = true;
	bDebug = false;
	bStep = true;
}

void BoxStack::Init(int argc, char* argv[]){
	FWApp::Init(argc, argv);
	Vec3d pos = Vec3d(0, 5, 15.0);						// カメラ初期位置
	GetCurrentWin()->GetTrackball()->SetPosition(pos);	// カメラ初期位置の設定

	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();

	BuildScene();		// 剛体を作成
	phscene->SetGravity(gravity);				
	phscene->SetTimeStep(dt);
	phscene->SetNumIteration((int)nIter);
}

void BoxStack::Reset(){
	GetSdk()->Clear();															// SDKの作成
	GetSdk()->CreateScene(PHSceneDesc(), GRSceneDesc());		// Sceneの作成
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();

	BuildScene();																// 剛体を作成

	phscene->SetGravity(gravity);				
	phscene->SetTimeStep(dt);
	phscene->SetNumIteration((int)nIter);

	GetCurrentWin()->SetScene(GetSdk()->GetScene());
}

void BoxStack::BuildScene(){
	// soFloor用のdesc
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	soFloor = phscene->CreateSolid(desc);		// 剛体をdescに基づいて作成
	soFloor->SetDynamical(false);
	soFloor->SetGravity(false);
	
	// soBox用のdesc
	{
		CDBoxDesc bd;
		bd.boxsize = Vec3f(2,2,2);
		bd.material.density = 2.0f;
		meshBox = XCAST(GetSdk()->GetPHSdk()->CreateShape(bd));
		meshBox->SetName("meshBox");
		CDSphereDesc sd;
		sd.radius = 1.2f;
		meshSphere = XCAST(GetSdk()->GetPHSdk()->CreateShape(sd));
		meshSphere->SetName("meshSphere");
		CDCapsuleDesc cd;
		cd.radius = 1;
		cd.length = 1;
		meshCapsule = XCAST(GetSdk()->GetPHSdk()->CreateShape(cd));
		meshCapsule->SetName("meshCapsule");
	}

	{
		// meshConvex(soBox)のメッシュ形状
		CDConvexMeshInterpolateDesc md;
		md.vertices.push_back(Vec3d(-1,-1,-1));
		md.vertices.push_back(Vec3d(-1,-1, 1));	
		md.vertices.push_back(Vec3d(-1, 1,-1));	
		md.vertices.push_back(Vec3d(-1, 1, 1));
		md.vertices.push_back(Vec3d( 1,-1,-1));	
		md.vertices.push_back(Vec3d( 1,-1, 1));
		md.vertices.push_back(Vec3d( 1, 1,-1));
		md.vertices.push_back(Vec3d( 1, 1, 1));
		meshConvex = DCAST(CDConvexMeshIf, GetSdk()->GetPHSdk()->CreateShape(md));
		meshConvex->SetName("meshConvex");

		// meshFloor(soFloor)のメッシュ形状
		for(unsigned i=0; i<md.vertices.size(); ++i){
			md.vertices[i].x *= 30;
			md.vertices[i].z *= 20;
			md.vertices[i].y *= 20;
		}
		meshFloor = DCAST(CDConvexMeshIf, GetSdk()->GetPHSdk()->CreateShape(md));
		meshFloor->SetName("meshFloor");
	}
	soFloor->AddShape(meshFloor);
	soFloor->SetFramePosition(Vec3d(0, -20 - 0.7, 0));
	soFloor->GetShape(0)->SetVibration(-100, 150, 150);
	soFloor->SetName("solidFloor");
}

int exitCount = 0;
void BoxStack::UserFunc() {
	exitCount++;
	if (exitCount > 60.0 / dt) exit(0);
}
void BoxStack::Keyboard(int key, int x, int y){
	exitCount = 0;
	PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
	switch (key) {
		case ESC:
		case  'q':
			exit(0);
			break;
		case 'r':
			Reset();
			break;
		case 'd':
			if(bDebug){
				bDebug = false;
				DSTR << "Debug Mode OFF" << endl;
			}else{
				bDebug = true;
				DSTR << "Debug Mode ON" << endl;
			}
			break;
		case 'g':
			if(bGravity){
				bGravity = false;
				Vec3d zeroG = Vec3d(0.0, 0.0, 0.0);
				phscene->SetGravity(zeroG);
				DSTR << "Gravity OFF" << endl;
			}else{
				bGravity = true;
				phscene->SetGravity(gravity);
				DSTR << "Gravity ON" << endl;
			}
			break;
		case 's':
			bStep = false;
			bOneStep = true;
			break;
		case 'a':
			bStep = true;
			break;
		case ' ':
			{
				// ConvexBox
				desc.mass = 0.05;
				desc.inertia = 0.0333 * Matrix3d::Unit();
				//desc.dynamical = false;
				soBox.push_back(phscene->CreateSolid(desc));
				soBox.back()->AddShape(meshBox);
				soBox.back()->SetFramePosition(Vec3d(-1, 5, 4));
				soBox.back()->GetShape(0)->SetVibration(-200,120,300);
				soBox.back()->GetShape(0)->SetStaticFriction(0.8f);
				soBox.back()->GetShape(0)->SetDynamicFriction(0.6f);
				//soBox.back()->SetOrientation(
				//	Quaternionf::Rot(Rad(30), 'y') * 
				//	Quaternionf::Rot(Rad(10), 'x'));  
				ostringstream os;
				os << "box" << (unsigned int)soBox.size();
				soBox.back()->SetName(os.str().c_str());
			}
			DSTR << "Create Box" << endl;
			DSTR << "NSolids		" <<  phscene->NSolids() << endl;
			break;
		case 'v':
			{
				// MeshCapsule
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

				soBox.push_back(phscene->CreateSolid(desc));
				soBox.back()->SetAngularVelocity(Vec3f(0, 0, 0.2f));
				soBox.back()->AddShape(meshCapsule);
				soBox.back()->SetFramePosition(Vec3f(0.5f, 20, 0));
				soBox.back()->GetShape(0)->SetVibration(-80,200,150);
				soBox.back()->GetShape(0)->SetElasticity(0.1f);
//				soBox.back()->SetFramePosition(Vec3f(0.5, 10+3*soBox.size(),0));
				soBox.back()->SetOrientation(Quaternionf::Rot((float)Rad(30), 'y'));  
				ostringstream os;
				os << "capsule" << (unsigned int)soBox.size();
				soBox.back()->SetName(os.str().c_str());
			}
			break;
		case 'b':
			{
				// MeshSphere
				desc.mass = 0.05;
				desc.inertia = 0.0288* Matrix3d::Unit();
				soBox.push_back(phscene->CreateSolid(desc));
				soBox.back()->AddShape(meshSphere);
//				soBox.back()->SetFramePosition(Vec3f(0.5, 10+3*soBox.size(),0));
				soBox.back()->GetShape(0)->SetElasticity(0.4f);
				soBox.back()->SetFramePosition(Vec3f(0.5f, 20, 0));
				soBox.back()->SetOrientation(Quaternionf::Rot((float)Rad(30), 'y'));  
				ostringstream os;
				os << "sphere" << (unsigned int)soBox.size();
				soBox.back()->SetName(os.str().c_str());
			}
			break;
		case 'n':
			{
				// ConvexMesh
				desc.mass = 0.1;
				desc.inertia = 0.36 * Matrix3d::Unit();
				soBox.push_back(phscene->CreateSolid(desc));
				CDConvexMeshDesc md;
				int nv = rand() % 100 + 50;
				for(int i=0; i < nv; ++i){
					Vec3d v;
					for(int c=0; c<3; ++c){
						v[c] = (rand() % 100 / 100.0 - 0.5) * 5 * 1.3;
					}
					md.vertices.push_back(v);
				}
				CDShapeIf* s = GetSdk()->GetPHSdk()->CreateShape(md);
				soBox.back()->AddShape(s);
				soBox.back()->SetFramePosition(Vec3f(0.5, 20, 0));
//				soBox.back()->SetFramePosition(Vec3f(0.5, 10+3*soBox.size(),0));
				soBox.back()->GetShape(0)->SetVibration(-100,60,100);
				soBox.back()->GetShape(0)->SetStaticFriction(0.8f);
				soBox.back()->GetShape(0)->SetDynamicFriction(0.6f);

				soBox.back()->SetOrientation(Quaternionf::Rot((float)Rad(30), 'y'));  
				ostringstream os;
				os << "sphere" << (unsigned int)soBox.size();
				soBox.back()->SetName(os.str().c_str());
			}
			break;
		case 'm':
			{
				// Lump of Box
				soBox.push_back(phscene->CreateSolid(desc));
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				soBox.back()->AddShape(meshBox);
				Posed pose;
				pose.Pos() = Vec3d(3, 0, 0);
				soBox.back()->SetShapePose(1, pose);
				pose.Pos() = Vec3d(-3, 0, 0);
				soBox.back()->SetShapePose(2, pose);
				pose.Pos() = Vec3d(0, 3, 0);
				soBox.back()->SetShapePose(3, pose);
				pose.Pos() = Vec3d(0, -3, 0);
				soBox.back()->SetShapePose(4, pose);
				pose.Pos() = Vec3d(0, 0, 3);
				soBox.back()->SetShapePose(5, pose);
				pose.Pos() = Vec3d(0, 0, -3);
				soBox.back()->SetShapePose(6, pose);
				
				soBox.back()->SetFramePosition(Vec3f(0.5, 20, 0));
	//			soBox.back()->SetFramePosition(Vec3f(0.5, 10+3*soBox.size(),0));
				soBox.back()->SetOrientation(Quaternionf::Rot((float)Rad(30), 'y'));  
				ostringstream os;
				os << "box" << (unsigned int)soBox.size();
				soBox.back()->SetName(os.str().c_str());
			    phscene->SetContactMode(soPointer, PHSceneDesc::MODE_NONE);				
			}
			break;	
		case 'x':
			{
				// Wall
				PHSolidDesc soliddesc;
				soliddesc.mass = 0.05;
				soliddesc.inertia = 0.0333 * Matrix3d::Unit();
				int wall_height = 4;
				int numbox = 5;
				for(int i = 0; i < wall_height; i++){
					for(int j = 0; j < numbox; j++){
						soBox.push_back(phscene->CreateSolid(soliddesc));
						soBox.back()->AddShape(meshBox);
						soBox.back()->SetFramePosition(Vec3d(-4.0 + (2.0 + 0.1) * j , (2.0 + 0.1) * (double)i, -2.0));  
						soBox.back()->GetShape(0)->SetVibration(-200,120,300);
						soBox.back()->GetShape(0)->SetStaticFriction(0.8f);
						soBox.back()->GetShape(0)->SetDynamicFriction(0.6f);

					}
				}
			}
			break;
		case 'z':
			{
				// Tower
				PHSolidDesc soliddesc;
				soliddesc.mass = 0.05;
				soliddesc.inertia = 0.0333 * Matrix3d::Unit();
				double tower_radius = 5;
				int tower_height = 5;
				int numbox = 5;
				double theta;
				for(int i = 0; i < tower_height; i++){
					for(int j = 0; j < numbox; j++){
						soBox.push_back(phscene->CreateSolid(soliddesc));
						soBox.back()->AddShape(meshBox);
						theta = ((double)j + (i % 2 ? 0.0 : 0.5)) * Rad(360) / (double)numbox;
//						soBox.back()->SetFramePosition(Vec3f(0.5, 20, 0));
						soBox.back()->SetFramePosition(Vec3d(tower_radius * cos(theta), 2.0 * ((double)i), tower_radius * sin(theta)));
						soBox.back()->SetOrientation(Quaterniond::Rot(-theta, 'y'));  
						soBox.back()->GetShape(0)->SetVibration(-200,120,300);
						soBox.back()->GetShape(0)->SetStaticFriction(0.4f);
						soBox.back()->GetShape(0)->SetDynamicFriction(0.3f);
					}
				}
			}
			break;
		default:
			break;
	}
}