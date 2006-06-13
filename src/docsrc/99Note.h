/**
\page pageSDKNode SDK開発時の注意点

\section secCast キャスト
APIクラス，実装クラスは，多重継承をすることになるため，
ポインタをキャストするときは，継承の関係に注意してキャストを行わないと，
ずれた場所を指してしまうことがあります．

dynamics_castを用いれば問題ありませんが，
dynamic_castをサポートしない処理系も多いので，次のようにマクロ DCAST で代用します．
<pre>
 Scene* s = new PHScene;
 PHScene* ps = DCAST(PHScene, s);
</pre>


\section sec_TypeInfo 型情報について
 Springhead2では，RTTIをサポートしていない処理系でも動くように，自前で型情報を
 用意しています．
\subsection sec_typeInfoForObject
 Springheadのオブジェクトのクラスの宣言には，
*/
