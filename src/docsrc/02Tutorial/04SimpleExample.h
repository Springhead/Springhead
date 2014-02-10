/** \page pageSimpleExample	簡単なプログラム

<b>このページは内容が古くなっています（ただ今改訂の準備中です）</b>
<br>

ここでは，簡単なプログラムとして，<b style="color: #3D578C;">PHSimple</b>
に沿って説明します．<br>
2つのティーポットにある一定の力を加え，10ステップ後の位置を出力するサンプルプログラムです．<br>
<b style="color: #3D578C">PHSimple</b> をOpenGLでレンダリングした
サンプルプログラムは，<b style="color: #3D578C;">PHSimpleGL</b> にあります.<br>
<br>

まずはインクルードファイルを定義します．
\verbatim
 #include <Springhead.h>		//	Springheadのインタフェース
\endverbatim
<br>

Springhead2のnamespaceも利用宣言しておきます．
\verbatim
 using namespace Spr;
\endverbatim
<br>

シミュレーションに必要な物理エンジンSDKのインスタンスを作成・取得し，
物体に対する質量・重心・慣性テンソルなどの必要な情報を設定します．
これに関係する内容は\ref pageApiBasic にも記載されています．
\verbatim
 int main(int argc, char* argv[]){
    sdk = CreatePHSdk();                //  物理エンジンSDKの作成
    scene = sdk->CreateScene();         //  シーンの作成
    PHSolidDesc desc;                   //  剛体のディスクリプタ
    desc.mass = 2.0;                    //  質量を2に設定
    desc.inertia *= 2.0;                //  慣性テンソルの対角成分を2倍する（inertiaの初期値は単位行列）
    solid1 = scene->CreateSolid(desc);  //  剛体１をdescに基づいて作成
    desc.center = Vec3f(1,0,0);         //  剛体の重心を(1,0,0)に設定
    solid2 = scene->CreateSolid(desc);  //  剛体２をdescに基づいて作成 	
	    :
\endverbatim 	
 	
初期設定を終えたら，シミュレーションを行います．
ループ処理で，Step()関数を呼び，シミュレーションステップ数を更新していきます．
ここでは，それぞれの剛体の重心から2m上を右方向に10Nの力を加え，Step()関数を呼んでいます．
また，ステップを進めるごとに，剛体の位置情報を出力しています．
\verbatim
    for(int i=0; i<10; ++i){
        solid1->AddForce(Vec3f(10,0,0), Vec3f(0,2,0)+solid1->GetCenterPosition());
        solid2->AddForce(Vec3f(10,0,0), Vec3f(0,2,0)+solid2->GetCenterPosition());
        scene->Step();
        std::cout << solid1->GetFramePosition();
        std::cout << solid2->GetFramePosition() << std::endl;
	}
\endverbatim 

シミュレーション終了時には，物理エンジンSDKの開放を行います．
ただし，物理エンジンSDKは開放しなくても，アプリケーション終了時に開放されるようになっています．
\verbatim
    delete sdk;
\endverbatim  	
 	
以上により，剛体にある一定の力を加えたシミュレーションが行えます．
なお，ここでは最後に，DBG_NEWでchar型のメモリ配列を用意し，VCでのメモリリーク検出テストを行っています．
\verbatim 	
    char* memoryLeak = DBG_NEW char[123];
 	
 	return 0;
 }
\endverbatim 
<br><br><br>

*/
