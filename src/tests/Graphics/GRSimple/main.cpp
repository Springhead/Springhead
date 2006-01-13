/** 
 @file  Springhead2/src/tests/Graphics/GRSimple/main.cpp

@brief 描画のテスト

 <PRE>
 <B>概要：</B>
  ・剛体の運動を確認する。
  ・位置を出力し、レンダリングは行わない。
  
 <B>終了基準：</B>
  ・プログラムが正常終了したら0を返す。
 
 <B>処理の流れ：</B>
  ・シミュレーションに必要な情報(質量・重心・慣性テンソルなど)を設定する。
  ・2つの剛体を用意し、各剛体に対して重心の2m上を右に10Nの力を加える。
  ・与えられた条件により⊿t秒後の位置の変化を10ステップ積分し、その都度位置を出力する。
  ・10ステップ分の計算を終えたら、プログラムを終了させる。
 </PRE>
    
 */
#include <Springhead.h>		//	Springheadのインタフェース
#pragma hdrstop
#include <gl/glut.h>
using namespace Spr;

GRSdkIf* grSdk;
GRDebugRenderIf* render;
GRDeviceGLIf* grDevice;

PHSdkIf* phSdk;
PHSceneIf* scene;
PHSolidIf* soFloor, *soBlock;
/**
 @brief     glutDisplayFuncで指定したコールバック関数
 @param	 	なし
 @return 	なし
 */
void display(){
}

/**
 @brief		glutReshapeFuncで指定するコールバック関数
 @param		<in/--> w　　幅
 @param		<in/--> h　　高さ
 @return	なし
 */
void reshape(int w, int h){
	render->Resize(Vec2f(w,h));
}

/**
 @brief 	glutKeyboardFuncで指定したコールバック関数 
 @param		<in/--> key　　 ASCIIコード
 @param 	<in/--> x　　　 キーが押された時のマウス座標
 @param 	<in/--> y　　　 キーが押された時のマウス座標
 @return 	なし
 */
void keyboard(unsigned char key, int x, int y){
	if (key == '\x1b') exit(0);
}	

/**
 @brief  	glutIdleFuncで指定したコールバック関数
 @param	 	なし
 @return 	なし
 */
void idle(){
	glutPostRedisplay();
	static int count;
	count ++;
	if (count > 200) exit(0);
}

int main(int argc, char* argv[]){
	phSdk = CreatePHSdk();						// SDKの作成　
	scene = phSdk->CreateScene();				// シーンの作成
	PHSolidDesc desc;
	desc.mass = 2.0;
	desc.inertia *= 2.0;
	soBlock = scene->CreateSolid(desc);			// 剛体をdescに基づいて作成

	Posed p = Posed::Rot(Rad(0.0), 'z');
	soBlock->SetPose(p);

	desc.mass = 1e20f;
	desc.inertia *= 1e20f;
	soFloor = scene->CreateSolid(desc);		// 剛体をdescに基づいて作成
	soFloor->SetGravity(false);
	
	//	形状の作成
	CDConvexMeshIf* meshFloor=NULL;
	CDConvexMeshIf* meshBlock=NULL;
	{
		CDConvexMeshDesc md;
		md.vertices.push_back(Vec3f(-1,-1,-1)); md.vertices.push_back(Vec3f(-1,-1, 1));	
		md.vertices.push_back(Vec3f(-1, 1,-1));	md.vertices.push_back(Vec3f(-1, 1, 1));
		md.vertices.push_back(Vec3f( 1,-1,-1));	md.vertices.push_back(Vec3f( 1,-1, 1));
		md.vertices.push_back(Vec3f( 1, 1,-1));	md.vertices.push_back(Vec3f( 1, 1, 1));
		meshBlock = ICAST(CDConvexMeshIf, scene->CreateShape(md));

		// soFloor(meshFloor)に対してスケーリング
		for(unsigned i=0; i<md.vertices.size(); ++i){
			md.vertices[i].x *= 3;
			md.vertices[i].z *= 3;
		}
		meshFloor = ICAST(CDConvexMeshIf, scene->CreateShape(md));
	}
	soFloor->AddShape(meshFloor);
	soBlock->AddShape(meshBlock);
	soFloor->SetFramePosition(Vec3f(0,-1,0));
	soBlock->SetFramePosition(Vec3f(-0.5,5,0));
	soBlock->SetOrientation(Quaternionf::Rot(Rad(30), 'z'));

	scene->SetGravity(Vec3f(0,-9.8f, 0));	// 重力を設定


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	int window = glutCreateWindow("GRSimple");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);

	grSdk = CreateGRSdk();
	render = grSdk->CreateDebugRender();
	grDevice = grSdk->CreateDeviceGL(window);
	render->SetDevice(grDevice);

	glutMainLoop();

	//	SDKは開放しなくて良い．しなくてもmainを抜けてから開放される．
}
