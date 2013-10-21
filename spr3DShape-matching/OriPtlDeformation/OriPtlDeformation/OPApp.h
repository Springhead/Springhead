#ifndef OPAPP_H
#define OPAPP_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>
#include <..\src\Graphics\GRMesh.h>
#include "FWAppSample.h"
#include "OpVertex.h"
#include "DeformParticle.h"
#include <list>



#define USE_DISPLAY = false;
#define tempPosDebug = false;
//#define OLD_DISPLAY
//#define DEFORM_TIME ;

using namespace Spr;
using namespace std;
class OPApp : public FWApp
{
public:  
		 PHSolidIf* floor;
		 DeformParticle dP,dP1;
		 
		 char* objFilePath;
		 GRMesh *grMeshBox,*grMeshMonkey,*grMeshBar;
		 //GRMesh *grFloor;
		 GRMesh backgrMesh;
		 //UTRef<string> fileName;
		 UTRef<ImportIf> import;
		 string fileName;
		 //MyCTimer mtimer;
		 float vertMass;
		 bool reset;
		 bool cluster8Base;
		 GRRenderIf* render;
		 int* topindexoftheBar;
		 

		 //static bool resetFlag = false;
	void Init(int argc=0, char* argv[]=0)
		{
			//ファイルのロード
			CreateSdk();			// SDKの作成
			import = GetSdk()->GetFISdk()->CreateImport();
			loadMyScene();			// sprファイルからシーンロード
			GRInit(argc, argv);		// ウィンドウマネジャ初期化
			CreateWin();			// ウィンドウを作成
			CreateTimer();			// タイマを作成
			reset = false;
			cluster8Base = false;
			vertMass = 1;

			render =  GetCurrentWin()->GetRender();
			
			// 床を作成
			/*PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
			floor = phscene->CreateSolid();
			CDBoxDesc bd;
			floor->SetDynamical(false);
			bd.boxsize = Vec3f(15.0f, 1.0f, 15.0f);
			floor->AddShape(GetSdk()->GetPHSdk()->CreateShape(bd));
			*/

			//カメラ位置調整
			Vec3d pos = Vec3d(0.0, 5.0, 10.0);		// カメラ初期位置
			GetCurrentWin()->GetTrackball()->SetPosition(pos);	// カメラ初期位置の設定
			
			//// 箱を作成
			//
			//boxPhIf = phscene->CreateSolid();
			//bd.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
			//
			//boxPhIf->AddShape(phSdk->CreateShape(bd));
			//boxPhIf->SetFramePosition(Vec3d(0.0, 15.0, 0.0));
			//
			initalGRMeshObjects();

			//STEPタイマ作成
			UTTimerIf* thermoTimer = CreateTimer(UTTimerIf::FRAMEWORK);
			int intervaltime = 10;
			//FEMdt = intervaltime / 1000.0; 
			thermoTimer->SetInterval(intervaltime);								// FEMを呼ぶ時間間隔 TimerFunc()で、fEngine->Set_dtでFEMdt==0.1を代入
			thermoTimer->SetResolution(1);
			//thermoTimerID = thermoTimer->GetID();

			GetSdk()->SetDebugMode(false);//trueにしたら画面上のシーンすべてが消える、
		}
		
		
		void initalGRMeshObjects()
		{//grMesh初期設定
			
			grMeshBox = GetSdk()->GetScene()->FindObject("boxMesh")->Cast();//->GetGRScene()->GetChildObject(1);
			//grMeshMonkey = GetSdk()->GetScene()->FindObject("monkeyMesh")->Cast();
			grMeshBar = GetSdk()->GetScene()->FindObject("barMesh")->Cast();

			initalDeformVertex(grMeshBox,dP);
			initalDeformVertex(grMeshBar,dP1);

			
			topindexoftheBar = new int[4];
			int k =0;
			for(int j=0;j<dP1.mPos.size();j++)
			{	
						if(dP1.mPos[j].y == -2.5 )
						{topindexoftheBar[k] = j;k++;}
			}
			if(grMeshBox!=NULL)
			grMeshBox->EnableAlwaysCreateBuffer();
			if(grMeshMonkey!=NULL)
			grMeshMonkey->EnableAlwaysCreateBuffer();
			if(grMeshBar!=NULL)
			grMeshBar->EnableAlwaysCreateBuffer();
		}
	
		void initalDeformVertex(GRMesh *grMesh, DeformParticle &dParticle)
		{//DeformVertex初期化
			if(grMesh&&!reset)
			{
				Vec3f* tempPos = grMesh->GetVertices();;
				for(int i=0;i<grMesh->NVertex();i++)
				{
						dParticle.addVertex(Vec3f(tempPos[i].x,tempPos[i].y,tempPos[i].z),vertMass);
				}

				cout<<"DeformVertexCreateSuccess"<<endl;
			}
		}

		void inputPosition(GRMesh *grMesh, DeformParticle &dParticle)
		{
			if(reset)
				return;
			int ind = grMesh->NTriangle();
			Vec3f* tempPos = grMesh->GetVertices();
			for(int i=0;i<grMesh->NVertex();i++)
				{
						dParticle.mPos[i].x = tempPos[i].x;
						dParticle.mPos[i].y = tempPos[i].y;
						dParticle.mPos[i].z = tempPos[i].z;
				}
		}
		void UserFunc()
		{
			//floor->SetFramePosition(Vec3d(10, -3.5, 10));
		}
		void TimerFunc(int id)
		{
			
			if(id !=1)
			{
				FWApp::TimerFunc(id);
				return;
			}
#ifdef DEFORM_TIME
			mt.startMyTimer();
#endif
			if(dP.debugStepOn||reset)
				return;

			inputPosition(grMeshBox,dP);
			inputPosition(grMeshBar,dP1);
			//dParticle.gravityStep();
			dP.timeStep();
			//dP1.timeStep();
			//dP.timeStepwithCluster(grMeshBox);
			if(cluster8Base)
			dP1.timeStep8BasedCluster(grMeshBar);
			else dP1.timeStep16BasedCluster(grMeshBar);
			UpdatePosition(grMeshBox,dP);
			UpdatePosition(grMeshBar,dP1);
			GRRenderIf* render = GetCurrentWin()->GetRender();
			render->SetMaterial(30);

#ifdef DEFORM_TIME
			mt.endMyTimer();
			mt.printElastedTime();
#endif
			
		}
		int ceilOrFloor(float f);
		void UpdatePosition(GRMesh *grMesh, DeformParticle &dParticle)//計算済みの位置を更新
		{
			Vec3f* tempPos= grMesh->GetVertices();
			
				for(int i=0;i<grMesh->NVertex();i++)
				{
						tempPos[i].x = dParticle.mPos[i].x;
						tempPos[i].y = dParticle.mPos[i].y;
						tempPos[i].z = dParticle.mPos[i].z;
				}
				//cout<<"updated";

		}



	void Display() {
		/*if (debugRender){
			GetCurrentWin()->GetScene()->DrawPHScene(render);
		}else{
			GetCurrentWin()->GetScene()->Sync();*/
		//FWApp::Display();
		FWApp::OnDisplay();

		Vec3f* tempPos = grMeshBox->GetVertices();
		
			//render->BeginScene();
			//render->ClearBuffer(true, true);
		//	剛体数の表示
		float pos = 10.0;
		std::ostringstream sstr;
		for(int i=0;i<grMeshBox->NVertex();i++)
		{
			sstr <<i+1 ;
			render->DrawFont(tempPos[i], sstr.str());
			sstr.str("");

		}
		for(int i=0;i<grMeshBar->NVertex();i++)
		{
			sstr <<i+1 ;
			render->DrawFont(dP1.mPos[i], sstr.str());
			//render->DrawFont(dP.mGoalPos[i], sstr.str());
			sstr.str("");

		}

		//printOnPos("Box alpha =", dP.params.alpha, pos);

		render->EnterScreenCoordinate();
		sstr << "Bar alpha =" <<dP1.params.alpha<<endl;
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");
		
		pos+=10.0;
		sstr << "Bar beta = " <<dP1.params.beta<<'\n';
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");

		pos+=10.0;
		sstr << "Box gama = " <<dP1.params.gama<<'\n';
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");

		pos+=10.0;
		sstr << "Bar LinearORQuadratic = " <<dP1.params.quadraticMatch<<'\n';
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");

		pos+=10.0;
		sstr << "Bar AllowFlip = " <<dP1.params.allowFlip<<'\n';
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");

		pos+=10.0;
		sstr << "Bar volumeConservation = " <<dP1.params.volumeConservation<<endl;
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");

		pos+=20.0;
		sstr << "Key Manual :" ;
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");

		pos+=10.0;
		sstr << "Key a: Fix a point of box  " ;
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");

		pos+=10.0;
		sstr << "Key x: Fix a point of bar  " ;
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");

		pos+=10.0;
		sstr << "Key z: Enter/Exit step simulation mode  " ;
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");

		pos+=10.0;
		sstr << "Key s: (After enter step simulation mode) simu-by-step   " ;
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");
		pos+=10.0;
		sstr << "Key '*'/'/': Bar alpha param edit   " ;
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");

		pos+=10.0;
		sstr << "Key +/-: Bar beta param edit   " ;
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");

		pos+=10.0;
		sstr << "Key e: Bar Cluster-deform 16ps/8ps Based switch " <<cluster8Base<<endl;
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");
		pos+=10.0;
		sstr << "Key q: Linear/Quadratic Deform " ;
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");
		pos+=10.0;
		sstr << "Key v: VolumeConservation" ;
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");
		pos+=10.0;
		sstr << "Key f: Fix bottom of Bar" ;
		render->DrawFont(Vec2f(0.5,pos), sstr.str());
		sstr.str("");
		render->LeaveScreenCoordinate();
		//	シミュレーション経過時間の表示
		//render->SetDepthTest(false);
		//render->EnterScreenCoordinate();
		
		//sstr <<"Press 'a' and 'x' to fix two object";
		//render->DrawFont(Vec3f(3,pos,3), sstr.str());
	}

#if OLD_DISPLAY
		
		void Display()
		{
			//FWApp::Display();
			// 描画モードの設定
			GetSdk()->SetDebugMode(true);				// デバックモードで描画
			GRRenderIf* render = GetCurrentWin()->GetRender();
			GRCameraDesc camera = render->GetCamera();
			camera.front = 0.3f;
			
			render->SetCamera(camera);
				FWWinIf* win = GetCurrentWin();	render->SetViewMatrix(win->GetTrackball()->GetAffine().inv());

			// 描画の実行
			if(!GetCurrentWin()) return;	
			render->BeginScene();
			render->SetClearColor(Vec4f(0,0,0,1));
			render->ClearBuffer(true,true);

			GRLightDesc light;
			light.diffuse = Vec4f(0.9,0.9,0.9,1);
			light.ambient = Vec4f(0.5,0.5,0.5,1);
			light.specular = Vec4f(0.9,0.9,0.9,1);
			light.position = Vec4f(1,1,1,0);
			render->PushLight(light);
			light.position = Vec4f(-2,-1,-1,0);
			render->PushLight(light);

	
			DrawInit(render);
			DrawModel(render);
			render->PopLight();
			render->PopLight();
			render->EndScene();
			render->SwapBuffers();

		}
#endif

		
#if OLD_OBJ_DRAW

				/*void Display()
		{
			render->DrawFont(Vec2f(200,70),"HelloWorld");
			FWApp::Display();
		}*/

		//void DrawInit(GRRenderIf* render)
		//{
		//	
		//		if(true)
		//	{
		//	render->SetAlphaTest(true);
		//	render->SetAlphaMode(Spr::GRRenderBaseIf::TBlendFunc::BF_SRCALPHA,Spr::GRRenderBaseIf::TBlendFunc::BF_INVSRCALPHA);
		//	}	
		//	//床の描画
		//	int p=5,h=-3;
		//	render->SetAlphaTest(false);
		//	glBegin(GL_QUADS);
		//	render->SetMaterial(50);
		//	glNormal3dv(Vec3d(0,1,0));
		//	glVertex3d(p,h,p);
		//	glVertex3d(p,h,-p);
		//	glVertex3d(-p,h,-p);
		//	glVertex3d(-p,h,p);
		//	glEnd();
		//	

		//}

		//void DrawModel(GRRenderIf* render)
		//{
		//	
		//		if(true)
		//	{
		//	render->SetAlphaTest(true);
		//	render->SetAlphaMode(Spr::GRRenderBaseIf::TBlendFunc::BF_SRCALPHA,Spr::GRRenderBaseIf::TBlendFunc::BF_INVSRCALPHA);
		//	}	
		//	
		//		//objModelの描画
		//	int p=2,h=2;
		//	render->SetAlphaTest(false);
		//	glBegin(GL_TRIANGLES);
		//	for(int i=0;i<dParticle.mDrawVertices.size();i++)
		//		{
		//			
		//			glVertex3d(dParticle.mDrawVertices[i].x,dParticle.mDrawVertices[i].y,dParticle.mDrawVertices[i].z);
		//		}
		//	//render->SetMaterial(30);
		//	//glNormal3dv(Vec3d(0,1,0));
		//	//glVertex3d(p,h,p);
		//	//glVertex3d(p,h,-p);
		//	//glVertex3d(-p,h,-p);
		//	////glVertex3d(-p,h,p);

		//	glEnd();
		//}
#endif
		void loadMyScene()
		{
			fileName = "./files/sceneSample.spr";
			
			if(!GetSdk()->LoadScene(fileName, import))
				cout<<"SceneLoadFromFileFailed"<<endl;			// ファイルのロード
			else cout<<"SceneLoadFromFileSuccess"<<endl;
		}

		//extra functions
		void Keyboard(int key, int x, int y);
		void CalGRObjNormal(GRMesh& objMesh);

struct MyCTimer
{
public:	SYSTEMTIME startT;
	SYSTEMTIME endT;
	SYSTEMTIME elastedT;
	void startMyTimer()
		{
			GetLocalTime(&startT);
		}
	void endMyTimer()
	{
		GetLocalTime(&endT);
		GetLocalTime(&elastedT);
		elastedT.wSecond = endT.wSecond - startT.wSecond;
		elastedT.wMilliseconds = endT.wMilliseconds - startT.wMilliseconds;
	}
	void printElastedTime()
	{
		printf("elasted:%02d,%03d\n", elastedT.wSecond,elastedT.wMilliseconds);
	}
} mt;

};
#endif