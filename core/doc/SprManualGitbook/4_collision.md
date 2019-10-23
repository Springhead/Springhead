
## 概要
Collisionモジュールは物理計算の基礎となる衝突判定機能を提供します．事実上CollisionモジュールはPhysicsモジュールのサブモジュールとなっており，両者は密接に依存しています．ユーザは主として剛体に衝突判定用形状を割り当てる際にCollisionモジュールの機能を利用することになります．




\includegraphics[width=.4\hsize]{fig/cdclass.eps}

\caption{Class hierarchy of Collision module}


Collisionモジュールのクラス階層をFig.\,\ref{fig_cdclass}に示します．衝突判定形状はすべて`CDShape`から派生します．アルゴリズムの性質上，形状はすべて凸形状でなければなりません．
## 形状の作成
衝突判定形状は次の手順で作成・登録します．

1.  形状を作成する
1.  剛体へ形状を追加する
1.  形状の位置を設定する

以下に順を追って説明します．まず形状を作成するには次のようにします．
```
// given PHSdkIf* phSdk

CDBoxDesc desc;
desc.boxsize = Vec3d(1.0, 1.0, 1.0);

CDBoxIf* box = phSdk->CreateShape(desc)->Cast();
```
衝突判定形状のオブジェクトはPhysicsモジュールが管理します．このため，形状を作成するには`PHSdk`クラスの`CreateShape`関数を使います．`PHSdk`については\ref{chap_physics}章を参照してください．形状を作成するには，まず種類に応じたディスクリプタを作成し，寸法などのパラメータを設定します．この例では直方体クラス`CDBox`のディスクリプタを作成して一辺が$1.0$の立方体を作成します．ディスクリプタを指定して`CreateShape`を呼び出すと，対応する種類の形状が作成され，そのインタフェースが返されます．ただし戻り値は形状の基底クラスである`CDShape`のインタフェースですので，派生クラス（ここでは`CDBox`）のインタフェースを得るには上のように`Cast`関数で動的キャストする必要があります．形状を作成したら，次にその形状を与えたい剛体に登録します．
```
// given PHSolidIf* solid

solid->AddShape(box);         // first box
```
剛体クラス`PHSolid`については\ref{chap_physics}章を参照してください．ここで重要なことは，一度作成した形状は1つの剛体にいくつでも登録でき，また異なる複数の剛体にも登録できるということです．つまり，同じ形状を複数の剛体間で共有することで，形状の作成コストやメモリ消費を抑えることができます．`AddShape`関数で登録した直後の形状は，剛体のローカル座標系の原点に位置しています．これを変更したい場合は`SetShapePose`関数を使います．
```
solid->AddShape(box);         // second box
solid->AddShape(box);         // third box 

// move first shape 1.0 in x-direction
solid->SetShapePose(0, Posed(Vec3d(1.0, 0.0, 0.0), Quaterniond());

// rotate second shape 30 degrees along y-axis
solid->SetShapePose(1, Posed(Vec3d(),
                    Quaterniond::Rot(Rad(30.0), 'y')));
```
`SetShapePose`の第1引数は操作する形状の番号です．最初に`AddShape`した形状の番号が$0$で，`AddShape`するたびに$1$増加します．形状の位置や向きは剛体のローカル座標系で指定します．また，形状の位置・向きを取得するには`GetShapePose`関数を使います．以下ではSpringheadでサポートされている形状を種類別に解説します．
### 直方体





\includegraphics[width=.4\hsize]{fig/cdbox.eps}

\caption{Box geometry}


直方体(Fig.\,\ref{fig_cdbox})のクラスは*CDBox*です．

\begin{tabular}{lll}
\multicolumn{3}{l}{*CDBoxDesc*}					\\ \midrule
*Vec3f*	&	*boxsize*	& 各辺の長さ 	\\
\\
\multicolumn{3}{l}{*CDBoxIf*}					\\ \midrule
\multicolumn{2}{l}{*Vec3f GetBoxSize()*}			\\
\multicolumn{2}{l}{*void SetBoxSize(Vec3f)*}		\\
\end{tabular}


### 球





\includegraphics[width=.4\hsize]{fig/cdsphere.eps}

\caption{Sphere geometry}


球(Fig.\,\ref{fig_cdsphere})のクラスは`CDSphere`です．

\begin{tabular}{lll}
\multicolumn{3}{l}{*CDSphereDesc*}				\\ \midrule
*float*	&	*radius*	& 半径 				\\
\\
\multicolumn{3}{l}{*CDSphereIf*}					\\ \midrule
\multicolumn{2}{l}{*float GetRadius()*}			\\
\multicolumn{2}{l}{*void SetRadius(float)*}		\\
\end{tabular}


### カプセル





\includegraphics[width=.4\hsize]{fig/cdcapsule.eps}

\caption{Capsule geometry}


カプセル(Fig.\,\ref{fig_cdcapsule})のクラスは`CDCapsule`です．カプセルは円柱の両端に半球がついた形をしています．

\begin{tabular}{lll}
\multicolumn{3}{l}{*CDCapsuleDesc*}				\\ \midrule
*float*	&	*radius*	& 半球の半径 		\\
*float*	&	*length* & 円柱の長さ		\\
\\
\multicolumn{3}{l}{*CDCapsuleIf*}				\\ \midrule
\multicolumn{2}{l}{*float GetRadius()*}			\\
\multicolumn{2}{l}{*void SetRadius(float)*}		\\
\multicolumn{2}{l}{*float GetLength()*}			\\
\multicolumn{2}{l}{*void SetLength(float)*}		\\
\end{tabular}


### 丸コーン





\includegraphics[width=.4\hsize]{fig/cdroundcone.eps}

\caption{Round cone geometry}


丸コーン(Fig.\,\ref{fig_cdroundcone})のクラスは`CDRoundCone`です．丸コーンはカプセルの両端の半径が非対称になったものです．

\begin{tabular}{lll}
\multicolumn{3}{l}{*CDRoundConeDesc*}			\\ \midrule
*Vec2f*	&	*radius*	& 各半球の半径		\\
*float*	&	*length* & 半球間の距離		\\
\\
\multicolumn{3}{l}{*CDRoundConeIf*}				\\ \midrule
\multicolumn{2}{l}{*Vec2f GetRadius()*}			\\
\multicolumn{2}{l}{*void SetRadius(Vec2f)*}		\\
\multicolumn{2}{l}{*float GetLength()*}			\\
\multicolumn{2}{l}{*void SetLength(float)*}		\\
\multicolumn{2}{l}{*void SetWidth(Vec2f)*}		\\
\end{tabular}

*SetWidth*関数は，丸コーンの全長を保存したまま半径を変更します．
### 凸メッシュ





\includegraphics[width=.4\hsize]{fig/cdconvexmesh.eps}

\caption{Convex mesh geometry}


凸メッシュ(Fig.\,\ref{fig_cdconvexmesh})のクラスは`CDConvexMesh`です．凸メッシュとは凹みや穴を持たない多面体です．頂点座標を指定することで自由な形を作成することができます．

\begin{tabular}{lll}
\multicolumn{3}{l}{*CDConvexMeshDesc*}						\\ \midrule
*vector<Vec3f>*	&	*vertices*	& 頂点座標の配列	\\
\\
\multicolumn{3}{l}{*CDConvexMeshIf*}					\\ \midrule
\multicolumn{2}{l}{*Vec3f* GetVertices()*}			& 頂点配列の先頭アドレス	\\
\multicolumn{2}{l}{*int NVertex()*}					& 頂点数					\\
\multicolumn{2}{l}{*CDFaceIf* GetFace(int i)*}		& $i$番目の面				\\
\multicolumn{2}{l}{*int NFace()*}					& 面数						\\
\end{tabular}

凸メッシュが作成される際，*CDConvexMeshDesc::vertices*に格納された頂点を内包する最小の凸多面体（凸包）が作成されます．多面体の面を表す*CDFace*のインタフェースを以下に示します．

\begin{tabular}{lll}
\multicolumn{3}{l}{*CDFaceIf*}						\\ \midrule
\multicolumn{2}{l}{*int* GetIndices()*}				& 頂点インデックス配列の先頭アドレス	\\
\multicolumn{2}{l}{*int NIndex()*}					& 面の頂点数							\\
\end{tabular}

*NIndex*は面を構成する頂点の数を返します（通常$3$か$4$です）．面は頂点配列を直接保有せず，インデックス配列として間接的に頂点を参照します．したがって，面の頂点座標を得るには
```
// given CDConvexMeshIf* mesh
CDFaceIf* face = mesh->GetFace(0);        // get 0-th face
int* idx = face->GetIndices();
Vec3f v = mesh->GetVertices()[idx[0]];    // get 0-th vertex
```
とします．
## 物性の指定
形状には摩擦係数や跳ね返り係数などの物性を指定することができます．形状の基本クラスである*CDShape*のディスクリプタ*CDShapeDesc*は*PHMaterial*型の変数*material*を持っています．

\begin{tabular}{lll}
\multicolumn{3}{l}{*PHMaterial*}							\\ \midrule
*float*	&	*density*		& 密度				\\
*float*	&	*mu0*			& 静止摩擦係数		\\
*float*	&	*mu*				& 動摩擦係数		\\
*float*	&	*e*				& 跳ね返り係数		\\
*float*	&	*reflexSpring*	& 跳ね返りバネ係数（ペナルティ法）	\\
*float*	&	*reflexDamper*	& 跳ね返りダンパ係数（ペナルティ法）\\
*float*	&	*frictionSpring*	& 摩擦バネ係数（ペナルティ法）	\\
*float*	&	*frictionDamper*	& 摩擦ダンパ係数（ペナルティ法）\\
\end{tabular}

形状作成後に物性を指定するには*CDShapeIf*の関数を使います．

\begin{tabular}{lll}
\multicolumn{3}{l}{*CDShapeIf*}						\\ \midrule
\multicolumn{2}{l}{*void SetDensity(float)*}				& \\
\multicolumn{2}{l}{*float GetDensity()*}					& \\
\multicolumn{2}{l}{*void SetStaticFriction(float)*}		& \\
\multicolumn{2}{l}{*float GetStaticFriction()*}			& \\
\multicolumn{2}{l}{*void SetDynamicFriction(float)*}		& \\
\multicolumn{2}{l}{*float GetDynamicFriction()*}			& \\
\multicolumn{2}{l}{*void SetElasticity(float)*}			& \\
\multicolumn{2}{l}{*float GetElasticity()*}				& \\
\multicolumn{2}{l}{*void SetReflexSpring(float)*}		& \\
\multicolumn{2}{l}{*float GetReflexSpring()*}			& \\
\multicolumn{2}{l}{*void SetReflexDamper(float)*}		& \\
\multicolumn{2}{l}{*float GetReflexDamper()*}			& \\
\multicolumn{2}{l}{*void SetFrictionSpring(float)*}		& \\
\multicolumn{2}{l}{*float GetFrictionSpring()*}			& \\
\multicolumn{2}{l}{*void SetFrictionDamper(float)*}		& \\
\multicolumn{2}{l}{*float GetFrictionDamper()*}			& \\
\end{tabular}

物性に基づいた接触力の具体的な計算法については第\ref{sec_physics_contact}節を参照して下さい．
## 幾何情報の計算
形状に関する幾何情報を計算する関数を紹介します．

\begin{tabular}{lll}
\multicolumn{3}{l}{*CDShapeIf*}							\\ \midrule
\multicolumn{2}{l}{*float CalcVolume()*}					& 体積を計算		\\
\multicolumn{2}{l}{*Vec3f CalcCenterOfMass()*}			& 質量中心を計算	\\
\multicolumn{2}{l}{*Matrix3f CalcMomentOfInertia()*}		& 慣性行列を計算	\\
\end{tabular}

*CalcVolume*は形状の体積を計算します．体積に密度（*GetDensity*で取得）を掛ければ質量が得られます．*CalcCenterOfMass*関数は，形状のローカル座標系で表された質量中心の座標を計算します．*CalcMomentOfInertia*関数は，形状のローカル座標系で表された質量中心に関する慣性行列を計算します．ただし，密度を$1$とした場合の値が返されますので，実際の慣性行列を得るには密度を掛ける必要があります．
