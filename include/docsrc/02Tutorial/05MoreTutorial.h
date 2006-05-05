/** \page pageMoreTutorial	チュートリアル（詳細編）

<br>

\section contentsMoreTutorial	◇ Contents ◇
	&nbsp;&nbsp;&nbsp; \ref moreTutPHSDK					     	<br>
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \ref moreTutCreatePHSdk	  	<br>
	&nbsp;&nbsp;&nbsp; \ref moreTutSceneDesc						<br>
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \ref moreTutContactSolver	<br>
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \ref moreTutTimeStep			<br>
	&nbsp;&nbsp;&nbsp; \ref moreTutScene 							<br>
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \ref moreTutCreateScene		<br>
	&nbsp;&nbsp;&nbsp; \ref moreTutSolid 						<br>
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \ref morTutCenter			<br>
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \ref moreTutPose 			<br>
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \ref moreTutOri 				<br>
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \ref moreTutPos 				<br>
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \ref moreTutForce 			<br>
<br><br>
<hr>

<!-------------------- 3．　物理エンジンSDK ---------------------------------------------------->
\section moreTutPHSDK		3．　物理エンジンSDK
まず、シミュレーションに必要となるSDKを用意します。

\subsection moreTutCreatePHSdk	3.1．　SDKの作成
SDKを作成します。
\verbatim
    PHSdkIf* sdk = CreatePHSdk();
\endverbatim
<br><br><br>
<div align="right">
<a href="#pageMoreTutorial">top of page</a>
</div> 
<hr>

<!-------------------- 4．　シーンのデスクリプタ ----------------------------------->
\section moreTutSceneDesc	4．　シーンのデスクリプタ
シーンを作成する際は、まず、シーンのデスクリプタを設定する必要があります。
ただし、以下に示すような設定をデフォルトのまま利用する場合は、シーンのデスクリプタを設定する必要はありません。
デスクリプタについては、\ref pageApiBasic も参照下さい。
また、サンプルとしては、<a href="../../src/html/BoxStack_2main_8cpp-example.html">BoxStack</a>を参照下さい。

\subsection moreTutContactSolver	4.1．　接触エンジン
接触エンジンとして、Springhead2では、ペナルティ法で解く手法と、解析法で解く手法を用意しています。<br>
デフォルトでは、解析法に設定されています。接触エンジンの切り替えは以下のように行ってください。<br>
\verbatim
	PHSceneDesc dscene;
	dscene.contactSolver = PHSceneDesc::SOLVER_CONSTRAINT;      // 解析法
	dscene.contactSolver = PHSceneDesc::SOLVER_CONSTRAINT;      // ペナルティ法
\endverbatim
<br>

\subsection moreTutTimeStep		4.2．　ステップ数
1ステップの実行時間を指定できます。デフォルトでは 0.005[s] に設定されています。
\verbatim
	dscene.timeStep = 0.01;
\endverbatim
<br><br><br>
<div align="right">
<a href="#pageMoreTutorial">top of page</a>
</div> 
<hr>

<!-------------------- 5．　シーン ------------------------------------------------->
\section moreTutScene		5．　シーン
シーンを構築します。

\subsection 	moreTutCreateScene		5.1．　シーンの作成
シーンを構築します。シーンを構築することで、オブジェクトを投入することが可能となります。<br>
あらかじめ用意しておいたSDKをもとに、デフォルトのシーンデスクリプタを利用する場合は、次のようにシーンを設定します。
\verbatim
    PHSceneIf* scene;
    scene = sdk->CreateScene();
\endverbatim
また、シーンのデスクリプタを指定することも可能です。<br>
シーンのデスクリプタについては、\ref moreTutSceneDesc を参照下さい。
\verbatim
    scene = sdk->CreateScene(dscene);
\endverbatim
<br><br><br>
<div align="right">
<a href="#pageMoreTutorial">top of page</a>
</div> 
<hr>

<!-------------------- 6．　剛体 --------------------------------------------------->
\section moreTutSolid		6．　剛体
剛体に対して、向きや、質量、重心、慣性テンソルなどを設定することができます。

\subsection morTutCenter		6.1．　剛体の重心
剛体の重心は、以下のようにして設定します。
\verbatim	
	Vec3d center = Vec3d(0.0, 1.0, 0.0);
	solid->SetCenterPosition(center);
\endverbatim
また、次のようにして、剛体の重心を取得することもできます。
\verbatim	
	Vec3d center = solid->GetCenterPosition();
\endverbatim
<br>

\subsection moreTutPose		6.2．　剛体の姿勢
剛体の姿勢は、Posed型で表されます。 <br>
Posed型は、Affineと同じ機能を持つ7次元ベクトルで、\ref moreTutOri と \ref moreTutPos を合わせたベクトルとなっています。
剛体の姿勢は以下のようにして設定します。
\verbatim
    Posed p = Posed::Rot(Rad(30.0), 'z');                       // z軸回りに30度回転させたPosed型pを用意する
    solid->SetPose(p);                                          // Posed型pを剛体の姿勢として設定する
\endverbatim
また、次のようにして、剛体の姿勢を取得することもできます。
\verbatim
    Posed pose = solid->GetPose();                              // 剛体の姿勢を取得
\endverbatim
<br>

\subsection moreTutOri			6.3．　剛体の向き
剛体は質点と違い、大きさを持っているので、剛体の向きも考慮しなくてはなりません。<br>
剛体の向きは以下のように設定します。
\verbatim	
    solid->SetOrientation(Quaternionf::Rot(Rad(30), 'z'));      // z軸回りに30度回転
\endverbatim
また、次のようにして、剛体の向きを取得することもできます。
\verbatim	
    Quaternion ori;
    ori = solid->GetOrientation();                              // 剛体の向きを取得
　　ori = solid->GetPose().Ori();                               // 剛体の向きを取得
\endverbatim
<br>

\subsection moreTutPos			6.4．　剛体の位置
通常、シミュレーションを行うには、その物体がどこにあるかという情報が必要になります。
その情報というのが、位置です。位置により、物体が空間のどの位置にいるかを示すことができます。
剛体の位置は以下のように設定します。
\verbatim	
    solid->SetFramePosition(Vec3f(0,-3,0));                     // 剛体の位置を(0,-3,0)に設定
\endverbatim
また、次のようにして、剛体の位置を取得することもできます。
\verbatim
    Vec3d pos;	
    pos = solid->GetFramePosition();                            // 剛体の位置を取得
    pos = solid->GetPose().Pos();                               // 剛体の位置を取得
\endverbatim
<br>

\subsection moreTutForce		6.5．　剛体に力を加える
剛体に対して、下図のような力を加えた場合を考えます。
<img src="../../include/docsrc/02Tutorial/force.jpg" width="682" height="146">
剛体に力が作用する場合を考え、質量中心に力を加えるには、以下のように設定することができます。
\verbatim
    solid->AddForce(Vec3f(10, 0, 0);                                                // 質量中心に右方向へ10Nの力を加える
\endverbatim
また、質量中心だけでなく、位置を指定して、力を加えることも可能です。
\verbatim    
    solid->AddForce(f, r);                                                          // 力を 位置r(World系) に加える
    solid->AddForce(Vec3f(10,0,0), Vec3f(0,2,0)+solid1->GetCenterPosition());       // 重心の2m上を右方向に10Nの力を加える
\endverbatim



<br>
<div align="right">
<a href="#pageMoreTutorial">top of page</a>
</div>    	
*/
