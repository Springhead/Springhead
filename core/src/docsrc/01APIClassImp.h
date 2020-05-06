/**
\page pageAPIClassImp APIクラスの作り方・実装の仕方

\section secCreateAPIClass APIクラスの作り方
ここでは，SpringheadのAPIクラスの宣言と実装の手順について説明します．
APIクラスの概要，使い方については，\ref pageApiBasic をご参照ください．

\subsection secDefAPIClass APIクラスの宣言
APIクラスのヘッダファイルは，Springhead2/include/Sdk名 に作ります．
(Sdk名はSDKの名前 Graphics，Physicsなど)<br>
ヘッダファイルの名前は，Sprオブジェクト名.h (SprPHSolid.h など)とします．
APIクラスを宣言するには，
<pre>
 struct SceneObjectIf: NamedObjectIf{
 	SPR_IFDEF(SceneObject);
 	///	所属Sceneの取得
 	virtual SceneIf* GetScene();
 };
</pre>
のように，
<ol>
 <li> 基本のインタフェースクラスを継承する．
 <li> SPR_IFDEF(クラス名)メンバを持つ．
 <li> API関数を普通の関数として宣言する(virtual をつけてはいけない)．
</ol>
をします．
SPR_IFDEF()は，いくつかのメンバの宣言をまとめたものです．
SPR_IFDEF()に対応する実装は，Springhead2/bin/swig/swig.exe が自動生成します．
Springhead2/src/Sdk名/Sdk名Stub.cpp
に，
<pre>
SPR_IFIMP?(クラス名, 基本クラス名);
</pre>
という行ができます．これはSPR_IFDEF()に対応するメンバの実装になります．
Cast()メンバ関数，DCAST()マクロが利用する型情報もここに入ります．

\section secCreateImplementClass 実装クラスの作り方
APIクラスを作ったら，それを実装するクラスを作ります．

\subsection defImplementClass 実装クラスの宣言
実装クラスは，~
<pre>
 class SceneObject:public NamedObject{
 	SPR_OBJECTDEF(SceneObject);		///<	クラス名の取得などの基本機能の実装
 public:
 	virtual void SetScene(SceneIf* s);
 	virtual SceneIf* GetScene();
 	///	デバッグ用の表示
 	virtual void Print(std::ostream& os) const {NamedObject::Print(os);}
 };
</pre>
のように，宣言します．

\subsubsection secOBJECT_DEF SPR_OBJECTDEF()マクロ
クラスの宣言のなかのSPR_OBJECTDEF()は，
型情報のためのStatic関数(GetTypeInfo(), GetTypeInfoStatic())などを宣言します．
抽象クラスの場合は，実体化できないというエラーがでるので，代わりにSPR_OBJECTDEF_ABSTを使います．
また，インタフェースを持たないクラスの場合は，代わりにSPR_OBJECTDEF_NOIFを使います．

基本クラスがテンプレートクラスの場合，基本クラスリストの取得が上手く行きません．
この場合，SPR_OBJECTDEF1(cls, base); のように，マクロで直接基本クラスを指定します．

SPR_OBJECTDEF()の実装も，SPR_IFDEF()の場合と同様に，
 Springhead2/src/Sdk名/Sdk名Stub.cpp に，
<pre>
SPR_OBJECTIMP?(クラス名, 基本クラス名);
</pre>
という行ができます．これはSPR_OBJECTDEF()に対応するメンバの実装になります．
Cast()メンバ関数，DCAST()マクロが利用する型情報もここに入ります．

\subsubsection secGetAPI APIの取得
インタフェースとオブジェクトを指すポインタは型が異なりますが，同じアドレスを指すポインタです．
Object::GetObjectIf() は ObjectのthisポインタをObjectIf*にキャストして返します．
APIクラスは thisポインタを
<pre>
size_t ObjectIf::NChildObject() const {
	return ((Object*)this)->NChildObject();
}
</pre>
のように，thisポインタの型を実装クラスに戻して関数を呼び出しています．

\subsection secImpAPIClass 宣言したAPIの実装
まず，Springhead/src/Sdk名/SceneObject.h で，
<pre>
class SceenObject: public NamedObject{
	SPR_OBJECTDEF(SceneObject);
	virutal void SetScene(SceneIf* s);	// 実装側の宣言
};
</pre>
のように，関数を宣言し，
まず，Springhead/src/Sdk名/SceneObject.cpp で，
<pre>
 void SceneObject::SetScene(SceneIf* s){
 	SetNameManager(s->GetObj<NameManager>());
 	nameManager->GetNameMap();
 }
</pre>
のように，宣言したAPIを実装します．
APIクラスだけにあり，実装クラスにない関数があると，
Springhead2/src/Sdk名/Sdk名Stub.cpp をコンパイルするときに，
エラーになります．

\section sec_FileLoadSave ファイルからのロード・ファイルへのセーブ
FileIO SDK (FileIO) でロード・セーブができるようにするためには，
APIクラスの定義に若干の細工をする必要があります．
FileIO SDK の詳細は，\ref pageFileIOImp を参照してください．

\subsection secCreateLoadableObject ロード・セーブ可能なオブジェクトの作り方

<ol>
 <li>ロードしたいデータを含んだディスクリプタ(例:PHSolidDesc)を作ります．
 <li>インタフェースクラス(例:PHSolidIf)を作ります．
 <li>実装クラス(例:PHSolid)を作ります．このとき実装クラスで，
	<pre>
	///	デスクリプタの読み出し(コピー版)
	virtual bool GetDesc(void* desc) const { return false; }
	///	デスクリプタのサイズ
	virtual size_t GetDescSize() const { return 0; };
	</pre>
	をオーバーロードしてください．
	<pre>
	///	ディスクリプタの読み出し(コピー版)
	virtual bool GetDesc(void* d) const;
	</pre>
	の代わりに
	<pre>
	///	ディスクリプタの読み出し(参照版)
	virtual const void* GetDescAddress() const { return NULL; }
	</pre>
	をオーバーロードするとコピーが減って効率がよくなります。
 <li> シーングラフ上で，実装クラス(例:PHSolid)の先祖になるクラスのなかから，
      実装クラス(例:PHSolid)を生成するクラス(例:PHScene)を決めます．
 <li> 生成クラス(例:PHScene)のCreateObject()が実装クラス(PHSolid)を作れるように，
      <pre>
	PHSceneIf::GetIfInfoStatic()->RegisterFactory(new FactoryImp(PHSolid));
	  </pre>
      のようにファクトリを登録します．
      登録は，ファイルのロードより前に行わなければなりません．
      SDKのコンストラクタで1度だけ呼び出すのが良いでしょう．
      詳しくは \ref secFactory を参照ください．
</ol>
手順3の3つの関数のオーバーロードは、オブジェクト(例:PHSolid)が、
デスクリプタ(例:PHSolidDesc)を継承しているならば，
  <pre>
	ACCESS_DESC(実装クラス名);
  </pre>
マクロを実装クラス(例:PHSolid)の宣言の中に置けば，オーバーライドしてくれます．


\subsection secCreateNonInheritLoadableObject 非継承時のメンバ宣言の自動化
多重継承の都合でデスクリプタ(例:PHBallJointDesc)を継承できない場合があります。
実装クラスが別の実装クラス(=基本実装クラス)を継承する場合、基本実装クラスは既に
基本用のDescを継承しているからです。
DescはDescで基本用のDescを継承していますから、
基本Descと派生Descを同時に継承すると、基本部分のメンバーがダブってしまいます。

そのため、派生実装クラスではデスクリプタが継承できません。
その場合は、
SPR_DECLMEMBEROF_デスクリプタ名 マクロ(例:SPR_DECLMEMBEROF_PHBallJointDesc)
を使えば自動化ができます。デスクリプタを定義すると、
それに対応するマクロSPR_DECLMEMBEROF_デスクリプタ名マクロ
(例:SPR_DECLMEMBEROF_PHBallJointDesc)が自動的に定義されます。
オブジェクト(例:PHBallJoint)の定義の中で、
<pre>
	SPR_DECLMEMBEROF_デスクリプタ名;
</pre>
を定義すると、GetDesc()とGetDescSize()がオーバーライドされます。

\section secStateLoadSave 状態の保存・再現
ファイルへのロードセーブでは，何も無い状態からオブジェクトを生成して
シーングラフを作ります．これに対して，
シーンのオブジェクトの構造は変わらないけれども，シミュレーション中に
10ステップ前の状態に戻したいなどということも良くあるでしょう．
そんな用途に使うのが状態の保存・再現です．

状態の保存・再現ができるクラスを作るには，
<ol>
 <li> 保存したい状態を保持する構造体(例:PHSolidState)を作ります．
 <li> 実装クラス (例:PHSolid)を作ります．
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
	実装クラス(例:PHSolid)が状態構造体(例:PHSolidState)を継承しているなら，
	<pre>
	ACCESS_STATE(実装クラス名);
	</pre>
	マクロを実装クラスの宣言の中に置けば，オーバーライドしてくれます．
	ACCESS_DESC_STATE() マクロを置けば，DescとState両方をオーバーライドしてくれます．
*/
