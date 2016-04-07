using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class CDSphereBehaviour : CDShapeBehaviour {
    public CDSphereDescStruct desc = null;

    public override CsObject descStruct {
        get { return desc; }
        set { desc = value as CDSphereDescStruct; }
    }

    public override void ApplyDesc(CsObject from, CsObject to) {
        (from as CDSphereDescStruct).ApplyTo(to as CDSphereDesc);
    }

    public override CsObject CreateDesc() {
        return new CDSphereDesc();
    }

    public override CDShapeIf CreateShape(GameObject shapeObject) {
        SphereCollider sc = shapeObject.GetComponent<SphereCollider>();
        if (sc == null) { throw new ObjectNotFoundException("CDSphereBehaviour requires SphereCollider", shapeObject); }

        Vector3 scale = shapeObject.transform.lossyScale;
        desc.radius = sc.radius * (Mathf.Max(Mathf.Max(scale.x, scale.y), scale.z));
        
        return phSdk.CreateShape(CDSphereIf.GetIfInfoStatic(), (CDSphereDesc)desc);
    }
}
