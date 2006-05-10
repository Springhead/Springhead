/** \page pageMoreTutorial	チュートリアル（詳細編）

<br>
\section contentsMoreTutorial	◇ Contents ◇
　　 \ref moreTutPHSDK						<br>
　　　　 \ref moreTutCreatePHSdk			<br>
　　 \ref moreTutSceneDesc					<br>
　　　　 \ref moreTutContactSolver			<br>
　　　　 \ref moreTutTimeStep				<br>
　　 \ref moreTutScene						<br>
　　　　 \ref moreTutCreateScene			<br>
　　 \ref moreTutSolidDesc					<br>
　　　　 \ref moreTutSolidDescMass			<br>
　　　　 \ref moreTutSolidDescInertia		<br>
　　　　 \ref moreTutSolidDescGravity		<br>
　　 \ref moreTutSolid						<br>
　　　　 \ref moreTutCenter					<br>
　　　　 \ref moreTutPose					<br>
　　　　 \ref moreTutOri					<br>
　　　　 \ref moreTutPos					<br>
　　　　 \ref moreTutForce					<br>
<br><br>
<hr>

<!-------------------- 1．　物理エンジンSDK ---------------------------------------------------->
\section moreTutPHSDK		1．　物理エンジンSDK
まず、シミュレーションに必要となるSDKを用意します．

\subsection moreTutCreatePHSdk	1.1．　SDKの作成
SDKを作成します．
\verbatim
    PHSdkIf* sdk = CreatePHSdk();
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutorial">top of page</a>
</div> 
<hr>

<!-------------------- 2．　シーンのデスクリプタ ----------------------------------->
\section moreTutSceneDesc	2．　シーンのデスクリプタ
シーンを作成する際は、まず、シーンのデスクリプタを設定する必要があります．
ただし、以下に示すような設定を初期設定のまま利用する場合は、シーンのデスクリプタを用意する必要はありません．
デスクリプタについては、\ref pageApiBasic も参照下さい．
また、サンプルとしては、<a href="../../src/html/BoxStack_2main_8cpp-example.html">BoxStack</a>を参照下さい．

\subsection moreTutContactSolver	2.1．　接触・拘束解決エンジン
Springhead2では、接触・拘束解決エンジンとして、ペナルティ法で解く手法と、解析法で解く手法を用意しています．<br>
初期設定では、解析法に設定されています．エンジンの切り替えは以下のように行ってください．<br>
\verbatim
	PHSceneDesc dscene;
	dscene.contactSolver = PHSceneDesc::SOLVER_CONSTRAINT;      // 解析法
	dscene.contactSolver = PHSceneDesc::SOLVER_PENALTY;         // ペナルティ法
\endverbatim
<br>

\subsection moreTutTimeStep		2.2．　ステップ数
1ステップの実行時間を指定できます．初期設定では、0.005[s] に設定されています．
\verbatim
	dscene.timeStep = 0.01;
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutorial">top of page</a>
</div> 
<hr>

<!-------------------- 3．　シーン ------------------------------------------------->
\section moreTutScene		3．　シーン
シーンを構築します．

\subsection 	moreTutCreateScene		3.1．　シーンの作成
シーンを構築します．シーンを構築することで、オブジェクトを投入することが可能となります．<br>
物理エンジンSDKをもとに、デフォルトのシーンデスクリプタを利用する場合は、次のようにシーンを設定します．
\verbatim
    PHSceneIf* scene;
    scene = sdk->CreateScene();
\endverbatim
また、シーンのデスクリプタを指定することも可能です．<br>
シーンのデスクリプタについては、\ref moreTutSceneDesc を参照下さい．
\verbatim
    scene = sdk->CreateScene(dscene);
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutorial">top of page</a>
</div> 
<hr>

<!-------------------- 4．　剛体のデスクリプタ ------------------------------------->
\section moreTutSolidDesc		4．　剛体のデスクリプタ
剛体を作成する際は、まず、剛体のデスクリプタを設定する必要があります．<br>
次に示すように、質量、慣性テンソル、重心の影響の有無を、剛体のデスクリプタで設定することができます．<br>
また、これらの設定は、\ref moreTutSolid でも設定可能です．

\subsection moreTutSolidDescMass	4.1．　質量
初期設定では、質量は 1.0f に設定されています．<br>
剛体デスクリプタにて、剛体に対する質量を設定するには、以下のようにします．
\verbatim
	PHSolidDesc desc;
	desc.mass = 2.0;
\endverbatim
<br>	

\subsection moreTutSolidDescInertia		4.2．　慣性テンソル
初期設定では、慣性テンソルは単位行列に設定されています．<br>
剛体デスクリプタにて、剛体に対する慣性テンソルを設定するには、以下のようにします．
\verbatim
	PHSolidDesc desc;
	desc.inertia = 2.0 * Matrix3d::Unit();
\endverbatim
<br>

\subsection moreTutSolidDescGravity		4.3．　重力の影響の有無
初期設定では、重量の影響を受けるように設定されています．<br>
剛体デスクリプタにて、剛体に対する重力の影響の有無を設定するには、以下のようにします．
\verbatim
	PHSolidDesc desc;
	desc.gravity = false;                   // 重力の影響を受けないように設定
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutorial">top of page</a>
</div> 
<hr>

<!-------------------- 5．　剛体 --------------------------------------------------->
\section moreTutSolid		5．　剛体
剛体に対して、重心、姿勢、向きなどを設定することができます．<br>
また、剛体の質量や、慣性テンソル、重力の影響の有無については、剛体のデスクリプタから与えることもできます．<br>
剛体のデスクリプタについては、\ref moreTutSolidDesc を参照下さい．

\subsection moreTutCenter		5.1．　剛体の重心
剛体の重心は、以下のようにして設定します．
\verbatim	
	Vec3d center = Vec3d(0.0, 1.0, 0.0);
	solid->SetCenterPosition(center);
\endverbatim
また、次のようにして、剛体の重心を取得することもできます．
\verbatim	
	Vec3d center = solid->GetCenterPosition();
\endverbatim
<br>

\subsection moreTutPose		5.2．　剛体の姿勢
剛体の姿勢は、Posed型で表されます． <br>
Posed型は、Affineと同じ機能を持つ7次元ベクトルで、\ref moreTutOri と \ref moreTutPos を合わせたベクトルとなっています．
剛体の姿勢は以下のようにして設定します．
\verbatim
    Posed p = Posed::Rot(Rad(30.0), 'z');                       // z軸回りに30度回転させたPosed型pを用意する
    solid->SetPose(p);                                          // Posed型pを剛体の姿勢として設定する
\endverbatim
また、次のようにして、剛体の姿勢を取得することもできます．
\verbatim
    Posed pose = solid->GetPose();                              // 剛体の姿勢を取得
\endverbatim
<br>

\subsection moreTutOri			5.3．　剛体の向き
剛体は質点と違い、大きさを持っているので、剛体の向きも考慮しなくてはなりません．<br>
剛体の向きは以下のように設定します．
\verbatim	
    solid->SetOrientation(Quaternionf::Rot(Rad(30), 'z'));      // z軸回りに30度回転
\endverbatim
また、次のようにして、剛体の向きを取得することもできます．
\verbatim	
    Quaternion ori;
    ori = solid->GetOrientation();                              // 剛体の向きを取得
　　ori = solid->GetPose().Ori();                               // 剛体の向きを取得
\endverbatim
<br>

\subsection moreTutPos			5.4．　剛体の位置
通常、シミュレーションを行うには、その物体がどこにあるかという情報が必要になります．
その情報というのが、位置です．位置により、物体が空間のどの位置にいるかを示すことができます．
剛体の位置は以下のように設定します．
\verbatim	
    solid->SetFramePosition(Vec3f(0,-3,0));                     // 剛体の位置を(0,-3,0)に設定
\endverbatim
また、次のようにして、剛体の位置を取得することもできます．
\verbatim
    Vec3d pos;	
    pos = solid->GetFramePosition();                            // 剛体の位置を取得
    pos = solid->GetPose().Pos();                               // 剛体の位置を取得
\endverbatim
<br>

\subsection moreTutForce		5.5．　剛体に力を加える
剛体に対して、下図のような力を加えた場合を考えます．
<img src="../../include/docsrc/02Tutorial/force.jpg" width="682" height="146">
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
<a href="#pageMoreTutorial">top of page</a>
</div>    	
*/
