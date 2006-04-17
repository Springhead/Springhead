/** \page pageConcept	コンセプト

\section secArchitecture Springheadのアーキテクチャ
Springheadは大きく分けて以下の3つのモジュールから構成されています．
- Springhead Physics 物理シミュレーションエンジン
- Springhead File I/O ファイルローダ
- Springhead Graphics グラフィックレンダラ

Springhead Physicsは，

Springhead File I/Oは，

Springhead Graphicsは，

これらのモジュールは，セットで使うことはもちろん，個別に用いることもできるように設計されています．
例えば，ユーザの中にはレンダリングは手持ちのライブラリを使い，物理シミュレーションのみを
Springheadで行いたいと考えている人もいるでしょう．このような場合にも，必要最低限の追加的な
コードだけでユーザのライブラリとSpringheadとを連動させることができます．

以下では個別のモジュールについて順に解説します．

\section physics 物理シミュレーションエンジン
Springhead Physicsモジュールは，

\subsection solid	剛体
剛体は，質量，慣性モーメント，重心位置を持ち，
位置・向き，速度・角速度を状態として持ちます．

また，\ref shape を持ち，互いに重ならないように保たれます．

\subsection shape	形状
剛体には1つ以上の形状を割り当てることができます．
形状には以下のものがあります．
- 任意の凸メッシュ
- 球
- 直方体

Springhead Physicsが扱うことのできる形状は凸形状のみです．
凸形状とは，一言でいえば凹みや穴の無い形状のことです．
凸でない（凹みや穴を持つ）形状を剛体に割り当てるには，あらかじめその形状を
複数の凸形状に分割して割り当てる必要があります．

\subsection joint 関節
関節とは，2つの剛体を連結し相対運動に制約を加えるものです．
Springheadがサポートする関節の種類を以下に示します．

<img src="../../include/docsrc/01UsersGuide/joints.png">

□&nbsp;共通の仕様
はじめに，全種類の関節に共通する概念について説明します．
剛体同士を関節でつなげるには，剛体のどの場所に関節を取り付けるかを指定する必要があります．
このためにSpringheadでは関節座標系という概念を用います．



□1軸関節

□ヒンジ

□スライダ

□パスジョイント

□バネ・ダンパ

□ユニバーサルジョイント

□ボールジョイント

*/
