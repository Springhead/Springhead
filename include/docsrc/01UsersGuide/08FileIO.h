/** \page pageFileIO ファイル入出力SDK 


\contents


ファイル入出力\c SDK は，
ファイルからロードしたデータを基にシーングラフの生成したり，
シーングラフのデータを読み出して，ファイルにデータを保存したりします．
下の図は，シーングラフとファイル入出力のイメージです．
（ファイル上でのノードの親子関係とシーングラフの親子関係は必ず対応するわけではありません．）

\image html FileScene.png


<!------------------------------ Xファイル --------------------------------------------->
\section secXFile 		Xファイル
「 \c X ファイル 」とは，ファイル名に <tt>.x</tt> 拡張子が付いたファイルを指し，  
\c Direct3D におけるモデルデータの標準フォーマットです． 
\c 3D の形状データ，マテリアル，テクスチャなどがテンプレートとして定義され， 
さらにユーザによって自由にテンプレートの定義の拡張を行うことができます．
\c Springhead2 のファイル入出力\c SDK では，この汎用的な \c X ファイルに対して，
ロードやセーブを行うことができます．（今後 \c VRML などもロードできるように拡張する予定です．）
\c X ファイルでは，\c Springhead2 独自の構文で自由にデータ型の定義を行えます．
ファイルの書式は，『 文法 』 と 『 ノードの定義 』 に分かれています．<br>


例えば，以下のように，『 文法 』 が決められています．<br>
『 ノードの定義 』（ノードの型名とそのノード型にどのようなフィールドがあるか）
については，『 文法 』 では決められていません．

\htmlonly
<table width=100% height="50" style="border:3px double #1841a5;">
<tr><td><tt>
　xof 0302txt 0064　　　　#　最初の行はこれから始まる 				<br>
 																	<br>
　#　　ノードは，													<br>
　#　　　　型名，ノード名 { フィールドの繰り返し   子ノード }		<br>
　#　　からなる．													<br>
　PHScene scene1{													<br>
　　　0.01;0;;　　　　　　#　フィールド は 値; の繰り返し			<br>
　　　1;0;-9.8;0;;　　　　#　値は 数値，文字列またはフィールド		<br>
　　　PHSolid soFloor{　　#　子ノードは，ノードと同じ				<br>
　　　　　(省略)													<br>
　　　}																<br>
　}																	<br>
</tt></td></tr></table>
\endhtmlonly

なお，\c X ファイルでは，コメントの開始記号として，\c C++ 形式のダブルスラッシュ \c "//" ，または特殊記号 \c "#" を使用します．
コメントの開始記号から行端までを，コメントと解釈します．

\htmlonly
<table width=100% height="50" style="border:3px double #1841a5;">
<tr><td><tt>
　　# This is a comment.　　<br>
　　// これもコメントです．
</tt></td></tr></table>
\endhtmlonly
<br><br>


<!------------------------------ ノードの定義 ------------------------------------------>
\section secNodeDef ノードの定義
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

\htmlonly
<table width=100% height="50" style="border:3px double #1841a5;">
<tr><td><tt>
　PHScene scene1{																			 <br>
　　　0.01;　　　　　　#　PHSceneState::timeStep											 <br>
　　　0;;　　　　　　　#　PHSceneState::count　　　　 最後の;はPHSceneState部の終わりを示す．<br>
　　　1;　　　　　　　 #　PHSceneDesc::ContactMode											 <br>
　　　0;-9.8;0;;　　　 #　PHSceneDesc::gravity　　　　最後の;はPHSceneDesc部の終わりを示す． <br>
　}																							 <br>
</tt></td></tr></table>
\endhtmlonly

のようになります．クラスのメンバ変数がそのままフィールドになります．
また，基本クラスは，先頭にフィールドが追加された形になります．

通常ノードの一覧は\ref pageNodeDefList を参照下さい．
<br><br><br>


<!------------------------------ 特殊ノード -------------------------------------------->
\section secSpecialNode 	特殊ノード
特殊ノードは，過去のファイルフォーマットとの互換性のためのノードです．
たとえば，\c DirectX の \c X ファイルにはもともと，

\htmlonly
<table width=100% height="50" style="border:3px double #1841a5;">
<tr><td><tt>
　Frame{																<br>
　　　　FrameTransfromMatrix{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1; }		<br>
　}																		<br>
</tt></td></tr></table>
\endhtmlonly

のようなフレームのノード型がありますが，\c Sprinhead2 には対応するディスクリプタやオブジェクトがありません．
そこで，これらは，\c GRFrame に変換されてロードされます．
このような，対応するオブジェクトがないノードが特殊ノードです．

特殊ノードの一覧は，\ref pageNodeDefList を参照下さい．
<br><br><br>


<!------------------------------ ノードのロード ---------------------------------------->
\section secLoadNode			ノードのロード
ディスクリプタに対応するノードは次の手順で生成されます．

<tt>
　　 1．ノードの型に対応するディスクリプタを生成する．						<br>
　　 2．ディスクリプタにノードのフィールドの内容を記入する．				<br>
　　 3．先祖ノードの CreateObject() API を用いて，オブジェクトを生成する．	<br>
　　 4．生成したオブジェクトに名前を設定する．								<br>
　　 5．生成したオブジェクトを親オブジェクトに追加する．					<br>
　　 6．子ノードをロードする．												<br>
</tt>
<br><br><br>


<!------------------------- ファイルのロードとセーブ ----------------------------------->
\section secFileIOLoadSave ファイルのロードとセーブ
ファイルのロードやセーブを行うには，まずFISdkを作成し，FISdkから，
FIFileXを作成します．次のような手順になります．

<tt>
　　 1．CreateFISdk() で，ISdkのインスタンス fiSdk を作成する．												<br>
　　 2．FISdk::CreateFileX() で FIFileX のインスタンス fileX を生成する．									<br>
　　 3．ObjectOfs objs を用意する．ロードしたノードのうち，ルートのノードがここに入る．						<br>
　　 4．FIFileX::Load() を呼び出して，ロードを行う．														<br>
　　 5．セーブするならば，セーブしたいオブジェクトだけを加えた objs を用意し，FIFileX::Save() を呼び出す．	<br>
</tt>
<br>

以下は，ファイルロードとセーブのコードのサンプルです．
\verbatim
	FISdkIf* fiSdk = CreateFISdk();	                           // FISdkの作成
	FIFileXIf* fileX = fiSdk->CreateFileX();                        // ファイルハンドル FIFileX の作成
	// オブジェクトの配列．
	// ここにファイルからロードしたオブジェクトが追加される．
	ObjectIfs objs;	
	if (! fileX->Load(objs, "test.x") ) {                           // Xファイルのロード
		DSTR << "Error: Cannot open load file. " << std::endl;
		exit(EXIT_FAILURE);
	}
	PHSdk* phSdk = NULL;
	for(unsigned  i=0; i<objs.size(); ++i){
		phSdk = DCAST(PHSdkIf, objs[i]);                       // ロードしたPHSdkを取り出す．
		if(phSdk) break;
	}
	objs.clear();
	objs.push_back(phSdk);
	fileX->Save(objs, "save.x");                                     // Xファイルのセーブ
\endverbatim
<br><br>


<!------------------------- 各データ型の記述 ------------------------------------------->
\section secDataFormat		各データ型の記述

\subsection subModelShape	モデル形状
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
　CDConvexMesh [ノード名] {																		<br>
　　　　頂点座標リスト;																			<br>
　　　　動摩擦摩擦係数; 静止摩擦係数; 跳ね返り係数;;　　# [物理シミュレーションに関係する材質]	<br>
　}																								<br>
</tt></td></tr></table>
\endhtmlonly

　次に示すのは，長さ \c 1 の立方体を，凸形状メッシュで定義した例です．
\htmlonly
<table width=100% height="50" style="border:3px double #1841a5;">
<tr><td><tt>
　CDConvexMesh meshBlock {																	<br>
　　　　0.5; 0.5; 0.5;, 0.5; 0.5;-0.5;, -0.5; 0.5;0.5;, -0.5; 0.5;-0.5;, 					<br>
　　　　0.5;-0.5; 0.5;, 0.5;-0.5;-0.5;, -0.5;-0.5;0.5;, -0.5;-0.5;-0.5;;					<br>
　　　　0.2; 0.5; 0.6;;　　　　　　　　　　　　　　　　　　　　　　　　　　# phmaterial	<br>
　}																							<br>
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
　CDSphere [ノード名] {																				<br>
　　　　半径;																						<br>
　　　　動摩擦摩擦係数; 静止摩擦係数; 跳ね返り係数;;　　　 # [物理シミュレーションに関係する材質]	<br>
　}																									<br>
</tt></td></tr></table>
\endhtmlonly

　次に示すのは，半径 \c 1 の球を定義した例です．
\htmlonly
<table width=100% height="50" style="border:3px double #1841a5;">
<tr><td><tt>
　CDSphere sphere {									<br>
　　　　1.0;　　　　　　　　　 //  radius			<br>
　　　　0.2; 0.5; 0.6;;　　　　//  phmaterial		<br>
　}													<br>
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
　CDBox [ノード名] {																				<br>
　　　　直方体サイズ;																				<br>
　　　　動摩擦摩擦係数; 静止摩擦係数; 跳ね返り係数;;　　　 # 物理シミュレーションに関係する材質	<br>
　}																									<br>
</tt></td></tr></table>
\endhtmlonly

　次に示すのは，\c X 方向のサイズが \c 1.0，Y 方向のサイズが \c 2.0，\c Z 方向のサイズが \c 3.0 の直方体を定義した例です．
\htmlonly
<table width=100% height="50" style="border:3px double #1841a5;">
<tr><td><tt>
  CDBox box {										<br>
　　　　1.0; 2.0; 3.0;;　　　　//  boxsize			<br>
　　　　0.2; 0.5; 0.6;;　　　　//  phmaterial		<br>
　}													<br>
</tt></td></tr></table>
\endhtmlonly
<br><br><br>

*/
