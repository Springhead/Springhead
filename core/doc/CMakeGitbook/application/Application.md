# アプリケーションのビルド (開発者向け)

- - -
この章での説明は、Springhead Library の開発者に向けたものです。
- - -

Springhead Library の開発をアプリケーションの開発と同時に並行して実施する場合、
これから説明する方法を用いることをお勧めします。

> [インストール](/install/Install.md) で説明した方法で問題はありませんが、
Visual Studio 等の開発ツールでの作業に慣れている場合には
**無駄なビルド**が気になるでしょう。
以下に説明する方法は、これらの**無駄**を少しでも無くすことを目標としています。

以下、[インストール](/install/Install.md) に従って、
**Springhead Library のインストールとビルド (正確には cmake) が
実行されていることを前提とします。**
また、Springehad aLibray をインストールしたディレクトリを **"C:/Springhead"**、
アプリケーションを開発するディレクトリを **"C:/Develop/Application"** として
説明を進めます。

<br>
また、[ビルドの準備](/install/Preparation.md) で示したファイルの他に、
次のファイルも使用します。

| ファイル名 | 説明 |
|:--|:--|
| CMakeLists.txt.Dev.dist | アプリケーション生成用設定ファイルの雛形 |
| CMakeSettings.txt.Dev.dist | ビルドパラメータ変更用ファイルの雛形 |
| CMakeTopdir.txt.dist | ダウンロードツリー位置指定用ファイル |

