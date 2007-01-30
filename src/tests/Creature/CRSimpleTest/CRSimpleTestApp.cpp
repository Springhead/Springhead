#include "CRSimpleTestApp.h"
#include <GL/glut.h>
#include <Foundation/Object.h>

using namespace Spr;
using namespace std;

CRSimpleTestApp::CRSimpleTestApp(int window, string filename)
:phSdk(NULL), grSdk(NULL), stepCount(0), isLoadComplete(false)
{
	Register();
	LoadFile(filename);
	CreateScene();
	CreateRender(window);
}


void CRSimpleTestApp::Register(){
	// TypeDesc‚Ì“o˜^
	PHRegisterTypeDescs();
	CDRegisterTypeDescs();
	GRRegisterTypeDescs();
	FIRegisterTypeDescs();
	CRRegisterTypeDescs();

	// Factory‚Ì“o˜^
	PHSdkIf::RegisterSdk();
	GRSdkIf::RegisterSdk();
	CRRegisterFactories();

	DSTR << "Register Complete." << endl;
}


void CRSimpleTestApp::LoadFile(string filename){
	fiSdk = FISdkIf::CreateSdk();
	fiFileX = fiSdk->CreateFileX();

	if (! fiFileX->Load(objs, filename.data()) ) {
		DSTR << "Error: Cannot open load file. " << endl;
		exit(EXIT_FAILURE);
	}
	DSTR << "Loaded " << (unsigned)(objs.size()) << "objects." << endl;

	DSTR << "LoadFile Complete." << endl;	
}


void CRSimpleTestApp::CreateScene(){
	for (unsigned  i=0; i<objs.size(); i++) {	
		if(!phSdk) phSdk = DCAST(PHSdkIf, objs[i]);
		if(!grSdk) grSdk = DCAST(GRSdkIf, objs[i]);
	}
	if (!phSdk) { DSTR << "PHSdk not found." << endl; }
	if (!grSdk) { DSTR << "GRSdk not found." << endl; }
	if (!phSdk || !grSdk) { exit(EXIT_FAILURE); }

	phScene = phSdk->GetScene(0);
	grScene = grSdk->GetScene(0);

	phSdk->Print(DSTR);
	grSdk->Print(DSTR);

	DSTR << "CreateScene Complete." << endl;	
}


void CRSimpleTestApp::CreateRender(int window){
	grRender = grSdk->CreateDebugRender();
	grDevice = grSdk->CreateDeviceGL(window);
	grDevice->Init();
	grRender->SetDevice(grDevice);

	Affinef view;
		view.Pos() = Vec3f(0.0, 3.0, 3.0);
		view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));
		view = view.inv();	
	grRender->SetViewMatrix(view);

	GRLightIf* light = NULL;
	for (unsigned i=0; i<grScene->NChildObject(); i++) {
		if (!light) light = DCAST(GRLightIf, grScene->GetChildObject(i));
	}
	grRender->PushLight(light);

	isLoadComplete = true;
	DSTR << "CreateRender Complete." << endl;	
}


void CRSimpleTestApp::Step(){
	if (!isLoadComplete) {return;}

	// cout << "cycle : " << stepCount << std::endl;
	phScene->Step();
	stepCount++;
}


void CRSimpleTestApp::Display(){
	if (!isLoadComplete) {return;}

	grRender->ClearBuffer();

	PHSolidIf** solids = phScene->GetSolids();
	for (int i_sol=0; i_sol<phScene->NSolids(); i_sol++) {
		Affinef af;
		solids[i_sol]->GetPose().ToAffine(af);
		grRender->PushModelMatrix();
		grRender->MultModelMatrix(af);

		for (int i_shp=0; i_shp<solids[i_sol]->NShape(); i_shp++) {
			CDShapeIf* shape = solids[i_sol]->GetShape(i_shp);
			Affinef af;
			solids[i_sol]->GetShapePose(i_shp).ToAffine(af);
			grRender->PushModelMatrix();
			grRender->MultModelMatrix(af);

			// Draw Mesh
			CDConvexMeshIf* mesh = DCAST(CDConvexMeshIf, shape);
			if (mesh) {
				Vec3f* vtx_base = mesh->GetVertices();
				for (size_t i_face=0; i_face<mesh->NFace(); i_face++) {
					CDFaceIf* face = mesh->GetFace(i_face);
					grRender->DrawFace(face, vtx_base);
				}
			}

			// Draw Sphere
			CDSphereIf* sphere = DCAST(CDSphereIf, shape);
			if (sphere) {
				float r = sphere->GetRadius();
				GLUquadricObj* quad = gluNewQuadric();
				gluSphere(quad, r, 16, 8);
				gluDeleteQuadric(quad);
			}

			// Draw Box
			CDBoxIf* box = DCAST(CDBoxIf, shape);
			if (box) {
				Vec3f* vtx = DBG_NEW Vec3f[4];
				Vec3f* vtx_base = box->GetVertices();
				for (size_t i_face=0; i_face<6; i_face++) {
					CDFaceIf* face = box->GetFace(i_face);
					for (int v=0; v<4; v++) {
						vtx[v] = vtx_base[face->GetIndices()[v]].data;
					}
					Vec3f normal, edge0, edge1;
					edge0 = vtx[1] - vtx[0];
					edge1 = vtx[2] - vtx[0];
					normal = edge0^edge1;
					normal.unitize();
					glNormal3fv(normal);
					grRender->SetVertexFormat(GRVertexElement::vfP3f);
					grRender->DrawDirect(GRRenderBaseIf::QUADS, vtx, 4);
				}
			}
			grRender->PopModelMatrix();
		}
		grRender->PopModelMatrix();
	}

	grRender->EndScene();	
	glutSwapBuffers();
}


void CRSimpleTestApp::Reshape(int w, int h){
	grRender->Reshape(Vec2f(), Vec2f(w,h));
}
