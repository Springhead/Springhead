using UnityEngine;
using System.Collections;
using SprCs;
using SprUnity;

public abstract class CDShapeBehaviour : SprSceneObjBehaviour {
    public GameObject shapeObject = null;

    public override ObjectIf Build() {
        if (shapeObject == null) {
            shapeObject = gameObject;
        }

        CDShapeIf shape = CreateShape(shapeObject);
        phSolid.AddShape(shape);

        GameObject solidObject = solidBehaviour.gameObject;

        // 剛体オブジェクトからの相対変換
        Posed relPoseFromSolid = solidObject.transform.ToPose().Inv() * shapeObject.transform.ToPose();

        phSolid.SetShapePose(phSolid.NShape() - 1, relPoseFromSolid * ShapePose(shapeObject));

        return shape;
    }

    public PHSolidBehaviour solidBehaviour {
        get {
            PHSolidBehaviour b = gameObject.GetComponentInParent<PHSolidBehaviour>();
            if (b == null) { throw new ObjectNotFoundException("The object should have PHSolidBehaviour", gameObject); }
            return b;
        }
    }

    public PHSolidIf phSolid {
        get {
            PHSolidIf so = solidBehaviour.sprObject as PHSolidIf;
            if (so == null) { throw new ObjectNotFoundException("The behaviour doesn't have PHSolid", gameObject); }
            return so;
        }
    }

    /// -- 派生クラスで実装するメソッド

    // SpringheadのShapeオブジェクトを構築する
    public abstract CDShapeIf CreateShape(GameObject shapeObject);

    // 形状固有のShapePoseの取得。剛体からの相対位置姿勢による分は除く
    public virtual Posed ShapePose(GameObject shapeObject) { return new Posed(); }

    // UnityのOnValidate : SprBehaviourのものをオーバーライド
    public override void OnValidate() {
        Initialize();
        if (sprObject != null) {
            CsObject d = CreateDesc();
            if (d != null) {
                sprObject.GetDesc(d);
                ApplyDesc(descStruct, d);
                sprObject.SetDesc(d);

                // <!!> うまく動いてない　あれー？
                (sprObject as CDShapeIf).SetMaterial((descStruct as CDShapeDescStruct).material);
            }
        }
    }
}
