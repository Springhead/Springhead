/** \page pagePhysics 物理シミュレーションSDK

- \ref sec_GRSdk
- \ref scene

\section sec_GRSdk 物理エンジンのオブジェクトのイメージ
1つの物理エンジン(PHSdk)は，いくつかのシーン(PHScene)を持つことができ，
シーンには剛体(PHSolid)，関節(PHJoint)があります．
剛体は形状(CDConvexMesh, CDSphere, CDBox, ...)を参照して形を持つことができます．
関節は2つの剛体を結び付けます．
物理エンジン内のオブジェクトの参照関係の例を図に示します．
<img src="../../include/docsrc/01UsersGuide/PHSdkScene.jpg">

\section scene シーン
*/