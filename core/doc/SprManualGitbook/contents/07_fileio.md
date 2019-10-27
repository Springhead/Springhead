# FileIO

FileIOはファイル入出力機能を提供するモジュールです．Frameworkから利用するのが簡単ですが、単体で用いるとより細かな作業ができます。





## FileIO SDK
FileIOモジュールのすべてのオブジェクトはSDKクラス*FISdk*によって管理されます．*FISdk*クラスは，プログラムの実行を通してただ１つのオブジェクトが存在するシングルトンクラスです．*FISdk*オブジェクトを作成するには以下のようにします．
```c++
FISdkIf* fiSdk = FISdkIf::CreateSdk();
```
通常この操作はプログラムの初期化時に一度だけ実行します．また，Frameworkモジュールを使用する場合はユーザが直接*FISdk*を作成する必要はありません．*FISdk*には以下の2つの機能があります．

-  ファイルオブジェクトの作成
-  インポートオブジェクトの作成



ファイルオブジェクトは，ファイルからのシーンのロードおよびセーブを担います．ファイルの基底クラスは*FIFile*で，ファイルフォーマットの種類ごとに専用のファイルクラスが派生します(次図)．

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/fifile.svg) 



ファイル作成に関する*FISdk*の関数を以下に示します．

|*FISdkIf*															 ||
|---|---|
|_FIFileSprIf*_	| *CreateFileSpr()*						|
|_FIFileBinaryIf*_ | *CreateFileBinary()*					|
|_FIFileXIf*_		| *CreateFileX()*						|
|_FIFileVRMLIf*_	| *CreateFileVRML()*						|
|_FIFileCOLLADAIf*_| *CreateFileCOLLADA()*					|
|_FIFileIf*_		| *CreateFileFromExt(UTString filename)*	|
*CreateFileFromExt*は*filename*の拡張子からファイルフォーマットを判別して対応するファイルオブジェクトを作成します．



## ファイルフォーマット
この節ではSpringheadでロード・セーブできるファイルのファイルフォーマットを紹介します。



### sprファイル

拡張子 .spr のファイルは、Springhead独自のファイル形式です。人が読み書きしやすく、Springheadの仕様が変化しても余り影響を受けないような形式になっています。ファイルを手書きする場合はこの形式を使ってください。sprファイルはノード定義の繰り返しです。sprファイルの例を示します。
```c++
PHSdk{                  #PHSdkノード
    CDSphere sphere{    #↑の子ノードにCDSphereノードを追加
        material = {    # CDSphere の material(PHMaterial型)の
            mu = 0.2    # 摩擦係数 mu に0.2を代入
        }
        radius = 0.5    # radiusに0.5を代入
    }
    CDBox bigBox{
        boxsize = 2.0 1.1 0.9
    }
}
```
Sprファイルのノードはディスクリプタ（\SECTION{if_desc})を参照）に１対１で対応します。ディスクリプタさえ用意すれば自動的に使えるノードの型が増えます。ファイルで値を代入しないと、ディスクリプタの初期値になります。上の例では、*PHSdk*に追加される*sphere*(*CDSphere*型)は、
```c++
CDSphereDesc desc;
desc.material.mu = 0.2;
desc.radius = 0.5;
```
としたディスクリプタ *desc* で作るのと同じことになります。Sprファイルの文法をBNF＋正規表現で書くと
```c++
spr   = node*
node  = node type, (node id)?, block
block = '{' (node|refer|data)*  '}'
refer = '*' node id
data  = field id, '=', (block | right)
right = '[' value*, ']' | value
value = bool | int | real | str | right
```
となります。*right*以降の解釈は*field*の型に依存します。



### Xファイル

「 X ファイル 」は、Direct3Dのファイルフォーマットで、拡張子は .x です。モデリングソフトXSIで使われており、多くのモデリングツールで出力できます。3Dの形状データ、マテリアル、テクスチャ、ボーンなどを含めることができます。Springhead2では、標準的なXファイルのロードと、Springhead2独自のノードのロードとセーブができます。ただし独自ノードを手書きする場合は Sprファイルの方が書きやすく便利ですのでそちらの使用をおすすめします。Xファイルの例を示します。
```c++
xof 0302txt 0064        #最初の行はこれから始まる

#    ノードは，
#        型名，ノード名 { フィールドの繰り返し   子ノード }
#    からなる．
PHScene scene1{
    0.01;0;;            #フィールド は 値; の繰り返し
    1;0;-9.8;0;;        #値は 数値，文字列またはフィールド
    PHSolid soFloor{    #子ノードは，ノードと同じ
        (省略)
    }
}
# コメントは #以外に // も使える
```

#### 独自ノードの定義
Springhead2 の通常のノードは，オブジェクトのディスクリプタ（\SECTION{if_desc}節）に１対１で対応します．ロード時には，ディスクリプタに対応するオブジェクトが生成され，シーングラフに追加されます．セーブ時には，オブジェクトからディスクリプタを読み出し，ノードの形式でファイルに保存されます．オブジェクトのディスクリプタには，必ず対応するノードがあります．例えば，*SprPHScene.h* には，
```c++
struct PHSceneState{
    double timeStep;      ///< 積分ステップ
    unsigned count;       ///< 積分した回数
};
struct PHSceneDesc:PHSceneState{
    /// 接触・拘束解決エンジンの種類
    enum ContactMode{ MODE_NONE, MODE_PENALTY, MODE_LCP};
    Vec3f gravity;      ///< 重力加速度ベクトル．デフォルト値は(0.0f, -9.8f,0.0f)．
};
```
のように，ステートとディスクリプタが宣言されています．この *PHSceneDesc* に対応する X ファイルのノードは，
```c++
PHScene scene1{                                                                     0.01;     #PHSceneState::timeStep
    0;;       #PHSceneState::count     最後の;はPHSceneState部の終わりを示す．
    1;        #PHSceneDesc::ContactMode
    0;-9.8;0;;#PHSceneDesc::gravity    最後の;はPHSceneDesc部の終わりを示す．
}
```
のようになります．クラスのメンバ変数がそのままフィールドになります．また，基本クラスは，先頭にフィールドが追加された形になります．通常ノードの一覧は \URL{TBU: デスクリプタ一覧のページ} を参照下さい．
#### Xファイルのノード
Springhead2の独自ノードだけでなく、普通のXファイルのノードもロードできます。Xファイルには、
```c++
Frame{
    FrameTransfromMatrix{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1; }
}
```
のようなフレームのノード型がありますが、Sprinhead2 には対応するディスクリプタやオブジェクトがありません．そこで，これらは、*GRFrame*や*PHFrame*に変換されてロードされます．\URL{TBW ノード一覧のページ(pageNodeDefList)} を参照下さい．





## ファイルのロード・セーブ

下図は、ファイルのロード・セーブの手順を示しています。

 ![img](http://springhead.info/dailybuild/generated/doc/SprManual/fig/fileOperation.svg) 



ロード時にはまずファイルをパースしてディスクリプタのツリーを作ります。次にディスクリプタのツリーをたどりながら、オブジェクトのツリーを作ります。一方、セーブ時には、ディスクリプタツリーは作りません。オブジェクトツリーをたどりながらオブジェクトからディスクリプタを作り、その場でファイルに書きだしていきます。

ファイルのノードとディスクリプタツリーのノードは１対１に対応しますが、オブジェクトのツリーではそうとは限りません。



### ファイルロードの仕組み

#### ファイルのパース
ファイルのロードは、*FIFileSpr*や*FIFileX*のような*FIFile*の派生クラスの*LoadImp()*メソッドが行います。ファイルパースの実装は、boost::spiritを用いて実装されています。*Init()*メソッドでパーサの文法を定義しています。
#### ディスクリプタの生成
パーサは*FILoadContext*をコンテキストとして用いながらパースを進めます。*fieldIts*にロード中のデータの型情報をセットしていきます。ノード名やメンバ名からディスクリプタやメンバの型を知る必要がありますが、ビルド時にSWIGで生成しているディスクリプタの型情報を*??Sdk::RegisterSdk()*が登録したものを用いています。新しいノードが出てくる度に*FILoadContext::datas*にディスクリプタを用意し、データをロードするとそこに値をセットしていきます。他のノードへの参照は、この時点ではノード名の文字列で記録しておきます。
#### 参照のリンク
ファイルをすべてロードし終わると、*LoadImp()*から抜けて、*FIFile::Load(FILoadContext*)*に戻ってきます。他のノード(他のディスクリプタ)への参照をノード名の文字列を頼りにポインタでつないでいきます。
#### オブジェクトの生成
オブジェクト生成は、*FILoadContext::CreateScene()*が、ディスクリプタツリーを根本からたどりながら順に行います。ディスクリプタからオブジェクトを生成するのは、そのオブジェクトの先祖オブジェクトです。先祖オブジェクトが生成できない場合はSDKの生成を試みます。SDK以外が一番根本にあるファイルをロードするためには、予め先祖オブジェクトを用意しておく必要があります。*FIFile::Load(ObjectIfs\& objs, const char* fn)*の*objs*引数はその役割をします。*

*生成されたオブジェクトは、親の*AddChildObject()*ですぐに子として追加されます。

#### 参照のリンク
ディスクリプタ間の参照はポインタになっていますが、シーングラフは繋がっていません。ディスクリプタの参照に従って、ディスクリプタから生成されたオブジェクト間に参照を追加します。リンクは、*AddChildObject()*関数を呼び出すことで行われます。親子と参照の区別はつかなくなります。あるノードの下に子ノードを書いても、別のところに書いたノードへの参照を書いても同じシーグラフになるわけです。



### ファイルロードの実際

Frameworkを使うのと簡単です。
```c++
virtual void FWMyApp::Init(int argc, char* argv[]){
    UTRef<ImportIf> import = GetSdk()->GetFISdk()->CreateImport();
    GetSdk()->LoadScene(fileName, import);  // ファイルのロード
    GetSdk()->SaveScene("save.spr", import);// ファイルのセーブテスト
```
FISdk単体で使う場合は次のようになります。
```c++
int main(){
    //  ファイルローダで生成できるように、各SDKの型情報を登録
    PHSdkIf::RegisterSdk();
    GRSdkIf::RegisterSdk();
    FWSdkIf::RegisterSdk();
    //  ファイルのロード
    UTRef<FISdkIf> fiSdk = FISdkIf::CreateSdk();
    FIFileIf* file = fiSdk->CreateFileFromExt(".spr");
    ObjectIfs objs; //  ロード用オブジェクトスタック
    fwSdk = FWSdkIf::CreateSdk();   //  FWSDKを用意
    //  子オブジェクト作成用にfwSdkをスタックに積む
    objs.push_back(fwSdk);
    //  FWSDK以下全体をファイルからロード
    if (! file->Load(objs, "test.spr") ) {  
        DSTR << "Error: Cannot open load file. " << std::endl;
        exit(-1);
    }
    //  ファイル中のルートノード（複数の可能性あり）がobjsに積まれる。
    for(unsigned  i=0; i<objs.size(); ++i){ 
        objs[i]->Print(DSTR);
    }
    ...
```



### ファイルセーブの仕組み

ファイルセーブは、*FIFile*がシーングラフをたどりながら、オブジェクトをセーブしていきます。各オブジェクトの*GetDescAddress()*か、実装されていなければ*GetDesc()*を呼び出してディスクリプタを読み出します。シーングラフには、あるノードが複数のノードの子ノードになっている場合があるため、2重にセーブしないように2度目以降は参照としてセーブします。

ディスクリプタを取り出したら、ディスクリプタの型情報を利用して、ディスクリプタのメンバを順番にセーブしていきます。実際にデータをファイルに保存するコードは、*FiFileSpr*など*FiFile*の派生クラスにあります。



### ファイルセーブの実際

Frameworkを使うのと簡単です。
```c++
virtual void FWMyApp::Save(const char* filename){
    UTRef<ImportIf> import = GetSdk()->GetFISdk()->CreateImport();
    GetSdk()->SaveScene(filename, import);	// filenameにシーンをセーブ
```
FISdk単体で使う場合は次のようになります。
```c++
void save(const char* filename, ImportIf* ex, ObjectIf* rootNode){
    //  ファイルのセーブ
    UTRef<FISdkIf> fiSdk = FISdkIf::CreateSdk();
    FIFileIf* file = fiSdk->CreateFileFromExt(".spr");
    ObjectIfs objs; //  ロード用オブジェクトスタック
    objs.push_back(rootNode);
    file->SetImport(ex);
    file->Save(*objs, filename);
}
```





## インポート情報の管理

T.B.W.（Importを使うと別のファイルに書いたノードを呼び出すことができる。Importを使ってロードしたシーンをセーブ場合、ファイル保存時にどこまでをファイルに保存するのかが問題になる。これを管理するのがImportの役割だと思う。by 長谷川)
