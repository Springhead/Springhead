/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/tests/GraphicsD3D/GRD3DLoadMeshmain.cpp

【概要】
  ファイル入出力SDKで、Xファイルの詳細を解析させる。
  （頂点位置、頂点インデックス、法線、マテリアル、テクスチャ）

【終了基準】
  ・プログラムが正常終了したら0を返す。  

【Note】
  本ファイルの26行目" #define TEST_CASE " で、ロードするXファイルの入力切り替えが可能。

*/
#include <Springhead.h>
#include <SprGraphicsD3D.h>
//#include <GL/glut.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#define	ESC				27				// Esc key

#define TEST_CASE		4				// テストケース

#if defined(TEST_CASE) && (TEST_CASE == 0)
#define EXIT_TIMER	20000				// 強制終了させるステップ数
#define TEST_FILEX	"box.x"				// ロードするXファイル

#elif defined(TEST_CASE) && (TEST_CASE == 1)
#define EXIT_TIMER	20000				
#define TEST_FILEX	"funiture.x"			

#elif defined(TEST_CASE) && (TEST_CASE == 2)
#define EXIT_TIMER	20000				
#define TEST_FILEX	"tire.x"				

#elif defined(TEST_CASE) && (TEST_CASE == 3)
#define EXIT_TIMER	20000				
#define TEST_FILEX	"kobito.x"			
#define TEST_MOTION	"walk"				// 再生するモーション

#elif defined(TEST_CASE) && (TEST_CASE == 4)
#define EXIT_TIMER	20000				
#define TEST_FILEX	"n175Anim.x"			
#define TEST_MOTION	"RunUpGround"				// 再生するモーション

#endif


namespace Spr{
	UTRef<GRSdkIf> grSdk;
	UTRef<GRSdkD3DIf> grSdkD3D;
	UTRef<GRSceneIf> scene;
	UTRef<FISdkIf> fiSdk; 
	//GRDeviceGLIf* grDevice;
	GRDeviceD3DIf* grDevice;
	GRDebugRenderIf* render = NULL;
	GRAnimationMeshIf* aniMesh = NULL;
	void FWRegisterTypeDescs();
	void FWRegisterOldSpringheadNode();
}
using namespace Spr;


/**
 brief     	WM_PAINTハンドラ
 param	 	なし
 return 	なし
 */
void display(){
	if(!render) return;

	//	バッファクリア
	render->ClearBuffer();
	render->BeginScene();
	scene->Render(render);
	if (!scene){
		DSTR << "scene == NULL. File may not found." << std::endl;
		exit(-1);
	}
	render->EndScene();
	//glutSwapBuffers();
}

/**
 brief		光源の設定
 param	 	なし
 return 	なし
 */
void setLight() {
	GRLightDesc light0;
	light0.ambient	= Vec4f(0.2, 0.2, 0.2, 1.0);
	light0.diffuse	= Vec4f(0.8, 0.8, 0.8, 1.0);
	light0.specular	= Vec4f(1.0, 1.0, 1.0, 1.0);
	light0.position = Vec4f(-10.81733, 100.99039, -21.19059, 0.0);
	light0.attenuation0  = 1.0;
	light0.attenuation1  = 0.0;
	light0.attenuation2  = 0.0;
	light0.spotDirection = Vec3f(0.0, -1.0, 0.0);
	light0.spotFalloff   = 10.0;
	light0.spotInner	 = 20;
	light0.spotCutoff	 = 150.0;
	render->PushLight(light0);

	GRLightDesc light1;
	light1.ambient	= Vec4f(0.2, 0.2, 0.2, 1.0);
	light1.diffuse	= Vec4f(0.8, 0.8, 0.8, 1.0);
	light1.specular	= Vec4f(1.0, 1.0, 1.0, 1.0);
	light1.position = Vec4f(-1.85930, 100.10970, -49.58080, 0.0);
	light1.attenuation0  = 1.0;
	light1.attenuation1  = 0.0;
	light1.attenuation2  = 0.0;
	light1.spotDirection = Vec3f(0.0, -1.0, 0.0);
	light1.spotFalloff   = 0.0;
	light1.spotInner	 = 10;
	light1.spotCutoff	 = 150.0;
	render->PushLight(light1);
}

/**
 brief  	WM_SIZEハンドラ
 param	 	<in/--> w　　幅
 param  	<in/--> h　　高さ
 return 	なし
 */
void reshape(int w, int h){
	if(render) render->Reshape(Vec2f(), Vec2f(w,h));
}

/**
 brief  	アイドル関数
 param	 	ウインドウハンドル
 return 	なし
 */
void idle(HWND hWnd){
//	if(scene && *scene) (*(scene))->Step();
	if(aniMesh){
		static DWORD prevTime = timeGetTime();
		DWORD currTime = timeGetTime();
		aniMesh->SetTime(currTime/1000.0);
		scene->GetWorld()->SetTransform( Affinef::Rot((float)M_PI/180.0f/10,'Y') * scene->GetWorld()->GetTransform() );
		if(GetKeyState('Q')<0 || GetKeyState('W')<0){
			static double w;
			if(GetKeyState('Q')<0) { w+=(currTime-prevTime)/500.0f;  if(w>1) w=1; }
			if(GetKeyState('W')<0) { w-=(currTime-prevTime)/500.0f;  if(w<0) w=0; }
			aniMesh->OverrideBoneOrientation("head", Quaterniond::Rot(Radf(-90), Vec3d(0,1,0)), w);
		}
		if(GetKeyState('A')<0 || GetKeyState('S')<0){
			static double w;
			if(GetKeyState('A')<0) { w+=(currTime-prevTime)/500.0f;  if(w>1) w=1; }
			if(GetKeyState('S')<0) { w-=(currTime-prevTime)/500.0f;  if(w<0) w=0; }
			aniMesh->OverrideBoneOrientation("arm1_L_", Quaterniond::Rot(Radf(-90), Vec3d(1,0,0)), w);
		}
		if(GetKeyState('Z')<0 || GetKeyState('X')<0){
			static double w;
			if(GetKeyState('Z')<0) { w+=(currTime-prevTime)/500.0f;  if(w>1) w=1; }
			if(GetKeyState('X')<0) { w-=(currTime-prevTime)/500.0f;  if(w<0) w=0; }
			aniMesh->OverrideBoneOrientation("leg1_R_", Quaterniond::Rot(Radf(90), Vec3d(1,0,0)), w);
		}
		prevTime = currTime;
	}
	else{
		scene->GetWorld()->SetTransform( Affinef::Rot((float)M_PI/180.0f/10,'X') * scene->GetWorld()->GetTransform() );
	}
	//glutPostRedisplay();
	InvalidateRect(hWnd, NULL, FALSE);
	static int count=0;
	count++;
	if (count > EXIT_TIMER){
		DSTR << EXIT_TIMER << " count passed." << std::endl;
		exit(0);
	}
}

/**
 brief  	ウインドウプロシージャ
 */
LRESULT CALLBACK mainWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg){
		case WM_KEYDOWN:
			switch(wParam){
				case VK_ESCAPE:
					PostQuitMessage(0);
					return 0;
			}
			break;

		case WM_SIZE:
			if(wParam==SIZE_RESTORED || wParam==SIZE_MAXIMIZED){
				reshape(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;
			}
			break;

		case WM_PAINT:
			display();
			ValidateRect(hWnd, NULL);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/**
 brief  	ウインドウ作成関数
 param	 	インスタンスハンドル
 return 	ウインドウハンドル
 */
HWND createMainWindow(HINSTANCE hInstance)
{
    WNDCLASSEX wndClass;

	wndClass.cbSize        = sizeof(WNDCLASSEX);
    wndClass.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndClass.lpfnWndProc   = mainWindowProc;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = 0;
    wndClass.hInstance     = hInstance;
    wndClass.hIcon         = NULL;
    wndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = NULL;
    wndClass.lpszMenuName  = NULL;
    wndClass.lpszClassName = "GRD3DLoadMesh";
	wndClass.hIconSm       = NULL;

	RegisterClassEx(&wndClass);


	HWND        result  = NULL;
	const DWORD style   = WS_OVERLAPPEDWINDOW;
	const DWORD exstyle = 0;
	RECT        rect    = {0, 0, 300, 300};

	AdjustWindowRectEx(&rect, style, FALSE, exstyle);

	result = CreateWindowEx(
		exstyle, wndClass.lpszClassName, "GRD3DLoadMesh", style,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right-rect.left, rect.bottom-rect.top,
		NULL, NULL, hInstance, NULL );
	
	assert( result!=NULL );

	return result;
}

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int main(int argc, char* argv[]){
	grSdk = GRSdkIf::CreateSdk();	
	grSdkD3D = GRSdkD3DIf::CreateSdk();
	scene = grSdk->CreateScene(GRSceneDesc());

#if defined(TEST_CASE) && (TEST_CASE == 3 || TEST_CASE == 4) 
	GRAnimationMeshDesc aniDesc;
	aniDesc.filename = TEST_FILEX;
	aniMesh = grSdkD3D->CreateAnimationMesh(aniDesc);
	scene->AddChildObject(aniMesh);
#else
	//	すべてのSDKとDirectXのローダの登録
	//	全ライブラリをリンクしなければならなくなる．
	FWSdkIf::RegisterSdk();

	fiSdk = FISdkIf::CreateSdk();
	FIFileXIf* fileX = fiSdk->CreateFileX();
	ObjectIfs objs;

	objs.push_back(grSdk);
	objs.push_back(scene);
	fileX->Load(objs, TEST_FILEX);
 

	if (!grSdk) return -1;
	objs.clear();
	objs.Push(grSdk);
	fileX->Save(objs, "out.x");
	
	fiSdk->Clear();	//	ファイルローダのメモリを解放．
	objs.clear();
	grSdk->Print(DSTR);
#endif

	DSTR << "Loaded : " << "NScene=" << (int)grSdk->NScene() << std::endl;
	scene->Print(DSTR);

	/*
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	int window = glutCreateWindow("GRD3DLoadMesh");
	*/
	HWND hWnd = createMainWindow(GetModuleHandle(NULL));
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	render = grSdk->CreateDebugRender();
	//grDevice = grSdk->CreateDeviceGL();
	grDevice = grSdkD3D->CreateDeviceD3D();
	grDevice->Init();
	render->SetDevice(grDevice);
	
	{
		RECT r;
		GetClientRect(hWnd, &r);
		render->Reshape(Vec2f(), Vec2f(r.right, r.bottom));
	}

	// 視点設定
#if defined(TEST_CASE) && (TEST_CASE == 4) 
	Affinef view;
	if(aniMesh){
		view.Pos() = Vec3f(0.0, 1.0,-2.0);								// eye
			view.LookAtGL(Vec3f(0.0, 1.0, 0.0), Vec3f(0.0, 1.1, 0.0));	// center, up 
	}
	else{
		view.Pos() = Vec3f(0.0, 3.0,-80.0);									// eye
			view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));		// center, up 
	}
#else
	Affinef view;
	if(aniMesh){
		view.Pos() = Vec3f(0.0, 40.0,-80.0);								// eye
			view.LookAtGL(Vec3f(0.0, 40.0, 0.0), Vec3f(0.0, 41.0, 0.0));	// center, up 
	}
	else{
		view.Pos() = Vec3f(0.0, 3.0,-80.0);									// eye
			view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));		// center, up 
	}
#endif
	view = view.inv();	
	render->SetViewMatrix(view);

	// 光源の設定
	setLight();

	if(aniMesh) aniMesh->SetMotion(TEST_MOTION);

	/*
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	*/
	while(true){
		MSG msg;
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if(msg.message==WM_QUIT) return (int)msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		idle(hWnd);
		WaitMessage();
	}

	return 0;
}
