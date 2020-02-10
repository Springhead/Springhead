## 対処法

前節で示した問題点への対処法を説明します。

** ソースファイルの整合性 **

Springhead Library のソースツリーにあるプロジェクトディレクトリ
 (Base, Collision, ...) を直接 `add_subdirectory` すれば十分です。

** ビルドの最適性 (無駄なコンパイル) **

Springhead Library, *App1*, *App2* などのすべてにおいて、
ライブラリのオブジェクトが生成される場所を共通化してしまうことで
この問題を回避することとします。
具体的には、Springhead Library ソースツリーの中 (ビルドツリーの外) に
オブジェクトの共通格納場所を作り、
Springhead Library およびすべてのアプリケーションで、
ライブラリのオブジェクト格納ディレクトリをそこへの link とします。

```
C:/Springhead/core/src/　…　ソースツリー
    +-- Base/　… (1)
    |    +-- <int-dir>/　…　中間ディレクトリ <platform> または <platform>/<VS-Version>
    |         +-- Base.dir/　…　オブジェクト共通格納領域(2)
    :
    +-- build/　…　ビルドツリー (他の場所でも構わない)
         +-- Base/
         |    +-- Base.dir/　…　(2)にlinkを張る
         +-- :
```
```
C:/Develop/Application/　…　ソースツリー
    +-- Base/　… 上図ソースツリーの(1)を add_subdirectory する
    :
    +-- build/　…　ビルドツリー (他の場所でも構わない)
         +-- Base/
         |    +-- Base.dir/　…　上図ソースツリーの(2)にlinkを張る
         +-- :
```

オブジェクトの共通格納領域を設定する作業は Springhead Library の
 cmake (configure) 時に、
link を張る作業はアプリケーションの cmake (configuure) 時に行なうものとします。

<a id="ProjectFileIntegration"></a>
** プロジェクトファイルの整合性 (Visual Studio の場合) **

ビルドの最適性の場合と同様、
ソリューションファイルおよびプロジェクトファイルを共通化することで
この問題に対処します。
具体的には、Springhead Library ビルドツリー上にあるものを最新の状態に保つ
ことを前提として、すべてのアプリケーションについて、
Springhead Library のプロジェクトに関わるソリューションファイルおよび
プロジェクトファイルはすべて
 Springhead Library ビルドツリーにあるものの複製をもつようにします。

ただしこれでは不完全で、
*App1* で実施したプロジェクトファイルへの変更が *App2* に伝わりません。
このため *App1* でプロジェクトファイルを変更した場合には、
その変更を Springhead Library ビルドツリーにあるプロジェクトファイルに
コピーするものとします。
つまり、Springhead Library のビルドツリーにあるプロジェクトファイルを
常に最新の状態に保つということです。

この作業はアプリケーションのビルド時に行なうものとします。そのために、
各アプリケーションのソリューションファイルに特別なターゲット *sync* を作成し、
このターゲットが毎回のビルドに先立って実行されるようにします。

```
C:/Springhead/core/src/　…　ソースツリー
    +-- Base/　… (1)
    :
    +-- build/　…　ビルドツリー (他の場所でも構わない)
         +-- Base/
         |    +-- Base.vcxproj　… 常に最新となるよう管理する(3)
         +-- :
```

```
C:/Develop/Application/　…　ソースツリー
    +-- Base/　… 上図ソースツリーの(1)を add_subdirectory する
    :
    +-- build/　…　ビルドツリー (他の場所でも構わない)
         +-- Base/
         |    +-- Base.vcxproj　…　上図ビルドツリーの(3)の複製とする
         +-- :
```

