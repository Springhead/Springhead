using UnityEngine;
using System.Collections;
using SprCs;

public abstract class SprBehaviour : MonoBehaviour {
    // 
    private ObjectIf sprObject_ = null;
    public ObjectIf sprObject { get { return sprObject_; }  protected set { sprObject_ = value; } }

    /// -- 派生クラスで実装するメソッド
    public abstract CsObject descStruct { get; set; }
    public abstract CsObject CreateDesc();
    public abstract void ApplyDesc(CsObject from, CsObject to);
    public abstract ObjectIf Build();
    public virtual void Link() { }

    // Springhead Behaviourの初期化。Reset / OnValidate 両方の先頭で呼ぶ
    public void Initialize() {
        SetDLLPath();
        InitDesc();
    }

    // デスクリプタ構造体を初期化する
    public void InitDesc() {
        if (descStruct == null) {
            descStruct = CreateDesc();
        }
    }

    // UnityのReset
    public void Reset() {
        Initialize();
    }

    // UnityのAwake
    void Awake() {
        if (!enabled) { return; }
        sprObject = Build();
    }

    // UnityのStart
    void Start() {
        Link();
        OnValidate();
    }

    // UnityのOnValidate
    public virtual void OnValidate() {
        Initialize();
        if (sprObject != null) {
            CsObject d = CreateDesc();
            if (d != null) {
                sprObject.GetDesc(d);
                ApplyDesc(descStruct, d);
                sprObject.SetDesc(d);
            }
        }
    }

    /// -- Privateメソッド
    [System.Runtime.InteropServices.DllImport("kernel32", SetLastError = true)]
    private static extern bool SetDllDirectory(string lpPathName);
    protected void SetDLLPath() {
        SetDllDirectory(Application.dataPath + "/Assets/Springhead/Plugins");
        SetDllDirectory(Application.dataPath + "/Springhead/Plugins");
    }
}

public abstract class SprSceneObjBehaviour : SprBehaviour {
    //
    public PHSceneIf phScene {
        get {
            PHSceneBehaviour pb = gameObject.GetComponentInParent<PHSceneBehaviour>();
            if (pb == null) { throw new ObjectNotFoundException("PHSceneBehaviour was not found in any tree parent", gameObject); }
            return pb.sprObject as PHSceneIf;
        }
    }

    //
    public PHSdkIf phSdk {
        get {
            return phScene.GetSdk();
        }
    }
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