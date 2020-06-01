## cmake

以下では、CMake の生成物 (ビルドの生成物ではありません) を格納する
作業場所 (ディレクトリ) を *build* として話を進めます
(作業場所の名前は任意で構いません)。

CMake には Configure と Generate の2段階があります。

コマンドプロンプトの場合は、1回のコマンドで両方を実行できます。
```
> chdir C:/Springhead
> mkdir build
> cmake -B build [generator]
```

<a id="generator"></a>
*generator* の詳細は、コマンドプロンプトで `cmake --help` とすると確認できます。

> *generator* の例<br>
Windows: `-G "Visual Studio 15 2017" -A x64`<br>
unix:    `-G "Unix Makefiles"`

*generataor*を省略した場合のデフォルトは、
Windows の場合にはインストールされている Visual Studio の最新バージョンが、
unix の場合には Unix Makefiles が選択されるようです。 
ただし、マシンアーキテクチャは自動的には判定されません。
Windows で 64 ビットマシンの場合には `-A x64` を指定してください。
これを忘れると Visual Studio のプラットフォームが `x64` となりません。

<br>
`cmake-gui`を利用する場合は、まず、次の画面で Configure ボタンを押します。
<img src="/fig/CmakeConfigure1.jpg" width="600px" alt="Configure 1">

"build" ディレクトリがなければ作成するかどうかを尋ねられ、<br>
![Configure 2](/fig/CmakeConfigure2.jpg)

次に generator 指定画面となります。<br>
![Configure 3](/fig/CmakeConfigure3.jpg)

最後に、最初の画面の Generate ボタンを押します。

以上で、"build" 以下に solution file / project file (Windowsの場合)
または Makefile (unixの場合) などが生成されたはずです。

