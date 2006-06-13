/** \page pageFoundation 基本SDKのAPI

\section sec_vectorMatrix	ベクトルクラス，行列クラス
Springhead2には，基本的なベクトル・行列のクラスが用意されています．
これらは，SDK全体で使用しており，ユーザも使用できます．
<br>
より詳細な説明は\ref pageAffine を参照して下さい．
\subsection sec_baseVec2		2次元ベクトル
2つの単精度float型データを持つベクトル型はVec2f型、2つの倍精度double型データを持つベクトル型をVec2d型となります．
\verbatim	
    Vec2f vec(a, b);                            // a、b で初期化
    Vec2f vec = Vec2f(a, b);                    // a、b で初期化
    vec[0] = a;                                 // vec.x = a;
    vec[1] = b;                                 // vec.y = b;
    Vec2f vec2= vec + Vec2f(x, y);              // 加算
\endverbatim
<br>

\subsection sec_baseVec3	3次元ベクトル
3つの単精度float型データを持つベクトル型はVec3f型、3つの倍精度double型データを持つベクトル型をVec3d型となります．
\verbatim	
    Vec3f vec(a, b, c);                         // a、b、c で初期化
    Vec3f vec = Vec3f(a, b, c);                 // a、b、c で初期化
    vec[0] = a;                                 // vec.x = a;
    vec[1] = b;                                 // vec.y = b;
    vec[2] = c;                                 // vec.z = c;
    Vec3f vec2= vec + Vec3f(x, y, z);           // 加算
\endverbatim
<br>

\subsection sec_baseVec4		4次元ベクトル
4つの単精度float型データを持つベクトル型はVec4f型、4つの倍精度double型データを持つベクトル型をVec4d型となります．
\verbatim	
    Vec4f vec(a, b, c);                         // a、b、c で初期化
    Vec4f vec = Vec4f(a, b, c);                 // a、b、c で初期化
    vec[0] = a;                                 // vec.x = a;
    vec[1] = b;                                 // vec.y = b;
    vec[2] = c;                                 // vec.z = c;
    Vec4f vec2= vec + Vec4f(x, y, z);           // 加算
\endverbatim
<hr>

\section sec_utilityType	ユーティリティクラス
Springhead2には，ユーティリティクラスが用意されています．
これらは，SDK全体で使用しており，ユーザも使用できます．

\subsection sec_baseUTRef		参照カウンタと参照ポインタ
メモリリーク対策として、参照カウンタを持つ基底クラスとカウンタを管理するスマートポインタのテンプレートを実装しています．<br>
<a href="../../src/html/classSpr_1_1UTRef.html">UTRef<クラス名></a>が参照ポインタ、
<a href="../../src/html/classSpr_1_1UTRefCount.html">UTRefCount</a>が参照カウンタとなります．<br>
<a href="../../src/html/classSpr_1_1UTRefCount.html">UTRefCount</a>を基本クラスに持つクラスAがある場合，次のように使うことができます．<br>
forループにて10回のメモリ確保をnewで行い、deleteでのメモリ開放を行っていませんが、メモリリークは発生しません．
\verbatim	
    UTRef<A> p;
    for(int i=0; i10; ++i){
        p = new A;
        p->Aのメソッド()
    }
\endverbatim

\section sec_SceneGraph シーングラフ
 一般に，物理シミュレータやバーチャルリアリティのSDKでは，
 バーチャル世界を構成するオブジェクト達のことをシーングラフと呼びます．
 シーングラフは，SceneとかWorldとかUniverseとか呼ばれるシーン全体を管理するオブジェクトと，
 それに所有される，座標系やポリゴンメッシュなどのオブジェクトからなります．
 例えば，Springheadの物理シミュレータのシーングラフは，
 <img src="../../include/docsrc/01UsersGuide/GRSdkScene.png">
 のようなイメージです．
 
 
\section sec_SceneObjectName シーンとオブジェクトと名前
Springheadの \ref pagePhysics \ref pageGraphics では，ひとつのVR世界をシーン(Scene)と呼び，
Sceneクラスを継承したPHSceneクラスやGRSceneクラスが対応します．
シーンの中には，オブジェクト(SceneObjectの派生クラス)を多数作ることができます．
オブジェクトには名前をつけることができ，\ref pageFileIO でロードした場合などは，
名前を指定してオブジェクトへのポインタを取得できます．
また，ファイルに保存するときには，オブジェクトへの参照は名前で保存されます．
これらは \ref pagePhysics や \ref pageGraphics で共通で \ref pageFoundation にあります．


*/
