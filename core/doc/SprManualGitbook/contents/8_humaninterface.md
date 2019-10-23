
## 概要
HumanInterfaceモジュールは，ハードウェアや入力デバイスを利用するための処理系に依存しないインタフェースを提供します．ほとんどの場合，HumanInterfaceの機能はFrameworkモジュールを介してアクセスすることになります．この場合は，後述するヒューマンインタフェースオブジェクトやデバイスの作成をユーザ自身で行う必要はありません．
## HumanInterface SDK
HumanInterfaceモジュールのすべてのオブジェクトはSDKクラス*HISdk*によって管理されます．*HISdk*クラスは，プログラムの実行を通してただ１つのオブジェクトが存在するシングルトンクラスです．*HISdk*オブジェクトを作成するには以下のようにします．
```c++
HISdkIf* hiSdk = HISdkIf::CreateSdk();
```
通常この操作はプログラムの初期化時に一度だけ実行します．また，Frameworkモジュールを使用する場合はユーザが直接*HISdk*を作成する必要はありません．
## クラス階層とデータ構造



HumanInterfaceモジュールのクラス階層を次図に示します．デバイスには実デバイスと仮想デバイスがあります．実デバイスは現実のハードウェアに対応し，例えばWin32マウスやあるメーカのA/D変換ボードを表す実デバイスがあります．一方，仮想デバイスは実デバイスが提供する機能単位を表し，処理系に依存しません．例えば，1つのA/D変換ポートや抽象化されたマウスインタフェースがこれにあたります．基本的に，初期化時を除いてはユーザは実デバイスに触れることはなく，仮想デバイスを通じてそれらの機能を利用することになります．ヒューマンインタフェースはデバイスよりも高度で抽象化された操作インタフェースを提供します．


次にHumanInterfaceモジュールのデータ構造を次図に示します．*HISdk*オブジェクトはヒューマンインタフェースプールとデバイスプールを持っています．デバイスプールとは実デバイスの集まりで，それぞれの実デバイスはその機能をいくつかの仮想デバイスとして外部に提供します．デバイスの機能を使うには，

1.  実デバイスを作成する
1.  実デバイスが提供する仮想デバイスにアクセスする

という2段階の手順を踏みます．以下にそれに関係する*HISdk*の関数を紹介します．

|*HISdkIf*																		 |
|---|---|
|_HIRealDeviceIf*_| *AddRealDevice(const IfInfo* ii, const void* desc = NULL)* |
|_HIRealDeviceIf*_| *FindRealDevice(const char* name)* |
|_HIRealDeviceIf*_| *FindRealDevice(const IfInfo* ii)*|
*AddRealDevice*は型情報*ii*とディスクリプタ*desc*を指定して実デバイスを作成します．*FindRealDevice*は名前か型情報を指定して，既存の実デバイスを検索します．たとえば，内部でGLUTを用いるキーボード・マウス実デバイスを取得するには
```c++
hiSdk->FindRealDevice(DRKeyMouseGLUTIf::GetIfInfoStatic());
```
とします．仮想デバイスを取得および返却する方法には*HISdk*を介する方法と*HIRealDevice*を直接呼び出す方法の2通りがあります．

|*HISdkIf*																							 |
|---|---|
|_HIVirtualDeviceIf*_| *RentVirtualDevice(const IfInfo* ii, const char* name, int portNo)*	|
|*bool*			| *ReturnVirtualDevice(HIVirtualDeviceIf* dev)*	|
*RentVirtualDevice*はデバイスプールをスキャンして型情報に合致した最初の仮想デバイスを返します．実デバイスを限定したい場合は*name*で実デバイス名を指定します．また，複数の仮想デバイスを提供する実デバイスもあります．この場合はポート番号*portNo*で取得したい仮想デバイスを指定できます．デバイスの競合を防ぐために，一度取得された仮想デバイスは利用中状態になります．利用中のデバイスは新たに取得することはできません．使い終わったデバイスは*ReturnVirtualDevice*で返却することによって再び取得可能になります．

|*HIRealDeviceIf*																				 |
|---|---|
|_HIVirtualDeviceIf*_| *Rent(const IfInfo* ii, const char* name, int portNo)*	|
|*bool*			| *Return(HIVirtualDeviceIf* dev)*|
こちらは実デバイスから直接取得，返却するための関数です．機能は同様です．
## 実デバイス
Springheadではいくつかのメーカ製のハードウェアが実デバイスとしてサポートされていますが，処理系に強く依存する部分であるため本ドキュメントの対象外とします．興味のある方はソースコードを見てください．
## キーボード・マウス
キーボードおよびマウスの機能は包括して1つのクラスとして提供されています．キーボード・マウスの仮想デバイスは*DVKeyMouse*です．実デバイスとしてはWin32 APIを用いる*DRKeyMouseWin32*とGLUTを用いる*DRKeyMouseGLUT*があります．提供される機能に多少の差異があるので注意して下さい．
### 仮想キーコード
Ascii外の特殊キーには処理系依存のキーコードが割り当てられています．この差を吸収するために以下のシンボルが*DVKeyCode*列挙型で定義されています．

|*DVKeyCode*									 |
|---|---|
|*ESC*			| エスケープ			|
|*F1* - *F12*| ファンクションキー	|
|*LEFT*			| ←					|
|*UP*				| ↑					|
|*RIGHT*			| →					|
|*DOWN*			| ↓					|
|*PAGE_UP*		| Page Up				|
|*PAGE_DOWN*		| Page Down				|
|*HOME*			| Home					|
|*END*			| End					|
|*INSERT*			| Insert				|
必要に応じてシンボルが追加される可能性がありますので，完全なリストはヘッダファイルで確認してください．
### コールバック
*DVKeyMouse*からのイベントを処理するには*DVKeyMouseCallback*クラスを継承し，イベントハンドラをオーバライドします．*DVKeyMouseCallback*はいくつかのヒューマンインタフェースクラスが継承しているほか，後述するアプリケーションクラス*FWApp*も継承しています．

|*DVKeyMouseCallback*								 |
|---|---|
|*virtual bool*| *OnMouse(int button, int state, int x, int y)*		|
|マウスボタンプッシュ/リリース| 	|
|*virtual bool*| *OnDoubleClick(int button, int x, int y)*			|
|ダブルクリック| 	|
|*virtual bool*| *OnMouseMove(int button, int x, int y, int zdelta)*	|
|マウスカーソル移動/マウスホイール回転| 	|
|*virtual bool*| *OnKey(int state, int key, int x, int y)*			|
|キープッシュ/リリース| 	|
*OnMouse*はマウスボタンのプッシュあるいはリリースが生じたときに呼び出されます．*button*はイベントに関係するマウスボタンおよびいくつかの特殊キーの識別子を保持し，その値は*DVButtonMask*列挙子の値のOR結合で表現されます．*state*はマウスボタン状態変化を示し，*DVButtonSt*列挙子のいずれかの値を持ちます．*x*，*y*はイベント生成時のカーソル座標を表します．例として，左ボタンのプッシュイベントを処理するには次のようにします．
```c++
// inside your class definition ...
virtual bool OnMouse(int button, int state, int x, int y){
    if(button & DVButtonMask::LBUTTON && state == DVButtonSt::DOWN){
        // do something here
    }
}
```
*OnDoubleClick*はマウスボタンのダブルクリックが生じたときに呼ばれます．引数の定義は*OnMouse*と同様です．*OnMouseMove*はマウスカーソルが移動するか，マウスホイールが回転した際に呼ばれます．*button*は直前のマウスプッシュイベントにおいて*OnMouse*に渡されたのと同じ値を持ちます．*x*, *y*は移動後のカーソル座標，*zdelta*はマウスカーソルの回転量です．*OnKey*はキーボードのキーがプッシュされるかリリースされた際に呼ばれます．*state*は*DVKeySt*列挙子の値を持ちます．*key*はプッシュあるいはリリースされたキーの仮想キーコードを保持します．以下に関連する列挙子の定義を示します．

|*DVButtonMask*									 |
|---|---|
|*LBUTTON*			| 左ボタン				|
|*RBUTTON*			| 右ボタン				|
|*MBUTTON*			| 中ボタン				|
|*SHIFT*				| Shiftキー押し下げ		|
|*CONTROL*			| Ctrlキー押し下げ		|
|*ALT*				| Altキー押し下げ		|


|*DVButtonSt*								 |
|---|---|
|*DOWN*		| ボタンプッシュ		|
|*UP*			| ボタンリリース		|


|*DVKeySt*								 |
|---|---|
|*PRESSED*	| 押されている			|
|*TOGGLE_ON*	| トグルされている		|

### APIとして提供される機能
以下に*DVKeyMouse*の関数を示します．

|*DVKeyMouseIf*																		 |
|---|---|
|*void*| *AddCallback(DVKeyMouseCallback*)* 	|
|*void*| *RemoveCallback(DVKeyMouseCallback*)* 	|
|*int*| *GetKeyState(int key)*					|
|*void*| *GetMousePosition(int& x, int& y, int& time, int count=0)*|
*AddCallback*はコールバッククラスを登録します．一つの仮想デバイスに対して複数個のコールバックを登録できます．*RemoveCallback*は登録済のコールバッククラスを解除します．*GetKeyState*は*DVKeyCode*で指定したキーの状態を*DVKeySt*の値で返します．*GetMousePosition*は*count*ステップ前のマウスカーソルの位置を取得するのに用います．ただし*count*は*0*以上*63*以下でなければなりません．*x*, *y*にカーソル座標が，*time*にタイムスタンプが格納されます．
### サポート状況に関する注意
使用する実デバイスによっては一部の機能が提供されないので注意して下さい．*OnMouseMove*においてマウスホイールの回転量を取得するには，実デバイスとして*DRKeyMouseWin32*を使用するか，freeglutとリンクしてビルドしたSpringhead上で*DRKeyMouseGLUT*を使用する必要があります．*OnKey*においてキーのトグル状態を取得するには実デバイスとして*DRKeyMouseWin32*を使用する必要があります．*GetKeyState*は*DRKeyMouseWin32*でのみサポートされます．*GetMousePosition*において，タイムスタンプを取得するには*DRKeyMouseWin32*を用いる必要があります．
## ジョイスティック
ジョイスティックの仮想デバイスは*DVJoyStick*です．実デバイスとしてはGLUTを用いる*DRJoyStickGLUT*のみがあります．T.B.D.
## トラックボール



トラックボールはキーボード・マウスにより並進・回転の6自由度を入力するヒューマンインタフェースです．トラックボールを使うことにより，カメラを注視点まわりに視点変更することができるようになります．トラックボールを操作する方法には，APIを直接呼び出す方法と，仮想マウスにコールバック登録する方法の二通りがあります．同様に，トラックボールの状態を取得する方法にもAPI呼び出しとコールバック登録の二通りがあります．仮想マウスとトラックボールおよびユーザプログラムの関係を\figurename\ref{fig_trackball}に示します．
### 回転中心と回転角度
カメラの位置と向きは，注視点，経度角，緯度角および注視点からの距離によって決まります．

|*HITrackballDesc*| | 				 |
|---|---|---|
|*Vec3f*|	*target*		| 回転中心		|
|*float*|	*longitude*	| 経度[rad]		|
|*float*|	*latitude*	| 緯度[rad]		|
|*float*|	*distance*	| 距離			|


|*HITrackballIf*| 									 |
|---|---|
|*Vec3f*| *GetTarget()*							|
|*void* | *SetTarget(Vec3f)*						|
|*void* | *GetAngle(float& lon, float& lat)*	|
|*void* | *SetAngle(float lon, float lat)*		|
|*float* | *GetDistance()*						|
|*void* | *SetDistance(float dist)*				|

### 範囲指定
以下の機能で角度および距離に範囲制限を加えられます．

|*HITrackballDesc*| | 					 |
|---|---|---|
|*Vec2f*|	*lonRange*	| 経度範囲			|
|*Vec2f*|	*latRange*	| 緯度範囲			|
|*Vec2f*|	*distRange*	| 距離範囲			|


|*HITrackballIf*| 									 |
|---|---|---|---|
|*void* | *GetLongitudeRange(float& rmin, float& rmax)*	|
|*void* | *SetLongitudeRange(float rmin, float rmax)*		|
|*void* | *GetLatitudeRange(float& rmin, float& rmax)*		|
|*void* | *SetLatitudeRange(float rmin, float rmax)*			|
|*void* | *GetDistanceRange(float& rmin, float& rmax)*		|
|*void* | *SetDistanceRange(float rmin, float rmax)*			|

### コールバック登録


|*HITrackballIf*| 								 |
|---|---|
|_DVKeyMouseIf*_ | *GetKeyMouse()*						|
|*void* 		| *SetKeyMouse(DVKeyMouseIf*)*			|
|*void* 		| *SetCallback(HITrackballCallback*)*	|
トラックボールをマウス操作するには*DVKeyMouse*クラスにコールバック登録する必要があります．コールバック登録するには*SetKeyMouse*，登録先の仮想マウスを取得するには*GetKeyMouse*を呼びます．また，ユーザプログラムがトラックボールにコールバック登録して状態変化に反応できるようにするには，*HITrackballCallback*クラスを継承し，*SetCallback*関数に渡します．*HITrackballCallback*は以下の単一の仮想関数を持ちます．

|*HITrackballCallback*| 					 |
|---|---|
|*virtual void* | *OnUpdatePose(HITrackballIf* tb)*	|
*OnUpdatePose*はトラックボールの位置・向きに変化が生じる度に呼ばれます．引数の*tb*は呼び出し元のトラックボールを示します．
### マウスボタン割当て
*HITrackball*は内部で*DVKeyMouseCallback*を継承します．*SetKeyMouse*により*DVKeyMouse*にコールバック登録すると，マウスカーソルが移動するたびに*OnMouseMove*イベントハンドラが呼び出され，トラックボールの内部状態が更新されます．マウス移動時のボタン状態に応じてトラックボールのどの状態が変化するかはある程度カスタマイズが可能です．以下に関連する機能を示します．

|*HITrackballDesc*| | 		 |
|---|---|---|
|*int*| *rotMask*	| 回転操作のボタン割当て		|
|*int*| *zoomMask*	| ズーム操作のボタン割当て		|
|*int*| *trnMask*	| 平行移動操作のボタン割当て	|


|*HITrackballIf*| 			 |
|---|---|
|*void* | *SetRotMask(int mask)*		|
|*void* | *SetZoomMask(int mask)*	|
|*void* | *SetTrnMask(int mask)*		|
*rotMask*, *zoomMask*, *trnMask*はそれぞれ回転操作，ズーム操作，平行移動操作に割り当てたいマウスボタンに対応する*OnMouseMove*の*button*引数の値を表します．以下に対応関係をまとめます．

|toprule|
|マウス移動方向	| *button*値	| 変化量		 |
|---|---|---|
|左右			| *rotMask*	| 経度			|
|上下			| *rotMask*	| 緯度			|
|上下			| *zoomMask*	| 距離			|
|左右			| *trnMask*	| 注視点x座標	|
|上下			| *trnMask*	| 注視点y座標	|
|bottomrule|
デフォルトのボタン割当ては以下の通りです．

|*rotMask*| *LBUTTON*					|
|---|---|
|*zoomMask*| *RBUTTON*					|
|*trnMask*| *LBUTTON* + *ALT*	|
したがって，左ボタンドラッグで回転操作，右ボタンドラッグでズーム操作，[ALT]キー+左ドラッグで平行移動となります．なお，現状ではマウスの移動方向との対応をカスタマイズすることはできません．また，マウスホイールの回転とトラックボールを連動させる機能も未実装です．
### マウス操作に対する極性と感度
マウス移動量と角度変化量，距離変化量との比例係数を下記の機能で設定できます．

|*HITrackballDesc*| | 							 |
|---|---|---|
|*float*|	*rotGain*	| 回転ゲイン[rad/pixel]		|
|*float*|	*zoomGain*	| ズームゲイン[rad/pixel]	|
|*float*|	*trnGain*	| 平行移動ゲイン			|


|*HITrackballIf*| 									 |
|---|---|
|*float* | *GetRotGain()*			|
|*void* | *SetRotGain(float g)*	|
|*float* | *GetZoomGain()*		|
|*void* | *SetZoomGain(float g)*	|
|*float* | *GetTrnGain()*			|
|*void* | *SetTrnGain(float g)*	|

### トラックボールで視点を動かす
トラックボールの位置と向きをカメラに反映するには，描画処理の冒頭で以下のようにします．
```c++
// given GRRenderIf* render
render->SetViewMatrix(trackball->GetAffine().inv());
```

## Spidar
Spidarはワイヤ駆動型の3軸・6軸力覚提示ヒューマンインタフェースです．T.B.D.
