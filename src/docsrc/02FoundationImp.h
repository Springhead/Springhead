/**
\page pageFoundationImp 基本SDKの実装


\section secCreateObject オブジェクトの生成
オブジェクトは，オブジェクトを生成するAPI ObjectIf::CreateObject() を持っており，
これによって生成できます．
例えば GRSdk －生成→ GRScene －生成→ GRFrame と生成できます．
生成されたオブジェクトは，生成したオブジェクトに所有されることが多いです．

SDKは、ルートのオブジェクトで，親オブジェクトを持たないことが多いです。
FWSdk, FISdk, は親オブジェクトを持ちません。
PHSdkやGRSdkは，FWSceneが親オブジェクトになることがあります。
これらは，静的メンバー関数 PHSdkIf::CreateSdk() GRSdkIf::CreateSdk() などで生成できます．


\subsection secFactory Factoryクラス
たとえば，PHSolid を継承し 形状から自動で質量と慣性モーメントを求める PHAutoSolid
を作ったとして，これをファイルローダーでロードして，PHSceneに追加するためには，
PHAutoSolidをPHScene::CreateObject()が生成できなければなりません．

あとから，新しいクラスを生成できるようにする仕組みがファクトリです．
FactoryBaseクラスを派生させてオブジェクトを作り，
PHSceneIf::GetIfStatic()->RegisterFactory()を使って登録すると，PHScene::CreateObject()
が登録されたファクトリを呼び出してオブジェクトを生成するようになります
(実際は，FactoryImpTemplate を使うと簡単です)．
GRSdk.cpp の GRRegisterFactories() などを参照ください．

ファクトリを呼び出す実装は，Object::CreateObject() にあるので，
CreateObject() をオーバーライドするときは，これを呼び出す必要があります．

*/
