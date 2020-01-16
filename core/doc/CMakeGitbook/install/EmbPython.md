## EmbPython

Python インタプリタに対する外部拡張モジュールを生成する 方法について説明します。

- - -
申し訳ありませんが、今のところ、unix 版および Windows CMake 版は
未だ準備ができておりません。しばらくお待ちください。
- - -

Windows visual Studio 用の solution file が "C:/Springhead/core/embed" に
置かれています。

| ファイル名 ||
|:--|:--|
| SprPythonDLL14.0sln | Visual Studio 2015 用 |
| SprPythonDLL15.0sln | Visual Studio 2017 用 |

Visual Studio で上記の solution file を実行し、ターゲット SprPythonDLL を
ビルドしてください。
DLL ファイルは、"C:/Springhead/generated/bin/&lt;*arch*&gt;" に次の名前で生成されます。

| ファイル名 | 構成 |
|:--|:--|
| SprD.pyd | Debug   構成 |
| Spr.pyd  | Release 構成 |
| SprT.pyd | Trace   構成 |

