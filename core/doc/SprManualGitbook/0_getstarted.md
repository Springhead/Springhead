Springheadをダウンロードしてから使えるようにするまでの流れを説明します．
## ダウンロード
Springheadのウェブサイト`http://springhead.info/wiki/`からzipアーカイブをダウンロードできます．ただし，アーカイブの更新は必ずしも頻繁ではありません (よくないことですが) ので，最新のコードが入手できない可能性があります．常に最新のコードを使用したい人は，次に説明するGitHubレポジトリからコードを入手してください．
## GitHubから入手する
SpringheadはGitHubを用いてバージョン管理を行っています．この文書の執筆時点でSpringheadのGitHubレポジトリは

`https://github.com/sprphys/Springhead`

です．レポジトリからのコードのダウンロード/クローンは任意のユーザが行えますが，コードをコミットするには開発者として登録されている必要があります．
## 開発環境
Springheadは処理系非依存の思想のもとで開発されています．このため，原理的にはWindows, macOS, unixなどの多くの処理系で動作するはずです．しかしながら，ほとんどの開発メンバーがWindows上のVisual Studioを用いて開発を行っているため，それ以外の環境で問題無く動作する保証は残念ながらありません (多分動かないでしょう)．したがって，現状ではユーザーにもWindows + Visual Studioという環境での使用を推奨します．WindowsやVisual Studioのバージョンについては，Windows 7/10, Visual Studio 2015/2017では問題なく動作します．
## ライブラリのビルド
以下では，Springheadを保存したディレクトリを`C:\Springhead`と仮定して話を進めます．Springheadを入手したら，まずライブラリをビルドします．ただし，サンプルプログラムをビルドする場合に限りここでの作業は不要です (ライブラリは自動的に作成されます)．まず，Visual Studioで以下のソリューションファイルを開いて下さい．

`C:\Springhead\core\src\Springhead15.0.sln`


\framebox{\small{%
\begin{minipage}{0.9\hsize}
【補足】 ファイル名末尾の数字は Visual Studio のバージョン番号を示しています．
その他のソリューションファイル，プロジェクトファイルも
同様の規則でナンバリングしてあります．
Visual Studio 2017 より以前の Visual Studioを使用する場合には適宜読み替えてください．
\end{minipage}
}}





\includegraphics[width=.6\hsize]{fig/libbuild.eps}

\caption{Building the library}


ソリューションを開いたらFig.\,\ref{fig_libbuild}に示すように`Springhead`プロジェクトをビルドしてください．ビルドに成功したら`C:\Springhead\generated\lib\win32\`または`C:\Springhead\generated\lib\win64\`ディレクトリにライブラリファイルが作成されるはずです．Table\,\ref{table_solution_config} に示すように，ビルドの設定ごとに異なるいくつかの構成が用意されています．ユーザアプリケーションの都合に合わせて使い分けてください．\begin{table}[t]\caption{Build configurations}

\begin{tabular}{lll}
\toprule
構成名	      & ビルド設定		& 作成されるライブラリファイル名 \\ \midrule
`Release` & multithread, DLL	& `Springhead\# \#.lib`	 \\
`Debug`   & multithread, Debug, DLL	& `Springhead\# \#D.lib`	 \\
`Trace`   & multithread, Debug, DLL	& `Springhead\# \#T.lib`	 \\ \bottomrule
\multicolumn{3}{l}
{\footnotesize{%
\vbox{\vbox to 1mm{}
    \hbox{・ `\# \#`はVisual Studioバージョンと
	  プラットフォームを表す`Win32`又は`x64`の組み合わせ}
    \hbox{\phantom{・ }となります("15.0x64" など)．}
    \hbox{・ `Trace`構成とは，フレームポインタ情報付き`Release`構成のことです．}}}}
\end{tabular}

\end{table}
















## サンプルプログラムのビルド
サンプルプログラムは，`C:\Springhead\core\src\Samples`以下にあります．残念なことですが，すべてのサンプルプログラムが問題なく動作する状態には維持されていません．`Physics\BoxStack`や`Physics\Joints`が比較的良くメンテナンスされていますので試してみてください．例えば，`Physics\BoxStack`をビルドするには，

`C:\Springhead\core\src\Samples\Physics\BoxStack\ `

に移動して`BoxStack15.0.sln`を開きます．`BoxStack`をスタートアッププロジェクトに設定し，ビルド，実行してください．実行時にDLLが見つからないためにエラーが起こるかもしれません。その場合には，32ビット環境ならば

`Springhead\core\bin\win32`, `Springhead\dependency\bin\win32`

64ビット環境ならば

`Springhead\core\bin\win64`，`Springhead\dependency\bin\win64`\\
`Springhead\core\bin\win32`，`Springhead\dependency\bin\win32`

のすべてにPathを通してください。
## アプリケーションの作成





\includegraphics[width=.6\hsize]{fig/newproject1.eps}

\caption{Create new project}







\includegraphics[width=.6\hsize]{fig/newproject2.eps}

\caption{Project configuration}







\includegraphics[width=.6\hsize]{fig/newproject3.eps}

\caption{Create source file}


Springheadを使って簡単なアプリケーションプログラムを作成する道筋を説明します．以下ではVisual Studio 2017を想定します．
### プロジェクトの作成
「Visual C++ Windows >  デスクトップ ウィザード」を作成します．作成するディレクトリを `C:\Experiments` と仮定します．他のディレクトリに作成する場合には，プロジェクトに指定するインクルードファイル及びライブラリファイルのパスが，保存したSpringheadを正しく参照するように注意してください．プロジェクト名は好きな名前を付けてください．アプリケーションの設定で「コンソールアプリケーション」を選び，空のプロジェクトをチェックします．
\framebox{\small{%
\begin{minipage}{0.9\hsize}
【補足】 Visual Studio 2015を使用した場合、
Fig.\,\ref{fig_newproject1}、Fig.\,\ref{fig_newproject2}とは
異なるウィンドウが表示されます。
その場合には「Win32プロジェクト >  次へ」と進んで、
「コンソールアプリケーション」、「空のプロジェクト」をチェックします。
\end{minipage}
}}
プロジェクトを作成したら「プロジェクト >  新しい項目の追加 >  C++ファイル(.cpp)」としてソースファイルを作成します．ここでは仮に`main.cpp`とします．
### ソースコードの編集
\begin{table}[t]\caption{Simplest program code}{\small
```
#include <Springhead.h>
#include <Framework/SprFWApp.h>
using namespace Spr;

class MyApp : public FWApp{
public:
    virtual void Init(int argc = 0, char* argv[] = 0){
        FWApp::Init(argc, argv);

        PHSdkIf* phSdk = GetSdk()->GetPHSdk();
        PHSceneIf* phscene = GetSdk()->GetScene()->GetPHScene();
        CDBoxDesc bd;
        
        // 床を作成
        PHSolidIf* floor = phscene->CreateSolid();
        floor->SetDynamical(false);
        bd.boxsize = Vec3f(5.0f, 1.0f, 5.0f);
        floor->AddShape(phSdk->CreateShape(bd));
        floor->SetFramePosition(Vec3d(0, -1.0, 0));
    
        // 箱を作成
        PHSolidIf* box = phscene->CreateSolid();
        bd.boxsize = Vec3f(0.2f, 0.2f, 0.2f);
        box->AddShape(phSdk->CreateShape(bd));
        box->SetFramePosition(Vec3d(0.0, 1.0, 0.0));

        GetSdk()->SetDebugMode(true);
    }
} app;

int main(int argc, char* argv[]){
    app.Init(argc, argv);
    app.StartMainLoop();
    return 0;
}
```
}\end{table}作成した`main.cpp`にTable\,\ref{table_simplest_code}に示すコードを書き込んでください．これがSpringheadを使用した (ほぼ) 最小のプログラムコードです．
## プロジェクト設定





\includegraphics[width=.6\hsize]{fig/newproject4.eps}

\caption{Add include path}







\includegraphics[width=.6\hsize]{fig/newproject5.eps}

\caption{Add library path}







\begin{tabular}{c}
\includegraphics[width=.6\hsize]{fig/newproject6_truncate.eps} \\
\includegraphics[width=.6\hsize]{fig/newproject7_truncate.eps}
\end{tabular}

\caption{Specify library file}







\includegraphics[width=.6\hsize]{fig/newproject8.eps}

\caption{Program running}


ビルドするまえにいくつかのプロジェクト設定が必要です．64ビットプラットフォームを使用する場合には，プロパティーページの「構成マネージャー」で「`x64`」プラットフォームを新規作成して選択しておきます．また，\ref{libbuild} で説明したライブラリのビルドは済んでいるものとします．まずプロジェクトのプロパティページを開き，構成を「すべての構成」としてください．次に「C/C++ >  全般 >  追加のインクルードディレクトリ」に，Fig.\,\ref{fig_newproject4}のようにSpringheadのインクルードファイルへのパスを指定してください．さらに，「リンカー >  全般 >  追加のライブラリディレクトリ」にFig.\,\ref{fig_newproject5}のようにSpringheadのライブラリファイルへのパスを指定します(64ビット構成の場合は`win32`の代わりに`win64`を指定します)．今度は構成を「Debug」にします．「C/C++ >  コード生成 >  ランタイムライブラリ」を「マルチスレッド デバッグ DLL(`/MDd`)」にします．次に「リンカー >  入力 >  追加の依存ファイル」に`Springhead15.0DWin32.lib` (または`Springhead15.0Dx64.lib`)を追加してください．最後に構成を「Release」に切り替えて同様の設定をします．ランタイムライブラリを「マルチスレッド DLL(`/MD`)」として，追加の依存ファイルに`Springhead15.0Win32.lib` (または`Springhead15.0Dx64.lib`)を追加します．
## ビルド・実行
以上で準備完了です．ビルド(F7)して，実行(F5)してみてください．Fig.\,\ref{fig_newproject8}のような画面が出てくれば成功です．
