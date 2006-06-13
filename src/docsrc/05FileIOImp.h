/**
\page pageFileIOImp ファイル入出力SDKの実装

\section secFileIMech FileIOの仕組み

\subsection secFileLoadSave ファイルのロード・セーブ
ファイルをロードするためには，ファイルフォーマットに合ったパーサを用意
する必要があります．
これは，FIFileクラスの派生クラスが行っています．
例えばFIFileXは，Xファイルをパースします．
Boost::spiritという生成文法からのパーサジェネレータを用いて実装しています．

ファイルのセーブもFIFileXが行います．パースに比べて簡単なので，
シーングラフを巡回しながら，目的のフォーマットのファイルが出力されるように，
仮想関数を実装して行きます．

\subsection secLoadProc ロードの手順
ここでは，ファイルに書かれたひとつのオブジェクトのデータから，
オブジェクトを生成する手順を説明します．

<ol>
<li>オブジェクトの型名を読み，それにあったディスクリプタを用意する．
例えば，GRFrameをロードする場合，
<pre>
 GRFrame{
 	0,0,0,1, 0,1,0,0, 0,0,1,0, 0,1,2,0;;
 }
</pre>
のGRFrameを読んだところで， パーサが FILoadContext::PushType() を呼び出して，
GRFrameDesc を用意します．

<li>データを読み出し，用意したディスクリプタに格納する．
0,0,0,1, 0,1,0,0, 0,0,1,0, 0,1,2,0;;を読み出し GRFrame::transform に格納して行きます．
これはFILoadContextが現在読み出し中のディスクリプタについての情報を持っていて，
読み出した数値をディスクリプタに書き込んで行きます．
Vec3fやMaterialのような組み立て型は，再帰的に処理され，最終的には，
数値を読み込めば，FILoadContext::WriteNumber(), 文字列ならFILoadContext::WriteString()
が呼び出されます．

<li>オブジェクトを作成する．
ディスクリプタからオブジェクトを生成します．
これは，FILoad::LoadNode()が，親オブジェクトのCreateObject()
を呼び出していくことで実現しています．
親が作ってくれない場合は，そのまた親のCreateObject()を呼び出し，
最後まで作られない場合はグローバルのCreateSdk()関数を呼び出します．

CreateObject()については，\ref pageFoundationImp の \ref secCreateObject を参照してください．
</ol>

このようにオブジェクトの生成やデータの設定にもAPIを用いており，
他のモジュール(たとえば src/Physics)の実装には依存していません．

\subsection secLoadHandler オブジェクトのロード以外の処理
ノードをロードしたとき，単純にオブジェクトを生成するだけでなく，
何らかの処理を行いたいこともあります．

たとえば，DirectXのXファイルの FrameとFrameTransformMatrix は GRFrameと似ていますが，
若干違います．Xファイルをロードするためには，FrameとFrameTransformMatrixから，
GRFrameを生成しなければなりません．

これは，次のような手順で行います．
<ol>
 <li> Frame, FrameTransformMatrix のデータをロードできるような 
      FrameDesc, FrameTransformMatrixDesc を作る．
 <li> Frame, FrameTransformMatrix に対応したハンドラ＝FINodeHandler(の派生クラス)を作り，登録する
      (FIOldSpringheadNodeHandler.cppを参照)．<br>
      FrameIf，FrameTransformMatrixIf は作らない．（作るとそのオブジェクトが生成されてしまう）
 <li> ハンドラの中で，FrameDesc, FrameTransformMatrixDesc を受け取って，
      GRFrame を生成したり，親の GRFrameの transform を設定したりといった，処理を行う．
</ol>

\subsection secSaveProc セーブの手順

*/
