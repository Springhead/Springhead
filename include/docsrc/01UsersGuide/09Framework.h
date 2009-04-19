/** \page pageFramework フレームワークSDK 


\contents

\section sec_FWSdkAbst フレームワークSDKの概要

フレームワーク\c SDK は，\ref pagePhysics \ref pageGraphics \ref pageFileIO
を統合し，アプリケーションとして動作させるためのフレームワークを提供するSDKです．

他のSDKとの関係を図にすると次のようになります．
\image html FWScene.png

FWObject は，グラフィクスを物理に同期，つまり GRFrameの位置姿勢をPHSolidの位置姿勢に同期させるためのオブジェクトです．
FWScene は，グラフィクスと物理のシーンを持ってまとめた処理をします．

FWSdkは，FileIOSDKを用いてファイルをロードしたり，セーブしたりもします．
また，レンダラの管理やGLUTなどのコールバックの処理なども行うことができます．

\section sec_FWSdkSample フレームワークSDKを使ったアプリケーション
FWSdkはファイルをロードしてシミュレーションをしながらグラフィクスで表示するというような，
典型的なアプリケーションを作るときに役立ちます．

以下のソースは，ファイル scene.x をロードしてシミュレーションするサンプルプログラムです．
\verbatim
#include <Springhead.h>
#include <GL/glut.h>
using namespace Spr;

UTRef<FWSdkIf> fwSdk;

void Display() {
	fwSdk->Draw();
	glutSwapBuffers();
}

void Timer(int id) {
	fwSdk->Step();	// シミュレーションのステップ
	glutPostRedisplay();
	glutTimerFunc(20, Timer, 0);
}
void Reshape(int w, int h){
	fwSdk->Reshape(w,h);
}
void Keyboard(unsigned char key, int x, int y){
	if (key == 27) exit(0);
}

int main(int argc, char* argv[]) {
	fwSdk = FWSdkIf::CreateSdk();
	fwSdk->LoadScene("scene.x");	//	ファイルをロード

	// OpenGLの初期化
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Springhead with framework");
	fwSdk->CreateRender();

	// GLUTのコールバックの登録
	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutTimerFunc(20, Timer, 0);

	// メインループ開始
	glutMainLoop();
	return 0;
}
\endverbatim

\section sec_FWApp アプリケーションクラス
フレームワークSDKは，ユーザアプリケーションを書くための基本クラスとして使うと便利な
FWApp クラスとその派生クラスも提供しています．

これらは，SpringheadのAPIクラスではなく，通常のC++のクラス定義になっています．
ユーザはこれらを継承して，仮想関数を必要に応じてオーバーライドすることで，
独自のアプリケーションを作ることができます．

【参考】
\ref pageFWAppGLUT
を使ったアプリケーション作成
\ref pageFWAppGLUI
を使ったアプリケーション作成

*/
