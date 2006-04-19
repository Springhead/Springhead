/** \page pageMoreTutorial	チュートリアル（詳細編）

<br>

\section contentsMoreTutorial	◇ Contents ◇
	&nbsp;&nbsp;&nbsp; \ref moreTutUTRef <br>
	&nbsp;&nbsp;&nbsp; \ref moreTutVec2 <br>
	&nbsp;&nbsp;&nbsp; \ref moreTutVec3 <br>
	&nbsp;&nbsp;&nbsp; \ref moreTutVec4 <br>
	&nbsp;&nbsp;&nbsp; \ref moreTutOri <br>
	&nbsp;&nbsp;&nbsp; \ref moreTutPose <br>
	&nbsp;&nbsp;&nbsp; \ref moreTutPose <br>
	&nbsp;&nbsp;&nbsp; \ref moreTutForce <br>
<br><br>
<hr>


<!-------------------- 1．　UTRef<T> 参照カウンタと参照ポインタ -------------------->

\subsection moreTutUTRef	1．　UTRef<T> 参照カウンタと参照ポインタ
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
<br><br><br>
<hr>

<!-------------------- 2．　2次元ベクトル ------------------------------------------>
\subsection moreTutVec2		2．　2次元ベクトル
2つの単精度float型データを持つベクトル型はVec2f型、2つの倍精度double型データを持つベクトル型をVec2d型となります。
\verbatim	
    Vec2f vec(a, b);                            // a、b で初期化
    Vec2f vec = Vec2f(a, b);                    // a、b で初期化
    vec[0] = a;                                 // vec.x = a;
    vec[1] = b;                                 // vec.y = b;
    Vec2f vec2= vec + Vec2f(x, y);              // 加算
\endverbatim
<br><br><br>
<hr>

<!-------------------- 3．　3次元ベクトル ------------------------------------------>
\subsection moreTutVec3		3．　3次元ベクトル
3つの単精度float型データを持つベクトル型はVec3f型、3つの倍精度double型データを持つベクトル型をVec3d型となります。
\verbatim	
    Vec3f vec(a, b, c);                         // a、b、c で初期化
    Vec3f vec = Vec3f(a, b, c);                 // a、b、c で初期化
    vec[0] = a;                                 // vec.x = a;
    vec[1] = b;                                 // vec.y = b;
    vec[2] = c;                                 // vec.z = c;
    Vec3f vec2= vec + Vec3f(x, y, z);           // 加算
\endverbatim
<br><br><br>
<hr>

<!-------------------- 4．　4次元ベクトル ------------------------------------------>
\subsection moreTutVec4		4．　4次元ベクトル
4つの単精度float型データを持つベクトル型はVec4f型、4つの倍精度double型データを持つベクトル型をVec4d型となります。
\verbatim	
    Vec4f vec(a, b, c);                         // a、b、c で初期化
    Vec4f vec = Vec4f(a, b, c);                 // a、b、c で初期化
    vec[0] = a;                                 // vec.x = a;
    vec[1] = b;                                 // vec.y = b;
    vec[2] = c;                                 // vec.z = c;
    Vec4f vec2= vec + Vec4f(x, y, z);           // 加算
\endverbatim
<br><br><br>
<hr>

<!-------------------- 5．　剛体の向き --------------------------------------------->
\subsection moreTutOri	5．　剛体の向き
剛体は質点と違い、大きさを持っているので、剛体の向きも考慮しなくてはなりません。<br>
剛体の向きは以下のように設定します。
\verbatim	
    solid->SetOrientation(Quaternionf::Rot(Rad(30), 'z'));      // z軸回りに30度回転
\endverbatim
また、次のようにして、剛体の向きを取得することもできます。
\verbatim	
    Quaterniond ori = solid->solid1-GetOrientation();           // 剛体の向きを取得
\endverbatim
<br><br><br>
<hr>

<!-------------------- 6．　剛体の位置 --------------------------------------------->
\subsection moreTutPose	6．　剛体の位置
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
<br><br><br>
<hr>

<!-------------------- 7．　剛体の姿勢 --------------------------------------------->
\subsection moreTutPose		7．　剛体の姿勢
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
<br><br><br>
<hr>

<!-------------------- 8．　剛体に力を加える --------------------------------------->
\subsection moreTutForce	8．　剛体に力を加える
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
<br><br><br>

<br>
<div align="right">
<a href="#pageTutorial">top of page</a>
</div>

*/
