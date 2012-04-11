/** \page pageEmbPython Python言語との連携機能

\contents

\section sec_EmbPythonAbst 概要

PythonインタプリタからSpringheadを触れるようにするモジュールです。
実行するとPythonインタプリタが起動し、Pythonインタプリタからシーンに触ることなどができます。

\section sec_EmbPythonInstall 埋め込みPythonの設定
Springheadの設定に加えて以下の設定が必要です。
-# 環境変数 「SPRPYTHONPATH」に Springheadのフォルダ\bin\src\Python\Lib を設定。
-# PCの再起動（Visual Studioのデバッグ環境に環境変数を反映させるために必要なようです。他の方法が見つかればよいのですが）
環境変数の設定が悪いと
バッファオーバランエラーが起こったりしてプログラムが動かなくなります。

\section sec_EmbPythonUsage 埋め込みPythonの使い方
/src/Samples/EmbPython/BoxStack にサンプルがあります。

*/
