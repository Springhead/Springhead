#include "FWAppGL.h"
#include <GL/glut.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
	IF_IMP(FWAppGL, Object);
	OBJECT_IMP(FWAppGL, Object);

	UTRef<FWAppGLIf> FWAppGL::instance = NULL;

	FWAppGLIf* SPR_CDECL CreateFWAppGL(){
		if (!FWAppGL::instance) {
			FWAppGL::instance = DBG_NEW FWAppGL;
		}
		return FWAppGL::instance;
	}

	void FWAppGL::GlutDisplayFunc(){
		FWAppGL::instance->Display();
	}

	void FWAppGL::GlutReshapeFunc(int w, int h){
		FWAppGL::instance->Reshape(w, h);
	}

	void FWAppGL::GlutIdleFunc(){
		FWAppGL::instance->Idle();
	}

	void FWAppGL::GlutKeyboardFunc(unsigned char key, int x, int y){
		FWAppGL::instance->Keyboard(key, x, y);
	}

	FWAppGL::FWAppGL(const FWAppGLDesc& d/*=FWAppGLDesc()*/)
	:phSdk(NULL), grSdk(NULL), fwScene(NULL), cycleCount(0), isLoadComplete(false), isSimulating(true)
	{
		 vtx = DBG_NEW Vec3f[4];
	}

	FWAppGL::~FWAppGL(){
		delete firstState;
		delete vtx;
		delete fwScene;
	}

	void FWAppGL::StartApp(std::string filename, int lim/*=-1*/){
		cycleLimit = lim;
		LoadFile(filename);
		CreateScene();
		CreateRender();		
		//
		glutDisplayFunc(FWAppGL::GlutDisplayFunc);
		glutReshapeFunc(FWAppGL::GlutReshapeFunc);
		glutKeyboardFunc(FWAppGL::GlutKeyboardFunc);
		glutIdleFunc(FWAppGL::GlutIdleFunc);
		glutMainLoop();
	}

	void FWAppGL::LoadFile(std::string filename){
		fiSdk = CreateFISdk();
		fiFileX = fiSdk->CreateFileX();

		if (! fiFileX->Load(objs, filename.data()) ) {
			DSTR << "Error: Cannot open load file. " << std::endl;
			exit(EXIT_FAILURE);
		}
		DSTR << "Loaded " << (unsigned)(objs.size()) << "objects." << std::endl;

		DSTR << "LoadFile Complete." << std::endl;	
	}

	void FWAppGL::CreateScene(){
		// Search FWScene
		for (unsigned  i=0; i<objs.size(); i++) {
			if(!fwScene) fwScene = DCAST(FWSceneIf, objs[i]);
		}
		if (!fwScene) {
			DSTR << "FWScene not found." << std::endl;
			exit(EXIT_FAILURE);
		}

		// Search PH and GR Scenes
		for (unsigned  i=0; i<objs.size(); i++) {
			if(!phSdk) phSdk = DCAST(PHSdkIf, objs[i]);
			if(!grSdk) grSdk = DCAST(GRSdkIf, objs[i]);
		}
		if (!phSdk || !grSdk) {
			if (!phSdk) { DSTR << "PHSdk not found." << std::endl; }
			if (!grSdk) { DSTR << "GRSdk not found." << std::endl; }
			exit(EXIT_FAILURE);
		}

		phScene = phSdk->GetScene(0);
		grScene = grSdk->GetScene(0);

		fwScene->Link(phScene, grScene);

		phScene->Print(DSTR);
		grScene->Print(DSTR);
		fwScene->Print(DSTR);

		firstState = CreateObjectStates();
		firstState->SaveState(phScene);

		DSTR << "CreateScene Complete." << std::endl;	
	}

	void FWAppGL::CreateRender(){
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		int window = glutCreateWindow("Springhead Application");

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
		DSTR << "CreateRender Complete." << std::endl;	
	}

	void FWAppGL::Step(){
		if (!isLoadComplete) {return;}
		if (cycleLimit > 0) {
			cycleCount++;
			if (cycleLimit < cycleCount) {
				exit(0);
			}
		}
		phScene->Step();
	}

	void FWAppGL::Display(){
		if (!isLoadComplete) {return;}

		grRender->ClearBuffer();

		//PHSolidIf** solids = phScene->GetSolids();
		for (size_t i_obj=0; i_obj<fwScene->NChildObject(); i_obj++) {
			FWObjectIf* obj = DCAST(FWObjectIf, fwScene->GetChildObject(i_obj));
			if (!obj) {continue;}
			PHSolidIf* solid = obj->GetPHSolid();
			
			Affinef af;
			solid->GetPose().ToAffine(af);
			grRender->PushModelMatrix();
			grRender->MultModelMatrix(af);

			for (int i_shp=0; i_shp<solid->NShape(); i_shp++) {
				CDShapeIf* shape = solid->GetShape(i_shp);
				Affinef af;
				solid->GetShapePose(i_shp).ToAffine(af);
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
					//Vec3f* vtx = DBG_NEW Vec3f[4];
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
	}

	void FWAppGL::Reshape(int w, int h){
		grRender->Reshape(Vec2f(), Vec2f(w,h));
	}

	void FWAppGL::Idle(){
		if (isSimulating){ Step(); }
		glutPostRedisplay();
	}

	void FWAppGL::Keyboard(unsigned char key, int x, int y){
		if (key == 27) {
			exit(0);
		}
		if (key == 'r') {
			firstState->LoadState(phScene);
		}
		if (key == '.') {
			isSimulating = false;
			Step();
		}
		if (key == ' ') {
			isSimulating = true;
		}
	}
}
