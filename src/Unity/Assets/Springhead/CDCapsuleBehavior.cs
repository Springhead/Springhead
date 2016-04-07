using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class CDCapsuleBehavior : CDShapeBehaviour {
    public CDCapsuleDescStruct desc = null;

    public override CsObject descStruct {
        get { return desc; }
        set { desc = value as CDCapsuleDescStruct; }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
        (from as CDCapsuleDescStruct).ApplyTo(to as CDCapsuleDesc);
    }

    public override CsObject CreateDesc() {
        return new CDCapsuleDesc();
    }

    public override CDShapeIf CreateShape(GameObject shapeObject) {
        CapsuleCollider cc = gameObject.GetComponent<CapsuleCollider>();
        if (cc == null) { throw new ObjectNotFoundException("CDCapsuleBehaviour requires CapsuleCollider", shapeObject); }

        Vector3 scale = shapeObject.transform.lossyScale;
        Vector3 position = shapeObject.GetComponent<Transform>().position;
        desc.radius = cc.radius * (Mathf.Max(scale.x, scale.z));
        desc.length = cc.height * scale.y - desc.radius * 2;

        return phSdk.CreateShape(CDCapsuleIf.GetIfInfoStatic(), (CDCapsuleDesc)desc);
    }

    public override Posed ShapePose(GameObject shapeObject) {
        // SpringheadとUnityでカプセルの向きが違うことに対する補正
        return new Posed(new Vec3d(), Quaterniond.Rot(90 * Mathf.Deg2Rad, new Vec3d(1,0,0)));
    }
}
