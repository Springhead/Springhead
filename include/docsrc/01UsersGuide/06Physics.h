/** \page pagePhysics 物理シミュレーションSDK


\contents 

<!---------------------------------- 物理シミュレーションSDKの初期化と終了 ------------------------------------------>
\section sec_PHSdk 		物理シミュレーションSDKの初期化と終了
物理シミュレーション\c SDK は，物理法則に基づいた複数物体のインタラクションを計算する機能を提供します．<br>
物理シミュレーション\c SDK の使用は，グローバル関数 \link Spr::CreatePHSdk() CreatePHSdk \endlink を呼ぶことから始まります．
\verbatim
    PHSdkIf* sdk = CreatePHSdk();
\endverbatim
以降，\c SDK の操作は \link Spr::CreatePHSdk() CreatePHSdk\endlink によって返された \c PHSdkIf 型のポインタを介して行います．

\c SDK は，1 つ以上のシーンを持つことができます．
また，1 つのシーンは 1 つ以上の剛体 <tt>(PHSolid)</tt> と関節 <tt>(PHJoint)</tt>を持つことができます．
さらに，剛体は形状 <tt>(CDConvexMesh，CDSphere，CDBox，...)</tt> を参照して形を持つことができます．
関節は \c 2 つの剛体を結び付けます．
物理エンジン内のオブジェクトの参照関係の例を図に示します．


	\image html PHSdkScene.png

\c SDK の終了処理は内部で自動的に行われます．ユーザから明示的に \c SDK を解放する必要はありません．
<br><br>
<div align="right">
<a href="#pagePhysics">top of page</a>
</div> 
<hr>




<!---------------------------------- シーン ------------------------------------------------------------------------->
\section scene 		シーン
シーンとは物理シミュレーションの舞台となる空間のことを言います．
シーンを作成することで，オブジェクトを投入することが可能となります．

\subsection sceneDesc		シーンデスクリプタ
シーンデスクリプタから，シーンを作成することができます．
以下に示すような設定を初期設定のまま利用する場合は，シーンデスクリプタを用意する必要はありません．
デスクリプタについては，\ref pageApiBasic も参照下さい．
また，シーンデスクリプタのサンプルとしては，
<a href="../../src/html/_2Samples_2BoxStack_2main_8cpp-example.html">BoxStack</a> を参照下さい．

シーンデスクリプタの初期設定では，ステップ実行時間は <tt>0.005[s]</tt> に設定されていますが，以下のように設定することもできます．
\verbatim
    dscene.timeStep = 0.01;
\endverbatim
<br><br>


\subsection scene_create	シーンの作成
シーンを作成するには \link Spr::PHSdkIf::CreateScene() PHSdkIf::CreateScene \endlink を呼びます．
デフォルトの設定でシーンを作成するには，以下のようにします．
\verbatim
    //デフォルトの設定でシーンを作成
    PHSceneIf* scene = sdk->CreateScene();
\endverbatim

また，デフォルトのシーンデスクリプタを割り当てることでも，デフォルト設定におけるシーンの作成が行えます．
\verbatim
    //デフォルトの設定でシーンを作成
    PHSceneDesc sceneDesc;
    PHSceneIf* scene = sdk->CreateScene(sceneDesc);
\endverbatim

同様に， \link Spr::PHSdkIf::CreateScene() PHSdkIf::CreateScene \endlink に対して，
任意のシーンデスクリプタを与えれば，そのシーンデスクリプタに応じたシーンを作成することができます．
シーンデスクリプタについては，\ref sceneDesc を参照下さい．
\verbatim
    PHSceneDesc sceneDesc;
    dscene.timeStep      = 0.05;                                // ステップ幅を 0.05[s] に設定
    PHSceneIf* scene     = sdk->CreateScene(sceneDesc);
\endverbatim

なお，シーンの操作は \link Spr::PHSdkIf::CreateScene() PHSdkIf::CreateScene \endlink によって返された
 \c PHSceneIf 型のポインタを介して行ないます．<br>
シーンを複数作成することは可能ですが，異なるシーン同士は互いに影響を及ぼしません．
<br><br><br>


\subsection scene_step 		シミュレーションの実行
シーンの時刻を進めるには \link Spr::PHSceneIf::Step() PHSceneIf::Step \endlink を呼びます．
\verbatim
    //0.05 * 100 = 5秒間のシミュレーション
    scene->SetTimeStep(0.05);
    for(int i = 0; i < 100; i++)  scene->Step();
\endverbatim

\link Spr::PHSceneIf::Step() Step\endlink の流れは大体以下のようになっています．<br>
　　 \c 1．剛体同士の衝突判定  			<br>
　　 \c 2．接触力，関節の拘束力の計算	<br>
　　 \c 3．剛体の速度・位置の更新		<br>

一度の \link Spr::PHSceneIf::Step() Step \endlink の呼び出しで進む時間の幅は
 \link Spr::PHSceneIf::GetTimeStep() PHSceneIf::GetTimeStep \endlink で取得，
 \link Spr::PHSceneIf::SetTimeStep() PHSceneIf::SetTimeStep \endlink で設定
することができますが，ここで設定する値はそのまま数値積分の積分ステップとなりますので，
\b あまり大きな値（例えば１秒）は設定しないで下さい．<br>
また，\ref paseBasicWord の「ステップ幅」も参照下さい．
<br><br><br>


\subsection scene_gravity 		重力の設定
シーンに働く重力を設定するには \link Spr::PHSceneIf::SetGravity() PHSceneIf::SetGravity \endlink を呼びます．
\verbatim
    scene->SetGravity(Vec3f(0.0f, -9.8f, 0.0f));
\endverbatim

デフォルトでは，シーンの重力加速度は <tt>Vec3f(0.0f, -9.8f, 0.0f)</tt> に設定されています．<br>
無重力空間とするには，以下のようにします．
\verbatim
    scene->SetGravity(Vec3f(0.0f, 0.0f, 0.0f));
\endverbatim

また，個々の剛体に対して重力の作用を有効・無効化するには
 \link Spr::PHSolidIf::SetGravity() PHSolidIf::SetGravity \endlink を使います．
\verbatim
    //solidはsceneに登録されている剛体とする
    solid->SetGravity(false);
\endverbatim
<br><br>
<div align="right">
<a href="#pagePhysics">top of page</a>
</div> 
<hr>




<!---------------------------------- 剛体 --------------------------------------------------------------------------->
\section solid	剛体
剛体とは，物理法則に従ってシーンの中を運動する物体で，特に，外から力を加えたときに変形が無視できる硬い物体を言います．
剛体に対しては，重心，姿勢，向きなどを設定することができます．

\subsection solidDesc	剛体デスクリプタ
剛体デスクリプタから剛体を作成することができます．<br>
以下に示すような設定を初期値のまま利用する場合は，剛体デスクリプタを用意する必要はありません．
ここでは，質量，慣性テンソル，質量中心の位置，重力の影響の有無を設定することができます．<br>
これらの設定は，剛体（\link Spr::PHSceneIf::CreateSolid() PHSceneIf::CreateSolid \endlink から作られる
剛体オブジェクト）からでも設定可能です．剛体からの設定については，\ref solid_inertia_parameter を参照下さい．
また，デスクリプタについては，\ref pageApiBasic を参照下さい．

質量は，初期値として \c 1.0 が設定されています．
剛体デスクリプタで，剛体に対する質量を設定するには，以下のようにします．
\verbatim
    PHSolidDesc desc;
    desc.mass = 2.0;                            // 質量 2.0 に設定
\endverbatim

慣性テンソルは，初期値として単位行列に設定されています．
剛体デスクリプタで，剛体に対する慣性テンソルを設定するには，以下のようにします．
\verbatim
    desc.inertia = 2.0 * Matrix3d::Unit();      // 対角成分が 2.0 の行列に設定
\endverbatim

剛体座標系（ローカル座標系）における質量中心の位置は，初期設定では，剛体座標系の原点と一致しています．
剛体デスクリプタで，この位置を設定するには，以下のようにします．
\verbatim
    desc.center = Vec3d(0.0, 1.0, 0.0);         // 上に 1.0 ずらす
\endverbatim

初期設定では，個々の剛体に対し，重力の影響を受けるように設定されています．
剛体デスクリプタで，剛体に対する重力の影響の有無を設定するには，以下のようにします．
\verbatim
    desc.gravity = false;                       // 重力の影響を受けないように設定
\endverbatim
なお，剛体からの重力設定については，\ref scene_gravity を参照して下さい．
<br><br><br>


\subsection solid_create 	剛体の作成
剛体を作成し，シーンに追加するには \link Spr::PHSceneIf::CreateSolid() PHSceneIf::CreateSolid \endlink を呼びます．
デフォルトの設定で，剛体を作成するには，以下のようにします．
\verbatim
    PHSolidIf* solid;
    solid = scene->CreateSolid();
\endverbatim

また，デフォルトの剛体デスクリプタを割り当てることでも，デフォルト設定における剛体の作成が行えます．
\verbatim
    //デフォルトの設定で剛体を作成
    PHSolidDesc solidDesc;
    PHSolidIf* solid = scene->CreateSolid(solidDesc);
\endverbatim

同様に，\link Spr::PHSceneIf::CreateSolid() PHSceneIf::CreateSolid \endlink に対して，
任意の剛体デスクリプタを与えれば，その剛体デスクリプタに応じた剛体を作成することができます．
剛体デスクリプタについては，\ref solidDesc を参照下さい．
\verbatim
    PHSolidDesc solidDesc;
    solidDesc.mass    = 0.1;                                   // 質量 0.1 に設定
    solidDesc.inertia = 0.1 * Matrix3d::Unit();                // 対角成分が 0.1 の行列に設定
    solidDesc.center  = Vec3d(0.1, 0.0, 0.0);                  // 重心を右に 0.1 ずらす
    PHSolidIf* solid  = scene->CreateSolid(solidDesc);
\endverbatim

なお，剛体に対する操作は \link Spr::PHSceneIf::CreateSolid() PHSceneIf::CreateSolid \endlink によって返された
 \c PHSolidIf 型のポインタを介して行います．
<br><br><br>


\subsection solid_pose 剛体の位置
通常，シミュレーションを行うには，その物体がどこにあるかという情報が必要になります．
その情報というのが，位置です．位置により，物体が空間のどの位置にいるかを示すことができます．
1 つの剛体には，1 つの座標系が貼り付いています．これを剛体座標系と呼びます．
シーン中の剛体の位置と傾きは，シーン座標系に対する剛体座標系の位置と傾きとして表現されます．

	\image html scene.png

剛体の位置を設定・取得するには， 
\link Spr::PHSolidIf::SetFramePosition() PHSolidIf::SetFramePosition \endlink と 
\link Spr::PHSolidIf::GetFramePosition() PHSolidIf::GetFramePosition \endlink 
を呼びます．
\verbatim	
    solid->SetFramePosition(Vec3f(0,-3,0));                     // 剛体の位置を(0,-3,0)に設定
    Vec3d pos = solid->GetFramePosition();                      // 剛体の位置を取得    
\endverbatim

剛体は質点とは違い，大きさを持っているので，剛体の傾きも考慮しなくてはなりません．<br>
剛体の傾き（向き）を設定・取得するには，
\link Spr::PHSolidIf::SetOrientation() PHSolidIf::SetOrientation \endlink と
\link Spr::PHSolidIf::GetOrientation() PHSolidIf::GetOrientation \endlink 
を呼びます．
\verbatim	
    solid->SetOrientation(Quaternionf::Rot(Rad(30), 'z'));      // z軸回りに30度回転
    Quaternion ori = solid->GetOrientation();                   // 剛体の傾きを取得
\endverbatim

また，剛体の位置・傾きを，同時に設定・取得することもできます．
これは \c Posed 型で表されます． \c Posed 型は， \c Affine と同じ機能を持つ 7 次元ベクトルで，
位置と傾きを合わせたベクトルとなっています．
この位置と傾きを合わせた成分を，設定・取得するには，
\link Spr::PHSolidIf::SetPose() PHSolidIf::SetPose \endlink と
\link Spr::PHSolidIf::GetPose() PHSolidIf::GetPose \endlink 
を使います．
\verbatim
    Posed p = Posed::Rot(Rad(30.0), 'z');       // z軸回りに30度回転させたPosed型pを用意する
    solid->SetPose(p);                          // Posed型pを剛体の姿勢として設定する
    Posed pose = solid->GetPose();              // 剛体の姿勢を取得
\endverbatim

シーン座標系における剛体の重心を設定・取得するには，
\link Spr::PHSolidIf::SetCenterPosition() PHSolidIf::SetCenterPosition \endlink と
\link Spr::PHSolidIf::GetCenterPosition() PHSolidIf::GetCenterPosition \endlink 
を呼びます．
\verbatim	
    Vec3d center = Vec3d(0.0, 1.0, 0.0);                        // 重心を上方向に1m上にずらす
    solid->SetCenterPosition(center);                           // 重心を設定
    Vec3d center = solid->GetCenterPosition();                  // 重心を取得
\endverbatim
<br><br>

\subsection solid_velocity 			剛体の速度
剛体の速度は，シーン座標系から観測した，剛体座標系原点の速度つまり質量中心の速度で表わされます．
なお，速度は， \c Vec3d で表されるベクトル（速度ベクトル）となります．
剛体の速度を設定・取得するには，
\link Spr::PHSolidIf::SetVelocity() PHSolidIf::SetVelocity \endlink と
\link Spr::PHSolidIf::GetVelocity() PHSolidIf::GetVelocity \endlink 
を呼びます．
\verbatim	
    solid->SetVelocity(Vec3d(-20.0, 0.0, 0.0));　　             // 速度の設定
    vec3d velocity = solid->GetVelocity();                      // 速度の取得
\endverbatim

同様に剛体の角速度は
\link Spr::PHSolidIf::SetAngularVelocity() PHSolidIf::SetAngularVelocity \endlink と
\link Spr::PHSolidIf::GetAngularVelocity() PHSolidIf::GetAngularVelocity \endlink 
を呼ぶことで設定・取得できます．
剛体の角速度は，シーン座標系から観測した剛体座標系の角速度です．
\verbatim	
    solid->SetAngularVelocity(Vec3d(1.0, 1.0, 1.0));            // シーンに対する角速度を設定
    vec3d velocity = solid->GetAngularVelocity();               // 角速度の取得
\endverbatim
<br><br>

\subsection solid_inertia_parameter 		剛体の慣性パラメータ
剛体の質量特性は，質量と，剛体座標系における剛体の質量中心位置，また慣性テンソルで表現されます．
これらは，剛体に対し，以下のように設定することも可能ですが，剛体デスクリプタから与えることもできます．
剛体デスクリプタからの設定方法については，\ref solidDesc を参照下さい．

剛体の質量を設定・取得するには，
\link Spr::PHSolidIf::SetMass() PHSolidIf::SetMass \endlink と 
\link Spr::PHSolidIf::GetMass() PHSolidIf::GetMass \endlink 
を呼びます．
\verbatim	
    solid->SetMass(2.0);                                // 質量を 2.0 に設定
    double mass = solid->GetMass();                     // 質量を取得
\endverbatim

重心位置はデフォルトでは剛体座標系の原点と一致していますが， 
\link Spr::PHSolidIf::GetCenterOfMass() PHSolidIf::GetCenterOfMass \endlink で取得することができます．
また、\link Spr::PHSolidIf::SetCenterOfMass() PHSolidIf::SetCenterOfMass \endlink を使い、設定することもできます．
\verbatim
    //質量中心を，剛体座標原点からx方向に0.1ずらす
	Vec3d center = solid->GetCenterOfMass();
	center.x += 0.1;
	solid->SetCenterOfMass(center);
\endverbatim

慣性テンソルを設定・取得するには，
\link Spr::PHSolidIf::SetInertia() PHSolidIf::SetInertia \endlink と 
\link Spr::PHSolidIf::GetInertia() PHSolidIf::GetInertia \endlink 
を呼びます．
\verbatim	
    solid->SetInertia(2.0 * Matrix3d::Unit());          // 対角成分が 2.0 の行列を設定
    Matrix3d inertia = solid->GetInertia();             // 慣性テンソルを取得
\endverbatim
<br><br>

\subsection solid_force 		剛体へ力を加える
剛体に作用する力には，
-# 外力
-# 接触力
-# 関節の拘束力
-# 関節のモータトルク
があります．

剛体へ並進力を加えるには \link Spr::PHSolidIf::AddForce() PHSolidIf::AddForce \endlink，
トルクを加えるには \link Spr::PHSolidIf::AddTorque() PHSolidIf::AddTorque \endlink を呼びます．<br>
例えば，剛体に対して，下図のような力を加えた場合を考えます．
	\image html force.jpg
剛体に力が作用する場合を考え，質量中心に右方向へ \c 10N の力を加えるには，以下のように設定することができます．
\verbatim	
    solid->AddForce(Vec3f(10, 0, 0);              　　　// 質量中心に右方向へ10Nの力を加える
\endverbatim

質量中心だけでなく，位置を指定して，力を加えることも可能です．
\verbatim    
    solid->AddForce(f, r);                                                          // 力を 位置r(World系) に加える
    solid->AddForce(Vec3f(10,0,0), Vec3f(0,2,0)+solid1->GetCenterPosition());       // 重心の2m上を右方向に10Nの力を加える
\endverbatim

また，剛体に対してトルクを加えるには，
以下のように\link Spr::PHSolidIf::AddTorque() PHSolidIf::AddTorque \endlink を呼びます．
\verbatim
    solid->AddTorque(f);                                // トルクfを加える
    solid->AddTorque(Vec3d(0,0,0));                     // トルクを初期化
\endverbatim

ただし，剛体に加えられている力・トルクは，\link Spr::PHSceneIf::Step() PHSceneIf::Step \endlink を
呼ぶ度に 0 にクリアされることに注意して下さい．
このため，定常的に力・トルクを加え続けるためには，\link Spr::PHSceneIf::Step() PHSceneIf::Step \endlink を呼ぶ前に毎回
  \link Spr::PHSolidIf::AddForce() PHSolidIf::AddForce \endlink \c /
  \link Spr::PHSolidIf::AddTorque() PHSolidIf::AddTorque \endlink を呼ぶ必要があります．
<br><br><br>

\subsection solid_dynamical 物理法則の影響を受けないようにする
場合によって，他の剛体との接触や重力からの影響を受けない剛体を作りたくなることがあります．
このような剛体を非動力学的な剛体と呼びます．
剛体を非動力学的としたり，元に戻したりするには \link Spr::PHSolidIf::SetDynamical() PHSolidIf::SetDynamical \endlink を呼びます．
非動力学的とすることが有効な 1 つの典型的な例はシーンの地面です．
普通，地面は空間に固定されていて，他の剛体との衝突や重力によって位置を変化させないことが求められます．
これは，
\verbatim
    floor->SetDynamical(false);		//floorはPHSolidIf*型の変数
\endverbatim
とすることで実現できます．
また，ある剛体が非動力学的かどうかは \link Spr::PHSolidIf::IsDynamical() PHSolidIf::IsDynamical \endlink を呼ぶことで取得できます．<br>
非動力学的な剛体でも位置，傾き，速度，角速度の変更は可能です．
<br><br><br>


\subsection solid_shape 		剛体への形状の割り当て
剛体には 1 つ以上の形状を割り当てることができます．
物理シミュレーション\c SDK は，形状の交差を検知し，
剛体間に働く接触力を自動的に計算します．
剛体に割り当てることのできる形状の種類および作成方法については \ref shape を参照下さい．

剛体に形状を割り当てるには \link Spr::PHSolidIf::AddShape() PHSolidIf::AddShape \endlink を使います．
剛体は，割り当てられた形状（\c CDShape 型のオブジェクト）への参照を保持するだけですので，
\b 1つの形状を複数の異なる剛体に割り当てても問題ありません． 
これによって，多数の同じ形の剛体を作成したい場合にメモリを節約できます．
また，1 つの剛体に同じ \c CDShape オブジェクトを複数回登録すると，それぞれ別の形状として認識されます．

形状を剛体に取り付ける位置を取得・設定するには，
\link Spr::PHSolidIf::GetShapePose() PHSolidIf::GetShapePose \endlink，
\link Spr::PHSolidIf::SetShapePose() PHSolidIf::SetShapePose \endlinkを呼びます．
<br><br>
<div align="right">
<a href="#pagePhysics">top of page</a>
</div> 
<hr>




<!---------------------------------- 形状 --------------------------------------------------------------------------->
\section shape		形状
形状を用意しておくと，同じ形状を新たに 作成したり，コピーしたりする必要がなく，
モデリングを効率的に行うことができます．\ref solid_shape も参照下さい．<br>
剛体に割り当てることができる形状には以下のものがあります．
- \ref shape_mesh
- \ref shape_sphere
- \ref shape_box　
    	
扱うことのできる形状は凸形状のみです．
凸形状とは，一言でいえば凹みや穴の無い形状のことです．
凸でない（凹みや穴を持つ）形状を剛体に割り当てるには，あらかじめその形状を
複数の凸形状に分割して割り当てる必要があります．


\subsection shape_mesh 		メッシュ 
各辺の長さが \c 2 の立方体を作成する方法を以下に示します．<br>
\c 1 つのメッシュ形状を用意し，それに基づき，剛体を \c 2 つ作成しています．
\verbatim
    CDConvexMeshDesc md;                                                    // メッシュディスクリプタ(md)
    md.vertices.push_back(Vec3f(-1,-1,-1));　　                             // 立方体の頂点座標を登録
    md.vertices.push_back(Vec3f(-1,-1, 1));	
    md.vertices.push_back(Vec3f(-1, 1,-1));	
    md.vertices.push_back(Vec3f(-1, 1, 1));
    md.vertices.push_back(Vec3f( 1,-1,-1));	
    md.vertices.push_back(Vec3f( 1,-1, 1));
    md.vertices.push_back(Vec3f( 1, 1,-1));
    md.vertices.push_back(Vec3f( 1, 1, 1));
    	
    // 剛体 solid1
    CDConvexMeshIf* mesh1 = DCAST(CDConvexMeshIf, sdk->CreateShape(md));　　// md に基づいて，メッシュ形状(mesh1)を作成
    PHSolidIf* solid1 = scene->CreateSolid();                               // 剛体(solid1)を作成
    solid1->AddShape(mesh1);                                                // 剛体(solid1)に，メッシュ形状(mesh1)を割り当てる
    
    // 剛体 solid2
    CDConvexMeshIf* mesh2 = DCAST(CDConvexMeshIf, sdk->CreateShape(md));　　// md に基づいて，メッシュ形状(mesh2)を作成
    PHSolidIf* solid2 = scene->CreateSolid();                               // 剛体(solid2)を作成
    solid2->AddShape(mesh2);                                                // 剛体(solid2)に，メッシュ形状(mesh2)を割り当てる
\endverbatim
<br><br>    	
    	
\subsection shape_sphere 	球体
半径が \c 1 の球体を作成する方法を以下に示します．<br>
\c 1 つの球体形状を用意し，それに基づき，剛体を \c 2 つ作成しています．
\verbatim
    CDSphereDesc sd;                                                         // 球体ディスクリプタ(sd)
    sd.radius = 1.0;                                                         // 球体の半径
    
    CDSphereIf* sphere1 = DCAST(CDSphereIf, sdk->CreateShape(sd));           // sd に基づいて, 球体形状(sphere1)を作成
    PHSolidIf* solid1 = scene->CreateSolid();                                // 剛体(solid1)を作成
    solid1->AddShape(sphere1)                                                // 剛体(solid1)に，球体形状(sphere1)を割り当てる

    CDSphereIf* sphere2 = DCAST(CDSphereIf, sdk->CreateShape(sd));           // sd に基づいて, 球体形状(sphere2)を作成
    PHSolidIf* solid2 = scene->CreateSolid();                                // 剛体(solid2)を作成
    solid2->AddShape(sphere2)                                                // 剛体(solid2)に，球体形状(sphere2)を割り当てる
\endverbatim
<br><br>    	

\subsection shape_box	 	直方体 
\c X 方向の長さが \c 1，Y 方向の長さ \c 2，\c Z 方向の長さが \c 3 の直方体を作成する方法を以下に示します．<br>
\c 1 つの直方体形状を用意し，それに基づき，剛体を \c 2 つ作成しています．
\verbatim    
    CDBoxDesc bd;                                                            // 直方体ディスクリプタ(bd);
    bd.boxsize = Vec3f(1,2,3);
    	
    CDBoxIf* box1 = DCAST(CDBoxIf, sdk->CreateShape(bd));                    // bd に基づいて，直方体形状(box1)を作成
    PHSolidIf* solid1 = scene->CreateSolid();                                // 剛体(solid1)を作成
    solid1->AddShape(box1)                                                   // 剛体(solid1)に，直方体形状(box1)を割り当てる
    	
    CDBoxIf* box2 = DCAST(CDBoxIf, sdk->CreateShape(bd));                    // bd に基づいて，直方体形状(box2)を作成
    PHSolidIf* solid2 = scene->CreateSolid();                                // 剛体(solid2)を作成
    solid2->AddShape(box2)                                                   // 剛体(solid2)に，直方体形状(box2)を割り当てる
\endverbatim
<br><br>
<div align="right">
<a href="#pagePhysics">top of page</a>
</div> 
<hr>




<!---------------------------------- 関節 --------------------------------------------------------------------------->
\section joint 		関節
関節とは，2つの剛体の相対運動に拘束を加えるものです．
2つの剛体の相対運動は，拘束が存在しない場合は並進3自由度，回転3自由度の計6つの自由度を持ちます．
この内どの自由度を拘束するかに応じて様々な関節の種類が考えられます．
Springheadがサポートする関節の種類を以下に示します．
	\image html joints.png
<br>


\subsection joint_create	関節の作成
関節を作成し，二つの剛体を連結する典型的なコードを以下に示します．
\verbatim
    PHHingeJointDesc descJoint;                         //ディスクリプタを作成
    descJoint.pose[0].Pos() = Vec3d( 1.0, 0.0, 0.0);    //ソケットの位置を設定
    descJoint.pose[1].Pos() = Vec3d(-1.0, 0.0, 0.0);    //プラグの位置を設定
    scene->CreateJoint(solid0, solid1, descJoint));     //関節を作成
\endverbatim

各関節のディスクリプタは，対応する関節クラス名から If をとって Desc をつけた名前を持ち，
以下のような継承関係にあります．

　　　　　　　　　　　　＜ディスクリプタの継承関係＞ 


関節を作成するには，始めに作成したい関節の種類に対応するディスクリプタを作成し，
必要ならばディスクリプタのプロパティを設定します．
次に，\link Spr::PHSceneIf::CreateJoint() PHSceneIf::CreateJoint \endlink 関数を，
連結したい剛体への参照と，先に作成したディスリプタを引数として呼びます．
\link Spr::PHSceneIf::CreateJoint() PHSceneIf::CreateJoint \endlink は，渡されたディスクリプタの
種類を内部で判別し，対応する関節を作成し，その参照を返します．
<br><br><br>


\subsection joint_frame 	プラグとソケット 
剛体と関節の位置関係は下の図のようになっています．

　　　　　　　　　　　　　　　＜ 関節フレームの図 ＞
	
片方の剛体にはソケット，もう片方の剛体にはプラグと呼ばれる座標系が取り付けられます．
関節の種類に応じて，ソケットとプラグの相対位置関係が拘束されます．
例えばヒンジの場合，ソケットとプラグの原点を一致させ，同時に両者のZ軸同士が平行となるように拘束されます．

ソケットおよびプラグの取り付け位置は \link Spr::PHConstraintDesc::pose PHConstraintDesc::pose[2] \endlink で
指定します（実際には PHConstraintDesc から派生したディスクリプタを使います）． 
pose[0] はソケットを取り付ける剛体（CreateJoint の第1引数)の座標系に対するソケットの位置・傾きを表わします． 
同様に pose[1] はプラグを取り付ける剛体（CreateJoint の第2引数)の座標系に対するプラグの位置・傾きを表わします．
<br><br><br>


\subsection joint_enable 	関節の有効化・無効化
関節を作成する処理は比較的ハイコストです．このため，一時的に関節による拘束を解除し，しばらくして
元に戻すような処理をするときに，その都度関節の作成・削除を繰り返すことはお勧めしません． 
\link Spr::PHConstraintIf::Enable() PHConstraintIf::Enable \endlink を呼ぶことによって関節を一時的に
無効化したり，再び有効化することができます．また，ある時点で関節が有効かどうかは 
\link Spr::PHConstraintIf::IsEnabled() PHConstraintIf::IsEnabled \endlink 呼ぶことで知ることができます．
<br><br><br>


\subsection joint_1dof 		1自由度関節
1 自由度関節とは，1 自由度の相対運動を実現する（言い換えると\c 5 つの自由度を拘束する）関節で，
以下のものがあります：
- \ref joint_hinge
- \ref joint_slider

全ての1自由度関節は PHJoint1DIf を親クラスに持ち，共通のインタフェースで状態の取得や操作が可能です．

関節変位を取得するには\link Spr::PHJoint1DIf::GetPosition() PHJoint1DIf::GetPosition\endlinkを呼びます．
関節変位の定義は関節の種類に依ります．例えばヒンジの場合はソケットとプラグの成す角度，スライダの場合は
ソケットに対するプラグの平行移動量になります．
関節速度（関節変位の時間変化率）を取得するには 
\link Spr::PHJoint1DIf::GetVelocity() PHJoint1DIf::GetVelocity \endlink を呼びます．

関節力を設定するには \link Spr::PHJoint1DIf::SetTorque() PHJoint1DIf::SetTorque \endlink を呼びます．
関節力とは，1自由度関節の運動方向に沿って加えられる力です．
関節に取り付けられたモータが出す力と考えれば分かりやすいでしょう．
剛体に加わる力（\link Spr::PHSolidIf::AddForce() PHSolidIf::AddForce\endlinkで設定される）と異なり，
関節力は \link Spr::PHSceneIf::Step() PHSceneIf::Step\endlink を実行してもクリアされません．
現在設定されている関節力を取得するには \link Spr::PHJoint1DIf::GetTorque() PHJoint1DIf::GetTorque \endlink を呼びます．

\link Spr::PHJointIf::SetRange PHJointIf::SetRange\endlink を呼ぶことで可動範囲を制限できます．

関節にバネやダンパを取り付けることができます．
バネ係数を取得・設定するには
\link Spr::PHJoint1DIf::GetSpring() PHJoint1DIf::GetSpring \endlink, 
\link Spr::PHJoint1DIf::SetSpring() PHJoint1DIf::SetSpring \endlink
を呼びます．また，バネの基準変位を取得・設定するには
\link Spr::PHJoint1DIf::GetSpringOrigin() PHJoint1DIf::GetSpringOrigin \endlink, 
\link Spr::PHJoint1DIf::SetSpringOrigin() PHJoint1DIf::SetSpringOrigin \endlink
を呼びます．
ダンパ係数を取得・設定するには
\link Spr::PHJoint1DIf::GetDamper() PHJoint1DIf::GetDamper \endlink, 
\link Spr::PHJoint1DIf::SetDamper() PHJoint1DIf::SetDamper \endlink
を呼びます．
バネ係数を\f$K\f$，バネの基準変位を\f$q_0\f$，ダンパ係数を\f$D\f$とすると，生じる関節トルク\f$\tau\f$は
\f[
\tau = -K (q - q_0) - D \dot{q}
\f]
と表わされます．ここで\f$q\f$は関節変位，\f$\dot{q}\f$は関節速度です．
<br><br><br>


\subsection joint_hinge 	ヒンジ

\image html hinge.png

ヒンジは1本の軸周りの回転運動を実現する関節です．
ソケットとプラグの原点を一致させ，かつZ軸同士を同じ向きに拘束します．
<br><br><br>


\subsection joint_slider スライダ

\image html slider.png

スライダは 1 方向の平行移動を実現する関節です．
ソケットとプラグの傾きを一致させ，かつプラグの原点をソケットの Z 軸上に拘束します．
<br><br><br>


\subsection joint_boll  ボールジョイント

\image html balljoint.png

ボールジョイントは  1点を中心とする任意方向の回転を実現する 3 自由度関節です．
ソケットとプラグの原点が一致するように拘束します．

<br><br>
<div align="right">
<a href="#pagePhysics">top of page</a>
</div> 


*/