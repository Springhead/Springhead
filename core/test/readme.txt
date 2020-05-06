

 - TestMain.batが行うこと - 

1. "Springhead/srcフォルダ"以下のファイルをすべて消去する

 ※※※※※※ 注意！！ ※※※※※※※※※※※※※※※※※※※※※※※※※※※
 ※ これによって、　　　　　　　　　　　　　　　                           ※
 ※ VSSに上がっていない自分のローカルにしかないファイルが消えてしまいます。※
 ※ 従って、ローカルで実行する際には十分注意するようにしてください。       ※
 ※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※

2. "bat\GetSpringhead.bat"を実行し、
   VSSから"Springheadフォルダ"以下のファイルを取得する。
   また、ログを"log\GetVSS.log"へ出力する。

3. "bat\TestAll.bat"を実行し、
   ビルドテスト及び、ビルド成功時の"Springheadフォルダ"へのラベル付けを行う。
   (ラベルには日時が、コメントにはビルドに成功した項目が付けられる)
   また、ビルドテストを行う項目を追加したい場合はこの"TestAll.bat"に追加する。
   詳しくは次節参照。


 - ビルドテストを行う項目を追加するには - 

bat\TestAll.bat(TestMain.batではないので注意!)の以下の２箇所に

1. bat\BuildMFC.bat (ビルドを行うdspファイルのあるディレクトリ) (dspファイル名)
      もしくは
   bat\BuildBCB.bat (ビルドを行うbpgファイルのあるディレクトリ) (bpgファイル名)

2.bat\Label.bat (exeファイルのあるディレクトリ) (exeファイル名)

をそれぞれ追加する

------ TestAll.bat ----------------------------------------------

			：
			：

rem **** ビルドテストを行う(テストを行うファイルは引数で指定) **** 
rem call bat\BuildMFC(BuildBCB).bat (引数1) (引数2) 
rem (引数1) : プロジェクトファイルの場所
rem (引数2) : プロジェクトファイル名(BuildMFC-.dspの前の部分, BuildBCB-.bpgの前の部分)

rem ----- この下にビルドを行うファイルを追加する -----
call bat\BuildMFC.bat Springhead\src\Samples\MFCD3DDyna MFCD3DDyna
call bat\BuildBCB.bat Springhead\src Springhead
														←←←ここに追加する
rem ----- ここまで -----

			：
			：

rem **** ラベル付け **** 
rem 上で行ったビルドが1つでも成功し、exeファイルができてたら$/Project/Springheadに成功ラベルを付ける
rem また、ビルドが成功したファイル名をコメントに羅列する 
rem call bat\Label.bat (引数1) (引数2)
rem (引数1) : exeファイルの場所
rem (引数2) : exeファイル名

rem ----- この下にビルドを行ったファイルを追加する -----
call bat\Label.bat Springhead\src\Samples\MFCD3DDyna\MFCDebug MFCD3DDyna 
call bat\Label.bat Springhead\src\BCBGui\Test Test
														←←←ここに追加する
rem ----- ここまで -----

------------------------------------------------------------------------------


 - ログについて - 
テストアプリを実行すると"Springhead\test\logフォルダ"にいろいろログが出力されます。通常はVSSに上がっているログを見ればいいと思います（1日1回更新）。

・Build.log
	ビルド結果

・BuildError.log
	Build.logでエラーに関係あると思われるところだけを出力

・History.log
	Springheadフォルダの履歴情報の最新50項目を出力


4. "bat\MakeDoc.bat"を実行し、
   doxygenを利用したドキュメントを作成する。
　 ドキュメントはsrc\html以下に作られる。

