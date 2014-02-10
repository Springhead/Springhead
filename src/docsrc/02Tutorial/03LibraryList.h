/** \page pageLibraryList	 ライブラリ

<br>

各ライブラリは，作成したプログラムをビルドする時にリンクすることで利用可能となります（\ref directory参照）．
<ul type="circle">
	<li> Baseライブラリ <br>
			<dl>
				<dt></dt>
					<dd>基本ユーティリティとして，Baseライブラリは，アフィン行列，ベクトル，
						デバッグ支援機能，コンパイラマクロ，参照カウンタ，
						その他ユーティリティから構成されます．</dd>
			</dl>
	</li><br>
	<li> Collisionライブラリ <br>
			<dl>
				<dt></dt>
					<dd>衝突判定エンジンとして，衝突検出を行います．物体同士の相互作用に不可欠です．</dd>
			</dl>
	</li><br>
	<li> FileIOライブラリ <br>
			<dl>
				<dt></dt>
					<dd>3D形状データ，マテリアル，テクスチャなどを定義したXファイルの
						データ情報をロードします．</dd>
			</dl>
	</li><br>
	<li> Foundationライブラリ <br>
			<dl>
				<dt></dt>
					<dd>シーンとオブジェクトの構築を行います．</dd>
			</dl>
	</li><br>
	<li> Graphicsライブラリ <br>
			<dl>
				<dt></dt>
					<dd> 補助ライブラリとしてGraphicsライブラリを用意しています．
						 レンダリングのデバイスはOpenGLまたはDirectXで接続切換え可能です．
						 ただし，今の段階ではDirectXは未実装となっており，OpenGLデバイスのみ利用可能です．</dd>
			</dl>
	</li><br>
	<li> Physicsライブラリ <br>
			<dl>
				<dt></dt>
					<dd>Springhead2で核となる3D物理シミュレーションエンジンです．</dd>
			</dl>
	</li><br>
</ul>	
<br>

*/
