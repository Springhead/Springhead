


ゲームエンジンUnity上でSpringheadを利用する方法について述べます。この機能は開発中のため、予告なく大きな仕様変更をする場合があります。専用のDLLをロードすることにより、Springheadの機能をC\#から利用することが可能です。また、SpringheadにはUnityのGameObjectとSpringheadのオブジェクトを接続・同期するための一連のC\#スクリプトが含まれます。以降では、Unity上でSpringheadを利用するためのDLLと一連のC\#スクリプト群をまとめて SprUnity と呼びます。
## Springhead C\# DLLのビルド
Springheadの機能をC\#で使うためには以下の3つのDLLファイルが必要になります。

-  `Springhead2\bin\win64\SprExport.dll`
-  `Springhead2\bin\win64\SprImport.dll`
-  `Springhead2\bin\win64\SprCSharp.dll`

これらのDLLはSprUnityアセットには含まれているのでビルドする必要はありませんが、うまく動かない場合や、最新のソースを使いたい場合などは、ビルドする必要があります。これらのファイルをビルドするには、以下のソリューションファイルをVisual Studio 2013で開き、Release構成・x64プラットフォームでビルドします。

`Springhead2\src\SprCSharp\SprCSharp12.0.sln`


## 利用法

### 環境変数PATHの設定
Springheadの動作は、`Springhead2\core\bin\win64`フォルダ、および`Springhead2\dependency\bin\win64`フォルダ内のdll群に依存しています。これらのフォルダの絶対パスを環境変数PATHに追加してください。環境変数の設定を避けたい場合は、Unityプロジェクト内のフォルダ`Assets\Springhead\Plugins`に必要なdllを全てコピーするという方法も使えるはずです。
### 動くか試してみる
Unityプロジェクト `Springhead2\src\Unity` をUnityエディタで開き、Scenesの中からRigidBodyを選んで開き、ゲームを実行してみてください。うまく動けば、箱が落ちたり、ボールが転がったりするはずです。
### アセットのエクスポート
Unityプロジェクト `Springhead2\src\Unity` をUnityエディタで開き、AssetsのSpringheadフォルダを右クリックして Export Package を実行します。\footnote{将来的には、最初からエクスポートしたパッケージを配布することになると思います。}出てきたダイアログでSpringhead以下の全てのフォルダにチェックを入れ、Exportボタンを押し、適当な場所に適当なファイル名で保存します。拡張子は `.unitypackage` になります。ここでは、`Springhead2\SprUnity.unitypackage` に保存したものとして進めます。
### 自分のUnityプロジェクトにアセットをインポート 
SprUnityを使いたいプロジェクトをUnityで開き、Assetsを右クリックして Import Package → Custom Package を選びます。ファイル選択ダイアログが開くので、先ほどエクスポートした `SprUnity.unitypackage` を選びます。インポート対象は特段の理由がなければ全てのファイルにチェックを入れます。インポートを実行すると、Assets内にSpringheadフォルダができます。
### シーンの作成
PHSceneに対応するGameObjectを作成する必要があります。GameObjectメニュー → Create Empty を実行します。作ったオブジェクトには分かりやすい名前をつけるとよいです。ここではSpringheadSceneという名前を付けたものとします。SpringheadSceneオブジェクトを選択し、インスペクタで Add Component → Scripts → PH Scene Behaviour を選びます。これで、SpringheadSceneオブジェクトにPHSceneスクリプトが対応付けられました。PHSceneのプロパティは、PHSceneBehaviourスクリプトインスペクタで Ph Scene Descriptor を展開すると表示されます。重力の向きや各種閾値などをここで変更することができます。
### 剛体の作成
GameObject → 3D Object → Cube などを選び、オブジェクトを作成します。ここでは名前をCubeとします。Cubeオブジェクトは、SpringheadSceneオブジェクトの子オブジェクトとしてください。次にCubeオブジェクトのインスペクタから、Add Component → Scripts → PH Solid Behaviour を選びます。PHSolidBehaviourのインスペクタでは、Solid Descriptorを展開することで剛体のパラメータ（質量や、静止するかどうかなど）を設定できます。例えばDynamicalのチェックを外すと、床のように動かないオブジェクトになります。この時点でゲームを実行すると、箱が落ちていくはずです。
### コリジョンの付与
Springheadで物理衝突判定を使うには、UnityオブジェクトのColliderとは別に、CDBoxBehaviourなどのスクリプトを紐付ける必要があります。Cubeオブジェクトを選択し、インスペクタで Add Component → Scripts → CD Box Behaviour を選びます。衝突判定形状のサイズはBox Colliderの大きさが使われます。Springhead剛体オブジェクトを2個用意し、片方のDynamicalのチェックを外して床とし、ゲームを実行すると、衝突の様子が確認できるでしょう。
### 関節の作成
TBW
### IKの作成
TBW
## デバッグの方法
SprUnityを利用するときに、Springhead DLL内部のデバッグを行いたい場合、DLLをDebug構成でビルドしておく必要があります。Unityエディタを起動した状態で、Springhead C\# DLLをビルドしたソリューションファイルをVisual Studioで開き、Unity.exeプロセスにアタッチします。この状態でゲームを実行すると、DLL内部でエラーが発生した時にVisual Studioでデバッグを行うことができます。
## SprUnityを開発する方へ

### 開発用Unityプロジェクト
SprUnityを開発するためのUnityプロジェクトが `Springhead2\src\Unity` にあります。SprUnityに必要なスクリプトはできるだけこのUnityプロジェクト内で開発してください（もし異なるUnityプロジェクトで開発した場合も最終的にこのプロジェクトに含めてください）。プロジェクトのフォルダ構成は以下の通りです。
```
+- Scenes/         開発用の各種シーン
+- Springhead/     Springheadアセット一式。このフォルダをエクスポートする想定
   +- Editor/      SpringheadのためのUnityエディタ拡張スクリプト
   +- Plugins/     Springhead C# DLLがここに入る
   +- PHxxxx.cs    PHxxxxを使うためのUnity Script
   +- ...
```
Springhead C\# DLLをビルドすると`Springhead2\bin\win64`に出力されるので、開発用Unityプロジェクトで利用するには`Springhead2\src\Unity\Assets\Springhead\Plugins`にコピーする必要があります。
### Behaviour開発の手引き
Unity上で利用したいSpringheadクラスごとに\footnote{議論の余地あり}Behaviourスクリプトを作り、そのスクリプトにSpringheadオブジェクトの作成・Unityとの同期等を担当させてください。例えば*PHSolid*を担当するスクリプトは*PHSolidBehaviour*で、このスクリプトはゲーム開始時にSpringheadシーン内に*PHSolid*を作成し、1ステップごとに*PHSolid*の位置に応じてゲームオブジェクトの位置を変更します。Behaviourスクリプトの各変数・関数の役割を、*PHSceneBehaviour*を題材に解説します。
```
using UnityEngine;
using SprCs;

public class PHSolidBehaviour : SprSceneObjBehaviour {
```
Springheadの機能を利用するのに*SprCs*名前空間をusingしておくと便利です。PHSdkやPHSceneの子要素を作成するBehaviourスクリプトは、*SprSceneObjBehaviour*を継承してください。これによりPHSceneBehaviourを探してPHSceneを取得する*phScene*プロパティや、*phSdk*プロパティが使えるようになるほか、インスペクタの値が変更された時に自動的にSpringheadオブジェクトのSetDescが呼ばれる機能などが実装されます。
```
    public PHSolidDescStruct desc = null;

    public override CsObject descStruct {
        get { return desc; }
        set { desc = value as PHSolidDescStruct; }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
        (from as PHSolidDescStruct).ApplyTo(to as PHSolidDesc);
    }

    public override CsObject CreateDesc() {
        return new PHSolidDesc();
    }
```
*XXXDescStruct*型のpublic変数を作ることで、デスクリプタの内容が設定項目としてインスペクタに表示されます。ここでは*XXXDesc*ではなく*XXXDescStruct*を利用してください。また、初期値は必ず*null*として下さい。SprSceneObjBehaviourを継承するクラスは、*descStruct*プロパティ、*ApplyDesc*関数、*CreateDesc*関数を定義しなければなりません。これらはインスペクタの値が変更されたときに自動でSpringheadオブジェクトに反映されるようにするために必要です（必要が無い場合は中身のない（あるいは*null*を返す）関数を定義して下さい）。
```
    public override ObjectIf Build() {
        PHSolidIf so = phScene.CreateSolid (desc);
        so.SetName("so:" + gameObject.name);

        Vector3 v = gameObject.transform.position;
        Quaternion q = gameObject.transform.rotation;
        so.SetPose (new Posed(q.w, q.x, q.y, q.z, v.x, v.y, v.z));

        return so;
    }
```
*Build()*は、ゲーム開始時に、*Awake()*のタイミングで実行されます\footnote{スクリプトのイベント関数については `http://docs.unity3d.com/ja/current/Manual/ExecutionOrder.html` を参照}。ここでdescに従ってSpringheadオブジェクトを作成し、作成したオブジェクトをreturnしてください。*Build()*の戻り値はBehaviourの *sprObject* プロパティに代入され、Behaviour内部や他のBehaviourからアクセス可能になります。
```
    void Link () {
        // PHSolid場合は特にやることはない
    }
```
*Link()*はあらゆるオブジェクトの*Build()*より後で呼ばれます。具体的には、*Build()*が*Awake()*のタイミングで実行されるのに対し、*Link()*は*Start()*のタイミングで実行されます。全オブジェクトの構築が終了した後に、構築されたオブジェクト同士の関係を設定（``Link''）する作業をここで行います。例えば*PHIKEndEffector*を*PHIKActuator*に*AddChildObject*するなど、全オブジェクト作成後でないと行えないような処理をここに記述します。
```
    public void Update () {
        if (sprObject != null) {
            PHSolidIf so = sprObject as PHSolidIf;
            if (so.IsDynamical()) {
                // Dynamicalな剛体はSpringheadのシミュレーション結果をUnityに反映
                Posed p = so.GetPose();
                gameObject.transform.position = new Vector3((float)p.px, (float)p.py, (float)p.pz);
                gameObject.transform.rotation = new Quaternion((float)p.x, (float)p.y, (float)p.z, (float)p.w);
            } else {
                // Dynamicalでない剛体はUnityの位置をSpringheadに反映（操作可能）
                Vector3 v = gameObject.transform.position;
                Quaternion q = gameObject.transform.rotation;
                so.SetPose(new Posed(q.w, q.x, q.y, q.z, v.x, v.y, v.z));
            }
        }
    }
```
Updateには、Springheadのシミュレーション結果をUnityのオブジェクトに反映するコードを書いて下さい。※なお、実際のPHSolidBehaviourでは、UpdateではなくUpdatePose関数が定義され、PHSceneBehaviourのUpdateが各SolidのUpdatePoseを呼ぶようになっています。これは剛体オブジェクトの位置の反映をPHSolidBehaviourのUpdateで行った場合、スキンメッシュの描画がうまくいかないためです。
### スクリプト実行順序の設定
各Stepにおいて、PHSceneBehaviourは他のPhysics系スクリプトより先に実行される必要があります。新たにスクリプトを追加した場合などは、開発者が適切な実行順序を設定してください。設定したスクリプト実行順序はエクスポートされる情報に含まれるので、SprUnityのユーザは特に気にせず利用できます。実行順序の設定は Editメニュー → Project Settings → Script Execution Order から行います。
