/** \page pageFileIO ファイル入出力SDK

\ref pageFileIO は，\ref sec_SceneGraph のファイル入出力を行います．
他のSDKのAPI(\ref pagePhysics \ref pageGraphics)を用いて，
ファイルからロードしたデータを基にシーングラフの生成したり，
シーングラフのデータを読み出して，ファイルにデータを保存します．

\image html FileScene.png

上の図は，シーングラフとファイル入出力のイメージです．
ファイル上でのノードの親子関係とシーングラフの親子関係は必ず対応するわけではありません．

\section secXFile Xファイル
Springhead2のFileIOは，今のところDirectXのXファイルをロードすることができます．
（今後VRMLなどもロードできるように拡張する予定です．）
Xファイルなどのファイルの書式は，文法とノードの定義に分かれて定められています．
例えばXファイルならば，
<pre>
xof 0302txt 0064	#最初の行はこれから始まる
#	#か//の後はコメント
#ノードは，型名，ノード名 { フィールドの繰り返し   子ノード }からなる．
PHScene scene1{
	0.01;0;;			#	フィールド は 値; の繰り返し
	1;0;-9.8;0;;		#	値は 数値，文字列またはフィールド
	PHSolid soFloor{	#	子ノードは，ノードと同じ
		(省略)
	}
}
</pre>
のように，文法が決められています．
ノードの定義（ノードの型名とそのノード型にどのようなフィールドがあるか）
については，文法では決められていません．

\section secNodeDef ノードの定義
Springhead2の通常のノードは，オブジェクトのディスクリプタ(\ref secStateDesc を参照)に1対1対応します．
ロード時には，ディスクリプタに対応するオブジェクトが生成され，シーングラフに追加されます．
セーブ時には，オブジェクトからディスクリプタを読み出し，ノードの形式でファイルに保存されます．

オブジェクトのディスクリプタには，必ず対応するノードがあります．
例えば，SprPHScene.h には，
<pre>
struct PHSceneState{
	double timeStep;	///< 積分ステップ
	unsigned count;		///< 積分した回数
};
struct PHSolidDesc:PHSceneState{
	///	接触・拘束解決エンジンの種類
	enum SolverType{ SOLVER_PENALTY, SOLVER_CONSTRAINT} contactSolver;
	Vec3f gravity;	/// 重力加速度ベクトル．デフォルト値は(0.0f, -9.8f, 0.0f)．
};
</pre>
のように，ステートとディスクリプタが宣言されています．このPHSceneDescに対応するXファイルのノードは，
<pre>
PHScene scene1{
	0.01;			#	PHSceneState::timeStep
	0;;				#	PHSceneState::count			最後の;はPHSceneState部の終わりを示す．
	1;				#	PHSceneDesc::solverType
	0;-9.8;0;;		#	PHSceneDesc::gravity		Vec3fのあとに，gravityの終わりを示す;がある．
}
</pre>
のようになります．クラスのメンバ変数がそのままフィールドになります．
また，基本クラスは，先頭にフィールドが追加された形になります．

通常ノードの一覧は \ref pageNodeDefList を参照ください．

\section secSpecialNode 特殊ノード
特殊ノードは，過去のファイルフォーマットとの互換性のためのノードです．
たとえば，DirectXのXファイルにはもともと，
<pre>
Frame{
	FrameTransfromMatrix{ 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1; }
}
</pre>
のようなフレームのノード型がありますが，Sprinhead2には対応するディスクリプタやオブジェクトがありません．
そこで，これらは，GRFrameに変換されてロードされます．
このような，対応するオブジェクトがないノードが特殊ノードです．
特殊ノードの一覧は，\ref pageNodeDefList を参照ください．

\section secLoadNode ノードのロード
ディスクリプタに対応するノードは次の手順で生成されます．
<ol>
 <li> ノードの型に対応するディスクリプタを生成する．
 <li> ディスクリプタにノードのフィールドの内容を記入する．
 <li> 先祖ノードの CreateObject() APIを用いて，オブジェクトを生成する．
 <li> 生成したオブジェクトに名前を設定する．
 <li> 生成したオブジェクトを親オブジェクトに追加する．
 <li> 子ノードをロードする．
</ol>

\section secFileIOLoadSave ファイルのロードとセーブ

ファイルのロードやセーブを行うには，まずFISdkを作成し，FISdkから，
FIFileXを作成します．

以下は，ファイルロードとセーブのコードのサンプルです．
<pre>
	FISdkIf* fiSdk = CreateFISdk();				//	FISdkの作成
	FIFileXIf* fileX = fiSdk->CreateFileX();	//	FIFileXの作成
	/*	オブジェクトの配列．
		ここにファイルからロードしたオブジェクトが追加される．*/
	ObjectIfs objs;	
	if (! fileX->Load(objs, "test.x") ) {	//	ロード
		DSTR << "Error: Cannot open load file. " << std::endl;
		exit(EXIT_FAILURE);
	}
	PHSdk* phSdk = NULL;
	for(unsigned  i=0; i<objs.size(); ++i){
		phSdk = DCAST(PHSdkIf, objs[i]);	//	ロードしたPHSdkを取り出す．
		if(phSdk) break;
	}
	objs.clear();
	objs.push_back(phSdk);
	fileX->Save(objs, "save.x");
</pre>

*/
