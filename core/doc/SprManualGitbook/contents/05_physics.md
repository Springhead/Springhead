# Physics

Physicsモジュールは物理シミュレーション機能を提供します．主にサポートされているのは，マルチボディダイナミクスと呼ばれる剛体と関節などの拘束からなる動力学シミュレーションです．今のところソフトボディや流体，パーティクルなどの機能はサポートされていません．





## Physics SDK
PhysicsモジュールのすべてのオブジェクトはSDKクラス*PHSdk*によって管理されます．*PHSdk*クラスは，プログラムの実行を通してただ１つのオブジェクトが存在するシングルトンクラスです．*PHSdk*オブジェクトを作成するには以下のようにします．
```c++
PHSdkIf* phSdk = PHSdkIf::CreateSdk();
```
通常この操作はプログラムの初期化時に一度だけ実行します．また，Frameworkモジュールを使用する場合はユーザが直接*PHSdk*を作成する必要はありません．*PHSdk*の機能はシーンと形状の管理です．シーンに関する機能は次節で説明します．また，形状に関する機能は以下の通りです．

|*PHSdkIf*||															 |
|---|---|---|---|
|_CDShapeIf*_| *CreateShape(const CDShapeDesc&)*| 形状を作成	|
|_CDShapeIf*_| *GetShape(int)*				| 形状を取得	|
|*int*	| *NShape()*						| 形状の数		|
異なるシーン間で形状を共有できるように，形状管理はシーンではなく*PHSdk*の機能になっています．詳しくは[Collision](04_collision.html)の項目を参照してください．



## シーン

シーンは物理シミュレーションを行う環境を表します．複数のシーンを作成できますが，シーン同士は互いに独立しており，ユーザが直接橋渡し処理をしない限りは影響を及ぼしあうことはありません．シーンクラスは*PHScene*で，*PHScene*オブジェクトは*PHSdk*により管理されます．

|*PHSdkIf*| | 															 |
|---|---|---|---|
|_PHSceneIf*_| *CreateScene(const PHSceneDesc& desc)*		| シーンを作成		|
|*int*	| *NScene()*										| シーンの数		|
|_PHSceneIf*_| *GetScene(int i)*								| シーンを取得		|
|*void*	| *MergeScene(PHSceneIf* scene0, PHSceneIf* scene1)*| シーンを統合		|
シーンを作成するには以下のようにします．
```c++
PHSceneIf* phScene = phSdk->CreateScene();
```
引数にディスクリプタを指定することもできます．*MergeScene*は，*scene1*が保有するオブジェクトをすべて*scene0*に移動した後に*scene1*を削除します．

シーンは剛体や関節などの様々な構成要素の管理を行うほか，物理シミュレーションに関する設定を行う機能を提供します．各構成要素の作成についてはそれぞれの節で説明しますので，以下ではシミュレーション設定機能について述べます．

|*PHSceneDesc*| | 										 |
|---|---|---|
|*double*	|	*timeStep*| 時間ステップ幅					|
|*unsigned*|	*count*	| シミュレーションしたステップ数	|
|*Vec3d*	|	*gravity*| 重力加速度						|
|*double*	|	*airResistanceRate*| 空気抵抗係数				|
|*int*	|	*numIteration*	| LCPの反復回数				|


|*PHSceneIf*| |
|---|---|
|*double*	| *GetTimeStep()*				|
|*void*	| *SetTimeStep(double)*			|
|*unsigned*| *GetCount()*					|
|*void*	| *SetCount(unsigned)*			|
|*void*	| *SetGravity(const Vec3d&)*	|
|*Vec3d*	| *GetGravity()*					|
|*void*	| *SetAirResistanceRate(double)*	|
|*double*	| *GetAirResistanceRate()*		|
|*int*	| *GetNumIteration()*			|
|*void*	| *SetNumIteration()*			|
- *timeStep*は一度のシミュレーションステップで進める時間幅です．小さいほどシミュレーションの精度は上がりますが，同じ時間シミュレーションを進めるのにかかる計算コストは増大します．

- *count*はシーン作成後にシミュレーションした累積ステップ数です．*count*と*timeStep*の積が経過時間を表します．

- *gravity*は重力加速度ベクトルです．

- *airResistanceRate*は，シミュレーションの安定性を向上するために毎ステップに各剛体の速度に掛けられる係数です．例えば*airRegistanceRate*が0.95であればステップごとに速度が95%になります．このように強制的に減速をかけることで，精度を犠牲に安定性を得ることができます．

- *numIteration*は，拘束力を計算するために内部で実行されるアルゴリズムの反復回数です．一般に，反復回数に関して指数関数的に拘束力の精度が向上し，計算コストは比例的に増大します．



### シミュレーションの実行
シミュレーションを*1*ステップ進めるには*Step*関数を呼びます．

|*PHSceneIf*| | 		 |
|---|---|---|
|*void*| *Step()*| シミュレーションを*1*ステップ進める |
*Step*を実行すると，おおまかに述べて内部で次の処理が行われます．

-  衝突判定と接触拘束の生成
-  拘束力の計算
-  剛体の速度および位置の更新





## 剛体

剛体は物理シミュレーションの基本要素です．剛体のクラスは*PHSolid*です．まず剛体を作成・管理するための*PHScene*の関数を示します．

|*PHSceneIf*| | 									 |
|---|---|---|---|
|_PHSolidIf*_	| *CreateSolid(const PHSolidDesc&)*| 剛体を作成する |
|*int*		| *NSolids()*					| 剛体の数 |
|_PHSolidIf*_* | *GetSolids()*					| 剛体配列の先頭アドレス |
剛体を作成するには
```c++
PHSolidIf* solid = phScene->CreateSolid();
```
とします．ディスクリプタを指定して作成することもできます．また，*GetSolids*は作成した剛体を格納した内部配列の先頭アドレスを返します．したがって，例えば*0*番目の剛体を取得するには
```c++
PHSolidIf* solid = phScene->GetSolids()[0];      // get 0-th solid
```
とします．つぎに剛体自身の機能を説明します．



### 物性


|*PHSolidDesc*| | 							 |
|---|---|---|
|*double*	|	*mass*	| 質量					|
|*Matrix3d*|	*inertia*| 慣性行列				|
|*Vec3d*	|	*center*	| 質量中心				|
|*bool*	|	*dynamical*| 物理法則にしたがうか	|


|*PHSolidIf*| |
|---|---|
|*double*	| *GetMass()*					|
|*double* | *GetMassInv()*					|
|*void* 	| *SetMass(double)*				|
|*Vec3d* 	| *GetCenterOfMass()*			|
|*void* 	| *SetCenterOfMass(const Vec3d&)*|
|*Matrix3d* | *GetInertia()*					|
|*Matrix3d* | *GetInertiaInv()*				|
|*void* 	| *SetInertia(const Matrix3d&)*	|
|*void* 	| *CompInertia()*				|
|*void* 	| *SetDynamical(bool)*			|
|*bool* 	| *IsDynamical()*				|
- *GetMassInv*と*GetInertiaInv*はそれぞれ質量の逆数と慣性行列の逆行列を返します．

- *CompInertia*は，その剛体が持つ形状とそれらの密度をもとに剛体の質量，質量中心と慣性行列を計算し，設定します．
- *dynamical*は，その剛体が物理法則に従うかどうかを指定するフラグです．もし*dynamical*が*true*の場合，その剛体に加わる力が計算され，ニュートンの運動法則にしたがって剛体の速度が変化します．一方，*dynamical*が*false*の場合は外力による影響を受けず，設定された速度で等速運動します．これはちょうど∞の質量をもつ場合と同じです．

### 状態


|*PHSolidDesc*| | 							 |
|---|---|---|
|*Vec3d*|	*velocity*	| 速度					|
|*Vec3d*|	*angVelocity*| 角速度				|
|*Posed*|	*pose*		| 位置と向き			|


|*PHSolidIf*| |
|---|---|
|*Vec3d*		| *GetVelocity()*					|
|*void* 		| *SetVelocity(const Vec3d&)*		|
|*Vec3d* 		| *GetAngularVelocity()*				|
|*void* 		| *SetAngularVelocity(const Vec3d&)*|
|*Posed* 		| *GetPose()*						|
|*void* 		| *SetPose(const Posed&)*			|
|*Vec3d* 		| *GetFramePosition()*				|
|*void* 		| *SetFramePosition(const Vec3d&)*	|
|*Vec3d* 		| *GetCenterPosition()*				|
|*void* 		| *SetCenterPosition(const Vec3d&)*	|
|*Quaterniond* | *GetOrientation()*					|
|*void* 		| *SetOrientation(const Quaterniond&)*|
- *velocity*, *angVelocity*, *pose*はそれぞれグローバル座標系に関する剛体の速度，角速度，位置および向きを表します．
- *[Get|Set]FramePosition*はグローバル座標系に関する剛体の位置を取得/設定します．これに対して*[Get|Set]CenterPosition*は剛体の質量中心の位置を取得/設定します．偏心している剛体はローカル座標原点と質量中心が一致しないことに注意してください．
- *[Get|Set]Orientation*はグローバル座標系に関する剛体の向きを取得/設定します．

### 力の印加と取得
剛体に加わる力には

-  ユーザが設定する外力
-  重力
-  関節や接触から加わる拘束力

の*3*種類があり，それぞれについて並進力とトルクがあります．ここで，重力は重力加速度と剛体の質量より決まり，拘束力は拘束条件を満たすように内部で自動的に計算されます．以下ではユーザが剛体に加える外力を設定・取得する方法を示します．

|*PHSolidIf*| |
|---|---|
|*void* | *AddForce(Vec3d)*				|
|*void* | *AddTorque(Vec3d)*				|
|*void* | *AddForce(Vec3d, Vec3d)*		|
|*Vec3d* | *GetForce()*					|
|*Vec3d* | *GetTorque()*					|
並進力を加えるには*AddForce*を使います．
```c++
solid->AddForce(Vec3d(0.0, -1.0, 0.0));
```
とすると剛体の質量中心に並進力$(0, -1, 0)$が加わります．ただし力はグローバル座標系で表現されます．一方
```c++
solid->AddTorque(Vec3d(1.0, 0.0, 0.0));
```
とすると剛体の質量中心に関してモーメント$(1, 0, 0)$が加わります．作用点を任意に指定するには
```c++
solid->AddForce(Vec3d(0.0, -1.0, 0.0), Vec3d(0.0, 0.0, 1.0));
```
とします．この場合は並進力*(0, -1, 0)*が作用点*(0, 0, 1)*に加わります．ここで作用点の位置は剛体のローカル座標ではなくグローバル座標で表現されることに注意してください．*AddForce*や*AddTorque*は複数回呼ぶと，それぞれで指定した外力の合力が最終的に剛体に加わる外力となります．

外力を取得するには*GetForce*，*GetTorque*を使います．ただし，これらの関数で取得できるのは直前のシミュレーションステップで剛体に作用した外力です．したがって直前のシミュレーションステップ後に*AddForce*した力は取得できません．





-----



## 関節

拘束とは剛体と剛体の間に作用してその相対的運動に制約を加える要素です．拘束のクラス階層を次図に示します．まず拘束は関節と接触に分かれます．関節はユーザが作成しますが，接触は衝突判定結果にもとづいて自動的に生成・削除されます．関節はさらにいくつかの種類に分けられます．

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/phconstraint.svg) 



細かな説明は後回しにして，まずは関節の作成方法から見ていきます．



### 関節の作成

以下ではもっとも使用頻度の高いヒンジの作成を例にとって関節の作成方法を説明します．ヒンジを作成するには次のようにします．
```c++
PHSolidIf* solid0 = phScene->GetSolids()[0];
PHSolidIf* solid1 = phScene->GetSolids()[1];

PHHingeJointDesc desc;
desc.poseSocket.Pos() = Vec3d( 1.0, 0.0, 0.0);
desc.posePlug.Pos()   = Vec3d(-1.0, 0.0, 0.0);
PHHingeJointIf* joint
    = phScene->CreateJoint(solid0, solid1, desc)->Cast();
```
作成したい関節の種類に応じたディスクリプタを作成し，これを*PHScene*の*CreateJoint*関数に渡して関節を作成します．このとき，ディスクリプタとともに連結したい剛体のインタフェースも渡します．*CreateJoint*は_PHJointIf*_を返しますので，作成した関節のインタフェースを得るには*Cast*で動的キャストします．関節に関する*PHScene*の関数を以下に示します．

|*PHSceneIf*| | 													 |
|---|---|---|---|
|_PHJointIf*_| *CreateJoint(PHSolidIf*, PHSolidIf*, const PHJointDesc&)*| |
|*int*	| *NJoint()*												| |
|_PHJointIf*_| *GetJoint(int i)*										| |
*NJoint*はシーン中の関節の個数を返します．*GetJoint*は*i*番目の関節を取得します．



### ソケットとプラグ

さて，上の例でディスクリプタに値を設定している箇所に注目してください．この部分で関節の取り付け位置を指定しています．

Springheadでは，ソケットとプラグと呼ばれるローカル座標系を用いて関節の取り付け位置を表現します．ソケットとプラグとは，その名前から連想するように，連結する剛体に取り付ける金具のようなものです．*CreateJoint*の第*1*引数の剛体にソケットがつき，第*2*引数の剛体にプラグがつきます．

ソケットとプラグがそれぞれの剛体のどの位置に取り付けられるかを指定するのがディスクリプタの*poseSocket*と*posePlug*です．上の例ではソケットの位置が*(1,0,0)*，プラグの位置が*(-1,0,0)*でした．これはソケットとプラグを次図のように設定することを意味します。![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/socket_plug1.svg) 



このようにソケットとプラグを設定すると、次のように剛体同士が連結されます．
 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/socket_plug2.svg) 

後述するように，ヒンジはソケットとプラグのz軸を一致させる拘束です．したがって連結された剛体同士はソケットとプラグのz軸を回転軸として相対的に回転することができます．

ソケットとプラグに関するディスクリプタとインタフェースを紹介します．

|*PHConstraintDesc*| | 					 |
|---|---|---|
|*Posed*|	*poseSocket*| ソケットの位置と向き	|
|*Posed*|	*posePlug*| プラグの位置と向き	|


|*PHConstraintIf*| | 								 |
|---|---|---|
|_PHSolidIf*_| *GetSocketSolid()*						| ソケット側の剛体 |
|_PHSolidIf*_| *GetPlugSolid()*						| プラグ側の剛体 |
|*void* 	| *GetSocketPose(Posed&)*				| |
|*void* 	| *SetSocketPose(const Posed&)*			| |
|*void* 	| *GetPlugPose(Posed&)*					| |
|*void* 	| *SetPlugPose(const Posed&)*			| |
|*void* 	| *GetRelativePose(Posed&)*				| 相対的な位置と向き |
|*void* 	| *GetRelativeVelocity(Vec3d&, Vec3d&)*| 相対速度 |
|*void* 	| *GetConstraintForce(Vec3d&, Vec3d&)*	| 拘束力 |
- *GetRelativePose*はソケット座標系から見たプラグ座標系の相対的な位置と向きを取得します．同様に，*GetRelativeVelocity*はソケットからみたプラグの相対速度をソケット座標系で取得します．ここで第*1*引数が並進速度，第*2*引数が角速度です．
- *GetConstraintForce*はこの拘束が剛体に加えた拘束力を取得します(第*1*引数が並進力，第*2*引数がモーメント)．具体的には，ソケット側剛体に作用した拘束力をソケット座標系で表現したものが得られます．プラグ側剛体には作用反作用の法則によって逆向きの力が作用しますが，これを直接取得する関数は用意されていません．



### 関節の種類
Springheadで使用可能な関節の種類は

-  ヒンジ (*PHHingeIf*)
-  スライダ (*PHSliderIf*)
-  パスジョイント (*PHPathJointIf*)
-  ボールジョイント (*PHBallJointIf*)
-  バネ (*PHSpringIf*)

の5種類です．種類ごとに，自由度・拘束の仕方・変位の求め方が異なります．



#### ヒンジ

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/phhingejoint.svg) 

ヒンジは*1*軸回転関節です．ヒンジは，上の図に示すようにソケットとプラグのz軸が一致するように拘束します．このときソケットのy軸とプラグのy軸の成す角(x軸同士でも同じことですが)が関節変位となります．関節変位を取得するAPIは*1*自由度関節(*PH1DJointIf*)で共通です．そのためヒンジに限らずスライダ・パスジョイントでも使用できます．

|PH1DJointIf||
|---|---|
| _double GetPosition()_ | 関節の変位を取得します．変位の測り方は関節の種類（Hinge, Slider, PathJoint）に依存します． |



#### スライダ
 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/phsliderjoint.svg) 

スライダは*1*自由度の直動関節です．スライダは，上の図に示すようにソケットとプラグのz軸が同一直線上に乗り，かつ両者のx軸，y軸が同じ向きを向くように拘束します．このときソケットの原点からプラグの原点までが関節変位となります．



#### パスジョイント

パスジョイントはソケットとプラグの相対位置関係を*1*パラメータの自由曲線で表現する関節です．詳細は後述します．



#### ボールジョイント

|(a)|(b)|
|---|---|
|![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/phballjoint.svg)|![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/swingtwist.svg) |

ボールジョイントは*3*自由度の回転関節です．ボールジョイントは上図(a)に示すようにソケットとプラグの原点が一致するように拘束します．ソケット座標系をプラグ座標系に変換するようなクォータニオンが変位となります．

一方で，ボールジョイントの変位はオイラー角の一種であるSwing-Twist座標系で取得することもできます（上図(b)）．ソケットとプラグのz軸同士がなす角をスイング角(Swing)，プラグのz軸をソケットのx-y平面への射影がソケットのx軸となす角をスイング方位角(Swing-Dir)，プラグのz軸周りの回転角度をツイスト角(Twist)と呼びます．Swing-Twist座標系は，後述するボールジョイントの関節可動範囲の指定に用います．この2種類の変位は，それぞれに対応した関数で取得することができます．

|PHBallJointIf||
|---|---|
| _Quaterniond GetPosition()_ | ソケット座標系をプラグ座標系に変換するようなクォータニオンを返します． |
|_Vec3d GetAngle()_| Swing-Twist座標系で表現された関節変位を返します． |



#### バネ

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/phspring.svg) 

剛体間を連結するダンパ付きバネです．ソケット座標系とプラグ座標系が一致するときが自然状態で，位置の変位・姿勢の変位に比例して自然状態に戻すような力・モーメントを発生します．並進運動に作用するバネ・ダンパ係数と，回転運動に作用するバネ・ダンパ係数はディスクリプタによってそれぞれ設定できます．

|PHSpringDesc||
|---|---|
|_Vec3d spring_|並進運動に対するバネ係数|
|_Vec3d damper_|並進運動に対するダンパ係数|
|_double springOri_|回転運動に対するバネ係数|
|_double damperOri_|回転運動に対するダンパ係数|



### 有効化と無効化


|*PHConstraintDesc*| | 					 |
|---|---|---|
|*bool*|	*bEnabled*| 有効/無効フラグ		|


|*PHConstraintIf*| | 						 |
|---|---|---|
|*void*| *Enable(bool)*				| |
|*bool* | *IsEnabled()*				| |
有効な拘束は拘束力を生じます．無効化された拘束は存在しないのと同じ状態になりますが，削除するのと異なりいつでも再度有効化することができます．作成直後の拘束は有効化されています．





### 関節制御

#### *1*自由度関節の場合


|*PHJoint1DDesc*| | 								 |
|---|---|---|
|*double*|	*spring*		| 可動範囲下限				|
|*double*|	*damper*		| 可動範囲上限				|
|*double*|	*targetPosition*| 可動範囲制限用バネ係数	|
|*double*|	*targetVelocity*| 可動範囲制限用ダンパ係数	|
|*double*|	*offsetForce*| |
|*double*|	*fMax*		| |

| PHJoint1DIf                                        |      |
| -------------------------------------------------- | ---- |
| _double GetPosition()_ | 関節変位を取得            |
| _double GetVelocity()_ | 関節速度を取得            |
| _void SetSpring(double)_                        |      |
| _double GetSpring()_                            |      |
| _void SetDamper(double)_                        |      |
| _double GetDamper()_                            |      |
| _void SetTargetPosition(double)_                |      |
| _double GetTargetPosition()_                    |      |
| _void SetTargetVelocity(double)_               |      |
| _double GetTargetVelocity()_                    |      |
| _void SetOffsetForce(double)_                   |      |
| _double GetOffsetForce()_                       |      |
| _void SetTorqueMax(double)_ | 最大関節トルクを設定 |
| _double GetTorqueMax()_ | 最大関節トルクを取得     |



関節を駆動する力*f*は次式で与えられます．

$$
f = K(p_0 - p) + D(v_0 - v) + f_0
$$

ここで*p*，*v*はそれぞれ関節変位と関節速度で*GetPosition*，*GetVelocity*で取得できます．その他の記号とディスクリプタ変数との対応は以下の通りです．

| 記号  | 変数             |
|---|---|
|*K*	|	*spring*				|
|*D*	|	*damper*				|
|*p_0*|	*targetPosition*		|
|*v_0*|	*targetVelocity*		|
|*f_0*|	*offsetForce*|
上の式はバネ・ダンパモデルとPD制御則の二通りの解釈ができます．前者としてとらえるなら*K*はバネ係数，*D*はダンパ係数，*p_0*はバネの自然長，*v_0*は基準速度となります．後者としてとらえる場合は*K*はPゲイン，*D*はDゲイン，*p_0*は目標変位，*v_0*は目標速度となります．また，*f_0*は関節トルクのオフセット項です．上の式で得られた関節トルクは最後に*±fMax*の範囲に収まるようにクランプされます．



#### ボールジョイントの場合

ヒンジと同様に，バネダンパモデル・PD制御を実現します．ボールジョイントの変位はクォータニオンで表されるため，目標変位*targetPosition*はクォータニオンで，目標速度*targetVelocity*は回転ベクトルで与えます．

| PHBallJointDesc                       |      |
| ------------------------------------- | ---- |
| _double spring_ | バネ係数              |      |
| _double damper_ | ダンパ係数            |      |
| _Quaterniond targetPosition_ | 目標変位 |      |
| _Vec3d targetVelocity_ | 目標速度       |      |
| _Vec3d offsetForce_ | モータートルク    |      |
| _double fMax_ | 関節トルクの限度        |      |





### 可動域制限
*CreateLimit*は可動範囲制約オブジェクトのディスクリプタを引数にとります．*1*自由度関節の可動範囲制約の場合，*Vec2d range*が可動域を表します．*range[0]*が可動域の下限，*range[1]*が上限です．*range[0] < range[1]*が満たされているときに限り可動範囲制約が有効となります．デフォルトでは*range[0] > range[1]*となる値が設定されていて，可動範囲制約は無効となっています．

関節の変位が可動範囲限界に到達したとき，範囲を超過しないように可動範囲制約の拘束力が作用します．このとき，関節変位を範囲内に押し戻す力はバネ・ダンパモデルで計算されます．このバネ係数とダンパ係数はそれぞれディスクリプタの*spring*，*damper*で指定します．

> 可動範囲用の*spring*，*damper*は初期値でも十分大きな値が設定されていますが，関節制御において非常に大きなバネ・ダンパ係数を用いると可動範囲制約のバネ・ダンパが負けてしまうことがあります．その場合には関節制御より大きな係数を適切に再設定すると，可動範囲内で関節を制御する事ができるようになります．



#### *1*自由度関節の場合

|PH1DJointLimitDesc||
|---|---|
|_Vec2d range_|可動範囲を表します．range[0]が下限，range[1]が上限です．|
|_double spring_|可動範囲を制限するためのバネ・ダンパモデルの係数です．|
|_double damper_|（同上）|

|PH1DJointLimitIf||
|---|---|
|_bool IsOnLimit()_|現在の関節姿勢が稼働範囲外にあるときに_true_を返します．この関数が_true_を返すようなとき，関節には可動域制約を実現するための拘束力が発生しています．|



#### ボールジョイントの場合

ボールジョイントの可動範囲は先の図で示したSwing-Twist座標系によって指定します．ボールジョイントに対しては2種類の可動範囲制約を使用することができます．

-  *ConeLimit*は円錐形の可動範囲制約で，主に関節のスイング角を一定範囲内に制約します．
-  *SplineLimit*は自由曲線形の可動範囲制約で，プラグ座標系z軸の可動範囲を閉曲線で指定することができます．

ここでは*ConeLimit*について説明します(*SplineLimit*については後述します)．

|PHBallJointConeLimitDesc||
|---|---|
|_Vec2d limitSwing_|スイング角の可動範囲です．概念的には，関節が一定以上に折れ曲がらないようにする制約です(スイング角の下限を設定する事もできるので，実際には一定以上にまっすぐにならないようにする機能も有しています)．limitSwing[0]が下限，limitSwing[1]が上限です．<br>limitSwing[0] >limitSwing[1]となる時は無効化されます．デフォルトではlimitSwing[0] >limitSwing[1]となる値がセットされています．|
|_Vec2d limitTwist_|ツイスト角の可動範囲です．概念的には，関節が一定以上にねじれないようにするための制約です．limitTwist[0]が下限，limitTwist[1]が上限です．<br>limitTwist[0] >limitTwist[1]となる時は無効化されます．デフォルトではlimitTwist[0] >limitTwist[1]となる値がセットされています．|
|_double spring_|可動範囲を制限するためのバネ・ダンパモデルの係数です．1自由度関節の場合と同じです．|
|_double damper_|（同上）|

|PHBallJointConeLimitIf||
|---|---|
|_IsOnLimit()_|現在の関節姿勢が可動範囲外にある時にtrueを返します．1自由度関節の場合と同じです．|
|_PHBallJointConeLimitIf::\[Set\|Get\]SwingRange(range)_|limitSwingを取得・設定します．|
|_PHBallJointConeLimitIf::[Set\|Get]TwistRange(range)_|limitTwistを取得・設定します．|



### ボールジョイントの自由曲線可動域 

T.B.D.



### パスジョイント 

T.B.D.



### 弾塑性変形バネダンパ

T.B.D.





-----



## 関節系の逆運動学
逆運動学(IK)は，剛体関節系において剛体が目標位置に到達するよう関節を制御する機能です．

Springheadでは，関節系のヤコビアンを用いたIK機能が使用可能です．物理シミュレーションの1ステップごとに関節系のヤコビアンを計算し，それに基づいて剛体を目標位置・姿勢に近づけるような各関節の角速度を計算します．シミュレーションを続けることで，最終的に剛体が目標位置・姿勢となった状態が得られます．



Springhead上の剛体関節系に対してIKを使用するには，少々下準備が必要です．次のように3つの剛体が直線状につながった関節系を例にとって解説します．

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/ikexample3link.svg) 



IKを使用するには，まずIKに用いるための関節を「アクチュエータ」として登録する必要があります．

```c++
// given PHSceneIf* phScene
// given PHSolidIf* solid1, solid2, solid3
// given PHHingeJointIf* joint1 (solid1 <-> solid2)
// given PHHingeJointIf* joint2 (solid2 <-> solid3)

PHIKHingeActuatorDesc descIKActuator;

PHIKHingeActuatorIf* ikActuator1
  = phScene->CreateIKActuator(descIKActuator);
ikActuator1.AddChildObject(joint1);

PHIKHingeActuatorIf* ikActuator2
  = phScene->CreateIKActuator(descIKActuator);
ikActuator1.AddChildObject(joint2);
```
*PHIKHingeActuatorIf*は*PHHingeJointIf*に対応するアクチュエータクラスです．



次に，関節系の親子関係を登録します．親アクチュエータに，子アクチュエータを登録します．

```c++
ikActuator1.AddChildObject(ikActuator2);
```


また，IKを用いて到達させる先端の剛体を「エンドエフェクタ」として登録する必要があります．

```c++
PHIKEndEffectorDesc descEndEffector;

PHIKEndEffectorIf* ikEndEffector1
  = phScene->CreateIKEndEffector(descEndEffector);
ikEndEffector1.AddChildObject(solid3);
```


最後に，剛体関節系の親子関係において，エンドエフェクタの直接の親にあたるアクチュエータに対し，エンドエフェクタを登録します．

```c++
ikActuator2.AddChildObject(ikEndEffector1);
```


この例では *solid1 -(joint1)-> solid2 -(joint2)-> solid3* のように関節が接続されていますから，関節系の末端である *solid3* をエンドエフェクタにした場合，直接の親にあたるアクチュエータは *joint2* に対応するアクチュエータ，すなわち *ikActuator2* ということになります．ここまでの作業で，生成されたオブジェクトの関係は以下のようになっているはずです．

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/ikexample3linkobjects.svg) 



また、IKエンジンは、計算結果に従って関節系を駆動するために関節のバネダンパを使用します。各関節にはあらかじめ適切なバネダンパ係数を設定しておく必要があります。

```c++
joint1->SetSpring(1000); joint1->SetDamper(100);
joint2->SetSpring(1000); joint2->SetDamper(100);
```



これで下準備は終わりです．目標位置をセットし，IKエンジンを有効にするとIKが動き始めます．

```c++
// solid3 goes to (2, 5, 0)
ikEndEffector1->SetTargetPosition(Vec3d(2, 5, 0)); 

phScene->GetIKEngine()->Enable(true);

...
phScene->Step(); // IK is calculated in physics step
...
```



### IKエンジン

IKの計算は，*PHScene*が持つIKエンジン(*PHIKEngine*)によって実現されています．IKエンジンはデフォルトでは無効となっています．
```c++
phScene->GetIKEngine()->Enable(true);
```
を実行することで有効となります．*GetIKEngine()*は，*PHScene*が持つIKエンジンを取得するAPIです．



SpringheadにおけるIKの計算原理は，関節系のヤコビ行列（ヤコビアン）に基づきます．全アクチュエータの関節角度に微小変化量*Δθ*を与えた時の，全エンドエフェクタの位置の微小変化量 *Δr* は，関節系のヤコビアン *J* を用いて
$$
J \varDelta \theta = \varDelta r
$$
と表されます．毎ステップごとに関節系ヤコビアン*J*および目標位置に向かう微小変位*Δr*を計算し，上記の線形連立方程式の解*Δθ*を求めることで各関節に与える角速度を求めます．



### アクチュエータ

Springheadでは，IKに使用する各関節をアクチュエータと呼びます．IKは，アクチュエータを駆動させて剛体を目標位置に到達させます．IKエンジンはアクチュエータを複数保持し，各アクチュエータが各関節を保持します．アクチュエータオブジェクト一つにつき，関節が一つ対応します．アクチュエータオブジェクトの具体的な役割は，関節の状態をIKエンジンに伝え，IKの計算のうち関節ヤコビアンの計算など関節ごとに行う部分を実行し，IKの計算結果に従って関節を動かす事です．
#### アクチュエータクラスの種類と作成
本稿執筆時点では，IK用アクチュエータとして使用できるのはヒンジとボールジョイントのみです．それぞれに対応したアクチュエータクラスがあります．

-  *PHIKHingeActuator*は*PHHingeJoint*に対するアクチュエータです．ヒンジジョイントの1自由度を駆動に用います．

-  *PHIKBallActuator*は*PHBallJoint*に対するアクチュエータです．ボールジョイントは3自由度の関節ですが，後述するエンドエフェクタの姿勢制御を行わない(エンドエフェクタの位置のみを制御する)場合は，エンドエフェクタの位置を変化させることのできる2自由度のみを駆動に用います(使用する2自由度の軸は1ステップごとに更新されます)．


```c++
// given PHSceneIf* phScene

PHIKHingeActuatorDesc descIKActuator;
PHIKHingeActuatorIf* ikActuator
    = phScene->CreateIKActuator(descActuator);
```
アクチュエータを作成するには，*PHSceneIf*の*CreateIKActuator*関数を用います．引数はアクチュエータのディスクリプタです．*PHIKHingeActuatorDesc*型のディスクリプタを渡すとヒンジ用のアクチュエータが作成され，*PHIKBallActuatorDesc*型のディスクリプタを渡すとボールジョイント用のアクチュエータが作成されます．作成された時点では，アクチュエータは関節と対応付けがされていません．アクチュエータの子要素に関節を登録することで対応付けが行われます．
```c++
// given PHHingeJointIf* joint
ikActuator->AddChildObject(joint);
```

#### アクチュエータの親子関係の登録
次図のように二股に分岐したリンクを例にとります：

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/ikexample5link.svg) 

計算上，IKで駆動する関節系は木構造でなければなりません．Springheadでは，アクチュエータの親子関係を作ることで関節の木構造を設定します．

```c++
// given PHIKActuator ikActuator1, ikActuator2
ikActuator1->AddChildObject(ikActuator2);
```
*AddChildObject*を呼び出すと，アクチュエータに対し「子要素」となるアクチュエータを登録することができます．これを全てのアクチュエータに対して行うことでアクチュエータの木構造が設定されます．このときアクチュエータの親子関係は，前出の図の右側のようになります．

#### 関節のダンパ係数とIK
関節の運動は，IK機能によって計算された目標関節角度を_SetTargetPosition_、および目標関節角速度を*SetTargetVelocity*することで実現します．目標速度に関する関節の振る舞いは，関節の*spring / damper*パラメータによって変化します．一般に*spring / damper*が大きいほど関節は固くなり，外乱の影響を受けづらくなります．この性質はそのままIKの振る舞いにも受け継がれます．
#### 重み付きIK
通常，IKは全ての関節を可能な限り均等に使用して目標を達成するよう計算されます．一方，キャラクタの動作に用いる場合などで，手先を優先的に動かし胴体はあまり動かさない，といった重み付けが要求される場面があります．SpringheadのIKには，このような重み付けを設定することができます．
```c++
// given PHIKActuator ikActuator1, ikActuator2
ikActuator1->SetBias(2.0);
ikActuator2->SetBias(1.0);
```
*SetBias*は，指定した関節をあまり動かさないように設定する関数です．Biasには1.0以上の値を設定します．大きな値を設定した関節ほど，IKによる動作は小さくなります．デフォルトではどのアクチュエータも1.0となっており，全関節が均等に使用されます．



### エンドエフェクタ

剛体・関節系を構成する剛体の一部を，「エンドエフェクタ」に指定することができます．エンドエフェクタには目標位置・姿勢を指示することができます．IKエンジンは，エンドエフェクタ剛体が指定された目標位置・姿勢を達成するようアクチュエータを制御します．
#### エンドエフェクタの作成
エンドエフェクタは*PHSceneIf*の*CreateIKEndEffector*を用いて作成します．引数には*PHIKEndEffectorDesc*を渡します．
```c++
// given PHSceneIf* phScene

PHIKEndEffectorDesc descEndEffector;

PHIKEndEffectorIf* ikEndEffector
  = phScene->CreateIKEndEffector(descEndEffector);
```
アクチュエータ同様，エンドエフェクタも作成時点では剛体との対応を持ちません．*AddChildObject*により剛体を子要素として登録する必要があります．
```c++
// given PHSolidIf* solid

ikEndEffector.AddChildObject(solid);
```
次に，エンドエフェクタ剛体を親剛体に連結しているアクチュエータに対し，エンドエフェクタを子要素として登録します．
```c++
// given PHIKActuatorIf* ikActuatorParent

ikActuatorParent.AddChildObject(ikEndEffector);
```
こうすることでエンドエフェクタはアクチュエータ木構造の葉ノードとなり，IKの計算に使用できるようになります．なお，エンドエフェクタは一つの関節系に対して複数作成することができます．この場合，IKは複数のエンドエフェクタが可能な限り同時に目標位置・姿勢を達成できるようアクチュエータを制御します．また，エンドエフェクタは関節系の先端剛体に限りません．

#### 目標位置の設定

エンドエフェクタの目標位置は*SetTargetPosition*によって指定します．
```c++
// solid3 goes to (2, 5, 0)
ikEndEffector->SetTargetPosition(Vec3d(2, 5, 0)); 
```
エンドエフェクタに目標姿勢を指示し，エンドエフェクタが特定の姿勢をとるように関節系を動作させることもできます．
```c++
ikEndEffector->SetTargetOrientation( Quaterniond::Rot('x', rad(30)) ); 
ikEndEffector->EnableOrientationControl(true);
```
目標姿勢は*Quaterniond*で設定します．姿勢制御はデフォルトでは無効になっており，使用するには*EnableOrientationControl*を呼んで有効化する必要があります．*EnablePositionControl*および*EnableOrientationControl*を用いると，位置制御・姿勢制御の両方を個別に有効・無効化することができます．
```c++
// 位置制御あり，姿勢制御なし（デフォルト）
ikEndEffector->EnablePositionControl(true);
ikEndEffector->EnableOrientationControl(false);
```

```c++
// 位置制御なし，姿勢制御あり
ikEndEffector->EnablePositionControl(false);
ikEndEffector->EnableOrientationControl(true);
```

```c++
// 位置制御あり，姿勢制御あり
ikEndEffector->EnablePositionControl(true);
ikEndEffector->EnableOrientationControl(true);
```





-----





## 接触



### 接触モデル

Springheadで採用している接触モデルについて説明します．[シーン](#シーン)の項目で述べたように，*PHSceneIf::Step*によってシミュレーションを1ステップ進めると，初めに形状の交差判定と接触拘束の生成が行われます．

交差する二つの形状の交差断面と，接触拘束の関係について次図に示します．

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/phcontact.svg) 

図では簡単のために二次元で描いていますが，実際には接触断面を表す多角形の各頂点に接触拘束が作られます．接触拘束も他の拘束と同様にソケットとプラグで構成されます．一方で，他の拘束とは違い接触拘束は交差判定アルゴリズムによって動的に生成・破棄されます．このため，接触し合う剛体のどちらにソケットあるいはプラグが取り付けられるかは状況依存であり，外部から選択することはできません．

プラグおよびソケットの向きは次のようにして決まります．まず，x軸は接触法線と平行に向きます．ただしどちらが正の向きかは状況依存です．次に，y軸は接触点における二つの剛体の相対速度ベクトルを接触断面へ投影した向きに向きます．最後にz軸はx，y軸に直交するように決まります．



以下では各接触拘束が課す条件について具体的に述べます．まず，法線方向の進入速度の大小に応じて衝突モデルと静的接触モデルのいずれかが選択されます．

$$
v^\mathrm{x} \lt -V^\mathrm{th}　\Rightarrow　　　衝突モデル \\
v^\mathrm{x} \geq -V^\mathrm{th}　\Rightarrow　静的接触モデル
$$

ここで$$v^\mathrm{x}$$はソケットから見たプラグの相対速度のx軸（接触法線）成分で，近づき合う向きを負とします．また，$$V^\mathrm{th}$$は衝突モデルへ切り替わる臨界速度です．

衝突モデルでは，1ステップ後の相対速度$$v^\mathrm{x}\prime$$が跳ね返り係数*e*にもとづいて決まり，それを満たすような接触力が計算されます．


$$
{v^\mathrm{x}}' = - e \, v^\mathrm{x}
$$


ここで，跳ね返り係数は衝突する形状の物性値に定義された跳ね返り係数の平均値です．

静的接触モデルでは，形状同士の進入深度*d*が1ステップで所定の割合で減少するような接触力を求めます．
つまり，1ステップ後の進入深度を*d'*とすると


$$
d' = d - \gamma \mathrm{max}(d - d^\mathrm{tol}, 0)
$$


となります．
ここで*γ*は接触拘束の誤差修正率です．また，$$d^\mathrm{tol}$$は許容進入深度です．

最後に，接触力が満たすべき条件について述べます．まず，法線方向には反発力のみ作用することから，接触力のx軸成分$$f^\mathrm{x}$$には


$$
f^\mathrm{x} \ge 0
$$


が課せられます．

一方で接触力のy軸成分$$f^\mathrm{y}$$，z軸成分$$f^\mathrm{z}$$は摩擦力を表します．摩擦力に関しては，その向きの相対速度にもとづき静止摩擦か動摩擦かが判定され，それに応じて最大摩擦力の制約が課されます．


$$
\begin{align*}
-\mu_0 f^\mathrm{x} \le &f^\mathrm{y} \le \mu_0 f^\mathrm{x} & & \text{if} \; -V^\mathrm{f} \le v^\mathrm{y} \le V^\mathrm{f},\\
 \mu   f^\mathrm{x} \le &f^\mathrm{y} \le \mu   f^\mathrm{x} & & \text{otherwise}
\end{align*}
$$


ここで，静止摩擦係数$$\mu_0$$および動摩擦係数$$\mu$$は跳ね返り係数と同様に各形状の物性値の平均値が用いられます．また，$$V^\mathrm{f}$$は静止摩擦と動摩擦が切り替わる臨界速度です．z軸方向についても同様の制約が課されます．



接触モデルの関係するインタフェースには以下があります．

| CDShapeIf |                              |                    |
| --------- | ---------------------------- | ------------------ |
| void      | SetElasticity(float e)       | 跳ね返り係数を設定 |
| float     | GetElasticity()              | 跳ね返り係数を取得 |
| void      | SetStaticFriction(float mu0) | 静摩擦係数を設定   |
| float     | GetStaticFriction()          | 静摩擦係数を取得   |
| void      | SetDynamicFriction(float mu) | 動摩擦係数を設定   |
| float     | GetDynamicFriction()         | 動摩擦係数を取得   |

| PHSceneIf |                                 |                      |
| --------- | ------------------------------- | -------------------- |
| void      | SetContactTolerance(double tol) | 許容交差深度を設定   |
| double    | GetContactTolerance()           | 許容交差深度を取得   |
| void      | SetImpactThreshold(double vth)  | 最小衝突速度を設定   |
| double    | GetImpactThreshold()            | 最小衝突速度を取得   |
| void      | SetFrictionThreshold(double vf) | 最小動摩擦速度を設定 |
| double    | GetFrictionThreshold()          | 最小動摩擦速度を取得 |



**備考**

-  接触断面の向きについては，形状同士の進入速度をもとに決定しますが，ここでは詳しく述べません．
-  摩擦力に関してはy軸，z軸が個別に扱われますが，実際の摩擦力はy成分とz成分の合力として与えられますので，合力が最大摩擦力を超過する可能性があります．このようにSpringheadの摩擦モデルはあくまで近似的なものですので注意して下さい．



### 接触力の取得

特定の剛体に作用する接触力を直接取得するためのインタフェースは用意されていません．このため，ユーザサイドである程度の計算を行う必要があります．以下に，ある剛体に作用する接触力の合力を求める例を示します．
```c++
// given PHSceneIf* scene
// given PHSolidIf* solid

Vec3d fsum;    //< sum of contact forces applied to "solid"
Vec3d tsum;    //< sum of contact torques applied to "solid"

int N = scene->NContacts();
Vec3d f, t;
Posed pose;

for(int i = 0; i < N; i++){
    PHContactPointIf* con = scene->GetContact(i);
    con->GetConstraintForce(f, t);

    if(con->GetSocketSolid() == solid){
        con->GetSocketPose(pose);
        fsum -= pose.Ori() * f;
        tsum -= pose.Pos() % pose.Ori() * f;
    }
    if(con->GetPlugSolid() == solid){
        con->GetPlugPose(pose);
        fsum += pose.Ori() * f;
        tsum += pose.Pos() % pose.Ori() * f;
    }
}
```
まず，シーン中の接触拘束の数を*PHSceneIf::NConstacts*で取得し，*for*ループ中で*i*番目の接触拘束を*PHSceneIf::GetContact*で取得します．次に*PHConstraintIf::GetConstraintForce*で接触力の並進力*f*とモーメント*t*を取得しますが，接触拘束の場合モーメントは*0*ですので用いません．また，得られる拘束力はソケット/プラグ座標系で表したもので，作用点はソケット/プラグ座標系の原点です．これを考慮して剛体に作用する力とモーメントへ変換し，合力に足し合わせていきます．剛体がソケット側である場合は作用・反作用を考慮して符号を反転することに注意して下さい．





### 接触力計算の有効/無効の切り替え
多くのアプリケーションでは，すべての剛体の組み合わせに関して接触を取り扱う必要はありません．このような場合は必要な剛体の対に関してのみ接触を有効化することで計算コストを削減できます．Springheadでは，剛体の組み合わせ毎に交差判定および接触力計算を行うかを切り替えることができます．これには*PHSceneIf::SetContactMode*を用います．

| PHSceneIf |                                                            |
| --------- | ---------------------------------------------------------- |
| void      | SetContactMode(PHSolidIf* lhs, PHSolidIf* rhs, int mode)   |
| void      | SetContactMode(PHSolidIf** group, size_t length, int mode) |
| void      | SetContactMode(PHSolidIf* solid, int mode)                 |
| void      | SetContactMode(int mode)                                   |

一番目は剛体*lhs*と*rhs*の対に関してモードを設定します．二番目は配列*[group, group + length)*に格納された剛体の全組み合わせに関して設定します．三番目は剛体*solid*と他の全剛体との組み合わせに関して設定します．四番目はシーン中のすべての剛体の組み合わせに関して設定します．設定可能なモードは以下の内の一つです．

| PHSceneDesc::ContactMode |                                          |
| ------------------------ | ---------------------------------------- |
| MODE_NONE                | 交差判定および接触力計算を行わない       |
| MODE_LCP                 | 交差判定を行い，拘束力計算法を用いる     |
| MODE_PENALTY             | 交差判定を行い，ペナルティ反力法を用いる |

デフォルトではすべての剛体対に関して*MODE\_LCP*が選択されています．例として，床面との接触以外をすべてオフにするには

```c++
// given PHSolidIf* floor

scene->SetContactMode(PHSceneDesc::MODE_NONE);
scene->SetContactMode(floor, PHSceneDesc::MODE_LCP);
```
とします．





## 関節座標系シミュレーション
T.B.D.





## ギア
T.B.D.





## 内部アルゴリズムの設定
以下では物理シミュレーションの内部で用いられているアルゴリズムの詳細な設定項目について説明します．
### 拘束力計算エンジン
拘束力計算エンジンは，関節や接触などの拘束を満足するための拘束力の計算を行います．拘束力計算エンジンのクラスは*PHConstraintEngineIf*で，これを取得するには以下の関数を用います．*PHConstraintEngineIf*のインタフェースを以下に示します．

| PHConstraintEngineIf |                                  |                            |
| -------------------- | -------------------------------- | -------------------------- |
| void                 | SetVelCorrectionRate(double)     | 関節拘束の誤差修正率を設定 |
| double               | GetVelCorrectionRate()           | 関節拘束の誤差修正率を取得 |
| void                 | SetContactCorrectionRate(double) | 接触拘束の誤差修正率を設定 |
| double               | GetContactCorrectionRate()       | 接触拘束の誤差修正率を取得 |

誤差修正率とは，1ステップで拘束誤差どの程度修正するかを示す比率で，通常$[0, 1]$の値を設定します．誤差修正率を*1*にすると，1ステップで拘束誤差を*0*にするような拘束力が計算されますが，発振現象などのシミュレーションの不安定化を招く傾向があります．逆に修正率を小さ目に設定すればシミュレーションは安定化しますが，定常誤差が増大します．拘束力計算エンジンは，内部で反復型のアルゴリズムで拘束力を計算します．アルゴリズムの反復回数は*PHSceneIf::SetNumIteration*で設定します．

*PHSceneIf::SetContactTolerance*で設定可能です．

*PHConstraintEngineIf::SetContactCorrectionRate*で設定可能です．