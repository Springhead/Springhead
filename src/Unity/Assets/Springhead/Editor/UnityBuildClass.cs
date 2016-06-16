using UnityEditor;
using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using SprCs;

public class BuildClass : MonoBehaviour
{
    public static void Build()
    {
        // 現在の設定を控える
        BuildTarget prevPlatform = EditorUserBuildSettings.activeBuildTarget;

        // 実行
	BuildTarget target = BuildTarget.StandaloneWindows;
	BuildOptions opt = BuildOptions.None;
	//BuildOptions opt = BuildOptions.Development;
	string output = "../UnityTest/bin/player.exe";

	string error = BuildPipeline.BuildPlayer(
                GetSceneName(),         //!< ビルド対象シーンリスト
                GetOutputPath(),        //!< 出力先
                target,                 //!< ビルド対象プラットフォーム
                opt
        );

        // 元に戻す
        EditorUserBuildSettings.SwitchActiveBuildTarget(prevPlatform);

        // 結果出力
        if (!string.IsNullOrEmpty(error)) {
            Debug.LogError("[Error!] " + error);
        } else {
            Debug.Log("[Success!]");
	}
    }

    // Unity起動引数を取得する
    static string[] GetSceneName()
    {
	string[] sceneList = new string[1];
	sceneList[0] = GetArg("-target");
	return sceneList;
    }

    static string GetOutputPath()
    {
	return GetArg("-output");
    }

    static string GetArg(string key)
    {
        string[] progArgs = System.Environment.GetCommandLineArgs();
	string[] args = progArgs[1].Split(' ');
        for (int i = 0; i < args.Length; ++i) {
	    //Debug.Log("args[" + i + "]: " + args[i]);
	    if (args[i] == key) {
		    Debug.Log(key + ": " + args[i+1]);
                    return new string(args[i+1].ToCharArray());
            }
        }
	return "";
    }

    // Unity起動引数を取得する
    static string[] GetSceneNameX()
    {
        string[] sceneList = new string[1];
        string[] progArgs = System.Environment.GetCommandLineArgs();
	string[] args = progArgs[1].Split(' ');
        int i, len = args.Length;
        for (i = 0; i < len; ++i) {
	    //Debug.Log("args[" + i + "]: " + args[i]);
            switch (args[i]) {
                case "-target":
                    string scene = "Assets/Scenes/" + args[i+1];
                    sceneList[0] = scene;
                    Debug.Log( "-target : " + sceneList[0]);
                    break;
            }
        }
        return sceneList;
    }

    // ビルド対象シーンを Scenes In Build から抽出する
    static string[] GetEnabeledScenes()
    {
        List<string> sceneList = new List<string>();

        // "Scenes In Build"に登録されているシーンリストを取得
        EditorBuildSettingsScene[] scenes = EditorBuildSettings.scenes;
        foreach (EditorBuildSettingsScene scene in scenes) {
            // チェックがついているか確認
            if (scene.enabled) {
                // リストに加える
                sceneList.Add(scene.path);
            }
        }

        // 配列にして返す
        return sceneList.ToArray();
    }
}
