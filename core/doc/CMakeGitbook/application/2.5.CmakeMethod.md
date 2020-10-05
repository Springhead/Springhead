## CMake を使用した場合

CMake は out of source (out of place) によるビルドに対応しています。
これはソースツリーの 外側 にビルドツリーを生成する機能で、
次のような特徴があります。

+ 互いに鑑賞しない複数のビルドツリーを作成することができる。
+ ビルドツリーが削除されてもソースツリーに影響が及ばない。

我々は CMake を out of source の方法で使用します。

ソースツリーおよびビルドツリーは、
ライブラリおよびアプリケーションのそれぞれで次のようになるでしょう。

```
C:/Springhead/core/src/　…　ソースツリー
    +-- CMakeLists.txt　…　ライブラリ全体のCMakeパラメータファイル
    +-- Base/　… (1)
    |    +-- CMakeLists.txt　…　プロジェクトBaseのCMakeパラメータファイル
    |    +-- :
    +-- Collision/　… (2)
    :
    +-- build/　…　ビルドツリー (他の場所でも構わない)
         +-- Springhead.sln　…　生成されたソリューションファイル
         +-- Base/
         |    +-- Base.vcxproj　…　生成されたプロジェクトファイル
         |    +-- Base.dir/　…　オブジェクトファイル(.obj)が生成される場所
         |    +-- CMakeFiles/
         |         +-- generate.stamp　…　スタンプファイル
         +-- Collision/
         :
```

```
C:/Develop/Application/　…　ソースツリー
    +-- CMakeLists.txt　…　アプリケーション全体のCMakeパラメータファイル
    +-- main.cpp
    +-- sub/
    |    +-- CMakeLists.txt　…　サブモジュールのCMakeパラメータファイル
    |    +-- sub.cpp
    +-- Base/　… 上図ソースツリーの(1)を add_subdirectory する
    +-- Collision/　… 上図ソースツリーの(2)を add_subdirectory する
    :
    +-- build/　…　ビルドツリー (他の場所でも構わない)
         +-- Application.sln　…　生成されたソリューションファイル
         +-- Base/
         |    +-- Base.vcxproj　…　生成されたプロジェクトファイル
         |    +-- Base.dir/　…　オブジェクトファイル(.obj)が生成される場所
         |    +-- CMakeFiles/
         |         +-- generate.stamp　…　スタンプファイル
         +-- Collision/
         :
```

ライブラリをビルドしたときもアプリケーションをビルドしたときも、
それぞれのビルドにおける生成物は、
すべてそれぞれのビルドツリー内におかれることに注意してください。
言い換えると、ライブラリのオブジェクトファイルが、
複数のビルドツリーに独立して配置されるということです。

