\page howtoCreateAPIClass APIクラスの作り方・実装の仕方

\section secCreateAPIClass APIクラスの作り方

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
同じGet関数の const 版と non const 版を作るときなど実装を書きたい場合は，
ヘッダに書くか， Springhead2/src/SDKNAME/オブジェクト名.cpp (PHSolid.cpp など) に書きます．

\subsection secTypeInfoOfAPIClass{APIクラスの型情報}
APIクラスを宣言したら，ライブラリのソースファイル
Springhead2/src/SDKNAME/オブジェクト名.cpp (PHSolid.cpp など)に
<pre>
 IF_IMP(SceneObject, NamedObject)
</pre>
を定義します．
このマクロは，クラスの継承関係をグローバル変数として保存します．
キャストのマクロ DCAST はこの情報を使ってキャストします．


\section secCreateImplementClass 実装クラスの作り方
APIクラスを作ったら，それを継承した実装クラスを作ります．

\subsection defImplementClass 実装クラスの宣言
実装クラスは，~
<pre>
 class SceneObject:public InheritNamedObject<SceneObjectIf, NamedObject>{
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
抽象クラスの場合は，実体化できないというエラーがでるので，
代わりにOBJECT_DEF_ABSTを使います．

\subsubsection secAutoOverride 先祖APIクラスの仮想関数の自動オーバーライド
実装クラスは，
<pre>
 class SceneObject:public InheritNamedObject<SceneObjectIf, NamedObject>{...}
</pre>
のように，APIクラス以外のクラス(この場合NamedObjectクラス)を継承している．
<pre>
 class NamedObject:public NamedObjectIf, public Object{...}
</pre>
のように，そのクラス(NamedObject)クラスが別のAPIクラス(NamedObjectIf)を
継承している場合，別のAPIクラス(NamedObjectIf)の仮想関数(API)を
オーバーライドしておかないと，コンパイルエラーになる．
継承するたびに，すべての関数の実装を書くのは面倒なので，
よく継承されそうなクラスを宣言したときは，
<pre>
 tempalte<class intf, class base>
 struct InheritNamedObject: public InheritObject<intf,base>{
 	const char* GetName() const { return base::GetName(); }
 	void SetName(const char* n) { base::SetName(n); }
 };
</pre>
のように，継承して実装するテンプレートを用意しておき，派生クラスは，
<pre>
 class SceneObject: public InheritNamedObject<SceneIf, NamedObject>{...};
</pre>
のようにこのクラスを継承します．

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
 	SetNameManager(OCAST(NameManager, s));
 	nameManager->GetNameMap();
 }
</pre>
のように，宣言したAPIを実装します．関数名のほか，型名などが違っても，
オーバーライドできないので，まったく同じ宣言になるように気をつけてください．

\section secFileLoadSave ファイルからのロード・ファイルへのセーブ
FileIO SDK(\ref pageFileIO) でロード・セーブができるようにするためには，APIクラスの定義に
若干の細工をする必要があります．

詳細は，\ref pageFileIOImp を参照してください

自動ロード・セーブ可能なオブジェクト Loadable を定義するには，

+ロードしたいデータを含んだLoadableDescを作ります．
+インタフェースクラス LoadableIf を作ります．
+実装クラス Loadableを作ります．
このときクラスに，
 // 自分のDescを((LoadableDesc*)d)にコピーする．
 virtual bool GetDesc(void* d);    
 // sizeof(LoadableDesc)を返す．
 virtual size_t GetDescSize() const;
を宣言してオーバーロードしてください．~
LoadableがLoadableDescを継承しているなら，
 DEF_DESC(Lodable);
とすると，マクロが実装までやってくれます．

+Loadableクラスの親クラスを決めて，親クラスに，
 LoadableIf* CreateLoadable(const LoadableDesc& desc);
を作ります．
+親クラスにCreateObjectが無ければ，
 ObjectIf* CreateObject(const IfInfo* info, const void* desc);
を作ります．
+親クラスのCreateObjectに，info==LoadableIf::GetStateicInfo() の場合の処理を追記します．
 }else if(info == LoadableIf::GetIfInfoStatic()){
  return CreateLoadable(*(const LoadableDesc*)desc);
 }else if ...

**状態の保存・再現ができるクラスの作り方
クラスRegretを定義するには，
+保存したい状態を保持する構造体RegretStateを作ります．
+インタフェースクラス RegretIf を作ります．
+実装クラス Regret を作ります．
このときクラスに，
 // 自分のStateを((RegretState*)s)にコピーする．
 virtual bool GetState(void* s);
 // (RegretState*)s)の状態に自分の状態をセットする．
 virtual bool SetState(void* s);
 // sizeof(LoadableState)を返す．
 virtual size_t GetStateSize() const;
 virtual void ConstructState(void* m){ new(m) RegretState;}
 virtual void DestructState(void* m){ ((RegretState*)m)->~RegretState();}	
を宣言してオーバーロードしてください．~
RegretがRegretStateを継承しているなら，
 DEF_State(Regret);
とすると，マクロが実装までやってくれます．
 DEF_DESC_STATE(Regert)とすると，DescとState両方を実装までしてくれます．
