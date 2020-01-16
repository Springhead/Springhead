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
| Springhead14.0sln | Visual Studio 2015用 |
| Springhead15.0sln | Visual Studio 2017用 |
| Makefile | unix 用 |

**Windows の場合**

Visual Studio を起動し、"スタートアッププロジェクト" に Springhead を指定して
ビルドしてください。
ライブラリファイルは、"C:/Springhead/generated/lib/&lt;*arch*&gt;" に生成されます。
ここで &lt;*arch*&gt; は "win64" または "win32" のいずれかです。

**unixの場合**

"install" をターゲットとして make してください。
ライブラリファイルは、"C:/Springhead/generated/lib" に生成されます。

