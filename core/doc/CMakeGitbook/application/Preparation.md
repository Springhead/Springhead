## ビルドの準備

"C:/Develop/Application" に移動してください。

** ファイルのコピー **

配布されたファイル "CMakeTopdir.txt.dist", "CMakeLists.txt.Dev.dist",
 "CMakeSettings.txt.dist" を、それぞれ "CMakeTopdir.txt",
 "CMakeLists.txt", "CMakeSettings.txt" という名前でコピーします。

```
> chdir C:/Application
> copy C:/Springhead/core/src/CMakeTopdir.txt.dist CMakeTopdir.txt
> copy C:/Springhead/core/src/CMakeLists.txt.Dev.dist CMakeLists.txt
> copy C:/Springhead/core/src/CMakeSettings.txt.dist CMakeSettings.txt
```

> .dist ファイルが誤って変更されてしまうのを防ぐためにも、
リネームではなくコピーをお願いします。

** "CMakeTopdir.txt" の編集 **

Springhead Library をダウンロードしたディレクトリを "CMakeTopdir.txt" に設定します。
これは、CMake に Springhead のソースツリーの場所を教えるため
 (Library のソースを `add_subdirectory` するため) に必要な設定です。

```
#set(TOPDIR "C:/Springhead")
　↓
set(TOPDIR "C:/Springhead")
```

** "CMakeSettings.txt" の編集 **

アプリケーションのビルド条件を設定します。
各変数の意味は次のとおりです。

| 変数名 | 説明 |
|:--|:--|
| `ProjectName` | プロジェクト名 |
| `OOS_BLD_DIR` | CMake の作業領域 (ディレクトリ) の名前 (本ドキュメントで *build* としているもの) |
| `CMAKE_CONFIGURATION_TYPES` | ビルド構成<br>unix の場合はここに複数の構成を指定することができません。作業ディレクトリを分けることで対処してください (`OOS_BLD_DIR` 参照)。 |
| `LIBTYPE` | 作成するライブラリの種別<br>Windows の場合は`STATIC`を指定してください。 unix の場合は、`STATIC`なら`.a`を、`SHARED`なら`.so`を作成します。 |
| `SRCS` | ビルドの対象とするファイル<br>設定は`set(SRCS …)`または`file(GLOB SRCS …)`とします。後者ではワイルドカードが使えます。<br>`SRCS`の直後に`RELATIVE <base-dir>`を付加すると`<base-dir>`に対する相対パスとなります。デフォルトは`file(GLOB RELATIVE ${CMAKE_SORUCE_DIR} *.cpp *.h)`です。|
| `EXECLUDE_SRCS` | ビルドの対象から外すファイル<br>`SRCS`でワイルドカードを使用した場合に有用です。<br>上の`SRCS`で`RELATIVE`としていないときは絶対パスで指定します。 |
| `SPR_PROJS` | アプリケーションに組み込む Springhead Library のプロジェクト名 (この中に RunSwig を含めてはいけません)<br>unix で "libSpringhead.a" をリンクするときは`${EMPTY}`のままとします。 |
| `ADDITIONAL_INCDIR` | 追加のインクルードパス指定<br>現在のディレクトリは`${CMAKE_SOURCE_DIR}`で参照できます。 |
| `ADDITIONAL_LIBDIR` | 追加のライブラリパス指定 |
| `ADDITIONAL_LIBS` | 追加のライブラリファイル名 |
| `EXCLUDE_LIBS` | リンクの対象から外すライブラリファイル名<br>デフォルトで組み込まれてしまうライブラリファイルを排除するために使用します。 |
| `DEBUGGER_WORKING_DIRECTORY` | Visual Studio Debugger の作業ディレクトリ名<br>デバッガはこのディレクトリで起動されたように振る舞います。 |
| `DEBUGGER_COMMAND_ARGUMENTS` | Visual Studio Debugger に渡すコマンド引数 |
| `WIN_COPT_COMMON_APPEND`<br>`WIN_COPT_DEBUG_APPEND`<br>`WIN_COPT_RELEASE_APPEND`<br>`WIN_COPT_TRACE_APPEND` | 追加コンパイルオプション (Windows 用)<br>デフォルトオプションの後に追加<br>"CMakeOpts.txt.dist" 参照 (以下同様) |
| `WIN_LINK_COMMON_APPEND`<br>`WIN_LINK_DEBUG_APPEND`<br>`WIN_LINK_RELEASE_APPEND`<br>`WIN_LINK_TRACE_APPEND` | 追加リンクオプション (Windows 用)<br>デフォルトオプションの後に追加 |
| `LINUX_INCDIRS_PREPEND`<br>`LINUX_INCDIRS_APPEND`<br>`LINUX_COPT_FLAGS_PREPEND`<br>`LINUX_COPT_MACROS_APPEND` | 追加コンパイルオプション (Linux 用)<br>デフォルトオプションの前 / 後に追加 |
| `LINUX_LDFLAGS_PREPEND`<br>`LINUX_LDFLAGS_APPEND` | 追加リンクオプション (Linux 用)<br>デフォルトオプションの前 / 後に追加 |

別途インストールしているパッケージ boost, glew, glut, glui を使用する場合には、配布されたファイル "CMakeConf.txt.dist" を "CMakeConf.txt" という名前でコピーして必要な編集をします。

ビルドの条件 (コンパイル / リンク) を変更したいときは、配布されたファイル "CMakeOpts.txt.dist" を "CMakeOpts.txt" という名前でコピーして必要な編集をします。

**ファイル "CMakeLists.txt" の変更は必要ありません。**

以上で準備作業は終了です。

