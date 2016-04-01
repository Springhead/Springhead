using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class CDCapsuleBehavior : CDShapeBehaviour {
    public CDCapsuleDescStruct capsuleDescripter = null;

    public override void InitDesc() {
        capsuleDescripter = new CDCapsuleDesc();
    }

    public override CDShapeIf CreateShape(GameObject shapeObject) {
        CapsuleCollider cc = gameObject.GetComponent<CapsuleCollider>();
        if (cc == null) { throw new ObjectNotFoundException("CDCapsuleBehaviour requires CapsuleCollider", shapeObject); }

        CDCapsuleDesc descCapsule = new CDCapsuleDesc();
        Vector3 scale = shapeObject.transform.lossyScale;
        Vector3 position = shapeObject.GetComponent<Transform>().position;
        descCapsule.radius = cc.radius * (Mathf.Max(scale.x, scale.z));
        descCapsule.length = cc.height * scale.y - descCapsule.radius * 2;

        return GetPHSdk().CreateShape(CDCapsuleIf.GetIfInfoStatic(), descCapsule);
    }

    public override Posed ShapePose(GameObject shapeObject) {
        // SpringheadとUnityでカプセルの向きが違うことに対する補正
        return new Posed(new Vec3d(), Quaterniond.Rot(90 * Mathf.Deg2Rad, new Vec3d(1,0,0)));
    }

    public void OnValidate() {

    }
}
