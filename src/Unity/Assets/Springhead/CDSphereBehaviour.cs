using UnityEngine;
using System.Collections;
using SprCs;
using System;

public class CDSphereBehaviour : CDShapeBehaviour {
    public CDSphereDescStruct sphereDescripter = null;

    public override void InitDesc() {
        sphereDescripter = new CDSphereDesc();
    }

    public override CDShapeIf CreateShape(GameObject shapeObject) {
        SphereCollider sc = shapeObject.GetComponent<SphereCollider>();
        if (sc == null) { throw new ObjectNotFoundException("CDSphereBehaviour requires SphereCollider", shapeObject); }

        CDSphereDesc descSphere = new CDSphereDesc();
        Vector3 scale = shapeObject.transform.lossyScale;
        descSphere.radius = sc.radius * (Mathf.Max(Mathf.Max(scale.x, scale.y), scale.z));
        
        return GetPHSdk().CreateShape(CDSphereIf.GetIfInfoStatic(), descSphere);
    }

    public void OnValidate() {

    }
}
