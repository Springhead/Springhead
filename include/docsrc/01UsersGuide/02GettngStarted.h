/**
\page pageGettingStarted	とりあえず使ってみよう



\section contentsGettingStarted	◇ Contents ◇
 &nbsp;&nbsp;&nbsp; \ref requirement <br>
 &nbsp;&nbsp;&nbsp; \ref directory   <br>
 &nbsp;&nbsp;&nbsp; \ref install     <br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \ref vc_install <br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \ref cygwin_install <br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \ref linux_install <br>
 &nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \ref mac_install <br>
 &nbsp;&nbsp;&nbsp; \ref test <br>
<br><br>
<hr>



\section requirement	1. 動作環境
本プログラムは以下いずれかの環境で動作します。<br>
この環境は動作を確認したものです。他の環境でも動作する場合もあります。</li>
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



\section directory	2. ディレクトリ構成
ディレクトリ構成は以下の通りです。
  <img src="../../include/docsrc/01UsersGuide/directory.jpg" width="486" height="351">
<br><br>
<hr>



\section install	3. インストールからビルドまで
ソースファイルからコンパイルする場合は、ソースファイルを適当な場所に置いて下さい。<br>
ここでは仮に C:￥Home￥C￥Project￥Springhead2とします。<br>
下記にそれぞれの環境での構築方法を記載しています。
 
\subsection vc_install	(１) Visual C++ .NET 2003 + DirectX SDK 9.0
<ol>
   <li><strong>Visual C++ .NET 2003 のインストール </strong><br>
  				 Visual C++ .NET 2003 をインストールして下さい。<br>
   </li><br>
<!--
	現時点ではDirectXには対応しておらずインストールは不要です．<br>
   <li><strong> DirectX SDK 9.0 のインストール </strong><br>
  				 DirectX SDK 9.0 をインストールして下さい。<br>
  				 Microsoftのダウンロードサイトから手に入れることができます。<br>
  				 インストール先は C:￥DXSDK として下さい。<br>
   </li><br>
-->
   <li><strong> 環境変数の設定 </strong><br>
  				 <ol type="i">
  				    <li>「マイコンピュータを右クリック → 詳細設定タブ → 環境変数」を選択します。</li>
  				    <li> 環境変数画面で、システム環境変数(あるいはユーザ環境変数)の「新規」ボタンを押し、以下を設定する。</li>
  				       <ul type="circle">
  				         <li>&nbsp; 変数名 ： SPRINGHEAD2 </li>
  				         <li>&nbsp; 変数値 ： C:￥Home￥C￥Project￥Springhead2 </li>
  				       </ul>
  				    </li>
  				 </ol>
  				 <img src="../../include/docsrc/01UsersGuide/setenv.jpg" width="423" height="154"><br>
   </li>
   <li><strong> GLUTの設定 </strong><br>
				\$(SPRINGHEAD2)/bin/win32/glut32.dll をC:\Windows など
				PATHの通ったフォルダにコピーするか，
				\$(SPRINGHEAD2)/bin/win32 を環境変数PATHに追加してください．
   </li>
   <li><strong> Visual C++ 開発環境の設定 </strong><br>
                Springhead2の開発を始める前に、Visual C++ の環境設定を行う必要があります。
				この環境設定はプロジェクト毎に保存されるのではなく、Visual C++ 開発環境の設定として保存されますので、
				一度行えば、今後行えば今後行う必要はありません。 また、DirectX SDKをインストールした段階で、自動的に、
				DirectX SDK へのパスが設定されますが、設定されない場合は以下の手順(vi)、(vii)に従い、パスを設定して下さい。
				<ol type="i">
				   <li> Visual C++ を起動します。</li>
				   <li> メニューから[ツール]－[オプション]を選択して、オプションダイアログを開きます。</li>
				   <li> [プロジェクト]－[VC++ディレクトリ]をクリックします。</li>
				   <li> [ディレクトリを表示するプロジェクト]から[実行可能ファイル]を選択します。</li>
				   <li> [新しい行]ボタンを押して、OpenGL のヘッダのあるディレクトリ ＄(Springhead2)￥bin￥win32 を追加します。</li>
				   <li> 同様に、 [ディレクトリを表示するプロジェクト]から[インクルードファイル]を選択し、C:￥DXSDK￥Include を追加します。</li>
				   <li> 同様に、 [ディレクトリを表示するプロジェクト]から[ライブラリファイル]を選択し、C:￥DXSDK￥Lib を追加します。</li>
					<img src="../../include/docsrc/01UsersGuide/vcpath.jpg" width="709" height="372"><br>
				</ol>
   </li><br>
   <li><strong>ビルド </strong><br>
   				実行したいサンプルプログラムを開き、メニューの[ビルド]－[ソリューションのビルド]を指定します。
   </li><br>
</ol>
 
\subsection cygwin_install	(２) Cygwin
<ol>
 	<li>ソースファイルのトップディレクトリにて、makeコマンドを実行します。<br>
 		ライブラリの構築、およびテスト用プログラム、サンプルプログラムのコンパイルを自動的に行います。<br>
 		&nbsp;&nbsp;&nbsp; % make
 	</li>			
</ol>

\subsection linux_install	(３) RedHat Linux 9
<ol>
 	<li>ソースファイルのトップディレクトリにて、makeコマンドを実行します。<br>
 		ライブラリの構築、およびテスト用プログラム、サンプルプログラムのコンパイルを自動的に行います。<br>
 		&nbsp;&nbsp;&nbsp; % make
 	</li>			
</ol>

\subsection mac_install	(４) Mac OS X（PowerPC、Aquq）
<ol>
 	<li>ソースファイルのトップディレクトリにて、makeコマンドを実行します。<br>
 		ライブラリの構築、およびテスト用プログラム、サンプルプログラムのコンパイルを自動的に行います。<br>
 		&nbsp;&nbsp;&nbsp; % make
 	</li>			
</ol>
<br>
<hr>


\section test	4. サンプルを動かす
サンプル・プログラムを動かしてみましょう。<br>
動作可能なテスト用プログラムおよびサンプルプログラムは以下のディレクトリにあります（参照 \ref directory)。
<ul type="circle"><li> Springhead2￥src￥tests
				  <li> Springhead2￥src￥Samples</ul> 
ここでは、例として、Springhead2￥src￥tests￥Graphics￥GRSimple を実行する方法を示します。<br>

\subsection vc_test	(１) Visual C++ .NET 2003 + DirectX SDK 9.0
<ol>
	<li> GRSimpleディレクトリにあるソリューションファイルを開きます。
	<li> ソリューションエクスプローラで、GRSimple がスタートアッププロジェクトに設定されていることを確認します。</li>
	<li> アプリケーションを実行します。</li>
</ol>

\subsection cygwin_test	(２) Cygwin
<ol>
	<li> GRSimple ディレクトリへ移動し、GRSimple.exe があることを確認します。</li>
	<li> 以下のコマンドを実行し、アプリケーションを起動します。<br>
		 &nbsp;&nbsp;&nbsp; %  ．/GRSimple.exe
	</li>
</ol>

\subsection linux_test	(３) RedHat Linux 9
<ol>
	<li> GRSimple ディレクトリへ移動し、GRSimple.exe があることを確認します。</li>
	<li> 以下のコマンドを実行し、アプリケーションを起動します。<br>
		 &nbsp;&nbsp;&nbsp; %  ．/GRSimple.exe
	</li>
</ol>

\subsection mac_test	(４) Mac OS X（PowerPC、Aquq）
<ol>
	<li> GRSimple ディレクトリへ移動し、GRSimple.exe があることを確認します。</li>
	<li> 以下のコマンドを実行し、アプリケーションを起動します。<br>
		 &nbsp;&nbsp;&nbsp; %  ．/GRSimple.exe
	</li>
</ol>
<br>


*/
