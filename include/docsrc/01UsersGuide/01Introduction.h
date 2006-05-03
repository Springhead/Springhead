/**
\page pageIntroduction はじめに
Springheadは，安定で高速な，多くの用途に十分な機能を持つ物理シミュレータです．

\section sec_characteristic	Springheadの特徴
Springheadは，接触，関節，バネ・ダンパなどが発生する力を拘束条件と捕らえ，
剛体の運動方程式を連立させて解く解析法のソルバーを持ちます．
また，凸多面体モデルの組み合わせで表される物体形状について正確な接触解析を行って
接触力の計算を行います．

関節の生成・削除のコストが低い，使用可能なバネ・ダンパ係数が非常に広い，
接触解析が高速で正確といった特徴を持ちます．

\subsection sec_others	他のシミュレータ
 Springhead(Springhead2)以外のシミュレータを紹介しながら，Springheadと比較します．
 - Springhead1 (OpenSource BSD Apache)
  - Springheadの前作．長谷川が接触，田崎が関節を主に開発した．
  - 高精度な接触力計算．
  - ペナルティ法のため，1ステップの計算が高速だが，ステップを大きくできない．
 - Open Tissue (OpenSource GPL)
  - Copenhagen University の Kenny Erlebenさんが剛体物理まわりを作っている．
  - LCPを用いた定式化で，剛体のシミュレーションを行う．
  - boost を多用した綺麗だが難しいプログラム．ホワイトボックス．
  - ソースには，いろいろなアルゴリズムを試した跡があり，テンプレート引数で切り替えられるようになっている．
 - Open Dynamics Engine (OpenSource BSD GPL)
  - Russell Smith さんが作っている．
  - LCPを用いた定式化で，剛体のシミュレーションを行う．
 - Novodex (Closed Souce 試用・教育研究用は無料)
  - ゲーム用物理エンジン．物理エンジンハードウェア開発会社AGEIAが開発．
  - 剛体物理はおそらく，LCP．
  - ConvexMeshやHeightMapをサポート

Springheadは，拘束条件をLCPとして定式化してガウスサイデル法を用いて解析解を近似的に求めています．
ここまでは，2006年現在もっとも一般的な物理シミュレーション手法だといえます．
接触判定・解析については，形状を凸多面体で表現し，GJKによる衝突判定と法線計算し，
QuickHull法を用いて接触解析で高速に接触領域を計算して接触力計算を行います．
この部分は，同じ方法を使っているものは少ないかもしれません．

Springhead(Springhead2)は，他のシミュレータにたいして，
 - Mesh(凸多面体)が扱える．
 - 任意(かなり広範囲)のバネダンパを安定にシミュレーションできる．
 - OpenSourceでありながら，分かりやすいAPIがある．
といった特徴を持ちます．

*/
