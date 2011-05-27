/** \page pageFileIO ファイル入出力SDK 


\contents
ファイル入出力\c SDK は，
ファイルからロードしたデータを基にシーングラフの生成したり，
シーングラフのデータを読み出して，ファイルにデータを保存したりします．
下の図は，シーングラフとファイル入出力のイメージです．
（ファイル上でのノードの親子関係とシーングラフの親子関係は必ず対応するわけではありません．）

\image html FileScene.png

\section secSprFile         Sprファイル
Springhead独自のファイル形式です。
人が読み書きしやすく、Springheadの仕様が変化しても余り影響を受けないような形式になっています。
ファイルを手書きする場合はこの形式を使ってください。

sprファイルはノード定義の繰り返しです。sprファイルの例を示します。
\verbatim
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
\endverbatim

SprファイルのノードはSpringhead2のディスクリプタ（\ref secStateDesc を参照）
に１対１で対応します。ディスクリプタさえ用意すれば自動的に使えるノードの型が増えます。
ファイルで値を代入しないと、ディスクリプタの初期値になります。
上の例では、PHSdkに追加されるsphere(CDSphere型)は、
\code
    CDSphereDesc desc;
    desc.material.mu = 0.2;
    desc.radius = 0.5;
\endcode
としたディスクリプタdescで作るのと同じことになります。

Sprファイルの文法をBNF＋正規表現で書くと
\verbatim
spr   = node*
node  = node type, (node id)?, block
block = '{' (node|refer|data)*  '}'
refer = '*' node id
data  = field id, '=', (block | right)
right = '[' value*, ']' | value
value = bool | int | real | str | right
\endverbatim
となります。right以降の解釈はfieldの型に依存します。



\section secXFile       Xファイル
「 X ファイル 」は、Direct3Dのファイルフォーマットで、拡張子は .x です。
モデリングソフトXSIで使われており、多くのモデリングツールで出力できます。
3Dの形状データ、マテリアル、テクスチャ、ボーンなどを含めることができます。
Springhead2では、標準的なXファイルのロードと、Springhead2独自のノードの
ロードとセーブができます。ただし独自ノードを手書きする場合は \ref secSprFile 
の方が書きやすく便利ですのでそちらの使用をおすすめします。

Xファイルの例を示します。
<table width=100% height="50" style="border:3px double #1841a5;">
<tr><td><tt>
　xof 0302txt 0064　　　　#　最初の行はこれから始まる               <br>
                                                                    <br>
　#　　ノードは，                                                   <br>
　#　　　　型名，ノード名 { フィールドの繰り返し   子ノード }       <br>
　#　　からなる．                                                   <br>
　PHScene scene1{                                                   <br>
　　　0.01;0;;　　　　　　#　フィールド は 値; の繰り返し           <br>
　　　1;0;-9.8;0;;　　　　#　値は 数値，文字列またはフィールド      <br>
　　　PHSolid soFloor{　　#　子ノードは，ノードと同じ               <br>
　　　　　(省略)                                                    <br>
　　　}                                                             <br>
　}                                                                 <br>
　# コメントは #以外に // も使える                                  <br>
</tt></td></tr></table>


\section secXNodeDef 独自ノードの定義
\c Springhead2 の通常のノードは，オブジェクトのディスクリプタ（\ref secStateDesc を参照）に１対１で対応します．
ロード時には，ディスクリプタに対応するオブジェクトが生成され，シーングラフに追加されます．
セーブ時には，オブジェクトからディスクリプタを読み出し，ノードの形式でファイルに保存されます．

オブジェクトのディスクリプタには，必ず対応するノードがあります．<br>
例えば，\c SprPHScene.h には，

\verbatim
 struct PHSceneState{
　　　double timeStep;      ///< 積分ステップ
　　　unsigned count;       ///< 積分した回数
 };
 struct PHSceneDesc:PHSceneState{
　　　　/// 接触・拘束解決エンジンの種類
　　　　enum ContactMode{ MODE_NONE, MODE_PENALTY, MODE_LCP};
　　　　Vec3f gravity;      ///< 重力加速度ベクトル．デフォルト値は(0.0f, -9.8f, 0.0f)．
 };
\endverbatim

のように，ステートとディスクリプタが宣言されています．この \c PHSceneDesc に対応する \c X ファイルのノードは，

<table width=100% height="50" style="border:3px double #1841a5;">
<tr><td><tt>
　PHScene scene1{                                                                            <br>
　　　0.01;　　　　　　#　PHSceneState::timeStep                                             <br>
　　　0;;　　　　　　　#　PHSceneState::count　　　　 最後の;はPHSceneState部の終わりを示す．<br>
　　　1;　　　　　　　 #　PHSceneDesc::ContactMode                                           <br>
　　　0;-9.8;0;;　　　 #　PHSceneDesc::gravity　　　　最後の;はPHSceneDesc部の終わりを示す． <br>
　}                                                                                          <br>
</tt></td></tr></table>

のようになります．クラスのメンバ変数がそのままフィールドになります．
また，基本クラスは，先頭にフィールドが追加された形になります．

通常ノードの一覧は\ref pageNodeDefList を参照下さい．
<br><br><br>

\section secXSpecialNode    Xファイルのノード
Springhead2の独自ノードだけでなく、普通のXファイルのノードもロードできます。
Xファイルには、
<table width=100% height="50" style="border:3px double #1841a5;">
<tr><td><tt>
　Frame{                                                                <br>
　　　　FrameTransfromMatrix{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1; }     <br>
　}                                                                     <br>
</tt></td></tr></table>
のようなフレームのノード型がありますが、
Sprinhead2 には対応するディスクリプタやオブジェクトがありません．
そこで，これらは、GRFrameに変換されてロードされます．
\ref pageNodeDefList にノードの一覧があります。
<br><br><br>


<!------------------------------ ノードのロード ---------------------------------------->
\section secXLoadNode           ノードのロード
TODO：記述が古い。要更新。

ディスクリプタに対応するノードは次の手順で生成されます．

<tt>
　　 1．ノードの型に対応するディスクリプタを生成する．                      <br>
　　 2．ディスクリプタにノードのフィールドの内容を記入する．                <br>
　　 3．先祖ノードの CreateObject() API を用いて，オブジェクトを生成する．  <br>
　　 4．生成したオブジェクトに名前を設定する．                              <br>
　　 5．生成したオブジェクトを親オブジェクトに追加する．                    <br>
　　 6．子ノードをロードする．                                              <br>
</tt>
<br><br><br>


\section secFileIOLoadSave ファイルのロードとセーブ
TODO：記述が古い。要更新。

ファイルのロードやセーブを行うには，まずFISdkを作成し，FISdkから，
FIFileXを作成します．次のような手順になります．

<tt>
　　 1．CreateFISdk() で，FISdkのインスタンス fiSdk を作成する．                                                <br>
　　 2．FISdk::CreateFileX() で FIFileX のインスタンス fileX を生成する．                                   <br>
　　 3．ObjectOfs objs を用意する．ロードしたノードのうち，ルートのノードがここに入る．                     <br>
　　 4．FIFileX::Load() を呼び出して，ロードを行う．                                                        <br>
　　 5．セーブするならば，セーブしたいオブジェクトだけを加えた objs を用意し，FIFileX::Save() を呼び出す．  <br>
</tt>
<br>



\section secXDataFormat     各データ型の記述
TODO：記述が古い。要更新。Xではなく、sprに移動すべき。

\subsection subXModelShape  モデル形状
モデル形状を一つの要素として定義します．ここで定義した形状は，オブジェクトタイプとして認識され，
シーン内の任意の位置に配置することができます．モデル形状を用意しておくと，同じ形状を新たに
作成したり，コピーしたりする必要がなく，モデリングを効率的に行うことができます．

\htmlonly 
<h3><a class="anchor">(1)　凸形状メッシュ </a></h3> 
\endhtmlonly
　凸形状メッシュを定義するには，<tt>[型名]</tt>は \c "CDConvexMesh" に，
<tt>[モデル形状タイプ]</tt>は \c "1" に設定して下さい．
\htmlonly
<table width=100% height="50" style="border:3px double #1841a5;">
<tr><td><tt>
　CDConvexMesh [ノード名] {                                                                     <br>
　　　　頂点座標リスト;                                                                         <br>
　　　　動摩擦摩擦係数; 静止摩擦係数; 跳ね返り係数;;　　# [物理シミュレーションに関係する材質]  <br>
　}                                                                                             <br>
</tt></td></tr></table>
\endhtmlonly

　次に示すのは，長さ \c 1 の立方体を，凸形状メッシュで定義した例です．
\htmlonly
<table width=100% height="50" style="border:3px double #1841a5;">
<tr><td><tt>
　CDConvexMesh meshBlock {                                                                  <br>
　　　　0.5; 0.5; 0.5;, 0.5; 0.5;-0.5;, -0.5; 0.5;0.5;, -0.5; 0.5;-0.5;,                    <br>
　　　　0.5;-0.5; 0.5;, 0.5;-0.5;-0.5;, -0.5;-0.5;0.5;, -0.5;-0.5;-0.5;;                    <br>
　　　　0.2; 0.5; 0.6;;　　　　　　　　　　　　　　　　　　　　　　　　　　# phmaterial <br>
　}                                                                                         <br>
</tt></td></tr></table>
\endhtmlonly
<br>


\htmlonly 
<h3><a class="anchor">(2)　球 </a></h3> 
\endhtmlonly
　球を定義するには，<tt>[型名]</tt>は \c "CDSphere" に設定して下さい．
\htmlonly
<table width=100% height="50" style="border:3px double #1841a5;">
<tr><td><tt>
　CDSphere [ノード名] {                                                                             <br>
　　　　半径;                                                                                       <br>
　　　　動摩擦摩擦係数; 静止摩擦係数; 跳ね返り係数;;　　　 # [物理シミュレーションに関係する材質]   <br>
　}                                                                                                 <br>
</tt></td></tr></table>
\endhtmlonly

　次に示すのは，半径 \c 1 の球を定義した例です．
\htmlonly
<table width=100% height="50" style="border:3px double #1841a5;">
<tr><td><tt>
　CDSphere sphere {                                 <br>
　　　　1.0;　　　　　　　　　 //  radius           <br>
　　　　0.2; 0.5; 0.6;;　　　　//  phmaterial       <br>
　}                                                 <br>
</tt></td></tr></table>
\endhtmlonly
<br><br>


\htmlonly 
<h3><a class="anchor">(3)　直方体 </a></h3> 
\endhtmlonly
　直方体を定義するには，<tt>[型名]</tt>は \c "CDBox" に設定して下さい．
\htmlonly
<table width=100% height="50" style="border:3px double #1841a5;">
<tr><td><tt>
　CDBox [ノード名] {                                                                                <br>
　　　　直方体サイズ;                                                                               <br>
　　　　動摩擦摩擦係数; 静止摩擦係数; 跳ね返り係数;;　　　 # 物理シミュレーションに関係する材質 <br>
　}                                                                                                 <br>
</tt></td></tr></table>
\endhtmlonly

　次に示すのは，\c X 方向のサイズが \c 1.0，Y 方向のサイズが \c 2.0，\c Z 方向のサイズが \c 3.0 の直方体を定義した例です．
\htmlonly
<table width=100% height="50" style="border:3px double #1841a5;">
<tr><td><tt>
  CDBox box {                                       <br>
　　　　1.0; 2.0; 3.0;;　　　　//  boxsize          <br>
　　　　0.2; 0.5; 0.6;;　　　　//  phmaterial       <br>
　}                                                 <br>
</tt></td></tr></table>
\endhtmlonly
<br><br><br>

*/
