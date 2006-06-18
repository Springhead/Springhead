/** \page pageFoundation 基本SDK
\ref pageFoundation には，シーンとオブジェクト，行列やベクトル，参照カウンタ
のような基本的なクラスがあります．
他のSDK（\ref pagePhysics や \ref pageGraphics など）は，\ref pageFoundation 
を使って書かれています．
 - \ref secSceneGraph
 - \ref secSceneObjectName
 - \ref secVectorMatrix
 - \ref secUtilityType

\section secSceneGraph シーングラフ
一般に，物理シミュレータやバーチャルリアリティのSDKでは，
バーチャル世界を構成するオブジェクト達のことをシーングラフと呼びます．
シーングラフは，SceneとかWorldとかUniverseとか呼ばれるシーン全体を管理するオブジェクトと，
それに所有される，座標系やポリゴンメッシュなどのオブジェクトからなります．
\image html PHSdkScene.png
上図は，Springhead2の物理シミュレータのシーングラフのオブジェクトと参照関係の一例です．

Springheadの \ref pagePhysics \ref pageGraphics はそれぞれシーングラフを持ちます．
\ref pageFoudation のSceneクラスを継承したPHSceneクラスとGRSceneクラスが対応します．
シーンの中には，オブジェクト(SceneObjectの派生クラス)を複数作って追加することができます．

\section secSceneObjectName シーンとオブジェクトと名前
シーングラフのオブジェクトには名前をつけることができます．
\ref pageFileIO でロードした場合など，ファイルに書いておいた名前を指定して，オブジェクトへのポインタを取得できて便利です．
<pre>
text.x:
------------------------------------------------
PHScene{ ...
	PHSolid soBox1 { ... }
	PHSolid soBox2 { ... }
	PHSolid soFloor { ... }
}
------------------------------------------------
source code:
------------------------------------------------
	phSdk = CreatePHSdk();
	objs.push_back(phSdk);
	fileX->Load(objs, "test.x");
	PHSolid* solid;
	phSdk->FindObject(solid, "soBox2");
------------------------------------------------
</pre>
また，ファイルに保存するときには，オブジェクトへの参照は名前で保存されます．

\section secVectorMatrix	ベクトルクラス，行列クラス
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

\section secUtilityType	ユーティリティクラス
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

*/
