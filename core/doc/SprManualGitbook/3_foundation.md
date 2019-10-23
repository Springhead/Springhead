FoundationモジュールはすべてのSpringheadクラスの基本クラスを定義します．普通に使っている限り，ユーザがFoundationの機能を直接利用することは少ないでしょう．
## 実行時型情報
（ほとんど）すべてのSpringheadオブジェクトは実行時型情報（RTTI）を持っています．C++にも*dynamic\_cast*などのRTTI機能がありますが，これよりも大幅にリッチな型情報が提供されます．実行時型情報のクラスは*IfInfo*です．*IfInfo*は次節で紹介する*Object*クラスから取得できます．
## オブジェクト





\includegraphics[width=.5\hsize]{fig/utclass.eps}

\caption{Object class hierarchy}


ほとんどすべてのSpringheadオブジェクトは*Object*クラスから派生します．オブジェクトは複数の子オブジェクトを持つことができます．Springheadのデータ構造はオブジェクトが成すツリー構造によって出来上がっています．Foundationモジュールにおける*Object*からのクラス階層をFig.\,\ref{fig_utclass}に示します．まず*Object*クラスの子オブジェクトの作成・管理に関係する関数を紹介します．\noindent\begin{tabular}{p{1.0\hsize}}\\*ObjectIf*										\\ \midrule*size\_t NChildObject()*							\\子オブジェクトの数を取得する．							\\														\\*ObjectIf* GetChildObject(size\_t pos)*			\\*pos*番目の子オブジェクトを取得する．			\\														\\*bool AddChildObject(ObjectIf* o)*				\\オブジェクト*o*を子オブジェクトとして追加する．正しく追加されたら*true*，それ以外は*false*を返す．\\														\\*bool DelChildObject(ObjectIf* o)*				\\オブジェクト*o*を子オブジェクトから削除する．正しく削除されたら*true*，それ以外は*false*を返す．\\														\\*void Clear();*									\\クリアする．												\\\\\end{tabular}これらの関数は派生クラスによって実装されますので，追加できる子オブジェクトの種類や数などはクラスごとに異なります．また，Springheadを普通に使用する範囲内ではユーザがこれらの関数を直接呼び出す場面はないでしょう．ストリーム出力のために以下の機能があります．\noindent\begin{tabular}{p{1.0\hsize}}\\*ObjectIf*										\\ \midrule*void Print(std::ostream\& os) const*			\\オブジェクトの内容をストリーム*os*に出力する．	\\\\\end{tabular}*Print*は，基本的にはそのオブジェクトの名前を出力し，子オブジェクトの*Print*を再帰的に呼び出します．ただし派生クラスによって*Print*で出力される内容がカスタマイズされている場合はその限りではありません．*NamedObject*は名前付きオブジェクトです．*NamedObject*の派生クラスには名前を文字列で与えることができ，名前からオブジェクトを検索することができます．名前付きオブジェクトには，直接の親オブジェクト以外に，名前を管理するためのネームマネジャが対応します．\noindent\begin{tabular}{p{1.0\hsize}}\\*NamedObjectIf*									\\ \midrule*const char* GetName()*			\\名前を取得する．						\\\\*void SetName(const char* n)*	\\名前を設定する．						\\\\*NameManagerIf* GetNameManager()*	\\ネームマネジャを取得する．					\\\\\end{tabular}名前付きオブジェクトからはさらにシーンオブジェクトが派生します．シーンオブジェクトからは周辺モジュールのオブジェクト(*PHSolid*, *GRVisual*など)が派生します．\noindent\begin{tabular}{p{1.0\hsize}}\\*SceneObjectIf*					\\ \midrule*SceneIf* GetScene()*			\\自身が所属するシーンを取得する．		\\\\\end{tabular}
## ネームマネジャとシーン
ネームマネジャは名前付きオブジェクトのコンテナとして働き，それらの名前を管理します．また，ネームマネジャはそれ自身名前付きオブジェクトです．\noindent\begin{tabular}{p{1.0\hsize}}\\*NameManagerIf*									\\ \midrule*NamedObjectIf* FindObject(UTString name)*		\\名前が*name*のオブジェクトを検索し，見つかればそのオブジェクトを返す．見つからなければ*NULL*を返す．					\\\\\end{tabular}シーンはシーンオブジェクトのコンテナです．シーンの基本クラスは*Scene*で，ここから各モジュールのシーン(*PHScene*, *GRScene*, *FWScene*など)が派生します．*Scene*クラスは特に機能を提供しません．
## タイマ
タイマ機能もFoundationで提供されます．タイマクラスは*UTTimer*です．タイマを作成するには
```
UTTimerIf* timer = UTTimerIf::Create();
```
とします．*UTTimer*には以下のAPIがあります．

\begin{tabular}{ll}
*[Get|Set]Resolution*		& 分解能の取得と設定	\\
*[Get|Set]Interval*			& 周期の取得と設定		\\
*[Get|Set]Mode*				& モードの取得と設定	\\
*[Get|Set]Callback*			& コールバック関数の取得と設定 \\
*IsStarted*					& 動いているかどうか	\\
*IsRunning*					& コールバック呼び出し中 \\
*Start*						& 始動	\\
*Stop*						& 停止	\\
*Call*						& コールバック呼び出し
\end{tabular}

*SetMode*で指定できるモードには以下があります．

\begin{tabular}{ll}
*MULTIEDIA*		& マルチメディアタイマ			\\
*THREAD*		& 独立スレッド					\\
*FRAMEWORK*		& Frameworkが提供するタイマ		\\
*IDLE*			& Frameworkが提供するアイドルコールバック
\end{tabular}

マルチメディアタイマはWindowsが提供する高機能タイマです．独立スレッドモードでは，タイマ用のスレッドが実行され*Sleep*関数により周期が制御されます．*FRAMEWORK*と*IDLE*モードを利用するには*FWApp*の*CreateTimer*関数を用いる必要があります．基本的に*FRAMEWORK*モードではGLUTのタイマコールバックが使われ，*IDLE*モードではGLUTのアイドルコールバックが使われます．Frameworkモジュールの*FWApp*を利用する場合は，*FWApp*の*CreateTimer*関数を利用する方が便利でしょう．
## 状態の保存・再現
シミュレーションを行うと、シーンを構成するオブジェクトの状態が変化する。ある時刻での状態を保存しておき、再現することができると、数ステップ前に戻ったり、あるステップのシミュレーションを、力を加えた場合と加えない場合で比べたりといった作業ができる。Springheadでは、*ObjectStatesIf*を用いることで、以下のようにシーン全体の状態をまとめてメモリ上に保存、再現することができる。
```
	PHSceneIf* phScene;
	省略：phScene（物理シミュレーションのシーン）の構築
	UTRef<ObjectStatesIf> states;
	states = ObjectStatesIf::Create();	// ObjectStatesオブジェクトの作成
	states->AllocateState(phScene);		// 保存用のメモリ確保
	states->SaveState(phScene);			// 状態の保存
	phScene->Step();					// 仮のシミュレーションを進める
	省略：加速度の取得など
	states->LoadState(phScene);			// 状態の再現
	states->ReleaseState();				// メモリの開放
	省略：力を加えるなどの処理
	phScene->Step();					// 本番のシミュレーションを進める
```

### 保存・再現のタイミング
Springheadのシーン(PHSceneやCRScene)は、複数のエンジン(PHEngineやCREngineの派生クラス)を呼び出すことで、シミュレーションを進める。シーンは、エンジンの呼び出し中以外のタイミングであればいつでも状態を保存・再現することができる。
### シーン構成変更の制約
状態保存用のメモリは、シーンの構成に依存している。*AllocateState(), SaveState(), LoadState()*だけでなく、*ObjectStatesIf::ReleaseState()*も依存するので、*ObjectIf::AddChildObject()*などのAPIによってシーンの構成を変化させてしまうと、保存・再現だけでなくメモリの開放もできなくなる。変更前に開放するか、シーン構成を戻してから開放する必要がある。
