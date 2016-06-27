using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class CDBoxBehaviour : CDShapeBehaviour {
    public CDBoxDescStruct desc = null;

    public override CsObject descStruct {
        get { return desc; }
        set { desc = value as CDBoxDescStruct; }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
        (from as CDBoxDescStruct).ApplyTo(to as CDBoxDesc);
    }

    public override CsObject CreateDesc() {
        return new CDBoxDesc();
    }

    public override CDShapeIf CreateShape(GameObject shapeObject) {
        BoxCollider bc = shapeObject.GetComponent<BoxCollider>();
        if (bc == null) { throw new ObjectNotFoundException("CDBoxBehaviour requires BoxCollider", shapeObject); }

        Vector3 size = bc.size;
        Vector3 scale = shapeObject.transform.lossyScale;
        desc.boxsize = new Vec3f((float)(size.x * scale.x), (float)(size.y * scale.y), (float)(size.z * scale.z));

        return phSdk.CreateShape(CDBoxIf.GetIfInfoStatic(), (CDBoxDesc)desc);
    }
}
