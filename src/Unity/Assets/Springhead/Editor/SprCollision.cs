using UnityEngine;
using UnityEditor;//エディタ拡張関連はUnityEditor名前空間に定義されているのでusingしておく
using System.Collections;
using System.Collections.Generic;
using SprCs;


public class SprCollision : EditorWindow
{
    public struct SprLayer
    {
        public string name;
        public List<GameObject> allObject;
    }
    public static List<GameObject> allChildren = new List<GameObject>();
    public static List<bool> toggleList = new List<bool>();
    public static List<SprLayer> SprLayerList = new List<SprLayer>();

    //メニューのWindowにEditorExという項目を追加
    [MenuItem("Window/SprCollision")]
    static void Open()                       
    {
        //メニューのWindow/EditorExを選択するとOpen()が呼ばれる。
        //表示させたいウインドウは基本的にGetWindow()で表示&取得する。
        EditorWindow.GetWindow<SprCollision>("SprCollision");//タイトル名をSprCollisionに指定 

        //Layerの名前をListに入れる（表示用）
        SprLayer tmpLayer = new SprLayer();
        for (int i = 0; i < 5; i++)
        {
            tmpLayer.name = LayerMask.LayerToName(i + 8);
            tmpLayer.allObject = new List<GameObject>();
            if (!tmpLayer.name.Equals(""))
            {
                SprLayerList.Add(tmpLayer);
            }

        }

        // Typeで指定した型の全てのオブジェクトを配列で取得し,その要素数分繰り返す
        //親オブジェクトを取得していく
        foreach (PHSceneBehaviour obj in UnityEngine.Resources.FindObjectsOfTypeAll(typeof(PHSceneBehaviour)))
        {

            // アセットからパスを取得.シーン上に存在するオブジェクトの場合,シーンファイル（.unity）のパスを取得.
            string path = AssetDatabase.GetAssetOrScenePath(obj);
            // シーン上に存在するオブジェクトかどうか文字列で判定.
            bool isScene = path.Contains(".unity");
            // シーン上に存在するオブジェクトならば処理.
            if (isScene)
            {
                if (obj.gameObject.layer - 8 >= 0)
                {
                    SprLayerList[obj.gameObject.layer - 8].allObject.Add(obj.gameObject);  
                }
            }
            //子をたどっていく                         
            Transform children = obj.GetComponentInChildren<Transform>();
            foreach (Transform ob in children)
            {
                GetChildren(ob);
            }
        }

        //必要な分だけtoggleボタンを用意していく
        for (int i = 0; i < SprLayerList.Count; i++)
        {
            for (int k = 0; k < SprLayerList.Count - i; k++)
            {
                toggleList.Add(true);
            }
        }
    }

    void OnGUI()
    {
        int num;
        int toggleNum;
        num = 0;
        toggleNum = 0;

        //横軸のレイヤー名を表示していく
        EditorGUILayout.BeginHorizontal(GUI.skin.box);
        {
            EditorGUILayout.LabelField("", GUILayout.Width(100));
            foreach (GameObject obj in allChildren)
            {
                EditorGUILayout.LabelField("" + num, GUILayout.Width(10));
                num++;
            }
        }
        EditorGUILayout.LabelField("", GUILayout.Width(5));
        EditorGUILayout.EndHorizontal();

        //縦軸のレイヤー名とtoggleボタンを配置していく
        num = 0;
        for (int i = 0; i < SprLayerList.Count; i++)
        {
            EditorGUILayout.BeginHorizontal(GUI.skin.box);
            {
                //縦軸のレイヤー名を表示
                EditorGUILayout.LabelField(SprLayerList[i].name, GUILayout.Width(100));

                //横軸の数だけforを回す
                for (int k = SprLayerList.Count - num-1; k >=0; k--)
                {
                    //toggleを配置                                                             
                    toggleList[toggleNum] = EditorGUILayout.Toggle("", toggleList[toggleNum], GUILayout.Width(10));
                    if (toggleList[toggleNum])
                    {
                        foreach (GameObject obj in SprLayerList[i].allObject)
                        {
                            foreach (GameObject obj2 in SprLayerList[k].allObject)
                            {
                                //ここに当たり判定ONの設定
                                //横軸のレイヤーに登録されている縦軸のレイヤーに登録されている
                                //全てのオブジェクトについて設定をしなければならない（？）
                            }
                        }
                    }
                    else
                    {
                        foreach (GameObject obj in SprLayerList[i].allObject)
                        {
                            foreach (GameObject obj2 in SprLayerList[k].allObject)
                            {
                                //ここに当たり判定OFFの設定                                          
                            }
                        }
                    }
                    toggleNum++;
                }
            }
            EditorGUILayout.LabelField("", GUILayout.Width(5));
            EditorGUILayout.EndHorizontal();
            num++;
        }
    }

    public static void GetChildren(Transform obj)
    {
        if (obj.gameObject.layer - 8 >= 0)
        {
            SprLayerList[obj.gameObject.layer - 8].allObject.Add(obj.gameObject); 
        }
        Transform children = obj.GetComponentInChildren<Transform>();
        //子要素がいなければ終了
        if (children.childCount == 0)
        {
            return;
        }
        foreach (Transform ob in children)
        {
            GetChildren(ob);
        }
    }
}
