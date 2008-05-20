/**
\page pageNodeDefList ノードの定義一覧


\ref pageFileIO がロード・セーブできる通常のノードは，ディスクリプタと1対1対応します．
また，対応するオブジェクトがない特殊ノードもディスクリプタにあたるC++の構造体だけはあります．
そこで，ノードの定義の一覧の代わりに，C++の構造体の一覧を示します．

\section secNormalNodeDefList 通常ノードの定義一覧
通常のノードはシーングラフのオブジェクト（のディスクリプタ）に対応します．
各SDKのディスクリプタの説明を参照してください．
<ul>
 <li> \ref gpPhysics
 <li> \ref gpGraphics
</ul>

\section secSpecialNodeDefList 特殊ノードの定義一覧

\ref gpFWOldSpringheadNode<br>
DirectXのXFileのノードや，旧Springheadのファイルをロードするための互換ノードです．
メタセコイアなどのツールが作ったメッシュデータのXファイルのロードにも使っています．
ロード時には，対応するSpringhead2のオブジェクトに変換されます．
セーブ時には使われません．
*/
