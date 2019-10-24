# Graphics

Graphicsは3Dシーンの描画機能を提供するモジュールです．





## Graphics SDK
GraphicsモジュールのすべてのオブジェクトはSDKクラス*GRSdk*によって管理されます．*GRSdk*クラスは，プログラムの実行を通してただ１つのオブジェクトが存在するシングルトンクラスです．*GRSdk*オブジェクトを作成するには以下のようにします．
```c++
    GRSdkIf* grSdk = GRSdkIf::CreateSdk();
```
通常この操作はプログラムの初期化時に一度だけ実行します．また，Frameworkモジュールを使用する場合はユーザが直接*GRSdk*を作成する必要はありません．*GRSdk*には以下の機能があります．

-  レンダラの作成
-  デバイスの作成
-  シーンの管理

レンダラとは処理系に依存しない抽象化された描画機能を提供するクラスです．レンダラのクラスは*GRRender*です．一方，デバイスは処理系ごとの描画処理の実装を行うクラスです．現在のSpringheadではOpenGLによる描画のみがサポートされています．OpenGL用デバイスクラスは*GRDeviceGL*です．レンダラをデバイスに関する*GRSdk*の関数を以下に示します．

|*GRSdkIf*	|							|	 |
|---|---|---|
|_GRRenderIf*_ | *CreateRender()*	| レンダラを作成		|
|_GRDeviceGLIf*_ | *CreateDeviceGL()*	| OpenGLデバイスを作成	|


|*GRRenderIf*	|								|	 |
|---|---|---|
|*void*		| *SetDevice(GRDeviceIf*)*| デバイスの設定	|
|_GRDeviceIf*_ | *GetDevice()*			| デバイスの取得	|

### 初期化
Graphicsモジュールを使用するには以下の初期化処理を必ず実行する必要があります．
```c++
   	GRRenderIf* render = grSdk->CreateRender();
    GRDeviceIf* device = grSdk->CreateDeviceGL();
    device->Init();
    render->SetDevice(device);
```
*GRRender*の*SetDevice*関数でデバイスを登録すると，レンダラは実際の描画処理をそのデバイスを用いて行います．将来的に処理系ごとにデバイスを使い分けることを想定し，上の処理はユーザが行うことになっています．Frameworkモジュールを使用する場合はユーザ自身で上の手続きを行う必要はありません．
## シーン

### シーンの作成
Graphicsモジュールのシーンは，コンピュータグラフィクスにおけるいわゆるシーングラフと同等のものです．シーンクラスは*GRScene*です．シーンを作成するには次のようにします．
```c++
    GRSceneIf* grScene = grSdk->CreateScene();
```
*GRScene*はディスクリプタによる設定項目を持ちません．また，次図に示すように*GRSdk*オブジェクトは任意の数のシーンを保持できます．シーン作成に関する*GRSdk*の関数は以下の通りです．

|*GRSdkIf*|											|	 |
|---|---|---|
|_GRSceneIf*_| *CreateScene()*					| シーンを作成			|
|_GRSceneIf*_| *GetScene(size_t)*				| シーンを取得			|
|*size_t*| *NScene()*							| シーンの数			|
|*void*	| *MergeScene(GRSceneIf*, GRSceneIf*)*| シーンの統合			|




### シーンの機能
シーンを作成したら，次はそのコンテンツであるフレームやメッシュ，カメラやライトなどを作成してシーンに加えていきます．この方法としては完全に手動でシーンを構築する他にもFileIOモジュールを使用してファイルからシーンをロードする方法もあります．以下に*GRScene*の関数を示します．

|*GRSceneIf*	|														|	 |
|---|---|---|
|_GRFrameIf*_	| *GetWorld()*									| ワールドフレームの取得	|
|_GRCameraIf*_| *GetCamera()*									| カメラの取得	|
|*void*		| *SetCamera(const GRCameraDesc&)*				| カメラの設定	|
|_GRVisualIf*_| *CreateVisual(const GRVisualDesc&, GRFrameIf*)*| 描画アイテムの作成	|
|*void*		| *Render(GRRenderIf*)*							| 描画	|
次図に示すように，1つのシーンはただ1つのワールドフレームを持ち，それを基点として任意の数の描画アイテムがツリー状に連なります．ワールドフレームは*GetWorld*で取得します．特殊な描画アイテムにカメラがあります．カメラはワールドフレーム以下のツリーとは別に，*GRScene*が保持します(次図)．カメラの設定は*SetCamera*で行います．カメラを取得するには*GetCamera*を使います．また，カメラはシーングラフ中の1つのフレームを参照し，これを視点の設定に用います．イメージとしてはカメラが参照先のフレームに取り付けられていると考える方が自然でしょう．参照先のフレームの移動に応じてカメラもシーン中を移動することになります．
### シーンの描画
描画処理はプログラムの描画ハンドラで行います．GLUTを使う場合は*glutDisplayFunc*で登録したコールバック関数がこれにあたり，またFrameworkモジュールの*FWApp*を使う場合は*Display*仮想関数がこれにあたります．以下が典型的な描画処理です．
```c++
    render->ClearBuffer();        // clear back buffer
    render->BeginScene();         // begin rendering

    grScene->Render(render);      // render scene

    render->EndScene();           // end rendering
    render->SwapBuffers();        // swap buffers
```
*ClearBuffer*は描画バッファを所定の色で塗りつぶします．塗りつぶし色の取得/設定は*GRRender*の*GetClearColor*，SetClearColorを使います．
```c++
    render->SetClearColor(Vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    render->ClearBuffer();        // clear back buffer in red
```
*BeginScene*と*EndScene*はシーンの描画の前後で必ず呼び出します．*SwapBuffers*はフロントバッファとバックバッファを切り換えることで描画内容を画面上に表示します．*GRScene*の*Render*関数は，カメラ(*GRCamera*)の*Render*とワールドフレーム(*GRFrame*)の*Render*を順次呼び出します．まずカメラの描画によって視点と投影変換が設定され，次にワールドフレームの描画によってシーングラフが再帰的に描画されます．
## 描画アイテム



シーングラフを構成する描画アイテムの基本クラスは*GRVisual*です．*GRVisual*から派生するクラスを次図に示します．描画アイテムには以下の共通の機能があります．

|*GRVisualIf*| | 					 |
|---|---|---|
|*void*| *Render(GRRenderIf*)*	| 	|
|*void* | *Rendered(GRRenderIf*)*| 	|
|*void* | *Enable(bool)*			| 	|
|*bool* | *IsEnabled()*			| 	|
*Render*はアイテムの描画を行い，*Rendered*は描画の後処理を行います．描画処理は描画アイテムの種類ごとに異なります．これについては次節以降で説明します．*Enable*関数は描画処理の有効化/無効化を行います．無効化されたアイテムは描画されません．*IsEnabled*関数は有効/無効状態を返します．描画アイテムを作成するには*GRScene*の*CreateVisual*関数に種類ごとのディスクリプタを指定して呼び出します．
## フレーム
フレームは座標変換を定義すると同時に他の描画アイテムのコンテナとしての役割を持ちます．フレームのクラスは*GRFrame*です．次のコードは，フレームを作成してワールドフレームの子として登録します．
```c++
    GRFrameDesc desc;
    GRFrameIf* frame =
        grScene->CreateVisual(desc, grScene->GetWorldFrame())->Cast();
```
*CreateVisual*関数は指定されたディスクリプタに対応する描画アイテムを作成し，指定された親フレームの子として登録します．親フレームを省くとデフォルトでワールドフレームに登録されます．したがって上のコードは*CreateVisual(desc)*としてもかまいません．*GRFrame*の*Render*関数は，子描画アイテムの*Render*を順次呼び出します．
### 親子関係
フレーム間の親子関係を管理する関数には次のものがあります．

|*GRFrameIf*| | 						 |
|---|---|---|
|_GRFrameIf*_	| *GetParent()*			| 	|
|*void* 		| *SetParent(GRFrameIf*)*| 	|
|*int* 		| *NChildren()*			| 	|
|_GRVisualIf*_* | *GetChildren()*		| 	|
*GetParent*は親フレームを取得します．*SetParent*はそのフレームの親フレームを変更するために使います．*NChildren*はそのフレームの子である描画アイテムの数を返します．これらにはフレーム以外の描画アイテムも含まれることに注意してください．*GetChildren*は子描画アイテムの配列を取得します．
### 座標変換
フレームの座標変換を操作する関数は以下の通りです．

|*GRFrameIf*| | 							 |
|---|---|---|
|*Affinef*| *GetTransform()*				| 	|
|*Affinef*| *GetWorldTransform()*			| 	|
|*void*	| *SetTransform(const Affinef&)*| 	|
*GetTransform*，*SetTransform*はそれぞれフレームとその親フレームとの間の相対的な座標変換を取得/設定します．例えば
```c++
    frame->SetTransform(Affinef::Trn(1.0, 0.0, 0.0));
```
とすると親フレームに対して相対的にx方向に$1.0$移動します．
## カメラ






カメラは描画における視点の設定と投影変換を管理します．はじめにカメラのディスクリプタを見ていきます．

|*GRCameraDesc*| | 					 |
|---|---|---|
|*Vec2f*|	*size*| スクリーンサイズ 		|
|*Vec2f*|	*center*| スクリーン中心座標 	|
|*float*|	*front*| 前方クリップ面		|
|*float*|	*back*| 後方クリップ面		|
各変数の定義は次図(b),(c)を参照してください．設定を変更するには以下のようにします．
```c++
    GRCameraDesc desc;
    grScene->GetCamera()->GetDesc(&desc);
    desc.front = 3.0f;
    grScene->SetCamera(desc);
```
上では*GetDesc*関数で既存の設定をディスクリプタにコピーし，*front*を変更してから*SetCamera*関数で再設定しています．一方，*GRCamera*の関数は以下の通りです．

|*GRCameraIf*| | 					 |
|---|---|---|
|_GRFrameIf*_| *GetFrame()*			|	|
|*void*	| *SetFrame(GRFrameIf*)*	|	|
*GetFrame*，*SetFrame*関数はカメラフレームを取得/設定します．次図(a)のように，カメラフレームはカメラの視点を定義します．
## ライト
ライトはシーンの照明を設定するための描画アイテムです．ライトのクラス*GRLight*のディスクリプタの代表的な変数を以下に示します．

|*GRLightDesc*| | 				 |
|---|---|---|
|*Vec4f*|	*ambient*| 環境光 		|
|*Vec4f*|	*diffuse*| 拡散光 		|
|*Vec4f*|	*specular*| 鏡面光		|
|*Vec4f*|	*position*| ライト位置	|
減衰係数やスポットライトなどのより詳細な設定項目についてはソースファイルを参照してください．OpenGLの仕様と同様，*position*の第4成分*position.w*が*0*の場合は平行光源となり，*(x,y,z)*方向の無限遠にライトがあることになり，*position.w*が*1*の場合は*(x,y,z)*の位置に点光源がおかれます．
## マテリアル
マテリアルは材質を指定するためのアイテムです．マテリアルのクラスは*GRMaterial*です．通常，マテリアルは次節で説明するメッシュの子描画アイテムとなります．ファイルからメッシュをロードする場合は，メッシュの作成と同時にマテリアルも自動的に作成され，メッシュの子として追加されます．*GRMaterial*のディスクリプタは以下の通りです．

|*GRMaterialDesc*| | 				 |
|---|---|---|
|*Vec4f*	|	*ambient*| 環境色 	|
|*Vec4f*	|	*diffuse*| 拡散色 	|
|*Vec4f*	|	*specular*| 鏡面色	|
|*Vec4f*	|	*emissive*| 自己発光	|
|*float*	|	*power*	| 鏡面係数	|
|*UTString*|	*texname*| テクスチャファイル名|
レンダラにマテリアルを設定すると，次に別のマテリアルを設定するまでの間の形状描画にそのマテリアルの描画属性が適用されます．マテリアルを設定するにはいくつかの方法があります．一つ目は*GRMaterialIf*の*Render*関数を呼ぶ方法です:これに加え，以下に示す*GRRender*の関数のいずれかを用いることもできます．

|*GRRenderIf*																 |
|---|---|---|---|
|*void*| *SetMaterial(const GRMaterialDesc&)*	| 描画マテリアルの設定	|
|*void*| *SetMaterial(const GRMaterialIf*)*		| 描画マテリアルの設定	|
|*void*| *SetMaterial(int)*						| 描画マテリアルの設定	|
以下の例はマテリアルを設定する3通りの方法を示しています．どの方法を用いても結果は変わりません．
```c++
    // given GRRenderIf* render, GRSceneIf* scene
    GRMaterialDesc md;
    md.diffuse = Vec4f(1.0f, 0.0f, 0.0f, 1.0f);
    // 1.
    render->SetMaterial(md);
    // 2.
    GRMaterialIf* mat = scene->CreateVisual(md)->Cast();
    mat->Render(render);
    // 3.
    render->SetMaterial(mat);
```
毎回マテリアルを作成するのは煩わしいことがあります．そのような場合はレンダラの予約色を指定することも可能です．
```c++
    // 4.
    render->SetMaterial(GRRenderBaseIf::RED);
```
使用可能な予約色はX11 web colorにもとづいています．詳しくは*SprGRRender.h*ヘッダファイルを











*GRRenderBaseIf*が持つ予約色（全24色，次表参照）です．
## メッシュ
メッシュは多面体形状を表現するための描画アイテムです．メッシュのクラスは*GRMesh*です．メッシュを作成する方法には

-  ディスクリプタを用いて手動で作成する
-  FileIOモジュールを利用してファイルからメッシュをロードする

の二通りがあります．後者の方法では，モデリングソフトで作成し，Direct3DのX形式などで出力したファイルから形状をロードすることができます．詳しくは\ref{chap_fileio}章を参照してください．また，メッシュのみをロードする簡易機能として*FWObjectIf::LoadMesh*が用意されています．以下では前者の手動構築の方法について説明します．メッシュのディスクリプタは次の通りです．

|*GRMeshDesc*| | 					 |
|---|---|---|
|*vector\<Vec3f>*	|	*vertices*	| 頂点	 			|
|*vector\<GRMeshFace>*|	*faces*		| 面	 			|
|*vector\<Vec3f>*	|	*normals*	| 法線				|
|*vector\<GRMeshFace>*|	*faceNormals*| 面法線			|
|*vector\<Vec4f>*	|	*colors*		| 色				|
|*vector\<Vec2f>*	|	*texCoords*	| テクスチャ座標	|
|*vector\<int>*	|	*materialList*| マテリアルリスト|
*vector*は*C++*の可変長配列コンテナです．*vertices*は頂点座標を格納した配列です．ただし頂点座標を設定しただけでは形状は定義されません．メッシュは面の集合ですので，*faces*を設定する必要があります．*GRMeshFace*の定義は以下の通りです．

|*GRMeshFace*| | 					 |
|---|---|---|
|*int*|	*nVertices*	| 頂点数 	|
|*int*|	*indices[4]*	| 頂点インデックス|
*nVertices*は1つの面を構成する頂点数で，3か4を設定します．*indices*には*nVertices*個の頂点インデックスを設定します．このとき

*vertices[faces[i].indices[j]]*

が*i*番目の面の*j*番目の頂点座標となります．*GRMeshDesc*のメンバ変数の中で*vertices*と*faces*は必須ですが，その他のメンバは必ずしも設定する必要はありません．*normals*は各頂点の法線の向きをを格納する配列です．*normals[i]*が*vertices[i]*の法線を与えます．*normals*を省略した場合，法線は自動生成されます．このとき，各頂点の法線はその頂点を共有する面の法線の平均で与えられます．*normals*に加えて*faceNormals*を設定した場合，異なる方法で法線が与えられます．このとき

*normals[faceNormals[i].indices[j]]*

が*i*番目の面の*j*番目の頂点に対応する法線となります．*colors*は頂点色です．*colors[i]*が*i*番目の頂点の色を与えます．*texCoords*は頂点ごとのテクスチャUV座標を与えます．テクスチャを描画するには，メッシュに割り当てるマテリアルにテクスチャファイル名が設定されている必要があります．*materialList*は面ごとに異なるマテリアルを割り当てるために用います．*materialList[i]*が*i*番目の面のマテリアル番号を与えます．ただし，番号に対応するマテリアルは別途メッシュに割り当てておく必要があります．
### メッシュへのマテリアルの割当て
ファイルからメッシュをロードする場合，もしファイル中にマテリアル情報が含まれていればそれをもとに自動的にマテリアルがメッシュへ割り当てられます．手動でメッシュに割り当てるには，*AddChildObject*を使います．以下に例を示します．
```c++
    // given GRSceneIf* scene, GRFrameIf* frame
    GRMeshDesc meshDesc;
    // ... setup discriptor here ...

    // create mesh and attach it to frame
    GRMeshIf* mesh = scene->CreateVisual(meshDesc, frame)->Cast();

    GRMaterialDesc matDesc0, matDesc1;
    // ... setup materials here ...
    GRMaterialIf* mat0 = scene->CreateVisual(matDesc0, frame)->Cast();
    GRMaterialIf* mat1 = scene->CreateVisual(matDesc1, frame)->Cast();

    // attach materials to mesh
    mesh->AddChildObject(mat0);    //< material no.0
    mesh->AddChildObject(mat1);    //< material no.1
```
最初に割り当てられたマテリアルを0番として昇順でマテリアル番号が決まります．前述のマテリアルリストを用いる場合はこのマテリアル番号を面毎に指定してください．
## レンダラ
レンダラの機能を項目別に説明します．レンダラは提供するプリミティブな描画機能は非常に多岐に渡りますが，これらのほとんどの関数は特別な描画処理を必要としない限りユーザが直接呼び出すことはありません．個々の関数を詳しく説明していくと膨大な量になりますので，ここでは一覧程度にとどめます．詳細な仕様はソースコードのコメントを参照してください．
### 基本機能
描画時のお決まりの処理です．\ref{sec_grscene}節を参照してください．

|*GRRenderIf*| 									 |
|---|---|---|---|
|*void*| *GetClearColor(Vec4f&)*		| 背景色の取得				|
|*void*| *SetClearColor(const Vec4f&)*| 背景色の設定				|
|*void*| *ClearBuffer()*				| 描画バッファをクリア		|
|*void*| *BeginScene()*				| 描画の開始				|
|*void*| *EndScene()*					| 描画の完了				|
|*void*| *SwapBuffers()*				| 描画バッファのスワップ	|

### ディスプレイリスト
ディスプレイリストに関係する機能です．*GRMesh*が内部で使用します．

|*GRRenderIf*| 					 |
|---|---|---|
|*int* | *StartList()*		| ディスプレイリスト作成開始	|
|*void*| *EndList()*			| ディスプレイリスト作成完了	|
|*void*| *DrawList(int)*		| ディスプレイリストの描画		|
|*void*| *ReleaseList(int)*	| ディスプレイリストの解放		|

### デプステスト，アルファブレンディング，ライティング
描画機能を切り替えるための関数です．

|*GRRenderIf*								|										 |
|---|---|---|
|*void*| *SetDepthWrite(bool)*				| デプスバッファへの書き込みOn/Off		|
|*void*| *SetDepthTest(bool)*					| デプステストのOn/Off					|
|*void*| *SetDepthFunc(TDepthFunc)*			| デプスバッファの判定条件				|
|*void*| *SetAlphaTest(bool)*					| アルファブレンディングのOn/Off		|
|*void*| *SetAlphaMode(TBlendFunc, TBlendFunc)*| アルファブレンディングのモード		|
|*void*| *SetLighting(bool)*					| ライティングのOn/Off					|

### テクスチャ


|*GRRenderIf*													|						 |
|---|---|---|
|*int* | *LoadTexture(UTString)*									| テクスチャのロード	|
|*void* | *SetTextureImage(UTString, int, int, int, int, char*)*	| テクスチャの設定		|

### シェーダ


|*GRRenderIf*											|								 |
|---|---|---|
|*void* 	| *InitShader()*									| シェーダの初期化				|
|*void* 	| *SetShaderFormat(ShaderType)*					| シェーダフォーマットの設定	|
|*bool* 	| *CreateShader(UTString, UTString, GRHandler&)*| シェーダオブジェクトの作成	|
|*GRHandler* | *CreateShader()*								| シェーダオブジェクトの作成	|
|*bool* 	| *ReadShaderSource(GRHandler, UTString)*		| シェーダプログラムをロード	|
|*void* 	| *GetShaderLocation(GRHandler, void*)*			| ロケーション情報の取得		|

### 直接描画


|*GRRenderIf*																 |
|---|---|---|
|*void*| *SetVertexFormat(const GRVertexElement*)*						| 頂点フォーマットの指定	|
|*void*| *SetVertexShader(void*)*										| 頂点シェーダーの指定		|
|*void*| *DrawDirect(TPrimitiveType, void*, size_t, size_t)*			| 頂点を指定してプリミティブを描画	|
|*void*| *DrawIndexed(TPrimitiveType, size_t*, void*, size_t, size_t)*| 頂点とインデックスを指定してプリミティブを描画	|
|*void*| *DrawArrays(TPrimitiveType, GRVertexArray*, size_t)*			| 頂点の成分ごとの配列を指定して，プリミティブを描画	|
|*void*| *DrawArrays(TPrimitiveType, size_t*, GRVertexArray*, size_t)*	| インデックスと頂点の成分ごとの配列を指定して，プリミティブを描画	|

### 基本形状描画


|*GRRenderIf*																					 |
|---|---|---|
|*void*| *DrawLine(Vec3f, Vec3f)*										| 線分を描画	|
|*void*| *DrawArrow(Vec3f, Vec3f, float, float, float, int, bool)*		| 矢印を描画	|
|*void*| *DrawBox(float, float, float, bool)*							| 直方体を描画	|
|*void*| *DrawSphere(float, int, int, bool)*								| 球体を描画	|
|*void*| *DrawCone(float, float, int, bool)*								| 円錐の描画	|
|*void*| *DrawCylinder(float, float, int, bool)*							| 円筒の描画	|
|*void*| *DrawCapsule(float, float, int, bool)*							| カプセルの描画	|
|*void*| *DrawRoundCone(float, float, float, int, bool)*					| 球円錐の描画	|
|*void*| *DrawGrid(float, int, float)*									| グリッドを描画	|
|*void*| *SetFont(const GRFont&)*										| フォントの設定	|
|*void*| *DrawFont(Vec2f, UTString)*										| 2次元テキストの描画	|
|*void*| *DrawFont(Vec3f, UTString)*										| 3次元テキストの描画	|
|*void*| *SetLineWidth(float)*											| 線の太さの設定	|

### カメラ


|*GRRenderIf*												 |
|---|---|---|---|
|*void* 				| *SetCamera(const GRCameraDesc&)*| カメラの設定	|
|*const GRCameraDesc&* | *GetCamera()*					| カメラの取得	|

### ライト


|*GRRenderIf*													 |
|---|---|---|---|
|*void*| *PushLight(const GRLightDesc&)*| ライトをプッシュ	|
|*void*| *PushLight(const GRLightIf*)*| ライトをプッシュ	|
|*void*| *PopLight()*					| ライトをポップ	|
|*int* | *NLights()*					| ライトの数		|

### 座標変換


|*GRRenderIf*												 |
|---|---|---|
|*void* | *Reshape(Vec2f, Vec2f)*				| ウィンドウサイズの変更				|
|*void* | *SetViewport(Vec2f, Vec2f)*			| ビューポートの設定					|
|*Vec2f* | *GetViewportPos()*						| ビューポート原点の取得				|
|*Vec2f* | *GetViewportSize()*					| ビューポートサイズの取得				|
|*Vec2f* | *GetPixelSize()*						| 1ピクセルの物理サイズを取得			|
|*Vec3f*| *ScreenToCamera(int, int, float, bool)*| スクリーン座標からカメラ座標への変換	|
|*void* | *EnterScreenCoordinate()*				| スクリーン座標系へ切り替える			|
|*void* | *LeaveScreenCoordinate()*				| スクリーン座標系から戻る				|


|*GRRenderIf*|| 										 |
|---|---|---|---|
|*void*| *SetViewMatrix(const Affinef&)*		| 視点行列の設定	|
|*void*| *GetViewMatrix(Affinef&)*			| 視点行列の取得	|
|*void*| *SetProjectionMatrix(const Affinef&)*| 投影行列の設定	|
|*void*| *GetProjectionMatrix(Affinef&)*		| 投影行列の取得	|
|*void*| *SetModelMatrix(const Affinef&)*	| モデル行列の設定	|
|*void*| *GetModelMatrix(Affinef&)*			| モデル行列の取得	|
|*void*| *MultModelMatrix(const Affinef&)*	| モデル行列に変換をかける	|
|*void*| *PushModelMatrix()*					| モデル行列をプッシュ	|
|*void*| *PopModelMatrix()*					| モデル行列をポップ	|
|*void*| *ClearBlendMatrix()*					| ブレンド変換行列のクリア	|
|*bool*| *SetBlendMatrix(const Affinef&, int)*| ブレンド変換行列の設定	|
