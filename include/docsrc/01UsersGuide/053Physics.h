/** \page pagePhysics 物理シミュレーションSDK

- \ref sec_PHSdk
- \ref scene
 - \ref scene_create
 - \ref scene_gravity
- \ref solid
 - \ref solid_create
 - \ref solid_shape
- \ref joint

\section sec_PHSdk 物理エンジンのオブジェクトのイメージ
1つの物理エンジン(PHSdk)は，いくつかのシーン(PHScene)を持つことができ，
シーンには剛体(PHSolid)，関節(PHJoint)があります．
剛体は形状(CDConvexMesh, CDSphere, CDBox, ...)を参照して形を持つことができます．
関節は2つの剛体を結び付けます．
物理エンジン内のオブジェクトの参照関係の例を図に示します．
<img src="../../include/docsrc/01UsersGuide/PHSdkScene.png">

\section scene シーン

シーンとは物理シミュレーションの舞台となる空間のことです．
シーンの操作はPHSceneIfクラスを介して行ないます．

\subsection scene_create シーンの作成

シーンを作成するには\link Spr::PHSdkIf::CreateScene() PHSdkIf::CreateScene\endlinkを呼びます．

\subsection scene_gravity 重力の設定

シーンに働く重力を設定するには\link Spr::PHSceneIf::SetGravity() PHSceneIf::SetGravity\endlinkを呼びます．
デフォルトで，シーンの重力加速度はVec3f(0.0f, -9.8f, 0.0f)に設定されています．
重力を無効化するにはSetGravity(Vec3f(0.0f, 0.0f, 0.0f))とします．
また，個々の剛体に対して重力の作用を有効・無効化するには
\link Spr::PHSolidIf::SetGravity() PHSolidIf::SetGravity\endlinkを使います．

\section solid	剛体

剛体とは，物理法則に従ってシーン中を運動する物体です．
剛体に対する操作は\link Spr::PHSolidIf PHSolidIf\endlinkを介して行います．

\subsection solid_create 剛体の作成

剛体を作成し，シーンに追加するには\link Spr::PHSceneIf::CreateSolid() PHSceneIf::CreateSolid\endlinkを呼びます．

\subsection solid_pose 剛体の位置と速度
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
重心位置はデフォルトでは剛体座標系の原点と一致していますが，
\link Spr::PHSolidIf::GetCenterOfMass() PHSolidIf::GetCenterOfMass \endlink，\link Spr::PHSolidIf::SetCenterOfMass() PHSolidIf::SetCenterOfMass \endlink，
あるいは\link Spr::PHSolidDesc::center PHSolidDesc::center\endlinkを用いて取得・設定できます．

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
また，ある剛体が非動力学的かどうかは\link Spr::PHSolidIf::IsDynamical PHSolidIf::IsDynamical \endlinkを呼ぶことで取得できます．

非動力学的な剛体でも位置，傾き，速度，角速度の変更は可能です．

\subsection solid_shape 剛体への形状の割り当て
剛体には1つ以上の形状を割り当てることができます．
Springhead 物理シミュレーションSDKは，形状の交差を検知し，
剛体間に働く接触力を自動的に計算します．
剛体に割り当てることのできる形状の種類および作成方法については\ref shapeを参照してください．

剛体に形状を割り当てるには\link Spr::PHSolidIf::AddShape() PHSolidIf::AddShape\endlinkを使います．
剛体は，割り当てられた形状への参照を保持するだけですので，1つの形状を複数の剛体に割り当てても問題ありません．
こうすることによって，多数の同じ形の剛体を作成したい場合にメモリを節約できます．

\section shape	形状
剛体には1つ以上の形状を割り当てることができます．
形状には以下のものがあります．
- 任意の凸メッシュ
- 球
- 直方体

Springhead Physicsが扱うことのできる形状は凸形状のみです．
凸形状とは，一言でいえば凹みや穴の無い形状のことです．
凸でない（凹みや穴を持つ）形状を剛体に割り当てるには，あらかじめその形状を
複数の凸形状に分割して割り当てる必要があります．

\section joint 関節

関節とは，2つの剛体の相対運動に拘束を加えるものです．
2つの剛体の相対運動は，拘束が存在しない場合は並進3自由度，回転3自由度の計6つの自由度を持ちます．
この内どの自由度を拘束するかに応じて様々な関節の種類が考えられます．
Springheadがサポートする関節の種類を以下に示します．

<img src="../../include/docsrc/01UsersGuide/joints.png">

□関節フレーム

剛体同士を関節でつなげるには，剛体のどの場所に関節を取り付けるかを指定する必要があります．
このためにSpringheadでは関節座標系という概念を用います．

＜ 関節フレームの図 ＞

図のように，連結するそれぞれの剛体に関節座標系を取り付けます
（ソケットとプラグのように考えると分かり易いでしょう）．

二つの関節座標系（ソケットとプラグ）がどのように拘束されるかは，関節の種類毎に異なります．
例えばヒンジの場合，関節座標系の原点を一致させ，かつZ軸同士が平行となるように拘束されます．

□関節の有効/無効化

□1自由度関節
1自由度関節とは，1自由度の相対運動を実現する（言い換えると5つの自由度を拘束する）関節で，
以下のものがあります：
- ヒンジ
- スライダ
- パスジョイント

ここでは1自由度関節に共通する機能を紹介します．
＊関節変位
＊関節速度
＊可動範囲
＊バネ、ダンパ
＊モータ力

□ヒンジ

＜ヒンジの図＞

ヒンジは1本の軸周りの回転運動を実現する関節です．
ソケットとプラグの原点を一致させ，かつZ軸同士を同じ向きに拘束します．

□スライダ

＜スライダの図＞

スライダは1方向の平行移動を実現する関節です．
ソケットとプラグの傾きを一致させ，かつプラグの原点をソケットのZ軸上に拘束します．


□パスジョイント

□バネ・ダンパ

□ユニバーサルジョイント

＜ユニバーサルジョイントの図＞

□ボールジョイント

＜ボールジョイントの図＞

ボールジョイントは1点を中心とする任意方向の回転を実現する3自由度関節です．
ソケットとプラグの原点が一致するように拘束します．

*/