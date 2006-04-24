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


*/
