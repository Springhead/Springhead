using UnityEngine;
using System.Collections;
using SprCs;
using SprUnity;

public abstract class CDShapeBehaviour : SpringheadBehaviour {
    public GameObject shapeObject = null;
    public CDShapeIf  shape = null;

    public void Awake() {
        if (!enabled) { return; }

        if (shapeObject == null) {
            shapeObject = gameObject;
        }

        shape = CreateShape(shapeObject);
        GetPHSolid().AddShape(shape);

        GameObject solidObject = GetSolidBehaviour().gameObject;

        // 剛体オブジェクトからの相対変換
        Posed relPoseFromSolid = solidObject.transform.ToPose().Inv() * shapeObject.transform.ToPose();

        GetPHSolid().SetShapePose(GetPHSolid().NShape() - 1, relPoseFromSolid * ShapePose(shapeObject));
    }

    public void Start() { }

    public PHSolidBehaviour GetSolidBehaviour() {
        PHSolidBehaviour b = gameObject.GetComponentInParent<PHSolidBehaviour>();
        if (b == null) { throw new ObjectNotFoundException("The object should have PHSolidBehaviour", gameObject); }
        return b;
    }

    public PHSolidIf GetPHSolid() {
        PHSolidIf so = GetSolidBehaviour().phSolid;
        if (so == null) { throw new ObjectNotFoundException("The behaviour doesn't have PHSolid", gameObject); }
        return so;
    }

    public PHSceneIf GetPHScene() {
        PHSceneBehaviour pb = gameObject.GetComponentInParent<PHSceneBehaviour>();
        if (pb == null) { throw new ObjectNotFoundException("PHSceneBehaviour was not found in any tree parent", gameObject); }
        return pb.GetPHScene();
    }

    public PHSdkIf GetPHSdk() {
        return GetPHScene().GetSdk();
    }

    /// -- 派生クラスで実装するメソッド

    // SpringheadのShapeオブジェクトを構築する
    public abstract CDShapeIf CreateShape(GameObject shapeObject);

    // 形状固有のShapePoseの取得。剛体からの相対位置姿勢による分は除く
    public virtual Posed ShapePose(GameObject shapeObject) { return new Posed(); }
}
