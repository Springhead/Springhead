/**
 Springhead2/src/tests/FileIO/FILoader/main.cpp

【概要】
  Xファイルをロードし、Physicsエンジンと接続してシミュレーションする。
  
【終了基準】
  ・プログラムが正常終了したら0を返す。  
 
【処理の流れ】
  ・Xファイルをロードする。
  ・ロードした情報を出力する。
  ・Physicsエンジンと接続し、シミュレーションさせる。

【テストパターン】
　  test1.x	：凸形状(mesh)のテスト
    test2.x	：凸形状(mesh)のテスト（お互い衝突させない剛体として、"soBlock2"と"soFloor"を設定）
    test3.x	：凸形状(mesh)と球(sphere)のテスト
	test4.x	：凸形状(mesh)と球(sphere)と直方体(box)のテスト
	test5.x : timeStep=0.001におけるテスト

 */
#include <Springhead.h>
#include <GL/glut.h>
#define	ESC				27
#define EXIT_TIMER		3000

namespace Spr{
	PHSdkIf* phSdk;
	GRSdkIf* grSdk;
	PHSceneIf** scene;
	CDShapeIf** shape;
	GRDeviceGLIf* grDevice;
	GRDebugRenderIf* render;
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
	//render->SetMaterial(mat_red);	

	PHSceneIf* scene = NULL;
	if (phSdk->NScene()){
		PHSceneIf** s = phSdk->GetScenes();
		scene = *s;
	}
	if (!scene){
		std::cout << "scene == NULL. File may not found." << std::endl;
		exit(-1);
	}
	PHSolidIf **solids = scene->GetSolids();
	for (int num=0; num < scene->NSolids(); ++num){
		render->SetMaterial(material[num]);			// 材質設定

		Affinef af;
		solids[num]->GetPose().ToAffine(af);
		render->PushModelMatrix();
		render->MultModelMatrix(af);
		
		CDShapeIf** shapes = solids[num]->GetShapes();
		for(int s=0; s<solids[num]->NShape(); ++s){
			Affinef af;
			shapes[s]->GetPose().ToAffine(af);
			render->PushModelMatrix();
			render->MultModelMatrix(af);
			CDConvexMeshIf* mesh = ICAST(CDConvexMeshIf, shapes[s]);
			if (mesh){
				Vec3f* base = mesh->GetVertices();
				for (size_t f=0; f<mesh->NFace(); ++f) {	
					CDFaceIf* face = mesh->GetFace(f);
					render->DrawFace(face, base);
				}
			}
			CDSphereIf* sphere = ICAST(CDSphereIf, shapes[s]);
			if (sphere){
				float r = sphere->GetRadius();
				GLUquadricObj* quad = gluNewQuadric();
				gluSphere(quad, r, 16, 8);
				gluDeleteQuadric(quad);
			}
			CDBoxIf* box = ICAST(CDBoxIf, shapes[s]);
			if (box){
#if 0				
				Vec3f boxsize = box->GetBoxSize();
				glScalef(boxsize.x, boxsize.y, boxsize.z);	
				glutSolidCube(1.0);		
#else
				Vec3f* base =  box->GetVertices();
				for (size_t f=0; f<6; ++f) {	
					CDFaceIf* face = box->GetFace(f);

					for (int v=0; v<4; ++v)
						vtx[v] = base[face->GetIndices()[v]].data;
					Vec3f normal, edge0, edge1;
					edge0 = vtx[1] - vtx[0];
					edge1 = vtx[2] - vtx[0];
					normal = edge0^edge1;
					normal.unitize();
					glNormal3fv(normal);
					render->DrawDirect(GRRenderBaseIf::QUADS, vtx, vtx+4);
				}
#endif 					
			}
			render->PopModelMatrix();
		}
		render->PopModelMatrix();
	}



	render->EndScene();
}
/**
 brief     	光源の設定
 param	 	なし
 return 	なし
 */
void setLight() {
	GRLight light0;
	light0.ambient			= Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	light0.diffuse			= Vec4f(0.7f, 0.7f, 0.7f, 1.0f);
	light0.specular			= Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
	light0.position			= Vec4f(0.0f, 10.0f, 0.0f, 1.0f);
	light0.spotDirection	= Vec3f(0.0f, -1.0f, 0.0f);
	light0.spotCutoff		= 145.0f;
	light0.spotFalloff		= 30.0f;
	render->PushLight(light0);

	GRLight light1;
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
}

/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void idle(){
	if(scene && *scene) (*(scene))->Step();
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
	FISdkIf* fiSdk = CreateFISdk();
	FIFileXIf* fileX = fiSdk->CreateFileX();
	ObjectIfs objs;
	if (argc>=2){
		phSdk = CreatePHSdk();					//	PHSDKを用意して，
		objs.push_back(phSdk);		
		fileX->Load(objs, argv[1]);				//	ファイルローダに渡す方式
	}else{
		if (! fileX->Load(objs, "test5.x") ) {	//	PHSDKごとロードして，
//		if (! fileX->Load(objs, "GRTest.x") ) {	//	
			DSTR << "Error: Cannot open load file. " << std::endl;
			exit(EXIT_FAILURE);
		}
		phSdk = NULL;
		grSdk = NULL;
		for(unsigned  i=0; i<objs.size(); ++i){	
			if(!phSdk) phSdk = ICAST(PHSdkIf, objs[i]);	//	PHSDKを受け取る方式
			if(!grSdk) grSdk = ICAST(GRSdkIf, objs[i]);	//	GRSdkも受け取る
		}
	}
	if (phSdk && phSdk->NScene()){
		ObjectIfs objs;
//		objs.Push(phSdk->GetScenes()[0]);
		objs.Push(phSdk);
		if(grSdk) objs.Push(grSdk);
		fileX->Save(objs, "out.x");
	}
	fiSdk->Clear();	//	ファイルローダのメモリを解放．
	objs.clear();
	phSdk->Print(DSTR);

	scene = phSdk->GetScenes();		// Sceneの取得
	shape = phSdk->GetShapes();		// Shapeの取得
	DSTR << "Loaded : " << "NScene=" << phSdk->NScene() << ", NShape=" << phSdk->NShape() << std::endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	int window = glutCreateWindow("FILoad");

	// Graphics Sdk
	grSdk = CreateGRSdk();
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
	glutMainLoop();		// これ追加すると大量のmemory leakが発生する

	
	return 0;
}
