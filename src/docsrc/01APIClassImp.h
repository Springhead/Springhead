/**
\page pageAPIClassImp APIクラスの作り方・実装の仕方

\section secCreateAPIClass APIクラスの作り方
ここでは，SpringheadのAPIクラスの宣言と実装の手順について説明します．
APIクラスの概要，使い方については，\ref pageApiBasic をご参照ください．

\subsection secDefAPIClass APIクラスの宣言
APIクラスのヘッダファイルは，Springhead2/include/SDKNAME に作ります．
(SDKNAMEはSDKの名前 Graphics，Physicsなど)<br>
ヘッダファイルの名前は，Sprオブジェクト名.h (SprPHSolid.h など)とします．
APIクラスを宣言するには，
<pre>
 struct SceneObjectIf: NamedObjectIf{
 	IF_DEF(SceneObject);
 	///	所属Sceneの取得
 	virtual SceneIf* GetScene() =0;
 };
</pre>
のように，
<ol>
 <li> 基本のインタフェースクラスを継承する．
 <li> IFDEF(クラス名)メンバを持つ．
 <li> API関数を純粋仮想関数として宣言する．
</ol>
をします．

\subsection secTypeInfoOfAPIClass APIクラスの型情報
APIクラスを宣言したら，ライブラリのソースファイル
Springhead2/src/SDKNAME/オブジェクト名.cpp (PHSolid.cpp など)に
<pre>
 IF_IMP(SceneObject, NamedObject)
</pre>
を定義します．
このマクロは，クラスの継承関係をグローバル変数として保存します．
キャストのマクロ DCAST はこの情報を使ってキャストします．


\section secCreateImplementClass 実装クラスの作り方
APIクラスを作ったら，それを実装するクラスを作ります．

\subsection defImplementClass 実装クラスの宣言
実装クラスは，~
<pre>
 class SceneObject:public NamedObject, SceneObjectIfInit{
 	OBJECT_DEF(SceneObject);		///<	クラス名の取得などの基本機能の実装
 public:
 	virtual void SetScene(SceneIf* s);
 	virtual SceneIf* GetScene();
 	///	デバッグ用の表示
 	virtual void Print(std::ostream& os) const {NamedObject::Print(os);}
 };
</pre>
のように，宣言します．

\subsubsection secOBJECT_DEF OBJECT_DEFマクロ
クラスの宣言のなかのOBJECT_DEFは，
<ul>
 <li> 型情報のためのStatic変数(UTTypeInfo* typeInfo, GetTypeInfo(), GetTypeInfoStatic())
 <li> APIクラスを取得するメソッド GetIf()
</ul>
を宣言します．
抽象クラスの場合は，実体化できないというエラーがでるので，代わりにOBJECT_DEF_ABSTを使います．

\subsubsection secGetAPI APIの取得
Objectは，APIクラスの領域(ObjectIfBuf部)を持っています．
（APIクラスは，仮想関数テーブルへのポインタ(vftableポインタ)だけを持ちます．
MSVC++7.1の場合，Objectの先頭から４バイト目からの４バイトになります．）

Object::GetIf() は this+4バイトをObjectIf*にキャストして返します．
Objectの中にあるAPIクラスのアドレスを返すわけです．
逆にObjectIf::GetObj()は，this-4バイトを返します．
ですので，APIクラスは必ず実装クラスの中になければなりません．

派生クラス，たとえばPHSolid::GetIf() も同様ですが，PHSolidIf*にキャストして返します．

\subsubsection secInitAPI APIと初期化
実装クラスは，
<pre>
 class SceneObject:public NamedObject, SceneObjectIfInit{...}
</pre>
のように，基本実装クラスを継承しますが，APIクラス(SceneObjectIf等)
は継承しません．代わりに，インタフェース初期化クラス(SceneObjectIfInit等)を
継承します．
SceneObjectIfInitは，コンストラクタでObjectのObjectIfBuf部を，正しいAPIクラスに初期化します．

\subsubsection secConnectAPI APIと実装の接続．
SceneObjectIfInitは，ObjectIfBuf部を初期化する際に，APIクラス(SceneObjectIf)に
初期化するのではなく，スタブクラス(SceneObjectIfStub)に初期化します．
初期化クラスとスタブクラスは，src/ライブラリ名/IfStubDumpライブラリ名.h にあります．
スタブクラスは，各API関数の呼び出しを実装関数の呼び出しにつなげています．
スタブクラスと初期化クラスのソースコードは，APIクラスのソースコードから自動生成されます．

\section secImpImplementationClass 実装クラスの定義
\subsection secOBJECT_IMP OBJECT_IMPマクロ
OBJECT_DEFで定義したメソッドや変数の実装は，マクロ OBJECT_IMP で
<pre>
 OBJECT_IMP(SceneObject, NamedObject);
</pre>
のように行います．抽象クラスの場合は，OBJECT_IMP_ABST を使います．

APIクラスの定義に必要な IF_IMP と OBEJCT_IMPは，両方をあわせたマクロ
IF_OBJECT_IMP，IF_OBJECT_IMP_ABSTを使うと1回で書けます．

\subsection secImpAPIClass 宣言したAPIの実装
<pre>
 void SceneObject::SetScene(SceneIf* s){
 	SetNameManager(s->GetObj<NameManager>());
 	nameManager->GetNameMap();
 }
</pre>
のように，宣言したAPIを実装します．関数名のほか，型名などが違っても，
オーバーライドできないので，まったく同じ宣言になるように気をつけてください．

\section secFileLoadSave ファイルからのロード・ファイルへのセーブ
FileIO SDK(\ref pageFileIO) でロード・セーブができるようにするためには，APIクラスの定義に
若干の細工をする必要があります．
FileIO SDK の詳細は，\ref pageFileIOImp を参照してください

自動ロード・セーブ可能なオブジェクトを定義するには，
<ol>
 <li>ロードしたいデータを含んだディスクリプタ (例:PHSolidDesc)を作ります．
 <li>インタフェースクラス (例：PHSolidIf) を作ります．
 <li>実装クラス (例：PHSolid)を作ります．このとき実装クラスで，
  <pre>
  	///	デスクリプタの読み出し(コピー版)
  	virtual bool GetDesc(void* desc) const { return false; }
  	///	デスクリプタの読み出し(参照版)
  	virtual const void* GetDescAddress() const { return NULL; }
  	///	デスクリプタのサイズ
  	virtual size_t GetDescSize() const { return 0; };
  </pre>
  をオーバーロードしてください．ディスクリプタを継承しているならば，
  LoadableがLoadableDescを継承しているなら，
  <pre>
	ACCESS_DESC(実装クラス名);
  </pre>
  マクロを実装クラスの宣言の中に置けば，オーバーライドしてくれます．
 <li> シーングラフ上で，実装クラス(例：PHSolid)の先祖になるクラスのなかから，
      実装クラス(例：PHSolid)を生成するクラス(例：PHScene)を決めます．
 <li> 生成クラス(例：PHScene)のCreateObject()が実装クラス(PHSolid)を作れるように，
      <pre>
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHSolid));
	  </pre>
      のようにファクトリを登録します．
      登録は，ファイルのロードより前に行わなければなりません．
      SDKのコンストラクタで1度だけ呼び出すのが良いでしょう．
      詳しくは \ref secFactory を参照ください．
</ol>

\section secStateLoadSave 状態の保存・再現
ファイルへのロードセーブでは，何も無い状態からオブジェクトを生成してシーングラフを作りますが，
シミュレーション中に，10ステップ前の状態に戻したいなどということも良くあるでしょう．
そんな用途に使うのが状態の保存・再現です．

状態の保存・再現ができるクラスを作るには，
<ol>
 <li> 保存したい状態を保持する構造体(例：PHSolidState)を作ります．
 <li> 実装クラス (例：PHSolid を作ります．
	このとき実装クラスで，
	<pre>
	///	状態の読み出し(コピー版)
	virtual bool GetState(void* state) const { return false; }
	///	状態の読み出し(参照版)
	virtual const void* GetStateAddress() const { return NULL; }
	///	状態の設定
	virtual void SetState(const void* state){}
	///	状態のサイズ
	virtual size_t GetStateSize() const { return 0; };
	///	メモリブロックを状態型に初期化
	virtual void ConstructState(void* m) const {}
	///	状態型をメモリブロックに戻す
	virtual void DestructState(void* m) const {}
	</pre>
	をオーバーロードしてください．<br>
	実装クラス(例：PHSolid)が状態構造体(例：PHSolidState)を継承しているなら，
	<pre>
	ACCESS_STATE(実装クラス名);
	</pre>
	マクロを実装クラスの宣言の中に置けば，オーバーライドしてくれます．
	ACCESS_DESC_STATE() マクロを置けば，DescとState両方をオーバーライドしてくれます．
*/
