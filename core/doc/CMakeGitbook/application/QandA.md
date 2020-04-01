## Q&A

前項 ([ビルドの準備](/application/Preparation.md)) に従って作成したアプリケーション
に関する Q&A 集です。
以下 Springehead Library の Base プロジェクトを例にとって説明しますが、
他のプロジェクト (Collision, Foundation, ...) についても同様です。

> unix 上で Springhead Library とアプリケーションの並行開発を行なうことはないと
思われますので、ここでは Windows 上で Visual Studio を使う場合について説明します。

----
** Q. プログラムの実行時に“リソースファイルが見つからない”というエラーが起きる **

プログラムは作業ディレクトリ *build* の下に作成されます。
プログラムが相対パス指定でリソースを参照している場合には、
CMakeLists.txt があるディレクトリ (C:/Develop/Application) から
プログラムを起動してください。

----
** Q. ソリューションファイルに新しいターゲットがある **

** ALL_BUILD **

これは CMake が自動的に作成するターゲットで、make all に相当するものとされています。
ただし Visual Studio 上では ALL_BUILD の依存関係の設定が不正確で、
このターゲットをビルドしても正しい結果は得られないようです。
** このターゲットは無視してください。 **

** sync **

これはプロジェクトファイルの整合性を保つために作られたターゲットで、
他のターゲットをビルドすることにより自動的にこのターゲットが最初に実行されます。
** このターゲットに対して何らかのアクションを起こす必要はありません。 **
[プロジェクトファイルの整合性](/application/Solutions.md#ProjectFileIntegration)
 参照。

----
** Q. ソリューションまたはプロジェクトが環境外で変更された旨のメッセージが出る **

これはアプリケーション側と Springhead Library 側との整合性を保つために
上記の sync ターゲットが実行されることで、
ソリューションファイル / プロジェクトファイルが変更されることがあるためです。
すべて ** 再読み込み ** としてください。
> 実際には、新しく生成されたプロジェクトファイルを取り込むために、
ソリューションファイル中の ProjectGuid を書き直すことがあるためです。

> なお、「再読み込み」を指定すると Visual Studio の出力ウィンドウに表示されている
メッセージがすべてクリアされてしまいます。
これを防ぐには、一旦「無視」を指定し、
その後にソリューションを開き直せば同じ結果が得られます。
なお、変更されるのはソリューションファイル / プロジェクトファイルだけなので、
「無視」のまま作業を続けても問題はないと思われます。

----
** Q. ディレクトリが作成できないエラーが発生する **

Springehad Library をビルドすると、ソースツリー上に
 "C:/Springhead/core/src/Base/*x64*/*15.0*/Base.dir"
 というディレクトリが作成されます
 (*x64*, *15.0* の部分は環境により異なります)。

アプリケーションの cmake をした後で上記のディレクトリを削除すると、
以降のビルドで<br>
　　“エラー MSB3191 ディレクトリ "Base.dir/Debug/" を作成できません”<br>
などというエラーが発生します。

**この問題を解消するためには、アプリケーション側または Springhead Library 側で
再度 cmake を実行する必要があります。**

また、Springhead Library 側で cmake (configure) を実行していないと
上記のディレクトリが作成されていないため、同じエラーが発生します。
**この場合には、Springhead Library 側で cmake を実行してください。**

----
<a id="CrumbleBuildOptimization"></a>
** Q. ビルドの最適性が崩れる **

アプリケーション側で "C:/Develop/Application/*build*/Base.Base.dir" などを削除すると、
ビルド時に Visual Studio が *build* 下に "Base.dir" を自動的に作成してしまうために
ビルドの最適性が崩れてしまいます。

> 無駄なビルドが発生するだけで、ビルド自体は正常に行なえます。
“ビルドの最適性”については [問題点](/application/Problems.md#BuildOptimality)
 を参照してください。

**この問題を解消するためには、アプリケーション側または Springhead Library 側で
再度 cmake を実行する必要があります。**

----
** Q. sync configuration でファイルオープンエラーが発生する **

Springhead Library 側で "*build*/Base" 下にあるプロジェクトファイル "Base.vcxproj" を
削除すると、sync ターゲット実行で link 先のファイルが見つからずに

```
1>sync configuration with C:/Springhead/core/src
　　：
1>Error : file open error : "Base/Base.vcxproj"
1>Traceback (most recent call last):
　　：
```
のようなエラーが発生します。

**この問題を解消するためには、Springhead Library 側で再度 cmake を実行する
必要があります (アプリケーション側では駄目)。**

----
** 2019/9/30 (commit 1d8e5ce) 以前に配布したバージョンをご使用の場合 **

**新しい配布ファイルから "CMakeLists.txt" を再作成すれば、
以下の問題は解消します。**

上記以前のバージョンで配布した "CMakeLists.txt.\*.dist" を元に
 "CMakeLists.txt" を作成して使用している場合は、
RunSwig で clean/rebuild の対応ができていなかったため、
clean と同等の機能を実現するためのターゲット RunSwig\_Clean が
作成されているはずです。
**上記日付以降の Springehad Library をダウンロードし cmake を実行していただければ、
RunSwig は clean/rebuild 対応となります。**

> RunSwig\_Clean をビルドすると<br>
　　python: can't open file '.../Clean.py': ... No such file ...<br>
というエラーが起きます。
実害はありませんが RunSwig の clean は行なわれません。

RunSwig\_Clean ターゲットを生成されないようにするには、
 "CMakeLists.txt" から以下の部分を削除して再 cmake してください。

```
# ------------------------------------------------------------------------------
#  Clean (only for RunSwig).
#
if ("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    set(LIBDIR ${SPR_TOP_DIR}/generated/lib/win64)
else ()
    set(LIBDIR ${SPR_TOP_DIR}/generated/lib/win32)
endif()
set(LIBPATH_DEBUG   ${LIBDIR}/Springhead${LIBNAME_DEBUG}.lib)
set(LIBPATH_RELEASE ${LIBDIR}/Springhead${LIBNAME_RELEASE}.lib)
set(LIBPATH_TRACE   ${LIBDIR}/Springhead${LIBNAME_TRACE}.lib)
set(CLEAN ${Python} ${CMAKE_SOURCE_DIR}/RunSwig/Clean.py ${CMAKE_SOURCE_DIR})

if(${Windows})
    add_custom_target(RunSwig_Clean
        COMMAND if "'$(Configuration)'" equ "'Debug'"   ${CLEAN} ${LIBPATH_DEBUG}
        COMMAND if "'$(Configuration)'" equ "'Release'" ${CLEAN} ${LIBPATH_RELEASE}
        COMMAND if "'$(Configuration)'" equ "'Trace'"   ${CLEAN} ${LIBPATH_TRACE}
        COMMENT [[  clearing RunSwig generated files and Springhead Library... ]]
    )
elseif(${Linux})
endif()
```	

EmbPython\_RunSwig\_Clean についても同様です。	

