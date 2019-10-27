# Collision

Collisionモジュールは物理計算の基礎となる衝突判定機能を提供します．事実上CollisionモジュールはPhysicsモジュールのサブモジュールとなっており，両者は密接に依存しています．ユーザは主として剛体に衝突判定用形状を割り当てる際にCollisionモジュールの機能を利用することになります．

Collisionモジュールのクラス階層を次図に示します．衝突判定形状はすべて`CDShape`から派生します．アルゴリズムの性質上，形状はすべて凸形状でなければなりません．

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/cdclass.svg) 



## 形状の作成
衝突判定形状は次の手順で作成・登録します．

1.  形状を作成する
1.  剛体へ形状を追加する
1.  形状の位置を設定する

以下に順を追って説明します．まず形状を作成するには次のようにします．
```c++
// given PHSdkIf* phSdk

CDBoxDesc desc;
desc.boxsize = Vec3d(1.0, 1.0, 1.0);

CDBoxIf* box = phSdk->CreateShape(desc)->Cast();
```
衝突判定形状のオブジェクトはPhysicsモジュールが管理します．このため，形状を作成するには`PHSdk`クラスの`CreateShape`関数を使います．`PHSdk`については\ref{chap_physics}章を参照してください．形状を作成するには，まず種類に応じたディスクリプタを作成し，寸法などのパラメータを設定します．この例では直方体クラス`CDBox`のディスクリプタを作成して一辺が$1.0$の立方体を作成します．ディスクリプタを指定して`CreateShape`を呼び出すと，対応する種類の形状が作成され，そのインタフェースが返されます．ただし戻り値は形状の基底クラスである`CDShape`のインタフェースですので，派生クラス（ここでは`CDBox`）のインタフェースを得るには上のように`Cast`関数で動的キャストする必要があります．形状を作成したら，次にその形状を与えたい剛体に登録します．
```c++
// given PHSolidIf* solid

solid->AddShape(box);         // first box
```
剛体クラス`PHSolid`については[Physics](05_physics.html)の項目を参照してください．ここで重要なことは，一度作成した形状は1つの剛体にいくつでも登録でき，また異なる複数の剛体にも登録できるということです．つまり，同じ形状を複数の剛体間で共有することで，形状の作成コストやメモリ消費を抑えることができます．`AddShape`関数で登録した直後の形状は，剛体のローカル座標系の原点に位置しています．これを変更したい場合は`SetShapePose`関数を使います．
```c++
solid->AddShape(box);         // second box
solid->AddShape(box);         // third box 

// move first shape 1.0 in x-direction
solid->SetShapePose(0, Posed(Vec3d(1.0, 0.0, 0.0), Quaterniond());

// rotate second shape 30 degrees along y-axis
solid->SetShapePose(1, Posed(Vec3d(),
                    Quaterniond::Rot(Rad(30.0), 'y')));
```
`SetShapePose`の第1引数は操作する形状の番号です．最初に`AddShape`した形状の番号が*0*で，`AddShape`するたびに*1*増加します．形状の位置や向きは剛体のローカル座標系で指定します．また，形状の位置・向きを取得するには`GetShapePose`関数を使います．



以下ではSpringheadでサポートされている形状を種類別に解説します．



### 直方体

直方体のクラスは*CDBox*です．

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/cdbox.svg) 

|*CDBoxDesc*| | 					 |
|---|---|---|
|*Vec3f*|	*boxsize*| 各辺の長さ 	|

|*CDBoxIf*| |
|---|---|
|*Vec3f GetBoxSize()*| 			|
|*void SetBoxSize(Vec3f)*| 		|



### 球

球のクラスは`CDSphere`です．

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/cdsphere.svg) 

|*CDSphereDesc*| | 				 |
|---|---|---|
|*float*|	*radius*| 半径 				|

|*CDSphereIf*| |
|---|---|
|*float GetRadius()*| 			|
|*void SetRadius(float)*| 		|



### カプセル

カプセルのクラスは`CDCapsule`です．カプセルは円柱の両端に半球がついた形をしています．

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/cdcapsule.svg) 

|*CDCapsuleDesc*| | 				 |
|---|---|---|
|*float*|	*radius*| 半球の半径 		|
|*float*|	*length*| 円柱の長さ		|

|*CDCapsuleIf*| |
|---|---|
|*float GetRadius()*| 			|
|*void SetRadius(float)*| 		|
|*float GetLength()*| 			|
|*void SetLength(float)*| 		|



### 丸コーン

丸コーンのクラスは`CDRoundCone`です．丸コーンはカプセルの両端の半径が非対称になったものです．

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/cdroundcone.svg) 

|*CDRoundConeDesc*| | 			 |
|---|---|---|
|*Vec2f*|	*radius*| 各半球の半径		|
|*float*|	*length*| 半球間の距離		|

|*CDRoundConeIf*| |
|---|---|
|*Vec2f GetRadius()*| 			|
|*void SetRadius(Vec2f)*| 		|
|*float GetLength()*| 			|
|*void SetLength(float)*| 		|
|*void SetWidth(Vec2f)*|丸コーンの全長を保存したまま半径を変更します．|




### 凸メッシュ

凸メッシュ(次図)のクラスは`CDConvexMesh`です．凸メッシュとは凹みや穴を持たない多面体です．頂点座標を指定することで自由な形を作成することができます．

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/cdconvexmesh.svg) 

|*CDConvexMeshDesc*| | 						 |
|---|---|---|
|*vector\<Vec3f>*|	*vertices*| 頂点座標の配列	|
|*CDConvexMeshIf*| | 					 |
|*Vec3f* GetVertices()*| 		| 頂点配列の先頭アドレス	|
|*int NVertex()*| 				| 頂点数					|
|*CDFaceIf* GetFace(int i)*| 	| *i*番目の面				|
|*int NFace()*| 				| 面数						|
凸メッシュが作成される際，*CDConvexMeshDesc::vertices*に格納された頂点を内包する最小の凸多面体（凸包）が作成されます．多面体の面を表す*CDFace*のインタフェースを以下に示します．

|*CDFaceIf*| | 						 |
|---|---|---|
|*int* GetIndices()*| 			| 頂点インデックス配列の先頭アドレス	|
|*int NIndex()*| 				| 面の頂点数							|
*NIndex*は面を構成する頂点の数を返します（通常*3*か*4*です）．面は頂点配列を直接保有せず，インデックス配列として間接的に頂点を参照します．したがって，面の頂点座標を得るには
```c++
// given CDConvexMeshIf* mesh
CDFaceIf* face = mesh->GetFace(0);        // get 0-th face
int* idx = face->GetIndices();
Vec3f v = mesh->GetVertices()[idx[0]];    // get 0-th vertex
```
とします．



## 物性の指定
形状には摩擦係数や跳ね返り係数などの物性を指定することができます．形状の基本クラスである*CDShape*のディスクリプタ*CDShapeDesc*は*PHMaterial*型の変数*material*を持っています．

|*PHMaterial*| | 							 |
|---|---|---|
|*float*|	*density*	| 密度				|
|*float*|	*mu0*		| 静止摩擦係数		|
|*float*|	*mu*			| 動摩擦係数		|
|*float*|	*e*			| 跳ね返り係数		|
|*float*|	*reflexSpring*| 跳ね返りバネ係数（ペナルティ法）	|
|*float*|	*reflexDamper*| 跳ね返りダンパ係数（ペナルティ法）|
|*float*|	*frictionSpring*| 摩擦バネ係数（ペナルティ法）	|
|*float*|	*frictionDamper*| 摩擦ダンパ係数（ペナルティ法）|
形状作成後に物性を指定するには*CDShapeIf*の関数を使います．

|*CDShapeIf*| | 						 |
|---|---|---|
|*void SetDensity(float)*| 			| |
|*float GetDensity()*| 				| |
|*void SetStaticFriction(float)*| 	| |
|*float GetStaticFriction()*| 		| |
|*void SetDynamicFriction(float)*| 	| |
|*float GetDynamicFriction()*| 		| |
|*void SetElasticity(float)*| 		| |
|*float GetElasticity()*| 			| |
|*void SetReflexSpring(float)*| 	| |
|*float GetReflexSpring()*| 		| |
|*void SetReflexDamper(float)*| 	| |
|*float GetReflexDamper()*| 		| |
|*void SetFrictionSpring(float)*| 	| |
|*float GetFrictionSpring()*| 		| |
|*void SetFrictionDamper(float)*| 	| |
|*float GetFrictionDamper()*| 		| |
物性に基づいた接触力の具体的な計算法については[Physics](05_physics.html)の項目を参照して下さい．



## 幾何情報の計算

形状に関する幾何情報を計算する関数を紹介します．

|*CDShapeIf*| | 							 |
|---|---|---|
|*float CalcVolume()*| 				| 体積を計算		|
|*Vec3f CalcCenterOfMass()*| 		| 質量中心を計算	|
|*Matrix3f CalcMomentOfInertia()*| 	| 慣性行列を計算	|
*CalcVolume*は形状の体積を計算します．体積に密度（*GetDensity*で取得）を掛ければ質量が得られます．*CalcCenterOfMass*関数は，形状のローカル座標系で表された質量中心の座標を計算します．*CalcMomentOfInertia*関数は，形状のローカル座標系で表された質量中心に関する慣性行列を計算します．ただし，密度を*1*とした場合の値が返されますので，実際の慣性行列を得るには密度を掛ける必要があります．

