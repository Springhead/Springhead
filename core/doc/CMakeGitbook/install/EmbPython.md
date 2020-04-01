## EmbPython

Python インタプリタに対する外部拡張モジュールを生成する 方法について説明します。

- - -
** Springhead アプリケーションに Python インタプリタを組み込む場合 **

** Windows の場合 **

アプリケーションをビルドするとき、"CMakeSettings.txt" のビルド条件 `SPR_PROJS` に
 `EmbPython` を加えてビルドしてください。
ライブラリファイルが "C:/Springhead/core/src/EmbPython" に次の名前で生成され、
アプリケーションプログラムに組み込まれます。
 (プラットフォームが 32 ビットの場合は x64 が x86 となります)。

| Visual Studio<br>バージョン | Debug 構成 | Release 構成 | Trace 構成 |
|:--:|:--:|:--:|:--:|
| 2015 | EmbPython14.0x64D.lib | EmbPython14.0x64.lib | EmbPython14.0x64T.lib |
| 2017 | EmbPython15.0x64D.lib | EmbPython15.0x64.lib | EmbPython15.0x64T.lib |

> ビルド条件については
 [ビルドの準備](/application/Preparation.md#BuildParameters)
 を参照してください。

> また、次の "unix の場合" と同様にすれば、
上記のライブラリファイルだけを作成することもできます。

** unix の場合 **
 
ディレクトリ "C:/Springhead/core/src/EmbPython" に移動してください。

次のように cmake コマンド (パラメータに注意) を実行してから
 make コマンドを実行してください。

```
> chdir C:/Sprintghead/core/src/EmbPython
> cmake -B build -DSTANDALONE=1 [generator]
> chdir build
> make
```

> *generator* については [cmake](/install/Cmake.md#generator) を参照してください。

ライブラリファイルは、ディレクトリ "C:/Springhead/generated/lib" に
次の名前で作成されます。
( SHARED 構成は未だ実装されていません )

| STATIC 構成 | SHARED 構成 |
|:--:|:--:|
| libEmbPython.a | libEmbPython.so |

アプリケーションをリンクするときは、このライブラリを Springhead ライブラリより
前に指定してください。

- - -
** Pythonインタプリタに対する外部拡張モジュール (Python DLL, pyd) を作成する場合 **

cmake 版は未だ実装されていません。Windows については下記を参照してください。

- - -
Windows Visual Studio 用のソリューションファイルを用いてビルドする方法については
[CMakeを利用しない場合 (非推奨)](/install/DontUseCmake.md#EmbPython) をご覧ください。
- - -

