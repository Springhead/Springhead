/**
\page pagePhysicsImp 物理シミュレーションSDKの実装

\section secPhysSim 物理シミュレーション機能
\subsection 物理シミュレーション計算の概要
Springhead2は、速度ベースLCPによる拘束力計算(PHConstraintEngineIf) と
ペナルティ法による接触力計算(PHPenaltyEngineIf)の2つのエンジンを持っています。
ほとんどの場合、速度ベースLCPによる拘束力計算(PHConstraintEngineIf)だけを使うことになります。

この拘束力計算や物理シミュレーションの原理については、 
EC2006での講演資料(EC2006PBM.ppt)と src/Physics/lcpnote.txt を参照してください。
ここには、どのクラスで、各計算を行っているのかを記すだけにとどめます。

\subsection 物理シミュレーションとクラスの対応
物理シミュレーションを統括しているのは、PHSceneIf です。
ひとつのシーンについてのすべての情報がPHSceneIf オブジェクトから参照できます。
PHSceneIf は、いくつかのエンジン PHEngineIf の子孫を持ちます。
その中でも重要なのは拘束力を計算する PHConstraintEngineIf です。

\subsubsection 拘束力計算とクラスの対応
拘束条件には、PHConstraintIf とその派生クラスが対応しています。
シーンの拘束条件達を持って、拘束計算の呼び出しを行っているのが、
PHConstraintEngineIf です。

拘束力計算の手順と関係するクラス・関数を示します。
-# LCPのセットアップを行う (PHConstraintEngineIf::SetupLCP() が PHConstraintIf::SetupLCP()を呼び出す）
	-# PHConstraint::SetupLCP()は、w = Aλ+b の A と b を計算したあと、ModifyJacobian(), CompBias()を呼び出す。
	-# ModifyJacobian()は、拘束座標系を変更する場合に、ヤコビアン J を書き換える。
	-# CompBias()は、拘束条件にあわせて Aとbの補正dAとdbの計算を行う。
-# ガウスザイデル法の繰り返し計算(numIter回)を行う。(PHConstraintEngineIf::IterateLCP() が PHConstraintIf::IterateLCP()を呼び出す。）
	-# PHConstraintIf::IterateLCP()は、λ(ソースではf)の計算をしたあと、PHConstraintIf::Projection()を呼び出してλが不等式制約を満たすかチェックし、制約を満たすようにλを変更する。

*/
