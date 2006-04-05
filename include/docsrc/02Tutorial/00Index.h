/** \page pageTutorial	【チュートリアル】



\section contentsTutorial	◇ Contents ◇
 &nbsp;&nbsp;&nbsp; \ref tutStarting <br>
 &nbsp;&nbsp;&nbsp; \ref basicWord <br>
 &nbsp;&nbsp;&nbsp; \ref librarylist <br> 
 &nbsp;&nbsp;&nbsp; \ref simpleExample <br>
 &nbsp;&nbsp;&nbsp; \ref moreTutorial <br> 
<br><br>
<hr>



<!--------- 1．はじめに ------------------------------------------------------------------------>
\section tutStarting	1．はじめに
まず最初にすることは、Springead2のソースをダウンロードし、インストールすることです。<br>
インストール方法については、\ref pageUsersGuide の \ref pageGettingStarted を参照して下さい。
<br><br><br><br>
<div align="right">
<a href="#pageTutorial">top of page</a>
</div>
<hr>



<!---------------------------------------- 2．基本的な用語 ---------------------------------------->
\section basicWord	2．基本的な用語
Springhead2を始める前に、基本的な用語を説明しておきます。<br>
より詳細な説明は\ref pageUsersGuideまたは\ref pageReferenceManual を参照して下さい。
<ul type="circle">
	<li> 剛体 <br>
		<dl>
			<dt></dt>
				<dd> 外から力を加えたときに変形が無視できる硬い物体です。</dd>
		</dl>
	</li>	
	<li> ステップ幅 <br>
		<dl>
			<dt></dt>
				<dd> ステップ幅はシミュレーションをどのくらい細かく行うかを示す値で、
			 		 小さければ小さいほど精度が良いが、計算量も増加するという関係があります。
			 		 ステップ幅を大きく取った場合は単に精度が悪くなるだけではなく、場合によっては
			 		 シミュレーション自体の破綻を招きます。</dd> 
		</dl>
	</li>
	<li> シーングラフ <br>
		<dl>
			<dt></dt>
				<dd> シーングラフは複数の3D物体モデルから構成されます。3D物体モデルは、形状、質量、
			 		 慣性テンソル、重心、位置、姿勢、速度などから構成されます。</dd> 
		</dl>
	</li>	
	<li> 衝突判定 <br>
		<dl>
			<dt></dt>
				<dd> シーングラフから渡された情報を元に、物体同士の衝突の有無を判定し、
			 		 その結果を物理エンジンに渡します。</dd> 
		</dl>
	</li>
	<li> 物理エンジン <br>
		<dl>
			<dt></dt>
				<dd> 衝突判定の結果を元に、物体に加わる力を求めます。また、物体の位置、速度、力から、
			 		 次のステップでの位置、速度を求め、シーングラフに渡し更新します。</dd> 
		</dl>
	</li>	
	<li>参照カウンタ<br>
		<dl>
			<dt></dt>
				<dd> C++では，メモリを new で確保して delete で削除します。
					 new してない領域を指すポインタを delete したり，delete を忘れると
					 一般保護違反やメモリリークという問題が起き，特にメモリリークをなくすのはとても面倒です。
					 そこで，C++に便利なポインタを導入するのが参照カウンタテンプレートクラスです。
					 参照カウンタとは，ポインタに指されるオブジェクトに，参照しているポインタ の数を
					 書いておき，0になったら自動的に削除する仕組みです。
					 参照ポインタ(<a href="../../src/html/classSpr_1_1UTRef.html">Spr::UTRef</a>)は，
					 参照するときにカウンタ値を増やし， 参照しなくなるときにカウンタ値を減らし，
					 カウンタが0だったら delete します。
					 参照カウンタは<a href="../../src/html/classSpr_1_1UTRefCount.html">UTRefCount</a>基本クラスに定義されています。
					 参照ポインタは<a href="../../src/html/classSpr_1_1UTRefCount.html">UTRefCount</a>クラスを継承したクラスにだけ使えます。</dd> 
		</dl>
	</li>		
	
</ul>
<br>
<div align="right">
<a href="#pageTutorial">top of page</a>
</div>
<hr>



<!---------------------------------------- 3．ライブラリ ---------------------------------------->
\section librarylist	3．ライブラリ

各ライブラリは、作成したプログラムをビルドする時にリンクすることで利用可能となります（\ref directory参照）。
<ul type="circle">
	<li> Baseライブラリ <br>
			<dl>
				<dt></dt>
					<dd>基本ユーティリティとして、Baseライブラリは、アフィン行列、ベクトル、
						デバッグ支援機能、コンパイラマクロ、参照カウンタ、
						その他ユーティリティから構成されます。</dd>
			</dl>
	</li>
	<li> Collisionライブラリ <br>
			<dl>
				<dt></dt>
					<dd>衝突判定エンジンとして、衝突検出を行います。物体同士の相互作用に不可欠です。</dd>
			</dl>
	</li>
	<li> FileIOライブラリ <br>
			<dl>
				<dt></dt>
					<dd>3D形状データ、マテリアル、テクスチャなどを定義したXファイルの
						データ情報をロードします。</dd>
			</dl>
	</li>
	<li> Foundationライブラリ <br>
			<dl>
				<dt></dt>
					<dd>シーンとオブジェクトの構築を行います。</dd>
			</dl>
	</li>
	<li> Graphicsライブラリ <br>
			<dl>
				<dt></dt>
					<dd> 補助ライブラリとしてGraphicsライブラリを用意しています。
						 レンダリングのデバイスはOpenGLまたはDirectXで接続切換え可能です。
						 ただし、今の段階ではDirectXは未実装となっており、OpenGLデバイスのみ利用可能です。</dd>
			</dl>
	</li>
	<li> Physicsライブラリ <br>
			<dl>
				<dt></dt>
					<dd>Springhead2で核となる3D物理シミュレーションエンジンです。</dd>
			</dl>
	</li>
</ul>	
<br>
<div align="right">
<a href="#pageTutorial">top of page</a>
</div>
<hr>
	


<!---------------------------------------- 4．簡単なプログラム ---------------------------------------->
\section simpleExample	4．簡単なプログラム
ここでは、簡単なプログラムとして、<a href="../../src/html/PHSimple_2main_8cpp-example.html">PHSimple</a>
に沿って説明します。<br>
2つのティーポットにある一定の力を加え、10ステップ後の位置を出力するサンプルプログラムです。<br>
<a href="../../src/html/PHSimple_2main_8cpp-example.html">PHSimple</a>をOpenGLでレンダリングした
サンプルプログラムは、<a href="../../src/html/PHSimpleGL_2main_8cpp-example.html">PHSimpleGL</a>にあります。<br>
<br>

まずはインクルードファイルを定義します。
\verbatim
 #include <Springhead.h>		//	Springheadのインタフェース
\endverbatim
<br>

Springhead2のnamespaceも利用宣言しておきます。
\verbatim
 using namespace Spr;
\endverbatim
<br>

シミュレーションに必要な物理エンジンSDKのインスタンスを作成・取得し、
物体に対する質量・重心・慣性テンソルなどの必要な情報を設定します。
これに関係する内容は\ref pageApiBasicにも記載されています。
\verbatim
 int main(int argc, char* argv[]){
    sdk = CreatePHSdk();                //  物理エンジンSDKの作成
    scene = sdk->CreateScene();         //  シーンの作成
    PHSolidDesc desc;                   //  剛体のディスクリプタ
    desc.mass = 2.0;                    //  質量を2に設定
    desc.inertia *= 2.0;                //  慣性テンソルの体格成分を2倍する（inertiaの初期値は単位行列）
    solid1 = scene->CreateSolid(desc);  //  剛体１をdescに基づいて作成
    desc.center = Vec3f(1,0,0);         //  剛体の重心を(1,0,0)に設定
    solid2 = scene->CreateSolid(desc);  //  剛体２をdescに基づいて作成 	
	    :
\endverbatim 	
 	
初期設定を終えたら、シミュレーションを行います。
ループ処理で、Step()関数を呼び、シミュレーションステップ数を更新していきます。
ここでは、それぞれの剛体の重心から2m上を右方向に10Nの力を加え、Step()関数を呼んでいます。
また、ステップを進めるごとに、剛体の位置情報を出力しています。
\verbatim
    for(int i=0; i<10; ++i){
        solid1->AddForce(Vec3f(10,0,0), Vec3f(0,2,0)+solid1->GetCenterPosition());
        solid2->AddForce(Vec3f(10,0,0), Vec3f(0,2,0)+solid2->GetCenterPosition());
        scene->Step();
        std::cout << solid1->GetFramePosition();
        std::cout << solid2->GetFramePosition() << std::endl;
	}
\endverbatim 

シミュレーション終了時には、物理エンジンSDKの開放を行います。
ただし、物理エンジンSDKは開放しなくても、アプリケーション終了時に開放されるようになっています。
\verbatim
    delete sdk;
\endverbatim  	
 	
以上により、剛体にある一定の力を加えたシミュレーションが行えます。
なお、ここでは最後に、DBG_NEWでchar型のメモリ配列を用意し、VCでのメモリリーク検出テストを行っています。
\verbatim 	
    char* memoryLeak = DBG_NEW char[123];
 	
 	return 0;
 }
\endverbatim 
<br><br><br>
<div align="right">
<a href="#pageTutorial">top of page</a>
</div>
<hr>



<!---------------------------------------- 5．その他の詳細 ---------------------------------------->
\section moreTutorial	5．その他の詳細
ここでは、その他の基本的なSpringhead2のテクニックをもとにした、一般的な使用方法を簡単に説明します。
<br>

<!-------------------- 5.1. UTRef<T> 参照カウンタと参照ポインタ  -------------------->
\subsection moreTutUTRef	5.1. UTRef<T> 参照カウンタと参照ポインタ
メモリリーク対策として、参照カウンタを持つ基底クラスとカウンタを管理するスマートポインタのテンプレートを実装しています。<br>
<a href="../../src/html/classSpr_1_1UTRef.html">UTRef<クラス名></a>が参照ポインタ、
<a href="../../src/html/classSpr_1_1UTRefCount.html">UTRefCount</a>が参照カウンタとなります。<br>
<a href="../../src/html/classSpr_1_1UTRefCount.html">UTRefCount</a>を基本クラスに持つクラスAがある場合，次のように使うことができます。<br>
forループにて10回のメモリ確保をnewで行い、deleteでのメモリ開放を行っていませんが、メモリリークは発生しません。
\verbatim	
    UTRef<A> p;
    for(int i=0; i10; ++i){
        p = new A;
        p->Aのメソッド()
    }
\endverbatim
<br><br>

<!-------------------- 5.2. 2次元ベクトル  ------------------------------------------>
\subsection moreTutVec2		5.2. 2次元ベクトル
2つの単精度float型データを持つベクトル型はVec2f型、2つの倍精度double型データを持つベクトル型をVec2d型となります。
\verbatim	
    Vec2f vec(a, b);                            // a、b で初期化
    Vec2f vec = Vec2f(a, b);                    // a、b で初期化
    vec[0] = a;                                 // vec.x = a;
    vec[1] = b;                                 // vec.y = b;
    Vec2f vec2= vec + Vec2f(x, y);              // 加算
\endverbatim
<br><br>

<!-------------------- 5.3. 3次元ベクトル  ------------------------------------------>
\subsection moreTutVec3		5.3. 3次元ベクトル
3つの単精度float型データを持つベクトル型はVec3f型、3つの倍精度double型データを持つベクトル型をVec3d型となります。
\verbatim	
    Vec3f vec(a, b, c);                         // a、b、c で初期化
    Vec3f vec = Vec3f(a, b, c);                 // a、b、c で初期化
    vec[0] = a;                                 // vec.x = a;
    vec[1] = b;                                 // vec.y = b;
    vec[2] = c;                                 // vec.z = c;
    Vec3f vec2= vec + Vec3f(x, y, z);           // 加算
\endverbatim
<br><br>

<!-------------------- 5.4. 4次元ベクトル  ------------------------------------------>
\subsection moreTutVec4		5.4. 4次元ベクトル
4つの単精度float型データを持つベクトル型はVec4f型、4つの倍精度double型データを持つベクトル型をVec4d型となります。
\verbatim	
    Vec4f vec(a, b, c);                         // a、b、c で初期化
    Vec4f vec = Vec4f(a, b, c);                 // a、b、c で初期化
    vec[0] = a;                                 // vec.x = a;
    vec[1] = b;                                 // vec.y = b;
    vec[2] = c;                                 // vec.z = c;
    Vec4f vec2= vec + Vec4f(x, y, z);           // 加算
\endverbatim
<br><br>

<!-------------------- 5.5. 剛体の向き ---------------------------------------------->
\subsection moreTutOri	5.5. 剛体の向き
剛体は質点と違い、大きさを持っているので、剛体の向きも考慮しなくてはなりません。<br>
剛体の向きは以下のように設定します。
\verbatim	
    solid->SetOrientation(Quaternionf::Rot(Rad(30), 'z'));      // z軸回りに30度回転
\endverbatim
また、次のようにして、剛体の向きを取得することもできます。
\verbatim	
    Quaterniond ori = solid->solid1-GetOrientation();           // 剛体の向きを取得
\endverbatim
<br><br>

<!-------------------- 5.6. 剛体の位置 -------------------->
\subsection moreTutPose	5.6. 剛体の位置
通常、シミュレーションを行うには、その物体がどこにあるかという情報が必要になります。
その情報というのが、座標です。座標により、物体が空間のどの位置にいるかを示すことができます。
剛体の位置は以下のように設定します。
\verbatim	
    solid->SetFramePosition(Vec3f(0,-3,0));                     // 剛体の位置を(0,-3,0)に設定
\endverbatim
また、次のようにして、剛体の位置を取得することもできます。
\verbatim	
    Vec3d framepos = solid->solid1->>GetFramePosition();        // 剛体の位置を取得
\endverbatim
<br><br>
 
<!-------------------- 5.7. 剛体の姿勢 -------------------->
\subsection moreTutPose		5.7. 剛体の姿勢
剛体の姿勢は、Posed型で表されます。 <br>
Posed型は、Affineと同じ機能を持つ7次元ベクトルで、向きと位置を合わせたベクトルとなっています。
剛体の姿勢は以下のようにして設定します。
\verbatim
    Posed p = Posed::Rot(Rad(30.0), 'z');                       // z軸回りに30度回転させたPosed型pを用意する
    solid->SetPose(p);                                          // Posed型pを剛体の姿勢として設定する
\endverbatim
また、次のようにして、剛体の姿勢を取得することもできます。
\verbatim
    Posed pose = soBlock->GetPose();                            // 剛体の姿勢を取得
\endverbatim
<br><br>

<!-------------------- 5.8. 剛体に力を加える -------------------->
\subsection moreTutForce	5.8. 剛体に力を加える
剛体に力が作用する場合を考え、質量中心に力を加える場合、以下のように設定することができます。
<img src="../../include/docsrc/02Tutorial/force.jpg" width="682" height="146">
\verbatim
    solid->AddForce(Vec3f(10, 0, 0);    // 質量中心に右方向へ10Nの力を加える
\endverbatim
また、質量中心ではなく、位置を指定して、力を加えることも可能です。
\verbatim    
    solid->AddForce(f, r);              　　　　　　　　　　　　　　　　            // 力を 位置r(World系) に加える
    solid->AddForce(Vec3f(10,0,0), Vec3f(0,2,0)+solid1->GetCenterPosition());       // 重心の2m上を右方向に10Nの力を加える
\endverbatim
<br><br>
 	 

    	
<div align="right">
<a href="#pageTutorial">top of page</a>
</div>    	
*/
