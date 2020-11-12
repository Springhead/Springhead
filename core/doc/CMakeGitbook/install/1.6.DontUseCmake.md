## CMakeを利用しない場合 (非推奨)

- - -
この章の冒頭でも述べたとおり、ここで説明する方法でビルドすることは推奨しません。
特に Windows の場合、Visual Studio の新しいバージョンに対応したファイルが
配布されることは期待しないでください。
また、これらのファイルはいずれ配布の対象から外される可能性がありますので、
できれば CMake を利用するビルド環境をご利用ください。
- - -

CMake を利用しないで直接ビルドをすることもできます。
現在配布しているファイルは次のもので、"C:/Springhead/core/src" に置かれています。

| ファイル名 ||
|:--|:--|
| Springhead14.0.sln | Windows Visual Studio 2015用 |
| Springhead15.0.sln | Windows Visual Studio 2017用 |
| Makefile | unix 用 |

** Windows の場合 **

Visual Studio を起動し、"スタートアッププロジェクト" に Springhead を指定して
ビルドしてください。
ライブラリファイルは、"C:/Springhead/generated/lib/*arch*" に生成されます
( *arch* は win64 または win32 のいずれかです)。

** unixの場合 **

`make install` を実行してください。
ライブラリファイルは、"C:/Springhead/generated/lib" に生成されます。

- - -
<a id="EmbPython"></a>
EmbPython 関係の配布ファイルは次のものです (unix 用の Makefile はありません)。

** Springhead アプリケーションに Python インタプリタを組み込む場合 **

Solution file が **"C:/Springhead/core/src/EmbPython"** に置かれています。

| Visual Studio 2017 用| Visual Studio 2015 用|
|:--|:--|
| EmbPython15.0.sln | EmbPython14.0.sln |

Visual Studio で上記の solution file を実行し、
ターゲット EmbPython をビルドしてください。
ライブラリファイルは、"C:/Springhead/core/src/EmbPython" に次の名前で生成されます
 (プラットフォームが 32 ビットの場合は x64 が x86 となります)。

| Visual Studio<br>バージョン | Debug 構成 | Release 構成 | Trace 構成 |
|:--:|:--:|:--:|:--:|
| 2015 | EmbPython14.0x64D.lib | EmbPython14.0x64.lib | EmbPython14.0x64T.lib |
| 2017 | EmbPython15.0x64D.lib | EmbPython15.0x64.lib | EmbPython15.0x64T.lib |

<br>
** Pythonインタプリタに対する外部拡張モジュール (Python DLL, pyd) を作成する場合 **

Solution file が **"C:/Springhead/core/embed"** に置かれています。

| Visual Studio 2017 用| Visual Studio 2015 用|
|:--|:--|
| SprPythonDLL15.0.sln | SprPythonDLL14.0.sln |

Visual Studio で上記の solution file を実行し、
ターゲット SprPythonDLL を ビルドしてください。
DLL ファイルは、"C:/Springhead/generated/bin/*arch*" に次の名前で生成されます
 (*arch* は win32 または win64 のいずれかです)。

| Debug 構成 | Release 構成 | Trace 構成 |
|:--:|:--:|:--:|
| SprD.pyd | Spr.pyd | SprT.pyd |

