using UnityEngine;
using UnityEditor;//エディタ拡張関連はUnityEditor名前空間に定義されているのでusingしておく
using System.Collections;

public class SpringheadView : EditorWindow
{
    //メニューのWindowにEditorExという項目を追加
    [MenuItem("Window/SpringheadView")]
    static void Open()
    {
        //メニューのWindow/EditorExを選択するとOpen()が呼ばれる。
        //表示させたいウインドウは基本的にGetWindow()で表示&取得する。
        EditorWindow.GetWindow<SpringheadView>("SpringheadView");//タイトル名をtestWindowに指定
    }

    static void Initialize()
    {
        SpringheadView window = (SpringheadView)EditorWindow.GetWindowWithRect(typeof(SpringheadView), new Rect(0, 0, 100, 150));
    }

    public Object source;

    int rightSize = 10;
    Vector2 rightScrollPos = Vector2.zero;


    void OnGUI()
    {


        EditorGUILayout.BeginVertical(GUI.skin.box);
        // 右側のスクロール
        rightScrollPos = EditorGUILayout.BeginScrollView(rightScrollPos, GUI.skin.box);
        {
            // Typeで指定した型の全てのオブジェクトを配列で取得し,その要素数分繰り返す.
            foreach (PHSceneBehaviour obj in UnityEngine.Resources.FindObjectsOfTypeAll(typeof(PHSceneBehaviour)))
            {

                // アセットからパスを取得.シーン上に存在するオブジェクトの場合,シーンファイル（.unity）のパスを取得.
                string path = AssetDatabase.GetAssetOrScenePath(obj);
                // シーン上に存在するオブジェクトかどうか文字列で判定.
                bool isScene = path.Contains(".unity");
                // シーン上に存在するオブジェクトならば処理.
                if (isScene)
                {
                    // GameObjectの名前を表示.
                    //EditorGUILayout.LabelField(obj.name);
                    source = EditorGUILayout.ObjectField(obj, typeof(object), true);
                }
                Transform children = obj.GetComponentInChildren<Transform>();
                foreach (Transform ob in children)
                {
                    GetChildren(ob.gameObject);
                }
            }
        }
        EditorGUILayout.EndScrollView();

        EditorGUILayout.EndVertical();
    }

    public void GetChildren(GameObject obj)
    {
        source = EditorGUILayout.ObjectField(obj, typeof(object), true);
        Transform children = obj.GetComponentInChildren<Transform>();
        //子要素がいなければ終了
        if (children.childCount == 0)
        {
            return;
        }
        foreach (Transform ob in children)
        {
            GetChildren(ob.gameObject);
        }
    }
}                     
