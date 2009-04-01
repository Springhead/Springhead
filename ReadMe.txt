Springhead2					2009年2月 長谷川晶一

◆インストール
１．svnからとってくる．
	http://springhead.info/wiki の「Subversionレポジトリの使い方」のページ
	を参考に．
２．環境変数の設定
	環境変数 Springhead2 にこのフォルダのフルパスを指定する．
	[マイ コンピュータ]を右クリック→[プロパティ]→[詳細設定]ページ→[環境変数]
	→[新規]→ 変数名：	Springhead2		変数値:	C:\Home\svn\Project\Springhead2 など
３．src/samples/ 以下にサンプルの.sln があるので，ビルド→実行してみる．

◆ライセンスについて
BSDライセンスです。

INRIAのSciLabと接続する部分はINRIAのライセンスになります．
商用利用の場合，ソースコードを公開したくない場合は，SciLabとの接続部分
(include/SciLab, src/ScilabStub.cpp) を削除してください．
