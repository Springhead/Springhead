## 実装

この節では、[対処法](/application/solutions.md) の実装に関する補足説明をします。

** ソースファイルの整合性 **

"CMakeLists.txt" に組み込みます
 ("CMakeSettings.txt" の`SPR_PROJS`で定義したプロジェクトのすべてを
`add_subdirectory` します)。
"CMakeLists.txt.Dev.dist" を参照のこと。

** ビルドの最適性 (無駄なコンパイル) **

組み込まれる Springhead Library の各プロジェクトに対して、
次のことを実施します (ここでは Base を例に説明します)。

cmake (configure) 実行時に作成されるオブジェクト格納ディレクトリ
 "C:/Develop/Application/*build*/Base/Base.dir" を、
オブジェクト共通格納場所
 "C/Springhead/core/src/Base/&lt;*platform*&gt;/&lt;*VS-Version*&gt;/Base.dir" への
 link (\*) にすげ替えます。

オブジェクト共通格納場所は、Springhead Library の cmake (configure) 時に作成します。
各プロジェクトの "CMakeLists.txt" から execute_process で呼び出される
 "make_prconfig.py" を参照のこと。

> (\*) ここで作成する link は、unix では symbolic link、Windows では junction です。
これは、Windows では通常の実行権限では symblic link が作成できないためです。

> Windows では、"Base.dir" が junction なのか通常のディレクトリなのかが、
 explorer でも command prompt でも区別がつきません。
このことが、[Q&A](/application/QandA.md#CrumbleBuildOptimization) の
 *ビルドの最適性が崩れる* の原因究明を困難にする可能性を孕んでいます。
ここでは、オブジェクト共通格納場所には "\_target\_body\_" という名前の空ファイルを
置くことで判定の補助としています。

** プロジェクトファイルの整合性 (Visual Studio の場合) **

アプリケーションのソリューションファイルに新たなターゲット sync を追加して、
次の処理を順に実行させます。
これにより、Springhead Library 側または他のアプリケーションが実施した変更は、
アプリケーションをビルドするだけで自動的に反映されます。

1. もしもアプリケーション側のプロジェクトファイルの内容と Springhead Library 側の
プロジェクトファイルの内容とが異なっていたならば、
アプリケーション側のプロジェクトファイルを Springhead Library 側にコピーする。
これは、アプリケーション側でソースファイル構成を変更 (追加・削除) を行ない、
Visual Studio 上でプロジェクトファイルを保存したとき、
またはアプリケーション側で再度 cmake を実行した場合に相当します。

1. Springhead Library 側のプロジェクトファイルをアプリケーション側にコピーする。

ターゲット sync は、アプリケーションの "CMakeLists.txt" で
 add_custom_target として作成されます。
また、このターゲットはアプリケーションのビルドにおいて
必ず最初に実行されるように依存関係が設定されます。
"CMakeLists.txt.Dev.dist" および "make\_sync.py" を参照のこと。

> 自アプリケーションで実施したソース構成の変更は、
ビルドを実施した時点で　Springhead Library 側に反映されます。
つまり、プロジェクトファイルの整合性を保つためには、
ソース構成の変更後に少なくとも1回はビルドを実行する必要があるということです
 ( sync の実行だけでもよい)。
しかしソースを変更したならばビルドするでしょうから、
このことが問題になることはないでしょう。

