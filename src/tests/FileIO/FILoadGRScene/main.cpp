/**
 Springhead2/src/tests/FileIO/FILoadGRScene/main.cpp

【概要】
  Xファイルをロードし、Graphicsシーンをロード・描画する。
  
【終了基準】
  ・プログラムが正常終了したら0を返す。  
 
【処理の流れ】
  ・Xファイルをロードする。
  ・ロードした情報を出力する。
  ・Graphicsエンジンと接続し、描画する。

【テストパターン】
  ※ 本ファイルの28行目 " #define TEST_FILEX " にて、入力ファイル名を指定する。
	GRTest.x : XファイルからのグラフィックスSDKの入力テスト
*/
#include <Springhead.h>
#include <GL/glut.h>
#define	ESC				27				// Esc key
#define EXIT_TIMER	12000			// 強制終了させるステップ数
#define TEST_FILEX	"GRTest.x"		// ロードするXファイル

namespace Spr{
	PHSdkIf* phSdk;
	GRSdkIf* grSdk;
	GRSceneIf* scene;
	GRDeviceGLIf* grDevice;
	GRDebugRenderIf* render;
	void PHRegisterTypeDescs();
	void CDRegisterTypeDescs();
	void GRRegisterTypeDescs();
	void FIRegisterTypeDescs();
}
using namespace Spr;

Vec3f *vtx=NULL;


// 材質の設定
GRMaterialDesc mat_red(Vec4f(0.0, 0.0, 0.0, 1.0),		// ambient
						Vec4f(0.7, 0.0, 0.0, 1.0),		// diffuse
						Vec4f(1.0, 1.0, 1.0, 1.0),		// specular
						Vec4f(0.0, 0.0, 0.0, 1.0),		// emissive
						100.0);							// power
GRMaterialDesc mat_green(Vec4f(0.0, 0.0, 0.0, 1.0),		
						Vec4f(0.0, 0.7, 0.0, 1.0),		
						Vec4f(1.0, 1.0, 1.0, 1.0),		
						Vec4f(0.0, 0.0, 0.0, 1.0),		
						20.0);							
GRMaterialDesc mat_blue(Vec4f(0.0, 0.0, 1.0, 1.0),		
						Vec4f(0.0, 0.0, 0.7, 1.0),		
						Vec4f(1.0, 1.0, 1.0, 1.0),		
						Vec4f(0.0, 0.0, 0.0, 1.0),		
						20.0);							
GRMaterialDesc mat_yellow(Vec4f(0.0, 0.0, 1.0, 1.0),		
						  Vec4f(1.0, 0.7, 0.0, 1.0),		
						  Vec4f(1.0, 1.0, 1.0, 1.0),		
						  Vec4f(0.0, 0.0, 0.0, 1.0),		
						  20.0);							
std::vector<GRMaterialDesc> material;

/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void display(){
	//	バッファクリア
	render->ClearBuffer();
	render->SetMaterial(mat_red);
	scene->Render(render);
	if (!scene){
		std::cout << "scene == NULL. File may not found." << std::endl;
		exit(-1);
	}

	render->EndScene();
}
/**
 brief     	光源の設定
 param	 	なし
 return 	なし
 */
void setLight() {
	GRLightDesc light0;
	light0.ambient			= Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	light0.diffuse			= Vec4f(0.7f, 0.7f, 0.7f, 1.0f);
	light0.specular			= Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
	light0.position			= Vec4f(0.0f, 10.0f, 0.0f, 1.0f);
	light0.spotDirection	= Vec3f(0.0f, -1.0f, 0.0f);
	light0.spotCutoff		= 145.0f;
	light0.spotFalloff		= 30.0f;
	render->PushLight(light0);

	GRLightDesc light1;
	light1.diffuse			= Vec4f(0.8f, 0.8f, 0.8f, 1.0f);
	light1.specular			= Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
	light1.position			= Vec4f(0.0f, 10.0f, 10.0f, 0.0f);
	light1.spotDirection	= Vec3f(0.0f, -1.0f, 0.0f);
	render->PushLight(light1);
}
/**
 brief     	材質の設定
 param	 	なし
 return 	なし
 */
void setMaterial() {
	material.push_back(mat_red);
	material.push_back(mat_green);
	material.push_back(mat_blue);
	material.push_back(mat_yellow);
}

/**
 brief  	glutReshapeFuncで指定したコールバック関数
 param	 	<in/--> w　　幅
 param  	<in/--> h　　高さ
 return 	なし
 */
void reshape(int w, int h){
	render->Reshape(Vec2f(w,h));
}

/**
 brief 		glutKeyboardFuncで指定したコールバック関数 
 param		<in/--> key　　 ASCIIコード
 param 		<in/--> x　　　 キーが押された時のマウス座標
 param 		<in/--> y　　　 キーが押された時のマウス座標
 return 	なし
 */
void keyboard(unsigned char key, int x, int y){
	if (key == ESC) {
		delete[] vtx;
		exit(0);
	}
	if ('0'<= key && key <= '9' && phSdk->NScene()){
		int i = key-'0';
		static UTRef<Spr::ObjectStatesIf> states[10];
		if (states[i]){
			states[i]->LoadState(scene);
		}else{
			states[i] = CreateObjectStates();
			states[i]->SaveState(scene);
		}
	}
}

/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void idle(){
//	if(scene && *scene) (*(scene))->Step();
	glutPostRedisplay();
	static int count;
	count ++;
	if (count > EXIT_TIMER){
		delete[] vtx;
		std::cout << EXIT_TIMER << " count passed." << std::endl;
		exit(0);
	}
}

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int main(int argc, char* argv[]){
	PHRegisterTypeDescs();
	CDRegisterTypeDescs();
	GRRegisterTypeDescs();

	FISdkIf* fiSdk = CreateFISdk();
	FIFileXIf* fileX = fiSdk->CreateFileX();
	ObjectIfs objs;
	if (argc>=2){
		phSdk = CreatePHSdk();					//	PHSDKを用意して，
		objs.push_back(phSdk);		
		fileX->Load(objs, argv[1]);				//	ファイルローダに渡す方式
	}else{
		if (! fileX->Load(objs, TEST_FILEX) ) {	//	PHSDKごとロードして，
			DSTR << "Error: Cannot open load file. " << std::endl;
			exit(EXIT_FAILURE);
		}
		phSdk = NULL;
		grSdk = NULL;
		for(unsigned  i=0; i<objs.size(); ++i){	
			if(!phSdk) phSdk = DCAST(PHSdkIf, objs[i]);	//	PHSDKを受け取る方式
			if(!grSdk) grSdk = DCAST(GRSdkIf, objs[i]);	//	GRSdkも受け取る
		}
	}
	if (!grSdk) return -1;
	objs.clear();
	objs.Push(grSdk);
	fileX->Save(objs, "out.x");
	
	fiSdk->Clear();	//	ファイルローダのメモリを解放．
	objs.clear();
	grSdk->Print(DSTR);
	scene = grSdk->GetScene(0);		// Sceneの取得
	DSTR << "Loaded : " << "NScene=" << (int)grSdk->NScene() << std::endl;
	scene->Print(DSTR);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	int window = glutCreateWindow("FILoad");

	render = grSdk->CreateDebugRender();
	grDevice = grSdk->CreateDeviceGL(window);
	grDevice->Init();
	render->SetDevice(grDevice);

	// 視点設定
	Affinef view;
	view.Pos() = Vec3f(0.0, 3.0, 3.0);									// eye
		view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));		// center, up 
	view = view.inv();	
	render->SetViewMatrix(view);

	setLight();			// 光源設定
	setMaterial();		// 材質設定

	vtx = DBG_NEW Vec3f[4];
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
