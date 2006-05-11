/**
\page pageGettingStarted	とりあえず使ってみよう

\section contentsGettingStarted	◇ Contents ◇
　　 \ref requirement				<br>
　　 \ref directory					<br>
　　 \ref install					<br>
　　　　　 \ref vc_install			<br>
　　　　　 \ref cygwin_install		<br>
　　　　　 \ref linux_install		<br>
　　　　　 \ref mac_install			<br>
　　 \ref test 						<br>
　　　　　 \ref vc_test				<br>
　　　　　 \ref cygwin_test			<br>
　　　　　 \ref linux_test			<br>
　　　　　 \ref mac_install			<br>
<br><br>
<hr>



\section requirement	1．　動作環境
本プログラムは以下いずれかの環境で動作します．<br>
この環境は動作を確認したものです．他の環境でも動作する場合もあります．</li>
          <td width="64%"> 
            <table width="100%" border="0" cellspacing="1" cellpadding="2" bgcolor="#305C78">
              <tr> 
                <td> 
                  <table width="100%" border="0" cellspacing="1" cellpadding="1" bgcolor="#FFFFFF">
                    <tr> 
                      <td> 
                        <p><br> 
                        	&nbsp;&nbsp;&nbsp; \ref vc_install <br>
                        	&nbsp;&nbsp;&nbsp; \ref cygwin_install <br>
                        	&nbsp;&nbsp;&nbsp; \ref linux_install <br>
                        	&nbsp;&nbsp;&nbsp; \ref mac_install <br><br>
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



\section directory	2．　ディレクトリ構成
ディレクトリ構成は以下の通りです．
  \image html directory.jpg
<br><br>
<hr>



\section install	3．　インストールからビルドまで
ソースファイルからコンパイルする場合は、ソースファイルを適当な場所に置いて下さい．<br>
ここでは仮に C:/Home/C/Project/Springhead2 とします．<br>
下記にそれぞれの環境での構築方法を記載しています．
  
\subsection vc_install	(１) Visual C++ .NET 2003 + DirectX SDK 9.0
\htmlonly
<strong>
<font color="gray">
　※　現時点ではDirectXには対応しておらずインストールは不要です．<br>
　　　(1-2)のインストール、および (1-4)の(v)、(vi)でのパスの設定は必要ありません．
</font>
</strong>
\endhtmlonly
<br>

\par 　　(1-1) 　 Visual C++ .NET 2003 のインストール
	Visual C++ .NET 2003 をインストールして下さい．					<br><br>

\par 　　(1-2) 　 DirectX SDK 9.0 のインストール
	DirectX SDK 9.0 をインストールして下さい．							
	Microsoft のダウンロードサイトから手に入れることができます．		
	インストール先は C:/DXSDK として下さい．						<br><br>

\par 　　(1-3) 　 環境変数の設定
	Springhead2 の環境変数を設定して下さい．
	<ol type="i">
 		<li>「マイコンピュータを右クリック → 詳細設定タブ → 環境変数」を選択します．</li>
  		<li> 環境変数画面で、システム環境変数(あるいはユーザ環境変数)の「新規」ボタンを押し、以下を設定します．</li>
  	</ol>
  	\image html setenv.jpg

\par 　　(1-4) 　 Visual C++ 開発環境の設定、OpenGL および DirectX へのパス設定
	Springhead2 の開発を始める前に、Visual C++ の環境設定を行う必要があります．							
	この環境設定はプロジェクト毎に保存されるのではなく、Visual C++ 開発環境の設定として保存されますので、
	一度行えば、今後行えば今後行う必要はありません． 													
	また、DirectX SDK をインストールした段階で、自動的に、DirectX SDK へのパスが設定されますが、
	設定されない場合は以下の手順 (vi)、(vii) に従い、パスを設定して下さい．<br>
	<ol type="i">
		<li> Visual C++ を起動します．</li>
		<li> メニューから [ツール]－[オプション] を選択して、オプションダイアログを開きます．</li>
		<li> [プロジェクト]－[VC++ディレクトリ] をクリックします．</li>
		<li> [ディレクトリを表示するプロジェクト] から [実行可能ファイル] を選択し、OpenGL のヘッダのあるディレクトリを追加します．
			 \code
　[実行可能ファイル]に追加　　　　　：$(SPRINGHEAD2)\bin\win32
		 	 \endcode
			 また、それ以外には、\$(SPRINGHEAD2)/bin/win32/glut32.dll をC:/Windows など、
			 PATH の通ったフォルダにコピーするか，\$(SPRINGHEAD2)/bin/win32 を環境変数 PATH に追加する方法もあります．
		</li>
		<li> 同様に、 [ディレクトリを表示するプロジェクト] から [インクルードファイル] を選択し、
	         DirectX SDK のインクルードファイルへパスを通します．
			 \code
　[インクルードファイル]に追加　　　：C:\DXSDK\Include
			 \endcode
		</li>
		<li> 同様に、 [ディレクトリを表示するプロジェクト] から [ライブラリファイル] を選択し、
		   	 DirectX SDK のライブラリファイルへパスを通します．
			 \code
　[ライブラリファイル]に追加　　　　：C:\DXSDK\Lib
			 \endcode	   
		</li>
		\image html vcpath.jpg
	</ol>
	
\par 　　(1-5) 　 VC でビルド
	実行したいサンプルプログラムを開き、メニューの [ビルド]－[ソリューションのビルド] を指定します．	

<br>



\subsection cygwin_install	(２) Cygwin
\par 　　(2-1) 　 Cygwin でビルド
	ソースファイルのトップディレクトリにて、makeコマンドを実行します．<br>
	ライブラリの構築、およびテスト用プログラム、サンプルプログラムのコンパイルを自動的に行います．<br>
	\code
　% make
	\endcode

<br>



\subsection linux_install	(３) RedHat Linux 9
\par 　　(3-1) 　 RedHat Linux 9 でビルド
	ソースファイルのトップディレクトリにて、makeコマンドを実行します．<br>
 	ライブラリの構築、およびテスト用プログラム、サンプルプログラムのコンパイルを自動的に行います．<br>
	\code
　% make
	\endcode

<br>



\subsection mac_install	(４) Mac OS X（PowerPC、Aquq）
\par 　　(4-1) 　 Mac OS X でビルド
	ソースファイルのトップディレクトリにて、makeコマンドを実行します．<br>
 	ライブラリの構築、およびテスト用プログラム、サンプルプログラムのコンパイルを自動的に行います．<br>
 	\code
　% make
 	\endcode

<br>
<hr>



\section test	4．　サンプルを動かす
サンプル・プログラムを動かしてみましょう．<br>
Springhead2では、動作可能なテスト用プログラム tests およびサンプルプログラム Samples を用意しています（参照 ： \ref directory)．
ここでは例として、テスト用プログラム tests の
<a href="../../src/html/GRSimple_2main_8cpp-example.html">GRSimple</a>
を実行する方法を紹介します．<br>

\subsection vc_test	(１) Visual C++ .NET 2003 + DirectX SDK 9.0
<ol>
	<li> GRSimpleディレクトリにあるソリューションファイルを開きます．
	<li> ソリューションエクスプローラで、GRSimple がスタートアッププロジェクトに設定されていることを確認します．</li>
	<li> アプリケーションを実行します．</li>
</ol>
<br>

\subsection cygwin_test	(２) Cygwin
<ol>
	<li> GRSimple ディレクトリへ移動し、GRSimple.exe があることを確認します．</li>
	<li> 以下のコマンドを実行し、アプリケーションを起動します．<br>
 		\code
　% ./GRSimple.exe
 		\endcode
	</li>
</ol>
<br>

\subsection linux_test	(３) RedHat Linux 9
<ol>
	<li> GRSimple ディレクトリへ移動し、GRSimple.exe があることを確認します．</li>
	<li> 以下のコマンドを実行し、アプリケーションを起動します．<br>
 		\code
　% ./GRSimple.exe
 		\endcode
	</li>
</ol>
<br>

\subsection mac_test	(４) Mac OS X（PowerPC、Aquq）
<ol>
	<li> GRSimple ディレクトリへ移動し、GRSimple.exe があることを確認します．</li>
	<li> 以下のコマンドを実行し、アプリケーションを起動します．<br>
 		\code
　% ./GRSimple.exe
 		\endcode
	</li>
</ol>
<br>


*/
