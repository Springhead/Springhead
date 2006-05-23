/** \page pageMoreTutoPHSdk	物理シミュレーションSDK チュートリアル

<br>
\section contentsMoreTutoPHSdk	◇ Contents ◇
　　 \ref moreTutoPHSdk						<br>
　　　　 \ref moreTutoCreatePHSdk			<br>
　　 \ref moreTutoSceneDesc					<br>
　　　　 \ref moreTutoContactSolver			<br>
　　　　 \ref moreTutoTimeStep				<br>
　　 \ref moreTutoScene						<br>
　　　　 \ref moreTutoCreateScene			<br>
　　 \ref moreTutoSolidDesc					<br>
　　　　 \ref moreTutoSolidDescMass			<br>
　　　　 \ref moreTutoSolidDescInertia		<br>
　　　　 \ref moreTutoSolidDescGravity		<br>
　　 \ref moreTutoSolid						<br>
　　　　 \ref moreTutoCreateSolid			<br>
　　　　 \ref moreTutoCenter				<br>
　　　　 \ref moreTutoPose					<br>
　　　　 \ref moreTutoOri					<br>
　　　　 \ref moreTutoPos					<br>
　　　　 \ref moreTutoForce					<br>
<br><br>
<hr>

<!-------------------- 1．　物理シミュレーションSDK ---------------------------------------------------->
\section moreTutoPHSdk		1．　物理シミュレーションSDK
はじめに、物理シミュレーションに必要となるSDKを用意します．

\subsection moreTutoCreatePHSdk	1.1．　物理シミュレーションSDKの作成
物理シミュレーションSDKを作成します．
\verbatim
    PHSdkIf* sdk = CreatePHSdk();
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutoPHSdk">top of page</a>
</div> 
<hr>

<!-------------------- 2．　シーンデスクリプタ ----------------------------------->
\section moreTutoSceneDesc	2．　シーンデスクリプタ
シーンデスクリプタから、シーンを作成することができます．
以下に示すような設定を初期設定のまま利用する場合は、シーンデスクリプタを用意する必要はありません．
デスクリプタについては、\ref pageApiBasic も参照下さい．
また、サンプルとしては、<a href="../../src/html/BoxStack_2main_8cpp-example.html">BoxStack</a>を参照下さい．

\subsection moreTutoContactSolver	2.1．　接触・拘束解決エンジン
Springhead2では、接触・拘束解決エンジンとして、ペナルティ法で解く手法と、解析法で解く手法を用意しています．<br>
初期設定では、解析法に設定されています．エンジンの切り替えは以下のように行ってください．<br>
\verbatim
    PHSceneDesc dscene;
    dscene.contactSolver = PHSceneDesc::SOLVER_CONSTRAINT;      // 解析法
    dscene.contactSolver = PHSceneDesc::SOLVER_PENALTY;         // ペナルティ法
\endverbatim
<br>

\subsection moreTutoTimeStep		2.2．　ステップ数
1ステップの実行時間を指定できます．初期設定では、0.005[s] に設定されています．
\verbatim
	dscene.timeStep = 0.01;
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutoPHSdk">top of page</a>
</div> 
<hr>

<!-------------------- 3．　シーン ------------------------------------------------->
\section moreTutoScene		3．　シーン
シーンを構築します．

\subsection 	moreTutoCreateScene		3.1．　シーンの作成
シーンを構築します．シーンを構築することで、オブジェクトを投入することが可能となります．<br>
物理エンジンSDKをもとに、デフォルトのシーンデスクリプタを利用する場合は、以下のようにシーンを設定します．
\verbatim
    PHSceneIf* scene;
    scene = sdk->CreateScene();
\endverbatim
また、シーンデスクリプタを指定することも可能です．<br>
シーンデスクリプタについては、\ref moreTutoSceneDesc を参照下さい．
\verbatim
    scene = sdk->CreateScene(dscene);
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutoPHSdk">top of page</a>
</div> 
<hr>

<!-------------------- 4．　剛体デスクリプタ ------------------------------------->
\section moreTutoSolidDesc		4．　剛体デスクリプタ
剛体デスクリプタから剛体を作成することができます．<br>
以下に示すような設定を初期値のまま利用する場合は、剛体デスクリプタを用意する必要はありません．
ここでは、質量、慣性テンソル、重心の影響の有無を設定することができます．<br>
また、これらの設定は、\ref moreTutoSolid でも設定可能です．

\subsection moreTutoSolidDescMass	4.1．　質量
初期設定では、質量は 1.0f に設定されています．<br>
剛体デスクリプタにて、剛体に対する質量を設定するには、以下のようにします．
\verbatim
    PHSolidDesc desc;
    desc.mass = 2.0;
\endverbatim
<br>	

\subsection moreTutoSolidDescInertia		4.2．　慣性テンソル
初期設定では、慣性テンソルは単位行列に設定されています．<br>
剛体デスクリプタにて、剛体に対する慣性テンソルを設定するには、以下のようにします．
\verbatim
    PHSolidDesc desc;
    desc.inertia = 2.0 * Matrix3d::Unit();
\endverbatim
<br>

\subsection moreTutoSolidDescGravity		4.3．　重力の影響の有無
初期設定では、重量の影響を受けるように設定されています．<br>
剛体デスクリプタにて、剛体に対する重力の影響の有無を設定するには、以下のようにします．
\verbatim
    PHSolidDesc desc;
    desc.gravity = false;                   // 重力の影響を受けないように設定
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutoPHSdk">top of page</a>
</div> 
<hr>

<!-------------------- 5．　剛体 --------------------------------------------------->
\section moreTutoSolid		5．　剛体
剛体に対して、重心、姿勢、向きなどを設定することができます．
また、剛体の質量や、慣性テンソル、重力の影響の有無については、剛体デスクリプタから与えることもできます．
剛体デスクリプタについては、\ref moreTutoSolidDesc を参照下さい．

\subsection moreTutoCreateSolid		5.1．　剛体の作成
デフォルトの剛体デスクリプタを利用する場合は、以下のように剛体を作成します．
\verbatim
    PHSolidIf* solid;
    solid = scene->CreateSolid();
\endverbatim
また、任意の剛体デスクリプタを指定し、剛体を作成することも可能です．<br>
\verbatim
    PHSolidIf* solid;
    solid = scene->CreateSolid(desc);
\endverbatim
<br>

\subsection moreTutoCenter		5.2．　剛体の重心
剛体の重心は、以下のように設定します．
\verbatim	
    Vec3d center = Vec3d(0.0, 1.0, 0.0);
    solid->SetCenterPosition(center);
\endverbatim
また、以下のように、剛体の重心を取得することもできます．
\verbatim	
    Vec3d center = solid->GetCenterPosition();
\endverbatim
<br>

\subsection moreTutoPose		5.3．　剛体の姿勢
剛体の姿勢は、Posed型で表されます． <br>
Posed型は、Affineと同じ機能を持つ7次元ベクトルで、\ref moreTutoOri と \ref moreTutoPos を合わせたベクトルとなっています．
剛体の姿勢は以下のように設定します．
\verbatim
    Posed p = Posed::Rot(Rad(30.0), 'z');                       // z軸回りに30度回転させたPosed型pを用意する
    solid->SetPose(p);                                          // Posed型pを剛体の姿勢として設定する
\endverbatim
また、以下のように、剛体の姿勢を取得することもできます．
\verbatim
    Posed pose = solid->GetPose();                              // 剛体の姿勢を取得
\endverbatim
<br>

\subsection moreTutoOri			5.4．　剛体の向き
剛体は質点と違い、大きさを持っているので、剛体の向きも考慮しなくてはなりません．<br>
剛体の向きは以下のように設定します．
\verbatim	
    solid->SetOrientation(Quaternionf::Rot(Rad(30), 'z'));      // z軸回りに30度回転
\endverbatim
また、以下のように、剛体の向きを取得することもできます．
\verbatim	
    Quaternion ori;
    ori = solid->GetOrientation();                              // 剛体の向きを取得
　　ori = solid->GetPose().Ori();                               // 剛体の向きを取得
\endverbatim
<br>

\subsection moreTutoPos			5.5．　剛体の位置
通常、シミュレーションを行うには、その物体がどこにあるかという情報が必要になります．
その情報というのが、位置です．位置により、物体が空間のどの位置にいるかを示すことができます．
剛体の位置は以下のように設定します．
\verbatim	
    solid->SetFramePosition(Vec3f(0,-3,0));                     // 剛体の位置を(0,-3,0)に設定
\endverbatim
また、以下のように、剛体の位置を取得することもできます．
\verbatim
    Vec3d pos;	
    pos = solid->GetFramePosition();                            // 剛体の位置を取得
    pos = solid->GetPose().Pos();                               // 剛体の位置を取得
\endverbatim
<br>

\subsection moreTutoForce		5.6．　剛体に力を加える
剛体に対して、下図のような力を加えた場合を考えます．
	\image html force.jpg
剛体に力が作用する場合を考え、質量中心に力を加えるには、以下のように設定することができます．
\verbatim
    solid->AddForce(Vec3f(10, 0, 0);                                                // 質量中心に右方向へ10Nの力を加える
\endverbatim
また、質量中心だけでなく、位置を指定して、力を加えることも可能です．
\verbatim    
    solid->AddForce(f, r);                                                          // 力を 位置r(World系) に加える
    solid->AddForce(Vec3f(10,0,0), Vec3f(0,2,0)+solid1->GetCenterPosition());       // 重心の2m上を右方向に10Nの力を加える
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutoPHSdk">top of page</a>
</div>    	
*/
