/** \page pageMoreTutoFISdk	ファイル入出力SDK チュートリアル

<br>
\section contentsMoreTutoFISdk	◇ Contents ◇
　　 \ref moreTutoFISdk						<br>
　　　　 \ref moreTutoCreateFISdk			<br>
　　 \ref moreTutoFileIO					<br>
　　　　 \ref moreTutoCreateFileX			<br>
　　　　 \ref moreTutoLoadFileX				<br>
　　　　 \ref moreTutoSaveFileX				<br>
<br><br>
<hr>

<!-------------------- 1．　ファイル入出力SDK ----------------------------------------->
\section moreTutoFISdk		1．　ファイル入出力SDK
はじめに、ファイル入出力に必要となるSDKを用意します．

\subsection moreTutoCreateFISdk		1.1．　ファイル入出力SDKの作成
ファイル入出力SDKを作成します．
\verbatim
    FISdkIf* fiSdk = CreateFISdk();
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutoFISdk">top of page</a>
</div> 
<hr>


<!-------------------- 2．　Xファイル ローダ＆セーバ -------------------------------------------->
\section  moreTutoFileIO		2．　ローダ＆セーバ
任意のXファイルの読み込み、書き出しが可能な、ローダ＆セーバがSpringhead2には用意されています．
ローダ機能を用いれば、オブジェクトの頂点座標や物理パラメータなどを気にせず、\ref pageGraphics の
DrawScene()メソッドなどを呼び出すだけで描画できます．

\subsection moreTutoCreateFileX		2.1．　ファイルハンドルの作成
ファイルの入出力にはファイルハンドルを使います．
\verbatim 
    FIFileXIf* fileX = fiSdk->CreateFileX();     // ファイルハンドルの作成
\endverbatim
<br>

\subsection moreTutoLoadFileX		2.2．　Xファイルローダー
指定されたXファイルをロードします．
\verbatim 
　　ObjectIfs objs;　　　　　　　　　　　　　　　// インタフェースクラスへのポインタ配列を用意
　　fileX->Load(objs, "test.x");                 // Xファイルをロード
\endverbatim
<br>

\subsection moreTutoSaveFileX		2.3．　Xファイルセーバー
指定した名前で、Xファイルのセーブを行います．
\verbatim 
　　fileX->Save(objs, "out.x");                  // Xファイルのセーブ
\endverbatim
<br><br>
<div align="right">
<a href="#pageMoreTutoFISdk">top of page</a>
</div> 


*/
