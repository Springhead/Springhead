/**
\page pageGettingStarted	はじめてみよう


\contents


\section secRequirement		動作環境
本プログラムは以下いずれの環境でも動作するように書いています．
ただし，現在毎日動作確認をしている環境はVisual Studio 2005だけです．<br>
          <td width="64%"> 
            <table width="100%" border="0" cellspacing="1" cellpadding="2" bgcolor="#305C78">
              <tr> 
                <td> 
                  <table width="100%" border="0" cellspacing="1" cellpadding="1" bgcolor="#FFFFFF">
                    <tr> 
                      <td> 
                        <p><br> 
                        	&nbsp;&nbsp;&nbsp; \ref secVcInstall <br>
                        	&nbsp;&nbsp;&nbsp; \ref secCygwinInstall <br>
                        	&nbsp;&nbsp;&nbsp; \ref secLinuxInstall <br>
                        	&nbsp;&nbsp;&nbsp; \ref secMacInstall <br><br>
                        </p>
                      </td>
                    </tr>
                  </table>
                </td>
              </tr>
            </table>
          </td>
          <br><br>
<hr>



\section secDirectory		ディレクトリ構成
ディレクトリ構成は以下の通りです．
  \image html directory.jpg
<br><br>
<hr>



\section secInstall		インストールからビルドまで
ソースファイルからコンパイルする場合は，ソースファイルを適当な場所に置いて下さい．<br>
ここでは仮に \c C:\\Home\\C\\Project\\Springhead2 とします．<br>
下記にそれぞれの環境での構築方法を記載しています．
  
\subsection secVcInstall	Visual C++ .NET 2005 (+ DirectX SDK の予定)
\htmlonly
<strong>
<font color="gray">
　※　現時点ではDirectXには対応しておらずインストールは不要です．<br>
　　　(1-2)のインストール，および (1-4)の(v)，(vi)でのパスの設定は必要ありません．
</font>
</strong>
\endhtmlonly
<br>

\par 　　(1) 　 Visual C++ .NET 2005 のインストール
	<tt>Visual C++ .NET 2005</tt> をインストールして下さい．<br><br>

\par 　　(2) 　 DirectX SDK のインストール
	<tt>DirectX SDK </tt> をインストールして下さい．
	<tt>Microsoft</tt> のダウンロードサイトから手に入れることができます．
	インストール先は \c C:\\DXSDK として下さい．<br><br>

\par 　　(3) 　 環境変数の設定
	\c Springhead2 の環境変数を次の手順で設定して下さい．
	<ol type="i">
 		<li>「マイコンピュータを右クリック → 詳細設定タブ → 環境変数」を選択します．</li>
		<li> 環境変数画面で，システム環境変数（あるいはユーザ環境変数）の「新規」ボタンを押し，以下を設定します．</li>
  	</ol>
  	\image html setenv.jpg

\par 　　(4) 　 Visual C++ 開発環境の設定，OpenGL および DirectX へのパス設定
	<tt>Springhead2</tt> の開発を始める前に，<tt>Visual C++</tt> の環境設定を行う必要があります．
	この環境設定はプロジェクト毎に保存されるのではなく，<tt>Visual C++</tt> 開発環境の設定として
	保存されますので，一度行えば，今後行う必要はありません．
	また，<tt>DirectX SDK</tt> をインストールした段階で，自動的に，<tt>DirectX SDK</tt> へのパスが
	設定されますが，設定されない場合は以下の手順 ⅴ，ⅵ に従い，パスを設定して下さい．<br>
	<ol type="i">
		<li> <tt>Visual C++</tt> を起動します．</li>
		<li> メニューから <tt>[ツール]－[オプション]</tt> を選択して，オプションダイアログを開きます．</li>
		<li> <tt>[プロジェクト]－[VC++ディレクトリ]</tt> をクリックします．</li>
		<li> <tt>[ディレクトリを表示するプロジェクト]</tt> から <tt>[実行可能ファイル]</tt> を選択し，
			<tt>OpenGL</tt> のヘッダのあるディレクトリを追加します．
			 \code
　[実行可能ファイル]に追加　　　　　：$(SPRINGHEAD2)\bin\win32
		 	 \endcode
		 	 また，それ以外には，<tt> \$(SPRINGHEAD2)\\bin\\win32\\glut32.dll </tt> と \c glew32.dll を
			 \c C:\\Windows など，パスの通ったフォルダにコピーするか，
			 <tt> \$(SPRINGHEAD2)\\bin\\win32 </tt>を
			 環境変数 \c PATH に追加する方法もあります．
		</li>
		<li> 同様に， <tt>[ディレクトリを表示するプロジェクト]</tt> から <tt>[インクルードファイル]</tt>
			 を選択し，<tt>DirectX SDK</tt> のインクルードファイルへパスを通します．
			 \code
　[インクルードファイル]に追加　　　：C:\DXSDK\Include
			 \endcode
		</li>
		<li> 同様に， <tt>[ディレクトリを表示するプロジェクト]</tt> から 
			 <tt>[ライブラリファイル]</tt> を選択し，<tt>DirectX SDK</tt> のライブラリファイルへパスを通します．</tt>
			 \code
　[ライブラリファイル]に追加　　　　：C:\DXSDK\Lib
			 \endcode	   
		</li>
		\image html vcpath.jpg
		
	</ol>
		
\par 　　(5) 　ライブラリのビルド
<tt>\$(SPRINGHEAD2)\\src\\Sprignhead.sln</tt> を開き，ビルド(B) - バッチ ビルド(T) のダイアログで，
すべて選択(S) - ビルド(B)を選び，デバッグ版，リリース版のライブラリをビルドしてください．
<tt>\$(SPRINGHEAD2)\\lib\\win32\\SpringheadD.lib</tt> と <tt>\$(SPRINGHEAD2)\\lib\\win32\\Springhead.lib</tt> が生成されます．

\par 　　(6) 　サンプルのビルド
	実行したいサンプルプログラムを開き，メニューの <tt>[ビルド]－[ソリューションのビルド]</tt> を指定します．
<br>

\subsection secCygwinInstall		Cygwin
\par 　　(1) 　 Cygwin でビルド
ソースファイルのトップディレクトリにて，<tt>make</tt>コマンドを実行します．<br>
ライブラリの構築，およびテスト用プログラム，サンプルプログラムのコンパイルを自動的に行います．
\code
　% make
\endcode
<br>



\subsection secLinuxInstall		RedHat Linux 9
\par 　　(1) 　 RedHat Linux 9 でビルド
ソースファイルのトップディレクトリにて，<tt>make</tt>コマンドを実行します．<br>
ライブラリの構築，およびテスト用プログラム，サンプルプログラムのコンパイルを自動的に行います．
\code
　% make
\endcode
<br>



\subsection  secMacInstall		Mac OS X（PowerPC，Aquq）
\par 　　(1) 　 Mac OS X でビルド
ソースファイルのトップディレクトリにて，<tt>make</tt>コマンドを実行します．<br>
ライブラリの構築，およびテスト用プログラム，サンプルプログラムのコンパイルを自動的に行います．
\code
　% make
\endcode
<br>
<hr>



\section secSample	サンプルを動かす
サンプル・プログラムを動かしてみましょう．<br>
\c Springhead2 では，テスト用プログラム \c tests およびサンプルプログラム
\c Samples (\ref pageSamples) を用意しています．
ここでは例として，サンプルプログラム \c Samples の \ref pageFWAppSample
を実行する方法を紹介します．<br>

\subsection secVcTest			Visual C++ .NET 2005 ( + DirectX)
<ol>
	<li> \c FWAppSample ディレクトリにあるソリューションファイルを開きます．
	<li> ソリューションエクスプローラで，\c FWAppSample がスタートアッププロジェクトに
		 設定されていることを確認します．</li>
	<li> アプリケーションを実行します．</li>
</ol>
<br>

\subsection secCygwinTest		Cygwin
<ol>
	<li> \c FWAppSample ディレクトリへ移動し，\c FWAppSample.exe があることを確認します．</li>
	<li> 以下のコマンドを実行し，アプリケーションを起動します．<br>
 		\code
　% ./FWAppSample.exe
 		\endcode
	</li>
</ol>
<br>

\subsection secLinuxTest		RedHat Linux 9
<ol>
	<li> \c FWAppSample ディレクトリへ移動し，\c FWAppSample.exe があることを確認します．</li>
	<li> 以下のコマンドを実行し，アプリケーションを起動します．<br>
 		\code
　% ./FWAppSample.exe
 		\endcode
	</li>
</ol>
<br>

\subsection secMacTest			Mac OS X（PowerPC，Aquq）
<ol>
	<li> \c FWAppSample ディレクトリへ移動し，\c FWAppSample.exe があることを確認します．</li>
	<li> 以下のコマンドを実行し，アプリケーションを起動します．<br>
 		\code
　% ./FWAppSample.exe
 		\endcode
	</li>
</ol>
<br>


*/
