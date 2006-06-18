/** \page pagePhysics 物理シミュレーションSDK

- \ref sec_PHSdk
 - \ref sdk_create
 - \ref sdk_finalize
- \ref scene
 - \ref scene_create
 - \ref scene_gravity
 - \ref scene_step
- \ref solid
 - \ref solid_create
 - \ref solid_pose
 - \ref solid_mass
 - \ref solid_force
 - \ref solid_dynamical
 - \ref solid_shape
- \ref shape
- \ref joint

\section sec_PHSdk 物理シミュレーションSDKの初期化と終了

物理シミュレーションSDKは，物理法則にもとづいた複数物体のインタラクションを計算する機能を提供します．

物理シミュレーションSDKの使用は，グローバル関数\link Spr::CreatePHSdk() CreatePHSdk\endlink を呼ぶことから始まります．
\verbatim
	PHSdkIf* sdk = CreatePHSdk();
\endverbatim
以降，SDKの操作は\link Spr::CreatePHSdk() CreatePHSdk\endlink によって返されたPHSdkIf型のポインタを介して行います．

SDKは，1つ以上のシーンを持つことができます．
また，1つのシーンは1つ以上の剛体(PHSolid)と関節(PHJoint)を持つことができます．
さらに，剛体は形状(CDConvexMesh, CDSphere, CDBox, ...)を参照して形を持つことができます．
関節は2つの剛体を結び付けます．
物理エンジン内のオブジェクトの参照関係の例を図に示します．
<img src="../../include/docsrc/01UsersGuide/PHSdkScene.png">

SDKの終了処理は内部で自動的に行われます．ユーザから明示的にSDKを解放する必要はありません．

\section scene シーン

シーンとは物理シミュレーションの舞台となる空間のことです．

\subsection scene_create シーンの作成

シーンを作成するには\link Spr::PHSdkIf::CreateScene() PHSdkIf::CreateScene\endlinkを呼びます．

\verbatim
	//デフォルトの設定でシーンを作成
	PHSceneDesc sceneDesc;
	PHSceneIf* scene = sdk->CreateScene(sceneDesc);
\endverbatim

シーンの操作は\link Spr::PHSdkIf::CreateScene() PHSdkIf::CreateScene\endlinkによって返された
PHSceneIf型のポインタを介して行ないます．

シーンを複数作成することは可能ですが，異なるシーン同士は互いに影響をおよぼしません．

\subsection scene_step シミュレーションの実行

シーンの時刻を進めるには\link Spr::PHSceneIf::Step() PHSceneIf::Step\endlinkを呼びます．

\verbatim
	//0.05 * 100 = 5秒間のシミュレーション
	scene->SetTimeStep(0.05);
	for(int i = 0; i < 100; i++)
		scene->Step();
\endverbatim

\link Spr::PHSceneIf::Step() Step\endlink の流れは大体以下のようになっています．
-# 剛体同士の衝突判定
-# 接触力，関節の拘束力の計算
-# 剛体の速度・位置の更新

一度の\link Spr::PHSceneIf::Step() Step\endlinkの呼び出しで進む時間の幅は
\link Spr::PHSceneIf::GetTimeStep() PHSceneIf::GetTimeStep\endlinkと
\link Spr::PHSceneIf::SetTimeStep() PHSceneIf::SetTimeStep\endlink
で取得・設定できます．<br>
＊ここで設定する値はそのまま数値積分の積分ステップとなりますので，あまり大きな値(例えば1秒)は
設定しないでください．

\subsection scene_gravity 重力の設定

シーンに働く重力を設定するには\link Spr::PHSceneIf::SetGravity() PHSceneIf::SetGravity\endlinkを呼びます．

\verbatim
	scene->SetGravity(Vec3f(0.0f, -9.8f, 0.0f));
\endverbatim

デフォルトで，シーンの重力加速度はVec3f(0.0f, -9.8f, 0.0f)に設定されています．
無重力空間とするには

\verbatim
	scene->SetGravity(Vec3f(0.0f, 0.0f, 0.0f));
\endverbatim

また，個々の剛体に対して重力の作用を有効・無効化するには
\link Spr::PHSolidIf::SetGravity() PHSolidIf::SetGravity\endlinkを使います．

\verbatim
	//solidはsceneに登録されている剛体とする
	solid->SetGravity(false);
\endverbatim

\section solid	剛体

剛体とは，物理法則に従ってシーン中を運動する物体です．

\subsection solid_create 剛体の作成

剛体を作成し，シーンに追加するには\link Spr::PHSceneIf::CreateSolid() PHSceneIf::CreateSolid\endlinkを呼びます．

\verbatim
	//デフォルトの設定で剛体を作成
	PHSolidDesc solidDesc;
	PHSolidIf* solid = scene->CreateSolid(solidDesc);
\endverbatim

剛体に対する操作は\link Spr::PHSceneIf::CreateSolid() PHSceneIf::CreateSolid\endlinkによって返された
PHSolidIf型のポインタを介して行います．

\subsection solid_pose 剛体の位置と速度

<img src="../../include/docsrc/01UsersGuide/scene.png">

1つの剛体には，1つの座標系が貼り付いています．これを剛体座標系と呼びます．
シーン中の剛体の位置と傾きは，シーン座標系に対する剛体座標系の位置と傾きとして表現されます．

剛体の位置を設定・取得するには
\link Spr::PHSolidIf::SetFramePosition() PHSolidIf::SetFramePosition\endlinkと
\link Spr::PHSolidIf::GetFramePosition() PHSolidIf::GetFramePosition\endlink
を呼びます．
また，シーン座標系に対する重心の位置によって剛体の位置を設定・取得することも出来ます．これには
\link Spr::PHSolidIf::SetCenterPosition() PHSolidIf::SetCenterPosition\endlinkと
\link Spr::PHSolidIf::GetCenterPosition() PHSolidIf::GetCenterPosition\endlink
を使います．

剛体の傾きを取得するには
\link Spr::PHSolidIf::SetOrientation() PHSolidIf::SetOrientation\endlinkと
\link Spr::PHSolidIf::GetOrientation() PHSolidIf::GetOrientation\endlink
を使います．

剛体の位置と傾きを同時に設定・取得することも出来ます．これには
\link Spr::PHSolidIf::SetPose() PHSolidIf::SetPose\endlinkと
\link Spr::PHSolidIf::GetPose() PHSolidIf::GetPose\endlink
を使います．

剛体の速度は
\link Spr::PHSolidIf::SetVelocity() PHSolidIf::SetVelocity\endlinkと
\link Spr::PHSolidIf::GetVelocity() PHSolidIf::GetVelocity\endlink
を呼ぶことで設定・取得できます．
ここで剛体の速度は，シーン座標系から観測した剛体座標系原点の速度で表わされます．
同様に剛体の角速度は
\link Spr::PHSolidIf::SetVelocity() PHSolidIf::SetVelocity\endlinkと
\link Spr::PHSolidIf::GetVelocity() PHSolidIf::GetVelocity\endlink
を呼ぶことで設定・取得できます．
剛体の角速度は，シーン座標系から観測した剛体座標系の角速度です．

\subsection solid_mass 剛体の質量
剛体の質量情報は，全質量と，重心に関する慣性行列で表現されます．
剛体の質量を取得・設定するには
\link Spr::PHSolidIf::GetMass() PHSolidIf::GetMass \endlinkと
\link Spr::PHSolidIf::SetMass() PHSolidIf::SetMass \endlink
を使います．
同様に，剛体の慣性行列を取得・設定するには
\link Spr::PHSolidIf::GetInertia() PHSolidIf::GetInertia \endlinkと
\link Spr::PHSolidIf::SetInertia() PHSolidIf::SetInertia \endlink
を使います．

\verbatim
	//質量を0.1，慣性行列を対角成分が0.1の行列に設定
	solid->SetMass(0.1);
	solid->SetInertia(0.1 * Matrix3d::Unit());
\endverbatim

重心位置はデフォルトでは剛体座標系の原点と一致していますが，
\link Spr::PHSolidIf::GetCenterOfMass() PHSolidIf::GetCenterOfMass \endlink，\link Spr::PHSolidIf::SetCenterOfMass() PHSolidIf::SetCenterOfMass \endlink，

\verbatim
	solid->SetCenterOfMass(Vec3d(0.1, 0.0, 0.0));
\endverbatim

剛体の作成時に，ディスクリプタを用いてこれらの情報を設定することもできます．

\verbatim
	PHSolidDesc solidDesc;
	solidDesc.mass = 0.1;
	solidDesc.inertia = 0.1 * Matrix3d::Unit();
	solidDesc.center = Vec3d(0.1, 0.0, 0.0);
	PHSolidIf* solid = scene->CreateSolid(solidDesc);
\endverbatim

\subsection solid_force 剛体へ力を加える
剛体へ並進力加えるには\link Spr::PHSolidIf::AddForce() PHSolidIf::AddForce\endlink，
トルクを加えるには\link Spr::PHSolidIf::AddTorque() PHSolidIf::AddTorque\endlinkを呼びます．

剛体に加えられている力・トルクは，\link Spr::PHSceneIf::Step() PHSceneIf::Step\endlinkを呼ぶ度に0にクリアされます．
このため，定常的に力・トルクを加え続けるためには，\link Spr::PHSceneIf::Step() PHSceneIf::Step\endlinkを呼ぶ前に毎回
AddForce/AddTorqueを呼ぶ必要があります．

\subsection solid_dynamical 物理法則の影響を受けないようにする
場合によって，他の剛体との接触や重力からの影響を受けない剛体を作りたくなることがあります．
このような剛体を非動力学的な剛体と呼びます．
剛体を非動力学的としたり，元に戻したりするには\link Spr::PHSolidIf::SetDynamical() PHSolidIf::SetDynamical\endlinkを呼びます．
非動力学的とすることが有効な1つの典型的な例はシーンの地面です．
普通，地面は空間に固定されていて，他の剛体との衝突や重力によって位置を変化させないことが求められます．
これは，
\verbatim
floor->SetDynamical(false);		//floorはPHSolidIf*型の変数
\endverbatim
とすることで実現できます．
また，ある剛体が非動力学的かどうかは\link Spr::PHSolidIf::IsDynamical() PHSolidIf::IsDynamical \endlinkを呼ぶことで取得できます．

非動力学的な剛体でも位置，傾き，速度，角速度の変更は可能です．

\subsection solid_shape 剛体への形状の割り当て
剛体には1つ以上の形状を割り当てることができます．
Springhead 物理シミュレーションSDKは，形状の交差を検知し，
剛体間に働く接触力を自動的に計算します．
剛体に割り当てることのできる形状の種類および作成方法については\ref shapeの項を参照してください．

剛体に形状を割り当てるには\link Spr::PHSolidIf::AddShape() PHSolidIf::AddShape\endlinkを使います．
剛体は，割り当てられた形状（CDShape型のオブジェクト）への参照を保持するだけですので，1つの形状を複数の異なる剛体に割り当てても問題ありません．
これによって，多数の同じ形の剛体を作成したい場合にメモリを節約できます．
また，1つの剛体に同じCDShapeオブジェクトを複数回登録すると，それぞれ別の形状として認識されます．

形状を剛体に取り付ける位置を取得・設定するには，
\link Spr::PHSolidIf::GetShapePose() PHSolidIf::GetShapePose \endlink，
\link Spr::PHSolidIf::SetShapePose() PHSolidIf::SetShapePose \endlinkを呼びます．

\section shape	形状
剛体に割り当てることができる形状には以下のものがあります．
- \ref shape_sphere
- \ref shape_box
- \ref shape_mesh

Springhead Physicsが扱うことのできる形状は凸形状のみです．
凸形状とは，一言でいえば凹みや穴の無い形状のことです．
凸でない（凹みや穴を持つ）形状を剛体に割り当てるには，あらかじめその形状を
複数の凸形状に分割して割り当てる必要があります．

\subsection shape_sphere 球

\subsection shape_box 直方体

\subsection shape_mesh メッシュ


\section joint 関節

関節とは，2つの剛体の相対運動に拘束を加えるものです．
2つの剛体の相対運動は，拘束が存在しない場合は並進3自由度，回転3自由度の計6つの自由度を持ちます．
この内どの自由度を拘束するかに応じて様々な関節の種類が考えられます．
Springheadがサポートする関節の種類を以下に示します．

<img src="../../include/docsrc/01UsersGuide/joints.png">

\subsection joint_create 関節の作成

関節を作成し，二つの剛体を連結する典型的なコードを以下に示します．

\verbatim
PHHingeJointDesc descJoint;                         //ディスクリプタを作成
descJoint.pose[0].Pos() = Vec3d( 1.0, 0.0, 0.0);    //ソケットの位置を設定
descJoint.pose[1].Pos() = Vec3d(-1.0, 0.0, 0.0);    //プラグの位置を設定
scene->CreateJoint(solid0, solid1, descJoint));     //関節を作成
\endverbatim

各関節のディスクリプタは，対応する関節クラス名からIfをとってDescをつけた名前を持ち，
以下のような継承関係にあります．

＜ディスクリプタの継承関係＞

関節を作成するには，まず作成したい関節の種類に対応するディスクリプタを作成し，
必要ならばディスクリプタのプロパティを設定します．
次に，\link Spr::PHSceneIf::CreateJoint() PHSceneIf::CreateJoint\endlink関数を，
連結したい剛体への参照と，先に作成したディスリプタを引数として呼びます．
\link Spr::PHSceneIf::CreateJoint() PHSceneIf::CreateJoint\endlinkは，渡されたディスクリプタの
種類を内部で判別し，対応する関節を作成し，その参照を返します．

\subsection joint_frame プラグとソケット

剛体と関節の位置関係は下の図のようになっています．

＜ 関節フレームの図 ＞

片方の剛体にはソケット，もう片方の剛体にはプラグと呼ばれる座標系が取り付けられます．
関節の種類に応じて，ソケットとプラグの相対位置関係が拘束されます．
例えばヒンジの場合，ソケットとプラグの原点を一致させ，同時に両者のZ軸同士が平行となるように拘束されます．

ソケットおよびプラグの取り付け位置は\link Spr::PHConstraintDesc::pose PHConstraintDesc::pose[2]\endlinkで
指定します（実際にはPHConstraintDescから派生したディスクリプタを使います）．
pose[0]はソケットを取り付ける剛体（CreateJointの第1引数)の座標系に対するソケットの位置・傾きを表わします．
同様にpose[1]はプラグを取り付ける剛体（CreateJointの第2引数)の座標系に対するプラグの位置・傾きを表わします．

\subsection joint_enable 関節の有効化・無効化

関節を作成する処理は比較的ハイコストです．このため，一時的に関節による拘束を解除し，しばらくして
元に戻すような処理をするときに，その都度関節の作成・削除を繰り返すことはお勧めしません．
\link Spr::PHConstraintIf::Enable() PHConstraintIf::Enable\endlinkを呼ぶことによって関節を一時的に
無効化したり，再び有効化することができます．また，ある時点で関節が有効かどうかは
\link Spr::PHConstraintIf::IsEnabled() PHConstraintIf::IsEnabled\endlink呼ぶことで知ることができます．

\subsection joint_1dof 1自由度関節

1自由度関節とは，1自由度の相対運動を実現する（言い換えると5つの自由度を拘束する）関節で，
以下のものがあります：
- \ref joint_hinge
- \ref joint_slider

全ての1自由度関節はPHJoint1DIfを親クラスに持ち，共通のインタフェースで状態の取得や操作が可能です．

関節変位を取得するには\link Spr::PHJoint1DIf::GetPosition() PHJoint1DIf::GetPosition\endlinkを呼びます．
関節変位の定義は関節の種類に依ります．例えばヒンジの場合はソケットとプラグの成す角度，スライダの場合は
ソケットに対するプラグの平行移動量になります．
関節速度（関節変位の時間変化率）を取得するには\link Spr::PHJoint1DIf::GetVelocity() PHJoint1DIf::GetVelocity\endlinkを呼びます．

関節力を設定するには\link Spr::PHJoint1DIf::SetTorque() PHJoint1DIf::SetTorque\endlinkを呼びます．
関節力とは，1自由度関節の運動方向に沿って加えられる力です．関節に取り付けられたモータが出す力と考えれば分かりやすいでしょう．
剛体に加わる力（\link Spr::PHSolidIf::AddForce() PHSolidIf::AddForce\endlinkで設定される）と異なり，
関節力は\link Spr::PHSceneIf::Step() PHSceneIf::Step\endlinkを実行してもクリアされません．
現在設定されている関節力を取得するには\link Spr::PHJoint1DIf::GetTorque() PHJoint1DIf::GetTorque\endlinkを呼びます．

\link Spr::PHJointIf::SetRange PHJointIf::SetRange\endlinkを呼ぶことで可動範囲を制限できます．

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


\subsection joint_hinge ヒンジ

＜ヒンジの図＞

ヒンジは1本の軸周りの回転運動を実現する関節です．
ソケットとプラグの原点を一致させ，かつZ軸同士を同じ向きに拘束します．

\subsection joint_slider スライダ

＜スライダの図＞

スライダは1方向の平行移動を実現する関節です．
ソケットとプラグの傾きを一致させ，かつプラグの原点をソケットのZ軸上に拘束します．

\section ボールジョイント

＜ボールジョイントの図＞

ボールジョイントは1点を中心とする任意方向の回転を実現する3自由度関節です．
ソケットとプラグの原点が一致するように拘束します．

*/