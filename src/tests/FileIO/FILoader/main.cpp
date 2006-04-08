/**
 Springhead2/src/tests/FileIO/FILoader/main.cpp

【概要】
  Xファイルをロードし、Physicsエンジンと接続してシミュレーションする。
  
【終了基準】
  ・プログラムが正常終了したら0を返す。  
 
【処理の流れ】
  ・test.xファイルをロードする。
  ・ロードした情報を出力する。
  ・Physicsエンジンと接続し、シミュレーションさせる。
  
 */
#include <FileIO/FITypeDesc.h>
#include <FileIO/FIFileContext.h>
#include <FileIO/FIFileX.h>
#include <Springhead.h>
#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif
#define	ESC		27
namespace Spr{
	PHSdkIf* phSdk;
	PHSceneIf** scene;
	CDShapeIf** shape;
	GRSdkIf* grSdk;
	GRDeviceGLIf* grDevice;
	GRDebugRenderIf* render;
}
using namespace Spr;


// 材質の設定
GRMaterial matFloor(Vec4f(0.0, 0.0, 0.0, 1.0),		// ambient
					Vec4f(0.9, 0.0, 0.0, 1.0),		// diffuse
					Vec4f(1.0, 1.0, 1.0, 1.0),		// specular
					Vec4f(0.0, 0.0, 0.0, 1.0),		// emissive
					20.0);							// power

GRMaterial matBox(Vec4f(0.0, 0.0, 0.0, 1.0),		// ambient
					Vec4f(0.0, 0.2, 0.8, 1.0),		// diffuse
					Vec4f(1.0, 1.0, 1.0, 1.0),		// specular
					Vec4f(0.0, 0.0, 0.0, 1.0),		// emissive
					20.0);							// power

/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void display(){
	//	バッファクリア
	render->ClearBuffer();
	render->SetMaterial(matFloor);	

	PHSceneIf* scene = NULL;
	if (phSdk->NScene()){
		PHSceneIf** s = phSdk->GetScenes();
		scene = *s;
	}
	PHSolidIf **solids = scene->GetSolids();
	for (int num=0; num < scene->NSolids(); ++num){
		if (num == 1){
			render->SetMaterial(matBox);	
		}
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
			Vec3f* base = mesh->GetVertices();
			for (size_t f=0; f<mesh->NFace(); ++f) {	
				CDFaceIf* face = mesh->GetFace(f);
				render->DrawFace(face, base);
			}
			render->PopModelMatrix();
		}
		render->PopModelMatrix();
	}
/*

	render->SetMaterial(material);	
	for (int num=0; num < phSdk->NShape(); ++num){
		CDShapeIf **shapes = phSdk->GetShapes();
		CDConvexMeshIf* mesh = ICAST(CDConvexMeshIf, shapes[num]);
		Vec3f* base = mesh->GetVertices();
		for (size_t f=0; f<mesh->NFace(); ++f) {	
			CDFaceIf* face = mesh->GetFace(f);
			render->DrawFace(face, base);
		}
	}
*/
	render->EndScene();
}
/**
 brief     	光源の設定
 param	 	なし
 return 	なし
 */
void setLight() {
	GRLight light0;
	light0.ambient	= Vec4f(0.0, 0.0, 0.0, 1.0);
	light0.diffuse	= Vec4f(0.9, 0.9, 0.9, 1.0);
	light0.specular	= Vec4f(1.0, 1.0, 1.0, 1.0);
	light0.position = Vec4f(2.0, 0.0, 3.0, 1.0);
	render->PushLight(light0);

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
	if (key == ESC) exit(0);
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
	if (count > 5000) exit(0);
}

/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */

int main(int argc, char* argv[]){
	static FIFileContext fc;
	fc.errorStream=&DSTR;
	fc.fileInfo.Push();
	fc.fileInfo.back().Map("test.x");
	phSdk = CreatePHSdk();
	fc.objects.Push(phSdk);
	static FIFileX fileX;	//<	glutは exitで終わるため，staticにしないとこの変数のCleanupが行われなくなる．
	fileX.Init();
	fileX.Load(&fc);
	phSdk->Print(DSTR);

	scene = phSdk->GetScenes();		// Sceneの取得
	shape = phSdk->GetShapes();		// Shapeの取得
	DSTR << "Loaded : " << "NScene=" << phSdk->NScene() << ", NSphape=" << phSdk->NShape() << std::endl;

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
	view.Pos() = Vec3f(2.0, 2.0, 3.0);								// eye
		view.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));		// center, up 
	view = view.inv();	
	render->SetViewMatrix(view);

	// 光源設定
	setLight();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();		// これ追加すると大量のmemory leakが発生する

	return 0;
}

