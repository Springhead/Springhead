/** \page users_guide		ユーザーズガイド

\section introduction はじめに
Springhead2は，安定で高速な，多くの用途に十分な機能を持つ物理シミュレータです．

\subsection characteristic	特徴

\subsection others	他のシミュレータ


\section install	インストール
詳細は \ref tutorial を参照ください．

\section concept	コンセプト
\subsection solid	剛体
剛体は，質量，慣性モーメント，重心位置を持ち，
位置・向き，速度・角速度を状態として持ちます．

また，\ref shape を持ち，互いに重ならないように保たれます．

\subsection joint	関節
関節は，2つの剛体を結ぶもので，2剛体の相対運動に制約を加えます．

\subsection spring	バネ・ダンパ
\subsection shape	形状

\section apiBasic		APIの基礎
SDKは，実装を持たないAPIクラスを介して使います．

\subsection apiClass APIクラス
APIクラスは，名前の最後にインタフェースを表すIfがつきます．
まず，SDKクラスのインスタンスを返すグローバル関数を用いて，
SDKクラスのインスタンスを作成・取得し，
次にSDKクラスから別のAPIクラスのインスタンスを作成・取得していきます．<br>
次のコードは物理エンジンAPIを使ったプログラムの例です．
\verbatim
int main(int argc, char* argv[]){
    PHSdkIf* sdk = CreatePHSdk();                   //  物理エンジンSDKの作成
    PHSceneIf* scene = sdk->CreateScene();          //  シーンの作成
    :
\endverbatim

\subsection state ステートとデスクリプタ
APIクラスのインスタンスを作るときには，デスクリプタという構造体を使って
初期値を渡します．デスクリプタは，APIクラスの名前からIfをとってDescをつけた名前です．
\verbatim
int main(int argc, char* argv[]){
    PHSdkIf* sdk = CreatePHSdk();                   //  物理エンジンSDKの作成
    PHSceneIf* scene = sdk->CreateScene();          //  シーンの作成
    PHSolidDesc desc;                               //  剛体のディスクリプタ
    desc.mass = 2.0;                                //  質量を2に設定
    desc.inertia *= 2.0;                            //  慣性テンソルの対角成分を2に設定
    PHSolidIf* solid1 = scene->CreateSolid(desc);   //  剛体１をdescに基づいて作成
    desc.center = Vec3f(1,0,0);                     //  剛体の重心を(1,0,0)に設定
    PHSolidIf* solid2 = scene->CreateSolid(desc);   //  剛体２をdescに基づいて作成
    :
\endverbatim
ディスクリプタのなかには， Spr::PHSolidDesc のように，ステート(Spr::PHSolidState)を継承しているものがあります．
ステートは，そのインスタンスの状態を表す変数でシミュレーションを進めると変化します．
ステートだけを保存しておけば，そのオブジェクトを保存しておいた時点の状態に戻すことができます．

\section structure		API構成

\subsection physics		物理シミュレーションライブラリ
\subsection graphics	グラフィックスライブラリ
\subsection fileio		ファイルIOライブラリ
*/
