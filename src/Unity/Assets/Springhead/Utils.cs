using UnityEngine;
using System.Collections;
using SprCs;

public abstract class SpringheadBehaviour : MonoBehaviour {
    [System.Runtime.InteropServices.DllImport("kernel32", SetLastError = true)]
    private static extern bool SetDllDirectory(string lpPathName);

    protected void SetDLLPath() {
        SetDllDirectory(Application.dataPath + "/Assets/Springhead/Plugins");
        SetDllDirectory(Application.dataPath + "/Springhead/Plugins");
    }

    public void Reset() {
        SetDLLPath();
        InitDesc();
    }

    /// -- 派生クラスで実装するメソッド

    // デスクリプタ構造体を初期化する
    public abstract void InitDesc();
}

namespace SprUnity {
    public static class SprUnityExtentions {
        // UnityのTransformをSpringheadのPosed型に変換する
        public static Posed ToPose(this Transform t) {
            Vector3 p = t.position;
            Quaternion q = t.rotation;
            return new Posed(q.w, q.x, q.y, q.z, p.x, p.y, p.z);
        }
    }
}

public class ObjectNotFoundException : System.Exception {
    public GameObject gameObject { get; private set; }

    public ObjectNotFoundException(string message, GameObject obj) : base(message) {
        gameObject = obj;
    }

    public override string ToString() {
        return gameObject.ToString() + " : " + Message;
    }
}