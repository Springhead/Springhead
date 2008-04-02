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
 	SPR_IFDEF(SceneObject);
 	///	所属Sceneの取得
 	virtual SceneIf* GetScene() =0;
 };
</pre>
のように，
<ol>
 <li> 基本のインタフェースクラスを継承する．
 <li> SPR_IFDEF(クラス名)メンバを持つ．
 <li> API関数を純粋仮想関数として宣言する．
</ol>
をします．
SPR_IFDEF()は，いくつかのメンバの宣言をまとめたものです．
SPR_IFDEF()に対応する実装は，Springhead2/bin/swig/swig.exe が自動生成します．
Springhead2/src/SDKNAME/SDKNAMEStub.cpp
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
 class SceneObject:public NamedObject, SceneObjectIfInit{
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

SPR_OBJECTDEF()の実装も，SPR_IFDEF()の場合と同様に，
 Springhead2/src/SDKNAME/SDKNAMEStub.cpp に，
<pre>
SPR_OBJECTIMP?(クラス名, 基本クラス名);
</pre>
という行ができます．これはSPR_OBJECTDEF()に対応するメンバの実装になります．
Cast()メンバ関数，DCAST()マクロが利用する型情報もここに入ります．

\subsubsection secGetAPI APIの取得
Object::GetObjectIf() は thisをObjectIf*にキャストして返します．
APIクラスは thisを
<pre>
size_t ObjectIf::NChildObject() const {
	return ((Object*)this)->NChildObject();
}
</pre>
のように，thisの型を実装クラスに戻して関数を呼び出しています．

\subsection secImpAPIClass 宣言したAPIの実装
まず，Springhead/src/SDKNAME/SceneObject.h で，
<pre>
class SceenObject: public NamedObject{
	SPR_OBJECTDEF(SceneObject);
	virutal void SetScene(SceneIf* s);	// 実装側の宣言
};
</pre>
のように，関数を宣言し，
まず，Springhead/src/SDKNAME/SceneObject.cpp で，
<pre>
 void SceneObject::SetScene(SceneIf* s){
 	SetNameManager(s->GetObj<NameManager>());
 	nameManager->GetNameMap();
 }
</pre>
のように，宣言したAPIを実装します．
APIクラスだけにあり，実装クラスにない関数があると，
Springhead2/src/SDKNAME/SDKNAMEStub.cpp をコンパイルするときに，
エラーになります．

\section secFileLoadSave ファイルからのロード・ファイルへのセーブ
FileIO SDK(\ref pageFileIO) でロード・セーブができるようにするためには，APIクラスの定義に
若干の細工をする必要があります．
FileIO SDK の詳細は，\ref pageFileIOImp を参照してください

自動ロード・セーブ可能なオブジェクトを定義するには，

以下若干古い． SPR_DECLMEMBEROFクラス名に言及すべき
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
