
## 概要



Frameworkはモジュール間の連携を促進してアプリケーションの作成を支援するためのモジュールです．Frameworkモジュールのデータ構造を次図に示します．最上位にはアプリケーションクラス*FWApp*があります．ユーザは*FWApp*を継承することで独自のアプリケーションを作成します．*FWApp*その中にトップレベルウィンドウ(*FWWin*)の配列，Framework SDK (*FWSdk*)，およびウィンドウマネジャ(*FWGraphicsAdaptee*)を持ちます．*FWWin*はトップレベルウィンドウで，そのウィンドウに対応する入力デバイスやビューポート情報を保持するレンダラ，そのウィンドウと関連付けられたシーンへの参照などを持ちます．また，図中では省略されていますがサブウィンドウやGUIコントロールを持つこともできます．*FWSdk*の役割は周辺モジュールの機能統合です．その中に周辺モジュールのSDKクラスやFrameworkシーン(*FWScene*)の配列を持ちます．ウィンドウマネジャは処理系に依存するデバイスの初期化やイベントハンドリングを行います．ウィンドウマネジャはインタフェースを公開していませんのでユーザはその存在を陽に意識する必要はありません．図ではデータ構造の説明のためにあえて記載しています．以下では個々の構成要素について説明していきます．
## Framework SDK
FrameworkモジュールのすべてのオブジェクトはSDKクラス*FWSdk*によって管理されます．*FWSdk*クラスは，プログラムの実行を通してただ１つのオブジェクトが存在するシングルトンクラスです．*FWSdk*オブジェクトを作成するには以下のようにします．
```c++
FWSdkIf* fwSdk = FWSdkIf::CreateSdk();
```
通常この操作はプログラムの初期化時に一度だけ実行します．*FWSdk*を作成すると，同時に*PHSdk*，*GRSdk*，*FISdk*，*HISdk*も作成されます．したがってこれらをユーザが手動で作成する必要はありません．各モジュールの機能にアクセスするには以下の関数によりSDKを取得します．


|*FWSdkIf*				 |
|---|
|*PHSdkIf* GetPHSdk()*	|
|Physics SDKを取得する．			|
|*GRSdkIf* GetGRSdk()*	|
|Graphics SDKを取得する．		|
|*FISdkIf* GetFISdk()*	|
|FileIO SDKを取得する．			|
|*HISdkIf* GetHISdk()*	|
|HumanInterface SDKを取得する．	|

## Framework シーン



Frameworkモジュールの主な機能の1つにPhysicsシーンとGraphicsシーンの同期があります．次図に3つのモジュールのSDKとシーンの関係を示します．*FWSdk*は任意の数のシーン（*FWScene*クラス）を保持します．また，シーンは任意の数のオブジェクト（*FWObject*クラス）を保持します．次図に示すように，オブジェクトはPhysicsモジュールの剛体とGraphicsモジュールのトップフレームを一対一に対応づけます．ここでトップフレームとはワールドフレームの直下にあるフレームのことです．物理シミュレーションにより計算される剛体の運動をフレームの座標変換に反映させることで，シミュレーションの様子をGraphicsモジュールの機能を利用して可視化することができるようになります．シーン作成に関する*FWSdk*の関数を以下に示します．


|*HITrackballIf*														 |
|---|
|*FWSceneIf* CreateScene(const PHSceneDesc&, const GRSceneDesc&)*	|
|シーンを作成する．															|
|*int NScene()*	|
|シーンの数を取得する	|
|*FWSceneIf* GetScene(int i)*	|
|*i*番目のシーンを取得する．	|
|*void MergeScene(FWSceneIf* scene0, FWSceneIf* scene1)*	|
|*scene1*の子オブジェクトを*scene0*に移す．		|
シーンを作成するには以下のようにします．
```c++
FWSceneIf* fwScene = fwSdk->CreateScene();
```
*FWScene*を作成すると，同時に*PHScene*と*GRScene*も作成され，*FWScene*とリンクされます．*CreateScene*にディスクリプタを指定することもできます．*NScene*は作成したシーンの数を返します．シーンを取得するには*GetScene*を使います．*GetScene*に指定する整数は作成された順番にシーンに与えられる通し番号です．
```c++
fwSdk->CreateScene();               // create two scenes
fwSdk->CreateScene();
FWSceneIf *fwScene0, *fwScene1;
fwScene0 = fwSdk->GetScene(0);      // get 1st scene
fwScene1 = fwSdk->GetScene(1);      // get 2nd scene
```
*MergeScene*を使うと2つのシーンを統合して1つのシーンにできます．
```c++
fwSdk->MergeScene(fwScene0, fwScene1);
```
上のコードでは*scene1*が持つ*FWObject*が*scene0*に移され，同時にシーンが参照する*PHScene*と*GRScene*に関してもそれぞれの*MergeScene*関数により統合が行われます．次に，*FWScene*の基本機能を以下に示します．


|*FWSceneIf*													 |
|---|
|*void SetPHScene(PHSceneIf*)*| Physicsシーンの設定		|
|*PHSceneIf* GetPHScene()*	| Physicsシーンの取得		|
|*void SetGRScene(GRSceneIf*)*| Graphicsシーンの設定		|
|*GRSceneIf* GetGRScene()*	| Graphicsシーンの取得		|
|*FWObjectIf* CreateFWObject()*| オブジェクトの作成		|
|*int NObject()const*			| オブジェクトの数			|
|_FWObjectIf*_ GetObjects()*	| オブジェクト配列の取得	|
|*void Sync(bool)*			| 同期						|
*[Set|Get][PH|GR]Scene*関数はシーンに割り当てられた*PHScene*や*GRScene*を取得したり，別のシーンを割り当てたりするのに使用します．*CreateFWObject*関数は*FWObject*オブジェクトを作成します．このとき，新たに作成された*FWObject*には*PHSolid*および*GRFrame*は割り当てられていない状態になっているので注意してください．これらも同時に作成するには，以下のコードを1セットで実行します．
```c++
FWObjectIf* fwObj = fwScene->CreateFWObject();
fwObj->SetPHSolid(fwScene->GetPHScene()->CreateSolid());
fwObj->SetGRFrame(
    fwScene->GetGRScene()->CreateVisual(GRFrameDesc())->Cast);
```
*Sync*関数は*PHScene*と*GRScene*の同期に用います．
```c++
fwScene->Sync(true);
```
とすると，このシーンが参照する*PHScene*中の剛体の位置と向きが，同じくこのシーンが参照する*GRScene*中のトップフレームの位置と向きに反映されます．このときの剛体とトップフレームとの対応関係は*FWObject*により定義されます．逆に
```c++
fwScene->Sync(false);
```
とすると，同様のメカニズムで各トップフレームの位置と向きが対応する剛体に反映されます．
## シーンのロードとセーブ
FileIOモジュールを利用してシーンをロード，セーブするための関数が用意されています．まずロードには以下の関数を用います．


|*FWSdkIf*														 |
|---|
|*bool LoadScene(UTString path, ImportIf* imp, const IfInfo* ii, ObjectIfs* objs)*	|
|シーンをロードする．		|
*path*はロードするファイルへのパスを格納した文字列です．*imp*にはインポート情報を格納するための*Import*オブジェクトを与えます．インポート情報を記憶する必要のない場合は*NULL*で構いません．*ii*はロードするファイルの種類を明示するための型情報です．*NULL*を指定するとパスの拡張子から自動判別されます．*objs*はロードによって作成されるオブジェクトツリーの親オブジェクトを格納した配列です．ロードに成功すると*true*，失敗すると*false*が返されます．ロードされたシーンは*FWSdk*のシーン配列の末尾に加えられます．次に，シーンをセーブするには以下の関数を使います．


|*FWSdkIf*														 |
|---|
|*bool SaveScene(UTString path, ImportIf* imp, const IfInfo* ii, ObjectIfs* objs)*	|
|シーンをセーブする．		|
引数の意味は*LoadScene*と同様です．*imp*にはロード時に記憶したインポート情報を与えます．省略するとシーン全体が単一のファイルにセーブされます．セーブに成功すると*true*，失敗すると*false*が返されます．
## Framework オブジェクト
*FWObject*は*PHSolid*と*GRFrame*の橋渡しが主な役割ですので，それ自体はそれほど多くの機能を持っていません．
## アプリケーションクラス
Springheadを利用するアプリケーションの作成を容易にするために，アプリケーションクラス*FWApp*が用意されています．\ref{sec_create_application}に*FWApp*を使って簡単なアプリケーションを作成する方法について説明しましたのでそちらも合わせて参考にしてください．冒頭で説明した通り，Springheadのほとんどのオブジェクトは，親オブジェクトの*Create*系関数を使って作成しますが，*FWApp*は例外的に，C++のクラス継承を用いてユーザのアプリケーションクラスを定義する方法をとります．この方が仮想関数によって動作のカスタマイズがフレキシブルに行えるからです．以下では*FWApp*の機能やユーザが実装すべき仮想関数について順に見ていきます．
### 初期化
*FWApp*の初期化処理は仮想関数*Init*で行います．


|*FWApp*											 |
|---|
|*virtual void Init(int argc, char* argv[])*|	|
以下に*Init*関数のデフォルトの実装を示します．
```c++
void FWApp::Init(int argc, char* argv[]){
    // create SDK
    CreateSdk();
    // create a single scene
    GetSdk()->CreateScene();
    // initialize window manager
    GRInit(argc, argv);
    // create main window
    CreateWin();
    // create timer
    CreateTimer();
}
```
はじめに
```c++
    CreateSdk();
```
でSDKを作成します．つぎに
```c++
    GRInit(argc, argv);
```
でウィンドウマネジャが作成されます．デフォルトではGLUTを用いるウィンドウマネジャが作成されます．さらに
```c++
    GetSdk()->CreateScene();
```
で*FWScene*を1つ作成します．つづいて
```c++
    CreateWin();
```
でメインウィンドウを作成します．最後に
```c++
    CreateTimer();
```
でタイマを作成します．この基本処理に追加してなんらかの処理を行う場合は
```c++
virtual void Init(int argc = 0, char* argv[] = 0){
    // select GLUI window manager
    SetGRAdaptee(TypeGLUI);

    // call base Init
    FWApp::Init(argc, argv);

    // do extra initialization here


}
```
のように，*FWApp:Init*を実行してから追加の処理を行うのが良いでしょう．一方，以下に挙げるようなカスタマイズが必要な場合は*Init*関数の処理全体を派生クラスに記述する必要があります．

-  シーン生成をカスタマイズしたい
-  ウィンドウの初期サイズやタイトルを変更したい
-  異なる種類のタイマが使いたい

この場合は，上に載せた*Init*のデフォルト処理をもとに必要な部分に修正を加えるのが良いでしょう．プログラムの全体の構造は通常以下のようになります．
```c++
MyApp app;

int main(int argc, char* argv[]){
    app.Init(argc, argv);
    app.StartMainLoop();
    return 0;
}
```
ここで*MyApp*はユーザが定義した*FWApp*の派生クラスです（もちろん他の名前でも構いません）．*MyApp*のインスタンスをグローバル変数として定義し，*main*関数で*Init*，*StartMainLoop*を順次実行します．*StartMainLoop*関数はアプリケーションのメインループを開始します．
### タイマ
タイマの作成には*CreateTimer*関数を使います．通常，*CreateTimer*は*Init*の中で呼びます．


|*FWApp*												 |
|---|
|*UTTimerIf* CreateTimer(UTTimerIf::Mode mode)*|	|
引数*mode*に指定できる値は*UTTimer*の*SetMode*と同じです．\ref{sec_uttimer}節を参照してください．戻り値として*UTTimer*のインタフェースが返されます．周期などの設定はこのインタフェースを介して行います．シミュレーション用と描画用に2つのタイマを作成する例を以下に示します．
```c++
UTTimerIf *timerSim, *timerDraw;
timerSim = CreateTimer(MULTIMEDIA);
timerSim->SetInterval(10);
timerDraw = CreateTimer(FRAMEWORK);
timerDraw->SetInterval(50);
```
この例ではシミュレーション用には周期を*10*[ms]のマルチメディアタイマを使い，描画用には周期*50*[ms]のフレームワークタイマ（GLUTタイマ）を使っています．タイマを始動すると，周期ごとに以下の仮想関数が呼ばれます．


|*FWApp*								 |
|---|
|*virtual void TimerFunc(int id)*|	|
タイマの判別は引数*id*で行います．*TimerFunc*のデフォルトの振る舞いでは，カレントウィンドウのシーンの*Step*を呼び，つぎに*PostRedisplay*で再描画要求を発行します（その結果，直後に*Display*関数が呼び出されます）．この振る舞いをカスタマイズしたい場合は*TimerFunc*関数をオーバライドします．
```c++
void TimerFunc(int id){
    // proceed simulation of scene attached to current window
    if(id == timerSim->GetID()){
        GetCurrentWin()->GetScene()->Step();
    }
    // generate redisplay request
    else if(id == timerDraw->GetID()){
        PostRedisplay();
    }
}
```
この例ではシミュレーションと描画に異なる2つのタイマを使用しています．
### 描画
描画処理は次の仮想関数で行います．


|*FWApp*						 |
|---|
|*virtual void Display()*|	|
*Display*は描画要求が発行されたときに呼び出されます．描画要求は*PostRedisplay*関数で行います．


|*FWApp*							 |
|---|
|*virtual void PostRedisplay()*|	|
*Display*関数のデフォルトの振る舞いではカレントウィンドウの*Display*関数が呼ばれます．
### キーボード・マウスイベント
*FWApp*は各ウィンドウに関連付けられた仮想キーボード・マウスデバイス*DVKeyMouse*にコールバック登録されています．したがって以下の仮想関数をオーバライドすることでキーボード・マウスイベントを処理できます．


|*FWApp*							 |
|---|
|*virtual bool OnMouse(int button, int state, int x, int y)*	|
|*virtual bool OnDoubleClick(int button, int x, int y)*	|
|*virtual bool OnMouseMove(int state, int x, int y, int zdelta)*	|
|*virtual bool OnKey(int state, int key, int x, int y)*	|
各イベントハンドラの詳細については\ref{sec_hi_keymouse}節を参照して下さい．
## ウィンドウ
ウィンドウやその他のGUIコントロールの作成もFrameworkによってサポートされています．すでに述べてきたとおり，*FWApp*はトップレベルウィンドウの配列を持ちます．
## Frameworkを用いたシミュレーションと描画
Frameworkモジュールを介して物理シミュレーションを行うには以下の関数を使います．


|*FWSdkIf*			 |
|---|
|*void Step()*| 	|

*FWSdk*の*Step*はアクティブシーンの*Step*を呼びます．したがって*GetScene()->Step()*と等価です．一方*FWScene*の*Step*は，保持している*PHScene*の*Step*を呼びます．したがって*GetPHScene()->Step()*と等価です．両方とも薄いラッパー関数ですが，ユーザのタイプ回数節約のために用意されています．Frameworkを用いた描画には2通りの方法があります．1つはGraphicsのシーングラフを用いる方法，もう1つはPhysicsシーンを直接描画する方法です．後者はデバッグ描画とも呼ばれています．


|*FWSdkIf*						 |
|---|
|*void Draw()*			|	|
|*void SetDebugMode(bool)*| 	|
|*bool GetDebugMode()*	|	|
*Draw*関数は描画モードに応じた描画処理を行います．*Draw*は通常アプリケーションの描画ハンドラから呼び出します．*[Set|Get]DebugMode*は通常描画モード(*false*)とデバッグ描画モード(*true*)を切り替えます．通常描画モードにおいて*Draw*関数を呼ぶと，はじめにアクティブシーンについて*Sync(true)*が呼ばれ，剛体の状態がシーングラフに反映されます．次にアクティブシーンが参照する*GRScene*の*Render*関数が呼ばれ，シーングラフが描画されます．この方法ではシーングラフが持つライトやテクスチャなどの情報を最大限利用してフォトリアリスティックな描画が可能です．その反面，物理シミュレーションが主目的である場合にはシーングラフの構築という付加的なコストを支払わなければならないというデメリットもあります．デバッグ描画については次節で説明します．
## デバッグ描画
デバッグ描画モードでは*PHScene*の情報だけを用いて描画が行われるので，シーングラフ構築の手間が省けます．また，剛体に加わる力などの物理シミュレーションに関する情報を可視化することができます．一方で，予約色しか使えないなど，描画の自由度には一定の制約が生じます．デバッグ描画モードでは*FWScene*の*DrawPHScene*関数により描画処理が行われます．


|*FWSceneIf*									 |
|---|
|*void DrawPHScene(GRRenderIf* render)*|	|
*DrawPHScene*は，各剛体に割り当てられている衝突判定形状，座標軸，作用している力，接触断面などを描画します．項目別に描画を行ったり，描画色を設定するには後述する描画制御関数を用います．
### デバッグ描画時のカメラとライト
デバッグ描画においてもカメラの情報は*GRScene*が参照されます．もし*GRScene*がカメラを保有している場合はそのカメラの*Render*が呼ばれ，視点と投影変換が設定されます．*GRScene*がカメラを持たない場合は手動で設定する必要があります．ライトについては，もし外部でレンダラに対してライト設定がされている場合はその設定が優先され，レンダラが1つもライトを持たない場合は内部でデフォルトライトが設定されます．
### 個別の描画
以下の関数は*DrawPHScene*から呼び出されますが，ユーザが個別に呼び出すこともできます．


|*FWSceneIf*												 |
|---|
|*void DrawSolid(GRRenderIf*, PHSolidIf*, bool)*	|	剛体を描画|
|*void DrawShape(GRRenderIf*, CDShapeIf*, bool)*	|	形状を描画|
|*void DrawConstraint(GRRenderIf*, PHConstraintIf*)*|	拘束を描画|
|*void DrawContact(GRRenderIf*, PHContactPointIf*)*|	接触を描画|
|*void DrawIK(GRRenderIf*, PHIKEngineIf*)*		|	IK情報を描画|

### 描画制御
以下の関数は描画のOn/Offを切り替えます．


|*FWSceneIf*													 |
|---|
|*void SetRenderMode(bool solid, bool wire)*				|	|
|*void EnableRender(ObjectIf* obj, bool enable)*			|	|
|*void EnableRenderAxis(bool world, bool solid, bool con)*|	|
|*void EnableRenderForce(bool solid, bool con)*			|	|
|*void EnableRenderContact(bool enable)*					|	|
|*void EnableRenderGrid(bool x, bool y, bool z)*			|	|
|*void EnableRenderIK(bool enable)*						|	|
*SetRenderMode*はソリッド描画（面を塗りつぶす）とワイヤフレーム描画（面の輪郭）のOn/Offを切り替えます．*EnableRender*は指定したオブジェクトの描画のOn/Offを切り替えます．項目ではなくオブジェクトレベルで描画制御したい場合に便利です．*obj*に指定できるのは剛体(_PHSolidIf*_)か拘束(_PHConstraintIf*_)です．*EnableRenderAxis*は項目別に座標軸の描画を設定します．*world*はワールド座標軸，*solid*は剛体，*con*は拘束の座標軸です．*EnableRenderForce*は力とモーメントの描画を設定します．*solid*は剛体に加わる力（ただし外力のみで拘束力は除く），*con*は拘束力です．*EnableRenderGrid*は各軸に関してグリッドの描画を設定します．*EnableRenderIK*はIK情報の描画を設定します．以下の関数は描画属性を指定するのに使います．


|*FWSceneIf*																 |
|---|
|*void SetSolidMaterial(int mat, PHSolidIf* solid)*					|	|
|*void SetWireMaterial (int mat, PHSolidIf* solid)*					|	|
|*void SetAxisMaterial(int matX, int matY, int matZ)*					|	|
|*void SetAxisScale(float world, float solid, float con)*				|	|
|*void SetAxisStyle(int style)*										|	|
|*void SetForceMaterial(int matForce, int matMoment)*					|	|
|*void SetForceScale(float scaleForce, float scaleMoment)*			|	|
|*void SetContactMaterial(int mat)*									|	|
|*void SetGridOption(char axis, float offset, float size, int slice)*	|	|
|*void SetGridMaterial(int matX, int matY, int matZ)*					|	|
|*void SetIKMaterial(int mat)*										|	|
|*void SetIKScale(float scale)*										|	|
*SetSolidMaterial*は指定した剛体のソリッド描画色を指定します．*mat*に指定できる値は\ref{sec_grmaterial}節で述べた予約色です．*solid*に*NULL*を指定するとすべての剛体の色が指定された値になります．*SetWireMaterial*は同様に剛体のワイヤフレーム描画色を指定します．*SetAxisMaterial*は座標軸の色をx, y, z個別に指定します．*SetAxisScale*は座標軸の縮尺を指定します．*SetAxisStyle*は座標軸のスタイルを指定します．*SetForceMaterial*，*SetForceScale*はそれぞれ力（並進力とモーメント）の描画色と縮尺を指定します．*SetContactMaterial*は接触断面の描画色を指定します．*SetGridOption*はグリッドのオプションを指定します．*SetGridMaterial*はグリッドの描画色を指定します．*SetIKMaterial*，*SetIKScale*はIK情報の描画色と縮尺を指定します．
## 力覚インタラクションのためのアプリケーション
Springhead2にはシーンとの力覚インタラクションのためのエンジン*PHHapticEngine*が含まれています．ここでは力覚インタラクションのためのアプリケーションの作成方法について説明します．まずは，通常の*Framework*アプリケーションの作成と同様に，ひな形クラスである*FWApp*を継承してアプリケーションを作成します．そして，*Init*関数内で力覚インタラクションを有効化と，力覚インタラクションシステムのモードを設定します．
```c++
	// given PHSceneIf* phScene,
    phScene->GetHapticEngine()->EnableHapticEngine(true);
    phScene->GetHapticEngine()->
    SetHapticEngineMode(PHHapticEngineDesc::MULTI_THREAD);
```
力覚インタラクションシステムのモードはシングルスレッドアプリケーションのための*SINGLE\_THREAD*，マルチメディアアプリケーションのための*MULTI\_THREAD*，局所シミュレーションを利用した*LOCAL\_DYNAMICS*の3種類があります．標準では*MULTI\_THREAD*が設定されています．*MULTI\_THREAD*，*LOCAL\_DYNAMICS*のモードはマルチスレッドを利用したアプリケーションとなり，物理シミュレーションを実行する物理スレッド，力覚レンダリングを実行する力覚スレッドが並列に動きます．そのため，それぞれのスレッドをコールバックするためにタイマを設定し直す必要があります．\clearpage
```c++
	// given PHSceneIf* phScene,
	int physicsTimerID, hapticTimerID // 各タイマのID
	// FWApp::TimerFuncをオーバライドしたコールバック関数
	void MyApp::TimerFunc(int id){
        if(hapticTimerID == id){
            // 力覚スレッドのコールバック
            phScene->StepHapticLoop();	
        }else{
            // 物理スレッドのコールバック
            phScene->GetHapticEngine()->StepPhysicsSimulation();	
            PostRedisplay();	// 描画
        }	
	}	
```
次にユーザがオブジェクトとインタラクションするためのポインタ，力覚ポインタ*PHHapticPointer*を作ります．そして，どのインタフェースと結合するのかを設定します．*PHHapticPointer*は*PHScene*から作ることができます．*PHHapticPointer*は*PHSolid*を継承したクラスで*PHSolid*の関数を利用して，質量，慣性テンソル，形状などを合わせて設定します．例えばSpidar-G6と接続する場合には，
```c++
	// given PHSceneIf* phScene,
	// given HISpidarIf* spg,
    PHHapticPointerIf* pointer = phScene->CreateHapticPointer();
    /*
        質量，慣性テンソル，形状などを設定する
    */
    pointer->SetHumanInterface(spg);
```
とします．さらにPHHapticPointerについて以下の関数を用いて，力覚提示のためのパラメータを設定します．


|*PHHapticPointerIf*													 |
|---|
|*void SetHumanInterface(HIBaseIf* interface)*					|	|
|*void SetDefaultPose(Posed pose)*								|	|
|*void SetPosScale(double scale)*									|	|
|*void SetReflexSpring(float s)*									|	|
|*void SetReflexDamper(float s)*									|	|
|*void EnableFriction(bool b)*									|	|
|*void EnableVibration(bool b)*									|	|
|*void SetLocalRange(float s)*									|	|
|*void SetHapticRenderMode(PHHapticPointerDesc::HapticRenderMode m )*|	|
*SetHumanInterface*は力覚ポインタにヒューマンインタフェースを割り当てます．*SetDefaultPose*はシーン内での力覚ポインタの初期位置を指定します．*SetPosScale*はシーン内での力覚ポインタの可動スケールを指定します．*SetReflexSpring*は力覚レンダリング（反力計算）のためのバネ係数値を設定します．*SetReflexDamper*は力覚レンダリングのためのダンパ係数値を設定します．*EnableFriction*は力覚ポインタの摩擦力提示を有効化します．*EnableVibration*は力覚ポインタの振動提示を有効化します．*SetLocalRange*は局所シミュレーションシステムを使用時の局所シミュレーション範囲を指定します．*SetHapticRenderMode*は力覚レンダリングのモードを指定します．最後の*SetHapticRenderMode*には*PENALTY*，*CONSTRAINT*のモードがあります．*PENALTY*は力覚ポインタが剛体に接触した時の各接触点の侵入量とバネダンパ係数を乗じたものを足しあわせたものが反力として計算され，インタフェースから出力されます．*CONSTRAINT*は力覚ポインタが剛体に侵入していない状態（プロキシ）を求め，力覚ポインタとプロキシの距離の差分にバネダンパ係数を乗じたものを反力として計算します．
