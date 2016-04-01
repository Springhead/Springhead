using UnityEngine;
using System.Collections;
using System.Linq;
using SprCs;
using System;

public class CDRoundConeBehavior : CDShapeBehaviour {
    public CDRoundConeDescStruct roundConeDescripter = null;

    public override void InitDesc() {
        roundConeDescripter = new CDRoundConeDesc();
    }

    public override CDShapeIf CreateShape(GameObject shapeObject) {
        MeshRoundCone mrc = shapeObject.GetComponent<MeshRoundCone>();
        if (mrc == null) { throw new ObjectNotFoundException("CDRoundConeBehaviour requires MeshRoundCone", shapeObject); }

        CDRoundConeDesc descRoundCone = new CDRoundConeDesc();
        descRoundCone.radius = new Vec2f(mrc.r1, mrc.r2);
        descRoundCone.length = mrc.length;

        return GetPHSdk().CreateShape(CDRoundConeIf.GetIfInfoStatic(), descRoundCone);
    }

    public override Posed ShapePose(GameObject shapeObject) {
        MeshRoundCone mrc = shapeObject.GetComponent<MeshRoundCone>();
        if (mrc == null) { throw new ObjectNotFoundException("CDRoundConeBehaviour requires MeshRoundCone", shapeObject); }

        // SpringheadとUnityでカプセルの向きが違うことに対する補正
        Vec3f p = new Vec3f();
        if (mrc.pivot == MeshRoundCone.Pivot.R1) {
            p = new Vec3f(+0.5f * mrc.length, 0, 0);
        } else if (mrc.pivot == MeshRoundCone.Pivot.R2) {
            p = new Vec3f(-0.5f * mrc.length, 0, 0);
        }
        return new Posed(p, Quaterniond.Rot(90.0f * Mathf.Deg2Rad, new Vec3d(0, 1, 0)));
    }

    public void OnValidate() {
    }
}
