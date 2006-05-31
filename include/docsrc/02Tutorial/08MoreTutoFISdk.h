/** \page pageMoreTutoFISdk	ファイル入出力SDK チュートリアル

<br>
\section contentsMoreTutoFISdk	◇ Contents ◇
　　 \ref moreTutoFISdk						<br>
　　　　 \ref moreTutoCreateFISdk			<br>
　　 \ref moreTutoFileIO					<br>
　　　　 \ref moreTutoCreateFileX			<br>
　　　　 \ref moreTutoLoadFileX				<br>
　　　　 \ref moreTutoSaveFileX				<br>
　　 \ref moreTutoFileX						<br>
　　　　 \ref moreTutoFileXComment			<br>
　　　　 \ref moreTutoFileXHeader			<br>
　　　　 \ref moreTutoFileXDataForm			<br>
　　　　 \ref moreTutoFileXModelShape		<br>
<br><br>
<hr>

<!-------------------- 1．　ファイル入出力SDK ------------------------------------------>
\section moreTutoFISdk		1．　ファイル入出力SDK
はじめに、ファイル入出力に必要となる \c SDK を用意します．

\subsection moreTutoCreateFISdk		1.1．　ファイル入出力SDKの作成
ファイル入出力 \c SDK を作成します．
\verbatim
    FISdkIf* fiSdk = CreateFISdk();
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutoFISdk">top of page</a>
</div> 
<hr>


<!-------------------- 2．　Xファイル ローダ＆セーバ ----------------------------------->
\section  moreTutoFileIO		2．　ローダ＆セーバ
任意の \c X ファイルの読み込み、書き出しが可能な、ローダ＆セーバが \c Springhead2 には用意されています．
ローダ機能を用いれば、オブジェクトの頂点座標や物理パラメータなどを気にせず、\ref pageGraphics の 
<tt>DrawScene()</tt> メソッドなどを呼び出すだけで描画できます．

\subsection moreTutoCreateFileX		2.1．　ファイルハンドルの作成
ファイルの入出力にはファイルハンドルを使います．
\verbatim 
    FIFileXIf* fileX = fiSdk->CreateFileX();     // ファイルハンドルの作成
\endverbatim
<br>

\subsection moreTutoLoadFileX		2.2．　Xファイルローダー
指定された \c X ファイルをロードします．
\verbatim 
　　ObjectIfs objs;　　　　　　　　　　　　　　　// インタフェースクラスへのポインタ配列を用意
　　fileX->Load(objs, "test.x");                 // Xファイルをロード
\endverbatim
<br>

\subsection moreTutoSaveFileX		2.3．　Xファイルセーバー
指定した名前で、\c X ファイルのセーブを行います．
\verbatim 
　　fileX->Save(objs, "out.x");                  // Xファイルのセーブ
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutoFISdk">top of page</a>
</div> 
<hr>

<!-------------------- 3．　Xファイル  ------------------------------------------------->
\section moreTutoFileX		3．　Xファイル
「\c X ファイル」とは、\c DirectX の汎用的なファイル形式であり、\ref pageFileIO では、
\c X ファイルに対して、ロードやセーブ、構造解析を行います．独自の \c template 構文で
自由にデータ型の定義が行えるのが特徴です．

\subsection moreTutoFileXComment		3.1．　コメント
\c X ファイルでは、コメントの開始記号として、\c C++ 形式のダブルスラッシュ \c "//" 、または特殊記号 \c "#" を使用します．
コメントの開始記号から行端までを、コメントと解釈します．
\verbatim 
　　# This is a comment.
　　// これもコメントです．
\endverbatim
<br>

\subsection moreTutoFileXHeader		3.2．　ヘッダー
\c X ファイルのヘッダーとして、有効な例を以下に示します．
\verbatim 
　　xof 0302txt 0064
\endverbatim
<br>

\subsection moreTutoFileXDataForm		3.3．　基本的なデータ形式
データ形式は基本的に次の通りです．<br>
\verbatim 
　　<データ型> [name] {
　　    メンバー 1;
　　    　 ：
　　    メンバー n;
　　 }
\endverbatim
<br>

\subsection moreTutoFileXModelShape		3.4．　モデル形状
モデル形状を一つの要素として定義します．ここで定義した形状は、オブジェクトタイプとして認識され、
シーン内の任意の位置に配置することができます．モデル形状を用意しておくと、同じ形状を新たに
作成したり、コピーしたりする必要がなく、モデリングを効率的に行うことができます．

\b 3.4.1.　 \b 凸形状メッシュ <br>
　凸形状メッシュを定義するには、<tt><データ型></tt>は \c "CDConvexMesh" に、
<tt>[モデル形状タイプ]</tt>は \c "1" に設定してください．
\verbatim 
　　CDConvexMesh [name] {
　　　　1;;                                                 # モデル形状タイプ          
　　　　頂点座標リスト;
　　　　動摩擦摩擦係数; 静止摩擦係数; 跳ね返り係数;;　　　　# 物理シミュレーションに関係する材質
　　}
\endverbatim
　次に示すのは、長さ \c 1 の立方体を、凸形状メッシュで定義した例です．
\verbatim 
　　CDConvexMesh meshBlock {
　　　　1;;                                                                 # type=mesh
　　　　0.5; 0.5; 0.5;, 0.5; 0.5;-0.5;, -0.5; 0.5;0.5;, -0.5; 0.5;-0.5;, 
　　　　0.5;-0.5; 0.5;, 0.5;-0.5;-0.5;, -0.5;-0.5;0.5;, -0.5;-0.5;-0.5;;
　　　　0.2; 0.5; 0.6;;                                                     # phmaterial
　　}
\endverbatim
<br>

\b 3.4.2.　 \b 球 <br>
　球を定義するには、<tt><データ型></tt>は \c "CDSphere" に、<tt>[モデル形状タイプ]</tt>は、\c "2" に設定してください．
\verbatim 
　　CDSphere [name] {
　　　　2;;                                                 # モデル形状タイプ          
　　　　半径;
　　　　動摩擦摩擦係数; 静止摩擦係数; 跳ね返り係数;;　　　　# 物理シミュレーションに関係する材質
　　}
\endverbatim
　次に示すのは、半径 \c 1 の球を定義した例です．
\verbatim 
　　CDSphere sphere {
　　　　2;;                     //  type=sphere
　　　　1.0;                    //  radius
　　　　0.2; 0.5; 0.6;;         //  phmaterial
　　}
\endverbatim
<br>

\b 3.4.3.　 \b 直方体 <br>
　直方体を定義するには、<tt><データ型></tt>は \c "CDBox" に、<tt>[モデル形状タイプ]</tt>は、\c "3" に設定してください．
\verbatim 
　　CDBox [name] {
　　　　3;;                                                 # モデル形状タイプ          
　　　　直方体サイズ;
　　　　動摩擦摩擦係数; 静止摩擦係数; 跳ね返り係数;;　　　　# 物理シミュレーションに関係する材質
　　}
\endverbatim
　次に示すのは、\c X 方向のサイズが \c 1.0、Y 方向のサイズが \c 2.0、\c Z 方向のサイズが \c 3.0 の直方体を定義した例です．
\verbatim 
    CDBox box {
　　　　3;;                     //  type=box
　　　　1.0; 2.0; 3.0;;         //  boxsize
　　　　0.2; 0.5; 0.6;;         //  phmaterial
　　}	
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutoFISdk">top of page</a>
</div> 


*/
