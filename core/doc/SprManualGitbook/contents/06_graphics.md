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
*GRScene*はディスクリプタによる設定項目を持ちません．また，次図に示すように*GRSdk*オブジェクトは任意の数のシーンを保持できます．

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/grscene.svg) 

シーン作成に関する*GRSdk*の関数は以下の通りです．

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

シーングラフを構成する描画アイテムの基本クラスは*GRVisual*です．*GRVisual*から派生するクラスを次図に示します．

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/grvisual.svg) 

描画アイテムには以下の共通の機能があります．

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
とすると親フレームに対して相対的にx方向に1.0移動します．



## カメラ


カメラは描画における視点の設定と投影変換を管理します．はじめにカメラのディスクリプタを見ていきます．

|*GRCameraDesc*| | 					 |
|---|---|---|
|*Vec2f*|	*size*| スクリーンサイズ 		|
|*Vec2f*|	*center*| スクリーン中心座標 	|
|*float*|	*front*| 前方クリップ面		|
|*float*|	*back*| 後方クリップ面		|
各変数の定義は次図を参照してください．

![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/grcamera.svg) 

| Front View                                                   | Top View                                                     |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/grcamera_front.svg) | ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/grcamera_top.svg) |



設定を変更するには以下のようにします．

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
*GetFrame*，*SetFrame*関数はカメラフレームを取得/設定します．先の図のように，カメラフレームはカメラの視点を定義します．



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

|*GRRenderIf*																 |||
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


使用可能な予約色はX11 web colorにもとづいています．詳しくは*SprGRRender.h*ヘッダファイルを参照してください。

| enum                 | Sample                                                       | RGB値         |
| -------------------- | ------------------------------------------------------------ | ------------- |
| INDIANRED            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/1595427C93E5881C0FBBEFBE0F1AB5B4.svg) | (205 92 92)   |
| LIGHTCORAL           | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/05928B57CA81400E0E7D74059A14D094.svg) | (240 128 128) |
| SALMON               | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/D3AAB1AFC7825CD8915CFA0BB82A1962.svg) | (250 128 114) |
| DARKSALMON           | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/01AE76D89DEAA3AF718FFBCDBA274D3F.svg) | (233 150 122) |
| LIGHTSALMON          | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/7E7A009CB4E1547179B6B724EAADB5D9.svg) | (255 160 122) |
| RED                  | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/40FB2EE6575C791B150FA46169658218.svg) | (255 0 0)     |
| CRIMSON              | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/9A321E7229A9B5AFB8AB8A33F5520EBC.svg) | (220 20 60)   |
| FIREBRICK            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/E6F9C96CA1E5855E9D836A8741E294C1.svg) | (178 34 34)   |
| DARKRED              | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/25B6E41D002B5F87B933B24EF4B0E35D.svg) | (139 0 0)     |
|                      |                                                              |               |
| PINK                 | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/AFADBB0FC1D499FF6EDA454719323C4A.svg) | (255 192 203) |
| LIGHTPINK            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/A02BC5651C992AC595976742CD309A47.svg) | (255 182 193) |
| HOTPINK              | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/DCC86C585AB62854FF25BB1EA9C229DC.svg) | (255 105 180) |
| DEEPPINK             | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/36275696F7BCEF8142AB1DEA7BDDDECE.svg) | (255 20 147)  |
| MEDIUMVIOLETRED      | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/C9AD85B5B0C3D823D787B1AFDF288DBD.svg) | (255 21 133)  |
| PALEVIOLETRED        | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/4165BB3F89A036D2B0B62CABC9D2EF86.svg) | (255 112 147) |
|                      |                                                              |               |
| CORAL                | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/D147BFDF01E02D29CAD49881C78816AE.svg) | (255 127 80)  |
| TOMATO               | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/36822A5041C09AB319353E2A7F20B57E.svg) | (255 99 71)   |
| ORANGERED            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/A564A621CB744FB34F7B4CE63CE55C6E.svg) | (255 69 0)    |
| DARKORANGE           | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/4571BE0D254CAF46BD76C5CAF92D2AC2.svg) | (255 140 0)   |
| ORANGE               | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/2A44388DE59DE205F78BEDFD2ECEF2F1.svg) | (255 165 0)   |
|                      |                                                              |               |
| GOLD                 | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/6D1D7273060A7AE7C4DBDE105276734D.svg) | (255 215 0)   |
| YELLOW               | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/F067F8827A32450373A5655C3FB2E017.svg) | (255 255 0)   |
| LIGHTYELLOW          | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/57372533DE8C49153112581E5A0C7B83.svg) | (255 255 224) |
| LEMONCHIFFON         | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/5E974513182E06ACF74617B6C37C5A8E.svg) | (255 250 205) |
| LIGHTGOLDENRODYELLOW | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/BFDAE2D0AE8EC0C629F4A77A60003EFC.svg) | (250 250 210) |
| PAPAYAWHIP           | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/83FDED6DAA7994500C934F3A0C5A9BE6.svg) | (255 239 213) |
| MOCCASIN             | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/04F985C0A6425F96B0A52A18C590405D.svg) | (255 228 181) |
| PEACHPUFF            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/7D9666983528BF0ECA91C552E1C2F9E8.svg) | (255 218 185) |
| PALEGOLDENROD        | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/B761EB80A13C100EA5AECED82220A679.svg) | (238 232 170) |
| KHAKI                | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/B11A8DF0DC145BC736C93D222B3745AB.svg) | (240 230 140) |
| DARKKHAKI            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/EE8DCB58ECB858DBA5DDA400EC6C42B5.svg) | (189 183 107) |
|                      |                                                              |               |
| LAVENDAR             | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/29783D929010DF7EAED7F9EFADFAD42A.svg) | (230 230 250) |
| THISTLE              | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/DCF6366264E2C948774297B55087B64B.svg) | (216 191 216) |
| PLUM                 | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/12A2C6AA698033D06CF2D9E0BA7B55A7.svg) | (221 160 221) |
| VIOLET               | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/D3A8C57E4A6C3D1CDFA1E87956EE7FA8.svg) | (238 130 238) |
| ORCHILD              | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/75849F1AC0C33EBD0BD888A913A0492F.svg) | (218 112 214) |
| FUCHSIA              | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/2EF0E869BB592C9911BA2EA44EA2364E.svg) | (255 0 255)   |
| MAGENTA              | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/2EF0E869BB592C9911BA2EA44EA2364E.svg) | (255 0 255)   |
| MEDIUMORCHILD        | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/7678C5C074B56D7FCA26902710C0C80C.svg) | (186 85 211)  |
| MEDIUMPURPLE         | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/9BF8C0BF7B4821289477DE94E079D095.svg) | (147 112 219) |
| BLUEVIOLET           | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/E50393C562C3FDF00B441FC63A48A9A5.svg) | (138 43 226)  |
| DARKVIOLET           | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/6BA50A556FE940A58DF9F56768D23BA2.svg) | (148 0 211)   |
| DARKORCHILD          | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/3861D1C91036B2D780D88BB31D6F060C.svg) | (153 50 204)  |
| DARKMAGENTA          | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/7535DA069F7EE130A51EC78D3F63B774.svg) | (139 0 139)   |
| PURPLE               | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/499A1C0538781FE3430A9CFA2313D349.svg) | (128 0 128)   |
| INDIGO               | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/5394817117968BF4EB362430AA82C874.svg) | (75 0 130)    |
| DARKSLATEBLUE        | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/96AD86B3CA49FEFE00FA2C2E965253BF.svg) | (72 61 139)   |
| SLATEBLUE            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/8B8FCE3D459B36481399825640E33A93.svg) | (106 90 205)  |
| MEDIUMSLATEBLUE      | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/675D671F82E426980708CF4F90596DC3.svg) | (123 104 238) |
|                      |                                                              |               |
| GREENYELLOW          | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/BF558D01E9D2E245A2AD1A0184AE87D7.svg) | (173 255 47)  |
| CHARTREUSE           | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/048F5C11FBF2831220E312494CDB529F.svg) | (127 255 0)   |
| LAWNGREEN            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/B6086FC5DB4015B7A2224EA642897A38.svg) | (124 252 0)   |
| LIME                 | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/7D46B86EA729DC29A77BC9B109484B8D.svg) | (0 255 0)     |
| LIMEGREEN            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/FCFB2D98B06D5BAEC028CB7635ADE44B.svg) | (50 205 50)   |
| PALEGREEN            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/346D9DE030C062C6769293B390E5B570.svg) | (152 251 152) |
| LIGHTGREEN           | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/D34D376BA241A7460380B69CAF50091F.svg) | (144 238 144) |
| MEDIUMSPRINGGREEN    | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/9F1D7DA173698D670DC76CEF55A8400D.svg) | (0 250 154)   |
| SPRINGGREEN          | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/A08ECE1947ABA60C0647754455A54323.svg) | (0 255 127)   |
| MEDIUMSEAGREEN       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/B45A1DFB0DD86F21DA578844DA09D94B.svg) | (60 179 113)  |
| SEAGREEN             | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/6E0DAC8329AB9CEDCEC8FF7BB0A0DCDB.svg) | (46 139 87)   |
| FORESTGREEN          | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/A5D8516EC90651B2E5B59365D5E31A34.svg) | (34 139 34)   |
| GREEN                | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/80F7610B1424036C72BB823B38C423FC.svg) | (0 128 0)     |
| DARKGREEN            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/53E492E9298CCFD27EE620283293ADD6.svg) | (0 100 0)     |
| YELLOWGREEN          | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/763C70BD1C286C586B9B98C4D5677FB7.svg) | (154 205 50)  |
| OLIVEDRAB            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/F8F2CC3A8EAEB36967B0DB8183D771A8.svg) | (107 142 35)  |
| OLIVE                | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/A46966405152D70630735399A8CF3DAE.svg) | (128 128 0)   |
| DARKOLIVEGREEN       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/09F72A7048F22AD5823E65805B3B7B44.svg) | (85 107 47)   |
| MEDIUMAQUAMARINE     | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/F8468358BC3D14E0228EB11444ECD28D.svg) | (102 205 170) |
| DARKSEAGREEN         | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/516C6F44E5A56FD5D151C6D413A64A7B.svg) | (143 188 143) |
| LIGHTSEAGREEN        | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/2656EE4725A18C0FE2A536DBF0CFA420.svg) | (32 178 170)  |
| DARKCYAN             | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/153C280EAED89629D29E94235C1873A8.svg) | (0 139 139)   |
| TEAL                 | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/C33B3EEBEABB7D6F4AE7452914E193B3.svg) | (0 128 128)   |
|                      |                                                              |               |
|                      |                                                              |               |
| AQUA            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/84DEF24FAD90ABA7D7B40E2042696C62.svg) | (0 255 255)   |
| --------------- | ------------------------------------------------------------ | ------------- |
| CYAN            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/84DEF24FAD90ABA7D7B40E2042696C62.svg) | (0 255 255)   |
| LIGHTCYAN       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/CC53FC4D73201BE5211FCF7ECD647DD0.svg) | (224 255 255) |
| PALETURQUOISE   | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/5A515E85B93AB5E0BF1A3FAB3C90F60F.svg) | (175 238 238) |
| AQUAMARINE      | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/CCA4708E00F965B094D4BF66150E4FFA.svg) | (127 255 212) |
| TURQUOISE       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/613DD44F3C0E43853C6963C983A1E53B.svg) | (64 224 208)  |
| MEDIUMTURQUOISE | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/0E5FCF966D83F4BFD900F1466F5D0180.svg) | (72 209 204)  |
| DARKTURQUOISE   | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/E07F03350DEFCBD7A83ADAB09E8BD0AA.svg) | (0 206 209)   |
| CADETBLUE       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/45CC053C4C2D389FB6E24FEC06E0436B.svg) | (95 158 160)  |
| STEELBLUE       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/04D628C8C0B6483DA1A71A8CFAB05AA7.svg) | (70 130 180)  |
| LIGHTSTEELBLUE  | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/FF2AD94D7357D92EFDA1A7AB835D6CD5.svg) | (176 196 222) |
| POWDERBLUE      | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/4AE6A7BA3E55C14628B7B9408DCA00E8.svg) | (176 224 230) |
| LIGHTBLUE       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/D833C53F6F2CD55D7D215355FB9A5222.svg) | (173 216 230) |
| SKYBLUE         | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/3B7AAFD6229AB5F114CB11E0066241A3.svg) | (135 206 235) |
| LIGHTSKYBLUE    | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/78C77888B5379C8E915436C66C98BDB6.svg) | (135 206 250) |
| DEEPSKYBLUE     | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/7415D931C8D5D6E13EC2B31CB931EBFB.svg) | (0 191 255)   |
| DODGERBLUE      | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/235609BC5E137F06E9E760947FE3C683.svg) | (30 144 237)  |
| CORNFLOWERBLUE  | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/D9DB0E3110635C823DD8F1A69D368068.svg) | (65 105 225)  |
| ROYALBLUE       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/662254D5207CEDA8E5B0F37401A63EF7.svg) | (65 105 225)  |
| BLUE            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/0FC335219631FAF3C43DD2A44431EFAA.svg) | (0 0 255)     |
| MEDIUMBLUE      | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/E075439F90638F1AAF327912EE91E8CC.svg) | (0 0 205)     |
| DARKBLUE        | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/576983D5B1B9CEFE641B5FE252ECC1AD.svg) | (0 0 139)     |
| NAVY            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/EA577640F2D9F82046763FBA7E3DB7D2.svg) | (0 0 128)     |
| MIDNIGHTBLUE    | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/DD61B1797CC2CB559CE7D6352611E336.svg) | (25 25 112)   |
|                 |                                                              |               |
| CORNSILK        | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/131BE39D2FA13C57BDCA47DE8E051CB7.svg) | (255 248 220) |
| BLANCHEDALMOND  | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/DCA435E5C684C0226AF4908DCFDA2958.svg) | (255 235 205) |
| BISQUE          | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/652B7E934A70605A7919CE079002DA8F.svg) | (255 228 196) |
| NAVAJOWHITE     | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/26818A6D8664249380754C383E8864AE.svg) | (255 222 173) |
| WHEAT           | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/112C053107C2F5B75E2FB6018408DA82.svg) | (245 222 179) |
| BURLYWOOD       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/064EE95F6E8C1AA9FBBDF37230342320.svg) | (222 184 135) |
| TAN             | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/8ACAEDC3C52BAB6D815A8831E958ECA1.svg) | (210 180 140) |
| ROSYBROWN       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/A3510DD67CC10A235B8241A4955DD4D5.svg) | (188 143 143) |
| SANDYBROWN      | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/2FC339C6DE21E8C758DFBEA4BF4CCF1F.svg) | (244 164 96)  |
| GOLDENROD       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/62916B1760ED101C54B343DE805B57E6.svg) | (218 165 32)  |
| DARKGOLDENROD   | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/3BCC5322FD3A92017D615E005F355187.svg) | (184 134 11)  |
| PERU            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/4250ABEDF2D046E2B3F25F3239D5D880.svg) | (205 133 63)  |
| CHOCOLATE       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/0D4015427566807F5B9B8AD2D60A1A20.svg) | (210 105 30)  |
| SADDLEBROWN     | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/58D9948D8A83C614EA41ED5ABE73B70C.svg) | (139 69 19)   |
| SIENNA          | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/A957465B6D14EE0F14E0D1F55B17DA37.svg) | (160 82 45)   |
| BROWN           | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/88CA2AAE59271D1A06DD67AB84ABC8E9.svg) | (154 42 42)   |
| MAROON          | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/8FDB24E4E4393B2CF6514FEF865CF66B.svg) | (128 0 0)     |
|                 |                                                              |               |
| WHITE           | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/DE9D364C547E629F77126A00F333C456.svg) | (255 255 255) |
| SNOW            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/8C061519FF096A1013E656C00BFCA701.svg) | (255 250 250) |
| HONEYDEW        | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/078F4908E4FBBFA8344E4D3BFDD341C4.svg) | (240 255 240) |
| MINTCREAM       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/572CBEF31449E24A8056D64EBCE6B812.svg) | (245 255 250) |
| AZURE           | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/981475C4EF228AC3CE29FBB1DD44A296.svg) | (240 255 255) |
| ALICEBLUE       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/F9F1EE8957C36A4579D1A197EF9B10AE.svg) | (240 248 255) |
| GHOSTWHITE      | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/54B69C9A3A90921322660E4CDC9839C7.svg) | (248 248 255) |
| WHITESMOKE      | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/72E37E5DD74234B7C35355AA99DA9C49.svg) | (245 245 245) |
| SEASHELL        | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/A93203D8851275FBC336050B29D9C24F.svg) | (255 245 238) |
| BEIGE           | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/7578988F11E95EF2A78A15398097CC2E.svg) | (245 245 220) |
| OLDLACE         | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/2A95047DC24088F0E256E395500E0A9C.svg) | (253 245 230) |
| FLORALWHITE     | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/24E7EB18F75F466AD59E718D06EB98FF.svg) | (255 250 240) |
| IVORY           | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/06C5A24586E9D64153BE8FB391FA168A.svg) | (255 255 240) |
| ANTIQUEWHITE    | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/95CECEE6B77AFA0A27D4E495A9DAEE4B.svg) | (250 235 215) |
| LINEN           | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/82835F9D5B3ED460382BA0023395D780.svg) | (250 240 230) |
| LAVENDERBLUSH   | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/5277ADAA4CCFF1668321433861085F3E.svg) | (255 240 245) |
| MISTYROSE       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/9BB3B0745EB6AA6A88F6B849B1D6BE31.svg) | (255 228 225) |
|                 |                                                              |               |
| GAINSBORO       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/EE60215C154B75FCCDD9F276CEDE162E.svg) | (220 220 220) |
| LIGHTGRAY       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/DC523250BF92BCD1586B0B34EF6769FB.svg) | (211 211 211) |
| SILVER          | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/D831E9CC739BF90FD1FE3BAFDB5F75A9.svg) | (192 192 192) |
| DARKGRAY        | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/3EC519DAA3A09C90E74783BBB30F1814.svg) | (169 169 169) |
| GRAY            | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/79658E4A7545F4542DA7C098591F4FD2.svg) | (128 128 128) |
| DIMGRAY         | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/2FACB14CED895E917A95D48248DD37DA.svg) | (105 105 105) |
| LIGHTSLATEGRAY  | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/5B05B21472029905DC1AEFE9A13C548B.svg) | (119 136 153) |
| SLATEGRAY       | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/F64C4E7C20FAD49874F0A118AF16BCAF.svg) | (112 128 144) |
| DARKSLATEGRAY   | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/E9A4BD5D1CDCA88CDA761B676B7E5E8A.svg) | (47 79 79)    |
| BLACK           | ![\( \blacksquare   \)](http://springhead.info/dailybuild/generated/doc/SprManual/lateximages/C0E0714ADD234EB77D3BA6B9F320BBB5.svg) | (0 0 0)       |



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
描画時のお決まりの処理です．[シーン](#シーン)の項目を参照してください．

|*GRRenderIf*| 									 ||
|---|---|---|---|
|*void*| *GetClearColor(Vec4f&)*		| 背景色の取得				|
|*void*| *SetClearColor(const Vec4f&)*| 背景色の設定				|
|*void*| *ClearBuffer()*				| 描画バッファをクリア		|
|*void*| *BeginScene()*				| 描画の開始				|
|*void*| *EndScene()*					| 描画の完了				|
|*void*| *SwapBuffers()*				| 描画バッファのスワップ	|

### ディスプレイリスト
ディスプレイリストに関係する機能です．*GRMesh*が内部で使用します．

|*GRRenderIf*| 					 ||
|---|---|---|
|*int* | *StartList()*		| ディスプレイリスト作成開始	|
|*void*| *EndList()*			| ディスプレイリスト作成完了	|
|*void*| *DrawList(int)*		| ディスプレイリストの描画		|
|*void*| *ReleaseList(int)*	| ディスプレイリストの解放		|

### デプステスト，アルファブレンディング，ライティング
描画機能を切り替えるための関数です．

|*GRRenderIf*								|										 ||
|---|---|---|
|*void*| *SetDepthWrite(bool)*				| デプスバッファへの書き込みOn/Off		|
|*void*| *SetDepthTest(bool)*					| デプステストのOn/Off					|
|*void*| *SetDepthFunc(TDepthFunc)*			| デプスバッファの判定条件				|
|*void*| *SetAlphaTest(bool)*					| アルファブレンディングのOn/Off		|
|*void*| *SetAlphaMode(TBlendFunc, TBlendFunc)*| アルファブレンディングのモード		|
|*void*| *SetLighting(bool)*					| ライティングのOn/Off					|

### テクスチャ


|*GRRenderIf*													|						 ||
|---|---|---|
|*int* | *LoadTexture(UTString)*									| テクスチャのロード	|
|*void* | *SetTextureImage(UTString, int, int, int, int, char*)*	| テクスチャの設定		|

### シェーダ


|*GRRenderIf*											|								 ||
|---|---|---|
|*void* 	| *InitShader()*									| シェーダの初期化				|
|*void* 	| *SetShaderFormat(ShaderType)*					| シェーダフォーマットの設定	|
|*bool* 	| *CreateShader(UTString, UTString, GRHandler&)*| シェーダオブジェクトの作成	|
|*GRHandler* | *CreateShader()*								| シェーダオブジェクトの作成	|
|*bool* 	| *ReadShaderSource(GRHandler, UTString)*		| シェーダプログラムをロード	|
|*void* 	| *GetShaderLocation(GRHandler, void*)*			| ロケーション情報の取得		|

### 直接描画


|*GRRenderIf*																 |||
|---|---|---|
|*void*| *SetVertexFormat(const GRVertexElement*)*						| 頂点フォーマットの指定	|
|*void*| *SetVertexShader(void*)*										| 頂点シェーダーの指定		|
|*void*| *DrawDirect(TPrimitiveType, void*, size_t, size_t)*			| 頂点を指定してプリミティブを描画	|
|*void*| *DrawIndexed(TPrimitiveType, size_t*, void*, size_t, size_t)*| 頂点とインデックスを指定してプリミティブを描画	|
|*void*| *DrawArrays(TPrimitiveType, GRVertexArray*, size_t)*			| 頂点の成分ごとの配列を指定して，プリミティブを描画	|
|*void*| *DrawArrays(TPrimitiveType, size_t*, GRVertexArray*, size_t)*	| インデックスと頂点の成分ごとの配列を指定して，プリミティブを描画	|

### 基本形状描画


|*GRRenderIf*																					 |||
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


|*GRRenderIf*												 |||
|---|---|---|---|
|*void* 				| *SetCamera(const GRCameraDesc&)*| カメラの設定	|
|*const GRCameraDesc&* | *GetCamera()*					| カメラの取得	|

### ライト


|*GRRenderIf*													 |||
|---|---|---|---|
|*void*| *PushLight(const GRLightDesc&)*| ライトをプッシュ	|
|*void*| *PushLight(const GRLightIf*)*| ライトをプッシュ	|
|*void*| *PopLight()*					| ライトをポップ	|
|*int* | *NLights()*					| ライトの数		|

### 座標変換


|*GRRenderIf*												 |||
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
