# EmbPython : Pythonとの連携機能

EmbPythonモジュールは，スクリプト言語Pythonとの連携機能を提供します．PythonインタプリタからSpringheadの機能を呼び出したり，SpringheadアプリケーションにPythonインタプリタを組み込んでスクリプティングエンジンとして使用するといった事ができます．EmbPythonモジュールの使用により，Pythonインタプリタ上にSpringhead APIクラスへのインタフェースクラスが提供されます．ユーザはPythonインタフェースクラスを使用してSpringheadの各機能にアクセスします．Pythonインタフェースクラスは内部的にSpringheadの機能を呼び出し，結果をPythonインタフェースクラスに変換して返します．





## 利用法
大きく分けて二通りの利用法を想定しています．

一つは，C++で実装されたSpringheadアプリケーションに対し，Pythonインタプリタを組み込むことです．Springheadアプリケーションの機能の一部をPythonスクリプト記述し，拡張性を高めます．

もう一つは，Pythonインタプリタに対する外部拡張モジュール(Python DLL, pyd)として提供されたSpringheadを利用することで，PythonアプリケーションにSpringheadの機能を組み込む利用法です．

どちらの場合においても，EmbPythonモジュールはPython側からSpringheadの関数を呼び出すためのインタフェースを提供します．

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/epoverview.svg) 



### 環境変数PATHの設定
Springheadの動作は、`Springhead2\core\bin\win64`フォルダ、および`Springhead2\dependency\bin\win64`フォルダ内のdll群に依存しています。これらのフォルダの絶対パスを環境変数PATHに追加してください。
### SpringheadへのPython組込み
SpringheadアプリケーションにPythonインタプリタを組み込んで利用する方法を解説します．本節ではまずSpringheadに同梱されたPythonインタプリタ組み込みサンプルを紹介し，簡単な使い方を説明します．その後，サンプルにおけるPythonインタプリタ組み込みのためのソースコードについて解説します．
#### PythonSprサンプルのビルドと実行
Pythonインタプリタ組み込みサンプルは `src\Samples\EmbPython\PythonSpr` にあります．ビルドすると `PythonSpr.exe` ができます．*PythonSpr*サンプルは標準的なSpringheadサンプルアプリケーションフレームワークにPythonインタプリタを組み込んだもので，物理シーンを構築・シミュレーション・描画する事ができます．Pythonインタプリタからは*phSdk*や*fwSdk*にアクセスすることができ，表示機能を切り替えたりシーンにオブジェクトを作成したりといったことがPythonから行えます．実行の前に，環境変数を設定します．これは，Springheadアプリケーションに組み込まれたPythonインタプリタがPythonの標準ライブラリ群にアクセスするために必要です．\begin{description}- [*SPRPYTHONPATH*環境変数]~Springheadリリースを展開したフォルダ内の`bin\src\Python32\Lib`へのフルパスを指定します．Python3.2を`c:\Python32`にインストールしてある場合，`C:\Python32\Lib`でもかまいません．\end{description}`PythonSpr.exe`を実行すると次のような画面が現れます．＜スクリーンショット＞右がSpringheadの実行画面，左のコンソールがPythonプロンプトです．起動時には，Springhead実行画面には何のシーンも構築されていないため，ワールド座標系を示す矢印のみが描画されています．操作法は以下の通りです．\begin{description}- [マウス 左ドラッグ] 視点変更（回転）- [マウス 右ドラッグ] 視点変更（拡大縮小）- [スペースキー] シミュレーション開始・一時停止（起動直後は停止しています）\end{description}
#### PythonSprサンプルの遊び方
この節では，Pythonコードを中心としてSpringheadの機能を利用する具体的な方法を紹介します．PythonからのSpringhead API利用に関する詳しい仕様は[PythonからのSpringhead API使用法](#PythonからのSpringhead API使用法)を参照してください．Pythonプロンプト上にSpringheadのコードを入力して実行することができます．以下のように入力してシミュレーションを開始（スペースキー）すると，剛体が作成されて落ちていきます．
```python
# 剛体が落ちるだけのサンプル

>>> fwScene   ← 初期状態で定義されている変数で，アプリケーションが保持するfwSceneにアクセスできます
<Framework.FWScene object at 0x05250A40>
>>> phScene = fwScene.GetPHScene()
>>> desc = Spr.PHSolidDesc()
>>> desc.mass = 2.0
>>> solid0 = phScene.CreateSolid(desc)
```
形状を与えることもできます．なお，最後の行の*solid0.AddShape(box0)*を実行するまで剛体に形状は割り当てられないので，この行を入力し終わるまではスペースキーを押さずにシミュレーションを一時停止状態にしておくとよいでしょう．
```python
# 形状のある剛体が落ちるだけのサンプル

>>> phScene = fwScene.GetPHScene()
>>> phSdk   = phScene.GetSdk()
>>> descSolid = Spr.PHSolidDesc()
>>> solid0 = phScene.CreateSolid(descSolid)
>>> descBox = Spr.CDBoxDesc()
>>> descBox.boxsize = Spr.Vec3f(1,2,3)
>>> box0 = phSdk.CreateShape(Spr.CDBox.GetIfInfoStatic(), descBox)
>>> solid0.AddShape(box0)
```
床（位置が固定された剛体）を作成すると，さらにそれらしくなります．
```python
>>> phScene = fwScene.GetPHScene()
>>> phSdk   = phScene.GetSdk()

# 床をつくる
>>> descSolid = Spr.PHSolidDesc()
>>> solid0 = phScene.CreateSolid(descSolid)
>>> descBox = Spr.CDBoxDesc()
>>> descBox.boxsize = Spr.Vec3f(10,2,10)
>>> boxifinfo = Spr.CDBox.GetIfInfoStatic()
>>> solid0.AddShape(phSdk.CreateShape(boxifinfo, descBox))
>>> solid0.SetFramePosition(Spr.Vec3d(0,-1,0))
>>> solid0.SetDynamical(False)

# 床の上に箱をつくって載せる
>>> solid1 = phScene.CreateSolid(descSolid)
>>> descBox.boxsize = Spr.Vec3f(1,1,1)
>>> boxifinfo = Spr.CDBox.GetIfInfoStatic()
>>> solid1.AddShape(phSdk.CreateShape(boxifinfo, descBox))
```
力を加えることもできます．
```python
>>> solid1.AddForce(Spr.Vec3d(0,200,0))
```
PythonのForやWhileを使って継続して力を加えることもできます．
```python
>>> import time
>>> for i in range(0,100):
>>>     solid1.AddForce(Spr.Vec3d(0,20,0))
>>>     time.sleep(0.01)
```
応用として，簡単な制御ループを走らせることもできます．
```python
>>> import time
>>> for i in range(0,500):
>>>   y  = solid1.GetPose().getPos().y
>>>   dy = solid1.GetVelocity().y
>>>   kp = 20.0
>>>   kd =  3.0
>>>   solid1.AddForce(Spr.Vec3d(0, (2.0 - y)*kp - dy*kd, 0))
>>>   time.sleep(0.01)
```
ここまでは剛体のみでしたが，関節も作成できます．
```python
>>> phScene = fwScene.GetPHScene()
>>> phSdk   = phScene.GetSdk()

>>> descSolid = Spr.PHSolidDesc()
>>> solid0 = phScene.CreateSolid(descSolid)
>>> descBox = Spr.CDBoxDesc()
>>> descBox.boxsize = Spr.Vec3f(1,1,1)
>>> boxifinfo = Spr.CDBox.GetIfInfoStatic()
>>> solid0.AddShape(phSdk.CreateShape(boxifinfo, descBox))
>>> solid0.SetDynamical(False)

>>> solid1 = phScene.CreateSolid(descSolid)
>>> solid1.AddShape(phSdk.CreateShape(boxifinfo, descBox))

>>> descJoint = Spr.PHHingeJointDesc()
>>> descJoint.poseSocket = Spr.Posed(1,0,0,0, 0,-1,0)
>>> descJoint.posePlug   = Spr.Posed(1,0,0,0, 0, 1,0)
>>> hingeifinfo = Spr.PHHingeJoint.GetIfInfoStatic()
>>> joint = phScene.CreateJoint(solid0, solid1, hingeifinfo, descJoint)
```
PythonSpr.exeに引数を与えると，pythonファイルを読み込んで実行することもできます．ここまでに書いた内容を *test.py* というファイルに書いて保存し，コマンドプロンプトから以下のように実行すると，test.pyに書いた内容が実行されます（スペースキーを押すまでシミュレーションは開始されないことに注意してください）．
```python
C:\src\Samples\EmbPython\PythonSpr> Release\PythonSpr.exe test.py
>>>
```

#### Pythonインタプリタ組み込みのためのコード例
PythonSprサンプルにおいて，Pythonインタプリタを組み込むためのコードについて紹介します．\begin{tips}Pythonインタプリタ組み込みの詳細を理解するためにはSpringheadだけでなくPythonのC言語APIについて知る必要があります．詳しく知りたい方はPython/C APIリファレンスマニュアル$^{*1}$等も参照してください．{\footnotesize *1 ... `http://docs.python.org/py3k/c-api/index.html`}\end{tips}PythonSprサンプルにおいて，Python組み込みのためのコードは *main.cpp* に記述されています．関連箇所を抜粋して紹介します．Python組み込み関連の機能を使用するには，*EmbPython.h* ヘッダをインクルードします．
```python
#include <EmbPython/EmbPython.h>
```
Pythonインタプリタは，Springheadアプリケーション本体とは異なるスレッドで動作します．物理シミュレーションステップの実行中や描画の最中にPythonがデータを書き換えてしまうことがないよう，排他ロックをかけて保護します．
```python
virtual void OnStep(){
  UTAutoLock critical(EPCriticalSection);
  ...
}
virtual void OnDraw(GRRenderIf* render) {
  UTAutoLock critical(EPCriticalSection);
  ...
}
virtual void OnAction(int menu, int id){
  UTAutoLock critical(EPCriticalSection);
  ...
}
```
*EPCriticalSection*はアプリケーションに一つしか存在しないインスタンスで，*EPCriticalSection*による排他ロックを取得できるのは全アプリケーション中で一つのスコープのみです．PythonからSpringheadの機能が呼び出される際には必ず*EPCriticalSection*の取得を待つようになっているので，排他ロックを取得した*OnStep*の実行中にPythonがSpringheadの機能を実行することはありません\footnote{ナイーブな実装のため少々過剰なロックとなっています．実際の競合リソースに根ざした排他制御ができるよう，将来のバージョンで変更がなされる可能性もあります．}．次に，Pythonインタプリタ初期化用の関数を定義します．
```python
void EPLoopInit(void* arg) {
  PythonSprApp* app = (PythonSprApp*)arg;

  // Pythonでモジュールの使用宣言
  PyRun_SimpleString("import Spr");
        
  // PythonからCの変数にアクセス可能にする準備
  PyObject *m = PyImport_AddModule("__main__");
  PyObject *dict = PyModule_GetDict(m);

  // PythonからfwSceneにアクセス可能にする
  PyObject* pyObj = (PyObject*)newEPFWSceneIf(app->fwScene);
  Py_INCREF(pyObj);
  PyDict_SetItemString(dict, "fwScene", pyObj);

  // Pythonファイルをロードして実行する
  if (app->argc == 2) {
    ostringstream loadfile;
    loadfile << "__mainfilename__ ='";
    loadfile << app->argv[1];
    loadfile << "'";
    PyRun_SimpleString("import codecs");
    PyRun_SimpleString(loadfile.str().c_str());
    PyRun_SimpleString(
      "__mainfile__ = codecs.open(__mainfilename__,'r','utf-8')");
    PyRun_SimpleString(
      "exec(compile( __mainfile__.read() , __mainfilename__, 'exec')"
      ",globals()"
      ",locals())" );
    PyRun_SimpleString("__mainfile__.close()");
  }
}
```
この関数は関数ポインタの形でインタプリタオブジェクトに渡され，実行開始時にコールバックされます．中身はPython上でSpringheadを使用可能にするための手続きと，C上の変数をブリッジするためのコード，そして起動時に指定された.pyファイルをロードするコードなどです．上の例では*app->fwScene*のみをPythonに渡していますが，他にも受け渡したい変数が複数出てきた場合は，以下のようなマクロが便利でしょう．
```python
#define ACCESS_SPR_FROM_PY(cls, name, obj)           \
{                                                    \
    PyObject* pyObj = (PyObject*)newEP##cls((obj));  \
    Py_INCREF(pyObj);                                \
    PyDict_SetItemString(dict, #name, pyObj);        \
}                                                    \

// 使い方:
// ACCESS_SPR_FROM_PY(型名, Python側での変数名, アクセスする変数)
ACCESS_SPR_FROM_PY(FWSceneIf, fwScene, app->fwScene);
```
実際のPythonSprサンプルでは，このマクロを用いていくつかの変数をPythonから呼び出せるようにしています．ループ関数も定義します．これについては変更することは稀でしょう．
```python
void EPLoop(void* arg) {
	PyRun_InteractiveLoop(stdin,"SpringheadPython Console");
}
```
最後に，*main*関数内でPythonインタプリタクラスである*EPInterpreter*を作成してコールバックを設定し，初期化・実行を行います．
```python
int main(int argc, char *argv[]) {
  app.Init(argc, argv);

  EPInterpreter* interpreter = EPInterpreter::Create();
  interpreter->Initialize();
  interpreter->EPLoopInit = EPLoopInit;
  interpreter->EPLoop = EPLoop;
  interpreter->Run(&app);

  app.StartMainLoop();
  return 0;
}
```





### PythonへのSpringhead組込み

PythonのDLLインポート機能を利用してSpringheadをPythonにロードして用いることができます．

Springheadの機能は*Spr.pyd*というDLLファイルにまとめられています．*Spr.pyd*は，`bin\win32\Spr.pyd`または`bin\win64\Spr.pyd`としてSpringheadリリースに含まれていますが，`src\EmbPython\SprPythonDLL.sln`をビルドして生成することもできます．

#### *Spr.pyd*の使い方
*Spr.pyd* は，Pythonのインストールフォルダ内にある*DLLs*フォルダにコピーして用います．importでロードします．

```python
Python 3.2.2 [MSC v.1500 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>> import Spr
```
Springheadアプリケーションに組み込む場合と違い，ロード時点では何のオブジェクトも生成されていません．まず*PHSdk*を生成し，次に*PHScene*を生成することで，*PHSolid*が生成できるようになります．
```python
>>> phSdk = Spr.PHSdk.CreateSdk()
>>> phScene = phSdk.CreateScene(Spr.PHSceneDesc())
>>> solid0 = phScene.CreateSolid(Spr.PHSolidDesc())
>>> for i in range(0,10):
...     print(solid0.GetPose().getPos())
...     phScene.Step()
... 
Vec3d(0.000,0.000,0.000)
Vec3d(0.000,-0.000,0.000)
Vec3d(0.000,-0.001,0.000)
...(中略)...
Vec3d(0.000,-0.011,0.000)
>>>
```
APIの呼び出し方はSpringheadアプリケーション組み込みの場合と変わりません．

#### Pythonから実行する場合のグラフィックス表示

以下のようなコードを書くことで、OpenGLのウィンドウを表示し実行中の物理シミュレーションをグラフィカルに表示することができます。 このサンプルでは、app.InitInNewThread()が、GLUTを動かすために、別のスレッドを作ってglutMainLoop()を呼び出しています。 

```python
import time
from Spr import *
app = EPApp()
app.InitInNewThread()
time.sleep(1)
fwSdk =  FWSdk()
fwSdk = app.GetSdk()
phSdk = PHSdk()
phSdk = fwSdk.GetPHSdk()
fwScene = fwSdk.GetScene()
phScene = PHScene()
phScene = fwScene.GetPHScene()
spd=CDSphereDesc()
sp=CDSphere()
sp=phSdk.CreateShape(sp.GetIfInfoStatic(), spd)
s1 = PHSolid()
s1 = phScene.CreateSolid()
s1.AddShape(sp)
s2 = PHSolid()
s2 = phScene.CreateSolid()
s2.AddShape(sp)
s2.SetFramePosition(Vec3d(0.1,0,0))
fwScene.EnableRenderGrid(False, True, False)

win = FWWin()
win = app.GetCurrentWin()
win.GetTrackball().SetPosition(Vec3f(0,10,10))
win.GetTrackball().SetTarget(Vec3f(0,0,0))

for i in range(100):
    phScene.Step()
    app.PostRedisplay()
    time.sleep(0.03)
```



#### SprBlender

*Spr.pyd*の応用例の一つにSprBlenderがあります．

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/epsprblender.svg) 

SprBlenderは，3DCGソフトBlenderにロードすることでSpringheadを使用可能にする拡張機能で，Springhead開発チームによって開発されました（**注：2019年現在、開発は停止しています**）．BlenderはUI機能の大半がPythonで記述されており，公開されたPython APIを通じて各種の機能を利用することができます．そこで，Blender上のPythonで*Spr.pyd*をロードし，Blender上のCGオブジェクトをSpringheadでシミュレーションできるように書かれたPythonスクリプトがSprBlenderです．詳しくは[Webサイト](http://springhead.info/wiki/SprBlender)を参照してください．





## PythonからのSpringhead API使用法

PythonからSpringhead APIを呼び出す際の詳細な方法といくつかの注意点について解説します．
#### Sprモジュールについて
Springheadの全クラスは*Spr*モジュールにパッケージされています．
```python
import Spr
```
を行うことで使用可能となります（Springheadアプリケーションに組み込む場合は*EPLoopInit*の中でインポートを実行します）．

Springheadに関連するクラスは全てSprモジュールの直下に定義されます．Springheadのインタフェースクラスはクラス名からIfを取ったもの(HogeHogeIf\* → HogeHoge)，ベクトルやクォータニオン等はそのままのクラス名で定義されています．現時点では，すべてのSpringheadクラスがPythonからの利用に対応しているわけではありません．Pythonから利用できるSpringheadクラスは，*dir*関数で確認できます．

```python
>>> import Spr
>>> dir(Spr)
```

#### オブジェクトの生成
C++でSpringheadを利用する場合と同様，まずはSdkを作成する必要があります．Sdkを作成するには，PHSdkクラスのインスタンスから*CreateSdk*を呼び出す必要があります．
```python
phSdk = Spr.PHSdk().CreateSdk()
grSdk = Spr.GRSdk().CreateSdk()
# ... etc.
```
シーンのCreateはSpringhead同様sdkのインスタンスから行います．
```python
phScene = phSdk.CreateScene(Spr.PHSceneDesc())
grScene = grSdk.CreateScene(Spr.GRSceneDesc())
# ... etc.
```

#### IfInfo，自動ダウンキャスト
オブジェクトをCreateするAPIの中には，引き渡すディスクリプタの型によって生成するオブジェクトの種類を判別するものがあります．例えば*PHScene::CreateJoint*は，*PHHingeJointDesc*を渡すとヒンジジョイントを生成し，*PHBallJointDesc*を渡すとボールジョイントを生成します．これらのCreate関数をPythonから利用する場合，ディスクリプタの型を判別する機能は現時点では用意されていないため，生成したいオブジェクトの型に対応するIfInfoオブジェクトを同時に引数に渡します．
```python
# Hinge
phScene.CreateJoint(so1,so2, Spr.PHHingeJoint.GetIfInfoStatic(), desc)

# Ball
phScene.CreateJoint(so1,so2, Spr.PHBallJoint.GetIfInfoStatic(),  desc)
```
IfInfoオブジェクトは*クラス名.GetIfInfoStatic()*で取得することができます．より正確には，ディスクリプタ型によって返すオブジェクトを変えるようなCreate関数は，以下のようにAPIヘッダファイルにおいてテンプレートを用いて記述されています．Python APIでは，非テンプレート版のCreate関数のみがポートされているため，*IfInfo* ii* に相当する引数が必要になります．
```python
// in SprPHScene.h
PHJointIf* CreateJoint(PHSolidIf* lhs, PHSolidIf* rhs,
  const IfInfo* ii, const PHJointDesc& desc);

template <class T> PHJointIf* CreateJoint
(PHSolidIf* lhs, PHSolidIf* rhs, const T& desc){
  return CreateJoint(lhs, rhs, T::GetIfInfo(), desc);
}
```
なお，複数種類のクラスのオブジェクトを返しうるAPI関数の場合，C++では共通するスーパークラス(関節なら*PHJointIf*など)が返るため自分で*DCAST*等を用いてダウンキャストする必要がありますが，Pythonにおいてははじめから個々のクラス(*PHHingeJoint*, *PHBallJoint*など)の型情報を持つように自動的にダウンキャストされたものが返されます．よって，ユーザが意識してダウンキャストする必要はありません．
#### enumの扱い
*PHSceneIf::SetContactMode*のように，enum型を引数にとる関数があります．残念ながら，現時点ではenumの定義はPythonへポートされていません．これらの関数を呼び出す場合は，対応する整数値を渡してください．
```python
# C++での phScene->SetContactMode(so1, so2, PHSceneDesc::MODE_NONE) と同じ
phScene.SetContactMode(so1, so2, 0)
```

#### ベクトル，ポーズ
*Vec3d*，*Quaterniond*，*Posed*等はSpringheadと同じクラス名で使用できます．各要素は *.x* *.y* 等のプロパティによりアクセスでき，値の変更も可能です．
```python
>>> v = Spr.Vec3d(1,2,3)
>>> v
(1.000,2.000,3.000)
>>> v.x
1.0
>>> v.x = 4.0
>>> v
(4.000,2.000,3.000)
```
*Posed*, *Posef*については，*w, x, y, z*プロパティがクォータニオン成分，*px, py, pz*プロパティがベクトル成分へのアクセスとなります．また，*Posed::Pos()*, *Posed::Ori()*に対応する関数として

-  *.getOri()*
-  *.setOri()*
-  *.getPos()*
-  *.setPos()*

が用意されています．
