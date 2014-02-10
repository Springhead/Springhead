/**
\page pageSDKNote SDK開発時の注意点

\section secCast キャスト
APIクラス，実装クラスは，多重継承をすることになるため，
ポインタをキャストするときは，継承の関係に注意してキャストを行わないと，
ずれた場所を指してしまうことがあります．

dynamics_castを用いれば問題ありませんが，
dynamic_castをサポートしない処理系も多いので，次のようにCast()メンバ関数または，マクロ DCAST で代用します．
<pre>
 Scene* s = new PHScene;
 PHScene* ps = s->Cast();
 PHScene* ps2 = DCAST(PHScene, s);
</pre>


\section secTypeInfo 型情報について
 Springhead2では，RTTIをサポートしていない処理系でも動くように，自前で型情報を
 用意しています．
\subsection sectypeInfoForObject オブジェクトの型情報
 Springheadのオブジェクトのクラスの宣言には，
<pre>
 SPR_OBJECTDEF???(クラス名);
</pre>
を書きます．このマクロは，オブジェクトからインタフェースの情報を取得するための関数
GetIfInfo()とGetIfInfoStatic()の宣言，型変換メンバ関数Cast()，
オブジェクトの型情報と継承関係の情報を持つ GetTypeInfo()とGetTypeInfoStatic()
の情報を生成します．
*/
